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
#include "MGEFRecord.h"
#include <vector>

MGEFRecord::MGEFDATA::MGEFDATA():
    flags(0),
    baseCost(0.0f),
    associated(0),
    school(0),
    resistValue(0xFFFFFFFF),
    numCounters(0),
    light(0),
    projectileSpeed(0.0f),
    effectShader(0),
    enchantEffect(0),
    castingSound(0),
    boltSound(0),
    hitSound(0),
    areaSound(0),
    cefEnchantment(0.0f),
    cefBarter(0.0f)
    {
    memset(&unused1, 0x00, 2);
    }

MGEFRecord::MGEFDATA::~MGEFDATA()
    {
    //
    }

bool MGEFRecord::MGEFDATA::operator ==(const MGEFDATA &other) const
    {
    return (flags == other.flags &&
            AlmostEqual(baseCost,other.baseCost,2) &&
            associated == other.associated &&
            school == other.school &&
            resistValue == other.resistValue &&
            light == other.light &&
            AlmostEqual(projectileSpeed,other.projectileSpeed,2) &&
            effectShader == other.effectShader &&
            enchantEffect == other.enchantEffect &&
            castingSound == other.castingSound &&
            boltSound == other.boltSound &&
            hitSound == other.hitSound &&
            areaSound == other.areaSound &&
            AlmostEqual(cefEnchantment,other.cefEnchantment,2) &&
            AlmostEqual(cefBarter,other.cefBarter,2));
    }

bool MGEFRecord::MGEFDATA::operator !=(const MGEFDATA &other) const
    {
    return !(*this == other);
    }

MGEFRecord::OBMEEDDX::OBMEEDDX()
    {
    memset(&mgefCode[0], 0x00, 5);
    }

MGEFRecord::OBMEEDDX::~OBMEEDDX()
    {
    //
    }

bool MGEFRecord::OBMEEDDX::operator ==(const OBMEEDDX& other) const
    {
    return mgefCode == other.mgefCode;
    }

bool MGEFRecord::OBMEEDDX::operator !=(const OBMEEDDX &other) const
    {
    return !(*this == other);
    }

MGEFRecord::MGEFOBME::MGEFOBME():
    recordVersion(0),
    betaVersion(0),
    minorVersion(0),
    majorVersion(0),
    mgefParamAInfo(0),
    mgefParamBInfo(0),
    handlerCode(0),
    flags(0),
    mgefParamB(0)
    {
    memset(&reserved1, 0x00, 2);
    memset(&reserved2, 0x00, 0x1C);
    }

MGEFRecord::MGEFOBME::~MGEFOBME()
    {
    //
    }

bool MGEFRecord::MGEFOBME::operator ==(const MGEFOBME &other) const
    {
    return (recordVersion == other.recordVersion &&
            betaVersion == other.betaVersion &&
            minorVersion == other.minorVersion &&
            majorVersion == other.majorVersion &&
            mgefParamAInfo == other.mgefParamAInfo &&
            mgefParamBInfo == other.mgefParamBInfo &&
            reserved1 == other.reserved1 &&
            handlerCode == other.handlerCode &&
            flags == other.flags &&
            mgefParamB == other.mgefParamB &&
            reserved2 == other.reserved2);
    }

bool MGEFRecord::MGEFOBME::operator !=(const MGEFOBME &other) const
    {
    return !(*this == other);
    }

bool MGEFRecord::OBMEMGEF::operator ==(const OBMEMGEF &other) const
    {
    return (OBME == other.OBME &&
            EDDX == other.EDDX &&
            DATX == other.DATX);
    }

bool MGEFRecord::OBMEMGEF::operator !=(const OBMEMGEF &other) const
    {
    return !(*this == other);
    }

MGEFRecord::MGEFRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

