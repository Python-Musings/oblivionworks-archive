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
#include "SOUNRecord.h"

SOUNRecord::SOUNSNDX::SOUNSNDX():
    minDistance(0),
    maxDistance(0),
    freqAdjustment(0),
    unused1(0),
    flags(0),
    staticAtten(0),
    stopTime(0),
    startTime(0)
    {
    memset(&unused2, 0x00, 2);
    }

SOUNRecord::SOUNSNDX::~SOUNSNDX()
    {
    //
    }

bool SOUNRecord::SOUNSNDX::operator ==(const SOUNSNDX &other) const
    {
    return (minDistance == other.minDistance &&
            maxDistance == other.maxDistance &&
            freqAdjustment == other.freqAdjustment &&
            flags == other.flags &&
            staticAtten == other.staticAtten &&
            stopTime == other.stopTime &&
            startTime == other.startTime);
    }

bool SOUNRecord::SOUNSNDX::operator !=(const SOUNSNDX &other) const
    {
    return !(*this == other);
    }

SOUNRecord::SOUNRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

SOUNRecord::SOUNRecord(SOUNRecord *srcRecord):
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
    FNAM = srcRecord->FNAM;
    SNDX = srcRecord->SNDX;
    }

SOUNRecord::~SOUNRecord()
    {
    //
    }

bool SOUNRecord::IsRandomFrequencyShift()
    {
    return (SNDX.value.flags & fIsRandomFrequencyShift) != 0;
    }

void SOUNRecord::IsRandomFrequencyShift(bool value)
    {
    if(value)
        SNDX.value.flags |= fIsRandomFrequencyShift;
    else
        SNDX.value.flags &= ~fIsRandomFrequencyShift;
    }

bool SOUNRecord::IsPlayAtRandom()
    {
    return (SNDX.value.flags & fIsPlayAtRandom) != 0;
    }

void SOUNRecord::IsPlayAtRandom(bool value)
    {
    if(value)
        SNDX.value.flags |= fIsPlayAtRandom;
    else
        SNDX.value.flags &= ~fIsPlayAtRandom;
    }

bool SOUNRecord::IsEnvironmentIgnored()
    {
    return (SNDX.value.flags & fIsEnvironmentIgnored) != 0;
    }

void SOUNRecord::IsEnvironmentIgnored(bool value)
    {
    if(value)
        SNDX.value.flags |= fIsEnvironmentIgnored;
    else
        SNDX.value.flags &= ~fIsEnvironmentIgnored;
    }

bool SOUNRecord::IsRandomLocation()
    {
    return (SNDX.value.flags & fIsRandomLocation) != 0;
    }

void SOUNRecord::IsRandomLocation(bool value)
    {
    if(value)
        SNDX.value.flags |= fIsRandomLocation;
    else
        SNDX.value.flags &= ~fIsRandomLocation;
    }

bool SOUNRecord::IsLoop()
    {
    return (SNDX.value.flags & fIsLoop) != 0;
    }

void SOUNRecord::IsLoop(bool value)
    {
    if(value)
        SNDX.value.flags |= fIsLoop;
    else
        SNDX.value.flags &= ~fIsLoop;
    }

bool SOUNRecord::IsMenuSound()
    {
    return (SNDX.value.flags & fIsMenuSound) != 0;
    }

void SOUNRecord::IsMenuSound(bool value)
    {
    if(value)
        SNDX.value.flags |= fIsMenuSound;
    else
        SNDX.value.flags &= ~fIsMenuSound;
    }

bool SOUNRecord::Is2D()
    {
    return (SNDX.value.flags & fIs2D) != 0;
    }

void SOUNRecord::Is2D(bool value)
    {
    if(value)
        SNDX.value.flags |= fIs2D;
    else
        SNDX.value.flags &= ~fIs2D;
    }

bool SOUNRecord::Is360LFE()
    {
    return (SNDX.value.flags & fIs360LFE) != 0;
    }

void SOUNRecord::Is360LFE(bool value)
    {
    if(value)
        SNDX.value.flags |= fIs360LFE;
    else
        SNDX.value.flags &= ~fIs360LFE;
    }

bool SOUNRecord::IsFlagMask(unsigned short Mask, bool Exact)
    {
    return Exact ? ((SNDX.value.flags & Mask) == Mask) : ((SNDX.value.flags & Mask) != 0);
    }

void SOUNRecord::SetFlagMask(unsigned short Mask)
    {
    SNDX.value.flags = Mask;
    }

UINT32 SOUNRecord::GetSize(bool forceCalc)
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

    if(FNAM.IsLoaded())
        {
        cSize = FNAM.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(SNDX.IsLoaded())
        TotSize += SNDX.GetSize() + 6;

    return TotSize;
    }

UINT32 SOUNRecord::GetType()
    {
    return 'NUOS';
    }

STRING SOUNRecord::GetStrType()
    {
    return "SOUN";
    }

SINT32 SOUNRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
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
            case 'MANF':
                FNAM.Read(buffer, subSize, curPos);
                break;
            case 'DDNS':
            case 'XDNS':
                SNDX.Read(buffer, subSize, curPos);
                //SNDX->minDistance *= 5;
                //SNDX->maxDistance *= 100;
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  SOUN: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 SOUNRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FNAM.Unload();
    SNDX.Unload();
    return 1;
    }

SINT32 SOUNRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANF', FNAM.value, FNAM.GetSize());
    if(SNDX.IsLoaded())
        if(SNDX.GetSize() == 8)
            SaveHandler.writeSubRecord('DDNS', &SNDX.value, SNDX.GetSize());
        else
            SaveHandler.writeSubRecord('XDNS', &SNDX.value, SNDX.GetSize());
    return -1;
    }

bool SOUNRecord::operator ==(const SOUNRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FNAM.equalsi(other.FNAM) &&
            SNDX == other.SNDX);
    }

bool SOUNRecord::operator !=(const SOUNRecord &other) const
    {
    return !(*this == other);
    }