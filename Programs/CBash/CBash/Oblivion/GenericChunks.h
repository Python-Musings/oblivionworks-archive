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
#pragma once
// BaseRecord.h
#include "../Common.h"

struct GENFLAG
    {
    UINT8   flags;

    GENFLAG();
    ~GENFLAG();

    bool operator ==(const GENFLAG &other) const;
    bool operator !=(const GENFLAG &other) const;
    };

struct GENXNAM
    {
    FORMID  faction;
    SINT32  mod;

    GENXNAM();
    ~GENXNAM();

    bool operator ==(const GENXNAM &other) const;
    bool operator !=(const GENXNAM &other) const;
    };

struct GENFID
    {
    FORMID  fid;

    GENFID();
    ~GENFID();

    bool operator ==(const GENFID &other) const;
    bool operator !=(const GENFID &other) const;
    };

struct GENSCHR
    {
    UINT8   unused1[4];
    UINT32  numRefs, compiledSize, lastIndex, scriptType;

    GENSCHR();
    ~GENSCHR();

    bool operator ==(const GENSCHR &other) const;
    bool operator !=(const GENSCHR &other) const;
    };

struct GENSCR_
    {
    FORMID_OR_UINT32 reference;
    bool    isSCRO;

    GENSCR_();
    ~GENSCR_();

    bool operator ==(const GENSCR_ &other) const;
    bool operator !=(const GENSCR_ &other) const;
    };

struct GENEFID
    {
    MGEFCODE_OR_UINT32 name;

    GENEFID();
    ~GENEFID();

    bool operator ==(const GENEFID &other) const;
    bool operator !=(const GENEFID &other) const;
    };

struct GENEFIT
    {
    MGEFCODE_OR_UINT32 name;
    UINT32  magnitude, area, duration, rangeType;
    FORMID_OR_MGEFCODE_OR_ACTORVALUE_OR_UINT32 actorValue;

    GENEFIT();
    ~GENEFIT();

    bool operator ==(const GENEFIT &other) const;
    bool operator !=(const GENEFIT &other) const;
    };

struct GENSCIT
    {
    FORMID_OR_MGEFCODE_OR_ACTORVALUE_OR_UINT32 script;
    UINT32  school;
    MGEFCODE_OR_UINT32 visual;
    UINT8   flags, unused1[3];

    GENSCIT();
    ~GENSCIT();

    bool operator ==(const GENSCIT &other) const;
    bool operator !=(const GENSCIT &other) const;
    };

struct OBMEEFME
    {
    UINT8   recordVersion, betaVersion, minorVersion, majorVersion;
    UINT8   efitParamInfo;
    UINT8   efixParamInfo;
    UINT8   reserved[0xA];

    OBMEEFME();
    ~OBMEEFME();

    bool operator ==(const OBMEEFME &other) const;
    bool operator !=(const OBMEEFME &other) const;
    };

struct OBMEEFIX
    {
    UINT32  efixOverrides;
    UINT32  efixFlags;
    FLOAT32 baseCost;
    ACTORVALUE resistAV;
    UINT8   reserved[0x10];

    OBMEEFIX();
    ~OBMEEFIX();

    bool operator ==(const OBMEEFIX &other) const;
    bool operator !=(const OBMEEFIX &other) const;
    };

struct OBMEEffect
    {
    ReqSubRecord<OBMEEFME> EFME;
    StringRecord EFII;
    OBMEEFIXSubRecord<OBMEEFIX> EFIX;

    bool operator ==(const OBMEEffect &other) const;
    bool operator !=(const OBMEEffect &other) const;
    };

