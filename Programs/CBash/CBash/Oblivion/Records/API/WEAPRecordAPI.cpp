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
#include "..\WEAPRecord.h"

UINT32 WEAPRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 9: //iconPath
            return ISTRING_FIELD;
        case 10: //script
            return FORMID_FIELD;
        case 11: //enchantment
            return FORMID_FIELD;
        case 12: //enchantPoints
            return UINT16_FIELD;
        case 13: //weaponType
            return UINT32_TYPE_FIELD;
        case 14: //speed
            return FLOAT32_FIELD;
        case 15: //reach
            return FLOAT32_FIELD;
        case 16: //flags
            return UINT32_FLAG_FIELD;
        case 17: //value
            return UINT32_FIELD;
        case 18: //health
            return UINT32_FIELD;
        case 19: //weight
            return FLOAT32_FIELD;
        case 20: //damage
            return UINT16_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * WEAPRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 9: //iconPath
            return ICON.value;
        case 10: //script
            return SCRI.IsLoaded() ? &SCRI->fid : NULL;
        case 11: //enchantment
            return ENAM.IsLoaded() ? &ENAM->fid : NULL;
        case 12: //enchantPoints
            return ANAM.IsLoaded() ? &ANAM->enchantPoints : NULL;
        case 13: //weaponType
            return &DATA.value.weaponType;
        case 14: //speed
            return &DATA.value.speed;
        case 15: //reach
            return &DATA.value.reach;
        case 16: //flags
            return &DATA.value.flags;
        case 17: //value
            return &DATA.value.value;
        case 18: //health
            return &DATA.value.health;
        case 19: //weight
            return &DATA.value.weight;
        case 20: //damage
            return &DATA.value.damage;
        default:
            return NULL;
        }
    }

bool WEAPRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 9: //iconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 10: //script
            SCRI.Load();
            SCRI->fid = *(FORMID *)FieldValue;
            return true;
        case 11: //enchantment
            ENAM.Load();
            ENAM->fid = *(FORMID *)FieldValue;
            return true;
        case 12: //enchantPoints
            ANAM.Load();
            ANAM->enchantPoints = *(UINT16 *)FieldValue;
            break;
        case 13: //weaponType
            SetType(*(UINT32 *)FieldValue);
            break;
        case 14: //speed
            DATA.value.speed = *(FLOAT32 *)FieldValue;
            break;
        case 15: //reach
            DATA.value.reach = *(FLOAT32 *)FieldValue;
            break;
        case 16: //flags
            SetFlagMask(*(UINT32 *)FieldValue);
            break;
        case 17: //value
            DATA.value.value = *(UINT32 *)FieldValue;
            break;
        case 18: //health
            DATA.value.health = *(UINT32 *)FieldValue;
            break;
        case 19: //weight
            DATA.value.weight = *(FLOAT32 *)FieldValue;
            break;
        case 20: //damage
            DATA.value.damage = *(UINT16 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void WEAPRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    WEAPDATA defaultDATA;

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
        case 9: //iconPath
            ICON.Unload();
            return;
        case 10: //script
            SCRI.Unload();
            return;
        case 11: //enchantment
            ENAM.Unload();
            return;
        case 12: //enchantPoints
            ANAM.Unload();
            return;
        case 13: //weaponType
            DATA.value.weaponType = defaultDATA.weaponType;
            return;
        case 14: //speed
            DATA.value.speed = defaultDATA.speed;
            return;
        case 15: //reach
            DATA.value.reach = defaultDATA.reach;
            return;
        case 16: //flags
            DATA.value.flags = defaultDATA.flags;
            return;
        case 17: //value
            DATA.value.value = defaultDATA.value;
            return;
        case 18: //health
            DATA.value.health = defaultDATA.health;
            return;
        case 19: //weight
            DATA.value.weight = defaultDATA.weight;
            return;
        case 20: //damage
            DATA.value.damage = defaultDATA.damage;
            return;
        default:
            return;
        }
    return;
    }