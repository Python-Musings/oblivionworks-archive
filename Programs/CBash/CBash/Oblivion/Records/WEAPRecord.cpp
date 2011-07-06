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
#include "WEAPRecord.h"

WEAPRecord::WEAPDATA::WEAPDATA():
    weaponType(0),
    speed(0.0f),
    reach(0.0f),
    flags(0),
    value(0),
    health(0),
    weight(0.0f),
    damage(0)
    {
    //
    }

WEAPRecord::WEAPDATA::~WEAPDATA()
    {
    //
    }

bool WEAPRecord::WEAPDATA::operator ==(const WEAPDATA &other) const
    {
    return (weaponType == other.weaponType &&
            AlmostEqual(speed,other.speed,2) &&
            AlmostEqual(reach,other.reach,2) &&
            flags == other.flags &&
            value == other.value &&
            health == other.health &&
            AlmostEqual(weight,other.weight,2) &&
            damage == other.damage);
    }

bool WEAPRecord::WEAPDATA::operator !=(const WEAPDATA &other) const
    {
    return !(*this == other);
    }

WEAPRecord::WEAPRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

WEAPRecord::WEAPRecord(WEAPRecord *srcRecord):Record()
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
    SCRI = srcRecord->SCRI;
    ENAM = srcRecord->ENAM;
    ANAM = srcRecord->ANAM;
    DATA = srcRecord->DATA;
    }

WEAPRecord::~WEAPRecord()
    {
    //
    }

bool WEAPRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);
    if(ENAM.IsLoaded())
        op.Accept(ENAM->fid);

    return op.Stop();
    }

bool WEAPRecord::IsNotNormalWeapon()
    {
    return (DATA.value.flags & fIsNotNormalWeapon) != 0;
    }

void WEAPRecord::IsNotNormalWeapon(bool value)
    {
    if(value)
        DATA.value.flags |= fIsNotNormalWeapon;
    else
        DATA.value.flags &= ~fIsNotNormalWeapon;
    }

bool WEAPRecord::IsNotNormal()
    {
    return (DATA.value.flags & fIsNotNormalWeapon) != 0;
    }

void WEAPRecord::IsNotNormal(bool value)
    {
    if(value)
        DATA.value.flags |= fIsNotNormalWeapon;
    else
        DATA.value.flags &= ~fIsNotNormalWeapon;
    }

bool WEAPRecord::IsNormalWeapon()
    {
    return (DATA.value.flags & fIsNotNormalWeapon) == 0;
    }

void WEAPRecord::IsNormalWeapon(bool value)
    {
    if(value)
        DATA.value.flags &= ~fIsNotNormalWeapon;
    else
        DATA.value.flags |= fIsNotNormalWeapon;
    }

bool WEAPRecord::IsNormal()
    {
    return (DATA.value.flags & fIsNotNormalWeapon) == 0;
    }

void WEAPRecord::IsNormal(bool value)
    {
    if(value)
        DATA.value.flags &= ~fIsNotNormalWeapon;
    else
        DATA.value.flags |= fIsNotNormalWeapon;
    }

bool WEAPRecord::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void WEAPRecord::SetFlagMask(UINT32 Mask)
    {
    DATA.value.flags = Mask;
    }

bool WEAPRecord::IsBlade1Hand()
    {
    return (DATA.value.weaponType == eBlade1Hand);
    }

void WEAPRecord::IsBlade1Hand(bool value)
    {
    if(value)
        DATA.value.weaponType = eBlade1Hand;
    else if(IsBlade1Hand())
        DATA.value.weaponType = eBlade2Hand;
    }

bool WEAPRecord::IsBlade2Hand()
    {
    return (DATA.value.weaponType == eBlade2Hand);
    }

void WEAPRecord::IsBlade2Hand(bool value)
    {
    if(value)
        DATA.value.weaponType = eBlade2Hand;
    else if(IsBlade2Hand())
        DATA.value.weaponType = eBlade1Hand;
    }

bool WEAPRecord::IsBlunt1Hand()
    {
    return (DATA.value.weaponType == eBlunt1Hand);
    }

void WEAPRecord::IsBlunt1Hand(bool value)
    {
    if(value)
        DATA.value.weaponType = eBlunt1Hand;
    else if(IsBlunt1Hand())
        DATA.value.weaponType = eBlade1Hand;
    }

bool WEAPRecord::IsBlunt2Hand()
    {
    return (DATA.value.weaponType == eBlunt2Hand);
    }

void WEAPRecord::IsBlunt2Hand(bool value)
    {
    if(value)
        DATA.value.weaponType = eBlunt2Hand;
    else if(IsBlunt2Hand())
        DATA.value.weaponType = eBlade1Hand;
    }

bool WEAPRecord::IsStaff()
    {
    return (DATA.value.weaponType == eStaff);
    }

void WEAPRecord::IsStaff(bool value)
    {
    if(value)
        DATA.value.weaponType = eStaff;
    else if(IsStaff())
        DATA.value.weaponType = eBlade1Hand;
    }

bool WEAPRecord::IsBow()
    {
    return (DATA.value.weaponType == eBow);
    }

void WEAPRecord::IsBow(bool value)
    {
    if(value)
        DATA.value.weaponType = eBow;
    else if(IsBow())
        DATA.value.weaponType = eBlade1Hand;
    }

bool WEAPRecord::IsType(UINT32 Type)
    {
    return (DATA.value.weaponType == Type);
    }

void WEAPRecord::SetType(UINT32 Type)
    {
    DATA.value.weaponType = Type;
    }

UINT32 WEAPRecord::GetSize(bool forceCalc)
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

    if(SCRI.IsLoaded())
        TotSize += SCRI.GetSize() + 6;

    if(ENAM.IsLoaded())
        TotSize += ENAM.GetSize() + 6;

    if(ANAM.IsLoaded())
        TotSize += ANAM.GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    return TotSize;
    }

UINT32 WEAPRecord::GetType()
    {
    return 'PAEW';
    }

STRING WEAPRecord::GetStrType()
    {
    return "WEAP";
    }

SINT32 WEAPRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'IRCS':
                SCRI.Read(buffer, subSize, curPos);
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
                printf("  WEAP: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 WEAPRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    MODL.Unload();
    ICON.Unload();
    SCRI.Unload();
    ENAM.Unload();
    ANAM.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 WEAPRecord::WriteRecord(_FileHandler &SaveHandler)
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
    if(SCRI.IsLoaded())
        SaveHandler.writeSubRecord('IRCS', SCRI.value, SCRI.GetSize());
    if(ENAM.IsLoaded())
        SaveHandler.writeSubRecord('MANE', ENAM.value, ENAM.GetSize());
    if(ANAM.IsLoaded())
        SaveHandler.writeSubRecord('MANA', ANAM.value, ANAM.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool WEAPRecord::operator ==(const WEAPRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            MODL == other.MODL &&
            ICON.equalsi(other.ICON) &&
            SCRI == other.SCRI &&
            ENAM == other.ENAM &&
            ANAM == other.ANAM &&
            DATA == other.DATA);
    }

bool WEAPRecord::operator !=(const WEAPRecord &other) const
    {
    return !(*this == other);
    }