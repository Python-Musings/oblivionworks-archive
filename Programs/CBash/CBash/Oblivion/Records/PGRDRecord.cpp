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
#include "PGRDRecord.h"

PGRDRecord::PGRDDATA::PGRDDATA():
    count(0)
    {
    //
    }

PGRDRecord::PGRDDATA::~PGRDDATA()
    {
    //
    }

bool PGRDRecord::PGRDDATA::operator ==(const PGRDDATA &other) const
    {
    return (count == other.count);
    }

bool PGRDRecord::PGRDDATA::operator !=(const PGRDDATA &other) const
    {
    return !(*this == other);
    }

PGRDRecord::PGRDPGRI::PGRDPGRI():point(0), x(0.0f), y(0.0f), z(0.0f)
    {
    memset(&unused1, 0x00, 2);
    }

PGRDRecord::PGRDPGRI::~PGRDPGRI()
    {
    //
    }

bool PGRDRecord::PGRDPGRI::operator ==(const PGRDPGRI &other) const
    {
    return (point == other.point &&
            AlmostEqual(x,other.x,2) &&
            AlmostEqual(y,other.y,2) &&
            AlmostEqual(z,other.z,2));
    }

bool PGRDRecord::PGRDPGRI::operator !=(const PGRDPGRI &other) const
    {
    return !(*this == other);
    }

PGRDRecord::PGRDPGRL::PGRDPGRL()
    {
    //First element must be allocated since it's where the reference is stored
    points.push_back(0);
    }

PGRDRecord::PGRDPGRL::~PGRDPGRL()
    {
    //
    }

bool PGRDRecord::PGRDPGRL::operator ==(const PGRDPGRL &other) const
    {
    if(points.size() == other.points.size())
        {
        //Not sure if record order matters on points, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < points.size(); ++x)
            if(points[x] != other.points[x])
                return false;
        return true;
        }

    return false;
    }

bool PGRDRecord::PGRDPGRL::operator !=(const PGRDPGRL &other) const
    {
    return !(*this == other);
    }

PGRDRecord::PGRDRecord(unsigned char *_recData):
    Record(_recData)
    {
    IsCompressed(true);
    }

PGRDRecord::PGRDRecord(PGRDRecord *srcRecord):
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


    DATA = srcRecord->DATA;
    PGRP = srcRecord->PGRP;
    PGAG = srcRecord->PGAG;
    PGRR = srcRecord->PGRR;
    PGRI = srcRecord->PGRI;

    PGRL.resize(srcRecord->PGRL.size());
    for(UINT32 x = 0; x < srcRecord->PGRL.size(); x++)
        {
        PGRL[x] = new PGRDPGRL;
        *PGRL[x] = *srcRecord->PGRL[x];
        }
    }

PGRDRecord::~PGRDRecord()
    {
    for(UINT32 x = 0; x < PGRL.size(); x++)
        delete PGRL[x];
    }

bool PGRDRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < PGRL.size(); x++)
        op.Accept(PGRL[x]->points[0]);

    return op.Stop();
    }

