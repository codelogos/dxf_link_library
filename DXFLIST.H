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
#include <iostream>
#include "dxfstrct.h"
#include "fileio.h"
//#define NULL 0
#include <stdio.h>
#ifndef DXFLIST_H
#define DXFLIST_H
#include <iostream>
#include <stdlib.h>

extern double scaleValue;
static int refcount;
static int entry;
class DXFLinkList
	{
	char* block;
	DXFLinkList* current,* temp,* prev,* next;
	DXFarc* darc;
	DXFline* dline;
	DXFcircle* dcircle;
	DXFpl *plTemp,*plHead,*plTail,*plCurrent;
	int joinVertex;
    DXFLinkList* dllHead, * dllTail,* dllTemp;
    bool closed = false;
	public:

	double entityLength;
	type entityType;

	double computeArcLength(double radius,double startangle, double endangle);
	double computeLineLength(double xstart,double ystart, double xend, double yend);

	~DXFLinkList();

	DXFLinkList();

	int isVertexJoined(DXFLinkList *dll);

	type returnType() const
					 {
					 return this->entityType;
					 };

	DXFarc* returnArc() const
		{
     	return this->darc;
		};

	DXFpolyline* returnPolyLine()
		{
		return this->plHead;
		};

	DXFline* returnSegment() const
		{
		return this->dline;
		};

	DXFcircle* returnCircle() const
		{
		return this->dcircle;
		};

	DXFpolyline* returnPolyLine() const
		{
		return this->plHead;
		};

int isNextNodeNull()
	{
	if (this->next == NULL)
		return 1;
	else
		return 0;
	}

DXFLinkList* incrementNode() const
		{
		return this->next;
		}

DXFLinkList* decrementNode() const
		{
		return this->prev;
		}
DXFLinkList* returnHead() const
		{
		return this->dllHead;
		}

	void addnode(type,const char*);
	void addnodepoly(char*);
	void removenode(){};
	};

#endif

