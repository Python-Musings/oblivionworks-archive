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
#include "..\NPC_Record.h"

UINT32 NPC_Record::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 9: //flags
            return UINT32_FLAG_FIELD;
        case 10: //baseSpell
            return UINT16_FIELD;
        case 11: //fatigue
            return UINT16_FIELD;
        case 12: //barterGold
            return UINT16_FIELD;
        case 13: //level
            return SINT16_FIELD;
        case 14: //calcMin
            return UINT16_FIELD;
        case 15: //calcMax
            return UINT16_FIELD;
        case 16: //factions
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
        case 17: //deathItem
            return FORMID_FIELD;
        case 18: //race
            return FORMID_FIELD;
        case 19: //spells
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)SPLO.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 20: //script
            return FORMID_FIELD;
        case 21: //items
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

            if(ListIndex >= CNTO.size())
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
        case 22: //aggression
            return UINT8_FIELD;
        case 23: //confidence
            return UINT8_FIELD;
        case 24: //energyLevel
            return UINT8_FIELD;
        case 25: //responsibility
            return UINT8_FIELD;
        case 26: //services
            return UINT32_FLAG_FIELD;
        case 27: //trainSkill
            return SINT8_FIELD;
        case 28: //trainLevel
            return UINT8_FIELD;
        case 29: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 30: //aiPackages
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)PKID.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 31: //animations
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return ISTRING_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)KFFZ.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 32: //iclass
            return FORMID_FIELD;
        case 33: //armorer
            return UINT8_FIELD;
        case 34: //athletics
            return UINT8_FIELD;
        case 35: //blade
            return UINT8_FIELD;
        case 36: //block
            return UINT8_FIELD;
        case 37: //blunt
            return UINT8_FIELD;
        case 38: //h2h
            return UINT8_FIELD;
        case 39: //heavyArmor
            return UINT8_FIELD;
        case 40: //alchemy
            return UINT8_FIELD;
        case 41: //alteration
            return UINT8_FIELD;
        case 42: //conjuration
            return UINT8_FIELD;
        case 43: //destruction
            return UINT8_FIELD;
        case 44: //illusion
            return UINT8_FIELD;
        case 45: //mysticism
            return UINT8_FIELD;
        case 46: //restoration
            return UINT8_FIELD;
        case 47: //acrobatics
            return UINT8_FIELD;
        case 48: //lightArmor
            return UINT8_FIELD;
        case 49: //marksman
            return UINT8_FIELD;
        case 50: //mercantile
            return UINT8_FIELD;
        case 51: //security
            return UINT8_FIELD;
        case 52: //sneak
            return UINT8_FIELD;
        case 53: //speechcraft
            return UINT8_FIELD;
        case 54: //health
            return UINT16_FIELD;
        case 55: //unused2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 56: //strength
            return UINT8_FIELD;
        case 57: //intelligence
            return UINT8_FIELD;
        case 58: //willpower
            return UINT8_FIELD;
        case 59: //agility
            return UINT8_FIELD;
        case 60: //speed
            return UINT8_FIELD;
        case 61: //endurance
            return UINT8_FIELD;
        case 62: //personality
            return UINT8_FIELD;
        case 63: //luck
            return UINT8_FIELD;
        case 64: //hair
            return FORMID_FIELD;
        case 65: //hairLength
            return FLOAT32_FIELD;
        case 66: //eye
            return FORMID_FIELD;
        case 67: //hairRed
            return UINT8_FIELD;
        case 68: //hairGreen
            return UINT8_FIELD;
        case 69: //hairBlue
            return UINT8_FIELD;
        case 70: //unused3
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 1;
                default:
                    return UNKNOWN_FIELD;
                }
        case 71: //combatStyle
            return FORMID_FIELD;
        case 72: //fggs_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return FGGS.IsLoaded() ? 200 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 73: //fgga_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return FGGA.IsLoaded() ? 120 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 74: //fgts_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return FGTS.IsLoaded() ? 200 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 75: //fnam
            return UINT16_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * NPC_Record::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 9: //flags
            return &ACBS.value.flags;
        case 10: //baseSpell
            return &ACBS.value.baseSpell;
        case 11: //fatigue
            return &ACBS.value.fatigue;
        case 12: //barterGold
            return &ACBS.value.barterGold;
        case 13: //level
            return &ACBS.value.level;
        case 14: //calcMin
            return &ACBS.value.calcMin;
        case 15: //calcMax
            return &ACBS.value.calcMax;
        case 16: //factions
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
        case 17: //deathItem
            return INAM.IsLoaded() ? &INAM->fid : NULL;
        case 18: //race
            return RNAM.IsLoaded() ? &RNAM->fid : NULL;
        case 19: //spells
            *FieldValues = SPLO.size() ? &SPLO[0] : NULL;
            return NULL;
        case 20: //script
            return SCRI.IsLoaded() ? &SCRI->fid : NULL;
        case 21: //items
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
        case 22: //aggression
            return &AIDT.value.aggression;
        case 23: //confidence
            return &AIDT.value.confidence;
        case 24: //energyLevel
            return &AIDT.value.energyLevel;
        case 25: //responsibility
            return &AIDT.value.responsibility;
        case 26: //services
            return &AIDT.value.flags;
        case 27: //trainSkill
            return &AIDT.value.trainSkill;
        case 28: //trainLevel
            return &AIDT.value.trainLevel;
        case 29: //unused1
            *FieldValues = &AIDT.value.unused1[0];
            return NULL;
        case 30: //aiPackages
            *FieldValues = PKID.size() ? &PKID[0] : NULL;
            return NULL;
        case 31: //animations
            for(UINT32 p = 0;p < KFFZ.size();p++)
                FieldValues[p] = KFFZ[p].value;
            return NULL;
        case 32: //iclass
            return &CNAM.value.fid;
        case 33: //armorer
            return &DATA.value.armorer;
        case 34: //athletics
            return &DATA.value.athletics;
        case 35: //blade
            return &DATA.value.blade;
        case 36: //block
            return &DATA.value.block;
        case 37: //blunt
            return &DATA.value.blunt;
        case 38: //h2h
            return &DATA.value.h2h;
        case 39: //heavyArmor
            return &DATA.value.heavyArmor;
        case 40: //alchemy
            return &DATA.value.alchemy;
        case 41: //alteration
            return &DATA.value.alteration;
        case 42: //conjuration
            return &DATA.value.conjuration;
        case 43: //destruction
            return &DATA.value.destruction;
        case 44: //illusion
            return &DATA.value.illusion;
        case 45: //mysticism
            return &DATA.value.mysticism;
        case 46: //restoration
            return &DATA.value.restoration;
        case 47: //acrobatics
            return &DATA.value.acrobatics;
        case 48: //lightArmor
            return &DATA.value.lightArmor;
        case 49: //marksman
            return &DATA.value.marksman;
        case 50: //mercantile
            return &DATA.value.mercantile;
        case 51: //security
            return &DATA.value.security;
        case 52: //sneak
            return &DATA.value.sneak;
        case 53: //speechcraft
            return &DATA.value.speechcraft;
        case 54: //health
            return &DATA.value.health;
        case 55: //unused2
            *FieldValues = &DATA.value.unused1[0];
            return NULL;
        case 56: //strength
            return &DATA.value.strength;
        case 57: //intelligence
            return &DATA.value.intelligence;
        case 58: //willpower
            return &DATA.value.willpower;
        case 59: //agility
            return &DATA.value.agility;
        case 60: //speed
            return &DATA.value.speed;
        case 61: //endurance
            return &DATA.value.endurance;
        case 62: //personality
            return &DATA.value.personality;
        case 63: //luck
            return &DATA.value.luck;
        case 64: //hair
            return HNAM.IsLoaded() ? &HNAM->fid : NULL;
        case 65: //hairLength
            return LNAM.IsLoaded() ? &LNAM->hairLength : NULL;
        case 66: //eye
            return ENAM.IsLoaded() ? &ENAM->fid : NULL;
        case 67: //hairRed
            return &HCLR.value.red;
        case 68: //hairGreen
            return &HCLR.value.green;
        case 69: //hairBlue
            return &HCLR.value.blue;
        case 70: //unused3
            *FieldValues = &HCLR.value.unused1;
            return NULL;
        case 71: //combatStyle
            return ZNAM.IsLoaded() ? &ZNAM->fid : NULL;
        case 72: //fggs_p
            *FieldValues = FGGS.value;
            return NULL;
        case 73: //fgga_p
            *FieldValues = FGGA.value;
            return NULL;
        case 74: //fgts_p
            *FieldValues = FGTS.value;
            return NULL;
        case 75: //fnam
            return &FNAM.value.fnam;
        default:
            return NULL;
        }
    }

