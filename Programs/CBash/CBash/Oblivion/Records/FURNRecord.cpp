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
#include "FURNRecord.h"

FURNRecord::FURNRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

FURNRecord::FURNRecord(FURNRecord *srcRecord):
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
    MNAM = srcRecord->MNAM;
    return;
    }

FURNRecord::~FURNRecord()
    {
    //
    }

bool FURNRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    if(SCRI.IsLoaded())
        op.Accept(SCRI->fid);

    return op.Stop();
    }

bool FURNRecord::IsAnim01()
    {
    return (MNAM.value.flags & fIsAnim01) != 0;
    }

void FURNRecord::IsAnim01(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim01) : (MNAM.value.flags & ~fIsAnim01);
    }

bool FURNRecord::IsAnim02()
    {
    return (MNAM.value.flags & fIsAnim02) != 0;
    }

void FURNRecord::IsAnim02(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim02) : (MNAM.value.flags & ~fIsAnim02);
    }

bool FURNRecord::IsAnim03()
    {
    return (MNAM.value.flags & fIsAnim03) != 0;
    }

void FURNRecord::IsAnim03(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim03) : (MNAM.value.flags & ~fIsAnim03);
    }

bool FURNRecord::IsAnim04()
    {
    return (MNAM.value.flags & fIsAnim04) != 0;
    }

void FURNRecord::IsAnim04(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim04) : (MNAM.value.flags & ~fIsAnim04);
    }

bool FURNRecord::IsAnim05()
    {
    return (MNAM.value.flags & fIsAnim05) != 0;
    }

void FURNRecord::IsAnim05(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim05) : (MNAM.value.flags & ~fIsAnim05);
    }

bool FURNRecord::IsAnim06()
    {
    return (MNAM.value.flags & fIsAnim06) != 0;
    }

void FURNRecord::IsAnim06(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim06) : (MNAM.value.flags & ~fIsAnim06);
    }

bool FURNRecord::IsAnim07()
    {
    return (MNAM.value.flags & fIsAnim07) != 0;
    }

void FURNRecord::IsAnim07(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim07) : (MNAM.value.flags & ~fIsAnim07);
    }

bool FURNRecord::IsAnim08()
    {
    return (MNAM.value.flags & fIsAnim08) != 0;
    }

void FURNRecord::IsAnim08(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim08) : (MNAM.value.flags & ~fIsAnim08);
    }

bool FURNRecord::IsAnim09()
    {
    return (MNAM.value.flags & fIsAnim09) != 0;
    }

void FURNRecord::IsAnim09(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim09) : (MNAM.value.flags & ~fIsAnim09);
    }

bool FURNRecord::IsAnim10()
    {
    return (MNAM.value.flags & fIsAnim10) != 0;
    }

void FURNRecord::IsAnim10(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim10) : (MNAM.value.flags & ~fIsAnim10);
    }

bool FURNRecord::IsAnim11()
    {
    return (MNAM.value.flags & fIsAnim11) != 0;
    }

void FURNRecord::IsAnim11(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim11) : (MNAM.value.flags & ~fIsAnim11);
    }

bool FURNRecord::IsAnim12()
    {
    return (MNAM.value.flags & fIsAnim12) != 0;
    }

void FURNRecord::IsAnim12(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim12) : (MNAM.value.flags & ~fIsAnim12);
    }

bool FURNRecord::IsAnim13()
    {
    return (MNAM.value.flags & fIsAnim13) != 0;
    }

void FURNRecord::IsAnim13(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim13) : (MNAM.value.flags & ~fIsAnim13);
    }

bool FURNRecord::IsAnim14()
    {
    return (MNAM.value.flags & fIsAnim14) != 0;
    }

void FURNRecord::IsAnim14(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim14) : (MNAM.value.flags & ~fIsAnim14);
    }

bool FURNRecord::IsAnim15()
    {
    return (MNAM.value.flags & fIsAnim15) != 0;
    }

void FURNRecord::IsAnim15(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim15) : (MNAM.value.flags & ~fIsAnim15);
    }

bool FURNRecord::IsAnim16()
    {
    return (MNAM.value.flags & fIsAnim16) != 0;
    }

void FURNRecord::IsAnim16(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim16) : (MNAM.value.flags & ~fIsAnim16);
    }

bool FURNRecord::IsAnim17()
    {
    return (MNAM.value.flags & fIsAnim17) != 0;
    }

void FURNRecord::IsAnim17(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim17) : (MNAM.value.flags & ~fIsAnim17);
    }

bool FURNRecord::IsAnim18()
    {
    return (MNAM.value.flags & fIsAnim18) != 0;
    }

void FURNRecord::IsAnim18(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim18) : (MNAM.value.flags & ~fIsAnim18);
    }

bool FURNRecord::IsAnim19()
    {
    return (MNAM.value.flags & fIsAnim19) != 0;
    }

void FURNRecord::IsAnim19(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim19) : (MNAM.value.flags & ~fIsAnim19);
    }

