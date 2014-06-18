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
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "dxfstrct.h"
#include <iostream>
extern char arctemp[6][4];
extern char linetemp[5][5];
extern char circletemp[4][7];
extern char polytemp[4][9];
#define IGNOREENTITY(x) if (strncmp(start,x,strlen(x)) == 0) start += strlen(x)
class DXFIdentEntity
{
    int entityLockout;
    int tempEntSize;
    int lockout;
    char* tempEntity,* start,* end,* tptr;
    char* ptrToEntities,* ptrToEOF,* ptrToBlkEnd;
    type t;

public:
    int parsePtrToData();
    char* returnEntityData()
    {
        return (char *)tempEntity;
    };
    void assignCharPtr(char *tmp)
    {
        tptr = tmp;
    }
    type returnEntityType()
    {
    return t;
    }
    ~DXFIdentEntity()
    {
        entityLockout = 0;
        if (tempEntity)
        {
            free(tempEntity);
            tempEntity = NULL;
        }
    }

    DXFIdentEntity()
    {
        lockout = 0;
        entityLockout = 0;
        tempEntity = NULL;
        ptrToEntities = ptrToEOF = ptrToBlkEnd = NULL;
    }
};
