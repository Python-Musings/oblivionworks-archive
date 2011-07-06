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
#include "..\LSCRRecord.h"

UINT32 LSCRRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //iconPath
            return ISTRING_FIELD;
        case 6: //text
            return STRING_FIELD;
        case 7: //locations
            if(ListFieldID == 0) //locations
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)LNAM.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= LNAM.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //direct
                    return FORMID_FIELD;
                case 2: //indirect
                    return FORMID_FIELD;
                case 3: //gridY
                    return SINT16_FIELD;
                case 4: //gridX
                    return SINT16_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * LSCRRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //iconPath
            return ICON.value;
        case 6: //text
            return DESC.value;
        case 7: //locations
            if(ListIndex >= LNAM.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //direct
                    return &LNAM[ListIndex]->value.direct;
                case 2: //indirect
                    return &LNAM[ListIndex]->value.indirect;
                case 3: //gridY
                    return &LNAM[ListIndex]->value.gridY;
                case 4: //gridX
                    return &LNAM[ListIndex]->value.gridX;
                default:
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool LSCRRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //iconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 6: //text
            DESC.Copy((STRING)FieldValue);
            break;
        case 7: //locations
            if(ListFieldID == 0) //locationsSize
                {
                ArraySize -= (UINT32)LNAM.size();
                while((SINT32)ArraySize > 0)
                    {
                    LNAM.push_back(new ReqSubRecord<LSCRLNAM>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete LNAM.back();
                    LNAM.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= LNAM.size())
                break;

            switch(ListFieldID)
                {
                case 1: //direct
                    LNAM[ListIndex]->value.direct = *(FORMID *)FieldValue;
                    return true;
                case 2: //indirect
                    LNAM[ListIndex]->value.indirect = *(FORMID *)FieldValue;
                    return true;
                case 3: //gridY
                    LNAM[ListIndex]->value.gridY = *(SINT16 *)FieldValue;
                    break;
                case 4: //gridX
                    LNAM[ListIndex]->value.gridX = *(SINT16 *)FieldValue;
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

void LSCRRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    LSCRLNAM defaultLNAM;

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
        case 5: //iconPath
            ICON.Unload();
            return;
        case 6: //text
            DESC.Unload();
            return;
        case 7: //locations
            if(ListFieldID == 0) //locationsSize
                {
                for(UINT32 x = 0; x < (UINT32)LNAM.size(); x++)
                    delete LNAM[x];
                LNAM.clear();
                return;
                }

            if(ListIndex >= LNAM.size())
                return;

            switch(ListFieldID)
                {
                case 1: //direct
                    LNAM[ListIndex]->value.direct = defaultLNAM.direct;
                    return;
                case 2: //indirect
                    LNAM[ListIndex]->value.indirect = defaultLNAM.indirect;
                    return;
                case 3: //gridY
                    LNAM[ListIndex]->value.gridY = defaultLNAM.gridY;
                    return;
                case 4: //gridX
                    LNAM[ListIndex]->value.gridX = defaultLNAM.gridX;
                    return;
                default:
                    return;
                }
            return;
        default:
            return;
        }
    }