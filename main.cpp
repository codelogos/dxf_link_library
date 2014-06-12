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
#include "main.h"

void DLLEXPORT readInAndBuildList(char * name)
{
    dIO = new DXFio(name,ios::app);//(const char *const) argv[1],1);//"c:\\98\\bh517.dxf",1);//cowbo379.dxf",1);
    dlink = new DXFLinkList();

    DXFIdentEntity * dxfIdent;
    dxfIdent = new DXFIdentEntity();

    dIO->fileRead();
    int j = 0, i = 0;
    int m = dIO->returnCount();

    while (j <= m)
    {
        dxfIdent->assignCharPtr(dIO->returnPointer(j));

        do
        {
            i = dxfIdent->parsePtrToData();

            if (i)
            {
                dlink->addnode(dxfIdent->returnEntityType(),dxfIdent->returnEntityData());

//MessageBox(NULL,"test.c_str()","",MB_OK);
            }
        }
        while (i == 1);
        j++;
    }
}

void computeExtents()
{
    dxfextents = new DXFImageExtents(dlink->dllHead); //dlink->returnCurrentNode(dlink););
    dxfextents->computeExtents();
}

BSTR DLLEXPORT returnExtents()
{
    computeExtents();
    EXTENT_RECT* rc = (EXTENT_RECT *)dxfextents->returnDXFImageExtents();


    sprintf(imgExtents,"%f,%f,%f,%f",rc->left,rc->top,rc->right,rc->bottom);
//MessageBox(NULL,imgExtents,"",MB_OK);
    /*stringstream sstr;
    static string serialize;
    sstr<<rc->left;
    //string test = sstr.str();
    //MessageBox(NULL,"test.c_str(),"",MB_OK);
    serialize = sstr.str() +",";
    sstr.clear();
    sstr<<rc->top;
    serialize += sstr.str() + ",";
    sstr.clear();
    sstr<<rc->bottom;
    serialize += sstr.str() + ",";
    sstr.clear();
    sstr<<rc->right;
    serialize += sstr.str() + ",";
    */

//char *str = (char *)serialize.c_str();

    glb_bstr = (BSTR)imgExtents;
//MessageBox(NULL,serialize.c_str(),"",MB_OK);
//MessageBox(NULL,serialize.c_str(),"",MB_OK);

    return  glb_bstr;
}