struct GENEffect
    {
    ReqSubRecord<GENEFID> EFID;
    ReqSubRecord<GENEFIT> EFIT;
    OptSubRecord<GENSCIT> SCIT;
    StringRecord FULL;
    OptSubRecord<OBMEEffect> OBME;

    enum SCITFlags
        {
        fIsHostile = 0x01
        };

    enum eRanges
        {
        eRangeSelf    = 0,
        eRangeTouch   = 1,
        eRangeTarget  = 2
        };

    enum OBMEOverrideFlags
        {
        fOBME_IsHostile           = 0x00000001,
        fOBME_IsRecovers          = 0x00000002,
        fOBME_IsParamFlagA        = 0x00000004,
        fOBME_IsBeneficial        = 0x00000008,
        fOBME_IsFXPersists        = 0x00000400,
        fOBME_IsUnknownF          = 0x00004000,
        fOBME_IsNoRecast          = 0x00008000,
        fOBME_IsParamFlagB        = 0x00010000,
        fOBME_IsMagnitudeIsRange  = 0x00020000,
        fOBME_IsAtomicResistance  = 0x00040000,
        fOBME_IsParamFlagC        = 0x00080000,
        fOBME_IsParamFlagD        = 0x00100000,
        fOBME_IsDisabled          = 0x00400000,
        fOBME_IsUnknownO          = 0x00800000,
        fOBME_IsNoHitEffect       = 0x08000000,
        fOBME_IsPersistOnDeath    = 0x10000000,
        fOBME_IsExplodesWithForce = 0x20000000,
        fOBME_IsHidden            = 0x40000000
        //fOBME_IsMagnitudeIsFeet = 0x80000000 //Marked as Deprecated
        };

    enum OBMEUsingOverrideFlags
        {
        fOBME_IsUsingHostileOverride           = 0x00000001,
        fOBME_IsUsingRecoversOverride          = 0x00000002,
        fOBME_IsUsingParamFlagAOverride        = 0x00000004,
        fOBME_IsUsingBeneficialOverride        = 0x00000008,
        fOBME_IsUsingEFIXParamOverride         = 0x00000010,
        fOBME_IsUsingSchoolOverride            = 0x00000020,
        fOBME_IsUsingNameOverride              = 0x00000040,
        fOBME_IsUsingVFXCodeOverride           = 0x00000080,
        fOBME_IsUsingBaseCostOverride          = 0x00000100,
        fOBME_IsUsingResistAVOverride          = 0x00000200,
        fOBME_IsUsingFXPersistsOverride        = 0x00000400,
        fOBME_IsUsingIconOverride              = 0x00000800,
        fOBME_IsUsingDoesntTeachOverride       = 0x00001000,
        fOBME_IsUsingUnknownFOverride          = 0x00004000,
        fOBME_IsUsingNoRecastOverride          = 0x00008000,
        fOBME_IsUsingParamFlagBOverride        = 0x00010000,
        fOBME_IsUsingMagnitudeIsRangeOverride  = 0x00020000,
        fOBME_IsUsingAtomicResistanceOverride  = 0x00040000,
        fOBME_IsUsingParamFlagCOverride        = 0x00080000,
        fOBME_IsUsingParamFlagDOverride        = 0x00100000,
        fOBME_IsUsingDisabledOverride          = 0x00400000,
        fOBME_IsUsingUnknownOOverride          = 0x00800000,
        fOBME_IsUsingNoHitEffectOverride       = 0x08000000,
        fOBME_IsUsingPersistOnDeathOverride    = 0x10000000,
        fOBME_IsUsingExplodesWithForceOverride = 0x20000000,
        fOBME_IsUsingHiddenOverride            = 0x40000000
        //fOBME_IsUsingMagnitudeIsFeetOverride = 0x80000000 //Marked as Deprecated
        };

    bool operator ==(const GENEffect &other) const;
    bool operator !=(const GENEffect &other) const;

    bool IsHostile();
    void IsHostile(bool value);
    bool IsFlagMask(UINT8   Mask, bool Exact=false);
    void SetFlagMask(UINT8   Mask);

    bool IsRangeSelf();
    void IsRangeSelf(bool value);
    bool IsRangeTouch();
    void IsRangeTouch(bool value);
    bool IsRangeTarget();
    void IsRangeTarget(bool value);
    bool IsRange(UINT32  Mask);
    void SetRange(UINT32  Mask);

    bool OBME_IsUsingHostileOverride();
    void OBME_IsUsingHostileOverride(bool value);
    bool OBME_IsUsingRecoversOverride();
    void OBME_IsUsingRecoversOverride(bool value);
    bool OBME_IsUsingParamFlagAOverride();
    void OBME_IsUsingParamFlagAOverride(bool value);
    bool OBME_IsUsingBeneficialOverride();
    void OBME_IsUsingBeneficialOverride(bool value);
    bool OBME_IsUsingEFIXParamOverride();
    void OBME_IsUsingEFIXParamOverride(bool value);
    bool OBME_IsUsingSchoolOverride();
    void OBME_IsUsingSchoolOverride(bool value);
    bool OBME_IsUsingNameOverride();
    void OBME_IsUsingNameOverride(bool value);
    bool OBME_IsUsingVFXCodeOverride();
    void OBME_IsUsingVFXCodeOverride(bool value);
    bool OBME_IsUsingBaseCostOverride();
    void OBME_IsUsingBaseCostOverride(bool value);
    bool OBME_IsUsingResistAVOverride();
    void OBME_IsUsingResistAVOverride(bool value);
    bool OBME_IsUsingFXPersistsOverride();
    void OBME_IsUsingFXPersistsOverride(bool value);
    bool OBME_IsUsingIconOverride();
    void OBME_IsUsingIconOverride(bool value);
    bool OBME_IsUsingDoesntTeachOverride();
    void OBME_IsUsingDoesntTeachOverride(bool value);
    bool OBME_IsUsingUnknownFOverride();
    void OBME_IsUsingUnknownFOverride(bool value);
    bool OBME_IsUsingNoRecastOverride();
    void OBME_IsUsingNoRecastOverride(bool value);
    bool OBME_IsUsingParamFlagBOverride();
    void OBME_IsUsingParamFlagBOverride(bool value);
    bool OBME_IsUsingMagnitudeIsRangeOverride();
    void OBME_IsUsingMagnitudeIsRangeOverride(bool value);
    bool OBME_IsUsingAtomicResistanceOverride();
    void OBME_IsUsingAtomicResistanceOverride(bool value);
    bool OBME_IsUsingParamFlagCOverride();
    void OBME_IsUsingParamFlagCOverride(bool value);
    bool OBME_IsUsingParamFlagDOverride();
    void OBME_IsUsingParamFlagDOverride(bool value);
    bool OBME_IsUsingDisabledOverride();
    void OBME_IsUsingDisabledOverride(bool value);
    bool OBME_IsUsingUnknownOOverride();
    void OBME_IsUsingUnknownOOverride(bool value);
    bool OBME_IsUsingNoHitEffectOverride();
    void OBME_IsUsingNoHitEffectOverride(bool value);
    bool OBME_IsUsingPersistOnDeathOverride();
    void OBME_IsUsingPersistOnDeathOverride(bool value);
    bool OBME_IsUsingExplodesWithForceOverride();
    void OBME_IsUsingExplodesWithForceOverride(bool value);
    bool OBME_IsUsingHiddenOverride();
    void OBME_IsUsingHiddenOverride(bool value);

    bool OBME_IsOverrideFlagMask(UINT32 Mask, bool Exact=false);
    void OBME_SetOverrideFlagMask(UINT32 Mask);

    bool OBME_IsHostile();
    void OBME_IsHostile(bool value);
    bool OBME_IsRecovers();
    void OBME_IsRecovers(bool value);
    bool OBME_IsParamFlagA();
    void OBME_IsParamFlagA(bool value);
    bool OBME_IsBeneficial();
    void OBME_IsBeneficial(bool value);
    bool OBME_IsFXPersists();
    void OBME_IsFXPersists(bool value);
    bool OBME_IsUnknownF();
    void OBME_IsUnknownF(bool value);
    bool OBME_IsNoRecast();
    void OBME_IsNoRecast(bool value);
    bool OBME_IsParamFlagB();
    void OBME_IsParamFlagB(bool value);
    bool OBME_IsMagnitudeIsRange();
    void OBME_IsMagnitudeIsRange(bool value);
    bool OBME_IsAtomicResistance();
    void OBME_IsAtomicResistance(bool value);
    bool OBME_IsParamFlagC();
    void OBME_IsParamFlagC(bool value);
    bool OBME_IsParamFlagD();
    void OBME_IsParamFlagD(bool value);
    bool OBME_IsDisabled();
    void OBME_IsDisabled(bool value);
    bool OBME_IsUnknownO();
    void OBME_IsUnknownO(bool value);
    bool OBME_IsNoHitEffect();
    void OBME_IsNoHitEffect(bool value);
    bool OBME_IsPersistOnDeath();
    void OBME_IsPersistOnDeath(bool value);
    bool OBME_IsExplodesWithForce();
    void OBME_IsExplodesWithForce(bool value);
    bool OBME_IsHidden();
    void OBME_IsHidden(bool value);

    bool OBME_IsFlagMask(UINT32 Mask, bool Exact=false);
    void OBME_SetFlagMask(UINT32 Mask);
    };

