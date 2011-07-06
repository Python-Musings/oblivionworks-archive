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
//#include "ACHRRecord.h"
//#include "ACRERecord.h"
//#include "REFRRecord.h"
//#include "PGRDRecord.h"
//#include "LANDRecord.h"
//#include "WRLDRecord.h"
#include <vector>

class CELLRecord : public Record
    {
    private:
        struct CELLXCLL
            {
            GENCLR  ambient;
            GENCLR  directional;
            GENCLR  fog;
            FLOAT32 fogNear, fogFar;
            SINT32  directionalXY, directionalZ;
            FLOAT32 directionalFade, fogClip;

            CELLXCLL();
            ~CELLXCLL();

            bool operator ==(const CELLXCLL &other) const;
            bool operator !=(const CELLXCLL &other) const;
            };

        struct CELLXCLW
            {
            FLOAT32 waterHeight;

            CELLXCLW();
            ~CELLXCLW();

            bool operator ==(const CELLXCLW &other) const;
            bool operator !=(const CELLXCLW &other) const;
            };

        struct CELLXCLC
            {
            SINT32  posX, posY;

            CELLXCLC();
            ~CELLXCLC();

            bool operator ==(const CELLXCLC &other) const;
            bool operator !=(const CELLXCLC &other) const;
            };

        enum flagsFlags
            {
            fIsInterior         = 0x00000001,
            fHasWater           = 0x00000002,
            fInvertFastTravel   = 0x00000004,
            fForceHideLand      = 0x00000008,
            //fIsOblivionInterior = 0x00000008, //From OBSE, unconfirmed
            fPublicPlace        = 0x00000020,
            fHandChanged        = 0x00000040,
            fBehaveLikeExterior = 0x00000080
            };

        enum eXCMTType
            {
            eDefault,
            ePublic,
            eDungeon
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        ReqSubRecord<GENFLAG> DATA;
        SemiOptSubRecord<CELLXCLL> XCLL;
        SubRecord<GENFLAG> XCMT;
        OptSubRecord<GENXOWN> Ownership;
        SubRecord<GENFID> XCCM;
        SubRecord<CELLXCLW> XCLW;
        std::vector<FORMID> XCLR;
        SemiOptSubRecord<CELLXCLC> XCLC;
        SubRecord<GENFID> XCWT;
        std::vector<Record *> ACHR;
        std::vector<Record *> ACRE;
        std::vector<Record *> REFR;
        Record *PGRD;
        Record *LAND;

        Record *Parent;

        CELLRecord(unsigned char *_recData=NULL);
        CELLRecord(CELLRecord *srcRecord);
        ~CELLRecord();

        bool   HasSubRecords();
        bool   VisitSubRecords(const UINT32 &RecordType, RecordOp &op);
        bool   VisitFormIDs(FormIDOp &op);

        bool   IsInterior();
        void   IsInterior(bool value);
        bool   IsHasWater();
        void   IsHasWater(bool value);
        bool   IsInvertFastTravel();
        void   IsInvertFastTravel(bool value);
        bool   IsForceHideLand();
        void   IsForceHideLand(bool value);
        bool   IsPublicPlace();
        void   IsPublicPlace(bool value);
        bool   IsHandChanged();
        void   IsHandChanged(bool value);
        bool   IsBehaveLikeExterior();
        void   IsBehaveLikeExterior(bool value);
        bool   IsFlagMask(UINT8 Mask, bool Exact=false);
        void   SetFlagMask(UINT8 Mask);

        bool   IsDefaultMusic();
        void   IsDefaultMusic(bool value);
        bool   IsPublicMusic();
        void   IsPublicMusic(bool value);
        bool   IsDungeonMusic();
        void   IsDungeonMusic(bool value);
        bool   IsMusicType(UINT8 Type);
        void   SetMusicType(UINT8 Type);

        UINT32 GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, UINT32 WhichAttribute=0);
        void * GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool   SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, UINT32 ArraySize=0);
        void   DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        UINT32 GetSize(bool forceCalc=false);
        UINT32 GetType();
        STRING GetStrType();
        UINT32 GetParentType();

        SINT32 ParseRecord(unsigned char *buffer, const UINT32 &recSize);
        SINT32 Unload();
        SINT32 WriteRecord(_FileHandler &SaveHandler);

        bool operator ==(const CELLRecord &other) const;
        bool operator !=(const CELLRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(CELLRecord *)other;}
    };