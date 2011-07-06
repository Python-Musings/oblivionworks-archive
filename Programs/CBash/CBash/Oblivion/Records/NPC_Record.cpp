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
#include "NPC_Record.h"
#include <vector>

NPC_Record::NPC_DATA::NPC_DATA():
    armorer(0),
    athletics(0),
    blade(0),
    block(0),
    blunt(0),
    h2h(0),
    heavyArmor(0),
    alchemy(0),
    alteration(0),
    conjuration(0),
    destruction(0),
    illusion(0),
    mysticism(0),
    restoration(0),
    acrobatics(0),
    lightArmor(0),
    marksman(0),
    mercantile(0),
    security(0),
    sneak(0),
    speechcraft(0),
    health(0),
    strength(0),
    intelligence(0),
    willpower(0),
    agility(0),
    speed(0),
    endurance(0),
    personality(0),
    luck(0)
    {
    memset(&unused1, 0x00, 2);
    }

NPC_Record::NPC_DATA::~NPC_DATA()
    {
    //
    }

bool NPC_Record::NPC_DATA::operator ==(const NPC_DATA &other) const
    {
    return (armorer == other.armorer &&
            athletics == other.athletics &&
            blade == other.blade &&
            block == other.block &&
            blunt == other.blunt &&
            h2h == other.h2h &&
            heavyArmor == other.heavyArmor &&
            alchemy == other.alchemy &&
            alteration == other.alteration &&
            conjuration == other.conjuration &&
            destruction == other.destruction &&
            illusion == other.illusion &&
            mysticism == other.mysticism &&
            restoration == other.restoration &&
            acrobatics == other.acrobatics &&
            lightArmor == other.lightArmor &&
            marksman == other.marksman &&
            mercantile == other.mercantile &&
            security == other.security &&
            sneak == other.sneak &&
            speechcraft == other.speechcraft &&
            health == other.health &&
            strength == other.strength &&
            intelligence == other.intelligence &&
            willpower == other.willpower &&
            agility == other.agility &&
            speed == other.speed &&
            endurance == other.endurance &&
            personality == other.personality &&
            luck == other.luck);
    }

bool NPC_Record::NPC_DATA::operator !=(const NPC_DATA &other) const
    {
    return !(*this == other);
    }

NPC_Record::NPC_LNAM::NPC_LNAM():
    hairLength(0.0f)
    {
    //
    }

NPC_Record::NPC_LNAM::~NPC_LNAM()
    {
    //
    }

bool NPC_Record::NPC_LNAM::operator ==(const NPC_LNAM &other) const
    {
    return (AlmostEqual(hairLength,other.hairLength,2));
    }

bool NPC_Record::NPC_LNAM::operator !=(const NPC_LNAM &other) const
    {
    return !(*this == other);
    }

NPC_Record::NPC_FNAM::NPC_FNAM():
    fnam(0)
    {
    //
    }

NPC_Record::NPC_FNAM::~NPC_FNAM()
    {
    //
    }

bool NPC_Record::NPC_FNAM::operator ==(const NPC_FNAM &other) const
    {
    return (fnam == other.fnam);
    }

bool NPC_Record::NPC_FNAM::operator !=(const NPC_FNAM &other) const
    {
    return !(*this == other);
    }

NPC_Record::NPC_Record(unsigned char *_recData):
    Record(_recData)
    {
    //NPC_ records are normally compressed
    IsCompressed(true);
    }