struct GENANAM
    {
    UINT16  enchantPoints;

    GENANAM();
    ~GENANAM();

    bool operator ==(const GENANAM &other) const;
    bool operator !=(const GENANAM &other) const;
    };

struct GENUFLAG
    {
    UINT32  flags;

    GENUFLAG();
    ~GENUFLAG();

    bool operator ==(const GENUFLAG &other) const;
    bool operator !=(const GENUFLAG &other) const;
    };

struct GENWEIGHT
    {
    FLOAT32 weight;

    GENWEIGHT();
    ~GENWEIGHT();

    bool operator ==(const GENWEIGHT &other) const;
    bool operator !=(const GENWEIGHT &other) const;
    };

struct GENENIT
    {
    SINT32  value;
    UINT8   flags;
    UINT8   unused1[3];

    GENENIT();
    ~GENENIT();

    bool operator ==(const GENENIT &other) const;
    bool operator !=(const GENENIT &other) const;
    };

struct GENVALUEWEIGHT
    {
    UINT32  value;
    FLOAT32 weight;

    GENVALUEWEIGHT();
    ~GENVALUEWEIGHT();

    bool operator ==(const GENVALUEWEIGHT &other) const;
    bool operator !=(const GENVALUEWEIGHT &other) const;
    };

struct GENCNTO
    {
    FORMID  item;
    SINT32  count;

    GENCNTO();
    ~GENCNTO();

    bool operator ==(const GENCNTO &other) const;
    bool operator !=(const GENCNTO &other) const;
    };