bool FURNRecord::IsAnim20()
    {
    return (MNAM.value.flags & fIsAnim20) != 0;
    }

void FURNRecord::IsAnim20(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim20) : (MNAM.value.flags & ~fIsAnim20);
    }

bool FURNRecord::IsAnim21()
    {
    return (MNAM.value.flags & fIsAnim21) != 0;
    }

void FURNRecord::IsAnim21(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim21) : (MNAM.value.flags & ~fIsAnim21);
    }

bool FURNRecord::IsAnim22()
    {
    return (MNAM.value.flags & fIsAnim22) != 0;
    }

void FURNRecord::IsAnim22(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim22) : (MNAM.value.flags & ~fIsAnim22);
    }

bool FURNRecord::IsAnim23()
    {
    return (MNAM.value.flags & fIsAnim23) != 0;
    }

void FURNRecord::IsAnim23(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim23) : (MNAM.value.flags & ~fIsAnim23);
    }

bool FURNRecord::IsAnim24()
    {
    return (MNAM.value.flags & fIsAnim24) != 0;
    }

void FURNRecord::IsAnim24(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim24) : (MNAM.value.flags & ~fIsAnim24);
    }

bool FURNRecord::IsAnim25()
    {
    return (MNAM.value.flags & fIsAnim25) != 0;
    }

void FURNRecord::IsAnim25(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim25) : (MNAM.value.flags & ~fIsAnim25);
    }

bool FURNRecord::IsAnim26()
    {
    return (MNAM.value.flags & fIsAnim26) != 0;
    }

void FURNRecord::IsAnim26(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim26) : (MNAM.value.flags & ~fIsAnim26);
    }

bool FURNRecord::IsAnim27()
    {
    return (MNAM.value.flags & fIsAnim27) != 0;
    }

void FURNRecord::IsAnim27(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim27) : (MNAM.value.flags & ~fIsAnim27);
    }

bool FURNRecord::IsAnim28()
    {
    return (MNAM.value.flags & fIsAnim28) != 0;
    }

void FURNRecord::IsAnim28(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim28) : (MNAM.value.flags & ~fIsAnim28);
    }

bool FURNRecord::IsAnim29()
    {
    return (MNAM.value.flags & fIsAnim29) != 0;
    }

void FURNRecord::IsAnim29(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim29) : (MNAM.value.flags & ~fIsAnim29);
    }

bool FURNRecord::IsAnim30()
    {
    return (MNAM.value.flags & fIsAnim30) != 0;
    }

void FURNRecord::IsAnim30(bool value)
    {
    MNAM.value.flags = value ? (MNAM.value.flags | fIsAnim30) : (MNAM.value.flags & ~fIsAnim30);
    }

bool FURNRecord::IsSitAnim()
    {
    return (MNAM.value.flags & fIsSitAnim) != 0;
    }

void FURNRecord::IsSitAnim(bool value)
    {
    if(value)
        {
        MNAM.value.flags &= ~fIsSleepAnim;
        MNAM.value.flags |= fIsSitAnim;
        }
    else
        {
        MNAM.value.flags &= ~fIsSitAnim;
        MNAM.value.flags |= fIsSleepAnim;
        }
    }

bool FURNRecord::IsSleepAnim()
    {
    return (MNAM.value.flags & fIsSleepAnim) != 0;
    }

void FURNRecord::IsSleepAnim(bool value)
    {
    if(value)
        {
        MNAM.value.flags &= ~fIsSitAnim;
        MNAM.value.flags |= fIsSleepAnim;
        }
    else
        {
        MNAM.value.flags &= ~fIsSleepAnim;
        MNAM.value.flags |= fIsSitAnim;
        }
    }

bool FURNRecord::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((MNAM.value.flags & Mask) == Mask) : ((MNAM.value.flags & Mask) != 0);
    }

void FURNRecord::SetFlagMask(UINT32 Mask)
    {
    MNAM.value.flags = Mask;
    }

UINT32 FURNRecord::GetSize(bool forceCalc)
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

    if(MNAM.IsLoaded())
        TotSize += MNAM.GetSize() + 6;

    return TotSize;
    }

UINT32 FURNRecord::GetType()
    {
    return 'NRUF';
    }

STRING FURNRecord::GetStrType()
    {
    return "FURN";
    }

SINT32 FURNRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
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
            case 'MANM':
                MNAM.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  FURN: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 FURNRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    MODL.Unload();
    SCRI.Unload();
    MNAM.Unload();
    return 1;
    }

SINT32 FURNRecord::WriteRecord(_FileHandler &SaveHandler)
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
    if(MNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANM', &MNAM.value, MNAM.GetSize());
    return -1;
    }

bool FURNRecord::operator ==(const FURNRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            FULL.equals(other.FULL) &&
            MODL == other.MODL &&
            SCRI == other.SCRI &&
            MNAM == other.MNAM);
    }

bool FURNRecord::operator !=(const FURNRecord &other) const
    {
    return !(*this == other);
    }