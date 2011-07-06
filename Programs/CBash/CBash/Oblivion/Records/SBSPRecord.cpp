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
#include "SBSPRecord.h"

SBSPRecord::SBSPDNAM::SBSPDNAM():
    sizeX(0.0f),
    sizeY(0.0f),
    sizeZ(0.0f)
    {
    //
    }

SBSPRecord::SBSPDNAM::~SBSPDNAM()
    {
    //
    }

bool SBSPRecord::SBSPDNAM::operator ==(const SBSPDNAM &other) const
    {
    return (AlmostEqual(sizeX,other.sizeX,2) &&
            AlmostEqual(sizeY,other.sizeY,2) &&
            AlmostEqual(sizeZ,other.sizeZ,2));
    }

bool SBSPRecord::SBSPDNAM::operator !=(const SBSPDNAM &other) const
    {
    return !(*this == other);
    }

SBSPRecord::SBSPRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

SBSPRecord::SBSPRecord(SBSPRecord *srcRecord):
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
    DNAM = srcRecord->DNAM;
    }

SBSPRecord::~SBSPRecord()
    {
    //
    }

UINT32 SBSPRecord::GetSize(bool forceCalc)
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

    if(DNAM.IsLoaded())
        TotSize += DNAM.GetSize() + 6;

    return TotSize;
    }

UINT32 SBSPRecord::GetType()
    {
    return 'PSBS';
    }

STRING SBSPRecord::GetStrType()
    {
    return "SBSP";
    }

SINT32 SBSPRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'MAND':
                DNAM.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  SBSP: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 SBSPRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    DNAM.Unload();
    return 1;
    }

SINT32 SBSPRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(DNAM.IsLoaded())
        SaveHandler.writeSubRecord('MAND', &DNAM.value, DNAM.GetSize());
    return -1;
    }

bool SBSPRecord::operator ==(const SBSPRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            DNAM == other.DNAM);
    }

bool SBSPRecord::operator !=(const SBSPRecord &other) const
    {
    return !(*this == other);
    }