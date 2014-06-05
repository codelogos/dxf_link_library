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
    plCurrent = plTemp =	plHead = plTail = NULL;
    if (refcount > 0)
    {
        darc = new DXFarc;

        dline = new DXFline;

        dcircle = new DXFcircle;
    }
    refcount++;
    //dpoly = new DXFpolyline;
}

DXFLinkList::~DXFLinkList()
{
    /*static int i;
    fileIO fio("c:\\crapola.txt",O_APPEND);
    char buffer[128];*/
    //cout<<"destructor called"<<endl;
    refcount--;
    if (darc)
        delete darc;
    if (dline)
        delete dline;
    if (dcircle)
        delete dcircle;
    while (plHead)
    {
        plTemp = plHead->next;
        delete plHead;
        plHead = plTemp;
    }

    while (dllHead)
    {

        //	fio.fileWrite(&fio,itoa(i,buffer,10),O_APPEND);
        //MessageBox(NULL,"43","",MB_OK);
        temp = dllHead->next;
        //MessageBox(NULL,"45","",MB_OK);
        /*delete head->darc;
        delete head->dline;
        delete head->dcircle;*/
//		if (head->plHead)
//			plHead->~DXFpolyline(plHead);
        delete dllHead;
        //count--;
        //cout<<head->next<<endl;
        //cout<<"node deleted count:"<<count<<endl;
        //MessageBox(NULL,"47","",MB_OK);
        dllHead=temp;
    }
    //cout<<refcount<<endl;
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
void DXFLinkList::addnode(type t, char* ctemp)
{
    static int blop;
    char* p,* tptr = NULL;
    double dtemp;
    static double totalPolyLength;
    static int joinVertex;
//	char temp2[256];
    char temp2[40];
    temp = new DXFLinkList();

//	fileIO fio("f:\\bc45\\bin\\death.txt",O_APPEND);
//	fio.fileWrite(&fio,ctemp,0);
    /*	if (t == vertex)
       	{
       	joinVertex = 1;
       	}
       if (joinVertex == 1)
       		temp->joinVertex = 1;
     */
    if (dllHead == NULL)
    {
        dllHead = temp;

        //cout<<"got to line 64"<<endl;
        dllHead->next = NULL;
        dllHead->prev = NULL;
        dllTail = dllHead;
        //printf("previous node:%p\n",temp->next);
        //printf("current node:%p\n",temp);
        //printf("%p\n",head);
        dllTemp = dllHead;
        dllTemp->current = dllHead;
    }
    else
    {
        //cout<<"got to line 80"<<endl;
        dllTemp->next = temp;
        temp->prev = dllTemp;
        temp->current = dllTemp->current;
        dllTemp = temp;
        dllTail = temp;
        //cout<<"previous node"<<temp->prev<<endl;
        //cin.get();
        //	printf("previous node next:%p\n",temp->prev->next);
        //printf("current node:%p\n",temp);
        //for(long int z=0; z< 100000000;z++);
    }

//	joinVertex = 0;
    dllTemp->entityLength = 0;
    dllTemp->entityType = t;
    //cout<<"^^^^^^^"<<temp->entityType<<" &&&&*****:"<<endl;
    if (t == polyline)// || t == vertex)
    {
        //MessageBox(NULL,ctemp,"",MB_OK);
        char enttemp[4][11] = {{" 10 "},{" 20 "},{"CONTINUOUS"},{"SEQ"}};
        char *p;
        char lastXVal[30], lastYVal[30];

        do
        {
            if (dllTemp->plHead == NULL )
            {
                dllTemp->plHead = new DXFpolyline();
                dllTemp->plTemp = dllTemp->plHead;
                dllTemp->plTail = dllTemp->plHead;
                //if (t == vertex)
                //	dllTemp->plHead->joinVertex = 1;
            }
            else
            {
                dllTemp->plCurrent = new DXFpolyline();
                dllTemp->plTail = dllTemp->plCurrent;
                if (dllTemp->plCurrent == NULL)
                    break;

                dllTemp->plTemp->next = dllTemp->plCurrent;
                dllTemp->plCurrent->prev = dllTemp->plTemp;
                dllTemp->plTemp = dllTemp->plCurrent;
            }
            char *_temp, *endptr;

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
                _temp = strchr(p+4,' ');
                *_temp = '\0';
                strcpy(temp2,p+4);
                tptr = p;

                *_temp = ' ';
                if (i == 0)
                {
                    //memccpy(pl->xcoor,temp2, ' ', strlen(temp2));
                    /*if (strstr(temp2,"-2.622201"))
                    	{
                    	int b;
                    	b = 12;
                    	}*/
                    strcpy(dllTemp->plTemp->xcoor,temp2);
                    strcpy(lastXVal,temp2);
                }
                if (i == 1)
                {
                    strcpy(dllTemp->plTemp->ycoor,temp2);
                    strcpy(lastYVal,temp2);
                }
            }
            if (tptr)
                ctemp = tptr+2;

        }
        while (strstr(p,enttemp[3]) - strstr(p,enttemp[1]) > 30);
        dllTemp->plTemp = dllTemp->plHead;
        static int i;
        while (dllTemp->plTemp)
        {
            if (i == 0)
            {
                strcpy(lastXVal,dllTemp->plTemp->xcoor);
                strcpy(lastYVal,dllTemp->plTemp->ycoor);
                i = 1;
            }
            else
            {
                totalPolyLength += computeLineLength(atof(dllTemp->plTemp->xcoor),
                                                     atof(lastXVal),atof(dllTemp->plTemp->ycoor),
                                                     atof(lastYVal));
                strcpy(lastXVal,dllTemp->plTemp->xcoor);
                strcpy(lastYVal,dllTemp->plTemp->ycoor);

            }
            dllTemp->plTemp = dllTemp->plTemp->next;
        }
        //sprintf(lastXVal,"%f",totalPolyLength);
        //MessageBox(NULL,lastXVal,"",MB_OK);
        dllTemp->entityLength = totalPolyLength;
        i = totalPolyLength = 0;
    }

    if (t == arc)
    {
        //printf("doin the arc\n");
        char enttemp[5][5] = {{" 10 "},{" 20 "},{" 40 "},{" 50 "},{" 51 "}};

        //fio.fileWrite(&fio,p,1);

        //sprintf(temp2,"%s",p);

        //fio.fileWrite(&fio,temp2,1);

        for (int i = 0; i<5; i++)
        {

            p = strstr(ctemp,enttemp[i]);//dcircle->ParseValue[i+1]);

            if (p == NULL)
            {

                dtemp = 0;
                //logerr
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

                //fio.fileWrite(&fio,ctemp,1);
                break;
            case 1:
                dllTemp->darc->ycenter=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->ycenter));
                D(fio.fileWrite(&fio,temp2,1));

                //fio.fileWrite(&fio,ctemp,1);
                break;

            case 2:
                dllTemp->darc->radius=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->radius));
                D(fio.fileWrite(&fio,temp2,1));

                //fio.fileWrite(&fio,ctemp,1);
                break;
            case 3:
                dllTemp->darc->startangle=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->startangle));
                D(fio.fileWrite(&fio,temp2,1));

                //fio.fileWrite(&fio,ctemp,1);
                break;
            case 4:
                dllTemp->darc->endangle=dtemp;
                D(sprintf(temp2,"%f",dllTemp->darc->endangle));
                D(fio.fileWrite(&fio,temp2,1));

                //fio.fileWrite(&fio,ctemp,1);
                break;
            }


        }
        dllTemp->entityLength = computeArcLength(dllTemp->darc->radius,
                                dllTemp->darc->startangle,
                                dllTemp->darc->endangle);
        D(fio.fileWrite(" ",1));
    }

    if (t == line)
    {
        //printf("doin the line\n");
        for (int i = 0; i<4; i++)
        {
            char enttemp[4][5] = {{" 10 "},{" 20 "},{" 11 "},{" 21 "}};
            p = strstr(ctemp,&enttemp[i][0]);//dcircle->ParseValue[i+1]);

            if (p == 0)
            {
                dtemp = 0;
                //logerr
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
    if (t == line)
    {
        double intermediateVal;
        if (DLLINE(startx) - DLLINE(endx) == 0 && DLLINE(starty) - DLLINE(endy) == 0)
            dllTemp->entityType = invalidline;
        else
        {
            intermediateVal = computeLineLength(dllTemp->dline->startx,
                                                dllTemp->dline->endx,
                                                dllTemp->dline->starty,
                                                dllTemp->dline->endy);
            dllTemp->entityLength = intermediateVal;
        }
    }

    if (t == circle)
    {
        //	printf("doin the circle\n");
        for (int i = 0; i<3; i++)
        {
            char enttemp[3][5] = {{" 10 "},{" 20 "},{" 40 "}};
            p = strstr(ctemp,&enttemp[i][0]);//dcircle->ParseValue[i+1]);

            if (p == NULL)
            {
                dtemp = 0;
                //logerr
            }
            else
            {
                dtemp = atof(p+4);
                *(p+4) = ' ';
            }

            char temp[256];
            //strncpy(temp,p+2,10);
//                        sprintf(temp,"%d",i);//enttemp[i]);//*p);//enttemp[i][0]);//(char *)p);//(char *)dcircle->ParseValue[i+1]);//,current->dcircle->xcenter);
//			MessageBox(NULL,temp,"",MB_OK);


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
        dllTemp->entityLength = computeArcLength(dllTemp->dcircle->radius,0,360);
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

double DXFLinkList::computeLineLength(double xstart,double xend, double ystart, double yend)
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
