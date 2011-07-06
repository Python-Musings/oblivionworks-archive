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
#include "..\RACERecord.h"

UINT32 RACERecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //text
            return STRING_FIELD;
        case 7: //spells
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)SPLO.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 8: //relations
            if(ListFieldID == 0) //relations
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)XNAM.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= XNAM.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //faction
                    return FORMID_FIELD;
                case 2: //mod
                    return SINT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 9: //skill1
            return SINT8_FIELD;
        case 10: //skill1Boost
            return SINT8_FIELD;
        case 11: //skill2
            return SINT8_FIELD;
        case 12: //skill2Boost
            return SINT8_FIELD;
        case 13: //skill3
            return SINT8_FIELD;
        case 14: //skill3Boost
            return SINT8_FIELD;
        case 15: //skill4
            return SINT8_FIELD;
        case 16: //skill4Boost
            return SINT8_FIELD;
        case 17: //skill5
            return SINT8_FIELD;
        case 18: //skill5Boost
            return SINT8_FIELD;
        case 19: //skill6
            return SINT8_FIELD;
        case 20: //skill6Boost
            return SINT8_FIELD;
        case 21: //skill7
            return SINT8_FIELD;
        case 22: //skill7Boost
            return SINT8_FIELD;
        case 23: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 24: //maleHeight
            return FLOAT32_FIELD;
        case 25: //femaleHeight
            return FLOAT32_FIELD;
        case 26: //maleWeight
            return FLOAT32_FIELD;
        case 27: //femaleWeight
            return FLOAT32_FIELD;
        case 28: //flags
            return UINT32_FIELD;
        case 29: //maleVoice
            return FORMID_FIELD;
        case 30: //femaleVoice
            return FORMID_FIELD;
        case 31: //defaultHairMale
            return FORMID_FIELD;
        case 32: //defaultHairFemale
            return FORMID_FIELD;
        case 33: //defaultHairColor
            return UINT8_FIELD;
        case 34: //mainClamp
            return FLOAT32_FIELD;
        case 35: //faceClamp
            return FLOAT32_FIELD;
        case 36: //maleStrength
            return UINT8_FIELD;
        case 37: //maleIntelligence
            return UINT8_FIELD;
        case 38: //maleWillpower
            return UINT8_FIELD;
        case 39: //maleAgility
            return UINT8_FIELD;
        case 40: //maleSpeed
            return UINT8_FIELD;
        case 41: //maleEndurance
            return UINT8_FIELD;
        case 42: //malePersonality
            return UINT8_FIELD;
        case 43: //maleLuck
            return UINT8_FIELD;
        case 44: //femaleStrength
            return UINT8_FIELD;
        case 45: //femaleIntelligence
            return UINT8_FIELD;
        case 46: //femaleWillpower
            return UINT8_FIELD;
        case 47: //femaleAgility
            return UINT8_FIELD;
        case 48: //femaleSpeed
            return UINT8_FIELD;
        case 49: //femaleEndurance
            return UINT8_FIELD;
        case 50: //femalePersonality
            return UINT8_FIELD;
        case 51: //femaleLuck
            return UINT8_FIELD;
        case 52: //head_modPath
            return ISTRING_FIELD;
        case 53: //head_modb
            return FLOAT32_FIELD;
        case 54: //head_iconPath
            return ISTRING_FIELD;
        case 55: //head_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD0.IsLoaded() ? MOD0->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 56: //maleEars_modPath
            return ISTRING_FIELD;
        case 57: //maleEars_modb
            return FLOAT32_FIELD;
        case 58: //maleEars_iconPath
            return ISTRING_FIELD;
        case 59: //maleEars_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD1.IsLoaded() ? MOD1->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 60: //femaleEars_modPath
            return ISTRING_FIELD;
        case 61: //femaleEars_modb
            return FLOAT32_FIELD;
        case 62: //femaleEars_iconPath
            return ISTRING_FIELD;
        case 63: //femaleEars_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD2.IsLoaded() ? MOD2->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 64: //mouth_modPath
            return ISTRING_FIELD;
        case 65: //mouth_modb
            return FLOAT32_FIELD;
        case 66: //mouth_iconPath
            return ISTRING_FIELD;
        case 67: //mouth_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD3.IsLoaded() ? MOD3->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 68: //teethLower_modPath
            return ISTRING_FIELD;
        case 69: //teethLower_modb
            return FLOAT32_FIELD;
        case 70: //teethLower_iconPath
            return ISTRING_FIELD;
        case 71: //teethLower_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD4.IsLoaded() ? MOD4->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 72: //teethUpper_modPath
            return ISTRING_FIELD;
        case 73: //teethUpper_modb
            return FLOAT32_FIELD;
        case 74: //teethUpper_iconPath
            return ISTRING_FIELD;
        case 75: //teethUpper_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD5.IsLoaded() ? MOD5->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 76: //tongue_modPath
            return ISTRING_FIELD;
        case 77: //tongue_modb
            return FLOAT32_FIELD;
        case 78: //tongue_iconPath
            return ISTRING_FIELD;
        case 79: //tongue_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD6.IsLoaded() ? MOD6->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 80: //leftEye_modPath
            return ISTRING_FIELD;
        case 81: //leftEye_modb
            return FLOAT32_FIELD;
        case 82: //leftEye_iconPath
            return ISTRING_FIELD;
        case 83: //leftEye_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD7.IsLoaded() ? MOD7->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 84: //rightEye_modPath
            return ISTRING_FIELD;
        case 85: //rightEye_modb
            return FLOAT32_FIELD;
        case 86: //rightEye_iconPath
            return ISTRING_FIELD;
        case 87: //rightEye_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD8.IsLoaded() ? MOD8->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 88: //maleTail_modPath
            return ISTRING_FIELD;
        case 89: //maleTail_modb
            return FLOAT32_FIELD;
        case 90: //maleTail_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MMODL.IsLoaded() ? MMODL->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 91: //maleUpperBodyPath
            return ISTRING_FIELD;
        case 92: //maleLowerBodyPath
            return ISTRING_FIELD;
        case 93: //maleHandPath
            return ISTRING_FIELD;
        case 94: //maleFootPath
            return ISTRING_FIELD;
        case 95: //maleTailPath
            return ISTRING_FIELD;
        case 96: //femaleTail_modPath
            return ISTRING_FIELD;
        case 97: //femaleTail_modb
            return FLOAT32_FIELD;
        case 98: //femaleTail_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return FMODL.IsLoaded() ? FMODL->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 99: //femaleUpperBodyPath
            return ISTRING_FIELD;
        case 100: //femaleLowerBodyPath
            return ISTRING_FIELD;
        case 101: //femaleHandPath
            return ISTRING_FIELD;
        case 102: //femaleFootPath
            return ISTRING_FIELD;
        case 103: //femaleTailPath
            return ISTRING_FIELD;
        case 104: //hairs
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)HNAM.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 105: //eyes
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)ENAM.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 106: //fggs_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return FGGS.IsLoaded() ? 200 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 107: //fgga_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return FGGA.IsLoaded() ? 120 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 108: //fgts_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return FGTS.IsLoaded() ? 200 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 109: //snam_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * RACERecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //text
            return DESC.value;
        case 7: //spells
            *FieldValues = SPLO.size() ? &SPLO[0] : NULL;
            return NULL;
        case 8: //relations
            if(ListIndex >= XNAM.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //faction
                    return &XNAM[ListIndex]->value.faction;
                case 2: //mod
                    return &XNAM[ListIndex]->value.mod;
                default:
                    return NULL;
                }
        case 9: //skill1
            return &DATA.value.skills[0].value;
        case 10: //skill1Boost
            return &DATA.value.skills[0].boost;
        case 11: //skill2
            return &DATA.value.skills[1].value;
        case 12: //skill2Boost
            return &DATA.value.skills[1].boost;
        case 13: //skill3
            return &DATA.value.skills[2].value;
        case 14: //skill3Boost
            return &DATA.value.skills[2].boost;
        case 15: //skill4
            return &DATA.value.skills[3].value;
        case 16: //skill4Boost
            return &DATA.value.skills[3].boost;
        case 17: //skill5
            return &DATA.value.skills[4].value;
        case 18: //skill5Boost
            return &DATA.value.skills[4].boost;
        case 19: //skill6
            return &DATA.value.skills[5].value;
        case 20: //skill6Boost
            return &DATA.value.skills[5].boost;
        case 21: //skill7
            return &DATA.value.skills[6].value;
        case 22: //skill7Boost
            return &DATA.value.skills[6].boost;
        case 23: //unused1
            *FieldValues = &DATA.value.unused1[0];
            return NULL;
        case 24: //maleHeight
            return &DATA.value.maleHeight;
        case 25: //femaleHeight
            return &DATA.value.femaleHeight;
        case 26: //maleWeight
            return &DATA.value.maleWeight;
        case 27: //femaleWeight
            return &DATA.value.femaleWeight;
        case 28: //flags
            return &DATA.value.flags;
        case 29: //maleVoice
            return VNAM.IsLoaded() ? &VNAM.value.maleVoice : NULL;
        case 30: //femaleVoice
            return VNAM.IsLoaded() ? &VNAM.value.femaleVoice : NULL;
        case 31: //defaultHairMale
            return DNAM.IsLoaded() ? &DNAM.value.defaultHairMale : NULL;
        case 32: //defaultHairFemale
            return DNAM.IsLoaded() ? &DNAM.value.defaultHairFemale : NULL;
        case 33: //defaultHairColor
            return &CNAM.value.defaultHairColor;
        case 34: //mainClamp
            return PNAM.IsLoaded() ? &PNAM->mainClamp : NULL;
        case 35: //faceClamp
            return UNAM.IsLoaded() ? &UNAM->faceClamp : NULL;
        case 36: //maleStrength
            return &ATTR.value.maleStrength;
        case 37: //maleIntelligence
            return &ATTR.value.maleIntelligence;
        case 38: //maleWillpower
            return &ATTR.value.maleWillpower;
        case 39: //maleAgility
            return &ATTR.value.maleAgility;
        case 40: //maleSpeed
            return &ATTR.value.maleSpeed;
        case 41: //maleEndurance
            return &ATTR.value.maleEndurance;
        case 42: //malePersonality
            return &ATTR.value.malePersonality;
        case 43: //maleLuck
            return &ATTR.value.maleLuck;
        case 44: //femaleStrength
            return &ATTR.value.femaleStrength;
        case 45: //femaleIntelligence
            return &ATTR.value.femaleIntelligence;
        case 46: //femaleWillpower
            return &ATTR.value.femaleWillpower;
        case 47: //femaleAgility
            return &ATTR.value.femaleAgility;
        case 48: //femaleSpeed
            return &ATTR.value.femaleSpeed;
        case 49: //femaleEndurance
            return &ATTR.value.femaleEndurance;
        case 50: //femalePersonality
            return &ATTR.value.femalePersonality;
        case 51: //femaleLuck
            return &ATTR.value.femaleLuck;
        case 52: //head_modPath
            return MOD0.IsLoaded() ? MOD0->MODL.value : NULL;
        case 53: //head_modb
            return MOD0.IsLoaded() ? &MOD0->MODB.value.MODB : NULL;
        case 54: //head_iconPath
            return MOD0.IsLoaded() ? MOD0->ICON.value : NULL;
        case 55: //head_modt_p
            *FieldValues = MOD0.IsLoaded() ? MOD0->MODT.value : NULL;
            return NULL;
        case 56: //maleEars_modPath
            return MOD1.IsLoaded() ? MOD1->MODL.value : NULL;
        case 57: //maleEars_modb
            return MOD1.IsLoaded() ? &MOD1->MODB.value.MODB : NULL;
        case 58: //maleEars_iconPath
            return MOD1.IsLoaded() ? MOD1->ICON.value : NULL;
        case 59: //maleEars_modt_p
            *FieldValues = MOD1.IsLoaded() ? MOD1->MODT.value : NULL;
            return NULL;
        case 60: //femaleEars_modPath
            return MOD2.IsLoaded() ? MOD2->MODL.value : NULL;
        case 61: //femaleEars_modb
            return MOD2.IsLoaded() ? &MOD2->MODB.value.MODB : NULL;
        case 62: //femaleEars_iconPath
            return MOD2.IsLoaded() ? MOD2->ICON.value : NULL;
        case 63: //femaleEars_modt_p
            *FieldValues = MOD2.IsLoaded() ? MOD2->MODT.value : NULL;
            return NULL;
        case 64: //mouth_modPath
            return MOD3.IsLoaded() ? MOD3->MODL.value : NULL;
        case 65: //mouth_modb
            return MOD3.IsLoaded() ? &MOD3->MODB.value.MODB : NULL;
        case 66: //mouth_iconPath
            return MOD3.IsLoaded() ? MOD3->ICON.value : NULL;
        case 67: //mouth_modt_p
            *FieldValues = MOD3.IsLoaded() ? MOD3->MODT.value : NULL;
            return NULL;
        case 68: //teethLower_modPath
            return MOD4.IsLoaded() ? MOD4->MODL.value : NULL;
        case 69: //teethLower_modb
            return MOD4.IsLoaded() ? &MOD4->MODB.value.MODB : NULL;
        case 70: //teethLower_iconPath
            return MOD4.IsLoaded() ? MOD4->ICON.value : NULL;
        case 71: //teethLower_modt_p
            *FieldValues = MOD4.IsLoaded() ? MOD4->MODT.value : NULL;
            return NULL;
        case 72: //teethUpper_modPath
            return MOD5.IsLoaded() ? MOD5->MODL.value : NULL;
        case 73: //teethUpper_modb
            return MOD5.IsLoaded() ? &MOD5->MODB.value.MODB : NULL;
        case 74: //teethUpper_iconPath
            return MOD5.IsLoaded() ? MOD5->ICON.value : NULL;
        case 75: //teethUpper_modt_p
            *FieldValues = MOD5.IsLoaded() ? MOD5->MODT.value : NULL;
            return NULL;
        case 76: //tongue_modPath
            return MOD6.IsLoaded() ? MOD6->MODL.value : NULL;
        case 77: //tongue_modb
            return MOD6.IsLoaded() ? &MOD6->MODB.value.MODB : NULL;
        case 78: //tongue_iconPath
            return MOD6.IsLoaded() ? MOD6->ICON.value : NULL;
        case 79: //tongue_modt_p
            *FieldValues = MOD6.IsLoaded() ? MOD6->MODT.value : NULL;
            return NULL;
        case 80: //leftEye_modPath
            return MOD7.IsLoaded() ? MOD7->MODL.value : NULL;
        case 81: //leftEye_modb
            return MOD7.IsLoaded() ? &MOD7->MODB.value.MODB : NULL;
        case 82: //leftEye_iconPath
            return MOD7.IsLoaded() ? MOD7->ICON.value : NULL;
        case 83: //leftEye_modt_p
            *FieldValues = MOD7.IsLoaded() ? MOD7->MODT.value : NULL;
            return NULL;
        case 84: //rightEye_modPath
            return MOD8.IsLoaded() ? MOD8->MODL.value : NULL;
        case 85: //rightEye_modb
            return MOD8.IsLoaded() ? &MOD8->MODB.value.MODB : NULL;
        case 86: //rightEye_iconPath
            return MOD8.IsLoaded() ? MOD8->ICON.value : NULL;
        case 87: //rightEye_modt_p
            *FieldValues = MOD8.IsLoaded() ? MOD8->MODT.value : NULL;
            return NULL;
        case 88: //maleTail_modPath
            return MMODL.IsLoaded() ? MMODL->MODL.value : NULL;
        case 89: //maleTail_modb
            return MMODL.IsLoaded() ? &MMODL->MODB.value.MODB : NULL;
        case 90: //maleTail_modt_p
            *FieldValues = MMODL.IsLoaded() ? MMODL->MODT.value : NULL;
            return NULL;
        case 91: //maleUpperBodyPath
            return MICON0.value;
        case 92: //maleLowerBodyPath
            return MICON1.value;
        case 93: //maleHandPath
            return MICON2.value;
        case 94: //maleFootPath
            return MICON3.value;
        case 95: //maleTailPath
            return MICON4.value;
        case 96: //femaleTail_modPath
            return FMODL.IsLoaded() ? FMODL->MODL.value : NULL;
        case 97: //femaleTail_modb
            return FMODL.IsLoaded() ? &FMODL->MODB.value.MODB : NULL;
        case 98: //femaleTail_modt_p
            *FieldValues = FMODL.IsLoaded() ? FMODL->MODT.value : NULL;
            return NULL;
        case 99: //femaleUpperBodyPath
            return FICON0.value;
        case 100: //femaleLowerBodyPath
            return FICON1.value;
        case 101: //femaleHandPath
            return FICON2.value;
        case 102: //femaleFootPath
            return FICON3.value;
        case 103: //femaleTailPath
            return FICON4.value;
        case 104: //hairs
            *FieldValues = HNAM.size() ? &HNAM[0] : NULL;
            return NULL;
        case 105: //eyes
            *FieldValues = ENAM.size() ? &ENAM[0] : NULL;
            return NULL;
        case 106: //fggs_p
            *FieldValues = FGGS.value;
            return NULL;
        case 107: //fgga_p
            *FieldValues = FGGA.value;
            return NULL;
        case 108: //fgts_p
            *FieldValues = FGTS.value;
            return NULL;
        case 109: //snam_p
            *FieldValues = &SNAM.value.SNAM[0];
            return NULL;
        default:
            return NULL;
        }
    }

