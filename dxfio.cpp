/*Copyright (c) 2014, Michael Peters
All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/
#include <iostream>
#include <string.h>
#include <ctime>
#include "dxfio.h"
#include <ctype.h>
using namespace std;
int DXFio::fileRead()
{

    bool section=false;

    std::string textline;
    ifstream fin(name);
    bool firstEntity = false;
    while (std::getline(fin,textline).good())
    {
    if (section == false)
        {
            if (textline =="ENTITIES")
            {
                section = true;
            }
        }
    else if (firstEntity == false)
        {
        for (int i=0; i<entitiesLen; i++)
        {
          if (textline == entities[i])
                {
                    firstEntity = true;
                    geometry.push_front(textline);
                }
        }
        }
    else
        {
        geometry.push_front(textline);
        }
    }

    if (section == 0)
    {
        return 0;
    }
    return 1;
}


