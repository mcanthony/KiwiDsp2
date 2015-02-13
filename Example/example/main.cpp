//
//  main.cpp
//  example
//
//  Created by Guillot Pierre on 28/01/2015.
//  Copyright (c) 2015 Kiwi. All rights reserved.
//

#include <iostream>

#include "../../Dsp.h"

using namespace Kiwi;

int main(int argc, const char * argv[])
{
    sDspContext ctxt = make_shared<DspContext>();
    if(ctxt)
    {
        sDspNode pSig = make_shared<DspSig>(1.1);
        sDspNode pAdd = make_shared<DspPlus<Scalar>>(1.2);
        sDspNode pvAdd = make_shared<DspPlus<Vector>>();
        sDspNode pvAdd2 = make_shared<DspPlus<Vector>>();
        sDspNode pNoise = make_shared<DspNoise>();
        sDspLink cnect1 = make_shared<DspLink>(pSig, 0, pAdd, 0);
        sDspLink cnect2 = make_shared<DspLink>(pAdd, 0, pvAdd, 0);
        sDspLink cnect3 = make_shared<DspLink>(pSig, 0, pvAdd, 1);
        sDspLink cnect4 = make_shared<DspLink>(pvAdd, 0, pvAdd2, 0);
        sDspLink cnect5 = make_shared<DspLink>(pNoise, 0, pvAdd2, 0);
        
        if(pSig && pAdd && pvAdd && cnect1 && cnect2 && cnect3)
        {
            try
            {
                ctxt->add(pSig);
                ctxt->add(pAdd);
                ctxt->add(pvAdd);
                ctxt->add(pvAdd2);
                ctxt->add(pNoise);
            }
            catch(exception& e)
            {
                cout << "DspError with compilation : " << e.what() << endl;
                return 1;
            }
            
            try
            {
                ctxt->add(cnect4);
                ctxt->add(cnect1);
                ctxt->add(cnect2);
                ctxt->add(cnect3);
                ctxt->add(cnect5);
            }
            catch(exception& e)
            {
                cout << "DspError with compilation : " << e.what() << endl;
                return 1;
            }
            
            try
            {
                ctxt->compile(44100, 64);
            }
            catch(exception& e)
            {
                cout << "DspError with compilation : " << e.what() << endl;
                return 1;
            }
            
            // Tester les threads !
            
            cout << "Number of nodes : " << ctxt->getNumberOfDspNodes() << endl;
            cout << "Tick" << endl;
            ctxt->tick();
            shared_ptr<DspPlus<Scalar>> addd = dynamic_pointer_cast<DspPlus<Scalar>>(pAdd);
            addd->setValue(1.3);
            cout << "Tick" << endl;
            ctxt->tick();
        }
    }
    
    return 0;
}
