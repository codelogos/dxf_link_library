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
#include "fileio.h"
#include <ctime>

void buildEntities(const char * const name)
{
    //std::clock_t start;
    //fileIO fio("c:/temp/profile.txt",ios::app);
    //double duration;
    dIO = new DXFio(name,ios::app);
  //  duration = (double) ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    //fio.fileWrite("dio");
    //fio.fileWrite(duration);

    dlink = new DXFLinkList();
    //duration = (double) ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    //fio.fileWrite("create link list");
    //fio.fileWrite(duration);


    DXFIdentEntity * dxfIdent;
    dxfIdent = new DXFIdentEntity();
//
    //duration = (double) ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    //fio.fileWrite("create ident entity");
    //fio.fileWrite(duration);


    dIO->fileRead();

 //   duration = (double) ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    //fio.fileWrite("reading input file");
    //fio.fileWrite(duration);
    short retVal;
    //char ch[2];
    dxfIdent->assignGeometry(dIO->returnGeometry());
    //fio.fileWrite("assigned geometry");
    //char ch[1];
        do
        {

            retVal = (int)dxfIdent->parsePtrToData();
            //MessageBox(NULL,"retval","",MB_OK);
            //MessageBox(NULL,itoa(retVal,ch,10),"",MB_OK);
            if (retVal == 1)
            {
                //MessageBox(NULL,"add node","",MB_OK);
                dlink->addnode(dxfIdent->returnEntityType(),dxfIdent->returnEntityData());
            }
        }
        while (retVal == 1);

  //  duration = (double) ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    //fio.fileWrite("parsing / adding node");
    //fio.fileWrite(duration);


tmplink = dlink->returnHead();

}

void computeExtents()
{
    dxfextents = new DXFImageExtents(dlink->returnHead()); //dlink->returnCurrentNode(dlink););
    dxfextents->computeExtents();
}

char* returnExtents()
{
    std::ostringstream ossImgExtents;
    computeExtents();
    EXTENT_RECT* rc = (EXTENT_RECT *)dxfextents->returnDXFImageExtents();
    ossImgExtents<<rc->left<<","<<rc->top<<","<<rc->right<<","<<rc->bottom;
    dataReturn = ossImgExtents.str();
    char* ptr =(char *)dataReturn.c_str();
    return ptr;
}

char* returnEntity()
{
    DXFarc *Arc;
    DXFline *Line;
    DXFcircle *Circle;
    DXFpolyline *Polyline;

    string polyLineResult="";
    std::vector<std::string> vecPoly;
    std::ostringstream ossEntity;

    if (tmplink->entityType == CIRCLE)
    {
        Circle = tmplink->returnCircle();
        ossEntity<<"Circle,"<<Circle->xcenter<<","<<Circle->ycenter<<","<<Circle->radius;
    }

    if (tmplink->entityType == LINE)
    {
        Line = tmplink->returnSegment();
        sprintf(entityBuffer,"Line,%f,%f,%f,%f",Line->startx,Line->starty,Line->endx,Line->endy);
        ossEntity<<"Line,"<<Line->startx<<","<<Line->starty<<","<<Line->endx<<","<<Line->endy;
    }

    if (tmplink->entityType == ARC)
    {
        Arc = tmplink->returnArc();
        ossEntity<<"Arc,"<<Arc->startangle<<","<<Arc->endangle<<","<<Arc->radius<<","<<Arc->xcenter<<","<<Arc->ycenter;
    }

    if (tmplink->entityType == VERTEX || tmplink->entityType == POLYLINE || tmplink->entityType == LWPOLYLINE)
    {
        Polyline = tmplink->returnPolyLine();
        do
        {
            static int reset;
            vecPoly.push_back(Polyline->xcoor);
            vecPoly.push_back(",");
            vecPoly.push_back(Polyline->ycoor);
            vecPoly.push_back(",");
        }
        while (Polyline = Polyline->next);
    polyLineResult += "Polyline,";

    for (vector<string>::iterator it = vecPoly.begin();it!=vecPoly.end();++it)
        polyLineResult += *it;
        polyLineResult += ",";
    }
    if (polyLineResult.length() > 0)
        dataReturn = polyLineResult;
    else
        dataReturn = ossEntity.str();

    char *ptr = (char*) dataReturn.c_str();//return ossEntity.str();
    return ptr;
}

void reset()
{
    tmplink = dlink->returnHead();
}

bool getNextEntity()
{
    tmplink = tmplink->incrementNode();

    if (tmplink == NULL)
        return false;
    else
        return true;
}

void cleanUp()
{
    delete dIO;
    delete dlink;
    delete dxfextents;
}

char* returnScaleDelta(double Left, double Top, double Right, double Bottom)
{
    computeExtents();
    EXTENT_RECT* rc = (EXTENT_RECT *)dxfextents->returnDXFImageExtents();
    std::ostringstream ossExtents;
    deltaXY *dxy;
    imageScaling imgS(Left,Top,Right,Bottom);

    float scale = imgS.computeScale(rc->left,rc->top,rc->right,rc->bottom);

    imageTranslation imgT(Left,Top,Right,Bottom);

    imgT.computeDelta(rc->left*scale,rc->top*scale,rc->right*scale,rc->bottom*scale);
    dxy = (deltaXY *)imgT.returnDeltaXY();

    ossExtents<<scale<<","<<dxy->dx<<","<<dxy->dy;
    dataReturn = ossExtents.str();
    char *ptr = (char *) dataReturn.c_str();
    return ptr;
}

