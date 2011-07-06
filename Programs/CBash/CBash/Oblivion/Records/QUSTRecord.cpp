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
#include "QUSTRecord.h"
#include <vector>

QUSTRecord::QUSTDATA::QUSTDATA():
    flags(0),
    priority(0)
    {
    //
    }

QUSTRecord::QUSTDATA::~QUSTDATA()
    {
    //
    }

bool QUSTRecord::QUSTDATA::operator ==(const QUSTDATA &other) const
    {
    return (flags == other.flags &&
            priority == other.priority);
    }

bool QUSTRecord::QUSTDATA::operator !=(const QUSTDATA &other) const
    {
    return !(*this == other);
    }

QUSTRecord::QUSTEntry::QUSTEntry()
    {
    //
    }

QUSTRecord::QUSTEntry::~QUSTEntry()
    {
    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    for(UINT32 x = 0; x < SCR_.size(); x++)
        delete SCR_[x];
    }

bool QUSTRecord::QUSTEntry::IsCompletes()
    {
    return (QSDT.value.flags & fIsCompletes) != 0;
    }

void QUSTRecord::QUSTEntry::IsCompletes(bool value)
    {
    QSDT.value.flags = value ? (QSDT.value.flags | fIsCompletes) : (QSDT.value.flags & ~fIsCompletes);
    }

bool QUSTRecord::QUSTEntry::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((QSDT.value.flags & Mask) == Mask) : ((QSDT.value.flags & Mask) != 0);
    }

void QUSTRecord::QUSTEntry::SetFlagMask(UINT8 Mask)
    {
    QSDT.value.flags = Mask;
    }

bool QUSTRecord::QUSTEntry::operator ==(const QUSTEntry &other) const
    {
    if(QSDT == other.QSDT &&
        CNAM.equals(other.CNAM) &&
        SCHR == other.SCHR &&
        SCDA == other.SCDA &&
        SCTX.equalsi(other.SCTX) &&
        CTDA.size() == other.CTDA.size() &&
        SCR_.size() == other.SCR_.size())
        {
        //Record order matters on conditions, so equality testing is easy
        for(UINT32 x = 0; x < CTDA.size(); ++x)
            if(*CTDA[x] != *other.CTDA[x])
                return false;

        //Record order matters on references, so equality testing is easy
        for(UINT32 x = 0; x < SCR_.size(); ++x)
            if(*SCR_[x] != *other.SCR_[x])
                return false;

        return true;
        }

    return false;
    }

bool QUSTRecord::QUSTEntry::operator !=(const QUSTEntry &other) const
    {
    return !(*this == other);
    }

QUSTRecord::QUSTINDX::QUSTINDX():
    stage(0)
    {
    //
    }

QUSTRecord::QUSTINDX::~QUSTINDX()
    {
    //
    }

bool QUSTRecord::QUSTINDX::operator ==(const QUSTINDX &other) const
    {
    return (stage == other.stage);
    }

bool QUSTRecord::QUSTINDX::operator !=(const QUSTINDX &other) const
    {
    return !(*this == other);
    }

QUSTRecord::QUSTStage::QUSTStage()
    {
    //
    }

QUSTRecord::QUSTStage::~QUSTStage()
    {
    for(UINT32 x = 0; x < Entries.size(); x++)
        delete Entries[x];
    }

