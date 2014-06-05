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
#include "dxfident.h"
//#include <fileio.h>
//extern int parselockout;
int DXFIdentEntity::parsePtrToData()
{
    char ch;
    static const char entities[6][9] = {{"CIRCLE"},{"LINE"},{"ARC"},{"POLYLINE"},{"VERTEX"}};
    //	cout<<"14 dxfiden1"<<endl;
    char test[20];
//       fileIO fio("F:\\bc45\\bin\\chucky.txt",O_APPEND);
    //	fio.fileWrite(&fio,tptr,1);
    if (lockout == 0)
    {
//cout<<lockout<<" lockout"<<endl;

        //fio.fileWrite(&fio,tptr,0);
        //fio.fileWrite(&fio,"\n\n\n\n\n",0);

        ptrToEntities = strstr(tptr,"ENTITIES");
        ptrToEOF = strstr(tptr,"EOF");
        ptrToBlkEnd = strstr(tptr,"~");

        if (ptrToEOF == NULL && ptrToBlkEnd == NULL)
        {
            //cout<<"runaway"<<endl;
            return -1;      //prevents runaway pointers
        }
        if (ptrToEntities == NULL && entityLockout != 0)
            ptrToEntities = tptr;

        else if (ptrToEntities)
        {
            entityLockout = 1;
        }
        else
            return 0;


        start = end = ptrToEntities;
        if (ptrToEntities == tptr)
        {
            start += 8;
            end = start;
            if (!strstr(start,"10") && !strstr(start,"20")) // we must have at least one entity
                return -1;             // following the ENTITY declaration
        }

        lockout = 1;
    }

    while (*(start)< 'A' || *(start) > 'Z' && start != ptrToEOF && start != ptrToBlkEnd || *start == 'D')
    {
    POINT:
        IGNOREENTITY("POINT");//if we find a POINT entity, bypass it and keep looking
        IGNOREENTITY("SOLID");//if we find a SOLID entity, bypass it and keep looking
        IGNOREENTITY("default");//if we find a default entity, bypass it and keep looking

        if (strncmp(start,"TEXT",strlen("TEXT")) == 0)
        {
            if (strstr(start+25," 0 "))
                start = strstr(start+25," 0 ");
            else
                start += strlen("TEXT");
        }
        start++;
        end = start; //keep the block pointers in sync
    }
    static int count;
    count++;
    /*		if (count == 620)
       		{
       		int b;
       		b =5;
       		}*/


    if (!strncmp(start,"CIRCLE",strlen(entities[0])))
        t = circle;
    else if (!strncmp(start,"LINE", strlen(entities[1])))
        t = line;
    else if (!strncmp(start,"ARC", strlen(entities[2])))
        t = arc;
    else if (!strncmp(start,"POLYLINE", strlen(entities[3])))
        t = polyline;
    else if (strstr(start,"SOLID"))
        goto POINT;
    else if (strstr(start,"default"))
        goto POINT;
    else if (strstr(start,"TEXT"))
        goto POINT;
    else if (strstr(start,"E+"))
        goto POINT;

    //else if (!strncmp(start,entities[4], strlen(entities[4])))
    //	t = vertex;
    else if (strstr(start,"POINT")) // this is needed to account for POINT
        goto POINT;						  // entities at the end of a DXF file
    else if (strstr(start,"EOF") - start < 30)
    {
        lockout = 0;
        return 0;
    }
    else if (strstr(start,"LAYER"))
    {
        start += 6;
        goto POINT;
    }

    else
        return 1;

    while (*end >= 'A' && *end <= 'Z')// && end < ptrToEOF && end < ptrToBlkEnd)
    {
        //cout<<*end;
        end++;
        if (end == ptrToEOF)
        {
            //cout<<"61"<<endl;
            lockout = 0;
            //cout<<"62 panic"<<endl;
            //cin.get();
            if (tempEntSize < (end - start))
            {
                tempEntity = (char *) realloc(tempEntity, end - start );
                tempEntSize =  end - start;
                memset(tempEntity,'\0', end - start);
            }
            //fio.fileWrite(&fio,tempEntity,1);
            memcpy((char *)(tempEntity),start,end - start - 1);
            return 0;
        }

        if (end == ptrToBlkEnd)
        {
            lockout = 0;
            if (tempEntSize < (end - start))
            {
                tempEntity = (char *) realloc(tempEntity, end - start );
                tempEntSize = end - start;
                memset(tempEntity,'\0',end-start);
            }
            //fio.fileWrite(&fio,tempEntity,1);
            memcpy((char *)(tempEntity),start,end - start - 1);
            return 2;
        }
    }

    if (t == polyline || t == vertex)
    {
        /*static int count;
        count++;
        if (count == 767)
        	{
        	int b ;
        	b = 5;
        	}*/
        while (strncmp(end,"SEQ",strlen("SEQ"))) //(*(end + 1) != 'S' && *(end + 2) != 'E' && *(end + 2) != 'Q'))//&& end != ptrToEOF)
        {
            end++;
            if (end == ptrToEOF)
            {
                //cout<<"74"<<endl;
                lockout = 0;
                //cout<<"75 panic2"<<endl;
                if (tempEntSize < (end - start))
                {
                    tempEntity = (char *) realloc(tempEntity, end - start );
                    tempEntSize = end - start;
                    memset(tempEntity,'\0',end-start);
                }
                //fio.fileWrite(&fio,tempEntity,1);
                memcpy((char *)(tempEntity),start,end - start - 1);
                return 0;
            }
            if (end == ptrToBlkEnd)
            {
                lockout = 0;
                if (tempEntSize < (end - start))
                {
                    tempEntity = (char *) realloc(tempEntity,end - start );
                    tempEntSize = end - start;
                    memset(tempEntity,'\0',end - start);
                }
                //fio.fileWrite(&fio,tempEntity,1);
                memcpy((char *)(tempEntity),start,end - start - 1);
                return 2;
            }
        }
        end += 6;
    }
    else
        while ((*(end + 1) < 'A' || *(end + 1) > 'Z' ) || (*(end + 1) == 'D' ||*(end + 1) == 'E'))//last test is for exponent
        {
            if (strncmp(end," 8 ",strlen(" 8 ")) == 0) //bypass any text that
            {
                //follows entity eight
                end += 3;    								//this code should eventually
                while (*end >= 'A' && *end <= 'Z' ) //be removed and the subsequent
                    end++;									//entity processed
            }
            if (strncmp(end," 6 ",strlen(" 6 ")) == 0)//bypass any text that
            {
                //follows entity siz
                end += 3;									//this code should eventually
                while (*end >= 'A' && *end <= 'Z' ) //be removed and the subsequent
                    end++;									//entity processed
            }



            end++;

            //cout<<*end;
            if (ptrToEOF)
            {
                if ((end+1) >= ptrToEOF)
                {
                    lockout = 0;
                    if (tempEntSize < (end - start))
                    {
                        tempEntity = (char *) realloc(tempEntity,end - start );
                        memset(tempEntity,'\0',end-start);
                        tempEntSize = end - start;
                    }
                    //	fio.fileWrite(&fio,tempEntity,1);
                    memcpy((char *)(tempEntity),start,end - start - 1);
                    return 0;
                }
            }
            else if ((end +1) >= ptrToBlkEnd)
            {
                lockout = 0;
                //cout<<"97"<<endl;
                //printf("%p\n",end);
                //printf("%s",start);
                //cin.get();
                if (tempEntSize < (end - start))
                {
                    tempEntity = (char *) realloc(tempEntity, end - start );
                    memset(tempEntity,'\0',end-start);
                    tempEntSize = end - start;
                }
                //	fio.fileWrite(&fio,tempEntity,1);
                memcpy((char *)(tempEntity),start,end - start - 1);
                return 2;
            }
        }
    //	if (*(end+1) == 'P' && *(end + 2) == 'O' && *(end + 3) == 'I')
    //		int p = 1;
    if (tempEntity == NULL)
    {
        if (end - start > 32767)
        {
            MessageBox(NULL,"memory overflow","",MB_OK);
            return 0;
        }
        tempEntity = (char *) malloc(end - start);
        memset(tempEntity,'\0',end-start);
        if (tempEntity == NULL)
        {
            //cout<<"105 panic"<<endl;
            //cin.get();
            return 0;
        }
        tempEntSize = end - start;
    }
    else if (tempEntSize < (end - start))
    {
        tempEntity = (char *) realloc(tempEntity,end - start );
        memset(tempEntity,'\0',end-start);
        tempEntSize = end - start;
        if (tempEntity == NULL)
        {
            //cout<<"117 panic"<<endl;
            //cin.get();
            return 0;
        }
    }
    char chr;
    chr = *end;
    //*end = '\0';
    //int L = strlen(start);
    //sprintf(test,"st:%p,en:%p",start,end);
    //fio.fileWrite(&fio,test,1);
    //sprintf(test,"%d",end - start - 1);
    //fio.fileWrite(&fio,test,1);

    memcpy((char *)(tempEntity),start,end - start -1);

    start = end;
    //if (strstr(end,"~") - end > 0)
    //	printf("%d",strstr(end,"~") - end);
    //cout<<strstr(end,"~") - end<<"   "<<*ptrToBlkEnd<<endl;
    //fio.fileWrite(&fio,tempEntity,1);
    //printf("%p\n",strstr(end,"~")-1);
    //printf("%p,%p",end,ptrToBlkEnd);
    //cin.get();

    if (ptrToEOF)
    {
        if (start >= ptrToEOF)
        {
            lockout = 0;
            //cout<<"ptr to eof"<<endl;
            //cin.get();
            return 0;
        }
        else
            return 1;
    }
    else if (start >= ptrToBlkEnd)
    {
        //cout<<"ptr to ~"<<endl;
        //cin.get();
        lockout = 0;
        return 2;
    }
    else
    {
        //cout<<"panic panic"<<endl;
        return 1;
    }
}
























