bool NPC_Record::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 9: //flags
            SetFlagMask(*(UINT32 *)FieldValue);
            break;
        case 10: //baseSpell
            ACBS.value.baseSpell = *(UINT16 *)FieldValue;
            break;
        case 11: //fatigue
            ACBS.value.fatigue = *(UINT16 *)FieldValue;
            break;
        case 12: //barterGold
            ACBS.value.barterGold = *(UINT16 *)FieldValue;
            break;
        case 13: //level
            ACBS.value.level = *(SINT16 *)FieldValue;
            break;
        case 14: //calcMin
            ACBS.value.calcMin = *(UINT16 *)FieldValue;
            break;
        case 15: //calcMax
            ACBS.value.calcMax = *(UINT16 *)FieldValue;
            break;
        case 16: //factions
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
        case 17: //deathItem
            INAM.Load();
            INAM->fid = *(FORMID *)FieldValue;
            return true;
        case 18: //race
            RNAM.Load();
            RNAM->fid = *(FORMID *)FieldValue;
            return true;
        case 19: //spells
            SPLO.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                SPLO[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 20: //script
            SCRI.Load();
            SCRI->fid = *(FORMID *)FieldValue;
            return true;
        case 21: //items
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
        case 22: //aggression
            AIDT.value.aggression = *(UINT8 *)FieldValue;
            break;
        case 23: //confidence
            AIDT.value.confidence = *(UINT8 *)FieldValue;
            break;
        case 24: //energyLevel
            AIDT.value.energyLevel = *(UINT8 *)FieldValue;
            break;
        case 25: //responsibility
            AIDT.value.responsibility = *(UINT8 *)FieldValue;
            break;
        case 26: //services
            SetServicesFlagMask(*(UINT32 *)FieldValue);
            break;
        case 27: //trainSkill
            AIDT.value.trainSkill = *(SINT8 *)FieldValue;
            break;
        case 28: //trainLevel
            AIDT.value.trainLevel = *(UINT8 *)FieldValue;
            break;
        case 29: //unused1
            if(ArraySize != 2)
                break;
            AIDT.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            AIDT.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 30: //aiPackages
            PKID.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                PKID[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 31: //animations
            KFFZ.resize(ArraySize);
            if(ArraySize)
                for(UINT32 x = 0; x < ArraySize; x++)
                    KFFZ[x].Copy((STRING)((STRINGARRAY)FieldValue)[x]);
            break;
        case 32: //iclass
            CNAM.value.fid = *(FORMID *)FieldValue;
            return true;
        case 33: //armorer
            DATA.value.armorer = *(UINT8 *)FieldValue;
            break;
        case 34: //athletics
            DATA.value.athletics = *(UINT8 *)FieldValue;
            break;
        case 35: //blade
            DATA.value.blade = *(UINT8 *)FieldValue;
            break;
        case 36: //block
            DATA.value.block = *(UINT8 *)FieldValue;
            break;
        case 37: //blunt
            DATA.value.blunt = *(UINT8 *)FieldValue;
            break;
        case 38: //h2h
            DATA.value.h2h = *(UINT8 *)FieldValue;
            break;
        case 39: //heavyArmor
            DATA.value.heavyArmor = *(UINT8 *)FieldValue;
            break;
        case 40: //alchemy
            DATA.value.alchemy = *(UINT8 *)FieldValue;
            break;
        case 41: //alteration
            DATA.value.alteration = *(UINT8 *)FieldValue;
            break;
        case 42: //conjuration
            DATA.value.conjuration = *(UINT8 *)FieldValue;
            break;
        case 43: //destruction
            DATA.value.destruction = *(UINT8 *)FieldValue;
            break;
        case 44: //illusion
            DATA.value.illusion = *(UINT8 *)FieldValue;
            break;
        case 45: //mysticism
            DATA.value.mysticism = *(UINT8 *)FieldValue;
            break;
        case 46: //restoration
            DATA.value.restoration = *(UINT8 *)FieldValue;
            break;
        case 47: //acrobatics
            DATA.value.acrobatics = *(UINT8 *)FieldValue;
            break;
        case 48: //lightArmor
            DATA.value.lightArmor = *(UINT8 *)FieldValue;
            break;
        case 49: //marksman
            DATA.value.marksman = *(UINT8 *)FieldValue;
            break;
        case 50: //mercantile
            DATA.value.mercantile = *(UINT8 *)FieldValue;
            break;
        case 51: //security
            DATA.value.security = *(UINT8 *)FieldValue;
            break;
        case 52: //sneak
            DATA.value.sneak = *(UINT8 *)FieldValue;
            break;
        case 53: //speechcraft
            DATA.value.speechcraft = *(UINT8 *)FieldValue;
            break;
        case 54: //health
            DATA.value.health = *(UINT16 *)FieldValue;
            break;
        case 55: //unused2
            if(ArraySize != 2)
                break;
            DATA.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            DATA.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 56: //strength
            DATA.value.strength = *(UINT8 *)FieldValue;
            break;
        case 57: //intelligence
            DATA.value.intelligence = *(UINT8 *)FieldValue;
            break;
        case 58: //willpower
            DATA.value.willpower = *(UINT8 *)FieldValue;
            break;
        case 59: //agility
            DATA.value.agility = *(UINT8 *)FieldValue;
            break;
        case 60: //speed
            DATA.value.speed = *(UINT8 *)FieldValue;
            break;
        case 61: //endurance
            DATA.value.endurance = *(UINT8 *)FieldValue;
            break;
        case 62: //personality
            DATA.value.personality = *(UINT8 *)FieldValue;
            break;
        case 63: //luck
            DATA.value.luck = *(UINT8 *)FieldValue;
            break;
        case 64: //hair
            HNAM.Load();
            HNAM->fid = *(FORMID *)FieldValue;
            return true;
        case 65: //hairLength
            LNAM.Load();
            LNAM->hairLength = *(FLOAT32 *)FieldValue;
            break;
        case 66: //eye
            ENAM.Load();
            ENAM->fid = *(FORMID *)FieldValue;
            return true;
        case 67: //hairRed
            HCLR.value.red = *(UINT8 *)FieldValue;
            break;
        case 68: //hairGreen
            HCLR.value.green = *(UINT8 *)FieldValue;
            break;
        case 69: //hairBlue
            HCLR.value.blue = *(UINT8 *)FieldValue;
            break;
        case 70: //unused3
            if(ArraySize != 1)
                break;
            HCLR.value.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 71: //combatStyle
            ZNAM.Load();
            ZNAM->fid = *(FORMID *)FieldValue;
            return true;
        case 72: //fggs_p
            if(ArraySize != 200)
                break;
            FGGS.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 73: //fgga_p
            if(ArraySize != 120)
                break;
            FGGA.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 74: //fgts_p
            if(ArraySize != 200)
                break;
            FGTS.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 75: //fnam
            FNAM.value.fnam = *(UINT16 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void NPC_Record::DeleteField(FIELD_IDENTIFIERS)
    {
    GENAIDT defaultAIDT;
    GENACBS defaultACBS;
    NPC_DATA defaultDATA;
    GENCLR defaultHCLR;
    NPC_FNAM defaultFNAM;

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
        case 9: //flags
            ACBS.value.flags = defaultACBS.flags;
            return;
        case 10: //baseSpell
            ACBS.value.baseSpell = defaultACBS.baseSpell;
            return;
        case 11: //fatigue
            ACBS.value.fatigue = defaultACBS.fatigue;
            return;
        case 12: //barterGold
            ACBS.value.barterGold = defaultACBS.barterGold;
            return;
        case 13: //level
            ACBS.value.level = defaultACBS.level;
            return;
        case 14: //calcMin
            ACBS.value.calcMin = defaultACBS.calcMin;
            return;
        case 15: //calcMax
            ACBS.value.calcMax = defaultACBS.calcMax;
            return;
        case 16: //factions
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
        case 17: //deathItem
            INAM.Unload();
            return;
        case 18: //race
            RNAM.Unload();
            return;
        case 19: //spells
            SPLO.clear();
            return;
        case 20: //script
            SCRI.Unload();
            return;
        case 21: //items
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
        case 22: //aggression
            AIDT.value.aggression = defaultAIDT.aggression;
            return;
        case 23: //confidence
            AIDT.value.confidence = defaultAIDT.confidence;
            return;
        case 24: //energyLevel
            AIDT.value.confidence = defaultAIDT.confidence;
            return;
        case 25: //responsibility
            AIDT.value.responsibility = defaultAIDT.responsibility;
            return;
        case 26: //services
            AIDT.value.flags = defaultAIDT.flags;
            return;
        case 27: //trainSkill
            AIDT.value.trainSkill = defaultAIDT.trainSkill;
            return;
        case 28: //trainLevel
            AIDT.value.trainLevel = defaultAIDT.trainLevel;
            return;
        case 29: //unused1
            AIDT.value.unused1[0] = defaultAIDT.unused1[0];
            AIDT.value.unused1[1] = defaultAIDT.unused1[1];
            return;
        case 30: //aiPackages
            PKID.clear();
            return;
        case 31: //animations
            KFFZ.clear();
            return;
        case 32: //iclass
            CNAM.Unload();
            return;
        case 33: //armorer
            DATA.value.armorer = defaultDATA.armorer;
            return;
        case 34: //athletics
            DATA.value.athletics = defaultDATA.athletics;
            return;
        case 35: //blade
            DATA.value.blade = defaultDATA.blade;
            return;
        case 36: //block
            DATA.value.block = defaultDATA.block;
            return;
        case 37: //blunt
            DATA.value.blunt = defaultDATA.blunt;
            return;
        case 38: //h2h
            DATA.value.h2h = defaultDATA.h2h;
            return;
        case 39: //heavyArmor
            DATA.value.heavyArmor = defaultDATA.heavyArmor;
            return;
        case 40: //alchemy
            DATA.value.alchemy = defaultDATA.alchemy;
            return;
        case 41: //alteration
            DATA.value.alteration = defaultDATA.alteration;
            return;
        case 42: //conjuration
            DATA.value.conjuration = defaultDATA.conjuration;
            return;
        case 43: //destruction
            DATA.value.destruction = defaultDATA.destruction;
            return;
        case 44: //illusion
            DATA.value.illusion = defaultDATA.illusion;
            return;
        case 45: //mysticism
            DATA.value.mysticism = defaultDATA.mysticism;
            return;
        case 46: //restoration
            DATA.value.restoration = defaultDATA.restoration;
            return;
        case 47: //acrobatics
            DATA.value.acrobatics = defaultDATA.acrobatics;
            return;
        case 48: //lightArmor
            DATA.value.lightArmor = defaultDATA.lightArmor;
            return;
        case 49: //marksman
            DATA.value.marksman = defaultDATA.marksman;
            return;
        case 50: //mercantile
            DATA.value.mercantile = defaultDATA.mercantile;
            return;
        case 51: //security
            DATA.value.security = defaultDATA.security;
            return;
        case 52: //sneak
            DATA.value.sneak = defaultDATA.sneak;
            return;
        case 53: //speechcraft
            DATA.value.speechcraft = defaultDATA.speechcraft;
            return;
        case 54: //health
            DATA.value.health = defaultDATA.health;
            return;
        case 55: //unused2
            DATA.value.unused1[0] = defaultDATA.unused1[0];
            DATA.value.unused1[1] = defaultDATA.unused1[1];
            return;
        case 56: //strength
            DATA.value.strength = defaultDATA.strength;
            return;
        case 57: //intelligence
            DATA.value.intelligence = defaultDATA.intelligence;
            return;
        case 58: //willpower
            DATA.value.willpower = defaultDATA.willpower;
            return;
        case 59: //agility
            DATA.value.agility = defaultDATA.agility;
            return;
        case 60: //speed
            DATA.value.speed = defaultDATA.speed;
            return;
        case 61: //endurance
            DATA.value.endurance = defaultDATA.endurance;
            return;
        case 62: //personality
            DATA.value.personality = defaultDATA.personality;
            return;
        case 63: //luck
            DATA.value.luck = defaultDATA.luck;
            return;
        case 64: //hair
            HNAM.Unload();
            return;
        case 65: //hairLength
            LNAM.Unload();
            return;
        case 66: //eye
            ENAM.Unload();
            return;
        case 67: //hairRed
            HCLR.value.red = defaultHCLR.red;
            return;
        case 68: //hairGreen
            HCLR.value.green = defaultHCLR.green;
            return;
        case 69: //hairBlue
            HCLR.value.blue = defaultHCLR.blue;
            return;
        case 70: //unused3
            HCLR.value.unused1 = defaultHCLR.unused1;
            return;
        case 71: //combatStyle
            ZNAM.Unload();
            return;
        case 72: //fggs_p
            FGGS.Unload();
            return;
        case 73: //fgga_p
            FGGA.Unload();
            return;
        case 74: //fgts_p
            FGTS.Unload();
            return;
        case 75: //fnam
            FNAM.value.fnam = defaultFNAM.fnam;
            return;
        default:
            return;
        }
    }