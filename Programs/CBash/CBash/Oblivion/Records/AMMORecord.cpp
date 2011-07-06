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
#include "AMMORecord.h"

AMMORecord::AMMODATA::AMMODATA():
    speed(0.0f),
    flags(0),
    value(0),
    weight(0.0f),
    damage(0)
    {
    memset(&unused1, 0xCD, 3);
    }

AMMORecord::AMMODATA::~AMMODATA()
    {
    //
    }

bool AMMORecord::AMMODATA::operator ==(const AMMODATA &other) const
    {
    return (AlmostEqual(speed,other.speed,2) &&
            flags == other.flags &&
            value == other.value &&
            AlmostEqual(weight,other.weight,2) &&
            damage == other.damage);
    }

bool AMMORecord::AMMODATA::operator !=(const AMMODATA &other) const
    {
    return !(*this == other);
    }

AMMORecord::AMMORecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

AMMORecord::AMMORecord(AMMORecord *srcRecord):
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
    FULL = srcRecord->FULL;
    if(srcRecord->MODL.IsLoaded())
        {
        MODL.Load();
        MODL->MODB = srcRecord->MODL->MODB;
        MODL->MODL = srcRecord->MODL->MODL;
        MODL->MODT = srcRecord->MODL->MODT;
        }
    ICON = srcRecord->ICON;
    ENAM = srcRecord->ENAM;
    ANAM = srcRecord->ANAM;
    DATA = srcRecord->DATA;
    return;
    }

AMMORecord::~AMMORecord()
    {
    //
    }

bool AMMORecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(ENAM.IsLoaded())
        op.Accept(ENAM->fid);

    return op.Stop();
    }

bool AMMORecord::IsNotNormalWeapon()
    {
    return (DATA.value.flags & fIsNotNormalWeapon) != 0;
    }

void AMMORecord::IsNotNormalWeapon(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsNotNormalWeapon) : (DATA.value.flags & ~fIsNotNormalWeapon);
    }

bool AMMORecord::IsNotNormal()
    {
    return (DATA.value.flags & fIsNotNormalWeapon) != 0;
    }

void AMMORecord::IsNotNormal(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsNotNormalWeapon) : (DATA.value.flags & ~fIsNotNormalWeapon);
    }

bool AMMORecord::IsNormalWeapon()
    {
    return (DATA.value.flags & fIsNotNormalWeapon) == 0;
    }

void AMMORecord::IsNormalWeapon(bool value)
    {
    if(value)
        DATA.value.flags &= ~fIsNotNormalWeapon;
    else
        DATA.value.flags |= fIsNotNormalWeapon;
    }

bool AMMORecord::IsNormal()
    {
    return (DATA.value.flags & fIsNotNormalWeapon) == 0;
    }

void AMMORecord::IsNormal(bool value)
    {
    if(value)
        DATA.value.flags &= ~fIsNotNormalWeapon;
    else
        DATA.value.flags |= fIsNotNormalWeapon;
    }

bool AMMORecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void AMMORecord::SetFlagMask(UINT8 Mask)
    {
    DATA.value.flags = Mask;
    }

UINT32 AMMORecord::GetSize(bool forceCalc)
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

    if(FULL.IsLoaded())
        {
        cSize = FULL.GetSize();
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

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ENAM.IsLoaded())
        TotSize += ENAM.GetSize() + 6;

    if(ANAM.IsLoaded())
        TotSize += ANAM.GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    return TotSize;
    }

UINT32 AMMORecord::GetType()
    {
    return 'OMMA';
    }

STRING AMMORecord::GetStrType()
    {
    return "AMMO";
    }

SINT32 AMMORecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'LLUF':
                FULL.Read(buffer, subSize, curPos);
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
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case 'MANE':
                ENAM.Read(buffer, subSize, curPos);
                break;
            case 'MANA':
                ANAM.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  AMMO: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 AMMORecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    MODL.Unload();
    ICON.Unload();
    ENAM.Unload();
    ANAM.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 AMMORecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MODL->MODL.value, MODL->MODL.GetSize());
        if(MODL->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MODL->MODB.value, MODL->MODB.GetSize());
        if(MODL->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MODL->MODT.value, MODL->MODT.GetSize());
        }
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(ENAM.IsLoaded())
        SaveHandler.writeSubRecord('MANE', ENAM.value, ENAM.GetSize());
    if(ANAM.IsLoaded())
        SaveHandler.writeSubRecord('MANA', ANAM.value, ANAM.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool AMMORecord::operator ==(const AMMORecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            MODL == other.MODL &&
            ICON.equals(other.ICON) &&
            ENAM == other.ENAM &&
            ANAM == other.ANAM &&
            DATA == other.DATA);
    }

bool AMMORecord::operator !=(const AMMORecord &other) const
    {
    return !(*this == other);
    }