NPC_Record::NPC_Record(NPC_Record *srcRecord):
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
    if(srcRecord->MODL.IsLoaded())
        {
        MODL.Load();
        MODL->MODB = srcRecord->MODL->MODB;
        MODL->MODL = srcRecord->MODL->MODL;
        MODL->MODT = srcRecord->MODL->MODT;
        }
    ACBS = srcRecord->ACBS;
    SNAM.clear();
    SNAM.resize(srcRecord->SNAM.size());
    for(UINT32 x = 0; x < srcRecord->SNAM.size(); x++)
        {
        SNAM[x] = new ReqSubRecord<GENSNAM>;
        *SNAM[x] = *srcRecord->SNAM[x];
        }
    INAM = srcRecord->INAM;
    RNAM = srcRecord->RNAM;
    SPLO.resize(srcRecord->SPLO.size());
    for(UINT32 x = 0; x < srcRecord->SPLO.size(); x++)
        SPLO[x] = srcRecord->SPLO[x];
    SCRI = srcRecord->SCRI;
    CNTO.clear();
    CNTO.resize(srcRecord->CNTO.size());
    for(UINT32 x = 0; x < srcRecord->CNTO.size(); x++)
        {
        CNTO[x] = new ReqSubRecord<GENCNTO>;
        *CNTO[x] = *srcRecord->CNTO[x];
        }
    AIDT = srcRecord->AIDT;
    PKID.resize(srcRecord->PKID.size());
    for(UINT32 x = 0; x < srcRecord->PKID.size(); x++)
        PKID[x] = srcRecord->PKID[x];
    KFFZ.clear();
    KFFZ.resize(srcRecord->KFFZ.size());
    for(UINT32 x = 0; x < srcRecord->KFFZ.size(); x++)
        KFFZ[x].Copy(srcRecord->KFFZ[x]);
    CNAM = srcRecord->CNAM;
    DATA = srcRecord->DATA;
    HNAM = srcRecord->HNAM;
    LNAM = srcRecord->LNAM;
    ENAM = srcRecord->ENAM;
    HCLR = srcRecord->HCLR;
    ZNAM = srcRecord->ZNAM;
    FGGS = srcRecord->FGGS;
    FGGA = srcRecord->FGGA;
    FGTS = srcRecord->FGTS;
    FNAM = srcRecord->FNAM;
    }

NPC_Record::~NPC_Record()
    {
    for(UINT32 x = 0; x < SNAM.size(); x++)
        delete SNAM[x];
    for(UINT32 x = 0; x < CNTO.size(); x++)
        delete CNTO[x];
    }

bool NPC_Record::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < SNAM.size(); x++)
        op.Accept(SNAM[x]->value.faction);

    if(INAM.IsLoaded())
        op.Accept(INAM->fid);

    if(RNAM.IsLoaded())
        op.Accept(RNAM->fid);

    for(UINT32 x = 0; x < SPLO.size(); x++)
        op.Accept(SPLO[x]);

    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);

    for(UINT32 x = 0; x < CNTO.size(); x++)
        op.Accept(CNTO[x]->value.item);

    for(UINT32 x = 0; x < PKID.size(); x++)
        op.Accept(PKID[x]);

    if(CNAM.IsLoaded())
        op.Accept(CNAM.value.fid);

    if(HNAM.IsLoaded())
        op.Accept(HNAM->fid);

    if(ENAM.IsLoaded())
        op.Accept(ENAM->fid);

    if(ZNAM.IsLoaded())
        op.Accept(ZNAM->fid);

    return op.Stop();
    }

bool NPC_Record::IsFemale()
    {
    return (ACBS.value.flags & fIsFemale) != 0;
    }

void NPC_Record::IsFemale(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsFemale) : (ACBS.value.flags & ~fIsFemale);
    }

bool NPC_Record::IsMale()
    {
    return !IsFemale();
    }

void NPC_Record::IsMale(bool value)
    {
    IsFemale(!value);
    }

bool NPC_Record::IsEssential()
    {
    return (ACBS.value.flags & fIsEssential) != 0;
    }

void NPC_Record::IsEssential(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsEssential) : (ACBS.value.flags & ~fIsEssential);
    }

bool NPC_Record::IsRespawn()
    {
    return (ACBS.value.flags & fIsRespawn) != 0;
    }

void NPC_Record::IsRespawn(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsRespawn) : (ACBS.value.flags & ~fIsRespawn);
    }

bool NPC_Record::IsAutoCalc()
    {
    return (ACBS.value.flags & fIsAutoCalc) != 0;
    }

void NPC_Record::IsAutoCalc(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsAutoCalc) : (ACBS.value.flags & ~fIsAutoCalc);
    }

bool NPC_Record::IsPCLevelOffset()
    {
    return (ACBS.value.flags & fIsPCLevelOffset) != 0;
    }

void NPC_Record::IsPCLevelOffset(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsPCLevelOffset) : (ACBS.value.flags & ~fIsPCLevelOffset);
    }

bool NPC_Record::IsNoLowLevel()
    {
    return (ACBS.value.flags & fIsNoLowLevel) != 0;
    }

void NPC_Record::IsNoLowLevel(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoLowLevel) : (ACBS.value.flags & ~fIsNoLowLevel);
    }

bool NPC_Record::IsLowLevel()
    {
    return !IsNoLowLevel();
    }

