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

#include "Error.h"

namespace Kiwi
{
    namespace Dsp
    {
        Error<Process>::Error(scProcess _process, Type const& _type) noexcept :
        type(_type),
        process(_process)
        {
            ;
        }
        
        Error<Process>::~Error() noexcept
        {
            ;
        }
        
        const char* Error<Process>::what() const noexcept
        {
            switch(type)
            {
                case Node:
                    return "The context has not been able to create a node for the process.";
                    break;
                case Duplicate:
                    return "The process is already present in the context.";
                    break;
                default:
                    return "The process isn't valid.";
                    break;
            }
        }
        
        Error<Connection>::Error(scConnection _connection, Type const& _type) noexcept :
        type(_type),
        connection(_connection)
        {
            ;
        }
        
        Error<Connection>::~Error() noexcept
        {
            ;
        }
        
        const char* Error<Connection>::what() const noexcept
        {
            switch(type)
            {
                case Duplicate:
                    return "The connection is already present in the context.";
                    break;
                case From:
                    return "The input process isn't in the context.";
                    break;
                case To:
                    return "The output process isn't in the context.";
                    break;
                case Input:
                    return "The input index doesn't match with the input process.";
                    break;
                case Output:
                    return "The output index doesn't match with the output process.";
                    break;
                default:
                    return "The connection isn't valid.";
                    break;
            }
        }
        
        Error<Node>::Error(sNode node1, sNode node2, Type const& type) noexcept :
        m_type(type),
        m_first(node1),
        m_second(node2)
        {
            ;
        }
        
        Error<Node>::~Error() noexcept
        {
            ;
        }
        
        const char* Error<Node>::what() const noexcept
        {
            switch(m_type)
            {
                case SampleRate:
                    return "Two connected nodes don't have the same sample rate.";
                    break;
                case VectorSize:
                    return "Two connected nodes don't have the same vector size.";
                    break;
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
    }
}















