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
#include "FLORRecord.h"

FLORRecord::FLORPFPC::FLORPFPC():
    spring(0),
    summer(0),
    fall(0),
    winter(0)
    {
    //
    }

FLORRecord::FLORPFPC::~FLORPFPC()
    {
    //
    }

bool FLORRecord::FLORPFPC::operator ==(const FLORPFPC &other) const
    {
    return (spring == other.spring &&
            summer == other.summer &&
            fall == other.fall &&
            winter == other.winter);
    }

bool FLORRecord::FLORPFPC::operator !=(const FLORPFPC &other) const
    {
    return !(*this == other);
    }

FLORRecord::FLORRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

FLORRecord::FLORRecord(FLORRecord *srcRecord):
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
    SCRI = srcRecord->SCRI;
    PFIG = srcRecord->PFIG;
    PFPC = srcRecord->PFPC;
    return;
    }

FLORRecord::~FLORRecord()
    {
    //
    }

bool FLORRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);
    if(PFIG.IsLoaded())
        op.Accept(PFIG.value.fid);

    return op.Stop();
    }

UINT32 FLORRecord::GetSize(bool forceCalc)
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

    if(SCRI.IsLoaded())
        TotSize += SCRI.GetSize() + 6;

    if(PFIG.IsLoaded())
        TotSize += PFIG.GetSize() + 6;

    if(PFPC.IsLoaded())
        TotSize += PFPC.GetSize() + 6;

    return TotSize;
    }

UINT32 FLORRecord::GetType()
    {
    return 'ROLF';
    }

STRING FLORRecord::GetStrType()
    {
    return "FLOR";
    }

SINT32 FLORRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'IRCS':
                SCRI.Read(buffer, subSize, curPos);
                break;
            case 'GIFP':
                PFIG.Read(buffer, subSize, curPos);
                break;
            case 'CPFP':
                PFPC.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  FLOR: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 FLORRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    MODL.Unload();
    SCRI.Unload();
    PFIG.Unload();
    PFPC.Unload();
    return 1;
    }

SINT32 FLORRecord::WriteRecord(_FileHandler &SaveHandler)
    {
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
    if(SCRI.IsLoaded())
        SaveHandler.writeSubRecord('IRCS', SCRI.value, SCRI.GetSize());
    if(PFIG.IsLoaded())
        SaveHandler.writeSubRecord('GIFP', &PFIG.value, PFIG.GetSize());
    if(PFPC.IsLoaded())
        SaveHandler.writeSubRecord('CPFP', &PFPC.value, PFPC.GetSize());
    return -1;
    }

bool FLORRecord::operator ==(const FLORRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            MODL == other.MODL &&
            SCRI == other.SCRI &&
            PFIG == other.PFIG &&
            PFPC == other.PFPC);
    }

bool FLORRecord::operator !=(const FLORRecord &other) const
    {
    return !(*this == other);
    }