MGEFRecord::MGEFRecord(MGEFRecord *srcRecord):
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
    DESC = srcRecord->DESC;
    ICON = srcRecord->ICON;
    if(srcRecord->MODL.IsLoaded())
        {
        MODL.Load();
        MODL->MODB = srcRecord->MODL->MODB;
        MODL->MODL = srcRecord->MODL->MODL;
        MODL->MODT = srcRecord->MODL->MODT;
        }
    DATA = srcRecord->DATA;
    ESCE = srcRecord->ESCE;
    if(srcRecord->OBME.IsLoaded())
        {
        OBME.Load();
        OBME->OBME = srcRecord->OBME->OBME;
        OBME->EDDX = srcRecord->OBME->EDDX;
        OBME->DATX = srcRecord->OBME->DATX;
        }
    }

MGEFRecord::~MGEFRecord()
    {
    //
    }

bool MGEFRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(OBME.IsLoaded())
        {
        //Conditional resolution of mgefCode's based on JRoush's OBME mod
        //It's resolved just like a formID, except it uses the lower byte instead of the upper
        if((MGEFCODE)OBME->EDDX.value.mgefCode >= 0x80000000)
            {
            MGEFCODE tempMgef = (MGEFCODE)OBME->EDDX.value.mgefCode;
            op.AcceptMGEF(tempMgef);
            memcpy(&OBME->EDDX.value.mgefCode[0], &tempMgef, 4);
            OBME->EDDX.value.mgefCode[4] = 0;
            }

        if(OBME->OBME.IsLoaded())
            {
            switch(OBME->OBME.value.mgefParamAInfo)
                {
                case 1: //It's a regular formID, so nothing fancy.
                    op.Accept(DATA.value.associated);
                    break;
                case 2: //It's a mgefCode, and not a formID at all.
                    //Conditional resolution of mgefCode's based on JRoush's OBME mod
                    //It's resolved just like a formID, except it uses the lower byte instead of the upper
                    if(DATA.value.associated >= 0x80000000)
                        op.AcceptMGEF(DATA.value.associated);
                    break;
                case 3: //It's an actor value, and not a formID at all.
                    //Conditional resolution of av's based on JRoush's OBME/AV mod(s)
                    //It's resolved just like a formID
                    if(DATA.value.associated >= 0x800)
                        op.Accept(DATA.value.associated);
                    break;
                default: //It's not a formID, mgefCode, or fancied up actor value
                    //so do nothing
                    break;
                }
            switch(OBME->OBME.value.mgefParamBInfo)
                {
                case 1: //It's a regular formID, so nothing fancy.
                    op.Accept(OBME->OBME.value.mgefParamB);
                    break;
                case 2: //It's a mgefCode, and not a formID at all.
                    //Conditional resolution of mgefCode's based on JRoush's OBME mod
                    //It's resolved just like a formID, except it uses the lower byte instead of the upper
                    if(OBME->OBME.value.mgefParamB >= 0x80000000)
                        op.AcceptMGEF(OBME->OBME.value.mgefParamB);
                    break;
                case 3: //It's an actor value, and not a formID at all.
                    //Conditional resolution of av's based on JRoush's OBME/AV mod(s)
                    //It's resolved just like a formID
                    if(OBME->OBME.value.mgefParamB >= 0x800)
                        op.Accept(OBME->OBME.value.mgefParamB);
                    break;
                default: //It's not a formID, mgefCode, or fancied up actor value
                    //so do nothing
                    break;
                }
            }

        op.Accept(DATA.value.light);
        op.Accept(DATA.value.effectShader);
        op.Accept(DATA.value.enchantEffect);
        op.Accept(DATA.value.castingSound);
        op.Accept(DATA.value.boltSound);
        op.Accept(DATA.value.hitSound);
        op.Accept(DATA.value.areaSound);

        if(DATA.value.resistValue >= 0x800)
            op.Accept(DATA.value.resistValue);

        for(UINT32 x = 0; x < ESCE.size(); ++x)
            if(ESCE[x] >= 0x80000000)
                op.AcceptMGEF(ESCE[x]);
        }
    else
        {
        op.Accept(DATA.value.associated);
        op.Accept(DATA.value.light);
        op.Accept(DATA.value.effectShader);
        op.Accept(DATA.value.enchantEffect);
        op.Accept(DATA.value.castingSound);
        op.Accept(DATA.value.boltSound);
        op.Accept(DATA.value.hitSound);
        op.Accept(DATA.value.areaSound);
        }

    return op.Stop();
    }

