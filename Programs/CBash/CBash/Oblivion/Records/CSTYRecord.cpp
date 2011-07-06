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
#include "..\..\Common.h"
#include "CSTYRecord.h"

CSTYRecord::CSTYCSTD::CSTYCSTD():
    dodgeChance(75),
    lrChance(50),
    lrTimerMin(0.5f),
    lrTimerMax(1.5f),
    forTimerMin(0.5f),
    forTimerMax(1.0f),
    backTimerMin(0.25f),
    backTimerMax(0.75f),
    idleTimerMin(0.5f),
    idleTimerMax(1.5f),
    blkChance(30),
    atkChance(40),
    atkBRecoil(5.0f),
    atkBUnc(5.0f),
    atkBh2h(5.0f),
    pAtkChance(25),
    pAtkBRecoil(5.0f),
    pAtkBUnc(5.0f),
    pAtkNormal(20),
    pAtkFor(20),
    pAtkBack(20),
    pAtkL(20),
    pAtkR(20),
    holdTimerMin(0.5f),
    holdTimerMax(1.5f),
    flagsA(0),
    acroDodge(0),
    rMultOpt(1.0f),
    rMultMax(1.0f),
    mDistance(250.0f),
    rDistance(1000.0f),
    buffStand(325.0f),
    rStand(500.0f),
    groupStand(325.0f),
    rushChance(25),
    rushMult(1.0f),
    flagsB(0)
    {
     memset(&unused1, 0xCD, 2);
     memset(&unused2, 0xCD, 2);
     memset(&unused3, 0xCD, 3);
     memset(&unused4, 0xCD, 3);
     memset(&unused5, 0xCD, 2);
     memset(&unused6, 0xCD, 3);
    }

CSTYRecord::CSTYCSTD::~CSTYCSTD()
    {
    //
    }

bool CSTYRecord::CSTYCSTD::operator ==(const CSTYCSTD &other) const
    {
    return (dodgeChance == other.dodgeChance &&
            lrChance == other.lrChance &&
            AlmostEqual(lrTimerMin,other.lrTimerMin,2) &&
            AlmostEqual(lrTimerMax,other.lrTimerMax,2) &&
            AlmostEqual(forTimerMin,other.forTimerMin,2) &&
            AlmostEqual(forTimerMax,other.forTimerMax,2) &&
            AlmostEqual(backTimerMin,other.backTimerMin,2) &&
            AlmostEqual(backTimerMax,other.backTimerMax,2) &&
            AlmostEqual(idleTimerMin,other.idleTimerMin,2) &&
            AlmostEqual(idleTimerMax,other.idleTimerMax,2) &&
            blkChance == other.blkChance &&
            atkChance == other.atkChance &&
            AlmostEqual(atkBRecoil,other.atkBRecoil,2) &&
            AlmostEqual(atkBUnc,other.atkBUnc,2) &&
            AlmostEqual(atkBh2h,other.atkBh2h,2) &&
            pAtkChance == other.pAtkChance &&
            AlmostEqual(pAtkBRecoil,other.pAtkBRecoil,2) &&
            AlmostEqual(pAtkBUnc,other.pAtkBUnc,2) &&
            pAtkNormal == other.pAtkNormal &&
            pAtkFor == other.pAtkFor &&
            pAtkBack == other.pAtkBack &&
            pAtkL == other.pAtkL &&
            pAtkR == other.pAtkR &&
            AlmostEqual(holdTimerMin,other.holdTimerMin,2) &&
            AlmostEqual(holdTimerMax,other.holdTimerMax,2) &&
            flagsA == other.flagsA &&
            acroDodge == other.acroDodge &&
            AlmostEqual(rMultOpt,other.rMultOpt,2) &&
            AlmostEqual(rMultMax,other.rMultMax,2) &&
            AlmostEqual(mDistance,other.mDistance,2) &&
            AlmostEqual(rDistance,other.rDistance,2) &&
            AlmostEqual(buffStand,other.buffStand,2) &&
            AlmostEqual(rStand,other.rStand,2) &&
            AlmostEqual(groupStand,other.groupStand,2) &&
            rushChance == other.rushChance &&
            AlmostEqual(rushMult,other.rushMult,2) &&
            flagsB == other.flagsB);
    }

bool CSTYRecord::CSTYCSTD::operator !=(const CSTYCSTD &other) const
    {
    return !(*this == other);
    }

