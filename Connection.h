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
            const ulong     m_output;
            const wProcess  m_to;
            const ulong     m_input;
        public:
            
            //! Constructor.
            /** You should never have to call this method.
             */
            Connection(const sProcess from, const ulong output, const sProcess to, const ulong input) noexcept :
            m_from(from),
            m_output(output),
            m_to(to),
            m_input(input)
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
            
            //! The connection creator.
            /** This function create a new connection.
             @param from    The output process.
             @param output  The index of the output.
             @param to      The input process.
             @param input   The index of the input.
             */
            static inline sConnection create(const sProcess from, const ulong output, const sProcess to, const ulong input)
            {
                if(from && to)
                {
                    return make_shared<Connection>(from, output, to, input);
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
            
            //! Retrieve the index of the output of the connection.
            /** The function retrieves the index of the output of the connection.
             @return The index of the output of the connection.
             */
            inline ulong getOutputIndex() const noexcept
            {
                return m_output;
            }
            
            //! Retrieve the index of the input of the connection.
            /** The function retrieves the index of the input of the connection.
             @return The index of the input of the connection.
             */
            inline ulong getInputIndex() const noexcept
            {
                return m_input;
            }
        };
    }
}


#endif