void NPC_Record::IsLowLevel(bool value)
    {
    IsNoLowLevel(!value);
    }

bool NPC_Record::IsNoRumors()
    {
    return (ACBS.value.flags & fIsNoRumors) != 0;
    }

void NPC_Record::IsNoRumors(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoRumors) : (ACBS.value.flags & ~fIsNoRumors);
    }

bool NPC_Record::IsRumors()
    {
    return !IsNoRumors();
    }

void NPC_Record::IsRumors(bool value)
    {
    IsNoRumors(!value);
    }

bool NPC_Record::IsSummonable()
    {
    return (ACBS.value.flags & fIsSummonable) != 0;
    }

void NPC_Record::IsSummonable(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsSummonable) : (ACBS.value.flags & ~fIsSummonable);
    }

bool NPC_Record::IsNoPersuasion()
    {
    return (ACBS.value.flags & fIsNoPersuasion) != 0;
    }

void NPC_Record::IsNoPersuasion(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsNoPersuasion) : (ACBS.value.flags & ~fIsNoPersuasion);
    }

bool NPC_Record::IsPersuasion()
    {
    return !IsNoPersuasion();
    }

void NPC_Record::IsPersuasion(bool value)
    {
    IsNoPersuasion(!value);
    }

bool NPC_Record::IsCanCorpseCheck()
    {
    return (ACBS.value.flags & fIsCanCorpseCheck) != 0;
    }

void NPC_Record::IsCanCorpseCheck(bool value)
    {
    ACBS.value.flags = value ? (ACBS.value.flags | fIsCanCorpseCheck) : (ACBS.value.flags & ~fIsCanCorpseCheck);
    }

bool NPC_Record::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((ACBS.value.flags & Mask) == Mask) : ((ACBS.value.flags & Mask) != 0);
    }

void NPC_Record::SetFlagMask(UINT32 Mask)
    {
    ACBS.value.flags = Mask;
    }

bool NPC_Record::IsServicesWeapons()
    {
    return (AIDT.value.flags & fWeapons) != 0;
    }

void NPC_Record::IsServicesWeapons(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fWeapons) : (AIDT.value.flags & ~fWeapons);
    }

bool NPC_Record::IsServicesArmor()
    {
    return (AIDT.value.flags & fArmor) != 0;
    }

void NPC_Record::IsServicesArmor(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fArmor) : (AIDT.value.flags & ~fArmor);
    }

bool NPC_Record::IsServicesClothing()
    {
    return (AIDT.value.flags & fClothing) != 0;
    }

void NPC_Record::IsServicesClothing(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fClothing) : (AIDT.value.flags & ~fClothing);
    }

bool NPC_Record::IsServicesBooks()
    {
    return (AIDT.value.flags & fBooks) != 0;
    }

void NPC_Record::IsServicesBooks(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fBooks) : (AIDT.value.flags & ~fBooks);
    }

bool NPC_Record::IsServicesIngredients()
    {
    return (AIDT.value.flags & fIngredients) != 0;
    }

void NPC_Record::IsServicesIngredients(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fIngredients) : (AIDT.value.flags & ~fIngredients);
    }

bool NPC_Record::IsServicesLights()
    {
    return (AIDT.value.flags & fLights) != 0;
    }

void NPC_Record::IsServicesLights(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fLights) : (AIDT.value.flags & ~fLights);
    }

bool NPC_Record::IsServicesApparatus()
    {
    return (AIDT.value.flags & fApparatus) != 0;
    }

void NPC_Record::IsServicesApparatus(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fApparatus) : (AIDT.value.flags & ~fApparatus);
    }

bool NPC_Record::IsServicesMiscItems()
    {
    return (AIDT.value.flags & fMiscItems) != 0;
    }

void NPC_Record::IsServicesMiscItems(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fMiscItems) : (AIDT.value.flags & ~fMiscItems);
    }

bool NPC_Record::IsServicesSpells()
    {
    return (AIDT.value.flags & fSpells) != 0;
    }

void NPC_Record::IsServicesSpells(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fSpells) : (AIDT.value.flags & ~fSpells);
    }

bool NPC_Record::IsServicesMagicItems()
    {
    return (AIDT.value.flags & fMagicItems) != 0;
    }

void NPC_Record::IsServicesMagicItems(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fMagicItems) : (AIDT.value.flags & ~fMagicItems);
    }

bool NPC_Record::IsServicesPotions()
    {
    return (AIDT.value.flags & fPotions) != 0;
    }

