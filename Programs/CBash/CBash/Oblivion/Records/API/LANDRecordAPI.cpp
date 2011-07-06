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
#include "..\LANDRecord.h"

UINT32 LANDRecord::GetFieldAttribute(FIELD_IDENTIFIERS, UINT32 WhichAttribute)
    {
    //normals, heights, and colors are accessed as if they were a list of lists
    //They aren't true lists, but rather 33x33 arrays
    //So there are some inconsistencies when compared to true lists of lists
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
        case 5: //data
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return DATA.GetSize();
                default:
                    return UNKNOWN_FIELD;
                }
        case 6: //normals
            if(ListFieldID == 0) //normals
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return ListIndex < 33 ? 33 : 0;
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListX2FieldID == 0) //normals
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return ListIndex < 33 ? 33 : 0;
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= 33)
                return UNKNOWN_FIELD;

            if(ListX2Index >= 33)
                return UNKNOWN_FIELD;

            switch(ListX2FieldID)
                {
                case 1: //x
                    return UINT8_FIELD;
                case 2: //y
                    return UINT8_FIELD;
                case 3: //z
                    return UINT8_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 7: //heightOffset
            return FLOAT32_FIELD;
        case 8: //heights
            if(ListFieldID == 0) //heights
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return ListIndex < 33 ? 33 : 0;
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListX2FieldID == 0) //heights
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return ListIndex < 33 ? 33 : 0;
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= 33)
                return UNKNOWN_FIELD;

            if(ListX2Index >= 33)
                return UNKNOWN_FIELD;

            switch(ListX2FieldID)
                {
                case 1: //height
                    return SINT8_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 9: //unused1
            switch(WhichAttribute)
                {
                case 0: //fieldType
                    return UINT8_ARRAY_FIELD;
                case 1: //fieldSize
                    return VHGT.IsLoaded() ? 3 : 0;
                default:
                    return UNKNOWN_FIELD;
                }
        case 10: //colors
            if(ListFieldID == 0) //colors
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return ListIndex < 33 ? 33 : 0;
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListX2FieldID == 0) //colors
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return ListIndex < 33 ? 33 : 0;
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= 33)
                return UNKNOWN_FIELD;

            if(ListX2Index >= 33)
                return UNKNOWN_FIELD;

            switch(ListX2FieldID)
                {
                case 1: //red
                    return UINT8_FIELD;
                case 2: //green
                    return UINT8_FIELD;
                case 3: //blue
                    return UINT8_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 11: //baseTextures
            if(ListFieldID == 0) //baseTextures
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)BTXT.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= BTXT.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //texture
                    return FORMID_FIELD;
                case 2: //quadrant
                    return SINT8_FIELD;
                case 3: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 1;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 4: //layer
                    return SINT16_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 12: //alphaLayers
            if(ListFieldID == 0) //alphaLayers
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)Layers.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= Layers.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //texture
                    return FORMID_FIELD;
                case 2: //quadrant
                    return SINT8_FIELD;
                case 3: //unused1
                    switch(WhichAttribute)
                        {
                        case 0: //fieldType
                            return UINT8_ARRAY_FIELD;
                        case 1: //fieldSize
                            return 1;
                        default:
                            return UNKNOWN_FIELD;
                        }
                case 4: //layer
                    return SINT16_FIELD;
                case 5: //opacities
                    if(ListX2FieldID == 0) //opacities
                        {
                        switch(WhichAttribute)
                            {
                            case 0: //fieldType
                                return LIST_FIELD;
                            case 1: //fieldSize
                                return (UINT32)Layers[ListIndex]->VTXT.size();
                            default:
                                return UNKNOWN_FIELD;
                            }
                        }

                    if(ListX2Index >= Layers[ListIndex]->VTXT.size())
                        return UNKNOWN_FIELD;

                    switch(ListX2FieldID)
                        {
                        case 1: //position
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
                        case 3: //opacity
                            return FLOAT32_FIELD;
                        default:
                            return UNKNOWN_FIELD;
                        }
                default:
                    return UNKNOWN_FIELD;
                }
        case 13: //vertexTextures
            if(ListFieldID == 0) //vertexTextures
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return (UINT32)VTEX.size();
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= VTEX.size())
                return UNKNOWN_FIELD;

            switch(ListFieldID)
                {
                case 1: //texture
                    return FORMID_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        case 14: //Position
            if(ListFieldID == 0) //Position
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return ListIndex < 33 ? 33 : 0;
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListX2FieldID == 0) //Position
                {
                switch(WhichAttribute)
                    {
                    case 0: //fieldType
                        return LIST_FIELD;
                    case 1: //fieldSize
                        return ListIndex < 33 ? 33 : 0;
                    default:
                        return UNKNOWN_FIELD;
                    }
                }

            if(ListIndex >= 33)
                return UNKNOWN_FIELD;

            if(ListX2Index >= 33)
                return UNKNOWN_FIELD;

            switch(ListX2FieldID)
                {
                case 1: //height
                    return FLOAT32_FIELD;
                case 2: //normalX
                    return UINT8_FIELD;
                case 3: //normalY
                    return UINT8_FIELD;
                case 4: //normalZ
                    return UINT8_FIELD;
                case 5: //red
                    return UINT8_FIELD;
                case 6: //green
                    return UINT8_FIELD;
                case 7: //blue
                    return UINT8_FIELD;
                case 8: //baseTexture
                    return FORMID_FIELD;
                case 9: //alphaLayer1Texture
                    return FORMID_FIELD;
                case 10: //alphaLayer1Opacity
                    return FLOAT32_FIELD;
                case 11: //alphaLayer2Texture
                    return FORMID_FIELD;
                case 12: //alphaLayer2Opacity
                    return FLOAT32_FIELD;
                case 13: //alphaLayer3Texture
                    return FORMID_FIELD;
                case 14: //alphaLayer3Opacity
                    return FLOAT32_FIELD;
                case 15: //alphaLayer4Texture
                    return FORMID_FIELD;
                case 16: //alphaLayer4Opacity
                    return FLOAT32_FIELD;
                case 17: //alphaLayer5Texture
                    return FORMID_FIELD;
                case 18: //alphaLayer5Opacity
                    return FLOAT32_FIELD;
                case 19: //alphaLayer6Texture
                    return FORMID_FIELD;
                case 20: //alphaLayer6Opacity
                    return FLOAT32_FIELD;
                case 21: //alphaLayer7Texture
                    return FORMID_FIELD;
                case 22: //alphaLayer7Opacity
                    return FLOAT32_FIELD;
                case 23: //alphaLayer8Texture
                    return FORMID_FIELD;
                case 24: //alphaLayer8Opacity
                    return FLOAT32_FIELD;
                default:
                    return UNKNOWN_FIELD;
                }
        default:
            return UNKNOWN_FIELD;
        }
    }

