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

#include "DspChain.h"
#include "DspContext.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP CONTEXT                                 //
    // ================================================================================ //
    
    DspChain::DspChain(sDspContext context) noexcept :
    m_context(context),
    m_running(false)
    {
        
    }
    
    DspChain::~DspChain()
    {
        if(m_running)
        {
            stop();
        }
        lock_guard<mutex> guard(m_mutex);
        m_nodes.clear();
        m_links.clear();
    }
    
    sDspDeviceManager DspChain::getDeviceManager() const noexcept
    {
        sDspContext context = getContext();
        if(context)
        {
            return context->getDeviceManager();
        }
        else
        {
            return nullptr;
        }
    }
    
    ulong DspChain::getSampleRate() const noexcept
    {
        sDspContext context = getContext();
        if(context)
        {
            return context->getSampleRate();
        }
        else
        {
            return 0;
        }
    }

    ulong DspChain::getVectorSize() const noexcept
    {
        sDspContext context = getContext();
        if(context)
        {
            return context->getVectorSize();
        }
        else
        {
            return 0;
        }
    }
    
    void DspChain::add(sDspNode node) throw(DspError&)
    {
        if(node)
        {
            const bool state = suspend();
            {
                lock_guard<mutex> guard(m_mutex);
                if(find(m_nodes.begin(), m_nodes.end(), node) == m_nodes.end())
                {
                    m_nodes.push_back(node);
                }
            }
            try
            {
                resume(state);
            }
            catch(DspError& e)
            {
                throw e;
            }
        }
    }
    
    void DspChain::add(sDspLink link)  throw(DspError&)
    {
        if(link && link->isValid())
        {
            const bool state = suspend();
            {
                lock_guard<mutex> guard(m_mutex);
                if(find(m_links.begin(), m_links.end(), link) == m_links.end())
                {
                    m_links.push_back(link);
                }
            }
            try
            {
                resume(state);
            }
            catch(DspError& e)
            {
                throw e;
            }
        }
    }
    
    void DspChain::remove(sDspNode node) throw(DspError&)
    {
        if(node)
        {
            const bool state = suspend();
            {
                lock_guard<mutex> guard(m_mutex);
                auto it = find(m_nodes.begin(), m_nodes.end(), node);
                if(it != m_nodes.end())
                {
                    m_nodes.erase(it);
                }
            }
            try
            {
                resume(state);
            }
            catch(DspError& e)
            {
                throw e;
            }
        }
    }
    
    void DspChain::remove(sDspLink link)  throw(DspError&)
    {
        if(link)
        {
            const bool state = suspend();
            {
                lock_guard<mutex> guard(m_mutex);
                auto it = find(m_links.begin(), m_links.end(), link);
                if(it != m_links.end())
                {
                    m_links.erase(it);
                }
            }
            try
            {
                resume(state);
            }
            catch(DspError& e)
            {
                throw e;
            }
        }
    }
    
    void DspChain::sortNodes(set<sDspNode>& nodes, ulong& index, sDspNode node) throw(DspError&)
    {
        if(!node->index)
        {
            nodes.insert(node);
            for(vector<sDspNode>::size_type i = 0; i < node->getNumberOfInputs(); i++)
            {
                DspNodeSet& link = node->m_inputs[i]->m_links;
                for(auto it = link.begin(); it != link.end(); ++it)
                {
                    sDspNode input = (*it).lock();
                    if(input && !input->index)
                    {
                        if(nodes.find(input) != nodes.end())
                        {
                            throw DspError(input, DspError::Loop);
                        }
                        else
                        {
                            sortNodes(nodes, index, input);
                        }
                    }
                }
            }
            
            nodes.erase(node);
            node->index = index++;
        }
    }
    
    void DspChain::start() throw(DspError&)
    {
        if(m_running)
        {
            stop();
        }
        
        lock_guard<mutex> guard(m_mutex);
        
        for(vector<sDspLink>::size_type i = 0; i < m_links.size(); i++)
        {
            m_links[i]->start();
        }
        for(vector<sDspNode>::size_type i = 0; i < m_nodes.size(); i++)
        {
            m_nodes[i]->index = 0;
        }
        
        ulong index = 1;
        set<sDspNode> temp;
        for(vector<sDspNode>::size_type i = 0; i < m_nodes.size(); i++)
        {
            try
            {
                sortNodes(temp, index, m_nodes[i]);
            }
            catch(DspError& e)
            {
                throw e;
            }
        }
        temp.clear();
        
        for(vector<sDspNode>::size_type i = 0; i < m_nodes.size(); i++)
        {
            try
            {
                m_nodes[i]->start();
            }
            catch(DspError& e)
            {
                throw e;
            }
        }
        
        m_running = true;
    }
    
    void DspChain::stop()
    {
        if(m_running)
        {
            m_running = false;
            lock_guard<mutex> guard(m_mutex);
            for(vector<sDspNode>::size_type i = 0; i < m_nodes.size(); i++)
            {
                m_nodes[i]->stop();
            }
        }
    }
    
    void DspChain::resume(const bool state) throw(DspError&)
    {
        if(state && !m_running)
        {
            try
            {
                start();
            }
            catch(DspError& e)
            {
                throw e;
            }
            
        }
        else if(!state)
        {
            stop();
        }
    }
    
    bool DspChain::suspend()
    {
        if(m_running)
        {
            stop();
            return true;
        }
        else
        {
            return false;
        }
    }
}
















