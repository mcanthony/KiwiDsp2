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

#include "DspError.h"

namespace Kiwi
{
    DspError<DspLink>::DspError(scDspLink _link, Type const& _type) noexcept :
    type(_type),
    link(_link)
    {
        ;
    }
    
    DspError<DspLink>::~DspError() noexcept
    {
        ;
    }
    
    const char* DspError<DspLink>::what() const noexcept
    {
        switch(type)
        {
            case Duplicate:
                return "The link is already present in the context.";
                break;
            case From:
                return "The input node isn't in the context.";
                break;
            case To:
                return "The output node isn't in the context.";
                break;
            case Input:
                return "The input index doesn't match with the input node.";
                break;
            case Output:
                return "The output index doesn't match with the output node.";
                break;
            default:
                return "The link isn't valid.";
                break;
        }
    }
    
    DspError<DspNode>::DspError(sDspNode node1, sDspNode node2, Type const& type) noexcept :
    m_type(type),
    m_first(node1),
    m_second(node2)
    {
        ;
    }
    
    DspError<DspNode>::~DspError() noexcept
    {
        ;
    }
    
    const char* DspError<DspNode>::what() const noexcept
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
                return "A node can't find its input signal for inplace nodeing.";
                break;
            case Alloc:
                return "A node can't allocate its signal.";
                break;
            case Duplicate:
                return "A node is already present in the context.";
                break;
            case Valid:
                return "A node isn't valid.";
                break;
            default:
                return "Two nodes generate a loop.";
                break;
        }
    }
}















