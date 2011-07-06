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
#include "GRASRecord.h"

GRASRecord::GRASDATA::GRASDATA():
    density(0),
    minSlope(0),
    maxSlope(0),
    unused1(0xCD),
    waterDistance(0),
    waterOp(0),
    posRange(0.0f),
    heightRange(0.0f),
    colorRange(0.0f),
    wavePeriod(0.0f),
    flags(0)
    {
    memset(&unused2, 0xCD, 2);
    memset(&unused3, 0xCD, 3);
    }

GRASRecord::GRASDATA::~GRASDATA()
    {
    //
    }

bool GRASRecord::GRASDATA::operator ==(const GRASDATA &other) const
    {
    return (density == other.density &&
            minSlope == other.minSlope &&
            maxSlope == other.maxSlope &&
            waterDistance == other.waterDistance &&
            waterOp == other.waterOp &&
            AlmostEqual(posRange,other.posRange,2) &&
            AlmostEqual(heightRange,other.heightRange,2) &&
            AlmostEqual(colorRange,other.colorRange,2) &&
            AlmostEqual(wavePeriod,other.wavePeriod,2) &&
            flags == other.flags);
    }

bool GRASRecord::GRASDATA::operator !=(const GRASDATA &other) const
    {
    return !(*this == other);
    }

GRASRecord::GRASRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

GRASRecord::GRASRecord(GRASRecord *srcRecord):
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
    DATA = srcRecord->DATA;
    return;
    }

GRASRecord::~GRASRecord()
    {
    //
    }

bool GRASRecord::IsVLighting()
    {
    return (DATA.value.flags & fIsVLighting) != 0;
    }

void GRASRecord::IsVLighting(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsVLighting) : (DATA.value.flags & ~fIsVLighting);
    }

bool GRASRecord::IsUScaling()
    {
    return (DATA.value.flags & fIsUScaling) != 0;
    }

void GRASRecord::IsUScaling(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsUScaling) : (DATA.value.flags & ~fIsUScaling);
    }

bool GRASRecord::IsFitSlope()
    {
    return (DATA.value.flags & fIsFitSlope) != 0;
    }

void GRASRecord::IsFitSlope(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsFitSlope) : (DATA.value.flags & ~fIsFitSlope);
    }

bool GRASRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void GRASRecord::SetFlagMask(UINT8 Mask)
    {
    DATA.value.flags = Mask;
    }

UINT32 GRASRecord::GetSize(bool forceCalc)
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

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    return TotSize;
    }

UINT32 GRASRecord::GetType()
    {
    return 'SARG';
    }

STRING GRASRecord::GetStrType()
    {
    return "GRAS";
    }

SINT32 GRASRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  GRAS: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 GRASRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    MODL.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 GRASRecord::WriteRecord(_FileHandler &SaveHandler)
    {
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
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool GRASRecord::operator ==(const GRASRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            MODL == other.MODL &&
            DATA == other.DATA);
    }

bool GRASRecord::operator !=(const GRASRecord &other) const
    {
    return !(*this == other);
    }