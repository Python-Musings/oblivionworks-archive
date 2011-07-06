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
#include "..\SOUNRecord.h"

UINT32 SOUNRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //soundPath
            return ISTRING_FIELD;
        case 6: //minDistance
            return UINT8_FIELD;
        case 7: //maxDistance
            return UINT8_FIELD;
        case 8: //freqAdjustment
            return SINT8_FIELD;
        case 9: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 1;
                default:
                    return UNKNOWN_FIELD;
                }
        case 10: //flags
            return UINT16_FLAG_FIELD;
        case 11: //unused2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 12: //staticAtten
            return SINT16_FIELD;
        case 13: //stopTime
            return UINT8_FIELD;
        case 14: //startTime
            return UINT8_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * SOUNRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //soundPath
            return FNAM.value;
        case 6: //minDistance
            return &SNDX.value.minDistance;
        case 7: //maxDistance
            return &SNDX.value.maxDistance;
        case 8: //freqAdjustment
            return &SNDX.value.freqAdjustment;
        case 9: //unused1
            *FieldValues = &SNDX.value.unused1;
            return NULL;
        case 10: //flags
            return &SNDX.value.flags;
        case 11: //unused2
            *FieldValues = &SNDX.value.unused2[0];
            return NULL;
        case 12: //staticAtten
            return &SNDX.value.staticAtten;
        case 13: //stopTime
            return &SNDX.value.stopTime;
        case 14: //startTime
            return &SNDX.value.startTime;
        default:
            return NULL;
        }
    }

bool SOUNRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //soundPath
            FNAM.Copy((STRING)FieldValue);
            break;
        case 6: //minDistance
            SNDX.value.minDistance = *(UINT8 *)FieldValue;
            break;
        case 7: //maxDistance
            SNDX.value.maxDistance = *(UINT8 *)FieldValue;
            break;
        case 8: //freqAdjustment
            SNDX.value.freqAdjustment = *(SINT8 *)FieldValue;
            break;
        case 9: //unused1
            if(ArraySize != 1)
                break;
            SNDX.value.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 10: //flags
            SetFlagMask(*(UINT16 *)FieldValue);
            break;
        case 11: //unused2
            if(ArraySize != 2)
                break;
            SNDX.value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
            SNDX.value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 12: //staticAtten
            SNDX.value.staticAtten = *(SINT16 *)FieldValue;
            break;
        case 13: //stopTime
            SNDX.value.stopTime = *(UINT8 *)FieldValue;
            break;
        case 14: //startTime
            SNDX.value.startTime = *(UINT8 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void SOUNRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    SOUNSNDX defaultSNDX;

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
        case 5: //soundPath
            FNAM.Unload();
            return;
        case 6: //minDistance
            SNDX.value.minDistance = defaultSNDX.minDistance;
            return;
        case 7: //maxDistance
            SNDX.value.maxDistance = defaultSNDX.maxDistance;
            return;
        case 8: //freqAdjustment
            SNDX.value.freqAdjustment = defaultSNDX.freqAdjustment;
            return;
        case 9: //unused1
            SNDX.value.unused1 = defaultSNDX.unused1;
            return;
        case 10: //flags
            SNDX.value.flags = defaultSNDX.flags;
            return;
        case 11: //unused2
            SNDX.value.unused2[0] = defaultSNDX.unused2[0];
            SNDX.value.unused2[1] = defaultSNDX.unused2[1];
            return;
        case 12: //staticAtten
            SNDX.value.staticAtten = defaultSNDX.staticAtten;
            return;
        case 13: //stopTime
            SNDX.value.stopTime = defaultSNDX.stopTime;
            return;
        case 14: //startTime
            SNDX.value.startTime = defaultSNDX.startTime;
            return;
        default:
            return;
        }
    }