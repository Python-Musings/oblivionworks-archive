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
#include "INFORecord.h"
#include <vector>

INFORecord::INFODATA::INFODATA():
    dialType(0),
    flags(0)
    {
    //
    }

INFORecord::INFODATA::~INFODATA()
    {
    //
    }

bool INFORecord::INFODATA::operator ==(const INFODATA &other) const
    {
    return (dialType == other.dialType &&
            flags == other.flags);
    }

bool INFORecord::INFODATA::operator !=(const INFODATA &other) const
    {
    return !(*this == other);
    }

INFORecord::INFOTRDT::INFOTRDT():
    emotionType(0),
    emotionValue(0),
    responseNum(0)
    {
    memset(&unused1, 0x00, 4);
    memset(&unused2, 0x00, 3);
    }

INFORecord::INFOTRDT::~INFOTRDT()
    {
    //
    }

bool INFORecord::INFOTRDT::operator ==(const INFOTRDT &other) const
    {
    return (emotionType == other.emotionType &&
            emotionValue == other.emotionValue &&
            responseNum == other.responseNum);
    }

bool INFORecord::INFOTRDT::operator !=(const INFOTRDT &other) const
    {
    return !(*this == other);
    }

bool INFORecord::INFOResponse::IsNeutral()
    {
    return (TRDT.value.emotionType == eNeutral);
    }

void INFORecord::INFOResponse::IsNeutral(bool value)
    {
    if(value)
        TRDT.value.emotionType = eNeutral;
    else if(IsNeutral())
        TRDT.value.emotionType = eAnger;
    }

bool INFORecord::INFOResponse::IsAnger()
    {
    return (TRDT.value.emotionType == eAnger);
    }

void INFORecord::INFOResponse::IsAnger(bool value)
    {
    if(value)
        TRDT.value.emotionType = eAnger;
    else if(IsAnger())
        TRDT.value.emotionType = eNeutral;
    }

bool INFORecord::INFOResponse::IsDisgust()
    {
    return (TRDT.value.emotionType == eDisgust);
    }

void INFORecord::INFOResponse::IsDisgust(bool value)
    {
    if(value)
        TRDT.value.emotionType = eDisgust;
    else if(IsDisgust())
        TRDT.value.emotionType = eNeutral;
    }

bool INFORecord::INFOResponse::IsFear()
    {
    return (TRDT.value.emotionType == eFear);
    }

void INFORecord::INFOResponse::IsFear(bool value)
    {
    if(value)
        TRDT.value.emotionType = eFear;
    else if(IsFear())
        TRDT.value.emotionType = eNeutral;
    }

bool INFORecord::INFOResponse::IsSad()
    {
    return (TRDT.value.emotionType == eSad);
    }

void INFORecord::INFOResponse::IsSad(bool value)
    {
    if(value)
        TRDT.value.emotionType = eSad;
    else if(IsSad())
        TRDT.value.emotionType = eNeutral;
    }

bool INFORecord::INFOResponse::IsHappy()
    {
    return (TRDT.value.emotionType == eHappy);
    }

void INFORecord::INFOResponse::IsHappy(bool value)
    {
    if(value)
        TRDT.value.emotionType = eHappy;
    else if(IsHappy())
        TRDT.value.emotionType = eNeutral;
    }

bool INFORecord::INFOResponse::IsSurprise()
    {
    return (TRDT.value.emotionType == eSurprise);
    }

void INFORecord::INFOResponse::IsSurprise(bool value)
    {
    if(value)
        TRDT.value.emotionType = eSurprise;
    else if(IsSurprise())
        TRDT.value.emotionType = eNeutral;
    }

bool INFORecord::INFOResponse::IsType(UINT32 Type)
    {
    return (TRDT.value.emotionType == Type);
    }

void INFORecord::INFOResponse::SetType(UINT32 Type)
    {
    TRDT.value.emotionType = Type;
    }

bool INFORecord::INFOResponse::operator ==(const INFOResponse &other) const
    {
    return (TRDT == other.TRDT &&
            NAM1.equals(other.NAM1) &&
            NAM2.equalsi(other.NAM2));
    }

bool INFORecord::INFOResponse::operator !=(const INFOResponse &other) const
    {
    return !(*this == other);
    }

