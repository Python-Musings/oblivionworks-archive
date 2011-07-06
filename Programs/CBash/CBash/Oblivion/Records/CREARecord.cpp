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
#include "CREARecord.h"

CREARecord::CREADATA::CREADATA():
    creatureType(0),
    combat(0),
    magic(0),
    stealth(0),
    soul(0),
    unused1(0x00),
    health(0),
    attackDamage(0),
    strength(0),
    intelligence(0),
    willpower(0),
    agility(0),
    speed(0),
    endurance(0),
    personality(0),
    luck(0)
    {
    memset(&unused2, 0x00, 2);
    }

CREARecord::CREADATA::~CREADATA()
    {
    //
    }

bool CREARecord::CREADATA::operator ==(const CREADATA &other) const
    {
    return (creatureType == other.creatureType &&
            combat == other.combat &&
            magic == other.magic &&
            stealth == other.stealth &&
            soul == other.soul &&
            health == other.health &&
            attackDamage == other.attackDamage &&
            strength == other.strength &&
            intelligence == other.intelligence &&
            willpower == other.willpower &&
            agility == other.agility &&
            speed == other.speed &&
            endurance == other.endurance &&
            personality == other.personality &&
            luck == other.luck);
    }

bool CREARecord::CREADATA::operator !=(const CREADATA &other) const
    {
    return !(*this == other);
    }

CREARecord::CREARNAM::CREARNAM():
    attackReach(0)
    {
    //
    }

CREARecord::CREARNAM::~CREARNAM()
    {
    //
    }

bool CREARecord::CREARNAM::operator ==(const CREARNAM &other) const
    {
    return (attackReach == other.attackReach);
    }

bool CREARecord::CREARNAM::operator !=(const CREARNAM &other) const
    {
    return !(*this == other);
    }

CREARecord::CREATNAM::CREATNAM():
    turningSpeed(0.0f)
    {
    //
    }

CREARecord::CREATNAM::~CREATNAM()
    {
    //
    }

bool CREARecord::CREATNAM::operator ==(const CREATNAM &other) const
    {
    return (AlmostEqual(turningSpeed,other.turningSpeed,2));
    }

bool CREARecord::CREATNAM::operator !=(const CREATNAM &other) const
    {
    return !(*this == other);
    }

CREARecord::CREABNAM::CREABNAM():
    baseScale(0.0f)
    {
    //
    }

CREARecord::CREABNAM::~CREABNAM()
    {
    //
    }

bool CREARecord::CREABNAM::operator ==(const CREABNAM &other) const
    {
    return (AlmostEqual(baseScale,other.baseScale,2));
    }

bool CREARecord::CREABNAM::operator !=(const CREABNAM &other) const
    {
    return !(*this == other);
    }

CREARecord::CREAWNAM::CREAWNAM():
    footWeight(0.0f)
    {
    //
    }

CREARecord::CREAWNAM::~CREAWNAM()
    {
    //
    }

bool CREARecord::CREAWNAM::operator ==(const CREAWNAM &other) const
    {
    return (AlmostEqual(footWeight,other.footWeight,2));
    }

bool CREARecord::CREAWNAM::operator !=(const CREAWNAM &other) const
    {
    return !(*this == other);
    }

CREARecord::CREACSDT::CREACSDT():soundType(0)
    {
    //
    }

CREARecord::CREACSDT::~CREACSDT()
    {
    //
    }

bool CREARecord::CREACSDT::operator ==(const CREACSDT &other) const
    {
    return (soundType == other.soundType);
    }

bool CREARecord::CREACSDT::operator !=(const CREACSDT &other) const
    {
    return !(*this == other);
    }

CREARecord::CREACSDC::CREACSDC():
    chance(0)
    {
    //
    }

CREARecord::CREACSDC::~CREACSDC()
    {
    //
    }

bool CREARecord::CREACSDC::operator ==(const CREACSDC &other) const
    {
    return (chance == other.chance);
    }

bool CREARecord::CREACSDC::operator !=(const CREACSDC &other) const
    {
    return !(*this == other);
    }

bool CREARecord::CREASound::IsLeftFoot()
    {
    return (CSDT.value.soundType == eLeftFoot);
    }

void CREARecord::CREASound::IsLeftFoot(bool value)
    {
    if(value)
        CSDT.value.soundType = eLeftFoot;
    else if(IsLeftFoot())
        CSDT.value.soundType = eRightFoot;
    }

bool CREARecord::CREASound::IsRightFoot()
    {
    return (CSDT.value.soundType == eRightFoot);
    }

