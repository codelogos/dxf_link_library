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

 bool buildEntities(char * name)
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
            }
        }
        while (i == 1);
        j++;
    }
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
    DXFpolyline *pl;


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
        //fileIO fio("test.txt",0);
        Arc = tmplink->returnArc();
        ossEntity<<"Arc,"<<Arc->startangle<<","<<Arc->endangle<<","<<Arc->radius<<","<<Arc->xcenter<<","<<Arc->ycenter;
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
            vecPoly.push_back(pl->xcoor);
            vecPoly.push_back(",");
            vecPoly.push_back(pl->ycoor);
        }
        while (pl = pl->next);
    }

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
    if (dxfextents)
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

