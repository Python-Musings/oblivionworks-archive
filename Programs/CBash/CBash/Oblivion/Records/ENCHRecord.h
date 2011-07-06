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

class ENCHRecord : public Record
    {
    private:
        struct ENCHENIT
            {
            UINT32 itemType, chargeAmount, enchantCost;
            UINT8  flags, unused1[3];

            ENCHENIT();
            ~ENCHENIT();

            bool operator ==(const ENCHENIT &other) const;
            bool operator !=(const ENCHENIT &other) const;
            };

        enum flagsFlags
            {
            fIsNoAutoCalc = 0x00000001
            };

        enum eItemType
            {
            eScroll  = 0,
            eStaff   = 1,
            eWeapon  = 2,
            eApparel = 3
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        ReqSubRecord<ENCHENIT> ENIT;
        std::vector<GENEffect *> Effects;
        OptSubRecord<OBMEMAGIC> OBME;

        ENCHRecord(unsigned char *_recData=NULL);
        ENCHRecord(ENCHRecord *srcRecord);
        ~ENCHRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsNoAutoCalc();
        void   IsNoAutoCalc(bool value);
        bool   IsFlagMask(UINT8 Mask, bool Exact=false);
        void   SetFlagMask(UINT8 Mask);

        bool   IsScroll();
        void   IsScroll(bool value);
        bool   IsStaff();
        void   IsStaff(bool value);
        bool   IsWeapon();
        void   IsWeapon(bool value);
        bool   IsApparel();
        void   IsApparel(bool value);
        bool   IsType(UINT32 Type);
        void   SetType(UINT32 Type);

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

        bool operator ==(const ENCHRecord &other) const;
        bool operator !=(const ENCHRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(ENCHRecord *)other;}
    };