void CREARecord::CREASound::IsRightFoot(bool value)
    {
    if(value)
        CSDT.value.soundType = eRightFoot;
    else if(IsRightFoot())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsLeftBackFoot()
    {
    return (CSDT.value.soundType == eLeftBackFoot);
    }

void CREARecord::CREASound::IsLeftBackFoot(bool value)
    {
    if(value)
        CSDT.value.soundType = eLeftBackFoot;
    else if(IsLeftBackFoot())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsRightBackFoot()
    {
    return (CSDT.value.soundType == eRightBackFoot);
    }

void CREARecord::CREASound::IsRightBackFoot(bool value)
    {
    if(value)
        CSDT.value.soundType = eRightBackFoot;
    else if(IsRightBackFoot())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsIdle()
    {
    return (CSDT.value.soundType == eIdle);
    }

void CREARecord::CREASound::IsIdle(bool value)
    {
    if(value)
        CSDT.value.soundType = eIdle;
    else if(IsIdle())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsAware()
    {
    return (CSDT.value.soundType == eAware);
    }

void CREARecord::CREASound::IsAware(bool value)
    {
    if(value)
        CSDT.value.soundType = eAware;
    else if(IsAware())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsAttack()
    {
    return (CSDT.value.soundType == eAttack);
    }

void CREARecord::CREASound::IsAttack(bool value)
    {
    if(value)
        CSDT.value.soundType = eAttack;
    else if(IsAttack())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsHit()
    {
    return (CSDT.value.soundType == eHit);
    }

void CREARecord::CREASound::IsHit(bool value)
    {
    if(value)
        CSDT.value.soundType = eHit;
    else if(IsHit())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsDeath()
    {
    return (CSDT.value.soundType == eDeath);
    }

void CREARecord::CREASound::IsDeath(bool value)
    {
    if(value)
        CSDT.value.soundType = eDeath;
    else if(IsDeath())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsWeapon()
    {
    return (CSDT.value.soundType == eWeapon);
    }

void CREARecord::CREASound::IsWeapon(bool value)
    {
    if(value)
        CSDT.value.soundType = eWeapon;
    else if(IsWeapon())
        CSDT.value.soundType = eLeftFoot;
    }

bool CREARecord::CREASound::IsType(UINT32 Type)
    {
    return (CSDT.value.soundType == Type);
    }

void CREARecord::CREASound::SetType(UINT32 Type)
    {
    CSDT.value.soundType = Type;
    }

bool CREARecord::CREASound::operator ==(const CREASound &other) const
    {
    return (CSDT == other.CSDT &&
            CSDI == other.CSDI &&
            CSDC == other.CSDC);
    }

bool CREARecord::CREASound::operator !=(const CREASound &other) const
    {
    return !(*this == other);
    }

CREARecord::CREARecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

CREARecord::CREARecord(CREARecord *srcRecord):
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
    FULL = srcRecord->FULL;
    if(srcRecord->MODL.IsLoaded())
        {
        MODL.Load();
        MODL->MODB = srcRecord->MODL->MODB;
        MODL->MODL = srcRecord->MODL->MODL;
        MODL->MODT = srcRecord->MODL->MODT;
        }
    SPLO.resize(srcRecord->SPLO.size());
    for(UINT32 x = 0; x < srcRecord->SPLO.size(); x++)
        SPLO[x] = srcRecord->SPLO[x];

    NIFZ.resize(srcRecord->NIFZ.size());
    for(UINT32 x = 0; x < srcRecord->NIFZ.size(); x++)
        NIFZ[x].Copy(srcRecord->NIFZ[x]);
    NIFT = srcRecord->NIFT;
    ACBS = srcRecord->ACBS;

    SNAM.resize(srcRecord->SNAM.size());
    for(UINT32 x = 0; x < srcRecord->SNAM.size(); x++)
        {
        SNAM[x] = new ReqSubRecord<GENSNAM>;
        *SNAM[x] = *srcRecord->SNAM[x];
        }
    INAM = srcRecord->INAM;
    SCRI = srcRecord->SCRI;

    CNTO.resize(srcRecord->CNTO.size());
    for(UINT32 x = 0; x < srcRecord->CNTO.size(); x++)
        {
        CNTO[x] = new ReqSubRecord<GENCNTO>;
        *CNTO[x] = *srcRecord->CNTO[x];
        }
    AIDT = srcRecord->AIDT;
    PKID.resize(srcRecord->PKID.size());
    for(UINT32 x = 0; x < srcRecord->PKID.size(); x++)
        PKID[x] = srcRecord->PKID[x];

    KFFZ.resize(srcRecord->KFFZ.size());
    for(UINT32 x = 0; x < srcRecord->KFFZ.size(); x++)
        KFFZ[x].Copy(srcRecord->KFFZ[x]);

    DATA = srcRecord->DATA;
    RNAM = srcRecord->RNAM;
    ZNAM = srcRecord->ZNAM;
    TNAM = srcRecord->TNAM;
    BNAM = srcRecord->BNAM;
    WNAM = srcRecord->WNAM;
    CSCR = srcRecord->CSCR;
    NAM0 = srcRecord->NAM0;
    NAM1 = srcRecord->NAM1;

    Sounds.resize(srcRecord->Sounds.size());
    for(UINT32 x = 0; x < srcRecord->Sounds.size(); x++)
        {
        Sounds[x] = new CREASound;
        Sounds[x]->CSDT = srcRecord->Sounds[x]->CSDT;
        Sounds[x]->CSDI = srcRecord->Sounds[x]->CSDI;
        Sounds[x]->CSDC = srcRecord->Sounds[x]->CSDC;
        }
    return;
    }

CREARecord::~CREARecord()
    {
    for(UINT32 x = 0; x < SNAM.size(); x++)
        delete SNAM[x];
    for(UINT32 x = 0; x < CNTO.size(); x++)
        delete CNTO[x];
    for(UINT32 x = 0; x < Sounds.size(); x++)
        delete Sounds[x];
    }

bool CREARecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < SPLO.size(); x++)
        op.Accept(SPLO[x]);
    for(UINT32 x = 0; x < SNAM.size(); x++)
        op.Accept(SNAM[x]->value.faction);
    if(INAM.IsLoaded())
        op.Accept(INAM->fid);
    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);
    for(UINT32 x = 0; x < CNTO.size(); x++)
        op.Accept(CNTO[x]->value.item);
    for(UINT32 x = 0; x < PKID.size(); x++)
        op.Accept(PKID[x]);
    if(ZNAM.IsLoaded())
        op.Accept(ZNAM->fid);
    if(CSCR.IsLoaded())
        op.Accept(CSCR->fid);
    for(UINT32 x = 0; x < Sounds.size(); x++)
        op.Accept(Sounds[x]->CSDI.value.fid);

    return op.Stop();
    }

bool CREARecord::IsBiped()
    {
    return (ACBS.value.flags & fIsBiped) != 0;
    }

void CREARecord::IsBiped(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsBiped) : (ACBS.value.flags & ~fIsBiped);
    }

bool CREARecord::IsEssential()
    {
    return (ACBS.value.flags & fIsEssential) != 0;
    }

void CREARecord::IsEssential(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsEssential) : (ACBS.value.flags & ~fIsEssential);
    }

bool CREARecord::IsWeaponAndShield()
    {
    return (ACBS.value.flags & fIsWeaponAndShield) != 0;
    }

void CREARecord::IsWeaponAndShield(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsWeaponAndShield) : (ACBS.value.flags & ~fIsWeaponAndShield);
    }

bool CREARecord::IsRespawn()
    {
    return (ACBS.value.flags & fIsRespawn) != 0;
    }

void CREARecord::IsRespawn(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsRespawn) : (ACBS.value.flags & ~fIsRespawn);
    }

