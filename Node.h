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
            class Output;
            class Input;
            typedef shared_ptr<Output>  sOutput;
            typedef shared_ptr<Input>   sInput;
            typedef set<weak_ptr<Node>, owner_less< weak_ptr<Node>>> NodeSet;
            
            // ================================================================================ //
            //                                      DSP OUTPUT                                  //
            // ================================================================================ //
            
            //! The ouput manages the sample vectors of one ouput of a node.
            /**
             The ouput owns a vector of sample and manages the ownership and sharing of the vector between several dsp nodes.
             */
            class Output
            {
            private:
                friend Context;
                const wNode   m_node;
                const ulong   m_index;
                sample*       m_vector;
                bool          m_owner;
                NodeSet       m_connections;
                
            public:
                //! Constructor.
                /** You should never have to call this method.
                 */
                Output(sNode node, const ulong index) noexcept;
                
                //! Destructor.
                /** You should never have to call this method.
                 */
                ~Output();
                
                //! The output creator.
                /** This method create an output.
                 @param node The owner node.
                 @return The output.
                 */
                static sOutput create(sNode owner, const ulong index) noexcept
                {
                    if(owner)
                    {
                        return make_shared<Output>(owner, index);
                    }
                    else
                    {
                        return nullptr;
                    }
                }
                
                //! Add a node to the output.
                /** This function adds a node to the output.
                 @param The node to add.
                 */
                void addNode(sNode node);
                
                //! Prepare the output.
                /** This function prepare the output.
                 */
                void prepare();
                
                //! Retrieve if the connections are empty.
                /** This function retrieves if the connections are empty.
                 @param true if if the connections are empty, otherwise false.
                 */
                inline bool empty() const noexcept
                {
                    return m_connections.empty();
                }
                
                //! Retrieve the number of connections.
                /** This function retrieves the number of connections.
                 @param The number of connections.
                 */
                inline ulong size() const noexcept
                {
                    return (ulong)m_connections.size();
                }
                
                //! Retrieve if the output has a node.
                /** This function retrieves if the output has a node.
                 @param true if the output has a node, otherwise false.
                 */
                inline bool hasNode(sNode node) const
                {
                    return m_connections.find(node) != m_connections.end();
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
            class Input
            {
            private:
                friend Context;
                const wNode   m_node;
                const ulong   m_index;
                const ulong   m_size;
                sample*       m_vector;
                ulong         m_nothers;
                sample**      m_others;
                NodeSet       m_connections;
            public:
                
                //! Constructor.
                /** You should never have to call this method.
                 */
                Input(sNode node, const ulong index) noexcept;
                
                //! Destructor.
                /** You should never have to call this method.
                 */
                ~Input();
                
                //! The output creator.
                /** This method create an output.
                 @param node The owner node.
                 @return The output.
                 */
                static sInput create(sNode owner, const ulong index) noexcept
                {
                    if(owner)
                    {
                        return make_shared<Input>(owner, index);
                    }
                    else
                    {
                        return nullptr;
                    }
                }
                
                //! Add a node to the input.
                /** This adds  a node to the input.
                 @param The node to add.
                 */
                void addNode(sNode node);
                
                //! Prepare the input.
                /** This function prepare the input.
                 */
                void prepare();
                
                //! Retrieve if the connections are empty.
                /** This function retrieves if the connections are empty.
                 @param true if if the connections are empty, otherwise false.
                 */
                inline bool empty() const noexcept
                {
                    return m_connections.empty();
                }
                
                //! Retrieve the number of connections.
                /** This function retrieves the number of connections.
                 @param The number of connections.
                 */
                inline ulong size() const noexcept
                {
                    return (ulong)m_connections.size();
                }
                
                //! Retrieve the vector of the input.
                /** This function retrieves the vector of the input.
                 @return The vector of the input.
                 */
                inline sample* getVector() const noexcept
                {
                    return m_vector;
                }
                
                //! Perform the copy of the connections to input vector.
                /** This function perform sthe copy of the connections to input vector.
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
            
            const wcContext m_context;
            const sProcess  m_process;
            
            const ulong     m_samplerate;
            const ulong     m_vectorsize;
            const ulong     m_nins;
            sample** const  m_sample_ins;
            const ulong     m_nouts;
            sample** const  m_sample_outs;
            vector<sInput>  m_inputs;
            vector<sOutput> m_outputs;
            
            bool            m_inplace;
            bool            m_shouldperform;
            ulong           m_index;

            void setIndex(const ulong index); // Later figure out how to avoid this
            
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
                    sNode node = make_shared<Node>(context, process);
                    if(node)
                    {
                        for(ulong i = 0; i < node->m_nins; i++)
                        {
                            node->m_inputs.push_back(Input::create(node, i));
                        }
                        for(ulong i = 0; i < node->m_nouts; i++)
                        {
                            node->m_outputs.push_back(Output::create(node, i));
                        }
                    }
                    return node;
                }
                else
                {
                    return nullptr;
                }
            }
            
            //! Add a node to an input.
            /** This function adds a node to an input.
             @param node The node to add.
             @param index The index of the input.
             */
            void addInputNode(sNode node, const ulong index);
            
            //! Add a node to an output.
            /** This function adds a node to an output.
             @param node The node to add.
             @param index The index of the output.
             */
            void addOutputNode(sNode node, const ulong index);
            
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
            
            //! Retrieve the index of the node in the dsp chain.
            /** This function retrieves the index of the node in the dsp chain. The index is for internal uses.
             @return The index of the node.
             */
            inline ulong getIndex() const noexcept
            {
                return m_index;
            }
            
            //! Check if a signal inlet is connected with signal.
            /** This function checks if a signal inlet is connected with signal.
             @return True if the inlet is connected otherwise it returns false.
             */
            inline bool isInputConnected(const ulong index) const noexcept
            {
                return !m_inputs[index]->empty();
            }
            
            //! Check if a signal outlet is connected with signal.
            /** This function checks if a signal outlet is connected with signal.
             @return True if the outlet is connected otherwise it returns false.
             */
            inline bool isOutputConnected(long index) const noexcept
            {
                return !m_outputs[index]->empty();
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
            /** This checks if the node is valid.
             @return True if the node is valid otherwise it returns false.
             */
            inline bool shouldPerform() const noexcept
            {
                return m_shouldperform;
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
             */
            void prepare();
            
            //! Call once the process method of the inputs and of the process class.
            /** This function calls once the process method of the inputs and of the process class.
             */
            inline void tick() const
            {
                for(ulong i = 0; i < m_nins; i++)
                {
                    m_inputs[i]->perform();
                }
                m_process->perform(shared_from_this());
            }
            
            //! Notify the process that the dsp has been stopped.
            /** This function notifies the process that the dsp has been stopped.
             */
            void stop() const;
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
        
        static bool operator==(sNode node, sProcess process) noexcept
        {
            if(node && process)
            {
                return node->getProcess() == process;
            }
            else
            {
                return false;
            }
        }
    }
}


#endif


