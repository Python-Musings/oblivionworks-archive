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
#include "IDLERecord.h"
#include <vector>

IDLERecord::IDLEANAM::IDLEANAM():
    group(0)
    {
    //
    }

IDLERecord::IDLEANAM::~IDLEANAM()
    {
    //
    }

bool IDLERecord::IDLEANAM::operator ==(const IDLEANAM &other) const
    {
    return (group == other.group);
    }

bool IDLERecord::IDLEANAM::operator !=(const IDLEANAM &other) const
    {
    return !(*this == other);
    }

IDLERecord::IDLEDATA::IDLEDATA():
    parent(0),
    prevId(0)
    {
    //
    }

IDLERecord::IDLEDATA::~IDLEDATA()
    {
    //
    }

bool IDLERecord::IDLEDATA::operator ==(const IDLEDATA &other) const
    {
    return (parent == other.parent &&
            prevId == other.prevId);
    }
bool IDLERecord::IDLEDATA::operator !=(const IDLEDATA &other) const
    {
    return !(*this == other);
    }

IDLERecord::IDLERecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

IDLERecord::IDLERecord(IDLERecord *srcRecord):
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
    if(srcRecord->MODL.IsLoaded())
        {
        MODL.Load();
        MODL->MODB = srcRecord->MODL->MODB;
        MODL->MODL = srcRecord->MODL->MODL;
        MODL->MODT = srcRecord->MODL->MODT;
        }
    CTDA.clear();
    CTDA.resize(srcRecord->CTDA.size());
    for(UINT32 x = 0; x < srcRecord->CTDA.size(); x++)
        {
        CTDA[x] = new ReqSubRecord<GENCTDA>;
        *CTDA[x] = *srcRecord->CTDA[x];
        }
    ANAM = srcRecord->ANAM;
    DATA = srcRecord->DATA;
    return;
    }

IDLERecord::~IDLERecord()
    {
    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    }

bool IDLERecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    FunctionArguments CTDAFunction;
    Function_Arguments_Iterator curCTDAFunction;
    for(UINT32 x = 0; x < CTDA.size(); x++)
        {
        //if(CTDA[x]->value.ifunc == 214)
        //    printf("%08X uses HasMagicEffect\n", formID);
        curCTDAFunction = Function_Arguments.find(CTDA[x]->value.ifunc);
        if(curCTDAFunction != Function_Arguments.end())
            {
            CTDAFunction = curCTDAFunction->second;
            if(CTDAFunction.first == eFORMID)
                op.Accept(CTDA[x]->value.param1);
            if(CTDAFunction.second == eFORMID)
                op.Accept(CTDA[x]->value.param2);
            }
        else
            printf("Warning: IDLERecord %08X uses an unknown function (%d)!\n", formID, CTDA[x]->value.ifunc);
        }

    if(DATA.IsLoaded())
        {
        op.Accept(DATA.value.parent);
        op.Accept(DATA.value.prevId);
        }

    return op.Stop();
    }

bool IDLERecord::IsLowerBody()
    {
    return ((ANAM.value.group & 0x0F) == eLowerBody);
    }

void IDLERecord::IsLowerBody(bool value)
    {
    if(value)
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLowerBody;
        }
    else if(IsLowerBody())
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLeftArm;
        }
    }

bool IDLERecord::IsLeftArm()
    {
    return ((ANAM.value.group & 0x0F) == eLeftArm);
    }

void IDLERecord::IsLeftArm(bool value)
    {
    if(value)
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLeftArm;
        }
    else if(IsLeftArm())
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLowerBody;
        }
    }

bool IDLERecord::IsLeftHand()
    {
    return ((ANAM.value.group & 0x0F) == eLeftHand);
    }

void IDLERecord::IsLeftHand(bool value)
    {
    if(value)
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLeftHand;
        }
    else if(IsLeftHand())
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLowerBody;
        }
    }

bool IDLERecord::IsRightArm()
    {
    return ((ANAM.value.group & 0x0F) == eRightArm);
    }

void IDLERecord::IsRightArm(bool value)
    {
    if(value)
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eRightArm;
        }
    else if(IsRightArm())
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLowerBody;
        }
    }

bool IDLERecord::IsSpecialIdle()
    {
    return ((ANAM.value.group & 0x0F) == eSpecialIdle);
    }

void IDLERecord::IsSpecialIdle(bool value)
    {
    if(value)
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eSpecialIdle;
        }
    else if(IsLeftArm())
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLowerBody;
        }
    }

bool IDLERecord::IsWholeBody()
    {
    return ((ANAM.value.group & 0x0F) == eWholeBody);
    }

void IDLERecord::IsWholeBody(bool value)
    {
    if(value)
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eWholeBody;
        }
    else if(IsWholeBody())
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLowerBody;
        }
    }

bool IDLERecord::IsUpperBody()
    {
    return ((ANAM.value.group & 0x0F) == eUpperBody);
    }

