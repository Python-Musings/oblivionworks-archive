/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is CBash code.
 *
 * The Initial Developer of the Original Code is
 * Waruddar.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#include "..\..\..\Common.h"
#include "..\CELLRecord.h"

namespace Sk
{
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
        case 3: //versionControl1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 4;
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 4: //eid
            return ISTRING_FIELD;
        case 5: //formVersion
            return UINT16_FIELD;
        case 6: //versionControl2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return 2;
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 7: //full
            return STRING_FIELD;
        case 8: //flags
            return UINT8_FIELD;
        case 9: //posX
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
            return UNKNOWN_FIELD;
        case 10: //posY
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
            return UNKNOWN_FIELD;
        case 11: //quadFlags
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UNKNOWN_OR_UINT32_FLAG_FIELD;
                case 2: //WhichType
                    if(!IsInterior())
                        return UINT32_FLAG_FIELD;
                    return UNKNOWN_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
#if 0
        case 12: //ambientRed
            return UINT8_FIELD;
        case 13: //ambientGreen
            return UINT8_FIELD;
        case 14: //ambientBlue
            return UINT8_FIELD;
        case 15: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XCLL.IsLoaded() ? 1 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 16: //directionalRed
            return UINT8_FIELD;
        case 17: //directionalGreen
            return UINT8_FIELD;
        case 18: //directionalBlue
            return UINT8_FIELD;
        case 19: //unused2
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XCLL.IsLoaded() ? 1 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 20: //fogRed
            return UINT8_FIELD;
        case 21: //fogGreen
            return UINT8_FIELD;
        case 22: //fogBlue
            return UINT8_FIELD;
        case 23: //unused3
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XCLL.IsLoaded() ? 1 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 24: //fogNear
            return FLOAT32_FIELD;
        case 25: //fogFar
            return FLOAT32_FIELD;
        case 26: //directionalXY
            return SINT32_FIELD;
        case 27: //directionalZ
            return SINT32_FIELD;
        case 28: //directionalFade
            return FLOAT32_FIELD;
        case 29: //fogClip
            return FLOAT32_FIELD;
        case 30: //fogPower
            return FLOAT32_FIELD;
#endif
        case 31: //swappedImpacts
            if(ListFieldID == 0) //swappedImpacts
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)IMPS.value.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                return UNKNOWN_FIELD;
                }

            if(ListIndex >= IMPS.value.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //material
                    return UINT32_TYPE_FIELD;
                case 2: //oldImpact
                    return FORMID_FIELD;
                case 3: //newImpact
                    return FORMID_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 32: //concSolid
            return STRING_FIELD;
        case 33: //concBroken
            return STRING_FIELD;
        case 34: //metalSolid
            return STRING_FIELD;
        case 35: //metalHollow
            return STRING_FIELD;
        case 36: //metalSheet
            return STRING_FIELD;
        case 37: //wood
            return STRING_FIELD;
        case 38: //sand
            return STRING_FIELD;
        case 39: //dirt
            return STRING_FIELD;
        case 40: //grass
            return STRING_FIELD;
        case 41: //water
            return STRING_FIELD;
        case 42: //lightTemplate
            return FORMID_FIELD;
        case 43: //lightFlags
            return UINT32_FLAG_FIELD;
        case 44: //waterHeight
            return FLOAT32_FIELD;
        case 45: //waterNoisePath
            return ISTRING_FIELD;
        case 46: //regions
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return FORMID_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)XCLR.value.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 47: //imageSpace
            return FORMID_FIELD;
        case 48: //xcet_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XCET.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 49: //encounterZone
            return FORMID_FIELD;
        case 50: //climate
            return FORMID_FIELD;
        case 51: //water
            return FORMID_FIELD;
        case 52: //owner
            return FORMID_FIELD;
        case 53: //rank
            return SINT32_FIELD;
        case 54: //acousticSpace
            return FORMID_FIELD;
        case 55: //xcmt_p
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return XCMT.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 56: //music
            return FORMID_FIELD;
