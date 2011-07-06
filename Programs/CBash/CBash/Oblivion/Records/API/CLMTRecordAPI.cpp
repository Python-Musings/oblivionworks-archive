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
#include "..\CLMTRecord.h"

UINT32 CLMTRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //weathers
            if(ListFieldID == 0) //weathers
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)Weathers.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= Weathers.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //weather
                    return FORMID_FIELD;
                case 2: //chance
                    return SINT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 6: //sunPath
            return ISTRING_FIELD;
        case 7: //glarePath
            return ISTRING_FIELD;
        case 8: //modPath
            return ISTRING_FIELD;
        case 9: //modb
            return FLOAT32_FIELD;
        case 10: //modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MODL.IsLoaded() ? MODL->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 11: //riseBegin
            return UINT8_FIELD;
        case 12: //riseEnd
            return UINT8_FIELD;
        case 13: //setBegin
            return UINT8_FIELD;
        case 14: //setEnd
            return UINT8_FIELD;
        case 15: //volatility
            return UINT8_FIELD;
        case 16: //phaseLength
            return UINT8_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * CLMTRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //weathers
            if(ListIndex >= Weathers.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //weather
                    return &Weathers[ListIndex].weather;
                case 2: //chance
                    return &Weathers[ListIndex].chance;
                default:
                    return NULL;
                }
        case 6: //sunPath
            return FNAM.value;
        case 7: //glarePath
            return GNAM.value;
        case 8: //modPath
            return MODL.IsLoaded() ? MODL->MODL.value : NULL;
        case 9: //modb
            return MODL.IsLoaded() ? &MODL->MODB.value.MODB : NULL;
        case 10: //modt_p
            *FieldValues = MODL.IsLoaded() ? MODL->MODT.value : NULL;
            return NULL;
        case 11: //riseBegin
            return &TNAM.value.riseBegin;
        case 12: //riseEnd
            return &TNAM.value.riseEnd;
        case 13: //setBegin
            return &TNAM.value.setBegin;
        case 14: //setEnd
            return &TNAM.value.setEnd;
        case 15: //volatility
            return &TNAM.value.volatility;
        case 16: //phaseLength
            return &TNAM.value.phaseLength;
        default:
            return NULL;
        }
    }

bool CLMTRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //weathers
            if(ListFieldID == 0) //weathersSize
                {
                Weathers.resize(ArraySize);
                return false;
                }

            if(ListIndex >= Weathers.size())
                break;

            switch(ListFieldID)
                {
                case 1: //weather
                    Weathers[ListIndex].weather = *(FORMID *)FieldValue;
                    return true;
                case 2: //chance
                    Weathers[ListIndex].chance = *(SINT32 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 6: //sunPath
            FNAM.Copy((STRING)FieldValue);
            break;
        case 7: //glarePath
            GNAM.Copy((STRING)FieldValue);
            break;
        case 8: //modPath
            MODL.Load();
            MODL->MODL.Copy((STRING)FieldValue);
            break;
        case 9: //modb
            MODL.Load();
            MODL->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 10: //modt_p
            MODL.Load();
            MODL->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 11: //riseBegin
            TNAM.value.riseBegin = *(UINT8 *)FieldValue;
            break;
        case 12: //riseEnd
            TNAM.value.riseEnd = *(UINT8 *)FieldValue;
            break;
        case 13: //setBegin
            TNAM.value.setBegin = *(UINT8 *)FieldValue;
            break;
        case 14: //setEnd
            TNAM.value.setEnd = *(UINT8 *)FieldValue;
            break;
        case 15: //volatility
            TNAM.value.volatility = *(UINT8 *)FieldValue;
            break;
        case 16: //phaseLength
            TNAM.value.phaseLength = *(UINT8 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void CLMTRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    CLMTTNAM defaultTNAM;
    CLMTWLST defaultWLST;

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
        case 5: //weathers
            if(ListFieldID == 0) //weathers
                {
                Weathers.clear();
                return;
                }

            if(ListIndex >= Weathers.size())
                return;

            switch(ListFieldID)
                {
                case 1: //weather
                    Weathers[ListIndex].weather = defaultWLST.weather;
                    return;
                case 2: //chance
                    Weathers[ListIndex].chance = defaultWLST.chance;
                    return;
                default:
                    return;
                }
        case 6: //sunPath
            FNAM.Unload();
            return;
        case 7: //glarePath
            GNAM.Unload();
            return;
        case 8: //modPath
            if(MODL.IsLoaded())
                MODL->MODL.Unload();
            return;
        case 9: //modb
            if(MODL.IsLoaded())
                MODL->MODB.Unload();
            return;
        case 10: //modt_p
            if(MODL.IsLoaded())
                MODL->MODT.Unload();
            return;
        case 11: //riseBegin
            TNAM.value.riseBegin = defaultTNAM.riseBegin;
            return;
        case 12: //riseEnd
            TNAM.value.riseEnd = defaultTNAM.riseEnd;
            return;
        case 13: //setBegin
            TNAM.value.setBegin = defaultTNAM.setBegin;
            return;
        case 14: //setEnd
            TNAM.value.setEnd = defaultTNAM.setEnd;
            return;
        case 15: //volatility
            TNAM.value.volatility = defaultTNAM.volatility;
            return;
        case 16: //phaseLength
            TNAM.value.phaseLength = defaultTNAM.phaseLength;
            return;
        default:
            return;
        }
    }