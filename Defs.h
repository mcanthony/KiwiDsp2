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

#ifndef __DEF_KIWI_DSP_DEFS__
#define __DEF_KIWI_DSP_DEFS__

#include <stdio.h>
#include <stdarg.h>
#include <cwchar>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <cmath>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <thread>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

using namespace std;

namespace Kiwi
{
    typedef unsigned long ulong;
    namespace Dsp
    {
#ifdef KIWI_DOUBLE
        typedef double sample;
#else
        typedef float  sample;
#endif
        
        class Signal;
        typedef shared_ptr<Signal>          sSignal;
        typedef weak_ptr<Signal>            wSignal;
        typedef shared_ptr<const Signal>    scSignal;
        typedef weak_ptr<const Signal>      wcSignal;
        
        class Node;
        typedef shared_ptr<Node>            sNode;
        typedef weak_ptr<Node>              wNode;
        typedef shared_ptr<const Node>      scNode;
        typedef weak_ptr<const Node>        wcNode;
        
        class Process;
        typedef shared_ptr<Process>         sProcess;
        typedef weak_ptr<Process>           wProcess;
        typedef shared_ptr<const Process>   scProcess;
        typedef weak_ptr<const Process>     wcProcess;
        
        class Connection;
        typedef shared_ptr<Connection>      sConnection;
        typedef weak_ptr<Connection>        wConnection;
        typedef shared_ptr<const Connection>scConnection;
        typedef weak_ptr<const Connection>  wcConnection;
    }
};

#endif


