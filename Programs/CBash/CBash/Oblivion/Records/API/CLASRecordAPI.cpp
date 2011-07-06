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
#include "..\CLASRecord.h"

UINT32 CLASRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //description
            return STRING_FIELD;
        case 7: //iconPath
            return ISTRING_FIELD;
        case 8: //primary1
            return SINT32_FIELD;
        case 9: //primary2
            return SINT32_FIELD;
        case 10: //specialization
            return UINT32_FIELD;
        case 11: //major1
            return SINT32_FIELD;
        case 12: //major2
            return SINT32_FIELD;
        case 13: //major3
            return SINT32_FIELD;
        case 14: //major4
            return SINT32_FIELD;
        case 15: //major5
            return SINT32_FIELD;
        case 16: //major6
            return SINT32_FIELD;
        case 17: //major7
            return SINT32_FIELD;
        case 18: //flags
            return UINT32_FLAG_FIELD;
        case 19: //services
            return UINT32_FLAG_FIELD;
        case 20: //trainSkill
            return SINT8_FIELD;
        case 21: //trainLevel
            return UINT8_FIELD;
        case 22: //unused1
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

void * CLASRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //description
            return DESC.value;
        case 7: //iconPath
            return ICON.value;
        case 8: //primary1
            return &DATA.value.primary[0];
        case 9: //primary2
            return &DATA.value.primary[1];
        case 10: //specialization
            return &DATA.value.specialization;
        case 11: //major1
            return &DATA.value.major[0];
        case 12: //major2
            return &DATA.value.major[1];
        case 13: //major3
            return &DATA.value.major[2];
        case 14: //major4
            return &DATA.value.major[3];
        case 15: //major5
            return &DATA.value.major[4];
        case 16: //major6
            return &DATA.value.major[5];
        case 17: //major7
            return &DATA.value.major[6];
        case 18: //flags
            return &DATA.value.flags;
        case 19: //services
            return &DATA.value.services;
        case 20: //trainSkill
            return &DATA.value.trainSkill;
        case 21: //trainLevel
            return &DATA.value.trainLevel;
        case 22: //unused1
            *FieldValues = &DATA.value.unused1[0];
            return NULL;
        default:
            return NULL;
        }
    }

bool CLASRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //description
            DESC.Copy((STRING)FieldValue);
            break;
        case 7: //iconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 8: //primary1
            DATA.value.primary[0] = *(SINT32 *)FieldValue;
            break;
        case 9: //primary2
            DATA.value.primary[1] = *(SINT32 *)FieldValue;
            break;
        case 10: //specialization
            DATA.value.specialization = *(UINT32 *)FieldValue;
            break;
        case 11: //major1
            DATA.value.major[0] = *(SINT32 *)FieldValue;
            break;
        case 12: //major2
            DATA.value.major[1] = *(SINT32 *)FieldValue;
            break;
        case 13: //major3
            DATA.value.major[2] = *(SINT32 *)FieldValue;
            break;
        case 14: //major4
            DATA.value.major[3] = *(SINT32 *)FieldValue;
            break;
        case 15: //major5
            DATA.value.major[4] = *(SINT32 *)FieldValue;
            break;
        case 16: //major6
            DATA.value.major[5] = *(SINT32 *)FieldValue;
            break;
        case 17: //major7
            DATA.value.major[6] = *(SINT32 *)FieldValue;
            break;
        case 18: //flags
            SetFlagMask(*(UINT32 *)FieldValue);
            break;
        case 19: //services
            SetServicesFlagMask(*(UINT32 *)FieldValue);
            break;
        case 20: //trainSkill
            DATA.value.trainSkill = *(SINT8 *)FieldValue;
            break;
        case 21: //trainLevel
            DATA.value.trainLevel = *(UINT8 *)FieldValue;
            break;
        case 22: //unused1
            if(ArraySize != 2)
                break;
            DATA.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            DATA.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        default:
            break;
        }
    return false;
    }

void CLASRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    CLASDATA defaultDATA;

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
        case 6: //description
            DESC.Unload();
            return;
        case 7: //iconPath
            ICON.Unload();
            return;
        case 8: //primary1
            DATA.value.primary[0] = defaultDATA.primary[0];
            return;
        case 9: //primary2
            DATA.value.primary[1] = defaultDATA.primary[1];
            return;
        case 10: //specialization
            DATA.value.specialization = defaultDATA.specialization;
            return;
        case 11: //major1
            DATA.value.major[0] = defaultDATA.major[0];
            return;
        case 12: //major2
            DATA.value.major[1] = defaultDATA.major[1];
            return;
        case 13: //major3
            DATA.value.major[2] = defaultDATA.major[2];
            return;
        case 14: //major4
            DATA.value.major[3] = defaultDATA.major[3];
            return;
        case 15: //major5
            DATA.value.major[4] = defaultDATA.major[4];
            return;
        case 16: //major6
            DATA.value.major[5] = defaultDATA.major[5];
            return;
        case 17: //major7
            DATA.value.major[6] = defaultDATA.major[6];
            return;
        case 18: //flags
            DATA.value.flags = defaultDATA.flags;
            return;
        case 19: //services
            DATA.value.services = defaultDATA.services;
            return;
        case 20: //trainSkill
            DATA.value.trainSkill = defaultDATA.trainSkill;
            return;
        case 21: //trainLevel
            DATA.value.trainLevel = defaultDATA.trainLevel;
            return;
        case 22: //unused1
            DATA.value.unused1[0] = defaultDATA.unused1[0];
            DATA.value.unused1[1] = defaultDATA.unused1[1];
            return;
        default:
            return;
        }
    }