void * LANDRecord::GetField(FIELD_IDENTIFIERS, void **FieldValues)
    {
    UINT8  curQuadrant;
    UINT16 curPosition;
    static FLOAT32 fRetValue;

    switch(FieldID)
        {
        case 1: //flags1
            return &flags;
        case 2: //fid
            return &formID;
        case 3: //flags2
            return &flagsUnk;
        case 5: //data
            *FieldValues = DATA.value;
            return NULL;
        case 6: //normals
            if(ListIndex >= 33)
                return NULL;

            if(ListX2Index >= 33)
                return NULL;

            switch(ListX2FieldID)
                {
                case 1: //x
                    return VNML.IsLoaded() ? &VNML->VNML[ListIndex][ListX2Index].x : NULL;
                case 2: //y
                    return VNML.IsLoaded() ? &VNML->VNML[ListIndex][ListX2Index].y : NULL;
                case 3: //z
                    return VNML.IsLoaded() ? &VNML->VNML[ListIndex][ListX2Index].z : NULL;
                default:
                    return NULL;
                }
        case 7: //heightOffset
            return VHGT.IsLoaded() ? &VHGT->offset : NULL;
        case 8: //heights
            if(ListIndex >= 33)
                return NULL;

            if(ListX2Index >= 33)
                return NULL;

            switch(ListX2FieldID)
                {
                case 1: //height
                    return VHGT.IsLoaded() ? &VHGT->VHGT[ListIndex][ListX2Index] : NULL;
                default:
                    return NULL;
                }
        case 9: //unused1
            *FieldValues = VHGT.IsLoaded() ? &VHGT->unused1[0] : NULL;
            return NULL;
        case 10: //colors
            if(ListIndex >= 33)
                return NULL;

            if(ListX2Index >= 33)
                return NULL;

            switch(ListX2FieldID)
                {
                case 1: //red
                    return VCLR.IsLoaded() ? &VCLR->VCLR[ListIndex][ListX2Index].red : NULL;
                case 2: //green
                    return VCLR.IsLoaded() ? &VCLR->VCLR[ListIndex][ListX2Index].green : NULL;
                case 3: //blue
                    return VCLR.IsLoaded() ? &VCLR->VCLR[ListIndex][ListX2Index].blue : NULL;
                default:
                    return NULL;
                }
        case 11: //baseTextures
            if(ListIndex >= BTXT.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //texture
                    return &BTXT[ListIndex]->value.texture;
                case 2: //quadrant
                    return &BTXT[ListIndex]->value.quadrant;
                case 3: //unused1
                    *FieldValues = &BTXT[ListIndex]->value.unused1;
                    return NULL;
                case 4: //layer
                    return &BTXT[ListIndex]->value.layer;
                default:
                    *FieldValues = NULL;
                    return NULL;
                }
        case 12: //alphaLayers
            if(ListIndex >= Layers.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //texture
                    return &Layers[ListIndex]->ATXT.value.texture;
                case 2: //quadrant
                    return &Layers[ListIndex]->ATXT.value.quadrant;
                case 3: //unused1
                    *FieldValues = &Layers[ListIndex]->ATXT.value.unused1;
                    return NULL;
                case 4: //layer
                    return &Layers[ListIndex]->ATXT.value.layer;
                case 5: //opacities
                    if(ListX2Index >= Layers[ListIndex]->VTXT.size())
                        return NULL;

                    switch(ListX2FieldID)
                        {
                        case 1: //position
                            return &Layers[ListIndex]->VTXT[ListX2Index].position;
                        case 2: //unused1
                            *FieldValues = &Layers[ListIndex]->VTXT[ListX2Index].unused1[0];
                            return NULL;
                        case 3: //opacity
                            return &Layers[ListIndex]->VTXT[ListX2Index].opacity;
                        default:
                            *FieldValues = NULL;
                            return NULL;
                        }
                default:
                    *FieldValues = NULL;
                    return NULL;
                }
        case 13: //vertexTextures
            if(ListIndex >= VTEX.size())
                return NULL;

            switch(ListFieldID)
                {
                case 1: //texture
                    return &VTEX[ListFieldID];
                default:
                    return NULL;
                }
        case 14: //Position
            if(ListIndex >= 33)
                return NULL;

            if(ListX2Index >= 33)
                return NULL;

            switch(ListX2FieldID)
                {
                case 1: //height
                    if(VHGT.IsLoaded())
                        {
                        fRetValue = CalcHeight(ListIndex, ListX2Index);
                        return &fRetValue;
                        }
                    return NULL;
                case 2: //normalX
                    return VNML.IsLoaded() ? &VNML->VNML[ListIndex][ListX2Index].x : NULL;
                case 3: //normalY
                    return VNML.IsLoaded() ? &VNML->VNML[ListIndex][ListX2Index].y : NULL;
                case 4: //normalZ
                    return VNML.IsLoaded() ? &VNML->VNML[ListIndex][ListX2Index].z : NULL;
                case 5: //red
                    return VCLR.IsLoaded() ? &VCLR->VCLR[ListIndex][ListX2Index].red : NULL;
                case 6: //green
                    return VCLR.IsLoaded() ? &VCLR->VCLR[ListIndex][ListX2Index].green : NULL;
                case 7: //blue
                    return VCLR.IsLoaded() ? &VCLR->VCLR[ListIndex][ListX2Index].blue : NULL;
                case 8: //baseTexture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < BTXT.size(); ++x)
                        if(BTXT[x]->value.quadrant == curQuadrant)
                            return &BTXT[x]->value.texture;
                    return NULL;
                case 9: //alphaLayer1Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 0)
                            return &Layers[x]->ATXT.value.texture;
                    return NULL;
                case 10: //alphaLayer1Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 0)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    return &Layers[x]->VTXT[y].opacity;
                    return NULL;
                case 11: //alphaLayer2Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 1)
                            return &Layers[x]->ATXT.value.texture;
                    return NULL;
                case 12: //alphaLayer2Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 1)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    return &Layers[x]->VTXT[y].opacity;
                    return NULL;
                case 13: //alphaLayer3Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 2)
                            return &Layers[x]->ATXT.value.texture;
                    return NULL;
                case 14: //alphaLayer3Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 2)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    return &Layers[x]->VTXT[y].opacity;
                    return NULL;
                case 15: //alphaLayer4Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 3)
                            return &Layers[x]->ATXT.value.texture;
                    return NULL;
                case 16: //alphaLayer4Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 3)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    return &Layers[x]->VTXT[y].opacity;
                    return NULL;
                case 17: //alphaLayer5Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 4)
                            return &Layers[x]->ATXT.value.texture;
                    return NULL;
                case 18: //alphaLayer5Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 4)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    return &Layers[x]->VTXT[y].opacity;
                    return NULL;
                case 19: //alphaLayer6Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 5)
                            return &Layers[x]->ATXT.value.texture;
                    return NULL;
                case 20: //alphaLayer6Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 5)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    return &Layers[x]->VTXT[y].opacity;
                    return NULL;
                case 21: //alphaLayer7Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 6)
                            return &Layers[x]->ATXT.value.texture;
                    return NULL;
                case 22: //alphaLayer7Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 6)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    return &Layers[x]->VTXT[y].opacity;
                    return NULL;
                case 23: //alphaLayer8Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 7)
                            return &Layers[x]->ATXT.value.texture;
                    return NULL;
                case 24: //alphaLayer8Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 7)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    return &Layers[x]->VTXT[y].opacity;
                    return NULL;
                default:
                    return NULL;
                }
        default:
            return NULL;
        }
    }

