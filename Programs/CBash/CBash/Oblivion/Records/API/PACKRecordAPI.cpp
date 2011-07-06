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
#include "..\PACKRecord.h"

UINT32 PACKRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
    {
    Function_Arguments_Iterator curCTDAFunction;

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
        case 5: //flags
            return UINT32_FLAG_FIELD;
        case 6: //aiType
            return UINT8_TYPE_FIELD;
        case 7: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 3;
                default:
                    return UNKNOWN_FIELD;
                }
        case 8: //locType
            return SINT32_TYPE_FIELD;
        case 9: //locId
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_OR_UINT32_FIELD;
                case 2: //WhichType
                    return (PLDT.IsLoaded() && PLDT->locType != 5) ? FORMID_FIELD : UINT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 10: //locRadius
            return SINT32_FIELD;
        case 11: //month
            return SINT8_FIELD;
        case 12: //day
            return SINT8_FIELD;
        case 13: //date
            return UINT8_FIELD;
        case 14: //time
            return SINT8_FIELD;
        case 15: //duration
            return SINT32_FIELD;
        case 16: //targetType
            return SINT32_TYPE_FIELD;
        case 17: //targetId
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_OR_UINT32_FIELD;
                case 2: //WhichType
                    return (PTDT.IsLoaded() && PTDT->targetType != 2) ? FORMID_FIELD : UINT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 18: //targetCount
            return SINT32_FIELD;
        case 19: //conditions
            if(ListFieldID == 0) //conditions
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)CTDA.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= CTDA.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //operType
                    return UINT8_FLAG_TYPE_FIELD;
                case 2: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 3;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 3: //compValue
                    return FLOAT32_FIELD;
                case 4: //ifunc
                    return UINT32_TYPE_FIELD;
                case 5: //param1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UNKNOWN_OR_FORMID_OR_UINT32_FIELD;
                        case 2: //WhichType
                            curCTDAFunction = Function_Arguments.find(CTDA[ListIndex]->value.ifunc);
                            if(curCTDAFunction != Function_Arguments.end())
                                switch(curCTDAFunction->second.first)
                                    {
                                    case eFORMID:
                                        return FORMID_FIELD;
                                    case eUINT32:
                                        return UINT32_FIELD;
                                    default:
                                        return UNKNOWN_FIELD;
                                    }
                            return UNKNOWN_FIELD;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 6: //param2
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UNKNOWN_OR_FORMID_OR_UINT32_FIELD;
                        case 2: //WhichType
                            curCTDAFunction = Function_Arguments.find(CTDA[ListIndex]->value.ifunc);
                            if(curCTDAFunction != Function_Arguments.end())
                                switch(curCTDAFunction->second.second)
                                    {
                                    case eFORMID:
                                        return FORMID_FIELD;
                                    case eUINT32:
                                        return UINT32_FIELD;
                                    default:
                                        return UNKNOWN_FIELD;
                                    }
                            return UNKNOWN_FIELD;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 7: //unused2
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
        default:
            return UNKNOWN_FIELD;
        }
    }

void * PACKRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //flags
            return &PKDT.value.flags;
        case 6: //aiType
            return &PKDT.value.aiType;
        case 7: //unused1
            *FieldValues = &PKDT.value.unused1[0];
            return NULL;
        case 8: //locType
            return PLDT.IsLoaded() ? &PLDT->locType : NULL;
        case 9: //locId
            return PLDT.IsLoaded() ? &PLDT->locId : NULL;
        case 10: //locRadius
            return PLDT.IsLoaded() ? &PLDT->locRadius : NULL;
        case 11: //month
            return &PSDT.value.month;
        case 12: //day
            return &PSDT.value.day;
        case 13: //date
            return &PSDT.value.date;
        case 14: //time
            return &PSDT.value.time;
        case 15: //duration
            return &PSDT.value.duration;
        case 16: //targetType
            return PTDT.IsLoaded() ? &PTDT->targetType : NULL;
        case 17: //targetId
            return PTDT.IsLoaded() ? &PTDT->targetId : NULL;
        case 18: //targetCount
            return PTDT.IsLoaded() ? &PTDT->targetCount : NULL;
        case 19: //conditions
            if(ListIndex >= CTDA.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //operType
                    return &CTDA[ListIndex]->value.operType;
                case 2: //unused1
                    *FieldValues = &CTDA[ListIndex]->value.unused1[0];
                    return NULL;
                case 3: //compValue
                    return &CTDA[ListIndex]->value.compValue;
                case 4: //ifunc
                    return &CTDA[ListIndex]->value.ifunc;
                case 5: //param1
                    return &CTDA[ListIndex]->value.param1;
                case 6: //param2
                    return &CTDA[ListIndex]->value.param2;
                case 7: //unused2
                    *FieldValues = &CTDA[ListIndex]->value.unused2[0];
                    return NULL;
                default:
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool PACKRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //flags
            SetFlagMask(*(UINT32 *)FieldValue);
            break;
        case 6: //aiType
            SetAIType(*(UINT8 *)FieldValue);
            break;
        case 7: //unused1
            if(ArraySize != 3)
                break;
            PKDT.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            PKDT.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            PKDT.value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        case 8: //locType
            SetLocType(*(SINT32 *)FieldValue);
            return true;
        case 9: //locId
            PLDT.Load();
            PLDT->locId = *(FORMID_OR_UINT32 *)FieldValue;
            return true;
        case 10: //locRadius
            PLDT.Load();
            PLDT->locRadius = *(SINT32 *)FieldValue;
            break;
        case 11: //month
            PSDT.value.month = *(SINT8 *)FieldValue;
            break;
        case 12: //day
            PSDT.value.day = *(SINT8 *)FieldValue;
            break;
        case 13: //date
            PSDT.value.date = *(UINT8 *)FieldValue;
            break;
        case 14: //time
            PSDT.value.time = *(SINT8 *)FieldValue;
            break;
        case 15: //duration
            PSDT.value.duration = *(SINT32 *)FieldValue;
            break;
        case 16: //targetType
            SetTargetType(*(SINT32 *)FieldValue);
            return true;
        case 17: //targetId
            PTDT.Load();
            PTDT->targetId = *(UINT32 *)FieldValue;
            return true;
        case 18: //targetCount
            PTDT.Load();
            PTDT->targetCount = *(SINT32 *)FieldValue;
            break;
        case 19: //conditions
            if(ListFieldID == 0) //conditionsSize
                {
                ArraySize -= (UINT32)CTDA.size();
                while((SINT32)ArraySize > 0)
                    {
                    CTDA.push_back(new ReqSubRecord<GENCTDA>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete CTDA.back();
                    CTDA.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= CTDA.size())
                break;

            switch(ListFieldID)
                {
                case 1: //operType
                    CTDA[ListIndex]->value.operType = *(UINT8 *)FieldValue;
                    break;
                case 2: //unused1
                    if(ArraySize != 3)
                        break;
                    CTDA[ListIndex]->value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                    CTDA[ListIndex]->value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                    CTDA[ListIndex]->value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
                    break;
                case 3: //compValue
                    CTDA[ListIndex]->value.compValue = *(FLOAT32 *)FieldValue;
                    break;
                case 4: //ifunc
                    CTDA[ListIndex]->value.ifunc = *(UINT32 *)FieldValue;
                    return true;
                case 5: //param1
                    CTDA[ListIndex]->value.param1 = *(UINT32 *)FieldValue;
                    return true;
                case 6: //param2
                    CTDA[ListIndex]->value.param2 = *(UINT32 *)FieldValue;
                    return true;
                case 7: //unused2
                    if(ArraySize != 4)
                        break;
                    CTDA[ListIndex]->value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
                    CTDA[ListIndex]->value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
                    CTDA[ListIndex]->value.unused2[2] = ((UINT8ARRAY)FieldValue)[2];
                    CTDA[ListIndex]->value.unused2[3] = ((UINT8ARRAY)FieldValue)[3];
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

void PACKRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    PACKPKDT defaultPKDT;
    PACKPLDT defaultPLDT;
    PACKPSDT defaultPSDT;
    PACKPTDT defaultPTDT;

    GENCTDA defaultCTDA;

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
        case 5: //flags
            PKDT.value.flags = defaultPKDT.flags;
            return;
        case 6: //aiType
            PKDT.value.aiType = defaultPKDT.aiType;
            return;
        case 7: //unused1
            PKDT.value.unused1[0] = defaultPKDT.unused1[0];
            PKDT.value.unused1[1] = defaultPKDT.unused1[1];
            PKDT.value.unused1[2] = defaultPKDT.unused1[2];
            return;
        case 8: //locType
            if(PLDT.IsLoaded())
                PLDT->locType = defaultPLDT.locType;
            return;
        case 9: //locId
            if(PLDT.IsLoaded())
                PLDT->locId = defaultPLDT.locId;
            return;
        case 10: //locRadius
            if(PLDT.IsLoaded())
                PLDT->locRadius = defaultPLDT.locRadius;
            return;
        case 11: //month
            PSDT.value.month = defaultPSDT.month;
            return;
        case 12: //day
            PSDT.value.day = defaultPSDT.day;
            return;
        case 13: //date
            PSDT.value.date = defaultPSDT.date;
            return;
        case 14: //time
            PSDT.value.time = defaultPSDT.time;
            return;
        case 15: //duration
            PSDT.value.duration = defaultPSDT.duration;
            return;
        case 16: //targetType
            if(PTDT.IsLoaded())
                PTDT->targetType = defaultPTDT.targetType;
            return;
        case 17: //targetId
            if(PTDT.IsLoaded())
                PTDT->targetId = defaultPTDT.targetId;
            return;
        case 18: //targetCount
            if(PTDT.IsLoaded())
                PTDT->targetCount = defaultPTDT.targetCount;
            return;
        case 19: //conditions
            if(ListFieldID == 0) //conditions
                {
                for(UINT32 x = 0; x < (UINT32)CTDA.size(); ++x)
                    delete CTDA[x];
                CTDA.clear();
                return;
                }

            if(ListIndex >= CTDA.size())
                return;

            switch(ListFieldID)
                {
                case 1: //operType
                    CTDA[ListIndex]->value.operType = defaultCTDA.operType;
                    return;
                case 2: //unused1
                    CTDA[ListIndex]->value.unused1[0] = defaultCTDA.unused1[0];
                    CTDA[ListIndex]->value.unused1[1] = defaultCTDA.unused1[1];
                    CTDA[ListIndex]->value.unused1[2] = defaultCTDA.unused1[2];
                    return;
                case 3: //compValue
                    CTDA[ListIndex]->value.compValue = defaultCTDA.compValue;
                    return;
                case 4: //ifunc
                    CTDA[ListIndex]->value.ifunc = defaultCTDA.ifunc;
                    return;
                case 5: //param1
                    CTDA[ListIndex]->value.param1 = defaultCTDA.param1;
                    return;
                case 6: //param2
                    CTDA[ListIndex]->value.param2 = defaultCTDA.param2;
                    return;
                case 7: //unused2
                    CTDA[ListIndex]->value.unused2[0] = defaultCTDA.unused2[0];
                    CTDA[ListIndex]->value.unused2[1] = defaultCTDA.unused2[1];
                    CTDA[ListIndex]->value.unused2[2] = defaultCTDA.unused2[2];
                    CTDA[ListIndex]->value.unused2[3] = defaultCTDA.unused2[3];
                    return;
                default:
                    return;
                }
        default:
            return;
        }
    }