bool CREARecord::IsSwims()
    {
    return (ACBS.value.flags & fIsSwims) != 0;
    }

void CREARecord::IsSwims(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsSwims) : (ACBS.value.flags & ~fIsSwims);
    }

bool CREARecord::IsFlies()
    {
    return (ACBS.value.flags & fIsFlies) != 0;
    }

void CREARecord::IsFlies(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsFlies) : (ACBS.value.flags & ~fIsFlies);
    }

bool CREARecord::IsWalks()
    {
    return (ACBS.value.flags & fIsWalks) != 0;
    }

void CREARecord::IsWalks(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsWalks) : (ACBS.value.flags & ~fIsWalks);
    }

bool CREARecord::IsPCLevelOffset()
    {
    return (ACBS.value.flags & fIsPCLevelOffset) != 0;
    }

void CREARecord::IsPCLevelOffset(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsPCLevelOffset) : (ACBS.value.flags & ~fIsPCLevelOffset);
    }

bool CREARecord::IsNoLowLevel()
    {
    return (ACBS.value.flags & fIsNoLowLevel) != 0;
    }

void CREARecord::IsNoLowLevel(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoLowLevel) : (ACBS.value.flags & ~fIsNoLowLevel);
    }

bool CREARecord::IsLowLevel()
    {
    return !IsNoLowLevel();
    }

void CREARecord::IsLowLevel(bool value)
    {
    IsNoLowLevel(!value);
    }

bool CREARecord::IsNoBloodSpray()
    {
    return (ACBS.value.flags & fIsNoBloodSpray) != 0;
    }

void CREARecord::IsNoBloodSpray(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoBloodSpray) : (ACBS.value.flags & ~fIsNoBloodSpray);
    }

bool CREARecord::IsBloodSpray()
    {
    return !IsNoBloodSpray();
    }

void CREARecord::IsBloodSpray(bool value)
    {
    IsNoBloodSpray(!value);
    }

bool CREARecord::IsNoBloodDecal()
    {
    return (ACBS.value.flags & fIsNoBloodDecal) != 0;
    }

void CREARecord::IsNoBloodDecal(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoBloodDecal) : (ACBS.value.flags & ~fIsNoBloodDecal);
    }

bool CREARecord::IsBloodDecal()
    {
    return !IsNoBloodDecal();
    }

void CREARecord::IsBloodDecal(bool value)
    {
    IsNoBloodDecal(!value);
    }

bool CREARecord::IsNoHead()
    {
    return (ACBS.value.flags & fIsNoHead) != 0;
    }

