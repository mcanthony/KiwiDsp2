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

#include "DspNode.h"
#include "DspContext.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP OUTPUT                                  //
    // ================================================================================ //
    
    DspNode::Output::Output(const ulong index) noexcept :
    m_index(index),
    m_vector(nullptr),
    m_owner(false)
    {
        
    }
    
    DspNode::Output::~Output()
    {
        if(m_owner && m_vector)
        {
            delete [] m_vector;
        }
        m_links.clear();
    }
    
    void DspNode::Output::addDspNode(sDspNode node) throw(bool)
    {
        if(!m_links.insert(node).second)
        {
            throw false;
        }
    }
    
    void DspNode::Output::prepare(sDspNode node) throw(DspError<DspNode>&)
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
                    throw DspError<DspNode>(node, node, DspError<DspNode>::Inplace);
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
                    throw DspError<DspNode>(node, node, DspError<DspNode>::Alloc);
                }
                
            }
        }
    }
    
    // ================================================================================ //
    //                                      DSP INPUT                                   //
    // ================================================================================ //
    
    DspNode::Input::Input(const ulong index) noexcept :
    m_index(index),
    m_vector(nullptr),
    m_nothers(0),
    m_others(nullptr)
    {
        
    }
    
    DspNode::Input::~Input()
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
    
    void DspNode::Input::addDspNode(sDspNode node) throw(bool)
    {
        if(!m_links.insert(node).second)
        {
            throw false;
        }
    }
    
    void DspNode::Input::prepare(sDspNode node) throw(DspError<DspNode>&)
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
                    if(in->getSampleRate() != node->getSampleRate())
                    {
                        throw DspError<DspNode>(node, in, DspError<DspNode>::SampleRate);
                    }
                    else if(in->getVectorSize() != node->getVectorSize())
                    {
                        throw DspError<DspNode>(node, in, DspError<DspNode>::VectorSize);
                    }
                    else
                    {
                        sOutput output = nullptr;
                        for(vector<DspNodeSet>::size_type i = 0; i < in->m_outputs.size(); i++)
                        {
                            if(in->m_outputs[i]->hasDspNode(node))
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
                            throw DspError<DspNode>(node, in, DspError<DspNode>::Recopy);
                        }
                    }
                }
            }
            try
            {
                m_vector    = new sample[node->getVectorSize()];
            }
            catch(bad_alloc& e)
            {
                throw DspError<DspNode>(node, node, DspError<DspNode>::Alloc);
            }
            
        }
        
    }
    
    // ================================================================================ //
    //                                      DSP NODE                                    //
    // ================================================================================ //
    
    DspNode::DspNode(const ulong nins, const ulong nouts) noexcept :
    m_nins(nins),
    m_sample_ins(new sample*[m_nins]),
    m_nouts(nouts),
    m_sample_outs(new sample*[m_nouts]),
    m_samplerate(0),
    m_vectorsize(0),
    m_inplace(true),
    m_shouldperform(false),
    m_compiled(false)
    {
        for(ulong i = 0; i < getNumberOfInputs(); i++)
        {
            m_inputs.push_back(make_shared<Input>(i));
        }
        for(ulong i = 0; i < getNumberOfOutputs(); i++)
        {
            m_outputs.push_back(make_shared<Output>(i));
        }
    }
    
    DspNode::~DspNode()
    {
        delete [] m_sample_ins;
        delete [] m_sample_outs;
        m_inputs.clear();
        m_outputs.clear();
    }
    
    void DspNode::addInputDspNode(sDspNode node, const ulong index) throw(bool)
    {
        if(index < (ulong)m_inputs.size())
        {
            try
            {
                m_inputs[index]->addDspNode(node);
            }
            catch(bool)
            {
                throw false;
            }
        }
        else
        {
            throw true;
        }
    }
    
    void DspNode::addOutputDspNode(sDspNode node, const ulong index) throw(bool)
    {
        if(index < (ulong)m_outputs.size())
        {
            try
            {
                m_outputs[index]->addDspNode(node);
            }
            catch(bool)
            {
                throw false;
            }
        }
        else
        {
            throw true;
        }
    }
    
    bool DspNode::isInputConnected(const ulong index) const noexcept
    {
        return !m_inputs[index]->empty();
    }
    
    bool DspNode::isOutputConnected(long index) const noexcept
    {
        return !m_outputs[index]->empty();
    }
    
    void DspNode::setInplace(const bool status) noexcept
    {
        m_inplace = status;
    }
    
    void DspNode::shouldPerform(const bool status) noexcept
    {
        m_shouldperform = status;
    }
    
    void DspNode::compile(scDspContext context) throw(DspError<DspNode>&)
    {
        if(context)
        {
            m_shouldperform = true;
            m_samplerate = context->getSampleRate();
            m_vectorsize = context->getVectorSize();
            
            prepare();
            
            if(shouldPerform())
            {
                for(ulong i = 0; i < getNumberOfInputs(); i++)
                {
                    try
                    {
                        m_inputs[i]->prepare(shared_from_this());
                    }
                    catch(DspError<DspNode>& e)
                    {
                        throw e;
                    }
                    m_sample_ins[i] = m_inputs[i]->getVector();
                }
                for(ulong i = 0; i < getNumberOfOutputs(); i++)
                {
                    try
                    {
                        m_outputs[i]->prepare(shared_from_this());
                    }
                    catch(DspError<DspNode>& e)
                    {
                        throw e;
                    }
                    
                    m_sample_outs[i] = m_outputs[i]->getVector();
                }
            }
            
            m_compiled = true;
        }
    }
    
    void DspNode::tick() const
    {
        for(ulong i = 0; i < getNumberOfInputs(); i++)
        {
            m_inputs[i]->perform();
        }
        perform();
    }
    
    void DspNode::stop()
    {
        release();
        m_shouldperform = false;
        m_compiled      = false;
    }

}
