bool MGEFRecord::IsHostile()
    {
    return (DATA.value.flags & fIsHostile) != 0;
    }

void MGEFRecord::IsHostile(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsHostile) : (DATA.value.flags & ~fIsHostile);
    }

bool MGEFRecord::IsRecover()
    {
    return (DATA.value.flags & fIsRecover) != 0;
    }

void MGEFRecord::IsRecover(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsRecover) : (DATA.value.flags & ~fIsRecover);
    }

bool MGEFRecord::IsDetrimental()
    {
    return (DATA.value.flags & fIsDetrimental) != 0;
    }

void MGEFRecord::IsDetrimental(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsDetrimental) : (DATA.value.flags & ~fIsDetrimental);
    }

bool MGEFRecord::IsMagnitudeIsPercent()
    {
    return (DATA.value.flags & fIsMagnitudeIsPercent) != 0;
    }

void MGEFRecord::IsMagnitudeIsPercent(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsMagnitudeIsPercent) : (DATA.value.flags & ~fIsMagnitudeIsPercent);
    }

bool MGEFRecord::IsSelf()
    {
    return (DATA.value.flags & fIsSelf) != 0;
    }

void MGEFRecord::IsSelf(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsSelf) : (DATA.value.flags & ~fIsSelf);
    }

bool MGEFRecord::IsTouch()
    {
    return (DATA.value.flags & fIsTouch) != 0;
    }

void MGEFRecord::IsTouch(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsTouch) : (DATA.value.flags & ~fIsTouch);
    }

bool MGEFRecord::IsTarget()
    {
    return (DATA.value.flags & fIsTarget) != 0;
    }

void MGEFRecord::IsTarget(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsTarget) : (DATA.value.flags & ~fIsTarget);
    }

bool MGEFRecord::IsNoDuration()
    {
    return (DATA.value.flags & fIsNoDuration) != 0;
    }

void MGEFRecord::IsNoDuration(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsNoDuration) : (DATA.value.flags & ~fIsNoDuration);
    }

bool MGEFRecord::IsNoMagnitude()
    {
    return (DATA.value.flags & fIsNoMagnitude) != 0;
    }

void MGEFRecord::IsNoMagnitude(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsNoMagnitude) : (DATA.value.flags & ~fIsNoMagnitude);
    }

bool MGEFRecord::IsNoArea()
    {
    return (DATA.value.flags & fIsNoArea) != 0;
    }

void MGEFRecord::IsNoArea(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsNoArea) : (DATA.value.flags & ~fIsNoArea);
    }

bool MGEFRecord::IsFXPersist()
    {
    return (DATA.value.flags & fIsFXPersist) != 0;
    }

void MGEFRecord::IsFXPersist(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsFXPersist) : (DATA.value.flags & ~fIsFXPersist);
    }

bool MGEFRecord::IsSpellmaking()
    {
    return (DATA.value.flags & fIsSpellmaking) != 0;
    }

void MGEFRecord::IsSpellmaking(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsSpellmaking) : (DATA.value.flags & ~fIsSpellmaking);
    }

bool MGEFRecord::IsEnchanting()
    {
    return (DATA.value.flags & fIsEnchanting) != 0;
    }

void MGEFRecord::IsEnchanting(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsEnchanting) : (DATA.value.flags & ~fIsEnchanting);
    }

bool MGEFRecord::IsNoIngredient()
    {
    return (DATA.value.flags & fIsNoIngredient) != 0;
    }

void MGEFRecord::IsNoIngredient(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsNoIngredient) : (DATA.value.flags & ~fIsNoIngredient);
    }

bool MGEFRecord::IsUnknownF()
    {
    return (DATA.value.flags & fIsUnknownF) != 0;
    }

void MGEFRecord::IsUnknownF(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsUnknownF) : (DATA.value.flags & ~fIsUnknownF);
    }

bool MGEFRecord::IsNoRecast()
    {
    return (DATA.value.flags & fIsNoRecast) != 0;
    }

