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
#include "fileio.h"
#include <windows.h>
#include <fstream>
#include <deque>
#ifndef DXFIO_H
#define DXFIO_H

const string entities[6] = {"CIRCLE","LWPOLYLINE","POLYLINE","LINE","ARC","VERTEX"};

class DXFio: public fileIO
	{
		std::ifstream* fin;
		deque <string> geometry;
		static const size_t entitiesLen = sizeof(entities) / sizeof(entities[0]);
	public:
		~DXFio()
		{
		if (fin)
			{
			fin->close();
			delete fin;
			fin = NULL;
			}
		}
		DXFio(const char* name, unsigned rwaccess):fileIO(name,rwaccess)
			{
			fin = new ifstream(name);
            }
    deque <string> *returnGeometry()
    {
        return &geometry;
    }
	int fileRead();
	} ;
#endif



