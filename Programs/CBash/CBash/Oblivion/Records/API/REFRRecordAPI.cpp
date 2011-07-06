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
#include "..\REFRRecord.h"

UINT32 REFRRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 6: //destination
            return FORMID_FIELD;
        case 7: //destinationPosX
            return FLOAT32_FIELD;
        case 8: //destinationPosY
            return FLOAT32_FIELD;
        case 9: //destinationPosZ
            return FLOAT32_FIELD;
        case 10: //destinationRotX
            return RADIAN_FIELD;
        case 11: //destinationRotY
            return RADIAN_FIELD;
        case 12: //destinationRotZ
            return RADIAN_FIELD;
        case 13: //lockLevel
            return UINT8_FIELD;
        case 14: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XLOC.IsLoaded() ? 3: 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 15: //lockKey
            return FORMID_FIELD;
        case 16: //unused2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XLOC.IsLoaded() ? 4: 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 17: //lockFlags
            return UINT8_FLAG_FIELD;
        case 18: //unused3
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XLOC.IsLoaded() ? 3: 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 19: //owner
            return FORMID_FIELD;
        case 20: //rank
            return SINT32_FIELD;
        case 21: //globalVariable
            return FORMID_FIELD;
        case 22: //parent
            return FORMID_FIELD;
        case 23: //parentFlags
            return UINT8_FLAG_FIELD;
        case 24: //unused4
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XESP.IsLoaded() ? 3: 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 25: //target
            return FORMID_FIELD;
        case 26: //seed
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_OR_UINT32_FIELD;
                case 2: //WhichType
                    if(XSED.IsLoaded())
                        return XSED->isOffset ? UINT8_FIELD : UINT32_FIELD;
                    return UINT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 27: //lod1
            return FLOAT32_FIELD;
        case 28: //lod2
            return FLOAT32_FIELD;
        case 29: //lod3
            return FLOAT32_FIELD;
        case 30: //charge
            return FLOAT32_FIELD;
        case 31: //health
            return SINT32_FIELD;
        case 32: //unknownXPCIFormID
            return FORMID_FIELD;
        case 33: //unknownXPCIString
            return ISTRING_FIELD;
        case 34: //levelMod
            return SINT32_FIELD;
        case 35: //unknownXRTMFormID
            return FORMID_FIELD;
        case 36: //actionFlags
            return UINT32_FLAG_FIELD;
        case 37: //count
            return SINT32_FIELD;
        case 38: //markerFlags
            return UINT8_FLAG_FIELD;
        case 39: //markerName
            return STRING_FIELD;
        case 40: //markerType
            return UINT8_TYPE_FIELD;
        case 41: //markerUnused
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return Marker.IsLoaded() ? 1: 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 42: //scale
            return FLOAT32_FIELD;
        case 43: //soulType
            return UINT8_TYPE_FIELD;
        case 44: //posX
            return FLOAT32_FIELD;
        case 45: //posY
            return FLOAT32_FIELD;
        case 46: //posZ
            return FLOAT32_FIELD;
        case 47: //rotX
            return RADIAN_FIELD;
        case 48: //rotY
            return RADIAN_FIELD;
        case 49: //rotZ
            return RADIAN_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * REFRRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 6: //destination
            return XTEL.IsLoaded() ? &XTEL->destinationFid : NULL;
        case 7: //destinationPosX
            return XTEL.IsLoaded() ? &XTEL->destination.posX : NULL;
        case 8: //destinationPosY
            return XTEL.IsLoaded() ? &XTEL->destination.posY : NULL;
        case 9: //destinationPosZ
            return XTEL.IsLoaded() ? &XTEL->destination.posZ : NULL;
        case 10: //destinationRotX
            return XTEL.IsLoaded() ? &XTEL->destination.rotX : NULL;
        case 11: //destinationRotY
            return XTEL.IsLoaded() ? &XTEL->destination.rotY : NULL;
        case 12: //destinationRotZ
            return XTEL.IsLoaded() ? &XTEL->destination.rotZ : NULL;
        case 13: //lockLevel
            return XLOC.IsLoaded() ? &XLOC->level : NULL;
        case 14: //unused1
            *FieldValues = XLOC.IsLoaded() ? &XLOC->unused1[0] : NULL;
            return NULL;
        case 15: //lockKey
            return XLOC.IsLoaded() ? &XLOC->key : NULL;
        case 16: //unused2
            *FieldValues = XLOC.IsLoaded() ? &XLOC->unused2[0] : NULL;
            return NULL;
        case 17: //lockFlags
            return XLOC.IsLoaded() ? &XLOC->flags : NULL;
        case 18: //unused3
            *FieldValues = XLOC.IsLoaded() ? &XLOC->unused3[0] : NULL;
            return NULL;
        case 19: //owner
            return Ownership.IsLoaded() ? &Ownership->XOWN.value.fid : NULL;
        case 20: //rank
            return (Ownership.IsLoaded() && Ownership->XRNK.IsLoaded()) ? &Ownership->XRNK->rank : NULL;
        case 21: //globalVariable
            return (Ownership.IsLoaded() && Ownership->XGLB.IsLoaded()) ? &Ownership->XGLB->fid : NULL;
        case 22: //parent
            return XESP.IsLoaded() ? &XESP->parent : NULL;
        case 23: //parentFlags
            return XESP.IsLoaded() ? &XESP->flags : NULL;
        case 24: //unused4
            *FieldValues = XESP.IsLoaded() ? &XESP->unused1[0] : NULL;
            return NULL;
        case 25: //target
            return XTRG.IsLoaded() ? &XTRG->fid : NULL;
        case 26: //seed
            if(XSED.IsLoaded())
                {
                if(XSED->offset)
                    return &XSED->offset;
                return &XSED->seed;
                }
            return NULL;
        case 27: //lod1
            return XLOD.IsLoaded() ? &XLOD->lod1 : NULL;
        case 28: //lod2
            return XLOD.IsLoaded() ? &XLOD->lod2 : NULL;
        case 29: //lod3
            return XLOD.IsLoaded() ? &XLOD->lod3 : NULL;
        case 30: //charge
            return XCHG.IsLoaded() ? &XCHG->charge : NULL;
        case 31: //health
            return XHLT.IsLoaded() ? &XHLT->health : NULL;
        case 32: //unknownXPCIFormID
            return (XPCI.IsLoaded() && XPCI->XPCI.IsLoaded()) ? &XPCI->XPCI->fid : NULL;
        case 33: //unknownXPCIString
            return XPCI.IsLoaded() ? XPCI->FULL.value : NULL;
        case 34: //levelMod
            return XLCM.IsLoaded() ? &XLCM->levelMod : NULL;
        case 35: //unknownXRTMFormID
            return XRTM.IsLoaded() ? &XRTM->fid : NULL;
        case 36: //actionFlags
            return XACT.IsLoaded() ? &XACT->flags : NULL;
        case 37: //count
            return XCNT.IsLoaded() ? &XCNT->count : NULL;
        case 38: //markerFlags
            return Marker.IsLoaded() ? &Marker->FNAM.value.flags : NULL;
        case 39: //markerName
            return Marker.IsLoaded() ? Marker->FULL.value : NULL;
        case 40: //markerType
            return Marker.IsLoaded() ? &Marker->TNAM.value.markerType : NULL;
        case 41: //markerUnused
            *FieldValues = Marker.IsLoaded() ? &Marker->TNAM.value.unused1 : NULL;
            return NULL;
        case 42: //scale
            return XSCL.IsLoaded() ? &XSCL->scale : NULL;
        case 43: //soulType
            return XSOL.IsLoaded() ? &XSOL->flags : NULL;
        case 44: //posX
            return &DATA.value.posX;
        case 45: //posY
            return &DATA.value.posY;
        case 46: //posZ
            return &DATA.value.posZ;
        case 47: //rotX
            return &DATA.value.rotX;
        case 48: //rotY
            return &DATA.value.rotY;
        case 49: //rotZ
            return &DATA.value.rotZ;
        default:
            return NULL;
        }
    }

