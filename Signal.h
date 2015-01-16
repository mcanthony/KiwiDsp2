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

#ifndef __DEF_KIWI_DSP_SIGNAL__
#define __DEF_KIWI_DSP_SIGNAL__

#include "Defs.h"

namespace Kiwi
{
    namespace Dsp
    {
        // ================================================================================ //
        //                                      SIGNAL                                      //
        // ================================================================================ //
        
        //! The signal manages the sample vectors.
        /**
         The signal owns a vector of sample and manages the ownership and sharing of the vector between several dsp nodes.
         */
        class Signal
        {
        private:
            sample* const   m_vector;
            const bool      m_owner;
            bool            m_borrowed;
            
        public:
            
            //! Constructor.
            /** You should never have to call this method.
             */
            Signal(sample* const vector, const bool owner) noexcept;
            
            //! Destructor.
            /** You should never have to call this method.
             */
            ~Signal();
            
            //! Signal creator.
            /** This function create a new signal.
             @param size The size of the vector.
             @return The signal.
             */
            static inline sSignal create(const ulong size)
            {
                sample* vec = new sample[size];
                if(vec)
                {
                    return make_shared<Signal>(vec, true);
                }
                else
                {
                    return nullptr;
                }
            }
            
            //! Signal creator.
            /** This function create a new signal from another one.
             @param sig     The other signal.
             @param borrow  If the other signal should be marked as borrowed.
             @return The signal.
             */
            static inline sSignal create(sSignal sig, const bool borrow)
            {
                if(sig)
                {
                    sig->m_borrowed = borrow;
                    return make_shared<Signal>(sig->getVector(), false);
                }
                else
                {
                    return nullptr;
                }
            }
            
            //! Check if the vector is borrowed.
            /** This function checks if the vector is borrowed.
             @return The borrowed status.
             */
            inline bool isBorrowed() const noexcept
            {
                return m_borrowed;
            }
            
            //! Check if the signal is the owner of the vector.
            /** This function checks if the signal is the owner of the vector.
             @return The owner status.
             */
            inline bool isOwner() const noexcept
            {
                return m_owner;
            }
            
            //! Retrieve the vector of the signal.
            /** This function retrieves the vector of the signal.
             @return The vector of the signal.
             */
            inline sample* getVector() const noexcept
            {
                return m_vector;
            }
            
            //! Check if two signals have the same vector.
            /** This function checks if two signals have the same vector.
             @return true if the two signals have the same vector, otherwise false.
             */
            inline bool operator==(Signal &other) noexcept
            {
                return other.m_vector == m_vector;
            }
            
            //! Check if two signals don't have the same vector.
            /** This function checks if two signals don't have the same vector.
             @return true if the two signals don't have the same vector, otherwise false.
             */
            inline bool operator!=(Signal &other) noexcept
            {
                return other.m_vector != m_vector;
            }
            
            
        };
    }
}


#endif