CSTYRecord::CSTYCSAD::CSTYCSAD():
    dodgeFMult(-20.0f),
    dodgeFBase(0.0f),
    encSBase(-110.0f),
    encSMult(1.0f),
    dodgeAtkMult(1.0f),
    dodgeNAtkMult(0.75f),
    dodgeBAtkMult(1.0f),
    dodgeBNAtkMult(0.7f),
    dodgeFAtkMult(1.0f),
    dodgeFNAtkMult(0.5f),
    blockMult(20.0f),
    blockBase(0.0f),
    blockAtkMult(2.0f),
    blockNAtkMult(1.0f),
    atkMult(20.0f),
    atkBase(0.0f),
    atkAtkMult(0.75f),
    atkNAtkMult(1.0f),
    atkBlockMult(0.5f),
    pAtkFBase(5.0f),
    pAtkFMult(-10.0f)
    {
    //
    }

CSTYRecord::CSTYCSAD::~CSTYCSAD()
    {
    //
    }

bool CSTYRecord::CSTYCSAD::operator ==(const CSTYCSAD &other) const
    {
    return (AlmostEqual(dodgeFMult,other.dodgeFMult,2) &&
            AlmostEqual(dodgeFBase,other.dodgeFBase,2) &&
            AlmostEqual(encSBase,other.encSBase,2) &&
            AlmostEqual(encSMult,other.encSMult,2) &&
            AlmostEqual(dodgeAtkMult,other.dodgeAtkMult,2) &&

            AlmostEqual(dodgeNAtkMult,other.dodgeNAtkMult,2) &&
            AlmostEqual(dodgeBAtkMult,other.dodgeBAtkMult,2) &&
            AlmostEqual(dodgeBNAtkMult,other.dodgeBNAtkMult,2) &&

            AlmostEqual(dodgeFAtkMult,other.dodgeFAtkMult,2) &&
            AlmostEqual(dodgeFNAtkMult,other.dodgeFNAtkMult,2) &&
            AlmostEqual(blockMult,other.blockMult,2) &&
            AlmostEqual(blockBase,other.blockBase,2) &&

            AlmostEqual(blockAtkMult,other.blockAtkMult,2) &&
            AlmostEqual(blockNAtkMult,other.blockNAtkMult,2) &&
            AlmostEqual(atkMult,other.atkMult,2) &&
            AlmostEqual(atkBase,other.atkBase,2) &&
            AlmostEqual(atkAtkMult,other.atkAtkMult,2) &&

            AlmostEqual(atkNAtkMult,other.atkNAtkMult,2) &&
            AlmostEqual(atkBlockMult,other.atkBlockMult,2) &&
            AlmostEqual(pAtkFBase,other.pAtkFBase,2) &&
            AlmostEqual(pAtkFMult,other.pAtkFMult,2));
    }

bool CSTYRecord::CSTYCSAD::operator !=(const CSTYCSAD &other) const
    {
    return !(*this == other);
    }

CSTYRecord::CSTYRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

CSTYRecord::CSTYRecord(CSTYRecord *srcRecord):
    Record()
    {
    if(srcRecord == NULL)
        return;

    flags = srcRecord->flags;
    formID = srcRecord->formID;
    flagsUnk = srcRecord->flagsUnk;

    if(!srcRecord->IsChanged())
        {
        IsLoaded(false);
        recData = srcRecord->recData;
        return;
        }

    EDID = srcRecord->EDID;
    CSTD = srcRecord->CSTD;
    CSAD = srcRecord->CSAD;
    return;
    }

CSTYRecord::~CSTYRecord()
    {
    //
    }

bool CSTYRecord::IsUseAdvanced()
    {
    return (CSTD.value.flagsA & fIsUseAdvanced) != 0;
    }

void CSTYRecord::IsUseAdvanced(bool value)
    {
    CSTD.value.flagsA = value ? (CSTD.value.flagsA | fIsUseAdvanced) : (CSTD.value.flagsA & ~fIsUseAdvanced);
    }

bool CSTYRecord::IsUseChanceForAttack()
    {
    return (CSTD.value.flagsA & fIsUseChanceForAttack) != 0;
    }

void CSTYRecord::IsUseChanceForAttack(bool value)
    {
    CSTD.value.flagsA = value ? (CSTD.value.flagsA | fIsUseChanceForAttack) : (CSTD.value.flagsA & ~fIsUseChanceForAttack);
    }

bool CSTYRecord::IsIgnoreAllies()
    {
    return (CSTD.value.flagsA & fIsIgnoreAllies) != 0;
    }

void CSTYRecord::IsIgnoreAllies(bool value)
    {
    CSTD.value.flagsA = value ? (CSTD.value.flagsA | fIsIgnoreAllies) : (CSTD.value.flagsA & ~fIsIgnoreAllies);
    }

bool CSTYRecord::IsWillYield()
    {
    return (CSTD.value.flagsA & fIsWillYield) != 0;
    }

