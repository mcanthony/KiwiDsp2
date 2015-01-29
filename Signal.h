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
    
    namespace Dsp
    {
#ifdef KIWI_DOUBLE
            typedef double sample;
#else
            typedef float  sample;
#endif

        class Process;
        typedef shared_ptr<Process>         sProcess;
        typedef weak_ptr<Process>           wProcess;
        typedef shared_ptr<const Process>   scProcess;
        typedef weak_ptr<const Process>     wcProcess;
        
        class Connection;
        typedef shared_ptr<Connection>      sConnection;
        typedef weak_ptr<Connection>        wConnection;
        typedef shared_ptr<const Connection>scConnection;
        typedef weak_ptr<const Connection>  wcConnection;
        
        class Node;
        typedef shared_ptr<Node>            sNode;
        typedef weak_ptr<Node>              wNode;
        typedef shared_ptr<const Node>      scNode;
        typedef weak_ptr<const Node>        wcNode;
        
        class Context;
        typedef shared_ptr<Context>         sContext;
        typedef weak_ptr<Context>           wContext;
        typedef shared_ptr<const Context>   scContext;
        typedef weak_ptr<const Context>     wcContext;
        
        // ================================================================================ //
        //                                      SIGNAL                                      //
        // ================================================================================ //
        
        //! The signal class offers static method to perform optimized operation with vectors of samples.
        /**
          The signal class offers static method to perform optimized operation with vectors of samples. All the methods offer are prototyped for single or double precision. It use the apple vDSP functions, the blas or atlas libraries or native c.
         */
        class Signal
        {
        public:
            
            static inline void vcopy(const ulong vectorsize, const float* in1, float* out1)
            {
#if defined (__APPLE__) || defined(__CBLAS__)
                cblas_scopy((const int)vectorsize, in1, 1, out1, 1);
#else
                memcpy(out1, in1, vectorsize * sizeof(float));
#endif
            }
            
            static inline void vcopy(const ulong vectorsize, const double* in1, double* out1)
            {
#if defined (__APPLE__) || defined(__CBLAS__)
               cblas_dcopy((const int)vectorsize, in1, 1, out1, 1);
#else
                memcpy(out1, in1, vectorsize * sizeof(double));
#endif
            }
            
            static inline void vfill(const ulong vectorsize, const float& in1, float* out1)
            {
#ifdef __APPLE__
                vDSP_vfill(&in1, out1, 1, (vDSP_Length)vectorsize);
#elif __CATLAS__
                catlas_sset((const int)vectorsize, &in1, out1, 1);
#else
                while(vectorsize--)
                    *(out1++) = in1;
#endif
            }
            
            static inline void vfill(const ulong vectorsize, const double& in1, double* out1)
            {
#ifdef __APPLE__
                vDSP_vfillD(&in1, out1, 1, (vDSP_Length)vectorsize);
#elif __CATLAS__
                catlas_sset((const int)vectorsize, &in1, out1, 1);
#else
                while(vectorsize--)
                    *(out1++) = in1;
#endif
            }
            
            static inline void vclear(const ulong vectorsize, float* out1)
            {
                
#ifdef __APPLE__
                vDSP_vclr(out1, 1, (vDSP_Length)vectorsize);
#else
                memset(out1, 0, vectorsize * sizeof(float));
#endif
            }
            
            static inline void vclear(const ulong vectorsize, double* out1)
            {
#ifdef __APPLE__
                vDSP_vclrD(out1, 1, (vDSP_Length)vectorsize);
#else
                memset(out1, 0, vectorsize * sizeof(double));
#endif
            }
            
            static inline void vsadd(const ulong vectorsize, const float& in1, float* out1)
            {
#ifdef __APPLE__
                vDSP_vsadd(out1, 1, &in1, out1, 1, vectorsize);
#else
                while(vectorsize--)
                    *(out1++) += in1;
#endif
            }
            
            static inline void vsadd(const ulong vectorsize, const double& in1, double* out1)
            {
#ifdef __APPLE__
                vDSP_vsaddD(out1, 1, &in1, out1, 1, vectorsize);
#else
                while(vectorsize--)
                    *(out1++) += in1;
#endif
            }
            
            static inline void vadd(const ulong vectorsize, const float* in1, float* out1)
            {
#if defined (__APPLE__) || defined(__CBLAS__)
                cblas_saxpy((const int)vectorsize, 1., in1, 1, out1, 1);
#else
                while(vectorsize--)
                    *(out1++) += *(in1++);
#endif
            }
            
            static inline void vadd(const ulong vectorsize, const double* in1, double* out1)
            {
#if defined (__APPLE__) || defined(__CBLAS__)
                cblas_daxpy((const int)vectorsize, 1., in1, 1, out1, 1);
#else
                while(vectorsize--)
                    *(out1++) += *(in1++);
#endif
            }
            
            static inline void vadd(const ulong vectorsize, const float* in1, const float* in2, float* out1)
            {
#ifdef __APPLE__
                vDSP_vadd(in1, 1, in2, 1, out1, 1, (vDSP_Length)vectorsize);
#elif __CBLAS__
                cblas_scopy(vectorsize, in1, 1, out1, 1);
                cblas_saxpy(vectorsize, 1., in2, 1, out1, 1);
#else
                while(vectorsize--)
                    *(out1++) = *(in1++) + *(in2++);
#endif
            }
            
            static inline void vadd(const ulong vectorsize, const double* in1, const double* in2, double* out1)
            {
#ifdef __APPLE__
                vDSP_vaddD(in1, 1, in2, 1, out1, 1, (vDSP_Length)vectorsize);
#elif __CBLAS__
                cblas_dcopy(vectorsize, in1, 1, out1, 1);
                cblas_daxpy(vectorsize, 1., in2, 1, out1, 1);
#else
                while(vectorsize--)
                    *(out1++) = *(in1++) + *(in2++);
#endif
            }
        };
    }
}


#endif


