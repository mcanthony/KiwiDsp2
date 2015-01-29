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

#include "../Context.h"

namespace Kiwi
{
    namespace Dsp
    {
        class Sig : public Process
        {
        private:
            sample m_value;
        public:
            Sig(const sample value = 0.) noexcept;
            ~Sig();
            string getName() const noexcept override;
            ulong getNumberOfInputs() const noexcept override;
            ulong getNumberOfOutputs() const noexcept override;
            void prepare(sNode node) const noexcept override;
            void perform(scNode node) const noexcept override;
            void release(scNode node) const noexcept override;
            void setValue(const sample value) noexcept;
            sample getValue() const noexcept;
        };
    }
}

#endif


