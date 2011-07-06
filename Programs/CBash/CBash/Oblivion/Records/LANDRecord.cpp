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
#include "..\..\Common.h"
#include "LANDRecord.h"

LANDRecord::LANDNORMALS::LANDNORMALS():
    x(0),
    y(0),
    z(0)
    {
    //
    }

LANDRecord::LANDNORMALS::~LANDNORMALS()
    {
    //
    }

bool LANDRecord::LANDNORMALS::operator ==(const LANDNORMALS &other) const
    {
    return (x == other.x &&
            y == other.y &&
            z == other.z);
    }

bool LANDRecord::LANDNORMALS::operator !=(const LANDNORMALS &other) const
    {
    return !(*this == other);
    }

bool LANDRecord::LANDVNML::operator ==(const LANDVNML &other) const
    {
    //Record order matters on normals, so equality testing is easy
    for(UINT32 x = 0; x < 33; ++x)
        for(UINT32 y = 0; y < 33; ++y)
            if(VNML[x][y] != other.VNML[x][y])
                return false;
    return true;
    }

bool LANDRecord::LANDVNML::operator !=(const LANDVNML &other) const
    {
    return !(*this == other);
    }

LANDRecord::LANDVHGT::LANDVHGT():
    offset(0.0f)
    {
    memset(&VHGT[0][0], 0, 1089);
    memset(&unused1[0], 0, 3);
    }

LANDRecord::LANDVHGT::~LANDVHGT()
    {
    //
    }

bool LANDRecord::LANDVHGT::operator ==(const LANDVHGT &other) const
    {
    if(AlmostEqual(offset,other.offset,2))
        {
        //Record order matters on heights, so equality testing is easy
        for(UINT32 x = 0; x < 33; ++x)
            for(UINT32 y = 0; y < 33; ++y)
                if(VHGT[x][y] != other.VHGT[x][y])
                    return false;
        return true;
        }

    return false;
    }

bool LANDRecord::LANDVHGT::operator !=(const LANDVHGT &other) const
    {
    return !(*this == other);
    }

LANDRecord::LANDCOLORS::LANDCOLORS():
    red(0),
    green(0),
    blue(0)
    {
    //
    }

LANDRecord::LANDCOLORS::~LANDCOLORS()
    {
    //
    }

bool LANDRecord::LANDCOLORS::operator ==(const LANDCOLORS &other) const
    {
    return (red == other.red &&
            green == other.green &&
            blue == other.blue);
    }

bool LANDRecord::LANDCOLORS::operator !=(const LANDCOLORS &other) const
    {
    return !(*this == other);
    }

bool LANDRecord::LANDVCLR::operator ==(const LANDVCLR &other) const
    {
    //Record order matters on colors, so equality testing is easy
    for(UINT32 x = 0; x < 33; ++x)
        for(UINT32 y = 0; y < 33; ++y)
            if(VCLR[x][y] != other.VCLR[x][y])
                return false;
    return true;
    }

bool LANDRecord::LANDVCLR::operator !=(const LANDVCLR &other) const
    {
    return !(*this == other);
    }

LANDRecord::LANDGENTXT::LANDGENTXT():
    texture(0),
    quadrant(0),
    unused1(0),
    layer(-1)
    {
    //
    }

LANDRecord::LANDGENTXT::~LANDGENTXT()
    {
    //
    }

bool LANDRecord::LANDGENTXT::operator ==(const LANDGENTXT &other) const
    {
    return (texture == other.texture &&
            quadrant == other.quadrant &&
            layer == other.layer);
    }

bool LANDRecord::LANDGENTXT::operator !=(const LANDGENTXT &other) const
    {
    return !(*this == other);
    }

LANDRecord::LANDVTXT::LANDVTXT():
    position(0),
    opacity(0.0f)
    {
    memset(&unused1[0], 0, 2);
    }

LANDRecord::LANDVTXT::~LANDVTXT()
    {
    //
    }

bool LANDRecord::LANDVTXT::operator ==(const LANDVTXT &other) const
    {
    return (position == other.position &&
            AlmostEqual(opacity,other.opacity,2));
    }

