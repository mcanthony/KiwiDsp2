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
        m_sample_outs(new sample*[m_nouts])
        {
            m_node_ins.resize(m_nins);
            m_node_outs.resize(m_nouts);
            m_signal_ins.resize(m_nins);
            m_signal_outs.resize(m_nins);
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
            m_signal_ins.clear();
            m_signal_outs.clear();
        }
        
        void Node::addInput(sNode node, const ulong index)
        {
            if(index < (ulong)m_node_ins.size())
            {
                m_node_ins[index].insert(node);
            }
        }
        
        void Node::addOutput(sNode node, const ulong index)
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
            if(!status)
            {
                m_valid = status;
            }
        }
        
        sSignal Node::getOutputSignal(sNode node)
        {
            for(vector<NodeSet>::size_type i = 0; i < m_node_outs.size(); i++)
            {
                if(m_node_outs[i].find(node) != m_node_outs[i].end())
                {
                    return m_signal_outs[i];
                }
            }
            return nullptr;
        }
        
        void Node::clean()
        {
            for(vector<NodeSet>::size_type i = 0; i < m_node_ins.size(); i++)
            {
                for(auto it = m_node_ins[i].begin(); it != m_node_ins[i].end();)
                {
                    sNode node = (*it).lock();
                    if(!node || !node->isValid())
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
        }
        
        void Node::allocSignals()
        {
            set<weak_ptr<Node>>::iterator it;
            
            // Inputs signals
            for(int i = 0; i < m_nins; i++)
            {
                // We try to find a signal not borrowed
                for(it = m_inputs_nodes[i].begin(); it != m_inputs_nodes[i].end() && !m_sig_ins[i]; ++it)
                {
                    sNode node = (*it).lock();
                    if(node)
                    {
                        sSignal sig = node->getOutputSignal(shared_from_this());
                        if(sig && !sig->isBorrowed()) // Can do this more if no inplace
                        {
                            m_sig_ins[i] = make_shared<Signal>(sig, m_inplace);
                            break;
                        }
                    }
                }
                
                // If we didn't find one
                if(!m_sig_ins[i])
                {
                    sSignal sig = make_shared<Signal>(m_vectorsize);
                    m_sig_ins[i] = sig;
                    context->addSignal(sig);
                }
                
                // We add copy
                for(it = m_inputs_nodes[i].begin(); it != m_inputs_nodes[i].end(); ++it)
                {
                    sNode node = (*it).lock();
                    if(node)
                    {
                        sSignal sig = node->getOutputSignal(shared_from_this());
                        if(sig && (*(sig.get()) != *(m_sig_ins[i].get())))
                        {
                            shared_ptr<DspProcess> process = make_shared<DspProcess>(DspProcess::copy, nullptr, 1, 1, m_vectorsize);
                            process->setInput(0, sig);
                            process->setOutput(0, m_sig_ins[i]);
                            context->addProcess(process);
                        }
                    }
                }
                
            }
            
            // Outputs signals
            if(m_inplace)
            {
                for(int i = 0; i < m_nouts; i++)
                {
                    if(i < m_nins)
                    {
                        m_sig_outs[i] = make_shared<Signal>(m_sig_ins[i], false);
                    }
                    else
                    {
                        sSignal sig = make_shared<Signal>(m_vectorsize);
                        m_sig_outs[i] = sig;
                        context->addSignal(sig);
                    }
                }
            }
            else
            {
                for(int i = 0; i < m_nouts; i++)
                {
                    sSignal sig = make_shared<Signal>(m_vectorsize);
                    m_sig_outs[i] = sig;
                    context->addSignal(sig);
                }
            }
        }
        
        void Node::prepare()
        {
           
        }        
    }
}
















