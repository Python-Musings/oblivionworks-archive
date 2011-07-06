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
#include "..\TES4Record.h"

UINT32 TES4Record::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
    {
    switch(FieldID)
        {
        case 0: //recType
            return GetType();
        case 1: //flags1
            return UINT32_FLAG_FIELD;
        case 2: //fid
            return MISSING_FIELD;
        case 3: //flags2
            return UINT32_FLAG_FIELD;
        case 4: //eid
            return MISSING_FIELD;
        case 5: //version
            return FLOAT32_FIELD;
        case 6: //numRecords
            return UINT32_FIELD;
        case 7: //nextObject
            return UINT32_FIELD;
        case 8: //ofst_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return OFST.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 9: //dele_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return DELE.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 10: //author
            return STRING_FIELD;
        case 11: //description
            return STRING_FIELD;
        case 12: //masters
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return ISTRING_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)MAST.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 13: //DATA
            return JUNK_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * TES4Record::GetField(FIELD_IDENTIFIERS, void **FieldValues)
    {
    switch(FieldID)
        {
        case 1: //flags1
            return &flags;
        case 3: //flags2
            return &flagsUnk;
        case 5: //version
            return &HEDR.value.version;
        case 6: //numRecords
            return &HEDR.value.numRecords;
        case 7: //nextObject
            return &HEDR.value.nextObject;
        case 8: //ofst_p
            *FieldValues = OFST.value;
            return NULL;
        case 9: //dele_p
            *FieldValues = DELE.value;
            return NULL;
        case 10: //author
            return CNAM.value;
        case 11: //description
            return SNAM.value;
        case 12: //masters
            for(UINT32 p = 0;p < MAST.size();p++)
                FieldValues[p] = MAST[p].value;
            return NULL;
        case 13: //DATA
            return NULL;
        default:
            return NULL;
        }
    }

bool TES4Record::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
    {
    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(*(UINT32 *)FieldValue);
            break;
        case 3: //flags2
            SetHeaderUnknownFlagMask(*(UINT32 *)FieldValue);
            break;
        case 5: //version
            HEDR.value.version = *(FLOAT32 *)FieldValue;
            break;
        case 6: //numRecords
            HEDR.value.numRecords = *(UINT32 *)FieldValue;
            break;
        case 7: //nextObject
            HEDR.value.nextObject = *(UINT32 *)FieldValue;
            break;
        case 8: //ofst_p
            OFST.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 9: //dele_p
            DELE.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 10: //author
            CNAM.Copy((STRING)FieldValue);
            break;
        case 11: //description
            SNAM.Copy((STRING)FieldValue);
            break;
        case 12: //masters
            MAST.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                MAST[x].Copy((STRING)((STRINGARRAY)FieldValue)[x]);
            break;
        default:
            break;
        }
    return false;
    }

void TES4Record::DeleteField(FIELD_IDENTIFIERS)
    {
    TES4HEDR defaultHEDR;

    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(0);
            return;
        case 3: //flags2
            flagsUnk = 0;
            return;
        case 5: //version
            HEDR.value.version = defaultHEDR.version;
            return;
        case 6: //numRecords
            HEDR.value.numRecords = defaultHEDR.numRecords;
            return;
        case 7: //nextObject
            HEDR.value.nextObject = defaultHEDR.nextObject;
            return;
        case 8: //ofst_p
            OFST.Unload();
            return;
        case 9: //dele_p
            DELE.Unload();
            return;
        case 10: //author
            CNAM.Unload();
            return;
        case 11: //description
            SNAM.Unload();
            return;
        case 12: //masters
            //Good chance of breaking the plugin if called. Might be better to disallow.
            //Or atleast try and fix things up on this side.
            MAST.clear();
            return;
        default:
            return;
        }
    }