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
#include "GLOBRecord.h"

GLOBRecord::GLOBFNAM::GLOBFNAM():
    format('f')
    {
    //
    }

GLOBRecord::GLOBFNAM::~GLOBFNAM()
    {
    //
    }

bool GLOBRecord::GLOBFNAM::operator ==(const GLOBFNAM &other) const
    {
    return (format == other.format);
    }

bool GLOBRecord::GLOBFNAM::operator !=(const GLOBFNAM &other) const
    {
    return !(*this == other);
    }

GLOBRecord::GLOBFLTV::GLOBFLTV():
    value(0.0f)
    {
    //
    }

GLOBRecord::GLOBFLTV::~GLOBFLTV()
    {
    //
    }

bool GLOBRecord::GLOBFLTV::operator ==(const GLOBFLTV &other) const
    {
    return (AlmostEqual(value,other.value,2));
    }

bool GLOBRecord::GLOBFLTV::operator !=(const GLOBFLTV &other) const
    {
    return !(*this == other);
    }

GLOBRecord::GLOBRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

GLOBRecord::GLOBRecord(GLOBRecord *srcRecord):
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
    FNAM = srcRecord->FNAM;
    FLTV = srcRecord->FLTV;
    }

GLOBRecord::~GLOBRecord()
    {
    //
    }

UINT32 GLOBRecord::GetSize(bool forceCalc)
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

    if(FNAM.IsLoaded())
        TotSize += FNAM.GetSize() + 6;

    if(FLTV.IsLoaded())
        TotSize += FLTV.GetSize() + 6;

    return TotSize;
    }

UINT32 GLOBRecord::GetType()
    {
    return 'BOLG';
    }

STRING GLOBRecord::GetStrType()
    {
    return "GLOB";
    }

SINT32 GLOBRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'MANF':
                FNAM.Read(buffer, subSize, curPos);
                break;
            case 'VTLF':
                FLTV.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  GLOB: Unknown subType = %04X\n", subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 GLOBRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FNAM.Unload();
    FLTV.Unload();
    return 1;
    }

SINT32 GLOBRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());

    if(FNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANF', &FNAM.value, FNAM.GetSize());

    if(FLTV.IsLoaded())
        SaveHandler.writeSubRecord('VTLF', &FLTV.value, FLTV.GetSize());

    return -1;
    }

bool GLOBRecord::operator ==(const GLOBRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FNAM == other.FNAM &&
            FLTV == other.FLTV);
    }

bool GLOBRecord::operator !=(const GLOBRecord &other) const
    {
    return !(*this == other);
    }