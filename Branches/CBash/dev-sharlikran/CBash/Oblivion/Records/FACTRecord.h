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
#include <vector>

class FACTRecord : public Record
    {
    private:
        struct FACTCNAM
            {
            FLOAT32 crimeGoldMultiplier;

            FACTCNAM();
            ~FACTCNAM();

            bool operator ==(const FACTCNAM &other) const;
            bool operator !=(const FACTCNAM &other) const;
            };

        struct FACTRNAM
            {
            SINT32  RNAM;
            StringRecord MNAM, FNAM, INAM;

            FACTRNAM();
            ~FACTRNAM();

            bool operator ==(const FACTRNAM &other) const;
            bool operator !=(const FACTRNAM &other) const;
            };

        enum flagsFlags
            {
            fIsHiddenFromPC  = 0x00000001,
            fIsEvil          = 0x00000002,
            fIsSpecialCombat = 0x00000004
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        std::vector<ReqSubRecord<GENXNAM> *> XNAM;
        ReqSubRecord<GENFLAG> DATA;
        SemiOptSubRecord<FACTCNAM> CNAM;
        std::vector<ReqSubRecord<FACTRNAM> *> RNAM;

        FACTRecord(unsigned char *_recData=NULL);
        FACTRecord(FACTRecord *srcRecord);
        ~FACTRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsHiddenFromPC();
        void   IsHiddenFromPC(bool value);
        bool   IsEvil();
        void   IsEvil(bool value);
        bool   IsSpecialCombat();
        void   IsSpecialCombat(bool value);
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

        bool operator ==(const FACTRecord &other) const;
        bool operator !=(const FACTRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(FACTRecord *)other;}
    };