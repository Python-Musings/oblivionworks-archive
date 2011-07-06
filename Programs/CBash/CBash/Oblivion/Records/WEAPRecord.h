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

class WEAPRecord : public Record
    {
    private:
        #pragma pack(push)
        #pragma pack(2)
        struct WEAPDATA
            {
            UINT32  weaponType;
            FLOAT32 speed, reach;
            UINT32  flags, value, health;
            FLOAT32 weight;
            UINT16  damage;

            WEAPDATA();
            ~WEAPDATA();

            bool operator ==(const WEAPDATA &other) const;
            bool operator !=(const WEAPDATA &other) const;
            };
        #pragma pack(pop)

        enum flagsFlags
            {
            fIsNotNormalWeapon = 0x00000001
            };

        enum eWeaponType
            {
            eBlade1Hand = 0,
            eBlade2Hand = 1,
            eBlunt1Hand = 2,
            eBlunt2Hand = 3,
            eStaff      = 4,
            eBow        = 5
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        OptSubRecord<GENMODEL> MODL;
        StringRecord ICON;
        OptSubRecord<GENFID> SCRI;
        OptSubRecord<GENFID> ENAM;
        OptSubRecord<GENANAM> ANAM;
        ReqSubRecord<WEAPDATA> DATA;

        WEAPRecord(unsigned char *_recData=NULL);
        WEAPRecord(WEAPRecord *srcRecord);
        ~WEAPRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsNotNormalWeapon();
        void   IsNotNormalWeapon(bool value);
        bool   IsNotNormal();
        void   IsNotNormal(bool value);
        bool   IsNormalWeapon();
        void   IsNormalWeapon(bool value);
        bool   IsNormal();
        void   IsNormal(bool value);
        bool   IsFlagMask(UINT32 Mask, bool Exact=false);
        void   SetFlagMask(UINT32 Mask);

        bool   IsBlade1Hand();
        void   IsBlade1Hand(bool value);
        bool   IsBlade2Hand();
        void   IsBlade2Hand(bool value);
        bool   IsBlunt1Hand();
        void   IsBlunt1Hand(bool value);
        bool   IsBlunt2Hand();
        void   IsBlunt2Hand(bool value);
        bool   IsStaff();
        void   IsStaff(bool value);
        bool   IsBow();
        void   IsBow(bool value);
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

        bool operator ==(const WEAPRecord &other) const;
        bool operator !=(const WEAPRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(WEAPRecord *)other;}
    };