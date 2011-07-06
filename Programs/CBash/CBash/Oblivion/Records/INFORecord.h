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

class INFORecord : public Record
    {
    private:
        struct INFODATA
            {
            UINT16  dialType;
            UINT8   flags;

            INFODATA();
            ~INFODATA();

            bool operator ==(const INFODATA &other) const;
            bool operator !=(const INFODATA &other) const;
            };

        struct INFOTRDT
            {
            UINT32  emotionType;
            SINT32  emotionValue;
            UINT8   unused1[4], responseNum, unused2[3];

            INFOTRDT();
            ~INFOTRDT();

            bool operator ==(const INFOTRDT &other) const;
            bool operator !=(const INFOTRDT &other) const;
            };

        struct INFOResponse
            {
            ReqSubRecord<INFOTRDT> TRDT;
            StringRecord NAM1;
            StringRecord NAM2;

            enum eEmotionType
                {
                eNeutral  = 0,
                eAnger    = 1,
                eDisgust  = 2,
                eFear     = 3,
                eSad      = 4,
                eHappy    = 5,
                eSurprise = 6
                };

            bool   IsNeutral();
            void   IsNeutral(bool value);
            bool   IsAnger();
            void   IsAnger(bool value);
            bool   IsDisgust();
            void   IsDisgust(bool value);
            bool   IsFear();
            void   IsFear(bool value);
            bool   IsSad();
            void   IsSad(bool value);
            bool   IsHappy();
            void   IsHappy(bool value);
            bool   IsSurprise();
            void   IsSurprise(bool value);
            bool   IsType(UINT32 Type);
            void   SetType(UINT32 Type);

            bool operator ==(const INFOResponse &other) const;
            bool operator !=(const INFOResponse &other) const;
            };

        enum eDialogType
            {
            eTopic        = 0,
            eConversation = 1,
            eCombat       = 2,
            ePersuasion   = 3,
            eDetection    = 4,
            eService      = 5,
            eMisc         = 6
            };

        enum flagsFlags
            {
            fIsGoodbye        = 0x00000001,
            fIsRandom         = 0x00000002,
            fIsSayOnce        = 0x00000004,
            fIsRunImmediately = 0x00000008,
            fIsInfoRefusal    = 0x00000010,
            fIsRandomEnd      = 0x00000020,
            fIsRunForRumors   = 0x00000040
            };

    public:
        StringRecord EDID;
        ReqSubRecord<INFODATA> DATA;
        ReqSubRecord<GENFID> QSTI;
        OptSubRecord<GENFID> TPIC;
        SemiOptSubRecord<GENFID> PNAM;
        std::vector<FORMID> NAME;
        std::vector<INFOResponse *> Responses;
        std::vector<ReqSubRecord<GENCTDA> *> CTDA;
        std::vector<FORMID> TCLT;
        std::vector<FORMID> TCLF;
        ReqSubRecord<GENSCHR> SCHR;
        RawRecord SCDA;
        NonNullStringRecord SCTX;
        std::vector<ReqSubRecord<GENSCR_> *> SCR_;

        INFORecord(unsigned char *_recData=NULL);
        INFORecord(INFORecord *srcRecord);
        ~INFORecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsTopic();
        void   IsTopic(bool value);
        bool   IsConversation();
        void   IsConversation(bool value);
        bool   IsCombat();
        void   IsCombat(bool value);
        bool   IsPersuasion();
        void   IsPersuasion(bool value);
        bool   IsDetection();
        void   IsDetection(bool value);
        bool   IsService();
        void   IsService(bool value);
        bool   IsMisc();
        void   IsMisc(bool value);
        bool   IsDialogType(UINT16 Type);
        void   SetDialogType(UINT16 Type);

        bool   IsGoodbye();
        void   IsGoodbye(bool value);
        bool   IsRandom();
        void   IsRandom(bool value);
        bool   IsSayOnce();
        void   IsSayOnce(bool value);
        bool   IsRunImmediately();
        void   IsRunImmediately(bool value);
        bool   IsInfoRefusal();
        void   IsInfoRefusal(bool value);
        bool   IsRandomEnd();
        void   IsRandomEnd(bool value);
        bool   IsRunForRumors();
        void   IsRunForRumors(bool value);
        bool   IsFlagMask(UINT8 Mask, bool Exact=false);
        void   SetFlagMask(UINT8 Mask);

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

        bool operator ==(const INFORecord &other) const;
        bool operator !=(const INFORecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(INFORecord *)other;}
    };