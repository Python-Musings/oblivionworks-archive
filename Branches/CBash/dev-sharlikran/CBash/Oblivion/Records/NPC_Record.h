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

class NPC_Record : public Record
    {
    private:
        #pragma pack(push)
        #pragma pack(1)
        struct NPC_DATA
            {
            UINT8   armorer, athletics, blade, block, blunt,
                    h2h, heavyArmor, alchemy, alteration,
                    conjuration, destruction, illusion,
                    mysticism, restoration, acrobatics,
                    lightArmor, marksman, mercantile,
                    security, sneak, speechcraft;
            UINT16  health;
            UINT8   unused1[2], strength, intelligence,
                    willpower, agility, speed, endurance,
                    personality, luck;

            NPC_DATA();
            ~NPC_DATA();

            bool operator ==(const NPC_DATA &other) const;
            bool operator !=(const NPC_DATA &other) const;
            };
        #pragma pack(pop)

        struct NPC_LNAM
            {
            FLOAT32 hairLength;

            NPC_LNAM();
            ~NPC_LNAM();

            bool operator ==(const NPC_LNAM &other) const;
            bool operator !=(const NPC_LNAM &other) const;
            };

        struct NPC_FNAM
            {
            UINT16  fnam;

            NPC_FNAM();
            ~NPC_FNAM();

            bool operator ==(const NPC_FNAM &other) const;
            bool operator !=(const NPC_FNAM &other) const;
            };

        enum flagsFlags
            {
            fIsFemale         = 0x00000001,
            fIsEssential      = 0x00000002,
            fIsRespawn        = 0x00000008,
            fIsAutoCalc       = 0x00000010,
            fIsPCLevelOffset  = 0x00000080,
            fIsNoLowLevel     = 0x00000200,
            fIsNoRumors       = 0x00002000,
            fIsSummonable     = 0x00004000,
            fIsNoPersuasion   = 0x00008000,
            fIsCanCorpseCheck = 0x00100000
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

    public:
        StringRecord EDID;
        StringRecord FULL;
        OptSubRecord<GENMODEL> MODL;
        ReqSubRecord<GENACBS> ACBS;
        std::vector<ReqSubRecord<GENSNAM> *> SNAM;
        OptSubRecord<GENFID> INAM;
        OptSubRecord<GENFID> RNAM;
        std::vector<FORMID> SPLO;
        OptSubRecord<GENFID> SCRI;
        std::vector<ReqSubRecord<GENCNTO> *> CNTO;
        ReqSubRecord<GENAIDT> AIDT;
        std::vector<FORMID> PKID;
        std::vector<StringRecord> KFFZ;
        ReqSubRecord<GENFID> CNAM;
        ReqSubRecord<NPC_DATA> DATA;
        OptSubRecord<GENFID> HNAM;
        SemiOptSubRecord<NPC_LNAM> LNAM;
        OptSubRecord<GENFID> ENAM;
        ReqSubRecord<GENCLR> HCLR;
        OptSubRecord<GENFID> ZNAM;
        RawRecord FGGS; //FaceGen Geometry-Symmetric
        RawRecord FGGA; //FaceGen Geometry-Asymmetric
        RawRecord FGTS; //FaceGen Texture-Symmetric
        ReqSubRecord<NPC_FNAM> FNAM;

        NPC_Record(unsigned char *_recData=NULL);
        NPC_Record(NPC_Record *srcRecord);
        ~NPC_Record();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsFemale();
        void   IsFemale(bool value);
        bool   IsMale();
        void   IsMale(bool value);
        bool   IsEssential();
        void   IsEssential(bool value);
        bool   IsRespawn();
        void   IsRespawn(bool value);
        bool   IsAutoCalc();
        void   IsAutoCalc(bool value);
        bool   IsPCLevelOffset();
        void   IsPCLevelOffset(bool value);
        bool   IsNoLowLevel();
        void   IsNoLowLevel(bool value);
        bool   IsLowLevel();
        void   IsLowLevel(bool value);
        bool   IsNoRumors();
        void   IsNoRumors(bool value);
        bool   IsRumors();
        void   IsRumors(bool value);
        bool   IsSummonable();
        void   IsSummonable(bool value);
        bool   IsNoPersuasion();
        void   IsNoPersuasion(bool value);
        bool   IsPersuasion();
        void   IsPersuasion(bool value);
        bool   IsCanCorpseCheck();
        void   IsCanCorpseCheck(bool value);
        bool   IsFlagMask(UINT32 Mask, bool Exact=false);
        void   SetFlagMask(UINT32 Mask);

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

        bool operator ==(const NPC_Record &other) const;
        bool operator !=(const NPC_Record &other) const;
		virtual bool equals(const Record *other) const {return *this == *(NPC_Record *)other;}
    };