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
#include "..\SKILRecord.h"

UINT32 SKILRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //skill
            return SINT32_FIELD;
        case 6: //description
            return STRING_FIELD;
        case 7: //iconPath
            return ISTRING_FIELD;
        case 8: //action
            return SINT32_FIELD;
        case 9: //attribute
            return SINT32_FIELD;
        case 10: //specialization
            return UINT32_FIELD;
        case 11: //use0
            return FLOAT32_FIELD;
        case 12: //use1
            return FLOAT32_FIELD;
        case 13: //apprentice
            return STRING_FIELD;
        case 14: //journeyman
            return STRING_FIELD;
        case 15: //expert
            return STRING_FIELD;
        case 16: //master
            return STRING_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * SKILRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //skill
            return &INDX.value.skill;
        case 6: //description
            return DESC.value;
        case 7: //iconPath
            return ICON.value;
        case 8: //action
            return &DATA.value.action;
        case 9: //attribute
            return &DATA.value.attribute;
        case 10: //specialization
            return &DATA.value.specialization;
        case 11: //use0
            return &DATA.value.use0;
        case 12: //use1
            return &DATA.value.use1;
        case 13: //apprentice
            return ANAM.value;
        case 14: //journeyman
            return JNAM.value;
        case 15: //expert
            return ENAM.value;
        case 16: //master
            return MNAM.value;
        default:
            return NULL;
        }
    }

bool SKILRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //skill
            INDX.value.skill = *(SINT32 *)FieldValue;
            break;
        case 6: //description
            DESC.Copy((STRING)FieldValue);
            break;
        case 7: //iconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 8: //action
            DATA.value.action = *(SINT32 *)FieldValue;
            break;
        case 9: //attribute
            DATA.value.attribute = *(SINT32 *)FieldValue;
            break;
        case 10: //specialization
            DATA.value.specialization = *(UINT32 *)FieldValue;
            break;
        case 11: //use0
            DATA.value.use0 = *(FLOAT32 *)FieldValue;
            break;
        case 12: //use1
            DATA.value.use1 = *(FLOAT32 *)FieldValue;
            break;
        case 13: //apprentice
            ANAM.Copy((STRING)FieldValue);
            break;
        case 14: //journeyman
            JNAM.Copy((STRING)FieldValue);
            break;
        case 15: //expert
            ENAM.Copy((STRING)FieldValue);
            break;
        case 16: //master
            MNAM.Copy((STRING)FieldValue);
            break;
        default:
            break;
        }
    return false;
    }

void SKILRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    SKILDATA defaultDATA;

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
        case 5: //skill
            INDX.Unload();
            return;
        case 6: //description
            DESC.Unload();
            return;
        case 7: //iconPath
            ICON.Unload();
            return;
        case 8: //action
            ICON.Unload();
            return;
        case 9: //attribute
            DATA.value.attribute = defaultDATA.attribute;
            return;
        case 10: //specialization
            DATA.value.specialization = defaultDATA.specialization;
            return;
        case 11: //use0
            DATA.value.use0 = defaultDATA.use0;
            return;
        case 12: //use1
            DATA.value.use1 = defaultDATA.use1;
            return;
        case 13: //apprentice
            ANAM.Unload();
            return;
        case 14: //journeyman
            JNAM.Unload();
            return;
        case 15: //expert
            ENAM.Unload();
            return;
        case 16: //master
            MNAM.Unload();
            return;
        default:
            return;
        }
    }