void MGEFRecord::IsNoRecast(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsNoRecast) : (DATA.value.flags & ~fIsNoRecast);
    }

bool MGEFRecord::IsUseWeapon()
    {
    return (DATA.value.flags & fIsUseWeapon) != 0;
    }

void MGEFRecord::IsUseWeapon(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsUseWeapon) : (DATA.value.flags & ~fIsUseWeapon);
    }

bool MGEFRecord::IsUseArmor()
    {
    return (DATA.value.flags & fIsUseArmor) != 0;
    }

void MGEFRecord::IsUseArmor(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsUseArmor) : (DATA.value.flags & ~fIsUseArmor);
    }

bool MGEFRecord::IsUseCreature()
    {
    return (DATA.value.flags & fIsUseCreature) != 0;
    }

void MGEFRecord::IsUseCreature(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsUseCreature) : (DATA.value.flags & ~fIsUseCreature);
    }

bool MGEFRecord::IsUseSkill()
    {
    return (DATA.value.flags & fIsUseSkill) != 0;
    }

void MGEFRecord::IsUseSkill(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsUseSkill) : (DATA.value.flags & ~fIsUseSkill);
    }

bool MGEFRecord::IsUseAttribute()
    {
    return (DATA.value.flags & fIsUseAttr) != 0;
    }

void MGEFRecord::IsUseAttribute(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsUseAttr) : (DATA.value.flags & ~fIsUseAttr);
    }

bool MGEFRecord::IsUseAttr()
    {
    return IsUseAttribute();
    }

void MGEFRecord::IsUseAttr(bool value)
    {
    IsUseAttribute(value);
    }

bool MGEFRecord::IsPCHasEffect()
    {
    return (DATA.value.flags & fIsPCHasEffect) != 0;
    }

void MGEFRecord::IsPCHasEffect(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsPCHasEffect) : (DATA.value.flags & ~fIsPCHasEffect);
    }

bool MGEFRecord::IsDisabled()
    {
    return (DATA.value.flags & fIsUseAttr) != 0;
    }

void MGEFRecord::IsDisabled(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsDisabled) : (DATA.value.flags & ~fIsDisabled);
    }

bool MGEFRecord::IsUnknownO()
    {
    return (DATA.value.flags & fIsUnknownO) != 0;
    }

void MGEFRecord::IsUnknownO(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsUnknownO) : (DATA.value.flags & ~fIsUnknownO);
    }

bool MGEFRecord::IsUseActorValue()
    {
    return (DATA.value.flags & fIsUseAV) != 0;
    }

void MGEFRecord::IsUseActorValue(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsUseAV) : (DATA.value.flags & ~fIsUseAV);
    }

bool MGEFRecord::IsUseAV()
    {
    return IsUseActorValue();
    }

void MGEFRecord::IsUseAV(bool value)
    {
    IsUseActorValue(value);
    }

bool MGEFRecord::IsSprayType()
    {
    return ((DATA.value.flags & fIsSprayType) != 0) && ((DATA.value.flags & fIsBoltType) == 0);
    }

void MGEFRecord::IsSprayType(bool value)
    {
    if(value)
        {
        DATA.value.flags &= ~fIsFogType;
        DATA.value.flags |= fIsSprayType;
        }
    else if(IsSprayType())
        IsBallType(true);
    }

bool MGEFRecord::IsBoltType()
    {
    return ((DATA.value.flags & fIsBoltType) != 0) && ((DATA.value.flags & fIsSprayType) == 0);
    }

void MGEFRecord::IsBoltType(bool value)
    {
    if(value)
        {
        DATA.value.flags &= ~fIsFogType;
        DATA.value.flags |= fIsBoltType;
        }
    else if(IsBoltType())
        IsBallType(true);
    }

bool MGEFRecord::IsFogType()
    {
    return (DATA.value.flags & fIsFogType) == fIsFogType;
    }

void MGEFRecord::IsFogType(bool value)
    {
    if(value)
        DATA.value.flags |= fIsFogType;
    else if(IsFogType())
        IsBallType(true);
    }

