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

#include "DspContext.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP CONTEXT                                 //
    // ================================================================================ //
    
    DspContext::DspContext() noexcept :
    m_samplerate(0),
    m_vectorsize(0),
    m_compiled(false)
    {
        
    }
    
    DspContext::~DspContext()
    {
        m_nodes.clear();
    }
    
    void DspContext::add(sDspNode node) throw(DspError<DspNode>&)
    {
        if(node)
        {
            lock_guard<mutex> guard(m_mutex);
            if(find(m_nodes.begin(), m_nodes.end(), node) != m_nodes.end())
            {
                throw DspError<DspNode>(node, node, DspError<DspNode>::Duplicate);
            }
            m_nodes.push_back(node);
        }
        else
        {
            throw DspError<DspNode>(node, node, DspError<DspNode>::Valid);
        }
    }
    
    void DspContext::add(sDspLink link) throw(DspError<DspLink>&)
    {
        if(link)
        {
            sDspNode from   = link->getDspNodeFrom();
            sDspNode to     = link->getDspNodeTo();
            if(from && to && from != to)
            {
                lock_guard<mutex> guard(m_mutex);
                if(find(m_nodes.begin(), m_nodes.end(), from) == m_nodes.end())
                {
                    throw DspError<DspLink>(link, DspError<DspLink>::From);
                }
                if(find(m_nodes.begin(), m_nodes.end(), to) == m_nodes.end())
                {
                    throw DspError<DspLink>(link, DspError<DspLink>::To);
                }
                
                try
                {
                    from->addOutputDspNode(to, link->getOutputIndex());
                }
                catch(bool e)
                {
                    if(e)
                    {
                        throw DspError<DspLink>(link, DspError<DspLink>::Input);
                    }
                    else
                    {
                        throw DspError<DspLink>(link, DspError<DspLink>::Duplicate);
                    }
                }
                
                try
                {
                    to->addInputDspNode(from, link->getInputIndex());
                }
                catch(bool e)
                {
                    if(e)
                    {
                        throw DspError<DspLink>(link, DspError<DspLink>::Output);
                    }
                    else
                    {
                        throw DspError<DspLink>(link, DspError<DspLink>::Duplicate);
                    }
                }
                return;
            }
        }
        throw DspError<DspLink>(link, DspError<DspLink>::Valid);
    }
    
    void DspContext::sortDspNodes(set<sDspNode>& nodes, sDspNode node) throw(DspError<DspNode>&)
    {
        if(!node->isCompiled())
        {
            nodes.insert(node);
            for(vector<sDspNode>::size_type i = 0; i < node->getNumberOfInputs(); i++)
            {
                DspNode::DspNodeSet& link = node->m_inputs[i]->m_links;
                for(auto it = link.begin(); it != link.end(); ++it)
                {
                    sDspNode input = (*it).lock();
                    if(input && !input->isCompiled())
                    {
                        if(nodes.find(input) != nodes.end())
                        {
                            throw DspError<DspNode>(input, node, DspError<DspNode>::Loop);
                        }
                        else
                        {
                            sortDspNodes(nodes, input);
                        }
                    }
                }
            }
            
            try
            {
                node->compile(shared_from_this());
            }
            catch(DspError<DspNode>& e)
            {
                throw e;
            }
            if(node->shouldPerform())
            {
                m_nodes.push_back(node);
            }
            nodes.erase(node);
        }
    }
    
    void DspContext::compile(const ulong samplerate, const ulong vectorsize) throw(DspError<DspNode>&)
    {
        if(m_compiled)
        {
            stop();
        }
        m_samplerate = samplerate;
        m_vectorsize = vectorsize;
        
        lock_guard<mutex> guard(m_mutex);
        set<sDspNode> nodes;
        vector<sDspNode> processes;
        processes.swap(m_nodes);
        
        for(auto it = processes.begin(); it != processes.end(); ++it)
        {
            try
            {
                sortDspNodes(nodes, (*it));
            }
            catch(DspError<DspNode>& e)
            {
                throw e;
            }
        }
        nodes.clear();
        
        m_compiled = true;
    }
    
    void DspContext::stop()
    {
        lock_guard<mutex> guard(m_mutex);
        for(vector<sDspNode>::size_type i = 0; i < m_nodes.size(); i++)
        {
            m_nodes[i]->stop();
        }
        m_compiled = false;
    }
}
















