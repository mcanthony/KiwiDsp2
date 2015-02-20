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

#ifndef __DEF_KIWI_DSP_CHAIN__
#define __DEF_KIWI_DSP_CHAIN__

#include "DspNode.h"

// TODO :
// - Check thread safety
// - Set io vectors to the chain
// - Math and FAUST compatibility for nodes (string)
// - Clean The errors
namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP CHAIN                                   //
    // ================================================================================ //
    
    //! The dsp chain manages a set of dsp nodes.
    /**
     The dsp chain initializes a dsp chain with a set of nodes and links. To create a dsp chain, first, you should add the nodes, then add the links, then you have to compile the dsp chain.
     */
    class DspChain: public inheritable_enable_shared_from_this<DspChain>
    {
        friend DspContext;
        
    private:
        wDspContext         m_context;
        vector<sDspNode>    m_nodes;
        vector<sDspLink>    m_links;
        mutable mutex       m_mutex;
        atomic_bool         m_running;
        
        void sortNodes(set<sDspNode>& nodes, ulong& index, sDspNode node) throw(DspError&);
        
        //! Perform a tick on the dsp chain.
        /** The function calls once all the node methods of the dsp nodes.
         */
        inline void tick() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            for(vector<sDspNode>::size_type i = 0; i < m_nodes.size(); i++)
            {
                if(m_nodes[i]->isRunning())
                {
                    m_nodes[i]->tick();
                }
            }
        }
        
    public:
        
        //! The constructor.
        /** The function initialize and empty chain.
         @param context The context.
         */
        DspChain(sDspContext context) noexcept;
        
        //! The destructor.
        /** Stop the digital signal processing if needed and free the nodes.
         */
        ~DspChain();
        
        //! Retrieve the context of the chain.
        /** This function retrieves the context of the chain.
         @return The context of the chain.
         */
        inline sDspContext getContext() const noexcept
        {
            return m_context.lock();
        }
        
        //! Retrieve the device manager of the context.
        /** This function retrieves the device manager of the context.
         @return The device manager of the context.
         */
        sDspDeviceManager getDeviceManager() const noexcept;
        
        //! Retrieve the sample rate of the chain.
        /** This function retrieves the sample rate of the chain.
         @return The sample rate of the chain.
         */
        ulong getSampleRate() const noexcept;
        
        //! Retrieve the vector size of the chain.
        /** This function retrieves the vector size of the chain.
         @return The vector size of the chain.
         */
        ulong getVectorSize() const noexcept;
        
        //! Check if the chain is compiled.
        /** This function checks if the chain is compiled.
         @return True if the chain is compiled otherwise it returns false.
         */
        inline bool isRunning() const noexcept
        {
            return m_running;
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
        
        //! Add a node to the dsp chain.
        /** The function adds a node to the dsp chain.
         @param node The node to add.
         */
        void add(sDspNode node)  throw(DspError&);
        
        //! Add a link to the dsp chain.
        /** The function adds a link to the dsp chain.
         @param link The link to add.
         */
        void add(sDspLink link)  throw(DspError&);
        
        //! Remove a node from the dsp chain.
        /** The function removes a node from the dsp chain.
         @param node The node to remove.
         */
        void remove(sDspNode node)  throw(DspError&);
        
        //! Remove a link to the dsp chain.
        /** The function removes a link to the dsp chain.
         @param link The link to remove.
         */
        void remove(sDspLink link)  throw(DspError&);
        
        //! Compile the dsp chain.
        /** The function sorts the dsp nodes and call the dsp methods of the nodes.
         */
        void start() throw(DspError&);
        
        //! Stop the dsp.
        /** The function call the stop the dsp of all the nodes.
         */
        void stop();
        
        //! Suspend the dsp context.
        /** The function suspends the dsp.
         @return The state of the dsp context.
         */
        bool suspend();
        
        //! Resume the process.
        /** The function resume the process.
         @param state The state of the process.
         */
        void resume(const bool state) throw(DspError&);

    };
}


#endif


