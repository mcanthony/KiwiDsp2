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

#include "DspDevice.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP DEVICE                                  //
    // ================================================================================ //
    
    DspDeviceManager::DspDeviceManager() noexcept
    {
        ;
    }
    
    DspDeviceManager::~DspDeviceManager() noexcept
    {
        lock_guard<mutex> guard(m_mutex);
        m_contexts.clear();
    }
    
    void DspDeviceManager::add(sDspContext context)
    {
        if(context)
        {
            lock_guard<mutex> guard(m_mutex);
            if(find(m_contexts.begin(), m_contexts.end(), context) == m_contexts.end())
            {
                m_contexts.push_back(context);
            }
        }
    }
    
    void DspDeviceManager::remove(sDspContext context)
    {
        if(context)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = find(m_contexts.begin(), m_contexts.end(), context);
            if(it != m_contexts.end())
            {
                m_contexts.erase(it);
            }
        }
    }
    
    bool DspDeviceManager::isDriverAvailable(string const& driver) const
    {
        vector<string> drivers;
        getAvailableDrivers(drivers);
        return find(drivers.begin(), drivers.end(), driver) != drivers.end();
    }
    
    bool DspDeviceManager::isInputDeviceAvailable(string const& device) const
    {
        vector<string> devices;
        getAvailableInputDevices(devices);
        return find(devices.begin(), devices.end(), device) != devices.end();
    }
    
    bool DspDeviceManager::isOutputDeviceAvailable(string const& device) const
    {
        vector<string> devices;
        getAvailableOutputDevices(devices);
        return find(devices.begin(), devices.end(), device) != devices.end();
    }
    
    bool DspDeviceManager::isSampleRateAvailable(const ulong samplerate) const
    {
        vector<ulong> samplerates;
        getAvailableSampleRates(samplerates);
        return find(samplerates.begin(), samplerates.end(), samplerate) != samplerates.end();
    }
    
    bool DspDeviceManager::isVectorSizeAvailable(const ulong vectorsize) const
    {
        vector<ulong> vectorsizes;
        getAvailableVectorSizes(vectorsizes);
        return find(vectorsizes.begin(), vectorsizes.end(), vectorsize) != vectorsizes.end();
    }
}
















