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
#include "..\BSGNRecord.h"
UINT32 BSGNRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //iconPath
            return ISTRING_FIELD;
        case 7: //text
            return STRING_FIELD;
        case 8: //spells
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)SPLO.size();
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * BSGNRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //iconPath
            return ICON.value;
        case 7: //text
            return DESC.value;
        case 8: //spells
            *FieldValues = SPLO.size() ? &SPLO[0] : NULL;
            return NULL;
        default:
            return NULL;
        }
    }

bool BSGNRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //iconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 7: //text
            DESC.Copy((STRING)FieldValue);
            break;
        case 8: //spells
            SPLO.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                SPLO[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        default:
            break;
        }
    return false;
    }

void BSGNRecord::DeleteField(FIELD_IDENTIFIERS)
    {
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
        case 6: //iconPath
            ICON.Unload();
            return;
        case 7: //text
            DESC.Unload();
            return;
        case 8: //spells
            SPLO.clear();
            return;
        default:
            return;
        }
    }