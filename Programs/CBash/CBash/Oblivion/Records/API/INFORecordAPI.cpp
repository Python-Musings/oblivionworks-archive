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
#include "..\INFORecord.h"

UINT32 INFORecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //dialType
            return UINT16_TYPE_FIELD;
        case 6: //flags
            return UINT8_FLAG_FIELD;
        case 7: //quest
            return FORMID_FIELD;
        case 8: //topic
            return FORMID_FIELD;
        case 9: //prevInfo
            return FORMID_FIELD;
        case 10: //addTopics
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)NAME.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 11: //responses
            if(ListFieldID == 0) //responses
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)Responses.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= Responses.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //emotionType
                    return UINT32_FIELD;
                case 2: //emotionValue
                    return SINT32_FIELD;
                case 3: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 4;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 4: //responseNum
                    return UINT8_FIELD;
                case 5: //unused2
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 3;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 6: //responseText
                    return STRING_FIELD;
                case 7: //actorNotes
                    return ISTRING_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 12: //conditions
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
        case 13: //choices
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)TCLT.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 14: //linksFrom
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)TCLF.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 15: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 4;
                default:
                    return UNKNOWN_FIELD;
                }
        case 16: //numRefs
            return UINT32_FIELD;
        case 17: //compiledSize
            return UINT32_FIELD;
        case 18: //lastIndex
            return UINT32_FIELD;
        case 19: //scriptType
            return UINT32_FIELD;
        case 20: //compiled_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return SCDA.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 21: //scriptText
            return ISTRING_FIELD;
        case 22: //references
            if(ListFieldID == 0) //references
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return FORMID_OR_UINT32_ARRAY_FIELD;
                    case 1: //fieldSize
                        return (UINT32)SCR_.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= SCR_.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //reference
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return FORMID_OR_UINT32_FIELD;
                        case 2: //WhichType
                            return (SCR_[ListIndex]->value.isSCRO ? FORMID_FIELD : UINT32_FIELD);
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

void * INFORecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //dialType
            return &DATA.value.dialType;
        case 6: //flags
            return &DATA.value.flags;
        case 7: //quest
            return &QSTI.value.fid;
        case 8: //topic
            return (TPIC.IsLoaded() ? &TPIC->fid : NULL);
        case 9: //prevInfo
            return (PNAM.IsLoaded() ? &PNAM->fid : NULL);
        case 10: //addTopics
            *FieldValues = NAME.size() ? &NAME[0] : NULL;
            return NULL;
        case 11: //responses
            if(ListIndex >= Responses.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //emotionType
                    return &Responses[ListIndex]->TRDT.value.emotionType;
                case 2: //emotionValue
                    return &Responses[ListIndex]->TRDT.value.emotionValue;
                case 3: //unused1
                    *FieldValues = &Responses[ListIndex]->TRDT.value.unused1[0];
                    return NULL;
                case 4: //responseNum
                    return &Responses[ListIndex]->TRDT.value.responseNum;
                case 5: //unused2
                    *FieldValues = &Responses[ListIndex]->TRDT.value.unused2[0];
                    return NULL;
                case 6: //responseText
                    return Responses[ListIndex]->NAM1.value;
                case 7: //actorNotes
                    return Responses[ListIndex]->NAM2.value;
                default:
                    *FieldValues = NULL;
                    return NULL;
                }
        case 12: //conditions
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
        case 13: //choices
            *FieldValues = TCLT.size() ? &TCLT[0] : NULL;
            return NULL;
        case 14: //linksFrom
            *FieldValues = TCLF.size() ? &TCLF[0] : NULL;
            return NULL;
        case 15: //unused1
            *FieldValues = &SCHR.value.unused1[0];
            return NULL;
        case 16: //numRefs
            return &SCHR.value.numRefs;
        case 17: //compiledSize
            return &SCHR.value.compiledSize;
        case 18: //lastIndex
            return &SCHR.value.lastIndex;
        case 19: //scriptType
            return &SCHR.value.scriptType;
        case 20: //compiled_p
            *FieldValues = SCDA.value;
            return NULL;
        case 21: //scriptText
            return SCTX.value;
        case 22: //references
            for(UINT32 x = 0; x < SCR_.size(); ++x)
                ((FORMIDARRAY)FieldValues)[x] = SCR_[x]->value.reference;
            return NULL;
        default:
            return NULL;
        }
    }

