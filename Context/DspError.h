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

#ifndef __DEF_KIWI_DSP_ERROR__
#define __DEF_KIWI_DSP_ERROR__

#include "DspSignal.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP ERROR                                   //
    // ================================================================================ //
    
    //! The dsp error is used to throw and catch error during the compilation of a chain.
    /**
     The dsp error defines the errors of the compilation.
     */
    class DspError : public exception
    {
    public:
        enum Type
        {
            Recopy     = 0, ///< Indicates that an node can't find the signal of an input link to recopy.
            Inplace    = 1, ///< Indicates that an output can't find the input signal for inplace processing.
            Alloc      = 2, ///< Indicates that the a signal has not been allocated.
            Loop       = 3, ///< Indicates a loop between two nodes.
        };
    private:
        const Type      m_type;
        const wDspNode  m_node;
    public:
        
        //! Constructor.
        /** The method creates a new error.
         @param first   The node that generates the error.
         @param type    The type of the error.
         */
        DspError(sDspNode node, Type const& type) noexcept : m_type(type), m_node(node)
        {
            ;
        }
        //! Destructor.
        /** The method does nothing.
         */
        ~DspError() noexcept
        {
            ;
        }
        
        //! Retrieve the information about the error.
        /** The method retrieves the information about the error.
         @return The information.
         */
        const char* what() const noexcept override
        {
            switch(m_type)
            {
                case Recopy:
                    return "A node can't find the signal to recopy from another node.";
                    break;
                case Inplace:
                    return "A node can't find its input signal for inplace processing.";
                    break;
                case Alloc:
                    return "A node can't allocate its signal.";
                    break;
                default:
                    return "Two nodes generate a loop.";
                    break;
            }
        }
        
        //! Retrieve the type of the error.
        /** The method retrieves the type of the error.
         @return The type of the error.
         */
        inline Type getType() const noexcept
        {
            return m_type;
        }
        
        //! Retrieve the node that generates the error.
        /** The method retrieves the node that generates the error.
         @return The node that generates the error.
         */
        inline sDspNode getNode() const noexcept
        {
            return m_node.lock();
        }
    };
}

#endif


