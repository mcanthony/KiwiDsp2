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
        }
        
        void Context::add(sProcess process) throw(Error<Process>&)
        {
            if(process)
            {
                lock_guard<mutex> guard(m_mutex);
                sNode node = Node::create(shared_from_this(), process);
                if(node)
                {
                    auto it = m_nodes.begin();
                    while(it != m_nodes.end())
                    {
                        if((*it)->getProcess() == process)
                        {
                             throw Error<Process>(process, Error<Process>::Duplicate);
                        }
                        ++it;
                    }
                    m_nodes.push_back(node);
                }
                else
                {
                    throw Error<Process>(process, Error<Process>::Node);
                }
            }
            else
            {
                throw Error<Process>(process, Error<Process>::Valid);
            }
        }
        
        void Context::add(sConnection connection) throw(Error<Connection>&)
        {
            if(connection)
            {
                sProcess from   = connection->getProcessFrom();
                sProcess to     = connection->getProcessTo();
                if(from && to && from != to)
                {
                    lock_guard<mutex> guard(m_mutex);
                    
                    auto nodeFrom = m_nodes.begin();
                    while(nodeFrom != m_nodes.end() && (*nodeFrom)->getProcess() != from)
                    {
                        ++nodeFrom;
                    }
                    
                    auto nodeTo  = m_nodes.begin();
                    while(nodeTo != m_nodes.end() && (*nodeTo)->getProcess() != to)
                    {
                        ++nodeTo;
                    }
                    
                    if(nodeFrom == m_nodes.end())
                    {
                        throw Error<Connection>(connection, Error<Connection>::From);
                    }
                    else if(nodeTo == m_nodes.end())
                    {
                        throw Error<Connection>(connection, Error<Connection>::To);
                    }
                    
                    try
                    {
                        (*nodeFrom)->addOutputNode((*nodeTo), connection->getOutputIndex());
                    }
                    catch(bool e)
                    {
                        if(e)
                        {
                            throw Error<Connection>(connection, Error<Connection>::Input);
                        }
                        else
                        {
                            throw Error<Connection>(connection, Error<Connection>::Duplicate);
                        }
                    }
                    
                    try
                    {
                        (*nodeTo)->addInputNode((*nodeFrom), connection->getInputIndex());
                    }
                    catch(bool e)
                    {
                        if(e)
                        {
                            throw Error<Connection>(connection, Error<Connection>::Output);
                        }
                        else
                        {
                            throw Error<Connection>(connection, Error<Connection>::Duplicate);
                        }
                    }
                }
                else
                {
                    throw Error<Connection>(connection, Error<Connection>::Valid);
                }
            }
            else
            {
                throw Error<Connection>(connection, Error<Connection>::Valid);
            }
        }
        
        void Context::sortNodes(set<sNode>& nodes, ulong& index, sNode node) throw(Error<Node>&)
        {
            if(!node->getIndex())
            {
                nodes.insert(node);
                for(vector<sNode>::size_type i = 0; i < node->getNumberOfInputs(); i++)
                {
                    for(auto it = node->m_inputs[i]->m_connections.begin(); it != node->m_inputs[i]->m_connections.end(); ++it)
                    {
                        sNode pnode = (*it).lock();
                        if(pnode && !pnode->m_index)
                        {
                            if(nodes.find(pnode) != nodes.end())
                            {
                                throw Error<Node>(pnode, node, Error<Node>::Loop);
                            }
                            else
                            {
                                sortNodes(nodes, index, pnode);
                            }
                        }
                    }
                }
                node->setIndex(++index);
                nodes.erase(node);
            }
        }
        
        void Context::compile() throw(Error<Node>&)
        {
            lock_guard<mutex> guard(m_mutex);
            set<sNode> nodes;
            ulong index = 0;
            
            for(auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
            {
                try
                {
                    sortNodes(nodes, index, (*it));
                }
                catch(Error<Node>& e)
                {
                    throw e;
                }
            }
            sort(m_nodes.begin(), m_nodes.end(), Node::compare);
        
            for(auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
            {
                try
                {
                    (*it)->prepare();
                }
                catch(Error<Node>& e)
                {
                    throw e;
                }
                
                if(!(*it)->shouldPerform())
                {
                    it = m_nodes.erase(it);
                }
            }
            
            nodes.clear();
        }
        
        void Context::stop()
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<sNode>::size_type i = 0; i < m_nodes.size(); i++)
            {
                m_nodes[i]->stop();
            }
        }
    }
}
















