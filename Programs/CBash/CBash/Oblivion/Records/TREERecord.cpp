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
#include "TREERecord.h"
#include <vector>

TREERecord::TREECNAM::TREECNAM():
    curvature(0.0f),
    minAngle(0.0f),
    maxAngle(0.0f),
    branchDim(0.0f),
    leafDim(0.0f),
    shadowRadius(0),
    rockSpeed(0.0f),
    rustleSpeed(0.0f)
    {
    //
    }

TREERecord::TREECNAM::~TREECNAM()
    {
    //
    }

bool TREERecord::TREECNAM::operator ==(const TREECNAM &other) const
    {
    return (AlmostEqual(curvature,other.curvature,2) &&
            AlmostEqual(minAngle,other.minAngle,2) &&
            AlmostEqual(maxAngle,other.maxAngle,2) &&
            AlmostEqual(branchDim,other.branchDim,2) &&
            AlmostEqual(leafDim,other.leafDim,2) &&
            shadowRadius == other.shadowRadius &&
            AlmostEqual(rockSpeed,other.rockSpeed,2) &&
            AlmostEqual(rustleSpeed,other.rustleSpeed,2));
    }

bool TREERecord::TREECNAM::operator !=(const TREECNAM &other) const
    {
    return !(*this == other);
    }

TREERecord::TREEBNAM::TREEBNAM():
    widthBill(0.0f),
    heightBill(0.0f)
    {
    //
    }

TREERecord::TREEBNAM::~TREEBNAM()
    {
    //
    }

bool TREERecord::TREEBNAM::operator ==(const TREEBNAM &other) const
    {
    return (AlmostEqual(widthBill,other.widthBill,2) &&
            AlmostEqual(heightBill,other.heightBill,2));
    }

bool TREERecord::TREEBNAM::operator !=(const TREEBNAM &other) const
    {
    return !(*this == other);
    }

TREERecord::TREERecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

TREERecord::TREERecord(TREERecord *srcRecord):
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
    ICON = srcRecord->ICON;
    SNAM = srcRecord->SNAM;
    CNAM = srcRecord->CNAM;
    BNAM = srcRecord->BNAM;
    }

TREERecord::~TREERecord()
    {
    //
    }

UINT32 TREERecord::GetSize(bool forceCalc)
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

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(SNAM.size())
        {
        cSize = (sizeof(UINT32) * (UINT32)SNAM.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(CNAM.IsLoaded())
        TotSize += CNAM.GetSize() + 6;

    if(BNAM.IsLoaded())
        TotSize += BNAM.GetSize() + 6;

    return TotSize;
    }

UINT32 TREERecord::GetType()
    {
    return 'EERT';
    }

STRING TREERecord::GetStrType()
    {
    return "TREE";
    }

SINT32 TREERecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case 'MANS':
                if(subSize % sizeof(UINT32) == 0)
                    {
                    if(subSize == 0)
                        break;
                    SNAM.resize(subSize / sizeof(UINT32));
                    _readBuffer(&SNAM[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized SNAM size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'MANC':
                CNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANB':
                BNAM.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  TREE: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 TREERecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    MODL.Unload();
    ICON.Unload();
    SNAM.clear();
    CNAM.Unload();
    BNAM.Unload();
    return 1;
    }

SINT32 TREERecord::WriteRecord(_FileHandler &SaveHandler)
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
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(SNAM.size())
        SaveHandler.writeSubRecord('MANS', &SNAM[0], (UINT32)SNAM.size() * sizeof(UINT32));
    //else
    //    SaveHandler.writeSubRecord('MANS', NULL, 0);
    if(CNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANC', &CNAM.value, CNAM.GetSize());
    if(BNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANB', &BNAM.value, BNAM.GetSize());
    return -1;
    }

bool TREERecord::operator ==(const TREERecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        MODL == other.MODL &&
        ICON.equalsi(other.ICON) &&
        CNAM == other.CNAM &&
        BNAM == other.BNAM &&
        SNAM.size() == other.SNAM.size())
        {
        //Not sure if record order matters on speed trees, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < SNAM.size(); ++x)
            if(SNAM[x] != other.SNAM[x])
                return false;
        return true;
        }

    return false;
    }

bool TREERecord::operator !=(const TREERecord &other) const
    {
    return !(*this == other);
    }