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

class LIGHRecord : public Record
    {
    private:
        struct LIGHDATA
            {
            SINT32  duration;
            UINT32  radius;
            GENCLR  color;
            UINT32  flags;
            FLOAT32 falloff, fov;
            UINT32  value;
            FLOAT32 weight;

            LIGHDATA();
            ~LIGHDATA();

            bool operator ==(const LIGHDATA &other) const;
            bool operator !=(const LIGHDATA &other) const;
            };

        struct LIGHFNAM
            {
            FLOAT32 fade;

            LIGHFNAM();
            ~LIGHFNAM();

            bool operator ==(const LIGHFNAM &other) const;
            bool operator !=(const LIGHFNAM &other) const;
            };

        enum flagsFlags
            {
            fIsDynamic      = 0x00000001,
            fIsCanTake      = 0x00000002,
            fIsNegative     = 0x00000004,
            fIsFlickers     = 0x00000008,
            fIsOffByDefault = 0x00000020,
            fIsFlickerSlow  = 0x00000040,
            fIsPulse        = 0x00000080,
            fIsPulseSlow    = 0x00000100,
            fIsSpotLight    = 0x00000200,
            fIsSpotShadow   = 0x00000400
            };

    public:
        StringRecord EDID;
        OptSubRecord<GENMODEL> MODL;
        OptSubRecord<GENFID> SCRI;
        StringRecord FULL;
        StringRecord ICON;
        ReqSubRecord<LIGHDATA> DATA;
        OptSubRecord<LIGHFNAM> FNAM;
        OptSubRecord<GENFID> SNAM;

        LIGHRecord(unsigned char *_recData=NULL);
        LIGHRecord(LIGHRecord *srcRecord);
        ~LIGHRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsDynamic();
        void   IsDynamic(bool value);
        bool   IsCanTake();
        void   IsCanTake(bool value);
        bool   IsTakeable();
        void   IsTakeable(bool value);
        bool   IsNegative();
        void   IsNegative(bool value);
        bool   IsFlickers();
        void   IsFlickers(bool value);
        bool   IsOffByDefault();
        void   IsOffByDefault(bool value);
        bool   IsFlickerSlow();
        void   IsFlickerSlow(bool value);
        bool   IsPulse();
        void   IsPulse(bool value);
        bool   IsPulseSlow();
        void   IsPulseSlow(bool value);
        bool   IsSpotLight();
        void   IsSpotLight(bool value);
        bool   IsSpotShadow();
        void   IsSpotShadow(bool value);
        bool   IsFlagMask(UINT32 Mask, bool Exact=false);
        void   SetFlagMask(UINT32 Mask);

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

        bool operator ==(const LIGHRecord &other) const;
        bool operator !=(const LIGHRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(LIGHRecord *)other;}
    };