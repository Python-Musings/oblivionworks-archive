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

class RACERecord : public Record
    {
    private:
        struct RACESKILL
            {
            SINT8   value, boost;

            RACESKILL();
            ~RACESKILL();

            bool operator ==(const RACESKILL &other) const;
            bool operator !=(const RACESKILL &other) const;
            };

        struct RACEDATA
            {
            RACESKILL skills[7];
            UINT8   unused1[2];
            FLOAT32 maleHeight, femaleHeight, maleWeight, femaleWeight;
            UINT32  flags;

            RACEDATA();
            ~RACEDATA();

            bool operator ==(const RACEDATA &other) const;
            bool operator !=(const RACEDATA &other) const;
            };

        struct RACEVNAM
            {
            FORMID  maleVoice, femaleVoice;

            RACEVNAM();
            ~RACEVNAM();

            bool operator ==(const RACEVNAM &other) const;
            bool operator !=(const RACEVNAM &other) const;
            };

        struct RACEDNAM
            {
            FORMID  defaultHairMale, defaultHairFemale;

            RACEDNAM();
            ~RACEDNAM();

            bool operator ==(const RACEDNAM &other) const;
            bool operator !=(const RACEDNAM &other) const;
            };

        struct RACECNAM
            {
            UINT8   defaultHairColor;

            RACECNAM();
            ~RACECNAM();

            bool operator ==(const RACECNAM &other) const;
            bool operator !=(const RACECNAM &other) const;
            };

        struct RACEPNAM
            {
            FLOAT32 mainClamp;

            RACEPNAM();
            ~RACEPNAM();

            bool operator ==(const RACEPNAM &other) const;
            bool operator !=(const RACEPNAM &other) const;
            };

        struct RACEUNAM
            {
            FLOAT32 faceClamp;

            RACEUNAM();
            ~RACEUNAM();

            bool operator ==(const RACEUNAM &other) const;
            bool operator !=(const RACEUNAM &other) const;
            };

        struct RACEATTR
            {
            UINT8   maleStrength, maleIntelligence, maleWillpower,
                    maleAgility, maleSpeed, maleEndurance,
                    malePersonality, maleLuck;
            UINT8   femaleStrength, femaleIntelligence, femaleWillpower,
                    femaleAgility, femaleSpeed, femaleEndurance,
                    femalePersonality, femaleLuck;

            RACEATTR();
            ~RACEATTR();

            bool operator ==(const RACEATTR &other) const;
            bool operator !=(const RACEATTR &other) const;
            };

        struct RACEMODEL
            {
            ReqSubRecord<MODELMODB> MODB;
            StringRecord MODL;
            RawRecord MODT;
            StringRecord ICON;

            bool operator ==(const RACEMODEL &other) const;
            bool operator !=(const RACEMODEL &other) const;
            };

        struct RACESNAM
            {
            UINT8   SNAM[2];

            RACESNAM();
            ~RACESNAM();

            bool operator ==(const RACESNAM &other) const;
            bool operator !=(const RACESNAM &other) const;
            };

        enum flagsFlags
            {
            fIsPlayable   = 0x00000001
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        StringRecord DESC;
        std::vector<FORMID> SPLO;
        std::vector<ReqSubRecord<GENXNAM> *> XNAM;
        ReqSubRecord<RACEDATA> DATA;
        SubRecord<RACEVNAM> VNAM;
        SubRecord<RACEDNAM> DNAM;
        ReqSubRecord<RACECNAM> CNAM;
        OptSubRecord<RACEPNAM> PNAM;
        OptSubRecord<RACEUNAM> UNAM;
        ReqSubRecord<RACEATTR> ATTR;
        OptSubRecord<RACEMODEL> MOD0;
        OptSubRecord<RACEMODEL> MOD1;
        OptSubRecord<RACEMODEL> MOD2;
        OptSubRecord<RACEMODEL> MOD3;
        OptSubRecord<RACEMODEL> MOD4;
        OptSubRecord<RACEMODEL> MOD5;
        OptSubRecord<RACEMODEL> MOD6;
        OptSubRecord<RACEMODEL> MOD7;
        OptSubRecord<RACEMODEL> MOD8;
        OptSubRecord<GENMODEL> MMODL;
        StringRecord MICON0;
        StringRecord MICON1;
        StringRecord MICON2;
        StringRecord MICON3;
        StringRecord MICON4;
        OptSubRecord<GENMODEL> FMODL;
        StringRecord FICON0;
        StringRecord FICON1;
        StringRecord FICON2;
        StringRecord FICON3;
        StringRecord FICON4;
        std::vector<FORMID> HNAM;
        std::vector<FORMID> ENAM;
        RawRecord FGGS; //FaceGen Geometry-Symmetric
        RawRecord FGGA; //FaceGen Geometry-Asymmetric
        RawRecord FGTS; //FaceGen Texture-Symmetric
        ReqSubRecord<RACESNAM> SNAM;

        RACERecord(unsigned char *_recData=NULL);
        RACERecord(RACERecord *srcRecord);
        ~RACERecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsPlayable();
        void   IsPlayable(bool value);
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

        bool operator ==(const RACERecord &other) const;
        bool operator !=(const RACERecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(RACERecord *)other;}
    };