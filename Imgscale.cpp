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
#include "imgscale.h"
#include <iostream>
#include <windows.h>
imageScaling::imageScaling(double Left, double Top, double Right, double Bottom)
{
    left = Left;
    top = Top;
    right = Right;
    bottom = Bottom;
}

float imageScaling::computeScale(double Left, double Top, double Right, double Bottom)
{
    char temp[256];
    double horizontalDim, verticalDim;
    double horizontalDimVwr, verticalDimVwr;
    double hRatio, vRatio;
    horizontalDim = Right - Left;

    verticalDim = Top - Bottom;
    horizontalDimVwr = right - left;
    verticalDimVwr = top - bottom;

    hRatio = fabs(horizontalDimVwr/horizontalDim);

    vRatio = fabs(verticalDimVwr/verticalDim);

    {
        if (vRatio > hRatio)
        {
            scaleFactor = hRatio;
        }
        else
        {
            scaleFactor = verticalDimVwr/verticalDim;
        }
        //sprintf(temp,"%f,%f", horizontalDimVwr,horizontalDim);
        //MessageBox(NULL,temp,"",MB_OK);
        float retval = fabs(scaleFactor);
        return retval;
    }
    /*else
    	{
                if (verticalDim > horizontalDim)
    		scaleFactor = verticalDimVwr/verticalDim;
                else
    	    	scaleFactor = horizontalDimVwr/horizontalDim;
                }*/
}
