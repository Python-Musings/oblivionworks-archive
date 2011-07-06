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
#include "..\FACTRecord.h"

UINT32 FACTRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //relations
            if(ListFieldID == 0) //relations
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)XNAM.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= XNAM.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //faction
                    return FORMID_FIELD;
                case 2: //mod
                    return SINT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 7: //flags
            return UINT8_FLAG_FIELD;
        case 8: //crimeGoldMultiplier
            return FLOAT32_FIELD;
        case 9: //ranks
            if(ListFieldID == 0) //ranks
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)RNAM.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= RNAM.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //rank
                    return SINT32_FIELD;
                case 2: //male
                    return STRING_FIELD;
                case 3: //female
                    return STRING_FIELD;
                case 4: //insigniaPath
                    return ISTRING_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * FACTRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //relations
            if(ListIndex >= XNAM.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //faction
                    return &XNAM[ListIndex]->value.faction;
                case 2: //mod
                    return &XNAM[ListIndex]->value.mod;
                default:
                    return NULL;
                }
        case 7: //flags
            return &DATA.value.flags;
        case 8: //crimeGoldMultiplier
            return CNAM.IsLoaded() ? &CNAM->crimeGoldMultiplier : NULL;
        case 9: //ranks
            if(ListIndex >= RNAM.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //rank
                    return &RNAM[ListIndex]->value.RNAM;
                case 2: //male
                    return RNAM[ListIndex]->value.MNAM.value;
                case 3: //female
                    return RNAM[ListIndex]->value.FNAM.value;
                case 4: //insigniaPath
                    return RNAM[ListIndex]->value.INAM.value;
                default:
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool FACTRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //relations
            if(ListFieldID == 0) //relationsSize
                {
                ArraySize -= (UINT32)XNAM.size();
                while((SINT32)ArraySize > 0)
                    {
                    XNAM.push_back(new ReqSubRecord<GENXNAM>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete XNAM.back();
                    XNAM.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= XNAM.size())
                break;

            switch(ListFieldID)
                {
                case 1: //faction
                    XNAM[ListIndex]->value.faction = *(FORMID *)FieldValue;
                    return true;
                case 2: //mod
                    XNAM[ListIndex]->value.mod = *(SINT32 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 7: //flags
            SetFlagMask(*(UINT8 *)FieldValue);
            break;
        case 8: //crimeGoldMultiplier
            CNAM.Load();
            CNAM->crimeGoldMultiplier = *(FLOAT32 *)FieldValue;
            break;
        case 9: //ranks
            if(ListFieldID == 0) //ranksSize
                {
                ArraySize -= (UINT32)RNAM.size();
                while((SINT32)ArraySize > 0)
                    {
                    RNAM.push_back(new ReqSubRecord<FACTRNAM>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete RNAM.back();
                    RNAM.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= RNAM.size())
                break;

            switch(ListFieldID)
                {
                case 1: //rank
                    RNAM[ListIndex]->value.RNAM = *(SINT32 *)FieldValue;
                    break;
                case 2: //male
                    RNAM[ListIndex]->value.MNAM.Copy((STRING)FieldValue);
                    break;
                case 3: //female
                    RNAM[ListIndex]->value.FNAM.Copy((STRING)FieldValue);
                    break;
                case 4: //insigniaPath
                    RNAM[ListIndex]->value.INAM.Copy((STRING)FieldValue);
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

void FACTRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GENXNAM defaultXNAM;
    FACTRNAM defaultRNAM;

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
        case 6: //relations
            if(ListFieldID == 0) //relations
                {
                for(UINT32 x = 0; x < (UINT32)XNAM.size(); x++)
                    delete XNAM[x];
                XNAM.clear();
                return;
                }

            if(ListIndex >= XNAM.size())
                return;

            switch(ListFieldID)
                {
                case 1: //faction
                    XNAM[ListIndex]->value.faction = defaultXNAM.faction;
                    return;
                case 2: //mod
                    XNAM[ListIndex]->value.mod = defaultXNAM.mod;
                    return;
                default:
                    return;
                }
        case 7: //flags
            DATA.Unload();
            return;
        case 8: //crimeGoldMultiplier
            CNAM.Unload();
            return;
        case 9: //ranks
            if(ListFieldID == 0) //ranks
                {
                for(UINT32 x = 0; x < (UINT32)RNAM.size(); x++)
                    delete RNAM[x];
                RNAM.clear();
                return;
                }

            if(ListIndex >= RNAM.size())
                return;

            switch(ListFieldID)
                {
                case 1: //rank
                    RNAM[ListIndex]->value.RNAM = defaultRNAM.RNAM;
                    return;
                case 2: //male
                    RNAM[ListIndex]->value.MNAM.Unload();
                    return;
                case 3: //female
                    RNAM[ListIndex]->value.FNAM.Unload();
                    return;
                case 4: //insigniaPath
                    RNAM[ListIndex]->value.INAM.Unload();
                    return;
                default:
                    return;
                }
        default:
            return;
        }
    }