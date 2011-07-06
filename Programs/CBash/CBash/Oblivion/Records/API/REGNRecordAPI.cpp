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
#include "..\REGNRecord.h"

UINT32 REGNRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //mapRed
            return UINT8_FIELD;
        case 7: //mapGreen
            return UINT8_FIELD;
        case 8: //mapBlue
            return UINT8_FIELD;
        case 9: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 1;
                default:
                    return UNKNOWN_FIELD;
                }
        case 10: //worldspace
            return FORMID_FIELD;
        case 11: //areas
            if(ListFieldID == 0) //areas
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)Areas.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= Areas.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //edgeFalloff
                    return FORMID_FIELD;
                case 2: //points
                    if(ListX2FieldID == 0) //points
                        {
                        switch(WhichAttribute)
                            {
                            case 0: //fieldType
                                return LIST_FIELD;
                            case 1: //fieldSize
                                return (UINT32)Areas[ListIndex]->RPLD.size();
                            default:
                                return UNKNOWN_FIELD;
                            }
                        }

                    if(ListX2Index >= Areas[ListIndex]->RPLD.size())
                        return UNKNOWN_FIELD;

                    switch(ListX2FieldID)
                        {
                        case 1: //posX
                            return FLOAT32_FIELD;
                        case 2: //posY
                            return FLOAT32_FIELD;
                        default:
                            return UNKNOWN_FIELD;
                        }
                default:
                    return UNKNOWN_FIELD;
                }
        case 12: //entries
            if(ListFieldID == 0) //entries
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)Entries.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= Entries.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //entryType
                    return UINT32_TYPE_FIELD;
                case 2: //flags
                    return UINT8_FLAG_FIELD;
                case 3: //priority
                    return UINT8_FIELD;
                case 4: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 2;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 5: //objects
                    if(ListX2FieldID == 0) //points
                        {
                        switch(WhichAttribute)
                            {
                            case 0: //fieldType
                                return LIST_FIELD;
                            case 1: //fieldSize
                                return (UINT32)Entries[ListIndex]->RDOT.size();
                            default:
                                return UNKNOWN_FIELD;
                            }
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDOT.size())
                        return UNKNOWN_FIELD;

                    switch(ListX2FieldID)
                        {
                        case 1: //objectId
                            return FORMID_FIELD;
                        case 2: //parentIndex
                            return UINT16_FIELD;
                        case 3: //unused1
                            switch(WhichAttribute)
                                {
                                case 0: //fieldType
                                    return UINT8_ARRAY_FIELD;
                                case 1: //fieldSize
                                    return 2;
                                default:
                                    return UNKNOWN_FIELD;
                                }
                        case 4: //density
                            return FLOAT32_FIELD;
                        case 5: //clustering
                            return UINT8_FIELD;
                        case 6: //minSlope
                            return UINT8_FIELD;
                        case 7: //maxSlope
                            return UINT8_FIELD;
                        case 8: //flags
                            return UINT8_FLAG_FIELD;
                        case 9: //radiusWRTParent
                            return UINT16_FIELD;
                        case 10: //radius
                            return UINT16_FIELD;
                        case 11: //unk1
                            switch(WhichAttribute)
                                {
                                case 0: //fieldType
                                    return UINT8_ARRAY_FIELD;
                                case 1: //fieldSize
                                    return 4;
                                default:
                                    return UNKNOWN_FIELD;
                                }
                        case 12: //maxHeight
                            return FLOAT32_FIELD;
                        case 13: //sink
                            return FLOAT32_FIELD;
                        case 14: //sinkVar
                            return FLOAT32_FIELD;
                        case 15: //sizeVar
                            return FLOAT32_FIELD;
                        case 16: //angleVarX
                            return UINT16_FIELD;
                        case 17: //angleVarY
                            return UINT16_FIELD;
                        case 18: //angleVarZ
                            return UINT16_FIELD;
                        case 19: //unused2
                            switch(WhichAttribute)
                                {
                                case 0: //fieldType
                                    return UINT8_ARRAY_FIELD;
                                case 1: //fieldSize
                                    return 2;
                                default:
                                    return UNKNOWN_FIELD;
                                }
                        case 20: //unk2
                            switch(WhichAttribute)
                                {
                                case 0: //fieldType
                                    return UINT8_ARRAY_FIELD;
                                case 1: //fieldSize
                                    return 4;
                                default:
                                    return UNKNOWN_FIELD;
                                }
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 6: //mapName
                    return STRING_FIELD;
                case 7: //iconPath
                    return ISTRING_FIELD;
                case 8: //grasses
                    if(ListX2FieldID == 0) //grasses
                        {
                        switch(WhichAttribute)
                            {
                            case 0: //fieldType
                                return LIST_FIELD;
                            case 1: //fieldSize
                                return (UINT32)Entries[ListIndex]->RDGS.size();
                            default:
                                return UNKNOWN_FIELD;
                            }
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDGS.size())
                        return UNKNOWN_FIELD;

                    switch(ListX2FieldID)
                        {
                        case 1: //grass
                            return FORMID_FIELD;
                        case 2: //unk1
                            switch(WhichAttribute)
                                {
                                case 0: //fieldType
                                    return UINT8_ARRAY_FIELD;
                                case 1: //fieldSize
                                    return 4;
                                default:
                                    return UNKNOWN_FIELD;
                                }
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 9: //musicType
                    return UINT32_TYPE_FIELD;
                case 10: //sounds
                    if(ListX2FieldID == 0) //sounds
                        {
                        switch(WhichAttribute)
                            {
                            case 0: //fieldType
                                return LIST_FIELD;
                            case 1: //fieldSize
                                return (UINT32)Entries[ListIndex]->RDSD.size();
                            default:
                                return UNKNOWN_FIELD;
                            }
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDSD.size())
                        return UNKNOWN_FIELD;

                    switch(ListX2FieldID)
                        {
                        case 1: //sound
                            return FORMID_FIELD;
                        case 2: //flags
                            return UINT32_FLAG_FIELD;
                        case 3: //chance
                            return UINT32_FIELD;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 11: //weathers
                    if(ListX2FieldID == 0) //weathers
                        {
                        switch(WhichAttribute)
                            {
                            case 0: //fieldType
                                return LIST_FIELD;
                            case 1: //fieldSize
                                return (UINT32)Entries[ListIndex]->RDWT.size();
                            default:
                                return UNKNOWN_FIELD;
                            }
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDWT.size())
                        return UNKNOWN_FIELD;

                    switch(ListX2FieldID)
                        {
                        case 1: //weather
                            return FORMID_FIELD;
                        case 2: //chance
                            return UINT32_FIELD;
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

void * REGNRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //mapRed
            return &RCLR.value.red;
        case 7: //mapGreen
            return &RCLR.value.green;
        case 8: //mapBlue
            return &RCLR.value.blue;
        case 9: //unused1
            *FieldValues = &RCLR.value.unused1;
            return NULL;
        case 10: //worldspace
            return &WNAM.value.fid;
        case 11: //areas
            if(ListIndex >= Areas.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //edgeFalloff
                    return &Areas[ListIndex]->RPLI.value.edgeFalloff;
                case 2: //points
                    if(ListX2Index >= Areas[ListIndex]->RPLD.size())
                        return NULL;

                    switch(ListX2FieldID)
                        {
                        case 1: //posX
                            return &Areas[ListIndex]->RPLD[ListX2Index].posX;
                        case 2: //posY
                            return &Areas[ListIndex]->RPLD[ListX2Index].posY;
                        default:
                            return NULL;
                        }
                default:
                    return NULL;
                }
        case 12: //entries
            if(ListIndex >= Entries.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //entryType
                    return &Entries[ListIndex]->RDAT.value.entryType;
                case 2: //flags
                    return &Entries[ListIndex]->RDAT.value.flags;
                case 3: //priority
                    return &Entries[ListIndex]->RDAT.value.priority;
                case 4: //unused1
                    *FieldValues = &Entries[ListIndex]->RDAT.value.unused1[0];
                    return NULL;
                case 5: //objects
                    if(ListX2Index >= Entries[ListIndex]->RDOT.size())
                        return NULL;

                    switch(ListX2FieldID)
                        {
                        case 1: //objectId
                            return &Entries[ListIndex]->RDOT[ListX2Index].objectId;
                        case 2: //parentIndex
                            return &Entries[ListIndex]->RDOT[ListX2Index].parentIndex;
                        case 3: //unused1
                            *FieldValues = &Entries[ListIndex]->RDOT[ListX2Index].unused1[0];
                            return NULL;
                        case 4: //density
                            return &Entries[ListIndex]->RDOT[ListX2Index].density;
                        case 5: //clustering
                            return &Entries[ListIndex]->RDOT[ListX2Index].clustering;
                        case 6: //minSlope
                            return &Entries[ListIndex]->RDOT[ListX2Index].minSlope;
                        case 7: //maxSlope
                            return &Entries[ListIndex]->RDOT[ListX2Index].maxSlope;
                        case 8: //flags
                            return &Entries[ListIndex]->RDOT[ListX2Index].flags;
                        case 9: //radiusWRTParent
                            return &Entries[ListIndex]->RDOT[ListX2Index].radiusWRTParent;
                        case 10: //radius
                            return &Entries[ListIndex]->RDOT[ListX2Index].radius;
                        case 11: //unk1
                            *FieldValues = &Entries[ListIndex]->RDOT[ListX2Index].unk1[0];
                            return NULL;
                        case 12: //maxHeight
                            return &Entries[ListIndex]->RDOT[ListX2Index].maxHeight;
                        case 13: //sink
                            return &Entries[ListIndex]->RDOT[ListX2Index].sink;
                        case 14: //sinkVar
                            return &Entries[ListIndex]->RDOT[ListX2Index].sinkVar;
                        case 15: //sizeVar
                            return &Entries[ListIndex]->RDOT[ListX2Index].sizeVar;
                        case 16: //angleVarX
                            return &Entries[ListIndex]->RDOT[ListX2Index].angleVarX;
                        case 17: //angleVarY
                            return &Entries[ListIndex]->RDOT[ListX2Index].angleVarY;
                        case 18: //angleVarZ
                            return &Entries[ListIndex]->RDOT[ListX2Index].angleVarZ;
                        case 19: //unused2
                            *FieldValues = &Entries[ListIndex]->RDOT[ListX2Index].unused2[0];
                            return NULL;
                        case 20: //unk2
                            *FieldValues = &Entries[ListIndex]->RDOT[ListX2Index].unk2[0];
                            return NULL;
                        default:
                            return NULL;
                        }
                case 6: //mapName
                    return Entries[ListIndex]->RDMP.value;
                case 7: //iconPath
                    return Entries[ListIndex]->ICON.value;
                case 8: //grasses
                    if(ListX2Index >= Entries[ListIndex]->RDGS.size())
                        return NULL;

                    switch(ListX2FieldID)
                        {
                        case 1: //grass
                            return &Entries[ListIndex]->RDGS[ListX2Index].grass;
                        case 2: //unk1
                            *FieldValues = &Entries[ListIndex]->RDGS[ListX2Index].unk1[0];
                            return NULL;
                        default:
                            return NULL;
                        }
                case 9: //musicType
                    return Entries[ListIndex]->RDMD.IsLoaded() ? &Entries[ListIndex]->RDMD->type : NULL;
                case 10: //sounds
                    if(ListX2Index >= Entries[ListIndex]->RDSD.size())
                        return NULL;

                    switch(ListX2FieldID)
                        {
                        case 1: //sound
                            return &Entries[ListIndex]->RDSD[ListX2Index].sound;
                        case 2: //flags
                            return &Entries[ListIndex]->RDSD[ListX2Index].flags;
                        case 3: //chance
                            return &Entries[ListIndex]->RDSD[ListX2Index].chance;
                        default:
                            return NULL;
                        }
                case 11: //weathers
                    if(ListX2Index >= Entries[ListIndex]->RDWT.size())
                        return NULL;

                    switch(ListX2FieldID)
                        {
                        case 1: //weather
                            return &Entries[ListIndex]->RDWT[ListX2Index].weather;
                        case 2: //chance
                            return &Entries[ListIndex]->RDWT[ListX2Index].chance;
                        default:
                            return NULL;
                        }
                default:
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool REGNRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //mapRed
            RCLR.value.red = *(UINT8 *)FieldValue;
        case 7: //mapGreen
            RCLR.value.green = *(UINT8 *)FieldValue;
            break;
        case 8: //mapBlue
            RCLR.value.blue = *(UINT8 *)FieldValue;
        case 9: //unused1
            if(ArraySize != 1)
                break;
            RCLR.value.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 10: //worldspace
            WNAM.value.fid = *(FORMID *)FieldValue;
            return true;
        case 11: //areas
            if(ListFieldID == 0) //areasSize
                {
                ArraySize -= (UINT32)Areas.size();
                while((SINT32)ArraySize > 0)
                    {
                    Areas.push_back(new REGNArea);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete Areas.back();
                    Areas.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= Areas.size())
                break;

            switch(ListFieldID)
                {
                case 1: //edgeFalloff
                    Areas[ListIndex]->RPLI.value.edgeFalloff = *(UINT32 *)FieldValue;
                    break;
                case 2: //points
                    if(ListX2FieldID == 0) //pointsSize
                        {
                        Areas[ListIndex]->RPLD.resize(ArraySize);
                        return false;
                        }

                    if(ListX2Index >= Areas[ListIndex]->RPLD.size())
                        break;

                    switch(ListX2FieldID)
                        {
                        case 1: //posX
                            Areas[ListIndex]->RPLD[ListX2Index].posX = *(FLOAT32 *)FieldValue;
                            break;
                        case 2: //posY
                            Areas[ListIndex]->RPLD[ListX2Index].posY = *(FLOAT32 *)FieldValue;
                            break;
                        default:
                            break;
                        }
                    break;
                default:
                    break;
                }
            break;
        case 12: //entries
            if(ListFieldID == 0) //areasSize
                {
                ArraySize -= (UINT32)Entries.size();
                while((SINT32)ArraySize > 0)
                    {
                    Entries.push_back(new REGNEntry);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete Entries.back();
                    Entries.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= Entries.size())
                break;

            switch(ListFieldID)
                {
                case 1: //entryType
                    Entries[ListIndex]->SetType(*(UINT32 *)FieldValue);
                    return true;
                case 2: //flags
                    Entries[ListIndex]->SetFlagMask(*(UINT8 *)FieldValue);
                    break;
                case 3: //priority
                    Entries[ListIndex]->RDAT.value.priority = *(UINT8 *)FieldValue;
                    break;
                case 4: //unused1
                    if(ArraySize != 2)
                        break;
                    Entries[ListIndex]->RDAT.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                    Entries[ListIndex]->RDAT.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                    break;
                case 5: //objects
                    if(ListX2FieldID == 0) //objectsSize
                        {
                        Entries[ListIndex]->RDOT.resize(ArraySize);
                        return false;
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDOT.size())
                        break;

                    switch(ListX2FieldID)
                        {
                        case 1: //objectId
                            Entries[ListIndex]->RDOT[ListX2Index].objectId = *(FORMID *)FieldValue;
                            return true;
                        case 2: //parentIndex
                            Entries[ListIndex]->RDOT[ListX2Index].parentIndex = *(UINT16 *)FieldValue;
                            break;
                        case 3: //unused1
                            if(ArraySize != 2)
                                break;
                            Entries[ListIndex]->RDOT[ListX2Index].unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                            Entries[ListIndex]->RDOT[ListX2Index].unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                            break;
                        case 4: //density
                            Entries[ListIndex]->RDOT[ListX2Index].density = *(FLOAT32 *)FieldValue;
                            break;
                        case 5: //clustering
                            Entries[ListIndex]->RDOT[ListX2Index].clustering = *(UINT8 *)FieldValue;
                            break;
                        case 6: //minSlope
                            Entries[ListIndex]->RDOT[ListX2Index].minSlope = *(UINT8 *)FieldValue;
                            break;
                        case 7: //maxSlope
                            Entries[ListIndex]->RDOT[ListX2Index].maxSlope = *(UINT8 *)FieldValue;
                            break;
                        case 8: //flags
                            Entries[ListIndex]->RDOT[ListX2Index].SetFlagMask(*(UINT8 *)FieldValue);
                            break;
                        case 9: //radiusWRTParent
                            Entries[ListIndex]->RDOT[ListX2Index].radiusWRTParent = *(UINT16 *)FieldValue;
                            break;
                        case 10: //radius
                            Entries[ListIndex]->RDOT[ListX2Index].radius = *(UINT16 *)FieldValue;
                            break;
                        case 11: //unk1
                            if(ArraySize != 4)
                                break;
                            Entries[ListIndex]->RDOT[ListX2Index].unk1[0] = ((UINT8ARRAY)FieldValue)[0];
                            Entries[ListIndex]->RDOT[ListX2Index].unk1[1] = ((UINT8ARRAY)FieldValue)[1];
                            Entries[ListIndex]->RDOT[ListX2Index].unk1[2] = ((UINT8ARRAY)FieldValue)[2];
                            Entries[ListIndex]->RDOT[ListX2Index].unk1[3] = ((UINT8ARRAY)FieldValue)[3];
                            break;
                        case 12: //maxHeight
                            Entries[ListIndex]->RDOT[ListX2Index].maxHeight = *(FLOAT32 *)FieldValue;
                            break;
                        case 13: //sink
                            Entries[ListIndex]->RDOT[ListX2Index].sink = *(FLOAT32 *)FieldValue;
                            break;
                        case 14: //sinkVar
                            Entries[ListIndex]->RDOT[ListX2Index].sinkVar = *(FLOAT32 *)FieldValue;
                            break;
                        case 15: //sizeVar
                            Entries[ListIndex]->RDOT[ListX2Index].sizeVar = *(FLOAT32 *)FieldValue;
                            break;
                        case 16: //angleVarX
                            Entries[ListIndex]->RDOT[ListX2Index].angleVarX = *(UINT16 *)FieldValue;
                            break;
                        case 17: //angleVarY
                            Entries[ListIndex]->RDOT[ListX2Index].angleVarY = *(UINT16 *)FieldValue;
                            break;
                        case 18: //angleVarZ
                            Entries[ListIndex]->RDOT[ListX2Index].angleVarZ = *(UINT16 *)FieldValue;
                            break;
                        case 19: //unused2
                            if(ArraySize != 2)
                                break;
                            Entries[ListIndex]->RDOT[ListX2Index].unused2[0] = ((UINT8ARRAY)FieldValue)[0];
                            Entries[ListIndex]->RDOT[ListX2Index].unused2[1] = ((UINT8ARRAY)FieldValue)[1];
                            break;
                        case 20: //unk2
                            if(ArraySize != 4)
                                break;
                            Entries[ListIndex]->RDOT[ListX2Index].unk2[0] = ((UINT8ARRAY)FieldValue)[0];
                            Entries[ListIndex]->RDOT[ListX2Index].unk2[1] = ((UINT8ARRAY)FieldValue)[1];
                            Entries[ListIndex]->RDOT[ListX2Index].unk2[2] = ((UINT8ARRAY)FieldValue)[2];
                            Entries[ListIndex]->RDOT[ListX2Index].unk2[3] = ((UINT8ARRAY)FieldValue)[3];
                            break;
                        default:
                            break;
                        }
                    break;
                case 6: //mapName
                    Entries[ListIndex]->RDMP.Copy((STRING)FieldValue);
                    break;
                case 7: //iconPath
                    Entries[ListIndex]->ICON.Copy((STRING)FieldValue);
                    break;
                case 8: //grasses
                    if(ListX2FieldID == 0) //grassesSize
                        {
                        Entries[ListIndex]->RDGS.resize(ArraySize);
                        return false;
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDGS.size())
                        break;

                    switch(ListX2FieldID)
                        {
                        case 1: //grass
                            Entries[ListIndex]->RDGS[ListX2Index].grass = *(FORMID *)FieldValue;
                            return true;
                        case 2: //unk1
                            if(ArraySize != 4)
                                break;
                            Entries[ListIndex]->RDGS[ListX2Index].unk1[0] = ((UINT8ARRAY)FieldValue)[0];
                            Entries[ListIndex]->RDGS[ListX2Index].unk1[1] = ((UINT8ARRAY)FieldValue)[1];
                            Entries[ListIndex]->RDGS[ListX2Index].unk1[2] = ((UINT8ARRAY)FieldValue)[2];
                            Entries[ListIndex]->RDGS[ListX2Index].unk1[3] = ((UINT8ARRAY)FieldValue)[3];
                            break;
                        default:
                            break;
                        }
                    break;
                case 9: //musicType
                    Entries[ListIndex]->SetMusicType(*(UINT32 *)FieldValue);
                    break;
                case 10: //sounds
                    if(ListX2FieldID == 0) //soundsSize
                        {
                        Entries[ListIndex]->RDSD.resize(ArraySize);
                        return false;
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDSD.size())
                        break;

                    switch(ListX2FieldID)
                        {
                        case 1: //sound
                            Entries[ListIndex]->RDSD[ListX2Index].sound = *(FORMID *)FieldValue;
                            return true;
                        case 2: //flags
                            Entries[ListIndex]->RDSD[ListX2Index].SetFlagMask(*(UINT32 *)FieldValue);
                            break;
                        case 3: //chance
                            Entries[ListIndex]->RDSD[ListX2Index].chance = *(UINT32 *)FieldValue;
                            break;
                        default:
                            break;
                        }
                    break;
                case 11: //weathers
                    if(ListX2FieldID == 0) //weathersSize
                        {
                        Entries[ListIndex]->RDWT.resize(ArraySize);
                        return false;
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDWT.size())
                        break;

                    switch(ListX2FieldID)
                        {
                        case 1: //weather
                            Entries[ListIndex]->RDWT[ListX2Index].weather = *(FORMID *)FieldValue;
                            return true;
                        case 2: //chance
                            Entries[ListIndex]->RDWT[ListX2Index].chance = *(UINT32 *)FieldValue;
                            break;
                        default:
                            break;
                        }
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

void REGNRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GENCLR defaultCLR;

    REGNRDAT defaultRDAT;

    REGNRPLD defaultRPLD;
    REGNRDOT defaultRDOT;
    REGNRDGS defaultRDGS;
    REGNRDSD defaultRDSD;
    REGNRDWT defaultRDWT;

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
        case 6: //mapRed
            RCLR.value.red = defaultCLR.red;
            return;
        case 7: //mapGreen
            RCLR.value.green = defaultCLR.green;
            return;
        case 8: //mapBlue
            RCLR.value.blue = defaultCLR.blue;
            return;
        case 9: //unused1
            RCLR.value.unused1 = defaultCLR.unused1;
            return;
        case 10: //worldspace
            WNAM.Unload();
            return;
        case 11: //areas
            if(ListFieldID == 0) //areas
                {
                for(UINT32 x = 0; x < (UINT32)Areas.size(); x++)
                    delete Areas[x];
                Areas.clear();
                return;
                }

            if(ListIndex >= Areas.size())
                return;

            switch(ListFieldID)
                {
                case 1: //edgeFalloff
                    Areas[ListIndex]->RPLI.Unload();
                    return;
                case 2: //points
                    if(ListX2FieldID == 0) //points
                        {
                        Areas[ListIndex]->RPLD.clear();
                        return;
                        }

                    if(ListX2Index >= Areas[ListIndex]->RPLD.size())
                        return;

                    switch(ListX2FieldID)
                        {
                        case 1: //posX
                            Areas[ListIndex]->RPLD[ListX2Index].posX = defaultRPLD.posX;
                            return;
                        case 2: //posY
                            Areas[ListIndex]->RPLD[ListX2Index].posY = defaultRPLD.posY;
                            return;
                        default:
                            return;
                        }
                    return;
                default:
                    return;
                }
            return;
        case 12: //entries
            if(ListFieldID == 0) //entries
                {
                for(UINT32 x = 0; x < (UINT32)Entries.size(); x++)
                    delete Entries[x];
                Entries.clear();
                return;
                }

            if(ListIndex >= Entries.size())
                return;

            switch(ListFieldID)
                {
                case 1: //entryType
                    Entries[ListIndex]->RDAT.value.entryType = defaultRDAT.entryType;
                    return;
                case 2: //flags
                    Entries[ListIndex]->RDAT.value.flags = defaultRDAT.flags;
                    return;
                case 3: //priority
                    Entries[ListIndex]->RDAT.value.priority = defaultRDAT.priority;
                    return;
                case 4: //unused1
                    Entries[ListIndex]->RDAT.value.unused1[0] = defaultRDAT.unused1[0];
                    Entries[ListIndex]->RDAT.value.unused1[1] = defaultRDAT.unused1[1];
                    return;
                case 5: //objects
                    if(ListX2FieldID == 0) //objects
                        {
                        Entries[ListIndex]->RDOT.clear();
                        return;
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDOT.size())
                        return;

                    switch(ListX2FieldID)
                        {
                        case 1: //objectId
                            Entries[ListIndex]->RDOT[ListX2Index].objectId = defaultRDOT.objectId;
                            return;
                        case 2: //parentIndex
                            Entries[ListIndex]->RDOT[ListX2Index].parentIndex = defaultRDOT.parentIndex;
                            return;
                        case 3: //unused1
                            Entries[ListIndex]->RDOT[ListX2Index].unused1[0] = defaultRDOT.unused1[0];
                            Entries[ListIndex]->RDOT[ListX2Index].unused1[1] = defaultRDOT.unused1[1];
                            return;
                        case 4: //density
                            Entries[ListIndex]->RDOT[ListX2Index].density = defaultRDOT.density;
                            return;
                        case 5: //clustering
                            Entries[ListIndex]->RDOT[ListX2Index].clustering = defaultRDOT.clustering;
                            return;
                        case 6: //minSlope
                            Entries[ListIndex]->RDOT[ListX2Index].minSlope = defaultRDOT.minSlope;
                            return;
                        case 7: //maxSlope
                            Entries[ListIndex]->RDOT[ListX2Index].maxSlope = defaultRDOT.maxSlope;
                            return;
                        case 8: //flags
                            Entries[ListIndex]->RDOT[ListX2Index].flags = defaultRDOT.flags;
                            return;
                        case 9: //radiusWRTParent
                            Entries[ListIndex]->RDOT[ListX2Index].radiusWRTParent = defaultRDOT.radiusWRTParent;
                            return;
                        case 10: //radius
                            Entries[ListIndex]->RDOT[ListX2Index].radius = defaultRDOT.radius;
                            return;
                        case 11: //unk1
                            Entries[ListIndex]->RDOT[ListX2Index].unk1[0] = defaultRDOT.unk1[0];
                            Entries[ListIndex]->RDOT[ListX2Index].unk1[1] = defaultRDOT.unk1[1];
                            Entries[ListIndex]->RDOT[ListX2Index].unk1[2] = defaultRDOT.unk1[2];
                            Entries[ListIndex]->RDOT[ListX2Index].unk1[3] = defaultRDOT.unk1[3];
                            return;
                        case 12: //maxHeight
                            Entries[ListIndex]->RDOT[ListX2Index].maxHeight = defaultRDOT.maxHeight;
                            return;
                        case 13: //sink
                            Entries[ListIndex]->RDOT[ListX2Index].sink = defaultRDOT.sink;
                            return;
                        case 14: //sinkVar
                            Entries[ListIndex]->RDOT[ListX2Index].sinkVar = defaultRDOT.sinkVar;
                            return;
                        case 15: //sizeVar
                            Entries[ListIndex]->RDOT[ListX2Index].sizeVar = defaultRDOT.sizeVar;
                            return;
                        case 16: //angleVarX
                            Entries[ListIndex]->RDOT[ListX2Index].angleVarX = defaultRDOT.angleVarX;
                            return;
                        case 17: //angleVarY
                            Entries[ListIndex]->RDOT[ListX2Index].angleVarY = defaultRDOT.angleVarY;
                            return;
                        case 18: //angleVarZ
                            Entries[ListIndex]->RDOT[ListX2Index].angleVarZ = defaultRDOT.angleVarZ;
                            return;
                        case 19: //unused2
                            Entries[ListIndex]->RDOT[ListX2Index].unused2[0] = defaultRDOT.unused2[0];
                            Entries[ListIndex]->RDOT[ListX2Index].unused2[1] = defaultRDOT.unused2[1];
                            return;
                        case 20: //unk2
                            Entries[ListIndex]->RDOT[ListX2Index].unk2[0] = defaultRDOT.unk2[0];
                            Entries[ListIndex]->RDOT[ListX2Index].unk2[1] = defaultRDOT.unk2[1];
                            Entries[ListIndex]->RDOT[ListX2Index].unk2[2] = defaultRDOT.unk2[2];
                            Entries[ListIndex]->RDOT[ListX2Index].unk2[3] = defaultRDOT.unk2[3];
                            return;
                        default:
                            return;
                        }
                    return;
                case 6: //mapName
                    Entries[ListIndex]->RDMP.Unload();
                    return;
                case 7: //iconPath
                    Entries[ListIndex]->ICON.Unload();
                    return;
                case 8: //grasses
                    if(ListX2FieldID == 0) //grasses
                        {
                        Entries[ListIndex]->RDGS.clear();
                        return;
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDGS.size())
                        return;

                    switch(ListX2FieldID)
                        {
                        case 1: //grass
                            Entries[ListIndex]->RDGS[ListX2Index].grass = defaultRDGS.grass;
                            return;
                        case 2: //unk1
                            Entries[ListIndex]->RDGS[ListX2Index].unk1[0] = defaultRDGS.unk1[0];
                            Entries[ListIndex]->RDGS[ListX2Index].unk1[1] = defaultRDGS.unk1[1];
                            Entries[ListIndex]->RDGS[ListX2Index].unk1[2] = defaultRDGS.unk1[2];
                            Entries[ListIndex]->RDGS[ListX2Index].unk1[3] = defaultRDGS.unk1[3];
                            return;
                        default:
                            return;
                        }
                    return;
                case 9: //musicType
                    Entries[ListIndex]->RDMD.Unload();
                    return;
                case 10: //sounds
                    if(ListX2FieldID == 0) //sounds
                        {
                        Entries[ListIndex]->RDSD.clear();
                        return;
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDSD.size())
                        return;

                    switch(ListX2FieldID)
                        {
                        case 1: //sound
                            Entries[ListIndex]->RDSD[ListX2Index].sound = defaultRDSD.sound;
                            return;
                        case 2: //flags
                            Entries[ListIndex]->RDSD[ListX2Index].flags = defaultRDSD.flags;
                            return;
                        case 3: //chance
                            Entries[ListIndex]->RDSD[ListX2Index].chance = defaultRDSD.chance;
                            return;
                        default:
                            return;
                        }
                case 11: //weathers
                    if(ListX2FieldID == 0) //weathers
                        {
                        Entries[ListIndex]->RDWT.clear();
                        return;
                        }

                    if(ListX2Index >= Entries[ListIndex]->RDWT.size())
                        return;

                    switch(ListX2FieldID)
                        {
                        case 1: //weather
                            Entries[ListIndex]->RDWT[ListX2Index].weather = defaultRDWT.weather;
                            return;
                        case 2: //chance
                            Entries[ListIndex]->RDWT[ListX2Index].chance = defaultRDWT.chance;
                            return;
                        default:
                            return;
                        }
                default:
                    return;
                }
            return;
        default:
            return;
        }
    }