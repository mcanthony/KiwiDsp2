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

#include "DspGenerator.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      SIG                                         //
    // ================================================================================ //
    
    DspSig::DspSig(sDspChain chain, const sample value) noexcept : DspNode(chain, 0, 1), m_value(value)
    {
        ;
    }
    
    DspSig::~DspSig()
    {
        ;
    }
    
    string DspSig::getName() const noexcept
    {
        return "DspSig";
    }
    
    void DspSig::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspSig::perform() noexcept
    {
        Signal::vfill(getVectorSize(), m_value, getOutputsSamples()[0]);
    }
    
    void DspSig::release() noexcept
    {
        ;
    }
    
    void DspSig::setValue(const sample value) noexcept
    {
        m_value = value;
    }
    
    sample DspSig::getValue() const noexcept
    {
        return m_value;
    }
    
    // ================================================================================ //
    //                                      PHASOR                                      //
    // ================================================================================ //
    
    /*
    Phasor<Scalar>::Phasor() noexcept : DspNode(0, 1), m_step(0.), m_phase(0.)
    {
        ;
    }
    
    Phasor<Scalar>::~Phasor()
    {
        ;
    }
    
    string Phasor<Scalar>::getName() const noexcept
    {
        return "Phasor (scalar)";
    }
    
    void Phasor<Scalar>::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
        m_step = 1. / (sample)getSampleRate();
    }
    
    void Phasor<Scalar>::perform() noexcept
    {
        m_phase = Signal::vphasor(getVectorSize(), m_step, m_phase, getOutputsSamples()[0]);
        sample* input0 = getInputsSamples()[0];
        sample* output0 = getOutputsSamples()[0];
        for(int i = 0; i < getVectorSize(); i++)
        {
            sample temp = (m_phase + (m_step * (float)input0[i]));
            output0[i] = (temp - floorf(temp));
        }
    }
    
    void Phasor<Scalar>::release() noexcept
    {
        
    }*/
    
    // ================================================================================ //
    //                                      NOISE                                       //
    // ================================================================================ //
    
    int DspNoise::c_seed = 0;
    
    DspNoise::DspNoise(sDspChain chain, const int seed) noexcept : DspNode(chain, 0, 1), m_seed(seed)
    {
        ;
    }
    
    DspNoise::~DspNoise()
    {
        ;
    }
    
    string DspNoise::getName() const noexcept
    {
        return "DspNoise";
    }
    
    void DspNoise::prepare() noexcept
    {
        shouldPerform(isOutputConnected(0));
    }
    
    void DspNoise::perform() noexcept
    {
        m_seed = Signal::vnoise(getVectorSize(), m_seed, getOutputsSamples()[0]);
    }
    
    void DspNoise::release() noexcept
    {
        
    }
    
    int DspNoise::nextSeed() noexcept
    {
        c_seed = (12345 + (1103515245 * c_seed));
        return c_seed;
    }
}

