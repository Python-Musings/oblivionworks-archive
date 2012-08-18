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

class LTEXRecord : public Record
    {
    private:
        struct LTEXHNAM
            {
            UINT8   flags, friction, restitution;

            LTEXHNAM();
            ~LTEXHNAM();

            bool operator ==(const LTEXHNAM &other) const;
            bool operator !=(const LTEXHNAM &other) const;
            };

        struct LTEXSNAM
            {
            UINT8   specular;

            LTEXSNAM();
            ~LTEXSNAM();

            bool operator ==(const LTEXSNAM &other) const;
            bool operator !=(const LTEXSNAM &other) const;
            };

        enum flagsFlags
            {
            fIsStone      = 0x00000001,
            fIsCloth      = 0x00000002,
            fIsDirt       = 0x00000004,
            fIsGlass      = 0x00000008,
            fIsGrass      = 0x00000010,
            fIsMetal      = 0x00000020,
            fIsOrganic    = 0x00000040,
            fIsSkin       = 0x00000080,
            fIsWater      = 0x00000100,
            fIsWood       = 0x00000200,
            fIsHeavyStone = 0x00000400,
            fIsHeavyMetal = 0x00000800,
            fIsHeavyWood  = 0x00001000,
            fIsChain      = 0x00002000,
            fIsSnow       = 0x00004000
            };

    public:
        StringRecord EDID;
        StringRecord ICON;
        SubRecord<LTEXHNAM> HNAM;
        SubRecord<LTEXSNAM> SNAM;
        std::vector<FORMID> GNAM;

        LTEXRecord(unsigned char *_recData=NULL);
        LTEXRecord(LTEXRecord *srcRecord);
        ~LTEXRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsStone();
        void   IsStone(bool value);
        bool   IsCloth();
        void   IsCloth(bool value);
        bool   IsDirt();
        void   IsDirt(bool value);
        bool   IsGlass();
        void   IsGlass(bool value);
        bool   IsGrass();
        void   IsGrass(bool value);
        bool   IsMetal();
        void   IsMetal(bool value);
        bool   IsOrganic();
        void   IsOrganic(bool value);
        bool   IsSkin();
        void   IsSkin(bool value);
        bool   IsWater();
        void   IsWater(bool value);
        bool   IsWood();
        void   IsWood(bool value);
        bool   IsHeavyStone();
        void   IsHeavyStone(bool value);
        bool   IsHeavyMetal();
        void   IsHeavyMetal(bool value);
        bool   IsHeavyWood();
        void   IsHeavyWood(bool value);
        bool   IsChain();
        void   IsChain(bool value);
        bool   IsSnow();
        void   IsSnow(bool value);
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

        bool operator ==(const LTEXRecord &other) const;
        bool operator !=(const LTEXRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(LTEXRecord *)other;}
    };