bool REFRRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 6: //destination
            XTEL.Load();
            XTEL->destinationFid = *(FORMID *)FieldValue;
            return true;
        case 7: //destinationPosX
            XTEL.Load();
            XTEL->destination.posX = *(FLOAT32 *)FieldValue;
            break;
        case 8: //destinationPosY
            XTEL.Load();
            XTEL->destination.posY = *(FLOAT32 *)FieldValue;
            break;
        case 9: //destinationPosZ
            XTEL.Load();
            XTEL->destination.posZ = *(FLOAT32 *)FieldValue;
            break;
        case 10: //destinationRotX
            XTEL.Load();
            XTEL->destination.rotX = *(FLOAT32 *)FieldValue;
            break;
        case 11: //destinationRotY
            XTEL.Load();
            XTEL->destination.rotY = *(FLOAT32 *)FieldValue;
            break;
        case 12: //destinationRotZ
            XTEL.Load();
            XTEL->destination.rotZ = *(FLOAT32 *)FieldValue;
            break;
        case 13: //lockLevel
            XLOC.Load();
            XLOC->level = *(UINT8 *)FieldValue;
            break;
        case 14: //unused1
            if(ArraySize != 3)
                break;
            XLOC.Load();
            XLOC->unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            XLOC->unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            XLOC->unused1[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        case 15: //lockKey
            XLOC.Load();
            XLOC->key = *(FORMID *)FieldValue;
            return true;
        case 16: //unused2
            if(ArraySize != 4)
                break;
            XLOC.Load();
            XLOC->unused2[0] = ((UINT8ARRAY)FieldValue)[0];
            XLOC->unused2[1] = ((UINT8ARRAY)FieldValue)[1];
            XLOC->unused2[2] = ((UINT8ARRAY)FieldValue)[2];
            XLOC->unused2[3] = ((UINT8ARRAY)FieldValue)[3];
            break;
        case 17: //lockFlags
            SetLockFlagMask(*(UINT8 *)FieldValue);
            break;
        case 18: //unused3
            if(ArraySize != 3)
                break;
            XLOC.Load();
            XLOC->unused3[0] = ((UINT8ARRAY)FieldValue)[0];
            XLOC->unused3[1] = ((UINT8ARRAY)FieldValue)[1];
            XLOC->unused3[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        case 19: //owner
            Ownership.Load();
            Ownership->XOWN.value.fid = *(FORMID *)FieldValue;
            return true;
        case 20: //rank
            Ownership.Load();
            Ownership->XRNK.Load();
            Ownership->XRNK->rank = *(SINT32 *)FieldValue;
            break;
        case 21: //globalVariable
            Ownership.Load();
            Ownership->XGLB.Load();
            Ownership->XGLB->fid = *(FORMID *)FieldValue;
            return true;
        case 22: //parent
            XESP.Load();
            XESP->parent = *(FORMID *)FieldValue;
            return true;
        case 23: //parentFlags
            SetParentFlagMask(*(UINT8 *)FieldValue);
            break;
        case 24: //unused4
            if(ArraySize != 3)
                break;
            XESP.Load();
            XESP->unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            XESP->unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            XESP->unused1[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        case 25: //target
            XTRG.Load();
            XTRG->fid = *(FORMID *)FieldValue;
            return true;
        case 26: //seed
            XSED.Load();
            //Borrowing ArraySize to flag if the new value is an offset
            if(ArraySize)
                {
                XSED->isOffset = true;
                XSED->offset = *(UINT8 *)FieldValue;
                }
            else
                {
                XSED->isOffset = false;
                XSED->seed = *(UINT32 *)FieldValue;
                }
            break;
        case 27: //lod1
            XLOD.Load();
            XLOD->lod1 = *(FLOAT32 *)FieldValue;
            break;
        case 28: //lod2
            XLOD.Load();
            XLOD->lod2 = *(FLOAT32 *)FieldValue;
            break;
        case 29: //lod3
            XLOD.Load();
            XLOD->lod3 = *(FLOAT32 *)FieldValue;
            break;
        case 30: //charge
            XCHG.Load();
            XCHG->charge = *(FLOAT32 *)FieldValue;
            break;
        case 31: //health
            XHLT.Load();
            XHLT->health = *(SINT32 *)FieldValue;
            break;
        case 32: //unknownXPCIFormID
            XPCI.Load();
            XPCI->XPCI.Load();
            XPCI->XPCI->fid = *(FORMID *)FieldValue;
            return true;
        case 33: //unknownXPCIString
            XPCI.Load();
            XPCI->FULL.Copy((STRING)FieldValue);
            break;
        case 34: //levelMod
            XLCM.Load();
            XLCM->levelMod = *(SINT32 *)FieldValue;
            break;
        case 35: //unknownXRTMFormID
            XRTM.Load();
            XRTM->fid = *(FORMID *)FieldValue;
            return true;
        case 36: //actionFlags
            SetActionFlagMask(*(UINT32 *)FieldValue);
            break;
        case 37: //count
            XCNT.Load();
            XCNT->count = *(SINT32 *)FieldValue;
            break;
        case 38: //markerFlags
            SetMapFlagMask(*(UINT8 *)FieldValue);
            break;
        case 39: //markerName
            Marker.Load();
            Marker->FULL.Copy((STRING)FieldValue);
            break;
        case 40: //markerType
            SetMarkerType(*(UINT8 *)FieldValue);
            break;
        case 41: //markerUnused
            if(ArraySize != 1)
                break;
            Marker.Load();
            Marker->TNAM.value.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 42: //scale
            XSCL.Load();
            XSCL->scale = *(FLOAT32 *)FieldValue;
            break;
        case 43: //soulType
            SetSoul(*(UINT8 *)FieldValue);
            break;
        case 44: //posX
            DATA.value.posX = *(FLOAT32 *)FieldValue;
            break;
        case 45: //posY
            DATA.value.posY = *(FLOAT32 *)FieldValue;
            break;
        case 46: //posZ
            DATA.value.posZ = *(FLOAT32 *)FieldValue;
            break;
        case 47: //rotX
            DATA.value.rotX = *(FLOAT32 *)FieldValue;
            break;
        case 48: //rotY
            DATA.value.rotY = *(FLOAT32 *)FieldValue;
            break;
        case 49: //rotZ
            DATA.value.rotZ = *(FLOAT32 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void REFRRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    REFRXTEL defaultXTEL;
    REFRXLOC defaultXLOC;
    GENXESP defaultXESP;
    GENXLOD defaultXLOD;
    REFRTNAM defaultTNAM;
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
        case 6: //destination
            if(XTEL.IsLoaded())
                XTEL->destinationFid = defaultXTEL.destinationFid;
            return;
        case 7: //destinationPosX
            if(XTEL.IsLoaded())
                XTEL->destination.posX = defaultXTEL.destination.posX;
            return;
        case 8: //destinationPosY
            if(XTEL.IsLoaded())
                XTEL->destination.posY = defaultXTEL.destination.posY;
            return;
        case 9: //destinationPosZ
            if(XTEL.IsLoaded())
                XTEL->destination.posZ = defaultXTEL.destination.posZ;
            return;
        case 10: //destinationRotX
            if(XTEL.IsLoaded())
                XTEL->destination.rotX = defaultXTEL.destination.rotX;
            return;
        case 11: //destinationRotY
            if(XTEL.IsLoaded())
                XTEL->destination.rotY = defaultXTEL.destination.rotY;
            return;
        case 12: //destinationRotZ
            if(XTEL.IsLoaded())
                XTEL->destination.rotZ = defaultXTEL.destination.rotZ;
            return;
        case 13: //lockLevel
            if(XLOC.IsLoaded())
                XLOC->level = defaultXLOC.level;
            return;
        case 14: //unused1
            if(XLOC.IsLoaded())
                {
                XLOC->unused1[0] = defaultXLOC.unused1[0];
                XLOC->unused1[1] = defaultXLOC.unused1[1];
                XLOC->unused1[2] = defaultXLOC.unused1[2];
                }
            return;
        case 15: //lockKey
            if(XLOC.IsLoaded())
                XLOC->key = defaultXLOC.key;
            return;
        case 16: //unused2
            if(XLOC.IsLoaded())
                {
                XLOC->unused2[0] = defaultXLOC.unused2[0];
                XLOC->unused2[1] = defaultXLOC.unused2[1];
                XLOC->unused2[2] = defaultXLOC.unused2[2];
                XLOC->unused2[3] = defaultXLOC.unused2[3];
                }
            return;
        case 17: //lockFlags
            if(XLOC.IsLoaded())
                XLOC->flags = defaultXLOC.flags;
            return;
        case 18: //unused3
            if(XLOC.IsLoaded())
                {
                XLOC->unused3[0] = defaultXLOC.unused3[0];
                XLOC->unused3[1] = defaultXLOC.unused3[1];
                XLOC->unused3[2] = defaultXLOC.unused3[2];
                }
            return;
        case 19: //owner
            if(Ownership.IsLoaded())
                Ownership->XOWN.Unload();
            return;
        case 20: //rank
            if(Ownership.IsLoaded())
                Ownership->XRNK.Unload();
            return;
        case 21: //globalVariable
            if(Ownership.IsLoaded())
                Ownership->XGLB.Unload();
            return;
        case 22: //parent
            if(XESP.IsLoaded())
                XESP->parent = defaultXESP.parent;
            return;
        case 23: //parentFlags
            if(XESP.IsLoaded())
                XESP->flags = defaultXESP.flags;
            return;
        case 24: //unused4
            if(XESP.IsLoaded())
                {
                XESP->unused1[0] = defaultXESP.unused1[0];
                XESP->unused1[1] = defaultXESP.unused1[1];
                XESP->unused1[2] = defaultXESP.unused1[2];
                }
            return;
        case 25: //target
            XTRG.Unload();
            return;
        case 26: //seed
            XSED.Unload();
            return;
        case 27: //lod1
            if(XLOD.IsLoaded())
                XLOD->lod1 = defaultXLOD.lod1;
            return;
        case 28: //lod2
            if(XLOD.IsLoaded())
                XLOD->lod2 = defaultXLOD.lod2;
            return;
        case 29: //lod3
            if(XLOD.IsLoaded())
                XLOD->lod3 = defaultXLOD.lod3;
            return;
        case 30: //charge
            XCHG.Unload();
            return;
        case 31: //health
            XHLT.Unload();
            return;
        case 32: //unknownXPCIFormID
            if(XPCI.IsLoaded())
                XPCI->XPCI.Unload();
            return;
        case 33: //unknownXPCIString
            if(XPCI.IsLoaded())
                XPCI->FULL.Unload();
            return;
        case 34: //levelMod
            XLCM.Unload();
            return;
        case 35: //unknownXRTMFormID
            XRTM.Unload();
            return;
        case 36: //actionFlags
            XACT.Unload();
            return;
        case 37: //count
            XCNT.Unload();
            return;
        case 38: //markerFlags
            if(Marker.IsLoaded())
                Marker->FNAM.Unload();
            return;
        case 39: //markerName
            if(Marker.IsLoaded())
                Marker->FULL.Unload();
            return;
        case 40: //markerType
            if(Marker.IsLoaded())
                Marker->TNAM.value.markerType = defaultTNAM.markerType;
            return;
        case 41: //markerUnused
            if(Marker.IsLoaded())
                Marker->TNAM.value.unused1 = defaultTNAM.unused1;
            return;
        case 42: //scale
            XSCL.Unload();
            return;
        case 43: //soulType
            XSOL.Unload();
            return;
        case 44: //posX
            DATA.value.posX = defaultDATA.posX;
            return;
        case 45: //posY
            DATA.value.posY = defaultDATA.posY;
            return;
        case 46: //posZ
            DATA.value.posZ = defaultDATA.posZ;
            return;
        case 47: //rotX
            DATA.value.rotX = defaultDATA.rotX;
            return;
        case 48: //rotY
            DATA.value.rotY = defaultDATA.rotY;
            return;
        case 49: //rotZ
            DATA.value.rotZ = defaultDATA.rotZ;
            return;
        default:
            return;
        }
    return;
    }