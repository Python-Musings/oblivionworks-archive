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
#include "TES4Record.h"

TES4Record::TES4HEDR::TES4HEDR(FLOAT32 _version, UINT32 _numRecords, UINT32 _nextObject):
    version(_version),
    numRecords(_numRecords),
    nextObject(_nextObject)
    {
    //
    }

TES4Record::TES4HEDR::~TES4HEDR()
    {
    //
    }

bool TES4Record::TES4HEDR::operator ==(const TES4HEDR &other) const
    {
    return (AlmostEqual(version,other.version,2) &&
            numRecords == other.numRecords &&
            nextObject == other.nextObject);
    }

bool TES4Record::TES4HEDR::operator !=(const TES4HEDR &other) const
    {
    return !(*this == other);
    }

TES4Record::TES4DATA::TES4DATA():
    unk1(0),
    unk2(0)
    {
    //
    }

TES4Record::TES4DATA::~TES4DATA()
    {
    //
    }

TES4Record::TES4Record(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

TES4Record::TES4Record(TES4Record *srcRecord):
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

    //EDID = srcRecord->EDID;
    HEDR = srcRecord->HEDR;
    OFST = srcRecord->OFST;
    DELE = srcRecord->DELE;
    CNAM = srcRecord->CNAM;
    SNAM = srcRecord->SNAM;
    MAST = srcRecord->MAST;
    return;
    }

TES4Record::~TES4Record()
    {
    //
    }

bool TES4Record::IsESM()
    {
    if(!IsLoaded()) return false;
    return (flags & fIsESM) != 0;
    }

void TES4Record::IsESM(bool value)
    {
    if(!IsLoaded()) return;
    flags = value ? (flags | fIsESM) : (flags & ~fIsESM);
    }

UINT32 TES4Record::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 0;

    if(HEDR.IsLoaded())
        TotSize += HEDR.GetSize() + 6;

    if(OFST.IsLoaded())
        {
        cSize = OFST.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(DELE.IsLoaded())
        {
        cSize = DELE.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(CNAM.IsLoaded())
        {
        cSize = CNAM.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(SNAM.IsLoaded())
        {
        cSize = SNAM.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    for(UINT32 p = 0; p < MAST.size(); p++)
        {
        cSize = MAST[p].GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 20;//accounts for associated DATA element, subTypes and sizes
        }

    return TotSize;
    }

UINT32 TES4Record::GetType()
    {
    return '4SET';
    }

STRING TES4Record::GetStrType()
    {
    return "TES4";
    }

SINT32 TES4Record::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    #ifdef CBASH_USE_LOGGING
        CLOGGER;
        BOOST_LOG_FUNCTION();
        BOOST_LOG_SEV(lg, trace) << "Parsing";
    #endif
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    StringRecord curMAST;
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
            case 'RDEH':
                HEDR.Read(buffer, subSize, curPos);
                break;
            case 'MANC':
                CNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANS':
                SNAM.Read(buffer, subSize, curPos);
                break;
            case 'TSAM':
                curMAST.Read(buffer, subSize, curPos);
                MAST.push_back(curMAST);
                curMAST.Unload();
                break;
            case 'ATAD':
                curPos += subSize;
                break;
            case 'TSFO':
                OFST.Read(buffer, subSize, curPos);
                break;
            case 'ELED':
                DELE.Read(buffer, subSize, curPos);
                break;
            default:
                #ifdef CBASH_USE_LOGGING
                    BOOST_LOG_SEV(lg, critical) << "TES4: Unknown subType = " << ((char *)&subType)[0] << ((char *)&subType)[1] << ((char *)&subType)[2] << ((char *)&subType)[3];
                    BOOST_LOG_SEV(lg, critical) << "      Size = " << subSize;
                    BOOST_LOG_SEV(lg, critical) << "      CurPos = " << curPos - 6;
                #endif
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 TES4Record::Unload()
    {
    //TES4 should never be unloaded, so do nothing
    return 1;
    }

SINT32 TES4Record::WriteRecord(_FileHandler &SaveHandler)
    {
    TES4DATA DATA;

    SaveHandler.writeSubRecord('RDEH', &HEDR.value, sizeof(TES4HEDR));
    if(OFST.IsLoaded())
        SaveHandler.writeSubRecord('TSFO', OFST.value, OFST.GetSize());
    if(DELE.IsLoaded())
        SaveHandler.writeSubRecord('ELED', DELE.value, DELE.GetSize());
    if(CNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANC', CNAM.value, CNAM.GetSize());
    if(SNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANS', SNAM.value, SNAM.GetSize());
    for(UINT32 p = 0; p < MAST.size(); p++)
        {
        SaveHandler.writeSubRecord('TSAM', MAST[p].value, MAST[p].GetSize());
        SaveHandler.writeSubRecord('ATAD', &DATA, sizeof(TES4DATA));
        }
    return -1;
    }

bool TES4Record::operator ==(const TES4Record &other) const
    {
    if(OFST == other.OFST &&
        DELE == other.DELE &&
        CNAM.equals(other.CNAM) &&
        SNAM.equals(other.SNAM) &&
        MAST.size() == other.MAST.size())
        {
        //Record order kinda sorta but doesn't really matter on masters, so equality testing is easy
        //The order determines the mod index of all formIDs in the mod file
        //If both records have the same masters in the same orders, the formIDs will have the same master indexing
        //If both records have the same masters in different orders, the formIDs will have different indexing but be logically equivalent
        //The ordering has no effect on load order in game or in the editor
        for(UINT32 x = 0; x < MAST.size(); ++x)
            if(!(MAST[x].equalsi(other.MAST[x])))
                return false;
        return true;
        }

    return false;
    }

bool TES4Record::operator !=(const TES4Record &other) const
    {
    return !(*this == other);
    }