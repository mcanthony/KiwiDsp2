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

#ifndef __DEF_KIWI_OSCILLATORS__
#define __DEF_KIWI_OSCILLATORS__

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
            Sig(const sample value = 0.) noexcept :
            m_value(value)
            {
                ;
            }
            
            ~Sig()
            {
                ;
            }
            
            //! Retrieve the number of inputs of the process.
            /** The method retrieves the number of inputs of the process.
             @return The number of inputs of the process.
             */
            ulong getNumberOfInputs() const noexcept override
            {
                return 0;
            }
            
            //! Retrieve the number of outputs of the process.
            /** The method retrieves the number of outputs of the process.
             @return The number of outputs of the process.
             */
            ulong getNumberOfOutputs() const noexcept override
            {
                return 1;
            }
            
            //! Prepare the process for the dsp.
            /** The method preprares the dsp.
             @param node The dsp node that owns the dsp informations and should be configured.
             */
            void prepare(sNode node) const noexcept override
            {
                ;
            }
            
            //! Perform the process for the dsp.
            /** The method performs the dsp.
             @param node The dsp node that owns the dsp informations and the signals.
             */
            void perform(scNode node) const noexcept override
            {
                Signal::vfill(node->getVectorSize(), m_value, node->getOutputsSamples()[0]);
            }
            
            //! Release the process after the dsp.
            /** The method releases the process after the dsp.
             @param node The dsp node that owns the dsp informations.
             */
            void release(scNode node) const noexcept override
            {
                ;
            }
            
            //! Set...
            /** T...
             @param value ...
             */
            inline void setValue(const sample value) noexcept
            {
                m_value = value;
            }
            
            //! Get...
            /** T...
             @param value ...
             */
            inline sample getValue() const noexcept
            {
                return m_value;
            }
        };
    }
}

#endif


