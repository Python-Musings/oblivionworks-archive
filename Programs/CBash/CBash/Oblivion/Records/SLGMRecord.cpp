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
#include "SLGMRecord.h"

SLGMRecord::SLGMSOUL::SLGMSOUL():
    soul(0)
    {
    //
    }

SLGMRecord::SLGMSOUL::~SLGMSOUL()
    {
    //
    }

bool SLGMRecord::SLGMSOUL::operator ==(const SLGMSOUL &other) const
    {
    return (soul == other.soul);
    }

bool SLGMRecord::SLGMSOUL::operator !=(const SLGMSOUL &other) const
    {
    return !(*this == other);
    }

SLGMRecord::SLGMSLCP::SLGMSLCP():
    capacity(1)
    {
    //
    }

SLGMRecord::SLGMSLCP::~SLGMSLCP()
    {
    //
    }

bool SLGMRecord::SLGMSLCP::operator ==(const SLGMSLCP &other) const
    {
    return (capacity == other.capacity);
    }

bool SLGMRecord::SLGMSLCP::operator !=(const SLGMSLCP &other) const
    {
    return !(*this == other);
    }

SLGMRecord::SLGMRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

SLGMRecord::SLGMRecord(SLGMRecord *srcRecord):
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
    SCRI = srcRecord->SCRI;
    DATA = srcRecord->DATA;
    SOUL = srcRecord->SOUL;
    SLCP = srcRecord->SLCP;
    }

SLGMRecord::~SLGMRecord()
    {
    //
    }

bool SLGMRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);

    return op.Stop();
    }

bool SLGMRecord::IsNoSoul()
    {
    return (SOUL.value.soul == eNone);
    }

void SLGMRecord::IsNoSoul(bool value)
    {
    if(value)
        SOUL.value.soul = eNone;
    else if(IsNoSoul())
        SOUL.value.soul = ePetty;
    }

bool SLGMRecord::IsPettySoul()
    {
    return (SOUL.value.soul == ePetty);
    }

void SLGMRecord::IsPettySoul(bool value)
    {
    if(value)
        SOUL.value.soul = ePetty;
    else if(IsPettySoul())
        SOUL.value.soul = eNone;
    }

bool SLGMRecord::IsLesserSoul()
    {
    return (SOUL.value.soul == eLesser);
    }

void SLGMRecord::IsLesserSoul(bool value)
    {
    if(value)
        SOUL.value.soul = eLesser;
    else if(IsLesserSoul())
        SOUL.value.soul = eNone;
    }

bool SLGMRecord::IsCommonSoul()
    {
    return (SOUL.value.soul == eCommon);
    }

void SLGMRecord::IsCommonSoul(bool value)
    {
    if(value)
        SOUL.value.soul = eCommon;
    else if(IsCommonSoul())
        SOUL.value.soul = eNone;
    }

bool SLGMRecord::IsGreaterSoul()
    {
    return (SOUL.value.soul == eGreater);
    }

void SLGMRecord::IsGreaterSoul(bool value)
    {
    if(value)
        SOUL.value.soul = eGreater;
    else if(IsGreaterSoul())
        SOUL.value.soul = eNone;
    }

bool SLGMRecord::IsGrandSoul()
    {
    return (SOUL.value.soul == eGrand);
    }

void SLGMRecord::IsGrandSoul(bool value)
    {
    if(value)
        SOUL.value.soul = eGrand;
    else if(IsGrandSoul())
        SOUL.value.soul = eNone;
    }

bool SLGMRecord::IsSoul(UINT8 Type)
    {
    return (SOUL.value.soul == Type);
    }

void SLGMRecord::SetSoul(UINT8 Type)
    {
    SOUL.value.soul = Type;
    }


bool SLGMRecord::IsNoCapacity()
    {
    return (SLCP.value.capacity == eNone);
    }

void SLGMRecord::IsNoCapacity(bool value)
    {
    if(value)
        SLCP.value.capacity = eNone;
    else if(IsNoCapacity())
        SLCP.value.capacity = ePetty;
    }

bool SLGMRecord::IsPettyCapacity()
    {
    return (SLCP.value.capacity == ePetty);
    }

void SLGMRecord::IsPettyCapacity(bool value)
    {
    if(value)
        SLCP.value.capacity = ePetty;
    else if(IsPettyCapacity())
        SLCP.value.capacity = eNone;
    }

bool SLGMRecord::IsLesserCapacity()
    {
    return (SLCP.value.capacity == eLesser);
    }

void SLGMRecord::IsLesserCapacity(bool value)
    {
    if(value)
        SLCP.value.capacity = eLesser;
    else if(IsLesserCapacity())
        SLCP.value.capacity = eNone;
    }

bool SLGMRecord::IsCommonCapacity()
    {
    return (SLCP.value.capacity == eCommon);
    }

void SLGMRecord::IsCommonCapacity(bool value)
    {
    if(value)
        SLCP.value.capacity = eCommon;
    else if(IsCommonCapacity())
        SLCP.value.capacity = eNone;
    }

bool SLGMRecord::IsGreaterCapacity()
    {
    return (SLCP.value.capacity == eGreater);
    }

void SLGMRecord::IsGreaterCapacity(bool value)
    {
    if(value)
        SLCP.value.capacity = eGreater;
    else if(IsGreaterCapacity())
        SLCP.value.capacity = eNone;
    }

bool SLGMRecord::IsGrandCapacity()
    {
    return (SLCP.value.capacity == eGrand);
    }

void SLGMRecord::IsGrandCapacity(bool value)
    {
    if(value)
        SLCP.value.capacity = eGrand;
    else if(IsGrandCapacity())
        SLCP.value.capacity = eNone;
    }

bool SLGMRecord::IsCapacity(UINT8 Type)
    {
    return (SLCP.value.capacity == Type);
    }

void SLGMRecord::SetCapacity(UINT8 Type)
    {
    SLCP.value.capacity = Type;
    }

UINT32 SLGMRecord::GetSize(bool forceCalc)
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

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(SOUL.IsLoaded())
        TotSize += SOUL.GetSize() + 6;

    if(SLCP.IsLoaded())
        TotSize += SLCP.GetSize() + 6;

    return TotSize;
    }

UINT32 SLGMRecord::GetType()
    {
    return 'MGLS';
    }

STRING SLGMRecord::GetStrType()
    {
    return "SLGM";
    }

SINT32 SLGMRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'LUOS':
                SOUL.Read(buffer, subSize, curPos);
                break;
            case 'PCLS':
                SLCP.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  SLGM: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 SLGMRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    MODL.Unload();
    ICON.Unload();
    SCRI.Unload();
    DATA.Unload();
    SOUL.Unload();
    SLCP.Unload();
    return 1;
    }

SINT32 SLGMRecord::WriteRecord(_FileHandler &SaveHandler)
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
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(SOUL.IsLoaded())
        SaveHandler.writeSubRecord('LUOS', &SOUL.value, SOUL.GetSize());
    if(SLCP.IsLoaded())
        SaveHandler.writeSubRecord('PCLS', &SLCP.value, SLCP.GetSize());
    return -1;
    }

bool SLGMRecord::operator ==(const SLGMRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            MODL == other.MODL &&
            ICON.equalsi(other.ICON) &&
            SCRI == other.SCRI &&
            DATA == other.DATA &&
            SOUL == other.SOUL &&
            SLCP == other.SLCP);
    }

bool SLGMRecord::operator !=(const SLGMRecord &other) const
    {
    return !(*this == other);
    }