bool MGEFRecord::IsBallType()
    {
    return (DATA.value.flags & fIsFogType) == 0;
    }

void MGEFRecord::IsBallType(bool value)
    {
    if(value)
        DATA.value.flags &= ~fIsFogType;
    else if(IsBallType())
        IsBoltType(true);
    }

bool MGEFRecord::IsNoHitEffect()
    {
    return (DATA.value.flags & fIsNoHitEffect) != 0;
    }

void MGEFRecord::IsNoHitEffect(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsNoHitEffect) : (DATA.value.flags & ~fIsNoHitEffect);
    }

bool MGEFRecord::IsPersistOnDeath()
    {
    return (DATA.value.flags & fIsPersistOnDeath) != 0;
    }

void MGEFRecord::IsPersistOnDeath(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsPersistOnDeath) : (DATA.value.flags & ~fIsPersistOnDeath);
    }

bool MGEFRecord::IsExplodesWithForce()
    {
    return (DATA.value.flags & fIsExplodesWithForce) != 0;
    }

void MGEFRecord::IsExplodesWithForce(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsExplodesWithForce) : (DATA.value.flags & ~fIsExplodesWithForce);
    }

bool MGEFRecord::IsMagnitudeIsLevel()
    {
    return (DATA.value.flags & fIsMagnitudeIsLevel) != 0;
    }

void MGEFRecord::IsMagnitudeIsLevel(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsMagnitudeIsLevel) : (DATA.value.flags & ~fIsMagnitudeIsLevel);
    }

bool MGEFRecord::IsMagnitudeIsFeet()
    {
    return (DATA.value.flags & fIsMagnitudeIsFeet) != 0;
    }

void MGEFRecord::IsMagnitudeIsFeet(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    DATA.value.flags = value ? (DATA.value.flags | fIsMagnitudeIsFeet) : (DATA.value.flags & ~fIsMagnitudeIsFeet);
    }

bool MGEFRecord::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void MGEFRecord::SetFlagMask(UINT32 Mask)
    {
    DATA.value.flags = Mask;
    }

//OBME flags
bool MGEFRecord::IsBeneficial()
    {
    return (OBME->OBME.value.flags & fOBME_IsBeneficial) != 0;
    }

void MGEFRecord::IsBeneficial(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    OBME->OBME.value.flags = value ? (OBME->OBME.value.flags | fOBME_IsBeneficial) : (OBME->OBME.value.flags & ~fOBME_IsBeneficial);
    }

bool MGEFRecord::IsMagnitudeIsRange()
    {
    return (OBME->OBME.value.flags & fOBME_IsMagnitudeIsRange) != 0;
    }

void MGEFRecord::IsMagnitudeIsRange(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    OBME->OBME.value.flags = value ? (OBME->OBME.value.flags | fOBME_IsMagnitudeIsRange) : (OBME->OBME.value.flags & ~fOBME_IsMagnitudeIsRange);
    }

bool MGEFRecord::IsAtomicResistance()
    {
    return (OBME->OBME.value.flags & fOBME_IsAtomicResistance) != 0;
    }

void MGEFRecord::IsAtomicResistance(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    OBME->OBME.value.flags = value ? (OBME->OBME.value.flags | fOBME_IsAtomicResistance) : (OBME->OBME.value.flags & ~fOBME_IsAtomicResistance);
    }

bool MGEFRecord::IsParamFlagA()
    {
    return (OBME->OBME.value.flags & fOBME_IsParamFlagA) != 0;
    }

void MGEFRecord::IsParamFlagA(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    OBME->OBME.value.flags = value ? (OBME->OBME.value.flags | fOBME_IsParamFlagA) : (OBME->OBME.value.flags & ~fOBME_IsParamFlagA);
    }

bool MGEFRecord::IsParamFlagB()
    {
    return (OBME->OBME.value.flags & fOBME_IsParamFlagB) != 0;
    }

void MGEFRecord::IsParamFlagB(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    OBME->OBME.value.flags = value ? (OBME->OBME.value.flags | fOBME_IsParamFlagB) : (OBME->OBME.value.flags & ~fOBME_IsParamFlagB);
    }

