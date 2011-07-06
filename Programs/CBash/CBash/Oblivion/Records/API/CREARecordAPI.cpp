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
#include "..\..\..\Common.h"
#include "..\CREARecord.h"

UINT32 CREARecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
    {
    switch(FieldID)
        {
        case 0: //recType
            return GetType();
        case 1: //flags1
            return UINT32_FLAG_FIELD;
        case 2: //fid
            return FORMID_FIELD;
        case 3: //flags2
            return UINT32_FLAG_FIELD;
        case 4: //eid
            return ISTRING_FIELD;
        case 5: //full
            return STRING_FIELD;
        case 6: //modPath
            return ISTRING_FIELD;
        case 7: //modb
            return FLOAT32_FIELD;
        case 8: //modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MODL.IsLoaded() ? MODL->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 9: //spells
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)SPLO.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 10: //bodyParts
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return ISTRING_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)NIFZ.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 11: //nift_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return NIFT.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 12: //flags
            return UINT32_FLAG_FIELD;
        case 13: //baseSpell
            return UINT16_FIELD;
        case 14: //fatigue
            return UINT16_FIELD;
        case 15: //barterGold
            return UINT16_FIELD;
        case 16: //level
            return SINT16_FIELD;
        case 17: //calcMin
            return UINT16_FIELD;
        case 18: //calcMax
            return UINT16_FIELD;
        case 19: //factions
            if(ListFieldID == 0) //factions
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)SNAM.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= SNAM.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //faction
                    return FORMID_FIELD;
                case 2: //rank
                    return UINT8_FIELD;
                case 3: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 3;
                        default:
                            return UNKNOWN_FIELD;
                        }
                default:
                    return UNKNOWN_FIELD;
                }
        case 20: //deathItem
            return FORMID_FIELD;
        case 21: //script
            return FORMID_FIELD;
        case 22: //items
            if(ListFieldID == 0) //items
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)CNTO.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= SNAM.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //item
                    return FORMID_FIELD;
                case 2: //count
                    return SINT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 23: //aggression
            return UINT8_FIELD;
        case 24: //confidence
            return UINT8_FIELD;
        case 25: //energyLevel
            return UINT8_FIELD;
        case 26: //responsibility
            return UINT8_FIELD;
        case 27: //services
            return UINT32_FLAG_FIELD;
        case 28: //trainSkill
            return SINT8_FIELD;
        case 29: //trainLevel
            return UINT8_FIELD;
        case 30: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 31: //aiPackages
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)PKID.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 32: //animations
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return ISTRING_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)KFFZ.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 33: //creatureType
            return UINT8_TYPE_FIELD;
        case 34: //combat
            return UINT8_FIELD;
        case 35: //magic
            return UINT8_FIELD;
        case 36: //stealth
            return UINT8_FIELD;
        case 37: //soulType
            return UINT8_TYPE_FIELD;
        case 38: //unused2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 1;
                default:
                    return UNKNOWN_FIELD;
                }
        case 39: //health
            return UINT16_FIELD;
        case 40: //unused3
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 41: //attackDamage
            return UINT16_FIELD;
        case 42: //strength
            return UINT8_FIELD;
        case 43: //intelligence
            return UINT8_FIELD;
        case 44: //willpower
            return UINT8_FIELD;
        case 45: //agility
            return UINT8_FIELD;
        case 46: //speed
            return UINT8_FIELD;
        case 47: //endurance
            return UINT8_FIELD;
        case 48: //personality
            return UINT8_FIELD;
        case 49: //luck
            return UINT8_FIELD;
        case 50: //attackReach
            return UINT8_FIELD;
        case 51: //combatStyle
            return FORMID_FIELD;
        case 52: //turningSpeed
            return FLOAT32_FIELD;
        case 53: //baseScale
            return FLOAT32_FIELD;
        case 54: //footWeight
            return FLOAT32_FIELD;
        case 55: //inheritsSoundsFrom
            return FORMID_FIELD;
        case 56: //bloodSprayPath
            return ISTRING_FIELD;
        case 57: //bloodDecalPath
            return ISTRING_FIELD;
        case 58: //sounds
            if(ListFieldID == 0) //sounds
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)Sounds.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= SNAM.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //soundType
                    return UINT32_FIELD;
                case 2: //sound
                    return FORMID_FIELD;
                case 3: //chance
                    return UINT8_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * CREARecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
    {
    switch(FieldID)
        {
        case 1: //flags1
            return &flags;
        case 2: //fid
            return &formID;
        case 3: //flags2
            return &flagsUnk;
        case 4: //eid
            return EDID.value;
        case 5: //full
            return FULL.value;
        case 6: //modPath
            return MODL.IsLoaded() ? MODL->MODL.value : NULL;
        case 7: //modb
            return MODL.IsLoaded() ? &MODL->MODB.value.MODB : NULL;
        case 8: //modt_p
            *FieldValues = MODL.IsLoaded() ? MODL->MODT.value : NULL;
            return NULL;
        case 9: //spells
            *FieldValues = SPLO.size() ? &SPLO[0] : NULL;
            return NULL;
        case 10: //bodyParts
            for(UINT32 p = 0;p < NIFZ.size();p++)
                FieldValues[p] = NIFZ[p].value;
            return NULL;
        case 11: //nift_p
            *FieldValues = NIFT.value;
            return NULL;
        case 12: //flags
            return &ACBS.value.flags;
        case 13: //baseSpell
            return &ACBS.value.baseSpell;
        case 14: //fatigue
            return &ACBS.value.fatigue;
        case 15: //barterGold
            return &ACBS.value.barterGold;
        case 16: //level
            return &ACBS.value.level;
        case 17: //calcMin
            return &ACBS.value.calcMin;
        case 18: //calcMax
            return &ACBS.value.calcMax;
        case 19: //factions
            if(ListIndex >= SNAM.size())
                {
                *FieldValues = NULL;
                return NULL;
                }

            switch(ListFieldID)
                {
                case 1: //faction
                    return &SNAM[ListIndex]->value.faction;
                case 2: //rank
                    return &SNAM[ListIndex]->value.rank;
                case 3: //unused1
                    *FieldValues = &SNAM[ListIndex]->value.unused1[0];
                    return NULL;
                default:
                    *FieldValues = NULL;
                    return NULL;
                }
        case 20: //deathItem
            return INAM.IsLoaded() ? &INAM->fid : NULL;
        case 21: //script
            return SCRI.IsLoaded() ? &SCRI->fid : NULL;
        case 22: //items
            if(ListIndex >= CNTO.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //item
                    return &CNTO[ListIndex]->value.item;
                case 2: //count
                    return &CNTO[ListIndex]->value.count;
                default:
                    return NULL;
                }
        case 23: //aggression
            return &AIDT.value.aggression;
        case 24: //confidence
            return &AIDT.value.confidence;
        case 25: //energyLevel
            return &AIDT.value.energyLevel;
        case 26: //responsibility
            return &AIDT.value.responsibility;
        case 27: //services
            return &AIDT.value.flags;
        case 28: //trainSkill
            return &AIDT.value.trainSkill;
        case 29: //trainLevel
            return &AIDT.value.trainLevel;
        case 30: //unused1
            *FieldValues = &AIDT.value.unused1[0];
            return NULL;
        case 31: //aiPackages
            *FieldValues = PKID.size() ? &PKID[0] : NULL;
            return NULL;
        case 32: //animations
            for(UINT32 p = 0;p < KFFZ.size();p++)
                FieldValues[p] = KFFZ[p].value;
            return NULL;
        case 33: //creatureType
            return &DATA.value.creatureType;
        case 34: //combat
            return &DATA.value.combat;
        case 35: //magic
            return &DATA.value.magic;
        case 36: //stealth
            return &DATA.value.stealth;
        case 37: //soulType
            return &DATA.value.soul;
        case 38: //unused2
            *FieldValues = &DATA.value.unused1;
            return NULL;
        case 39: //health
            return &DATA.value.health;
        case 40: //unused3
            *FieldValues = &DATA.value.unused2[0];
            return NULL;
        case 41: //attackDamage
            return &DATA.value.attackDamage;
        case 42: //strength
            return &DATA.value.strength;
        case 43: //intelligence
            return &DATA.value.intelligence;
        case 44: //willpower
            return &DATA.value.willpower;
        case 45: //agility
            return &DATA.value.agility;
        case 46: //speed
            return &DATA.value.speed;
        case 47: //endurance
            return &DATA.value.endurance;
        case 48: //personality
            return &DATA.value.personality;
        case 49: //luck
            return &DATA.value.luck;
        case 50: //attackReach
            return &RNAM.value.attackReach;
        case 51: //combatStyle
            return ZNAM.IsLoaded() ? &ZNAM->fid : NULL;
        case 52: //turningSpeed
            return &TNAM.value.turningSpeed;
        case 53: //baseScale
            return &BNAM.value.baseScale;
        case 54: //footWeight
            return &WNAM.value.footWeight;
        case 55: //inheritsSoundsFrom
            return CSCR.IsLoaded() ? &CSCR->fid : NULL;
        case 56: //bloodSprayPath
            return NAM0.value;
        case 57: //bloodDecalPath
            return NAM1.value;
        case 58: //sounds
            if(ListIndex >= Sounds.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //soundType
                    return &Sounds[ListIndex]->CSDT.value.soundType;
                case 2: //sound
                    return &Sounds[ListIndex]->CSDI.value.fid;
                case 3: //chance
                    return &Sounds[ListIndex]->CSDC.value.chance;
                default:
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool CREARecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
    {
    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(*(UINT32 *)FieldValue);
            break;
        case 3: //flags2
            SetHeaderUnknownFlagMask(*(UINT32 *)FieldValue);
            break;
        case 4: //eid
            EDID.Copy((STRING)FieldValue);
            break;
        case 5: //full
            FULL.Copy((STRING)FieldValue);
            break;
        case 6: //modPath
            MODL.Load();
            MODL->MODL.Copy((STRING)FieldValue);
            break;
        case 7: //modb
            MODL.Load();
            MODL->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 8: //modt_p
            MODL.Load();
            MODL->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 9: //spells
            SPLO.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                SPLO[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 10: //bodyParts
            NIFZ.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                NIFZ[x] = StringRecord(((STRINGARRAY)FieldValue)[x]);
            break;
        case 11: //nift_p
            NIFT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 12: //flags
            SetFlagMask(*(UINT32 *)FieldValue);
            break;
        case 13: //baseSpell
            ACBS.value.baseSpell = *(UINT16 *)FieldValue;
            break;
        case 14: //fatigue
            ACBS.value.fatigue = *(UINT16 *)FieldValue;
            break;
        case 15: //barterGold
            ACBS.value.barterGold = *(UINT16 *)FieldValue;
            break;
        case 16: //level
            ACBS.value.level = *(SINT16 *)FieldValue;
            break;
        case 17: //calcMin
            ACBS.value.calcMin = *(UINT16 *)FieldValue;
            break;
        case 18: //calcMax
            ACBS.value.calcMax = *(UINT16 *)FieldValue;
            break;
        case 19: //factions
            if(ListFieldID == 0) //factionsSize
                {
                ArraySize -= (UINT32)SNAM.size();
                while((SINT32)ArraySize > 0)
                    {
                    SNAM.push_back(new ReqSubRecord<GENSNAM>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete SNAM.back();
                    SNAM.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= SNAM.size())
                break;

            switch(ListFieldID)
                {
                case 1: //faction
                    SNAM[ListIndex]->value.faction = *(FORMID *)FieldValue;
                    return true;
                case 2: //rank
                    SNAM[ListIndex]->value.rank = *(UINT8 *)FieldValue;
                    break;
                case 3: //unused1
                    if(ArraySize != 3)
                        break;
                    SNAM[ListIndex]->value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                    SNAM[ListIndex]->value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                    SNAM[ListIndex]->value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
                    break;
                default:
                    break;
                }
            break;
        case 20: //deathItem
            INAM.Load();
            INAM->fid = *(FORMID *)FieldValue;
            return true;
        case 21: //script
            SCRI.Load();
            SCRI->fid = *(FORMID *)FieldValue;
            return true;
        case 22: //items
            if(ListFieldID == 0) //itemsSize
                {
                ArraySize -= (UINT32)CNTO.size();
                while((SINT32)ArraySize > 0)
                    {
                    CNTO.push_back(new ReqSubRecord<GENCNTO>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete CNTO.back();
                    CNTO.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= CNTO.size())
                break;

            switch(ListFieldID)
                {
                case 1: //item
                    CNTO[ListIndex]->value.item = *(FORMID *)FieldValue;
                    return true;
                case 2: //count
                    CNTO[ListIndex]->value.count = *(SINT32 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 23: //aggression
            AIDT.value.aggression = *(UINT8 *)FieldValue;
            break;
        case 24: //confidence
            AIDT.value.confidence = *(UINT8 *)FieldValue;
            break;
        case 25: //energyLevel
            AIDT.value.energyLevel = *(UINT8 *)FieldValue;
            break;
        case 26: //responsibility
            AIDT.value.responsibility = *(UINT8 *)FieldValue;
            break;
        case 27: //services
            SetServicesFlagMask(*(UINT32 *)FieldValue);
            break;
        case 28: //trainSkill
            AIDT.value.trainSkill = *(SINT8 *)FieldValue;
            break;
        case 29: //trainLevel
            AIDT.value.trainLevel = *(UINT8 *)FieldValue;
            break;
        case 30: //unused1
            if(ArraySize != 2)
                break;
            AIDT.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            AIDT.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 31: //aiPackages
            PKID.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                PKID[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 32: //animations
            KFFZ.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                KFFZ[x] = StringRecord(((STRINGARRAY)FieldValue)[x]);
            break;
        case 33: //creatureType
            SetType(*(UINT8 *)FieldValue);
            break;
        case 34: //combat
            DATA.value.combat = *(UINT8 *)FieldValue;
            break;
        case 35: //magic
            DATA.value.magic = *(UINT8 *)FieldValue;
            break;
        case 36: //stealth
            DATA.value.stealth = *(UINT8 *)FieldValue;
            break;
        case 37: //soulType
            SetSoul(*(UINT8 *)FieldValue);
            break;
        case 38: //unused2
            if(ArraySize != 1)
                break;
            DATA.value.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 39: //health
            DATA.value.health = *(UINT16 *)FieldValue;
            break;
        case 40: //unused3
            if(ArraySize != 2)
                break;
            DATA.value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
            DATA.value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 41: //attackDamage
            DATA.value.attackDamage = *(UINT16 *)FieldValue;
            break;
        case 42: //strength
            DATA.value.strength = *(UINT8 *)FieldValue;
            break;
        case 43: //intelligence
            DATA.value.intelligence = *(UINT8 *)FieldValue;
            break;
        case 44: //willpower
            DATA.value.willpower = *(UINT8 *)FieldValue;
            break;
        case 45: //agility
            DATA.value.agility = *(UINT8 *)FieldValue;
            break;
        case 46: //speed
            DATA.value.speed = *(UINT8 *)FieldValue;
            break;
        case 47: //endurance
            DATA.value.endurance = *(UINT8 *)FieldValue;
            break;
        case 48: //personality
            DATA.value.personality = *(UINT8 *)FieldValue;
            break;
        case 49: //luck
            DATA.value.luck = *(UINT8 *)FieldValue;
            break;
        case 50: //attackReach
            RNAM.value.attackReach = *(UINT8 *)FieldValue;
            break;
        case 51: //combatStyle
            ZNAM.Load();
            ZNAM->fid = *(FORMID *)FieldValue;
            return true;
        case 52: //turningSpeed
            TNAM.value.turningSpeed = *(FLOAT32 *)FieldValue;
            break;
        case 53: //baseScale
            BNAM.value.baseScale = *(FLOAT32 *)FieldValue;
            break;
        case 54: //footWeight
            WNAM.value.footWeight = *(FLOAT32 *)FieldValue;
            break;
        case 55: //inheritsSoundsFrom
            CSCR.Load();
            CSCR->fid = *(FORMID *)FieldValue;
            return true;
        case 56: //bloodSprayPath
            NAM0.Copy((STRING)FieldValue);
            break;
        case 57: //bloodDecalPath
            NAM1.Copy((STRING)FieldValue);
            break;
        case 58: //sounds
            if(ListFieldID == 0) //soundsSize
                {
                ArraySize -= (UINT32)Sounds.size();
                while((SINT32)ArraySize > 0)
                    {
                    Sounds.push_back(new CREASound());
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete Sounds.back();
                    Sounds.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= Sounds.size())
                break;

            switch(ListFieldID)
                {
                case 1: //soundType
                    Sounds[ListIndex]->SetType(*(UINT32 *)FieldValue);
                    break;
                case 2: //sound
                    Sounds[ListIndex]->CSDI.value.fid = *(FORMID *)FieldValue;
                    return true;
                case 3: //chance
                    Sounds[ListIndex]->CSDC.value.chance = *(UINT8 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        default:
            break;
        }
    return false;
    }

void CREARecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GENAIDT defaultAIDT;
    GENACBS defaultACBS;
    CREADATA defaultDATA;
    GENSNAM defaultSNAM;
    GENCNTO defaultCNTO;

    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(0);
            return;
        case 3: //flags2
            flagsUnk = 0;
            return;
        case 4: //eid
            EDID.Unload();
            return;
        case 5: //full
            FULL.Unload();
            return;
        case 6: //modPath
            if(MODL.IsLoaded())
                MODL->MODL.Unload();
            return;
        case 7: //modb
            if(MODL.IsLoaded())
                MODL->MODB.Unload();
            return;
        case 8: //modt_p
            if(MODL.IsLoaded())
                MODL->MODT.Unload();
            return;
        case 9: //spells
            SPLO.clear();
            return;
        case 10: //bodyParts
            NIFZ.clear();
            return;
        case 11: //nift_p
            NIFT.Unload();
            return;
        case 12: //flags
            ACBS.value.flags = defaultACBS.flags;
            return;
        case 13: //baseSpell
            ACBS.value.baseSpell = defaultACBS.baseSpell;
            return;
        case 14: //fatigue
            ACBS.value.fatigue = defaultACBS.fatigue;
            return;
        case 15: //barterGold
            ACBS.value.barterGold = defaultACBS.barterGold;
            return;
        case 16: //level
            ACBS.value.level = defaultACBS.level;
            return;
        case 17: //calcMin
            ACBS.value.calcMin = defaultACBS.calcMin;
            return;
        case 18: //calcMax
            ACBS.value.calcMax = defaultACBS.calcMax;
            return;
        case 19: //factions
            if(ListFieldID == 0) //factions
                {
                for(UINT32 x = 0; x < (UINT32)SNAM.size(); x++)
                    delete SNAM[x];
                SNAM.clear();
                return;
                }

            if(ListIndex >= SNAM.size())
                return;

            switch(ListFieldID)
                {
                case 1: //faction
                    SNAM[ListIndex]->value.faction = defaultSNAM.faction;
                    return;
                case 2: //rank
                    SNAM[ListIndex]->value.rank = defaultSNAM.rank;
                    return;
                case 3: //unused1
                    SNAM[ListIndex]->value.unused1[0] = defaultSNAM.unused1[0];
                    SNAM[ListIndex]->value.unused1[1] = defaultSNAM.unused1[1];
                    SNAM[ListIndex]->value.unused1[2] = defaultSNAM.unused1[2];
                    return;
                default:
                    return;
                }
        case 20: //deathItem
            INAM.Unload();
            return;
        case 21: //script
            SCRI.Unload();
            return;
        case 22: //items
            if(ListFieldID == 0) //items
                {
                for(UINT32 x = 0; x < (UINT32)CNTO.size(); x++)
                    delete CNTO[x];
                CNTO.clear();
                return;
                }

            if(ListIndex >= CNTO.size())
                return;

            switch(ListFieldID)
                {
                case 1: //item
                    CNTO[ListIndex]->value.item = defaultCNTO.item;
                    return;
                case 2: //count
                    CNTO[ListIndex]->value.count = defaultCNTO.count;
                    return;
                default:
                    return;
                }
        case 23: //aggression
            AIDT.value.aggression = defaultAIDT.aggression;
            return;
        case 24: //confidence
            AIDT.value.confidence = defaultAIDT.confidence;
            return;
        case 25: //energyLevel
            AIDT.value.confidence = defaultAIDT.confidence;
            return;
        case 26: //responsibility
            AIDT.value.responsibility = defaultAIDT.responsibility;
            return;
        case 27: //services
            AIDT.value.flags = defaultAIDT.flags;
            return;
        case 28: //trainSkill
            AIDT.value.trainSkill = defaultAIDT.trainSkill;
            return;
        case 29: //trainLevel
            AIDT.value.trainLevel = defaultAIDT.trainLevel;
            return;
        case 30: //unused1
            AIDT.value.unused1[0] = defaultAIDT.unused1[0];
            AIDT.value.unused1[1] = defaultAIDT.unused1[1];
            return;
        case 31: //aiPackages
            PKID.clear();
            return;
        case 32: //animations
            KFFZ.clear();
            return;
        case 33: //creatureType
            DATA.value.creatureType = defaultDATA.creatureType;
            return;
        case 34: //combat
            DATA.value.combat = defaultDATA.combat;
            return;
        case 35: //magic
            DATA.value.magic = defaultDATA.magic;
            return;
        case 36: //stealth
            DATA.value.stealth = defaultDATA.stealth;
            return;
        case 37: //soulType
            DATA.value.soul = defaultDATA.soul;
            return;
        case 38: //unused2
            DATA.value.unused1 = defaultDATA.unused1;
            return;
        case 39: //health
            DATA.value.health = defaultDATA.health;
            return;
        case 40: //unused3
            DATA.value.unused2[0] = defaultDATA.unused2[0];
            DATA.value.unused2[1] = defaultDATA.unused2[1];
            return;
        case 41: //attackDamage
            DATA.value.attackDamage = defaultDATA.attackDamage;
            return;
        case 42: //strength
            DATA.value.strength = defaultDATA.strength;
            return;
        case 43: //intelligence
            DATA.value.intelligence = defaultDATA.intelligence;
            return;
        case 44: //willpower
            DATA.value.willpower = defaultDATA.willpower;
            return;
        case 45: //agility
            DATA.value.agility = defaultDATA.agility;
            return;
        case 46: //speed
            DATA.value.speed = defaultDATA.speed;
            return;
        case 47: //endurance
            DATA.value.endurance = defaultDATA.endurance;
            return;
        case 48: //personality
            DATA.value.personality = defaultDATA.personality;
            return;
        case 49: //luck
            DATA.value.luck = defaultDATA.luck;
            return;
        case 50: //attackReach
            RNAM.Unload();
            return;
        case 51: //combatStyle
            ZNAM.Unload();
            return;
        case 52: //turningSpeed
            TNAM.Unload();
            return;
        case 53: //baseScale
            BNAM.Unload();
            return;
        case 54: //footWeight
            WNAM.Unload();
            return;
        case 55: //inheritsSoundsFrom
            CSCR.Unload();
            return;
        case 56: //bloodSprayPath
            NAM0.Unload();
            return;
        case 57: //bloodDecalPath
            NAM1.Unload();
            return;
        case 58: //sounds
            if(ListFieldID == 0) //sounds
                {
                for(UINT32 x = 0; x < (UINT32)Sounds.size(); x++)
                    delete Sounds[x];
                Sounds.clear();
                return;
                }

            if(ListIndex >= Sounds.size())
                return;

            switch(ListFieldID)
                {
                case 1: //soundType
                    Sounds[ListIndex]->CSDT.Unload();
                    return;
                case 2: //sound
                    Sounds[ListIndex]->CSDI.Unload();
                    return;
                case 3: //chance
                    Sounds[ListIndex]->CSDC.Unload();
                    return;
                default:
                    return;
                }
            return;
        default:
            return;
        }
    }