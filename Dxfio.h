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
#ifndef DXFIO_H
#define DXFIO_H
const int NUMBLOCKS = 10000;
#define TEXTLINE_SIZE 256
class DXFio: public fileIO
	{
long int MAXBLOCKSIZE;
		int arrayIdx, numblocks;
		ifstream* fin;
		long int fileLength;
		char * ptr[NUMBLOCKS];
	public:
		int returnCount(){return numblocks-1;}
	~DXFio()
		{
		int j = 0;
		while (j<=arrayIdx)
			{
			delete[] ptr[j];
			j++;
			}

		fileLength = 0;
		if (fin)
			{
			fin->close();
			delete fin;
			fin = NULL;
			}
		}



		DXFio(const char* name, unsigned rwaccess):fileIO(name,rwaccess)
			{
			int i = 0;
			numblocks =	arrayIdx = 0;
			fileLength = 0;

			fin = NULL;

			ifstream Fin(name);
			char ch, textline[TEXTLINE_SIZE];

			while(Fin.get(ch))
				{
				if ((ch >= 'A' && ch <= 'Z') || (ch > 44 && ch < 58) || ch == 43)
					{
					textline[i]=ch;
					i++;
					}
				else
					{
					i = 0;
					if (strlen(textline) > 0)
						{
						fileLength += (strlen(textline)+1);
						memset(textline,0,sizeof(textline));
						}
					}
				}
			i = 0;
			MAXBLOCKSIZE = fileLength + 1024;

			while(i < NUMBLOCKS)
				{
				ptr[i] = NULL;
				i++;
				}

			Fin.close();
			numblocks =  (int)fileLength / (MAXBLOCKSIZE-1024);
			fin = new ifstream(name);
		}

	char * returnPointer( int k ){ return ptr[k]; }
	int fileRead();
	} ;
#endif



