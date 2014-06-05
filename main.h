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
#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dxfident.h"
#include "dxfio.h"
#include "dxflist.h"
#include "dxfextnt.h"
#include "IMGSCALE.H"
#include "IMGXLATE.H"

#define DLLEXPORT __declspec(dllexport)
#define MAXBUFFER 256

typedef struct
		{
		double dx,dy;
		} deltaXY;


using namespace std;


char imgExtents[MAXBUFFER];
char entityBuffer[MAXBUFFER];
char deltaScale[MAXBUFFER];

BSTR glb_bstr = NULL;

enum {CIRCLE,LINE,ARC,POLYLINE};

int extentslockout;

typedef struct
{
    double left,right,top,bottom;
}EXTENT_RECT;

DXFio* dIO;
DXFLinkList* dlink, *tmplink;
DXFImageExtents* dxfextents;

#ifdef __cplusplus
extern "C"
{
#endif

void DLLEXPORT readInAndBuildList(char *);
void computeExtents();
BSTR DLLEXPORT returnExtents();
BSTR DLLEXPORT returnEntity();
BSTR DLLEXPORT returnScaleDelta(double,double,double,double);
bool DLLEXPORT incrementNode();
void DLLEXPORT resetList();
void DLLEXPORT cleanUp();
#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