UINT32 PGRDRecord::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 0;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(PGRP.size())
        {
        cSize = (sizeof(GENPGRP) * (UINT32)PGRP.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(PGAG.IsLoaded())
        {
        cSize = PGAG.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(PGRR.IsLoaded())
        {
        cSize = PGRR.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    //if(PGRR.size())
    //    cSize += 6 + (sizeof(PGRDPGRR) * (UINT32)PGRR.size());

    if(PGRI.size())
        {
        cSize = (sizeof(PGRDPGRI) * (UINT32)PGRI.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    for(UINT32 x = 0; x < PGRL.size(); ++x)
        {
        cSize = (sizeof(UINT32) * (UINT32)PGRL[x]->points.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    return TotSize;
    }

UINT32 PGRDRecord::GetType()
    {
    return 'DRGP';
    }

STRING PGRDRecord::GetStrType()
    {
    return "PGRD";
    }

UINT32 PGRDRecord::GetParentType()
    {
    return 'LLEC';
    }

SINT32 PGRDRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    PGRDPGRL *curPGRL = NULL;
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
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
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
            case 'GAGP':
                PGAG.Read(buffer, subSize, curPos);
                break;
            case 'RRGP':
                PGRR.Read(buffer, subSize, curPos);
                break;
                //if(subSize % sizeof(PGRDPGRR) == 0)
                //    {
                //    if(subSize == 0)
                //        break;
                //    PGRR.resize(subSize / sizeof(PGRDPGRR));
                //    _readBuffer(&PGRR[0], buffer, subSize, curPos);
                //    }
                //else
                //    {
                //    printf("  Unrecognized PGRR size: %i\n", subSize);
                //    curPos += subSize;
                //    }
                //break;
            case 'IRGP':
                if(subSize % sizeof(PGRDPGRI) == 0)
                    {
                    if(subSize == 0)
                        break;
                    PGRI.resize(subSize / sizeof(PGRDPGRI));
                    _readBuffer(&PGRI[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized PGRI size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'LRGP':
                if(subSize % sizeof(UINT32) == 0)
                    {
                    if(subSize == 0)
                        break;
                    curPGRL = new PGRDPGRL;
                    curPGRL->points.resize(subSize / sizeof(UINT32));
                    _readBuffer(&curPGRL->points[0], buffer, subSize, curPos);
                    PGRL.push_back(curPGRL);
                    }
                else
                    {
                    printf("  Unrecognized PGRL size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  PGRD: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 PGRDRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    DATA.Unload();
    PGRP.clear();
    PGAG.Unload();
    PGRR.Unload();
    PGRI.clear();
    for(UINT32 x = 0; x < PGRL.size(); x++)
        delete PGRL[x];
    PGRL.clear();
    return 1;
    }

SINT32 PGRDRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(PGRP.size())
        SaveHandler.writeSubRecord('PRGP', &PGRP[0], sizeof(GENPGRP) * (UINT32)PGRP.size());
    if(PGAG.IsLoaded())
        SaveHandler.writeSubRecord('GAGP', PGAG.value, PGAG.GetSize());
    if(PGRR.IsLoaded())
        SaveHandler.writeSubRecord('RRGP', PGRR.value, PGRR.GetSize());
    //if(PGRR.size())
    //    SaveHandler.writeSubRecord('RRGP', &PGRR[0], sizeof(PGRDPGRR) * (UINT32)PGRR.size());
    if(PGRI.size())
        SaveHandler.writeSubRecord('IRGP', &PGRI[0], sizeof(PGRDPGRI) * (UINT32)PGRI.size());
    for(UINT32 x = 0; x < PGRL.size(); ++x)
        SaveHandler.writeSubRecord('LRGP', &PGRL[x]->points[0], (sizeof(UINT32) * (UINT32)PGRL[x]->points.size()));
    return -1;
    }

bool PGRDRecord::operator ==(const PGRDRecord &other) const
    {
    if(DATA == other.DATA &&
        PGAG == other.PGAG &&
        PGRR == other.PGRR &&
        PGRP.size() == other.PGRP.size() &&
        PGRI.size() == other.PGRI.size() &&
        PGRL.size() == other.PGRL.size())
        {
        //Not sure if record order matters on pgrp, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < PGRP.size(); ++x)
            if(PGRP[x] != other.PGRP[x])
                return false;

        //Not sure if record order matters on pgri, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < PGRI.size(); ++x)
            if(PGRI[x] != other.PGRI[x])
                return false;

        //Not sure if record order matters on pgrl, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < PGRL.size(); ++x)
            if(*PGRL[x] != *other.PGRL[x])
                return false;
        return true;
        }

    return false;
    }

bool PGRDRecord::operator !=(const PGRDRecord &other) const
    {
    return !(*this == other);
    }