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
//#include <windows.h>
#include "dxfio.h"
#include <ctype.h>
#define PAD 256
int DXFio::fileRead()
{
    unsigned long int flength, numbytes;
    char *tmpptr;
    static int section, entities, entityfound;
    static int flag;
    //cout<<sizeof(ptr)<<" ptr size"<<endl;
    long int prevByteSize;
    char ch;
    char textline[256]={'\n'};
    char *temp;
    long int i = 0, n = 0, totalbytes=0;
//	fileIO fio("f:\\bc45\\bin\\temp2.txt",O_APPEND);
//flength = filelength(fio);
    if (ptr[0] == NULL)
    {

        ptr[0] = new char[MAXBLOCKSIZE+1];//fileLength];
        memset(ptr[0],'\0',MAXBLOCKSIZE+1);//sizeof ptr[0]);
    }
    while (fin->get(ch))
    {
     //   cout<<textline<<endl;

//cout<<arrayIdx<<endl;
        //cout<<arrayIdx<<" "<<textline<<endl;

        if (section == 0)
        {
            textline[i] = ch;
            if (strstr(textline,"ENTITIES"))
            {
                memset(textline,'\0',TEXTLINE_SIZE);
                strcpy(textline,"ENTITIES");

                i = 8;
                section = 1;
            }
            i++;
            if (i>=TEXTLINE_SIZE)
            {
                memset(textline,'\0',TEXTLINE_SIZE);
                i=0;

            }
        }

        else if ((ch >= 'A' && ch <= 'Z') || (ch > 44 && ch < 58) || ch == 43)

        {
            textline[i]=ch;
            i++;
            if (i>TEXTLINE_SIZE)
            {
                delete ptr[0];
                return 0;
            }
        }

        else
        {
            /*static long int j;
            fileIO fio("c:\\test.txt",1);
            char fw[10];
            itoa(j,fw,10);
            fio.fileWrite((char*)fw);
            //fio.fileWrite(ptr[k]);
            cout<<j<<endl;
            j++;*/
            i = 0;

            if (strlen(textline) > 0)
            {
                temp = (char *) textline;
                strcat(temp," ");

                if (strstr(temp,"EOF"))
                {
                    break;
                }

                //cout<<arrayIdx<<"  "<<i<<endl;
                prevByteSize = totalbytes;
                totalbytes += strlen(temp);

                if (fileLength < MAXBLOCKSIZE); //don't need to find entity boundaries in this case
                /*else if (totalbytes > MAXBLOCKSIZE &&
                        strstr(textline," 0 ")
                        )
                {
                    strcat(ptr[arrayIdx],"~");
                    arrayIdx++;
                    ptr[arrayIdx] =	new char[MAXBLOCKSIZE+ 1];
                    memset(ptr[arrayIdx],'\0',MAXBLOCKSIZE+ 1);
                    totalbytes = 0;
                }*/
                else if (strstr(temp,"ARC")      ||  //do we have the beginning of an entity?
                         strstr(temp,"LINE")     ||
                         strstr(temp,"CIRCLE")   ||
                         strstr(temp,"VERTEX")   ||
                         strstr(temp,"POINT")    ||
                         strstr(temp,"POLYLINE"))
                {

                    if (flag == 0)                  //yes
                    {
                        entityfound = 1;
                        flag = 1;
                    }
                    else if (totalbytes > MAXBLOCKSIZE+1)
                    {
                        if (strstr(temp,"VERTEX"))
                        {
                            int verCount = strlen(ptr[arrayIdx])+1;
                            char* resize;
                            resize = new char[verCount+1];
                            memset(resize,'\0',verCount+1);
                            strncpy(resize,ptr[arrayIdx],verCount);
                            delete[] ptr[arrayIdx];
                            ptr[arrayIdx] = new char[verCount+1];
                            memset(ptr[arrayIdx],'\0',verCount+1);
                            strncpy(ptr[arrayIdx],resize,verCount+1);
                            delete[] resize;
                            strcat(ptr[arrayIdx],"SEQ");
                            strcpy(temp,"POLYLINE");

                        }
                        char * resize;
                        resize = new char[totalbytes + 2];
                        memset(resize,'\0',totalbytes + 2);
                        strncpy(resize,ptr[arrayIdx],prevByteSize);
                        delete[] ptr[arrayIdx];
                        ptr[arrayIdx] = new char[totalbytes+strlen(temp)+2];
                        memset(ptr[arrayIdx],0,totalbytes+strlen(temp)+2);
                        strncpy(ptr[arrayIdx],resize,totalbytes);
                        delete[] resize;

                        strcat(ptr[arrayIdx],"~");
                        arrayIdx++;
                        ptr[arrayIdx] =	new char[MAXBLOCKSIZE+2];
                        memset(ptr[arrayIdx],0,MAXBLOCKSIZE+2);
                        totalbytes = strlen(temp);
                    }
                }
                else if (totalbytes > MAXBLOCKSIZE+1 && entityfound && //processing entity elements - haven't found end
                         (!strstr(temp,"ARC")      ||
                          !strstr(temp,"LINE")     ||
                          !strstr(temp,"CIRCLE")   ||
                          !strstr(temp,"POINT")    ||
                          !strstr(temp,"VERTEX")   ||
                          !strstr(temp,"SEQ")      ||
                          !strstr(temp,"POLYLINE")))
                {
                    char * resize;
                    resize = new char[totalbytes];
                    memset(resize,'\0',totalbytes);
                    strncpy(resize,ptr[arrayIdx],prevByteSize);
                    delete[] ptr[arrayIdx];
                    ptr[arrayIdx] = new char[totalbytes+strlen(temp)+1+ PAD ];
                    memset(ptr[arrayIdx],0,totalbytes+strlen(temp)+1+ PAD);
                    strncpy(ptr[arrayIdx],resize,totalbytes);
                    delete[] resize;
                }
                else if (totalbytes > MAXBLOCKSIZE+1 && entityfound &&
                         (strstr(temp,"ARC")      ||
                          strstr(temp,"LINE")     ||
                          strstr(temp,"CIRCLE")   ||
                          strstr(temp,"POINT")    ||
                          strstr(temp,"VERTEX")    ||
                          strstr(temp,"SEQ")    ||
                          strstr(temp,"POLYLINE")))
                {
                    if (strstr(temp,"VERTEX"))
                    {
                        char * resize;
                        long int ptrLen = strlen(ptr[arrayIdx]);
                        resize = new char[ptrLen+1];
                        memset(resize,'\0',ptrLen+1);
                        strncpy(resize,ptr[arrayIdx],ptrLen-1);
                        delete[] ptr[arrayIdx];
                        ptr[arrayIdx] = new char[ptrLen+1];
                        memset(ptr[arrayIdx],0,ptrLen+1);
                        strncpy(ptr[arrayIdx],resize,ptrLen + 1);
                        delete[] resize;
                        strcat(ptr[arrayIdx],"JN");
                    }

                    arrayIdx++;

                    ptr[arrayIdx] =	new char[MAXBLOCKSIZE+1];
                    memset(ptr[arrayIdx],0,MAXBLOCKSIZE+1);
                    totalbytes = strlen(temp);
                }



                strcat(ptr[arrayIdx],temp);

                //cout<<arrayIdx<<endl;
                memset(textline,'\0',strlen(textline));
            }
        }
    }

    if (section == 0)
    {
        return 0;
    }

    char * resize;

    if (fileLength < MAXBLOCKSIZE+1)
        resize = new char[fileLength + 1+ PAD];
    else
        resize = new char[totalbytes + 1+ PAD];

    memset(resize,'\0',totalbytes);
    strcpy(resize,ptr[arrayIdx]);

    /*delete[] ptr[arrayIdx];

    if (fileLength < MAXBLOCKSIZE)
        ptr[arrayIdx] = new char[fileLength + 4];
    else
        ptr[arrayIdx] = new char[totalbytes + 4];*/

    strncpy(ptr[arrayIdx],resize,strlen(resize));
    strcat(ptr[arrayIdx],"EOF");
    delete[] resize;
    flag = entities = section = entityfound = 0;

//fio.fileWrite(&fio,ptr[k],1);
    return 1;

//cout<<totalbytes<<endl;
//cout<<fileLength<<endl;
}


