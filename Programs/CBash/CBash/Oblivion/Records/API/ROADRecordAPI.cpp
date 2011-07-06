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
#include "..\ROADRecord.h"

UINT32 ROADRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
            return MISSING_FIELD;
        case 5: //pgrp
            if(ListFieldID == 0) //pgrp
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)PGRP.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= PGRP.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //x
                    return FLOAT32_FIELD;
                case 2: //y
                    return FLOAT32_FIELD;
                case 3: //z
                    return FLOAT32_FIELD;
                case 4: //connections
                    return UINT8_FIELD;
                case 5: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 3;
                        default:
                            return UNKNOWN_FIELD;
                        }
                default:
                    return UNKNOWN_FIELD;
                }
        case 6: //pgrr
            if(ListFieldID == 0) //pgrr
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)PGRR.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= PGRR.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //x
                    return FLOAT32_FIELD;
                case 2: //y
                    return FLOAT32_FIELD;
                case 3: //z
                    return FLOAT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * ROADRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
    {
    switch(FieldID)
        {
        case 1: //flags1
            return &flags;
        case 2: //fid
            return &formID;
        case 3: //flags2
            return &flagsUnk;
        case 5: //pgrp
            if(ListIndex >= PGRP.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //x
                    return &PGRP[ListIndex].x;
                case 2: //y
                    return &PGRP[ListIndex].y;
                case 3: //z
                    return &PGRP[ListIndex].z;
                case 4: //connections
                    return &PGRP[ListIndex].connections;
                case 5: //unused1
                    *FieldValues = &PGRP[ListIndex].unused1[0];
                    return NULL;
                default:
                    return NULL;
                }
        case 6: //pgrr
            if(ListIndex >= PGRR.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //x
                    return &PGRR[ListIndex].x;
                case 2: //y
                    return &PGRR[ListIndex].y;
                case 3: //z
                    return &PGRR[ListIndex].z;
                default:
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool ROADRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
    {
    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(*(UINT32 *)FieldValue);
            break;
        case 3: //flags2
            SetHeaderUnknownFlagMask(*(UINT32 *)FieldValue);
            break;
        case 5: //pgrp
            if(ListFieldID == 0) //pgrpSize
                {
                PGRP.resize(ArraySize);
                return false;
                }

            if(ListIndex >= PGRP.size())
                break;

            switch(ListFieldID)
                {
                case 1: //x
                    PGRP[ListIndex].x = *(FLOAT32 *)FieldValue;
                    break;
                case 2: //y
                    PGRP[ListIndex].y = *(FLOAT32 *)FieldValue;
                    break;
                case 3: //z
                    PGRP[ListIndex].z = *(FLOAT32 *)FieldValue;
                    break;
                case 4: //connections
                    PGRP[ListIndex].connections = *(UINT8 *)FieldValue;
                    break;
                case 5: //unused1
                    if(ArraySize != 3)
                        break;
                    PGRP[ListIndex].unused1[0] = ((UINT8 *)FieldValue)[0];
                    PGRP[ListIndex].unused1[1] = ((UINT8 *)FieldValue)[1];
                    PGRP[ListIndex].unused1[2] = ((UINT8 *)FieldValue)[2];
                    break;
                default:
                    break;
                }
            break;
        case 6: //pgrr
            if(ListFieldID == 0) //pgrrSize
                {
                PGRR.resize(ArraySize);
                return false;
                }

            if(ListIndex >= PGRR.size())
                break;

            switch(ListFieldID)
                {
                case 1: //x
                    PGRR[ListIndex].x = *(FLOAT32 *)FieldValue;
                    break;
                case 2: //y
                    PGRR[ListIndex].y = *(FLOAT32 *)FieldValue;
                    break;
                case 3: //z
                    PGRR[ListIndex].z = *(FLOAT32 *)FieldValue;
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

void ROADRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GENPGRP defaultPGRP;
    ROADPGRR defaultPGRR;

    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(0);
            return;
        case 3: //flags2
            flagsUnk = 0;
            return;
        case 5: //pgrp
            if(ListFieldID == 0) //pgrp
                {
                PGRP.clear();
                return;
                }

            if(ListIndex >= PGRP.size())
                return;

            switch(ListFieldID)
                {
                case 1: //x
                    PGRP[ListIndex].x = defaultPGRP.x;
                    return;
                case 2: //y
                    PGRP[ListIndex].y = defaultPGRP.y;
                    return;
                case 3: //z
                    PGRP[ListIndex].z = defaultPGRP.z;
                    return;
                case 4: //connections
                    PGRP[ListIndex].connections = defaultPGRP.connections;
                    return;
                case 5: //unused1
                    PGRP[ListIndex].unused1[0] = defaultPGRP.unused1[0];
                    PGRP[ListIndex].unused1[1] = defaultPGRP.unused1[1];
                    PGRP[ListIndex].unused1[2] = defaultPGRP.unused1[2];
                    return;
                default:
                    return;
                }
        case 6: //pgrr
            if(ListFieldID == 0) //pgrr
                {
                PGRR.clear();
                return;
                }

            if(ListIndex >= PGRR.size())
                return;

            switch(ListFieldID)
                {
                case 1: //x
                    PGRR[ListIndex].x = defaultPGRP.x;
                    return;
                case 2: //y
                    PGRR[ListIndex].y = defaultPGRP.y;
                    return;
                case 3: //z
                    PGRR[ListIndex].z = defaultPGRP.z;
                    return;
                default:
                    return;
                }
        default:
            return;
        }
    }