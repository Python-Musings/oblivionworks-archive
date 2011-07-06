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
#include "WRLDRecord.h"
#include <vector>

WRLDRecord::WRLDMNAM::WRLDMNAM():
    dimX(0),
    dimY(0),
    NWCellX(0),
    NWCellY(0),
    SECellX(0),
    SECellY(0)
    {
    //
    }

WRLDRecord::WRLDMNAM::~WRLDMNAM()
    {
    //
    }

bool WRLDRecord::WRLDMNAM::operator ==(const WRLDMNAM &other) const
    {
    return (dimX == other.dimX &&
            dimY == other.dimY &&
            NWCellX == other.NWCellX &&
            NWCellY == other.NWCellY &&
            SECellX == other.SECellX &&
            SECellY == other.SECellY);
    }

bool WRLDRecord::WRLDMNAM::operator !=(const WRLDMNAM &other) const
    {
    return !(*this == other);
    }

WRLDRecord::WRLDUNK::WRLDUNK():
    unk1(0.0f),
    unk2(0.0f)
    {
    //
    }

WRLDRecord::WRLDUNK::~WRLDUNK()
    {
    //
    }

bool WRLDRecord::WRLDUNK::operator ==(const WRLDUNK &other) const
    {
    return (AlmostEqual(unk1,other.unk1,2) &&
            AlmostEqual(unk2,other.unk2,2));
    }

bool WRLDRecord::WRLDUNK::operator !=(const WRLDUNK &other) const
    {
    return !(*this == other);
    }

WRLDRecord::WRLDRecord(unsigned char *_recData):
    Record(_recData),
    ROAD(NULL),
    CELL(NULL)
    {
    //
    }

WRLDRecord::WRLDRecord(WRLDRecord *srcRecord):
    Record(),
    ROAD(NULL),
    CELL(NULL)
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
    WNAM = srcRecord->WNAM;
    CNAM = srcRecord->CNAM;
    NAM2 = srcRecord->NAM2;
    ICON = srcRecord->ICON;
    MNAM = srcRecord->MNAM;
    DATA = srcRecord->DATA;
    NAM0 = srcRecord->NAM0;
    NAM9 = srcRecord->NAM9;
    SNAM = srcRecord->SNAM;
    OFST = srcRecord->OFST;
    }

WRLDRecord::~WRLDRecord()
    {
    delete ROAD;
    delete CELL;
    for(UINT32 x = 0; x < CELLS.size(); ++x)
        delete CELLS[x];
    }

bool WRLDRecord::HasSubRecords()
    {
    return true;
    }

bool WRLDRecord::VisitSubRecords(const UINT32 &RecordType, RecordOp &op)
    {
    bool stop;

    if(RecordType == NULL || RecordType == 'DAOR')
        {
        if(ROAD != NULL)
            {
            if(op.Accept(ROAD))
                return true;
            }
        }

    if(RecordType == NULL || RecordType != 'LLEC' || 
        RecordType != 'DRGP' || RecordType != 'DNAL' || 
        RecordType != 'RFER' || RecordType != 'RHCA' || 
        RecordType != 'ERCA')
        {
        if(CELL != NULL)
            {
            if(op.Accept(CELL))
                return true;
            }

        for(UINT32 x = 0; x < CELLS.size();++x)
            {
            stop = op.Accept(CELLS[x]);
            if(CELLS[x] == NULL)
                {
                CELLS.erase(CELLS.begin() + x);
                --x;
                }
            if(stop)
                return stop;
            }
        }

    return op.Stop();
    }

bool WRLDRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(WNAM.IsLoaded())
        op.Accept(WNAM->fid);
    if(CNAM.IsLoaded())
        op.Accept(CNAM->fid);
    if(NAM2.IsLoaded())
        op.Accept(NAM2->fid);

    return op.Stop();
    }

bool WRLDRecord::IsSmallWorld()
    {
    return (DATA.value.flags & fSmallWorld) != 0;
    }

void WRLDRecord::IsSmallWorld(bool value)
    {
    if(value)
        DATA.value.flags |= fSmallWorld;
    else
        DATA.value.flags &= ~fSmallWorld;
    }

bool WRLDRecord::IsNoFastTravel()
    {
    return (DATA.value.flags & fNoFastTravel) != 0;
    }

void WRLDRecord::IsNoFastTravel(bool value)
    {
    if(value)
        DATA.value.flags |= fNoFastTravel;
    else
        DATA.value.flags &= ~fNoFastTravel;
    }

bool WRLDRecord::IsFastTravel()
    {
    return !IsNoFastTravel();
    }

void WRLDRecord::IsFastTravel(bool value)
    {
    IsNoFastTravel(!value);
    }

bool WRLDRecord::IsOblivionWorldspace()
    {
    return (DATA.value.flags & fOblivionWorldspace) != 0;
    }

void WRLDRecord::IsOblivionWorldspace(bool value)
    {
    if(value)
        DATA.value.flags |= fOblivionWorldspace;
    else
        DATA.value.flags &= ~fOblivionWorldspace;
    }

bool WRLDRecord::IsNoLODWater()
    {
    return (DATA.value.flags & fNoLODWater) != 0;
    }

