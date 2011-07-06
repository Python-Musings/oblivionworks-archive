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
#include "PACKRecord.h"
#include <vector>

PACKRecord::PACKPKDT::PACKPKDT():
    flags(0),
    aiType(0)
    {
    memset(&unused1, 0x00, 3);
    }

PACKRecord::PACKPKDT::~PACKPKDT()
    {
    //
    }

bool PACKRecord::PACKPKDT::operator ==(const PACKPKDT &other) const
    {
    return (flags == other.flags &&
            aiType == other.aiType);
    }

bool PACKRecord::PACKPKDT::operator !=(const PACKPKDT &other) const
    {
    return !(*this == other);
    }

PACKRecord::PACKPLDT::PACKPLDT():
    locType(0),
    locId(0),
    locRadius(0)
    {
    //
    }

PACKRecord::PACKPLDT::~PACKPLDT()
    {
    //
    }

bool PACKRecord::PACKPLDT::operator ==(const PACKPLDT &other) const
    {
    return (locType == other.locType &&
            locId == other.locId &&
            locRadius == other.locRadius);
    }

bool PACKRecord::PACKPLDT::operator !=(const PACKPLDT &other) const
    {
    return !(*this == other);
    }

PACKRecord::PACKPSDT::PACKPSDT():
    month(0),
    day(0),
    date(0),
    time(0),
    duration(0)
    {
    //
    }

PACKRecord::PACKPSDT::~PACKPSDT()
    {
    //
    }

bool PACKRecord::PACKPSDT::operator ==(const PACKPSDT &other) const
    {
    return (month == other.month &&
            day == other.day &&
            date == other.date &&
            time == other.time &&
            duration == other.duration);
    }

bool PACKRecord::PACKPSDT::operator !=(const PACKPSDT &other) const
    {
    return !(*this == other);
    }

PACKRecord::PACKPTDT::PACKPTDT():
    targetType(0),
    targetId(0),
    targetCount(0)
    {
    //
    }

PACKRecord::PACKPTDT::~PACKPTDT()
    {
    //
    }

bool PACKRecord::PACKPTDT::operator ==(const PACKPTDT &other) const
    {
    return (targetType == other.targetType &&
            targetId == other.targetId &&
            targetCount == other.targetCount);
    }
bool PACKRecord::PACKPTDT::operator !=(const PACKPTDT &other) const
    {
    return !(*this == other);
    }

PACKRecord::PACKRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

bool PACKRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    FunctionArguments CTDAFunction;
    Function_Arguments_Iterator curCTDAFunction;

    if(PLDT.IsLoaded() && PLDT->locType != 5)
        op.Accept(PLDT->locId);

    if(PTDT.IsLoaded() && PTDT->targetType != 2)
        op.Accept(PTDT->targetId);

    for(UINT32 x = 0; x < CTDA.size(); x++)
        {
        //if(CTDA[x]->value.ifunc == 214)
        //    printf("%08X uses HasMagicEffect\n", formID);
        curCTDAFunction = Function_Arguments.find(CTDA[x]->value.ifunc);
        if(curCTDAFunction != Function_Arguments.end())
            {
            CTDAFunction = curCTDAFunction->second;
            if(CTDAFunction.first == eFORMID)
                op.Accept(CTDA[x]->value.param1);
            if(CTDAFunction.second == eFORMID)
                op.Accept(CTDA[x]->value.param2);
            }
        else
            printf("Warning: PACKRecord %08X uses an unknown function (%d)!\n", formID, CTDA[x]->value.ifunc);
        }

    return op.Stop();
    }

bool PACKRecord::IsAIFind()
    {
    return (PKDT.value.aiType == eAIFind);
    }

void PACKRecord::IsAIFind(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIFind;
    else if(IsAIFind())
        PKDT.value.aiType = eAIFollow;
    }

bool PACKRecord::IsAIFollow()
    {
    return (PKDT.value.aiType == eAIFollow);
    }