bool MGEFRecord::IsParamFlagC()
    {
    return (OBME->OBME.value.flags & fOBME_IsParamFlagC) != 0;
    }

void MGEFRecord::IsParamFlagC(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    OBME->OBME.value.flags = value ? (OBME->OBME.value.flags | fOBME_IsParamFlagC) : (OBME->OBME.value.flags & ~fOBME_IsParamFlagC);
    }

bool MGEFRecord::IsParamFlagD()
    {
    return (OBME->OBME.value.flags & fOBME_IsParamFlagD) != 0;
    }

void MGEFRecord::IsParamFlagD(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    OBME->OBME.value.flags = value ? (OBME->OBME.value.flags | fOBME_IsParamFlagD) : (OBME->OBME.value.flags & ~fOBME_IsParamFlagD);
    }

bool MGEFRecord::IsHidden()
    {
    return (OBME->OBME.value.flags & fOBME_IsHidden) != 0;
    }

void MGEFRecord::IsHidden(bool value)
    {
    if(!OBME.IsLoaded()) return; //Only settable if OBME is enabled
    OBME->OBME.value.flags = value ? (OBME->OBME.value.flags | fOBME_IsHidden) : (OBME->OBME.value.flags & ~fOBME_IsHidden);
    }

bool MGEFRecord::IsOBMEFlagMask(UINT32 Mask, bool Exact)
    {
    if(!OBME.IsLoaded()) return false;
    return Exact ? ((OBME->OBME.value.flags & Mask) == Mask) : ((OBME->OBME.value.flags & Mask) != 0);
    }

void MGEFRecord::SetOBMEFlagMask(UINT32 Mask)
    {
    OBME.Load();
    OBME->OBME.value.flags = Mask;
    }

