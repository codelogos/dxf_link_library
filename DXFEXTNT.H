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

#ifndef DXFEXTNT_H
#define DXFEXTNT_H
const double PI = 3.1415926;
const double PIDIV180 = 0.017453;
const double PIMUL2 = 6.283185;
const double PIDIV2 = 1.570796;
const double PIMUL1_5 = 4.712388;
typedef struct
	{
	double left,right,top,bottom;
	}RECTEXTENTS;
enum extents {left,top,bottom,right};
class DXFImageExtents
	{
	protected:
	double left, top, bottom, right, scale;

	DXFLinkList* dll;
	RECTEXTENTS pt,pt1;
	public:
     //DXFImageExtents(){};
	DXFImageExtents(DXFLinkList* DLL):dll(DLL){left=10000000;top=-10000000; bottom=10000000;  right=-10000000;scale =1;}

	RECTEXTENTS* returnDXFImageExtents(){return &pt;}
	void computeExtents();
	RECTEXTENTS returnExtents()
			{
		pt1.left = this->left;
		pt1.top = this->top;
		pt1.bottom = this->bottom;
		pt1.right = this->right;
		return pt1;
		}
	void resetCoordinates()
		{
		left = top = bottom = top = 0.0;
		}
	void assignment(float Right, float Left, float Top, float Bottom)
		{
		//cout<<"assignment 22"<<endl;
		if (right < Right)
			{
			right = Right;
			pt.right = right;
			}
		if (left > Left)
			{
			left = Left;
			pt.left = left;
			}
		if (top < Top)
			{
			top = Top;
			pt.top = top;
			}
		if (bottom >= Bottom)
			{

			bottom = Bottom;
			pt.bottom = bottom;
			}
		}
	int get_quad(double Angle)
	{
	Angle = Angle*PIDIV180;

	if (Angle > PIMUL2)
		Angle = Angle - PIMUL2;
	if (Angle < PIDIV2 && Angle > 0)
		return(1);
	else if (Angle < PI && Angle > PIDIV2)
		return(2);
	else if (Angle < PIMUL1_5 && Angle > PI)
		return(3);
	else if (Angle < PIMUL2 && Angle > PIMUL1_5)
		return(4);
	else if (Angle == 0.0)
		return(10);
	else if (Angle == PIDIV2)
		return(11);
	else if (Angle == PI)
		return(12);
	else if (Angle == PIMUL1_5)
		return(13);
	else if (Angle == PIMUL2)
		return(10);
	else
		{

		return(-1);
		}
	//cout<<"assignment 69"<<endl;
	}

void	assignPrivatePtr(DXFLinkList * DXFLL){dll = DXFLL;}

	};
#endif
