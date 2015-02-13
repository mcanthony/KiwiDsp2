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

#include <stdio.h>
#include <stdarg.h>
#include <cwchar>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <cmath>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <thread>
#include <exception>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

using namespace std;

namespace Kiwi
{
#ifndef ulong
    typedef unsigned long ulong;
#endif
    
#ifdef KIWI_DOUBLE
    typedef double sample;
#else
    typedef float  sample;
#endif
    enum DspMode : bool
    {
        Scalar = 0,
        Vector = 1
    };
        
    class DspLink;
    typedef shared_ptr<DspLink>         sDspLink;
    typedef weak_ptr<DspLink>           wDspLink;
    typedef shared_ptr<const DspLink>   scDspLink;
    typedef weak_ptr<const DspLink>     wcDspLink;
    
    class DspNode;
    typedef shared_ptr<DspNode>         sDspNode;
    typedef weak_ptr<DspNode>           wDspNode;
    typedef shared_ptr<const DspNode>   scDspNode;
    typedef weak_ptr<const DspNode>     wcDspNode;
    
    class DspContext;
    typedef shared_ptr<DspContext>      sDspContext;
    typedef weak_ptr<DspContext>        wDspContext;
    typedef shared_ptr<const DspContext>scDspContext;
    typedef weak_ptr<const DspContext>  wcDspContext;
        
    // ================================================================================ //
    //                                      DSP ERROR                                   //
    // ================================================================================ //
    
    //! The dsp error is used to throw and catch error in the compilation chain.
    /**
     The dsp error defines the error of the compilation.
     */
    template<class T> class DspError;
    
    //! The link error is used to throw and catch error while adding a link in the context.
    /**
     The dsp error defines the error while adding a link in the context. An error is thrown if the link is no more valid, if the link is already present in the context, if a node isn't in the connext or if the index of the input or the index of the output doesn't match with the number of input or the number of output of the nodes.
     */
    template<> class DspError<DspLink> : public exception
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
        const scDspLink link;
        
        //! Constructor.
        /** The method creates a new error.
         @param _link The link that generated an error.
         @param _type       The type of error.
         */
        DspError(scDspLink _link, Type const& _type) noexcept;
        
        //! Destructor.
        /** The method does nothing.
         */
        ~DspError() noexcept;
        
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
    template<> class DspError<DspNode> : public exception
    {
    public:
        enum Type
        {
            SampleRate = 0, ///< Indicates that an input and an output in a link don't have the same sample rate.
            VectorSize = 1, ///< Indicates that an input and an output in a link don't have the same vector size.
            Recopy     = 2, ///< Indicates that an input can't find the signal of an input link to recopy.
            Inplace    = 3, ///< Indicates that an output can't find the input signal for inplace processing.
            Alloc      = 4, ///< Indicates that the a signal has not been allocated.
            Loop       = 7, ///< Indicates a loop between two nodes.
            Duplicate  = 8, ///< Indicates that the node is already present in the context.
            Valid      = 9  ///< Indicates that the node isn't valid.
        };
    private:
        const Type     m_type;
        const wDspNode    m_first;
        const wDspNode    m_second;
    public:
        
        //! Constructor.
        /** The method creates a new error.
         @param first   The first node that generates the error.
         @param second  The second node associated with the error.
         @param type    The type of the error.
         */
        DspError(sDspNode node1, sDspNode node2, Type const& type) noexcept;
        
        //! Destructor.
        /** The method does nothing.
         */
        ~DspError() noexcept;
        
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
        inline sDspNode getFirst() const noexcept
        {
            return m_first.lock();
        }
        
        //! Retrieve the second node associated with the error.
        /** The method retrieves the second node associated with the error.
         @return The second node associated with the error.
         */
        inline sDspNode getSecond() const noexcept
        {
            return m_second.lock();
        }
    };
}

#endif