struct GENACBS
    {
    UINT32  flags;
    UINT16  baseSpell, fatigue, barterGold;
    SINT16  level;
    UINT16  calcMin, calcMax;

    GENACBS();
    ~GENACBS();

    bool operator ==(const GENACBS &other) const;
    bool operator !=(const GENACBS &other) const;
    };

struct GENSNAM
    {
    FORMID  faction;
    UINT8   rank, unused1[3];

    GENSNAM();
    ~GENSNAM();

    bool operator ==(const GENSNAM &other) const;
    bool operator !=(const GENSNAM &other) const;
    };

struct GENAIDT
    {
    UINT8   aggression, confidence, energyLevel, responsibility;
    UINT32  flags;
    SINT8   trainSkill;
    UINT8   trainLevel, unused1[2];

    GENAIDT();
    ~GENAIDT();

    bool operator ==(const GENAIDT &other) const;
    bool operator !=(const GENAIDT &other) const;
    };

struct GENCTDA
    {
    UINT8   operType, unused1[3];
    FLOAT32 compValue;
    UINT32  ifunc;
    FORMID_OR_UINT32 param1, param2;
    UINT8   unused2[4];

    enum operTypeType
        {
        eEqual          = 0x00,
        eNotEqual       = 0x20,
        eGreater        = 0x40,
        eGreaterOrEqual = 0x60,
        eLess           = 0x80,
        eLessOrEqual    = 0xA0
        };
    enum operTypeFlag
        {
        fIsNone        = 0x00,
        fIsOr          = 0x01,
        fIsRunOnTarget = 0x02,
        fIsUseGlobal   = 0x04
        };

    GENCTDA();
    ~GENCTDA();

    bool operator ==(const GENCTDA &other) const;
    bool operator !=(const GENCTDA &other) const;

    bool IsEqual();
    void IsEqual(bool value);
    bool IsNotEqual();
    void IsNotEqual(bool value);
    bool IsGreater();
    void IsGreater(bool value);
    bool IsGreaterOrEqual();
    void IsGreaterOrEqual(bool value);
    bool IsLess();
    void IsLess(bool value);
    bool IsLessOrEqual();
    void IsLessOrEqual(bool value);
    bool IsType(UINT8 Type);
    void SetType(UINT8 Type);

    bool IsNone();
    void IsNone(bool value);
    bool IsOr();
    void IsOr(bool value);
    bool IsRunOnTarget();
    void IsRunOnTarget(bool value);
    bool IsUseGlobal();
    void IsUseGlobal(bool value);
    bool IsFlagMask(UINT8 Mask, bool Exact=false);
    void SetFlagMask(UINT8 Mask);
    };

