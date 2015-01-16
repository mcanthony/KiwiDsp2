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

#include "Context.h"

namespace Kiwi
{
    namespace Dsp
    {
        // ================================================================================ //
        //                                      DSP CONTEXT                                 //
        // ================================================================================ //
        
        Context::Context(const ulong samplerate, const ulong vectorsize) noexcept :
        m_samplerate(samplerate),
        m_vectorsize(vectorsize)
        {
            
        }
        
        Context::~Context()
        {
            m_nodes.clear();
            m_nodes_temp.clear();
        }
        
        void Context::add(sProcess process)
        {
            if(process)
            {
                lock_guard<mutex> guard(m_mutex);
                auto it = find(m_nodes.begin(), m_nodes.end(), process);
                if(it != m_nodes.end())
                {
                    m_nodes.push_back(Node::create(shared_from_this(), process));
                }
            }
        }
        
        void Context::add(sConnection connection)
        {
            if(connection)
            {
                sProcess from   = connection->getProcessFrom();
                sProcess to     = connection->getProcessTo();
                if(from && to)
                {
                    lock_guard<mutex> guard(m_mutex);
                    auto nodeFrom   = find(m_nodes.begin(), m_nodes.end(), from);
                    auto nodeTo     = find(m_nodes.begin(), m_nodes.end(), to);
                    if(nodeFrom != m_nodes.end() && nodeTo != m_nodes.end() && nodeFrom != nodeTo)
                    {
                        (*nodeFrom)->addOutput((*nodeTo), connection->getOutletIndex());
                        (*nodeTo)->addOutput((*nodeFrom), connection->getInletIndex());
                    }
                }
            }
        }
        
        void Context::indexNode(sNode node)
        {
            if(!node->getIndex())
            {
                m_nodes_temp.insert(node);
                for(vector<sNode>::size_type i = 0; i < m_nodes.size(); i++)
                {
                    for(auto it = node->m_node_ins[i].begin(); it != node->m_node_ins[i].end(); ++it)
                    {
                        sNode pnode = (*it).lock();
                        if(pnode && !pnode->m_index)
                        {
                            if(m_nodes_temp.find(pnode) != m_nodes_temp.end())
                            {
                                throw pnode;
                            }
                            else
                            {
                                indexNode(pnode);
                            }
                        }
                    }
                }
                node->setIndex(++m_index);
                m_nodes_temp.erase(node);
            }
        }
        
        void Context::compile()
        {
            lock_guard<mutex> guard(m_mutex);
            m_index = 0;
            m_nodes_temp.clear();
            
            try
            {
                for(auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
                {
                    indexNode((*it));
                }
                sort(m_nodes.begin(), m_nodes.end());
            }
            catch(sNode node)
            {
                throw node->getProcess();
            }
            
            try
            {
                for(auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
                {
                    (*it)->prepare();
                    if(!(*it)->isValid())
                    {
                        it = m_nodes.erase(it);
                    }
                }
            }
            catch(sNode node)
            {
                throw node->getProcess();
            }
            
            m_nodes_temp.clear();
        }
    }
}
