bool QUSTRecord::QUSTStage::operator ==(const QUSTStage &other) const
    {
    if(INDX == other.INDX &&
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

bool QUSTRecord::QUSTStage::operator !=(const QUSTStage &other) const
    {
    return !(*this == other);
    }

QUSTRecord::QUSTQSTA::QUSTQSTA():
    targetId(0),
    flags(0)
    {
    memset(&unused1, 0x00, 3);
    }

QUSTRecord::QUSTQSTA::~QUSTQSTA()
    {
    //
    }

bool QUSTRecord::QUSTQSTA::operator ==(const QUSTQSTA &other) const
    {
    return (targetId == other.targetId &&
            flags == other.flags);
    }

bool QUSTRecord::QUSTQSTA::operator !=(const QUSTQSTA &other) const
    {
    return !(*this == other);
    }

QUSTRecord::QUSTTarget::QUSTTarget()
    {
    //
    }

QUSTRecord::QUSTTarget::~QUSTTarget()
    {
    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    }

bool QUSTRecord::QUSTTarget::IsIgnoresLocks()
    {
    return (QSTA.value.flags & fIsIgnoresLocks) != 0;
    }

void QUSTRecord::QUSTTarget::IsIgnoresLocks(bool value)
    {
    QSTA.value.flags = value ? (QSTA.value.flags | fIsIgnoresLocks) : (QSTA.value.flags & ~fIsIgnoresLocks);
    }

bool QUSTRecord::QUSTTarget::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((QSTA.value.flags & Mask) == Mask) : ((QSTA.value.flags & Mask) != 0);
    }

void QUSTRecord::QUSTTarget::SetFlagMask(UINT8 Mask)
    {
    QSTA.value.flags = Mask;
    }

bool QUSTRecord::QUSTTarget::operator ==(const QUSTTarget &other) const
    {
    if(QSTA == other.QSTA &&
        CTDA.size() == other.CTDA.size())
        {
        //Record order matters on conditions, so equality testing is easy
        for(UINT32 x = 0; x < CTDA.size(); ++x)
            if(*CTDA[x] != *other.CTDA[x])
                return false;

        return true;
        }

    return false;
    }

bool QUSTRecord::QUSTTarget::operator !=(const QUSTTarget &other) const
    {
    return !(*this == other);
    }

QUSTRecord::QUSTRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

QUSTRecord::QUSTRecord(QUSTRecord *srcRecord):
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
    SCRI = srcRecord->SCRI;
    FULL = srcRecord->FULL;
    ICON = srcRecord->ICON;
    DATA = srcRecord->DATA;

    CTDA.resize(srcRecord->CTDA.size());
    for(UINT32 x = 0; x < srcRecord->CTDA.size(); x++)
        {
        CTDA[x] = new ReqSubRecord<GENCTDA>;
        *CTDA[x] = *srcRecord->CTDA[x];
        }
    Stages.resize(srcRecord->Stages.size());
    for(UINT32 x = 0; x < srcRecord->Stages.size(); x++)
        {
        Stages[x] = new QUSTStage;
        Stages[x]->INDX = srcRecord->Stages[x]->INDX;
        Stages[x]->Entries.resize(srcRecord->Stages[x]->Entries.size());
        for(UINT32 y = 0; y < srcRecord->Stages[x]->Entries.size(); y++)
            {
            Stages[x]->Entries[y] = new QUSTEntry;
            Stages[x]->Entries[y]->QSDT = srcRecord->Stages[x]->Entries[y]->QSDT;
            Stages[x]->Entries[y]->CTDA.resize(srcRecord->Stages[x]->Entries[y]->CTDA.size());
            for(UINT32 p = 0; p < srcRecord->Stages[x]->Entries[y]->CTDA.size(); p++)
                {
                Stages[x]->Entries[y]->CTDA[p] = new ReqSubRecord<GENCTDA>;
                *Stages[x]->Entries[y]->CTDA[p] = *srcRecord->Stages[x]->Entries[y]->CTDA[p];
                }
            Stages[x]->Entries[y]->CNAM = srcRecord->Stages[x]->Entries[y]->CNAM;
            Stages[x]->Entries[y]->SCHR = srcRecord->Stages[x]->Entries[y]->SCHR;
            Stages[x]->Entries[y]->SCDA = srcRecord->Stages[x]->Entries[y]->SCDA;
            Stages[x]->Entries[y]->SCTX = srcRecord->Stages[x]->Entries[y]->SCTX;
            Stages[x]->Entries[y]->SCR_.resize(srcRecord->Stages[x]->Entries[y]->SCR_.size());
            for(UINT32 p = 0; p < srcRecord->Stages[x]->Entries[y]->SCR_.size(); p++)
                {
                Stages[x]->Entries[y]->SCR_[p] = new ReqSubRecord<GENSCR_>;
                *Stages[x]->Entries[y]->SCR_[p] = *srcRecord->Stages[x]->Entries[y]->SCR_[p];
                }
            }
        }
    Targets.resize(srcRecord->Targets.size());
    for(UINT32 x = 0; x < srcRecord->Targets.size(); x++)
        {
        Targets[x] = new QUSTTarget;
        Targets[x]->QSTA = srcRecord->Targets[x]->QSTA;
        Targets[x]->CTDA.resize(srcRecord->Targets[x]->CTDA.size());
        for(UINT32 y = 0; y < srcRecord->Targets[x]->CTDA.size(); y++)
            {
            Targets[x]->CTDA[y] = new ReqSubRecord<GENCTDA>;
            *Targets[x]->CTDA[y] = *srcRecord->Targets[x]->CTDA[y];
            }
        }
    return;
    }

