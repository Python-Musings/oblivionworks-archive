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
#include "..\CONTRecord.h"

UINT32 CONTRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 10: //items
            if(ListFieldID == 0) //items
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)CNTO.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= CNTO.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //item
                    return FORMID_FIELD;
                case 2: //count
                    return SINT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 11: //flags
            return UINT8_FLAG_FIELD;
        case 12: //weight
            return FLOAT32_FIELD;
        case 13: //soundOpen
            return FORMID_FIELD;
        case 14: //soundClose
            return FORMID_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * CONTRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 10: //items
            if(ListIndex >= CNTO.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //item
                    return &CNTO[ListIndex]->value.item;
                case 2: //count
                    return &CNTO[ListIndex]->value.count;
                default:
                    return NULL;
                }
        case 11: //flags
            return &DATA.value.flags;
        case 12: //weight
            return &DATA.value.weight;
        case 13: //soundOpen
            return SNAM.IsLoaded() ? &SNAM->fid : NULL;
        case 14: //soundClose
            return QNAM.IsLoaded() ? &QNAM->fid : NULL;
        default:
            return NULL;
        }
    }

bool CONTRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 10: //items
            if(ListFieldID == 0) //itemsSize
                {
                ArraySize -= (UINT32)CNTO.size();
                while((SINT32)ArraySize > 0)
                    {
                    CNTO.push_back(new ReqSubRecord<GENCNTO>());
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete CNTO.back();
                    CNTO.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= CNTO.size())
                break;

            switch(ListFieldID)
                {
                case 1: //item
                    CNTO[ListIndex]->value.item = *(FORMID *)FieldValue;
                    return true;
                case 2: //count
                    CNTO[ListIndex]->value.count = *(SINT32 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 11: //flags
            SetFlagMask(*(UINT8 *)FieldValue);
            break;
        case 12: //weight
            DATA.value.weight = *(FLOAT32 *)FieldValue;
            break;
        case 13: //soundOpen
            SNAM.Load();
            SNAM->fid = *(FORMID *)FieldValue;
            return true;
        case 14: //soundClose
            QNAM.Load();
            QNAM->fid = *(FORMID *)FieldValue;
            return true;
        default:
            break;
        }
    return false;
    }

void CONTRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    CONTDATA defaultDATA;
    GENCNTO defaultCNTO;

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
        case 10: //items
            if(ListFieldID == 0) //items
                {
                for(UINT32 x = 0; x < (UINT32)CNTO.size(); ++x)
                    delete CNTO[x];
                CNTO.clear();
                return;
                }

            if(ListIndex >= CNTO.size())
                return;

            switch(ListFieldID)
                {
                case 1: //item
                    CNTO[ListIndex]->value.item = defaultCNTO.item;
                    return;
                case 2: //count
                    CNTO[ListIndex]->value.count = defaultCNTO.count;
                    return;
                default:
                    return;
                }
        case 11: //flags
            DATA.value.flags = defaultDATA.flags;
            return;
        case 12: //weight
            DATA.value.weight = defaultDATA.weight;
            return;
        case 13: //soundOpen
            SNAM.Unload();
            return;
        case 14: //soundClose
            QNAM.Unload();
            return;
        default:
            return;
        }
    }