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
#include "dxfextnt.h"
#include <math.h>
#include "fileio.h"
#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif
extern int extentslockout;
void DXFImageExtents::computeExtents()
	{

		DXFarc* dxfarc;
		DXFline* dxfline;
		DXFcircle* dxfcircle;
		DXFpolyline* dxfpoly;
		type t;
		int quad1,quad2,quad,temp;


		double radius, a, b, tx,ty,start_angle,
				 end_angle,rl,rr,ru,rd,cx,cy;
		int cw = 0;

	do
		{
		t = dll->returnType();

		if (t == circle)
				{
				dxfcircle = dll->returnCircle();
				radius = dxfcircle->radius*scale;
				rl = dxfcircle->xcenter*scale - radius; rr = dxfcircle->xcenter*scale + radius; ru = dxfcircle->ycenter*scale + radius; rd = dxfcircle->ycenter*scale - radius;
				if (extentslockout == 0)
					{
					pt.left = rl;
					pt.right = rr;
					pt.top = ru;
					pt.bottom = rd;
					right = rr;
					left = rl;
					top = ru;
					bottom = rd;
					extentslockout = 1;
					}

				assignment(rr,rl,ru,rd);
				}

		if (t == arc)
			{
			dxfarc = dll->returnArc();

			radius = dxfarc->radius*scale;
			rl = dxfarc->xcenter*scale - radius; rr = dxfarc->xcenter*scale + radius; ru = dxfarc->ycenter*scale + radius; rd = dxfarc->ycenter*scale - radius;
			start_angle = dxfarc->startangle;
			end_angle = dxfarc->endangle;

			cx = dxfarc->xcenter*scale;
			cy = dxfarc->ycenter*scale;

			D(cout<<"dxfxtnt 42"<<endl);
			tx = (float)(radius* cos(start_angle/180*PI)+cx);
			ty = (float)(radius* sin(start_angle/180*PI)+cy);
			a = (float)(radius* cos(end_angle/180*PI)+cx);
			b = (float)(radius* sin(end_angle/180*PI)+cy);
			D(cout<<"dxfxtnt 47"<<endl);
			if (extentslockout == 0)
				{
				pt.left = pt.right = tx; pt.top = pt.bottom = ty;
				right = left = tx;
				top = bottom = ty;
				extentslockout = 1;
				}
			quad1=get_quad (start_angle);
			quad2=get_quad (end_angle);
			quad = abs(quad1 - quad2);
			D(cout<<"dxfxtnt 57"<<endl);
			}


	if (t == arc && quad1 < 5 && quad2<5 )
	{
		D(cout<<"dxfxtnt 63"<<endl);
		switch (quad)
		{
		case 0:

			D(cout<<"dxfxtnt 68"<<endl);
				if ((start_angle<end_angle&&cw ==1)||(start_angle>end_angle&&cw == 0))
					assignment(rr,rl,ru,rd);

				else if ((quad1 ==1 && cw == 0)||(quad1==3 && cw==1))
					assignment(tx,a,b,ty);
				else if ((quad1 == 1 && cw == 1)||(quad1 == 3 && cw == 0))
					assignment(a,tx,ty,b);
				else if ((quad1 == 2 && cw == 0)||(quad1 == 4 && cw == 1))
					assignment(tx,a,ty,b);
				else if ((quad1 == 2 && cw == 1)||(quad1 == 4 && cw == 0))
					assignment(a,tx,b,ty);

				break;

		case 1:
		if (quad1 == 1)
			{
			D(cout<<"dxfxtnt 86"<<endl);
				if (cw == 0)
					{
					D(cout<<"dxfxtnt 89"<<endl);

                                        D(cout<<ty<<" "<<b<<" "<<bottom<<" "<<ty<<endl);
					if (ty < b && bottom>ty)
						assignment(tx,a,ru,ty);
					else if (bottom>b)
						assignment(tx,a,ru,b);
					else
						assignment(tx,a,ru,bottom);
					}
				else
					{
					D(cout<<"dxfxtnt 99"<<endl);
					if (b>ty  && top <b)
						assignment(rr,rl,b,rd);

					else if (top < ty)
						assignment(rr,rl,ty,rd);
					else
						assignment(rr,rl,top,rd);
					}

			}

		else if (quad1 ==2)
			{

					if (start_angle > end_angle && cw ==0)
						{

						if (b > ty && top<b)
							assignment(rr,rl,b,rd);
						else if (top<ty)
							assignment(rr,rl,ty,rd);
						else
							assignment(rr,rl,top,rd);

						}


					else if (start_angle > end_angle && cw == 1)

						{
						if (ty < b && bottom>ty)
							assignment(a,tx,ru,ty);
						else if (bottom>b)
							assignment(a,tx,ru,b);
						else
							assignment(a,tx,ru,bottom);
						}

					else if (start_angle < end_angle && cw ==0)
						{


						if (tx > a && right< tx)
							assignment(tx,rl,ty,b);

						else if (right<a)
							assignment(a,rl,ty,b);

						else
							assignment(right,rl,ty,b);

						}


					else if (start_angle < end_angle && cw == 1)

						{
						if (tx < a && left>tx)
							assignment(rr,tx,ru,rd);

						else if (left>a)
							assignment(rr,a,ru,rd);
						else
							assignment(rr,left,b,ty);

						}
			}

		else if (quad1 == 3)
			{
					D(cout<<"dxfxtnt 169"<<endl);
					if (start_angle > end_angle && cw == 0)
						{

					 D(cout<<"dxfxtnt 173"<<endl);
						if (tx < a && left>tx)
							assignment(rr,tx,ru,rd);

						else if (left>a)
							assignment(rr,a,ru,rd);
						else
							assignment(rr,left,ru,rd);

						}
					else if (start_angle > end_angle && cw == 1)
						{
						D(cout<<"dxfxtnt 185"<<endl);
						if (tx > a && right<tx)
							assignment(tx,rl,b,ty);

						else if (right<a)
							assignment(a,rl,b,ty);
						else
							assignment(right,rl,b,ty);

						}
					else if (start_angle < end_angle && cw ==0)
						{

						if (ty > b && top<ty)
							assignment(a,tx,ty,rd);

						else if (top<b)
							assignment(a,tx,b,rd);
						else
							assignment(a,tx,top,rd);

						}


					else if (start_angle < end_angle && cw == 1)

						{
						if (ty < b && bottom>ty)
							assignment(rr,rl,ru,ty);

						else if (bottom>b)
							assignment(rr,rl,ru,b);
						else
							assignment(rr,rl,ru,bottom);

						}

			}
			else if (quad1 == 4)
			{
				if (cw == 0)
					{

					if (ty < b && bottom>ty)
						assignment(rr,rl,ru,ty);

					else if (bottom>b)
						assignment(rr,rl,ru,b);
					else
						assignment(rr,rl,ru,bottom);
					}
				else
					{
					if (ty>b && top < ty)
						assignment(tx,a,ty,rd);
					else if (top < b)
						assignment(tx,a,b,rd);
					else
						assignment(tx,a,top,rd);
					}

			}
			break;

		case 2:


			if (quad1 == 1)
			{

				if (cw ==1)
					{
					assignment(rr,a,ty,rd);
					}

				else
					{
					assignment(tx,rl,ru,b);
					}
			}
			else if (quad1 ==2)
			{
				if (cw ==1)
					{
					assignment(rr,tx,ru,b);

					}
				else
					{
					assignment(a,rl,ty,rd);

					}
			}

			else if (quad1 == 3)
			{
				if (cw ==1)
					{
					assignment(a,rl,ru,ty);
					}
				else
					{
					assignment(rr,tx,b,rd);

					}
			}
			else if (quad1 == 4)
			{
				if (cw ==1)
					{
					assignment(tx,rl,b,rd);
					}
				else
					{
					assignment(rr,a,ru,ty);
					}

			}
			break;


		case 3:
			if (quad1 == 1 && cw == 0)
				{
				if (tx > a && right<tx)
					assignment(tx,rl,ru,rd);
				else if (right<a)
					assignment(a,rl,ru,rd);
				else
					assignment(right,rl,b,ty);
				}
			else if (quad1 == 1 && cw == 1)
				{
				if (tx<a && left>tx)
					assignment(rr,tx,ty,b);

				else if (left>a)
					assignment(rr,a,ty,b);
				else
					assignment(rr,left,b,ty);
				}

			if (quad1 == 4 && cw == 0)
				{

				if (tx<a && left>tx)
					assignment(rr,tx,b,ty);

				else if (left>a)
					assignment(rr,a,b,ty);
				else
					assignment(rr,left,b,ty);
				}
			else if (quad1 == 4 && cw == 1)
				{

				if (tx > a && right < tx)
					assignment(tx,rl,ru,rd);

				else if (right<a)
					assignment(a,rl,ru,rd);

				else
					assignment(right,rl,ru,rd);
				}

			break;
		}

	}
	else if((quad1>5 && quad2 > 5 && quad1 != quad2)&& t == arc)
		{
		D(cout<<"dxfxtnt 355"<<endl);
		int a1=0,b1=0,c1=0,d1=0,a2 = 0,b2 = 0, c2 = 0, d2 = 0;

			if (quad ==1|| quad == 3)

			{

				if(quad1 == 10 && ((quad2 == 11 && cw==1)||(quad2 ==13 && cw ==0)))
					a1 = 1;
				else if(quad1 == 11 && ((quad2 == 12&&cw==1)||(quad2 == 10&&cw == 0)))
					b1 = 1;
				else if(quad1 == 12 && ((quad2 == 13&&cw == 1)||(quad2 == 11&&cw==0)))
					c1 = 1;
				else if(quad1 == 13 && ((quad2 == 10&&cw==1)||(quad2 == 12&&cw == 0)))
					d1 = 1;
										  if(quad1 == 10 && ((quad2 == 11 && cw==0)||(quad2 ==13 && cw ==1)))
					a2 = 1;
				else if(quad1 == 11 && ((quad2 == 12&&cw==0)||(quad2 == 10&&cw == 1)))
					b2 = 1;
				else if(quad1 == 12 && ((quad2 == 13&&cw == 0)||(quad2 == 11&&cw==1)))
					c2 = 1;
				else if(quad1 == 13 && ((quad2 == 10&&cw==0)||(quad2 == 12&&cw == 1)))
					d2 = 1;

				if (a1||b1||c1||d1)
					{
					assignment(rr,rl,ru,rd);
					}

				else if (a2||b2||c2||d2)
					{
					if ((quad1==10&&quad2==11) || (quad1 == 11&&quad2==10))
						{
						assignment(rr,cx,ru,cy);
						}
					else if ((quad1==10&&quad2==13) || (quad1 == 13&&quad2==10))
						{
						assignment(rr,cx,cy,rd);
						}
					else if ((quad1==11&&quad2==12) || (quad1 == 12&&quad2==11))
						{
						assignment(cx,rl,ru,cy);
						}
					else if ((quad1==12&&quad2==13) || (quad1 == 13&&quad2==12))
						{
						assignment(cx,rl,cy,rd);
						}
					}
				}
			else if (quad == 2)
					{
					if ((quad1 == 11 && quad2 == 13&&cw == 0)||(quad1 == 13 && quad2 == 11&&cw == 1))
						{
						assignment(cx,rl,ru,rd);
						}
					else if ((quad1 == 11 && quad2 == 13&&cw == 1)||(quad1 == 13 && quad2 == 11&&cw == 0))
						{
						assignment(rr,cx,ru,rd);
						}
					else if ((quad1 == 10 && quad2 == 12&&cw == 1)||(quad1 == 12 && quad2 == 10&&cw == 0))
						{
						assignment(rr,rl,cy,rd);
						}
					else if ((quad1 == 10 && quad2 == 12&&cw == 0)||(quad1 == 12 && quad2 == 10&&cw == 1))
						{
						assignment(rr,rl,ru,cy);

						}

			}
		}
	else if (((quad1<5&&quad2>5)||(quad1>5&&quad2<5))&& t == arc)
		{
		D(cout<<"dxfxtnt 428"<<endl);
		int a1 = 0, a2 = 0, a3 = 0, a4 = 0,b1 = 0, b2 = 0,b3 = 0,b4=0;

		if (quad1<5 && quad2 > 5)
			{

			temp = quad1;
			quad1 = quad2;
			quad2 = temp;
			cw = !cw;
			}

		if ((quad1 == 10&&quad2==1&&cw==0)||(quad1 == 10&&quad2==4&&cw==1))
			b1 = 1;
		if ((quad1 == 11&&quad2==1&&cw==1)||(quad1 == 11&&quad2==2&&cw==0))
			b2 = 1;
		if ((quad1 == 12&&quad2==2&&cw==1)||(quad1 == 12&&quad2==3&&cw==0))
			b3 = 1;
		if ((quad1 == 13&&quad2==4&&cw==0)||(quad1 == 13&&quad2==3&&cw==1))
			b4 = 1;

		if ((quad1==10 && quad2 == 1 && cw ==1)||(quad1==10 && quad2 == 3 && cw == 0))
			a1 = 1;
		if ((quad1==11 && quad2 == 2 && cw ==1)||(quad1==11 && quad2 == 1 && cw == 0))
			a2 = 1;
		if ((quad1==12 && quad2 == 2 && cw ==0)||(quad1==12 && quad2 == 3 && cw == 1))
			a3 = 1;
		if ((quad1==13 && quad2 == 4 && cw ==1)||(quad1==13 && quad2 == 3 && cw == 0))
			a4 = 1;

		if (a1||a2||a3||a4)
			{
			assignment(rr,rl,ru,rd);
			}
		else if (b1||b2||b3||b4)
			{
			if (tx>a&& tx>right)
				right =tx;
			else if (right<a)
				right = a;
			if (tx<b && tx<left)
				left = tx;
			else if (left>a)
				left = a;
			if (ty>b && top<ty)
				top = ty;
			else if (top<b)
				top = b;
			if (ty<b && bottom>ty)
				bottom = ty;
			else if (bottom>b)
				bottom = b;
			}
		else if ((quad1 == 10&&quad2 == 2&&cw == 1)||(quad1==12&&quad2==1&&cw==0))
			{

			if (ty>b && top<ty)
				assignment(rr,rl,ty,rd);

			else if (top<b)
				assignment(rr,rl,b,rd);
			else
				assignment(rr,rl,top,rd);
			}

		else if ((quad1 == 10&&quad2 == 3&&cw == 0)||(quad1 == 12&&quad2 == 4&&cw == 1))
			{
			if (ty<b && bottom>ty)
				assignment(rr,rl,ru,ty);

			else if (bottom>b)
				assignment(rr,rl,ru,b);
			else
				assignment(rr,rl,ru,bottom);
			}

		else if ((quad1 == 13&&quad2 == 1&&cw == 1)||(quad1 == 11&&quad2 == 4&&cw == 0))
			{
			if (tx>a && right<tx)
				assignment(tx,rl,ru,rd);

			else if (right<a)
				assignment(a,rl,ru,rd);
			else
				assignment(right,rl,ru,rd);
			}

		else if ((quad1 == 11&&quad2 == 3&&cw == 1)||(quad1 == 13&&quad2 == 2&&cw == 0))
			{
			if (tx<a && left>tx)
				assignment(rr,tx,ru,rd);

			else if (left>a)
				assignment(rr,a,ru,rd);
			else
				assignment(rr,left,ru,rd);
			}

		else if (quad1 == 10&&quad2 == 2&&cw == 0)
			{
			assignment(rr,a,ru,cy);

			}

		else if (quad1 == 10&&quad2 == 2&&cw == 1)
			{
			assignment(rr,a,cy,rd);
			}

		else if (quad1 == 11&&quad2 == 3&&cw == 0)
			{
			assignment(cx,rr,ru,b);
			}

		else if (quad1 == 11&&quad2 == 4&&cw == 1)
			{
			assignment(rr,cx,ru,b);
			}

		else if (quad1 == 12&&quad2 == 4&&cw == 0)
			{
			assignment(a,rl,cy,rd);
			}

		else if (quad1 == 12&&quad2 == 1&&cw == 1)
			{
			assignment(a,rl,ru, cy);
			}
		else if (quad1 == 13&&quad2 == 1&&cw == 0)
			{
			assignment(rr,cx,b,rd);
			}
		else if (quad1 == 13&&quad2 == 2&&cw == 1)
			{
			assignment(cx,rl,b,rd);
			}
		}
	else if ( t == line)
		{

		dxfline = dll->returnSegment();

		if (extentslockout == 0)
			{
			pt.left = pt.right = dxfline->startx*scale; pt.top = pt.bottom = dxfline->starty*scale;
			right = left = dxfline->startx*scale;
			top = bottom = dxfline->starty*scale;
			extentslockout = 1;
			}
		if (dxfline->startx == 0.0 && dxfline->starty == 0.0 && dxfline->endy == 0.0 && dxfline->endx == 0.0);
		else
			{
			assignment(dxfline->startx*scale, dxfline->startx*scale, dxfline->starty*scale, dxfline->starty*scale);
			assignment(dxfline->endx*scale, dxfline->endx*scale, dxfline->endy*scale, dxfline->endy*scale);
			}
		}

	else if (t == polyline || t == lwpolyline)
		{

		dxfpoly = dll->returnPolyLine();
		while (dxfpoly)
			{

			double xcoor, ycoor;
				xcoor = atof(dxfpoly->xcoor)*scale;
				ycoor = atof(dxfpoly->ycoor)*scale;

			if (extentslockout == 0)
				{
				pt.left = pt.right = xcoor; pt.top = pt.bottom = ycoor;
				right = left = xcoor;
				top = bottom = ycoor;
				extentslockout = 1;
				}
			assignment(xcoor, xcoor, ycoor, ycoor);
			dxfpoly = dxfpoly->next;
			}
		}
	else if (t == invalidline);
		D(cout<<"compute extents587"<<endl);
	}	while (dll = dll->incrementNode());
 }


