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

class FURNRecord : public Record
    {
    private:
        enum flagsFlags
            {
            fIsAnim01     = 0x00000001,
            fIsAnim02     = 0x00000002,
            fIsAnim03     = 0x00000004,
            fIsAnim04     = 0x00000008,
            fIsAnim05     = 0x00000010,
            fIsAnim06     = 0x00000020,
            fIsAnim07     = 0x00000040,
            fIsAnim08     = 0x00000080,
            fIsAnim09     = 0x00000100,
            fIsAnim10     = 0x00000200,
            fIsAnim11     = 0x00000400,
            fIsAnim12     = 0x00000800,
            fIsAnim13     = 0x00001000,
            fIsAnim14     = 0x00002000,
            fIsAnim15     = 0x00004000,
            fIsAnim16     = 0x00008000,
            fIsAnim17     = 0x00010000,
            fIsAnim18     = 0x00020000,
            fIsAnim19     = 0x00040000,
            fIsAnim20     = 0x00080000,
            fIsAnim21     = 0x00100000,
            fIsAnim22     = 0x00200000,
            fIsAnim23     = 0x00400000,
            fIsAnim24     = 0x00800000,
            fIsAnim25     = 0x01000000,
            fIsAnim26     = 0x02000000,
            fIsAnim27     = 0x04000000,
            fIsAnim28     = 0x08000000,
            fIsAnim29     = 0x10000000,
            fIsAnim30     = 0x20000000,
            fIsSitAnim    = 0x40000000,
            fIsSleepAnim  = 0x80000000
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        OptSubRecord<GENMODEL> MODL;
        OptSubRecord<GENFID> SCRI;
        ReqSubRecord<GENUFLAG> MNAM;

        FURNRecord(unsigned char *_recData=NULL);
        FURNRecord(FURNRecord *srcRecord);
        ~FURNRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsAnim01();
        void   IsAnim01(bool value);
        bool   IsAnim02();
        void   IsAnim02(bool value);
        bool   IsAnim03();
        void   IsAnim03(bool value);
        bool   IsAnim04();
        void   IsAnim04(bool value);
        bool   IsAnim05();
        void   IsAnim05(bool value);
        bool   IsAnim06();
        void   IsAnim06(bool value);
        bool   IsAnim07();
        void   IsAnim07(bool value);
        bool   IsAnim08();
        void   IsAnim08(bool value);
        bool   IsAnim09();
        void   IsAnim09(bool value);
        bool   IsAnim10();
        void   IsAnim10(bool value);
        bool   IsAnim11();
        void   IsAnim11(bool value);
        bool   IsAnim12();
        void   IsAnim12(bool value);
        bool   IsAnim13();
        void   IsAnim13(bool value);
        bool   IsAnim14();
        void   IsAnim14(bool value);
        bool   IsAnim15();
        void   IsAnim15(bool value);
        bool   IsAnim16();
        void   IsAnim16(bool value);
        bool   IsAnim17();
        void   IsAnim17(bool value);
        bool   IsAnim18();
        void   IsAnim18(bool value);
        bool   IsAnim19();
        void   IsAnim19(bool value);
        bool   IsAnim20();
        void   IsAnim20(bool value);
        bool   IsAnim21();
        void   IsAnim21(bool value);
        bool   IsAnim22();
        void   IsAnim22(bool value);
        bool   IsAnim23();
        void   IsAnim23(bool value);
        bool   IsAnim24();
        void   IsAnim24(bool value);
        bool   IsAnim25();
        void   IsAnim25(bool value);
        bool   IsAnim26();
        void   IsAnim26(bool value);
        bool   IsAnim27();
        void   IsAnim27(bool value);
        bool   IsAnim28();
        void   IsAnim28(bool value);
        bool   IsAnim29();
        void   IsAnim29(bool value);
        bool   IsAnim30();
        void   IsAnim30(bool value);
        bool   IsSitAnim();
        void   IsSitAnim(bool value);
        bool   IsSleepAnim();
        void   IsSleepAnim(bool value);
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

        bool operator ==(const FURNRecord &other) const;
        bool operator !=(const FURNRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(FURNRecord *)other;}
    };