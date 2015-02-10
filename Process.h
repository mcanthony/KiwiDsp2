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

#ifndef __DEF_KIWI_DSP_PROCESS__
#define __DEF_KIWI_DSP_PROCESS__

#include "Signal.h"

namespace Kiwi
{
    namespace Dsp
    {
        // ================================================================================ //
        //                                      DSP PROCESS                                 //
        // ================================================================================ //
        
        //! The dsp process performs a digital signal processing.
        /**
         The dsp process performs a digital signal processing. It is called and configured by a dsp node. The dsp process is a pure virtual class that owns the prototype of all the methods required for a digital signal processing.
         */
        class Process
        {
        public:
            
            //! Constructor.
            /** The method does nothing.
             */
            Process() noexcept
            {
                ;
            }
            
            //! Destructor.
            /** The method does nothing.
             */
            virtual ~Process()
            {
                ;
            }
            
            //! Retrieve the name of the process.
            /** The method retrieves the name of the process.
             @return The name of the process.
             */
            virtual string getName() const noexcept = 0;
            
            //! Retrieve the number of inputs of the process.
            /** The method retrieves the number of inputs of the process.
             @return The number of inputs of the process.
             */
            virtual ulong getNumberOfInputs() const noexcept = 0;
            
            //! Retrieve the number of outputs of the process.
            /** The method retrieves the number of outputs of the process.
             @return The number of outputs of the process.
             */
            virtual ulong getNumberOfOutputs() const noexcept = 0;
            
            //! Prepare the process for the dsp.
            /** The method preprares the dsp.
             @param node The dsp node that owns the dsp informations and should be configured.
             */
            virtual void prepare(sNode node) const noexcept = 0;
            
            //! Perform the process for the dsp.
            /** The method performs the dsp.
             @param node The dsp node that owns the dsp informations and the signals.
             */
            virtual void perform(scNode node) const noexcept = 0;
            
            //! Release the process after the dsp.
            /** The method releases the process after the dsp.
             @param node The dsp node that owns the dsp informations.
             */
            virtual void release(scNode node) const noexcept = 0;
        };
    }
}


#endif