QUSTRecord::~QUSTRecord()
    {
    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    for(UINT32 x = 0; x < Stages.size(); x++)
        delete Stages[x];
    for(UINT32 x = 0; x < Targets.size(); x++)
        delete Targets[x];
    }

bool QUSTRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    FunctionArguments CTDAFunction;
    Function_Arguments_Iterator curCTDAFunction;
    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);
    for(UINT32 x = 0; x < CTDA.size(); x++)
        {
        //if(CTDA[x]->value.ifunc == 214)
        //    printf("%08X uses HasMagicEffect\n", formID);
        curCTDAFunction = Function_Arguments.find(CTDA[x]->value.ifunc);
        if(curCTDAFunction != Function_Arguments.end())
            {
            CTDAFunction = curCTDAFunction->second;
            if(CTDAFunction.first == eFORMID)
                op.Accept(CTDA[x]->value.param1);
            if(CTDAFunction.second == eFORMID)
                op.Accept(CTDA[x]->value.param2);
            }
        else
            printf("Warning: QUSTRecord %08X uses an unknown function (%d)!\n", formID, CTDA[x]->value.ifunc);
        }
    for(UINT32 x = 0; x < Stages.size(); x++)
        {
        for(UINT32 y = 0; y < Stages[x]->Entries.size(); y++)
            {
            for(UINT32 p = 0; p < Stages[x]->Entries[y]->CTDA.size(); p++)
                {
                //if(Stages[x]->Entries[y]->CTDA[p]->value.ifunc == 214)
                //    printf("%08X uses HasMagicEffect\n", formID);
                curCTDAFunction = Function_Arguments.find(Stages[x]->Entries[y]->CTDA[p]->value.ifunc);
                if(curCTDAFunction != Function_Arguments.end())
                    {
                    CTDAFunction = curCTDAFunction->second;
                    if(CTDAFunction.first == eFORMID)
                        op.Accept(Stages[x]->Entries[y]->CTDA[p]->value.param1);
                    if(CTDAFunction.second == eFORMID)
                        op.Accept(Stages[x]->Entries[y]->CTDA[p]->value.param2);
                    }
                else
                    printf("Warning: QUSTRecord Stage %08X uses an unknown function (%d)!\n", formID, Stages[x]->Entries[y]->CTDA[p]->value.ifunc);
                }
            for(UINT32 p = 0; p < Stages[x]->Entries[y]->SCR_.size(); p++)
                if(Stages[x]->Entries[y]->SCR_[p]->value.isSCRO)
                    op.Accept(Stages[x]->Entries[y]->SCR_[p]->value.reference);
            }
        }

    for(UINT32 x = 0; x < Targets.size(); x++)
        {
        op.Accept(Targets[x]->QSTA.value.targetId);
        for(UINT32 y = 0; y < Targets[x]->CTDA.size(); y++)
            {
            //if(Targets[x]->CTDA[y]->value.ifunc == 214)
            //    printf("%08X uses HasMagicEffect\n", formID);
            curCTDAFunction = Function_Arguments.find(Targets[x]->CTDA[y]->value.ifunc);
            if(curCTDAFunction != Function_Arguments.end())
                {
                CTDAFunction = curCTDAFunction->second;
                if(CTDAFunction.first == eFORMID)
                    op.Accept(Targets[x]->CTDA[y]->value.param1);
                if(CTDAFunction.second == eFORMID)
                    op.Accept(Targets[x]->CTDA[y]->value.param2);
                }
            else
                printf("Warning: QUSTRecord Target %08X uses an unknown function (%d)!\n", formID, Targets[x]->CTDA[y]->value.ifunc);
            }
        }

    return op.Stop();
    }

