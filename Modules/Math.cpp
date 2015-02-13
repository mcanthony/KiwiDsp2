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

#include "Math.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      PLUS                                        //
    // ================================================================================ //
    
    DspPlus<Scalar>::DspPlus(const sample value) noexcept : DspNode(1, 1),
    m_value(value)
    {
        ;
    }
    
    DspPlus<Scalar>::~DspPlus()
    {
        ;
    }
    
    string DspPlus<Scalar>::getName() const noexcept
    {
        return "Plus (scalar)";
    }
    
    void DspPlus<Scalar>::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspPlus<Scalar>::perform() const noexcept
    {
        Signal::vsadd(getVectorSize(), m_value, getOutputsSamples()[0]);
        Signal::vpost(5, getOutputsSamples()[0]);
    }
    
    void DspPlus<Scalar>::release() noexcept
    {
        ;
    }
    
    void DspPlus<Scalar>::setValue(const sample value) noexcept
    {
        m_value = value;
    }
    
    sample DspPlus<Scalar>::getValue() const noexcept
    {
        return m_value;
    }
    
    DspPlus<Vector>::DspPlus() noexcept : DspNode(2, 1)
    {
        ;
    }
    
    DspPlus<Vector>::~DspPlus()
    {
        ;
    }
    
    string DspPlus<Vector>::getName() const noexcept
    {
        return "Plus (vector)";
    }
    
    
    void DspPlus<Vector>::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspPlus<Vector>::perform() const noexcept
    {
        Signal::vadd(getVectorSize(), getInputsSamples()[1], getOutputsSamples()[0]);
        Signal::vpost(5, getOutputsSamples()[0]);
    }
    
    void DspPlus<Vector>::release() noexcept
    {
        ;
    }
}