bool RACERecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //text
            DESC.Copy((STRING)FieldValue);
            break;
        case 7: //spells
            SPLO.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                SPLO[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 8: //relations
            if(ListFieldID == 0) //relationsSize
                {
                ArraySize -= (UINT32)XNAM.size();
                while((SINT32)ArraySize > 0)
                    {
                    XNAM.push_back(new ReqSubRecord<GENXNAM>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete XNAM.back();
                    XNAM.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= XNAM.size())
                break;

            switch(ListFieldID)
                {
                case 1: //faction
                    XNAM[ListIndex]->value.faction = *(FORMID *)FieldValue;
                    return true;
                case 2: //mod
                    XNAM[ListIndex]->value.mod = *(SINT32 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 9: //skill1
            DATA.value.skills[0].value = *(SINT8 *)FieldValue;
            break;
        case 10: //skill1Boost
            DATA.value.skills[0].boost = *(SINT8 *)FieldValue;
            break;
        case 11: //skill2
            DATA.value.skills[1].value = *(SINT8 *)FieldValue;
            break;
        case 12: //skill2Boost
            DATA.value.skills[1].boost = *(SINT8 *)FieldValue;
            break;
        case 13: //skill3
            DATA.value.skills[2].value = *(SINT8 *)FieldValue;
            break;
        case 14: //skill3Boost
            DATA.value.skills[2].boost = *(SINT8 *)FieldValue;
            break;
        case 15: //skill4
            DATA.value.skills[3].value = *(SINT8 *)FieldValue;
            break;
        case 16: //skill4Boost
            DATA.value.skills[3].boost = *(SINT8 *)FieldValue;
            break;
        case 17: //skill5
            DATA.value.skills[4].value = *(SINT8 *)FieldValue;
            break;
        case 18: //skill5Boost
            DATA.value.skills[4].boost = *(SINT8 *)FieldValue;
            break;
        case 19: //skill6
            DATA.value.skills[5].value = *(SINT8 *)FieldValue;
            break;
        case 20: //skill6Boost
            DATA.value.skills[5].boost = *(SINT8 *)FieldValue;
            break;
        case 21: //skill7
            DATA.value.skills[6].value = *(SINT8 *)FieldValue;
            break;
        case 22: //skill7Boost
            DATA.value.skills[6].boost = *(SINT8 *)FieldValue;
            break;
        case 23: //unused1
            if(ArraySize != 2)
                break;
            DATA.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            DATA.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 24: //maleHeight
            DATA.value.maleHeight = *(FLOAT32 *)FieldValue;
            break;
        case 25: //femaleHeight
            DATA.value.femaleHeight = *(FLOAT32 *)FieldValue;
            break;
        case 26: //maleWeight
            DATA.value.maleWeight = *(FLOAT32 *)FieldValue;
            break;
        case 27: //femaleWeight
            DATA.value.femaleWeight = *(FLOAT32 *)FieldValue;
            break;
        case 28: //flags
            SetFlagMask(*(UINT32 *)FieldValue);
            break;
        case 29: //maleVoice
            VNAM.Load();
            //The voice is set to 0 if it happens to be set to the record's formID
            VNAM.value.maleVoice = *(FORMID *)FieldValue == formID ? 0 : *(FORMID *)FieldValue;
            return true;
        case 30: //femaleVoice
            VNAM.Load();
            //The voice is set to 0 if it happens to be set to the record's formID
            VNAM.value.femaleVoice = *(FORMID *)FieldValue == formID ? 0 : *(FORMID *)FieldValue;
            return true;
        case 31: //defaultHairMale
            DNAM.Load();
            DNAM.value.defaultHairMale = *(FORMID *)FieldValue;
            break;
        case 32: //defaultHairFemale
            DNAM.Load();
            DNAM.value.defaultHairFemale =  *(FORMID *)FieldValue;
            break;
        case 33: //defaultHairColor
            CNAM.value.defaultHairColor = *(UINT8 *)FieldValue;
            break;
        case 34: //mainClamp
            PNAM.Load();
            PNAM->mainClamp = *(FLOAT32 *)FieldValue;
            break;
        case 35: //faceClamp
            UNAM.Load();
            UNAM->faceClamp = *(FLOAT32 *)FieldValue;
            break;
        case 36: //maleStrength
            ATTR.value.maleStrength = *(UINT8 *)FieldValue;
            break;
        case 37: //maleIntelligence
            ATTR.value.maleIntelligence = *(UINT8 *)FieldValue;
            break;
        case 38: //maleWillpower
            ATTR.value.maleWillpower = *(UINT8 *)FieldValue;
            break;
        case 39: //maleAgility
            ATTR.value.maleAgility = *(UINT8 *)FieldValue;
            break;
        case 40: //maleSpeed
            ATTR.value.maleSpeed = *(UINT8 *)FieldValue;
            break;
        case 41: //maleEndurance
            ATTR.value.maleEndurance = *(UINT8 *)FieldValue;
            break;
        case 42: //malePersonality
            ATTR.value.malePersonality = *(UINT8 *)FieldValue;
            break;
        case 43: //maleLuck
            ATTR.value.maleLuck = *(UINT8 *)FieldValue;
            break;
        case 44: //femaleStrength
            ATTR.value.femaleStrength = *(UINT8 *)FieldValue;
            break;
        case 45: //femaleIntelligence
            ATTR.value.femaleIntelligence = *(UINT8 *)FieldValue;
            break;
        case 46: //femaleWillpower
            ATTR.value.femaleWillpower = *(UINT8 *)FieldValue;
            break;
        case 47: //femaleAgility
            ATTR.value.femaleAgility = *(UINT8 *)FieldValue;
            break;
        case 48: //femaleSpeed
            ATTR.value.femaleSpeed = *(UINT8 *)FieldValue;
            break;
        case 49: //femaleEndurance
            ATTR.value.femaleEndurance = *(UINT8 *)FieldValue;
            break;
        case 50: //femalePersonality
            ATTR.value.femalePersonality = *(UINT8 *)FieldValue;
            break;
        case 51: //femaleLuck
            ATTR.value.femaleLuck = *(UINT8 *)FieldValue;
            break;
        case 52: //head_modPath
            MOD0.Load();
            MOD0->MODL.Copy((STRING)FieldValue);
            break;
        case 53: //head_modb
            MOD0.Load();
            MOD0->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 54: //head_iconPath
            MOD0.Load();
            MOD0->ICON.Copy((STRING)FieldValue);
            break;
        case 55: //head_modt_p
            MOD0.Load();
            MOD0->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 56: //maleEars_modPath
            MOD1.Load();
            MOD1->MODL.Copy((STRING)FieldValue);
            break;
        case 57: //maleEars_modb
            MOD1.Load();
            MOD1->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 58: //maleEars_iconPath
            MOD1.Load();
            MOD1->ICON.Copy((STRING)FieldValue);
            break;
        case 59: //maleEars_modt_p
            MOD1.Load();
            MOD1->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 60: //femaleEars_modPath
            MOD2.Load();
            MOD2->MODL.Copy((STRING)FieldValue);
            break;
        case 61: //femaleEars_modb
            MOD2.Load();
            MOD2->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 62: //femaleEars_iconPath
            MOD2.Load();
            MOD2->ICON.Copy((STRING)FieldValue);
            break;
        case 63: //femaleEars_modt_p
            MOD2.Load();
            MOD2->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 64: //mouth_modPath
            MOD3.Load();
            MOD3->MODL.Copy((STRING)FieldValue);
            break;
        case 65: //mouth_modb
            MOD3.Load();
            MOD3->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 66: //mouth_iconPath
            MOD3.Load();
            MOD3->ICON.Copy((STRING)FieldValue);
            break;
        case 67: //mouth_modt_p
            MOD3.Load();
            MOD3->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 68: //teethLower_modPath
            MOD4.Load();
            MOD4->MODL.Copy((STRING)FieldValue);
            break;
        case 69: //teethLower_modb
            MOD4.Load();
            MOD4->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 70: //teethLower_iconPath
            MOD4.Load();
            MOD4->ICON.Copy((STRING)FieldValue);
            break;
        case 71: //teethLower_modt_p
            MOD4.Load();
            MOD4->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 72: //teethUpper_modPath
            MOD5.Load();
            MOD5->MODL.Copy((STRING)FieldValue);
            break;
        case 73: //teethUpper_modb
            MOD5.Load();
            MOD5->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 74: //teethUpper_iconPath
            MOD5.Load();
            MOD5->ICON.Copy((STRING)FieldValue);
            break;
        case 75: //teethUpper_modt_p
            MOD5.Load();
            MOD5->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 76: //tongue_modPath
            MOD6.Load();
            MOD6->MODL.Copy((STRING)FieldValue);
            break;
        case 77: //tongue_modb
            MOD6.Load();
            MOD6->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 78: //tongue_iconPath
            MOD6.Load();
            MOD6->ICON.Copy((STRING)FieldValue);
            break;
        case 79: //tongue_modt_p
            MOD6.Load();
            MOD6->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 80: //leftEye_modPath
            MOD7.Load();
            MOD7->MODL.Copy((STRING)FieldValue);
            break;
        case 81: //leftEye_modb
            MOD7.Load();
            MOD7->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 82: //leftEye_iconPath
            MOD7.Load();
            MOD7->ICON.Copy((STRING)FieldValue);
            break;
        case 83: //leftEye_modt_p
            MOD7.Load();
            MOD7->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 84: //rightEye_modPath
            MOD8.Load();
            MOD8->MODL.Copy((STRING)FieldValue);
            break;
        case 85: //rightEye_modb
            MOD8.Load();
            MOD8->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 86: //rightEye_iconPath
            MOD8.Load();
            MOD8->ICON.Copy((STRING)FieldValue);
            break;
        case 87: //rightEye_modt_p
            MOD8.Load();
            MOD8->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 88: //maleTail_modPath
            MMODL.Load();
            MMODL->MODL.Copy((STRING)(STRING)FieldValue);
            break;
        case 89: //maleTail_modb
            MMODL.Load();
            MMODL->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 90: //maleTail_modt_p
            MMODL.Load();
            MMODL->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 91: //maleUpperBodyPath
            MICON0.Copy((STRING)FieldValue);
            break;
        case 92: //maleLowerBodyPath
            MICON1.Copy((STRING)FieldValue);
            break;
        case 93: //maleHandPath
            MICON2.Copy((STRING)FieldValue);
            break;
        case 94: //maleFootPath
            MICON3.Copy((STRING)FieldValue);
            break;
        case 95: //maleTailPath
            MICON4.Copy((STRING)FieldValue);
            break;
        case 96: //femaleTail_modPath
            FMODL.Load();
            FMODL->MODL.Copy((STRING)(STRING)FieldValue);
            break;
        case 97: //femaleTail_modb
            FMODL.Load();
            FMODL->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 98: //femaleTail_modt_p
            FMODL.Load();
            FMODL->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 99: //femaleUpperBodyPath
            FICON0.Copy((STRING)FieldValue);
            break;
        case 100: //femaleLowerBodyPath
            FICON1.Copy((STRING)FieldValue);
            break;
        case 101: //femaleHandPath
            FICON2.Copy((STRING)FieldValue);
            break;
        case 102: //femaleFootPath
            FICON3.Copy((STRING)FieldValue);
            break;
        case 103: //femaleTailPath
            FICON4.Copy((STRING)FieldValue);
            break;
        case 104: //hairs
            HNAM.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                HNAM[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 105: //eyes
            ENAM.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                ENAM[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 106: //fggs_p
            if(ArraySize != 200)
                break;
            FGGS.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 107: //fgga_p
            if(ArraySize != 120)
                break;
            FGGA.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 108: //fgts_p
            if(ArraySize != 200)
                break;
            FGTS.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 109: //snam_p
            if(ArraySize != 2)
                break;
            SNAM.value.SNAM[0] = ((UINT8ARRAY)FieldValue)[0];
            SNAM.value.SNAM[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        default:
            break;
        }
    return false;
    }

void RACERecord::DeleteField(FIELD_IDENTIFIERS)
    {
    RACEDATA defaultDATA;
    RACEVNAM defaultVNAM;
    RACEDNAM defaultDNAM;
    RACEATTR defaultATTR;

    GENXNAM defaultXNAM;

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
        case 6: //text
            DESC.Unload();
            return;
        case 7: //spells
            SPLO.clear();
            return;
        case 8: //relations
            if(ListFieldID == 0) //relations
                {
                for(UINT32 x = 0; x < (UINT32)XNAM.size(); x++)
                    delete XNAM[x];
                XNAM.clear();
                return;
                }

            if(ListIndex >= XNAM.size())
                return;

            switch(ListFieldID)
                {
                case 1: //faction
                    XNAM[ListIndex]->value.faction = defaultXNAM.faction;
                    return;
                case 2: //mod
                    XNAM[ListIndex]->value.mod = defaultXNAM.mod;
                    return;
                default:
                    return;
                }
        case 9: //skill1
            DATA.value.skills[0].value = defaultDATA.skills[0].value;
            return;
        case 10: //skill1Boost
            DATA.value.skills[0].boost = defaultDATA.skills[0].boost;
            return;
        case 11: //skill2
            DATA.value.skills[1].value = defaultDATA.skills[1].value;
            return;
        case 12: //skill2Boost
            DATA.value.skills[1].boost = defaultDATA.skills[1].boost;
            return;
        case 13: //skill3
            DATA.value.skills[2].value = defaultDATA.skills[2].value;
            return;
        case 14: //skill3Boost
            DATA.value.skills[2].boost = defaultDATA.skills[2].boost;
            return;
        case 15: //skill4
            DATA.value.skills[3].value = defaultDATA.skills[3].value;
            return;
        case 16: //skill4Boost
            DATA.value.skills[3].boost = defaultDATA.skills[3].boost;
            return;
        case 17: //skill5
            DATA.value.skills[4].value = defaultDATA.skills[4].value;
            return;
        case 18: //skill5Boost
            DATA.value.skills[4].boost = defaultDATA.skills[4].boost;
            return;
        case 19: //skill6
            DATA.value.skills[5].value = defaultDATA.skills[5].value;
            return;
        case 20: //skill6Boost
            DATA.value.skills[5].boost = defaultDATA.skills[5].boost;
            return;
        case 21: //skill7
            DATA.value.skills[6].value = defaultDATA.skills[6].value;
            return;
        case 22: //skill7Boost
            DATA.value.skills[6].boost = defaultDATA.skills[6].boost;
            return;
        case 23: //unused1
            DATA.value.unused1[0] = defaultDATA.unused1[0];
            DATA.value.unused1[1] = defaultDATA.unused1[1];
            return;
        case 24: //maleHeight
            DATA.value.maleHeight = defaultDATA.maleHeight;
            return;
        case 25: //femaleHeight
            DATA.value.femaleHeight = defaultDATA.femaleHeight;
            return;
        case 26: //maleWeight
            DATA.value.maleWeight = defaultDATA.maleWeight;
            return;
        case 27: //femaleWeight
            DATA.value.femaleWeight = defaultDATA.femaleWeight;
            return;
        case 28: //flags
            DATA.value.flags = defaultDATA.flags;
            return;
        case 29: //maleVoice
            VNAM.value.maleVoice = defaultVNAM.maleVoice;
            return;
        case 30: //femaleVoice
            VNAM.value.femaleVoice = defaultVNAM.femaleVoice;
            return;
        case 31: //defaultHairMale
            DNAM.value.defaultHairMale = defaultDNAM.defaultHairMale;
            return;
        case 32: //defaultHairFemale
            DNAM.value.defaultHairFemale = defaultDNAM.defaultHairFemale;
            return;
        case 33: //defaultHairColor
            CNAM.Unload();
            return;
        case 34: //mainClamp
            PNAM.Unload();
            return;
        case 35: //faceClamp
            UNAM.Unload();
            return;
        case 36: //maleStrength
            ATTR.value.maleStrength = defaultATTR.maleStrength;
            return;
        case 37: //maleIntelligence
            ATTR.value.maleIntelligence = defaultATTR.maleIntelligence;
            return;
        case 38: //maleWillpower
            ATTR.value.maleWillpower = defaultATTR.maleWillpower;
            return;
        case 39: //maleAgility
            ATTR.value.maleAgility = defaultATTR.maleAgility;
            return;
        case 40: //maleSpeed
            ATTR.value.maleSpeed = defaultATTR.maleSpeed;
            return;
        case 41: //maleEndurance
            ATTR.value.maleEndurance = defaultATTR.maleEndurance;
            return;
        case 42: //malePersonality
            ATTR.value.malePersonality = defaultATTR.malePersonality;
            return;
        case 43: //maleLuck
            ATTR.value.maleLuck = defaultATTR.maleLuck;
            return;
        case 44: //femaleStrength
            ATTR.value.femaleStrength = defaultATTR.femaleStrength;
            return;
        case 45: //femaleIntelligence
            ATTR.value.femaleIntelligence = defaultATTR.femaleIntelligence;
            return;
        case 46: //femaleWillpower
            ATTR.value.femaleWillpower = defaultATTR.femaleWillpower;
            return;
        case 47: //femaleAgility
            ATTR.value.femaleAgility = defaultATTR.femaleAgility;
            return;
        case 48: //femaleSpeed
            ATTR.value.femaleSpeed = defaultATTR.femaleSpeed;
            return;
        case 49: //femaleEndurance
            ATTR.value.femaleEndurance = defaultATTR.femaleEndurance;
            return;
        case 50: //femalePersonality
            ATTR.value.femalePersonality = defaultATTR.femalePersonality;
            return;
        case 51: //femaleLuck
            ATTR.value.femaleLuck = defaultATTR.femaleLuck;
            return;
        case 52: //head_modPath
            if(MOD0.IsLoaded())
                MOD0->MODL.Unload();
            return;
        case 53: //head_modb
            if(MOD0.IsLoaded())
                MOD0->MODB.Unload();
            return;
        case 54: //head_iconPath
            if(MOD0.IsLoaded())
                MOD0->ICON.Unload();
            return;
        case 55: //head_modt_p
            if(MOD0.IsLoaded())
                MOD0->MODT.Unload();
            return;
        case 56: //maleEars_modPath
            if(MOD1.IsLoaded())
                MOD1->MODL.Unload();
            return;
        case 57: //maleEars_modb
            if(MOD1.IsLoaded())
                MOD1->MODB.Unload();
            return;
        case 58: //maleEars_iconPath
            if(MOD1.IsLoaded())
                MOD1->ICON.Unload();
            return;
        case 59: //maleEars_modt_p
            if(MOD1.IsLoaded())
                MOD1->MODT.Unload();
            return;
        case 60: //femaleEars_modPath
            if(MOD2.IsLoaded())
                MOD2->MODL.Unload();
            return;
        case 61: //femaleEars_modb
            if(MOD2.IsLoaded())
                MOD2->MODB.Unload();
            return;
        case 62: //femaleEars_iconPath
            if(MOD2.IsLoaded())
                MOD2->ICON.Unload();
            return;
        case 63: //femaleEars_modt_p
            if(MOD2.IsLoaded())
                MOD2->MODT.Unload();
            return;
        case 64: //mouth_modPath
            if(MOD3.IsLoaded())
                MOD3->MODL.Unload();
            return;
        case 65: //mouth_modb
            if(MOD3.IsLoaded())
                MOD3->MODB.Unload();
            return;
        case 66: //mouth_iconPath
            if(MOD3.IsLoaded())
                MOD3->ICON.Unload();
            return;
        case 67: //mouth_modt_p
            if(MOD3.IsLoaded())
                MOD3->MODT.Unload();
            return;
        case 68: //teethLower_modPath
            if(MOD4.IsLoaded())
                MOD4->MODL.Unload();
            return;
        case 69: //teethLower_modb
            if(MOD4.IsLoaded())
                MOD4->MODB.Unload();
            return;
        case 70: //teethLower_iconPath
            if(MOD4.IsLoaded())
                MOD4->ICON.Unload();
            return;
        case 71: //teethLower_modt_p
            if(MOD4.IsLoaded())
                MOD4->MODT.Unload();
            return;
        case 72: //teethUpper_modPath
            if(MOD5.IsLoaded())
                MOD5->MODL.Unload();
            return;
        case 73: //teethUpper_modb
            if(MOD5.IsLoaded())
                MOD5->MODB.Unload();
            return;
        case 74: //teethUpper_iconPath
            if(MOD5.IsLoaded())
                MOD5->ICON.Unload();
            return;
        case 75: //teethUpper_modt_p
            if(MOD5.IsLoaded())
                MOD5->MODT.Unload();
            return;
        case 76: //tongue_modPath
            if(MOD6.IsLoaded())
                MOD6->MODL.Unload();
            return;
        case 77: //tongue_modb
            if(MOD6.IsLoaded())
                MOD6->MODB.Unload();
            return;
        case 78: //tongue_iconPath
            if(MOD6.IsLoaded())
                MOD6->ICON.Unload();
            return;
        case 79: //tongue_modt_p
            if(MOD6.IsLoaded())
                MOD6->MODT.Unload();
            return;
        case 80: //leftEye_modPath
            if(MOD7.IsLoaded())
                MOD7->MODL.Unload();
            return;
        case 81: //leftEye_modb
            if(MOD7.IsLoaded())
                MOD7->MODB.Unload();
            return;
        case 82: //leftEye_iconPath
            if(MOD7.IsLoaded())
                MOD7->ICON.Unload();
            return;
        case 83: //leftEye_modt_p
            if(MOD7.IsLoaded())
                MOD7->MODT.Unload();
            return;
        case 84: //rightEye_modPath
            if(MOD8.IsLoaded())
                MOD8->MODL.Unload();
            return;
        case 85: //rightEye_modb
            if(MOD8.IsLoaded())
                MOD8->MODB.Unload();
            return;
        case 86: //rightEye_iconPath
            if(MOD8.IsLoaded())
                MOD8->ICON.Unload();
            return;
        case 87: //rightEye_modt_p
            if(MOD8.IsLoaded())
                MOD8->MODT.Unload();
            return;
        case 88: //maleTail_modPath
            if(MMODL.IsLoaded())
                MMODL->MODL.Unload();
            return;
        case 89: //maleTailModel_modb
            if(MMODL.IsLoaded())
                MMODL->MODB.Unload();
            return;
        case 90: //maleTailModel_modt_p
            if(MMODL.IsLoaded())
                MMODL->MODT.Unload();
            return;
        case 91: //maleUpperBodyPath
            MICON0.Unload();
            return;
        case 92: //maleLowerBodyPath
            MICON1.Unload();
            return;
        case 93: //maleHandPath
            MICON2.Unload();
            return;
        case 94: //maleFootPath
            MICON3.Unload();
            return;
        case 95: //maleTailPath
            MICON4.Unload();
            return;
        case 96: //femaleTail_modPath
            if(FMODL.IsLoaded())
                FMODL->MODL.Unload();
            return;
        case 97: //femaleTailModel_modb
            if(FMODL.IsLoaded())
                FMODL->MODB.Unload();
            return;
        case 98: //femaleTailModel_modt_p
            if(FMODL.IsLoaded())
                FMODL->MODT.Unload();
            return;
        case 99: //femaleUpperBodyPath
            FICON0.Unload();
            return;
        case 100: //femaleLowerBodyPath
            FICON1.Unload();
            return;
        case 101: //femaleHandPath
            FICON2.Unload();
            return;
        case 102: //femaleFootPath
            FICON3.Unload();
            return;
        case 103: //femaleTailPath
            FICON4.Unload();
            return;
        case 104: //hairs
            HNAM.clear();
            return;
        case 105: //eyes
            ENAM.clear();
            return;
        case 106: //fggs_p
            FGGS.Unload();
            return;
        case 107: //fgga_p
            FGGA.Unload();
            return;
        case 108: //fgts_p
            FGTS.Unload();
            return;
        case 109: //snam_p
            SNAM.Unload();
            return;
        default:
            return;
        }
    }