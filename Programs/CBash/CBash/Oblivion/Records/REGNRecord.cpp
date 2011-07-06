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
#include "REGNRecord.h"
#include <vector>

REGNRecord::REGNRPLD::REGNRPLD():
    posX(0.0f),
    posY(0.0f)
    {
    //
    }

REGNRecord::REGNRPLD::~REGNRPLD()
    {
    //
    }

bool REGNRecord::REGNRPLD::operator ==(const REGNRPLD &other) const
    {
    return (AlmostEqual(posX,other.posX,2) &&
            AlmostEqual(posY,other.posY,2));
    }

bool REGNRecord::REGNRPLD::operator !=(const REGNRPLD &other) const
    {
    return !(*this == other);
    }

REGNRecord::REGNRPLI::REGNRPLI():
    edgeFalloff(0)
    {
    //
    }

REGNRecord::REGNRPLI::~REGNRPLI()
    {
    //
    }

bool REGNRecord::REGNRPLI::operator ==(const REGNRPLI &other) const
    {
    return (edgeFalloff == other.edgeFalloff);
    }

bool REGNRecord::REGNRPLI::operator !=(const REGNRPLI &other) const
    {
    return !(*this == other);
    }

bool REGNRecord::REGNArea::operator ==(const REGNArea &other) const
    {
    if(RPLI == other.RPLI &&
        RPLD.size() == other.RPLD.size())
        {
        //Not sure if record order matters on areas, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < RPLD.size(); ++x)
            if(RPLD[x] != other.RPLD[x])
                return false;
        return true;
        }

    return false;
    }

bool REGNRecord::REGNArea::operator !=(const REGNArea &other) const
    {
    return !(*this == other);
    }

REGNRecord::REGNRDWT::REGNRDWT():
    weather(0),
    chance(0)
    {
    //
    }

REGNRecord::REGNRDWT::~REGNRDWT()
    {
    //
    }

bool REGNRecord::REGNRDWT::operator ==(const REGNRDWT &other) const
    {
    return (weather == other.weather &&
            chance == other.chance);
    }

bool REGNRecord::REGNRDWT::operator !=(const REGNRDWT &other) const
    {
    return !(*this == other);
    }

REGNRecord::REGNRDSD::REGNRDSD():
    sound(0),
    flags(0),
    chance(0)
    {
    //
    }

REGNRecord::REGNRDSD::~REGNRDSD()
    {
    //
    }

bool REGNRecord::REGNRDSD::IsPleasant()
    {
    return (flags & fIsPleasant) != 0;
    }

void REGNRecord::REGNRDSD::IsPleasant(bool value)
    {
    flags = value ? (flags | fIsPleasant) : (flags & ~fIsPleasant);
    }

bool REGNRecord::REGNRDSD::IsCloudy()
    {
    return (flags & fIsCloudy) != 0;
    }

void REGNRecord::REGNRDSD::IsCloudy(bool value)
    {
    flags = value ? (flags | fIsCloudy) : (flags & ~fIsCloudy);
    }

bool REGNRecord::REGNRDSD::IsRainy()
    {
    return (flags & fIsRainy) != 0;
    }

void REGNRecord::REGNRDSD::IsRainy(bool value)
    {
    flags = value ? (flags | fIsRainy) : (flags & ~fIsRainy);
    }

bool REGNRecord::REGNRDSD::IsSnowy()
    {
    return (flags & fIsSnowy) != 0;
    }

void REGNRecord::REGNRDSD::IsSnowy(bool value)
    {
    flags = value ? (flags | fIsSnowy) : (flags & ~fIsSnowy);
    }

bool REGNRecord::REGNRDSD::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((flags & Mask) == Mask) : ((flags & Mask) != 0);
    }

void REGNRecord::REGNRDSD::SetFlagMask(UINT32 Mask)
    {
    flags = Mask;
    }

bool REGNRecord::REGNRDSD::operator ==(const REGNRDSD &other) const
    {
    return (sound == other.sound &&
            flags == other.flags &&
            flags == other.flags);
    }

bool REGNRecord::REGNRDSD::operator !=(const REGNRDSD &other) const
    {
    return !(*this == other);
    }

REGNRecord::REGNRDMD::REGNRDMD():
    type(0)
    {
    //
    }

