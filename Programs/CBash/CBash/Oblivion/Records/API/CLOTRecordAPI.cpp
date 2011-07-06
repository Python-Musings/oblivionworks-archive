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
#include "..\CLOTRecord.h"

UINT32 CLOTRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //script
            return FORMID_FIELD;
        case 7: //enchantment
            return FORMID_FIELD;
        case 8: //enchantPoints
            return UINT16_FIELD;
        case 9: //flags
            return UINT32_FLAG_FIELD;
        case 10: //maleBody_modPath
            return STRING_FIELD;
        case 11: //maleBody_modb
            return FLOAT32_FIELD;
        case 12: //maleBody_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MODL.IsLoaded() ? MODL->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 13: //maleWorld_modPath
            return STRING_FIELD;
        case 14: //maleWorld_modb
            return FLOAT32_FIELD;
        case 15: //maleWorld_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD2.IsLoaded() ? MOD2->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 16: //maleIconPath
            return ISTRING_FIELD;
        case 17: //femaleBody_modPath
            return STRING_FIELD;
        case 18: //femaleBody_modb
            return FLOAT32_FIELD;
        case 19: //femaleBody_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD3.IsLoaded() ? MOD3->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 20: //femaleWorld_modPath
            return STRING_FIELD;
        case 21: //femaleWorld_modb
            return FLOAT32_FIELD;
        case 22: //femaleWorld_modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MOD4.IsLoaded() ? MOD4->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 23: //femaleIconPath
            return ISTRING_FIELD;
        case 24: //value
            return UINT32_FIELD;
        case 25: //weight
            return FLOAT32_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * CLOTRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //script
            return SCRI.IsLoaded() ? &SCRI->fid : NULL;
        case 7: //enchantment
            return ENAM.IsLoaded() ? &ENAM->fid : NULL;
        case 8: //enchantPoints
            return ANAM.IsLoaded() ? &ANAM->enchantPoints : NULL;
        case 9: //flags
            return &BMDT.value.flags;
        case 10: //maleBody_modPath
            return MODL.IsLoaded() ? MODL->MODL.value : NULL;
        case 11: //maleBody_modb
            return MODL.IsLoaded() ? &MODL->MODB.value.MODB : NULL;
        case 12: //maleBody_modt_p
            *FieldValues = MODL.IsLoaded() ? MODL->MODT.value : NULL;
            return NULL;
        case 13: //maleWorld_modPath
            return MOD2.IsLoaded() ? MOD2->MODL.value : NULL;
        case 14: //maleWorld_modb
            return MOD2.IsLoaded() ? &MOD2->MODB.value.MODB : NULL;
        case 15: //maleWorld_modt_p
            *FieldValues = MOD2.IsLoaded() ? MOD2->MODT.value : NULL;
            return NULL;
        case 16: //maleIconPath
            return ICON.value;
        case 17: //femaleBody_modPath
            return MOD3.IsLoaded() ? MOD3->MODL.value : NULL;
        case 18: //femaleBody_modb
            return MOD3.IsLoaded() ? &MOD3->MODB.value.MODB : NULL;
        case 19: //femaleBody_modt_p
            *FieldValues = MOD3.IsLoaded() ? MOD3->MODT.value : NULL;
            return NULL;
        case 20: //femaleWorld_modPath
            return MOD4.IsLoaded() ? MOD4->MODL.value : NULL;
        case 21: //femaleWorld_modb
            return MOD4.IsLoaded() ? &MOD4->MODB.value.MODB : NULL;
        case 22: //femaleWorld_modt_p
            *FieldValues = MOD4.IsLoaded() ? MOD4->MODT.value : NULL;
            return NULL;
        case 23: //femaleIconPath
            return ICO2.value;
        case 24: //value
            return &DATA.value.value;
        case 25: //weight
            return &DATA.value.weight;
        default:
            return NULL;
        }
    }

