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

class REGNRecord : public Record
    {
    private:
        struct REGNRPLD
            {
            FLOAT32 posX, posY;

            REGNRPLD();
            ~REGNRPLD();

            bool operator ==(const REGNRPLD &other) const;
            bool operator !=(const REGNRPLD &other) const;
            };

        struct REGNRPLI
            {
            UINT32  edgeFalloff;

            REGNRPLI();
            ~REGNRPLI();

            bool operator ==(const REGNRPLI &other) const;
            bool operator !=(const REGNRPLI &other) const;
            };

        struct REGNArea
            {
            ReqSubRecord<REGNRPLI> RPLI;
            std::vector<REGNRPLD> RPLD;

            bool operator ==(const REGNArea &other) const;
            bool operator !=(const REGNArea &other) const;
            };

        struct REGNRDWT
            {
            FORMID  weather;
            UINT32  chance;

            REGNRDWT();
            ~REGNRDWT();

            bool operator ==(const REGNRDWT &other) const;
            bool operator !=(const REGNRDWT &other) const;
            };

        struct REGNRDSD
            {
            FORMID  sound;
            UINT32  flags, chance;

            enum RDSDFlags
                {
                fIsPleasant = 0x00000001,
                fIsCloudy   = 0x00000002,
                fIsRainy    = 0x00000004,
                fIsSnowy    = 0x00000008
                };

            REGNRDSD();
            ~REGNRDSD();

            bool IsPleasant();
            void IsPleasant(bool value);
            bool IsCloudy();
            void IsCloudy(bool value);
            bool IsRainy();
            void IsRainy(bool value);
            bool IsSnowy();
            void IsSnowy(bool value);
            bool IsFlagMask(UINT32 Mask, bool Exact=false);
            void SetFlagMask(UINT32 Mask);

            bool operator ==(const REGNRDSD &other) const;
            bool operator !=(const REGNRDSD &other) const;
            };

        struct REGNRDMD
            {
            UINT32  type;

            REGNRDMD();
            ~REGNRDMD();

            bool operator ==(const REGNRDMD &other) const;
            bool operator !=(const REGNRDMD &other) const;
            };

        struct REGNRDGS
            {
            FORMID  grass;
            UINT8   unk1[4];

            REGNRDGS();
            ~REGNRDGS();

            bool operator ==(const REGNRDGS &other) const;
            bool operator !=(const REGNRDGS &other) const;
            };

        struct REGNRDOT
            {
            FORMID  objectId;
            UINT16  parentIndex;
            UINT8   unused1[2];
            FLOAT32 density;
            UINT8   clustering, minSlope, maxSlope, flags;
            UINT16  radiusWRTParent, radius;
            UINT8   unk1[4];
            FLOAT32 maxHeight, sink, sinkVar, sizeVar;
            UINT16  angleVarX, angleVarY, angleVarZ;
            UINT8   unused2[2], unk2[4];

            enum RDOTFlags
                {
                fIsConformToSlope = 0x00000001,
                fIsPaintVertices  = 0x00000002,
                fIsSizeVariance   = 0x00000004,
                fIsXVariance      = 0x00000008,
                fIsYVariance      = 0x00000010,
                fIsZVariance      = 0x00000020,
                fIsTree           = 0x00000040,
                fIsHugeRock       = 0x00000080
                };

            REGNRDOT();
            ~REGNRDOT();

            bool IsConformToSlope();
            void IsConformToSlope(bool value);
            bool IsPaintVertices();
            void IsPaintVertices(bool value);
            bool IsSizeVariance();
            void IsSizeVariance(bool value);
            bool IsXVariance();
            void IsXVariance(bool value);
            bool IsYVariance();
            void IsYVariance(bool value);
            bool IsZVariance();
            void IsZVariance(bool value);
            bool IsTree();
            void IsTree(bool value);
            bool IsHugeRock();
            void IsHugeRock(bool value);
            bool IsFlagMask(UINT8 Mask, bool Exact=false);
            void SetFlagMask(UINT8 Mask);

            bool operator ==(const REGNRDOT &other) const;
            bool operator !=(const REGNRDOT &other) const;
            };

        struct REGNRDAT
            {
            UINT32  entryType;
            UINT8   flags, priority, unused1[2];

            REGNRDAT();
            ~REGNRDAT();

            bool operator ==(const REGNRDAT &other) const;
            bool operator !=(const REGNRDAT &other) const;
            };

        struct REGNEntry
            {
            ReqSubRecord<REGNRDAT> RDAT;
            std::vector<REGNRDOT> RDOT;
            StringRecord RDMP;
            StringRecord ICON;
            std::vector<REGNRDGS> RDGS;
            SemiOptSubRecord<REGNRDMD> RDMD;
            std::vector<REGNRDSD> RDSD;
            std::vector<REGNRDWT> RDWT;

            enum RDATFlags
                {
                fIsOverride = 0x00000001
                };

            enum eRDATType
                {
                eObject  = 2,
                eWeather = 3,
                eMap     = 4,
                eUnkIcon = 5,
                eGrass   = 6,
                eSound   = 7
                };

            enum eRDMDType
                {
                eDefault,
                ePublic,
                eDungeon
                };

            bool IsOverride();
            void IsOverride(bool value);
            bool IsFlagMask(UINT8 Mask, bool Exact=false);
            void SetFlagMask(UINT8 Mask);

            bool IsObject();
            void IsObject(bool value);
            bool IsWeather();
            void IsWeather(bool value);
            bool IsMap();
            void IsMap(bool value);
            bool IsIcon();
            void IsIcon(bool value);
            bool IsGrass();
            void IsGrass(bool value);
            bool IsSound();
            void IsSound(bool value);
            bool IsType(UINT32 Type);
            void SetType(UINT32 Type);

            bool IsDefaultMusic();
            void IsDefaultMusic(bool value);
            bool IsPublicMusic();
            void IsPublicMusic(bool value);
            bool IsDungeonMusic();
            void IsDungeonMusic(bool value);
            bool IsMusicType(UINT32 Type);
            void SetMusicType(UINT32 Type);

            bool operator ==(const REGNEntry &other) const;
            bool operator !=(const REGNEntry &other) const;
            };

        enum eREGNType {
            eREGNObjects=2,
            eREGNWeathers=3,
            eREGNMap=4,
            eREGNIcon=5,
            eREGNGrasses=6,
            eREGNSounds=7
            };

    public:
        StringRecord EDID;
        StringRecord ICON;
        ReqSubRecord<GENCLR> RCLR;
        SubRecord<GENFID> WNAM;
        std::vector<REGNArea *> Areas;
        std::vector<REGNEntry *> Entries;

        REGNRecord(unsigned char *_recData=NULL);
        REGNRecord(REGNRecord *srcRecord);
        ~REGNRecord();

        bool   VisitFormIDs(FormIDOp &op);

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

        bool operator ==(const REGNRecord &other) const;
        bool operator !=(const REGNRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(REGNRecord *)other;}
    };