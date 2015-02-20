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

#ifndef __DEF_KIWI_DSP_NODE__
#define __DEF_KIWI_DSP_NODE__

#include "DspIoput.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP NODE                                    //
    // ================================================================================ //
    
    //! The dsp nodes performs the digital signal processing.
    /**
     The dsp node is a virtual class that performs the digital signal processing and manages input and output connections.
     */
    class DspNode: public inheritable_enable_shared_from_this<DspNode>
    {
        friend DspChain;
        friend DspOutput;
        friend DspInput;
        friend DspLink;
    private:
        
        const wDspChain m_chain;
        const ulong     m_nins;
        sample** const  m_sample_ins;
        const ulong     m_nouts;
        sample** const  m_sample_outs;
        ulong           m_samplerate;
        ulong           m_vectorsize;
        vector<sDspInput>  m_inputs;
        vector<sDspOutput> m_outputs;
        
        bool            m_inplace;
        bool            m_running;
        ulong           index;
        
        //! Prepare the node to process.
        /** This function prepares the node to process. It allocates the signals for the inputs and the outputs.
         @param The chain that owns the node.
         */
        void start() throw(DspError&);
        
        //! Call once the process method of the inputs and of the process class.
        /** This function calls once the process.
         */
        inline void tick() noexcept
        {
            for(ulong i = 0; i < m_nins; i++)
            {
                m_inputs[i]->perform();
            }
            perform();
        }
        
        //! Notify the process that the dsp has been stopped.
        /** This function notifies that the dsp has been stopped.
         */
        void stop();
        
        //! Add a node to an input.
        /** This function adds a node to an input.
         @param node The node to add.
         @param index The index of the input.
         */
        void addInput(sDspNode node, const ulong index);
        
        //! Add a node to an output.
        /** This function adds a node to an output.
         @param node The node to add.
         @param index The index of the output.
         */
        void addOutput(sDspNode node, const ulong index);
        
        //! Remove a node from an input.
        /** This function removes a node from an input.
         @param node The node to remove.
         @param index The index of the input.
         */
        void removeInput(sDspNode node, const ulong index);
        
        //! Remove a node from an output.
        /** This function removes a node from an output.
         @param node The node to remove.
         @param index The index of the output.
         */
        void removeOutput(sDspNode node, const ulong index);
        
        //! Prepare the process for the dsp.
        /** The method preprares the dsp.
         @param node The dsp node that owns the dsp informations and should be configured.
         */
        virtual void prepare() noexcept = 0;
        
        //! Perform the process for the dsp.
        /** The method performs the dsp.
         @param node The dsp node that owns the dsp informations and the signals.
         */
        virtual void perform() noexcept = 0;
        
        //! Release the process after the dsp.
        /** The method releases the process after the dsp.
         @param node The dsp node that owns the dsp informations.
         */
        virtual void release() noexcept = 0;
        
    public:
        
        //! The constructor.
        /** Create the input and output vectors and allocate the input and output sample matrices.
         @param chain   The dsp chain.
         @param nins    The number of inputs.
         @param nouts   The number of outputs.
         */
        DspNode(sDspChain chain, const ulong nins, const ulong nouts) noexcept;
        
        //! The destructor.
        /** Free the input and ouputs vectors and matrices.
         */
        virtual ~DspNode();
        
        //! Retrieve the dsp chain of the node.
        /** This function retrieves the dsp chain of the node.
         @return The chain of the node.
         */
        inline sDspChain getChain() const noexcept
        {
            return m_chain.lock();
        }
        
        //! Retrieve the context of the dsp chain.
        /** This function retrieves the context of the dsp chain.
         @return The context of the dsp chain.
         */
        sDspContext getContext() const noexcept;
        
        //! Retrieve the device manager of the context.
        /** This function retrieves the device manager of the context.
         @return The device manager of the context.
         */
        sDspDeviceManager getDeviceManager() const noexcept;
        
        //! Retrieve the sample rate of the node.
        /** This function retrieves the sample rate of the node.
         @return The sample rate of the node.
         */
        inline ulong getSampleRate() const noexcept
        {
            return m_samplerate;
        }
        
        //! Retrieve the vector size of the node.
        /** This function retrieves the vector size of the node.
         @return The vector size of the node.
         */
        inline ulong getVectorSize() const noexcept
        {
            return m_vectorsize;
        }
        
        //! Retrieve the number of inputs of the process.
        /** The method retrieves the number of inputs of the process.
         @return The number of inputs of the process.
         */
        inline ulong getNumberOfInputs() const noexcept
        {
            return m_nins;
        }
        
        //! Retrieve the number of outputs of the process.
        /** The method retrieves the number of outputs of the process.
         @return The number of outputs of the process.
         */
        inline ulong getNumberOfOutputs() const noexcept
        {
            return m_nouts;
        }
        
        //! Retrieve the inputs sample matrix.
        /** This function retrieves the inputs sample matrix.
         @return The inputs sample matrix.
         */
        inline sample *const *const getInputsSamples() const noexcept
        {
            return m_sample_ins;
        }
        
        //! Retrieve the outputs sample matrix.
        /** This function retrieves the outputs sample matrix.
         @return The outputs sample matrix.
         */
        inline sample** getOutputsSamples() const noexcept
        {
            return m_sample_outs;
        }
        
        //! Check if the inputs and outputs signals owns the same vectors.
        /** This function checks if the signals owns the same vectors.
         @return True if the signals owns the same vectors it returns false.
         */
        inline bool isInplace() const noexcept
        {
            return m_inplace;
        }
        
        //! Check if the node is running in the dsp chain.
        /** This function checks if the node is running in the dsp chain.
         @return True if the node is running in the dsp chain otherwise it returns false.
         */
        inline bool isRunning() const noexcept
        {
            return m_running;
        }
        
        //! Check if a signal inlet is connected with signal.
        /** This function checks if a signal inlet is connected with signal.
         @return True if the inlet is connected otherwise it returns false.
         */
        bool isInputConnected(const ulong index) const noexcept;
        
        //! Check if a signal outlet is connected with signal.
        /** This function checks if a signal outlet is connected with signal.
         @return True if the outlet is connected otherwise it returns false.
         */
        bool isOutputConnected(const ulong index) const noexcept;
        
        //! Retrieve the name of the process.
        /** The method retrieves the name of the process.
         @return The name of the process.
         */
        virtual string getName() const noexcept = 0;
        
    protected:
        
        //! Set if the inputs and outputs signals owns the same vectors.
        /** This function sets if the signals owns the same vectors.
         @param status The inplace status.
         */
        void setInplace(const bool status) noexcept;
        
        //! Set if the node should be call in the dsp chain.
        /** This function sets if the node should be call in the dsp chain.
         @param status The perform status.
         */
        void shouldPerform(const bool status) noexcept;
    };
}


#endif


