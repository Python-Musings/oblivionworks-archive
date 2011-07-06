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
#include "EFSHRecord.h"

EFSHRecord::EFSHDATA::EFSHDATA():
    flags(0),
    memSBlend(5),
    memBlendOp(1),
    memZFunc(3),
    fillAIn(0.0f),
    fillAFull(0.0f),
    fillAOut(0.0f),
    fillAPRatio(0.0f),
    fillAAmp(0.0f),
    fillAFreq(1.0f),
    fillAnimSpdU(0.0f),
    fillAnimSpdV(0.0f),
    edgeOff(1.0f),
    edgeAIn(0.0f),
    edgeAFull(0.0f),
    edgeAOut(0.0f),
    edgeAPRatio(0.0f),
    edgeAAmp(0.0f),
    edgeAFreq(1.0f),
    fillAFRatio(1.0f),
    edgeAFRatio(1.0f),
    memDBlend(6),
    partSBlend(5),
    partBlendOp(1),
    partZFunc(4),
    partDBlend(6),
    partBUp(0.0f),
    partBFull(0.0f),
    partBDown(0.0f),
    partBFRatio(1.0f),
    partBPRatio(1.0f),
    partLTime(1.0f),
    partLDelta(0.0f),
    partNSpd(0.0f),
    partNAcc(0.0f),
    partVel1(0.0f),
    partVel2(0.0f),
    partVel3(0.0f),
    partAcc1(0.0f),
    partAcc2(0.0f),
    partAcc3(0.0f),
    partKey1(1.0f),
    partKey2(1.0f),
    partKey1Time(0.0f),
    partKey2Time(1.0f),
    key1(0xFF, 0xFF, 0xFF, 0x00),
    key2(0xFF, 0xFF, 0xFF, 0x00),
    key3(0xFF, 0xFF, 0xFF, 0x00),
    key1A(1.0f),
    key2A(1.0f),
    key3A(1.0f),
    key1Time(0.0f),
    key2Time(0.5f),
    key3Time(1.0f)
    {
    memset(&unused1, 0xCD, 3);
    }

EFSHRecord::EFSHDATA::~EFSHDATA()
    {
    //
    }

bool EFSHRecord::EFSHDATA::operator ==(const EFSHDATA &other) const
    {
    return (flags == other.flags &&
            memSBlend == other.memSBlend &&
            memBlendOp == other.memBlendOp &&
            memZFunc == other.memZFunc &&
            fill == other.fill &&
            AlmostEqual(fillAIn,other.fillAIn,2) &&
            AlmostEqual(fillAFull,other.fillAFull,2) &&
            AlmostEqual(fillAOut,other.fillAOut,2) &&
            AlmostEqual(fillAPRatio,other.fillAPRatio,2) &&
            AlmostEqual(fillAAmp,other.fillAAmp,2) &&
            AlmostEqual(fillAFreq,other.fillAFreq,2) &&
            AlmostEqual(fillAnimSpdU,other.fillAnimSpdU,2) &&
            AlmostEqual(fillAnimSpdV,other.fillAnimSpdV,2) &&
            AlmostEqual(edgeOff,other.edgeOff,2) &&
            edge == other.edge &&
            AlmostEqual(edgeAIn,other.edgeAIn,2) &&
            AlmostEqual(edgeAFull,other.edgeAFull,2) &&
            AlmostEqual(edgeAOut,other.edgeAOut,2) &&
            AlmostEqual(edgeAPRatio,other.edgeAPRatio,2) &&
            AlmostEqual(edgeAAmp,other.edgeAAmp,2) &&
            AlmostEqual(edgeAFreq,other.edgeAFreq,2) &&
            AlmostEqual(fillAFRatio,other.fillAFRatio,2) &&
            AlmostEqual(edgeAFRatio,other.edgeAFRatio,2) &&
            memDBlend == other.memDBlend &&
            partSBlend == other.partSBlend &&
            partBlendOp == other.partBlendOp &&
            partZFunc == other.partZFunc &&
            partDBlend == other.partDBlend &&
            AlmostEqual(partBUp,other.partBUp,2) &&
            AlmostEqual(partBFull,other.partBFull,2) &&
            AlmostEqual(partBDown,other.partBDown,2) &&
            AlmostEqual(partBFRatio,other.partBFRatio,2) &&
            AlmostEqual(partBPRatio,other.partBPRatio,2) &&
            AlmostEqual(partLTime,other.partLTime,2) &&
            AlmostEqual(partLDelta,other.partLDelta,2) &&
            AlmostEqual(partNSpd,other.partNSpd,2) &&
            AlmostEqual(partNAcc,other.partNAcc,2) &&
            AlmostEqual(partVel1,other.partVel1,2) &&
            AlmostEqual(partVel2,other.partVel2,2) &&
            AlmostEqual(partVel3,other.partVel3,2) &&
            AlmostEqual(partAcc1,other.partAcc1,2) &&
            AlmostEqual(partAcc2,other.partAcc2,2) &&
            AlmostEqual(partAcc3,other.partAcc3,2) &&
            AlmostEqual(partKey1,other.partKey1,2) &&
            AlmostEqual(partKey2,other.partKey2,2) &&
            AlmostEqual(partKey1Time,other.partKey1Time,2) &&
            AlmostEqual(partKey2Time,other.partKey2Time,2) &&
            key1 == other.key1 &&
            key2 == other.key2 &&
            key3 == other.key3 &&
            AlmostEqual(key1A,other.key1A,2) &&
            AlmostEqual(key2A,other.key2A,2) &&
            AlmostEqual(key3A,other.key3A,2) &&
            AlmostEqual(key1Time,other.key1Time,2) &&
            AlmostEqual(key2Time,other.key2Time,2) &&
            AlmostEqual(key3Time,other.key3Time,2));
    }