void NPC_Record::IsServicesPotions(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fPotions) : (AIDT.value.flags & ~fPotions);
    }

bool NPC_Record::IsServicesTraining()
    {
    return (AIDT.value.flags & fTraining) != 0;
    }

void NPC_Record::IsServicesTraining(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fTraining) : (AIDT.value.flags & ~fTraining);
    }

bool NPC_Record::IsServicesRecharge()
    {
    return (AIDT.value.flags & fRecharge) != 0;
    }

void NPC_Record::IsServicesRecharge(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fRecharge) : (AIDT.value.flags & ~fRecharge);
    }

bool NPC_Record::IsServicesRepair()
    {
    return (AIDT.value.flags & fRepair) != 0;
    }

void NPC_Record::IsServicesRepair(bool value)
    {
    AIDT.value.flags = value ? (AIDT.value.flags | fRepair) : (AIDT.value.flags & ~fRepair);
    }

bool NPC_Record::IsServicesFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((AIDT.value.flags & Mask) == Mask) : ((AIDT.value.flags & Mask) != 0);
    }

void NPC_Record::SetServicesFlagMask(UINT32 Mask)
    {
    AIDT.value.flags = Mask;
    }

UINT32 NPC_Record::GetSize(bool forceCalc)
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

    if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
        {
        cSize = MODL->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MODL->MODB.IsLoaded())
            TotSize += MODL->MODB.GetSize() + 6;

        if(MODL->MODT.IsLoaded())
            {
            cSize = MODL->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(ACBS.IsLoaded())
        TotSize += ACBS.GetSize() + 6;

    for(UINT32 p = 0; p < SNAM.size(); p++)
        if(SNAM[p]->IsLoaded())
            TotSize += SNAM[p]->GetSize() + 6;

    if(INAM.IsLoaded())
        TotSize += INAM.GetSize() + 6;

    if(RNAM.IsLoaded())
        TotSize += RNAM.GetSize() + 6;

    if(SPLO.size())
        TotSize += (UINT32)SPLO.size() * (sizeof(UINT32) + 6);

    if(SCRI.IsLoaded())
        TotSize += SCRI.GetSize() + 6;

    for(UINT32 p = 0; p < CNTO.size(); p++)
        if(CNTO[p]->IsLoaded())
            TotSize += CNTO[p]->GetSize() + 6;

    if(AIDT.IsLoaded())
        TotSize += AIDT.GetSize() + 6;

    if(PKID.size())
        TotSize += (UINT32)PKID.size() * (sizeof(UINT32) + 6);

    if(KFFZ.size())
        {
        cSize = 1; //Type, size, and final null terminator
        for(UINT32 p = 0; p < KFFZ.size(); p++)
            if(KFFZ[p].IsLoaded())
                cSize += KFFZ[p].GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(CNAM.IsLoaded())
        TotSize += CNAM.GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(HNAM.IsLoaded())
        TotSize += HNAM.GetSize() + 6;

    if(LNAM.IsLoaded())
        TotSize += LNAM.GetSize() + 6;

    if(ENAM.IsLoaded())
        TotSize += ENAM.GetSize() + 6;

    if(HCLR.IsLoaded())
        TotSize += HCLR.GetSize() + 6;

    if(ZNAM.IsLoaded())
        TotSize += ZNAM.GetSize() + 6;

    if(FGGS.IsLoaded())
        TotSize += FGGS.GetSize() + 6;

    if(FGGA.IsLoaded())
        TotSize += FGGA.GetSize() + 6;

    if(FGTS.IsLoaded())
        TotSize += FGTS.GetSize() + 6;

    if(FNAM.IsLoaded())
        TotSize += FNAM.GetSize() + 6;

    return TotSize;
    }

UINT32 NPC_Record::GetType()
    {
    return '_CPN';
    }


STRING NPC_Record::GetStrType()
    {
    return "NPC_";
    }

SINT32 NPC_Record::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    FORMID curFormID = 0;
    ReqSubRecord<GENSNAM> *newSNAM = NULL;
    ReqSubRecord<GENCNTO> *newCNTO = NULL;
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
            case 'LDOM':
                MODL.Load();
                MODL->MODL.Read(buffer, subSize, curPos);
                break;
            case 'BDOM':
                MODL.Load();
                MODL->MODB.Read(buffer, subSize, curPos);
                break;
            case 'TDOM':
                MODL.Load();
                MODL->MODT.Read(buffer, subSize, curPos);
                break;
            case 'SBCA':
                ACBS.Read(buffer, subSize, curPos);
                break;
            case 'MANS':
                newSNAM = new ReqSubRecord<GENSNAM>;
                newSNAM->Read(buffer, subSize, curPos);
                SNAM.push_back(newSNAM);
                break;
            case 'MANI':
                INAM.Read(buffer, subSize, curPos);
                break;
            case 'MANR':
                RNAM.Read(buffer, subSize, curPos);
                break;
            case 'OLPS':
                _readBuffer(&curFormID,buffer,subSize,curPos);
                SPLO.push_back(curFormID);
                break;
            case 'IRCS':
                SCRI.Read(buffer, subSize, curPos);
                break;
            case 'OTNC':
                newCNTO = new ReqSubRecord<GENCNTO>;
                newCNTO->Read(buffer, subSize, curPos);
                CNTO.push_back(newCNTO);
                break;
            case 'TDIA':
                AIDT.Read(buffer, subSize, curPos);
                break;
            case 'DIKP':
                _readBuffer(&curFormID, buffer, subSize, curPos);
                PKID.push_back(curFormID);
                break;
            case 'ZFFK':
                for(subSize += curPos;curPos < (subSize - 1);curPos += (UINT32)strlen((char*)&buffer[curPos]) + 1)
                    KFFZ.push_back(StringRecord((char*)&buffer[curPos]));
                curPos++;
                break;
            case 'MANC':
                CNAM.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'MANH':
                HNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANL':
                LNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANE':
                ENAM.Read(buffer, subSize, curPos);
                break;
            case 'RLCH':
                HCLR.Read(buffer, subSize, curPos);
                break;
            case 'MANZ':
                ZNAM.Read(buffer, subSize, curPos);
                break;
            case 'SGGF':
                FGGS.Read(buffer, subSize, curPos);
                break;
            case 'AGGF':
                FGGA.Read(buffer, subSize, curPos);
                break;
            case 'STGF':
                FGTS.Read(buffer, subSize, curPos);
                break;
            case 'MANF':
                FNAM.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  NPC_: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;

            }
        }
    return 0;
    }

