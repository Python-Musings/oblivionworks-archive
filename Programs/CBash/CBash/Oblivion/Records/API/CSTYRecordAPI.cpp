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
#include "..\CSTYRecord.h"

UINT32 CSTYRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
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
        case 5: //dodgeChance
            return UINT8_FIELD;
        case 6: //lrChance
            return UINT8_FIELD;
        case 7: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 8: //lrTimerMin
            return FLOAT32_FIELD;
        case 9: //lrTimerMax
            return FLOAT32_FIELD;
        case 10: //forTimerMin
            return FLOAT32_FIELD;
        case 11: //forTimerMax
            return FLOAT32_FIELD;
        case 12: //backTimerMin
            return FLOAT32_FIELD;
        case 13: //backTimerMax
            return FLOAT32_FIELD;
        case 14: //idleTimerMin
            return FLOAT32_FIELD;
        case 15: //idleTimerMax
            return FLOAT32_FIELD;
        case 16: //blkChance
            return UINT8_FIELD;
        case 17: //atkChance
            return UINT8_FIELD;
        case 18: //unused2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 19: //atkBRecoil
            return FLOAT32_FIELD;
        case 20: //atkBUnc
            return FLOAT32_FIELD;
        case 21: //atkBh2h
            return FLOAT32_FIELD;
        case 22: //pAtkChance
            return UINT8_FIELD;
        case 23: //unused3
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 3;
                default:
                    return UNKNOWN_FIELD;
                }
        case 24: //pAtkBRecoil
            return FLOAT32_FIELD;
        case 25: //pAtkBUnc
            return FLOAT32_FIELD;
        case 26: //pAtkNormal
            return UINT8_FIELD;
        case 27: //pAtkFor
            return UINT8_FIELD;
        case 28: //pAtkBack
            return UINT8_FIELD;
        case 29: //pAtkL
            return UINT8_FIELD;
        case 30: //pAtkR
            return UINT8_FIELD;
        case 31: //unused4
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 3;
                default:
                    return UNKNOWN_FIELD;
                }
        case 32: //holdTimerMin
            return FLOAT32_FIELD;
        case 33: //holdTimerMax
            return FLOAT32_FIELD;
        case 34: //flagsA
            return UINT8_FLAG_FIELD;
        case 35: //acroDodge
            return UINT8_FIELD;
        case 36: //unused5
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
        case 37: //rMultOpt
            return FLOAT32_FIELD;
        case 38: //rMultMax
            return FLOAT32_FIELD;
        case 39: //mDistance
            return FLOAT32_FIELD;
        case 40: //rDistance
            return FLOAT32_FIELD;
        case 41: //buffStand
            return FLOAT32_FIELD;
        case 42: //rStand
            return FLOAT32_FIELD;
        case 43: //groupStand
            return FLOAT32_FIELD;
        case 44: //rushChance
            return UINT8_FIELD;
        case 45: //unused6
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 3;
                default:
                    return UNKNOWN_FIELD;
                }
        case 46: //rushMult
            return FLOAT32_FIELD;
        case 47: //flagsB
            return UINT32_FLAG_FIELD;
        case 48: //dodgeFMult
            return FLOAT32_FIELD;
        case 49: //dodgeFBase
            return FLOAT32_FIELD;
        case 50: //encSBase
            return FLOAT32_FIELD;
        case 51: //encSMult
            return FLOAT32_FIELD;
        case 52: //dodgeAtkMult
            return FLOAT32_FIELD;
        case 53: //dodgeNAtkMult
            return FLOAT32_FIELD;
        case 54: //dodgeBAtkMult
            return FLOAT32_FIELD;
        case 55: //dodgeBNAtkMult
            return FLOAT32_FIELD;
        case 56: //dodgeFAtkMult
            return FLOAT32_FIELD;
        case 57: //dodgeFNAtkMult
            return FLOAT32_FIELD;
        case 58: //blockMult
            return FLOAT32_FIELD;
        case 59: //blockBase
            return FLOAT32_FIELD;
        case 60: //blockAtkMult
            return FLOAT32_FIELD;
        case 61: //blockNAtkMult
            return FLOAT32_FIELD;
        case 62: //atkMult
            return FLOAT32_FIELD;
        case 63: //atkBase
            return FLOAT32_FIELD;
        case 64: //atkAtkMult
            return FLOAT32_FIELD;
        case 65: //atkNAtkMult
            return FLOAT32_FIELD;
        case 66: //atkBlockMult
            return FLOAT32_FIELD;
        case 67: //pAtkFBase
            return FLOAT32_FIELD;
        case 68: //pAtkFMult
            return FLOAT32_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    }

void * CSTYRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
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
        case 5: //dodgeChance
            return &CSTD.value.dodgeChance;
        case 6: //lrChance
            return &CSTD.value.lrChance;
        case 7: //unused1
            *FieldValues = &CSTD.value.unused1[0];
            return NULL;
        case 8: //lrTimerMin
            return &CSTD.value.lrTimerMin;
        case 9: //lrTimerMax
            return &CSTD.value.lrTimerMax;
        case 10: //forTimerMin
            return &CSTD.value.forTimerMin;
        case 11: //forTimerMax
            return &CSTD.value.forTimerMax;
        case 12: //backTimerMin
            return &CSTD.value.backTimerMin;
        case 13: //backTimerMax
            return &CSTD.value.backTimerMax;
        case 14: //idleTimerMin
            return &CSTD.value.idleTimerMin;
        case 15: //idleTimerMax
            return &CSTD.value.idleTimerMax;
        case 16: //blkChance
            return &CSTD.value.blkChance;
        case 17: //atkChance
            return &CSTD.value.atkChance;
        case 18: //unused2
            *FieldValues = &CSTD.value.unused2[0];
            return NULL;
        case 19: //atkBRecoil
            return &CSTD.value.atkBRecoil;
        case 20: //atkBUnc
            return &CSTD.value.atkBUnc;
        case 21: //atkBh2h
            return &CSTD.value.atkBh2h;
        case 22: //pAtkChance
            return &CSTD.value.pAtkChance;
        case 23: //unused3
            *FieldValues = &CSTD.value.unused3[0];
            return NULL;
        case 24: //pAtkBRecoil
            return &CSTD.value.pAtkBRecoil;
        case 25: //pAtkBUnc
            return &CSTD.value.pAtkBUnc;
        case 26: //pAtkNormal
            return &CSTD.value.pAtkNormal;
        case 27: //pAtkFor
            return &CSTD.value.pAtkFor;
        case 28: //pAtkBack
            return &CSTD.value.pAtkBack;
        case 29: //pAtkL
            return &CSTD.value.pAtkL;
        case 30: //pAtkR
            return &CSTD.value.pAtkR;
        case 31: //unused4
            *FieldValues = &CSTD.value.unused4[0];
            return NULL;
        case 32: //holdTimerMin
            return &CSTD.value.holdTimerMin;
        case 33: //holdTimerMax
            return &CSTD.value.holdTimerMax;
        case 34: //flagsA
            return &CSTD.value.flagsA;
        case 35: //acroDodge
            return &CSTD.value.acroDodge;
        case 36: //unused5
            *FieldValues = &CSTD.value.unused5[0];
            return NULL;
        case 37: //rMultOpt
            return &CSTD.value.rMultOpt;
        case 38: //rMultMax
            return &CSTD.value.rMultMax;
        case 39: //mDistance
            return &CSTD.value.mDistance;
        case 40: //rDistance
            return &CSTD.value.rDistance;
        case 41: //buffStand
            return &CSTD.value.buffStand;
        case 42: //rStand
            return &CSTD.value.rStand;
        case 43: //groupStand
            return &CSTD.value.groupStand;
        case 44: //rushChance
            return &CSTD.value.rushChance;
        case 45: //unused6
            *FieldValues = &CSTD.value.unused6[0];
            return NULL;
        case 46: //rushMult
            return &CSTD.value.rushMult;
        case 47: //flagsB
            return &CSTD.value.flagsB;
        case 48: //dodgeFMult
            return CSAD.IsLoaded() ? &CSAD->dodgeFMult : NULL;
        case 49: //dodgeFBase
            return CSAD.IsLoaded() ? &CSAD->dodgeFBase : NULL;
        case 50: //encSBase
            return CSAD.IsLoaded() ? &CSAD->encSBase : NULL;
        case 51: //encSMult
            return CSAD.IsLoaded() ? &CSAD->encSMult : NULL;
        case 52: //dodgeAtkMult
            return CSAD.IsLoaded() ? &CSAD->dodgeAtkMult : NULL;
        case 53: //dodgeNAtkMult
            return CSAD.IsLoaded() ? &CSAD->dodgeNAtkMult : NULL;
        case 54: //dodgeBAtkMult
            return CSAD.IsLoaded() ? &CSAD->dodgeBAtkMult : NULL;
        case 55: //dodgeBNAtkMult
            return CSAD.IsLoaded() ? &CSAD->dodgeBNAtkMult : NULL;
        case 56: //dodgeFAtkMult
            return CSAD.IsLoaded() ? &CSAD->dodgeFAtkMult : NULL;
        case 57: //dodgeFNAtkMult
            return CSAD.IsLoaded() ? &CSAD->dodgeFNAtkMult : NULL;
        case 58: //blockMult
            return CSAD.IsLoaded() ? &CSAD->blockMult : NULL;
        case 59: //blockBase
            return CSAD.IsLoaded() ? &CSAD->blockBase : NULL;
        case 60: //blockAtkMult
            return CSAD.IsLoaded() ? &CSAD->blockAtkMult : NULL;
        case 61: //blockNAtkMult
            return CSAD.IsLoaded() ? &CSAD->blockNAtkMult : NULL;
        case 62: //atkMult
            return CSAD.IsLoaded() ? &CSAD->atkMult : NULL;
        case 63: //atkBase
            return CSAD.IsLoaded() ? &CSAD->atkBase : NULL;
        case 64: //atkAtkMult
            return CSAD.IsLoaded() ? &CSAD->atkAtkMult : NULL;
        case 65: //atkNAtkMult
            return CSAD.IsLoaded() ? &CSAD->atkNAtkMult : NULL;
        case 66: //atkBlockMult
            return CSAD.IsLoaded() ? &CSAD->atkBlockMult : NULL;
        case 67: //pAtkFBase
            return CSAD.IsLoaded() ? &CSAD->pAtkFBase : NULL;
        case 68: //pAtkFMult
            return CSAD.IsLoaded() ? &CSAD->pAtkFMult : NULL;
        default:
            return NULL;
        }
    }

