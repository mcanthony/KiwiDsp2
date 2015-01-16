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

#include "Connection.h"

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
            friend Context;
            
            void setIndex(const ulong index);
            typedef set<weak_ptr<Node>, owner_less< weak_ptr<Node>>> NodeSet;
            
            const wcContext m_context;
            const sProcess  m_process;
            
            const ulong     m_samplerate;
            const ulong     m_vectorsize;
            const ulong     m_nins;
            sample**        m_sample_ins;
            const ulong     m_nouts;
            sample**        m_sample_outs;
            
            vector<NodeSet> m_node_ins;
            vector<NodeSet> m_node_outs;
            vector<sSignal> m_signal_ins;
            vector<sSignal> m_signal_outs;
            
            bool            m_inplace;
            bool            m_shouldprocess;
            bool            m_valid;
            ulong           m_index;

            void prepare();
            void tick() const noexcept;
            void allocSignals(sContext context);
            
            void addInput(weak_ptr<Node>, int inlet);
            void addOutput(weak_ptr<Node>, int outlet);
            
            void removeInput(weak_ptr<Node>, int inlet);
            void removeOutput(weak_ptr<Node>, int outlet);
            
            sSignal getOutputSignal(shared_ptr<Node> inputnode);
            
            void clean();
            
        public:
            
            //! The constructor.
            /** You should never use this method except if you really know what you're doing.
             */
            Node(sContext context, sProcess process);
            
            //! The destructor.
            /** You should never use this method except if you really know what you're doing.
             */
            ~Node();
            
            //! The node creator.
            /** This function create a new node.
             @param context The context owner.
             @param process The process of the node.
             */
            static inline sNode create(sContext context, sProcess process)
            {
                if(context && process)
                {
                    return make_shared<Node>(context, process);
                }
                else
                {
                    return nullptr;
                }
            }
            
            //! Retrieve the context of the node.
            /** This function retrieves context of the node.
             @return The context of the node.
             */
            inline scContext getContext() const noexcept
            {
                return m_context.lock();
            }
            
            //! Retrieve the process of the node.
            /** This function retrieves process of the node.
             @return The process of the node.
             */
            inline scProcess getProcess() const noexcept
            {
                return static_pointer_cast<const Process>(m_process);
            }
            
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
            
            //! Retrieve the number of inputs.
            /** This function retrieves the number of inputs.
             @return The number of inputs.
             */
            inline ulong getNumberOfInputs() const noexcept
            {
                return m_nins;
            }
            
            //! Retrieve the number of outputs.
            /** This function retrieves the number of outputs.
             @return The number of outputs.
             */
            inline ulong getNumberOfOutputs() const noexcept
            {
                return m_nouts;
            }
            
            //! Check if a signal inlet is connected with signal.
            /** This function checks if a signal inlet is connected with signal.
             @return True if the inlet is connected otherwise it returns false.
             */
            inline bool isInputConnected(const ulong index) const noexcept
            {
                return !m_node_ins[index].empty();
            }
            
            //! Check if a signal outlet is connected with signal.
            /** This function checks if a signal outlet is connected with signal.
             @return True if the outlet is connected otherwise it returns false.
             */
            inline bool isOutputConnected(long index) const noexcept
            {
                return !m_node_outs[index].empty();
            }
            
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
            
            ulong getIndex();
        };
        
        static bool operator<(sNode node1, sNode node2) noexcept
        {
            if(node1 && node2)
            {
                return node1->getIndex() < node2->getIndex();
            }
            else
            {
                return false;
            }
        }
    }
}


#endif


