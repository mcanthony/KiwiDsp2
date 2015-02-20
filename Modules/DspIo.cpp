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

#include "DspIo.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      PLUS                                        //
    // ================================================================================ //
    
    DspDac::DspDac(sDspChain chain, vector<ulong> const& channels) noexcept :
    DspNode(chain, channels.size(), 0)
    {
        m_channels = channels;
    }
    
    DspDac::~DspDac()
    {
        m_outputs.clear();
    }
    
    string DspDac::getName() const noexcept
    {
        return "Dac";
    }
    
    void DspDac::prepare() noexcept
    {
        shouldPerform(false);
        m_outputs.clear();
        scDspDeviceManager device = getDeviceManager();
        sDspContext contxt = getContext();

        if(device && !m_channels.empty())
        {
            for(vector<ulong>::size_type i = 0; i < m_channels.size(); i++)
            {
                if(m_channels[i] <= device->getNumberOfOutputs())
                {
                    sample* out = device->getOutputsSamples(m_channels[i] - 1);
                    if(out)
                    {
                        m_outputs.push_back(device->getOutputsSamples(m_channels[i] - 1));
                        shouldPerform(true);
                    }
                }
            }
        }
    }
    
    void DspDac::perform() noexcept
    {
        for(vector<float*>::size_type i = 0; i < m_outputs.size(); i++)
        {
            Signal::vadd(getVectorSize(), getInputsSamples()[i], m_outputs[i]);
        }
    }
    
    void DspDac::release() noexcept
    {
        m_outputs.clear();
    }
    
    void DspDac::setChannels(vector<ulong> const& channels) noexcept
    {
        for(vector<ulong>::size_type i = 0; i < m_channels.size() && i < channels.size(); i++)
        {
            m_channels[i] = channels[i];
        }
    }
    
    void DspDac::getChannels(vector<ulong>& channels) const noexcept
    {
        channels = m_channels;
    }
}

