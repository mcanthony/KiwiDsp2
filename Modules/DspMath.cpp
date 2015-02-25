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

#include "DspMath.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      PLUS                                        //
    // ================================================================================ //
    
    DspPlus<DspScalar>::DspPlus(sDspChain chain, const sample value) noexcept : DspNode(chain, 1, 1),
    m_value(value)
    {
        ;
    }
    
    DspPlus<DspScalar>::~DspPlus()
    {
        ;
    }
    
    string DspPlus<DspScalar>::getName() const noexcept
    {
        return "Plus (scalar)";
    }
    
    void DspPlus<DspScalar>::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspPlus<DspScalar>::perform() noexcept
    {
        Signal::vsadd(getVectorSize(), m_value, getOutputsSamples()[0]);
    }
    
    void DspPlus<DspScalar>::release() noexcept
    {
        ;
    }
    
    void DspPlus<DspScalar>::setValue(const sample value) noexcept
    {
        m_value = value;
    }
    
    sample DspPlus<DspScalar>::getValue() const noexcept
    {
        return m_value;
    }
    
    DspPlus<DspVector>::DspPlus(sDspChain chain) noexcept : DspNode(chain, 2, 1)
    {
        ;
    }
    
    DspPlus<DspVector>::~DspPlus()
    {
        ;
    }
    
    string DspPlus<DspVector>::getName() const noexcept
    {
        return "Plus (vector)";
    }
    
    
    void DspPlus<DspVector>::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspPlus<DspVector>::perform() noexcept
    {
        Signal::vadd(getVectorSize(), getInputsSamples()[1], getOutputsSamples()[0]);
    }
    
    void DspPlus<DspVector>::release() noexcept
    {
        ;
    }
}

