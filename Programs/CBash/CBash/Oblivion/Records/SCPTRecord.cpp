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
#include "SCPTRecord.h"
#include <vector>

SCPTRecord::SCPTSLSD::SCPTSLSD():
    index(0),
    flags(0)
    {
    memset(&unused1[0], 0, sizeof(unused1));
    memset(&unused2[0], 0, sizeof(unused2));
    }

SCPTRecord::SCPTSLSD::~SCPTSLSD()
    {
    //
    }

bool SCPTRecord::SCPTSLSD::operator ==(const SCPTSLSD &other) const
    {
    return (index == other.index &&
            flags == other.flags);
    }

bool SCPTRecord::SCPTSLSD::operator !=(const SCPTSLSD &other) const
    {
    return !(*this == other);
    }

bool SCPTRecord::SCPTVARS::IsLongOrShort()
    {
    return (SLSD.value.flags & fIsLongOrShort) != 0;
    }

void SCPTRecord::SCPTVARS::IsLongOrShort(bool value)
    {
    SLSD.value.flags = value ? (SLSD.value.flags | fIsLongOrShort) : (SLSD.value.flags & ~fIsLongOrShort);
    }

bool SCPTRecord::SCPTVARS::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((SLSD.value.flags & Mask) == Mask) : ((SLSD.value.flags & Mask) != 0);
    }

void SCPTRecord::SCPTVARS::SetFlagMask(UINT8 Mask)
    {
    SLSD.value.flags = Mask;
    }

bool SCPTRecord::SCPTVARS::operator ==(const SCPTVARS &other) const
    {
    return (SLSD == other.SLSD &&
            SCVR.equalsi(other.SCVR));
    }

bool SCPTRecord::SCPTVARS::operator !=(const SCPTVARS &other) const
    {
    return !(*this == other);
    }

SCPTRecord::SCPTRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

SCPTRecord::SCPTRecord(SCPTRecord *srcRecord):
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
    SCHR = srcRecord->SCHR;
    SCDA = srcRecord->SCDA;
    SCTX = srcRecord->SCTX;

    VARS.resize(srcRecord->VARS.size());
    for(UINT32 x = 0; x < srcRecord->VARS.size(); x++)
        {
        VARS[x] = new SCPTVARS;
        VARS[x]->SLSD = srcRecord->VARS[x]->SLSD;
        VARS[x]->SCVR = srcRecord->VARS[x]->SCVR;
        }

    SCR_.resize(srcRecord->SCR_.size());
    for(UINT32 x = 0; x < srcRecord->SCR_.size(); x++)
        {
        SCR_[x] = new ReqSubRecord<GENSCR_>;
        *SCR_[x] = *srcRecord->SCR_[x];
        }
    }

SCPTRecord::~SCPTRecord()
    {
    for(UINT32 x = 0; x < VARS.size(); x++)
        delete VARS[x];
    for(UINT32 x = 0; x < SCR_.size(); x++)
        delete SCR_[x];
    }

bool SCPTRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < SCR_.size(); x++)
        if(SCR_[x]->value.isSCRO)
            op.Accept(SCR_[x]->value.reference);

    return op.Stop();
    }

bool SCPTRecord::IsType(UINT32 Type)
    {
    return SCHR.value.scriptType == Type;
    }

void SCPTRecord::SetType(UINT32 Type)
    {
    SCHR.value.scriptType = Type;
    }

