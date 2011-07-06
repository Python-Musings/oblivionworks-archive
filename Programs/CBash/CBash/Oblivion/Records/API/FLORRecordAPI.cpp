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
#include "..\FLORRecord.h"

UINT32 FLORRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //modPath
            return ISTRING_FIELD;
        case 7: //modb
            return FLOAT32_FIELD;
        case 8: //modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MODL.IsLoaded() ? MODL->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 9: //script
            return FORMID_FIELD;
        case 10: //ingredient
            return FORMID_FIELD;
        case 11: //spring
            return UINT8_FIELD;
        case 12: //summer
            return UINT8_FIELD;
        case 13: //fall
            return UINT8_FIELD;
        case 14: //winter
            return UINT8_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * FLORRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //modPath
            return MODL.IsLoaded() ? MODL->MODL.value : NULL;
        case 7: //modb
            return MODL.IsLoaded() ? &MODL->MODB.value.MODB : NULL;
        case 8: //modt_p
            *FieldValues = MODL.IsLoaded() ? MODL->MODT.value : NULL;
            return NULL;
        case 9: //script
            return SCRI.IsLoaded() ? &SCRI->fid : NULL;
        case 10: //ingredient
            return &PFIG.value.fid;
        case 11: //spring
            return &PFPC.value.spring;
        case 12: //summer
            return &PFPC.value.summer;
        case 13: //fall
            return &PFPC.value.fall;
        case 14: //winter
            return &PFPC.value.winter;
        default:
            return NULL;
        }
    }

bool FLORRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //modPath
            MODL.Load();
            MODL->MODL.Copy((STRING)FieldValue);
            break;
        case 7: //modb
            MODL.Load();
            MODL->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 8: //modt_p
            MODL.Load();
            MODL->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 9: //script
            SCRI.Load();
            SCRI->fid = *(FORMID *)FieldValue;
            return true;
        case 10: //ingredient
            PFIG.Load();
            PFIG.value.fid = *(FORMID *)FieldValue;
            return true;
        case 11: //spring
            PFPC.value.spring = *(UINT8 *)FieldValue;
            break;
        case 12: //summer
            PFPC.value.summer = *(UINT8 *)FieldValue;
            break;
        case 13: //fall
            PFPC.value.fall = *(UINT8 *)FieldValue;
            break;
        case 14: //winter
            PFPC.value.winter = *(UINT8 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void FLORRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    FLORPFPC defaultPFPC;

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
        case 6: //modPath
            if(MODL.IsLoaded())
                MODL->MODL.Unload();
            return;
        case 7: //modb
            if(MODL.IsLoaded())
                MODL->MODB.Unload();
            return;
        case 8: //modt_p
            if(MODL.IsLoaded())
                MODL->MODT.Unload();
            return;
        case 9: //script
            SCRI.Unload();
            return;
        case 10: //ingredient
            PFIG.Unload();
            return;
        case 11: //spring
            PFPC.value.spring = defaultPFPC.spring;
            return;
        case 12: //summer
            PFPC.value.summer = defaultPFPC.summer;
            return;
        case 13: //fall
            PFPC.value.fall = defaultPFPC.fall;
            return;
        case 14: //winter
            PFPC.value.winter = defaultPFPC.winter;
            return;
        default:
            return;
        }
    }