BSTR DLLEXPORT returnEntity()
{
    DXFarc *Arc;
    DXFline *Line;
    DXFcircle *Circle;
    DXFpolyline *pl;

    BSTR ebstr;
    //tmplink = dlink->returnHead();
    if (tmplink->entityType == CIRCLE)
    {
        Circle = tmplink->returnCircle();
        //circle(Circle->xcenter*scale+dx+deltax,bottom - Circle->ycenter*scale+dy+deltay,Circle->radius*scale);
        //gP->drawArc(0,0,circle->xcenter*scale+dx+deltax,circle->ycenter*scale+dy+deltay,circle->radius*scale,0,1);
        /*fio.fileWrite("CIRCLE",0);
        fio.fileWrite(Circle->xcenter*scale+dx+deltax,0);
        fio.fileWrite(bottom - Circle->ycenter*scale+dy+deltay,0);
        fio.fileWrite(Circle->radius*scale,0);*/
        sprintf(entityBuffer,"Circle,%f,%f,%f",Circle->xcenter,Circle->ycenter,Circle->radius);
    }

    if (tmplink->entityType == LINE)
    {
        Line = tmplink->returnSegment();
        /*fio.fileWrite("LINE",0);
        fio.fileWrite(Line->startx);
        fio.fileWrite(Line->starty);
        	fio.fileWrite(Line->endx);
        	fio.fileWrite(Line->endy);*/

        //line(Line->startx*scale+dx+deltax,bottom - (Line->starty*scale+dy+deltay),Line->endx*scale+dx+deltax,bottom-(Line->endy*scale+dy+deltay));
        //gP->drawSegment(line->startx*scale+dx+deltax,line->starty*scale+dy+deltay,line->endx*scale+dx+deltax,line->endy*scale+dy+deltay);
        sprintf(entityBuffer,"Line,%f,%f,%f,%f",Line->startx,Line->starty,Line->endx,Line->endy);
    }
    if (tmplink->entityType == ARC)
    {
        //fileIO fio("test.txt",0);
        Arc = tmplink->returnArc();

        /* fio.fileWrite(Arc->startangle);
         fio.fileWrite(Arc->endangle);
         fio.fileWrite(Arc->xcenter);//*scale+dx+deltax,0);
         fio.fileWrite(Arc->ycenter);//*scale+dy+deltay),0);
         fio.fileWrite(Arc->radius);
        */

        sprintf(entityBuffer,"Arc,%f,%f,%f,%f,%f",Arc->startangle,Arc->endangle,Arc->radius,Arc->xcenter,Arc->ycenter);

        /*if (abs((int)Arc->startangle - (int)Arc->endangle) < 5)
        {
            float x1,y1,x2,y2;
            x1 = Arc->radius * cos(Arc->startangle*PI/180)+ Arc->xcenter;
            y1 = Arc->radius * sin(Arc->startangle*PI/180)+ Arc->ycenter;
            x2 = Arc->radius * cos(Arc->endangle*PI/180)+ Arc->xcenter;
            y2 = Arc->radius * sin(Arc->endangle*PI/180)+ Arc->ycenter;

            //cout<<Arc->startangle<<" "<<y1<<"   "<<x2<<" "<<y2<<endl;
            //line(x1*scale+dx+deltax,bottom - (y1*scale+dy+deltay),x2*scale+dx+deltax,bottom-(y2*scale+dy+deltay));
            //	cin.get();
        }
        else;*/


        //	arc(Arc->xcenter*scale+dx+deltax,bottom - (Arc->ycenter*scale+dy+deltay),Arc->startangle,Arc->endangle,Arc->radius*scale);
        //gP->drawArc(arc->startangle,arc->endangle,arc->xcenter*scale+dx+deltax,arc->ycenter*scale+dy+deltay,arc->radius*scale,0,0);
    }
    if (tmplink->entityType == POLYLINE)
    {
        pl = tmplink->returnPolyLine();
        do
        {
            static int reset;

            if (pl->next == NULL)
                reset = 1;
            else
                reset = 0;

            //gP->drawPolyLine(atof(pl->xcoor)*scale+dx+deltax,atof(pl->ycoor)*scale+dy+deltay,reset);
        }
        while (pl = pl->next);
    }
    ebstr = (BSTR)entityBuffer;
    return ebstr;
}

void DLLEXPORT resetList()
{
    tmplink = dlink->returnHead();
    // if (tmplink == NULL)
}

bool DLLEXPORT incrementNode()
{


    tmplink = tmplink->incrementNode();

    if (tmplink == NULL)
    {
        //  MessageBox(NULL,"gothere","",MB_OK);
        return false;
    }
    else
        return true;

}

void DLLEXPORT cleanUp()
{
    delete dIO;
    delete dlink;
    if (dxfextents)
        delete dxfextents;
}

BSTR DLLEXPORT returnScaleDelta(double Left, double Top, double Right, double Bottom)
{

    BSTR ebstr;
    computeExtents();
    EXTENT_RECT* rc = (EXTENT_RECT *)dxfextents->returnDXFImageExtents();

    deltaXY *dxy;
    char test[256];



    imageScaling imgS(Left,Top,Right,Bottom);

//sprintf(test,"%f,%f,%f,%f",rc->eft,rc->top,rc->right,rc->bottom);
    float scale = imgS.computeScale(rc->left,rc->top,rc->right,rc->bottom);

//sprintf(test,"%f",scale);
//MessageBox(NULL,test,"",MB_OK);


    imageTranslation imgT(Left,Top,Right,Bottom);

    imgT.computeDelta(rc->left*scale,rc->top*scale,rc->right*scale,rc->bottom*scale);
    dxy = (deltaXY *)imgT.returnDeltaXY();

    sprintf(deltaScale,"%f,%f,%f",scale,dxy->dx,dxy->dy);
//sprintf(test,"%f",scale);
//MessageBox(NULL,deltaScale,"",MB_OK);

    ebstr = (BSTR)deltaScale;

    sprintf(test,"%s",deltaScale);
//MessageBox(NULL,test,"",MB_OK);


    return ebstr;
}