void PACKRecord::IsAIFollow(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIFollow;
    else if(IsAIFollow())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAIEscort()
    {
    return (PKDT.value.aiType == eAIEscort);
    }

void PACKRecord::IsAIEscort(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIEscort;
    else if(IsAIEscort())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAIEat()
    {
    return (PKDT.value.aiType == eAIEat);
    }

void PACKRecord::IsAIEat(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIEat;
    else if(IsAIEat())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAISleep()
    {
    return (PKDT.value.aiType == eAISleep);
    }

void PACKRecord::IsAISleep(bool value)
    {
    if(value)
        PKDT.value.aiType = eAISleep;
    else if(IsAISleep())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAIWander()
    {
    return (PKDT.value.aiType == eAIWander);
    }

void PACKRecord::IsAIWander(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIWander;
    else if(IsAIWander())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAITravel()
    {
    return (PKDT.value.aiType == eAITravel);
    }

void PACKRecord::IsAITravel(bool value)
    {
    if(value)
        PKDT.value.aiType = eAITravel;
    else if(IsAIFollow())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAIAccompany()
    {
    return (PKDT.value.aiType == eAIAccompany);
    }

void PACKRecord::IsAIAccompany(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIAccompany;
    else if(IsAIAccompany())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAIUseItemAt()
    {
    return (PKDT.value.aiType == eAIUseItemAt);
    }

void PACKRecord::IsAIUseItemAt(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIUseItemAt;
    else if(IsAIUseItemAt())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAIAmbush()
    {
    return (PKDT.value.aiType == eAIAmbush);
    }

void PACKRecord::IsAIAmbush(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIAmbush;
    else if(IsAIAmbush())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAIFleeNotCombat()
    {
    return (PKDT.value.aiType == eAIFleeNotCombat);
    }

void PACKRecord::IsAIFleeNotCombat(bool value)
    {
    if(value)
        PKDT.value.aiType = eAIFleeNotCombat;
    else if(IsAIFleeNotCombat())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAICastMagic()
    {
    return (PKDT.value.aiType == eAICastMagic);
    }

void PACKRecord::IsAICastMagic(bool value)
    {
    if(value)
        PKDT.value.aiType = eAICastMagic;
    else if(IsAICastMagic())
        PKDT.value.aiType = eAIFind;
    }

bool PACKRecord::IsAIType(UINT8 Type)
    {
    return (PKDT.value.aiType == Type);
    }

void PACKRecord::SetAIType(UINT8 Type)
    {
    PKDT.value.aiType = Type;
    }

bool PACKRecord::IsOffersServices()
    {
    return (PKDT.value.flags & fIsOffersServices) != 0;
    }

void PACKRecord::IsOffersServices(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsOffersServices) : (PKDT.value.flags & ~fIsOffersServices);
    }

bool PACKRecord::IsMustReachLocation()
    {
    return (PKDT.value.flags & fIsMustReachLocation) != 0;
    }

void PACKRecord::IsMustReachLocation(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsMustReachLocation) : (PKDT.value.flags & ~fIsMustReachLocation);
    }

bool PACKRecord::IsMustComplete()
    {
    return (PKDT.value.flags & fIsMustComplete) != 0;
    }

void PACKRecord::IsMustComplete(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsMustComplete) : (PKDT.value.flags & ~fIsMustComplete);
    }

bool PACKRecord::IsLockAtStart()
    {
    return (PKDT.value.flags & fIsLockAtStart) != 0;
    }

void PACKRecord::IsLockAtStart(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsLockAtStart) : (PKDT.value.flags & ~fIsLockAtStart);
    }

bool PACKRecord::IsLockAtEnd()
    {
    return (PKDT.value.flags & fIsLockAtEnd) != 0;
    }

void PACKRecord::IsLockAtEnd(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsLockAtEnd) : (PKDT.value.flags & ~fIsLockAtEnd);
    }

bool PACKRecord::IsLockAtLocation()
    {
    return (PKDT.value.flags & fIsLockAtLocation) != 0;
    }

void PACKRecord::IsLockAtLocation(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsLockAtLocation) : (PKDT.value.flags & ~fIsLockAtLocation);
    }

bool PACKRecord::IsUnlockAtStart()
    {
    return (PKDT.value.flags & fIsUnlockAtStart) != 0;
    }

void PACKRecord::IsUnlockAtStart(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsUnlockAtStart) : (PKDT.value.flags & ~fIsUnlockAtStart);
    }

bool PACKRecord::IsUnlockAtEnd()
    {
    return (PKDT.value.flags & fIsUnlockAtEnd) != 0;
    }

void PACKRecord::IsUnlockAtEnd(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsUnlockAtEnd) : (PKDT.value.flags & ~fIsUnlockAtEnd);
    }

bool PACKRecord::IsUnlockAtLocation()
    {
    return (PKDT.value.flags & fIsUnlockAtLocation) != 0;
    }

void PACKRecord::IsUnlockAtLocation(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsUnlockAtLocation) : (PKDT.value.flags & ~fIsUnlockAtLocation);
    }

bool PACKRecord::IsContinueIfPcNear()
    {
    return (PKDT.value.flags & fIsContinueIfPcNear) != 0;
    }

void PACKRecord::IsContinueIfPcNear(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsContinueIfPcNear) : (PKDT.value.flags & ~fIsContinueIfPcNear);
    }