bool CLOTRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //script
            SCRI.Load();
            SCRI->fid = *(FORMID *)FieldValue;
            return true;
        case 7: //enchantment
            ENAM.Load();
            ENAM->fid = *(FORMID *)FieldValue;
            return true;
        case 8: //enchantPoints
            ANAM.Load();
            ANAM->enchantPoints = *(UINT16 *)FieldValue;
            break;
        case 9: //flags
            SetFlagMask(*(UINT32 *)FieldValue);
            break;
        case 10: //maleBody_modPath
            MODL.Load();
            MODL->MODL.Copy((STRING)FieldValue);
            break;
        case 11: //maleBody_modb
            MODL.Load();
            MODL->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 12: //maleBody_modt_p
            MODL.Load();
            MODL->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 13: //maleWorld_modPath
            MOD2.Load();
            MOD2->MODL.Copy((STRING)FieldValue);
            break;
        case 14: //maleWorld_modb
            MOD2.Load();
            MOD2->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 15: //maleWorld_modt_p
            MOD2.Load();
            MOD2->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 16: //maleIconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 17: //femaleBody_modPath
            MOD3.Load();
            MOD3->MODL.Copy((STRING)FieldValue);
            break;
        case 18: //femaleBody_modb
            MOD3.Load();
            MOD3->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 19: //femaleBody_modt_p
            MOD3.Load();
            MOD3->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 20: //femaleWorld_modPath
            MOD4.Load();
            MOD4->MODL.Copy((STRING)FieldValue);
            break;
        case 21: //femaleWorld_modb
            MOD4.Load();
            MOD4->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 22: //femaleWorld_modt_p
            MOD4.Load();
            MOD4->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 23: //femaleIconPath
            ICO2.Copy((STRING)FieldValue);
            break;
        case 24: //value
            DATA.value.value = *(UINT32 *)FieldValue;
            break;
        case 25: //weight
            DATA.value.weight = *(FLOAT32 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void CLOTRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GENVALUEWEIGHT defaultDATA;

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
        case 6: //script
            SCRI.Unload();
            return;
        case 7: //enchantment
            ENAM.Unload();
            return;
        case 8: //enchantPoints
            ANAM.Unload();
            return;
        case 9: //flags
            BMDT.Unload();
            return;
        case 10: //maleBody_modPath
            if(MODL.IsLoaded())
                MODL->MODL.Unload();
            return;
        case 11: //maleBody_modb
            if(MODL.IsLoaded())
                MODL->MODB.Unload();
            return;
        case 12: //maleBody_modt_p
            if(MODL.IsLoaded())
                MODL->MODT.Unload();
            return;
        case 13: //maleWorld_modPath
            if(MOD2.IsLoaded())
                MOD2->MODL.Unload();
            return;
        case 14: //maleWorld_modb
            if(MOD2.IsLoaded())
                MOD2->MODB.Unload();
            return;
        case 15: //maleWorld_modt_p
            if(MOD2.IsLoaded())
                MOD2->MODT.Unload();
            return;
        case 16: //maleIconPath
            ICON.Unload();
            return;
        case 17: //femaleBody_modPath
            if(MOD3.IsLoaded())
                MOD3->MODL.Unload();
            return;
        case 18: //femaleBody_modb
            if(MOD3.IsLoaded())
                MOD3->MODB.Unload();
            return;
        case 19: //femaleBody_modt_p
            if(MOD3.IsLoaded())
                MOD3->MODT.Unload();
            return;
        case 20: //femaleWorld_modPath
            if(MOD4.IsLoaded())
                MOD4->MODL.Unload();
            return;
        case 21: //femaleWorld_modb
            if(MOD4.IsLoaded())
                MOD4->MODB.Unload();
            return;
        case 22: //femaleWorld_modt_p
            if(MOD4.IsLoaded())
                MOD4->MODT.Unload();
            return;
        case 23: //femaleIconPath
            ICO2.Unload();
            return;
        case 24: //value
            DATA.value.value = defaultDATA.value;
            return;
        case 25: //weight
            DATA.value.weight = defaultDATA.weight;
            return;
        default:
            return;
        }
    }