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

class CLOTRecord : public Record
    {
    private:
        enum flagsFlags
            {
            fIsHead        = 0x00000001,
            fIsHair        = 0x00000002,
            fIsUpperBody   = 0x00000004,
            fIsLowerBody   = 0x00000008,
            fIsHand        = 0x00000010,
            fIsFoot        = 0x00000020,
            fIsRightRing   = 0x00000040,
            fIsLeftRing    = 0x00000080,
            fIsAmulet      = 0x00000100,
            fIsWeapon      = 0x00000200,
            fIsBackWeapon  = 0x00000400,
            fIsSideWeapon  = 0x00000800,
            fIsQuiver      = 0x00001000,
            fIsShield      = 0x00002000,
            fIsTorch       = 0x00004000,
            fIsTail        = 0x00008000,
            fIsHideRings   = 0x00010000,
            fIsHideAmulets = 0x00020000,
            fIsNonPlayable = 0x00400000
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        OptSubRecord<GENFID> SCRI;
        OptSubRecord<GENFID> ENAM;
        OptSubRecord<GENANAM> ANAM;
        ReqSubRecord<GENUFLAG> BMDT;
        OptSubRecord<GENMODEL> MODL;
        OptSubRecord<GENMODEL> MOD2;
        StringRecord ICON;
        OptSubRecord<GENMODEL> MOD3;
        OptSubRecord<GENMODEL> MOD4;
        StringRecord ICO2;
        ReqSubRecord<GENVALUEWEIGHT> DATA;

        CLOTRecord(unsigned char *_recData=NULL);
        CLOTRecord(CLOTRecord *srcRecord);
        ~CLOTRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsHead();
        void   IsHead(bool value);
        bool   IsHair();
        void   IsHair(bool value);
        bool   IsUpperBody();
        void   IsUpperBody(bool value);
        bool   IsLowerBody();
        void   IsLowerBody(bool value);
        bool   IsHand();
        void   IsHand(bool value);
        bool   IsFoot();
        void   IsFoot(bool value);
        bool   IsRightRing();
        void   IsRightRing(bool value);
        bool   IsLeftRing();
        void   IsLeftRing(bool value);
        bool   IsAmulet();
        void   IsAmulet(bool value);
        bool   IsWeapon();
        void   IsWeapon(bool value);
        bool   IsBackWeapon();
        void   IsBackWeapon(bool value);
        bool   IsSideWeapon();
        void   IsSideWeapon(bool value);
        bool   IsQuiver();
        void   IsQuiver(bool value);
        bool   IsShield();
        void   IsShield(bool value);
        bool   IsTorch();
        void   IsTorch(bool value);
        bool   IsTail();
        void   IsTail(bool value);
        bool   IsHideRings();
        void   IsHideRings(bool value);
        bool   IsHideAmulets();
        void   IsHideAmulets(bool value);
        bool   IsNonPlayable();
        void   IsNonPlayable(bool value);
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

        bool operator ==(const CLOTRecord &other) const;
        bool operator !=(const CLOTRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(CLOTRecord *)other;}
    };