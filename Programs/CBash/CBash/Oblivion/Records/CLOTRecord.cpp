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
#include "CLOTRecord.h"
#include <vector>

CLOTRecord::CLOTRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

CLOTRecord::CLOTRecord(CLOTRecord *srcRecord):
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
    SCRI = srcRecord->SCRI;
    ENAM = srcRecord->ENAM;
    ANAM = srcRecord->ANAM;
    BMDT = srcRecord->BMDT;
    if(srcRecord->MODL.IsLoaded())
        {
        MODL.Load();
        MODL->MODB = srcRecord->MODL->MODB;
        MODL->MODL = srcRecord->MODL->MODL;
        MODL->MODT = srcRecord->MODL->MODT;
        }
    if(srcRecord->MOD2.IsLoaded())
        {
        MOD2.Load();
        MOD2->MODB = srcRecord->MOD2->MODB;
        MOD2->MODL = srcRecord->MOD2->MODL;
        MOD2->MODT = srcRecord->MOD2->MODT;
        }
    ICON = srcRecord->ICON;
    if(srcRecord->MOD3.IsLoaded())
        {
        MOD3.Load();
        MOD3->MODB = srcRecord->MOD3->MODB;
        MOD3->MODL = srcRecord->MOD3->MODL;
        MOD3->MODT = srcRecord->MOD3->MODT;
        }
    if(srcRecord->MOD4.IsLoaded())
        {
        MOD4.Load();
        MOD4->MODB = srcRecord->MOD4->MODB;
        MOD4->MODL = srcRecord->MOD4->MODL;
        MOD4->MODT = srcRecord->MOD4->MODT;
        }
    ICO2 = srcRecord->ICO2;
    DATA = srcRecord->DATA;
    return;
    }

CLOTRecord::~CLOTRecord()
    {
    //
    }

bool CLOTRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);
    if(ENAM.IsLoaded())
        op.Accept(ENAM->fid);

    return op.Stop();
    }

bool CLOTRecord::IsHead()
    {
    return (BMDT.value.flags & fIsHead) != 0;
    }

void CLOTRecord::IsHead(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsHead) : (BMDT.value.flags & ~fIsHead);
    }

bool CLOTRecord::IsHair()
    {
    return (BMDT.value.flags & fIsHair) != 0;
    }

void CLOTRecord::IsHair(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsHair) : (BMDT.value.flags & ~fIsHair);
    }

bool CLOTRecord::IsUpperBody()
    {
    return (BMDT.value.flags & fIsUpperBody) != 0;
    }

void CLOTRecord::IsUpperBody(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsUpperBody) : (BMDT.value.flags & ~fIsUpperBody);
    }

bool CLOTRecord::IsLowerBody()
    {
    return (BMDT.value.flags & fIsLowerBody) != 0;
    }

void CLOTRecord::IsLowerBody(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsLowerBody) : (BMDT.value.flags & ~fIsLowerBody);
    }

bool CLOTRecord::IsHand()
    {
    return (BMDT.value.flags & fIsHand) != 0;
    }

void CLOTRecord::IsHand(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsHand) : (BMDT.value.flags & ~fIsHand);
    }

bool CLOTRecord::IsFoot()
    {
    return (BMDT.value.flags & fIsFoot) != 0;
    }

void CLOTRecord::IsFoot(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsFoot) : (BMDT.value.flags & ~fIsFoot);
    }

bool CLOTRecord::IsRightRing()
    {
    return (BMDT.value.flags & fIsRightRing) != 0;
    }

void CLOTRecord::IsRightRing(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsRightRing) : (BMDT.value.flags & ~fIsRightRing);
    }

bool CLOTRecord::IsLeftRing()
    {
    return (BMDT.value.flags & fIsLeftRing) != 0;
    }

void CLOTRecord::IsLeftRing(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsLeftRing) : (BMDT.value.flags & ~fIsLeftRing);
    }

bool CLOTRecord::IsAmulet()
    {
    return (BMDT.value.flags & fIsAmulet) != 0;
    }

void CLOTRecord::IsAmulet(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsAmulet) : (BMDT.value.flags & ~fIsAmulet);
    }

bool CLOTRecord::IsWeapon()
    {
    return (BMDT.value.flags & fIsWeapon) != 0;
    }

void CLOTRecord::IsWeapon(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsWeapon) : (BMDT.value.flags & ~fIsWeapon);
    }

bool CLOTRecord::IsBackWeapon()
    {
    return (BMDT.value.flags & fIsBackWeapon) != 0;
    }