void CSTYRecord::IsWillYield(bool value)
    {
    CSTD.value.flagsA = value ? (CSTD.value.flagsA | fIsWillYield) : (CSTD.value.flagsA & ~fIsWillYield);
    }

bool CSTYRecord::IsRejectsYields()
    {
    return (CSTD.value.flagsA & fIsRejectsYields) != 0;
    }

void CSTYRecord::IsRejectsYields(bool value)
    {
    CSTD.value.flagsA = value ? (CSTD.value.flagsA | fIsRejectsYields) : (CSTD.value.flagsA & ~fIsRejectsYields);
    }

bool CSTYRecord::IsFleeingDisabled()
    {
    return (CSTD.value.flagsA & fIsFleeingDisabled) != 0;
    }

void CSTYRecord::IsFleeingDisabled(bool value)
    {
    CSTD.value.flagsA = value ? (CSTD.value.flagsA | fIsFleeingDisabled) : (CSTD.value.flagsA & ~fIsFleeingDisabled);
    }

bool CSTYRecord::IsPrefersRanged()
    {
    return (CSTD.value.flagsA & fIsPrefersRanged) != 0;
    }

void CSTYRecord::IsPrefersRanged(bool value)
    {
    CSTD.value.flagsA = value ? (CSTD.value.flagsA | fIsPrefersRanged) : (CSTD.value.flagsA & ~fIsPrefersRanged);
    }

bool CSTYRecord::IsMeleeAlertOK()
    {
    return (CSTD.value.flagsA & fIsMeleeAlertOK) != 0;
    }

void CSTYRecord::IsMeleeAlertOK(bool value)
    {
    CSTD.value.flagsA = value ? (CSTD.value.flagsA | fIsMeleeAlertOK) : (CSTD.value.flagsA & ~fIsMeleeAlertOK);
    }

bool CSTYRecord::IsFlagAMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((CSTD.value.flagsA & Mask) == Mask) : ((CSTD.value.flagsA & Mask) != 0);
    }

void CSTYRecord::SetFlagAMask(UINT8 Mask)
    {
    CSTD.value.flagsA = Mask;
    }

bool CSTYRecord::IsDoNotAcquire()
    {
    return (CSTD.value.flagsB & fIsDoNotAcquire) != 0;
    }

void CSTYRecord::IsDoNotAcquire(bool value)
    {
    CSTD.value.flagsB = value ? (CSTD.value.flagsB | fIsDoNotAcquire) : (CSTD.value.flagsB & ~fIsDoNotAcquire);
    }

bool CSTYRecord::IsFlagBMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((CSTD.value.flagsB & Mask) == Mask) : ((CSTD.value.flagsB & Mask) != 0);
    }

void CSTYRecord::SetFlagBMask(UINT8 Mask)
    {
    CSTD.value.flagsB = Mask;
    }

UINT32 CSTYRecord::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 0;

    if(EDID.IsLoaded())
        {
        cSize = EDID.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(CSTD.IsLoaded())
        TotSize += CSTD.GetSize() + 6;

    if(CSAD.IsLoaded())
        TotSize += CSAD.GetSize() + 6;

    return TotSize;
    }

UINT32 CSTYRecord::GetType()
    {
    return 'YTSC';
    }

STRING CSTYRecord::GetStrType()
    {
    return "CSTY";
    }

SINT32 CSTYRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    while(curPos < recSize){
        _readBuffer(&subType, buffer, 4, curPos);
        switch(subType)
            {
            case 'XXXX':
                curPos += 2;
                _readBuffer(&subSize, buffer, 4, curPos);
                _readBuffer(&subType, buffer, 4, curPos);
                curPos += 2;
                break;
            default:
                subSize = 0;
                _readBuffer(&subSize, buffer, 2, curPos);
                break;
            }
        switch(subType)
            {
            case 'DIDE':
                EDID.Read(buffer, subSize, curPos);
                break;
            case 'DTSC':
                CSTD.Read(buffer, subSize, curPos);
                break;
            case 'DASC':
                CSAD.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  CSTY: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 CSTYRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    CSTD.Unload();
    CSAD.Unload();
    return 1;
    }

SINT32 CSTYRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(CSTD.IsLoaded())
        SaveHandler.writeSubRecord('DTSC', &CSTD.value, CSTD.GetSize());
    if(CSAD.IsLoaded())
        SaveHandler.writeSubRecord('DASC', CSAD.value, CSAD.GetSize());
    return -1;
    }

bool CSTYRecord::operator ==(const CSTYRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            CSTD == other.CSTD &&
            CSAD == other.CSAD);
    }

bool CSTYRecord::operator !=(const CSTYRecord &other) const
    {
    return !(*this == other);
    }