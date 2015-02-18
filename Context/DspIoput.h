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

#ifndef __DEF_KIWI_DSP_IOPUT__
#define __DEF_KIWI_DSP_IOPUT__

#include "DspError.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP OUTPUT                                  //
    // ================================================================================ //
    
    //! The ouput manages the sample vectors of one ouput of a node.
    /**
     The ouput owns a vector of sample and manages the ownership and sharing of the vector between several dsp nodes.
     */
    class DspOutput
    {
    private:
        friend DspChain;
        const ulong   m_index;
        sample*       m_vector;
        bool          m_owner;
        DspNodeSet    m_links;
        
    public:
        //! Constructor.
        /** You should never have to call this method.
         */
        DspOutput(const ulong index) noexcept;
        
        //! Destructor.
        /** You should never have to call this method.
         */
        ~DspOutput();
        
        //! Add a node to the output.
        /** This function adds a node to the output.
         @param The node to add.
         */
        void add(sDspNode node);
        
        //! Remove a node from the output.
        /** This function removes a node from the output.
         @param The node to remove.
         */
        void remove(sDspNode node);
        
        //! Remove all the nodes from the output.
        /** This function removes all the nodes from the output.
         */
        void clear();
        
        //! Prepare the output.
        /** This function prepare the output.
         @param The owner node.
         */
        void start(sDspNode node) throw(DspError&);
        
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
        inline bool hasNode(sDspNode node) const
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
    class DspInput
    {
    private:
        friend DspChain;
        const ulong   m_index;
        ulong         m_size;
        sample*       m_vector;
        ulong         m_nothers;
        sample**      m_others;
        DspNodeSet    m_links;
    public:
        
        //! Constructor.
        /** You should never have to call this method.
         */
        DspInput(const ulong index) noexcept;
        
        //! Destructor.
        /** You should never have to call this method.
         */
        ~DspInput();
        
        //! Add a node to the input.
        /** This function adds  a node to the input.
         @param The node to add.
         */
        void add(sDspNode node);
        
        //! Remove a node from the input.
        /** This function removes  a node from the input.
         @param The node to remove.
         */
        void remove(sDspNode node);
        
        //! Remove all the nodes from the input.
        /** This function removes all the nodes from the input.
         */
        void clear();
        
        //! Prepare the input.
        /** This function prepare the input.
         */
        void start(sDspNode node) throw(DspError&);
        
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
    //                                      DSP LINK                                    //
    // ================================================================================ //
    
    //! The dsp link owns the basic informations of a link between two node.
    /**
     The dsp link owns an input and an output node and the indices of the input and the ouput.
     */
    class DspLink
    {
    private:
        const wDspChain m_chain;
        const wDspNode  m_from;
        const ulong     m_output;
        const wDspNode  m_to;
        const ulong     m_input;
    public:
        
        //! Constructor.
        /** You should never have to call this method.
         */
        DspLink(const sDspChain chain, const sDspNode from, const ulong output, const sDspNode to, const ulong input) noexcept;
        
        //! Destructor.
        /** You should never have to call this method.
         */
        ~DspLink();
        
        //! Retrieve the chain.
        /** The function retrieves the chain of the link.
         @return The chain.
         */
        inline sDspChain getChain() const noexcept
        {
            return m_chain.lock();
        }
        
        //! Retrieve the output node.
        /** The function retrieves the output node of the link.
         @return The output node.
         */
        inline sDspNode getOutpuNode() const noexcept
        {
            return m_from.lock();
        }
        
        //! Retrieve the input node.
        /** The function retrieves the input node of the link.
         @return The input node.
         */
        inline sDspNode getInputNode() const noexcept
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
        
        //! Retrieve if the link is valid.
        /** The function retrieves if the link is valid.
         @return true if the link is valid, otherwise false.
         */
        bool isValid() const noexcept;
        
        //! Prepare the link to process.
        /** This function link the node to process. It connects the output node to the input node.
         */
        void start() const noexcept;
    };
}


#endif


