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
#include "BSGNRecord.h"

BSGNRecord::BSGNRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

BSGNRecord::BSGNRecord(BSGNRecord *srcRecord):
    Record()
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

    EDID = srcRecord->EDID;
    FULL = srcRecord->FULL;
    ICON = srcRecord->ICON;
    DESC = srcRecord->DESC;
    SPLO.resize(srcRecord->SPLO.size());
    for(UINT32 x = 0; x < srcRecord->SPLO.size(); x++)
        SPLO[x] = srcRecord->SPLO[x];
    return;
    }

BSGNRecord::~BSGNRecord()
    {
    //
    }

bool BSGNRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < SPLO.size(); x++)
        op.Accept(SPLO[x]);

    return op.Stop();
    }

UINT32 BSGNRecord::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 0;

    if(EDID.IsLoaded())
        {
        cSize = EDID.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(FULL.IsLoaded())
        {
        cSize = FULL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(DESC.IsLoaded())
        {
        cSize = DESC.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(SPLO.size())
        TotSize += (UINT32)SPLO.size() * (sizeof(UINT32) + 6);

    return TotSize;
    }

UINT32 BSGNRecord::GetType()
    {
    return 'NGSB';
    }

STRING BSGNRecord::GetStrType()
    {
    return "BSGN";
    }

SINT32 BSGNRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    FORMID curFormID = 0;
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
            case 'DIDE':
                EDID.Read(buffer, subSize, curPos);
                break;
            case 'LLUF':
                FULL.Read(buffer, subSize, curPos);
                break;
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case 'CSED':
                DESC.Read(buffer, subSize, curPos);
                break;
            case 'OLPS':
                _readBuffer(&curFormID,buffer,subSize,curPos);
                SPLO.push_back(curFormID);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  BSGN: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 BSGNRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    ICON.Unload();
    DESC.Unload();

    SPLO.clear();
    return 1;
    }

SINT32 BSGNRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(DESC.IsLoaded())
        SaveHandler.writeSubRecord('CSED', DESC.value, DESC.GetSize());

    for(UINT32 p = 0; p < SPLO.size(); p++)
        SaveHandler.writeSubRecord('OLPS', &SPLO[p], sizeof(UINT32));
    return -1;
    }

bool BSGNRecord::operator ==(const BSGNRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        FULL.equals(other.FULL) &&
        ICON.equalsi(other.ICON) &&
        DESC.equals(other.DESC) &&
        SPLO.size() == other.SPLO.size())
        {
        //Record order doesn't matter on spells, so equality testing isn't easy
        //The proper solution would be to check each spell against every other spell to see if there's a one-to-one match
        //Perhaps using a disjoint set
        //Fix-up later
        for(UINT32 x = 0; x < SPLO.size(); ++x)
            if(SPLO[x] != other.SPLO[x])
                return false;

        return true;
        }

    return false;
    }

bool BSGNRecord::operator !=(const BSGNRecord &other) const
    {
    return !(*this == other);
    }