void CREARecord::IsNoHead(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoHead) : (ACBS.value.flags & ~fIsNoHead);
    }

bool CREARecord::IsHead()
    {
    return !IsNoHead();
    }

void CREARecord::IsHead(bool value)
    {
    IsNoHead(!value);
    }

bool CREARecord::IsNoRightArm()
    {
    return (ACBS.value.flags & fIsNoRightArm) != 0;
    }

void CREARecord::IsNoRightArm(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoRightArm) : (ACBS.value.flags & ~fIsNoRightArm);
    }

bool CREARecord::IsRightArm()
    {
    return !IsNoRightArm();
    }

void CREARecord::IsRightArm(bool value)
    {
    IsNoRightArm(!value);
    }

bool CREARecord::IsNoLeftArm()
    {
    return (ACBS.value.flags & fIsNoLeftArm) != 0;
    }

void CREARecord::IsNoLeftArm(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoLeftArm) : (ACBS.value.flags & ~fIsNoLeftArm);
    }

bool CREARecord::IsLeftArm()
    {
    return !IsNoLeftArm();
    }

void CREARecord::IsLeftArm(bool value)
    {
    IsNoLeftArm(!value);
    }

bool CREARecord::IsNoCombatInWater()
    {
    return (ACBS.value.flags & fIsNoCombatInWater) != 0;
    }

void CREARecord::IsNoCombatInWater(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoCombatInWater) : (ACBS.value.flags & ~fIsNoCombatInWater);
    }

bool CREARecord::IsCombatInWater()
    {
    return !IsNoCombatInWater();
    }

void CREARecord::IsCombatInWater(bool value)
    {
    IsNoCombatInWater(!value);
    }

bool CREARecord::IsNoShadow()
    {
    return (ACBS.value.flags & fIsNoShadow) != 0;
    }

void CREARecord::IsNoShadow(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoShadow) : (ACBS.value.flags & ~fIsNoShadow);
    }

bool CREARecord::IsShadow()
    {
    return !IsNoShadow();
    }

void CREARecord::IsShadow(bool value)
    {
    IsNoShadow(!value);
    }

bool CREARecord::IsNoCorpseCheck()
    {
    return (ACBS.value.flags & fIsNoCorpseCheck) != 0;
    }

void CREARecord::IsNoCorpseCheck(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoCorpseCheck) : (ACBS.value.flags & ~fIsNoCorpseCheck);
    }

bool CREARecord::IsCorpseCheck()
    {
    return !IsNoCorpseCheck();
    }

void CREARecord::IsCorpseCheck(bool value)
    {
    IsNoCorpseCheck(!value);
    }

bool CREARecord::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((ACBS.value.flags & Mask) == Mask) : ((ACBS.value.flags & Mask) != 0);
    }

void CREARecord::SetFlagMask(UINT32 Mask)
    {
    ACBS.value.flags = Mask;
    }

bool CREARecord::IsCreature()
    {
    return (DATA.value.creatureType == eCreature);
    }

void CREARecord::IsCreature(bool value)
    {
    if(value)
        DATA.value.creatureType = eCreature;
    else if(IsCreature())
        DATA.value.creatureType = eDaedra;
    }

bool CREARecord::IsDaedra()
    {
    return (DATA.value.creatureType == eDaedra);
    }

void CREARecord::IsDaedra(bool value)
    {
    if(value)
        DATA.value.creatureType = eDaedra;
    else if(IsDaedra())
        DATA.value.creatureType = eCreature;
    }

bool CREARecord::IsUndead()
    {
    return (DATA.value.creatureType == eUndead);
    }

void CREARecord::IsUndead(bool value)
    {
    if(value)
        DATA.value.creatureType = eUndead;
    else if(IsUndead())
        DATA.value.creatureType = eCreature;
    }

bool CREARecord::IsHumanoid()
    {
    return (DATA.value.creatureType == eHumanoid);
    }

void CREARecord::IsHumanoid(bool value)
    {
    if(value)
        DATA.value.creatureType = eHumanoid;
    else if(IsHumanoid())
        DATA.value.creatureType = eCreature;
    }

bool CREARecord::IsHorse()
    {
    return (DATA.value.creatureType == eHorse);
    }

void CREARecord::IsHorse(bool value)
    {
    if(value)
        DATA.value.creatureType = eHorse;
    else if(IsHorse())
        DATA.value.creatureType = eCreature;
    }

bool CREARecord::IsGiant()
    {
    return (DATA.value.creatureType == eGiant);
    }

void CREARecord::IsGiant(bool value)
    {
    if(value)
        DATA.value.creatureType = eGiant;
    else if(IsGiant())
        DATA.value.creatureType = eCreature;
    }

bool CREARecord::IsType(UINT8 Type)
    {
    return (DATA.value.creatureType == Type);
    }

void CREARecord::SetType(UINT8 Type)
    {
    DATA.value.creatureType = Type;
    }

bool CREARecord::IsNoSoul()
    {
    return (DATA.value.soul == eNone);
    }

