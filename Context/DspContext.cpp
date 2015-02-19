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
#include "DspDevice.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP CONTEXT                                 //
    // ================================================================================ //
    
    DspContext::DspContext(sDspDeviceManager device) noexcept :
    m_device(device),
    m_cpu_factor(0.),
    m_running(false)
    {
        
    }
    
    DspContext::~DspContext()
    {
        if(m_running)
        {
            stop();
        }
        m_chains.clear();
    }
    
    ulong DspContext::getSampleRate() const noexcept
    {
        sDspDeviceManager device = getDeviceManager();
        if(device)
        {
            return device->getSampleRate();
        }
        else
        {
            return 0;
        }
    }
    
    ulong DspContext::getVectorSize() const noexcept
    {
        sDspDeviceManager device = getDeviceManager();
        if(device)
        {
            return device->getVectorSize();
        }
        else
        {
            return 0;
        }
    }
    
    void DspContext::add(sDspChain chain)
    {
        if(chain)
        {
            lock_guard<mutex> guard(m_mutex);
            if(find(m_chains.begin(), m_chains.end(), chain) == m_chains.end())
            {
                m_chains.push_back(chain);
            }
        }
    }
    
    void DspContext::remove(sDspChain chain)
    {
        bool finded = false;
        if(chain)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_chains.begin(), m_chains.end(), chain);
            if(it != m_chains.end())
            {
                m_chains.erase(it);
                finded = true;
            }
        }
        if(finded && m_running)
        {
            chain->stop();
        }
    }
    
    void DspContext::start()
    {
        sDspDeviceManager device = m_device.lock();
        if(device)
        {
            if(m_running)
            {
                stop();
            }
            device->add(shared_from_this());
            m_running = true;
            m_cpu_factor = 10e-6 * (double)getSampleRate() / (double)getVectorSize();
        }
    }
    
    void DspContext::stop()
    {
        if(m_running)
        {
            m_running = false;
            lock_guard<mutex> guard(m_mutex);
            for(vector<sDspNode>::size_type i = 0; i < m_chains.size(); i++)
            {
                if(m_chains[i]->isRunning())
                {
                    m_chains[i]->stop();
                }
            }
            sDspDeviceManager device = m_device.lock();
            if(device)
            {
                device->remove(shared_from_this());
            }
        }
    }
    
    void DspContext::resume(const bool state)
    {
        if(state && !m_running)
        {
            start();
        }
        else if(!state)
        {
            stop();
        }
    }
    
    bool DspContext::suspend()
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
















