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
#include "..\..\Common.h"
#include "..\..\GenericRecord.h"
#include <vector>

class MGEFRecord : public Record
    {
    private:
        struct MGEFDATA
            {
            UINT32  flags;
            FLOAT32 baseCost;
            UINT32  associated;
            SINT32  school;
            UINT32  resistValue;
            UINT16  numCounters; //According to OBME
            UINT8   unused1[2];
            UINT32  light;
            FLOAT32 projectileSpeed;
            UINT32  effectShader, enchantEffect, castingSound, boltSound, hitSound, areaSound;
            FLOAT32 cefEnchantment, cefBarter;

            MGEFDATA();
            ~MGEFDATA();

            bool operator ==(const MGEFDATA &other) const;
            bool operator !=(const MGEFDATA &other) const;
            };

        struct OBMEEDDX
            {
            char    mgefCode[5];

            OBMEEDDX();
            ~OBMEEDDX();

            bool operator ==(const OBMEEDDX& other) const;
            bool operator !=(const OBMEEDDX &other) const;
            };

        struct MGEFOBME
            {
            UINT8   recordVersion, betaVersion, minorVersion, majorVersion;
            UINT8   mgefParamAInfo;
            UINT8   mgefParamBInfo;
            UINT8   reserved1[2];
            UINT32  handlerCode;
            UINT32  flags;
            UINT32  mgefParamB;
            UINT8   reserved2[0x1C];

            MGEFOBME();
            ~MGEFOBME();

            bool operator ==(const MGEFOBME &other) const;
            bool operator !=(const MGEFOBME &other) const;
            };

        struct OBMEMGEF
            {
            ReqSubRecord<MGEFOBME> OBME;
            //Uses SubRecord so that it isn't reloaded everytime the record is parsed
            SubRecord<OBMEEDDX> EDDX; //Is switched with the normal EDID on read and write so that the EDID field is a char *as expected
            RawRecord DATX;

            bool operator ==(const OBMEMGEF &other) const;
            bool operator !=(const OBMEMGEF &other) const;
            };

        //Note: the vanilla code discards mod changes to most flag bits
        //  only those listed as changeable below may be edited by non-obme mods
        // comments garnered from JRoush's OBME
        enum flagsFlags
            {
            fIsHostile            = 0x00000001,
            fIsRecover            = 0x00000002,
            fIsDetrimental        = 0x00000004, //OBME Deprecated, used for ValueModifier effects AV is decreased rather than increased
            fIsMagnitudeIsPercent = 0x00000008, //OBME Deprecated
            fIsSelf               = 0x00000010,
            fIsTouch              = 0x00000020,
            fIsTarget             = 0x00000040,
            fIsNoDuration         = 0x00000080,
            fIsNoMagnitude        = 0x00000100,
            fIsNoArea             = 0x00000200,
            fIsFXPersist          = 0x00000400, //Changeable
            fIsSpellmaking        = 0x00000800, //Changeable
            fIsEnchanting         = 0x00001000, //Changeable
            fIsNoIngredient       = 0x00002000, //Changeable
            fIsUnknownF           = 0x00004000, //no effects have this flag set
            fIsNoRecast           = 0x00008000, //no effects have this flag set
            fIsUseWeapon          = 0x00010000, //OBME Deprecated
            fIsUseArmor           = 0x00020000, //OBME Deprecated
            fIsUseCreature        = 0x00040000, //OBME Deprecated
            fIsUseSkill           = 0x00080000, //OBME Deprecated
            fIsUseAttr            = 0x00100000, //OBME Deprecated
            fIsPCHasEffect        = 0x00200000, //whether or not PC has effect, forced to zero during loading
            fIsDisabled           = 0x00400000, //Changeable, many (all?) methods that loop over effects ignore those with this flag. 
                                                //  Spells with an effect with this flag are apparently uncastable.

            fIsUnknownO           = 0x00800000, //Changeable, POSN,DISE - these effects have *only* this bit set,
                                                //  perhaps a flag for meta effects 

            fIsUseAV              = 0x01000000, //OBME Deprecated, Changeable, but once set (by default or by a previously loaded mod file)
                                                //  it cannot be unset by another mod, nor can the mgefParam be overriden

            fIsSprayType          = 0x02000000, //Changeable
            fIsBoltType           = 0x04000000, //Changeable
            fIsFogType            = 0x06000000, //Changeable
            fIsNoHitEffect        = 0x08000000, //Changeable, no hit shader on target 
            fIsPersistOnDeath     = 0x10000000, //Effect is not automatically removed when its target dies
            fIsExplodesWithForce  = 0x20000000, //causes explosion that can move loose objects (e.g. ragdolls)
            fIsMagnitudeIsLevel   = 0x40000000, //OBME Deprecated
            fIsMagnitudeIsFeet    = 0x80000000  //OBME Deprecated
            };

        //When OBME is in use, flagsFlags with the same bits are redefined to the following
        //  The new flags are also stored in a new field
        enum obmeFlags
            {
            fOBME_IsBeneficial       = 0x00000008,
            fOBME_IsMagnitudeIsRange = 0x00020000,
            fOBME_IsAtomicResistance = 0x00040000,
            fOBME_IsParamFlagA       = 0x00000004, //Meaning varies with effect handler
            fOBME_IsParamFlagB       = 0x00010000, //Meaning varies with effect handler
            fOBME_IsParamFlagC       = 0x00080000, //Meaning varies with effect handler
            fOBME_IsParamFlagD       = 0x00100000, //Meaning varies with effect handler
            fOBME_IsHidden           = 0x40000000
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        StringRecord DESC;
        StringRecord ICON;
        OptSubRecord<GENMODEL> MODL;
        ReqSubRecord<MGEFDATA> DATA;
        std::vector<MGEFCODE_OR_UINT32> ESCE;
        OptSubRecord<OBMEMGEF> OBME;

        MGEFRecord(unsigned char *_recData=NULL);
        MGEFRecord(MGEFRecord *srcRecord);
        ~MGEFRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsHostile();
        void   IsHostile(bool value);
        bool   IsRecover();
        void   IsRecover(bool value);
        bool   IsDetrimental();
        void   IsDetrimental(bool value);
        bool   IsMagnitudeIsPercent();
        void   IsMagnitudeIsPercent(bool value);
        bool   IsSelf();
        void   IsSelf(bool value);
        bool   IsTouch();
        void   IsTouch(bool value);
        bool   IsTarget();
        void   IsTarget(bool value);
        bool   IsNoDuration();
        void   IsNoDuration(bool value);
        bool   IsNoMagnitude();
        void   IsNoMagnitude(bool value);
        bool   IsNoArea();
        void   IsNoArea(bool value);
        bool   IsFXPersist();
        void   IsFXPersist(bool value);
        bool   IsSpellmaking();
        void   IsSpellmaking(bool value);
        bool   IsEnchanting();
        void   IsEnchanting(bool value);
        bool   IsNoIngredient();
        void   IsNoIngredient(bool value);
        bool   IsUnknownF();
        void   IsUnknownF(bool value);
        bool   IsNoRecast();
        void   IsNoRecast(bool value);
        bool   IsUseWeapon();
        void   IsUseWeapon(bool value);
        bool   IsUseArmor();
        void   IsUseArmor(bool value);
        bool   IsUseCreature();
        void   IsUseCreature(bool value);
        bool   IsUseSkill();
        void   IsUseSkill(bool value);
        bool   IsUseAttribute();
        void   IsUseAttribute(bool value);
        bool   IsUseAttr();
        void   IsUseAttr(bool value);
        bool   IsPCHasEffect();
        void   IsPCHasEffect(bool value);
        bool   IsDisabled();
        void   IsDisabled(bool value);
        bool   IsUnknownO();
        void   IsUnknownO(bool value);
        bool   IsUseActorValue();
        void   IsUseActorValue(bool value);
        bool   IsUseAV();
        void   IsUseAV(bool value);
        bool   IsSprayType();
        void   IsSprayType(bool value);
        bool   IsBoltType();
        void   IsBoltType(bool value);
        bool   IsFogType();
        void   IsFogType(bool value);
        bool   IsBallType();
        void   IsBallType(bool value);
        bool   IsNoHitEffect();
        void   IsNoHitEffect(bool value);
        bool   IsPersistOnDeath();
        void   IsPersistOnDeath(bool value);
        bool   IsExplodesWithForce();
        void   IsExplodesWithForce(bool value);
        bool   IsMagnitudeIsLevel();
        void   IsMagnitudeIsLevel(bool value);
        bool   IsMagnitudeIsFeet();
        void   IsMagnitudeIsFeet(bool value);
        bool   IsFlagMask(UINT32 Mask, bool Exact=false);
        void   SetFlagMask(UINT32 Mask);

        //OBME flags
        bool   IsBeneficial();
        void   IsBeneficial(bool value);
        bool   IsMagnitudeIsRange();
        void   IsMagnitudeIsRange(bool value);
        bool   IsAtomicResistance();
        void   IsAtomicResistance(bool value);
        bool   IsParamFlagA();
        void   IsParamFlagA(bool value);
        bool   IsParamFlagB();
        void   IsParamFlagB(bool value);
        bool   IsParamFlagC();
        void   IsParamFlagC(bool value);
        bool   IsParamFlagD();
        void   IsParamFlagD(bool value);
        bool   IsHidden();
        void   IsHidden(bool value);
        bool   IsOBMEFlagMask(UINT32 Mask, bool Exact=false);
        void   SetOBMEFlagMask(UINT32 Mask);

        UINT32 GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, UINT32 WhichAttribute=0);
        void * GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool   SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, UINT32 ArraySize=0);
        void   DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        UINT32 GetSize(bool forceCalc=false);
        UINT32 GetType();
        STRING GetStrType();
        bool   IsKeyedByEditorID();

        SINT32 ParseRecord(unsigned char *buffer, const UINT32 &recSize);
        SINT32 Unload();
        SINT32 WriteRecord(_FileHandler &SaveHandler);

        bool operator ==(const MGEFRecord &other) const;
        bool operator !=(const MGEFRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(MGEFRecord *)other;}
    };