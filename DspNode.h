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

#include "Signal.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP NODE                                    //
    // ================================================================================ //
    
    //! The dsp nodes manages the digital signal processing of a process.
    /**
     The dsp node manages the dsp of a process and should be configure during the call of the prepare method of the process.
     */
    class DspNode: public enable_shared_from_this<DspNode>
    {
    private:
        friend DspContext;
        class Output;
        class Input;
        typedef shared_ptr<Output>  sOutput;
        typedef shared_ptr<Input>   sInput;
        typedef set<weak_ptr<DspNode>, owner_less< weak_ptr<DspNode>>> DspNodeSet;
        
        const ulong     m_nins;
        sample** const  m_sample_ins;
        const ulong     m_nouts;
        sample** const  m_sample_outs;
        ulong           m_samplerate;
        ulong           m_vectorsize;
        vector<sInput>  m_inputs;
        vector<sOutput> m_outputs;
        
        bool            m_inplace;
        bool            m_shouldperform;
        bool            m_compiled;
        
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        DspNode(const ulong nins, const ulong nouts) noexcept;
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        virtual ~DspNode();
        
        //! Add a node to an input.
        /** This function adds a node to an input.
         @param node The node to add.
         @param index The index of the input.
         */
        void addInputDspNode(sDspNode node, const ulong index) throw(bool);
        
        //! Add a node to an output.
        /** This function adds a node to an output.
         @param node The node to add.
         @param index The index of the output.
         */
        void addOutputDspNode(sDspNode node, const ulong index) throw(bool);
        
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
        
        //! Check if the node is valid.
        /** This function checks if the node is valid.
         @return True if the node is valid otherwise it returns false.
         */
        inline bool shouldPerform() const noexcept
        {
            return m_shouldperform;
        }
        
        //! Check if the node is compiled.
        /** This function checks if the node is compiled.
         @return True if the node is compiled otherwise it returns false.
         */
        inline bool isCompiled() const noexcept
        {
            return m_compiled;
        }
        
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
        
        //! Prepare the node to process.
        /** This function prepares the node to process. It allocates the signals for the inputs and the outputs.
         @param The context that owns the node.
         */
        void compile(scDspContext context) throw(DspError<DspNode>&);
        
        //! Call once the process method of the inputs and of the process class.
        /** This function calls once the process.
         */
        void tick() const;
        
        //! Notify the process that the dsp has been stopped.
        /** This function notifies that the dsp has been stopped.
         */
        void stop();
        
        //! Check if a signal inlet is connected with signal.
        /** This function checks if a signal inlet is connected with signal.
         @return True if the inlet is connected otherwise it returns false.
         */
        bool isInputConnected(const ulong index) const noexcept;
        
        //! Check if a signal outlet is connected with signal.
        /** This function checks if a signal outlet is connected with signal.
         @return True if the outlet is connected otherwise it returns false.
         */
        bool isOutputConnected(long index) const noexcept;
        
        //! Retrieve the name of the process.
        /** The method retrieves the name of the process.
         @return The name of the process.
         */
        virtual string getName() const noexcept = 0;
        
        //! Prepare the process for the dsp.
        /** The method preprares the dsp.
         @param node The dsp node that owns the dsp informations and should be configured.
         */
        virtual void prepare() noexcept = 0;
        
        //! Perform the process for the dsp.
        /** The method performs the dsp.
         @param node The dsp node that owns the dsp informations and the signals.
         */
        virtual void perform() const noexcept = 0;
        
        //! Release the process after the dsp.
        /** The method releases the process after the dsp.
         @param node The dsp node that owns the dsp informations.
         */
        virtual void release() noexcept = 0;
    };
    
    // ================================================================================ //
    //                                      DSP OUTPUT                                  //
    // ================================================================================ //
    
    //! The ouput manages the sample vectors of one ouput of a node.
    /**
     The ouput owns a vector of sample and manages the ownership and sharing of the vector between several dsp nodes.
     */
    class DspNode::Output
    {
    private:
        friend DspContext;
        const ulong   m_index;
        sample*       m_vector;
        bool          m_owner;
        DspNodeSet       m_links;
        
    public:
        //! Constructor.
        /** You should never have to call this method.
         */
        Output(const ulong index) noexcept;
        
        //! Destructor.
        /** You should never have to call this method.
         */
        ~Output();
        
        //! Add a node to the output.
        /** This function adds a node to the output.
         @param The node to add.
         */
        void addDspNode(sDspNode node) throw(bool);
        
        //! Prepare the output.
        /** This function prepare the output.
         @param The owner node.
         */
        void prepare(sDspNode node) throw(DspError<DspNode>&);
        
        //! Retrieve if the links are empty.
        /** This function retrieves if the links are empty.
         @param true if if the links are empty, otherwise false.
         */
        inline bool empty() const noexcept
        {
            return m_links.empty();
        }
        
        //! Retrieve the number of links.
        /** This function retrieves the number of links.
         @param The number of links.
         */
        inline ulong size() const noexcept
        {
            return (ulong)m_links.size();
        }
        
        //! Retrieve if the output has a node.
        /** This function retrieves if the output has a node.
         @param true if the output has a node, otherwise false.
         */
        inline bool hasDspNode(sDspNode node) const
        {
            return m_links.find(node) != m_links.end();
        }
        
        //! Check if the output is the owner of the vector.
        /** This function checks if the output is the owner of the vector.
         @return The owner status.
         */
        inline bool isOwner() const noexcept
        {
            return m_owner;
        }
        
        //! Retrieve the vector of the output.
        /** This function retrieves the vector of the output.
         @return The vector of the output.
         */
        inline sample* getVector() const noexcept
        {
            return m_vector;
        }
    };
    
    // ================================================================================ //
    //                                      DSP INPUT                                   //
    // ================================================================================ //
    
    //! The input manages the sample vectors of one input of a node.
    /**
     The input owns a vector of sample and manages the ownership and sharing of the vector between several dsp nodes.
     */
    class DspNode::Input
    {
    private:
        friend DspContext;
        const ulong   m_index;
        ulong         m_size;
        sample*       m_vector;
        ulong         m_nothers;
        sample**      m_others;
        DspNodeSet       m_links;
    public:
        
        //! Constructor.
        /** You should never have to call this method.
         */
        Input(const ulong index) noexcept;
        
        //! Destructor.
        /** You should never have to call this method.
         */
        ~Input();
        
        //! Add a node to the input.
        /** This adds  a node to the input.
         @param The node to add.
         */
        void addDspNode(sDspNode node) throw(bool);
        
        //! Prepare the input.
        /** This function prepare the input.
         */
        void prepare(sDspNode node) throw(DspError<DspNode>&);
        
        //! Retrieve if the links are empty.
        /** This function retrieves if the links are empty.
         @param true if if the links are empty, otherwise false.
         */
        inline bool empty() const noexcept
        {
            return m_links.empty();
        }
        
        //! Retrieve the number of links.
        /** This function retrieves the number of links.
         @param The number of links.
         */
        inline ulong size() const noexcept
        {
            return (ulong)m_links.size();
        }
        
        //! Retrieve the vector of the input.
        /** This function retrieves the vector of the input.
         @return The vector of the input.
         */
        inline sample* getVector() const noexcept
        {
            return m_vector;
        }
        
        //! Perform the copy of the links to input vector.
        /** This function perform sthe copy of the links to input vector.
         */
        inline void perform() noexcept
        {
            if(m_nothers)
            {
                Signal::vcopy(m_size, m_others[0], m_vector);
            }
            for(ulong i = 1; i < m_nothers; i++)
            {
                Signal::vadd(m_size, m_others[i], m_vector);
            }
        }
    };
    
    // ================================================================================ //
    //                                      DSP CONNECTION                              //
    // ================================================================================ //
    
    //! The dsp link owns the basic informations of a link between two node.
    /**
     The dsp link owns an input and an output node and the indices of the input and the ouput.
     */
    class DspLink
    {
    private:
        const wDspNode     m_from;
        const ulong     m_output;
        const wDspNode     m_to;
        const ulong     m_input;
    public:
        
        //! Constructor.
        /** You should never have to call this method.
         */
        DspLink(const sDspNode from, const ulong output, const sDspNode to, const ulong input) noexcept :
        m_from(from),
        m_output(output),
        m_to(to),
        m_input(input)
        {
            ;
        }
        
        //! Destructor.
        /** You should never have to call this method.
         */
        ~DspLink()
        {
            ;
        }
        
        //! Retrieve the output node.
        /** The function retrieves the output node of the link.
         @return The output node.
         */
        inline sDspNode getDspNodeFrom() const noexcept
        {
            return m_from.lock();
        }
        
        //! Retrieve the input node.
        /** The function retrieves the input node of the link.
         @return The input node.
         */
        inline sDspNode getDspNodeTo() const noexcept
        {
            return m_to.lock();
        }
        
        //! Retrieve the index of the output of the link.
        /** The function retrieves the index of the output of the link.
         @return The index of the output of the link.
         */
        inline ulong getOutputIndex() const noexcept
        {
            return m_output;
        }
        
        //! Retrieve the index of the input of the link.
        /** The function retrieves the index of the input of the link.
         @return The index of the input of the link.
         */
        inline ulong getInputIndex() const noexcept
        {
            return m_input;
        }
    };
}


#endif