bool CSTYRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
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
        case 5: //dodgeChance
            CSTD.value.dodgeChance = *(UINT8 *)FieldValue;
            break;
        case 6: //lrChance
            CSTD.value.lrChance = *(UINT8 *)FieldValue;
            break;
        case 7: //unused1
            if(ArraySize != 2)
                break;
            CSTD.value.unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            CSTD.value.unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 8: //lrTimerMin
            CSTD.value.lrTimerMin = *(FLOAT32 *)FieldValue;
            break;
        case 9: //lrTimerMax
            CSTD.value.lrTimerMax = *(FLOAT32 *)FieldValue;
            break;
        case 10: //forTimerMin
            CSTD.value.forTimerMin = *(FLOAT32 *)FieldValue;
            break;
        case 11: //forTimerMax
            CSTD.value.forTimerMax = *(FLOAT32 *)FieldValue;
            break;
        case 12: //backTimerMin
            CSTD.value.backTimerMin = *(FLOAT32 *)FieldValue;
            break;
        case 13: //backTimerMax
            CSTD.value.backTimerMax = *(FLOAT32 *)FieldValue;
            break;
        case 14: //idleTimerMin
            CSTD.value.idleTimerMin = *(FLOAT32 *)FieldValue;
            break;
        case 15: //idleTimerMax
            CSTD.value.idleTimerMax = *(FLOAT32 *)FieldValue;
            break;
        case 16: //blkChance
            CSTD.value.blkChance = *(UINT8 *)FieldValue;
            break;
        case 17: //atkChance
            CSTD.value.atkChance = *(UINT8 *)FieldValue;
            break;
        case 18: //unused2
            if(ArraySize != 2)
                break;
            CSTD.value.unused2[0] = ((UINT8ARRAY)FieldValue)[0];
            CSTD.value.unused2[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 19: //atkBRecoil
            CSTD.value.atkBRecoil = *(FLOAT32 *)FieldValue;
            break;
        case 20: //atkBUnc
            CSTD.value.atkBUnc = *(FLOAT32 *)FieldValue;
            break;
        case 21: //atkBh2h
            CSTD.value.atkBh2h = *(FLOAT32 *)FieldValue;
            break;
        case 22: //pAtkChance
            CSTD.value.pAtkChance = *(UINT8 *)FieldValue;
            break;
        case 23: //unused3
            if(ArraySize != 3)
                break;
            CSTD.value.unused3[0] = ((UINT8ARRAY)FieldValue)[0];
            CSTD.value.unused3[1] = ((UINT8ARRAY)FieldValue)[1];
            CSTD.value.unused3[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        case 24: //pAtkBRecoil
            CSTD.value.pAtkBRecoil = *(FLOAT32 *)FieldValue;
            break;
        case 25: //pAtkBUnc
            CSTD.value.pAtkBUnc = *(FLOAT32 *)FieldValue;
            break;
        case 26: //pAtkNormal
            CSTD.value.pAtkNormal = *(UINT8 *)FieldValue;
            break;
        case 27: //pAtkFor
            CSTD.value.pAtkFor = *(UINT8 *)FieldValue;
            break;
        case 28: //pAtkBack
            CSTD.value.pAtkBack = *(UINT8 *)FieldValue;
            break;
        case 29: //pAtkL
            CSTD.value.pAtkL = *(UINT8 *)FieldValue;
            break;
        case 30: //pAtkR
            CSTD.value.pAtkR = *(UINT8 *)FieldValue;
            break;
        case 31: //unused4
            if(ArraySize != 3)
                break;
            CSTD.value.unused4[0] = ((UINT8ARRAY)FieldValue)[0];
            CSTD.value.unused4[1] = ((UINT8ARRAY)FieldValue)[1];
            CSTD.value.unused4[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        case 32: //holdTimerMin
            CSTD.value.holdTimerMin = *(FLOAT32 *)FieldValue;
            break;
        case 33: //holdTimerMax
            CSTD.value.holdTimerMax = *(FLOAT32 *)FieldValue;
            break;
        case 34: //flagsA
            SetFlagAMask(*(UINT8 *)FieldValue);
            break;
        case 35: //acroDodge
            CSTD.value.acroDodge = *(UINT8 *)FieldValue;
            break;
        case 36: //unused5
            if(ArraySize != 2)
                break;
            CSTD.value.unused5[0] = ((UINT8ARRAY)FieldValue)[0];
            CSTD.value.unused5[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 37: //rMultOpt
            CSTD.value.rMultOpt = *(FLOAT32 *)FieldValue;
            break;
        case 38: //rMultMax
            CSTD.value.rMultMax = *(FLOAT32 *)FieldValue;
            break;
        case 39: //mDistance
            CSTD.value.mDistance = *(FLOAT32 *)FieldValue;
            break;
        case 40: //rDistance
            CSTD.value.rDistance = *(FLOAT32 *)FieldValue;
            break;
        case 41: //buffStand
            CSTD.value.buffStand = *(FLOAT32 *)FieldValue;
            break;
        case 42: //rStand
            CSTD.value.rStand = *(FLOAT32 *)FieldValue;
            break;
        case 43: //groupStand
            CSTD.value.groupStand = *(FLOAT32 *)FieldValue;
            break;
        case 44: //rushChance
            CSTD.value.rushChance = *(UINT8 *)FieldValue;
            break;
        case 45: //unused6
            if(ArraySize != 3)
                break;
            CSTD.value.unused6[0] = ((UINT8ARRAY)FieldValue)[0];
            CSTD.value.unused6[1] = ((UINT8ARRAY)FieldValue)[1];
            CSTD.value.unused6[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        case 46: //rushMult
            CSTD.value.rushMult = *(FLOAT32 *)FieldValue;
            break;
        case 47: //flagsB
            SetFlagBMask(*(UINT32 *)FieldValue);
            break;
        case 48: //dodgeFMult
            CSAD.Load();
            CSAD->dodgeFMult = *(FLOAT32 *)FieldValue;
            break;
        case 49: //dodgeFBase
            CSAD.Load();
            CSAD->dodgeFBase = *(FLOAT32 *)FieldValue;
            break;
        case 50: //encSBase
            CSAD.Load();
            CSAD->encSBase = *(FLOAT32 *)FieldValue;
            break;
        case 51: //encSMult
            CSAD.Load();
            CSAD->encSMult = *(FLOAT32 *)FieldValue;
            break;
        case 52: //dodgeAtkMult
            CSAD.Load();
            CSAD->dodgeAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 53: //dodgeNAtkMult
            CSAD.Load();
            CSAD->dodgeNAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 54: //dodgeBAtkMult
            CSAD.Load();
            CSAD->dodgeBAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 55: //dodgeBNAtkMult
            CSAD.Load();
            CSAD->dodgeBNAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 56: //dodgeFAtkMult
            CSAD.Load();
            CSAD->dodgeFAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 57: //dodgeFNAtkMult
            CSAD.Load();
            CSAD->dodgeFNAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 58: //blockMult
            CSAD.Load();
            CSAD->blockMult = *(FLOAT32 *)FieldValue;
            break;
        case 59: //blockBase
            CSAD.Load();
            CSAD->blockBase = *(FLOAT32 *)FieldValue;
            break;
        case 60: //blockAtkMult
            CSAD.Load();
            CSAD->blockAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 61: //blockNAtkMult
            CSAD.Load();
            CSAD->blockNAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 62: //atkMult
            CSAD.Load();
            CSAD->atkMult = *(FLOAT32 *)FieldValue;
            break;
        case 63: //atkBase
            CSAD.Load();
            CSAD->atkBase = *(FLOAT32 *)FieldValue;
            break;
        case 64: //atkAtkMult
            CSAD.Load();
            CSAD->atkAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 65: //atkNAtkMult
            CSAD.Load();
            CSAD->atkNAtkMult = *(FLOAT32 *)FieldValue;
            break;
        case 66: //atkBlockMult
            CSAD.Load();
            CSAD->atkBlockMult = *(FLOAT32 *)FieldValue;
            break;
        case 67: //pAtkFBase
            CSAD.Load();
            CSAD->pAtkFBase = *(FLOAT32 *)FieldValue;
            break;
        case 68: //pAtkFMult
            CSAD.Load();
            CSAD->pAtkFMult = *(FLOAT32 *)FieldValue;
            break;
        default:
            break;
        }
    return false;
    }

void CSTYRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    CSTYCSTD defaultCSTD;
    CSTYCSAD defaultCSAD;

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
        case 5: //dodgeChance
            CSTD.value.dodgeChance = defaultCSTD.dodgeChance;
            return;
        case 6: //lrChance
            CSTD.value.lrChance = defaultCSTD.lrChance;
            return;
        case 7: //unused1
            CSTD.value.unused1[0] = defaultCSTD.unused1[0];
            CSTD.value.unused1[1] = defaultCSTD.unused1[1];
            return;
        case 8: //lrTimerMin
            CSTD.value.lrTimerMin = defaultCSTD.lrTimerMin;
            return;
        case 9: //lrTimerMax
            CSTD.value.lrTimerMax = defaultCSTD.lrTimerMax;
            return;
        case 10: //forTimerMin
            CSTD.value.forTimerMin = defaultCSTD.forTimerMin;
            return;
        case 11: //forTimerMax
            CSTD.value.forTimerMax = defaultCSTD.forTimerMax;
            return;
        case 12: //backTimerMin
            CSTD.value.backTimerMin = defaultCSTD.backTimerMin;
            return;
        case 13: //backTimerMax
            CSTD.value.backTimerMax = defaultCSTD.backTimerMax;
            return;
        case 14: //idleTimerMin
            CSTD.value.idleTimerMin = defaultCSTD.idleTimerMin;
            return;
        case 15: //idleTimerMax
            CSTD.value.idleTimerMax = defaultCSTD.idleTimerMax;
            return;
        case 16: //blkChance
            CSTD.value.blkChance = defaultCSTD.blkChance;
            return;
        case 17: //atkChance
            CSTD.value.atkChance = defaultCSTD.atkChance;
            return;
        case 18: //unused2
            CSTD.value.unused2[0] = defaultCSTD.unused2[0];
            CSTD.value.unused2[1] = defaultCSTD.unused2[1];
            return;
        case 19: //atkBRecoil
            CSTD.value.atkBRecoil = defaultCSTD.atkBRecoil;
            return;
        case 20: //atkBUnc
            CSTD.value.atkBUnc = defaultCSTD.atkBUnc;
            return;
        case 21: //atkBh2h
            CSTD.value.atkBh2h = defaultCSTD.atkBh2h;
            return;
        case 22: //pAtkChance
            CSTD.value.pAtkChance = defaultCSTD.pAtkChance;
            return;
        case 23: //unused3
            CSTD.value.unused3[0] = defaultCSTD.unused3[0];
            CSTD.value.unused3[1] = defaultCSTD.unused3[1];
            CSTD.value.unused3[2] = defaultCSTD.unused3[2];
            return;
        case 24: //pAtkBRecoil
            CSTD.value.pAtkBRecoil = defaultCSTD.pAtkBRecoil;
            return;
        case 25: //pAtkBUnc
            CSTD.value.pAtkBUnc = defaultCSTD.pAtkBUnc;
            return;
        case 26: //pAtkNormal
            CSTD.value.pAtkNormal = defaultCSTD.pAtkNormal;
            return;
        case 27: //pAtkFor
            CSTD.value.pAtkFor = defaultCSTD.pAtkFor;
            return;
        case 28: //pAtkBack
            CSTD.value.pAtkBack = defaultCSTD.pAtkBack;
            return;
        case 29: //pAtkL
            CSTD.value.pAtkL = defaultCSTD.pAtkL;
            return;
        case 30: //pAtkR
            CSTD.value.pAtkR = defaultCSTD.pAtkR;
            return;
        case 31: //unused4
            CSTD.value.unused4[0] = defaultCSTD.unused4[0];
            CSTD.value.unused4[1] = defaultCSTD.unused4[1];
            CSTD.value.unused4[2] = defaultCSTD.unused4[2];
            return;
        case 32: //holdTimerMin
            CSTD.value.holdTimerMin = defaultCSTD.holdTimerMin;
            return;
        case 33: //holdTimerMax
            CSTD.value.holdTimerMax = defaultCSTD.holdTimerMax;
            return;
        case 34: //flagsA
            CSTD.value.flagsA = defaultCSTD.flagsA;
            return;
        case 35: //acroDodge
            CSTD.value.acroDodge = defaultCSTD.acroDodge;
            return;
        case 36: //unused5
            CSTD.value.unused5[0] = defaultCSTD.unused5[0];
            CSTD.value.unused5[1] = defaultCSTD.unused5[1];
            return;
        case 37: //rMultOpt
            CSTD.value.rMultOpt = defaultCSTD.rMultOpt;
            return;
        case 38: //rMultMax
            CSTD.value.rMultMax = defaultCSTD.rMultMax;
            return;
        case 39: //mDistance
            CSTD.value.mDistance = defaultCSTD.mDistance;
            return;
        case 40: //rDistance
            CSTD.value.rDistance = defaultCSTD.rDistance;
            return;
        case 41: //buffStand
            CSTD.value.buffStand = defaultCSTD.buffStand;
            return;
        case 42: //rStand
            CSTD.value.rStand = defaultCSTD.rStand;
            return;
        case 43: //groupStand
            CSTD.value.groupStand = defaultCSTD.groupStand;
            return;
        case 44: //rushChance
            CSTD.value.rushChance = defaultCSTD.rushChance;
            return;
        case 45: //unused6
            CSTD.value.unused6[0] = defaultCSTD.unused6[0];
            CSTD.value.unused6[1] = defaultCSTD.unused6[1];
            CSTD.value.unused6[2] = defaultCSTD.unused6[2];
            return;
        case 46: //rushMult
            CSTD.value.rushMult = defaultCSTD.rushMult;
            return;
        case 47: //flagsB
            CSTD.value.flagsB = defaultCSTD.flagsB;
            return;
        case 48: //dodgeFMult
            if(CSAD.IsLoaded())
                CSAD->dodgeFMult = defaultCSAD.dodgeFMult;
            return;
        case 49: //dodgeFBase
            if(CSAD.IsLoaded())
                CSAD->dodgeFBase = defaultCSAD.dodgeFBase;
            return;
        case 50: //encSBase
            if(CSAD.IsLoaded())
                CSAD->encSBase = defaultCSAD.encSBase;
            return;
        case 51: //encSMult
            if(CSAD.IsLoaded())
                CSAD->encSMult = defaultCSAD.encSMult;
            return;
        case 52: //dodgeAtkMult
            if(CSAD.IsLoaded())
                CSAD->dodgeAtkMult = defaultCSAD.dodgeAtkMult;
            return;
        case 53: //dodgeNAtkMult
            if(CSAD.IsLoaded())
                CSAD->dodgeNAtkMult = defaultCSAD.dodgeNAtkMult;
            return;
        case 54: //dodgeBAtkMult
            if(CSAD.IsLoaded())
                CSAD->dodgeBAtkMult = defaultCSAD.dodgeBAtkMult;
            return;
        case 55: //dodgeBNAtkMult
            if(CSAD.IsLoaded())
                CSAD->dodgeBNAtkMult = defaultCSAD.dodgeBNAtkMult;
            return;
        case 56: //dodgeFAtkMult
            if(CSAD.IsLoaded())
                CSAD->dodgeFAtkMult = defaultCSAD.dodgeFAtkMult;
            return;
        case 57: //dodgeFNAtkMult
            if(CSAD.IsLoaded())
                CSAD->dodgeFNAtkMult = defaultCSAD.dodgeFNAtkMult;
            return;
        case 58: //blockMult
            if(CSAD.IsLoaded())
                CSAD->blockMult = defaultCSAD.blockMult;
            return;
        case 59: //blockBase
            if(CSAD.IsLoaded())
                CSAD->blockBase = defaultCSAD.blockBase;
            return;
        case 60: //blockAtkMult
            if(CSAD.IsLoaded())
                CSAD->blockAtkMult = defaultCSAD.blockAtkMult;
            return;
        case 61: //blockNAtkMult
            if(CSAD.IsLoaded())
                CSAD->blockNAtkMult = defaultCSAD.blockNAtkMult;
            return;
        case 62: //atkMult
            if(CSAD.IsLoaded())
                CSAD->atkMult = defaultCSAD.atkMult;
            return;
        case 63: //atkBase
            if(CSAD.IsLoaded())
                CSAD->atkBase = defaultCSAD.atkBase;
            return;
        case 64: //atkAtkMult
            if(CSAD.IsLoaded())
                CSAD->atkAtkMult = defaultCSAD.atkAtkMult;
            return;
        case 65: //atkNAtkMult
            if(CSAD.IsLoaded())
                CSAD->atkNAtkMult = defaultCSAD.atkNAtkMult;
            return;
        case 66: //atkBlockMult
            if(CSAD.IsLoaded())
                CSAD->atkBlockMult = defaultCSAD.atkBlockMult;
            return;
        case 67: //pAtkFBase
            if(CSAD.IsLoaded())
                CSAD->pAtkFBase = defaultCSAD.pAtkFBase;
            return;
        case 68: //pAtkFMult
            if(CSAD.IsLoaded())
                CSAD->pAtkFMult = defaultCSAD.pAtkFMult;
            return;
        default:
            return;
        }
    }