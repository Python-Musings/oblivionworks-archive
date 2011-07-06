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

//Possibly extra crap exposed. Copied from NPC_ and hastily incorporated, may have missed stuff.
class CREARecord : public Record
    {
    private:
        struct CREADATA
            {
            UINT8   creatureType, combat, magic, stealth, soul;
            UINT8   unused1;
            UINT16  health;
            UINT8   unused2[2];
            UINT16  attackDamage;
            UINT8   strength, intelligence, willpower, agility, speed, endurance, personality, luck;

            CREADATA();
            ~CREADATA();

            bool operator ==(const CREADATA &other) const;
            bool operator !=(const CREADATA &other) const;
            };

        struct CREARNAM
            {
            UINT8   attackReach;

            CREARNAM();
            ~CREARNAM();

            bool operator ==(const CREARNAM &other) const;
            bool operator !=(const CREARNAM &other) const;
            };

        struct CREATNAM
            {
            FLOAT32 turningSpeed;

            CREATNAM();
            ~CREATNAM();

            bool operator ==(const CREATNAM &other) const;
            bool operator !=(const CREATNAM &other) const;
            };

        struct CREABNAM
            {
            FLOAT32 baseScale;

            CREABNAM();
            ~CREABNAM();

            bool operator ==(const CREABNAM &other) const;
            bool operator !=(const CREABNAM &other) const;
            };

        struct CREAWNAM
            {
            FLOAT32 footWeight;

            CREAWNAM();
            ~CREAWNAM();

            bool operator ==(const CREAWNAM &other) const;
            bool operator !=(const CREAWNAM &other) const;
            };

        struct CREACSDT
            {
            UINT32  soundType;

            CREACSDT();
            ~CREACSDT();

            bool operator ==(const CREACSDT &other) const;
            bool operator !=(const CREACSDT &other) const;
            };

        struct CREACSDC
            {
            UINT8   chance;

            CREACSDC();
            ~CREACSDC();

            bool operator ==(const CREACSDC &other) const;
            bool operator !=(const CREACSDC &other) const;
            };

        struct CREASound
            {
            ReqSubRecord<CREACSDT> CSDT;
            ReqSubRecord<GENFID> CSDI;
            ReqSubRecord<CREACSDC> CSDC;

            enum eSoundType
                {
                eLeftFoot      = 0,
                eRightFoot     = 1,
                eLeftBackFoot  = 2,
                eRightBackFoot = 3,
                eIdle          = 4,
                eAware         = 5,
                eAttack        = 6,
                eHit           = 7,
                eDeath         = 8,
                eWeapon        = 9
                };

            bool   IsLeftFoot();
            void   IsLeftFoot(bool value);
            bool   IsRightFoot();
            void   IsRightFoot(bool value);
            bool   IsLeftBackFoot();
            void   IsLeftBackFoot(bool value);
            bool   IsRightBackFoot();
            void   IsRightBackFoot(bool value);
            bool   IsIdle();
            void   IsIdle(bool value);
            bool   IsAware();
            void   IsAware(bool value);
            bool   IsAttack();
            void   IsAttack(bool value);
            bool   IsHit();
            void   IsHit(bool value);
            bool   IsDeath();
            void   IsDeath(bool value);
            bool   IsWeapon();
            void   IsWeapon(bool value);
            bool   IsType(UINT32 Type);
            void   SetType(UINT32 Type);

            bool   operator ==(const CREASound &other) const;
            bool   operator !=(const CREASound &other) const;
            };

        enum flagsFlags
            {
            fIsBiped           = 0x00000001,
            fIsEssential       = 0x00000002,
            fIsWeaponAndShield = 0x00000004,
            fIsRespawn         = 0x00000008,
            fIsSwims           = 0x00000010,
            fIsFlies           = 0x00000020,
            fIsWalks           = 0x00000040,
            fIsPCLevelOffset   = 0x00000080,
            fIsNoLowLevel      = 0x00000200,
            fIsNoBloodSpray    = 0x00000800,
            fIsNoBloodDecal    = 0x00001000,
            //fIsSummonable      = 0x00004000, //OBSE
            fIsNoHead          = 0x00008000,
            fIsNoRightArm      = 0x00010000,
            fIsNoLeftArm       = 0x00020000,
            fIsNoCombatInWater = 0x00040000,
            fIsNoShadow        = 0x00080000,
            fIsNoCorpseCheck   = 0x00100000
            };

        enum servicesFlags
            {
            fWeapons     = 0x00000001,
            fArmor       = 0x00000002,
            fClothing    = 0x00000004,
            fBooks       = 0x00000008,
            fIngredients = 0x00000010,
            fLights      = 0x00000080,
            fApparatus   = 0x00000100,
            fMiscItems   = 0x00000400,
            fSpells      = 0x00000800,
            fMagicItems  = 0x00001000,
            fPotions     = 0x00002000,
            fTraining    = 0x00004000,
            fRecharge    = 0x00010000,
            fRepair      = 0x00020000
            };

        enum eCreatureType
            {
            eCreature = 0,
            eDaedra   = 1,
            eUndead   = 2,
            eHumanoid = 3,
            eHorse    = 4,
            eGiant    = 5
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
        std::vector<FORMID> SPLO;
        std::vector<StringRecord> NIFZ;
        RawRecord NIFT;
        ReqSubRecord<GENACBS> ACBS;
        std::vector<ReqSubRecord<GENSNAM> *> SNAM;
        OptSubRecord<GENFID> INAM;
        OptSubRecord<GENFID> SCRI;
        std::vector<ReqSubRecord<GENCNTO> *> CNTO;
        ReqSubRecord<GENAIDT> AIDT;
        std::vector<FORMID> PKID;
        std::vector<StringRecord> KFFZ;
        ReqSubRecord<CREADATA> DATA;
        ReqSubRecord<CREARNAM> RNAM;
        OptSubRecord<GENFID> ZNAM;
        ReqSubRecord<CREATNAM> TNAM;
        ReqSubRecord<CREABNAM> BNAM;
        ReqSubRecord<CREAWNAM> WNAM;
        OptSubRecord<GENFID> CSCR;
        StringRecord NAM0;
        StringRecord NAM1;
        std::vector<CREASound *> Sounds;

        CREARecord(unsigned char *_recData=NULL);
        CREARecord(CREARecord *srcRecord);
        ~CREARecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsBiped();
        void   IsBiped(bool value);
        bool   IsEssential();
        void   IsEssential(bool value);
        bool   IsWeaponAndShield();
        void   IsWeaponAndShield(bool value);
        bool   IsRespawn();
        void   IsRespawn(bool value);
        bool   IsSwims();
        void   IsSwims(bool value);
        bool   IsFlies();
        void   IsFlies(bool value);
        bool   IsWalks();
        void   IsWalks(bool value);
        bool   IsPCLevelOffset();
        void   IsPCLevelOffset(bool value);
        bool   IsNoLowLevel();
        void   IsNoLowLevel(bool value);
        bool   IsLowLevel();
        void   IsLowLevel(bool value);
        bool   IsNoBloodSpray();
        void   IsNoBloodSpray(bool value);
        bool   IsBloodSpray();
        void   IsBloodSpray(bool value);
        bool   IsNoBloodDecal();
        void   IsNoBloodDecal(bool value);
        bool   IsBloodDecal();
        void   IsBloodDecal(bool value);
        bool   IsNoHead();
        void   IsNoHead(bool value);
        bool   IsHead();
        void   IsHead(bool value);
        bool   IsNoRightArm();
        void   IsNoRightArm(bool value);
        bool   IsRightArm();
        void   IsRightArm(bool value);
        bool   IsNoLeftArm();
        void   IsNoLeftArm(bool value);
        bool   IsLeftArm();
        void   IsLeftArm(bool value);
        bool   IsNoCombatInWater();
        void   IsNoCombatInWater(bool value);
        bool   IsCombatInWater();
        void   IsCombatInWater(bool value);
        bool   IsNoShadow();
        void   IsNoShadow(bool value);
        bool   IsShadow();
        void   IsShadow(bool value);
        bool   IsNoCorpseCheck();
        void   IsNoCorpseCheck(bool value);
        bool   IsCorpseCheck();
        void   IsCorpseCheck(bool value);
        bool   IsFlagMask(UINT32 Mask, bool Exact=false);
        void   SetFlagMask(UINT32 Mask);

        bool   IsCreature();
        void   IsCreature(bool value);
        bool   IsDaedra();
        void   IsDaedra(bool value);
        bool   IsUndead();
        void   IsUndead(bool value);
        bool   IsHumanoid();
        void   IsHumanoid(bool value);
        bool   IsHorse();
        void   IsHorse(bool value);
        bool   IsGiant();
        void   IsGiant(bool value);
        bool   IsType(UINT8 Type);
        void   SetType(UINT8 Type);

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

        bool   IsServicesWeapons();
        void   IsServicesWeapons(bool value);
        bool   IsServicesArmor();
        void   IsServicesArmor(bool value);
        bool   IsServicesClothing();
        void   IsServicesClothing(bool value);
        bool   IsServicesBooks();
        void   IsServicesBooks(bool value);
        bool   IsServicesIngredients();
        void   IsServicesIngredients(bool value);
        bool   IsServicesLights();
        void   IsServicesLights(bool value);
        bool   IsServicesApparatus();
        void   IsServicesApparatus(bool value);
        bool   IsServicesMiscItems();
        void   IsServicesMiscItems(bool value);
        bool   IsServicesSpells();
        void   IsServicesSpells(bool value);
        bool   IsServicesMagicItems();
        void   IsServicesMagicItems(bool value);
        bool   IsServicesPotions();
        void   IsServicesPotions(bool value);
        bool   IsServicesTraining();
        void   IsServicesTraining(bool value);
        bool   IsServicesRecharge();
        void   IsServicesRecharge(bool value);
        bool   IsServicesRepair();
        void   IsServicesRepair(bool value);
        bool   IsServicesFlagMask(UINT32 Mask, bool Exact=false);
        void   SetServicesFlagMask(UINT32 Mask);

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

        bool operator ==(const CREARecord &other) const;
        bool operator !=(const CREARecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(CREARecord *)other;}
    };