bool QUSTRecord::IsStartEnabled()
    {
    return (DATA.value.flags & fIsStartEnabled) != 0;
    }

void QUSTRecord::IsStartEnabled(bool value)
    {
    if(value)
        DATA.value.flags |= fIsStartEnabled;
    else
        DATA.value.flags &= ~fIsStartEnabled;
    }

bool QUSTRecord::IsRepeatedTopics()
    {
    return (DATA.value.flags & fIsRepeatedTopics) != 0;
    }

void QUSTRecord::IsRepeatedTopics(bool value)
    {
    if(value)
        DATA.value.flags |= fIsRepeatedTopics;
    else
        DATA.value.flags &= ~fIsRepeatedTopics;
    }

bool QUSTRecord::IsRepeatedStages()
    {
    return (DATA.value.flags & fIsRepeatedStages) != 0;
    }

void QUSTRecord::IsRepeatedStages(bool value)
    {
    if(value)
        DATA.value.flags |= fIsRepeatedStages;
    else
        DATA.value.flags &= ~fIsRepeatedStages;
    }

bool QUSTRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void QUSTRecord::SetFlagMask(UINT8 Mask)
    {
    DATA.value.flags = Mask;
    }

UINT32 QUSTRecord::GetSize(bool forceCalc)
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

    if(SCRI.IsLoaded())
        TotSize += SCRI.GetSize() + 6;

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

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    for(UINT32 p = 0; p < CTDA.size(); p++)
        if(CTDA[p]->IsLoaded())
            TotSize += CTDA[p]->GetSize() + 6;

    for(UINT32 p = 0; p < Stages.size(); p++)
        {
        if(Stages[p]->INDX.IsLoaded())
            TotSize += Stages[p]->INDX.GetSize() + 6;

        for(UINT32 x = 0; x < Stages[p]->Entries.size(); x++)
            {

            if(Stages[p]->Entries[x]->QSDT.IsLoaded())
                TotSize += Stages[p]->Entries[x]->QSDT.GetSize() + 6;

            if(Stages[p]->Entries[x]->CTDA.size())
                for(UINT32 y = 0; y < Stages[p]->Entries[x]->CTDA.size(); y++)
                    if(Stages[p]->Entries[x]->CTDA[y]->IsLoaded())
                        TotSize += Stages[p]->Entries[x]->CTDA[y]->GetSize() + 6;

            if(Stages[p]->Entries[x]->CNAM.IsLoaded())
                {
                cSize = Stages[p]->Entries[x]->CNAM.GetSize();
                if(cSize > 65535) cSize += 10;
                TotSize += cSize += 6;
                }

            if(Stages[p]->Entries[x]->SCHR.IsLoaded())
                TotSize += Stages[p]->Entries[x]->SCHR.GetSize() + 6;

            if(Stages[p]->Entries[x]->SCDA.IsLoaded())
                {
                cSize = Stages[p]->Entries[x]->SCDA.GetSize();
                if(cSize > 65535) cSize += 10;
                TotSize += cSize += 6;
                }

            if(Stages[p]->Entries[x]->SCTX.IsLoaded())
                {
                cSize = Stages[p]->Entries[x]->SCTX.GetSize();
                if(cSize > 65535) cSize += 10;
                TotSize += cSize += 6;
                }

            TotSize += (sizeof(UINT32) + 6) * (UINT32)Stages[p]->Entries[x]->SCR_.size();
            }
        }

    for(UINT32 p = 0; p < Targets.size(); p++)
        {
        if(Targets[p]->QSTA.IsLoaded())
            TotSize += Targets[p]->QSTA.GetSize() + 6;

        for(UINT32 y = 0; y < Targets[p]->CTDA.size(); y++)
            if(Targets[p]->CTDA[y]->IsLoaded())
                TotSize += Targets[p]->CTDA[y]->GetSize() + 6;
        }

    return TotSize;
    }