bool INFORecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //dialType
            SetDialogType(*(UINT16 *)FieldValue);
            break;
        case 6: //flags
            SetFlagMask(*(UINT8 *)FieldValue);
            break;
        case 7: //quest
            QSTI.value.fid = *(FORMID *)FieldValue;
            return true;
        case 8: //topic
            TPIC.Load();
            TPIC->fid = *(FORMID *)FieldValue;
            return true;
        case 9: //prevInfo
            PNAM.Load();
            PNAM->fid = *(FORMID *)FieldValue;
            return true;
        case 10: //addTopics
            NAME.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                NAME[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 11: //responses
            if(ListFieldID == 0) //responsesSize
                {
                ArraySize -= (UINT32)Responses.size();
                while((SINT32)ArraySize > 0)
                    {
                    Responses.push_back(new INFOResponse);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete Responses.back();
                    Responses.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= Responses.size())
                break;

            switch(ListFieldID)
                {
                case 1: //emotionType
                    Responses[ListIndex]->SetType(*(UINT32 *)FieldValue);
                    break;
                case 2: //emotionValue
                    Responses[ListIndex]->TRDT.value.emotionValue = *(SINT32 *)FieldValue;
                    break;
                case 3: //unused1
                    if(ArraySize != 4)
                        break;
                    Responses[ListIndex]->TRDT.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                    Responses[ListIndex]->TRDT.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                    Responses[ListIndex]->TRDT.value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
                    Responses[ListIndex]->TRDT.value.unused1[3] = ((UINT8ARRAY)FieldValue)[3];
                    break;
                case 4: //responseNum
                    Responses[ListIndex]->TRDT.value.responseNum = *(UINT8 *)FieldValue;
                    break;
                case 5: //unused2
                    if(ArraySize != 3)
                        break;
                    Responses[ListIndex]->TRDT.value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
                    Responses[ListIndex]->TRDT.value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
                    Responses[ListIndex]->TRDT.value.unused2[2] = ((UINT8ARRAY)FieldValue)[2];
                    break;
                case 6: //responseText
                    Responses[ListIndex]->NAM1.Copy((STRING)FieldValue);
                    break;
                case 7: //actorNotes
                    Responses[ListIndex]->NAM2.Copy((STRING)FieldValue);
                    break;
                default:
                    break;
                }
            break;
        case 12: //conditions
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
        case 13: //choices
            TCLT.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                TCLT[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 14: //linksFrom
            TCLF.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                TCLF[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 15: //unused1
            if(ArraySize != 4)
                break;
            SCHR.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            SCHR.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            SCHR.value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
            SCHR.value.unused1[3] = ((UINT8ARRAY)FieldValue)[3];
            break;
        case 16: //numRefs
            SCHR.value.numRefs = *(UINT32 *)FieldValue;
            break;
        case 17: //compiledSize
            SCHR.value.compiledSize = *(UINT32 *)FieldValue;
            break;
        case 18: //lastIndex
            SCHR.value.lastIndex = *(UINT32 *)FieldValue;
            break;
        case 19: //scriptType
            SCHR.value.scriptType = *(UINT32 *)FieldValue;
            break;
        case 20: //compiled_p
            SCDA.Copy(((UINT8ARRAY)FieldValue), ArraySize);
            break;
        case 21: //scriptText
            SCTX.Copy((STRING)FieldValue);
            break;
        case 22: //references
            if(ListFieldID == 0) //referencesSize
                {
                ArraySize -= (UINT32)SCR_.size();
                while((SINT32)ArraySize > 0)
                    {
                    SCR_.push_back(new ReqSubRecord<GENSCR_>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete SCR_.back();
                    SCR_.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= SCR_.size())
                break;

            switch(ListFieldID)
                {
                case 1: //reference
                    //Borrowing ArraySize to flag if the new value is a formID
                    SCR_[ListIndex]->value.reference = *(UINT32 *)FieldValue;
                    SCR_[ListIndex]->value.isSCRO = ArraySize ? true : false;
                    return ArraySize != 0;
                default:
                    break;
                }
            break;
        default:
            break;
        }
    return false;
    }

void INFORecord::DeleteField(FIELD_IDENTIFIERS)
    {
    INFODATA defaultDATA;
    GENSCHR defaultSCHR;

    INFOTRDT defaultTRDT;
    GENCTDA defaultCTDA;
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
        case 5: //dialType
            DATA.value.dialType = defaultDATA.dialType;
            return;
        case 6: //flags
            DATA.value.flags = defaultDATA.flags;
            return;
        case 7: //quest
            QSTI.Unload();
            return;
        case 8: //topic
            TPIC.Unload();
            return;
        case 9: //prevInfo
            PNAM.Unload();
            return;
        case 10: //addTopics
            NAME.clear();
            return;
        case 11: //responses
            if(ListFieldID == 0) //responses
                {
                for(UINT32 x = 0; x < (UINT32)Responses.size(); x++)
                    delete Responses[x];
                Responses.clear();
                return;
                }

            if(ListIndex >= Responses.size())
                return;

            switch(ListFieldID)
                {
                case 1: //emotionType
                    Responses[ListIndex]->TRDT.value.emotionType = defaultTRDT.emotionType;
                    return;
                case 2: //emotionValue
                    Responses[ListIndex]->TRDT.value.emotionValue = defaultTRDT.emotionValue;
                    return;
                case 3: //unused1
                    Responses[ListIndex]->TRDT.value.unused1[0] = defaultTRDT.unused1[0];
                    Responses[ListIndex]->TRDT.value.unused1[1] = defaultTRDT.unused1[1];
                    Responses[ListIndex]->TRDT.value.unused1[2] = defaultTRDT.unused1[2];
                    Responses[ListIndex]->TRDT.value.unused1[3] = defaultTRDT.unused1[3];
                    return;
                case 4: //responseNum
                    Responses[ListIndex]->TRDT.value.responseNum = defaultTRDT.responseNum;
                    return;
                case 5: //unused2
                    Responses[ListIndex]->TRDT.value.unused2[0] = defaultTRDT.unused2[0];
                    Responses[ListIndex]->TRDT.value.unused2[1] = defaultTRDT.unused2[1];
                    Responses[ListIndex]->TRDT.value.unused2[2] = defaultTRDT.unused2[2];
                    return;
                case 6: //responseText
                    Responses[ListIndex]->NAM1.Unload();
                    return;
                case 7: //actorNotes
                    Responses[ListIndex]->NAM2.Unload();
                    return;
                default:
                    return;
                }
            return;
        case 12: //conditions
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
        case 13: //choices
            TCLT.clear();
            return;
        case 14: //linksFrom
            TCLF.clear();
            return;
        case 15: //unused1
            SCHR.value.unused1[0] = defaultSCHR.unused1[0];
            SCHR.value.unused1[1] = defaultSCHR.unused1[1];
            SCHR.value.unused1[2] = defaultSCHR.unused1[2];
            SCHR.value.unused1[3] = defaultSCHR.unused1[3];
            return;
        case 16: //numRefs
            SCHR.value.numRefs = defaultSCHR.numRefs;
            return;
        case 17: //compiledSize
            SCHR.value.compiledSize = defaultSCHR.compiledSize;
            return;
        case 18: //lastIndex
            SCHR.value.lastIndex = defaultSCHR.lastIndex;
            return;
        case 19: //scriptType
            SCHR.value.scriptType = defaultSCHR.scriptType;
            return;
        case 20: //compiled_p
            SCDA.Unload();
            return;
        case 21: //scriptText
            SCTX.Unload();
            return;
        case 22: //references
            if(ListFieldID == 0) //references
                {
                for(UINT32 x = 0; x < (UINT32)SCR_.size(); x++)
                    delete SCR_[x];
                SCR_.clear();
                return;
                }

            if(ListIndex >= SCR_.size())
                return;

            switch(ListFieldID)
                {
                case 1: //reference
                    SCR_[ListIndex]->value.reference = defaultSCR_.reference;
                    SCR_[ListIndex]->value.isSCRO = defaultSCR_.isSCRO;
                    return;
                default:
                    return;
                }
        default:
            return;
        }
    }