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
#include "..\GRASRecord.h"

UINT32 GRASRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 8: //density
            return UINT8_FIELD;
        case 9: //minSlope
            return UINT8_FIELD;
        case 10: //maxSlope
            return UINT8_FIELD;
        case 11: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 1;
                default:
                    return UNKNOWN_FIELD;
                }
        case 12: //waterDistance
            return UINT16_FIELD;
        case 13: //unused2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 14: //waterOp
            return UINT32_FIELD;
        case 15: //posRange
            return FLOAT32_FIELD;
        case 16: //heightRange
            return FLOAT32_FIELD;
        case 17: //colorRange
            return FLOAT32_FIELD;
        case 18: //wavePeriod
            return FLOAT32_FIELD;
        case 19: //flags
            return UINT8_FLAG_FIELD;
        case 20: //unused3
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
    }

void * GRASRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 8: //density
            return &DATA.value.density;
        case 9: //minSlope
            return &DATA.value.minSlope;
        case 10: //maxSlope
            return &DATA.value.maxSlope;
        case 11: //unused1
            *FieldValues = &DATA.value.unused1;
            return NULL;
        case 12: //waterDistance
            return &DATA.value.waterDistance;
        case 13: //unused2
            *FieldValues = &DATA.value.unused2[0];
            return NULL;
        case 14: //waterOp
            return &DATA.value.waterOp;
        case 15: //posRange
            return &DATA.value.posRange;
        case 16: //heightRange
            return &DATA.value.heightRange;
        case 17: //colorRange
            return &DATA.value.colorRange;
        case 18: //wavePeriod
            return &DATA.value.wavePeriod;
        case 19: //flags
            return &DATA.value.flags;
        case 20: //unused3
            *FieldValues = &DATA.value.unused3[0];
            return NULL;
        default:
            return NULL;
        }
    }

bool GRASRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 8: //density
            DATA.value.density = *(UINT8 *)FieldValue;
            break;
        case 9: //minSlope
            DATA.value.minSlope = *(UINT8 *)FieldValue;
            break;
        case 10: //maxSlope
            DATA.value.maxSlope = *(UINT8 *)FieldValue;
            break;
        case 11: //unused1
            if(ArraySize != 1)
                break;
            DATA.value.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 12: //waterDistance
            DATA.value.waterDistance = *(UINT16 *)FieldValue;
            break;
        case 13: //unused2
            if(ArraySize != 2)
                break;
            DATA.value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
            DATA.value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 14: //waterOp
            DATA.value.waterOp = *(UINT32 *)FieldValue;
            break;
        case 15: //posRange
            DATA.value.posRange = *(FLOAT32 *)FieldValue;
            break;
        case 16: //heightRange
            DATA.value.heightRange = *(FLOAT32 *)FieldValue;
            break;
        case 17: //colorRange
            DATA.value.colorRange = *(FLOAT32 *)FieldValue;
            break;
        case 18: //wavePeriod
            DATA.value.wavePeriod = *(FLOAT32 *)FieldValue;
            break;
        case 19: //flags
            SetFlagMask(*(UINT8 *)FieldValue);
            break;
        case 20: //unused3
            if(ArraySize != 3)
                break;
            DATA.value.unused3[0] = ((UINT8ARRAY)FieldValue)[0];
            DATA.value.unused3[1] = ((UINT8ARRAY)FieldValue)[1];
            DATA.value.unused3[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        default:
            break;
        }
    return false;
    }

void GRASRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GRASDATA defaultDATA;

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
        case 8: //density
            DATA.value.density = defaultDATA.density;
            return;
        case 9: //minSlope
            DATA.value.minSlope = defaultDATA.minSlope;
            return;
        case 10: //maxSlope
            DATA.value.maxSlope = defaultDATA.maxSlope;
            return;
        case 11: //unused1
            DATA.value.unused1 = defaultDATA.unused1;
            return;
        case 12: //waterDistance
            DATA.value.waterDistance = defaultDATA.waterDistance;
            return;
        case 13: //unused2
            DATA.value.unused2[0] = defaultDATA.unused2[0];
            DATA.value.unused2[1] = defaultDATA.unused2[1];
            return;
        case 14: //waterOp
            DATA.value.waterOp = defaultDATA.waterOp;
            return;
        case 15: //posRange
            DATA.value.posRange = defaultDATA.posRange;
            return;
        case 16: //heightRange
            DATA.value.heightRange = defaultDATA.heightRange;
            return;
        case 17: //colorRange
            DATA.value.colorRange = defaultDATA.colorRange;
            return;
        case 18: //wavePeriod
            DATA.value.wavePeriod = defaultDATA.wavePeriod;
            return;
        case 19: //flags
            DATA.value.flags = defaultDATA.flags;
            return;
        case 20: //unused3
            DATA.value.unused3[0] = defaultDATA.unused3[0];
            DATA.value.unused3[1] = defaultDATA.unused3[1];
            DATA.value.unused3[2] = defaultDATA.unused3[2];
            return;
        default:
            return;
        }
    }