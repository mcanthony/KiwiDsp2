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

#include "Math.h"

namespace Kiwi
{
    namespace Dsp
    {
        // ================================================================================ //
        //                                      PLUS                                        //
        // ================================================================================ //
        
        // ================================================================================ //
        //                                      SCALAR                                      //
        // ================================================================================ //
        
        Plus<Scalar>::Plus(const sample value) noexcept :
        m_value(value)
        {
            ;
        }
        
        Plus<Scalar>::~Plus()
        {
            ;
        }
        
        string Plus<Scalar>::getName() const noexcept
        {
            return "Plus (scalar)";
        }
        
        ulong Plus<Scalar>::getNumberOfInputs() const noexcept
        {
            return 1;
        }
        
        ulong Plus<Scalar>::getNumberOfOutputs() const noexcept
        {
            return 1;
        }
        
        void Plus<Scalar>::prepare(sNode node) const noexcept
        {
            ;
        }
        
        void Plus<Scalar>::perform(scNode node) const noexcept
        {
            Signal::vsadd(node->getVectorSize(), m_value, node->getOutputsSamples()[0]);
            cout << "Add samples out : " << node->getOutputsSamples()[0] << endl;
            for(ulong i = 0; i < node->getVectorSize(); i++)
            {
                cout << node->getOutputsSamples()[0][i] << " ";
            }
            cout << endl;
        }
        
        void Plus<Scalar>::release(scNode node) const noexcept
        {
            ;
        }
        
        void Plus<Scalar>::setValue(const sample value) noexcept
        {
            m_value = value;
        }
        
        sample Plus<Scalar>::getValue() const noexcept
        {
            return m_value;
        }
        
        // ================================================================================ //
        //                                      VECTOR                                      //
        // ================================================================================ //
        
        Plus<Vector>::Plus() noexcept
        {
            ;
        }
        
        Plus<Vector>::~Plus()
        {
            ;
        }
            
        string Plus<Vector>::getName() const noexcept
        {
            return "Plus (vector)";
        }
        
        ulong Plus<Vector>::getNumberOfInputs() const noexcept
        {
            return 2;
        }
        
        ulong Plus<Vector>::getNumberOfOutputs() const noexcept
        {
            return 1;
        }
        
        void Plus<Vector>::prepare(sNode node) const noexcept
        {
            ;
        }
        
        void Plus<Vector>::perform(scNode node) const noexcept
        {
            Signal::vadd(node->getVectorSize(), node->getInputsSamples()[1], node->getOutputsSamples()[0]);
            cout << "Add samples out : " << node->getOutputsSamples()[0] << endl;
            for(ulong i = 0; i < node->getVectorSize(); i++)
            {
                cout << node->getOutputsSamples()[0][i] << " ";
            }
            cout << endl;
        }
        
        void Plus<Vector>::release(scNode node) const noexcept
        {
            ;
        }
    }
}

