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

class GRASRecord : public Record
    {
    private:
        struct GRASDATA
            {
            UINT8   density, minSlope, maxSlope, unused1;
            UINT16  waterDistance;
            UINT8   unused2[2];
            UINT32  waterOp;
            FLOAT32 posRange, heightRange, colorRange, wavePeriod;
            UINT8   flags, unused3[3];

            GRASDATA();
            ~GRASDATA();

            bool operator ==(const GRASDATA &other) const;
            bool operator !=(const GRASDATA &other) const;
            };

        enum flagsFlags
            {
            fIsVLighting = 0x00000001,
            fIsUScaling  = 0x00000002,
            fIsFitSlope  = 0x00000004
            };

    public:
        StringRecord EDID;
        OptSubRecord<GENMODEL> MODL;
        ReqSubRecord<GRASDATA> DATA;

        GRASRecord(unsigned char *_recData=NULL);
        GRASRecord(GRASRecord *srcRecord);
        ~GRASRecord();

        bool   IsVLighting();
        void   IsVLighting(bool value);
        bool   IsUScaling();
        void   IsUScaling(bool value);
        bool   IsFitSlope();
        void   IsFitSlope(bool value);
        bool   IsFlagMask(UINT8 Mask, bool Exact=false);
        void   SetFlagMask(UINT8 Mask);

        UINT32 GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, UINT32 WhichAttribute=0);
        void * GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool   SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, UINT32 ArraySize=0);
        void   DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        UINT32 GetSize(bool forceCalc=false);
        UINT32 GetType();
        STRING GetStrType();

        SINT32 ParseRecord(unsigned char *buffer, const UINT32 &recSize);
        SINT32 Unload();
        SINT32 WriteRecord(_FileHandler &SaveHandler);

        bool operator ==(const GRASRecord &other) const;
        bool operator !=(const GRASRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(GRASRecord *)other;}
    };