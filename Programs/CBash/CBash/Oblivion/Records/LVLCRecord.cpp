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
#include "LVLCRecord.h"
#include <vector>

LVLCRecord::LVLCRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

LVLCRecord::LVLCRecord(LVLCRecord *srcRecord):
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
    LVLD = srcRecord->LVLD;
    LVLF = srcRecord->LVLF;
    SCRI = srcRecord->SCRI;
    TNAM = srcRecord->TNAM;
    Entries.clear();
    Entries.resize(srcRecord->Entries.size());
    for(UINT32 x = 0; x < srcRecord->Entries.size(); x++)
        {
        Entries[x] = new ReqSubRecord<LVLLVLO>;
        *Entries[x] = *srcRecord->Entries[x];
        }
    return;
    }

LVLCRecord::~LVLCRecord()
    {
    for(UINT32 x = 0; x < Entries.size(); x++)
        delete Entries[x];
    }

bool LVLCRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);
    if(TNAM.IsLoaded())
        op.Accept(TNAM->fid);
    for(UINT32 x = 0; x < Entries.size(); x++)
        op.Accept(Entries[x]->value.listId);

    return op.Stop();
    }

bool LVLCRecord::IsCalcFromAllLevels()
    {
    return LVLF.IsLoaded() ? (LVLF->flags & fCalcFromAllLevels) != 0 : false;
    }

void LVLCRecord::IsCalcFromAllLevels(bool value)
    {
    if(!LVLF.IsLoaded()) return;
    LVLF->flags = value ? (LVLF->flags | fCalcFromAllLevels) : (LVLF->flags & ~fCalcFromAllLevels);
    }

bool LVLCRecord::IsCalcForEachItem()
    {
    return LVLF.IsLoaded() ? (LVLF->flags & fCalcForEachItem) != 0 : false;
    }

void LVLCRecord::IsCalcForEachItem(bool value)
    {
    if(!LVLF.IsLoaded()) return;
    LVLF->flags = value ? (LVLF->flags | fCalcForEachItem) : (LVLF->flags & ~fCalcForEachItem);
    }

bool LVLCRecord::IsUseAllSpells()
    {
    return LVLF.IsLoaded() ? (LVLF->flags & fUseAllSpells) != 0 : false;
    }

void LVLCRecord::IsUseAllSpells(bool value)
    {
    if(!LVLF.IsLoaded()) return;
    LVLF->flags = value ? (LVLF->flags | fUseAllSpells) : (LVLF->flags & ~fUseAllSpells);
    }

bool LVLCRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    if(!LVLF.IsLoaded()) return false;
    return Exact ? ((LVLF->flags & Mask) == Mask) : ((LVLF->flags & Mask) != 0);
    }

void LVLCRecord::SetFlagMask(UINT8 Mask)
    {
    //if(Mask)
    //    {
        LVLF.Load();
        LVLF->flags = Mask;
    //    }
    //else
    //    LVLF.Unload();
    }

UINT32 LVLCRecord::GetSize(bool forceCalc)
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

    if(LVLD.IsLoaded())
        TotSize += LVLD.GetSize() + 6;

    if(LVLF.IsLoaded())
        TotSize += LVLF.GetSize() + 6;

    if(SCRI.IsLoaded())
        TotSize += SCRI.GetSize() + 6;

    if(TNAM.IsLoaded())
        TotSize += TNAM.GetSize() + 6;

    for(UINT32 p = 0; p < Entries.size(); p++)
        if(Entries[p]->IsLoaded())
            TotSize += Entries[p]->GetSize() + 6;

    return TotSize;
    }

UINT32 LVLCRecord::GetType()
    {
    return 'CLVL';
    }

STRING LVLCRecord::GetStrType()
    {
    return "LVLC";
    }

SINT32 LVLCRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    ReqSubRecord<LVLLVLO> *newEntry = NULL;
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
            case 'DLVL':
                LVLD.Read(buffer, subSize, curPos);
                if((LVLD.value.chanceNone & fAltCalcFromAllLevels) != 0)
                    {
                    LVLD.value.chanceNone &= ~fAltCalcFromAllLevels;
                    LVLF.Load();
                    IsCalcFromAllLevels(true);
                    }
                break;
            case 'FLVL':
                LVLF.Read(buffer, subSize, curPos);
                break;
            case 'IRCS':
                SCRI.Read(buffer, subSize, curPos);
                break;
            case 'MANT':
                TNAM.Read(buffer, subSize, curPos);
                break;
            case 'OLVL':
                newEntry = new ReqSubRecord<LVLLVLO>;
                newEntry->Read(buffer, subSize, curPos);
                Entries.push_back(newEntry);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  LVLC: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 LVLCRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    LVLD.Unload();
    LVLF.Unload();
    SCRI.Unload();
    TNAM.Unload();
    for(UINT32 x = 0; x < Entries.size(); x++)
        delete Entries[x];
    Entries.clear();
    return 1;
    }

SINT32 LVLCRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(LVLD.IsLoaded())
        SaveHandler.writeSubRecord('DLVL', &LVLD.value, LVLD.GetSize());
    if(LVLF.IsLoaded())
        SaveHandler.writeSubRecord('FLVL', LVLF.value, LVLF.GetSize());
    if(SCRI.IsLoaded())
        SaveHandler.writeSubRecord('IRCS', SCRI.value, SCRI.GetSize());
    if(TNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANT', TNAM.value, TNAM.GetSize());
    for(UINT32 p = 0; p < Entries.size(); p++)
        if(Entries[p]->IsLoaded())
            SaveHandler.writeSubRecord('OLVL', &Entries[p]->value, Entries[p]->GetSize());
    return -1;
    }

bool LVLCRecord::operator ==(const LVLCRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        LVLD == other.LVLD &&
        LVLF == other.LVLF &&
        SCRI == other.SCRI &&
        TNAM == other.TNAM &&
        Entries.size() == other.Entries.size())
        {
        //Not sure if record order matters on entries, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < Entries.size(); ++x)
            if(*Entries[x] != *other.Entries[x])
                return false;

        return true;
        }

    return false;
    }

bool LVLCRecord::operator !=(const LVLCRecord &other) const
    {
    return !(*this == other);
    }