void CREARecord::IsNoSoul(bool value)
    {
    if(value)
        DATA.value.soul = eNone;
    else if(IsNoSoul())
        DATA.value.soul = ePetty;
    }

bool CREARecord::IsPettySoul()
    {
    return (DATA.value.soul == ePetty);
    }

void CREARecord::IsPettySoul(bool value)
    {
    if(value)
        DATA.value.soul = ePetty;
    else if(IsPettySoul())
        DATA.value.soul = eNone;
    }

bool CREARecord::IsLesserSoul()
    {
    return (DATA.value.soul == eLesser);
    }

void CREARecord::IsLesserSoul(bool value)
    {
    if(value)
        DATA.value.soul = eLesser;
    else if(IsLesserSoul())
        DATA.value.soul = eNone;
    }

bool CREARecord::IsCommonSoul()
    {
    return (DATA.value.soul == eCommon);
    }

void CREARecord::IsCommonSoul(bool value)
    {
    if(value)
        DATA.value.soul = eCommon;
    else if(IsCommonSoul())
        DATA.value.soul = eNone;
    }

bool CREARecord::IsGreaterSoul()
    {
    return (DATA.value.soul == eGreater);
    }

void CREARecord::IsGreaterSoul(bool value)
    {
    if(value)
        DATA.value.soul = eGreater;
    else if(IsGreaterSoul())
        DATA.value.soul = eNone;
    }

bool CREARecord::IsGrandSoul()
    {
    return (DATA.value.soul == eGrand);
    }

void CREARecord::IsGrandSoul(bool value)
    {
    if(value)
        DATA.value.soul = eGrand;
    else if(IsGrandSoul())
        DATA.value.soul = eNone;
    }

bool CREARecord::IsSoul(UINT8 Type)
    {
    return (DATA.value.soul == Type);
    }

void CREARecord::SetSoul(UINT8 Type)
    {
    DATA.value.soul = Type;
    }

bool CREARecord::IsServicesWeapons()
    {
    return (AIDT.value.flags & fWeapons) != 0;
    }

void CREARecord::IsServicesWeapons(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fWeapons) : (AIDT.value.flags & ~fWeapons);
    }

bool CREARecord::IsServicesArmor()
    {
    return (AIDT.value.flags & fArmor) != 0;
    }

void CREARecord::IsServicesArmor(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fArmor) : (AIDT.value.flags & ~fArmor);
    }

bool CREARecord::IsServicesClothing()
    {
    return (AIDT.value.flags & fClothing) != 0;
    }

void CREARecord::IsServicesClothing(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fClothing) : (AIDT.value.flags & ~fClothing);
    }

bool CREARecord::IsServicesBooks()
    {
    return (AIDT.value.flags & fBooks) != 0;
    }

void CREARecord::IsServicesBooks(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fBooks) : (AIDT.value.flags & ~fBooks);
    }

bool CREARecord::IsServicesIngredients()
    {
    return (AIDT.value.flags & fIngredients) != 0;
    }

void CREARecord::IsServicesIngredients(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fIngredients) : (AIDT.value.flags & ~fIngredients);
    }

bool CREARecord::IsServicesLights()
    {
    return (AIDT.value.flags & fLights) != 0;
    }

void CREARecord::IsServicesLights(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fLights) : (AIDT.value.flags & ~fLights);
    }

bool CREARecord::IsServicesApparatus()
    {
    return (AIDT.value.flags & fApparatus) != 0;
    }

void CREARecord::IsServicesApparatus(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fApparatus) : (AIDT.value.flags & ~fApparatus);
    }

bool CREARecord::IsServicesMiscItems()
    {
    return (AIDT.value.flags & fMiscItems) != 0;
    }

void CREARecord::IsServicesMiscItems(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fMiscItems) : (AIDT.value.flags & ~fMiscItems);
    }

bool CREARecord::IsServicesSpells()
    {
    return (AIDT.value.flags & fSpells) != 0;
    }

void CREARecord::IsServicesSpells(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fSpells) : (AIDT.value.flags & ~fSpells);
    }

bool CREARecord::IsServicesMagicItems()
    {
    return (AIDT.value.flags & fMagicItems) != 0;
    }

void CREARecord::IsServicesMagicItems(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fMagicItems) : (AIDT.value.flags & ~fMagicItems);
    }

bool CREARecord::IsServicesPotions()
    {
    return (AIDT.value.flags & fPotions) != 0;
    }

void CREARecord::IsServicesPotions(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fPotions) : (AIDT.value.flags & ~fPotions);
    }

bool CREARecord::IsServicesTraining()
    {
    return (AIDT.value.flags & fTraining) != 0;
    }

void CREARecord::IsServicesTraining(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fTraining) : (AIDT.value.flags & ~fTraining);
    }

bool CREARecord::IsServicesRecharge()
    {
    return (AIDT.value.flags & fRecharge) != 0;
    }

void CREARecord::IsServicesRecharge(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fRecharge) : (AIDT.value.flags & ~fRecharge);
    }

