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

#include "DspError.h"

namespace Kiwi
{    
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
        
        static inline void vpost(ulong vectorsize, const float* in1)
        {
            while(vectorsize--)
                cout << *(in1++) << " ";
            cout << endl;
        }
        
        static inline void vpost(ulong vectorsize, const double* in1)
        {
            while(vectorsize--)
                cout << *(in1++) << " ";
            cout << endl;
        }
        
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
        
        static inline void vfill(ulong vectorsize, const float& in1, float* out1)
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
        
        static inline void vfill(ulong vectorsize, const double& in1, double* out1)
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
        
        static inline void vsadd(ulong vectorsize, const float& in1, float* out1)
        {
#ifdef __APPLE__
            vDSP_vsadd(out1, 1, &in1, out1, 1, vectorsize);
#else
            while(vectorsize--)
                *(out1++) += in1;
#endif
        }
        
        static inline void vsadd(ulong vectorsize, const double& in1, double* out1)
        {
#ifdef __APPLE__
            vDSP_vsaddD(out1, 1, &in1, out1, 1, vectorsize);
#else
            while(vectorsize--)
                *(out1++) += in1;
#endif
        }
        
        static inline void vadd(ulong vectorsize, const float* in1, float* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            cblas_saxpy((const int)vectorsize, 1., in1, 1, out1, 1);
#else
            while(vectorsize--)
                *(out1++) += *(in1++);
#endif
        }
        
        static inline void vadd(ulong vectorsize, const double* in1, double* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            cblas_daxpy((const int)vectorsize, 1., in1, 1, out1, 1);
#else
            while(vectorsize--)
                *(out1++) += *(in1++);
#endif
        }
        
        static inline void vadd(ulong vectorsize, const float* in1, const float* in2, float* out1)
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
        
        static inline void vadd(ulong vectorsize, const double* in1, const double* in2, double* out1)
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
        
        static inline int vnoise(ulong vectorsize, int seed, float* out1)
        {
            while(vectorsize--)
            {
                seed = (12345 + (1103515245 * seed));
                *(out1++) = (float)(4.656612875245797e-10f * seed);
            }
            return seed;
        }
        
        static inline int vnoise(ulong vectorsize, int seed, double* out1)
        {
            while(vectorsize--)
            {
                seed = (12345 + (1103515245 * seed));
                *(out1++) = (double)(4.656612875245797e-10 * seed);
            }
            return seed;
        }
        /*
         static inline double vphasor(const ulong vectorsize, const float step, float phase, float* out1)
         {
         for(ulong i = 0; i < vectorsize; i++)
         {
         sample temp = (phase + (step * (float)input0[i]));
         *(out1++) = phase = (temp - floorf(temp));
         }
         return phase;
         }
         
         static inline double vphasor(ulong vectorsize, const double step, double phase, double* out1)
         {
         return phase;
         }*/
    };
}


#endif