UINT32 QUSTRecord::GetType()
    {
    return 'TSUQ';
    }

STRING QUSTRecord::GetStrType()
    {
    return "QUST";
    }

SINT32 QUSTRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    int whichCTDA = 0;
    ReqSubRecord<GENCTDA> *newCTDA = NULL;
    QUSTStage *newStage = NULL;
    QUSTEntry *newEntry = NULL;
    QUSTTarget *newTarget = NULL;
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
            case 'IRCS':
                SCRI.Read(buffer, subSize, curPos);
                break;
            case 'LLUF':
                FULL.Read(buffer, subSize, curPos);
                break;
            case 'NOCI':
                ICON.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'TDTC':
            case 'ADTC':
                newCTDA = new ReqSubRecord<GENCTDA>;
                newCTDA->Read(buffer, subSize, curPos);
                switch(whichCTDA)
                    {
                    case 1:
                        if(newStage == NULL)
                            {
                            newStage = new QUSTStage;
                            Stages.push_back(newStage);
                            }
                        if(newEntry == NULL)
                            {
                            newEntry = new QUSTEntry;
                            newStage->Entries.push_back(newEntry);
                            }
                        newEntry->CTDA.push_back(newCTDA);
                        break;
                    case 2:
                        if(newTarget == NULL)
                            {
                            newTarget = new QUSTTarget;
                            Targets.push_back(newTarget);
                            }
                        newTarget->CTDA.push_back(newCTDA);
                        break;
                    default:
                        CTDA.push_back(newCTDA);
                        break;
                    }
                break;
            case 'XDNI':
                newStage = new QUSTStage;
                newStage->INDX.Read(buffer, subSize, curPos);
                Stages.push_back(newStage);
                whichCTDA = 1;
                break;
            case 'TDSQ':
                if(newStage == NULL)
                    {
                    newStage = new QUSTStage;
                    Stages.push_back(newStage);
                    }
                newEntry = new QUSTEntry;
                newEntry->QSDT.Read(buffer, subSize, curPos);
                newStage->Entries.push_back(newEntry);
                break;
            case 'MANC':
                if(newStage == NULL)
                    {
                    newStage = new QUSTStage;
                    Stages.push_back(newStage);
                    }
                if(newEntry == NULL)
                    {
                    newEntry = new QUSTEntry;
                    newStage->Entries.push_back(newEntry);
                    }
                newEntry->CNAM.Read(buffer, subSize, curPos);
                break;
            case 'RHCS':
                if(newStage == NULL)
                    {
                    newStage = new QUSTStage;
                    Stages.push_back(newStage);
                    }
                if(newEntry == NULL)
                    {
                    newEntry = new QUSTEntry;
                    newStage->Entries.push_back(newEntry);
                    }
                newEntry->SCHR.Read(buffer, subSize, curPos);
                break;
            case 'ADCS':
                if(newStage == NULL)
                    {
                    newStage = new QUSTStage;
                    Stages.push_back(newStage);
                    }
                if(newEntry == NULL)
                    {
                    newEntry = new QUSTEntry;
                    newStage->Entries.push_back(newEntry);
                    }
                newEntry->SCDA.Read(buffer, subSize, curPos);
                break;
            case 'XTCS':
                if(newStage == NULL)
                    {
                    newStage = new QUSTStage;
                    Stages.push_back(newStage);
                    }
                if(newEntry == NULL)
                    {
                    newEntry = new QUSTEntry;
                    newStage->Entries.push_back(newEntry);
                    }
                newEntry->SCTX.Read(buffer, subSize, curPos);
                break;
            case 'VRCS':
                if(newStage == NULL)
                    {
                    newStage = new QUSTStage;
                    Stages.push_back(newStage);
                    }
                if(newEntry == NULL)
                    {
                    newEntry = new QUSTEntry;
                    newStage->Entries.push_back(newEntry);
                    }
                newSCR_ = new ReqSubRecord<GENSCR_>;
                newSCR_->Read(buffer, subSize, curPos);
                newSCR_->value.isSCRO = false;
                newEntry->SCR_.push_back(newSCR_);
                break;
            case 'ORCS':
                if(newStage == NULL)
                    {
                    newStage = new QUSTStage;
                    Stages.push_back(newStage);
                    }
                if(newEntry == NULL)
                    {
                    newEntry = new QUSTEntry;
                    newStage->Entries.push_back(newEntry);
                    }
                newSCR_ = new ReqSubRecord<GENSCR_>;
                newSCR_->Read(buffer, subSize, curPos);
                newSCR_->value.isSCRO = true;
                newEntry->SCR_.push_back(newSCR_);
                break;
            case 'ATSQ':
                newTarget = new QUSTTarget;
                newTarget->QSTA.Read(buffer, subSize, curPos);
                Targets.push_back(newTarget);
                whichCTDA = 2;
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  QUST: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 QUSTRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    SCRI.Unload();
    FULL.Unload();
    ICON.Unload();
    DATA.Unload();

    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    CTDA.clear();

    for(UINT32 x = 0; x < Stages.size(); x++)
        delete Stages[x];
    Stages.clear();

    for(UINT32 x = 0; x < Targets.size(); x++)
        delete Targets[x];
    Targets.clear();
    return 1;
    }

