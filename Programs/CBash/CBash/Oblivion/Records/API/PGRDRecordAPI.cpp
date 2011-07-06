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
#include "..\PGRDRecord.h"

UINT32 PGRDRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //count
            return UINT16_FIELD;
        case 6: //pgrp
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
        case 7: //pgag_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return PGAG.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 8: //pgrr_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return PGRR.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 9: //pgri
            if(ListFieldID == 0) //pgri
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)PGRI.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= PGRI.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //point
                    return UINT16_FIELD;
                case 2: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 2;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 3: //x
                    return FLOAT32_FIELD;
                case 4: //y
                    return FLOAT32_FIELD;
                case 5: //z
                    return FLOAT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 10: //pgrl
            if(ListFieldID == 0) //pgrl
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)PGRL.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= PGRL.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //reference
                    return FORMID_FIELD;
                case 2: //points
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT32_ARRAY_FIELD;
                        case 1: //fieldSize
                            //The first entry in the points array is actually the reference
                            return PGRL[ListIndex]->points.size() - 1;
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

void * PGRDRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
    {
    switch(FieldID)
        {
        case 1: //flags1
            return &flags;
        case 2: //fid
            return &formID;
        case 3: //flags2
            return &flagsUnk;
        case 5: //count
            return &DATA.value.count;
        case 6: //pgrp
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
        case 7: //pgag_p
            *FieldValues = PGAG.value;
            return NULL;
        case 8: //pgrr_p
            *FieldValues = PGRR.value;
            return NULL;
        case 9: //pgri
            if(ListIndex >= PGRI.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //point
                    return &PGRI[ListIndex].point;
                case 2: //unused1
                    *FieldValues = &PGRI[ListIndex].unused1[0];
                    return NULL;
                case 3: //x
                    return &PGRI[ListIndex].x;
                case 4: //y
                    return &PGRI[ListIndex].y;
                case 5: //z
                    return &PGRI[ListIndex].z;
                default:
                    *FieldValues = NULL;
                    return NULL;
                }
        case 10: //pgrl
            if(ListIndex >= PGRL.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //reference
                    return PGRL[ListIndex]->points.size() ? &PGRL[ListIndex]->points[0] : NULL;
                case 2: //points
                    *FieldValues = (PGRL[ListIndex]->points.size() > 1) ? &PGRL[ListIndex]->points[1] : NULL;
                    return NULL;
                default:
                    *FieldValues = NULL;
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool PGRDRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
    {
    UINT32 reference = 0;

    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(*(UINT32 *)FieldValue);
            break;
        case 3: //flags2
            SetHeaderUnknownFlagMask(*(UINT32 *)FieldValue);
            break;
        case 5: //count
            DATA.value.count = *(UINT16 *)FieldValue;
            break;
        case 6: //pgrp
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
        case 7: //pgag_p
            PGAG.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 8: //pgrr_p
            PGRR.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 9: //pgri
            if(ListFieldID == 0) //pgriSize
                {
                PGRI.resize(ArraySize);
                return false;
                }

            if(ListIndex >= PGRI.size())
                break;

            switch(ListFieldID)
                {
                case 1: //point
                    PGRI[ListIndex].point = *(UINT16 *)FieldValue;
                    break;
                case 2: //unused1
                    if(ArraySize != 2)
                        break;
                    PGRI[ListIndex].unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                    PGRI[ListIndex].unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                    break;
                case 3: //x
                    PGRI[ListIndex].x = *(FLOAT32 *)FieldValue;
                case 4: //y
                    PGRI[ListIndex].y = *(FLOAT32 *)FieldValue;
                case 5: //z
                    PGRI[ListIndex].z = *(FLOAT32 *)FieldValue;
                default:
                    break;
                }
            break;
        case 10: //pgrl
            if(ListFieldID == 0) //pgriSize
                {
                ArraySize -= (UINT32)PGRL.size();
                while((SINT32)ArraySize > 0)
                    {
                    PGRL.push_back(new PGRDPGRL);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete PGRL.back();
                    PGRL.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= PGRL.size())
                break;

            switch(ListFieldID)
                {
                case 1: //reference
                    if(PGRL[ListIndex]->points.size() == 0) //Sanity check, should already have a minimum of 1
                        PGRL[ListIndex]->points.resize(1);
                    PGRL[ListIndex]->points[0] = *(FORMID *)FieldValue;
                    return true;
                case 2: //points
                    reference = PGRL[ListIndex]->points.size() ? PGRL[ListIndex]->points[0] : 0;

                    PGRL[ListIndex]->points.resize(ArraySize + 1);
                    PGRL[ListIndex]->points[0] = reference;

                    for(UINT32 x = 0; x < ArraySize; x++)
                        PGRL[ListIndex]->points[x + 1] = ((UINT32ARRAY)FieldValue)[x];
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

void PGRDRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GENPGRP defaultPGRP;
    PGRDPGRI defaultPGRI;
    PGRDPGRL defaultPGRL;
    UINT32 reference;

    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(0);
            return;
        case 3: //flags2
            flagsUnk = 0;
            return;
        case 5: //count
            DATA.Unload();
            return;
        case 6: //pgrp
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
        case 7: //pgag_p
            PGAG.Unload();
            return;
        case 8: //pgrr_p
            PGRR.Unload();
            return;
        case 9: //pgri
            if(ListFieldID == 0) //pgri
                {
                PGRI.clear();
                return;
                }

            if(ListIndex >= PGRI.size())
                return;

            switch(ListFieldID)
                {
                case 1: //point
                    PGRI[ListIndex].point = defaultPGRI.point;
                    return;
                case 2: //unused1
                    PGRI[ListIndex].unused1[0] = defaultPGRI.unused1[0];
                    PGRI[ListIndex].unused1[1] = defaultPGRI.unused1[1];
                    return;
                case 3: //x
                    PGRI[ListIndex].x = defaultPGRI.x;
                    return;
                case 4: //y
                    PGRI[ListIndex].y = defaultPGRI.y;
                    return;
                case 5: //z
                    PGRI[ListIndex].z = defaultPGRI.z;
                    return;
                default:
                    return;
                }
        case 10: //pgrl
            if(ListFieldID == 0) //pgrl
                {
                for(UINT32 x = 0; x < (UINT32)PGRL.size(); x++)
                    delete PGRL[x];
                PGRL.clear();
                return;
                }

            if(ListIndex >= PGRL.size())
                return;

            switch(ListFieldID)
                {
                case 1: //reference
                    PGRL[ListIndex]->points[0] = defaultPGRL.points[0];
                    return;
                case 2: //points
                    reference = PGRL[ListIndex]->points[0];
                    PGRL[ListIndex]->points.clear();
                    PGRL[ListIndex]->points.push_back(reference);
                    return;
                default:
                    return;
                }
        default:
            return;
        }
    }