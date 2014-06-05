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
#include "imgxlate.h"
imageTranslation::imageTranslation(double left, double top, double right, double bottom)
	{
	Left = left; Top = top; Bottom = bottom; Right = right;
	dxy = cxy = NULL;
	dxy = new deltaXY;
	cxy = new deltaXY;
	}
void imageTranslation::computeDelta(double left, double top, double right, double bottom)
	{
	double centerx, centerx2, centery, centery2;
	centerx = (Right - Left)/2 + Left;
	centerx2 = (right - left)/2 + left;
	centery = (Top - Bottom)/2 + Bottom;
	centery2 = (top - bottom)/2 + bottom;
	deltax = centerx - centerx2;
	deltay = centery - centery2;
	dxy->dx = deltax;
	dxy->dy = deltay;
	cxy->dx = centerx2;
	cxy->dy = centery2;
	}
