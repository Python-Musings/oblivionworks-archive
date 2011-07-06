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
#include "..\TREERecord.h"

UINT32 TREERecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 8: //iconPath
            return ISTRING_FIELD;
        case 9: //speedTree
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT32_ARRAY_FIELD;
                case 1: //fieldSize
                    return SNAM.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 10: //curvature
            return FLOAT32_FIELD;
        case 11: //minAngle
            return FLOAT32_FIELD;
        case 12: //maxAngle
            return FLOAT32_FIELD;
        case 13: //branchDim
            return FLOAT32_FIELD;
        case 14: //leafDim
            return FLOAT32_FIELD;
        case 15: //shadowRadius
            return SINT32_FIELD;
        case 16: //rockSpeed
            return FLOAT32_FIELD;
        case 17: //rustleSpeed
            return FLOAT32_FIELD;
        case 18: //widthBill
            return FLOAT32_FIELD;
        case 19: //heightBill
            return FLOAT32_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * TREERecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 8: //iconPath
            return ICON.value;
        case 9: //speedTree
            *FieldValues = &SNAM[0];
            return NULL;
        case 10: //curvature
            return &CNAM.value.curvature;
        case 11: //minAngle
            return &CNAM.value.minAngle;
        case 12: //maxAngle
            return &CNAM.value.maxAngle;
        case 13: //branchDim
            return &CNAM.value.branchDim;
        case 14: //leafDim
            return &CNAM.value.leafDim;
        case 15: //shadowRadius
            return &CNAM.value.shadowRadius;
        case 16: //rockSpeed
            return &CNAM.value.rockSpeed;
        case 17: //rustleSpeed
            return &CNAM.value.rustleSpeed;
        case 18: //widthBill
            return &BNAM.value.widthBill;
        case 19: //heightBill
            return &BNAM.value.heightBill;
        default:
            return NULL;
        }
    }

bool TREERecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 8: //iconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 9: //speedTree
            SNAM.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                SNAM[x] = ((UINT32ARRAY)FieldValue)[x];
            break;
        case 10: //curvature
            CNAM.value.curvature = *(FLOAT32 *)FieldValue;
            break;
        case 11: //minAngle
            CNAM.value.minAngle = *(FLOAT32 *)FieldValue;
            break;
        case 12: //maxAngle
            CNAM.value.maxAngle = *(FLOAT32 *)FieldValue;
            break;
        case 13: //branchDim
            CNAM.value.branchDim = *(FLOAT32 *)FieldValue;
            break;
        case 14: //leafDim
            CNAM.value.leafDim = *(FLOAT32 *)FieldValue;
            break;
        case 15: //shadowRadius
            CNAM.value.shadowRadius = *(SINT32 *)FieldValue;
            break;
        case 16: //rockSpeed
            CNAM.value.rockSpeed = *(FLOAT32 *)FieldValue;
            break;
        case 17: //rustleSpeed
            CNAM.value.rustleSpeed = *(FLOAT32 *)FieldValue;
            break;
        case 18: //widthBill
            BNAM.value.widthBill = *(FLOAT32 *)FieldValue;
            break;
        case 19: //heightBill
            BNAM.value.heightBill = *(FLOAT32 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void TREERecord::DeleteField(FIELD_IDENTIFIERS)
    {
    TREECNAM defaultCNAM;
    TREEBNAM defaultBNAM;

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
        case 8: //iconPath
            ICON.Unload();
            return;
        case 9: //speedTree
            SNAM.clear();
            return;
        case 10: //curvature
            CNAM.value.curvature = defaultCNAM.curvature;
            return;
        case 11: //minAngle
            CNAM.value.minAngle = defaultCNAM.minAngle;
            return;
        case 12: //maxAngle
            CNAM.value.maxAngle = defaultCNAM.maxAngle;
            return;
        case 13: //branchDim
            CNAM.value.branchDim = defaultCNAM.branchDim;
            return;
        case 14: //leafDim
            CNAM.value.leafDim = defaultCNAM.leafDim;
            return;
        case 15: //shadowRadius
            CNAM.value.shadowRadius = defaultCNAM.shadowRadius;
            return;
        case 16: //rockSpeed
            CNAM.value.rockSpeed = defaultCNAM.rockSpeed;
            return;
        case 17: //rustleSpeed
            CNAM.value.rustleSpeed = defaultCNAM.rustleSpeed;
            return;
        case 18: //widthBill
            BNAM.value.widthBill = defaultBNAM.widthBill;
            return;
        case 19: //heightBill
            BNAM.value.heightBill = defaultBNAM.heightBill;
            return;
        default:
            return;
        }
    return;
    }