bool EFSHRecord::EFSHDATA::operator !=(const EFSHDATA &other) const
    {
    return !(*this == other);
    }

EFSHRecord::EFSHRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }
EFSHRecord::EFSHRecord(EFSHRecord *srcRecord):
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
    ICON = srcRecord->ICON;
    ICO2 = srcRecord->ICO2;
    DATA = srcRecord->DATA;
    return;
    }

EFSHRecord::~EFSHRecord()
    {
    //
    }

bool EFSHRecord::IsNoMemShader()
    {
    return (DATA.value.flags & fIsNoMemShader) != 0;
    }

void EFSHRecord::IsNoMemShader(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsNoMemShader) : (DATA.value.flags & ~fIsNoMemShader);
    }

bool EFSHRecord::IsNoMembraneShader()
    {
    return (DATA.value.flags & fIsNoMemShader) != 0;
    }

void EFSHRecord::IsNoMembraneShader(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsNoMemShader) : (DATA.value.flags & ~fIsNoMemShader);
    }

bool EFSHRecord::IsNoPartShader()
    {
    return (DATA.value.flags & fIsNoPartShader) != 0;
    }

void EFSHRecord::IsNoPartShader(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsNoPartShader) : (DATA.value.flags & ~fIsNoPartShader);
    }

bool EFSHRecord::IsNoParticleShader()
    {
    return (DATA.value.flags & fIsNoPartShader) != 0;
    }

void EFSHRecord::IsNoParticleShader(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsNoPartShader) : (DATA.value.flags & ~fIsNoPartShader);
    }

bool EFSHRecord::IsEdgeInverse()
    {
    return (DATA.value.flags & fIsEdgeInverse) != 0;
    }

void EFSHRecord::IsEdgeInverse(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsEdgeInverse) : (DATA.value.flags & ~fIsEdgeInverse);
    }

bool EFSHRecord::IsEdgeEffectInverse()
    {
    return (DATA.value.flags & fIsEdgeInverse) != 0;
    }

void EFSHRecord::IsEdgeEffectInverse(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsEdgeInverse) : (DATA.value.flags & ~fIsEdgeInverse);
    }

bool EFSHRecord::IsMemSkinOnly()
    {
    return (DATA.value.flags & fIsMemSkinOnly) != 0;
    }

void EFSHRecord::IsMemSkinOnly(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsMemSkinOnly) : (DATA.value.flags & ~fIsMemSkinOnly);
    }

bool EFSHRecord::IsMembraneShaderSkinOnly()
    {
    return (DATA.value.flags & fIsMemSkinOnly) != 0;
    }

void EFSHRecord::IsMembraneShaderSkinOnly(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsMemSkinOnly) : (DATA.value.flags & ~fIsMemSkinOnly);
    }

bool EFSHRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void EFSHRecord::SetFlagMask(UINT8 Mask)
    {
    DATA.value.flags = Mask;
    }

UINT32 EFSHRecord::GetSize(bool forceCalc)
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

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ICO2.IsLoaded())
        {
        cSize = ICO2.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    return TotSize;
    }

SINT32 EFSHRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case '2OCI':
                ICO2.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  EFSH: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

UINT32 EFSHRecord::GetType()
    {
    return 'HSFE';
    }

STRING EFSHRecord::GetStrType()
    {
    return "EFSH";
    }

SINT32 EFSHRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    ICON.Unload();
    ICO2.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 EFSHRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(ICO2.IsLoaded())
        SaveHandler.writeSubRecord('2OCI', ICO2.value, ICO2.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool EFSHRecord::operator ==(const EFSHRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            ICON.equalsi(other.ICON) &&
            ICO2.equalsi(other.ICO2) &&
            DATA == other.DATA);
    }

bool EFSHRecord::operator !=(const EFSHRecord &other) const
    {
    return !(*this == other);
    }