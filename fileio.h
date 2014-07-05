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
#include<stdio.h>
#include <io.h>
#include <dos.h>
#include <string.h>
#include <sstream>
#include <fcntl.h>
#include <fstream>

#ifndef FILE_IO
#define FILE_IO
using namespace std;
class fileIO
{
protected:
    int fio;
    char name[256];
    char  * ptr;
public:
    fileIO(const char* name, unsigned int rwaccess)
    {
        strcpy(fileIO::name,name);
        fio = open(name,rwaccess);
        strcpy(fileIO::name, name);
        ptr = NULL;
    }
    virtual ~fileIO()
    {
        if (ptr)
        {
        delete ptr;
        ptr = NULL;
        }
        close(fio);
    }

    int fileWrite(char*  input)
    {
        ofstream fout(name,ios::app);
        stringstream str;
        str<<input;
        if (fout<<str.str()<<endl)
        {
            fout.close();
            return 1;
        }
    }
    int fileWrite(float input)
    {
        ofstream fout(name,ios::app);
        std::stringstream str;
        str<<input;
        if (fout<<str.str()<<endl)
        {
            fout.close();
            return 1;
        }
        fout.close();
        return 0;
    }

    virtual int checkRecordExist(char * string){return 0;} ;
    virtual int fileRead() {return 0;};

};

#endif