INFORecord::INFORecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

INFORecord::INFORecord(INFORecord *srcRecord):
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

    DATA = srcRecord->DATA;
    QSTI = srcRecord->QSTI;
    TPIC = srcRecord->TPIC;
    PNAM = srcRecord->PNAM;
    NAME.resize(srcRecord->NAME.size());
    for(UINT32 x = 0; x < srcRecord->NAME.size(); x++)
        NAME[x] = srcRecord->NAME[x];
    Responses.clear();
    Responses.resize(srcRecord->Responses.size());
    for(UINT32 x = 0; x < srcRecord->Responses.size(); x++)
        {
        Responses[x] = new INFOResponse;
        Responses[x]->TRDT = srcRecord->Responses[x]->TRDT;
        Responses[x]->NAM1 = srcRecord->Responses[x]->NAM1;
        Responses[x]->NAM2 = srcRecord->Responses[x]->NAM2;
        }
    CTDA.clear();
    CTDA.resize(srcRecord->CTDA.size());
    for(UINT32 x = 0; x < srcRecord->CTDA.size(); x++)
        {
        CTDA[x] = new ReqSubRecord<GENCTDA>;
        *CTDA[x] = *srcRecord->CTDA[x];
        }
    TCLT.resize(srcRecord->TCLT.size());
    for(UINT32 x = 0; x < srcRecord->TCLT.size(); x++)
        TCLT[x] = srcRecord->TCLT[x];
    TCLF.resize(srcRecord->TCLF.size());
    for(UINT32 x = 0; x < srcRecord->TCLF.size(); x++)
        TCLF[x] = srcRecord->TCLF[x];
    //SCHD = srcRecord->SCHD;
    SCHR = srcRecord->SCHR;
    SCDA = srcRecord->SCDA;
    SCTX = srcRecord->SCTX;
    SCR_.clear();
    SCR_.resize(srcRecord->SCR_.size());
    for(UINT32 x = 0; x < srcRecord->SCR_.size(); x++)
        {
        SCR_[x] = new ReqSubRecord<GENSCR_>;
        *SCR_[x] = *srcRecord->SCR_[x];
        }
    return;
    }

INFORecord::~INFORecord()
    {
    for(UINT32 x = 0; x < Responses.size(); x++)
        delete Responses[x];
    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    for(UINT32 x = 0; x < SCR_.size(); x++)
        delete SCR_[x];
    }

bool INFORecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    FunctionArguments CTDAFunction;
    Function_Arguments_Iterator curCTDAFunction;
    op.Accept(QSTI.value.fid);
    if(TPIC.IsLoaded())
        op.Accept(TPIC->fid);
    if(PNAM.IsLoaded())
        op.Accept(PNAM->fid);
    for(UINT32 x = 0; x < NAME.size(); x++)
        op.Accept(NAME[x]);
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
            printf("Warning: INFORecord %08X uses an unknown function (%d)!\n", formID, CTDA[x]->value.ifunc);
        }
    for(UINT32 x = 0; x < TCLT.size(); x++)
        op.Accept(TCLT[x]);
    for(UINT32 x = 0; x < TCLF.size(); x++)
        op.Accept(TCLF[x]);
    for(UINT32 x = 0; x < SCR_.size(); x++)
        if(SCR_[x]->value.isSCRO)
            op.Accept(SCR_[x]->value.reference);

    return op.Stop();
    }

bool INFORecord::IsTopic()
    {
    return (DATA.value.dialType == eTopic);
    }

void INFORecord::IsTopic(bool value)
    {
    if(value)
        DATA.value.dialType = eTopic;
    else if(IsTopic())
        DATA.value.dialType = eConversation;
    }

bool INFORecord::IsConversation()
    {
    return (DATA.value.dialType == eConversation);
    }

void INFORecord::IsConversation(bool value)
    {
    if(value)
        DATA.value.dialType = eConversation;
    else if(IsConversation())
        DATA.value.dialType = eTopic;
    }

bool INFORecord::IsCombat()
    {
    return (DATA.value.dialType == eCombat);
    }

void INFORecord::IsCombat(bool value)
    {
    if(value)
        DATA.value.dialType = eCombat;
    else if(IsCombat())
        DATA.value.dialType = eTopic;
    }

