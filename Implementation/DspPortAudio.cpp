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

#include "DspPortAudio.h"

namespace Kiwi
{
    ulong PortAudioDeviceManager::m_nmanagers = 0;
    
    PortAudioDeviceManager::DeviceNode::DeviceNode(PortAudioDeviceManager* _device) :
    device(_device),
    nins(_device->m_ninputs),
    inputs(_device->m_sample_ins),
    nouts(_device->m_noutputs),
    outputs(_device->m_sample_outs),
    samplerate(_device->m_samplerate),
    vectorsize(_device->m_vectorsize)
    {
        ;
    }
    
    PortAudioDeviceManager::PortAudioDeviceManager() :
    m_stream(nullptr),
    m_sample_ins(nullptr),
    m_sample_outs(nullptr)
    {
        lock_guard<mutex> guard(m_mutex);
        if(!m_nmanagers)
        {
            PaError err = Pa_Initialize();
            if(err != paNoError)
            {
                cout << "PortAudio error: %s\n" << Pa_GetErrorText(err) << endl;
            }
        }
        m_nmanagers++;
        m_driver = Pa_GetDefaultHostApi();
        m_device_input = Pa_GetDefaultInputDevice();
        m_device_output = Pa_GetDefaultOutputDevice();
        m_ninputs = 2;
        m_noutputs = 2;
        m_samplerate = 44100;
        m_vectorsize = 256;
        initialize();
    }
    
    PortAudioDeviceManager::~PortAudioDeviceManager()
    {
        if(m_stream && Pa_IsStreamActive(m_stream))
        {
            Pa_AbortStream(m_stream);
        }
        if(m_sample_ins)
        {
            delete [] m_sample_ins;
        }
        if(m_sample_outs)
        {
            delete [] m_sample_outs;
        }
        if(m_nmanagers == 1)
        {
            PaError err = Pa_Terminate();
            if(err != paNoError)
            {
                cout << "PortAudio error: %s\n" << Pa_GetErrorText(err) << endl;
            }
        }
        m_nmanagers--;
        cout << "Terminate" << endl;
    }
    
