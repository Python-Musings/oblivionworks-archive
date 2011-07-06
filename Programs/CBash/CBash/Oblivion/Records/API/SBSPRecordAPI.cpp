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
#include "..\SBSPRecord.h"

UINT32 SBSPRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //sizeX
            return FLOAT32_FIELD;
        case 6: //sizeY
            return FLOAT32_FIELD;
        case 7: //sizeZ
            return FLOAT32_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * SBSPRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //sizeX
            return &DNAM.value.sizeX;
        case 6: //sizeY
            return &DNAM.value.sizeY;
        case 7: //sizeZ
            return &DNAM.value.sizeZ;
        default:
            return NULL;
        }
    }

bool SBSPRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //sizeX
            DNAM.value.sizeX = *(FLOAT32 *)FieldValue;
            break;
        case 6: //sizeY
            DNAM.value.sizeY = *(FLOAT32 *)FieldValue;
            break;
        case 7: //sizeZ
            DNAM.value.sizeZ = *(FLOAT32 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void SBSPRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    SBSPDNAM defaultDNAM;

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
        case 5: //sizeX
            DNAM.value.sizeX = defaultDNAM.sizeX;
            return;
        case 6: //sizeY
            DNAM.value.sizeY = defaultDNAM.sizeY;
            return;
        case 7: //sizeZ
            DNAM.value.sizeZ = defaultDNAM.sizeZ;
            return;
        default:
            return;
        }
    }