void IDLERecord::IsUpperBody(bool value)
    {
    if(value)
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eUpperBody;
        }
    else if(IsUpperBody())
        {
        ANAM.value.group &= 0xF0;
        ANAM.value.group |= eLowerBody;
        }
    }

bool IDLERecord::IsType(UINT8 Type)
    {
    return ((ANAM.value.group & 0x0F) == (Type & 0x0F));
    }

void IDLERecord::SetType(UINT8 Type)
    {
    Type &= 0x0F;
    ANAM.value.group &= 0xF0;
    ANAM.value.group |= Type;
    }

bool IDLERecord::IsNotReturnFile()
    {
    return (ANAM.value.group & fIsNotReturnFile) != 0;
    }

void IDLERecord::IsNotReturnFile(bool value)
    {
    ANAM.value.group = value ? (ANAM.value.group | fIsNotReturnFile) : (ANAM.value.group & ~fIsNotReturnFile);
    }

bool IDLERecord::IsReturnFile()
    {
    return !(IsNotReturnFile());
    }

void IDLERecord::IsReturnFile(bool value)
    {
    if(value)
        IsNotReturnFile(false);
    else
        IsNotReturnFile(true);
    }

bool IDLERecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? (((ANAM.value.group & 0xF0) & (Mask & 0xF0)) == Mask) : (((ANAM.value.group & 0xF0) & (Mask & 0xF0)) != 0);
    }

void IDLERecord::SetFlagMask(UINT8 Mask)
    {
    Mask &= 0xF0;
    ANAM.value.group &= 0x0F;
    ANAM.value.group |= Mask;
    }

UINT32 IDLERecord::GetSize(bool forceCalc)
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

    if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
        {
        cSize = MODL->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MODL->MODB.IsLoaded())
            TotSize += MODL->MODB.GetSize() + 6;

        if(MODL->MODT.IsLoaded())
            {
            cSize = MODL->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    for(UINT32 p = 0; p < CTDA.size(); p++)
        if(CTDA[p]->IsLoaded())
            TotSize += CTDA[p]->GetSize() + 6;

    if(ANAM.IsLoaded())
        TotSize += ANAM.GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    return TotSize;
    }

UINT32 IDLERecord::GetType()
    {
    return 'ELDI';
    }

STRING IDLERecord::GetStrType()
    {
    return "IDLE";
    }

SINT32 IDLERecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    ReqSubRecord<GENCTDA> *newCTDA = NULL;
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
            case 'LDOM':
                MODL.Load();
                MODL->MODL.Read(buffer, subSize, curPos);
                break;
            case 'BDOM':
                MODL.Load();
                MODL->MODB.Read(buffer, subSize, curPos);
                break;
            case 'TDOM':
                MODL.Load();
                MODL->MODT.Read(buffer, subSize, curPos);
                break;
            case 'TDTC':
            case 'ADTC':
                newCTDA = new ReqSubRecord<GENCTDA>;
                newCTDA->Read(buffer, subSize, curPos);
                CTDA.push_back(newCTDA);
                break;
            case 'MANA':
                ANAM.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  IDLE: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 IDLERecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    MODL.Unload();
    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    CTDA.clear();
    ANAM.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 IDLERecord::WriteRecord(_FileHandler &SaveHandler)
    {
    FunctionArguments CTDAFunction;
    Function_Arguments_Iterator curCTDAFunction;

    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MODL->MODL.value, MODL->MODL.GetSize());
        if(MODL->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MODL->MODB.value, MODL->MODB.GetSize());
        if(MODL->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MODL->MODT.value, MODL->MODT.GetSize());
        }
    for(UINT32 p = 0; p < CTDA.size(); p++)
        {
        curCTDAFunction = Function_Arguments.find(CTDA[p]->value.ifunc);
        if(curCTDAFunction != Function_Arguments.end())
            {
            CTDAFunction = curCTDAFunction->second;
            if(CTDAFunction.first == eNONE)
                CTDA[p]->value.param1 = 0;
            if(CTDAFunction.second == eNONE)
                CTDA[p]->value.param2 = 0;
            }
        SaveHandler.writeSubRecord('ADTC', &CTDA[p]->value, CTDA[p]->GetSize());
        }
    if(ANAM.IsLoaded())
        SaveHandler.writeSubRecord('MANA', &ANAM.value, ANAM.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool IDLERecord::operator ==(const IDLERecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        MODL == other.MODL &&
        ANAM == other.ANAM &&
        DATA == other.DATA &&
        CTDA.size() == other.CTDA.size())
        {
        //Record order matters on conditions, so equality testing is easy
        for(UINT32 x = 0; x < CTDA.size(); ++x)
            if(*CTDA[x] != *other.CTDA[x])
                return false;
        return true;
        }

    return false;
    }

bool IDLERecord::operator !=(const IDLERecord &other) const
    {
    return !(*this == other);
    }