void CLOTRecord::IsBackWeapon(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsBackWeapon) : (BMDT.value.flags & ~fIsBackWeapon);
    }

bool CLOTRecord::IsSideWeapon()
    {
    return (BMDT.value.flags & fIsSideWeapon) != 0;
    }

void CLOTRecord::IsSideWeapon(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsSideWeapon) : (BMDT.value.flags & ~fIsSideWeapon);
    }

bool CLOTRecord::IsQuiver()
    {
    return (BMDT.value.flags & fIsQuiver) != 0;
    }

void CLOTRecord::IsQuiver(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsQuiver) : (BMDT.value.flags & ~fIsQuiver);
    }

bool CLOTRecord::IsShield()
    {
    return (BMDT.value.flags & fIsShield) != 0;
    }

void CLOTRecord::IsShield(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsShield) : (BMDT.value.flags & ~fIsShield);
    }

bool CLOTRecord::IsTorch()
    {
    return (BMDT.value.flags & fIsTorch) != 0;
    }

void CLOTRecord::IsTorch(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsTorch) : (BMDT.value.flags & ~fIsTorch);
    }

bool CLOTRecord::IsTail()
    {
    return (BMDT.value.flags & fIsTail) != 0;
    }

void CLOTRecord::IsTail(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsTail) : (BMDT.value.flags & ~fIsTail);
    }

bool CLOTRecord::IsHideRings()
    {
    return (BMDT.value.flags & fIsHideRings) != 0;
    }

void CLOTRecord::IsHideRings(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsHideRings) : (BMDT.value.flags & ~fIsHideRings);
    }

bool CLOTRecord::IsHideAmulets()
    {
    return (BMDT.value.flags & fIsHideAmulets) != 0;
    }

void CLOTRecord::IsHideAmulets(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsHideAmulets) : (BMDT.value.flags & ~fIsHideAmulets);
    }

bool CLOTRecord::IsNonPlayable()
    {
    return (BMDT.value.flags & fIsNonPlayable) != 0;
    }

void CLOTRecord::IsNonPlayable(bool value)
    {
    BMDT.value.flags = value ? (BMDT.value.flags | fIsNonPlayable) : (BMDT.value.flags & ~fIsNonPlayable);
    }

bool CLOTRecord::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((BMDT.value.flags & Mask) == Mask) : ((BMDT.value.flags & Mask) != 0);
    }

void CLOTRecord::SetFlagMask(UINT32 Mask)
    {
    BMDT.value.flags = Mask;
    }