bool LANDRecord::LANDVTXT::operator !=(const LANDVTXT &other) const
    {
    return !(*this == other);
    }

bool LANDRecord::LANDLAYERS::operator ==(const LANDLAYERS &other) const
    {
    if(ATXT == other.ATXT &&
        VTXT.size() == other.VTXT.size())
        {
        //Record order doesn't matter on opacities, so equality testing isn't easy
        //Instead, they're keyed by position (VTXT.value.position)
        //The proper solution would be to see if the opacity at each position matches
        //Fix-up later
        for(UINT32 x = 0; x < VTXT.size(); ++x)
            if(VTXT[x] != other.VTXT[x])
                return false;
        return true;
        }

    return false;
    }

bool LANDRecord::LANDLAYERS::operator !=(const LANDLAYERS &other) const
    {
    return !(*this == other);
    }

LANDRecord::LANDRecord(unsigned char *_recData):
    Record(_recData),
    WestLand(NULL),
    EastLand(NULL),
    NorthLand(NULL),
    SouthLand(NULL)
    {
    //LAND records are normally compressed due to size
    IsCompressed(true);
    }

LANDRecord::LANDRecord(LANDRecord *srcRecord):
    Record(),
    WestLand(NULL),
    EastLand(NULL),
    NorthLand(NULL),
    SouthLand(NULL)
    {
    if(srcRecord == NULL)
        return;

    flags = srcRecord->flags;
    formID = srcRecord->formID;
    flagsUnk = srcRecord->flagsUnk;

    if(!srcRecord->IsChanged())
        {
        IsLoaded(false);
        recData = srcRecord->recData;
        return;
        }


    DATA = srcRecord->DATA;
    VNML = srcRecord->VNML;
    VHGT = srcRecord->VHGT;
    VCLR = srcRecord->VCLR;

    BTXT.resize(srcRecord->BTXT.size());
    for(UINT32 x = 0; x < srcRecord->BTXT.size(); ++x)
        {
        BTXT[x] = new ReqSubRecord<LANDGENTXT>;
        *BTXT[x] = *srcRecord->BTXT[x];
        }

    Layers.resize(srcRecord->Layers.size());
    for(UINT32 x = 0; x < srcRecord->Layers.size(); ++x)
        {
        Layers[x] = new LANDLAYERS;
        Layers[x]->ATXT = srcRecord->Layers[x]->ATXT;
        Layers[x]->VTXT = srcRecord->Layers[x]->VTXT;
        }
    VTEX = srcRecord->VTEX;
    return;
    }

LANDRecord::~LANDRecord()
    {
    for(UINT32 x = 0; x < BTXT.size(); ++x)
        delete BTXT[x];
    for(UINT32 x = 0; x < Layers.size(); ++x)
        delete Layers[x];
    //WestLand,EastLand,NorthLand,SouthLand are all shared and are deleted when the LAND is deleted
    }

bool LANDRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < BTXT.size(); ++x)
        if(BTXT[x] != NULL)
            op.Accept(BTXT[x]->value.texture);
    for(UINT32 x = 0; x < Layers.size(); ++x)
        op.Accept(Layers[x]->ATXT.value.texture);
    for(UINT32 x = 0; x < VTEX.size(); x++)
        op.Accept(VTEX[x]);

    return op.Stop();
    }

UINT8 LANDRecord::CalcQuadrant(const UINT32 &row, const UINT32 &column)
    {
    if(column > 16)
        {
        if(row > 16) return eTopRight;
        else return eBottomRight;
        }
    else
        {
        if(row > 16) return eTopLeft;
        else return eBottomLeft;
        }
    }

UINT16 LANDRecord::CalcPosition(const UINT8 &curQuadrant, const UINT32 &row, const UINT32 &column)
    {
    switch(curQuadrant)
        {
        case eTopRight:
            return (unsigned short)((column - 16) + ((row - 16) * 17));
        case eBottomRight:
            return (unsigned short)((column - 16) + ((row) * 17));
        case eTopLeft:
            return (unsigned short)((column) + ((row - 16) * 17));
        case eBottomLeft:
            return (unsigned short)(column + (row * 17));
        default:
            return 0;
        }
    }

