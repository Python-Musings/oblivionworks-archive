/*
GPL License and Copyright Notice ============================================
 This file is part of CBash.

 CBash is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 CBash is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with CBash; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 CBash copyright (C) 2010 Waruddar
=============================================================================
*/
#pragma once
#include "..\..\Common.h"
#include "..\..\GenericRecord.h"

class LANDRecord : public Record
    {
    private:
        struct LANDNORMALS
            {
            UINT8   x;
            UINT8   y;
            UINT8   z;

            LANDNORMALS();
            ~LANDNORMALS();

            bool operator ==(const LANDNORMALS &other) const;
            bool operator !=(const LANDNORMALS &other) const;
            };

        struct LANDVNML
            {
            LANDNORMALS VNML[33][33];

            bool operator ==(const LANDVNML &other) const;
            bool operator !=(const LANDVNML &other) const;
            };

        struct LANDVHGT
            {
            FLOAT32 offset;
            SINT8   VHGT[33][33];
            UINT8   unused1[3];

            LANDVHGT();
            ~LANDVHGT();

            bool operator ==(const LANDVHGT &other) const;
            bool operator !=(const LANDVHGT &other) const;
            };

        struct LANDCOLORS
            {
            UINT8   red;
            UINT8   green;
            UINT8   blue;

            LANDCOLORS();
            ~LANDCOLORS();

            bool operator ==(const LANDCOLORS &other) const;
            bool operator !=(const LANDCOLORS &other) const;
            };

        struct LANDVCLR
            {
            LANDCOLORS VCLR[33][33];

            bool operator ==(const LANDVCLR &other) const;
            bool operator !=(const LANDVCLR &other) const;
            };

        struct LANDGENTXT
            {
            FORMID  texture;
            UINT8   quadrant;
            UINT8   unused1;
            SINT16  layer;

            LANDGENTXT();
            ~LANDGENTXT();

            bool operator ==(const LANDGENTXT &other) const;
            bool operator !=(const LANDGENTXT &other) const;
            };

        struct LANDVTXT
            {
            UINT16  position;
            UINT8   unused1[2];
            FLOAT32 opacity;

            LANDVTXT();
            ~LANDVTXT();

            bool operator ==(const LANDVTXT &other) const;
            bool operator !=(const LANDVTXT &other) const;
            };

        struct LANDLAYERS
            {
            ReqSubRecord<LANDGENTXT> ATXT;
            std::vector<LANDVTXT> VTXT;

            bool operator ==(const LANDLAYERS &other) const;
            bool operator !=(const LANDLAYERS &other) const;
            };

        //struct LANDPOINTS
        //    {
        //    float AlphaLayer[8];
        //    unsigned char height;
        //    unsigned char normalX;
        //    unsigned char normalY;
        //    unsigned char normalZ;
        //    unsigned char red;
        //    unsigned char green;
        //    unsigned char blue;
        //    LANDPOINTS():AlphaLayer(8, 0.0) {}
        //    };
        //
        //struct LANDMERGED
        //    {//156B, 40*33*33 (1.319GB), 28B - 7B*33*33 (265MB)
        //    LANDPOINTS Points[33][33];
        //    //UINT32 bottomLeftAlphas[8];
        //    //UINT32 bottomRightAlphas[8];
        //    //UINT32 topLeftAlphas[8];
        //    //UINT32 topRightAlphas[8];
        //    UINT32 bottomLeftBaseTexture;
        //    UINT32 bottomRightBaseTexture;
        //    UINT32 topLeftBaseTexture;
        //    UINT32 topRightBaseTexture;
        //    float heightOffset;
        //    short bottomLeftBaseLayer;
        //    short bottomRightBaseLayer;
        //    short topLeftBaseLayer;
        //    short topRightBaseLayer;
        //    //LANDMERGED():Points(33, std::vector<LANDPOINTS>(33)), bottomLeftAlphas(8),  bottomRightAlphas(8),  topLeftAlphas(8),  topRightAlphas(8) {}
        //    };

        enum QuadrantEnum {
            eBottomLeft,
            eBottomRight,
            eTopLeft,
            eTopRight
            };

    public:
        RawRecord DATA;
        OptSubRecord<LANDVNML> VNML;
        OptSubRecord<LANDVHGT> VHGT;
        OptSubRecord<LANDVCLR> VCLR;
        std::vector<ReqSubRecord<LANDGENTXT> *> BTXT;
        std::vector<LANDLAYERS *> Layers;
        std::vector<FORMID> VTEX;
        //LANDMERGED *Merged;

        LANDRecord *WestLand;
        LANDRecord *EastLand;
        LANDRecord *NorthLand;
        LANDRecord *SouthLand;

        LANDRecord(unsigned char *_recData=NULL);
        LANDRecord(LANDRecord *srcRecord);
        ~LANDRecord();

        bool    VisitFormIDs(FormIDOp &op);

        UINT8   CalcQuadrant(const UINT32 &row, const UINT32 &column);
        UINT16  CalcPosition(const UINT8 &curQuadrant, const UINT32 &row, const UINT32 &column);
        FLOAT32 CalcHeight(const UINT32 &row, const UINT32 &column);

        UINT32  GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, UINT32 WhichAttribute=0);
        void *  GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool    SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, UINT32 ArraySize=0);
        void    DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        UINT32  GetSize(bool forceCalc=false);
        UINT32  GetType();
        STRING  GetStrType();
        UINT32  GetParentType();

        SINT32  ParseRecord(unsigned char *buffer, const UINT32 &recSize);
        SINT32  Unload();
        SINT32  WriteRecord(_FileHandler &SaveHandler);

        bool operator ==(const LANDRecord &other) const;
        bool operator !=(const LANDRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(LANDRecord *)other;}
    };