bool PACKRecord::IsOncePerDay()
    {
    return (PKDT.value.flags & fIsOncePerDay) != 0;
    }

void PACKRecord::IsOncePerDay(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsOncePerDay) : (PKDT.value.flags & ~fIsOncePerDay);
    }

bool PACKRecord::IsSkipFallout()
    {
    return (PKDT.value.flags & fIsSkipFallout) != 0;
    }

void PACKRecord::IsSkipFallout(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsSkipFallout) : (PKDT.value.flags & ~fIsSkipFallout);
    }

bool PACKRecord::IsAlwaysRun()
    {
    return (PKDT.value.flags & fIsAlwaysRun) != 0;
    }

void PACKRecord::IsAlwaysRun(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsAlwaysRun) : (PKDT.value.flags & ~fIsAlwaysRun);
    }

bool PACKRecord::IsAlwaysSneak()
    {
    return (PKDT.value.flags & fIsAlwaysSneak) != 0;
    }

void PACKRecord::IsAlwaysSneak(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsAlwaysSneak) : (PKDT.value.flags & ~fIsAlwaysSneak);
    }

bool PACKRecord::IsAllowSwimming()
    {
    return (PKDT.value.flags & fIsAllowSwimming) != 0;
    }

void PACKRecord::IsAllowSwimming(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsAllowSwimming) : (PKDT.value.flags & ~fIsAllowSwimming);
    }

bool PACKRecord::IsAllowFalls()
    {
    return (PKDT.value.flags & fIsAllowFalls) != 0;
    }

void PACKRecord::IsAllowFalls(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsAllowFalls) : (PKDT.value.flags & ~fIsAllowFalls);
    }

bool PACKRecord::IsUnequipArmor()
    {
    return (PKDT.value.flags & fIsUnequipArmor) != 0;
    }

void PACKRecord::IsUnequipArmor(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsUnequipArmor) : (PKDT.value.flags & ~fIsUnequipArmor);
    }

bool PACKRecord::IsUnequipWeapons()
    {
    return (PKDT.value.flags & fIsUnequipWeapons) != 0;
    }

void PACKRecord::IsUnequipWeapons(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsUnequipWeapons) : (PKDT.value.flags & ~fIsUnequipWeapons);
    }

bool PACKRecord::IsDefensiveCombat()
    {
    return (PKDT.value.flags & fIsDefensiveCombat) != 0;
    }

void PACKRecord::IsDefensiveCombat(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsDefensiveCombat) : (PKDT.value.flags & ~fIsDefensiveCombat);
    }

bool PACKRecord::IsUseHorse()
    {
    return (PKDT.value.flags & fIsUseHorse) != 0;
    }

void PACKRecord::IsUseHorse(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsUseHorse) : (PKDT.value.flags & ~fIsUseHorse);
    }

bool PACKRecord::IsNoIdleAnims()
    {
    return (PKDT.value.flags & fIsNoIdleAnims) != 0;
    }

void PACKRecord::IsNoIdleAnims(bool value)
    {
    PKDT.value.flags = value ? (PKDT.value.flags | fIsNoIdleAnims) : (PKDT.value.flags & ~fIsNoIdleAnims);
    }


bool PACKRecord::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((PKDT.value.flags & Mask) == Mask) : ((PKDT.value.flags & Mask) != 0);
    }

void PACKRecord::SetFlagMask(UINT32 Mask)
    {
    PKDT.value.flags = Mask;
    }

bool PACKRecord::IsLocNearReference()
    {
    return PLDT.IsLoaded() ? (PLDT->locType == eLocNearReference) : false;
    }

void PACKRecord::IsLocNearReference(bool value)
    {
    if(!PLDT.IsLoaded()) return;
    if(value)
        PLDT->locType = eLocNearReference;
    else if(IsLocNearReference())
        PLDT->locType = eLocInCell;
    }

