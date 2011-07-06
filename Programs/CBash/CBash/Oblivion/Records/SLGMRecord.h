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

class SLGMRecord : public Record
    {
    private:
        struct SLGMSOUL
            {
            UINT8   soul;

            SLGMSOUL();
            ~SLGMSOUL();

            bool operator ==(const SLGMSOUL &other) const;
            bool operator !=(const SLGMSOUL &other) const;
            };

        struct SLGMSLCP
            {
            UINT8   capacity;

            SLGMSLCP();
            ~SLGMSLCP();

            bool operator ==(const SLGMSLCP &other) const;
            bool operator !=(const SLGMSLCP &other) const;
            };

        enum eSoulType
            {
            eNone    = 0,
            ePetty   = 1,
            eLesser  = 2,
            eCommon  = 3,
            eGreater = 4,
            eGrand   = 5
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        OptSubRecord<GENMODEL> MODL;
        StringRecord ICON;
        OptSubRecord<GENFID> SCRI;
        ReqSubRecord<GENVALUEWEIGHT> DATA;
        ReqSubRecord<SLGMSOUL> SOUL;
        ReqSubRecord<SLGMSLCP> SLCP;

        SLGMRecord(unsigned char *_recData=NULL);
        SLGMRecord(SLGMRecord *srcRecord);
        ~SLGMRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsNoSoul();
        void   IsNoSoul(bool value);
        bool   IsPettySoul();
        void   IsPettySoul(bool value);
        bool   IsLesserSoul();
        void   IsLesserSoul(bool value);
        bool   IsCommonSoul();
        void   IsCommonSoul(bool value);
        bool   IsGreaterSoul();
        void   IsGreaterSoul(bool value);
        bool   IsGrandSoul();
        void   IsGrandSoul(bool value);
        bool   IsSoul(UINT8 Type);
        void   SetSoul(UINT8 Type);

        bool   IsNoCapacity();
        void   IsNoCapacity(bool value);
        bool   IsPettyCapacity();
        void   IsPettyCapacity(bool value);
        bool   IsLesserCapacity();
        void   IsLesserCapacity(bool value);
        bool   IsCommonCapacity();
        void   IsCommonCapacity(bool value);
        bool   IsGreaterCapacity();
        void   IsGreaterCapacity(bool value);
        bool   IsGrandCapacity();
        void   IsGrandCapacity(bool value);
        bool   IsCapacity(UINT8 Type);
        void   SetCapacity(UINT8 Type);

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

        bool operator ==(const SLGMRecord &other) const;
        bool operator !=(const SLGMRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(SLGMRecord *)other;}
    };