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
#include "CELLRecord.h"
#include <vector>

CELLRecord::CELLXCLL::CELLXCLL():
    fogNear(0),
    fogFar(0),
    directionalXY(0),
    directionalZ(0),
    directionalFade(1.0f),
    fogClip(0)
    {
    //
    }

CELLRecord::CELLXCLL::~CELLXCLL()
    {
    //
    }

bool CELLRecord::CELLXCLL::operator ==(const CELLXCLL &other) const
    {
    return (ambient == other.ambient &&
            directional == other.directional &&
            fog == other.fog &&
            AlmostEqual(fogNear,other.fogNear,2) &&
            AlmostEqual(fogFar,other.fogFar,2) &&
            directionalXY == other.directionalXY &&
            directionalZ == other.directionalZ &&
            AlmostEqual(directionalFade,other.directionalFade,2) &&
            AlmostEqual(fogClip,other.fogClip,2));
    }

bool CELLRecord::CELLXCLL::operator !=(const CELLXCLL &other) const
    {
    return !(*this == other);
    }

CELLRecord::CELLXCLW::CELLXCLW():
    waterHeight(-2147483648.0f)
    {
    //
    }

CELLRecord::CELLXCLW::~CELLXCLW()
    {
    //
    }

bool CELLRecord::CELLXCLW::operator ==(const CELLXCLW &other) const
    {
    return (AlmostEqual(waterHeight,other.waterHeight,2));
    }

bool CELLRecord::CELLXCLW::operator !=(const CELLXCLW &other) const
    {
    return !(*this == other);
    }

CELLRecord::CELLXCLC::CELLXCLC():
    posX(0),
    posY(0)
    {
    //
    }

CELLRecord::CELLXCLC::~CELLXCLC()
    {
    //
    }

bool CELLRecord::CELLXCLC::operator ==(const CELLXCLC &other) const
    {
    return (posX == other.posX &&
            posY == other.posY);
    }

bool CELLRecord::CELLXCLC::operator !=(const CELLXCLC &other) const
    {
    return !(*this == other);
    }

CELLRecord::CELLRecord(unsigned char *_recData):
    Record(_recData),
    PGRD(NULL),
    LAND(NULL),
    Parent(NULL)
    {
    //
    }

CELLRecord::CELLRecord(CELLRecord *srcRecord):
    Record(),
    PGRD(NULL),
    LAND(NULL),
    Parent(NULL)
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
    DATA = srcRecord->DATA;
    XCLL = srcRecord->XCLL;
    XCMT = srcRecord->XCMT;
    if(srcRecord->Ownership.IsLoaded())
        {
        Ownership.Load();
        Ownership->XOWN = srcRecord->Ownership->XOWN;
        Ownership->XRNK = srcRecord->Ownership->XRNK;
        Ownership->XGLB = srcRecord->Ownership->XGLB;
        }
    XCCM = srcRecord->XCCM;
    XCLW = srcRecord->XCLW;
    XCLR = srcRecord->XCLR;
    XCLC = srcRecord->XCLC;
    XCWT = srcRecord->XCWT;
    return;
    }

CELLRecord::~CELLRecord()
    {
    for(UINT32 x = 0; x < ACHR.size(); ++x)
        delete ACHR[x];
    for(UINT32 x = 0; x < ACRE.size(); ++x)
        delete ACRE[x];
    for(UINT32 x = 0; x < REFR.size(); ++x)
        delete REFR[x];
    delete PGRD;
    delete LAND;
    //Parent is a shared pointer that's deleted when the WRLD group is deleted
    }


bool CELLRecord::HasSubRecords()
    {
    return true;
    }

bool CELLRecord::VisitSubRecords(const UINT32 &RecordType, RecordOp &op)
    {
    bool stop;

    if(RecordType == NULL || RecordType == 'RHCA')
        for(UINT32 x = 0; x < ACHR.size();++x)
            {
            stop = op.Accept(ACHR[x]);
            if(ACHR[x] == NULL)
                {
                ACHR.erase(ACHR.begin() + x);
                --x;
                }
            if(stop)
                return stop;
            }

    if(RecordType == NULL || RecordType == 'ERCA')
        for(UINT32 x = 0; x < ACRE.size();++x)
            {
            stop = op.Accept(ACRE[x]);
            if(ACRE[x] == NULL)
                {
                ACRE.erase(ACRE.begin() + x);
                --x;
                }
            if(stop)
                return stop;
            }

    if(RecordType == NULL || RecordType == 'RFER')
        for(UINT32 x = 0; x < REFR.size();++x)
            {
            stop = op.Accept(REFR[x]);
            if(REFR[x] == NULL)
                {
                REFR.erase(REFR.begin() + x);
                --x;
                }
            if(stop)
                return stop;
            }

    if(RecordType == NULL || RecordType == 'DRGP')
        {
        if(PGRD != NULL)
            {
            if(op.Accept(PGRD))
                return true;
            }
        }

    if(RecordType == NULL || RecordType == 'DNAL')
        {
        if(LAND != NULL)
            {
            if(op.Accept(LAND))
                return true;
            }
        }

    return op.Stop();
    }

