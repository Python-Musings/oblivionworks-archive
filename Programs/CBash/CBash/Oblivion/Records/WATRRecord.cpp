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
#include "WATRRecord.h"

WATRRecord::WATRANAM::WATRANAM():
    opacity(75)
    {
    //
    }

WATRRecord::WATRANAM::~WATRANAM()
    {
    //
    }

bool WATRRecord::WATRANAM::operator ==(const WATRANAM &other) const
    {
    return (opacity == other.opacity);
    }
bool WATRRecord::WATRANAM::operator !=(const WATRANAM &other) const
    {
    return !(*this == other);
    }

WATRRecord::WATRDATA::WATRDATA():
    windVelocity(0.1f),
    windDirection(90.0f),
    waveAmp(0.5f),
    waveFreq(1.0f),
    sunPower(50.0f),
    reflectAmt(0.5f),
    fresnelAmt(0.025f),
    xSpeed(0.0f),
    ySpeed(0.0f),
    fogNear(27852.801f),
    fogFar(163840.0f),
    shallow(0, 128, 128, 0),
    deep(0, 0, 25, 0),
    refl(255, 255, 255, 0),
    blend(50),
    rainForce(0.1f),
    rainVelocity(0.6f),
    rainFalloff(0.985f),
    rainDampner(2.0f),
    rainSize(0.01f),
    dispForce(0.4f),
    dispVelocity(0.6f),
    dispFalloff(0.985f),
    dispDampner(10.0f),
    dispSize(0.05f),
    damage(0)
    {
    memset(&unused1, 0xCD, 3);
    }

WATRRecord::WATRDATA::~WATRDATA()
    {
    //
    }

bool WATRRecord::WATRDATA::operator ==(const WATRDATA &other) const
    {
    return (AlmostEqual(windVelocity,other.windVelocity,2) &&
            AlmostEqual(windDirection,other.windDirection,2) &&
            AlmostEqual(waveAmp,other.waveAmp,2) &&
            AlmostEqual(waveFreq,other.waveFreq,2) &&
            AlmostEqual(sunPower,other.sunPower,2) &&
            AlmostEqual(reflectAmt,other.reflectAmt,2) &&
            AlmostEqual(fresnelAmt,other.fresnelAmt,2) &&
            AlmostEqual(xSpeed,other.xSpeed,2) &&
            AlmostEqual(ySpeed,other.ySpeed,2) &&
            AlmostEqual(fogNear,other.fogNear,2) &&
            AlmostEqual(fogFar,other.fogFar,2) &&
            shallow == other.shallow &&
            deep == other.deep &&
            refl == other.refl &&
            blend == other.blend &&
            AlmostEqual(rainForce,other.rainForce,2) &&
            AlmostEqual(rainVelocity,other.rainVelocity,2) &&
            AlmostEqual(rainFalloff,other.rainFalloff,2) &&
            AlmostEqual(rainDampner,other.rainDampner,2) &&
            AlmostEqual(rainSize,other.rainSize,2) &&
            AlmostEqual(dispForce,other.dispForce,2) &&
            AlmostEqual(dispVelocity,other.dispVelocity,2) &&
            AlmostEqual(dispFalloff,other.dispFalloff,2) &&
            AlmostEqual(dispDampner,other.dispDampner,2) &&
            AlmostEqual(dispSize,other.dispSize,2) &&
            damage == other.damage);
    }

bool WATRRecord::WATRDATA::operator !=(const WATRDATA &other) const
    {
    return !(*this == other);
    }

WATRRecord::WATRGNAM::WATRGNAM():
    dayWater(0),
    nightWater(0),
    underWater(0)
    {
    //
    }

WATRRecord::WATRGNAM::~WATRGNAM()
    {
    //
    }

bool WATRRecord::WATRGNAM::operator ==(const WATRGNAM &other) const
    {
    return (dayWater == other.dayWater &&
            nightWater == other.nightWater &&
            underWater == other.underWater);
    }

bool WATRRecord::WATRGNAM::operator !=(const WATRGNAM &other) const
    {
    return !(*this == other);
    }

WATRRecord::WATRRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

