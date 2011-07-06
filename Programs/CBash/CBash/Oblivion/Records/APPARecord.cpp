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
#include "APPARecord.h"

APPARecord::APPADATA::APPADATA():
    apparatus(0),
    value(25),
    weight(1.0f),
    quality(10.0f)
    {
    //
    }

APPARecord::APPADATA::~APPADATA()
    {
    //
    }

bool APPARecord::APPADATA::operator ==(const APPADATA &other) const
    {
    return (apparatus == other.apparatus &&
            value == other.value &&
            AlmostEqual(weight,other.weight,2) &&
            AlmostEqual(quality,other.quality,2));
    }

bool APPARecord::APPADATA::operator !=(const APPADATA &other) const
    {
    return !(*this == other);
    }

APPARecord::APPARecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

APPARecord::APPARecord(APPARecord *srcRecord):
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
    return;
    }

APPARecord::~APPARecord()
    {
    //
    }

bool APPARecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);

    return op.Stop();
    }

bool APPARecord::IsMortarPestle()
    {
    return (DATA.value.apparatus == eMortarPestle);
    }

void APPARecord::IsMortarPestle(bool value)
    {
    if(value)
        DATA.value.apparatus = eMortarPestle;
    else
        DATA.value.apparatus = eAlembic;
    }

bool APPARecord::IsAlembic()
    {
    return (DATA.value.apparatus == eAlembic);
    }

void APPARecord::IsAlembic(bool value)
    {
    if(value)
        DATA.value.apparatus = eAlembic;
    else
        DATA.value.apparatus = eMortarPestle;
    }

bool APPARecord::IsCalcinator()
    {
    return (DATA.value.apparatus == eCalcinator);
    }

void APPARecord::IsCalcinator(bool value)
    {
    if(value)
        DATA.value.apparatus = eCalcinator;
    else
        DATA.value.apparatus = eMortarPestle;
    }

bool APPARecord::IsRetort()
    {
    return (DATA.value.apparatus == eRetort);
    }

void APPARecord::IsRetort(bool value)
    {
    if(value)
        DATA.value.apparatus = eRetort;
    else
        DATA.value.apparatus = eMortarPestle;
    }

bool APPARecord::IsType(UINT8 Type)
    {
    return (DATA.value.apparatus == Type);
    }

void APPARecord::SetType(UINT8 Type)
    {
    DATA.value.apparatus = Type;
    }

UINT32 APPARecord::GetSize(bool forceCalc)
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

    return TotSize;
    }

UINT32 APPARecord::GetType()
    {
    return 'APPA';
    }

STRING APPARecord::GetStrType()
    {
    return "APPA";
    }

SINT32 APPARecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            default:
                //printf("FileName = %s\n", FileName);
                printf("  APPA: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 APPARecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    MODL.Unload();
    ICON.Unload();
    SCRI.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 APPARecord::WriteRecord(_FileHandler &SaveHandler)
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
    return -1;
    }

bool APPARecord::operator ==(const APPARecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            MODL == other.MODL &&
            ICON.equalsi(other.ICON) &&
            SCRI == other.SCRI &&
            DATA == other.DATA);
    }

bool APPARecord::operator !=(const APPARecord &other) const
    {
    return !(*this == other);
    }