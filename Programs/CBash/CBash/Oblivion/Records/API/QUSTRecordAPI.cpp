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
#include "..\QUSTRecord.h"

UINT32 QUSTRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //script
            return FORMID_FIELD;
        case 6: //full
            return STRING_FIELD;
        case 7: //iconPath
            return ISTRING_FIELD;
        case 8: //flags
            return UINT8_FLAG_FIELD;
        case 9: //priority
            return UINT8_FIELD;
        case 10: //conditions
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
        case 11: //stages
            if(ListFieldID == 0) //conditions
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)Stages.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= Stages.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //stage
                    return UINT16_FIELD;
                case 2: //entries
                    if(ListX2FieldID == 0) //entries
                        {
                        switch(WhichAttribute)
                            {
                            case 0: //fieldType
                                return LIST_FIELD;
                            case 1: //fieldSize
                                return (UINT32)Stages[ListIndex]->Entries.size();
                            default:
                                return UNKNOWN_FIELD;
                            }
                        }

                    if(ListX2Index >= Stages[ListIndex]->Entries.size())
                        return UNKNOWN_FIELD;

                    switch(ListX2FieldID)
                        {
                        case 1: //flags
                            return UINT8_FLAG_FIELD;
                        case 2: //conditions
                            if(ListX3FieldID == 0) //conditions
                                {
                                switch(WhichAttribute)
                                    {
                                    case 0: //fieldType
                                        return LIST_FIELD;
                                    case 1: //fieldSize
                                        return (UINT32)Stages[ListIndex]->Entries[ListX2Index]->CTDA.size();
                                    default:
                                        return UNKNOWN_FIELD;
                                    }
                                }

                            if(ListX3Index >= Stages[ListIndex]->Entries[ListX2Index]->CTDA.size())
                                return UNKNOWN_FIELD;

                            switch(ListX3FieldID)
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
                                            curCTDAFunction = Function_Arguments.find(Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.ifunc);
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
                                            curCTDAFunction = Function_Arguments.find(Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.ifunc);
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
                        case 3: //text
                            return STRING_FIELD;
                        case 4: //unused1
                            switch(WhichAttribute)
                                {
                                case 0: //fieldType
                                    return UINT8_ARRAY_FIELD;
                                case 1: //fieldSize
                                    return 4;
                                default:
                                    return UNKNOWN_FIELD;
                                }
                        case 5: //numRefs
                            return UINT32_FIELD;
                        case 6: //compiledSize
                            return UINT32_FIELD;
                        case 7: //lastIndex
                            return UINT32_FIELD;
                        case 8: //scriptType
                            return UINT32_FIELD;
                        case 9: //compiled_p
                            switch(WhichAttribute)
                                {
                                case 0: //fieldType
                                    return UINT8_ARRAY_FIELD;
                                case 1: //fieldSize
                                    return Stages[ListIndex]->Entries[ListX2Index]->SCDA.GetSize();
                                default:
                                    return UNKNOWN_FIELD;
                                }
                        case 10: //scriptText
                            return ISTRING_FIELD;
                        case 11: //references
                            if(ListX3FieldID == 0) //references
                                {
                                switch(WhichAttribute)
                                    {
                                    case 0: //fieldType
                                        return FORMID_OR_UINT32_ARRAY_FIELD;
                                    case 1: //fieldSize
                                        return (UINT32)Stages[ListIndex]->Entries[ListX2Index]->SCR_.size();
                                    default:
                                        return UNKNOWN_FIELD;
                                    }
                                }

                            if(ListX3Index >= Stages[ListIndex]->Entries[ListX2Index]->SCR_.size())
                                return UNKNOWN_FIELD;

                            switch(ListX3FieldID)
                                {
                                case 1: //reference
                                    switch(WhichAttribute)
                                        {
                                        case 0: //fieldType
                                            return FORMID_OR_UINT32_FIELD;
                                        case 2: //WhichType
                                            return (Stages[ListIndex]->Entries[ListX2Index]->SCR_[ListX3Index]->value.isSCRO ? FORMID_FIELD : UINT32_FIELD);
                                        default:
                                            return UNKNOWN_FIELD;
                                        }
                                default:
                                    return UNKNOWN_FIELD;
                                }
                        default:
                            return UNKNOWN_FIELD;
                        }
                default:
                    return UNKNOWN_FIELD;
                }
        case 12: //targets
            if(ListFieldID == 0) //targets
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)Targets.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= Targets.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //targetId
                    return FORMID_FIELD;
                case 2: //flags
                    return UINT8_FLAG_FIELD;
                case 3: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 3;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 4: //conditions
                    if(ListX2FieldID == 0) //conditions
                        {
                        switch(WhichAttribute)
                            {
                            case 0: //fieldType
                                return LIST_FIELD;
                            case 1: //fieldSize
                                return (UINT32)Targets[ListIndex]->CTDA.size();
                            default:
                                return UNKNOWN_FIELD;
                            }
                        }

                    if(ListX2Index >= Targets[ListIndex]->CTDA.size())
                        return UNKNOWN_FIELD;

                    switch(ListX2FieldID)
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
                                    curCTDAFunction = Function_Arguments.find(Targets[ListIndex]->CTDA[ListX2Index]->value.ifunc);
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
                                    curCTDAFunction = Function_Arguments.find(Targets[ListIndex]->CTDA[ListX2Index]->value.ifunc);
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
        default:
            return UNKNOWN_FIELD;
        }
    }

void * QUSTRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //script
            return SCRI.IsLoaded() ? &SCRI->fid : NULL;
        case 6: //full
            return FULL.value;
        case 7: //iconPath
            return ICON.value;
        case 8: //flags
            return &DATA.value.flags;
        case 9: //priority
            return &DATA.value.priority;
        case 10: //conditions
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
        case 11: //stages
            if(ListIndex >= Stages.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //stage
                    return &Stages[ListIndex]->INDX.value.stage;
                case 2: //entries
                    if(ListX2Index >= Stages[ListIndex]->Entries.size())
                        {
                        *FieldValues = NULL;
                        return NULL;
                        }
                    switch(ListX2FieldID)
                        {
                        case 1: //flags
                            return &Stages[ListIndex]->Entries[ListX2Index]->QSDT.value.flags;
                        case 2: //conditions
                            if(ListX3Index >= Stages[ListIndex]->Entries[ListX2Index]->CTDA.size())
                                return NULL;

                            switch(ListX3FieldID)
                                {
                                case 1: //operType
                                    return &Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.operType;
                                case 2: //unused1
                                    *FieldValues = &Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused1[0];
                                    return NULL;
                                case 3: //compValue
                                    return &Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.compValue;
                                case 4: //ifunc
                                    return &Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.ifunc;
                                case 5: //param1
                                    return &Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.param1;
                                case 6: //param2
                                    return &Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.param2;
                                case 7: //unused2
                                    *FieldValues = &Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[0];
                                    return NULL;
                                default:
                                    return NULL;
                                }
                        case 3: //text
                            return Stages[ListIndex]->Entries[ListX2Index]->CNAM.value;
                        case 4: //unused1
                            *FieldValues = &Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[0];
                            return NULL;
                        case 5: //numRefs
                            return &Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.numRefs;
                        case 6: //compiledSize
                            return &Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.compiledSize;
                        case 7: //lastIndex
                            return &Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.lastIndex;
                        case 8: //scriptType
                            return &Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.scriptType;
                        case 9: //compiled_p
                            *FieldValues = Stages[ListIndex]->Entries[ListX2Index]->SCDA.value;
                            return NULL;
                        case 10: //scriptText
                            return Stages[ListIndex]->Entries[ListX2Index]->SCTX.value;
                        case 11: //references
                            for(UINT32 x = 0; x < Stages[ListIndex]->Entries[ListX2Index]->SCR_.size(); ++x)
                                ((FORMIDARRAY)FieldValues)[x] = Stages[ListIndex]->Entries[ListX2Index]->SCR_[x]->value.reference;
                            return NULL;
                        default:
                            return NULL;
                        }
                default:
                    return NULL;
                }
        case 12: //targets
            if(ListIndex >= Targets.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //targetId
                    return &Targets[ListIndex]->QSTA.value.targetId;
                case 2: //flags
                    return &Targets[ListIndex]->QSTA.value.flags;
                case 3: //unused1
                    *FieldValues = &Targets[ListIndex]->QSTA.value.unused1[0];
                    return NULL;
                case 4: //conditions
                    if(ListX2Index >= Targets[ListIndex]->CTDA.size())
                        return NULL;

                    switch(ListX2FieldID)
                        {
                        case 1: //operType
                            return &Targets[ListIndex]->CTDA[ListX2Index]->value.operType;
                        case 2: //unused1
                            *FieldValues = &Targets[ListIndex]->CTDA[ListX2Index]->value.unused1[0];
                            return NULL;
                        case 3: //compValue
                            return &Targets[ListIndex]->CTDA[ListX2Index]->value.compValue;
                        case 4: //ifunc
                            return &Targets[ListIndex]->CTDA[ListX2Index]->value.ifunc;
                        case 5: //param1
                            return &Targets[ListIndex]->CTDA[ListX2Index]->value.param1;
                        case 6: //param2
                            return &Targets[ListIndex]->CTDA[ListX2Index]->value.param2;
                        case 7: //unused2
                            *FieldValues = &Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[0];
                            return NULL;
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

bool QUSTRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //script
            SCRI.Load();
            SCRI->fid = *(FORMID *)FieldValue;
            return true;
        case 6: //full
            FULL.Copy((STRING)FieldValue);
            break;
        case 7: //iconPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 8: //flags
            SetFlagMask(*(UINT8 *)FieldValue);
            break;
        case 9: //priority
            DATA.value.priority = *(UINT8 *)FieldValue;
            break;
        case 10: //conditions
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
        case 11: //stages
            if(ListFieldID == 0) //stagesSize
                {
                ArraySize -= (UINT32)Stages.size();
                while((SINT32)ArraySize > 0)
                    {
                    Stages.push_back(new QUSTStage);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete Stages.back();
                    Stages.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= Stages.size())
                break;

            switch(ListFieldID)
                {
                case 1: //stage
                    Stages[ListIndex]->INDX.value.stage = *(UINT16 *)FieldValue;
                    break;
                case 2: //entries
                    if(ListX2FieldID == 0) //entriesSize
                        {
                        ArraySize -= (UINT32)Stages[ListIndex]->Entries.size();
                        while((SINT32)ArraySize > 0)
                            {
                            Stages[ListIndex]->Entries.push_back(new QUSTEntry);
                            --ArraySize;
                            }
                        while((SINT32)ArraySize < 0)
                            {
                            delete Stages[ListIndex]->Entries.back();
                            Stages[ListIndex]->Entries.pop_back();
                            ++ArraySize;
                            }
                        return false;
                        }

                    if(ListX2Index >= Stages[ListIndex]->Entries.size())
                        break;

                    switch(ListX2FieldID)
                        {
                        case 1: //flags
                            Stages[ListIndex]->Entries[ListX2Index]->SetFlagMask(*(UINT8 *)FieldValue);
                            break;
                        case 2: //conditions
                            if(ListX3FieldID == 0) //conditionsSize
                                {
                                ArraySize -= (UINT32)Stages[ListIndex]->Entries[ListX2Index]->CTDA.size();
                                while((SINT32)ArraySize > 0)
                                    {
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA.push_back(new ReqSubRecord<GENCTDA>);
                                    --ArraySize;
                                    }
                                while((SINT32)ArraySize < 0)
                                    {
                                    delete Stages[ListIndex]->Entries[ListX2Index]->CTDA.back();
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA.pop_back();
                                    ++ArraySize;
                                    }
                                return false;
                                }

                            if(ListX3Index >= (UINT32)Stages[ListIndex]->Entries[ListX2Index]->CTDA.size())
                                break;

                            switch(ListX3FieldID)
                                {
                                case 1: //operType
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.operType = *(UINT8 *)FieldValue;
                                    break;
                                case 2: //unused1
                                    if(ArraySize != 3)
                                        break;
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
                                    break;
                                case 3: //compValue
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.compValue = *(FLOAT32 *)FieldValue;
                                    break;
                                case 4: //ifunc
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.ifunc = *(UINT32 *)FieldValue;
                                    return true;
                                case 5: //param1
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.param1 = *(UINT32 *)FieldValue;
                                    return true;
                                case 6: //param2
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.param2 = *(UINT32 *)FieldValue;
                                    return true;
                                case 7: //unused2
                                    if(ArraySize != 4)
                                        break;
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[2] = ((UINT8ARRAY)FieldValue)[2];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[3] = ((UINT8ARRAY)FieldValue)[3];
                                    break;
                                default:
                                    break;
                                }
                            break;
                        case 3: //text
                            Stages[ListIndex]->Entries[ListX2Index]->CNAM.Copy((STRING)FieldValue);
                            break;
                        case 4: //unused1
                            if(ArraySize != 4)
                                break;
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[3] = ((UINT8ARRAY)FieldValue)[3];
                            break;
                        case 5: //numRefs
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.numRefs = *(UINT32 *)FieldValue;
                            break;
                        case 6: //compiledSize
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.compiledSize = *(UINT32 *)FieldValue;
                            break;
                        case 7: //lastIndex
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.lastIndex = *(UINT32 *)FieldValue;
                            break;
                        case 8: //scriptType
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.scriptType = *(UINT32 *)FieldValue;
                            break;
                        case 9: //compiled_p
                            Stages[ListIndex]->Entries[ListX2Index]->SCDA.Copy((UINT8ARRAY)FieldValue, ArraySize);
                            break;
                        case 10: //scriptText
                            Stages[ListIndex]->Entries[ListX2Index]->SCTX.Copy((STRING)FieldValue);
                            break;
                        case 11: //references
                            if(ListX3FieldID == 0) //referencesSize
                                {
                                ArraySize -= (UINT32)Stages[ListIndex]->Entries[ListX2Index]->SCR_.size();
                                while((SINT32)ArraySize > 0)
                                    {
                                    Stages[ListIndex]->Entries[ListX2Index]->SCR_.push_back(new ReqSubRecord<GENSCR_>);
                                    --ArraySize;
                                    }
                                while((SINT32)ArraySize < 0)
                                    {
                                    delete Stages[ListIndex]->Entries[ListX2Index]->SCR_.back();
                                    Stages[ListIndex]->Entries[ListX2Index]->SCR_.pop_back();
                                    ++ArraySize;
                                    }
                                return false;
                                }

                            if(ListX3Index >= Stages[ListIndex]->Entries[ListX2Index]->SCR_.size())
                                break;

                            switch(ListX3FieldID)
                                {
                                case 1: //reference
                                    //Borrowing ArraySize to flag if the new value is a formID
                                    Stages[ListIndex]->Entries[ListX2Index]->SCR_[ListX3Index]->value.reference = *(UINT32 *)FieldValue;
                                    Stages[ListIndex]->Entries[ListX2Index]->SCR_[ListX3Index]->value.isSCRO = ArraySize ? true : false;
                                    return ArraySize != 0;
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
            break;
        case 12: //targets
            if(ListFieldID == 0) //targetsSize
                {
                ArraySize -= (UINT32)Targets.size();
                while((SINT32)ArraySize > 0)
                    {
                    Targets.push_back(new QUSTTarget);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete Targets.back();
                    Targets.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= Targets.size())
                break;

            switch(ListFieldID)
                {
                case 1: //targetId
                    Targets[ListIndex]->QSTA.value.targetId = *(FORMID *)FieldValue;
                    return true;
                case 2: //flags
                    Targets[ListIndex]->SetFlagMask(*(UINT8 *)FieldValue);
                    break;
                case 3: //unused1
                    if(ArraySize != 3)
                        break;
                    Targets[ListIndex]->QSTA.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                    Targets[ListIndex]->QSTA.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                    Targets[ListIndex]->QSTA.value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
                    break;
                case 4: //conditions
                    if(ListX2FieldID == 0) //conditionsSize
                        {
                        ArraySize -= (UINT32)Targets[ListIndex]->CTDA.size();
                        while((SINT32)ArraySize > 0)
                            {
                            Targets[ListIndex]->CTDA.push_back(new ReqSubRecord<GENCTDA>);
                            --ArraySize;
                            }
                        while((SINT32)ArraySize < 0)
                            {
                            delete Targets[ListIndex]->CTDA.back();
                            Targets[ListIndex]->CTDA.pop_back();
                            ++ArraySize;
                            }
                        return false;
                        }

                    if(ListX2Index >= (UINT32)Targets[ListIndex]->CTDA.size())
                        break;

                    switch(ListX2FieldID)
                        {
                        case 1: //operType
                            Targets[ListIndex]->CTDA[ListX2Index]->value.operType = *(UINT8 *)FieldValue;
                            break;
                        case 2: //unused1
                            if(ArraySize != 3)
                                break;
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
                            break;
                        case 3: //compValue
                            Targets[ListIndex]->CTDA[ListX2Index]->value.compValue = *(FLOAT32 *)FieldValue;
                            break;
                        case 4: //ifunc
                            Targets[ListIndex]->CTDA[ListX2Index]->value.ifunc = *(UINT32 *)FieldValue;
                            return true;
                        case 5: //param1
                            Targets[ListIndex]->CTDA[ListX2Index]->value.param1 = *(UINT32 *)FieldValue;
                            return true;
                        case 6: //param2
                            Targets[ListIndex]->CTDA[ListX2Index]->value.param2 = *(UINT32 *)FieldValue;
                            return true;
                        case 7: //unused2
                            if(ArraySize != 4)
                                break;
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[2] = ((UINT8ARRAY)FieldValue)[2];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[3] = ((UINT8ARRAY)FieldValue)[3];
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

void QUSTRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    QUSTDATA defaultDATA;

    GENCTDA defaultCTDA;
    QUSTQSTA defaultQSTA;

    GENSCHR defaultSCHR;

    GENSCR_ defaultSCR_;

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
        case 5: //script
            SCRI.Unload();
            return;
        case 6: //full
            FULL.Unload();
            return;
        case 7: //iconPath
            ICON.Unload();
            return;
        case 8: //flags
            DATA.value.flags = defaultDATA.flags;
            return;
        case 9: //priority
            DATA.value.priority = defaultDATA.priority;
            return;
        case 10: //conditions
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
        case 11: //stages
            if(ListFieldID == 0) //stages
                {
                for(UINT32 x = 0; x < (UINT32)Stages.size(); x++)
                    delete Stages[x];
                Stages.clear();
                return;
                }

            if(ListIndex >= Stages.size())
                return;

            switch(ListFieldID)
                {
                case 1: //stage
                    Stages[ListIndex]->INDX.Unload();
                    return;
                case 2: //entries
                    if(ListX2FieldID == 0) //entries
                        {
                        for(UINT32 x = 0; x < (UINT32)Stages[ListIndex]->Entries.size(); x++)
                            delete Stages[ListIndex]->Entries[x];
                        Stages[ListIndex]->Entries.clear();
                        return;
                        }

                    if(ListX2Index >= Stages[ListIndex]->Entries.size())
                        return;

                    switch(ListX2FieldID)
                        {
                        case 1: //flags
                            Stages[ListIndex]->Entries[ListX2Index]->QSDT.Unload();
                            return;
                        case 2: //conditions
                            if(ListX3FieldID == 0) //conditions
                                {
                                for(UINT32 x = 0; x < (UINT32)Stages[ListIndex]->Entries[ListX2Index]->CTDA.size(); ++x)
                                    delete Stages[ListIndex]->Entries[ListX2Index]->CTDA[x];
                                Stages[ListIndex]->Entries[ListX2Index]->CTDA.clear();
                                return;
                                }

                            if(ListX3Index >= Stages[ListIndex]->Entries[ListX2Index]->CTDA.size())
                                return;

                            switch(ListX3FieldID)
                                {
                                case 1: //operType
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.operType = defaultCTDA.operType;
                                    return;
                                case 2: //unused1
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused1[0] = defaultCTDA.unused1[0];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused1[1] = defaultCTDA.unused1[1];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused1[2] = defaultCTDA.unused1[2];
                                    return;
                                case 3: //compValue
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.compValue = defaultCTDA.compValue;
                                    return;
                                case 4: //ifunc
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.ifunc = defaultCTDA.ifunc;
                                    return;
                                case 5: //param1
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.param1 = defaultCTDA.param1;
                                    return;
                                case 6: //param2
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.param2 = defaultCTDA.param2;
                                    return;
                                case 7: //unused2
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[0] = defaultCTDA.unused2[0];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[1] = defaultCTDA.unused2[1];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[2] = defaultCTDA.unused2[2];
                                    Stages[ListIndex]->Entries[ListX2Index]->CTDA[ListX3Index]->value.unused2[3] = defaultCTDA.unused2[3];
                                    return;
                                default:
                                    return;
                                }
                        case 3: //text
                            Stages[ListIndex]->Entries[ListX2Index]->CNAM.Unload();
                            return;
                        case 4: //unused1
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[0] = defaultSCHR.unused1[0];
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[1] = defaultSCHR.unused1[1];
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[2] = defaultSCHR.unused1[2];
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.unused1[3] = defaultSCHR.unused1[3];
                            return;
                        case 5: //numRefs
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.numRefs = defaultSCHR.numRefs;
                            return;
                        case 6: //compiledSize
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.compiledSize = defaultSCHR.compiledSize;
                            return;
                        case 7: //lastIndex
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.lastIndex = defaultSCHR.lastIndex;
                            return;
                        case 8: //scriptType
                            Stages[ListIndex]->Entries[ListX2Index]->SCHR.value.scriptType = defaultSCHR.scriptType;
                            return;
                        case 9: //compiled_p
                            Stages[ListIndex]->Entries[ListX2Index]->SCDA.Unload();
                            return;
                        case 10: //scriptText
                            Stages[ListIndex]->Entries[ListX2Index]->SCTX.Unload();
                            return;
                        case 11: //references
                            if(ListX3FieldID == 0) //references
                                {
                                for(UINT32 x = 0; x < (UINT32)Stages[ListIndex]->Entries[ListX2Index]->SCR_.size(); x++)
                                    delete Stages[ListIndex]->Entries[ListX2Index]->SCR_[x];
                                Stages[ListIndex]->Entries[ListX2Index]->SCR_.clear();
                                return;
                                }

                            if(ListX3Index >= Stages[ListIndex]->Entries[ListX2Index]->SCR_.size())
                                return;

                            switch(ListX3FieldID)
                                {
                                case 1: //reference
                                    Stages[ListIndex]->Entries[ListX2Index]->SCR_[ListX3Index]->value.reference = defaultSCR_.reference;
                                    Stages[ListIndex]->Entries[ListX2Index]->SCR_[ListX3Index]->value.isSCRO = defaultSCR_.isSCRO;
                                    return;
                                default:
                                    return;
                                }
                        default:
                            return;
                        }
                default:
                    return;
                }
            return;
        case 12: //targets
            if(ListFieldID == 0) //stages
                {
                for(UINT32 x = 0; x < (UINT32)Targets.size(); x++)
                    delete Targets[x];
                Targets.clear();
                return;
                }

            if(ListIndex >= Targets.size())
                return;

            switch(ListFieldID)
                {
                case 1: //targetId
                    Targets[ListIndex]->QSTA.value.targetId = defaultQSTA.targetId;
                    return;
                case 2: //flags
                    Targets[ListIndex]->QSTA.value.flags = defaultQSTA.flags;
                    return;
                case 3: //unused1
                    Targets[ListIndex]->QSTA.value.unused1[0] = defaultQSTA.unused1[0];
                    Targets[ListIndex]->QSTA.value.unused1[1] = defaultQSTA.unused1[1];
                    Targets[ListIndex]->QSTA.value.unused1[2] = defaultQSTA.unused1[2];
                    return;
                case 4: //conditions
                    if(ListX2FieldID == 0) //conditions
                        {
                        for(UINT32 x = 0; x < (UINT32)Targets[ListIndex]->CTDA.size(); ++x)
                            delete Targets[ListIndex]->CTDA[x];
                        Targets[ListIndex]->CTDA.clear();
                        return;
                        }

                    if(ListX2Index >= Targets[ListIndex]->CTDA.size())
                        return;

                    switch(ListX2FieldID)
                        {
                        case 1: //operType
                            Targets[ListIndex]->CTDA[ListX2Index]->value.operType = defaultCTDA.operType;
                            return;
                        case 2: //unused1
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused1[0] = defaultCTDA.unused1[0];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused1[1] = defaultCTDA.unused1[1];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused1[2] = defaultCTDA.unused1[2];
                            return;
                        case 3: //compValue
                            Targets[ListIndex]->CTDA[ListX2Index]->value.compValue = defaultCTDA.compValue;
                            return;
                        case 4: //ifunc
                            Targets[ListIndex]->CTDA[ListX2Index]->value.ifunc = defaultCTDA.ifunc;
                            return;
                        case 5: //param1
                            Targets[ListIndex]->CTDA[ListX2Index]->value.param1 = defaultCTDA.param1;
                            return;
                        case 6: //param2
                            Targets[ListIndex]->CTDA[ListX2Index]->value.param2 = defaultCTDA.param2;
                            return;
                        case 7: //unused2
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[0] = defaultCTDA.unused2[0];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[1] = defaultCTDA.unused2[1];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[2] = defaultCTDA.unused2[2];
                            Targets[ListIndex]->CTDA[ListX2Index]->value.unused2[3] = defaultCTDA.unused2[3];
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