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
#include "CLASRecord.h"

CLASRecord::CLASDATA::CLASDATA():
    specialization(0),
    flags(0),
    services(0),
    trainSkill(0),
    trainLevel(0)
    {
    primary[0]=0;
    primary[1]=1;
    major[0]=12;
    major[1]=13;
    major[2]=14;
    major[3]=15;
    major[4]=16;
    major[5]=17;
    major[6]=18;
    memset(&unused1, 0x00, 2);
    }

CLASRecord::CLASDATA::~CLASDATA()
    {
    //
    }

bool CLASRecord::CLASDATA::operator ==(const CLASDATA &other) const
    {
    return (primary[0] == other.primary[0] &&
            primary[1] == other.primary[1] &&
            specialization == other.specialization &&
            major[0] == other.major[0] &&
            major[1] == other.major[1] &&
            major[2] == other.major[2] &&
            major[3] == other.major[3] &&
            major[4] == other.major[4] &&
            major[5] == other.major[5] &&
            major[6] == other.major[6] &&
            flags == other.flags &&
            services == other.services &&
            trainSkill == other.trainSkill &&
            trainLevel == other.trainLevel);
    }

bool CLASRecord::CLASDATA::operator !=(const CLASDATA &other) const
    {
    return !(*this == other);
    }

CLASRecord::CLASRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

CLASRecord::CLASRecord(CLASRecord *srcRecord):
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
    DESC = srcRecord->DESC;
    ICON = srcRecord->ICON;
    DATA = srcRecord->DATA;
    return;
    }

CLASRecord::~CLASRecord()
    {
    //
    }

bool CLASRecord::IsPlayable()
    {
    return (DATA.value.flags & fIsPlayable) != 0;
    }

void CLASRecord::IsPlayable(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsPlayable) : (DATA.value.flags & ~fIsPlayable);
    }

bool CLASRecord::IsGuard()
    {
    return (DATA.value.flags & fIsGuard) != 0;
    }

void CLASRecord::IsGuard(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsGuard) : (DATA.value.flags & ~fIsGuard);
    }

bool CLASRecord::IsServicesWeapons()
    {
    return (DATA.value.services & fWeapons) != 0;
    }

void CLASRecord::IsServicesWeapons(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fWeapons) : (DATA.value.services & ~fWeapons);
    }

bool CLASRecord::IsServicesArmor()
    {
    return (DATA.value.services & fArmor) != 0;
    }

void CLASRecord::IsServicesArmor(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fArmor) : (DATA.value.services & ~fArmor);
    }

bool CLASRecord::IsServicesClothing()
    {
    return (DATA.value.services & fClothing) != 0;
    }

void CLASRecord::IsServicesClothing(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fClothing) : (DATA.value.services & ~fClothing);
    }

bool CLASRecord::IsServicesBooks()
    {
    return (DATA.value.services & fBooks) != 0;
    }

void CLASRecord::IsServicesBooks(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fBooks) : (DATA.value.services & ~fBooks);
    }

bool CLASRecord::IsServicesIngredients()
    {
    return (DATA.value.services & fIngredients) != 0;
    }

void CLASRecord::IsServicesIngredients(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fIngredients) : (DATA.value.services & ~fIngredients);
    }

bool CLASRecord::IsServicesLights()
    {
    return (DATA.value.services & fLights) != 0;
    }

void CLASRecord::IsServicesLights(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fLights) : (DATA.value.services & ~fLights);
    }

bool CLASRecord::IsServicesApparatus()
    {
    return (DATA.value.services & fApparatus) != 0;
    }

void CLASRecord::IsServicesApparatus(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fApparatus) : (DATA.value.services & ~fApparatus);
    }

bool CLASRecord::IsServicesMiscItems()
    {
    return (DATA.value.services & fMiscItems) != 0;
    }

void CLASRecord::IsServicesMiscItems(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fMiscItems) : (DATA.value.services & ~fMiscItems);
    }

bool CLASRecord::IsServicesSpells()
    {
    return (DATA.value.services & fSpells) != 0;
    }

void CLASRecord::IsServicesSpells(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fSpells) : (DATA.value.services & ~fSpells);
    }

bool CLASRecord::IsServicesMagicItems()
    {
    return (DATA.value.services & fMagicItems) != 0;
    }

void CLASRecord::IsServicesMagicItems(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fMagicItems) : (DATA.value.services & ~fMagicItems);
    }

bool CLASRecord::IsServicesPotions()
    {
    return (DATA.value.services & fPotions) != 0;
    }

void CLASRecord::IsServicesPotions(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fPotions) : (DATA.value.services & ~fPotions);
    }

bool CLASRecord::IsServicesTraining()
    {
    return (DATA.value.services & fTraining) != 0;
    }

void CLASRecord::IsServicesTraining(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fTraining) : (DATA.value.services & ~fTraining);
    }

bool CLASRecord::IsServicesRecharge()
    {
    return (DATA.value.services & fRecharge) != 0;
    }

void CLASRecord::IsServicesRecharge(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fRecharge) : (DATA.value.services & ~fRecharge);
    }

bool CLASRecord::IsServicesRepair()
    {
    return (DATA.value.services & fRepair) != 0;
    }

void CLASRecord::IsServicesRepair(bool value)
    {
    DATA.value.services = value ? (DATA.value.services | fRepair) : (DATA.value.services & ~fRepair);
    }

bool CLASRecord::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void CLASRecord::SetFlagMask(UINT32 Mask)
    {
    DATA.value.flags = Mask;
    }

bool CLASRecord::IsServicesFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.services & Mask) == Mask) : ((DATA.value.services & Mask) != 0);
    }

void CLASRecord::SetServicesFlagMask(UINT32 Mask)
    {
    DATA.value.services = Mask;
    }

UINT32 CLASRecord::GetSize(bool forceCalc)
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

    if(DESC.IsLoaded())
        {
        cSize = DESC.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ICON.IsLoaded())
        {
        cSize = ICON.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    return TotSize;
    }

UINT32 CLASRecord::GetType()
    {
    return 'SALC';
    }

STRING CLASRecord::GetStrType()
    {
    return "CLAS";
    }

SINT32 CLASRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'LLUF':
                FULL.Read(buffer, subSize, curPos);
                break;
            case 'CSED':
                DESC.Read(buffer, subSize, curPos);
                break;
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  CLAS: Unknown subType = %04X\n", subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 CLASRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    DESC.Unload();
    ICON.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 CLASRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(DESC.IsLoaded())
        SaveHandler.writeSubRecord('CSED', DESC.value, DESC.GetSize());
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    return -1;
    }

bool CLASRecord::operator ==(const CLASRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            DESC.equals(other.DESC) &&
            ICON.equalsi(other.ICON) &&
            DATA == other.DATA);
    }

bool CLASRecord::operator !=(const CLASRecord &other) const
    {
    return !(*this == other);
    }