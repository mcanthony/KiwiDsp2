/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#include "Node.h"
#include "Context.h"

namespace Kiwi
{
    namespace Dsp
    {
        
        // ================================================================================ //
        //                                      DSP OUTPUT                                  //
        // ================================================================================ //
        
        Node::Output::Output(sNode node, const ulong index) noexcept :
        m_node(node),
        m_index(index),
        m_vector(nullptr),
        m_owner(false)
        {
            
        }
        
        Node::Output::~Output()
        {
            if(m_owner && m_vector)
            {
                delete [] m_vector;
            }
            m_connections.clear();
        }
        
        void Node::Output::addNode(sNode node) throw(bool)
        {
            if(!m_connections.insert(node).second)
            {
                throw false;
            }
        }
        
        void Node::Output::prepare() throw(Error<Node>&)
        {
            if(m_owner && m_vector)
            {
                delete [] m_vector;
                m_vector = nullptr;
            }
            m_owner     = false;
            
            sNode node = m_node.lock();
            if(node)
            {
                if(node->isInplace() && node->getNumberOfInputs() > m_index)
                {
                    m_vector = node->m_inputs[m_index]->getVector();
                    if(!m_vector)
                    {
                        throw Error<Node>(node, node, Error<Node>::Inplace);
                    }
                }
                if(!m_vector)
                {
                    m_owner     = true;
                    try
                    {
                        m_vector    = new sample[node->getVectorSize()];
                    }
                    catch(bad_alloc& e)
                    {
                        throw Error<Node>(node, node, Error<Node>::Alloc);
                    }
                    
                }
            }
        }
        
        // ================================================================================ //
        //                                      DSP INPUT                                   //
        // ================================================================================ //
        
        Node::Input::Input(sNode node, const ulong index) noexcept :
        m_node(node),
        m_index(index),
        m_size(node->getVectorSize()),
        m_vector(nullptr),
        m_nothers(0),
        m_others(nullptr)
        {
            
        }
        
        Node::Input::~Input()
        {
            if(m_nothers && m_others)
            {
                delete [] m_others;
                m_others = nullptr;
            }
            if(m_vector)
            {
                delete [] m_vector;
                m_vector = nullptr;
            }
            m_connections.clear();
        }
        
        void Node::Input::addNode(sNode node) throw(bool)
        {
            if(!m_connections.insert(node).second)
            {
                throw false;
            }
        }
        
        void Node::Input::prepare() throw(Error<Node>&)
        {
            if(m_vector)
            {
                delete [] m_vector;
                m_vector = nullptr;
            }
            if(m_nothers && m_others)
            {
                delete [] m_others;
                m_others = nullptr;
            }
            m_nothers   = 0;
            
            sNode node = m_node.lock();
            if(node)
            {
                for(auto it = m_connections.begin(); it != m_connections.end(); )
                {
                    sNode in = (*it).lock();
                    if(in)
                    {
                        ++it;
                    }
                    else
                    {
                        it = m_connections.erase(it);
                    }
                }
                m_nothers = m_connections.size();
                m_others  = new sample*[m_nothers];
                ulong inc = 0;
                for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
                {
                    sNode in = (*it).lock();
                    if(in)
                    {
                        if(in->getSampleRate() != node->getSampleRate())
                        {
                            throw Error<Node>(node, in, Error<Node>::SampleRate);
                        }
                        else if(in->getVectorSize() != node->getVectorSize())
                        {
                            throw Error<Node>(node, in, Error<Node>::VectorSize);
                        }
                        else
                        {
                            sOutput output = nullptr;
                            for(vector<NodeSet>::size_type i = 0; i < in->m_outputs.size(); i++)
                            {
                                if(in->m_outputs[i]->hasNode(node))
                                {
                                    output = in->m_outputs[i];
                                    break;
                                }
                            }
                            if(output)
                            {
                                m_others[inc++] = output->getVector();
                            }
                            else
                            {
                                throw Error<Node>(node, in, Error<Node>::Recopy);
                            }
                        }
                    }
                }
                try
                {
                    m_vector    = new sample[node->getVectorSize()];
                }
                catch(bad_alloc& e)
                {
                    throw Error<Node>(node, node, Error<Node>::Alloc);
                }
                
            }
            
        }
        
        // ================================================================================ //
        //                                      DSP NODE                                    //
        // ================================================================================ //
        
        Node::Node(sContext context, sProcess process):
        m_context(context),
        m_process(process),
        m_samplerate(context->getSampleRate()),
        m_vectorsize(context->getVectorSize()),
        m_nins(process->getNumberOfInputs()),
        m_sample_ins(new sample*[m_nins]),
        m_nouts(process->getNumberOfOutputs()),
        m_sample_outs(new sample*[m_nouts]),
        m_inplace(true),
        m_shouldperform(false),
        m_index(0)
        {
            ;
        }
        
        Node::~Node()
        {
            delete [] m_sample_ins;
            delete [] m_sample_outs;
            m_inputs.clear();
            m_outputs.clear();
        }
        
        void Node::setIndex(const ulong index)
        {
            m_index = index;
        }
        
        void Node::addInputNode(sNode node, const ulong index) throw(bool)
        {
            if(index < (ulong)m_inputs.size())
            {
                try
                {
                    m_inputs[index]->addNode(node);
                }
                catch(bool)
                {
                    throw false;
                }
            }
            else
            {
                throw true;
            }
        }
        
        void Node::addOutputNode(sNode node, const ulong index) throw(bool)
        {
            if(index < (ulong)m_outputs.size())
            {
                try
                {
                    m_outputs[index]->addNode(node);
                }
                catch(bool)
                {
                    throw false;
                }
            }
            else
            {
                throw true;
            }
        }
        
        void Node::setInplace(const bool status) noexcept
        {
            m_inplace = status;
        }
        
        void Node::shouldPerform(const bool status) noexcept
        {
            m_shouldperform = status;
        }
        
        void Node::prepare() throw(Error<Node>&)
        {
            m_shouldperform = true;
            m_process->prepare(shared_from_this());
            
            if(shouldPerform())
            {
                for(ulong i = 0; i < m_nins; i++)
                {
                    try
                    {
                        m_inputs[i]->prepare();
                    }
                    catch(Error<Node>& e)
                    {
                        throw e;
                    }
                    m_sample_ins[i] = m_inputs[i]->getVector();
                }
                for(ulong i = 0; i < m_nouts; i++)
                {
                    try
                    {
                        m_outputs[i]->prepare();
                    }
                    catch(Error<Node>& e)
                    {
                        throw e;
                    }
                    
                    m_sample_outs[i] = m_outputs[i]->getVector();
                }
            }
        }
    
        void Node::stop() const
        {
            m_process->release(shared_from_this());
        }
    }
}
















