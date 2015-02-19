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

#include "DspIoput.h"
#include "DspNode.h"
#include "DspChain.h"
#include "DspContext.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP OUTPUT                                  //
    // ================================================================================ //
    
    DspOutput::DspOutput(const ulong index) noexcept :
    m_index(index),
    m_vector(nullptr),
    m_owner(false)
    {
        
    }
    
    DspOutput::~DspOutput()
    {
        if(m_owner && m_vector)
        {
            delete [] m_vector;
        }
        m_links.clear();
    }
    
    void DspOutput::add(sDspNode node)
    {
        m_links.insert(node);
    }
    
    void DspOutput::remove(sDspNode node)
    {
        m_links.erase(node);
    }
    
    void DspOutput::clear()
    {
        m_links.clear();
        if(m_owner && m_vector)
        {
            delete [] m_vector;
            m_vector = nullptr;
        }
        m_owner     = false;
    }
    
    void DspOutput::start(sDspNode node) throw(DspError&)
    {
        if(m_owner && m_vector)
        {
            delete [] m_vector;
            m_vector = nullptr;
        }
        m_owner     = false;
        
        if(node)
        {
            if(node->isInplace() && node->getNumberOfInputs() > m_index)
            {
                m_vector = node->m_inputs[m_index]->getVector();
                if(!m_vector)
                {
                    throw DspError(node, DspError::Inplace);
                }
            }
            if(!m_vector)
            {
                m_owner     = true;
                try
                {
                    m_vector    = new sample[node->getVectorSize()];
                }
                catch(bad_alloc& e)
                {
                    throw DspError(node, DspError::Alloc);
                }
                Signal::vclear(node->getVectorSize(), m_vector);
            }
        }
    }
    
    // ================================================================================ //
    //                                      DSP INPUT                                   //
    // ================================================================================ //
    
    DspInput::DspInput(const ulong index) noexcept :
    m_index(index),
    m_vector(nullptr),
    m_nothers(0),
    m_others(nullptr)
    {
        
    }
    
    DspInput::~DspInput()
    {
        if(m_nothers && m_others)
        {
            delete [] m_others;
            m_others = nullptr;
        }
        if(m_vector)
        {
            delete [] m_vector;
            m_vector = nullptr;
        }
        m_links.clear();
    }
    
    void DspInput::add(sDspNode node)
    {
        m_links.insert(node);
    }
    
    void DspInput::remove(sDspNode node)
    {
        m_links.erase(node);
    }
    
    void DspInput::clear()
    {
        m_links.clear();
        if(m_vector)
        {
            delete [] m_vector;
            m_vector = nullptr;
        }
        if(m_nothers && m_others)
        {
            delete [] m_others;
            m_others = nullptr;
        }
        m_nothers   = 0;
    }
    
    void DspInput::start(sDspNode node) throw(DspError&)
    {
        if(m_vector)
        {
            delete [] m_vector;
            m_vector = nullptr;
        }
        if(m_nothers && m_others)
        {
            delete [] m_others;
            m_others = nullptr;
        }
        m_nothers   = 0;
        
        if(node)
        {
            m_size = node->getVectorSize();
            for(auto it = m_links.begin(); it != m_links.end(); )
            {
                sDspNode in = (*it).lock();
                if(in)
                {
                    ++it;
                }
                else
                {
                    it = m_links.erase(it);
                }
            }
            m_nothers = m_links.size();
            m_others  = new sample*[m_nothers];
            ulong inc = 0;
            for(auto it = m_links.begin(); it != m_links.end(); ++it)
            {
                sDspNode in = (*it).lock();
                if(in)
                {
                    sDspOutput output = nullptr;
                    for(vector<DspNodeSet>::size_type i = 0; i < in->m_outputs.size(); i++)
                    {
                        if(in->m_outputs[i]->hasNode(node))
                        {
                            output = in->m_outputs[i];
                            break;
                        }
                    }
                    if(output)
                    {
                        m_others[inc++] = output->getVector();
                    }
                    else
                    {
                        throw DspError(node, DspError::Recopy);
                    }
                }
            }
            try
            {
                m_vector    = new sample[node->getVectorSize()];
            }
            catch(bad_alloc& e)
            {
                throw DspError(node, DspError::Alloc);
            }
            Signal::vclear(node->getVectorSize(), m_vector);
        }
    }
    
    // ================================================================================ //
    //                                      DSP LINK                                    //
    // ================================================================================ //
    
    DspLink::DspLink(const sDspChain chain, const sDspNode from, const ulong output, const sDspNode to, const ulong input) noexcept :
    m_chain(chain),
    m_from(from),
    m_output(output),
    m_to(to),
    m_input(input)
    {
        ;
    }
    
    DspLink::~DspLink()
    {
        
    }
    
    bool DspLink::isValid() const noexcept
    {
        sDspChain chain = getChain();
        sDspNode  from  = getOutpuNode();
        sDspNode  to    = getInputNode();
        if(chain && from && to && from != to)
        {
            return getOutputIndex() < from->getNumberOfOutputs() && getInputIndex() < to->getNumberOfInputs();
        }
        else
        {
            return false;
        }
    }
    
    void DspLink::start() const noexcept
    {
        sDspChain chain = getChain();
        sDspNode  from  = getOutpuNode();
        sDspNode  to    = getInputNode();
        if(chain && from && to && from != to)
        {
            from->addOutput(to, getOutputIndex());
            to->addInput(from, getInputIndex());
        }
    }
}
















