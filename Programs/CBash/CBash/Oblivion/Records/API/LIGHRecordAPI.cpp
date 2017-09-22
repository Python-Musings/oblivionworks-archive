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
#include "..\LIGHRecord.h"

UINT32 LIGHRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //modPath
            return ISTRING_FIELD;
        case 6: //modb
            return FLOAT32_FIELD;
        case 7: //modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MODL.IsLoaded() ? MODL->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 8: //script
            return FORMID_FIELD;
        case 9: //full
            return STRING_FIELD;
        case 10: //iconPath
            return ISTRING_FIELD;
        case 11: //duration
            return SINT32_FIELD;
        case 12: //radius
            return UINT32_FIELD;
        case 13: //red
            return UINT8_FIELD;
        case 14: //green
            return UINT8_FIELD;
        case 15: //blue
            return UINT8_FIELD;
        case 16: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 1;
                default:
                    return UNKNOWN_FIELD;
                }
        case 17: //flags
            return UINT32_FLAG_FIELD;
        case 18: //falloff
            return FLOAT32_FIELD;
        case 19: //fov
            return FLOAT32_FIELD;
        case 20: //value
            return UINT32_FIELD;
        case 21: //weight
            return FLOAT32_FIELD;
        case 22: //fade
            return FLOAT32_FIELD;
        case 23: //sound
            return FORMID_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * LIGHRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //modPath
            return MODL.IsLoaded() ? MODL->MODL.value : NULL;
        case 6: //modb
            return MODL.IsLoaded() ? &MODL->MODB.value.MODB : NULL;
        case 7: //modt_p
            *FieldValues = MODL.IsLoaded() ? MODL->MODT.value : NULL;
            return NULL;
        case 8: //script
            return SCRI.IsLoaded() ? &SCRI->fid : NULL;
        case 9: //full
            return FULL.value;
        case 10: //iconPath
            return ICON.value;
        case 11: //duration
            return &DATA.value.duration;
        case 12: //radius
            return &DATA.value.radius;
        case 13: //red
            return &DATA.value.color.red;
        case 14: //green
            return &DATA.value.color.green;
        case 15: //blue
            return &DATA.value.color.blue;
        case 16: //unused1
            *FieldValues = &DATA.value.color.unused1;
            return NULL;
        case 17: //flags
            return &DATA.value.flags;
        case 18: //falloff
            return &DATA.value.falloff;
        case 19: //fov
            return &DATA.value.fov;
        case 20: //value
            return &DATA.value.value;
        case 21: //weight
            return &DATA.value.weight;
        case 22: //fade
            return FNAM.IsLoaded() ? &FNAM->fade : NULL;
        case 23: //sound
            return SNAM.IsLoaded() ? &SNAM->fid : NULL;
        default:
            return NULL;
        }
    }

bool LIGHRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //modPath
            MODL.Load();
            MODL->MODL.Copy((STRING)FieldValue);
            break;
        case 6: //modb
            MODL.Load();
            MODL->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 7: //modt_p
            MODL.Load();
            MODL->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 8: //script
            SCRI.Load();
            SCRI->fid = *(FORMID *)FieldValue;
            return true;
        case 9: //full
            FULL.Copy((STRING)FieldValue);
            break;
        case 10: //iconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 11: //duration
            DATA.value.duration = *(SINT32 *)FieldValue;
            break;
        case 12: //radius
            DATA.value.radius = *(UINT32 *)FieldValue;
            break;
        case 13: //red
            DATA.value.color.red = *(UINT8 *)FieldValue;
            break;
        case 14: //green
            DATA.value.color.green = *(UINT8 *)FieldValue;
            break;
        case 15: //blue
            DATA.value.color.blue = *(UINT8 *)FieldValue;
            break;
        case 16: //unused1
            if(ArraySize != 1)
                break;
            DATA.value.color.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 17: //flags
            SetFlagMask(*(UINT32 *)FieldValue);
            break;
        case 18: //falloff
            DATA.value.falloff = *(FLOAT32 *)FieldValue;
            break;
        case 19: //fov
            DATA.value.fov = *(FLOAT32 *)FieldValue;
            break;
        case 20: //value
            DATA.value.value = *(UINT32 *)FieldValue;
            break;
        case 21: //weight
            DATA.value.weight = *(FLOAT32 *)FieldValue;
            break;
        case 22: //fade
            FNAM.Load();
            FNAM->fade = *(FLOAT32 *)FieldValue;
            break;
        case 23: //sound
            SNAM.Load();
            SNAM->fid = *(FORMID *)FieldValue;
            return true;
        default:
            break;
        }
    return false;
    }

void LIGHRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    LIGHDATA defaultDATA;

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
        case 5: //modPath
            if(MODL.IsLoaded())
                MODL->MODL.Unload();
            return;
        case 6: //modb
            if(MODL.IsLoaded())
                MODL->MODB.Unload();
            return;
        case 7: //modt_p
            if(MODL.IsLoaded())
                MODL->MODT.Unload();
            return;
        case 8: //script
            SCRI.Unload();
            return;
        case 9: //full
            FULL.Unload();
            return;
        case 10: //iconPath
            ICON.Unload();
            return;
        case 11: //duration
            DATA.value.duration = defaultDATA.duration;
            return;
        case 12: //radius
            DATA.value.radius = defaultDATA.radius;
            return;
        case 13: //red
            DATA.value.color.red = defaultDATA.color.red;
            return;
        case 14: //green
            DATA.value.color.green = defaultDATA.color.green;
            return;
        case 15: //blue
            DATA.value.color.blue = defaultDATA.color.blue;
            return;
        case 16: //unused1
            DATA.value.color.unused1 = defaultDATA.color.unused1;
            return;
        case 17: //flags
            DATA.value.flags = defaultDATA.flags;
            return;
        case 18: //falloff
            DATA.value.falloff = defaultDATA.falloff;
            return;
        case 19: //fov
            DATA.value.fov = defaultDATA.fov;
            return;
        case 20: //value
            DATA.value.value = defaultDATA.value;
            return;
        case 21: //weight
            DATA.value.weight = defaultDATA.weight;
            return;
        case 22: //fade
            FNAM.Unload();
            return;
        case 23: //sound
            SNAM.Unload();
            return;
        default:
            return;
        }
    }