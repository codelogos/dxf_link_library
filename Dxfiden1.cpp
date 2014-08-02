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
#include "fileio.h"


int DXFIdentEntity::parsePtrToData()
{

        string currentLine;
        int entityCount = 0;
        bool extantData;
        entity.clear();
        do
        {
        currentLine = geometry->back();
        if (entityCount == 0)
        {
            for (int i=0; i<entitiesLen;i++)
            {

                if (currentLine == entities[i])
                {
                    entityType = (type)i;
                    break;
                }
            }
        }
        else if (entityCount <=8)
            {
                entity += currentLine;
                entity += " ";
            }
        else
        {
            for (int i=0; i<entitiesLen;i++)
            {
                if (currentLine==entities[i] || currentLine =="EOF")
                {
                    geometry->push_back(entities[i]);
                    goto TERMINATELOOP;
                }
            }
            entity += currentLine;
            entity += " ";

        }

        entityCount++;
        geometry->pop_back();
        extantData = geometry->size();
        char ch[20];
        }
        while (extantData);
        TERMINATELOOP:

        extantData = geometry->size();

if (extantData)
    return 1;
else
    return 0;
}





















































