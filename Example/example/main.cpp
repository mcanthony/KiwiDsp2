//
//  main.cpp
//  example
//
//  Created by Guillot Pierre on 28/01/2015.
//  Copyright (c) 2015 Kiwi. All rights reserved.
//

#include <iostream>

#include "../../Dsp.h"

using namespace Kiwi::Dsp;

int main(int argc, const char * argv[])
{
    sContext ctxt = Context::create(44100, 8);
    if(ctxt)
    {
        cout << "Context" << endl << "{" << endl;
        cout << "   Samplerate : " << ctxt->getSampleRate() << endl;
        cout << "   Vector : " << ctxt->getVectorSize() << endl;
        
        sProcess pSig = make_shared<Sig>(1.1);
        sProcess pAdd = make_shared<Plus<Scalar>>(1.2);
        sProcess pvAdd = make_shared<Plus<Vector>>();
        sProcess pvAdd2 = make_shared<Plus<Vector>>();
        sConnection cnect1 = make_shared<Connection>(pSig, 0, pAdd, 0);
        sConnection cnect2 = make_shared<Connection>(pAdd, 0, pvAdd, 0);
        sConnection cnect3 = make_shared<Connection>(pSig, 0, pvAdd, 1);
        sConnection cnect4 = make_shared<Connection>(pvAdd, 0, pAdd, 0);
        
        if(pSig && pAdd && pvAdd && cnect1 && cnect2 && cnect3)
        {
            try
            {
                ctxt->add(pSig);
                ctxt->add(pAdd);
                ctxt->add(pvAdd);
            }
            catch(exception& e)
            {
                cout << "Error with compilation : " << e.what() << endl;
                cout << "}" << endl;
                return 1;
            }
            
            try
            {
                //ctxt->add(cnect4);
                ctxt->add(cnect1);
                ctxt->add(cnect2);
                ctxt->add(cnect3);
            }
            catch(exception& e)
            {
                cout << "Error with compilation : " << e.what() << endl;
                cout << "}" << endl;
                return 1;
            }
            
            try
            {
                ctxt->compile();
            }
            catch(exception& e)
            {
                cout << "Error with compilation : " << e.what() << endl;
                cout << "}" << endl;
                return 1;
            }
            
            // Tester les threads !
            
            cout << "   Number of nodes : " << ctxt->getNumberOfNodes() << endl;
            ctxt->tick();
            shared_ptr<Plus<Scalar>> addd = dynamic_pointer_cast<Plus<Scalar>>(pAdd);
            addd->setValue(1.3);
            ctxt->tick();
        }
        
        cout << "}" << endl;
    }
    
    return 0;
}