bool CREARecord::IsServicesRepair()
    {
    return (AIDT.value.flags & fRepair) != 0;
    }

void CREARecord::IsServicesRepair(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fRepair) : (AIDT.value.flags & ~fRepair);
    }

bool CREARecord::IsServicesFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((AIDT.value.flags & Mask) == Mask) : ((AIDT.value.flags & Mask) != 0);
    }

void CREARecord::SetServicesFlagMask(UINT32 Mask)
    {
    AIDT.value.flags = Mask;
    }

UINT32 CREARecord::GetSize(bool forceCalc)
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

    if(SPLO.size())
        TotSize += (UINT32)SPLO.size() * (sizeof(UINT32) + 6);

    if(NIFZ.size())
        {
        cSize = 1; //Type, size, and final null terminator
        for(UINT32 p = 0; p < NIFZ.size(); p++)
            if(NIFZ[p].IsLoaded())
                cSize += NIFZ[p].GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(NIFT.IsLoaded())
        {
        cSize = NIFT.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ACBS.IsLoaded())
        TotSize += ACBS.GetSize() + 6;

    for(UINT32 p = 0; p < SNAM.size(); p++)
        if(SNAM[p]->IsLoaded())
            TotSize += SNAM[p]->GetSize() + 6;

    if(INAM.IsLoaded())
        TotSize += INAM.GetSize() + 6;

    if(SCRI.IsLoaded())
        TotSize += SCRI.GetSize() + 6;

    if(CNTO.size())
        for(UINT32 p = 0; p < CNTO.size(); p++)
            if(CNTO[p]->IsLoaded())
                TotSize += CNTO[p]->GetSize() + 6;

    if(AIDT.IsLoaded())
        TotSize += AIDT.GetSize() + 6;

    if(PKID.size())
        TotSize += (UINT32)PKID.size() * (sizeof(UINT32) + 6);

    if(KFFZ.size())
        {
        cSize = 1; //Type, size, and final null terminator
        for(UINT32 p = 0; p < KFFZ.size(); p++)
            if(KFFZ[p].IsLoaded())
                cSize += KFFZ[p].GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(RNAM.IsLoaded())
        TotSize += RNAM.GetSize() + 6;

    if(ZNAM.IsLoaded())
        TotSize += ZNAM.GetSize() + 6;

    if(TNAM.IsLoaded())
        TotSize += TNAM.GetSize() + 6;

    if(BNAM.IsLoaded())
        TotSize += BNAM.GetSize() + 6;

    if(WNAM.IsLoaded())
        TotSize += WNAM.GetSize() + 6;

    if(CSCR.IsLoaded())
        TotSize += CSCR.GetSize() + 6;

    if(NAM0.IsLoaded())
        {
        cSize = NAM0.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(NAM1.IsLoaded())
        {
        cSize = NAM1.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    for(UINT32 p = 0; p < Sounds.size(); p++)
        {
        if(Sounds[p]->CSDT.IsLoaded())
            TotSize += Sounds[p]->CSDT.GetSize() + 6;

        if(Sounds[p]->CSDI.IsLoaded())
            TotSize += Sounds[p]->CSDI.GetSize() + 6;

        if(Sounds[p]->CSDC.IsLoaded())
            TotSize += Sounds[p]->CSDC.GetSize() + 6;
        }

    return TotSize;
    }

UINT32 CREARecord::GetType()
    {
    return 'AERC';
    }

STRING CREARecord::GetStrType()
    {
    return "CREA";
    }

SINT32 CREARecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    FORMID curFormID = 0;
    ReqSubRecord<GENSNAM> *newSNAM = NULL;
    ReqSubRecord<GENCNTO> *newCNTO = NULL;
    CREASound *newSound = NULL;
    UINT32 testNIFT = 0;
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
            case 'OLPS':
                _readBuffer(&curFormID, buffer, subSize, curPos);
                SPLO.push_back(curFormID);
                break;
            case 'ZFIN':
                for(subSize += curPos;curPos < (subSize - 1);curPos += (UINT32)strlen((char*)&buffer[curPos]) + 1)
                    NIFZ.push_back(StringRecord((char*)&buffer[curPos]));
                curPos++;
                break;
            case 'TFIN':
                NIFT.Read(buffer, subSize, curPos);
                //Hack
                testNIFT = 0;
                for(UINT32 x = 0; x < NIFT.GetSize() && testNIFT == 0; ++x)
                    testNIFT += NIFT.value[x];
                if(testNIFT == 0)
                    NIFT.Unload();
                break;
            case 'SBCA':
                ACBS.Read(buffer, subSize, curPos);
                break;
            case 'MANS':
                newSNAM = new ReqSubRecord<GENSNAM>;
                newSNAM->Read(buffer, subSize, curPos);
                SNAM.push_back(newSNAM);
                break;
            case 'MANI':
                INAM.Read(buffer, subSize, curPos);
                break;
            case 'IRCS':
                SCRI.Read(buffer, subSize, curPos);
                break;
            case 'OTNC':
                newCNTO = new ReqSubRecord<GENCNTO>;
                newCNTO->Read(buffer, subSize, curPos);
                CNTO.push_back(newCNTO);
                break;
            case 'TDIA':
                AIDT.Read(buffer, subSize, curPos);
                break;
            case 'DIKP':
                _readBuffer(&curFormID, buffer, subSize, curPos);
                PKID.push_back(curFormID);
                break;
            case 'ZFFK':
                for(subSize += curPos;curPos < (subSize - 1);curPos += (UINT32)strlen((char*)&buffer[curPos]) + 1)
                    KFFZ.push_back(StringRecord((char*)&buffer[curPos]));
                curPos++;
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'MANR':
                RNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANZ':
                ZNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANT':
                TNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANB':
                BNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANW':
                WNAM.Read(buffer, subSize, curPos);
                break;
            case 'RCSC':
                CSCR.Read(buffer, subSize, curPos);
                break;
            case '0MAN':
                NAM0.Read(buffer, subSize, curPos);
                break;
            case '1MAN':
                NAM1.Read(buffer, subSize, curPos);
                break;
            case 'TDSC':
                newSound = new CREASound;
                newSound->CSDT.Read(buffer, subSize, curPos);
                Sounds.push_back(newSound);
                break;
            case 'IDSC':
                if(newSound == NULL)
                    {
                    newSound = new CREASound;
                    Sounds.push_back(newSound);
                    }
                newSound->CSDI.Read(buffer, subSize, curPos);
                break;
            case 'CDSC':
                if(newSound == NULL)
                    {
                    newSound = new CREASound;
                    Sounds.push_back(newSound);
                    }
                newSound->CSDC.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  CREA: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        }
    return 0;
    }

SINT32 CREARecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    MODL.Unload();

    SPLO.clear();
    NIFZ.clear();

    NIFT.Unload();
    ACBS.Unload();

    for(UINT32 x = 0; x < SNAM.size(); x++)
        delete SNAM[x];
    SNAM.clear();

    INAM.Unload();
    SCRI.Unload();

    for(UINT32 x = 0; x < CNTO.size(); x++)
        delete CNTO[x];
    CNTO.clear();

    AIDT.Unload();

    PKID.clear();
    KFFZ.clear();

    DATA.Unload();
    RNAM.Unload();
    ZNAM.Unload();
    TNAM.Unload();
    BNAM.Unload();
    WNAM.Unload();
    CSCR.Unload();
    NAM0.Unload();
    NAM1.Unload();

    for(UINT32 x = 0; x < Sounds.size(); x++)
        delete Sounds[x];
    Sounds.clear();
    return 1;
    }