    void PortAudioDeviceManager::getAvailableDrivers(vector<string>& drivers) const
    {
        drivers.clear();
        const PaHostApiIndex numHost = Pa_GetHostApiCount();
        for(PaHostApiIndex i = 0; i < numHost; i++)
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(i);
            if(hostInfo)
            {
                drivers.push_back(hostInfo->name);
            }
        }
    }
    
    string PortAudioDeviceManager::getDriverName() const
    {
        return Pa_GetHostApiInfo(m_driver)->name;
    }
    
    void PortAudioDeviceManager::getAvailableInputDevices(vector<string>& devices) const
    {
        devices.clear();
        const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(m_driver);
        if(hostInfo)
        {
            const PaDeviceInfo *deviceInfo;
            const int numDevices = hostInfo->deviceCount;
            for(int i = 0; i < numDevices; i++)
            {
                const PaDeviceIndex index = Pa_HostApiDeviceIndexToDeviceIndex(m_driver, i);
                deviceInfo = Pa_GetDeviceInfo(index);
                if(deviceInfo)
                {
                    if(deviceInfo->maxInputChannels)
                    {
                        devices.push_back(deviceInfo->name);
                    }
                }
            }
        }
    }
    
    void PortAudioDeviceManager::getAvailableOutputDevices(vector<string>& devices) const
    {
        devices.clear();
        const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(m_driver);
        if(hostInfo)
        {
            const PaDeviceInfo *deviceInfo;
            const int numDevices = hostInfo->deviceCount;
            for(int i = 0; i < numDevices; i++)
            {
                const PaDeviceIndex index = Pa_HostApiDeviceIndexToDeviceIndex(m_driver, i);
                deviceInfo = Pa_GetDeviceInfo(index);
                if(deviceInfo)
                {
                    if(deviceInfo->maxOutputChannels)
                    {
                        devices.push_back(deviceInfo->name);
                    }
                }
            }
        }
    }
    
    string PortAudioDeviceManager::getInputDeviceName() const
    {
        return Pa_GetDeviceInfo(m_device_input)->name;
    }
    
    string PortAudioDeviceManager::getOutputDeviceName() const
    {
        return Pa_GetDeviceInfo(m_device_output)->name;
    }
    
    void PortAudioDeviceManager::setInputDevice(string const& device)
    {
        const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(m_driver);
        if(hostInfo)
        {
            const PaDeviceInfo *deviceInfo;
            const int numDevices = hostInfo->deviceCount;
            for(int i = 0; i < numDevices; i++)
            {
                const PaDeviceIndex index = Pa_HostApiDeviceIndexToDeviceIndex(m_driver, i);
                deviceInfo = Pa_GetDeviceInfo(index);
                if(deviceInfo && deviceInfo->name == device)
                {
                    m_device_input = index;
                }
            }
        }
    }
    
    void PortAudioDeviceManager::setOutputDevice(string const& device)
    {
        const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(m_driver);
        if(hostInfo)
        {
            const PaDeviceInfo *deviceInfo;
            const int numDevices = hostInfo->deviceCount;
            for(int i = 0; i < numDevices; i++)
            {
                const PaDeviceIndex index = Pa_HostApiDeviceIndexToDeviceIndex(m_driver, i);
                deviceInfo = Pa_GetDeviceInfo(index);
                if(deviceInfo && deviceInfo->name == device)
                {
                    m_device_output = index;
                }
            }
        }
    }
    
    ulong PortAudioDeviceManager::getNumberOfInputs() const
    {
        return m_ninputs;
    }
    
    ulong PortAudioDeviceManager::getNumberOfOutputs() const
    {
        return m_noutputs;
    }
    
    void PortAudioDeviceManager::getAvailableSampleRates(vector<ulong>& samplerates) const
    {
        PaStreamParameters input;
        PaStreamParameters output;
        input.device        = m_device_input;
        input.channelCount  = (int)m_ninputs;
        input.sampleFormat  = paFloat32;
        input.hostApiSpecificStreamInfo = NULL;
        output.device        = m_device_output;
        output.channelCount  = (int)m_noutputs;
        output.sampleFormat  = paFloat32;
        output.hostApiSpecificStreamInfo = NULL;
    
        input.suggestedLatency = 0;
        output.suggestedLatency = 0;
        
        for(ulong i = 1; i < 6; i++)
        {
            if(Pa_IsFormatSupported(&input, &output, (double)(11025 * i)) == paFormatIsSupported)
            {
                samplerates.push_back((double)(11025 * i));
            }
            if(Pa_IsFormatSupported(&input, &output, (double)(12000 * i)) == paFormatIsSupported)
            {
                samplerates.push_back((double)(12000 * i));
            }
            if(Pa_IsFormatSupported(&input, &output, (double)(16000 * i)) == paFormatIsSupported)
            {
                samplerates.push_back((double)(16000 * i));
            }
        }
    }
    
    ulong PortAudioDeviceManager::getSampleRate() const
    {
        return m_samplerate;
    }
    
    void PortAudioDeviceManager::setSampleRate(ulong const samplerate)
    {
        vector<ulong> availables;
        getAvailableSampleRates(availables);
        if(find(availables.begin(), availables.end(), samplerate) != availables.end())
        {
            m_samplerate = samplerate;
        }
    }
    
    void PortAudioDeviceManager::getAvailableVectorSizes(vector<ulong>& vectorsizes) const
    {
        for(ulong i = 1; i <= 8192; i *= 2)
        {
            vectorsizes.push_back(i);
        }
    }
    
    ulong PortAudioDeviceManager::getVectorSize() const
    {
        return m_vectorsize;
    }
    
    void PortAudioDeviceManager::setVectorSize(ulong const vectorsize)
    {
        vector<ulong> availables;
        getAvailableVectorSizes(availables);
        if(find(availables.begin(), availables.end(), vectorsize) != availables.end())
        {
            m_vectorsize = vectorsize;
        }
    }
    
    void PortAudioDeviceManager::initialize()
    {
        if(m_stream && Pa_IsStreamActive(m_stream))
        {
            Pa_CloseStream(m_stream);
        }
        if(m_sample_ins)
        {
            delete [] m_sample_ins;
        }
        if(m_sample_outs)
        {
            delete [] m_sample_outs;
        }
        m_sample_ins    = new sample[m_ninputs * m_vectorsize];
        m_sample_outs   = new sample[m_noutputs * m_vectorsize];
        PaStreamParameters input;
        PaStreamParameters output;
        
        input.device        = m_device_input;
        input.channelCount  = (int)m_ninputs;
        input.sampleFormat  = paFloat32;
        input.hostApiSpecificStreamInfo = NULL;
        output.device        = m_device_output;
        output.channelCount  = (int)m_noutputs;
        output.sampleFormat  = paFloat32;
        output.hostApiSpecificStreamInfo = NULL;
        
        input.suggestedLatency = 0;
        output.suggestedLatency = 0;
        
        DeviceNode* node = new DeviceNode(this);
        PaError err = Pa_OpenStream(&m_stream, &input, &output, m_samplerate, m_vectorsize, paClipOff, &callback, node);
        if(err != paNoError)
        {
            return;
        }
        
        err = Pa_SetStreamFinishedCallback(m_stream, &finish);
        if(err != paNoError)
        {
            return;
        }
        
        err = Pa_StartStream(m_stream);
        if(err != paNoError)
        {
            return;
        }
    }
    
    void PortAudioDeviceManager::setDriver(string const& driver)
    {
        const PaHostApiIndex numHost = Pa_GetHostApiCount();
        for(PaHostApiIndex i = 0; i < numHost; i++)
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo(i);
            if(hostInfo && hostInfo->name == driver)
            {
                if(i != m_driver)
                {
                    m_driver = i;
                }
            }
        }
    }
    
    sample const* PortAudioDeviceManager::getInputsSamples(const ulong channel) const noexcept
    {
        if(m_sample_ins && channel < getNumberOfInputs())
        {
            return m_sample_ins + channel * getVectorSize();
        }
        else
        {
            return nullptr;
        }
    }
    
    sample* PortAudioDeviceManager::getOutputsSamples(const ulong channel) const noexcept
    {
        if(m_sample_outs && channel < getNumberOfOutputs())
        {
            return m_sample_outs + channel * getVectorSize();
        }
        else
        {
            return nullptr;
        }
    }
    
    int PortAudioDeviceManager::callback(const void *inputBuffer, void *outputBuffer, ulong framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
    {
        DeviceNode* d = (DeviceNode*)userData;
        Signal::vdeterleaved(d->vectorsize, d->nins, (float *)inputBuffer, d->inputs);
        Signal::vclear(d->vectorsize * d->nouts, d->outputs);
        d->device->tick();
        Signal::vinterleaved(d->vectorsize, d->nouts, (float *)d->outputs, (float *)outputBuffer);
        return paContinue;
    }
    
    void PortAudioDeviceManager::finish(void *userData )
    {
        DeviceNode* d = (DeviceNode*)userData;
        delete d;
    }
}

