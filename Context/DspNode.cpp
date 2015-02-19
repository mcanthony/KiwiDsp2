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
#include "DspChain.h"
#include "DspContext.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      DSP NODE                                    //
    // ================================================================================ //
    
    DspNode::DspNode(sDspChain chain, const ulong nins, const ulong nouts) noexcept :
    m_chain(chain),
    m_nins(nins),
    m_sample_ins(new sample*[m_nins]),
    m_nouts(nouts),
    m_sample_outs(new sample*[m_nouts]),
    m_samplerate(0),
    m_vectorsize(0),
    m_inplace(true),
    m_running(false)
    {
        for(ulong i = 0; i < getNumberOfInputs(); i++)
        {
            m_inputs.push_back(make_shared<DspInput>(i));
        }
        for(ulong i = 0; i < getNumberOfOutputs(); i++)
        {
            m_outputs.push_back(make_shared<DspOutput>(i));
        }
    }
    
    DspNode::~DspNode()
    {
        delete [] m_sample_ins;
        delete [] m_sample_outs;
        m_inputs.clear();
        m_outputs.clear();
    }
    
    sDspContext DspNode::getContext() const noexcept
    {
        sDspChain chain = getChain();
        if(chain)
        {
            return chain->getContext();
        }
        else
        {
            return nullptr;
        }
    }
    
    sDspDeviceManager DspNode::getDeviceManager() const noexcept
    {
        sDspContext context = getContext();
        if(context)
        {
            return context->getDeviceManager();
        }
        else
        {
            return nullptr;
        }
    }
    
    void DspNode::addInput(sDspNode node, const ulong index)
    {
        if(index < (ulong)m_inputs.size())
        {
            m_inputs[index]->add(node);
        }
    }
    
    void DspNode::addOutput(sDspNode node, const ulong index)
    {
        if(index < (ulong)m_outputs.size())
        {
            m_outputs[index]->add(node);
        }
    }
    
    void DspNode::removeInput(sDspNode node, const ulong index)
    {
        if(index < (ulong)m_outputs.size())
        {
            m_inputs[index]->remove(node);
        }
    }
    
    void DspNode::removeOutput(sDspNode node, const ulong index)
    {
        if(index < (ulong)m_outputs.size())
        {
            m_outputs[index]->remove(node);
        }
    }
    
    bool DspNode::isInputConnected(const ulong index) const noexcept
    {
        return !m_inputs[index]->empty();
    }
    
    bool DspNode::isOutputConnected(const ulong index) const noexcept
    {
        return !m_outputs[index]->empty();
    }
    
    void DspNode::setInplace(const bool status) noexcept
    {
        m_inplace = status;
    }
    
    void DspNode::shouldPerform(const bool status) noexcept
    {
        m_running = status;
    }
    
    void DspNode::start() throw(DspError&)
    {
        sDspChain chain = getChain();
        if(chain)
        {
            if(m_running)
            {
                stop();
            }

            m_samplerate = chain->getSampleRate();
            m_vectorsize = chain->getVectorSize();
            
            prepare();
            
            if(m_running)
            {
                for(ulong i = 0; i < getNumberOfInputs(); i++)
                {
                    try
                    {
                        m_inputs[i]->start(shared_from_this());
                    }
                    catch(DspError& e)
                    {
                        m_running = false;
                        throw e;
                    }
                    m_sample_ins[i] = m_inputs[i]->getVector();
                }
                for(ulong i = 0; i < getNumberOfOutputs(); i++)
                {
                    try
                    {
                        m_outputs[i]->start(shared_from_this());
                    }
                    catch(DspError& e)
                    {
                        m_running = false;
                        throw e;
                    }
                    
                    m_sample_outs[i] = m_outputs[i]->getVector();
                }
            }
        }
    }
    
    void DspNode::stop()
    {
        m_running = false;
        release();
        for(ulong i = 0; i < getNumberOfInputs(); i++)
        {
            m_inputs[i]->clear();
        }
        for(ulong i = 0; i < getNumberOfOutputs(); i++)
        {
            m_outputs[i]->clear();
        }
    }
}
