UINT32 MGEFRecord::GetSize(bool forceCalc)
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

    if(OBME.IsLoaded())
        {
        if(OBME->OBME.IsLoaded())
            TotSize += OBME->OBME.GetSize() + 6;

        if(OBME->EDDX.IsLoaded())
            TotSize += OBME->EDDX.GetSize() + 7;

        if(OBME->DATX.IsLoaded())
            {
            cSize = OBME->DATX.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(FULL.IsLoaded())
        {
        cSize = FULL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(DESC.IsLoaded())
        {
        cSize = DESC.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
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

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(ESCE.size())
        {
        cSize = (sizeof(UINT32) * (UINT32)ESCE.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    return TotSize;
    }

UINT32 MGEFRecord::GetType()
    {
    return 'FEGM';
    }


STRING MGEFRecord::GetStrType()
    {
    return "MGEF";
    }

bool MGEFRecord::IsKeyedByEditorID()
    {
    return true;
    }

SINT32 MGEFRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'EMBO':
                OBME.Load();
                OBME->OBME.Read(buffer, subSize, curPos);
                break;
            case 'XDDE':
                OBME.Load();
                memcpy(&OBME->EDDX.value.mgefCode[0], EDID.value, 4);
                OBME->EDDX.value.mgefCode[4] = 0x00;
                EDID.Unload();
                EDID.Read(buffer, subSize, curPos);
                break;
            case 'LLUF':
                FULL.Read(buffer, subSize, curPos);
                break;
            case 'CSED':
                DESC.Read(buffer, subSize, curPos);
                break;
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
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
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'XTAD':
                OBME.Load();
                OBME->DATX.Read(buffer, subSize, curPos);
                break;
            case 'ECSE':
                if(subSize % sizeof(UINT32) == 0)
                    {
                    if(subSize == 0)
                        break;
                    ESCE.resize(subSize / sizeof(UINT32));
                    _readBuffer(&ESCE[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized ESCE size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  MGEF: Unknown subType = %04X\n", subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    //MGEFs should always be loaded since they're keyed by editorID (or mgefCode)
    //By marking it as changed, it prevents the record from being unloaded
    //IsChanged(true);
    return 0;
    }

SINT32 MGEFRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    if(OBME.IsLoaded())
        EDID.Unload(); //Don't unload OBME since OBME->EDDX.value.mgefCode is used to index the record
    else
        OBME.Unload(); //Don't unload EDID since it is used to index the record

    FULL.Unload();
    DESC.Unload();
    ICON.Unload();
    MODL.Unload();
    DATA.Unload();
    ESCE.clear();
    return 1;
    }

SINT32 MGEFRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(OBME.IsLoaded())
        {
        //EDDX and EDID are switched internally for consistency
        //So EDDX is written as if it was the EDID chunk, and vice versa
        //Hence the mismatched type in writeSubRecord
        if(OBME->EDDX.IsLoaded())
            SaveHandler.writeSubRecord('DIDE', &OBME->EDDX.value.mgefCode[0], 5);
        if(OBME->OBME.IsLoaded())
            SaveHandler.writeSubRecord('EMBO', &OBME->OBME.value, OBME->OBME.GetSize());
        if(EDID.IsLoaded())
            SaveHandler.writeSubRecord('XDDE', EDID.value, EDID.GetSize());
        if(FULL.IsLoaded())
            SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
        if(DESC.IsLoaded())
            SaveHandler.writeSubRecord('CSED', DESC.value, DESC.GetSize());
        if(ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
        if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
            {
            SaveHandler.writeSubRecord('LDOM', MODL->MODL.value, MODL->MODL.GetSize());
            if(MODL->MODB.IsLoaded())
                SaveHandler.writeSubRecord('BDOM', &MODL->MODB.value, MODL->MODB.GetSize());
            if(MODL->MODT.IsLoaded())
                SaveHandler.writeSubRecord('TDOM', MODL->MODT.value, MODL->MODT.GetSize());
            }
        if(DATA.IsLoaded())
            {
            DATA.value.numCounters = (UINT16)ESCE.size(); //Just to ensure that the proper value is written
            SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
            }
        if(OBME->DATX.IsLoaded())
            SaveHandler.writeSubRecord('XTAD', OBME->DATX.value, OBME->DATX.GetSize());
        if(ESCE.size())
            SaveHandler.writeSubRecord('ECSE', &ESCE[0], (UINT32)ESCE.size() * sizeof(UINT32));
        }
    else
        {
        if(EDID.IsLoaded())
            SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
        if(FULL.IsLoaded())
            SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
        if(DESC.IsLoaded())
            SaveHandler.writeSubRecord('CSED', DESC.value, DESC.GetSize());
        if(ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
        if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
            {
            SaveHandler.writeSubRecord('LDOM', MODL->MODL.value, MODL->MODL.GetSize());
            if(MODL->MODB.IsLoaded())
                SaveHandler.writeSubRecord('BDOM', &MODL->MODB.value, MODL->MODB.GetSize());
            if(MODL->MODT.IsLoaded())
                SaveHandler.writeSubRecord('TDOM', MODL->MODT.value, MODL->MODT.GetSize());
            }
        if(DATA.IsLoaded())
            {
            DATA.value.numCounters = (UINT16)ESCE.size(); //Just to ensure that the proper value is written
            SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
            }
        if(ESCE.size())
            SaveHandler.writeSubRecord('ECSE', &ESCE[0], (UINT32)ESCE.size() * sizeof(UINT32));
        }

    return -1;
    }

bool MGEFRecord::operator ==(const MGEFRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        FULL.equals(other.FULL) &&
        DESC.equals(other.DESC) &&
        ICON.equalsi(other.ICON) &&
        MODL == other.MODL &&
        DATA == other.DATA &&
        OBME == other.OBME &&
        ESCE.size() == other.ESCE.size())
        {
        //Record order doesn't matter on counter effects, so equality testing isn't easy
        //The proper solution would be to check each counter effect against every other counter effect to see if there's a one-to-one match
        //Fix-up later
        for(UINT32 x = 0; x < ESCE.size(); ++x)
            if(ESCE[x] != other.ESCE[x])
                return false;
        return true;
        }

    return false;
    }

bool MGEFRecord::operator !=(const MGEFRecord &other) const
    {
    return !(*this == other);
    }