bool INFORecord::IsPersuasion()
    {
    return (DATA.value.dialType == ePersuasion);
    }

void INFORecord::IsPersuasion(bool value)
    {
    if(value)
        DATA.value.dialType = ePersuasion;
    else if(IsPersuasion())
        DATA.value.dialType = eTopic;
    }

bool INFORecord::IsDetection()
    {
    return (DATA.value.dialType == eDetection);
    }

void INFORecord::IsDetection(bool value)
    {
    if(value)
        DATA.value.dialType = eDetection;
    else if(IsDetection())
        DATA.value.dialType = eTopic;
    }

bool INFORecord::IsService()
    {
    return (DATA.value.dialType == eService);
    }

void INFORecord::IsService(bool value)
    {
    if(value)
        DATA.value.dialType = eService;
    else if(IsService())
        DATA.value.dialType = eTopic;
    }

bool INFORecord::IsMisc()
    {
    return (DATA.value.dialType == eMisc);
    }

void INFORecord::IsMisc(bool value)
    {
    if(value)
        DATA.value.dialType = eMisc;
    else if(IsMisc())
        DATA.value.dialType = eTopic;
    }

bool INFORecord::IsDialogType(UINT16 Type)
    {
    return (DATA.value.dialType == Type);
    }

void INFORecord::SetDialogType(UINT16 Type)
    {
    DATA.value.dialType = Type;
    }

bool INFORecord::IsGoodbye()
    {
    return (DATA.value.flags & fIsGoodbye) != 0;
    }

void INFORecord::IsGoodbye(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsGoodbye) : (DATA.value.flags & ~fIsGoodbye);
    }

bool INFORecord::IsRandom()
    {
    return (DATA.value.flags & fIsRandom) != 0;
    }

void INFORecord::IsRandom(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsRandom) : (DATA.value.flags & ~fIsRandom);
    }

bool INFORecord::IsSayOnce()
    {
    return (DATA.value.flags & fIsSayOnce) != 0;
    }

void INFORecord::IsSayOnce(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsSayOnce) : (DATA.value.flags & ~fIsSayOnce);
    }

bool INFORecord::IsRunImmediately()
    {
    return (DATA.value.flags & fIsRunImmediately) != 0;
    }

void INFORecord::IsRunImmediately(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsRunImmediately) : (DATA.value.flags & ~fIsRunImmediately);
    }

bool INFORecord::IsInfoRefusal()
    {
    return (DATA.value.flags & fIsInfoRefusal) != 0;
    }

void INFORecord::IsInfoRefusal(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsInfoRefusal) : (DATA.value.flags & ~fIsInfoRefusal);
    }

bool INFORecord::IsRandomEnd()
    {
    return (DATA.value.flags & fIsRandomEnd) != 0;
    }

void INFORecord::IsRandomEnd(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsRandomEnd) : (DATA.value.flags & ~fIsRandomEnd);
    }

bool INFORecord::IsRunForRumors()
    {
    return (DATA.value.flags & fIsRunForRumors) != 0;
    }

void INFORecord::IsRunForRumors(bool value)
    {
    DATA.value.flags = value ? (DATA.value.flags | fIsRunForRumors) : (DATA.value.flags & ~fIsRunForRumors);
    }

bool INFORecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void INFORecord::SetFlagMask(UINT8 Mask)
    {
    DATA.value.flags = Mask;
    }

