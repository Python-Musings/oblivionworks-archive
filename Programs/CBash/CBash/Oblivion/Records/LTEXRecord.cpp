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
#include "LTEXRecord.h"
#include <vector>

LTEXRecord::LTEXHNAM::LTEXHNAM():
    flags(0),
    friction(0),
    restitution(0)
    {
    //
    }

LTEXRecord::LTEXHNAM::~LTEXHNAM()
    {
    //
    }

bool LTEXRecord::LTEXHNAM::operator ==(const LTEXHNAM &other) const
    {
    return (flags == other.flags &&
            friction == other.friction &&
            restitution == other.restitution);
    }
bool LTEXRecord::LTEXHNAM::operator !=(const LTEXHNAM &other) const
    {
    return !(*this == other);
    }

LTEXRecord::LTEXSNAM::LTEXSNAM():
    specular(0)
    {
    //
    }

LTEXRecord::LTEXSNAM::~LTEXSNAM()
    {
    //
    }

bool LTEXRecord::LTEXSNAM::operator ==(const LTEXSNAM &other) const
    {
    return (specular == other.specular);
    }

bool LTEXRecord::LTEXSNAM::operator !=(const LTEXSNAM &other) const
    {
    return !(*this == other);
    }

LTEXRecord::LTEXRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

LTEXRecord::LTEXRecord(LTEXRecord *srcRecord):
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
    ICON = srcRecord->ICON;
    HNAM = srcRecord->HNAM;
    SNAM = srcRecord->SNAM;

    GNAM.resize(srcRecord->GNAM.size());
    for(UINT32 x = 0; x < srcRecord->GNAM.size(); x++)
        GNAM[x] = srcRecord->GNAM[x];
    return;
    }

LTEXRecord::~LTEXRecord()
    {
    //
    }

bool LTEXRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < GNAM.size(); x++)
        op.Accept(GNAM[x]);

    return op.Stop();
    }

bool LTEXRecord::IsStone()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsStone) != 0;
    }

void LTEXRecord::IsStone(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsStone) : (HNAM.value.flags & ~fIsStone);
    }

bool LTEXRecord::IsCloth()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsCloth) != 0;
    }

void LTEXRecord::IsCloth(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsCloth) : (HNAM.value.flags & ~fIsCloth);
    }

bool LTEXRecord::IsDirt()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsDirt) != 0;
    }

void LTEXRecord::IsDirt(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsDirt) : (HNAM.value.flags & ~fIsDirt);
    }

bool LTEXRecord::IsGlass()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsGlass) != 0;
    }

void LTEXRecord::IsGlass(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsGlass) : (HNAM.value.flags & ~fIsGlass);
    }

bool LTEXRecord::IsGrass()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsGrass) != 0;
    }

void LTEXRecord::IsGrass(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsGrass) : (HNAM.value.flags & ~fIsGrass);
    }

bool LTEXRecord::IsMetal()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsMetal) != 0;
    }

void LTEXRecord::IsMetal(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsMetal) : (HNAM.value.flags & ~fIsMetal);
    }

bool LTEXRecord::IsOrganic()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsOrganic) != 0;
    }

void LTEXRecord::IsOrganic(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsOrganic) : (HNAM.value.flags & ~fIsOrganic);
    }

bool LTEXRecord::IsSkin()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsSkin) != 0;
    }

void LTEXRecord::IsSkin(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsSkin) : (HNAM.value.flags & ~fIsSkin);
    }

bool LTEXRecord::IsWater()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsWater) != 0;
    }

void LTEXRecord::IsWater(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsWater) : (HNAM.value.flags & ~fIsWater);
    }

bool LTEXRecord::IsWood()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsWood) != 0;
    }

void LTEXRecord::IsWood(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsWood) : (HNAM.value.flags & ~fIsWood);
    }

bool LTEXRecord::IsHeavyStone()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsHeavyStone) != 0;
    }

void LTEXRecord::IsHeavyStone(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsHeavyStone) : (HNAM.value.flags & ~fIsHeavyStone);
    }

bool LTEXRecord::IsHeavyMetal()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsHeavyMetal) != 0;
    }

void LTEXRecord::IsHeavyMetal(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsHeavyMetal) : (HNAM.value.flags & ~fIsHeavyMetal);
    }

bool LTEXRecord::IsHeavyWood()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsHeavyWood) != 0;
    }

void LTEXRecord::IsHeavyWood(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsHeavyWood) : (HNAM.value.flags & ~fIsHeavyWood);
    }

bool LTEXRecord::IsChain()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsChain) != 0;
    }

void LTEXRecord::IsChain(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsChain) : (HNAM.value.flags & ~fIsChain);
    }

bool LTEXRecord::IsSnow()
    {
    HNAM.Load();
    return (HNAM.value.flags & fIsSnow) != 0;
    }

void LTEXRecord::IsSnow(bool value)
    {
    HNAM.Load();
    HNAM.value.flags = value ? (HNAM.value.flags | fIsSnow) : (HNAM.value.flags & ~fIsSnow);
    }

bool LTEXRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    HNAM.Load();
    return Exact ? ((HNAM.value.flags & Mask) == Mask) : ((HNAM.value.flags & Mask) != 0);
    }

void LTEXRecord::SetFlagMask(UINT8 Mask)
    {
    HNAM.Load();
    HNAM.value.flags = Mask;
    }

UINT32 LTEXRecord::GetSize(bool forceCalc)
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

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(HNAM.IsLoaded())
        TotSize += HNAM.GetSize() + 6;

    if(SNAM.IsLoaded())
        TotSize += SNAM.GetSize() + 6;

    if(GNAM.size())
        TotSize += (UINT32)GNAM.size() * (sizeof(UINT32) + 6);

    return TotSize;
    }

UINT32 LTEXRecord::GetType()
    {
    return 'XETL';
    }


STRING LTEXRecord::GetStrType()
    {
    return "LTEX";
    }

SINT32 LTEXRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case 'MANH':
                HNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANS':
                SNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANG':
                _readBuffer(&curFormID,buffer,subSize,curPos);
                GNAM.push_back(curFormID);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  LTEX: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 LTEXRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    ICON.Unload();
    HNAM.Unload();
    SNAM.Unload();
    GNAM.clear();
    return 1;
    }

SINT32 LTEXRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(HNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANH', &HNAM.value, HNAM.GetSize());
    if(SNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANS', &SNAM.value, SNAM.GetSize());
    for(UINT32 p = 0; p < GNAM.size(); p++)
        SaveHandler.writeSubRecord('MANG', &GNAM[p], sizeof(UINT32));
    return -1;
    }

bool LTEXRecord::operator ==(const LTEXRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        ICON.equals(other.ICON) &&
        HNAM == other.HNAM &&
        SNAM == other.SNAM &&
        GNAM.size() == other.GNAM.size())
        {
        //Not sure if record order matters on grasses, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < GNAM.size(); ++x)
            if(GNAM[x] != other.GNAM[x])
                return false;
        return true;
        }
    return false;
    }

bool LTEXRecord::operator !=(const LTEXRecord &other) const
    {
    return !(*this == other);
    }