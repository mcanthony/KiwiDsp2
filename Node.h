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
    namespace Dsp
    {
        // ================================================================================ //
        //                                      DSP NODE                                    //
        // ================================================================================ //
        
        //! The dsp nodes manages the digital signal processing of a process.
        /**
         The dsp node manages the dsp of a process and should be configure during the call of the prepare method of the process.
         */
        class Node: public enable_shared_from_this<Node>
        {
        private:
            typedef set<weak_ptr<Node>, owner_less< weak_ptr<Node>>> NodeSet;
            const sBox   m_box;
            const int               m_nins;
            const int               m_nouts;
            long                    m_vectorsize;
            double                  m_samplerate;
            
            vector<shared_ptr<Signal>> m_sig_ins;
            vector<shared_ptr<Signal>> m_sig_outs;
            
            bool                    m_inplace;
            
            bool                    m_valid;
            int                     m_index;
            
            vector<NodeSet>  m_inputs_nodes;
            vector<NodeSet>  m_outputs_nodes;
            
            void prepare(shared_ptr<DspContext> context);
            void allocSignals(shared_ptr<DspContext> context);
            
            void addInput(weak_ptr<Node>, int inlet);
            void addOutput(weak_ptr<Node>, int outlet);
            
            void removeInput(weak_ptr<Node>, int inlet);
            void removeOutput(weak_ptr<Node>, int outlet);
            
            shared_ptr<Signal> getOutputSignal(shared_ptr<Node> inputnode);
            
            void clean();
            
        public:
            
            //! The constructor.
            /** You should never use this method except if you really know what you're doing.
             */
            Node(sBox);
            
            //! The destructor.
            /** You should never use this method except if you really know what you're doing.
             */
            ~Node();
            
            //! Add a process method to the node.
            /** This function adds a process method to the node.
             @param method The process method to add.
             */
            //void    addMethod(Method method);
            
            //! Retrieve the sample rate of the node.
            /** This function retrieves the sample rate of the node.
             @return The sample rate of the node.
             */
            double  getSamplerate() const noexcept;
            
            //! Retrieve the vector size of the node.
            /** This function retrieves the vector size of the node.
             @return The vector size of the node.
             */
            long    getVectorsize() const noexcept;
            
            //! Retrieve the number of inputs.
            /** This function retrieves the number of inputs.
             @return The number of inputs.
             */
            int  getNumberOfInputs() const noexcept;
            
            //! Retrieve the number of outputs.
            /** This function retrieves the number of outputs.
             @return The number of outputs.
             */
            int    getNumberOfOutputs() const noexcept;
            
            //! Check if a signal inlet is connected with signal.
            /** This function checks if a signal inlet is connected with signal.
             @return True if the inlet is connected otherwise it returns false.
             */
            bool    isInputConnected(long index) const noexcept;
            
            //! Check if a signal outlet is connected with signal.
            /** This function checks if a signal outlet is connected with signal.
             @return True if the outlet is connected otherwise it returns false.
             */
            bool    isOutputConnected(long index) const noexcept;
            
            //! Check if the inputs and outputs signals owns the same vectors.
            /** This function checks if the signals owns the same vectors.
             @return True if the signals owns the same vectors it returns false.
             */
            bool isInplace() const noexcept;
            
            //! Set if the inputs and outputs signals owns the same vectors.
            /** This function sets if the signals owns the same vectors.
             @param status The inplace status.
             */
            void setInplace(bool status);
            
            //! Check if the node is valid.
            /** This checks if the node is valid.
             @return True if the node is valid otherwise it returns false.
             */
            bool    isValid() const noexcept;
        };
    }
}


#endif