bool LANDRecord::SetField(FIELD_IDENTIFIERS, void *FieldValue, UINT32 ArraySize)
    {
    SINT8    newOffset;
    UINT8    curQuadrant;
    UINT16   curPosition;
    LANDVTXT curVTXT;
    ReqSubRecord<LANDGENTXT> *curTexture;
    LANDLAYERS *curLayer;

    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(*(UINT32 *)FieldValue);
            break;
        case 3: //flags2
            SetHeaderUnknownFlagMask(*(UINT32 *)FieldValue);
            break;
        case 5: //data
            DATA.Copy((UINT8ARRAY)FieldValue, ArraySize);
            break;
        case 6: //normals
            if(ListFieldID == 0) //normalsSize, not really a list so it can't be resized
                break;

            if(ListIndex >= 33)
                break;

            if(ListX2Index >= 33)
                break;

            switch(ListX2FieldID)
                {
                case 1: //x
                    VNML.Load();
                    VNML->VNML[ListIndex][ListX2Index].x = *(UINT8 *)FieldValue;
                    break;
                case 2: //y
                    VNML.Load();
                    VNML->VNML[ListIndex][ListX2Index].y = *(UINT8 *)FieldValue;
                    break;
                case 3: //z
                    VNML.Load();
                    VNML->VNML[ListIndex][ListX2Index].z = *(UINT8 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 7: //heightOffset
            VHGT.Load();
            VHGT->offset = *(FLOAT32 *)FieldValue;
            break;
        case 8: //heights
            if(ListFieldID == 0) //heightsSize, not really a list so it can't be resized
                break;

            if(ListIndex >= 33)
                break;

            if(ListX2Index >= 33)
                break;

            switch(ListX2FieldID)
                {
                case 1: //height
                    VHGT.Load();
                    VHGT->VHGT[ListIndex][ListX2Index] = *(SINT8 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 9: //unused1
            if(ArraySize != 3)
                break;
            VHGT.Load();
            VHGT->unused1[0] = ((UINT8ARRAY)FieldValue)[0];
            VHGT->unused1[1] = ((UINT8ARRAY)FieldValue)[1];
            VHGT->unused1[2] = ((UINT8ARRAY)FieldValue)[2];
            break;
        case 10: //colors
            if(ListFieldID == 0) //colorsSize, not really a list so it can't be resized
                break;

            if(ListIndex >= 33)
                break;

            if(ListX2Index >= 33)
                break;

            switch(ListX2FieldID)
                {
                case 1: //red
                    VCLR.Load();
                    VCLR->VCLR[ListIndex][ListX2Index].red = *(UINT8 *)FieldValue;
                    break;
                case 2: //green
                    VCLR.Load();
                    VCLR->VCLR[ListIndex][ListX2Index].green = *(UINT8 *)FieldValue;
                    break;
                case 3: //blue
                    VCLR.Load();
                    VCLR->VCLR[ListIndex][ListX2Index].blue = *(UINT8 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 11: //baseTextures
            if(ListFieldID == 0) //baseTexturesSize
                {
                ArraySize -= (UINT32)BTXT.size();
                while((SINT32)ArraySize > 0)
                    {
                    BTXT.push_back(new ReqSubRecord<LANDGENTXT>);
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete BTXT.back();
                    BTXT.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= BTXT.size())
                break;

            switch(ListFieldID)
                {
                case 1: //texture
                    BTXT[ListIndex]->value.texture = *(FORMID *)FieldValue;
                    return true;
                case 2: //quadrant
                    BTXT[ListIndex]->value.quadrant = *(UINT8 *)FieldValue;
                    break;
                case 3: //unused1
                    if(ArraySize != 1)
                        break;
                    BTXT[ListIndex]->value.unused1 = ((UINT8ARRAY)FieldValue)[0];
                    break;
                case 4: //layer
                    BTXT[ListIndex]->value.layer = *(SINT16 *)FieldValue;
                    break;
                default:
                    break;
                }
            break;
        case 12: //alphaLayers
            if(ListFieldID == 0) //alphaLayersSize
                {
                ArraySize -= (UINT32)Layers.size();
                while((SINT32)ArraySize > 0)
                    {
                    Layers.push_back(new LANDLAYERS());
                    --ArraySize;
                    }
                while((SINT32)ArraySize < 0)
                    {
                    delete Layers.back();
                    Layers.pop_back();
                    ++ArraySize;
                    }
                return false;
                }

            if(ListIndex >= Layers.size())
                break;

            switch(ListFieldID)
                {
                case 1: //texture
                    Layers[ListIndex]->ATXT.value.texture = *(FORMID *)FieldValue;
                    return true;
                case 2: //quadrant
                    Layers[ListIndex]->ATXT.value.quadrant = *(UINT8 *)FieldValue;
                    break;
                case 3: //unused1
                    if(ArraySize != 1)
                        break;
                    Layers[ListIndex]->ATXT.value.unused1 = ((UINT8ARRAY)FieldValue)[0];
                    break;
                case 4: //layer
                    Layers[ListIndex]->ATXT.value.layer = *(SINT16 *)FieldValue;
                    break;
                case 5: //opacities
                    if(ListX2Index >= Layers[ListIndex]->VTXT.size())
                        break;

                    switch(ListX2FieldID)
                        {
                        case 1: //position
                            Layers[ListIndex]->VTXT[ListX2Index].position = *(UINT16 *)FieldValue;
                            break;
                        case 2: //unused1
                            if(ArraySize != 2)
                                break;
                            Layers[ListIndex]->VTXT[ListX2Index].unused1[0] = ((UINT8ARRAY)FieldValue)[0];
                            Layers[ListIndex]->VTXT[ListX2Index].unused1[1] = ((UINT8ARRAY)FieldValue)[1];
                            break;
                        case 3: //opacity
                            Layers[ListIndex]->VTXT[ListX2Index].opacity = *(FLOAT32 *)FieldValue;
                            break;
                        default:
                            break;
                        }
                    break;
                default:
                    break;
                }
            break;
        case 13: //vertexTextures
            if(ListFieldID == 0) //vertexTexturesSize
                {
                VTEX.resize(ArraySize);
                return false;
                }

            if(ListIndex >= VTEX.size())
                break;

            switch(ListFieldID)
                {
                case 1: //texture
                    VTEX[ListIndex] = *(FORMID *)FieldValue;
                    return true;
                default:
                    break;
                }
            break;
        case 14: //Position
            if(ListFieldID == 0) //PositionSize, not really a list so can't resize
                break;

            if(ListIndex >= 33)
                break;

            if(ListX2Index >= 33)
                break;

            switch(ListX2FieldID)
                {
                case 1: //height
                    //Changing height is tricky because the co-ords to the east and possibly north have to be updated as well
                    //These other co-ords may be in other cells.  Example: the corners of the cell must update four different VHGT records
                    //If any of the cells happen to be 0,0, then the baseHeight offset of that cell must be changed instead
                    //The borders of the cells must match the height of the surrounding cells or there will be visual tearing
                    VHGT.Load();
                    newOffset = (SINT8)((*(FLOAT32 *)FieldValue - CalcHeight(ListIndex, ListX2Index)) / 8.0f);

                    //Set the co-ords to the proper offset.  If the co-ords are 0,0, then change the baseHeight offset instead
                    if(ListIndex == 0 && ListX2Index == 0)
                        {
                        VHGT->VHGT[0][0] = 0;
                        VHGT->offset = *(FLOAT32 *)FieldValue;
                        }
                    else
                        VHGT->VHGT[ListIndex][ListX2Index] += newOffset;

                    //Update the co-ords to the east
                    if(ListX2Index == 32) //the co-ords to update are in the next cell to the east
                        {
                        if(ListIndex == 0)
                            {
                            //At a corner, so update the three overlapped cells
                            if(EastLand != NULL)
                                {
                                EastLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - EastLand->VHGT->offset) / 8.0f);
                                EastLand->VHGT->VHGT[0][0] = 0;
                                EastLand->VHGT->offset = *(FLOAT32 *)FieldValue;
                                EastLand->VHGT->VHGT[0][1] -= newOffset;
                                EastLand->VHGT->VHGT[1][0] -= newOffset;
                                }

                            if(EastLand != NULL && EastLand->SouthLand != NULL)
                                {
                                EastLand->SouthLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - EastLand->SouthLand->CalcHeight(32, 0)) / 8.0f);
                                EastLand->SouthLand->VHGT->VHGT[32][0] += newOffset;
                                EastLand->SouthLand->VHGT->VHGT[32][1] -= newOffset;
                                }
                            else if(SouthLand != NULL && SouthLand->EastLand != NULL)
                                {
                                SouthLand->EastLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - SouthLand->EastLand->CalcHeight(32, 0)) / 8.0f);
                                SouthLand->EastLand->VHGT->VHGT[32][0] += newOffset;
                                SouthLand->EastLand->VHGT->VHGT[32][1] -= newOffset;
                                }

                            if(SouthLand != NULL)
                                {
                                SouthLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - SouthLand->CalcHeight(32, 32)) / 8.0f);
                                SouthLand->VHGT->VHGT[32][32] += newOffset;
                                }
                            }
                        else if(ListIndex == 32)
                            {
                            //At a corner, so update the three overlapped cells
                            if(EastLand != NULL)
                                {
                                EastLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - EastLand->CalcHeight(32, 0)) / 8.0f);
                                EastLand->VHGT->VHGT[32][0] += newOffset;
                                EastLand->VHGT->VHGT[32][1] -= newOffset;
                                }

                            //Two ways of getting to the same cell
                            if(EastLand != NULL && EastLand->NorthLand != NULL)
                                {
                                EastLand->NorthLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - EastLand->NorthLand->VHGT->offset) / 8.0f);
                                EastLand->NorthLand->VHGT->VHGT[0][0] = 0;
                                EastLand->NorthLand->VHGT->offset = *(FLOAT32 *)FieldValue;
                                EastLand->NorthLand->VHGT->VHGT[0][1] -= newOffset;
                                EastLand->NorthLand->VHGT->VHGT[1][0] -= newOffset;
                                }
                            else if(NorthLand != NULL && NorthLand->EastLand != NULL)
                                {
                                NorthLand->EastLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - NorthLand->EastLand->VHGT->offset) / 8.0f);
                                NorthLand->EastLand->VHGT->VHGT[0][0] = 0;
                                NorthLand->EastLand->VHGT->offset = *(FLOAT32 *)FieldValue;
                                NorthLand->EastLand->VHGT->VHGT[0][1] -= newOffset;
                                NorthLand->EastLand->VHGT->VHGT[1][0] -= newOffset;
                                }

                            if(NorthLand != NULL)
                                {
                                NorthLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - NorthLand->CalcHeight(0, 32)) / 8.0f);
                                NorthLand->VHGT->VHGT[0][32] += newOffset;
                                }
                            }
                        else
                            {
                            //In between the corners of the eastern edge, so update overlapped cell
                            if(EastLand != NULL)
                                {
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - EastLand->CalcHeight(ListIndex, 0)) / 8.0f);
                                EastLand->VHGT->VHGT[ListIndex][0] += newOffset;

                                //Then, update the co-ords to the east and north.
                                EastLand->VHGT->VHGT[ListIndex][1] -= newOffset;
                                EastLand->VHGT->VHGT[ListIndex + 1][0] -= newOffset;
                                }
                            }
                        }
                    //Update the co-ords to the north, if needed
                    else if(ListX2Index == 0) //first column is offset by the values to the south
                        {
                        if(ListIndex == 0)
                            {
                            VHGT->VHGT[0][1] -= newOffset;
                            VHGT->VHGT[1][0] -= newOffset;

                            //At a corner, so update the three overlapped cells
                            if(SouthLand != NULL)
                                {
                                SouthLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - SouthLand->CalcHeight(32, 0)) / 8.0f);
                                SouthLand->VHGT->VHGT[32][0] += newOffset;
                                SouthLand->VHGT->VHGT[32][1] -= newOffset;
                                }

                            if(SouthLand != NULL && SouthLand->WestLand != NULL)
                                {
                                SouthLand->WestLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - SouthLand->WestLand->CalcHeight(32, 32)) / 8.0f);
                                SouthLand->WestLand->VHGT->VHGT[32][32] += newOffset;
                                }
                            else if(WestLand != NULL && WestLand->SouthLand != NULL)
                                {
                                WestLand->SouthLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - WestLand->SouthLand->CalcHeight(32, 32)) / 8.0f);
                                WestLand->SouthLand->VHGT->VHGT[32][32] += newOffset;
                                }

                            if(WestLand != NULL)
                                {
                                WestLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - WestLand->CalcHeight(0, 32)) / 8.0f);
                                WestLand->VHGT->VHGT[0][32] += newOffset;
                                }

                            if(EastLand != NULL)
                                {
                                EastLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - EastLand->VHGT->offset) / 8.0f);
                                EastLand->VHGT->VHGT[0][0] = 0;
                                EastLand->VHGT->offset = *(FLOAT32 *)FieldValue;
                                EastLand->VHGT->VHGT[0][1] -= newOffset;
                                EastLand->VHGT->VHGT[1][0] -= newOffset;
                                }
                            }
                        else if(ListIndex == 32)
                            {
                            VHGT->VHGT[32][1] -= newOffset;

                            //At a corner, so update the three overlapped cells
                            if(WestLand != NULL)
                                {
                                WestLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - WestLand->CalcHeight(32, 32)) / 8.0f);
                                WestLand->VHGT->VHGT[32][32] += newOffset;
                                }

                            if(NorthLand != NULL)
                                {
                                NorthLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - NorthLand->VHGT->offset) / 8.0f);
                                NorthLand->VHGT->VHGT[0][0] = 0;
                                NorthLand->VHGT->offset = *(FLOAT32 *)FieldValue;
                                NorthLand->VHGT->VHGT[0][1] -= newOffset;
                                NorthLand->VHGT->VHGT[1][0] -= newOffset;
                                }

                            if(NorthLand != NULL && NorthLand->WestLand != NULL)
                                {
                                NorthLand->WestLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - NorthLand->WestLand->CalcHeight(0, 32)) / 8.0f);
                                NorthLand->WestLand->VHGT->VHGT[0][32] += newOffset;
                                }
                            else if(WestLand != NULL && WestLand->NorthLand != NULL)
                                {
                                WestLand->NorthLand->VHGT.Load();
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - WestLand->NorthLand->CalcHeight(0, 32)) / 8.0f);
                                WestLand->NorthLand->VHGT->VHGT[0][32] += newOffset;
                                }
                            }
                        else
                            {
                            VHGT->VHGT[ListIndex][1] -= newOffset;
                            VHGT->VHGT[ListIndex + 1][0] -= newOffset;
                            //In between the corners of the western edge, so update overlapped cell
                            if(WestLand != NULL)
                                {
                                newOffset = (SINT8)((*(FLOAT32 *)FieldValue - WestLand->CalcHeight(ListIndex, 32)) / 8.0f);
                                WestLand->VHGT->VHGT[ListIndex][32] += newOffset;
                                }
                            }
                        }
                    //Update the co-ords to the south, if needed
                    else if(ListIndex == 0)
                        {
                        VHGT->VHGT[0][ListX2Index + 1] -= newOffset;
                        //In between the corners of the western edge, so update overlapped cell
                        if(SouthLand != NULL)
                            {
                            newOffset = (SINT8)((*(FLOAT32 *)FieldValue - SouthLand->CalcHeight(32, ListX2Index)) / 8.0f);
                            SouthLand->VHGT->VHGT[32][ListX2Index] += newOffset;
                            SouthLand->VHGT->VHGT[32][ListX2Index + 1] -= newOffset;
                            }
                        }
                    //Update the co-ords to the north, if needed
                    else if(ListIndex == 32)
                        {
                        VHGT->VHGT[0][ListX2Index + 1] -= newOffset;
                        //In between the corners of the western edge, so update overlapped cell
                        if(NorthLand != NULL)
                            {
                            newOffset = (SINT8)((*(FLOAT32 *)FieldValue - NorthLand->CalcHeight(0, ListX2Index)) / 8.0f);
                            NorthLand->VHGT->VHGT[0][ListX2Index] += newOffset;
                            NorthLand->VHGT->VHGT[0][ListX2Index + 1] -= newOffset;
                            }
                        }
                    else //the co-ords are contained within the cell
                        VHGT->VHGT[ListIndex][ListX2Index + 1] -= newOffset;
                    break;
                case 2: //normalX
                    VNML.Load();
                    VNML->VNML[ListIndex][ListX2Index].x = *(UINT8 *)FieldValue;
                    break;
                case 3: //normalY
                    VNML.Load();
                    VNML->VNML[ListIndex][ListX2Index].y = *(UINT8 *)FieldValue;
                    break;
                case 4: //normalZ
                    VNML.Load();
                    VNML->VNML[ListIndex][ListX2Index].z = *(UINT8 *)FieldValue;
                    break;
                case 5: //red
                    VCLR.Load();
                    VCLR->VCLR[ListIndex][ListX2Index].red = *(UINT8 *)FieldValue;
                    break;
                case 6: //green
                    VCLR.Load();
                    VCLR->VCLR[ListIndex][ListX2Index].green = *(UINT8 *)FieldValue;
                    break;
                case 7: //blue
                    VCLR.Load();
                    VCLR->VCLR[ListIndex][ListX2Index].blue = *(UINT8 *)FieldValue;
                    break;

                case 8: //baseTexture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < BTXT.size(); ++x)
                        if(BTXT[x]->value.quadrant == curQuadrant)
                            {
                            BTXT[x]->value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    //No existing BTXT, so make one if able
                    if(BTXT.size() > 3)
                        break;
                    curTexture = new ReqSubRecord<LANDGENTXT>;
                    curTexture->value.layer = -1;
                    curTexture->value.quadrant = curQuadrant;
                    curTexture->value.texture = *(FORMID *)FieldValue;
                    BTXT.push_back(curTexture);
                    return true;
                case 9: //alphaLayer1Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 0)
                            {
                            Layers[x]->ATXT.value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    curLayer = new LANDLAYERS;
                    curLayer->ATXT.value.layer = 0;
                    curLayer->ATXT.value.quadrant = curQuadrant;
                    curLayer->ATXT.value.texture = *(FORMID *)FieldValue;
                    Layers.push_back(curLayer);
                    return true;
                case 10: //alphaLayer1Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 0)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = *(FLOAT32 *)FieldValue;
                                    return false;
                                    }
                    //No existing VTXT, so make one
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 0)
                            {
                            curVTXT.position = curPosition;
                            curVTXT.opacity = *(FLOAT32 *)FieldValue;
                            Layers[x]->VTXT.push_back(curVTXT);
                            return false;
                            }
                    //No existing ATXT, so do nothing
                    break;
                case 11: //alphaLayer2Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 1)
                            {
                            Layers[x]->ATXT.value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    curLayer = new LANDLAYERS;
                    curLayer->ATXT.value.layer = 1;
                    curLayer->ATXT.value.quadrant = curQuadrant;
                    curLayer->ATXT.value.texture = *(FORMID *)FieldValue;
                    Layers.push_back(curLayer);
                    return true;
                case 12: //alphaLayer2Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 1)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = *(FLOAT32 *)FieldValue;
                                    return false;
                                    }
                    //No existing VTXT, so make one
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 1)
                            {
                            curVTXT.position = curPosition;
                            curVTXT.opacity = *(FLOAT32 *)FieldValue;
                            Layers[x]->VTXT.push_back(curVTXT);
                            return false;
                            }
                    //No existing ATXT, so do nothing
                    return false;
                case 13: //alphaLayer3Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 2)
                            {
                            Layers[x]->ATXT.value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    curLayer = new LANDLAYERS;
                    curLayer->ATXT.value.layer = 2;
                    curLayer->ATXT.value.quadrant = curQuadrant;
                    curLayer->ATXT.value.texture = *(FORMID *)FieldValue;
                    Layers.push_back(curLayer);
                    return true;
                case 14: //alphaLayer3Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 2)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = *(FLOAT32 *)FieldValue;
                                    return false;
                                    }
                    //No existing VTXT, so make one
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 2)
                            {
                            curVTXT.position = curPosition;
                            curVTXT.opacity = *(FLOAT32 *)FieldValue;
                            Layers[x]->VTXT.push_back(curVTXT);
                            return false;
                            }
                    //No existing ATXT, so do nothing
                    return false;
                case 15: //alphaLayer4Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 3)
                            {
                            Layers[x]->ATXT.value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    curLayer = new LANDLAYERS;
                    curLayer->ATXT.value.layer = 3;
                    curLayer->ATXT.value.quadrant = curQuadrant;
                    curLayer->ATXT.value.texture = *(FORMID *)FieldValue;
                    Layers.push_back(curLayer);
                    return true;
                case 16: //alphaLayer4Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 3)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = *(FLOAT32 *)FieldValue;
                                    return false;
                                    }
                    //No existing VTXT, so make one
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 3)
                            {
                            curVTXT.position = curPosition;
                            curVTXT.opacity = *(FLOAT32 *)FieldValue;
                            Layers[x]->VTXT.push_back(curVTXT);
                            return false;
                            }
                    //No existing ATXT, so do nothing
                    return false;
                case 17: //alphaLayer5Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 4)
                            {
                            Layers[x]->ATXT.value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    curLayer = new LANDLAYERS;
                    curLayer->ATXT.value.layer = 4;
                    curLayer->ATXT.value.quadrant = curQuadrant;
                    curLayer->ATXT.value.texture = *(FORMID *)FieldValue;
                    Layers.push_back(curLayer);
                    return true;
                case 18: //alphaLayer5Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 4)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = *(FLOAT32 *)FieldValue;
                                    return false;
                                    }
                    //No existing VTXT, so make one
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 4)
                            {
                            curVTXT.position = curPosition;
                            curVTXT.opacity = *(FLOAT32 *)FieldValue;
                            Layers[x]->VTXT.push_back(curVTXT);
                            return false;
                            }
                    //No existing ATXT, so do nothing
                    return false;
                case 19: //alphaLayer6Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 5)
                            {
                            Layers[x]->ATXT.value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    curLayer = new LANDLAYERS;
                    curLayer->ATXT.value.layer = 5;
                    curLayer->ATXT.value.quadrant = curQuadrant;
                    curLayer->ATXT.value.texture = *(FORMID *)FieldValue;
                    Layers.push_back(curLayer);
                    return true;
                case 20: //alphaLayer6Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 5)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = *(FLOAT32 *)FieldValue;
                                    return false;
                                    }
                    //No existing VTXT, so make one
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 5)
                            {
                            curVTXT.position = curPosition;
                            curVTXT.opacity = *(FLOAT32 *)FieldValue;
                            Layers[x]->VTXT.push_back(curVTXT);
                            return false;
                            }
                    //No existing ATXT, so do nothing
                    return false;
                case 21: //alphaLayer7Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 6)
                            {
                            Layers[x]->ATXT.value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    curLayer = new LANDLAYERS;
                    curLayer->ATXT.value.layer = 6;
                    curLayer->ATXT.value.quadrant = curQuadrant;
                    curLayer->ATXT.value.texture = *(FORMID *)FieldValue;
                    Layers.push_back(curLayer);
                    return true;
                case 22: //alphaLayer7Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 6)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = *(FLOAT32 *)FieldValue;
                                    return false;
                                    }
                    //No existing VTXT, so make one
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 6)
                            {
                            curVTXT.position = curPosition;
                            curVTXT.opacity = *(FLOAT32 *)FieldValue;
                            Layers[x]->VTXT.push_back(curVTXT);
                            return false;
                            }
                    //No existing ATXT, so do nothing
                    return false;
                case 23: //alphaLayer8Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 7)
                            {
                            Layers[x]->ATXT.value.texture = *(FORMID *)FieldValue;
                            return true;
                            }
                    curLayer = new LANDLAYERS;
                    curLayer->ATXT.value.layer = 7;
                    curLayer->ATXT.value.quadrant = curQuadrant;
                    curLayer->ATXT.value.texture = *(FORMID *)FieldValue;
                    Layers.push_back(curLayer);
                    return true;
                case 24: //alphaLayer8Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 7)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = *(FLOAT32 *)FieldValue;
                                    return false;
                                    }
                    //No existing VTXT, so make one
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 7)
                            {
                            curVTXT.position = curPosition;
                            curVTXT.opacity = *(FLOAT32 *)FieldValue;
                            Layers[x]->VTXT.push_back(curVTXT);
                            return false;
                            }
                    //No existing ATXT, so do nothing
                    return false;
                default:
                    break;
                }
            break;
        default:
            break;
        }
    return false;
    }