REGNRecord::REGNRDMD::~REGNRDMD()
    {
    //
    }

bool REGNRecord::REGNRDMD::operator ==(const REGNRDMD &other) const
    {
    return (type == other.type);
    }

bool REGNRecord::REGNRDMD::operator !=(const REGNRDMD &other) const
    {
    return !(*this == other);
    }

REGNRecord::REGNRDGS::REGNRDGS():
    grass(0)
    {
    memset(&unk1, 0x00, 4);
    }

REGNRecord::REGNRDGS::~REGNRDGS()
    {
    //
    }

bool REGNRecord::REGNRDGS::operator ==(const REGNRDGS &other) const
    {
    return (grass == other.grass);
    }

bool REGNRecord::REGNRDGS::operator !=(const REGNRDGS &other) const
    {
    return !(*this == other);
    }

REGNRecord::REGNRDOT::REGNRDOT():
    objectId(0),
    parentIndex(0),
    density(0.0f),
    clustering(0),
    minSlope(0),
    maxSlope(0),
    flags(0),
    radiusWRTParent(0),
    radius(0),
    maxHeight(0.0f),
    sink(0.0f),
    sinkVar(0.0f),
    sizeVar(0.0f),
    angleVarX(0),
    angleVarY(0),
    angleVarZ(0)
    {
    memset(&unused1, 0x00, 2);
    memset(&unk1, 0x00, 4);
    memset(&unused2, 0x00, 2);
    memset(&unk2, 0x00, 4);
    }

REGNRecord::REGNRDOT::~REGNRDOT()
    {
    //
    }

bool REGNRecord::REGNRDOT::IsConformToSlope()
    {
    return (flags & fIsConformToSlope) != 0;
    }

void REGNRecord::REGNRDOT::IsConformToSlope(bool value)
    {
    flags = value ? (flags | fIsConformToSlope) : (flags & ~fIsConformToSlope);
    }

bool REGNRecord::REGNRDOT::IsPaintVertices()
    {
    return (flags & fIsPaintVertices) != 0;
    }

void REGNRecord::REGNRDOT::IsPaintVertices(bool value)
    {
    flags = value ? (flags | fIsPaintVertices) : (flags & ~fIsPaintVertices);
    }

bool REGNRecord::REGNRDOT::IsSizeVariance()
    {
    return (flags & fIsSizeVariance) != 0;
    }

void REGNRecord::REGNRDOT::IsSizeVariance(bool value)
    {
    flags = value ? (flags | fIsSizeVariance) : (flags & ~fIsSizeVariance);
    }

bool REGNRecord::REGNRDOT::IsXVariance()
    {
    return (flags & fIsXVariance) != 0;
    }

void REGNRecord::REGNRDOT::IsXVariance(bool value)
    {
    flags = value ? (flags | fIsXVariance) : (flags & ~fIsXVariance);
    }

bool REGNRecord::REGNRDOT::IsYVariance()
    {
    return (flags & fIsYVariance) != 0;
    }

void REGNRecord::REGNRDOT::IsYVariance(bool value)
    {
    flags = value ? (flags | fIsYVariance) : (flags & ~fIsYVariance);
    }

bool REGNRecord::REGNRDOT::IsZVariance()
    {
    return (flags & fIsZVariance) != 0;
    }

void REGNRecord::REGNRDOT::IsZVariance(bool value)
    {
    flags = value ? (flags | fIsZVariance) : (flags & ~fIsZVariance);
    }

bool REGNRecord::REGNRDOT::IsTree()
    {
    return (flags & fIsTree) != 0;
    }

void REGNRecord::REGNRDOT::IsTree(bool value)
    {
    flags = value ? (flags | fIsTree) : (flags & ~fIsTree);
    }

bool REGNRecord::REGNRDOT::IsHugeRock()
    {
    return (flags & fIsHugeRock) != 0;
    }

void REGNRecord::REGNRDOT::IsHugeRock(bool value)
    {
    flags = value ? (flags | fIsHugeRock) : (flags & ~fIsHugeRock);
    }

bool REGNRecord::REGNRDOT::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((flags & Mask) == Mask) : ((flags & Mask) != 0);
    }

