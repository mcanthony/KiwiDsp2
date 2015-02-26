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

#include "../Core/Core.h"

namespace Kiwi
{
    
#ifdef __KIWI_DSP_DOUBLE__
    typedef double sample;
#else
    typedef float  sample;
#endif
    
    class DspOutput;
    class DspInput;
    typedef shared_ptr<DspOutput>   sDspOutput;
    typedef shared_ptr<DspInput>    sDspInput;
    
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
    
    class DspChain;
    typedef shared_ptr<DspChain>        sDspChain;
    typedef weak_ptr<DspChain>          wDspChain;
    typedef shared_ptr<const DspChain>  scDspChain;
    typedef weak_ptr<const DspChain>    wcDspChain;
    
    class DspContext;
    typedef shared_ptr<DspContext>        sDspContext;
    typedef weak_ptr<DspContext>          wDspContext;
    typedef shared_ptr<const DspContext>  scDspContext;
    typedef weak_ptr<const DspContext>    wcDspContext;
    
    class DspDeviceManager;
    typedef shared_ptr<DspDeviceManager>        sDspDeviceManager;
    typedef weak_ptr<DspDeviceManager>          wDspDeviceManager;
    typedef shared_ptr<const DspDeviceManager>  scDspDeviceManager;
    typedef weak_ptr<const DspDeviceManager>    wcDspDeviceManager;
    
    typedef set<weak_ptr<DspNode>, owner_less< weak_ptr<DspNode>>> DspNodeSet;
    
    enum DspMode : bool
    {
        DspScalar = false,
        DspVector = true
    };
}

namespace Kiwi
{
    // ================================================================================ //
    //                                      SIGNAL                                      //
    // ================================================================================ //
    
    //! The signal class offers static method to perform optimized operations with vectors of samples.
    /**
     The signal class offers static method to perform optimized operations with vectors of samples. All the methods are prototyped for single or double precision. It use the apple vDSP functions, the blas or atlas libraries or native c.
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
        
        static inline void vinterleave(const ulong vectorsize, const ulong nrow, const float* in1, float* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            for(ulong i = 0; i < nrow; i++)
            {
                cblas_scopy((const int)vectorsize, in1+i*vectorsize, 1, out1+i, (const int)nrow);
            }
#else
            for(ulong i = 0; i < nrow; i++)
            {
                for(ulong j = 0; j < vectorsize; j++)
                {
                    *(out1++) = *(in1+nrow+j*nrow);
                }
            }
#endif
        }
        
        static inline void vinterleave(const ulong vectorsize, const ulong nrow, const double* in1, double* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            for(ulong i = 0; i < nrow; i++)
            {
                cblas_dcopy((const int)vectorsize, in1+i*vectorsize, 1, out1+i, (const int)nrow);
            }
#else
            for(ulong i = 0; i < nrow; i++)
            {
                for(ulong j = 0; j < vectorsize; j++)
                {
                    *(out1++) = *(in1+nrow+j*nrow);
                }
            }
#endif
        }
        
        static inline void vdeterleave(const ulong vectorsize, const ulong nrow, const float* in1, float* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            for(ulong i = 0; i < nrow; i++)
            {
                cblas_scopy((const int)vectorsize, in1+i, (const int)nrow, out1+i*vectorsize, 1);
            }
#else
            for(ulong i = 0; i < nrow; i++)
            {
                for(ulong j = 0; j < vectorsize; j++)
                {
                    *(out1++) = *(in1+nrow+j*nrow);
                }
            }
#endif
        }
        
        static inline void vdeterleave(const ulong vectorsize, const ulong nrow, const double* in1, double* out1)
        {
#if defined (__APPLE__) || defined(__CBLAS__)
            for(ulong i = 0; i < nrow; i++)
            {
                cblas_dcopy((const int)vectorsize, in1+i, (const int)nrow, out1+i*vectorsize, 1);
            }
#else
            for(ulong i = 0; i < nrow; i++)
            {
                for(ulong j = 0; j < vectorsize; j++)
                {
                    *(out1++) = *(in1+nrow+j*nrow);
                }
            }
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