bool CELLRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(Ownership.IsLoaded())
        {
        if(Ownership->XOWN.IsLoaded())
            op.Accept(Ownership->XOWN.value.fid);
        if(Ownership->XGLB.IsLoaded())
            op.Accept(Ownership->XGLB->fid);
        }
    op.Accept(XCCM.value.fid);
    for(UINT32 x = 0; x < XCLR.size(); x++)
        op.Accept(XCLR[x]);
    op.Accept(XCWT.value.fid);

    return op.Stop();
    }

bool CELLRecord::IsInterior()
    {
    return (DATA.value.flags & fIsInterior) != 0;
    }

void CELLRecord::IsInterior(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsInterior) : (DATA.value.flags & ~fIsInterior);
    }

bool CELLRecord::IsHasWater()
    {
    return (DATA.value.flags & fHasWater) != 0;
    }

void CELLRecord::IsHasWater(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fHasWater) : (DATA.value.flags & ~fHasWater);
    }

bool CELLRecord::IsInvertFastTravel()
    {
    return (DATA.value.flags & fInvertFastTravel) != 0;
    }

void CELLRecord::IsInvertFastTravel(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fInvertFastTravel) : (DATA.value.flags & ~fInvertFastTravel);
    }

bool CELLRecord::IsForceHideLand()
    {
    return (DATA.value.flags & fForceHideLand) != 0;
    }

void CELLRecord::IsForceHideLand(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fForceHideLand) : (DATA.value.flags & ~fForceHideLand);
    }

bool CELLRecord::IsPublicPlace()
    {
    return (DATA.value.flags & fPublicPlace) != 0;
    }

void CELLRecord::IsPublicPlace(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fPublicPlace) : (DATA.value.flags & ~fPublicPlace);
    }

bool CELLRecord::IsHandChanged()
    {
    return (DATA.value.flags & fHandChanged) != 0;
    }

void CELLRecord::IsHandChanged(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fHandChanged) : (DATA.value.flags & ~fHandChanged);
    }

bool CELLRecord::IsBehaveLikeExterior()
    {
    return (DATA.value.flags & fBehaveLikeExterior) != 0;
    }

void CELLRecord::IsBehaveLikeExterior(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fBehaveLikeExterior) : (DATA.value.flags & ~fBehaveLikeExterior);
    }

bool CELLRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void CELLRecord::SetFlagMask(UINT8 Mask)
    {
    DATA.value.flags = Mask;
    }

bool CELLRecord::IsDefaultMusic()
    {
    XCMT.Load();
    return XCMT.IsLoaded() ? (XCMT.value.flags == eDefault) : false;
    }

void CELLRecord::IsDefaultMusic(bool value)
    {
    XCMT.Load();
    if(value)
        XCMT.value.flags = eDefault;
    else if(IsDefaultMusic())
        XCMT.value.flags = ePublic;
    }

bool CELLRecord::IsPublicMusic()
    {
    XCMT.Load();
    return XCMT.IsLoaded() ? (XCMT.value.flags == ePublic) : false;
    }

void CELLRecord::IsPublicMusic(bool value)
    {
    XCMT.Load();
    if(value)
        XCMT.value.flags = ePublic;
    else if(IsPublicMusic())
        XCMT.value.flags = eDefault;
    }

bool CELLRecord::IsDungeonMusic()
    {
    XCMT.Load();
    return XCMT.IsLoaded() ? (XCMT.value.flags == eDungeon) : false;
    }

void CELLRecord::IsDungeonMusic(bool value)
    {
    XCMT.Load();
    if(value)
        XCMT.value.flags = eDungeon;
    else if(IsDungeonMusic())
        XCMT.value.flags = eDefault;
    }

bool CELLRecord::IsMusicType(UINT8 Type)
    {
    XCMT.Load();
    return XCMT.IsLoaded() ? (XCMT.value.flags == Type) : false;
    }

void CELLRecord::SetMusicType(UINT8 Type)
    {
    XCMT.Load();
    XCMT.value.flags = Type;
    }

