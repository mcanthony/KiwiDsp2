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

#include "Node.h"

namespace Kiwi
{
    namespace Dsp
    {
        // ================================================================================ //
        //                                      DSP CONTEXT                                 //
        // ================================================================================ //
        
        //! The dsp context manages a set of dsp nodes and builds the dsp chain.
        /**
         The dsp context initializes a dsp chain with a set of processes and connections. To create a dsp chain, first, you should add the processes, then add the connections, then you have to compile the dsp context.
         */
        class Context: public enable_shared_from_this<Context>
        {
        private:
            const ulong       m_samplerate;
            const ulong       m_vectorsize;
            vector<sNode>     m_nodes;
            mutable mutex     m_mutex;
            
            void sortNodes(set<sNode>& nodes, ulong& index, sNode node);
        public:
            
            //! The constructor.
            /** You should never use this method except if you really know what you're doing.
             */
            Context(const ulong samplerate, const ulong vectorsize) noexcept;
            
            //! The destructor.
            /** You should never use this method except if you really know what you're doing.
             */
            ~Context();
            
            //! Context creator.
            /** This function create a new context.
             @param samplerate    The sample rate.
             @param vectorsize    The vector size.
             */
            static inline sContext create(const ulong samplerate, const ulong vectorsize)
            {
                return make_shared<Context>(samplerate, vectorsize);
            }
            
            //! Retrieve the sample rate of the context.
            /** This function retrieves the sample rate of the context.
             @return The sample rate of the context.
             */
            inline ulong getSampleRate() const noexcept
            {
                return m_samplerate;
            }
            
            //! Retrieve the vector size of the context.
            /** This function retrieves the vector size of the context.
             @return The vector size of the context.
             */
            inline ulong getVectorSize() const noexcept
            {
                return m_vectorsize;
            }
            
            //! Retrieve the number of nodes.
            /** The function retrieves the number of nodes.
             @return The number of nodes.
             */
            inline ulong getNumberOfNodes() const noexcept
            {
                lock_guard<mutex> guard(m_mutex);
                return (ulong)m_nodes.size();
            }
            
            //! Add a process to the dsp context.
            /** The function adds a process to the dsp context.
             @param process The process to add.
             */
            void add(sProcess process);
            
            //! Add a connection to the dsp context.
            /** The function adds a connection to the dsp context.
             @param connection The connection to add.
             */
            void add(sConnection connection);
            
            //! Compile the dsp context.
            /** The function sorts the dsp nodes and call the dsp methods of the processes.
             */
            void compile();
            
            //! Perform a tick on the dsp context.
            /** The function calls once all the process methods of the dsp nodes.
             */
            inline void tick() const
            {
                lock_guard<mutex> guard(m_mutex);
                for(vector<sNode>::size_type i = 0; i < m_nodes.size(); i++)
                {
                    m_nodes[i]->tick();
                }
            }
        };
    }
}


#endif


