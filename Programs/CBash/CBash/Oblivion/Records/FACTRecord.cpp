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
#include "FACTRecord.h"

FACTRecord::FACTCNAM::FACTCNAM():
    crimeGoldMultiplier(1.0f)
    {
    //
    }

FACTRecord::FACTCNAM::~FACTCNAM()
    {
    //
    }

bool FACTRecord::FACTCNAM::operator ==(const FACTCNAM &other) const
    {
    return (AlmostEqual(crimeGoldMultiplier,other.crimeGoldMultiplier,2));
    }

bool FACTRecord::FACTCNAM::operator !=(const FACTCNAM &other) const
    {
    return !(*this == other);
    }

FACTRecord::FACTRNAM::FACTRNAM():
    RNAM(0)
    {
    //
    }

FACTRecord::FACTRNAM::~FACTRNAM()
    {
    //
    }

bool FACTRecord::FACTRNAM::operator ==(const FACTRNAM &other) const
    {
    return (RNAM == other.RNAM &&
            MNAM.equals(other.MNAM) &&
            FNAM.equals(other.FNAM) &&
            INAM.equalsi(other.INAM));
    }

bool FACTRecord::FACTRNAM::operator !=(const FACTRNAM &other) const
    {
    return !(*this == other);
    }

FACTRecord::FACTRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

FACTRecord::FACTRecord(FACTRecord *srcRecord):
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

    XNAM.clear();
    XNAM.resize(srcRecord->XNAM.size());
    for(UINT32 x = 0; x < srcRecord->XNAM.size(); x++)
        {
        XNAM[x] = new ReqSubRecord<GENXNAM>;
        *XNAM[x] = *srcRecord->XNAM[x];
        }

    DATA = srcRecord->DATA;
    CNAM = srcRecord->CNAM;

    RNAM.clear();
    RNAM.resize(srcRecord->RNAM.size());
    for(UINT32 x = 0; x < srcRecord->RNAM.size(); x++)
        {
        RNAM[x] = new ReqSubRecord<FACTRNAM>;
        *RNAM[x] = *srcRecord->RNAM[x];
        }
    }

FACTRecord::~FACTRecord()
    {
    for(UINT32 x = 0; x < XNAM.size(); x++)
        delete XNAM[x];
    for(UINT32 x = 0; x < RNAM.size(); x++)
        delete RNAM[x];
    }

bool FACTRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < XNAM.size(); x++)
        op.Accept(XNAM[x]->value.faction);

    return op.Stop();
    }

bool FACTRecord::IsHiddenFromPC()
    {
    return (DATA.value.flags & fIsHiddenFromPC) != 0;
    }

void FACTRecord::IsHiddenFromPC(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsHiddenFromPC) : (DATA.value.flags & ~fIsHiddenFromPC);
    }

bool FACTRecord::IsEvil()
    {
    return (DATA.value.flags & fIsEvil) != 0;
    }

void FACTRecord::IsEvil(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsEvil) : (DATA.value.flags & ~fIsEvil);
    }

bool FACTRecord::IsSpecialCombat()
    {
    return (DATA.value.flags & fIsSpecialCombat) != 0;
    }

void FACTRecord::IsSpecialCombat(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsSpecialCombat) : (DATA.value.flags & ~fIsSpecialCombat);
    }

bool FACTRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void FACTRecord::SetFlagMask(UINT8 Mask)
    {
    DATA.value.flags = Mask;
    }

