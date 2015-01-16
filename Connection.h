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

#ifndef __DEF_KIWI_DSP_CONNECTION__
#define __DEF_KIWI_DSP_CONNECTION__

#include "Process.h"

namespace Kiwi
{
    namespace Dsp
    {
        // ================================================================================ //
        //                                      DSP CONNECTION                              //
        // ================================================================================ //
        
        //! The dsp connection owns the basic informations of a connection between two process.
        /**
         The dsp connection owns an input and an output process and the indices of the input and the ouput.
         */
        class Connection
        {
        private:
            const wProcess  m_from;
            const ulong     m_outlet;
            const wProcess  m_to;
            const ulong     m_inlet;
        public:
            
            //! Constructor.
            /** You should never have to call this method.
             */
            Connection(const sProcess from, const ulong outlet, const sProcess to, const ulong inlet) noexcept :
            m_from(from),
            m_outlet(outlet),
            m_to(to),
            m_inlet(inlet)
            {
                ;
            }
            
            //! Destructor.
            /** You should never have to call this method.
             */
            ~Connection()
            {
                ;
            }
            
            //! Connection creator.
            /** This function create a new connection.
             @param from    The output process.
             @param outlet  The index of the outlet.
             @param to      The input process.
             @param inlet   The index of the inlet.
             */
            static inline sConnection create(const sProcess from, const ulong outlet, const sProcess to, const ulong inlet)
            {
                if(from && to)
                {
                    return make_shared<Connection>(from, outlet, to, inlet);
                }
                else
                {
                    return nullptr;
                }
            }
            
            //! Retrieve the output process.
            /** The function retrieves the output process of the connection.
             @return The output process.
             */
            inline sProcess getProcessFrom() const noexcept
            {
                return m_from.lock();
            }
            
            //! Retrieve the input process.
            /** The function retrieves the input process of the connection.
             @return The input process.
             */
            inline sProcess getProcessTo() const noexcept
            {
                return m_to.lock();
            }
            
            //! Retrieve the index of the outlet of the connection.
            /** The function retrieves the index of the outlet of the connection.
             @return The index of the outlet of the connection.
             */
            inline ulong getOutletIndex() const noexcept
            {
                return m_outlet;
            }
            
            //! Retrieve the index of the inlet of the connection.
            /** The function retrieves the index of the inlet of the connection.
             @return The index of the inlet of the connection.
             */
            inline ulong getInletIndex() const noexcept
            {
                return m_inlet;
            }
        };
    }
}


#endif


