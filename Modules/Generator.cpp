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

#include "Generator.h"

namespace Kiwi
{
    namespace Dsp
    {
        Sig::Sig(const sample value) noexcept :
        m_value(value)
        {
            ;
        }
        
        Sig::~Sig()
        {
            ;
        }
        
        string Sig::getName() const noexcept
        {
            return "Sig";
        }
        
        ulong Sig::getNumberOfInputs() const noexcept
        {
            return 0;
        }
        
        ulong Sig::getNumberOfOutputs() const noexcept
        {
            return 1;
        }
        
        void Sig::prepare(sNode node) const noexcept
        {
            ;
        }
        
        void Sig::perform(scNode node) const noexcept
        {
            Signal::vfill(node->getVectorSize(), m_value, node->getOutputsSamples()[0]);
            cout << "Sig samples out : " << node->getOutputsSamples() << endl;
            for(ulong i = 0; i < node->getVectorSize(); i++)
            {
                cout << node->getOutputsSamples()[0][i] << " ";
            }
            cout << endl;
        }
        
        void Sig::release(scNode node) const noexcept
        {
            ;
        }
        
        void Sig::setValue(const sample value) noexcept
        {
            m_value = value;
        }
        
        sample Sig::getValue() const noexcept
        {
            return m_value;
        }        
    }
}