void WRLDRecord::IsNoLODWater(bool value)
    {
    if(value)
        DATA.value.flags |= fNoLODWater;
    else
        DATA.value.flags &= ~fNoLODWater;
    }

bool WRLDRecord::IsLODWater()
    {
    return !IsNoLODWater();
    }

void WRLDRecord::IsLODWater(bool value)
    {
    IsNoLODWater(!value);
    }

bool WRLDRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void WRLDRecord::SetFlagMask(UINT8 Mask)
    {
    DATA.value.flags = Mask;
    }

bool WRLDRecord::IsDefaultMusic()
    {
    if(!SNAM.IsLoaded()) return false;
    return (SNAM->flags == eDefault);
    }

void WRLDRecord::IsDefaultMusic(bool value)
    {
    if(!SNAM.IsLoaded()) return;
    if(value)
        SNAM->flags = eDefault;
    else if(IsDefaultMusic())
        SNAM->flags = ePublic;
    }

bool WRLDRecord::IsPublicMusic()
    {
    if(!SNAM.IsLoaded()) return false;
    return (SNAM->flags == ePublic);
    }

void WRLDRecord::IsPublicMusic(bool value)
    {
    if(!SNAM.IsLoaded()) return;
    if(value)
        SNAM->flags = ePublic;
    else if(IsPublicMusic())
        SNAM->flags = eDefault;
    }

bool WRLDRecord::IsDungeonMusic()
    {
    if(!SNAM.IsLoaded()) return false;
    return (SNAM->flags == eDungeon);
    }

void WRLDRecord::IsDungeonMusic(bool value)
    {
    if(!SNAM.IsLoaded()) return;
    if(value)
        SNAM->flags = eDungeon;
    else if(IsDungeonMusic())
        SNAM->flags = eDefault;
    }

bool WRLDRecord::IsMusicType(UINT32 Type)
    {
    if(!SNAM.IsLoaded()) return false;
    return (SNAM->flags == Type);
    }

void WRLDRecord::SetMusicType(UINT32 Type)
    {
    SNAM.Load();
    SNAM->flags = Type;
    }

UINT32 WRLDRecord::GetSize(bool forceCalc)
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

    if(WNAM.IsLoaded())
        TotSize += WNAM.GetSize() + 6;

    if(CNAM.IsLoaded())
        TotSize += CNAM.GetSize() + 6;

    if(NAM2.IsLoaded())
        TotSize += NAM2.GetSize() + 6;

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(MNAM.IsLoaded())
        TotSize += MNAM.GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(NAM0.IsLoaded())
        TotSize += NAM0.GetSize() + 6;

    if(NAM9.IsLoaded())
        TotSize += NAM9.GetSize() + 6;

    if(SNAM.IsLoaded())
        TotSize += SNAM.GetSize() + 6;

    if(OFST.IsLoaded())
        {
        cSize = OFST.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    return TotSize;
    }

UINT32 WRLDRecord::GetType()
    {
    return 'DLRW';
    }

STRING WRLDRecord::GetStrType()
    {
    return "WRLD";
    }

SINT32 WRLDRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'MANW':
                WNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANC':
                CNAM.Read(buffer, subSize, curPos);
                break;
            case '2MAN':
                NAM2.Read(buffer, subSize, curPos);
                break;
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case 'MANM':
                MNAM.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case '0MAN':
                NAM0.Read(buffer, subSize, curPos);
                break;
            case '9MAN':
                NAM9.Read(buffer, subSize, curPos);
                break;
            case 'MANS':
                SNAM.Read(buffer, subSize, curPos);
                break;
            case 'TSFO':
                OFST.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  WRLD: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 WRLDRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    WNAM.Unload();
    CNAM.Unload();
    NAM2.Unload();
    ICON.Unload();
    MNAM.Unload();
    DATA.Unload();
    NAM0.Unload();
    NAM9.Unload();
    SNAM.Unload();
    OFST.Unload();
    return 1;
    }

SINT32 WRLDRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(WNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANW', WNAM.value, WNAM.GetSize());
    if(CNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANC', CNAM.value, CNAM.GetSize());
    if(NAM2.IsLoaded())
        SaveHandler.writeSubRecord('2MAN', NAM2.value, NAM2.GetSize());
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(MNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANM', MNAM.value, MNAM.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(NAM0.IsLoaded())
        SaveHandler.writeSubRecord('0MAN', &NAM0.value, NAM0.GetSize());
    if(NAM9.IsLoaded())
        SaveHandler.writeSubRecord('9MAN', &NAM9.value, NAM9.GetSize());
    if(SNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANS', SNAM.value, SNAM.GetSize());
    if(OFST.IsLoaded())
        SaveHandler.writeSubRecord('TSFO', OFST.value, OFST.GetSize());
    return -1;
    }

bool WRLDRecord::operator ==(const WRLDRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            WNAM == other.WNAM &&
            CNAM == other.CNAM &&
            NAM2 == other.NAM2 &&
            ICON.equalsi(other.ICON) &&
            MNAM == other.MNAM &&
            DATA == other.DATA &&
            NAM0 == other.NAM0 &&
            NAM9 == other.NAM9 &&
            SNAM == other.SNAM &&
            OFST == other.OFST);
    }

bool WRLDRecord::operator !=(const WRLDRecord &other) const
    {
    return !(*this == other);
    }