#if 0
        case 57: //ACHR
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)ACHR.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 58: //ACRE
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)ACRE.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 59: //REFR
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)REFR.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 60: //PGRE
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)PGRE.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 61: //PMIS
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)PMIS.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 62: //PBEA
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)PBEA.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 63: //PFLA
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)PFLA.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 64: //PCBE
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)PCBE.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 65: //NAVM
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return SUBRECORD_ARRAY_FIELD;
                case 1: //fieldSize
                    return (UINT32)NAVM.size();
                default:
                    return UNKNOWN_FIELD;
                }
            return UNKNOWN_FIELD;
        case 66: //LAND
	    return SUBRECORD_FIELD;
#endif
        case 67: //Parent
            return PARENTRECORD_FIELD;
        default:
            return UNKNOWN_FIELD;
        }
    return UNKNOWN_FIELD;
    }

void * CELLRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
    {
    switch(FieldID)
        {
        case 1: //flags1
            return &flags;
        case 2: //fid
            return &formID;
        case 3: //versionControl1
            *FieldValues = &flagsUnk;
            return NULL;
        case 4: //eid
            return EDID.value;
        case 5: //formVersion
            return &formVersion;
        case 6: //versionControl2
            *FieldValues = &versionControl2[0];
            return NULL;
        case 7: //full
            return FULL.value;
        case 8: //flags
            return &DATA.value;
        case 9: //posX
            return (!IsInterior() & XCLC.IsLoaded()) ? &XCLC->posX : NULL;
        case 10: //posY
            return (!IsInterior() & XCLC.IsLoaded()) ? &XCLC->posY : NULL;
        case 11: //quadFlags
            return (!IsInterior() & XCLC.IsLoaded()) ? &XCLC->flags : NULL;
#if 0
        case 12: //ambientRed
            return XCLL.IsLoaded() ? &XCLL->ambient.red : NULL;
        case 13: //ambientGreen
            return XCLL.IsLoaded() ? &XCLL->ambient.green : NULL;
        case 14: //ambientBlue
            return XCLL.IsLoaded() ? &XCLL->ambient.blue : NULL;
        case 15: //unused1
            *FieldValues = XCLL.IsLoaded() ? &XCLL->ambient.unused1 : NULL;
            return NULL;
        case 16: //directionalRed
            return XCLL.IsLoaded() ? &XCLL->directional.red : NULL;
        case 17: //directionalGreen
            return XCLL.IsLoaded() ? &XCLL->directional.green : NULL;
        case 18: //directionalBlue
            return XCLL.IsLoaded() ? &XCLL->directional.blue : NULL;
        case 19: //unused2
            *FieldValues = XCLL.IsLoaded() ? &XCLL->directional.unused1 : NULL;
            return NULL;
        case 20: //fogRed
            return XCLL.IsLoaded() ? &XCLL->fog.red : NULL;
        case 21: //fogGreen
            return XCLL.IsLoaded() ? &XCLL->fog.green : NULL;
        case 22: //fogBlue
            return XCLL.IsLoaded() ? &XCLL->fog.blue : NULL;
        case 23: //unused3
            *FieldValues = XCLL.IsLoaded() ? &XCLL->fog.unused1 : NULL;
            return NULL;
        case 24: //fogNear
            return XCLL.IsLoaded() ? &XCLL->fogNear : NULL;
        case 25: //fogFar
            return XCLL.IsLoaded() ? &XCLL->fogFar : NULL;
        case 26: //directionalXY
            return XCLL.IsLoaded() ? &XCLL->directionalXY : NULL;
        case 27: //directionalZ
            return XCLL.IsLoaded() ? &XCLL->directionalZ : NULL;
        case 28: //directionalFade
            return XCLL.IsLoaded() ? &XCLL->directionalFade : NULL;
        case 29: //fogClip
            return XCLL.IsLoaded() ? &XCLL->fogClip : NULL;
        case 30: //fogPower
            return XCLL.IsLoaded() ? &XCLL->fogPower : NULL;
#endif
        case 31: //swappedImpacts
            if(ListIndex >= IMPS.value.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //material
                    return &IMPS.value[ListIndex]->material;
                case 2: //oldImpact
                    return &IMPS.value[ListIndex]->oldImpact;
                case 3: //newImpact
                    return &IMPS.value[ListIndex]->newImpact;
                default:
                    return NULL;
                }
            return NULL;
        case 32: //concSolid
            return IMPF.IsLoaded() ? &IMPF->concSolid[0] : NULL;
        case 33: //concBroken
            return IMPF.IsLoaded() ? &IMPF->concBroken[0] : NULL;
        case 34: //metalSolid
            return IMPF.IsLoaded() ? &IMPF->metalSolid[0] : NULL;
        case 35: //metalHollow
            return IMPF.IsLoaded() ? &IMPF->metalHollow[0] : NULL;
        case 36: //metalSheet
            return IMPF.IsLoaded() ? &IMPF->metalSheet[0] : NULL;
        case 37: //wood
            return IMPF.IsLoaded() ? &IMPF->wood[0] : NULL;
        case 38: //sand
            return IMPF.IsLoaded() ? &IMPF->sand[0] : NULL;
        case 39: //dirt
            return IMPF.IsLoaded() ? &IMPF->dirt[0] : NULL;
        case 40: //grass
            return IMPF.IsLoaded() ? &IMPF->grass[0] : NULL;
        case 41: //water
            return IMPF.IsLoaded() ? &IMPF->water[0] : NULL;
        case 42: //lightTemplate
            return &LTMP.value;
        case 43: //lightFlags
            return &LNAM.value;
        case 44: //waterHeight
            return &XCLW.value;
        case 45: //waterNoisePath
            return XNAM.value;
        case 46: //regions
            *FieldValues = XCLR.IsLoaded() ? &XCLR.value[0] : NULL;
            return NULL;
        case 47: //imageSpace
            return &XCIM.value;
        case 48: //xcet_p
            *FieldValues = XCET.value;
            return NULL;
        case 49: //encounterZone
            return &XEZN.value;
        case 50: //climate
            return &XCCM.value;
        case 51: //water
            return &XCWT.value;
#if 0
        case 52: //owner
            return Ownership.IsLoaded() ? &Ownership->XOWN.value : NULL;
        case 53: //rank
            return Ownership.IsLoaded() ? Ownership->XRNK.value : NULL;
#endif
        case 54: //acousticSpace
            return &XCAS.value;
        case 55: //xcmt_p
            *FieldValues = XCMT.value;
            return NULL;
        case 56: //music
            return &XCMO.value;
#if 0
        case 57: //ACHR
            for(UINT32 p = 0;p < (UINT32)ACHR.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = ACHR[p];
            return NULL;
        case 58: //ACRE
            for(UINT32 p = 0;p < (UINT32)ACRE.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = ACRE[p];
            return NULL;
            //*FieldValues = ACRE.size() != 0 ? &ACRE[0] : NULL;
            //return NULL;
        case 59: //REFR
            for(UINT32 p = 0;p < (UINT32)REFR.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = REFR[p];
            return NULL;
        case 60: //PGRE
            for(UINT32 p = 0;p < (UINT32)PGRE.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = PGRE[p];
            return NULL;
        case 61: //PMIS
            for(UINT32 p = 0;p < (UINT32)PMIS.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = PMIS[p];
            return NULL;
        case 62: //PBEA
            for(UINT32 p = 0;p < (UINT32)PBEA.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = PBEA[p];
            return NULL;
        case 63: //PFLA
            for(UINT32 p = 0;p < (UINT32)PFLA.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = PFLA[p];
            return NULL;
        case 64: //PCBE
            for(UINT32 p = 0;p < (UINT32)PCBE.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = PCBE[p];
            return NULL;
        case 65: //NAVM
            for(UINT32 p = 0;p < (UINT32)NAVM.size();++p)
                ((RECORDIDARRAY)FieldValues)[p] = NAVM[p];
            return NULL;
        case 66: //LAND
            return LAND;
#endif
        case 67: //Parent
            return GetParentRecord();
        default:
            return NULL;
        }
    return NULL;
    }

bool CELLRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
    {
    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(*(UINT32 *)FieldValue);
            break;
        case 3: //versionControl1
            if(ArraySize != 4)
                break;
            ((UINT8ARRAY)&flagsUnk)[0] = ((UINT8ARRAY)FieldValue)[0];
            ((UINT8ARRAY)&flagsUnk)[1] = ((UINT8ARRAY)FieldValue)[1];
            ((UINT8ARRAY)&flagsUnk)[2] = ((UINT8ARRAY)FieldValue)[2];
            ((UINT8ARRAY)&flagsUnk)[3] = ((UINT8ARRAY)FieldValue)[3];
            break;
        case 4: //eid
            EDID.Copy((STRING)FieldValue);
            break;
        case 5: //formVersion
            formVersion = *(UINT16 *)FieldValue;
            break;
        case 6: //versionControl2
            if(ArraySize != 2)
                break;
            versionControl2[0] = ((UINT8ARRAY)FieldValue)[0];
            versionControl2[1] = ((UINT8ARRAY)FieldValue)[1];
            break;
        case 7: //full
            FULL.Copy((STRING)FieldValue);
            break;
        case 8: //flags
            SetFlagMask(*(UINT8 *)FieldValue);
            break;
        case 9: //posX
            if(IsInterior())
                break;
            XCLC.Load();
            XCLC->posX = *(SINT32 *)FieldValue;
            break;
        case 10: //posY
            if(IsInterior())
                break;
            XCLC.Load();
            XCLC->posY = *(SINT32 *)FieldValue;
            break;
        case 11: //quadFlags
            if(IsInterior())
                break;
            SetQuadFlagMask(*(UINT32 *)FieldValue);
            break;
#if 0
        case 12: //ambientRed
            XCLL.Load();
            XCLL->ambient.red = *(UINT8 *)FieldValue;
            break;
        case 13: //ambientGreen
            XCLL.Load();
            XCLL->ambient.green = *(UINT8 *)FieldValue;
            break;
        case 14: //ambientBlue
            XCLL.Load();
            XCLL->ambient.blue = *(UINT8 *)FieldValue;
            break;
        case 15: //unused1
            if(ArraySize != 1)
                break;
            XCLL.Load();
            XCLL->ambient.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 16: //directionalRed
            XCLL.Load();
            XCLL->directional.red = *(UINT8 *)FieldValue;
            break;
        case 17: //directionalGreen
            XCLL.Load();
            XCLL->directional.green = *(UINT8 *)FieldValue;
            break;
        case 18: //directionalBlue
            XCLL.Load();
            XCLL->directional.blue = *(UINT8 *)FieldValue;
            break;
        case 19: //unused2
            if(ArraySize != 1)
                break;
            XCLL.Load();
            XCLL->directional.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 20: //fogRed
            XCLL.Load();
            XCLL->fog.red = *(UINT8 *)FieldValue;
            break;
        case 21: //fogGreen
            XCLL.Load();
            XCLL->fog.green = *(UINT8 *)FieldValue;
            break;
        case 22: //fogBlue
            XCLL.Load();
            XCLL->fog.blue = *(UINT8 *)FieldValue;
            break;
        case 23: //unused3
            if(ArraySize != 1)
                break;
            XCLL.Load();
            XCLL->fog.unused1 = ((UINT8ARRAY)FieldValue)[0];
            break;
        case 24: //fogNear
            XCLL.Load();
            XCLL->fogNear = *(FLOAT32 *)FieldValue;
            break;
        case 25: //fogFar
            XCLL.Load();
            XCLL->fogFar = *(FLOAT32 *)FieldValue;
            break;
        case 26: //directionalXY
            XCLL.Load();
            XCLL->directionalXY = *(SINT32 *)FieldValue;
            break;
        case 27: //directionalZ
            XCLL.Load();
            XCLL->directionalZ = *(SINT32 *)FieldValue;
            break;
        case 28: //directionalFade
            XCLL.Load();
            XCLL->directionalFade = *(FLOAT32 *)FieldValue;
            break;
        case 29: //fogClip
            XCLL.Load();
            XCLL->fogClip = *(FLOAT32 *)FieldValue;
            break;
        case 30: //fogPower
            XCLL.Load();
            XCLL->fogPower = *(FLOAT32 *)FieldValue;
            break;
#endif
        case 31: //swappedImpacts
            if(ListFieldID == 0) //swappedImpactsSize
                {
                IMPS.resize(ArraySize);
                return false;
                }

            if(ListIndex >= IMPS.value.size())
                break;

            switch(ListFieldID)
                {
                case 1: //material
                    IMPS.value[ListIndex]->material = *(UINT32 *)FieldValue;
                    break;
                case 2: //oldImpact
                    IMPS.value[ListIndex]->oldImpact = *(FORMID *)FieldValue;
                    return true;
                case 3: //newImpact
                    IMPS.value[ListIndex]->newImpact = *(FORMID *)FieldValue;
                    return true;
                default:
                    break;
                }
            break;
        case 32: //concSolid
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->concSolid[0], 0x00, sizeof(IMPF->concSolid));
            memcpy(&IMPF->concSolid[0], FieldValue, ArraySize);
            break;
        case 33: //concBroken
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->concBroken[0], 0x00, sizeof(IMPF->concBroken));
            memcpy(&IMPF->concBroken[0], FieldValue, ArraySize);
            break;
        case 34: //metalSolid
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->metalSolid[0], 0x00, sizeof(IMPF->metalSolid));
            memcpy(&IMPF->metalSolid[0], FieldValue, ArraySize);
            break;
        case 35: //metalHollow
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->metalHollow[0], 0x00, sizeof(IMPF->metalHollow));
            memcpy(&IMPF->metalHollow[0], FieldValue, ArraySize);
            break;
        case 36: //metalSheet
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->metalSheet[0], 0x00, sizeof(IMPF->metalSheet));
            memcpy(&IMPF->metalSheet[0], FieldValue, ArraySize);
            break;
        case 37: //wood
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->wood[0], 0x00, sizeof(IMPF->wood));
            memcpy(&IMPF->wood[0], FieldValue, ArraySize);
            break;
        case 38: //sand
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->sand[0], 0x00, sizeof(IMPF->sand));
            memcpy(&IMPF->sand[0], FieldValue, ArraySize);
            break;
        case 39: //dirt
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->dirt[0], 0x00, sizeof(IMPF->dirt));
            memcpy(&IMPF->dirt[0], FieldValue, ArraySize);
            break;
        case 40: //grass
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->grass[0], 0x00, sizeof(IMPF->grass));
            memcpy(&IMPF->grass[0], FieldValue, ArraySize);
            break;
        case 41: //water
            if(FieldValue == NULL)
                break;
            ArraySize = (UINT32)strlen((STRING)FieldValue);
            if(ArraySize > 29)
                break;
            IMPF.Load();
            memset(&IMPF->water[0], 0x00, sizeof(IMPF->water));
            memcpy(&IMPF->water[0], FieldValue, ArraySize);
            break;
        case 42: //lightTemplate
            LTMP.value = *(FORMID *)FieldValue;
            return true;
        case 43: //lightFlags
            SetLightFlagMask(*(UINT32 *)FieldValue);
            break;
        case 44: //waterHeight
            XCLW.value = *(FLOAT32 *)FieldValue;
            break;
        case 45: //waterNoisePath
            XNAM.Copy((STRING)FieldValue);
            break;
        case 46: //regions
            XCLR.value.resize(ArraySize);
            for(UINT32 x = 0; x < ArraySize; x++)
                XCLR.value[x] = ((FORMIDARRAY)FieldValue)[x];
            return true;
        case 47: //imageSpace
            XCIM.value = *(FORMID *)FieldValue;
            return true;
        case 48: //xcet_p
            XCET.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 49: //encounterZone
            XEZN.value = *(FORMID *)FieldValue;
            return true;
        case 50: //climate
            XCCM.value = *(FORMID *)FieldValue;
            return true;
        case 51: //water
            XCWT.value = *(FORMID *)FieldValue;
            return true;
