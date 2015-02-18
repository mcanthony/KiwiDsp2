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

#ifndef __DEF_KIWI_DSP_DEVICE__
#define __DEF_KIWI_DSP_DEVICE__

#include "DspContext.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP DEVICE                                  //
    // ================================================================================ //
    
    //! The dsp device manager owns the informations....
    /**
     The dsp device manager
     */
    class DspDeviceManager
    {
    private:
        vector<sDspContext> m_contexts;
        mutable mutex       m_mutex;
        
    protected:
        
        //! The tick function to call at each dsp cycle.
        /** The function ticks all the contexts.
         */
        inline void tick() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<sDspContext>::size_type i = 0; i < m_contexts.size(); i++)
            {
                if(m_contexts[i]->isRunning())
                {
                    m_contexts[i]->tick();
                }
            }
        }
        
    public:
        
        //! The constructor.
        /** The function doesn't do anything.
         */
        DspDeviceManager() noexcept;
        
        //! The destructor.
        /** The function stops the dsp of the contexts and detach them.
         */
        virtual ~DspDeviceManager() noexcept;
        
        //! Add a context to the device manager.
        /** The function adds a context to the device manager.
         @param context The context to add.
         */
        void add(sDspContext context);
        
        //! Remove a context from the device manager.
        /** The function removes a context from the device manager.
         @param context The context to remove.
         */
        void remove(sDspContext chain);
        
        //! Retrieve the names of the available drivers.
        /** This function retrieves the names of the available drivers.
         @param drivers The names of the drivers.
         */
        virtual void getDriversNames(vector<string>& drivers) const = 0;
        
        //! Retrieve the names of the current driver.
        /** This function retrieves the names of the current driver.
         @return The names of the current driver.
         */
        virtual string getDriverName()  const = 0;
        
        //! Set the driver.
        /** This function sets the driver.
         @param The names of the driver.
         */
        virtual void setDriver(string const& driver) = 0;
    
        //! Retrieve the names of the available devices.
        /** This function retrieves the names of the available devices.
         @param devices The names of the devices.
         */
        virtual void getDevicesNames(vector<string>& devices) const = 0;
        
        //! Retrieve the names of the current input device.
        /** This function retrieves the names of the current input device.
         @return The name of the current input device.
         */
        virtual string getInputDeviceName() const = 0;
        
        //! Retrieve the names of the current output device.
        /** This function retrieves the names of the current output device.
         @return The name of the current output device.
         */
        virtual string getOutputDeviceName() const = 0;
        
        //! Set the input device.
        /** This function sets the input device.
         @param The names of the device.
         */
        virtual void setInputDevice(string const& device) = 0;
        
        //! Set the output device.
        /** This function sets the output device.
         @param The names of the device.
         */
        virtual void setOutputDevice(string const& device) = 0;
        
        //! Retrieve the maximum number of inputs of the current device.
        /** This function retrieves the maximum number of inputs of the current device.
         @return The maximum number of inputs of the current device.
         */
        virtual ulong getMaximumNumberOfInputs() const = 0;
        
        //! Retrieve the maximum number of outputs of the current device.
        /** This function retrieves the maximum number of outputs of the current device.
         @return The maximum number of outputs of the current device.
         */
        virtual ulong getMaximumNumberOfOutputs() const = 0;
        
        //! Retrieve the number of inputs of the current device.
        /** This function retrieves the number of inputs of the current device.
         @return The number of inputs of the current device.
         */
        virtual ulong getNumberOfInputs() const = 0;
        
        //! Retrieve the number of outputs of the current device.
        /** This function retrieves the number of outputs of the current device.
         @return The number of outputs of the current device.
         */
        virtual ulong getNumberOfOutputs() const = 0;
        
        //! Set the number of inputs of the current device.
        /** This function sets the number of inputs of the current device.
         @param ninputs The number of inputs.
         */
        virtual void setNumberOfInputs(ulong const ninputs) = 0;
        
        //! Retrieve the number of outputs of the current device.
        /** This function retrieves the number of outputs of the current device.
         @param noutputs The number of outputs.
         */
        virtual void setNumberOfOutputs(ulong const noutputs) = 0;
        
        //! Retrieve the available sample rates for the current devices.
        /** This function retrieves the available sample rates for the current devices.
         @param samplerates The available sample rates.
         */
        virtual void getAvailableSampleRates(vector<ulong>& samplerates) const = 0;
        
        //! Retrieve the current sample rate.
        /** This function retrieves the current sample rate.
         @return The current sample rate.
         */
        virtual ulong getSampleRate() const = 0;
        
        //! Set the sample rate.
        /** This function sets the sample rate.
         @param samplerate The sample rate.
         */
        virtual void setSampleRate(ulong const samplerate) = 0;
        
        //! Retrieve the available vector sizes for the current devices.
        /** This function retrieves the available vector sizes for the current devices.
         @param vectorsizes The available vector sizes.
         */
        virtual void getAvailableVectorSizes(vector<ulong>& vectorsizes) const = 0;
        
        //! Retrieve the current vector size.
        /** This function retrieves the current vector size.
         @return The current vector size.
         */
        virtual ulong getVectorSize() const = 0;
        
        //! Set the vector size.
        /** This function sets the vector size.
         @param vectorsize The vector size.
         */
        virtual void setVectorSize(ulong const vectorsize) = 0;
        
        //! Retrieve the inputs sample matrix.
        /** This function retrieves the inputs sample matrix.
         @return The inputs sample matrix.
         */
        virtual sample const* getInputsSamples() const noexcept = 0;
        
        //! Retrieve the outputs sample matrix.
        /** This function retrieves the outputs sample matrix.
         @return The outputs sample matrix.
         */
        virtual sample* getOutputsSamples() const noexcept = 0;
        
        //! Retrieve the number of contexts.
        /** The function retrieves the number of contexts.
         @return The number of contexts.
         */
        inline ulong getNumberOfContext() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (ulong)m_contexts.size();
        }
    };
}

#endif