void REGNRecord::REGNRDOT::SetFlagMask(UINT8 Mask)
    {
    flags = Mask;
    }

bool REGNRecord::REGNRDOT::operator ==(const REGNRDOT &other) const
    {
    return (objectId == other.objectId &&
            parentIndex == other.parentIndex &&
            AlmostEqual(density,other.density,2) &&
            clustering == other.clustering &&
            minSlope == other.minSlope &&
            maxSlope == other.maxSlope &&
            flags == other.flags &&
            radiusWRTParent == other.radiusWRTParent &&
            radius == other.radius &&
            AlmostEqual(maxHeight,other.maxHeight,2) &&
            AlmostEqual(sink,other.sink,2) &&
            AlmostEqual(sinkVar,other.sinkVar,2) &&
            AlmostEqual(sizeVar,other.sizeVar,2) &&
            angleVarX == other.angleVarX &&
            angleVarY == other.angleVarY &&
            angleVarZ == other.angleVarZ);
    }

bool REGNRecord::REGNRDOT::operator !=(const REGNRDOT &other) const
    {
    return !(*this == other);
    }

REGNRecord::REGNRDAT::REGNRDAT():
    entryType(0),
    flags(0),
    priority(0)
    {
    memset(&unused1, 0x00, 2);
    }

REGNRecord::REGNRDAT::~REGNRDAT()
    {
    //
    }

bool REGNRecord::REGNRDAT::operator ==(const REGNRDAT &other) const
    {
    return (entryType == other.entryType &&
            flags == other.flags &&
            priority == other.priority);
    }

bool REGNRecord::REGNRDAT::operator !=(const REGNRDAT &other) const
    {
    return !(*this == other);
    }

bool REGNRecord::REGNEntry::IsOverride()
    {
    return (RDAT.value.flags & fIsOverride) != 0;
    }

void REGNRecord::REGNEntry::IsOverride(bool value)
    {
    RDAT.value.flags = value ? (RDAT.value.flags | fIsOverride) : (RDAT.value.flags & ~fIsOverride);
    }

bool REGNRecord::REGNEntry::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((RDAT.value.flags & Mask) == Mask) : ((RDAT.value.flags & Mask) != 0);
    }

void REGNRecord::REGNEntry::SetFlagMask(UINT8 Mask)
    {
    RDAT.value.flags = Mask;
    }

bool REGNRecord::REGNEntry::IsObject()
    {
    return (RDAT.value.entryType == eObject);
    }

void REGNRecord::REGNEntry::IsObject(bool value)
    {
    if(value)
        RDAT.value.entryType = eObject;
    else if(IsObject())
        RDAT.value.entryType = eWeather;
    }

bool REGNRecord::REGNEntry::IsWeather()
    {
    return (RDAT.value.entryType == eWeather);
    }

void REGNRecord::REGNEntry::IsWeather(bool value)
    {
    if(value)
        RDAT.value.entryType = eWeather;
    else if(IsWeather())
        RDAT.value.entryType = eObject;
    }

bool REGNRecord::REGNEntry::IsMap()
    {
    return (RDAT.value.entryType == eMap);
    }

void REGNRecord::REGNEntry::IsMap(bool value)
    {
    if(value)
        RDAT.value.entryType = eMap;
    else if(IsMap())
        RDAT.value.entryType = eObject;
    }

bool REGNRecord::REGNEntry::IsIcon()
    {
    return (RDAT.value.entryType == eUnkIcon);
    }

void REGNRecord::REGNEntry::IsIcon(bool value)
    {
    if(value)
        RDAT.value.entryType = eUnkIcon;
    else if(IsIcon())
        RDAT.value.entryType = eObject;
    }

bool REGNRecord::REGNEntry::IsGrass()
    {
    return (RDAT.value.entryType == eGrass);
    }

void REGNRecord::REGNEntry::IsGrass(bool value)
    {
    if(value)
        RDAT.value.entryType = eGrass;
    else if(IsGrass())
        RDAT.value.entryType = eObject;
    }

bool REGNRecord::REGNEntry::IsSound()
    {
    return (RDAT.value.entryType == eSound);
    }

void REGNRecord::REGNEntry::IsSound(bool value)
    {
    if(value)
        RDAT.value.entryType = eSound;
    else if(IsSound())
        RDAT.value.entryType = eObject;
    }

