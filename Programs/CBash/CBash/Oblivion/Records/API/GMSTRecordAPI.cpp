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
#include "..\GMSTRecord.h"

UINT32 GMSTRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //value
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return STRING_OR_FLOAT32_OR_SINT32_FIELD;
                case 2: //WhichType
                    switch(DATA.format)
                        {
                        case 's':
                            return STRING_FIELD;
                        case 'i':
                            return SINT32_FIELD;
                        case 'f':
                            return FLOAT32_FIELD;
                        default:
                            return UNKNOWN_FIELD;
                        }
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * GMSTRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //value
            switch(DATA.format)
                {
                case 's':
                    return DATA.s;
                case 'i':
                    return &DATA.i;
                case 'f':
                    return &DATA.f;
                default:
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool GMSTRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
    {
    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(*(UINT32 *)FieldValue);
            break;
        case 2: //fid
            formID = *(FORMID *)FieldValue;
            break;
        case 3: //flags2
            SetHeaderUnknownFlagMask(*(UINT32 *)FieldValue);
            break;
        case 4: //eid
            EDID.Copy((STRING)FieldValue);
            break;
        case 5: //value
            switch(DATA.format)
                {
                case 's':
                    delete []DATA.s;
                    ArraySize = (UINT32)strlen((STRING)FieldValue) + 1;
                    DATA.s = new char[ArraySize];
                    strcpy_s(DATA.s, ArraySize, (STRING)FieldValue);
                    break;
                case 'i':
                    DATA.i = *(SINT32 *)FieldValue;
                    break;
                case 'f':
                    DATA.f = *(FLOAT32 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        default:
            break;
        }
    return false;
    }

void GMSTRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GMSTDATA defaultDATA;
    UINT32 ArraySize = 0;

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
        case 5: //value
            switch(DATA.format)
                {
                case 's':
                    delete []DATA.s;
                    if(defaultDATA.s != NULL)
                        {
                        ArraySize = (UINT32)strlen(defaultDATA.s) + 1;
                        DATA.s = new char[ArraySize];
                        strcpy_s(DATA.s, ArraySize, defaultDATA.s);
                        }
                    else
                        DATA.s = defaultDATA.s;
                    return;
                case 'i':
                    DATA.i = defaultDATA.i;
                    return;
                case 'f':
                    DATA.f = defaultDATA.f;
                    return;
                default:
                    return;
                }
        default:
            return;
        }
    }