SINT32 NPC_Record::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    MODL.Unload();
    ACBS.Unload();

    for(UINT32 x = 0; x < SNAM.size(); x++)
        delete SNAM[x];
    SNAM.clear();

    INAM.Unload();
    RNAM.Unload();

    SPLO.clear();

    SCRI.Unload();

    for(UINT32 x = 0; x < CNTO.size(); x++)
        delete CNTO[x];
    CNTO.clear();

    AIDT.Unload();

    PKID.clear();

    KFFZ.clear();

    CNAM.Unload();
    DATA.Unload();
    HNAM.Unload();
    LNAM.Unload();
    ENAM.Unload();
    HCLR.Unload();
    ZNAM.Unload();
    FGGS.Unload();
    FGGA.Unload();
    FGTS.Unload();
    FNAM.Unload();
    return 1;
    }

SINT32 NPC_Record::WriteRecord(_FileHandler &SaveHandler)
    {
    UINT32 cSize = 0;
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(MODL.IsLoaded() && MODL->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MODL->MODL.value, MODL->MODL.GetSize());
        if(MODL->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MODL->MODB.value, MODL->MODB.GetSize());
        if(MODL->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MODL->MODT.value, MODL->MODT.GetSize());
        }

    if(ACBS.IsLoaded())
        SaveHandler.writeSubRecord('SBCA', &ACBS.value, ACBS.GetSize());
    for(UINT32 p = 0; p < SNAM.size(); p++)
        if(SNAM[p]->IsLoaded())
            SaveHandler.writeSubRecord('MANS', &SNAM[p]->value, SNAM[p]->GetSize());
    if(INAM.IsLoaded())
        SaveHandler.writeSubRecord('MANI', INAM.value, INAM.GetSize());
    if(RNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANR', RNAM.value, RNAM.GetSize());
    for(UINT32 p = 0; p < SPLO.size(); p++)
        SaveHandler.writeSubRecord('OLPS', &SPLO[p], sizeof(UINT32));
    if(SCRI.IsLoaded())
        SaveHandler.writeSubRecord('IRCS', SCRI.value, SCRI.GetSize());
    for(UINT32 p = 0; p < CNTO.size(); p++)
        if(CNTO[p]->IsLoaded())
            SaveHandler.writeSubRecord('OTNC', &CNTO[p]->value, sizeof(GENCNTO));
    if(AIDT.IsLoaded())
        SaveHandler.writeSubRecord('TDIA', &AIDT.value, AIDT.GetSize());
    for(UINT32 p = 0; p < PKID.size(); p++)
        SaveHandler.writeSubRecord('DIKP', &PKID[p], sizeof(UINT32));
    if(KFFZ.size())
        {
        cSize = 1; //final null terminator
        for(UINT32 p = 0; p < KFFZ.size(); p++)
            if(KFFZ[p].IsLoaded())
                cSize += KFFZ[p].GetSize();
        SaveHandler.writeSubRecord('ZFFK', NULL, cSize);
        for(UINT32 p = 0; p < KFFZ.size(); p++)
            if(KFFZ[p].IsLoaded())
                SaveHandler.write(KFFZ[p].value, KFFZ[p].GetSize());
        cSize = 0;
        //write final null terminator
        SaveHandler.write(&cSize, 1);
        }
    if(CNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANC', &CNAM.value, CNAM.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(HNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANH', HNAM.value, HNAM.GetSize());
    if(LNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANL', LNAM.value, LNAM.GetSize());
    if(ENAM.IsLoaded())
        SaveHandler.writeSubRecord('MANE', ENAM.value, ENAM.GetSize());
    if(HCLR.IsLoaded())
        SaveHandler.writeSubRecord('RLCH', &HCLR.value, HCLR.GetSize());
    if(ZNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANZ', ZNAM.value, ZNAM.GetSize());
    if(FGGS.IsLoaded())
        SaveHandler.writeSubRecord('SGGF', FGGS.value, FGGS.GetSize());
    if(FGGA.IsLoaded())
        SaveHandler.writeSubRecord('AGGF', FGGA.value, FGGA.GetSize());
    if(FGTS.IsLoaded())
        SaveHandler.writeSubRecord('STGF', FGTS.value, FGTS.GetSize());
    if(FNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANF', &FNAM.value, FNAM.GetSize());
    return -1;
    }

bool NPC_Record::operator ==(const NPC_Record &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        FULL.equals(other.FULL) &&
        MODL == other.MODL &&
        ACBS == other.ACBS &&
        INAM == other.INAM &&
        RNAM == other.RNAM &&
        SCRI == other.SCRI &&
        AIDT == other.AIDT &&
        CNAM == other.CNAM &&
        DATA == other.DATA &&
        HNAM == other.HNAM &&
        LNAM == other.LNAM &&
        ENAM == other.ENAM &&
        HCLR == other.HCLR &&
        ZNAM == other.ZNAM &&
        FGGS == other.FGGS &&
        FGGA == other.FGGA &&
        FGTS == other.FGTS &&
        FNAM == other.FNAM &&
        SNAM.size() == other.SNAM.size() &&
        SPLO.size() == other.SPLO.size() &&
        CNTO.size() == other.CNTO.size() &&
        PKID.size() == other.PKID.size() &&
        KFFZ.size() == other.KFFZ.size())
        {
        //Not sure if record order matters on factions, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < SNAM.size(); ++x)
            if(*SNAM[x] != *other.SNAM[x])
                return false;

        //Record order doesn't matter on spells, so equality testing isn't easy
        //The proper solution would be to check each spell against every other spell to see if there's a one-to-one match
        //Perhaps using a disjoint set
        //Fix-up later
        for(UINT32 x = 0; x < SPLO.size(); ++x)
            if(SPLO[x] != other.SPLO[x])
                return false;

        //Record order doesn't matter on items, so equality testing isn't easy
        //The proper solution would be to check each item against every other item to see if there's a one-to-one match
        //Fix-up later
        for(UINT32 x = 0; x < CNTO.size(); ++x)
            if(*CNTO[x] != *other.CNTO[x])
                return false;

        //Record order matters on ai packages, so equality testing is easy
        for(UINT32 x = 0; x < PKID.size(); ++x)
            if(PKID[x] != other.PKID[x])
                return false;

        //Not sure if record order matters on animations, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < KFFZ.size(); ++x)
            if(KFFZ[x].equalsi(other.KFFZ[x]))
                return false;

        return true;
        }

    return false;
    }

bool NPC_Record::operator !=(const NPC_Record &other) const
    {
    return !(*this == other);
    }