bool PACKRecord::IsLocInCell()
    {
    return PLDT.IsLoaded() ? (PLDT->locType == eLocInCell) : false;
    }

void PACKRecord::IsLocInCell(bool value)
    {
    if(!PLDT.IsLoaded()) return;
    if(value)
        PLDT->locType = eLocInCell;
    else if(IsLocInCell())
        PLDT->locType = eLocNearReference;
    }

bool PACKRecord::IsLocNearCurrentLoc()
    {
    return PLDT.IsLoaded() ? (PLDT->locType == eLocNearCurrentLocation) : false;
    }

void PACKRecord::IsLocNearCurrentLoc(bool value)
    {
    if(!PLDT.IsLoaded()) return;
    if(value)
        PLDT->locType = eLocNearCurrentLocation;
    else if(IsLocNearCurrentLoc())
        PLDT->locType = eLocNearReference;
    }

bool PACKRecord::IsLocNearEditorLoc()
    {
    return PLDT.IsLoaded() ? (PLDT->locType == eLocNearEditorLocation) : false;
    }

void PACKRecord::IsLocNearEditorLoc(bool value)
    {
    if(!PLDT.IsLoaded()) return;
    if(value)
        PLDT->locType = eLocNearEditorLocation;
    else if(IsLocNearEditorLoc())
        PLDT->locType = eLocNearReference;
    }

bool PACKRecord::IsLocObjectID()
    {
    return PLDT.IsLoaded() ? (PLDT->locType == eLocObjectID) : false;
    }

void PACKRecord::IsLocObjectID(bool value)
    {
    if(!PLDT.IsLoaded()) return;
    if(value)
        PLDT->locType = eLocObjectID;
    else if(IsLocObjectID())
        PLDT->locType = eLocNearReference;
    }

bool PACKRecord::IsLocObjectType()
    {
    return PLDT.IsLoaded() ? (PLDT->locType == eLocObjectType) : false;
    }

void PACKRecord::IsLocObjectType(bool value)
    {
    if(!PLDT.IsLoaded()) return;
    if(value)
        PLDT->locType = eLocObjectType;
    else if(IsLocObjectType())
        PLDT->locType = eLocNearReference;
    }

bool PACKRecord::IsLocType(SINT32 Type)
    {
    return PLDT.IsLoaded() ? (PLDT->locType == Type) : false;
    }

void PACKRecord::SetLocType(SINT32 Type)
    {
    PLDT.Load();
    PLDT->locType = Type;
    }

bool PACKRecord::IsTargetReference()
    {
    return PTDT.IsLoaded() ? (PTDT->targetType == eTargetReference) : false;
    }

void PACKRecord::IsTargetReference(bool value)
    {
    if(!PTDT.IsLoaded()) return;
    if(value)
        PTDT->targetType = eTargetReference;
    else if(IsTargetReference())
        PTDT->targetType = eTargetObjectID;
    }

bool PACKRecord::IsTargetObjectID()
    {
    return PTDT.IsLoaded() ? (PTDT->targetType == eTargetObjectID) : false;
    }

void PACKRecord::IsTargetObjectID(bool value)
    {
    if(!PTDT.IsLoaded()) return;
    if(value)
        PTDT->targetType = eTargetObjectID;
    else if(IsTargetObjectID())
        PTDT->targetType = eTargetReference;
    }

bool PACKRecord::IsTargetObjectType()
    {
    return PTDT.IsLoaded() ? (PTDT->targetType == eTargetObjectType) : false;
    }

void PACKRecord::IsTargetObjectType(bool value)
    {
    if(!PTDT.IsLoaded()) return;
    if(value)
        PTDT->targetType = eTargetObjectType;
    else if(IsTargetObjectType())
        PTDT->targetType = eTargetReference;
    }

bool PACKRecord::IsTargetType(SINT32 Type)
    {
    return PTDT.IsLoaded() ? (PTDT->targetType == Type) : false;
    }

void PACKRecord::SetTargetType(SINT32 Type)
    {
    PTDT.Load();
    PTDT->targetType = Type;
    }

UINT32 PACKRecord::GetSize(bool forceCalc)
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

    if(PKDT.IsLoaded())
        TotSize += PKDT.GetSize() + 6;

    if(PLDT.IsLoaded())
        TotSize += PLDT.GetSize() + 6;

    if(PSDT.IsLoaded())
        TotSize += PSDT.GetSize() + 6;

    if(PTDT.IsLoaded())
        TotSize += PTDT.GetSize() + 6;

    for(UINT32 p = 0; p < CTDA.size(); p++)
        if(CTDA[p]->IsLoaded())
            TotSize += CTDA[p]->GetSize() + 6;

    return TotSize;
    }