UINT32 CLOTRecord::GetSize(bool forceCalc)
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

    if(SCRI.IsLoaded())
        TotSize += SCRI.GetSize() + 6;

    if(ENAM.IsLoaded())
        TotSize += ENAM.GetSize() + 6;

    if(ANAM.IsLoaded())
        TotSize += ANAM.GetSize() + 6;

    if(BMDT.IsLoaded())
        TotSize += BMDT.GetSize() + 6;

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

    if(MOD2.IsLoaded() && MOD2->MODL.IsLoaded())
        {
        cSize = MOD2->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD2->MODB.IsLoaded())
            TotSize += MOD2->MODB.GetSize() + 6;

        if(MOD2->MODT.IsLoaded())
            {
            cSize = MOD2->MODT.GetSize();
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

    if(MOD3.IsLoaded() && MOD3->MODL.IsLoaded())
        {
        cSize = MOD3->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD3->MODB.IsLoaded())
            TotSize += MOD3->MODB.GetSize() + 6;

        if(MOD3->MODT.IsLoaded())
            {
            cSize = MOD3->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD4.IsLoaded() && MOD4->MODL.IsLoaded())
        {
        cSize = MOD4->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD4->MODB.IsLoaded())
            TotSize += MOD4->MODB.GetSize() + 6;

        if(MOD4->MODT.IsLoaded())
            {
            cSize = MOD4->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
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

UINT32 CLOTRecord::GetType()
    {
    return 'TOLC';
    }

STRING CLOTRecord::GetStrType()
    {
    return "CLOT";
    }

SINT32 CLOTRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'IRCS':
                SCRI.Read(buffer, subSize, curPos);
                break;
            case 'MANE':
                ENAM.Read(buffer, subSize, curPos);
                break;
            case 'MANA':
                ANAM.Read(buffer, subSize, curPos);
                break;
            case 'TDMB':
                BMDT.Read(buffer, subSize, curPos);
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

            case '2DOM':
                MOD2.Load();
                MOD2->MODL.Read(buffer, subSize, curPos);
                break;
            case 'B2OM':
                MOD2.Load();
                MOD2->MODB.Read(buffer, subSize, curPos);
                break;
            case 'T2OM':
                MOD2.Load();
                MOD2->MODT.Read(buffer, subSize, curPos);
                break;

            case '3DOM':
                MOD3.Load();
                MOD3->MODL.Read(buffer, subSize, curPos);
                break;
            case 'B3OM':
                MOD3.Load();
                MOD3->MODB.Read(buffer, subSize, curPos);
                break;
            case 'T3OM':
                MOD3.Load();
                MOD3->MODT.Read(buffer, subSize, curPos);
                break;

            case '4DOM':
                MOD4.Load();
                MOD4->MODL.Read(buffer, subSize, curPos);
                break;
            case 'B4OM':
                MOD4.Load();
                MOD4->MODB.Read(buffer, subSize, curPos);
                break;
            case 'T4OM':
                MOD4.Load();
                MOD4->MODT.Read(buffer, subSize, curPos);
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
                printf("  CLOT: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 CLOTRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    SCRI.Unload();
    ENAM.Unload();
    ANAM.Unload();
    BMDT.Unload();
    MODL.Unload();
    MOD2.Unload();
    ICON.Unload();
    MOD3.Unload();
    MOD4.Unload();
    ICO2.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 CLOTRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(SCRI.IsLoaded())
        SaveHandler.writeSubRecord('IRCS', SCRI.value, SCRI.GetSize());
    if(ENAM.IsLoaded())
        SaveHandler.writeSubRecord('MANE', ENAM.value, ENAM.GetSize());
    if(ANAM.IsLoaded())
        SaveHandler.writeSubRecord('MANA', ANAM.value, ANAM.GetSize());
    if(BMDT.IsLoaded())
        SaveHandler.writeSubRecord('TDMB', &BMDT.value, BMDT.GetSize());
    if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MODL->MODL.value, MODL->MODL.GetSize());
        if(MODL->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MODL->MODB.value, MODL->MODB.GetSize());
        if(MODL->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MODL->MODT.value, MODL->MODT.GetSize());
        }
    if(MOD2.IsLoaded() && MOD2->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('2DOM', MOD2->MODL.value, MOD2->MODL.GetSize());
        if(MOD2->MODB.IsLoaded())
            SaveHandler.writeSubRecord('B2OM', &MOD2->MODB.value, MOD2->MODB.GetSize());
        if(MOD2->MODT.IsLoaded())
            SaveHandler.writeSubRecord('T2OM', MOD2->MODT.value, MOD2->MODT.GetSize());
        }
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());

    if(MOD3.IsLoaded() && MOD3->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('3DOM', MOD3->MODL.value, MOD3->MODL.GetSize());
        if(MOD3->MODB.IsLoaded())
            SaveHandler.writeSubRecord('B3OM', &MOD3->MODB.value, MOD3->MODB.GetSize());
        if(MOD3->MODT.IsLoaded())
            SaveHandler.writeSubRecord('T3OM', MOD3->MODT.value, MOD3->MODT.GetSize());
        }
    if(MOD4.IsLoaded() && MOD4->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('4DOM', MOD4->MODL.value, MOD4->MODL.GetSize());
        if(MOD4->MODB.IsLoaded())
            SaveHandler.writeSubRecord('B4OM', &MOD4->MODB.value, MOD4->MODB.GetSize());
        if(MOD4->MODT.IsLoaded())
            SaveHandler.writeSubRecord('T4OM', MOD4->MODT.value, MOD4->MODT.GetSize());
        }
    if(ICO2.IsLoaded())
        SaveHandler.writeSubRecord('2OCI', ICO2.value, ICO2.GetSize());

    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool CLOTRecord::operator ==(const CLOTRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            SCRI == other.SCRI &&
            ENAM == other.ENAM &&
            ANAM == other.ANAM &&
            BMDT == other.BMDT &&
            MODL == other.MODL &&
            MOD2 == other.MOD2 &&
            ICON.equalsi(other.ICON) &&
            MOD3 == other.MOD3 &&
            MOD4 == other.MOD4 &&
            ICO2.equalsi(other.ICO2) &&
            DATA == other.DATA);
    }

bool CLOTRecord::operator !=(const CLOTRecord &other) const
    {
    return !(*this == other);
    }