struct GENCLR
    {
    UINT8   red, green, blue, unused1;

    GENCLR(UINT8   _red=0, UINT8   _green=0, UINT8   _blue=0, UINT8   _unused1=0);
    ~GENCLR();

    bool operator ==(const GENCLR &other) const;
    bool operator !=(const GENCLR &other) const;
    };

struct MODELMODB
    {
    FLOAT32 MODB;

    MODELMODB();
    ~MODELMODB();

    bool operator ==(const MODELMODB &other) const;
    bool operator !=(const MODELMODB &other) const;
    };

struct GENMODEL
    {
    ReqSubRecord<MODELMODB> MODB;
    StringRecord MODL;
    RawRecord MODT;

    bool operator ==(const GENMODEL &other) const;
    bool operator !=(const GENMODEL &other) const;
    };

struct GENRANK
    {
    SINT32  rank;

    GENRANK();
    ~GENRANK();

    bool operator ==(const GENRANK &other) const;
    bool operator !=(const GENRANK &other) const;
    };

struct GENXOWN
    {
    ReqSubRecord<GENFID> XOWN;
    SemiOptSubRecord<GENRANK> XRNK;
    OptSubRecord<GENFID> XGLB;

    bool operator ==(const GENXOWN &other) const;
    bool operator !=(const GENXOWN &other) const;
    };

struct GENXPCI
    {
    OptSubRecord<GENFID> XPCI;
    StringRecord FULL;

    bool operator ==(const GENXPCI &other) const;
    bool operator !=(const GENXPCI &other) const;
    };

struct GENXLOD
    {
    FLOAT32 lod1, lod2, lod3;

    GENXLOD();
    ~GENXLOD();

    bool operator ==(const GENXLOD &other) const;
    bool operator !=(const GENXLOD &other) const;
    };

struct GENXESP
    {
    FORMID  parent;
    UINT8   flags, unused1[3];

    GENXESP();
    ~GENXESP();

    bool operator ==(const GENXESP &other) const;
    bool operator !=(const GENXESP &other) const;
    };

struct GENXSCL
    {
    FLOAT32 scale;

    GENXSCL();
    ~GENXSCL();

    bool operator ==(const GENXSCL &other) const;
    bool operator !=(const GENXSCL &other) const;
    };

struct GENPOSDATA
    {
    FLOAT32 posX, posY, posZ, rotX, rotY, rotZ;

    GENPOSDATA();
    ~GENPOSDATA();

    bool operator ==(const GENPOSDATA &other) const;
    bool operator !=(const GENPOSDATA &other) const;
    };

struct GENPGRP
    {
    FLOAT32 x, y, z;
    UINT8   connections;
    UINT8   unused1[3];

    GENPGRP();
    ~GENPGRP();

    bool operator ==(const GENPGRP &other) const;
    bool operator !=(const GENPGRP &other) const;
    };

struct LVLLVLD
    {
    UINT8   chanceNone;

    LVLLVLD();
    ~LVLLVLD();

    bool operator ==(const LVLLVLD &other) const;
    bool operator !=(const LVLLVLD &other) const;
    };

struct LVLLVLO
    {
    SINT16  level;
    UINT8   unused1[2];
    UINT32  listId;
    SINT16  count;
    UINT8   unused2[2];

    LVLLVLO();
    ~LVLLVLO();

    bool operator ==(const LVLLVLO &other) const;
    bool operator !=(const LVLLVLO &other) const;
    };

struct MAGICOBME
    {
    UINT8   recordVersion, betaVersion, minorVersion, majorVersion;
    UINT8   reserved[0x1C];

    MAGICOBME();
    ~MAGICOBME();

    bool operator ==(const MAGICOBME &other) const;
    bool operator !=(const MAGICOBME &other) const;
    };

struct OBMEMAGIC
    {
    ReqSubRecord<MAGICOBME> OBME;
    RawRecord DATX;

    bool operator ==(const OBMEMAGIC &other) const;
    bool operator !=(const OBMEMAGIC &other) const;
    };