UINT32 INFORecord::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 0;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(QSTI.IsLoaded())
        TotSize += QSTI.GetSize() + 6;

    if(TPIC.IsLoaded())
        TotSize += TPIC.GetSize() + 6;

    if(PNAM.IsLoaded())
        TotSize += PNAM.GetSize() + 6;

    if(NAME.size())
        TotSize += (UINT32)NAME.size() * (sizeof(UINT32) + 6);

    for(UINT32 p = 0; p < Responses.size(); p++)
        {
        if(Responses[p]->TRDT.IsLoaded())
            TotSize += Responses[p]->TRDT.GetSize() + 6;

        if(Responses[p]->NAM1.IsLoaded())
            {
            cSize = Responses[p]->NAM1.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(Responses[p]->NAM2.IsLoaded())
            {
            cSize = Responses[p]->NAM2.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    for(UINT32 p = 0; p < CTDA.size(); p++)
        if(CTDA[p]->IsLoaded())
            TotSize += CTDA[p]->GetSize() + 6;

    if(TCLT.size())
        TotSize += (UINT32)TCLT.size() * (sizeof(UINT32) + 6);

    if(TCLF.size())
        TotSize += (UINT32)TCLF.size() * (sizeof(UINT32) + 6);

    //if(SCHD.IsLoaded())
    //    cSize += SCHD.GetSize() + 6;

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

    TotSize += (sizeof(UINT32) + 6) * (UINT32)SCR_.size();

    return TotSize;
    }

UINT32 INFORecord::GetType()
    {
    return 'OFNI';
    }

STRING INFORecord::GetStrType()
    {
    return "INFO";
    }

UINT32 INFORecord::GetParentType()
    {
    return 'LAID';
    }

SINT32 INFORecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    FORMID curFormID = 0;
    INFOResponse *newResponse = NULL;
    ReqSubRecord<GENCTDA> *newCTDA = NULL;
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
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'ITSQ':
                QSTI.Read(buffer, subSize, curPos);
                break;
            case 'CIPT':
                TPIC.Read(buffer, subSize, curPos);
                break;
            case 'MANP':
                PNAM.Read(buffer, subSize, curPos);
                break;
            case 'EMAN':
                _readBuffer(&curFormID,buffer,subSize,curPos);
                NAME.push_back(curFormID);
                break;
            case 'TDRT':
                newResponse = new INFOResponse;
                newResponse->TRDT.Read(buffer, subSize, curPos);
                Responses.push_back(newResponse);
                break;
            case '1MAN':
                if(newResponse == NULL)
                    {
                    newResponse = new INFOResponse;
                    Responses.push_back(newResponse);
                    }
                newResponse->NAM1.Read(buffer, subSize, curPos);
                break;
            case '2MAN':
                if(newResponse == NULL)
                    {
                    newResponse = new INFOResponse;
                    Responses.push_back(newResponse);
                    }
                newResponse->NAM2.Read(buffer, subSize, curPos);
                break;
            case 'TDTC':
            case 'ADTC':
                newCTDA = new ReqSubRecord<GENCTDA>;
                newCTDA->Read(buffer, subSize, curPos);
                CTDA.push_back(newCTDA);
                break;
            case 'TLCT':
                _readBuffer(&curFormID,buffer,subSize,curPos);
                TCLT.push_back(curFormID);
                break;
            case 'FLCT':
                _readBuffer(&curFormID,buffer,subSize,curPos);
                TCLF.push_back(curFormID);
                break;
            case 'DHCS': //replace it with SCHR. SCHDs are always zero filled in oblivion.esm...
                curPos += subSize;
                break;
            case 'RHCS': //SCHDs are also larger than SCHRs, so the end will be truncated.
                SCHR.Read(buffer, subSize, curPos);
                break;
            case 'ADCS':
                SCDA.Read(buffer, subSize, curPos);
                break;
            case 'XTCS':
                SCTX.Read(buffer, subSize, curPos);
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
                printf("  INFO: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 INFORecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    DATA.Unload();
    QSTI.Unload();
    TPIC.Unload();
    PNAM.Unload();

    NAME.clear();

    for(UINT32 x = 0; x < Responses.size(); x++)
        delete Responses[x];
    Responses.clear();

    for(UINT32 x = 0; x < CTDA.size(); x++)
        delete CTDA[x];
    CTDA.clear();

    TCLT.clear();
    TCLF.clear();

    //SCHD.Unload();
    SCHR.Unload();
    SCDA.Unload();
    SCTX.Unload();

    for(UINT32 x = 0; x < SCR_.size(); x++)
        delete SCR_[x];
    SCR_.clear();
    return 1;
    }

SINT32 INFORecord::WriteRecord(_FileHandler &SaveHandler)
    {
    FunctionArguments CTDAFunction;
    Function_Arguments_Iterator curCTDAFunction;

    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(QSTI.IsLoaded())
        SaveHandler.writeSubRecord('ITSQ', &QSTI.value, QSTI.GetSize());
    if(TPIC.IsLoaded())
        SaveHandler.writeSubRecord('CIPT', TPIC.value, TPIC.GetSize());
    if(PNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANP', PNAM.value, PNAM.GetSize());
    for(UINT32 p = 0; p < NAME.size(); p++)
        SaveHandler.writeSubRecord('EMAN', &NAME[p], sizeof(UINT32));
    if(Responses.size())
        {
        for(UINT32 p = 0; p < Responses.size(); p++)
            {
            if(Responses[p]->TRDT.IsLoaded())
                SaveHandler.writeSubRecord('TDRT', &Responses[p]->TRDT.value, Responses[p]->TRDT.GetSize());
            if(Responses[p]->NAM1.IsLoaded())
                SaveHandler.writeSubRecord('1MAN', Responses[p]->NAM1.value, Responses[p]->NAM1.GetSize());
            if(Responses[p]->NAM2.IsLoaded())
                SaveHandler.writeSubRecord('2MAN', Responses[p]->NAM2.value, Responses[p]->NAM2.GetSize());
            }
        }
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
    for(UINT32 p = 0; p < TCLT.size(); p++)
        SaveHandler.writeSubRecord('TLCT', &TCLT[p], sizeof(UINT32));
    for(UINT32 p = 0; p < TCLF.size(); p++)
        SaveHandler.writeSubRecord('FLCT', &TCLF[p], sizeof(UINT32));
    //if(SCHD.IsLoaded())
    //    SaveHandler.writeSubRecord('DHCS', SCHD.value, SCHD.GetSize());
    if(SCHR.IsLoaded())
        SaveHandler.writeSubRecord('RHCS', &SCHR.value, SCHR.GetSize());
    if(SCDA.IsLoaded())
        SaveHandler.writeSubRecord('ADCS', SCDA.value, SCDA.GetSize());
    if(SCTX.IsLoaded())
        SaveHandler.writeSubRecord('XTCS', SCTX.value, SCTX.GetSize());
    for(UINT32 p = 0; p < SCR_.size(); p++)
        if(SCR_[p]->IsLoaded())
            if(SCR_[p]->value.isSCRO)
                SaveHandler.writeSubRecord('ORCS', &SCR_[p]->value.reference, sizeof(UINT32));
            else
                SaveHandler.writeSubRecord('VRCS', &SCR_[p]->value.reference, sizeof(UINT32));
    return -1;
    }

bool INFORecord::operator ==(const INFORecord &other) const
    {
    if(DATA == other.DATA &&
        QSTI == other.QSTI &&
        TPIC == other.TPIC &&
        PNAM == other.PNAM &&
        SCHR == other.SCHR &&
        SCDA == other.SCDA &&
        SCTX.equalsi(other.SCTX) &&
        NAME.size() == other.NAME.size() &&
        Responses.size() == other.Responses.size() &&
        CTDA.size() == other.CTDA.size() &&
        TCLT.size() == other.TCLT.size() &&
        TCLF.size() == other.TCLF.size() &&
        SCR_.size() == other.SCR_.size())
        {
        //Not sure if record order matters on add topics, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < NAME.size(); ++x)
            if(NAME[x] != other.NAME[x])
                return false;

        //Not sure if record order matters on responses, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < Responses.size(); ++x)
            if(*Responses[x] != *other.Responses[x])
                return false;

        //Record order matters on conditions, so equality testing is easy
        for(UINT32 x = 0; x < CTDA.size(); ++x)
            if(*CTDA[x] != *other.CTDA[x])
                return false;

        //Not sure if record order matters on choices, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < TCLT.size(); ++x)
            if(TCLT[x] != other.TCLT[x])
                return false;

        //Not sure if record order matters on links from, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < TCLF.size(); ++x)
            if(TCLF[x] != other.TCLF[x])
                return false;

        //Record order matters on references, so equality testing is easy
        for(UINT32 x = 0; x < SCR_.size(); ++x)
            if(*SCR_[x] != *other.SCR_[x])
                return false;
        return true;
        }

    return false;
    }

bool INFORecord::operator !=(const INFORecord &other) const
    {
    return !(*this == other);
    }