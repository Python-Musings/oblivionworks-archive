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
#include "..\SCPTRecord.h"

UINT32 SCPTRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 4;
                default:
                    return UNKNOWN_FIELD;
                }
        case 6: //numRefs
            return UINT32_FIELD;
        case 7: //compiledSize
            return UINT32_FIELD;
        case 8: //lastIndex
            return UINT32_FIELD;
        case 9: //scriptType
            return UINT32_FIELD;
        case 10: //compiled_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return SCDA.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 11: //scriptText
            return ISTRING_FIELD;
        case 12: //vars
            if(ListFieldID == 0) //vars
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)VARS.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= VARS.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //index
                    return UINT32_FIELD;
                case 2: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 12;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 3: //flags
                    return UINT8_FLAG_FIELD;
                case 4: //unused2
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 7;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 5: //name
                    return STRING_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 13: //references
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

void * SCPTRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //unused1
            *FieldValues = &SCHR.value.unused1[0];
            return NULL;
        case 6: //numRefs
            return &SCHR.value.numRefs;
        case 7: //compiledSize
            return &SCHR.value.compiledSize;
        case 8: //lastIndex
            return &SCHR.value.lastIndex;
        case 9: //scriptType
            return &SCHR.value.scriptType;
        case 10: //compiled_p
            *FieldValues = SCDA.value;
            return NULL;
        case 11: //scriptText
            return SCTX.value;
        case 12: //vars
            if(ListIndex >= VARS.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //index
                    return &VARS[ListIndex]->SLSD.value.index;
                case 2: //unused1
                    *FieldValues = &VARS[ListIndex]->SLSD.value.unused1[0];
                    return NULL;
                case 3: //flags
                    return &VARS[ListIndex]->SLSD.value.flags;
                case 4: //unused2
                    *FieldValues = &VARS[ListIndex]->SLSD.value.unused2[0];
                    return NULL;
                case 5: //name
                    return VARS[ListIndex]->SCVR.value;
                default:
                    *FieldValues = NULL;
                    return NULL;
                }
        case 13: //references
            for(UINT32 x = 0; x < SCR_.size(); ++x)
                ((FORMIDARRAY)FieldValues)[x] = SCR_[x]->value.reference;
            return NULL;
        default:
            return NULL;
        }
    }