#if 0
        case 52: //owner
            Ownership.Load();
            Ownership->XOWN.value = *(FORMID *)FieldValue;
            return true;
        case 53: //rank
            Ownership.Load();
            Ownership->XRNK.Load();
            *Ownership->XRNK.value = *(SINT32 *)FieldValue;
            break;
#endif
        case 54: //acousticSpace
            XCAS.value = *(FORMID *)FieldValue;
            return true;
        case 55: //xcmt_p
            XCMT.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 56: //music
            XCMO.value = *(FORMID *)FieldValue;
            return true;
        default:
            break;
        }
    return false;
    }

void CELLRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    CELLXCLC defaultXCLC;
//  TES5LIGHT defaultXCLL;
    GENIMPS defaultIMPS;
    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(0);
            return;
        case 3: //versionControl1
            flagsUnk = 0;
            return;
        case 4: //eid
            EDID.Unload();
            return;
        case 5: //formVersion
            formVersion = 0;
            return;
        case 6: //versionControl2
            versionControl2[0] = 0;
            versionControl2[1] = 0;
            return;
        case 7: //full
            FULL.Unload();
            return;
        case 8: //flags
            DATA.Unload();
            return;
#if 0
        case 9: //posX
            if(XCLC.IsLoaded())
                XCLC->posX = defaultXCLC.posX;
            return;
        case 10: //posY
            if(XCLC.IsLoaded())
                XCLC->posY = defaultXCLC.posY;
            return;
        case 11: //quadFlags
            if(XCLC.IsLoaded())
                XCLC->flags = defaultXCLC.flags;
            return;
        case 12: //ambientRed
            if(XCLL.IsLoaded())
                XCLL->ambient.red = defaultXCLL.ambient.red;
            return;
        case 13: //ambientGreen
            if(XCLL.IsLoaded())
                XCLL->ambient.green = defaultXCLL.ambient.green;
            return;
        case 14: //ambientBlue
            if(XCLL.IsLoaded())
                XCLL->ambient.blue = defaultXCLL.ambient.blue;
            return;
        case 15: //unused1
            if(XCLL.IsLoaded())
                XCLL->ambient.unused1 = defaultXCLL.ambient.unused1;
            return;
        case 16: //directionalRed
            if(XCLL.IsLoaded())
                XCLL->directional.red = defaultXCLL.directional.red;
            return;
        case 17: //directionalGreen
            if(XCLL.IsLoaded())
                XCLL->directional.green = defaultXCLL.directional.green;
            return;
        case 18: //directionalBlue
            if(XCLL.IsLoaded())
                XCLL->directional.blue = defaultXCLL.directional.blue;
            return;
        case 19: //unused2
            if(XCLL.IsLoaded())
                XCLL->directional.unused1 = defaultXCLL.directional.unused1;
            return;
        case 20: //fogRed
            if(XCLL.IsLoaded())
                XCLL->fog.red = defaultXCLL.fog.red;
            return;
        case 21: //fogGreen
            if(XCLL.IsLoaded())
                XCLL->fog.green = defaultXCLL.fog.green;
            return;
        case 22: //fogBlue
            if(XCLL.IsLoaded())
                XCLL->fog.blue = defaultXCLL.fog.blue;
            return;
        case 23: //unused3
            if(XCLL.IsLoaded())
                XCLL->fog.unused1 = defaultXCLL.fog.unused1;
            return;
        case 24: //fogNear
            if(XCLL.IsLoaded())
                XCLL->fogNear = defaultXCLL.fogNear;
            return;
        case 25: //fogFar
            if(XCLL.IsLoaded())
                XCLL->fogFar = defaultXCLL.fogFar;
            return;
        case 26: //directionalXY
            if(XCLL.IsLoaded())
                XCLL->directionalXY = defaultXCLL.directionalXY;
            return;
        case 27: //directionalZ
            if(XCLL.IsLoaded())
                XCLL->directionalZ = defaultXCLL.directionalZ;
            return;
        case 28: //directionalFade
            if(XCLL.IsLoaded())
                XCLL->directionalFade = defaultXCLL.directionalFade;
            return;
        case 29: //fogClip
            if(XCLL.IsLoaded())
                XCLL->fogClip = defaultXCLL.fogClip;
            return;
        case 30: //fogPower
            if(XCLL.IsLoaded())
                XCLL->fogPower = defaultXCLL.fogPower;
            return;
