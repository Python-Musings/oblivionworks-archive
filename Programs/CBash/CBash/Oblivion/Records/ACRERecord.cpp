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
#include "ACRERecord.h"

ACRERecord::ACRERecord(unsigned char *_recData):
Record(_recData)
    {
    IsTemporary(true);
    }

ACRERecord::ACRERecord(ACRERecord *srcRecord):
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
    NAME = srcRecord->NAME;
    if(srcRecord->Ownership.IsLoaded())
        {
        Ownership.Load();
        Ownership->XOWN = srcRecord->Ownership->XOWN;
        Ownership->XRNK = srcRecord->Ownership->XRNK;
        Ownership->XGLB = srcRecord->Ownership->XGLB;
        }
    XLOD = srcRecord->XLOD;
    XESP = srcRecord->XESP;
    XRGD = srcRecord->XRGD;
    XSCL = srcRecord->XSCL;
    DATA = srcRecord->DATA;
    return;
    }

ACRERecord::~ACRERecord()
    {
    //
    }

bool ACRERecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    op.Accept(NAME.value.fid);
    if(Ownership.IsLoaded())
        {
        if(Ownership->XOWN.IsLoaded())
            op.Accept(Ownership->XOWN.value.fid);
        if(Ownership->XGLB.IsLoaded())
            op.Accept(Ownership->XGLB->fid);
        }
    if(XESP.IsLoaded())
        op.Accept(XESP->parent);

    return op.Stop();
    }

bool ACRERecord::IsOppositeParent()
    {
    return XESP.IsLoaded() ? (XESP->flags & fIsOppositeParent) != 0 : false;
    }

void ACRERecord::IsOppositeParent(bool value)
    {
    if(!XESP.IsLoaded()) return;
    XESP->flags = value ? (XESP->flags | fIsOppositeParent) : (XESP->flags & ~fIsOppositeParent);
    }

bool ACRERecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    if(!XESP.IsLoaded()) return false;
    return Exact ? ((XESP->flags & Mask) == Mask) : ((XESP->flags & Mask) != 0);
    }

void ACRERecord::SetFlagMask(UINT8 Mask)
    {
    XESP.Load();
    XESP->flags = Mask;
    }

UINT32 ACRERecord::GetSize(bool forceCalc)
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

    if(NAME.IsLoaded())
        TotSize += NAME.GetSize() + 6;

    if(Ownership.IsLoaded() && Ownership->XOWN.IsLoaded())
        {
        TotSize += Ownership->XOWN.GetSize() + 6;
        if(Ownership->XRNK.IsLoaded())
            TotSize += Ownership->XRNK.GetSize() + 6;
        if(Ownership->XGLB.IsLoaded())
            TotSize += Ownership->XGLB.GetSize() + 6;
        }

    if(XLOD.IsLoaded())
        TotSize += XLOD.GetSize() + 6;

    if(XESP.IsLoaded())
        TotSize += XESP.GetSize() + 6;

    if(XRGD.IsLoaded())
        {
        cSize = XRGD.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(XSCL.IsLoaded())
        TotSize += XSCL.GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    return TotSize;
    }

UINT32 ACRERecord::GetType()
    {
    return 'ERCA';
    }

STRING ACRERecord::GetStrType()
    {
    return "ACRE";
    }

UINT32 ACRERecord::GetParentType()
    {
    return 'LLEC';
    }

SINT32 ACRERecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'EMAN':
                NAME.Read(buffer, subSize, curPos);
                break;
            case 'NWOX':
                Ownership.Load();
                Ownership->XOWN.Read(buffer, subSize, curPos);
                break;
            case 'KNRX':
                Ownership.Load();
                Ownership->XRNK.Read(buffer, subSize, curPos);
                break;
            case 'BLGX':
                Ownership.Load();
                Ownership->XGLB.Read(buffer, subSize, curPos);
                break;
            case 'DOLX':
                XLOD.Read(buffer, subSize, curPos);
                break;
            case 'PSEX':
                XESP.Read(buffer, subSize, curPos);
                break;
            case 'DGRX':
                XRGD.Read(buffer, subSize, curPos);
                break;
            case 'LCSX':
                XSCL.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  ACRE: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 ACRERecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    NAME.Unload();
    Ownership.Unload();
    XLOD.Unload();
    XESP.Unload();
    XRGD.Unload();
    XSCL.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 ACRERecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());

    if(NAME.IsLoaded())
        SaveHandler.writeSubRecord('EMAN', &NAME.value, NAME.GetSize());

    if(Ownership.IsLoaded() && Ownership->XOWN.IsLoaded())
        {
        SaveHandler.writeSubRecord('NWOX', &Ownership->XOWN.value, Ownership->XOWN.GetSize());
        if(Ownership->XRNK.IsLoaded())
            SaveHandler.writeSubRecord('KNRX', Ownership->XRNK.value, Ownership->XRNK.GetSize());
        if(Ownership->XGLB.IsLoaded())
            SaveHandler.writeSubRecord('BLGX', Ownership->XGLB.value, Ownership->XGLB.GetSize());
        }

    if(XLOD.IsLoaded())
        SaveHandler.writeSubRecord('DOLX', XLOD.value, XLOD.GetSize());

    if(XESP.IsLoaded())
        SaveHandler.writeSubRecord('PSEX', XESP.value, XESP.GetSize());

    if(XRGD.IsLoaded())
        SaveHandler.writeSubRecord('DGRX', XRGD.value, XRGD.GetSize());

    if(XSCL.IsLoaded())
        SaveHandler.writeSubRecord('LCSX', XSCL.value, XSCL.GetSize());

    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool ACRERecord::operator ==(const ACRERecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            NAME == other.NAME &&
            Ownership == other.Ownership &&
            XLOD == other.XLOD &&
            XESP == other.XESP &&
            XRGD == other.XRGD &&
            XSCL == other.XSCL &&
            DATA == other.DATA);
    }

bool ACRERecord::operator !=(const ACRERecord &other) const
    {
    return !(*this == other);
    }