UINT32 PACKRecord::GetType()
    {
    return 'KCAP';
    }

STRING PACKRecord::GetStrType()
    {
    return "PACK";
    }

PACKRecord::PACKRecord(PACKRecord *srcRecord):
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
    PKDT = srcRecord->PKDT;
    PLDT = srcRecord->PLDT;
    PSDT = srcRecord->PSDT;
    PTDT = srcRecord->PTDT;
    CTDA.clear();
    CTDA.resize(srcRecord->CTDA.size());
    for(UINT32 x = 0; x < srcRecord->CTDA.size(); x++)
        {
        CTDA[x] = new ReqSubRecord<GENCTDA>;
        *CTDA[x] = *srcRecord->CTDA[x];
        }
    return;
    }

PACKRecord::~PACKRecord()
    {
    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    }

SINT32 PACKRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    ReqSubRecord<GENCTDA> *newCTDA = NULL;
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
            case 'TDKP':
                switch(subSize)
                    {
                    case 4:
                        //old format (flags was originally a short, but changed to a uint; this change also affected the padding)
                        //PKDT.size = 8; //Force it to write out the updated version
                        _readBuffer(&PKDT.value.flags, buffer, 2, curPos);
                        _readBuffer(&PKDT.value.aiType, buffer, 1, curPos);
                        curPos++; //Skip over junk value. unused padding will default to 0
                        break;
                    default:
                        PKDT.Read(buffer, subSize, curPos);
                        break;
                    }
                break;
            case 'TDLP':
                PLDT.Read(buffer, subSize, curPos);
                break;
            case 'TDSP':
                PSDT.Read(buffer, subSize, curPos);
                break;
            case 'TDTP':
                PTDT.Read(buffer, subSize, curPos);
                break;
            case 'TDTC':
            case 'ADTC':
                newCTDA = new ReqSubRecord<GENCTDA>;
                newCTDA->Read(buffer, subSize, curPos);
                CTDA.push_back(newCTDA);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  PACK: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 PACKRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    PKDT.Unload();
    PLDT.Unload();
    PSDT.Unload();
    PTDT.Unload();

    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    CTDA.clear();
    return 1;
    }

SINT32 PACKRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    FunctionArguments CTDAFunction;
    Function_Arguments_Iterator curCTDAFunction;

    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(PKDT.IsLoaded())
        SaveHandler.writeSubRecord('TDKP', &PKDT.value, PKDT.GetSize());
    if(PLDT.IsLoaded())
        SaveHandler.writeSubRecord('TDLP', PLDT.value, PLDT.GetSize());
    if(PSDT.IsLoaded())
        SaveHandler.writeSubRecord('TDSP', &PSDT.value, PSDT.GetSize());
    if(PTDT.IsLoaded())
        SaveHandler.writeSubRecord('TDTP', PTDT.value, PTDT.GetSize());
    for(UINT32 p = 0; p < CTDA.size(); p++)
        {
        curCTDAFunction = Function_Arguments.find(CTDA[p]->value.ifunc);
        if(curCTDAFunction != Function_Arguments.end())
            {
            CTDAFunction = curCTDAFunction->second;
            if(CTDAFunction.first == eNONE)
                CTDA[p]->value.param1 = 0;
            if(CTDAFunction.second == eNONE)
                CTDA[p]->value.param2 = 0;
            }
        SaveHandler.writeSubRecord('ADTC', &CTDA[p]->value, CTDA[p]->GetSize());
        }
    return -1;
    }

bool PACKRecord::operator ==(const PACKRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        PKDT == other.PKDT &&
        PLDT == other.PLDT &&
        PSDT == other.PSDT &&
        PTDT == other.PTDT &&
        CTDA.size() == other.CTDA.size())
        {
        //Record order matters on conditions, so equality testing is easy
        for(UINT32 x = 0; x < CTDA.size(); ++x)
            if(*CTDA[x] != *other.CTDA[x])
                return false;
        return true;
        }

    return false;
    }

bool PACKRecord::operator !=(const PACKRecord &other) const
    {
    return !(*this == other);
    }