UINT32 LANDRecord::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 0;

    if(DATA.IsLoaded())
        {
        cSize = DATA.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(VNML.IsLoaded())
        TotSize += VNML.GetSize() + 6;

    if(VHGT.IsLoaded())
        TotSize += VHGT.GetSize() + 6;

    if(VCLR.IsLoaded())
        TotSize += VCLR.GetSize() + 6;

    for(UINT32 p = 0; p < BTXT.size(); p++)
        if(BTXT[p] != NULL && BTXT[p]->IsLoaded())
            TotSize += BTXT[p]->GetSize() + 6;

    for(UINT32 p = 0; p < Layers.size(); p++)
        {
        if(Layers[p]->ATXT.IsLoaded())
            TotSize += Layers[p]->ATXT.GetSize() + 6;

        if(Layers[p]->VTXT.size())
            {
            cSize = (sizeof(LANDVTXT) * (UINT32)Layers[p]->VTXT.size());
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(VTEX.size())
        {
        cSize = (sizeof(UINT32) * (UINT32)VTEX.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    return TotSize;
    }

UINT32 LANDRecord::GetType()
    {
    return 'DNAL';
    }

STRING LANDRecord::GetStrType()
    {
    return "LAND";
    }

UINT32 LANDRecord::GetParentType()
    {
    return 'LLEC';
    }

FLOAT32 LANDRecord::CalcHeight(const UINT32 &row, const UINT32 &column)
    {
    if(!VHGT.IsLoaded())
        return 0.0f;

    FLOAT32 fRetValue = VHGT->offset * 8.0f;

    for(UINT32 curRow = 0; curRow <= row; ++curRow)
        fRetValue += (VHGT->VHGT[curRow][0] * 8.0f);

    for(UINT32 curColumn = 1; curColumn <= column; ++curColumn)
        fRetValue += (VHGT->VHGT[row][curColumn] * 8.0f);

    return fRetValue;
    }

SINT32 LANDRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    ReqSubRecord<LANDGENTXT> *curTexture = NULL;
    LANDLAYERS *curLayer = NULL;
    while(curPos < recSize){
        _readBuffer(&subType, buffer, 4, curPos);
        switch(subType)
            {
            case 'XXXX':
                curPos += 2;
                _readBuffer(&subSize, buffer, 4, curPos);
                _readBuffer(&subType, buffer, 4, curPos);
                curPos += 2;
                break;
            default:
                subSize = 0;
                _readBuffer(&subSize, buffer, 2, curPos);
                break;
            }
        switch(subType)
            {
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'LMNV':
                VNML.Read(buffer, subSize, curPos);
                break;
            case 'TGHV':
                VHGT.Read(buffer, subSize, curPos);
                break;
            case 'RLCV':
                VCLR.Read(buffer, subSize, curPos);
                break;
            case 'TXTB':
                curTexture = new ReqSubRecord<LANDGENTXT>;
                curTexture->Read(buffer, subSize, curPos);
                BTXT.push_back(curTexture);
                break;
            case 'TXTA':
                curLayer = new LANDLAYERS;
                curLayer->ATXT.Read(buffer, subSize, curPos);
                Layers.push_back(curLayer);
                break;
            case 'TXTV':
                if(subSize % sizeof(LANDVTXT) == 0)
                    {
                    if(subSize == 0)
                        break;
                    curLayer->VTXT.resize(subSize / sizeof(LANDVTXT));
                    _readBuffer(&curLayer->VTXT[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized VTXT size: %i\n", subSize);
                    curPos += subSize;
                    }
                //switch(curTexture.value.quadrant)
                //    {
                //    case eBottomLeft:
                //        for(UINT32 x = 0; x < VTXT.size(); ++x)
                //            Merged->Points[(VTXT[x].position % 17)][(VTXT[x].position / 17)].AlphaLayer[curTexture.value.layer] = VTXT[x].opacity;
                //        break;
                //    case eBottomRight:
                //        for(UINT32 x = 0; x < VTXT.size(); ++x)
                //            Merged->Points[(VTXT[x].position % 17) + 16][(VTXT[x].position / 17)].AlphaLayer[curTexture.value.layer] = VTXT[x].opacity;
                //        break;
                //    case eTopLeft:
                //        for(UINT32 x = 0; x < VTXT.size(); ++x)
                //            Merged->Points[(VTXT[x].position % 17)][(VTXT[x].position / 17) + 16].AlphaLayer[curTexture.value.layer] = VTXT[x].opacity;
                //        break;
                //    case eTopRight:
                //        for(UINT32 x = 0; x < VTXT.size(); ++x)
                //            Merged->Points[(VTXT[x].position % 17) + 16][(VTXT[x].position / 17) + 16].AlphaLayer[curTexture.value.layer] = VTXT[x].opacity;
                //        break;
                //    default:
                //        printf("Invalid ATXT quadrant when parsing VTXT: %u\n", curTexture.value.quadrant);
                //        break;
                //    }
                break;
            case 'XETV':
                if(subSize % sizeof(UINT32) == 0)
                    {
                    if(subSize == 0)
                        break;
                    VTEX.resize(subSize / sizeof(UINT32));
                    _readBuffer(&VTEX[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized VTEX size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  LAND: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 LANDRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    DATA.Unload();
    VNML.Unload();
    VHGT.Unload();
    VCLR.Unload();
    for(UINT32 x = 0; x < BTXT.size(); ++x)
        delete BTXT[x];
    BTXT.clear();

    for(UINT32 x = 0; x < Layers.size(); ++x)
        delete Layers[x];
    Layers.clear();

    VTEX.clear();
    return 1;
    }

SINT32 LANDRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', DATA.value, DATA.GetSize());
    if(VNML.IsLoaded())
        SaveHandler.writeSubRecord('LMNV', VNML.value, VNML.GetSize());
    if(VHGT.IsLoaded())
        SaveHandler.writeSubRecord('TGHV', VHGT.value, VHGT.GetSize());
    if(VCLR.IsLoaded())
        SaveHandler.writeSubRecord('RLCV', VCLR.value, VCLR.GetSize());
    if(BTXT.size())
        for(UINT32 p = 0; p < BTXT.size(); p++)
            if(BTXT[p]->IsLoaded())
                SaveHandler.writeSubRecord('TXTB', &BTXT[p]->value, BTXT[p]->GetSize());

    if(Layers.size())
        for(UINT32 p = 0; p < Layers.size(); p++)
            {
            if(Layers[p]->ATXT.IsLoaded())
                SaveHandler.writeSubRecord('TXTA', &Layers[p]->ATXT.value, Layers[p]->ATXT.GetSize());
            if(Layers[p]->VTXT.size())
                SaveHandler.writeSubRecord('TXTV', &Layers[p]->VTXT[0], (UINT32)Layers[p]->VTXT.size() * sizeof(LANDVTXT));
            }
    if(VTEX.size())
        SaveHandler.writeSubRecord('XETV', &VTEX[0], (UINT32)VTEX.size() * sizeof(UINT32));

    return -1;
    }

bool LANDRecord::operator ==(const LANDRecord &other) const
    {
    if(DATA == other.DATA &&
        VNML == other.VNML &&
        VHGT == other.VHGT &&
        VCLR == other.VCLR &&
        BTXT.size() == other.BTXT.size() &&
        Layers.size() == other.Layers.size() &&
        VTEX.size() == other.VTEX.size())
        {
        //Not sure if record order matters on base textures, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < BTXT.size(); ++x)
            if(*BTXT[x] != *other.BTXT[x])
                return false;

        //Not sure if record order matters on alpha layers, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < Layers.size(); ++x)
            if(*Layers[x] != *other.Layers[x])
                return false;

        //Not sure if record order matters on vertex textures, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < VTEX.size(); ++x)
            if(VTEX[x] != other.VTEX[x])
                return false;
        return true;
        }

    return false;
    }

bool LANDRecord::operator !=(const LANDRecord &other) const
    {
    return !(*this == other);
    }