#endif
        case 31: //swappedImpacts
            if(ListFieldID == 0) //swappedImpactsSize
                {
                IMPS.Unload();
                return;
                }

            if(ListIndex >= IMPS.value.size())
                return;

            switch(ListFieldID)
                {
                case 1: //material
                    IMPS.value[ListIndex]->material = defaultIMPS.material;
                    return;
                case 2: //oldImpact
                    IMPS.value[ListIndex]->oldImpact = defaultIMPS.oldImpact;
                    return;
                case 3: //newImpact
                    IMPS.value[ListIndex]->newImpact = defaultIMPS.newImpact;
                    return;
                default:
                    return;
                }
            return;
        case 32: //concSolid
            if(IMPF.IsLoaded())
                memset(&IMPF->concSolid[0], 0x00, sizeof(IMPF->concSolid));
            return;
        case 33: //concBroken
            if(IMPF.IsLoaded())
                memset(&IMPF->concBroken[0], 0x00, sizeof(IMPF->concBroken));
            return;
        case 34: //metalSolid
            if(IMPF.IsLoaded())
                memset(&IMPF->metalSolid[0], 0x00, sizeof(IMPF->metalSolid));
            return;
        case 35: //metalHollow
            if(IMPF.IsLoaded())
                memset(&IMPF->metalHollow[0], 0x00, sizeof(IMPF->metalHollow));
            return;
        case 36: //metalSheet
            if(IMPF.IsLoaded())
                memset(&IMPF->metalSheet[0], 0x00, sizeof(IMPF->metalSheet));
            return;
        case 37: //wood
            if(IMPF.IsLoaded())
                memset(&IMPF->wood[0], 0x00, sizeof(IMPF->wood));
            return;
        case 38: //sand
            if(IMPF.IsLoaded())
                memset(&IMPF->sand[0], 0x00, sizeof(IMPF->sand));
            return;
        case 39: //dirt
            if(IMPF.IsLoaded())
                memset(&IMPF->dirt[0], 0x00, sizeof(IMPF->dirt));
            return;
        case 40: //grass
            if(IMPF.IsLoaded())
                memset(&IMPF->grass[0], 0x00, sizeof(IMPF->grass));
            return;
        case 41: //water
            if(IMPF.IsLoaded())
                memset(&IMPF->water[0], 0x00, sizeof(IMPF->water));
            return;
        case 42: //lightTemplate
            LTMP.Unload();
            return;
        case 43: //lightFlags
            LNAM.Unload();
            return;
        case 44: //waterHeight
            XCLW.Unload();
            return;
        case 45: //waterNoisePath
            XNAM.Unload();
            return;
        case 46: //regions
            XCLR.Unload();
            return;
        case 47: //imageSpace
            XCIM.Unload();
            return;
        case 48: //xcet_p
            XCET.Unload();
            return;
        case 49: //encounterZone
            XEZN.Unload();
            return;
        case 50: //climate
            XCCM.Unload();
            return;
        case 51: //water
            XCWT.Unload();
            return;
#if 0
        case 52: //owner
            if(Ownership.IsLoaded())
                Ownership->XOWN.Unload();
            return;
        case 53: //rank
            if(Ownership.IsLoaded())
                Ownership->XRNK.Unload();
            return;
#endif
        case 54: //acousticSpace
            XCAS.Unload();
            return;
        case 55: //xcmt_p
            XCMT.Unload();
            return;
        case 56: //music
            XCMO.Unload();
            return;
        default:
            return;
        }
    }
}