WATRRecord::WATRRecord(WATRRecord *srcRecord):
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
    TNAM = srcRecord->TNAM;
    ANAM = srcRecord->ANAM;
    FNAM = srcRecord->FNAM;
    MNAM = srcRecord->MNAM;
    SNAM = srcRecord->SNAM;
    DATA = srcRecord->DATA;
    GNAM = srcRecord->GNAM;
    }

WATRRecord::~WATRRecord()
    {
    //
    }

bool WATRRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(SNAM.IsLoaded())
        op.Accept(SNAM->fid);
    if(GNAM.IsLoaded())
        {
        op.Accept(GNAM->dayWater);
        op.Accept(GNAM->nightWater);
        op.Accept(GNAM->underWater);
        }

    return op.Stop();
    }

bool WATRRecord::IsCausesDmg()
    {
    return (FNAM.value.flags & fIsCausesDamage) != 0;
    }

void WATRRecord::IsCausesDmg(bool value)
    {
    if(value)
        FNAM.value.flags |= fIsCausesDamage;
    else
        FNAM.value.flags &= ~fIsCausesDamage;
    }

bool WATRRecord::IsCausesDamage()
    {
    return (FNAM.value.flags & fIsCausesDamage) != 0;
    }

void WATRRecord::IsCausesDamage(bool value)
    {
    if(value)
        FNAM.value.flags |= fIsCausesDamage;
    else
        FNAM.value.flags &= ~fIsCausesDamage;
    }

bool WATRRecord::IsReflective()
    {
    return (FNAM.value.flags & fIsReflective) != 0;
    }

void WATRRecord::IsReflective(bool value)
    {
    if(value)
        FNAM.value.flags |= fIsReflective;
    else
        FNAM.value.flags &= ~fIsReflective;
    }

bool WATRRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((FNAM.value.flags & Mask) == Mask) : ((FNAM.value.flags & Mask) != 0);
    }

void WATRRecord::SetFlagMask(UINT8 Mask)
    {
    FNAM.value.flags = Mask;
    }

UINT32 WATRRecord::GetSize(bool forceCalc)
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

    if(TNAM.IsLoaded())
        {
        cSize = TNAM.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ANAM.IsLoaded())
        TotSize += ANAM.GetSize() + 6;

    if(FNAM.IsLoaded())
        TotSize += FNAM.GetSize() + 6;

    if(MNAM.IsLoaded())
        {
        cSize = MNAM.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(SNAM.IsLoaded())
        TotSize += SNAM.GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(GNAM.IsLoaded())
        TotSize += GNAM.GetSize() + 6;

    return TotSize;
    }

UINT32 WATRRecord::GetType()
    {
    return 'RTAW';
    }

STRING WATRRecord::GetStrType()
    {
    return "WATR";
    }

SINT32 WATRRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'MANT':
                TNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANA':
                ANAM.Read(buffer, subSize, curPos);
                break;
            case 'MANF':
                FNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANM':
                MNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANS':
                SNAM.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'MANG':
                GNAM.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  WATR: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 WATRRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    TNAM.Unload();
    ANAM.Unload();
    FNAM.Unload();
    MNAM.Unload();
    SNAM.Unload();
    DATA.Unload();
    GNAM.Unload();
    return 1;
    }

SINT32 WATRRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(TNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANT', TNAM.value, TNAM.GetSize());
    if(ANAM.IsLoaded())
        SaveHandler.writeSubRecord('MANA', &ANAM.value, ANAM.GetSize());
    if(FNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANF', &FNAM.value, FNAM.GetSize());
    if(MNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANM', MNAM.value, MNAM.GetSize());
    if(SNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANS', SNAM.value, SNAM.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', DATA.value, DATA.GetSize());
    if(GNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANG', GNAM.value, GNAM.GetSize());
    return -1;
    }

bool WATRRecord::operator ==(const WATRRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            TNAM.equalsi(other.TNAM) &&
            ANAM == other.ANAM &&
            FNAM == other.FNAM &&
            MNAM.equalsi(other.MNAM) &&
            SNAM == other.SNAM &&
            DATA == other.DATA &&
            GNAM == other.GNAM);
    }

bool WATRRecord::operator !=(const WATRRecord &other) const
    {
    return !(*this == other);
    }