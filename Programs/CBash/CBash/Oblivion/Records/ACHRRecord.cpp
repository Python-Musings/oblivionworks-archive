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
#include "ACHRRecord.h"

ACHRRecord::ACHRRecord(unsigned char *_recData):
    Record(_recData)
    {
    IsPersistent(true);
    }

ACHRRecord::ACHRRecord(ACHRRecord *srcRecord):
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
    if(srcRecord->XPCI.IsLoaded())
        {
        XPCI.Load();
        XPCI->XPCI = srcRecord->XPCI->XPCI;
        XPCI->FULL = srcRecord->XPCI->FULL;
        }
    XLOD = srcRecord->XLOD;
    XESP = srcRecord->XESP;
    XMRC = srcRecord->XMRC;
    XHRS = srcRecord->XHRS;
    XRGD = srcRecord->XRGD;
    XSCL = srcRecord->XSCL;
    DATA = srcRecord->DATA;
    return;
    }

ACHRRecord::~ACHRRecord()
    {
    //
    }

bool ACHRRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    op.Accept(NAME.value.fid);
    if(XPCI.IsLoaded() && XPCI->XPCI.IsLoaded())
        op.Accept(XPCI->XPCI->fid);
    if(XESP.IsLoaded())
        op.Accept(XESP->parent);
    if(XMRC.IsLoaded())
        op.Accept(XMRC->fid);
    op.Accept(XHRS.value.fid);

    return op.Stop();
    }

bool ACHRRecord::IsOppositeParent()
    {
    return XESP.IsLoaded() ? (XESP->flags & fIsOppositeParent) != 0 : false;
    }

void ACHRRecord::IsOppositeParent(bool value)
    {
    if(!XESP.IsLoaded()) return;
    XESP->flags = value ? (XESP->flags | fIsOppositeParent) : (XESP->flags & ~fIsOppositeParent);
    }

bool ACHRRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    if(!XESP.IsLoaded()) return false;
    return Exact ? ((XESP->flags & Mask) == Mask) : ((XESP->flags & Mask) != 0);
    }

void ACHRRecord::SetFlagMask(UINT8 Mask)
    {
    XESP.Load();
    XESP->flags = Mask;
    }

UINT32 ACHRRecord::GetSize(bool forceCalc)
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

    if(XPCI.IsLoaded() && XPCI->XPCI.IsLoaded())
        {
        TotSize += XPCI->XPCI.GetSize() + 6;
        if(XPCI->FULL.IsLoaded())
            {
            cSize = XPCI->FULL.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        else TotSize += 7;
        }

    if(XLOD.IsLoaded())
        TotSize += XLOD.GetSize() + 6;

    if(XESP.IsLoaded())
        TotSize += XESP.GetSize() + 6;

    if(XMRC.IsLoaded())
        TotSize += XMRC.GetSize() + 6;

    if(XHRS.IsLoaded())
        TotSize += XHRS.GetSize() + 6;

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

UINT32 ACHRRecord::GetType()
    {
    return 'RHCA';
    }

STRING ACHRRecord::GetStrType()
    {
    return "ACHR";
    }

UINT32 ACHRRecord::GetParentType()
    {
    return 'LLEC';
    }

SINT32 ACHRRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'ICPX':
                XPCI.Load();
                XPCI->XPCI.Read(buffer, subSize, curPos);
                break;
            case 'LLUF':
                XPCI.Load();
                XPCI->FULL.Read(buffer, subSize, curPos);
                break;
            case 'DOLX':
                XLOD.Read(buffer, subSize, curPos);
                break;
            case 'PSEX':
                XESP.Read(buffer, subSize, curPos);
                break;
            case 'CRMX':
                XMRC.Read(buffer, subSize, curPos);
                break;
            case 'SRHX':
                XHRS.Read(buffer, subSize, curPos);
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
                printf("  ACHR: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 ACHRRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    NAME.Unload();
    XPCI.Unload();
    XLOD.Unload();
    XESP.Unload();
    XMRC.Unload();
    XHRS.Unload();
    XRGD.Unload();
    XSCL.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 ACHRRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    char null = 0;

    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());

    if(NAME.IsLoaded())
        SaveHandler.writeSubRecord('EMAN', &NAME.value, NAME.GetSize());

    if(XPCI.IsLoaded() && XPCI->XPCI.IsLoaded())
        {
        SaveHandler.writeSubRecord('ICPX', XPCI->XPCI.value, XPCI->XPCI.GetSize());
        if(XPCI->FULL.IsLoaded())
            SaveHandler.writeSubRecord('LLUF', XPCI->FULL.value, XPCI->FULL.GetSize());
        else
            SaveHandler.writeSubRecord('LLUF', &null, 1);
        }

    if(XLOD.IsLoaded())
        SaveHandler.writeSubRecord('DOLX', XLOD.value, XLOD.GetSize());

    if(XESP.IsLoaded())
        SaveHandler.writeSubRecord('PSEX', XESP.value, XESP.GetSize());

    if(XMRC.IsLoaded())
        SaveHandler.writeSubRecord('CRMX', XMRC.value, XMRC.GetSize());

    if(XHRS.IsLoaded())
        SaveHandler.writeSubRecord('SRHX', &XHRS.value, XHRS.GetSize());

    if(XRGD.IsLoaded())
        SaveHandler.writeSubRecord('DGRX', XRGD.value, XRGD.GetSize());

    if(XSCL.IsLoaded())
        SaveHandler.writeSubRecord('LCSX', XSCL.value, XSCL.GetSize());

    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool ACHRRecord::operator ==(const ACHRRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            NAME == other.NAME &&
            XPCI == other.XPCI &&
            XLOD == other.XLOD &&
            XESP == other.XESP &&
            XMRC == other.XMRC &&
            XHRS == other.XHRS &&
            XRGD == other.XRGD &&
            XSCL == other.XSCL &&
            DATA == other.DATA);
    }

bool ACHRRecord::operator !=(const ACHRRecord &other) const
    {
    return !(*this == other);
    }