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
#include "..\ACHRRecord.h"

UINT32 ACHRRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //base
            return FORMID_FIELD;
        case 6: //unknownXPCIFormID
            return FORMID_FIELD;
        case 7: //unknownXPCIString
            return ISTRING_FIELD;
        case 8: //lod1
            return FLOAT32_FIELD;
        case 9: //lod2
            return FLOAT32_FIELD;
        case 10: //lod3
            return FLOAT32_FIELD;
        case 11: //parent
            return FORMID_FIELD;
        case 12: //parentFlags
            return UINT8_FLAG_FIELD;
        case 13: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XESP.IsLoaded() ? 3 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 14: //merchantContainer
            return FORMID_FIELD;
        case 15: //horse
            return FORMID_FIELD;
        case 16: //xrgd_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XRGD.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 17: //scale
            return FLOAT32_FIELD;
        case 18: //posX
            return FLOAT32_FIELD;
        case 19: //posY
            return FLOAT32_FIELD;
        case 20: //posZ
            return FLOAT32_FIELD;
        case 21: //rotX
            return RADIAN_FIELD;
        case 22: //rotY
            return RADIAN_FIELD;
        case 23: //rotZ
            return RADIAN_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * ACHRRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //base
            return &NAME.value.fid;
        case 6: //unknownXPCIFormID
            return (XPCI.IsLoaded() && XPCI->XPCI.IsLoaded()) ? &XPCI->XPCI->fid : NULL;
        case 7: //unknownXPCIString
            return XPCI.IsLoaded() ? XPCI->FULL.value : NULL;
        case 8: //lod1
            return XLOD.IsLoaded() ? &XLOD->lod1 : NULL;
        case 9: //lod2
            return XLOD.IsLoaded() ? &XLOD->lod2 : NULL;
        case 10: //lod3
            return XLOD.IsLoaded() ? &XLOD->lod3 : NULL;
        case 11: //parent
            return XESP.IsLoaded() ? &XESP->parent : NULL;
        case 12: //parentFlags
            return XESP.IsLoaded() ? &XESP->flags : NULL;
        case 13: //unused1
            *FieldValues = XESP.IsLoaded() ? &XESP->unused1[0] : NULL;
            return NULL;
        case 14: //merchantContainer
            return XMRC.IsLoaded() ? &XMRC->fid : NULL;
        case 15: //horse
            return &XHRS.value.fid;
        case 16: //xrgd_p
            *FieldValues = XRGD.value;
            return NULL;
        case 17: //scale
            return XSCL.IsLoaded() ? &XSCL->scale : NULL;
        case 18: //posX
            return &DATA.value.posX;
        case 19: //posY
            return &DATA.value.posY;
        case 20: //posZ
            return &DATA.value.posZ;
        case 21: //rotX
            return &DATA.value.rotX;
        case 22: //rotY
            return &DATA.value.rotY;
        case 23: //rotZ
            return &DATA.value.rotZ;
        default:
            return NULL;
        }
    }

bool ACHRRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //base
            NAME.value.fid = *(FORMID *)FieldValue;
            return true;
        case 6: //unknownXPCIFormID
            XPCI.Load();
            XPCI->XPCI.Load();
            XPCI->XPCI->fid = *(FORMID *)FieldValue;
            return true;
        case 7: //unknownXPCIString
            XPCI.Load();
            XPCI->FULL.Copy((STRING)FieldValue);
            break;
        case 8: //lod1
            XLOD.Load();
            XLOD->lod1 = *(FLOAT32 *)FieldValue;
            break;
        case 9: //lod2
            XLOD.Load();
            XLOD->lod2 = *(FLOAT32 *)FieldValue;
            break;
        case 10: //lod3
            XLOD.Load();
            XLOD->lod3 = *(FLOAT32 *)FieldValue;
            break;
        case 11: //parent
            XESP.Load();
            XESP->parent = *(FORMID *)FieldValue;
            return true;
        case 12: //parentFlags
            SetFlagMask(*(UINT8 *)FieldValue);
            break;
        case 13: //unused1
            if(ArraySize != 3)
                break;
            XESP.Load();
            XESP->unused1[0] = ((UINT8 *)FieldValue)[0];
            XESP->unused1[1] = ((UINT8 *)FieldValue)[1];
            XESP->unused1[2] = ((UINT8 *)FieldValue)[2];
            break;
        case 14: //merchantContainer
            XMRC.Load();
            XMRC->fid = *(FORMID *)FieldValue;
            return true;
        case 15: //horse
            XHRS.Load();
            XHRS.value.fid = *(FORMID *)FieldValue;
            return true;
        case 16: //xrgd_p
            XRGD.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 17: //scale
            XSCL.Load();
            XSCL->scale = *(FLOAT32 *)FieldValue;
            break;
        case 18: //posX
            DATA.value.posX = *(FLOAT32 *)FieldValue;
            break;
        case 19: //posY
            DATA.value.posY = *(FLOAT32 *)FieldValue;
            break;
        case 20: //posZ
            DATA.value.posZ = *(FLOAT32 *)FieldValue;
            break;
        case 21: //rotX
            DATA.value.rotX = *(FLOAT32 *)FieldValue;
            break;
        case 22: //rotY
            DATA.value.rotY = *(FLOAT32 *)FieldValue;
            break;
        case 23: //rotZ
            DATA.value.rotZ = *(FLOAT32 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void ACHRRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    GENXESP defaultXESP;
    GENXLOD defaultXLOD;
    GENPOSDATA defaultDATA;

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
        case 5: //base
            NAME.Unload();
            return;
        case 6: //unknownXPCIFormID
            if(XPCI.IsLoaded())
                XPCI->XPCI.Unload();
            return;
        case 7: //unknownXPCIString
            if(XPCI.IsLoaded())
                XPCI->FULL.Unload();
            return;
        case 8: //lod1
            if(XLOD.IsLoaded())
                XLOD->lod1 = defaultXLOD.lod1;
            return;
        case 9: //lod2
            if(XLOD.IsLoaded())
                XLOD->lod2 = defaultXLOD.lod2;
            return;
        case 10: //lod3
            if(XLOD.IsLoaded())
                XLOD->lod3 = defaultXLOD.lod3;
            return;
        case 11: //parent
            if(XESP.IsLoaded())
                XESP->parent = defaultXESP.parent;
            return;
        case 12: //parentFlags
            if(XESP.IsLoaded())
                XESP->flags = defaultXESP.flags;
            return;
        case 13: //unused1
            if(XESP.IsLoaded())
                {
                XESP->unused1[0] = defaultXESP.unused1[0];
                XESP->unused1[1] = defaultXESP.unused1[1];
                XESP->unused1[2] = defaultXESP.unused1[2];
                }
            return;
        case 14: //merchantContainer
            XMRC.Unload();
            return;
        case 15: //horse
            XHRS.Unload();
            return;
        case 16: //xrgd_p
            XRGD.Unload();
            return;
        case 17: //scale
            XSCL.Unload();
            return;
        case 18: //posX
            DATA.value.posX = defaultDATA.posX;
            return;
        case 19: //posY
            DATA.value.posY = defaultDATA.posY;
            return;
        case 20: //posZ
            DATA.value.posZ = defaultDATA.posZ;
            return;
        case 21: //rotX
            DATA.value.rotX = defaultDATA.rotX;
            return;
        case 22: //rotY
            DATA.value.rotY = defaultDATA.rotY;
            return;
        case 23: //rotZ
            DATA.value.rotZ = defaultDATA.rotZ;
            return;
        default:
            return;
        }
    }