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
#include "ROADRecord.h"

ROADRecord::ROADPGRR::ROADPGRR():
    x(0.0f),
    y(0.0f),
    z(0.0f)
    {
    //
    }

ROADRecord::ROADPGRR::~ROADPGRR()
    {
    //
    }

bool ROADRecord::ROADPGRR::operator ==(const ROADPGRR &other) const
    {
    return (AlmostEqual(x,other.x,2) &&
            AlmostEqual(y,other.y,2) &&
            AlmostEqual(z,other.z,2));
    }

bool ROADRecord::ROADPGRR::operator !=(const ROADPGRR &other) const
    {
    return !(*this == other);
    }


ROADRecord::ROADRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

ROADRecord::ROADRecord(ROADRecord *srcRecord):
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

    PGRP = srcRecord->PGRP;
    PGRR = srcRecord->PGRR;
    return;
    }

ROADRecord::~ROADRecord()
    {
    //
    }

UINT32 ROADRecord::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 0;

    if(PGRP.size())
        {
        cSize = (sizeof(GENPGRP) * (UINT32)PGRP.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(PGRR.size())
        {
        cSize = (sizeof(ROADPGRR) * (UINT32)PGRR.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    return TotSize;
    }

UINT32 ROADRecord::GetType()
    {
    return 'DAOR';
    }

STRING ROADRecord::GetStrType()
    {
    return "ROAD";
    }

UINT32 ROADRecord::GetParentType()
    {
    return 'DLRW';
    }

SINT32 ROADRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'PRGP':
                if(subSize % sizeof(GENPGRP) == 0)
                    {
                    if(subSize == 0)
                        break;
                    PGRP.resize(subSize / sizeof(GENPGRP));
                    _readBuffer(&PGRP[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized PGRP size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'RRGP':
                if(subSize % sizeof(ROADPGRR) == 0)
                    {
                    if(subSize == 0)
                        break;
                    PGRR.resize(subSize / sizeof(ROADPGRR));
                    _readBuffer(&PGRR[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized ROADPGRR size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  ROADPGRR: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 ROADRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    PGRP.clear();
    PGRR.clear();
    return 1;
    }

SINT32 ROADRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(PGRP.size())
        SaveHandler.writeSubRecord('PRGP', &PGRP[0], sizeof(GENPGRP) * (UINT32)PGRP.size());
    if(PGRR.size())
        SaveHandler.writeSubRecord('RRGP', &PGRR[0], sizeof(ROADPGRR) * (UINT32)PGRR.size());
    return -1;
    }

bool ROADRecord::operator ==(const ROADRecord &other) const
    {
    if(PGRP.size() == other.PGRP.size() &&
        PGRR.size() == other.PGRR.size())
        {
        //Not sure if record order matters on pgrp, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < PGRP.size(); ++x)
            if(PGRP[x] != other.PGRP[x])
                return false;

        //Not sure if record order matters on pgrr, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < PGRR.size(); ++x)
            if(PGRR[x] != other.PGRR[x])
                return false;
        return true;
        }

    return false;
    }

bool ROADRecord::operator !=(const ROADRecord &other) const
    {
    return !(*this == other);
    }