SINT32 QUSTRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    FunctionArguments CTDAFunction;
    Function_Arguments_Iterator curCTDAFunction;

    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(SCRI.IsLoaded())
        SaveHandler.writeSubRecord('IRCS', SCRI.value, SCRI.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(ICON.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', ICON.value, ICON.GetSize());
    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    for(UINT32 p = 0; p < CTDA.size(); p++)
        {
        curCTDAFunction = Function_Arguments.find(CTDA[p]->value.ifunc);
        if(curCTDAFunction != Function_Arguments.end())
            {
            CTDAFunction = curCTDAFunction->second;
            if(CTDAFunction.first == eNONE)
                CTDA[p]->value.param1 = 0;
            if(CTDAFunction.second == eNONE)
                CTDA[p]->value.param2 = 0;
            }
        SaveHandler.writeSubRecord('ADTC', &CTDA[p]->value, CTDA[p]->GetSize());
        }
    if(Stages.size())
        for(UINT32 p = 0; p < Stages.size(); p++)
            {
            if(Stages[p]->INDX.IsLoaded())
                SaveHandler.writeSubRecord('XDNI', &Stages[p]->INDX.value, Stages[p]->INDX.GetSize());
            if(Stages[p]->Entries.size())
                for(UINT32 x = 0; x < Stages[p]->Entries.size(); x++)
                    {
                    if(Stages[p]->Entries[x]->QSDT.IsLoaded())
                        SaveHandler.writeSubRecord('TDSQ', &Stages[p]->Entries[x]->QSDT.value, Stages[p]->Entries[x]->QSDT.GetSize());
                    for(UINT32 y = 0; y < Stages[p]->Entries[x]->CTDA.size(); y++)
                        {
                        curCTDAFunction = Function_Arguments.find(Stages[p]->Entries[x]->CTDA[y]->value.ifunc);
                        if(curCTDAFunction != Function_Arguments.end())
                            {
                            CTDAFunction = curCTDAFunction->second;
                            if(CTDAFunction.first == eNONE)
                                Stages[p]->Entries[x]->CTDA[y]->value.param1 = 0;
                            if(CTDAFunction.second == eNONE)
                                Stages[p]->Entries[x]->CTDA[y]->value.param2 = 0;
                            }
                        SaveHandler.writeSubRecord('ADTC', &Stages[p]->Entries[x]->CTDA[y]->value, Stages[p]->Entries[x]->CTDA[y]->GetSize());
                        }
                    if(Stages[p]->Entries[x]->CNAM.IsLoaded())
                        SaveHandler.writeSubRecord('MANC', Stages[p]->Entries[x]->CNAM.value, Stages[p]->Entries[x]->CNAM.GetSize());
                    if(Stages[p]->Entries[x]->SCHR.IsLoaded())
                        SaveHandler.writeSubRecord('RHCS', &Stages[p]->Entries[x]->SCHR.value, Stages[p]->Entries[x]->SCHR.GetSize());
                    if(Stages[p]->Entries[x]->SCDA.IsLoaded())
                        SaveHandler.writeSubRecord('ADCS', Stages[p]->Entries[x]->SCDA.value, Stages[p]->Entries[x]->SCDA.GetSize());
                    if(Stages[p]->Entries[x]->SCTX.IsLoaded())
                        SaveHandler.writeSubRecord('XTCS', Stages[p]->Entries[x]->SCTX.value, Stages[p]->Entries[x]->SCTX.GetSize());
                    if(Stages[p]->Entries[x]->SCR_.size())
                        for(UINT32 y = 0; y < Stages[p]->Entries[x]->SCR_.size(); y++)
                            if(Stages[p]->Entries[x]->SCR_[y]->IsLoaded())
                                if(Stages[p]->Entries[x]->SCR_[y]->value.isSCRO)
                                    SaveHandler.writeSubRecord('ORCS', &Stages[p]->Entries[x]->SCR_[y]->value.reference, sizeof(UINT32));
                                else
                                    SaveHandler.writeSubRecord('VRCS', &Stages[p]->Entries[x]->SCR_[y]->value.reference, sizeof(UINT32));
                    }
            }
    if(Targets.size())
        for(UINT32 p = 0; p < Targets.size(); p++)
            {
            if(Targets[p]->QSTA.IsLoaded())
                SaveHandler.writeSubRecord('ATSQ', &Targets[p]->QSTA.value, Targets[p]->QSTA.GetSize());
            for(UINT32 y = 0; y < Targets[p]->CTDA.size(); y++)
                {
                curCTDAFunction = Function_Arguments.find(Targets[p]->CTDA[y]->value.ifunc);
                if(curCTDAFunction != Function_Arguments.end())
                    {
                    CTDAFunction = curCTDAFunction->second;
                    if(CTDAFunction.first == eNONE)
                        Targets[p]->CTDA[y]->value.param1 = 0;
                    if(CTDAFunction.second == eNONE)
                        Targets[p]->CTDA[y]->value.param2 = 0;
                    }
                SaveHandler.writeSubRecord('ADTC', &Targets[p]->CTDA[y]->value, Targets[p]->CTDA[y]->GetSize());
                }
            }
    return -1;
    }

bool QUSTRecord::operator ==(const QUSTRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        SCRI == other.SCRI &&
        FULL.equals(other.FULL) &&
        ICON.equalsi(other.ICON) &&
        DATA == other.DATA &&
        CTDA.size() == other.CTDA.size() &&
        Stages.size() == other.Stages.size() &&
        Targets.size() == other.Targets.size())
        {
        //Record order matters on conditions, so equality testing is easy
        for(UINT32 x = 0; x < CTDA.size(); ++x)
            if(*CTDA[x] != *other.CTDA[x])
                return false;

        //Record order doesn't matter on stages, so equality testing isn't easy
        //Instead, they're keyed by stage index (QUSTStage.value.QUSTINDX.value.stage)
        //The proper solution would be to see if each stage index matches the other
        //Fix-up later
        for(UINT32 x = 0; x < Stages.size(); ++x)
            if(*Stages[x] != *other.Stages[x])
                return false;

        //Not sure if record order matters on targets, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < Targets.size(); ++x)
            if(*Targets[x] != *other.Targets[x])
                return false;

        return true;
        }

    return false;
    }

bool QUSTRecord::operator !=(const QUSTRecord &other) const
    {
    return !(*this == other);
    }