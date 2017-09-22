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
#include "..\CELLRecord.h"

UINT32 CELLRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //flags
            return UINT8_FLAG_FIELD;
        case 7: //ambientRed
            return UINT8_FIELD;
        case 8: //ambientGreen
            return UINT8_FIELD;
        case 9: //ambientBlue
            return UINT8_FIELD;
        case 10: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XCLL.IsLoaded() ? 1 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 11: //directionalRed
            return UINT8_FIELD;
        case 12: //directionalGreen
            return UINT8_FIELD;
        case 13: //directionalBlue
            return UINT8_FIELD;
        case 14: //unused2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XCLL.IsLoaded() ? 1 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 15: //fogRed
            return UINT8_FIELD;
        case 16: //fogGreen
            return UINT8_FIELD;
        case 17: //fogBlue
            return UINT8_FIELD;
        case 18: //unused3
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XCLL.IsLoaded() ? 1 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 19: //fogNear
            return FLOAT32_FIELD;
        case 20: //fogFar
            return FLOAT32_FIELD;
        case 21: //directionalXY
            return SINT32_FIELD;
        case 22: //directionalZ
            return SINT32_FIELD;
        case 23: //directionalFade
            return FLOAT32_FIELD;
        case 24: //fogClip
            return FLOAT32_FIELD;
        case 25: //musicType
            return UINT8_TYPE_FIELD;
        case 26: //owner
            return FORMID_FIELD;
        case 27: //rank
            return SINT32_FIELD;
        case 28: //globalVariable
            return FORMID_FIELD;
        case 29: //climate
            return FORMID_FIELD;
        case 30: //waterHeight
            return FLOAT32_FIELD;
        case 31: //regions
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)XCLR.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 32: //posX
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UNKNOWN_OR_SINT32_FIELD;
                case 2: //WhichType
                    if(!IsInterior())
                        return SINT32_FIELD;
                    return UNKNOWN_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 33: //posY
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UNKNOWN_OR_SINT32_FIELD;
                case 2: //WhichType
                    if(!IsInterior())
                        return SINT32_FIELD;
                    return UNKNOWN_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 34: //water
            return FORMID_FIELD;
        case 35: //ACHR
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)ACHR.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 36: //ACRE
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)ACRE.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 37: //REFR
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)REFR.size();
                default:
                    return UNKNOWN_FIELD;
                }
        case 38: //PGRD
            return SUBRECORD_FIELD;
        case 39: //LAND
            return SUBRECORD_FIELD;
        case 40: //Parent
            return PARENTRECORD_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * CELLRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //flags
            return &DATA.value.flags;
        case 7: //ambientRed
            return XCLL.IsLoaded() ? &XCLL->ambient.red : NULL;
        case 8: //ambientGreen
            return XCLL.IsLoaded() ? &XCLL->ambient.green : NULL;
        case 9: //ambientBlue
            return XCLL.IsLoaded() ? &XCLL->ambient.blue : NULL;
        case 10: //unused1
            *FieldValues = XCLL.IsLoaded() ? &XCLL->ambient.unused1 : NULL;
            return NULL;
        case 11: //directionalRed
            return XCLL.IsLoaded() ? &XCLL->directional.red : NULL;
        case 12: //directionalGreen
            return XCLL.IsLoaded() ? &XCLL->directional.green : NULL;
        case 13: //directionalBlue
            return XCLL.IsLoaded() ? &XCLL->directional.blue : NULL;
        case 14: //unused2
            *FieldValues = XCLL.IsLoaded() ? &XCLL->directional.unused1 : NULL;
            return NULL;
        case 15: //fogRed
            return XCLL.IsLoaded() ? &XCLL->fog.red : NULL;
        case 16: //fogGreen
            return XCLL.IsLoaded() ? &XCLL->fog.green : NULL;
        case 17: //fogBlue
            return XCLL.IsLoaded() ? &XCLL->fog.blue : NULL;
        case 18: //unused3
            *FieldValues = XCLL.IsLoaded() ? &XCLL->fog.unused1 : NULL;
            return NULL;
        case 19: //fogNear
            return XCLL.IsLoaded() ? &XCLL->fogNear : NULL;
        case 20: //fogFar
            return XCLL.IsLoaded() ? &XCLL->fogFar : NULL;
        case 21: //directionalXY
            return XCLL.IsLoaded() ? &XCLL->directionalXY : NULL;
        case 22: //directionalZ
            return XCLL.IsLoaded() ? &XCLL->directionalZ : NULL;
        case 23: //directionalFade
            return XCLL.IsLoaded() ? &XCLL->directionalFade : NULL;
        case 24: //fogClip
            return XCLL.IsLoaded() ? &XCLL->fogClip : NULL;
        case 25: //musicType
            return &XCMT.value.flags;
        case 26: //owner
            return Ownership.IsLoaded() ? &Ownership->XOWN.value.fid : NULL;
        case 27: //rank
            return (Ownership.IsLoaded() && Ownership->XRNK.IsLoaded()) ? &Ownership->XRNK->rank : NULL;
        case 28: //globalVariable
            return (Ownership.IsLoaded() && Ownership->XGLB.IsLoaded()) ? &Ownership->XGLB->fid : NULL;
        case 29: //climate
            return &XCCM.value.fid;
        case 30: //waterHeight
            return &XCLW.value.waterHeight;
        case 31: //regions
            *FieldValues = &XCLR[0];
            return NULL;
        case 32: //posX
            return (!IsInterior() & XCLC.IsLoaded()) ? &XCLC->posX : NULL;
        case 33: //posY
            return (!IsInterior() & XCLC.IsLoaded()) ? &XCLC->posY : NULL;
        case 34: //water
            return &XCWT.value.fid;
        case 35: //ACHR
            for(UINT32 p = 0;p < (UINT32)ACHR.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = ACHR[p];
            return NULL;
        case 36: //ACRE
            for(UINT32 p = 0;p < (UINT32)ACRE.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = ACRE[p];
            return NULL;
        case 37: //REFR
            for(UINT32 p = 0;p < (UINT32)REFR.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = REFR[p];
            return NULL;
        case 38: //PGRD
            return PGRD;
        case 39: //LAND
            return LAND;
        case 40: //Parent
            return Parent;
        default:
            return NULL;
        }
    }

bool CELLRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //flags
            SetFlagMask(*(UINT8 *)FieldValue);
            break;
        case 7: //ambientRed
            XCLL.Load();
            XCLL->ambient.red = *(UINT8 *)FieldValue;
            break;
        case 8: //ambientGreen
            XCLL.Load();
            XCLL->ambient.green = *(UINT8 *)FieldValue;
            break;
        case 9: //ambientBlue
            XCLL.Load();
            XCLL->ambient.blue = *(UINT8 *)FieldValue;
            break;
        case 10: //unused1
            if(ArraySize != 1)
                break;
            XCLL.Load();
            XCLL->ambient.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 11: //directionalRed
            XCLL.Load();
            XCLL->directional.red = *(UINT8 *)FieldValue;
            break;
        case 12: //directionalGreen
            XCLL.Load();
            XCLL->directional.green = *(UINT8 *)FieldValue;
            break;
        case 13: //directionalBlue
            XCLL.Load();
            XCLL->directional.blue = *(UINT8 *)FieldValue;
            break;
        case 14: //unused2
            if(ArraySize != 1)
                break;
            XCLL.Load();
            XCLL->directional.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 15: //fogRed
            XCLL.Load();
            XCLL->fog.red = *(UINT8 *)FieldValue;
            break;
        case 16: //fogGreen
            XCLL.Load();
            XCLL->fog.green = *(UINT8 *)FieldValue;
            break;
        case 17: //fogBlue
            XCLL.Load();
            XCLL->fog.blue = *(UINT8 *)FieldValue;
            break;
        case 18: //unused3
            if(ArraySize != 1)
                break;
            XCLL.Load();
            XCLL->fog.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 19: //fogNear
            XCLL.Load();
            XCLL->fogNear = *(FLOAT32 *)FieldValue;
            break;
        case 20: //fogFar
            XCLL.Load();
            XCLL->fogFar = *(FLOAT32 *)FieldValue;
            break;
        case 21: //directionalXY
            XCLL.Load();
            XCLL->directionalXY = *(SINT32 *)FieldValue;
            break;
        case 22: //directionalZ
            XCLL.Load();
            XCLL->directionalZ = *(SINT32 *)FieldValue;
            break;
        case 23: //directionalFade
            XCLL.Load();
            XCLL->directionalFade = *(FLOAT32 *)FieldValue;
            break;
        case 24: //fogClip
            XCLL.Load();
            XCLL->fogClip = *(FLOAT32 *)FieldValue;
            break;
        case 25: //musicType
            SetMusicType(*(UINT8 *)FieldValue);
            break;
        case 26: //owner
            Ownership.Load();
            Ownership->XOWN.value.fid = *(FORMID *)FieldValue;
            return true;
        case 27: //rank
            Ownership.Load();
            Ownership->XRNK.Load();
            Ownership->XRNK->rank = *(SINT32 *)FieldValue;
            break;
        case 28: //globalVariable
            Ownership.Load();
            Ownership->XGLB.Load();
            Ownership->XGLB->fid = *(FORMID *)FieldValue;
            return true;
        case 29: //climate
            XCCM.Load();
            XCCM.value.fid = *(FORMID *)FieldValue;
            return true;
        case 30: //waterHeight
            XCLW.Load();
            XCLW.value.waterHeight = *(FLOAT32 *)FieldValue;
            break;
        case 31: //regions
            XCLR.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; ++x)
                XCLR[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 32: //posX
            if(IsInterior())
                break;
            XCLC.Load();
            XCLC->posX = *(SINT32 *)FieldValue;
            break;
        case 33: //posY
            if(IsInterior())
                break;
            XCLC.Load();
            XCLC->posY = *(SINT32 *)FieldValue;
            break;
        case 34: //water
            XCWT.Load();
            XCWT.value.fid = *(FORMID *)FieldValue;
            return true;
        default:
            break;
        }
    return false;
    }

void CELLRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    CELLXCLL defaultXCLL;
    CELLXCLC defaultXCLC;

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
        case 6: //flags
            DATA.Unload();
            return;
        case 7: //ambientRed
            if(XCLL.IsLoaded())
                XCLL->ambient.red = defaultXCLL.ambient.red;
            return;
        case 8: //ambientGreen
            if(XCLL.IsLoaded())
                XCLL->ambient.green = defaultXCLL.ambient.green;
            return;
        case 9: //ambientBlue
            if(XCLL.IsLoaded())
                XCLL->ambient.blue = defaultXCLL.ambient.blue;
            return;
        case 10: //unused1
            if(XCLL.IsLoaded())
                XCLL->ambient.unused1 = defaultXCLL.ambient.unused1;
            return;
        case 11: //directionalRed
            if(XCLL.IsLoaded())
                XCLL->directional.red = defaultXCLL.directional.red;
            return;
        case 12: //directionalGreen
            if(XCLL.IsLoaded())
                XCLL->directional.green = defaultXCLL.directional.green;
            return;
        case 13: //directionalBlue
            if(XCLL.IsLoaded())
                XCLL->directional.blue = defaultXCLL.directional.blue;
            return;
        case 14: //unused2
            if(XCLL.IsLoaded())
                XCLL->directional.unused1 = defaultXCLL.directional.unused1;
            return;
        case 15: //fogRed
            if(XCLL.IsLoaded())
                XCLL->fog.red = defaultXCLL.fog.red;
            return;
        case 16: //fogGreen
            if(XCLL.IsLoaded())
                XCLL->fog.green = defaultXCLL.fog.green;
            return;
        case 17: //fogBlue
            if(XCLL.IsLoaded())
                XCLL->fog.blue = defaultXCLL.fog.blue;
            return;
        case 18: //unused3
            if(XCLL.IsLoaded())
                XCLL->fog.unused1 = defaultXCLL.fog.unused1;
            return;
        case 19: //fogNear
            if(XCLL.IsLoaded())
                XCLL->fogNear = defaultXCLL.fogNear;
            return;
        case 20: //fogFar
            if(XCLL.IsLoaded())
                XCLL->fogFar = defaultXCLL.fogFar;
            return;
        case 21: //directionalXY
            if(XCLL.IsLoaded())
                XCLL->directionalXY = defaultXCLL.directionalXY;
            return;
        case 22: //directionalZ
            if(XCLL.IsLoaded())
                XCLL->directionalZ = defaultXCLL.directionalZ;
            return;
        case 23: //directionalFade
            if(XCLL.IsLoaded())
                XCLL->directionalFade = defaultXCLL.directionalFade;
            return;
        case 24: //fogClip
            if(XCLL.IsLoaded())
                XCLL->fogClip = defaultXCLL.fogClip;
            return;
        case 25: //musicType
            XCMT.Unload();
            return;
        case 26: //owner
            if(Ownership.IsLoaded())
                Ownership->XOWN.Unload();
            return;
        case 27: //rank
            if(Ownership.IsLoaded())
                Ownership->XRNK.Unload();
            return;
        case 28: //globalVariable
            if(Ownership.IsLoaded())
                Ownership->XGLB.Unload();
            return;
        case 29: //climate
            XCCM.Unload();
            return;
        case 30: //waterHeight
            XCLW.Unload();
            return;
        case 31: //regions
            XCLR.clear();
            return;
        case 32: //posX
            if(!IsInterior() && XCLC.IsLoaded())
                XCLC->posX = defaultXCLC.posX;
            return;
        case 33: //posY
            if(!IsInterior() && XCLC.IsLoaded())
                XCLC->posY = defaultXCLC.posY;
            return;
        case 34: //water
            XCWT.Unload();
            return;
        default:
            return;
        }
    }