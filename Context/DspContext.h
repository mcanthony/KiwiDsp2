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

#ifndef __DEF_KIWI_DSP_CONTEXT__
#define __DEF_KIWI_DSP_CONTEXT__

#include "DspChain.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP CONTEXT                                 //
    // ================================================================================ //
    
    //! The dsp context manages a set of dsp chains.
    /**
     The dsp context
     */
    class DspContext : public inheritable_enable_shared_from_this<DspContext>
    {
        friend DspDeviceManager;
        
    private:
        const wDspDeviceManager m_device;
        vector<sDspChain>       m_chains;
        mutable mutex           m_mutex;
        mutable double          m_cpu;
        double                  m_cpu_factor;
        atomic_bool             m_running;
        
        //! Perform a tick on the dsp context.
        /** The function calls once all the node methods of the dsp chains.
         */
        inline void tick() const noexcept
        {
            auto start = std::chrono::high_resolution_clock::now();
            lock_guard<mutex> guard(m_mutex);
            for(vector<sDspChain>::size_type i = 0; i < m_chains.size(); i++)
            {
                if(m_chains[i]->isRunning())
                {
                    m_chains[i]->tick();
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            m_cpu = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }
        
    public:
        
        //! The constructor.
        /** The function initialize and empty context.
         @param device The device manager.
         */
        DspContext(sDspDeviceManager device) noexcept;
        
        //! The destructor.
        /** Stop the digital signal processing if needed and free the chains.
         */
        ~DspContext();
        
        //! Retrieve the device manager of the context.
        /** This function retrieves the sample rate of the chain.
         @return The sample rate of the chain.
         */
        inline sDspDeviceManager getDeviceManager() const noexcept
        {
            return m_device.lock();
        }
        
        //! Retrieve the sample rate of the context.
        /** This function retrieves the sample rate of the chain.
         @return The sample rate of the chain.
         */
        ulong getSampleRate() const noexcept;
        
        //! Retrieve the vector size of the context.
        /** This function retrieves the vector size of the chain.
         @return The vector size of the chain.
         */
        ulong getVectorSize() const noexcept;
        
        //! Check if the dsp is running.
        /** This function checks if the dsp is running.
         @return True if the dsp is running otherwise it returns false.
         */
        inline bool isRunning() const noexcept
        {
            return m_running;
        }
        
        //! Retrieve the number of chains.
        /** The function retrieves the number of chains.
         @return The number of chains.
         */
        inline ulong getNumberOfChains() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (ulong)m_chains.size();
        }
        
        //! Retrieve the CPU of the context.
        /** The function retrieves the CPU of the context.
         @return The CPU of the context in percent.
         */
        inline double getCPU() const noexcept
        {
            return m_cpu * m_cpu_factor;
        }
        
        //! Add a chain to the dsp context.
        /** The function adds a chain to the dsp context.
         @param chain The chain to add.
         */
        void add(sDspChain chain);
        
        //! Remove a chain from the dsp context.
        /** The function removes a chain from the dsp context.
         @param chain The chain to remove.
         */
        void remove(sDspChain chain);
        
        //! Start the dsp context.
        /** The function starts the dsp.
         */
        void start();
        
        //! Resume the dsp context.
        /** The function resume the dsp.
         @param state The state of the dsp context.
         */
        void resume(const bool state);
        
        //! Suspend the dsp context.
        /** The function suspends the dsp.
         @return The state of the dsp context.
         */
        bool suspend();
        
        //! Stop the dsp.
        /** The function call the stop the dsp of all the chains.
         */
        void stop();
    };
}


#endif


