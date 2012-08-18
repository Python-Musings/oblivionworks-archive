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

class CSTYRecord : public Record
    {
    private:
        struct CSTYCSTD
            {
            UINT8   dodgeChance, lrChance, unused1[2];
            FLOAT32 lrTimerMin, lrTimerMax, forTimerMin, forTimerMax, backTimerMin,
                    backTimerMax, idleTimerMin, idleTimerMax;
            UINT8   blkChance, atkChance, unused2[2];
            FLOAT32 atkBRecoil, atkBUnc, atkBh2h;
            UINT8   pAtkChance, unused3[3];
            FLOAT32 pAtkBRecoil, pAtkBUnc;
            UINT8   pAtkNormal, pAtkFor, pAtkBack, pAtkL, pAtkR, unused4[3];
            FLOAT32 holdTimerMin, holdTimerMax;
            UINT8   flagsA, acroDodge, unused5[2];
            FLOAT32 rMultOpt, rMultMax, mDistance, rDistance, buffStand, rStand, groupStand;
            UINT8   rushChance, unused6[3];
            FLOAT32 rushMult;
            UINT32  flagsB;

            CSTYCSTD();
            ~CSTYCSTD();

            bool operator ==(const CSTYCSTD &other) const;
            bool operator !=(const CSTYCSTD &other) const;
            };

        struct CSTYCSAD
            {
            FLOAT32 dodgeFMult, dodgeFBase, encSBase, encSMult, dodgeAtkMult,
                    dodgeNAtkMult, dodgeBAtkMult, dodgeBNAtkMult,
                    dodgeFAtkMult, dodgeFNAtkMult, blockMult, blockBase,
                    blockAtkMult, blockNAtkMult, atkMult, atkBase, atkAtkMult,
                    atkNAtkMult, atkBlockMult, pAtkFBase, pAtkFMult;

            CSTYCSAD();
            ~CSTYCSAD();

            bool operator ==(const CSTYCSAD &other) const;
            bool operator !=(const CSTYCSAD &other) const;
            };

        enum flagsAFlags
            {
            fIsUseAdvanced        = 0x00000001,
            fIsUseChanceForAttack = 0x00000002,
            fIsIgnoreAllies       = 0x00000004,
            fIsWillYield          = 0x00000008,
            fIsRejectsYields      = 0x00000010,
            fIsFleeingDisabled    = 0x00000020,
            fIsPrefersRanged      = 0x00000040,
            fIsMeleeAlertOK       = 0x00000080
            };

        enum flagsBFlags
            {
            fIsDoNotAcquire       = 0x00000001 //OBSE, ignored?
            };

    public:
        StringRecord EDID;
        ReqSubRecord<CSTYCSTD> CSTD;
        SemiOptSubRecord<CSTYCSAD> CSAD;

        CSTYRecord(unsigned char *_recData=NULL);
        CSTYRecord(CSTYRecord *srcRecord);
        ~CSTYRecord();

        bool   IsUseAdvanced();
        void   IsUseAdvanced(bool value);
        bool   IsUseChanceForAttack();
        void   IsUseChanceForAttack(bool value);
        bool   IsIgnoreAllies();
        void   IsIgnoreAllies(bool value);
        bool   IsWillYield();
        void   IsWillYield(bool value);
        bool   IsRejectsYields();
        void   IsRejectsYields(bool value);
        bool   IsFleeingDisabled();
        void   IsFleeingDisabled(bool value);
        bool   IsPrefersRanged();
        void   IsPrefersRanged(bool value);
        bool   IsMeleeAlertOK();
        void   IsMeleeAlertOK(bool value);
        bool   IsFlagAMask(UINT8 Mask, bool Exact=false);
        void   SetFlagAMask(UINT8 Mask);

        bool   IsDoNotAcquire();
        void   IsDoNotAcquire(bool value);
        bool   IsFlagBMask(UINT8 Mask, bool Exact=false);
        void   SetFlagBMask(UINT8 Mask);

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

        bool operator ==(const CSTYRecord &other) const;
        bool operator !=(const CSTYRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(CSTYRecord *)other;}
    };