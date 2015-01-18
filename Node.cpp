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
        
        Node::Output::Output(sample* const vector, const bool owner) noexcept :
        m_vector(vector),
        m_owner(owner),
        m_borrowed(false)
        {
            ;
        }
        
        Node::Output::~Output()
        {
            if(m_owner && m_vector)
            {
                delete [] m_vector;
            }
        }
        
        // ================================================================================ //
        //                                      DSP INPUT                                   //
        // ================================================================================ //
        
        Node::Input::Input(const ulong size, sample* const vector, const ulong nothers, sample *const *const others, const bool owner) noexcept :
        m_vector(vector),
        m_nothers(nothers),
        m_others(others),
        m_size(size),
        m_owner(owner)
        {
            ;
        }
        
        Node::Input::~Input()
        {
            if(m_nothers && m_others)
            {
                delete [] m_others;
            }
            if(m_owner && m_vector)
            {
                delete [] m_vector;
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
            m_node_ins.resize(m_nins);
            m_node_outs.resize(m_nouts);
            m_inputs.resize(m_nins);
            m_outputs.resize(m_nins);
        }
        
        Node::~Node()
        {
            delete [] m_sample_ins;
            delete [] m_sample_outs;
            for(vector<NodeSet>::size_type i = 0; i < m_node_ins.size(); i++)
            {
                m_node_ins[i].clear();
            }
            for(vector<NodeSet>::size_type i = 0; i < m_node_outs.size(); i++)
            {
                m_node_outs[i].clear();
            }
            m_node_ins.clear();
            m_node_outs.clear();
            m_inputs.clear();
            m_outputs.clear();
        }
        
        void Node::setIndex(const ulong index)
        {
            m_index = index;
        }
        
        void Node::addInputNode(sNode node, const ulong index)
        {
            if(index < (ulong)m_node_ins.size())
            {
                m_node_ins[index].insert(node);
            }
        }
        
        void Node::addOutputNode(sNode node, const ulong index)
        {
            if(index < (ulong)m_node_outs.size())
            {
                m_node_outs[index].insert(node);
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
        
        Node::sOutput Node::getOutput(sNode node)
        {
            for(vector<NodeSet>::size_type i = 0; i < m_node_outs.size(); i++)
            {
                if(m_node_outs[i].find(node) != m_node_outs[i].end())
                {
                    return m_outputs[i];
                }
            }
            return nullptr;
        }
        
        Node::sInput Node::createInput(const ulong index)
        {
            bool owner      = false;
            NodeSet& set    = m_node_ins[index];
            
            sample*     vec     = nullptr;
            sample**    others  = nullptr;
            ulong       nothers = 0;
            
            for(auto it = set.begin(); it != set.end(); ++it)
            {
                sNode node = (*it).lock();
                if(node)
                {
                    sOutput output = node->getOutput(node);
                    if(output && !output->isBorrowed() && !vec)
                    {
                        output->m_borrowed = node->isInplace();
                        vec = output->getVector();
                    }
                    else if(output)
                    {
                        nothers++;
                    }
                }
            }
            
            others = new sample*[nothers];
            nothers = 0;
            for(auto it = set.begin(); it != set.end(); ++it)
            {
                sNode node = (*it).lock();
                if(node)
                {
                    sOutput output = node->getOutput(node);
                    if(output && output->getVector() != vec)
                    {
                        others[nothers++] = output->getVector();
                    }
                }
            }
            
            if(!vec)
            {
                owner   = true;
                vec     = new sample[getVectorSize()];
            }
            if(vec)
            {
                return make_shared<Input>(getVectorSize(), vec, nothers, others, true);
            }
            return nullptr;
        }
        
        Node::sOutput Node::createOutput(const ulong index)
        {
            if(index < getNumberOfInputs() && isInplace())
            {
                return make_shared<Output>(m_inputs[index]->getVector(), false);
            }
            else
            {
                sample* vec = new sample[getVectorSize()];
                if(vec)
                {
                    return make_shared<Output>(vec, true);
                }
                else
                {
                    return nullptr;
                }
            }
        }
        
        void Node::prepare()
        {
            for(vector<NodeSet>::size_type i = 0; i < m_node_ins.size(); i++)
            {
                for(auto it = m_node_ins[i].begin(); it != m_node_ins[i].end();)
                {
                    sNode node = (*it).lock();
                    if(!node || !node->shouldPerform())
                    {
                        it = m_node_ins[i].erase(it);
                    }
                    else if((node->m_samplerate != m_samplerate) || (node->m_vectorsize != m_vectorsize))
                    {
                        throw node;
                        return;
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            
            m_shouldperform = true;
            m_process->prepare(shared_from_this());
            
            if(shouldPerform())
            {
                for(ulong i = 0; i < m_nins; i++)
                {
                    m_inputs[i] = createInput(i);
                    if(m_outputs[i])
                    {
                        m_sample_outs[i] = m_outputs[i]->getVector();
                    }
                    else
                    {
                        m_shouldperform = false;
                        throw shared_from_this();
                        return;
                    }
                }
                for(ulong i = 0; i < m_nouts; i++)
                {
                    m_outputs[i] = createOutput(i);
                    if(m_outputs[i])
                    {
                        m_sample_outs[i] = m_outputs[i]->getVector();
                    }
                    else
                    {
                        m_shouldperform = false;
                        throw shared_from_this();
                        return;
                    }
                }
            }
        }
    
        void Node::stop() const
        {
            m_process->release(shared_from_this());
        }
    }
}
