SINT32 CREARecord::WriteRecord(_FileHandler &SaveHandler)
    {
    UINT32 cSize = 0;

    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());

    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());

    if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MODL->MODL.value, MODL->MODL.GetSize());
        if(MODL->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MODL->MODB.value, MODL->MODB.GetSize());
        if(MODL->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MODL->MODT.value, MODL->MODT.GetSize());
        }

    for(UINT32 p = 0; p < SPLO.size(); p++)
        SaveHandler.writeSubRecord('OLPS', &SPLO[p], sizeof(UINT32));

    if(NIFZ.size())
        {
        cSize = 1; //final null terminator
        for(UINT32 p = 0; p < NIFZ.size(); p++)
            if(NIFZ[p].IsLoaded())
                cSize += NIFZ[p].GetSize();
        if(cSize > 65535) cSize += 10;
        SaveHandler.writeSubRecord('ZFIN', NULL, cSize);
        for(UINT32 p = 0; p < NIFZ.size(); p++)
            if(NIFZ[p].IsLoaded())
                SaveHandler.write(NIFZ[p].value, NIFZ[p].GetSize());
        cSize = 0;
        //write final null terminator
        SaveHandler.write(&cSize, 1);
        }

    if(NIFT.IsLoaded())
        SaveHandler.writeSubRecord('TFIN', NIFT.value, NIFT.GetSize());

    if(ACBS.IsLoaded())
        SaveHandler.writeSubRecord('SBCA', &ACBS.value, ACBS.GetSize());

    for(UINT32 p = 0; p < SNAM.size(); p++)
        if(SNAM[p]->IsLoaded())
            SaveHandler.writeSubRecord('MANS', &SNAM[p]->value, SNAM[p]->GetSize());

    if(INAM.IsLoaded())
        SaveHandler.writeSubRecord('MANI', INAM.value, INAM.GetSize());

    if(SCRI.IsLoaded())
        SaveHandler.writeSubRecord('IRCS', SCRI.value, SCRI.GetSize());

    for(UINT32 p = 0; p < CNTO.size(); p++)
        if(CNTO[p]->IsLoaded())
            SaveHandler.writeSubRecord('OTNC', &CNTO[p]->value, sizeof(GENCNTO));

    if(AIDT.IsLoaded())
        SaveHandler.writeSubRecord('TDIA', &AIDT.value, AIDT.GetSize());

    for(UINT32 p = 0; p < PKID.size(); p++)
        SaveHandler.writeSubRecord('DIKP', &PKID[p], sizeof(UINT32));

    if(KFFZ.size())
        {
        cSize = 1; //final null terminator
        for(UINT32 p = 0; p < KFFZ.size(); p++)
            if(KFFZ[p].IsLoaded())
                cSize += KFFZ[p].GetSize();
        SaveHandler.writeSubRecord('ZFFK', NULL, cSize);
        for(UINT32 p = 0; p < KFFZ.size(); p++)
            if(KFFZ[p].IsLoaded())
                SaveHandler.write(KFFZ[p].value, KFFZ[p].GetSize());
        cSize = 0;
        //write final null terminator
        SaveHandler.write(&cSize, 1);
        }

    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());

    if(RNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANR', &RNAM.value, RNAM.GetSize());

    if(ZNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANZ', ZNAM.value, ZNAM.GetSize());

    if(TNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANT', &TNAM.value, TNAM.GetSize());

    if(BNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANB', &BNAM.value, BNAM.GetSize());

    if(WNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANW', &WNAM.value, WNAM.GetSize());

    if(CSCR.IsLoaded())
        SaveHandler.writeSubRecord('RCSC', CSCR.value, CSCR.GetSize());

    if(NAM0.IsLoaded())
        SaveHandler.writeSubRecord('0MAN', NAM0.value, NAM0.GetSize());

    if(NAM1.IsLoaded())
        SaveHandler.writeSubRecord('1MAN', NAM1.value, NAM1.GetSize());

    for(UINT32 p = 0; p < Sounds.size(); p++)
        {
        if(Sounds[p]->CSDT.IsLoaded())
            SaveHandler.writeSubRecord('TDSC', &Sounds[p]->CSDT.value, Sounds[p]->CSDT.GetSize());

        if(Sounds[p]->CSDI.IsLoaded())
            SaveHandler.writeSubRecord('IDSC', &Sounds[p]->CSDI.value, Sounds[p]->CSDI.GetSize());

        if(Sounds[p]->CSDC.IsLoaded())
            SaveHandler.writeSubRecord('CDSC', &Sounds[p]->CSDC.value, Sounds[p]->CSDC.GetSize());
        }
    return -1;
    }

bool CREARecord::operator ==(const CREARecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        FULL.equals(other.FULL) &&
        MODL == other.MODL &&
        NIFT == other.NIFT &&
        ACBS == other.ACBS &&
        INAM == other.INAM &&
        SCRI == other.SCRI &&
        AIDT == other.AIDT &&
        DATA == other.DATA &&
        RNAM == other.RNAM &&
        ZNAM == other.ZNAM &&
        TNAM == other.TNAM &&
        BNAM == other.BNAM &&
        WNAM == other.WNAM &&
        CSCR == other.CSCR &&
        NAM0.equalsi(other.NAM0) &&
        NAM1.equalsi(other.NAM1) &&
        SPLO.size() == other.SPLO.size() &&
        NIFZ.size() == other.NIFZ.size() &&
        SNAM.size() == other.SNAM.size() &&
        CNTO.size() == other.CNTO.size() &&
        PKID.size() == other.PKID.size() &&
        KFFZ.size() == other.KFFZ.size() &&
        Sounds.size() == other.Sounds.size())
        {
        //Record order doesn't matter on spells, so equality testing isn't easy
        //The proper solution would be to check each spell against every other spell to see if there's a one-to-one match
        //Perhaps using a disjoint set
        //Fix-up later
        for(UINT32 x = 0; x < SPLO.size(); ++x)
            if(SPLO[x] != other.SPLO[x])
                return false;

        //Not sure if record order matters on body parts, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < NIFZ.size(); ++x)
            if(NIFZ[x].equalsi(other.NIFZ[x]))
                return false;

        //Not sure if record order matters on factions, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < SNAM.size(); ++x)
            if(*SNAM[x] != *other.SNAM[x])
                return false;

        //Record order doesn't matter on items, so equality testing isn't easy
        //The proper solution would be to check each item against every other item to see if there's a one-to-one match
        //Fix-up later
        for(UINT32 x = 0; x < CNTO.size(); ++x)
            if(*CNTO[x] != *other.CNTO[x])
                return false;

        //Record order matters on ai packages, so equality testing is easy
        for(UINT32 x = 0; x < PKID.size(); ++x)
            if(PKID[x] != other.PKID[x])
                return false;

        //Not sure if record order matters on animations, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < KFFZ.size(); ++x)
            if(KFFZ[x].equalsi(other.KFFZ[x]))
                return false;

        //Not sure if record order matters on sounds, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < Sounds.size(); ++x)
            if(*Sounds[x] != *other.Sounds[x])
                return false;

        return true;
        }

    return false;
    }

bool CREARecord::operator !=(const CREARecord &other) const
    {
    return !(*this == other);
    }