bool REGNRecord::REGNEntry::IsType(UINT32 Type)
    {
    return (RDAT.value.entryType == Type);
    }

void REGNRecord::REGNEntry::SetType(UINT32 Type)
    {
    RDAT.value.entryType = Type;
    }

bool REGNRecord::REGNEntry::IsDefaultMusic()
    {
    return RDMD.IsLoaded() ? (RDMD->type == eDefault) : false;
    }

void REGNRecord::REGNEntry::IsDefaultMusic(bool value)
    {
    if(!RDMD.IsLoaded())
        return;

    if(value)
        RDMD->type = eDefault;
    else if(IsDefaultMusic())
        RDMD->type = ePublic;
    }

bool REGNRecord::REGNEntry::IsPublicMusic()
    {
    return RDMD.IsLoaded() ? (RDMD->type == ePublic) : false;
    }

void REGNRecord::REGNEntry::IsPublicMusic(bool value)
    {
    if(!RDMD.IsLoaded())
        return;

    if(value)
        RDMD->type = ePublic;
    else if(IsPublicMusic())
        RDMD->type = eDefault;
    }

bool REGNRecord::REGNEntry::IsDungeonMusic()
    {
    return RDMD.IsLoaded() ? (RDMD->type == eDungeon) : false;
    }

void REGNRecord::REGNEntry::IsDungeonMusic(bool value)
    {
    if(!RDMD.IsLoaded())
        return;

    if(value)
        RDMD->type = eDungeon;
    else if(IsDungeonMusic())
        RDMD->type = eDefault;
    }

bool REGNRecord::REGNEntry::IsMusicType(UINT32 Type)
    {
    return RDMD.IsLoaded() ? (RDMD->type == Type) : false;
    }

void REGNRecord::REGNEntry::SetMusicType(UINT32 Type)
    {
    RDMD.Load();
    RDMD->type = Type;
    }

bool REGNRecord::REGNEntry::operator ==(const REGNEntry &other) const
    {
    if(RDAT == other.RDAT &&
        RDMP.equals(other.RDMP) &&
        ICON.equalsi(other.ICON) &&
        RDMD == other.RDMD &&

        RDOT.size() == other.RDOT.size() &&
        RDGS.size() == other.RDGS.size() &&
        RDSD.size() == other.RDSD.size() &&
        RDWT.size() == other.RDWT.size())
        {
        //Not sure if record order matters on objects, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < RDOT.size(); ++x)
            if(RDOT[x] != other.RDOT[x])
                return false;

        //Not sure if record order matters on grasses, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < RDGS.size(); ++x)
            if(RDGS[x] != other.RDGS[x])
                return false;

        //Not sure if record order matters on sounds, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < RDSD.size(); ++x)
            if(RDSD[x] != other.RDSD[x])
                return false;

        //Not sure if record order matters on weathers, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < RDWT.size(); ++x)
            if(RDWT[x] != other.RDWT[x])
                return false;

        return true;
        }

    return false;
    }

bool REGNRecord::REGNEntry::operator !=(const REGNEntry &other) const
    {
    return !(*this == other);
    }

REGNRecord::REGNRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

REGNRecord::REGNRecord(REGNRecord *srcRecord):
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
    ICON = srcRecord->ICON;
    RCLR = srcRecord->RCLR;
    WNAM = srcRecord->WNAM;

    Areas.clear();
    Areas.resize(srcRecord->Areas.size());
    for(UINT32 x = 0; x < srcRecord->Areas.size(); x++)
        {
        Areas[x] = new REGNArea;
        Areas[x]->RPLI = srcRecord->Areas[x]->RPLI;
        Areas[x]->RPLD = srcRecord->Areas[x]->RPLD;
        }

    Entries.clear();
    Entries.resize(srcRecord->Entries.size());
    for(UINT32 x = 0; x < srcRecord->Entries.size(); x++)
        {
        Entries[x] = new REGNEntry;
        Entries[x]->RDAT = srcRecord->Entries[x]->RDAT;
        Entries[x]->RDOT = srcRecord->Entries[x]->RDOT;
        Entries[x]->RDMP = srcRecord->Entries[x]->RDMP;
        Entries[x]->ICON = srcRecord->Entries[x]->ICON;
        Entries[x]->RDGS = srcRecord->Entries[x]->RDGS;
        Entries[x]->RDMD = srcRecord->Entries[x]->RDMD;
        Entries[x]->RDSD = srcRecord->Entries[x]->RDSD;
        Entries[x]->RDWT = srcRecord->Entries[x]->RDWT;
        }
    return;
    }

