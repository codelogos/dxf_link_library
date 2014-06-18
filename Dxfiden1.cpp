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

int DXFIdentEntity::parsePtrToData()
{

    static const char entities[6][9] = {{"CIRCLE"},{"LINE"},{"ARC"},{"POLYLINE"},{"VERTEX"}};

    if (lockout == 0)
    {
        ptrToEntities = strstr(tptr,"ENTITIES");
        ptrToEOF = strstr(tptr,"EOF");
        ptrToBlkEnd = strstr(tptr,"~");

        if (ptrToEOF == NULL && ptrToBlkEnd == NULL)
        {
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

    while ((*(start)< 'A' || *(start) > 'Z') && start != ptrToEOF && start != ptrToBlkEnd || *start == 'D')
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
        end++;
        if (end == ptrToEOF)
        {
            lockout = 0;
            if (tempEntSize < (end - start))
            {
                tempEntity = (char *) realloc(tempEntity, end - start );
                tempEntSize =  end - start;
                memset(tempEntity,'\0', end - start);
            }
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
            memcpy((char *)(tempEntity),start,end - start - 1);
            return 2;
        }
    }

    if (t == polyline || t == vertex)
    {
        while (strncmp(end,"SEQ",strlen("SEQ"))) //(*(end + 1) != 'S' && *(end + 2) != 'E' && *(end + 2) != 'Q'))//&& end != ptrToEOF)
        {
            end++;
            if (end == ptrToEOF)
            {
                lockout = 0;
                if (tempEntSize < (end - start))
                {
                    tempEntity = (char *) realloc(tempEntity, end - start );
                    tempEntSize = end - start;
                    memset(tempEntity,'\0',end-start);
                }
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
                    memcpy((char *)(tempEntity),start,end - start - 1);
                    return 0;
                }
            }
            else if ((end +1) >= ptrToBlkEnd)
            {
                lockout = 0;
                if (tempEntSize < (end - start))
                {
                    tempEntity = (char *) realloc(tempEntity, end - start );
                    memset(tempEntity,'\0',end-start);
                    tempEntSize = end - start;
                }
                memcpy((char *)(tempEntity),start,end - start - 1);
                return 2;
            }
        }
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
            return 0;
        }
    }

    memcpy((char *)(tempEntity),start,end - start -1);

    start = end;

    if (ptrToEOF)
    {
        if (start >= ptrToEOF)
        {
            lockout = 0;
            return 0;
        }
        else
            return 1;
    }
    else if (start >= ptrToBlkEnd)
    {
        lockout = 0;
        return 2;
    }
    else
    {
        return 1;
    }
}
























































