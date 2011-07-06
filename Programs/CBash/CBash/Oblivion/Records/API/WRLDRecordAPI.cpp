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
#include "..\WRLDRecord.h"

UINT32 WRLDRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //parent
            return FORMID_FIELD;
        case 7: //climate
            return FORMID_FIELD;
        case 8: //water
            return FORMID_FIELD;
        case 9: //mapPath
            return ISTRING_FIELD;
        case 10: //dimX
            return SINT32_FIELD;
        case 11: //dimY
            return SINT32_FIELD;
        case 12: //NWCellX
            return SINT16_FIELD;
        case 13: //NWCellY
            return SINT16_FIELD;
        case 14: //SECellX
            return SINT16_FIELD;
        case 15: //SECellY
            return SINT16_FIELD;
        case 16: //flags
            return UINT8_FLAG_FIELD;
        case 17: //unknown00
            return FLOAT32_FIELD;
        case 18: //unknown01
            return FLOAT32_FIELD;
        case 19: //unknown90
            return FLOAT32_FIELD;
        case 20: //unknown91
            return FLOAT32_FIELD;
        case 21: //soundType
            return UINT32_FIELD;
        case 22: //ofst_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return OFST.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 23: //ROAD
            return SUBRECORD_FIELD;
        case 24: //CELL
            return SUBRECORD_FIELD;
        case 25: //CELLS
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)CELLS.size();
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * WRLDRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //parent
            return WNAM.IsLoaded() ? &WNAM->fid : NULL;
        case 7: //climate
            return CNAM.IsLoaded() ? &CNAM->fid : NULL;
        case 8: //water
            return NAM2.IsLoaded() ? &NAM2->fid : NULL;
        case 9: //mapPath
            return ICON.value;
        case 10: //dimX
            return MNAM.IsLoaded() ? &MNAM->dimX : NULL;
        case 11: //dimY
            return MNAM.IsLoaded() ? &MNAM->dimY : NULL;
        case 12: //NWCellX
            return MNAM.IsLoaded() ? &MNAM->NWCellX : NULL;
        case 13: //NWCellY
            return MNAM.IsLoaded() ? &MNAM->NWCellY : NULL;
        case 14: //SECellX
            return MNAM.IsLoaded() ? &MNAM->SECellX : NULL;
        case 15: //SECellY
            return MNAM.IsLoaded() ? &MNAM->SECellY : NULL;
        case 16: //flags
            return &DATA.value.flags;
        case 17: //unknown00
            return &NAM0.value.unk1;
        case 18: //unknown01
            return &NAM0.value.unk2;
        case 19: //unknown90
            return &NAM9.value.unk1;
        case 20: //unknown91
            return &NAM9.value.unk2;
        case 21: //soundType
            return SNAM.IsLoaded() ? &SNAM->flags : NULL;
        case 22: //ofst_p
            *FieldValues = OFST.value;
            return NULL;
        case 23: //ROAD
            return ROAD;
        case 24: //CELL
            return CELL;
        case 25: //CELLS
            for(UINT32 p = 0;p < (UINT32)CELLS.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = CELLS[p];
            return NULL;
        default:
            return NULL;
        }
    }

bool WRLDRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //parent
            WNAM.Load();
            WNAM->fid = *(FORMID *)FieldValue;
            return true;
        case 7: //climate
            CNAM.Load();
            CNAM->fid = *(FORMID *)FieldValue;
            return true;
        case 8: //water
            NAM2.Load();
            NAM2->fid = *(FORMID *)FieldValue;
            return true;
        case 9: //mapPath
            ICON.Copy((STRING)FieldValue);
            break;
        case 10: //dimX
            MNAM.Load();
            MNAM->dimX = *(SINT32 *)FieldValue;
            break;
        case 11: //dimY
            MNAM.Load();
            MNAM->dimY = *(SINT32 *)FieldValue;
            break;
        case 12: //NWCellX
            MNAM.Load();
            MNAM->NWCellX = *(SINT16 *)FieldValue;
            break;
        case 13: //NWCellY
            MNAM.Load();
            MNAM->NWCellY = *(SINT16 *)FieldValue;
            break;
        case 14: //SECellX
            MNAM.Load();
            MNAM->SECellX = *(SINT16 *)FieldValue;
            break;
        case 15: //SECellY
            MNAM.Load();
            MNAM->SECellY = *(SINT16 *)FieldValue;
            break;
        case 16: //flags
            DATA.value.flags = *(UINT8 *)FieldValue;
            break;
        case 17: //unknown00
            NAM0.value.unk1 = *(FLOAT32 *)FieldValue;
            break;
        case 18: //unknown01
            NAM0.value.unk2 = *(FLOAT32 *)FieldValue;
            break;
        case 19: //unknown90
            NAM9.value.unk1 = *(FLOAT32 *)FieldValue;
            break;
        case 20: //unknown91
            NAM9.value.unk2 = *(FLOAT32 *)FieldValue;
            break;
        case 21: //musicType
            SetMusicType(*(UINT32 *)FieldValue);
            break;
        case 22: //ofst_p
            OFST.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        default:
            break;
        }
    return false;
    }

void WRLDRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    WRLDMNAM defaultMNAM;
    WRLDUNK defaultUNK;

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
        case 6: //parent
            WNAM.Unload();
            return;
        case 7: //climate
            CNAM.Unload();
            return;
        case 8: //water
            NAM2.Unload();
            return;
        case 9: //mapPath
            ICON.Unload();
            return;
        case 10: //dimX
            if(MNAM.IsLoaded())
                MNAM->dimX = defaultMNAM.dimX;
            return;
        case 11: //dimY
            if(MNAM.IsLoaded())
                MNAM->dimY = defaultMNAM.dimY;
            return;
        case 12: //NWCellX
            if(MNAM.IsLoaded())
                MNAM->NWCellX = defaultMNAM.NWCellX;
            return;
        case 13: //NWCellY
            if(MNAM.IsLoaded())
                MNAM->NWCellY = defaultMNAM.NWCellY;
            return;
        case 14: //SECellX
            if(MNAM.IsLoaded())
                MNAM->SECellX = defaultMNAM.SECellX;
            return;
        case 15: //SECellY
            if(MNAM.IsLoaded())
                MNAM->SECellY = defaultMNAM.SECellY;
            return;
        case 16: //flags
            DATA.Unload();
            return;
        case 17: //unknown00
            NAM0.value.unk1 = defaultUNK.unk1;
            return;
        case 18: //unknown01
            NAM0.value.unk2 = defaultUNK.unk2;
            return;
        case 19: //unknown90
            NAM9.value.unk1 = defaultUNK.unk1;
            return;
        case 20: //unknown91
            NAM9.value.unk2 = defaultUNK.unk2;
            return;
        case 21: //soundType
            SNAM.Unload();
            return;
        case 22: //ofst_p
            OFST.Unload();
            return;
        default:
            return;
        }
    return;
    }