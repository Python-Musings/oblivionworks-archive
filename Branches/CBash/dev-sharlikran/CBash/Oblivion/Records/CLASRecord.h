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

class CLASRecord : public Record
    {
    private:
        struct CLASDATA
            {
            SINT32  primary[2];
            UINT32  specialization;
            SINT32  major[7];
            UINT32  flags, services;
            SINT8   trainSkill;
            UINT8   trainLevel;
            SINT8   unused1[2];

            CLASDATA();
            ~CLASDATA();

            bool operator ==(const CLASDATA &other) const;
            bool operator !=(const CLASDATA &other) const;
            };

        enum flagsFlags
            {
            fIsPlayable = 0x00000001,
            fIsGuard    = 0x00000002
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
        StringRecord DESC;
        StringRecord ICON;
        ReqSubRecord<CLASDATA> DATA;

        CLASRecord(unsigned char *_recData=NULL);
        CLASRecord(CLASRecord *srcRecord);
        ~CLASRecord();

        bool   IsPlayable();
        void   IsPlayable(bool value);
        bool   IsGuard();
        void   IsGuard(bool value);
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
        bool   IsFlagMask(UINT32 Mask, bool Exact=false);
        void   SetFlagMask(UINT32 Mask);
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

        bool operator ==(const CLASRecord &other) const;
        bool operator !=(const CLASRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(CLASRecord *)other;}
    };