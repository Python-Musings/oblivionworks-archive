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
#include "SKILRecord.h"

SKILRecord::SKILINDX::SKILINDX():
    skill(26)
    {
    //
    }

SKILRecord::SKILINDX::~SKILINDX()
    {
    //
    }

bool SKILRecord::SKILINDX::operator ==(const SKILINDX &other) const
    {
    return (skill == other.skill);
    }

bool SKILRecord::SKILINDX::operator !=(const SKILINDX &other) const
    {
    return !(*this == other);
    }

SKILRecord::SKILDATA::SKILDATA():
    action(26),
    attribute(0),
    specialization(0),
    use0(1.0f),
    use1(1.0f)
    {
    //
    }

SKILRecord::SKILDATA::~SKILDATA()
    {
    //
    }

bool SKILRecord::SKILDATA::operator ==(const SKILDATA &other) const
    {
    return (action == other.action &&
            attribute == other.attribute &&
            specialization == other.specialization &&
            AlmostEqual(use0,other.use0,2) &&
            AlmostEqual(use1,other.use1,2));
    }

bool SKILRecord::SKILDATA::operator !=(const SKILDATA &other) const
    {
    return !(*this == other);
    }

SKILRecord::SKILRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

SKILRecord::SKILRecord(SKILRecord *srcRecord):
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

    INDX = srcRecord->INDX;
    DESC = srcRecord->DESC;
    ICON = srcRecord->ICON;
    DATA = srcRecord->DATA;
    ANAM = srcRecord->ANAM;
    JNAM = srcRecord->JNAM;
    ENAM = srcRecord->ENAM;
    MNAM = srcRecord->MNAM;
    }

SKILRecord::~SKILRecord()
    {
    //
    }

UINT32 SKILRecord::GetSize(bool forceCalc)
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

    if(INDX.IsLoaded())
        TotSize += INDX.GetSize() + 6;

    if(DESC.IsLoaded())
        {
        cSize = DESC.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(ANAM.IsLoaded())
        TotSize += ANAM.GetSize() + 6;

    if(JNAM.IsLoaded())
        {
        cSize = JNAM.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ENAM.IsLoaded())
        {
        cSize = ENAM.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(MNAM.IsLoaded())
        {
        cSize = MNAM.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    return TotSize;
    }

UINT32 SKILRecord::GetType()
    {
    return 'LIKS';
    }

STRING SKILRecord::GetStrType()
    {
    return "SKIL";
    }

SINT32 SKILRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'XDNI':
                INDX.Read(buffer, subSize, curPos);
                break;
            case 'CSED':
                DESC.Read(buffer, subSize, curPos);
                break;
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'MANA':
                ANAM.Read(buffer, subSize, curPos);
                break;
            case 'MANJ':
                JNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANE':
                ENAM.Read(buffer, subSize, curPos);
                break;
            case 'MANM':
                MNAM.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  SKIL: Unknown subType = %04X\n", subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 SKILRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    INDX.Unload();
    DESC.Unload();
    ICON.Unload();
    DATA.Unload();
    ANAM.Unload();
    JNAM.Unload();
    ENAM.Unload();
    MNAM.Unload();
    return 1;
    }

SINT32 SKILRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(INDX.IsLoaded())
        SaveHandler.writeSubRecord('XDNI', &INDX.value, INDX.GetSize());
    if(DESC.IsLoaded())
        SaveHandler.writeSubRecord('CSED', DESC.value, DESC.GetSize());
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(ANAM.IsLoaded())
        SaveHandler.writeSubRecord('MANA', ANAM.value, ANAM.GetSize());
    if(JNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANJ', JNAM.value, JNAM.GetSize());
    if(ENAM.IsLoaded())
        SaveHandler.writeSubRecord('MANE', ENAM.value, ENAM.GetSize());
    if(MNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANM', MNAM.value, MNAM.GetSize());
    return -1;
    }

bool SKILRecord::operator ==(const SKILRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            INDX == other.INDX &&
            DESC.equals(other.DESC) &&
            ICON.equalsi(other.ICON) &&
            DATA == other.DATA &&
            ANAM.equals(other.ANAM) &&
            JNAM.equals(other.JNAM) &&
            ENAM.equals(other.ENAM) &&
            MNAM.equals(other.MNAM));
    }

bool SKILRecord::operator !=(const SKILRecord &other) const
    {
    return !(*this == other);
    }