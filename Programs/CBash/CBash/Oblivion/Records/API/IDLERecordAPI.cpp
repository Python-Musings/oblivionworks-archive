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
#include "..\IDLERecord.h"

UINT32 IDLERecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //modPath
            return ISTRING_FIELD;
        case 6: //modb
            return FLOAT32_FIELD;
        case 7: //modt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return MODL.IsLoaded() ? MODL->MODT.GetSize() : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 8: //conditions
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
        case 9: //group
            return UINT8_FLAG_TYPE_FIELD;
        case 10: //parent
            return FORMID_FIELD;
        case 11: //prevId
            return FORMID_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * IDLERecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //modPath
            return MODL.IsLoaded() ? MODL->MODL.value : NULL;
        case 6: //modb
            return MODL.IsLoaded() ? &MODL->MODB.value.MODB : NULL;
        case 7: //modt_p
            *FieldValues = MODL.IsLoaded() ? MODL->MODT.value : NULL;
            return NULL;
        case 8: //conditions
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
        case 9: //group
            return &ANAM.value.group;
        case 10: //parent
            return &DATA.value.parent;
        case 11: //prevId
            return &DATA.value.prevId;
        default:
            return NULL;
        }
    }

bool IDLERecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //modPath
            MODL.Load();
            MODL->MODL.Copy((STRING)FieldValue);
            break;
        case 6: //modb
            MODL.Load();
            MODL->MODB.value.MODB = *(FLOAT32 *)FieldValue;
            break;
        case 7: //modt_p
            MODL.Load();
            MODL->MODT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 8: //conditions
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
        case 9: //group
            ANAM.value.group = *(UINT8 *)FieldValue;
            break;
        case 10: //parent
            DATA.value.parent = *(FORMID *)FieldValue;
            return true;
        case 11: //prevId
            DATA.value.prevId = *(FORMID *)FieldValue;
            return true;
        default:
            break;
        }
    return false;
    }

void IDLERecord::DeleteField(FIELD_IDENTIFIERS)
    {
    IDLEDATA defaultDATA;
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
        case 5: //modPath
            if(MODL.IsLoaded())
                MODL->MODL.Unload();
            return;
        case 6: //modb
            if(MODL.IsLoaded())
                MODL->MODB.Unload();
            return;
        case 7: //modt_p
            if(MODL.IsLoaded())
                MODL->MODT.Unload();
            return;
        case 8: //conditions
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
        case 9: //group
            ANAM.Unload();
            return;
        case 10: //parent
            DATA.value.parent = defaultDATA.parent;
            return;
        case 11: //prevId
            DATA.value.prevId = defaultDATA.prevId;
            return;
        default:
            return;
        }
    }