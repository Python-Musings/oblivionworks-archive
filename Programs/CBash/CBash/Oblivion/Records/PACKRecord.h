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

class PACKRecord : public Record
    {
    private:
        struct PACKPKDT
            {
            UINT32  flags;
            UINT8   aiType, unused1[3];

            PACKPKDT();
            ~PACKPKDT();

            bool operator ==(const PACKPKDT &other) const;
            bool operator !=(const PACKPKDT &other) const;
            };

        struct PACKPLDT
            {
            SINT32  locType;
            FORMID_OR_UINT32  locId;
            SINT32  locRadius;

            PACKPLDT();
            ~PACKPLDT();

            bool operator ==(const PACKPLDT &other) const;
            bool operator !=(const PACKPLDT &other) const;
            };

        struct PACKPSDT
            {
            SINT8   month, day;
            UINT8   date;
            SINT8   time;
            SINT32 duration;

            PACKPSDT();
            ~PACKPSDT();

            bool operator ==(const PACKPSDT &other) const;
            bool operator !=(const PACKPSDT &other) const;
            };

        struct PACKPTDT
            {
            SINT32 targetType;
            FORMID_OR_UINT32 targetId;
            SINT32 targetCount;

            PACKPTDT();
            ~PACKPTDT();

            bool operator ==(const PACKPTDT &other) const;
            bool operator !=(const PACKPTDT &other) const;
            };

        enum flagsFlags
            {
            fIsOffersServices    = 0x00000001,
            fIsMustReachLocation = 0x00000002,
            fIsMustComplete      = 0x00000004,
            fIsLockAtStart       = 0x00000008,
            fIsLockAtEnd         = 0x00000010,
            fIsLockAtLocation    = 0x00000020,
            fIsUnlockAtStart     = 0x00000040,
            fIsUnlockAtEnd       = 0x00000080,
            fIsUnlockAtLocation  = 0x00000100,
            fIsContinueIfPcNear  = 0x00000200,
            fIsOncePerDay        = 0x00000400,
            fIsSkipFallout       = 0x00001000,
            fIsAlwaysRun         = 0x00002000,
            fIsAlwaysSneak       = 0x00020000,
            fIsAllowSwimming     = 0x00040000,
            fIsAllowFalls        = 0x00080000,
            fIsUnequipArmor      = 0x00100000,
            fIsUnequipWeapons    = 0x00200000,
            fIsDefensiveCombat   = 0x00400000,
            fIsUseHorse          = 0x00800000,
            fIsNoIdleAnims       = 0x01000000
            };

        enum aiTypeType
            {
            eAIFind          = 0,
            eAIFollow        = 1,
            eAIEscort        = 2,
            eAIEat           = 3,
            eAISleep         = 4,
            eAIWander        = 5,
            eAITravel        = 6,
            eAIAccompany     = 7,
            eAIUseItemAt     = 8,
            eAIAmbush        = 9,
            eAIFleeNotCombat = 10,
            eAICastMagic     = 11
            };

        enum locTypeType
            {
            eLocNearReference       = 0,
            eLocInCell              = 1,
            eLocNearCurrentLocation = 2,
            eLocNearEditorLocation  = 3,
            eLocObjectID            = 4,
            eLocObjectType          = 5
            };

        enum targetTypeType
            {
            eTargetReference   = 0,
            eTargetObjectID    = 1,
            eTargetObjectType  = 2
            };

    public:
        StringRecord EDID;
        ReqSubRecord<PACKPKDT> PKDT;
        OptSubRecord<PACKPLDT> PLDT;
        ReqSubRecord<PACKPSDT> PSDT;
        OptSubRecord<PACKPTDT> PTDT;
        std::vector<ReqSubRecord<GENCTDA> *> CTDA;

        PACKRecord(unsigned char *_recData=NULL);
        PACKRecord(PACKRecord *srcRecord);
        ~PACKRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsAIFind();
        void   IsAIFind(bool value);
        bool   IsAIFollow();
        void   IsAIFollow(bool value);
        bool   IsAIEscort();
        void   IsAIEscort(bool value);
        bool   IsAIEat();
        void   IsAIEat(bool value);
        bool   IsAISleep();
        void   IsAISleep(bool value);
        bool   IsAIWander();
        void   IsAIWander(bool value);
        bool   IsAITravel();
        void   IsAITravel(bool value);
        bool   IsAIAccompany();
        void   IsAIAccompany(bool value);
        bool   IsAIUseItemAt();
        void   IsAIUseItemAt(bool value);
        bool   IsAIAmbush();
        void   IsAIAmbush(bool value);
        bool   IsAIFleeNotCombat();
        void   IsAIFleeNotCombat(bool value);
        bool   IsAICastMagic();
        void   IsAICastMagic(bool value);
        bool   IsAIType(UINT8 Type);
        void   SetAIType(UINT8 Type);

        bool   IsOffersServices();
        void   IsOffersServices(bool value);
        bool   IsMustReachLocation();
        void   IsMustReachLocation(bool value);
        bool   IsMustComplete();
        void   IsMustComplete(bool value);
        bool   IsLockAtStart();
        void   IsLockAtStart(bool value);
        bool   IsLockAtEnd();
        void   IsLockAtEnd(bool value);
        bool   IsLockAtLocation();
        void   IsLockAtLocation(bool value);
        bool   IsUnlockAtStart();
        void   IsUnlockAtStart(bool value);
        bool   IsUnlockAtEnd();
        void   IsUnlockAtEnd(bool value);
        bool   IsUnlockAtLocation();
        void   IsUnlockAtLocation(bool value);
        bool   IsContinueIfPcNear();
        void   IsContinueIfPcNear(bool value);
        bool   IsOncePerDay();
        void   IsOncePerDay(bool value);
        bool   IsSkipFallout();
        void   IsSkipFallout(bool value);
        bool   IsAlwaysRun();
        void   IsAlwaysRun(bool value);
        bool   IsAlwaysSneak();
        void   IsAlwaysSneak(bool value);
        bool   IsAllowSwimming();
        void   IsAllowSwimming(bool value);
        bool   IsAllowFalls();
        void   IsAllowFalls(bool value);
        bool   IsUnequipArmor();
        void   IsUnequipArmor(bool value);
        bool   IsUnequipWeapons();
        void   IsUnequipWeapons(bool value);
        bool   IsDefensiveCombat();
        void   IsDefensiveCombat(bool value);
        bool   IsUseHorse();
        void   IsUseHorse(bool value);
        bool   IsNoIdleAnims();
        void   IsNoIdleAnims(bool value);
        bool   IsFlagMask(UINT32 Mask, bool Exact=false);
        void   SetFlagMask(UINT32 Mask);

        bool   IsLocNearReference();
        void   IsLocNearReference(bool value);
        bool   IsLocInCell();
        void   IsLocInCell(bool value);
        bool   IsLocNearCurrentLoc();
        void   IsLocNearCurrentLoc(bool value);
        bool   IsLocNearEditorLoc();
        void   IsLocNearEditorLoc(bool value);
        bool   IsLocObjectID();
        void   IsLocObjectID(bool value);
        bool   IsLocObjectType();
        void   IsLocObjectType(bool value);
        bool   IsLocType(SINT32 Type);
        void   SetLocType(SINT32 Type);

        bool   IsTargetReference();
        void   IsTargetReference(bool value);
        bool   IsTargetObjectID();
        void   IsTargetObjectID(bool value);
        bool   IsTargetObjectType();
        void   IsTargetObjectType(bool value);
        bool   IsTargetType(SINT32 Type);
        void   SetTargetType(SINT32 Type);

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

        bool operator ==(const PACKRecord &other) const;
        bool operator !=(const PACKRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(PACKRecord *)other;}
    };