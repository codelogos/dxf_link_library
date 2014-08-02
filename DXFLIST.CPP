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

#include "dxflist.h"
#include <string.h>
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <stdlib.h>
//#define DEBUG
#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif
#define DLLINE(x) dllTemp->dline->x

DXFLinkList::DXFLinkList()
{
    current = temp = prev = next = NULL;
    dllHead = dllTail = dllTemp = NULL;
    darc = NULL;
    dline = NULL;
    dcircle = NULL;
    entityLength = 0;
    plCurrent = plTemp = plHead = plTail = NULL;
    if (refcount > 0)
    {
        darc = new DXFarc;
        dline = new DXFline;
        dcircle = new DXFcircle;
    }
    refcount++;
}

DXFLinkList::~DXFLinkList()
{
    refcount--;

    while (dllHead)
    {
        if (dllHead->plHead)
            while (plHead)
                {
                plTemp = plHead->next;
                delete plHead;
                plHead = plTemp;
                }

        delete dllHead->dline;
        delete dllHead->darc;
        delete dllHead->dcircle;
        temp = dllHead->next;
        delete dllHead;
        dllHead=temp;
    }
}

int DXFLinkList::isVertexJoined(DXFLinkList *dll)
{

    if (dll->next)
    {
        if (dll->next->plHead->joinVertex == 1)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}
void DXFLinkList::addnode(type t, const char* ctemp)
{
    char* p,* tptr = NULL;
    //MessageBox(NULL,ctemp,"",MB_OK);
    double dtemp;
    static double totalPolyLength;
    temp = new DXFLinkList();


    if (dllHead == NULL)
    {
        dllHead = temp;
        dllHead->next = NULL;
        dllHead->prev = NULL;
        dllTail = dllHead;
        dllTemp = dllHead;
        dllTemp->current = dllHead;
    }
    else if (t == vertex)
    {
        goto VERTEX;
    }
     else
    {
        dllTemp->next = temp;
        temp->prev = dllTemp;
        temp->current = dllTemp->current;
        dllTemp = temp;
        dllTail = temp;
    }

    dllTemp->entityLength = 0;
    dllTemp->entityType = t;

if (t == polyline || t == lwpolyline)
{

    char *p70;
    char ch70[4];
    p70 = strstr(ctemp," 70 ");
    if (p70)
    {
    int closePoly = atoi(p70+3);

    if (closePoly == 1)
        closed = true;

    }
}

VERTEX:
    if (t == vertex)
    {
        char enttemp[5][11] = {{" 10 "},{" 20 "},{"CONTINUOUS"},{"SEQEND"}};
        char *p;
        char lastXVal[30], lastYVal[30];
            if (dllTemp->plHead == NULL )
            {
                dllTemp->plHead = new DXFpolyline();
                dllTemp->plTemp = dllTemp->plHead;
                dllTemp->plTail = dllTemp->plHead;
                dllTemp->plTemp->next = NULL;
                dllTemp->plTemp->prev = NULL;

                if (closed == true)
                {
                dllTemp->plTemp->next = new DXFpolyline();
                }
            }
            else
            {
                dllTemp->plCurrent = new DXFpolyline();
                if (dllTemp->plTemp->next)
                    {
                        dllTemp->plCurrent->next = dllTemp->plTemp->next;
                    }
                dllTemp->plTail = dllTemp->plCurrent;


                dllTemp->plTemp->next = dllTemp->plCurrent;
                dllTemp->plCurrent->prev = dllTemp->plTemp;
                dllTemp->plTemp = dllTemp->plCurrent;
            }

            char *_temp;

            for (int i = 0; i<2; i++)
            {
                p = strstr(ctemp,enttemp[i]);
                ctemp = p;
                if (p == NULL)
                {
                    strcpy(dllTemp->plTemp->xcoor,lastXVal);
                    strcpy(dllTemp->plTemp->ycoor,lastYVal);
                    return;
                }
                char temp2[40];
                _temp = strchr(p+4,' ');
                *_temp = '\0';
                strcpy(temp2,p+4);
                tptr = p;

                *_temp = ' ';

                if (i == 0)
                {
                    strcpy(dllTemp->plTemp->xcoor,temp2);
                    strcpy(lastXVal,temp2);

                    if (closed == true && dllTemp->plHead == dllTemp->plTail)
                    {
                        strcpy(dllTemp->plTemp->next->xcoor,temp2);
                    }
                }
                if (i == 1)
                {
                    strcpy(dllTemp->plTemp->ycoor,temp2);
                    strcpy(lastYVal,temp2);
                    if (closed == true && dllTemp->plHead == dllTemp->plTail)
                    {
                        strcpy(dllTemp->plTemp->next->ycoor,temp2);
                    }
                }
            }
    }
    if (t == lwpolyline)
    {
        char enttemp[3][5] = {{" 10 "},{" 20 "},{" 70 "}};

        char x[30], y[30];
        char *p10, *p20, *p70;

        closed = false;
        p10 = p20 = (char *)ctemp;
        p10 = strstr(ctemp,enttemp[0]);
        p20 = strstr(p10,enttemp[1]);

        char *_temp;

        while(1)
            {
            if (dllTemp->plHead == NULL )
            {
                dllTemp->plHead = new DXFpolyline();
                dllTemp->plTemp = dllTemp->plHead;
                dllTemp->plTail = dllTemp->plHead;
                dllTemp->plTemp->next = NULL;
                dllTemp->plTemp->prev = NULL;
            }
            else
            {
                dllTemp->plCurrent = new DXFpolyline();
                dllTemp->plTail = dllTemp->plCurrent;
                dllTemp->plTemp->next = dllTemp->plCurrent;
                dllTemp->plCurrent->prev = dllTemp->plTemp;
                dllTemp->plTemp = dllTemp->plCurrent;
            }
            char *_temp;
            _temp = strchr(p10+4,' ');
            *_temp = '\0';

                strcpy(x,p10+4);

                _temp = strchr(p20+4,' ');
                *_temp = '\0';

                strcpy(y,p20+4);

                strcpy(dllTemp->plTemp->xcoor,x);
                strcpy(dllTemp->plTemp->ycoor,y);
                p10 = strstr(_temp + 1,enttemp[0]);
                p20 = strstr(p10,enttemp[1]);

               if (!strstr(p20,enttemp[0]))
                        break;
        }
        if (closed == true)
        {
                dllTemp->plCurrent = new DXFpolyline();
                dllTemp->plTail = dllTemp->plCurrent;
                dllTemp->plTemp->next = dllTemp->plCurrent;
                dllTemp->plCurrent->prev = dllTemp->plTemp;
                dllTemp->plTemp = dllTemp->plCurrent;
                strcpy(dllTemp->plTemp->xcoor,dllTemp->plHead->xcoor);
                strcpy(dllTemp->plTemp->ycoor,dllTemp->plHead->ycoor);
                closed = false;
        }
    }
    if (t == arc)
    {
        char enttemp[5][5] = {{" 10 "},{" 20 "},{" 40 "},{" 50 "},{" 51 "}};
        for (int i = 0; i<5; i++)
        {
            p = strstr(ctemp,enttemp[i]);//dcircle->ParseValue[i+1]);

            if (p == NULL)
            {
                dtemp = 0;
            }
            else
            {
                dtemp = atof(p+3);
                *(p+3) = ' ';
            }

            D(fio.fileWrite(&fio,p,1));
            D(fio.fileWrite(&fio," HEAD ",1));
            switch (i)
            {
            case 0:
                dllTemp->darc->xcenter=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->xcenter));
                D(fio.fileWrite(&fio,temp2,1));
                break;
            case 1:
                dllTemp->darc->ycenter=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->ycenter));
                D(fio.fileWrite(&fio,temp2,1));
                break;
            case 2:
                dllTemp->darc->radius=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->radius));
                D(fio.fileWrite(&fio,temp2,1));
                break;
            case 3:
                dllTemp->darc->startangle=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->startangle));
                D(fio.fileWrite(&fio,temp2,1));
                break;
            case 4:
                dllTemp->darc->endangle=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->endangle));
                D(fio.fileWrite(&fio,temp2,1));
                break;
            }
       }
        //dllTemp->entityLength = computeArcLength(dllTemp->darc->radius,
        //                        dllTemp->darc->startangle,
        //                          dllTemp->darc->endangle);
        D(fio.fileWrite(" ",1));
    }

    if (t == line)
    {
        for (int i = 0; i<4; i++)
        {
            char enttemp[4][5] = {{" 10 "},{" 20 "},{" 11 "},{" 21 "}};
            p = strstr(ctemp,&enttemp[i][0]);//dcircle->ParseValue[i+1]);

            if (p == 0)
            {
                dtemp = 0;
            }
            else
            {
                dtemp = atof(p+4);
                *(p+4) = ' ';//removes value from parsed string in case value
            }            //matches one of the identifiers,i.e. 10 20 11 21

            switch (i)
            {
            case 0:
                dllTemp->dline->startx=dtemp;
                break;
            case 1:
                dllTemp->dline->starty=dtemp;
                break;
            case 2:
                dllTemp->dline->endx=dtemp;
                break;
            case 3:
                dllTemp->dline->endy=dtemp;
                break;
            }
        }
    }

    if (t == circle)
    {
        for (int i = 0; i<3; i++)
        {
            char enttemp[3][5] = {{" 10 "},{" 20 "},{" 40 "}};
            p = strstr(ctemp,&enttemp[i][0]);

            if (p == NULL)
            {
                dtemp = 0;
            }
            else
            {
                dtemp = atof(p+4);
                *(p+4) = ' ';
            }


            switch (i)
            {
            case 0:
                dllTemp->dcircle->xcenter=dtemp;
                break;
            case 1:
                dllTemp->dcircle->ycenter=dtemp;
                break;
            case 2:
                dllTemp->dcircle->radius=dtemp;
                break;
            }
        }
        //dllTemp->entityLength = computeArcLength(dllTemp->dcircle->radius,0,360);
    }

}

double DXFLinkList::computeArcLength(double radius,double startangle, double endangle)
{
    double circumference, sweepangle, retval;
    double degPI = M_PI/180;
    startangle *= degPI;
    endangle *= degPI;
    circumference = 2*M_PI*radius;
    if (startangle > endangle) //does the arc straddle the +x axis?
        sweepangle = 2*M_PI + (endangle - startangle); //yes
    else
        sweepangle = endangle - startangle; //no
    retval = circumference * sweepangle/(2*M_PI);
    return retval;

}

double DXFLinkList::computeLineLength(double xstart, double xend, double ystart, double yend)
{
    static double length;
    double dx,dy;
    dx = xend - xstart;
    dx *= dx;
    dy = yend - ystart;
    dy *= dy;
    length = sqrt(dx + dy);
    return length;
}

