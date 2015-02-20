//
//  main.cpp
//  PortAudioExample
//
//  Created by Guillot Pierre on 19/02/2015.
//  Copyright (c) 2015 Kiwi. All rights reserved.
//

#include "../../../Implementation/DspPortAudio.h"

using namespace Kiwi;

int main(int argc, const char * argv[])
{
    shared_ptr<PortAudioDeviceManager> manager = make_shared<PortAudioDeviceManager>();
    if(manager)
    {
        manager->setSampleRate(44100);
        manager->setVectorSize(64);
        
        vector<string> names;
        cout << "Available Drivers : ";
        manager->getAvailableDrivers(names);
        for(ulong i = 0; i < names.size(); i++)
        {
            cout << names[i] << ", ";
        }
        
        cout << endl << "Available Input Devices : ";
        manager->getAvailableInputDevices(names);
        for(ulong i = 0; i < names.size(); i++)
        {
            cout << names[i] << ", ";
        }
        
        cout << endl << "Available Output Devices : ";
        manager->getAvailableOutputDevices(names);
        for(ulong i = 0; i < names.size(); i++)
        {
            cout << names[i] << ", ";
        }
        vector<ulong> sizes;
        cout << endl << "Available SampleRate : ";
        manager->getAvailableSampleRates(sizes);
        for(ulong i = 0; i < sizes.size(); i++)
        {
            cout << sizes[i] << ", ";
        }
        
        cout << endl << "Available VectorSize : ";
        manager->getAvailableVectorSizes(sizes);
        for(ulong i = 0; i < sizes.size(); i++)
        {
            cout << sizes[i] << ", ";
        }
        
        cout << endl << endl << "Current configuration : " << endl;
        cout << "Driver : " << manager->getDriverName() << endl;
        cout << "Devices : " << manager->getInputDeviceName() << ", " << manager->getOutputDeviceName() << endl;
        cout << "Ioputs : " << manager->getNumberOfInputs() << ", " << manager->getNumberOfOutputs() << endl;
        cout << "Sr & Vs : " << manager->getSampleRate() << ", " << manager->getVectorSize() << endl << endl;
        
        sDspContext ctxt = make_shared<DspContext>(manager);
        if(ctxt)
        {
            try
            {
                ctxt->start();
            }
            catch (exception& e)
            {
                cout << e.what() << endl;
            }
            sDspChain chain = make_shared<DspChain>(ctxt);
            if(chain)
            {
                ctxt->add(chain);
                sDspNode Noise = make_shared<DspNoise>(chain);
                sDspNode Dac = make_shared<DspDac>(chain, vector<ulong>{(ulong)1, (ulong)2});
                sDspLink cnectLeft = make_shared<DspLink>(chain, Noise, 0, Dac, 0);
                sDspLink cnectRight = make_shared<DspLink>(chain, Noise, 0, Dac, 1);
                chain->add(Noise);
                chain->add(Dac);
                chain->start();
                chain->add(cnectRight);
                
                cout << "CPU : ";
                int zaza = 20;
                while (zaza--)
                {
                    
                    try
                    {
                        chain->remove(cnectRight);
                    }
                    catch (exception& e)
                    {
                        cout << e.what() << endl;
                    }
                    
                    try
                    {
                        chain->add(cnectLeft);
                    }
                    catch (exception& e)
                    {
                        cout << e.what() << endl;
                    }
                    
                    this_thread::sleep_for(chrono::milliseconds(100));
                    
                    try
                    {
                        chain->remove(cnectLeft);
                    }
                    catch (exception& e)
                    {
                        cout << e.what() << endl;
                    }
                    
                    try
                    {
                        chain->add(cnectRight);
                    }
                    catch (exception& e)
                    {
                        cout << e.what() << endl;
                    }
                    
                    this_thread::sleep_for(chrono::milliseconds(100));
                    cout << ctxt->getCPU() << ", ";
                    
                    if(zaza == 10)
                    {
                        manager->setSampleRate(48000);
                        manager->setVectorSize(128);
                        cout << endl << "Sr & Vs : " << manager->getSampleRate() << ", " << manager->getVectorSize() << endl;
                        cout << "CPU : ";
                    }
                }
            }
        }
    }
    
    return 0;
}