void LANDRecord::DeleteField(FIELD_IDENTIFIERS)
    {
    LANDNORMALS defaultVNML;
    LANDVHGT defaultVHGT;
    LANDVCLR defaultVCLR;
    LANDGENTXT defaultGENTXT;
    LANDVTXT defaultVTXT;

    UINT8  curQuadrant;
    UINT16 curPosition;

    switch(FieldID)
        {
        case 1: //flags1
            SetHeaderFlagMask(0);
            return;
        case 3: //flags2
            flagsUnk = 0;
            return;
        case 5: //data
            DATA.Unload();
            return;
        case 6: //normals
            if(ListFieldID == 0) //normalsSize
                {
                VNML.Unload();
                return;
                }

            if(ListIndex >= 33)
                return;

            if(ListX2FieldID == 0) //normalsSize
                {
                if(VNML.IsLoaded())
                    {
                    for(UINT32 x = 0; x < 33; ++x)
                        {
                        VNML->VNML[ListIndex][x].x = defaultVNML.x;
                        VNML->VNML[ListIndex][x].y = defaultVNML.y;
                        VNML->VNML[ListIndex][x].z = defaultVNML.z;
                        }
                    }
                return;
                }

            if(ListX2Index >= 33)
                return;

            switch(ListX2FieldID)
                {
                case 1: //x
                    if(VNML.IsLoaded())
                        VNML->VNML[ListIndex][ListX2Index].x = defaultVNML.x;
                    return;
                case 2: //y
                    if(VNML.IsLoaded())
                        VNML->VNML[ListIndex][ListX2Index].y = defaultVNML.y;
                    return;
                case 3: //z
                    if(VNML.IsLoaded())
                        VNML->VNML[ListIndex][ListX2Index].z = defaultVNML.z;
                    return;
                default:
                    return;
                }
        case 7: //heightOffset
            if(VHGT.IsLoaded())
                VHGT->offset = defaultVHGT.offset;
            return;
        case 8: //heights
            if(ListFieldID == 0) //heightsSize
                {
                VHGT.Unload();
                return;
                }

            if(ListIndex >= 33)
                return;

            if(ListX2FieldID == 0) //heightsSize
                {
                if(VHGT.IsLoaded())
                    for(UINT32 x = 0; x < 33; ++x)
                        VHGT->VHGT[ListIndex][x] = defaultVHGT.VHGT[ListIndex][x];
                return;
                }

            if(ListX2Index >= 33)
                return;

            switch(ListX2FieldID)
                {
                case 1: //height
                    if(VHGT.IsLoaded())
                        VHGT->VHGT[ListIndex][ListX2Index] = defaultVHGT.VHGT[ListIndex][ListX2Index];
                    return;
                default:
                    return;
                }
            return;
        case 9: //unused1
            if(VHGT.IsLoaded())
                {
                VHGT->unused1[0] = defaultVHGT.unused1[0];
                VHGT->unused1[1] = defaultVHGT.unused1[1];
                VHGT->unused1[2] = defaultVHGT.unused1[2];
                }
            return;
        case 10: //colors
            if(ListFieldID == 0) //colorsSize, not really a list so it can't be resized
                {
                VCLR.Unload();
                return;
                }

            if(ListIndex >= 33)
                return;

            if(ListX2FieldID == 0) //heightsSize
                {
                if(VCLR.IsLoaded())
                    for(UINT32 x = 0; x < 33; ++x)
                        {
                        VCLR->VCLR[ListIndex][x].red = defaultVCLR.VCLR[ListIndex][x].red;
                        VCLR->VCLR[ListIndex][x].green = defaultVCLR.VCLR[ListIndex][x].green;
                        VCLR->VCLR[ListIndex][x].blue = defaultVCLR.VCLR[ListIndex][x].blue;
                        }
                return;
                }

            if(ListX2Index >= 33)
                return;

            switch(ListX2FieldID)
                {
                case 1: //red
                    if(VCLR.IsLoaded())
                        VCLR->VCLR[ListIndex][ListX2Index].red = defaultVCLR.VCLR[ListIndex][ListX2Index].red;
                    return;
                case 2: //green
                    if(VCLR.IsLoaded())
                        VCLR->VCLR[ListIndex][ListX2Index].green = defaultVCLR.VCLR[ListIndex][ListX2Index].green;
                    return;
                case 3: //blue
                    if(VCLR.IsLoaded())
                        VCLR->VCLR[ListIndex][ListX2Index].blue = defaultVCLR.VCLR[ListIndex][ListX2Index].blue;
                    return;
                default:
                    return;
                }
        case 11: //baseTextures
            if(ListFieldID == 0) //baseTexturesSize
                {
                for(UINT32 x = 0; x < BTXT.size(); ++x)
                    delete BTXT[x];
                BTXT.clear();
                return;
                }

            if(ListIndex >= BTXT.size())
                return;

            switch(ListFieldID)
                {
                case 1: //texture
                    if(BTXT[ListIndex]->IsLoaded())
                        BTXT[ListIndex]->value.texture = defaultGENTXT.texture;
                    return;
                case 2: //quadrant
                    if(BTXT[ListIndex]->IsLoaded())
                        BTXT[ListIndex]->value.quadrant = defaultGENTXT.quadrant;
                    return;
                case 3: //unused1
                    if(BTXT[ListIndex]->IsLoaded())
                        BTXT[ListIndex]->value.unused1 = defaultGENTXT.unused1;
                    return;
                case 4: //layer
                    if(BTXT[ListIndex]->IsLoaded())
                        BTXT[ListIndex]->value.layer = defaultGENTXT.layer;
                    return;
                default:
                    return;
                }
        case 12: //alphaLayers
            if(ListFieldID == 0) //alphaLayersSize
                {
                for(UINT32 x = 0; x < Layers.size(); ++x)
                    delete Layers[x];
                Layers.clear();
                return;
                }

            if(ListIndex >= Layers.size())
                return;

            switch(ListFieldID)
                {
                case 1: //texture
                    if(Layers[ListIndex]->ATXT.IsLoaded())
                        Layers[ListIndex]->ATXT.value.texture = defaultGENTXT.texture;
                    return;
                case 2: //quadrant
                    if(Layers[ListIndex]->ATXT.IsLoaded())
                        Layers[ListIndex]->ATXT.value.quadrant = defaultGENTXT.quadrant;
                    return;
                case 3: //unused1
                    if(Layers[ListIndex]->ATXT.IsLoaded())
                        Layers[ListIndex]->ATXT.value.unused1 = defaultGENTXT.unused1;
                    return;
                case 4: //layer
                    if(Layers[ListIndex]->ATXT.IsLoaded())
                        Layers[ListIndex]->ATXT.value.layer = defaultGENTXT.layer;
                    return;
                case 5: //opacities
                    if(ListFieldID == 0) //opacitiesSize
                        {
                        Layers[ListIndex]->VTXT.clear();
                        return;
                        }

                    if(ListX2Index >= Layers[ListIndex]->VTXT.size())
                        return;

                    switch(ListX2FieldID)
                        {
                        case 1: //position
                            Layers[ListIndex]->VTXT[ListX2Index].position = defaultVTXT.position;
                            return;
                        case 2: //unused1
                            Layers[ListIndex]->VTXT[ListX2Index].unused1[0] = defaultVTXT.unused1[0];
                            Layers[ListIndex]->VTXT[ListX2Index].unused1[1] = defaultVTXT.unused1[1];
                            return;
                        case 3: //opacity
                            Layers[ListIndex]->VTXT[ListX2Index].opacity = defaultVTXT.opacity;
                            return;
                        default:
                            return;
                        }
                default:
                    return;
                }
        case 13: //vertexTextures
            if(ListFieldID == 0) //vertexTexturesSize
                {
                VTEX.clear();
                return;
                }

            if(ListIndex >= VTEX.size())
                return;

            switch(ListFieldID)
                {
                case 1: //texture
                    VTEX.clear();
                    return;
                default:
                    return;
                }
        case 14: //Position

            if(ListIndex >= 33)
                return;

            if(ListX2Index >= 33)
                return;

            if(ListX2FieldID == 0) //PositionSize
                {
                for(UINT32 x = 1; x <= 24; ++x)
                    DeleteField(FieldID, ListIndex, ListFieldID, ListX2Index, x, ListX3Index, ListX3FieldID);
                return;
                }

            switch(ListX2FieldID)
                {
                case 1: //height
                    SetField(FieldID, ListIndex, ListFieldID, ListX2Index, ListX2FieldID, ListX3Index, ListX3FieldID, &defaultVHGT.VHGT[ListIndex][ListX2Index], 0);
                    return;
                case 2: //normalX
                    if(VNML.IsLoaded())
                        VNML->VNML[ListIndex][ListX2Index].x = defaultVNML.x;
                    return;
                case 3: //normalY
                    if(VNML.IsLoaded())
                        VNML->VNML[ListIndex][ListX2Index].y = defaultVNML.y;
                    return;
                case 4: //normalZ
                    if(VNML.IsLoaded())
                        VNML->VNML[ListIndex][ListX2Index].z = defaultVNML.z;
                    return;
                case 5: //red
                    if(VCLR.IsLoaded())
                        VCLR->VCLR[ListIndex][ListX2Index].red = defaultVCLR.VCLR[ListIndex][ListX2Index].red;
                    return;
                case 6: //green
                    if(VCLR.IsLoaded())
                        VCLR->VCLR[ListIndex][ListX2Index].green = defaultVCLR.VCLR[ListIndex][ListX2Index].green;
                    return;
                case 7: //blue
                    if(VCLR.IsLoaded())
                        VCLR->VCLR[ListIndex][ListX2Index].blue = defaultVCLR.VCLR[ListIndex][ListX2Index].blue;
                    return;
                case 8: //baseTexture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < BTXT.size(); ++x)
                        if(BTXT[x]->value.quadrant == curQuadrant)
                            {
                            BTXT[x]->value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing BTXT, so nothing to do
                    return;
                case 9: //alphaLayer1Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 0)
                            {
                            Layers[x]->ATXT.value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing alpha layer, so nothing to do
                    return;
                case 10: //alphaLayer1Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 0)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = defaultVTXT.opacity;
                                    return;
                                    }
                    //No existing VTXT, so nothing to do
                    return;
                case 11: //alphaLayer2Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 1)
                            {
                            Layers[x]->ATXT.value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing alpha layer, so nothing to do
                    return;
                case 12: //alphaLayer2Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 1)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = defaultVTXT.opacity;
                                    return;
                                    }
                    //No existing VTXT, so nothing to do
                    return;
                case 13: //alphaLayer3Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 2)
                            {
                            Layers[x]->ATXT.value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing alpha layer, so nothing to do
                    return;
                case 14: //alphaLayer3Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 2)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = defaultVTXT.opacity;
                                    return;
                                    }
                    //No existing VTXT, so nothing to do
                    return;
                case 15: //alphaLayer4Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 3)
                            {
                            Layers[x]->ATXT.value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing alpha layer, so nothing to do
                    return;
                case 16: //alphaLayer4Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 3)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = defaultVTXT.opacity;
                                    return;
                                    }
                    //No existing VTXT, so nothing to do
                    return;
                case 17: //alphaLayer5Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 4)
                            {
                            Layers[x]->ATXT.value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing alpha layer, so nothing to do
                    return;
                case 18: //alphaLayer5Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 4)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = defaultVTXT.opacity;
                                    return;
                                    }
                    //No existing VTXT, so nothing to do
                    return;
                case 19: //alphaLayer6Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 5)
                            {
                            Layers[x]->ATXT.value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing alpha layer, so nothing to do
                    return;
                case 20: //alphaLayer6Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 5)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = defaultVTXT.opacity;
                                    return;
                                    }
                    //No existing VTXT, so nothing to do
                    return;
                case 21: //alphaLayer7Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 6)
                            {
                            Layers[x]->ATXT.value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing alpha layer, so nothing to do
                    return;
                case 22: //alphaLayer7Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 6)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = defaultVTXT.opacity;
                                    return;
                                    }
                    //No existing VTXT, so nothing to do
                    return;
                case 23: //alphaLayer8Texture
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 7)
                            {
                            Layers[x]->ATXT.value.texture = defaultGENTXT.texture;
                            return;
                            }
                    //No existing alpha layer, so nothing to do
                    return;
                case 24: //alphaLayer8Opacity
                    curQuadrant = CalcQuadrant(ListIndex, ListX2Index);
                    curPosition = CalcPosition(curQuadrant, ListIndex, ListX2Index);
                    for(UINT32 x = 0; x < Layers.size(); ++x)
                        if(Layers[x]->ATXT.value.quadrant == curQuadrant && Layers[x]->ATXT.value.layer == 7)
                            for(UINT32 y = 0; y < Layers[x]->VTXT.size(); ++y)
                                if(Layers[x]->VTXT[y].position == curPosition)
                                    {
                                    Layers[x]->VTXT[y].opacity = defaultVTXT.opacity;
                                    return;
                                    }
                    //No existing VTXT, so nothing to do
                    return;
                default:
                    return;
                }
        default:
            return;
        }
    }