UINT32 FACTRecord::GetSize(bool forceCalc)
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

    for(UINT32 p = 0; p < XNAM.size(); p++)
        TotSize += XNAM[p]->GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(CNAM.IsLoaded())
        TotSize += CNAM.GetSize() + 6;

    for(UINT32 p = 0; p < RNAM.size(); p++)
        {
        TotSize += sizeof(RNAM[p]->value.RNAM) + 6;

        if(RNAM[p]->value.MNAM.IsLoaded())
            {
            cSize = RNAM[p]->value.MNAM.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(RNAM[p]->value.FNAM.IsLoaded())
            {
            cSize = RNAM[p]->value.FNAM.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(RNAM[p]->value.INAM.IsLoaded())
            {
            cSize = RNAM[p]->value.INAM.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    return TotSize;
    }

UINT32 FACTRecord::GetType()
    {
    return 'TCAF';
    }

STRING FACTRecord::GetStrType()
    {
    return "FACT";
    }

SINT32 FACTRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    ReqSubRecord<GENXNAM> *newXNAM = NULL;
    ReqSubRecord<FACTRNAM> *newRNAM = NULL;
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
            case 'MANX':
                newXNAM = new ReqSubRecord<GENXNAM>;
                newXNAM->Read(buffer, subSize, curPos);
                XNAM.push_back(newXNAM);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'MANC':
                CNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANR':
                newRNAM = new ReqSubRecord<FACTRNAM>;
                newRNAM->Read(buffer, subSize, curPos);
                RNAM.push_back(newRNAM);
                break;
            case 'MANM':
                if(newRNAM == NULL)
                    {
                    newRNAM = new ReqSubRecord<FACTRNAM>;
                    RNAM.push_back(newRNAM);
                    }
                newRNAM->value.MNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANF':
                if(newRNAM == NULL)
                    {
                    newRNAM = new ReqSubRecord<FACTRNAM>;
                    RNAM.push_back(newRNAM);
                    }
                newRNAM->value.FNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANI':
                if(newRNAM == NULL)
                    {
                    newRNAM = new ReqSubRecord<FACTRNAM>;
                    RNAM.push_back(newRNAM);
                    }
                newRNAM->value.INAM.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  FACT: Unknown subType = %04X\n", subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 FACTRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();

    for(UINT32 x = 0; x < XNAM.size(); x++)
        delete XNAM[x];
    XNAM.clear();

    DATA.Unload();
    CNAM.Unload();

    for(UINT32 x = 0; x < RNAM.size(); x++)
        delete RNAM[x];
    RNAM.clear();
    return 1;
    }

SINT32 FACTRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    for(UINT32 p = 0; p < XNAM.size(); p++)
        if(XNAM[p]->IsLoaded())
            SaveHandler.writeSubRecord('MANX', &XNAM[p]->value, XNAM[p]->GetSize());

    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(CNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANC', CNAM.value, CNAM.GetSize());

    for(UINT32 p = 0; p < RNAM.size(); p++)
        {
        if(RNAM[p]->IsLoaded())
            SaveHandler.writeSubRecord('MANR', &RNAM[p]->value.RNAM, sizeof(RNAM[p]->value.RNAM));
        if(RNAM[p]->value.MNAM.IsLoaded())
            SaveHandler.writeSubRecord('MANM', RNAM[p]->value.MNAM.value, RNAM[p]->value.MNAM.GetSize());
        if(RNAM[p]->value.FNAM.IsLoaded())
            SaveHandler.writeSubRecord('MANF', RNAM[p]->value.FNAM.value, RNAM[p]->value.FNAM.GetSize());
        if(RNAM[p]->value.INAM.IsLoaded())
            SaveHandler.writeSubRecord('MANI', RNAM[p]->value.INAM.value, RNAM[p]->value.INAM.GetSize());
        }
    return -1;
    }

bool FACTRecord::operator ==(const FACTRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        FULL.equals(other.FULL) &&
        DATA == other.DATA &&
        CNAM == other.CNAM &&
        XNAM.size() == other.XNAM.size() &&
        RNAM.size() == other.RNAM.size())
        {
        //Not sure if record order matters on relations, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < XNAM.size(); ++x)
            if(*XNAM[x] != *other.XNAM[x])
                return false;

        //Record order doesn't matter on faction ranks, so equality testing isn't easy
        //Instead, they're keyed by faction rank (RNAM.value.RNAM)
        //The proper solution would be to see if each rank matches the other
        //But they're usually ordered, so the lazy approach is to not bother
        //Fix-up later
        for(UINT32 x = 0; x < RNAM.size(); ++x)
            if(*RNAM[x] != *other.RNAM[x])
                return false;
        return true;
        }
    return false;
    }

bool FACTRecord::operator !=(const FACTRecord &other) const
    {
    return !(*this == other);
    }