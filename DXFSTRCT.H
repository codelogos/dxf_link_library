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
#ifndef DXFSTRCT_H
#define DXFSTRCT_H
enum type{circle,lwpolyline,polyline,line,arc,vertex,invalidcircle,invalidline,invalidarc};

typedef struct
        {
        float xcenter, ycenter;
        } DXFpoint;
typedef struct
	 {
	 float xcenter, ycenter, startangle, endangle, radius;
	 } DXFarc;

typedef struct
	 {
	 float startx, starty, endx,endy;
	 } DXFline;

typedef struct
	 {
	 float xcenter, ycenter, radius;
	 } DXFcircle;

typedef struct DXFpolyline
	{
	DXFpolyline* prev,* next,* temp;
	int joinVertex;
	bool closed;
	char xcoor[64],ycoor[64];
	DXFpolyline()
		{
		prev = next = temp = NULL;
		}


	} DXFpl;


#endif




