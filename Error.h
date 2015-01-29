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

#include "Signal.h"

namespace Kiwi
{
    namespace Dsp
    {
        // ================================================================================ //
        //                                      DSP ERROR                                   //
        // ================================================================================ //
        
        //! The dsp error is used to throw and catch error in the compilation chain.
        /**
         The dsp error defines the error of the compilation.
         */
        template<class T> class Error;
        
        //! The connection error is used to throw and catch error while adding a process in the context.
        /**
         The dsp error defines the error while adding a process in the context. An error is thrown if the process is no more valid, if the node of the process hasn't been created or if the process is already present in the context.
         */
        template<> class Error<Process> : public exception
        {
        public:
            enum Type
            {
                Valid       = 0,
                Node        = 1,
                Duplicate   = 2
            };
            
            const Type      type;
            const scProcess process;
            
            //! Constructor.
            /** The method creates a new error.
             @param _process The process that generated an error.
             @param _type    The type of error.
             */
            Error(scProcess _process, Type const& _type) noexcept;
            
            //! Destructor.
            /** The method does nothing.
             */
            ~Error() noexcept;
            
            //! Retrieve the information about the error.
            /** The method retrieves the information about the error.
             @return The information.
             */
            const char* what() const noexcept override;
        };
        
        //! The connection error is used to throw and catch error while adding a connection in the context.
        /**
         The dsp error defines the error while adding a connection in the context. An error is thrown if the connection is no more valid, if the connection is already present in the context, if a node isn't in the connext or if the index of the input or the index of the output doesn't match with the number of input or the numerb of outpur of the processes.
         */
        template<> class Error<Connection> : public exception
        {
        public:
            enum Type
            {
                Valid       = 0,
                Duplicate   = 1,
                From        = 2,
                To          = 3,
                Input       = 4,
                Output      = 5
            };
            
            const Type         type;
            const scConnection connection;
            
            //! Constructor.
            /** The method creates a new error.
             @param _connection The connection that generated an error.
             @param _type       The type of error.
             */
            Error(scConnection _connection, Type const& _type) noexcept;
            
            //! Destructor.
            /** The method does nothing.
             */
            ~Error() noexcept;
            
            //! Retrieve the information about the error.
            /** The method retrieves the information about the error.
             @return The information.
             */
            const char* what() const noexcept override;
        };
        
        //! The dsp error is used to throw and catch error in the compilation chain.
        /**
         The dsp error defines the error of the compilation.
         */
        template<> class Error<Node> : public exception
        {
        public:
            enum Type
            {
                SampleRate = 0, ///< Indicates that an input and an output in a connection don't have the same sample rate.
                VectorSize = 1, ///< Indicates that an input and an output in a connection don't have the same vector size.
                Recopy     = 2, ///< Indicates that an input can't find the signal of an input connection to recopy.
                Inplace    = 3, ///< Indicates that an output can't find the input signal for inplace processing.
                Alloc      = 4, ///< Indicates that the a signal has not been allocated.
                Loop       = 7  ///< Indicates a loop between two nodes.
            };
        private:
            const Type     m_type;
            const wNode    m_first;
            const wNode    m_second;
        public:
            
            //! Constructor.
            /** The method creates a new error.
             @param first   The first node that generates the error.
             @param second  The second node associated with the error.
             @param type    The type of the error.
             */
            Error(sNode node1, sNode node2, Type const& type) noexcept;
            
            //! Destructor.
            /** The method does nothing.
             */
            ~Error() noexcept;
            
            //! Retrieve the information about the error.
            /** The method retrieves the information about the error.
             @return The information.
             */
            const char* what() const noexcept override;
            
            //! Retrieve the type of the error.
            /** The method retrieves the type of the error.
             @return The type of the error.
             */
            inline Type getType() const noexcept
            {
                return m_type;
            }
            
            //! Retrieve the first node that generates the error.
            /** The method retrieves the first node that generates the error.
             @return The first node that generates the error.
             */
            inline sNode getFirst() const noexcept
            {
                return m_first.lock();
            }
            
            //! Retrieve the second node associated with the error.
            /** The method retrieves the second node associated with the error.
             @return The second node associated with the error.
             */
            inline sNode getSecond() const noexcept
            {
                return m_second.lock();
            }
        };
    }
}


#endif


