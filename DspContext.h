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

#include "DspNode.h"

// TODO :
// - Think about exception
// - Check thread safety
// - Set io vectors to the context
// - Math and FAUST compatibility for nodes (string)
// - Allows to create node without context ?
namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP CONTEXT                                 //
    // ================================================================================ //
    
    //! The dsp context manages a set of dsp nodes and builds the dsp chain.
    /**
     The dsp context initializes a dsp chain with a set of nodes and links. To create a dsp chain, first, you should add the nodes, then add the links, then you have to compile the dsp context.
     */
    class DspContext: public enable_shared_from_this<DspContext>
    {
    private:
        vector<sDspNode>   m_nodes;
        mutable mutex   m_mutex;
        ulong           m_samplerate;
        ulong           m_vectorsize;
        atomic_bool     m_compiled;
        
        void sortDspNodes(set<sDspNode>& nodes, sDspNode node) throw(DspError<DspNode>&);
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you're doing.
         */
        DspContext() noexcept;
        
        //! The destructor.
        /** You should never use this method except if you really know what you're doing.
         */
        ~DspContext();
        
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
        
        //! Check if the context is compiled.
        /** This function checks if the context is compiled.
         @return True if the context is compiled otherwise it returns false.
         */
        inline bool isCompiled() const noexcept
        {
            return m_compiled;
        }
        
        //! Retrieve the number of nodes.
        /** The function retrieves the number of nodes.
         @return The number of nodes.
         */
        inline ulong getNumberOfDspNodes() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return (ulong)m_nodes.size();
        }
        
        //! Add a node to the dsp context.
        /** The function adds a node to the dsp context.
         @param node The node to add.
         */
        void add(sDspNode node) throw(DspError<DspNode>&);
        
        //! Add a link to the dsp context.
        /** The function adds a link to the dsp context.
         @param link The link to add.
         */
        void add(sDspLink link) throw(DspError<DspLink>&);
        
        //! Compile the dsp context.
        /** The function sorts the dsp nodes and call the dsp methods of the nodes.
         */
        void compile(const ulong samplerate, const ulong vectorsize) throw(DspError<DspNode>&);
        
        //! Perform a tick on the dsp context.
        /** The function calls once all the node methods of the dsp nodes.
         */
        inline void tick() const
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<sDspNode>::size_type i = 0; i < m_nodes.size(); i++)
            {
                m_nodes[i]->tick();
            }
        }
        
        //! Stop the dsp.
        /** The function call the stop the dsp of all the nodes.
         */
        void stop();
    };
}


#endif