UINT32 CELLRecord::GetSize(bool forceCalc)
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

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(XCLL.IsLoaded())
        TotSize += XCLL.GetSize() + 6;

    if(XCMT.IsLoaded())
        TotSize += XCMT.GetSize() + 6;

    if(Ownership.IsLoaded() && Ownership->XOWN.IsLoaded())
        {
        TotSize += Ownership->XOWN.GetSize() + 6;

        if(Ownership->XRNK.IsLoaded())
            TotSize += Ownership->XRNK.GetSize() + 6;

        if(Ownership->XGLB.IsLoaded())
            TotSize += Ownership->XGLB.GetSize() + 6;
        }

    if(XCCM.IsLoaded())
        TotSize += XCCM.GetSize() + 6;

    if(XCLW.IsLoaded())
        TotSize += XCLW.GetSize() + 6;

    if(XCLR.size())
        {
        cSize = (sizeof(UINT32) * (UINT32)XCLR.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(XCLC.IsLoaded() && !IsInterior())
        TotSize += XCLC.GetSize() + 6;

    if(XCWT.IsLoaded())
        TotSize += XCWT.GetSize() + 6;

    return TotSize;
    }

UINT32 CELLRecord::GetType()
    {
    return 'LLEC';
    }

STRING CELLRecord::GetStrType()
    {
    return "CELL";
    }

UINT32 CELLRecord::GetParentType()
    {
    if(Parent != NULL)
        return Parent->GetType();
    return 0;
    }

SINT32 CELLRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'LLCX':
                XCLL.Read(buffer, subSize, curPos);
                break;
            case 'TMCX':
                XCMT.Read(buffer, subSize, curPos);
                break;
            case 'NWOX':
                Ownership.Load();
                Ownership->XOWN.Read(buffer, subSize, curPos);
                break;
            case 'KNRX':
                Ownership.Load();
                Ownership->XRNK.Read(buffer, subSize, curPos);
                break;
            case 'BLGX':
                Ownership.Load();
                Ownership->XGLB.Read(buffer, subSize, curPos);
                break;
            case 'MCCX':
                XCCM.Read(buffer, subSize, curPos);
                break;
            case 'WLCX':
                XCLW.Read(buffer, subSize, curPos);
                break;
            case 'RLCX':
                if(subSize % sizeof(UINT32) == 0)
                    {
                    if(subSize == 0)
                        break;
                    XCLR.resize(subSize / sizeof(UINT32));
                    _readBuffer(&XCLR[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized XCLR size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'CLCX':
                XCLC.Read(buffer, subSize, curPos);
                break;
            case 'TWCX':
                XCWT.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  CELL: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 CELLRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    DATA.Unload();
    XCLL.Unload();
    XCMT.Unload();
    Ownership.Unload();
    XCCM.Unload();
    XCLW.Unload();
    XCLR.clear();
    XCLC.Unload();
    XCWT.Unload();
    return 1;
    }

SINT32 CELLRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());

    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(XCLL.IsLoaded())
        SaveHandler.writeSubRecord('LLCX', XCLL.value, XCLL.GetSize());
    if(XCMT.IsLoaded())
        SaveHandler.writeSubRecord('TMCX', &XCMT.value, XCMT.GetSize());

    if(Ownership.IsLoaded() && Ownership->XOWN.IsLoaded())
        {
        SaveHandler.writeSubRecord('NWOX', &Ownership->XOWN.value, Ownership->XOWN.GetSize());
        if(Ownership->XRNK.IsLoaded())
            SaveHandler.writeSubRecord('KNRX', Ownership->XRNK.value, Ownership->XRNK.GetSize());
        if(Ownership->XGLB.IsLoaded())
            SaveHandler.writeSubRecord('BLGX', Ownership->XGLB.value, Ownership->XGLB.GetSize());
        }

    if(XCCM.IsLoaded())
        SaveHandler.writeSubRecord('MCCX', &XCCM.value, XCCM.GetSize());
    if(XCLW.IsLoaded())
        SaveHandler.writeSubRecord('WLCX', &XCLW.value, XCLW.GetSize());

    if(XCLR.size())
        SaveHandler.writeSubRecord('RLCX', &XCLR[0], (UINT32)XCLR.size() * sizeof(UINT32));
    //else
    //    SaveHandler.writeSubRecord('RLCX', NULL, 0);

    if(XCLC.IsLoaded() && !IsInterior())
        SaveHandler.writeSubRecord('CLCX', XCLC.value, XCLC.GetSize());
    if(XCWT.IsLoaded())
        SaveHandler.writeSubRecord('TWCX', &XCWT.value, XCWT.GetSize());
    return -1;
    }

bool CELLRecord::operator ==(const CELLRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        FULL.equals(other.FULL) &&
        DATA == other.DATA &&
        XCLL == other.XCLL &&
        XCMT == other.XCMT &&
        Ownership == other.Ownership &&
        XCCM == other.XCCM &&
        XCLW == other.XCLW &&
        XCLC == other.XCLC &&
        XCWT == other.XCWT &&
        XCLR.size() == other.XCLR.size())
        {
        //Order doesn't matter
        for(UINT32 x = 0; x < XCLR.size(); ++x)
        {
            bool good = false;
            for(UINT32 y = 0; y < XCLR.size(); ++y)
            {
                if (XCLR[x] == other.XCLR[y])
                {
                    // Found it's match
                    good = true;
                    break;
                }
            }
            if (!good) return false;
        }
        return true;
        }

    return false;
    }

bool CELLRecord::operator !=(const CELLRecord &other) const
    {
    return !(*this == other);
    }