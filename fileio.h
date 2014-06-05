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
    //const char rwAccess[128];
    char  * ptr;
public:
    fileIO(const char* name, unsigned int rwaccess)
    {
        //char* p = (char*)rwAccess;
        strcpy(fileIO::name,name);
        //if (!strstr(rwAccess,rwaccess) || rwAccess == " ")
        //	fio = open(name,O_APPEND);//this should really be an error, however
        //constructors don't have return values
        //else
        fio = open(name,rwaccess);
        //cout<<fio<<" 1fio"<<endl;
        strcpy(fileIO::name, name);
        ptr = NULL;
    }
    ~fileIO()
    {
        //MessageBox(NULL,"fileio dtor","",MB_OK);
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
        //cout<<string<<" filename"<<endl;
        if (fout<<str.str()<<endl)
        {
            fout.close();
            return 1;
        }
        fout.close();
        return 0;
        /*cout<<fIO->fio<<" hi"<<endl;
                //cin.get();
                cout<<string<<" "<<length<<" string length"<<endl;
        if(write(fIO->fio,string,length) != -1)
        	return 0;
                cout<<"failure"<<endl;
        return 1;*/
    }

    virtual int checkRecordExist(char * string){} ;
    virtual int fileRead() {};

};

#endif
