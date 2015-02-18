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

#ifndef __DEF_KIWI_DSP_GENERATOR__
#define __DEF_KIWI_DSP_GENERATOR__

#include "../Context/DspDevice.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      SIG                                         //
    // ================================================================================ //
    
    class DspSig : public DspNode
    {
    private:
        sample m_value;
    public:
        DspSig(sDspChain chain, const sample value = 0.) noexcept;
        ~DspSig();
        string getName() const noexcept override;
        void prepare() noexcept override;
        void perform() noexcept override;
        void release() noexcept override;
        void setValue(const sample value) noexcept;
        sample getValue() const noexcept;
    };
    
    // ================================================================================ //
    //                                      PHASOR                                      //
    // ================================================================================ //
    /*
    template <DspMode mode> class Phasor;
    
    template <> class Phasor<Scalar> : public DspNode
    {
    private:
        sample m_step;
        mutable sample m_phase;
    public:
        Phasor() noexcept;
        ~Phasor();
        string getName() const noexcept override;
        void prepare() noexcept override;
        void perform() noexcept override;
        void release() noexcept override;
    };
    
    template <> class Phasor<Vector> : public DspNode
    {
    private:
        sample m_step;
        mutable sample m_phase;
    public:
        Phasor() noexcept;
        ~Phasor();
        string getName() const noexcept override;
        void prepare() noexcept override;
        void perform() noexcept override;
        void release() noexcept override;
    };*/
    
    // ================================================================================ //
    //                                      NOISE                                       //
    // ================================================================================ //
    
    class DspNoise : public DspNode
    {
    private:
        static int c_seed;
        int m_seed;
    public:
        DspNoise(sDspChain chain, const int seed = 0.) noexcept;
        ~DspNoise();
        string getName() const noexcept override;
        void prepare() noexcept override;
        void perform() noexcept override;
        void release() noexcept override;
        static int nextSeed() noexcept;
    };
}

#endif