REGNRecord::~REGNRecord()
    {
    for(UINT32 x = 0; x < Areas.size(); x++)
        delete Areas[x];
    for(UINT32 x = 0; x < Entries.size(); x++)
        delete Entries[x];
    }

bool REGNRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(WNAM.IsLoaded())
        op.Accept(WNAM.value.fid);
    for(UINT32 x = 0; x < Entries.size(); x++)
        {
        for(UINT32 y = 0; y < Entries[x]->RDOT.size(); y++)
            op.Accept(Entries[x]->RDOT[y].objectId);
        for(UINT32 y = 0; y < Entries[x]->RDGS.size(); y++)
            op.Accept(Entries[x]->RDGS[y].grass);
        for(UINT32 y = 0; y < Entries[x]->RDSD.size(); y++)
            op.Accept(Entries[x]->RDSD[y].sound);
        for(UINT32 y = 0; y < Entries[x]->RDWT.size(); y++)
            op.Accept(Entries[x]->RDWT[y].weather);
        }

    return op.Stop();
    }

UINT32 REGNRecord::GetSize(bool forceCalc)
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

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(RCLR.IsLoaded())
        TotSize += RCLR.GetSize() + 6;

    if(WNAM.IsLoaded())
        TotSize += WNAM.GetSize() + 6;

    for(UINT32 p = 0; p < Areas.size(); p++)
        {
        if(Areas[p]->RPLI.IsLoaded())
            TotSize += Areas[p]->RPLI.GetSize() + 6;
        if(Areas[p]->RPLD.size())
            {
            cSize = (sizeof(REGNRPLD) * (UINT32)Areas[p]->RPLD.size());
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    for(UINT32 p = 0; p < Entries.size(); p++)
        {
        if(Entries[p]->RDAT.IsLoaded())
            TotSize += Entries[p]->RDAT.GetSize() + 6;

        switch(Entries[p]->RDAT.value.entryType)
            {
            case eREGNObjects:
                TotSize += 6; //RDOT written even if empty
                if(Entries[p]->RDOT.size())
                    {
                    cSize = (sizeof(REGNRDOT) * (UINT32)Entries[p]->RDOT.size());
                    if(cSize > 65535) cSize += 10;
                    TotSize += cSize;
                    }
                break;
            case eREGNWeathers:
                if(Entries[p]->RDWT.size())
                    {
                    cSize = (sizeof(REGNRDWT) * (UINT32)Entries[p]->RDWT.size());
                    if(cSize > 65535) cSize += 10;
                    TotSize += cSize += 6;
                    }
                break;
            case eREGNMap:
                if(Entries[p]->RDMP.IsLoaded())
                    {
                    cSize = Entries[p]->RDMP.GetSize();
                    if(cSize > 65535) cSize += 10;
                    TotSize += cSize += 6;
                    }
                break;
            case eREGNIcon:
                if(Entries[p]->ICON.IsLoaded())
                    {
                    cSize = Entries[p]->ICON.GetSize();
                    if(cSize > 65535) cSize += 10;
                    TotSize += cSize += 6;
                    }
                break;
            case eREGNGrasses:
                if(Entries[p]->RDGS.size())
                    {
                    cSize = (sizeof(REGNRDGS) * (UINT32)Entries[p]->RDGS.size());
                    if(cSize > 65535) cSize += 10;
                    TotSize += cSize += 6;
                    }
                break;
            case eREGNSounds:
                if(Entries[p]->RDMD.IsLoaded())
                    TotSize += Entries[p]->RDMD.GetSize() + 6;
                TotSize += 6; //RDSD written even if empty
                if(Entries[p]->RDSD.size())
                    {
                    cSize = (sizeof(REGNRDSD) * (UINT32)Entries[p]->RDSD.size());
                    if(cSize > 65535) cSize += 10;
                    TotSize += cSize;
                    }
                break;
            default:
                printf("!!!%08X: Unknown REGN Entry type: %i, Index:%i!!!\n", formID, Entries[p]->RDAT.value.entryType, p);
                break;
            }
        }

    return TotSize;
    }

UINT32 REGNRecord::GetType()
    {
    return 'NGER';
    }


STRING REGNRecord::GetStrType()
    {
    return "REGN";
    }

SINT32 REGNRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    REGNArea *newArea = NULL;
    REGNEntry *newEntry = NULL;
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
            case 'NOCI':
                if(newEntry == NULL)
                    ICON.Read(buffer, subSize, curPos);
                else
                    newEntry->ICON.Read(buffer, subSize, curPos);
                break;
            case 'RLCR':
                RCLR.Read(buffer, subSize, curPos);
                break;
            case 'MANW':
                WNAM.Read(buffer, subSize, curPos);
                break;
            case 'ILPR':
                newArea = new REGNArea;
                newArea->RPLI.Read(buffer, subSize, curPos);
                Areas.push_back(newArea);
                break;
            case 'DLPR':
                if(subSize % sizeof(REGNRPLD) == 0)
                    {
                    if(subSize == 0)
                        break;
                    if(newArea == NULL)
                        {
                        newArea = new REGNArea;
                        Areas.push_back(newArea);
                        }
                    newArea->RPLD.resize(subSize / sizeof(REGNRPLD));
                    _readBuffer(&newArea->RPLD[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized RPLD size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'TADR':
                newEntry = new REGNEntry;
                newEntry->RDAT.Read(buffer, subSize, curPos);
                Entries.push_back(newEntry);
                break;
            case 'TODR':
                if(subSize % sizeof(REGNRDOT) == 0)
                    {
                    if(subSize == 0)
                        break;
                    if(newEntry == NULL)
                        {
                        newEntry = new REGNEntry;
                        Entries.push_back(newEntry);
                        }
                    newEntry->RDOT.resize(subSize / sizeof(REGNRDOT));
                    _readBuffer(&newEntry->RDOT[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized RDOT size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'PMDR':
                if(newEntry == NULL)
                    {
                    newEntry = new REGNEntry;
                    Entries.push_back(newEntry);
                    }
                newEntry->RDMP.Read(buffer, subSize, curPos);
                break;
            case 'SGDR':
                if(subSize % sizeof(REGNRDGS) == 0)
                    {
                    if(subSize == 0)
                        break;
                    if(newEntry == NULL)
                        {
                        newEntry = new REGNEntry;
                        Entries.push_back(newEntry);
                        }
                    newEntry->RDGS.resize(subSize / sizeof(REGNRDGS));
                    _readBuffer(&newEntry->RDGS[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized RDOT size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'DMDR':
                if(newEntry == NULL)
                    {
                    newEntry = new REGNEntry;
                    Entries.push_back(newEntry);
                    }
                newEntry->RDMD.Read(buffer, subSize, curPos);
                break;
            case 'DSDR':
                if(subSize % sizeof(REGNRDSD) == 0)
                    {
                    if(subSize == 0)
                        break;
                    if(newEntry == NULL)
                        {
                        newEntry = new REGNEntry;
                        Entries.push_back(newEntry);
                        }
                    newEntry->RDSD.resize(subSize / sizeof(REGNRDSD));
                    _readBuffer(&newEntry->RDSD[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized RDSD size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'TWDR':
                if(subSize % sizeof(REGNRDWT) == 0)
                    {
                    if(subSize == 0)
                        break;
                    if(newEntry == NULL)
                        {
                        newEntry = new REGNEntry;
                        Entries.push_back(newEntry);
                        }
                    newEntry->RDWT.resize(subSize / sizeof(REGNRDWT));
                    _readBuffer(&newEntry->RDWT[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized RDWT size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  REGN: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 REGNRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    ICON.Unload();
    RCLR.Unload();
    WNAM.Unload();
    for(UINT32 x = 0; x < Areas.size(); x++)
        delete Areas[x];
    Areas.clear();
    for(UINT32 x = 0; x < Entries.size(); x++)
        delete Entries[x];
    Entries.clear();
    return 1;
    }

SINT32 REGNRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(RCLR.IsLoaded())
        SaveHandler.writeSubRecord('RLCR', &RCLR.value, RCLR.GetSize());
    if(WNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANW', &WNAM.value, WNAM.GetSize());
    if(Areas.size())
        for(UINT32 p = 0; p < Areas.size(); p++)
            {
            if(Areas[p]->RPLI.IsLoaded())
                SaveHandler.writeSubRecord('ILPR', &Areas[p]->RPLI.value, Areas[p]->RPLI.GetSize());
            if(Areas[p]->RPLD.size())
                SaveHandler.writeSubRecord('DLPR', &Areas[p]->RPLD[0], (UINT32)Areas[p]->RPLD.size() * sizeof(REGNRPLD));
            //else
            //    SaveHandler.writeSubRecord('DLPR', NULL, 0);
            }
    if(Entries.size())
        for(UINT32 p = 0; p < Entries.size(); p++)
            {
            if(Entries[p]->RDAT.IsLoaded())
                SaveHandler.writeSubRecord('TADR', &Entries[p]->RDAT.value, Entries[p]->RDAT.GetSize());
            switch(Entries[p]->RDAT.value.entryType)
                {
                case eREGNObjects:
                    if(Entries[p]->RDOT.size())
                        SaveHandler.writeSubRecord('TODR', &Entries[p]->RDOT[0], (UINT32)Entries[p]->RDOT.size() * sizeof(REGNRDOT));
                    else
                        SaveHandler.writeSubRecord('TODR', NULL, 0);
                    break;
                case eREGNWeathers:
                    if(Entries[p]->RDWT.size())
                        SaveHandler.writeSubRecord('TWDR', &Entries[p]->RDWT[0], (UINT32)Entries[p]->RDWT.size() * sizeof(REGNRDWT));
                    //else
                    //    SaveHandler.writeSubRecord('TWDR', NULL, 0);
                    break;
                case eREGNMap:
                    if(Entries[p]->RDMP.IsLoaded())
                        SaveHandler.writeSubRecord('PMDR', Entries[p]->RDMP.value, Entries[p]->RDMP.GetSize());
                    break;
                case eREGNIcon:
                    if(Entries[p]->ICON.IsLoaded())
                        SaveHandler.writeSubRecord('NOCI', Entries[p]->ICON.value, Entries[p]->ICON.GetSize());
                    break;
                case eREGNGrasses:
                    if(Entries[p]->RDGS.size())
                        SaveHandler.writeSubRecord('SGDR', &Entries[p]->RDGS[0], (UINT32)Entries[p]->RDGS.size() * sizeof(REGNRDGS));
                    //else
                    //    SaveHandler.writeSubRecord('SGDR', NULL, 0);
                    break;
                case eREGNSounds:
                    if(Entries[p]->RDMD.IsLoaded())
                        SaveHandler.writeSubRecord('DMDR', Entries[p]->RDMD.value, Entries[p]->RDMD.GetSize());
                    if(Entries[p]->RDSD.size())
                        SaveHandler.writeSubRecord('DSDR', &Entries[p]->RDSD[0], (UINT32)Entries[p]->RDSD.size() * sizeof(REGNRDSD));
                    else
                        SaveHandler.writeSubRecord('DSDR', NULL, 0);
                    break;
                default:
                    printf("!!!%08X: Unknown REGN Entry type: %i, Index:%i!!!\n", formID, Entries[p]->RDAT.value.entryType, p);
                    break;
                }
            }
    return -1;
    }

bool REGNRecord::operator ==(const REGNRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        ICON.equalsi(other.ICON) &&
        RCLR == other.RCLR &&
        WNAM == other.WNAM &&
        Areas.size() == other.Areas.size() &&
        Entries.size() == other.Entries.size())
        {
        //Not sure if record order matters on areas, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < Areas.size(); ++x)
            if(*Areas[x] != *other.Areas[x])
                return false;

        //Not sure if record order matters on entries, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < Entries.size(); ++x)
            if(*Entries[x] != *other.Entries[x])
                return false;
        return true;
        }

    return false;
    }

bool REGNRecord::operator !=(const REGNRecord &other) const
    {
    return !(*this == other);
    }