bool SCPTRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //unused1
            if(ArraySize != 4)
                break;
            SCHR.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            SCHR.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            SCHR.value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
            SCHR.value.unused1[3] = ((UINT8ARRAY)FieldValue)[3];
            break;
        case 6: //numRefs
            SCHR.value.numRefs = *(UINT32 *)FieldValue;
            break;
        case 7: //compiledSize
            SCHR.value.compiledSize = *(UINT32 *)FieldValue;
            break;
        case 8: //lastIndex
            SCHR.value.lastIndex = *(UINT32 *)FieldValue;
            break;
        case 9: //scriptType
            SetType(*(UINT32 *)FieldValue);
            break;
        case 10: //compiled_p
            SCDA.Copy((UINT8ARRAY)FieldValue, ArraySize);
            SCHR.value.compiledSize = ArraySize;
            break;
        case 11: //scriptText
            SCTX.Copy((STRING)FieldValue);
            break;
        case 12: //vars
            if(ListFieldID == 0) //varsSize
                {
                ArraySize -= (UINT32)VARS.size();
                while((SINT32)ArraySize > 0)
                    {
                    VARS.push_back(new SCPTVARS);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete VARS.back();
                    VARS.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= VARS.size())
                break;

            switch(ListFieldID)
                {
                case 1: //index
                    VARS[ListIndex]->SLSD.value.index = *(UINT32 *)FieldValue;
                    break;
                case 2: //unused1
                    if(ArraySize != 12)
                        break;
                    VARS[ListIndex]->SLSD.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                    VARS[ListIndex]->SLSD.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                    VARS[ListIndex]->SLSD.value.unused1[2] = ((UINT8ARRAY)FieldValue)[2];
                    VARS[ListIndex]->SLSD.value.unused1[3] = ((UINT8ARRAY)FieldValue)[3];
                    VARS[ListIndex]->SLSD.value.unused1[4] = ((UINT8ARRAY)FieldValue)[4];
                    VARS[ListIndex]->SLSD.value.unused1[5] = ((UINT8ARRAY)FieldValue)[5];
                    VARS[ListIndex]->SLSD.value.unused1[6] = ((UINT8ARRAY)FieldValue)[6];
                    VARS[ListIndex]->SLSD.value.unused1[7] = ((UINT8ARRAY)FieldValue)[7];
                    VARS[ListIndex]->SLSD.value.unused1[8] = ((UINT8ARRAY)FieldValue)[8];
                    VARS[ListIndex]->SLSD.value.unused1[9] = ((UINT8ARRAY)FieldValue)[9];
                    VARS[ListIndex]->SLSD.value.unused1[10] = ((UINT8ARRAY)FieldValue)[10];
                    VARS[ListIndex]->SLSD.value.unused1[11] = ((UINT8ARRAY)FieldValue)[11];
                    break;
                case 3: //flags
                    VARS[ListIndex]->SetFlagMask(*(UINT8 *)FieldValue);
                    break;
                case 4: //unused2
                    if(ArraySize != 7)
                        break;
                    VARS[ListIndex]->SLSD.value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
                    VARS[ListIndex]->SLSD.value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
                    VARS[ListIndex]->SLSD.value.unused2[2] = ((UINT8ARRAY)FieldValue)[2];
                    VARS[ListIndex]->SLSD.value.unused2[3] = ((UINT8ARRAY)FieldValue)[3];
                    VARS[ListIndex]->SLSD.value.unused2[4] = ((UINT8ARRAY)FieldValue)[4];
                    VARS[ListIndex]->SLSD.value.unused2[5] = ((UINT8ARRAY)FieldValue)[5];
                    VARS[ListIndex]->SLSD.value.unused2[6] = ((UINT8ARRAY)FieldValue)[6];
                    break;
                case 5: //name
                    VARS[ListIndex]->SCVR.Copy((STRING)FieldValue);
                    break;
                default:
                    break;
                }
            break;
        case 13: //references
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

void SCPTRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GENSCHR defaultSCHR;

    SCPTVARS defaultVARS;
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
        case 5: //unused1
            SCHR.value.unused1[0] = defaultSCHR.unused1[0];
            SCHR.value.unused1[1] = defaultSCHR.unused1[1];
            SCHR.value.unused1[2] = defaultSCHR.unused1[2];
            SCHR.value.unused1[3] = defaultSCHR.unused1[3];
            return;
        case 6: //numRefs
            SCHR.value.numRefs = defaultSCHR.numRefs;
            return;
        case 7: //compiledSize
            SCHR.value.compiledSize = defaultSCHR.compiledSize;
            return;
        case 8: //lastIndex
            SCHR.value.lastIndex = defaultSCHR.lastIndex;
            return;
        case 9: //scriptType
            SCHR.value.scriptType = defaultSCHR.scriptType;
            return;
        case 10: //compiled_p
            SCDA.Unload();
            return;
        case 11: //scriptText
            SCTX.Unload();
            return;
        case 12: //vars
            if(ListFieldID == 0) //vars
                {
                for(UINT32 x = 0; x < (UINT32)VARS.size(); x++)
                    delete VARS[x];
                VARS.clear();
                return;
                }

            if(ListIndex >= VARS.size())
                return;

            switch(ListFieldID)
                {
                case 1: //index
                    VARS[ListIndex]->SLSD.value.index = defaultVARS.SLSD.value.index;
                    return;
                case 2: //unused1
                    VARS[ListIndex]->SLSD.value.unused1[0] = defaultVARS.SLSD.value.unused1[0];
                    VARS[ListIndex]->SLSD.value.unused1[1] = defaultVARS.SLSD.value.unused1[1];
                    VARS[ListIndex]->SLSD.value.unused1[2] = defaultVARS.SLSD.value.unused1[2];
                    VARS[ListIndex]->SLSD.value.unused1[3] = defaultVARS.SLSD.value.unused1[3];
                    VARS[ListIndex]->SLSD.value.unused1[4] = defaultVARS.SLSD.value.unused1[4];
                    VARS[ListIndex]->SLSD.value.unused1[5] = defaultVARS.SLSD.value.unused1[5];
                    VARS[ListIndex]->SLSD.value.unused1[6] = defaultVARS.SLSD.value.unused1[6];
                    VARS[ListIndex]->SLSD.value.unused1[7] = defaultVARS.SLSD.value.unused1[7];
                    VARS[ListIndex]->SLSD.value.unused1[8] = defaultVARS.SLSD.value.unused1[8];
                    VARS[ListIndex]->SLSD.value.unused1[9] = defaultVARS.SLSD.value.unused1[9];
                    VARS[ListIndex]->SLSD.value.unused1[10] = defaultVARS.SLSD.value.unused1[10];
                    VARS[ListIndex]->SLSD.value.unused1[11] = defaultVARS.SLSD.value.unused1[11];
                    return;
                case 3: //flags
                    VARS[ListIndex]->SLSD.value.flags = defaultVARS.SLSD.value.flags;
                    return;
                case 4: //unused2
                    VARS[ListIndex]->SLSD.value.unused2[0] = defaultVARS.SLSD.value.unused2[0];
                    VARS[ListIndex]->SLSD.value.unused2[1] = defaultVARS.SLSD.value.unused2[1];
                    VARS[ListIndex]->SLSD.value.unused2[2] = defaultVARS.SLSD.value.unused2[2];
                    VARS[ListIndex]->SLSD.value.unused2[3] = defaultVARS.SLSD.value.unused2[3];
                    VARS[ListIndex]->SLSD.value.unused2[4] = defaultVARS.SLSD.value.unused2[4];
                    VARS[ListIndex]->SLSD.value.unused2[5] = defaultVARS.SLSD.value.unused2[5];
                    VARS[ListIndex]->SLSD.value.unused2[6] = defaultVARS.SLSD.value.unused2[6];
                    return;
                case 5: //name
                    VARS[ListIndex]->SCVR.Unload();
                    return;
                default:
                    return;
                }
        case 13: //references
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
    return;
    }