UINT32 SCPTRecord::GetSize(bool forceCalc)
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

    if(SCHR.IsLoaded())
        TotSize += SCHR.GetSize() + 6;

    if(SCDA.IsLoaded())
        {
        cSize = SCDA.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(SCTX.IsLoaded())
        {
        cSize = SCTX.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    for(UINT32 p = 0; p < VARS.size(); p++)
        {
        if(VARS[p]->SLSD.IsLoaded())
            TotSize += VARS[p]->SLSD.GetSize() + 6;

        if(VARS[p]->SCVR.IsLoaded())
            {
            cSize = VARS[p]->SCVR.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    TotSize += (sizeof(UINT32) + 6) * (UINT32)SCR_.size();

    return TotSize;
    }

UINT32 SCPTRecord::GetType()
    {
    return 'TPCS';
    }

STRING SCPTRecord::GetStrType()
    {
    return "SCPT";
    }

SINT32 SCPTRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    SCPTVARS *newVARS = NULL;
    ReqSubRecord<GENSCR_> *newSCR_ = NULL;
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
            case 'RHCS':
                SCHR.Read(buffer, subSize, curPos);
                break;
            case 'ADCS':
                SCDA.Read(buffer, subSize, curPos);
                break;
            case 'XTCS':
                SCTX.Read(buffer, subSize, curPos);
                break;
            case 'DSLS':
                newVARS = new SCPTVARS;
                newVARS->SLSD.Read(buffer, subSize, curPos);
                VARS.push_back(newVARS);
                break;
            case 'RVCS':
                if(newVARS == NULL)
                    {
                    newVARS = new SCPTVARS;
                    VARS.push_back(newVARS);
                    }
                newVARS->SCVR.Read(buffer, subSize, curPos);
                break;
            case 'VRCS':
                newSCR_ = new ReqSubRecord<GENSCR_>;
                newSCR_->Read(buffer, subSize, curPos);
                newSCR_->value.isSCRO = false;
                SCR_.push_back(newSCR_);
                break;
            case 'ORCS':
                newSCR_ = new ReqSubRecord<GENSCR_>;
                newSCR_->Read(buffer, subSize, curPos);
                newSCR_->value.isSCRO = true;
                SCR_.push_back(newSCR_);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  SCPT: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 SCPTRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    SCHR.Unload();
    SCDA.Unload();
    SCTX.Unload();
    for(UINT32 x = 0; x < VARS.size(); x++)
        delete VARS[x];
    VARS.clear();
    for(UINT32 x = 0; x < SCR_.size(); x++)
        delete SCR_[x];
    SCR_.clear();
    return 1;
    }

SINT32 SCPTRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(SCHR.IsLoaded())
        {
        SCHR.value.compiledSize = SCDA.GetSize(); //Just to ensure that the value is correct
        SaveHandler.writeSubRecord('RHCS', &SCHR.value, SCHR.GetSize());
        }
    if(SCDA.IsLoaded())
        SaveHandler.writeSubRecord('ADCS', SCDA.value, SCDA.GetSize());
    if(SCTX.IsLoaded())
        SaveHandler.writeSubRecord('XTCS', SCTX.value, SCTX.GetSize());
    for(UINT32 p = 0; p < VARS.size(); p++)
        {
        if(VARS[p]->SLSD.IsLoaded())
            SaveHandler.writeSubRecord('DSLS', &VARS[p]->SLSD.value, VARS[p]->SLSD.GetSize());
        if(VARS[p]->SCVR.IsLoaded())
            SaveHandler.writeSubRecord('RVCS', VARS[p]->SCVR.value, VARS[p]->SCVR.GetSize());
        }

    for(UINT32 p = 0; p < SCR_.size(); p++)
        if(SCR_[p]->IsLoaded())
            if(SCR_[p]->value.isSCRO)
                SaveHandler.writeSubRecord('ORCS', &SCR_[p]->value.reference, sizeof(UINT32));
            else
                SaveHandler.writeSubRecord('VRCS', &SCR_[p]->value.reference, sizeof(UINT32));
    return -1;
    }

bool SCPTRecord::operator ==(const SCPTRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        SCHR == other.SCHR &&
        SCDA == other.SCDA &&
        SCTX.equalsi(other.SCTX) &&
        VARS.size() == other.VARS.size() &&
        SCR_.size() == other.SCR_.size())
        {
        //Record order doesn't matter on vars, so equality testing isn't easy
        //Instead, they're keyed by var index (SLSD.value.index)
        //The proper solution would be to see if each indexed var matches the other
        //But they're usually ordered, so the lazy approach is to not bother
        //Fix-up later
        for(UINT32 x = 0; x < VARS.size(); ++x)
            if(*VARS[x] != *other.VARS[x])
                return false;

        //Record order matters on references, so equality testing is easy
        for(UINT32 x = 0; x < SCR_.size(); ++x)
            if(*SCR_[x] != *other.SCR_[x])
                return false;
        return true;
        }

    return false;
    }

bool SCPTRecord::operator !=(const SCPTRecord &other) const
    {
    return !(*this == other);
    }