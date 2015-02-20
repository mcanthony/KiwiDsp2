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

#ifndef __DEF_KIWI_DSP_IO__
#define __DEF_KIWI_DSP_IO__

#include "../Context/DspDevice.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DAC                                         //
    // ================================================================================ //
    
    class DspDac : public DspNode
    {
    private:
        vector<ulong>    m_channels;
        vector<sample*>  m_outputs;
    public:
        DspDac(sDspChain chain, vector<ulong> const& channels = {}) noexcept;
        ~DspDac();
        string getName() const noexcept override;
        void prepare() noexcept override;
        void perform() noexcept override;
        void release() noexcept override;
        void setChannels(vector<ulong> const& channels) noexcept;
        void getChannels(vector<ulong>& channels) const noexcept;
    };
    
}

#endif


