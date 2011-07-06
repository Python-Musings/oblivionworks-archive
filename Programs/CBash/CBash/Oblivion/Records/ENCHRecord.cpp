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
#include "ENCHRecord.h"
#include <vector>

ENCHRecord::ENCHENIT::ENCHENIT():
    itemType(0),
    chargeAmount(0),
    enchantCost(0),
    flags(0)
    {
    memset(&unused1, 0x00, 3);
    }

ENCHRecord::ENCHENIT::~ENCHENIT()
    {
    //
    }

bool ENCHRecord::ENCHENIT::operator ==(const ENCHENIT &other) const
    {
    return (itemType == other.itemType &&
            chargeAmount == other.chargeAmount &&
            enchantCost == other.enchantCost &&
            flags == other.flags);
    }

bool ENCHRecord::ENCHENIT::operator !=(const ENCHENIT &other) const
    {
    return !(*this == other);
    }

ENCHRecord::ENCHRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

ENCHRecord::ENCHRecord(ENCHRecord *srcRecord):
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
    ENIT = srcRecord->ENIT;
    Effects.clear();
    Effects.resize(srcRecord->Effects.size());
    for(UINT32 x = 0; x < srcRecord->Effects.size(); x++)
        {
        Effects[x] = new GENEffect;
        Effects[x]->EFID = srcRecord->Effects[x]->EFID;
        Effects[x]->EFIT = srcRecord->Effects[x]->EFIT;
        Effects[x]->SCIT = srcRecord->Effects[x]->SCIT;
        Effects[x]->FULL = srcRecord->Effects[x]->FULL;
        if(srcRecord->Effects[x]->OBME.IsLoaded())
            {
            Effects[x]->OBME.Load();
            Effects[x]->OBME->EFME = srcRecord->Effects[x]->OBME->EFME;
            Effects[x]->OBME->EFII = srcRecord->Effects[x]->OBME->EFII;
            Effects[x]->OBME->EFIX = srcRecord->Effects[x]->OBME->EFIX;
            }
        }
    if(srcRecord->OBME.IsLoaded())
        {
        OBME.Load();
        OBME->OBME = srcRecord->OBME->OBME;
        OBME->DATX = srcRecord->OBME->DATX;
        }
    return;
    }

ENCHRecord::~ENCHRecord()
    {
    for(UINT32 x = 0; x < Effects.size(); x++)
        delete Effects[x];
    }

bool ENCHRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < Effects.size(); x++)
        {
        if(Effects[x]->OBME.IsLoaded())
            {
            if(Effects[x]->EFID.value.name >= 0x80000000)
                op.AcceptMGEF(Effects[x]->EFID.value.name);
            if(Effects[x]->EFIT.value.name >= 0x80000000)
                op.AcceptMGEF(Effects[x]->EFIT.value.name);
            switch(Effects[x]->OBME->EFME.value.efitParamInfo)
                {
                case 1: //It's a regular formID, so nothing fancy.
                    op.Accept(Effects[x]->EFIT.value.actorValue);
                    break;
                case 2: //It's a mgefCode, and not a formID at all.
                    //Conditional resolution of mgefCode's based on JRoush's OBME mod
                    //It's resolved just like a formID, except it uses the lower byte instead of the upper
                    if(Effects[x]->EFIT.value.actorValue >= 0x80000000)
                        op.AcceptMGEF(Effects[x]->EFIT.value.actorValue);
                    break;
                case 3: //It's an actor value, and not a formID at all.
                    //Conditional resolution of av's based on JRoush's OBME/AV mod(s)
                    //It's resolved just like a formID
                    if(Effects[x]->EFIT.value.actorValue >= 0x800)
                        op.Accept(Effects[x]->EFIT.value.actorValue);
                    break;
                default: //It's not a formID, mgefCode, or fancied up actor value
                    //so do nothing
                    break;
                }
            if(Effects[x]->SCIT.IsLoaded())
                {
                if(Effects[x]->SCIT->visual >= 0x80000000)
                    op.AcceptMGEF(Effects[x]->SCIT->visual);
                switch(Effects[x]->OBME->EFME.value.efixParamInfo)
                    {
                    case 1: //It's a regular formID, so nothing fancy.
                        op.Accept(Effects[x]->SCIT->script);
                        break;
                    case 2: //It's a mgefCode, and not a formID at all.
                        //Conditional resolution of mgefCode's based on JRoush's OBME mod
                        //It's resolved just like a formID, except it uses the lower byte instead of the upper
                        if(Effects[x]->SCIT->script >= 0x80000000)
                            op.AcceptMGEF(Effects[x]->SCIT->script);
                        break;
                    case 3: //It's an actor value, and not a formID at all.
                        //Conditional resolution of av's based on JRoush's OBME/AV mod(s)
                        //It's resolved just like a formID
                        if(Effects[x]->SCIT->script >= 0x800)
                            op.Accept(Effects[x]->SCIT->script);
                        break;
                    default: //It's not a formID, mgefCode, or fancied up actor value
                        //so do nothing
                        break;
                    }
                }
            if(Effects[x]->OBME->EFIX.IsLoaded())
                if(Effects[x]->OBME->EFIX->resistAV >= 0x800)
                    op.Accept(Effects[x]->OBME->EFIX->resistAV);
            }
        else
            {
            if(Effects[x]->SCIT.IsLoaded())
                op.Accept(Effects[x]->SCIT->script);
            }
        }

    return op.Stop();
    }

bool ENCHRecord::IsNoAutoCalc()
    {
    return (ENIT.value.flags & fIsNoAutoCalc) != 0;
    }

void ENCHRecord::IsNoAutoCalc(bool value)
    {
    ENIT.value.flags = value ? (ENIT.value.flags | fIsNoAutoCalc) : (ENIT.value.flags & ~fIsNoAutoCalc);
    }

bool ENCHRecord::IsFlagMask(UINT8 Mask, bool Exact)
    {
    return Exact ? ((ENIT.value.flags & Mask) == Mask) : ((ENIT.value.flags & Mask) != 0);
    }

void ENCHRecord::SetFlagMask(UINT8 Mask)
    {
    ENIT.value.flags = Mask;
    }

bool ENCHRecord::IsScroll()
    {
    return (ENIT.value.itemType == eScroll);
    }

void ENCHRecord::IsScroll(bool value)
    {
    if(value)
        ENIT.value.itemType = eScroll;
    else
        ENIT.value.itemType = eStaff;
    }

bool ENCHRecord::IsStaff()
    {
    return (ENIT.value.itemType == eStaff);
    }

void ENCHRecord::IsStaff(bool value)
    {
    if(value)
        ENIT.value.itemType = eStaff;
    else
        ENIT.value.itemType = eScroll;
    }

bool ENCHRecord::IsWeapon()
    {
    return (ENIT.value.itemType == eWeapon);
    }

void ENCHRecord::IsWeapon(bool value)
    {
    if(value)
        ENIT.value.itemType = eWeapon;
    else
        ENIT.value.itemType = eScroll;
    }

bool ENCHRecord::IsApparel()
    {
    return (ENIT.value.itemType == eApparel);
    }

void ENCHRecord::IsApparel(bool value)
    {
    if(value)
        ENIT.value.itemType = eApparel;
    else
        ENIT.value.itemType = eScroll;
    }

bool ENCHRecord::IsType(UINT32 Type)
    {
    return (ENIT.value.itemType == Type);
    }

void ENCHRecord::SetType(UINT32 Type)
    {
    ENIT.value.itemType = Type;
    }

UINT32 ENCHRecord::GetSize(bool forceCalc)
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

    if(OBME.IsLoaded())
        {
        if(OBME->OBME.IsLoaded())
            TotSize += OBME->OBME.GetSize() + 6;
        if(OBME->DATX.IsLoaded())
            {
            cSize = OBME->DATX.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        if(Effects.size())
            TotSize += 6; //EFXX chunk
        }

    if(FULL.IsLoaded())
        {
        cSize = FULL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(ENIT.IsLoaded())
        TotSize += ENIT.GetSize() + 6;

    for(UINT32 p = 0; p < Effects.size(); p++)
        {
        if(Effects[p]->EFID.IsLoaded())
            TotSize += Effects[p]->EFID.GetSize() + 6;

        if(Effects[p]->EFIT.IsLoaded())
            TotSize += Effects[p]->EFIT.GetSize() + 6;

        if(Effects[p]->SCIT.IsLoaded() || Effects[p]->FULL.IsLoaded())
            TotSize += Effects[p]->SCIT.GetSize() + 6;

        if(Effects[p]->FULL.IsLoaded())
            {
            cSize = Effects[p]->FULL.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(Effects[p]->OBME.IsLoaded())
            {
            if(Effects[p]->OBME->EFME.IsLoaded())
                TotSize += Effects[p]->OBME->EFME.GetSize() + 6;

            if(Effects[p]->OBME->EFII.IsLoaded())
                {
                cSize = Effects[p]->OBME->EFII.GetSize();
                if(cSize > 65535) cSize += 10;
                TotSize += cSize += 6;
                }

            if(Effects[p]->OBME->EFIX.IsLoaded())
                TotSize += Effects[p]->OBME->EFIX.GetSize() + 6;
            }
        }
    return TotSize;
    }


UINT32 ENCHRecord::GetType()
    {
    return 'HCNE';
    }

STRING ENCHRecord::GetStrType()
    {
    return "ENCH";
    }

SINT32 ENCHRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    GENEffect *newEffect = NULL;
    bool bNoOBME = true;
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
            case 'EMBO':
                OBME.Load();
                OBME->OBME.Read(buffer, subSize, curPos);
                break;
            case 'LLUF':
                if(newEffect == NULL)
                    FULL.Read(buffer, subSize, curPos);
                else
                    newEffect->FULL.Read(buffer, subSize, curPos);
                break;
            case 'TINE':
                ENIT.Read(buffer, subSize, curPos);
                break;
            case 'EMFE':
                bNoOBME = false;
                newEffect = new GENEffect;
                newEffect->OBME.Load();
                newEffect->OBME->EFME.Read(buffer, subSize, curPos);
                break;
            case 'DIFE':
                if(bNoOBME || newEffect == NULL)
                    newEffect = new GENEffect;
                newEffect->EFID.Read(buffer, subSize, curPos);
                Effects.push_back(newEffect);
                bNoOBME = true;
                break;
            case 'TIFE':
                if(newEffect == NULL)
                    {
                    newEffect = new GENEffect;
                    Effects.push_back(newEffect);
                    }
                newEffect->EFIT.Read(buffer, subSize, curPos);
                break;
            case 'TICS':
                if(newEffect == NULL)
                    {
                    newEffect = new GENEffect;
                    Effects.push_back(newEffect);
                    }
                newEffect->SCIT.Read(buffer, subSize, curPos);
                break;
            case 'IIFE':
                if(newEffect == NULL)
                    {
                    newEffect = new GENEffect;
                    Effects.push_back(newEffect);
                    }
                newEffect->OBME.Load();
                newEffect->OBME->EFII.Read(buffer, subSize, curPos);
                break;
            case 'XIFE':
                if(newEffect == NULL)
                    {
                    newEffect = new GENEffect;
                    Effects.push_back(newEffect);
                    }
                newEffect->OBME.Load();
                newEffect->OBME->EFIX.Read(buffer, subSize, curPos);
                break;
            case 'XXFE':
                curPos += subSize;
                break;
            case 'XTAD':
                OBME.Load();
                OBME->DATX.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  ENCH: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 ENCHRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    ENIT.Unload();
    for(UINT32 x = 0; x < Effects.size(); x++)
        delete Effects[x];
    Effects.clear();
    OBME.Unload();
    return 1;
    }

SINT32 ENCHRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(OBME.IsLoaded() && OBME->OBME.IsLoaded())
        SaveHandler.writeSubRecord('EMBO', &OBME->OBME.value, OBME->OBME.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(ENIT.IsLoaded())
        SaveHandler.writeSubRecord('TINE', &ENIT.value, ENIT.GetSize());
    if(Effects.size())
        for(UINT32 p = 0; p < Effects.size(); p++)
            {
            if(Effects[p]->OBME.IsLoaded() && Effects[p]->OBME->EFME.IsLoaded())
                SaveHandler.writeSubRecord('EMFE', &Effects[p]->OBME->EFME.value, Effects[p]->OBME->EFME.GetSize());
            if(Effects[p]->EFID.IsLoaded())
                SaveHandler.writeSubRecord('DIFE', &Effects[p]->EFID.value, Effects[p]->EFID.GetSize());
            if(Effects[p]->EFIT.IsLoaded())
                SaveHandler.writeSubRecord('TIFE', &Effects[p]->EFIT.value, Effects[p]->EFIT.GetSize());
            if(Effects[p]->SCIT.IsLoaded() || Effects[p]->FULL.IsLoaded())
                SaveHandler.writeSubRecord('TICS', Effects[p]->SCIT.value, Effects[p]->SCIT.GetSize());
            if(Effects[p]->FULL.IsLoaded())
                SaveHandler.writeSubRecord('LLUF', Effects[p]->FULL.value, Effects[p]->FULL.GetSize());
            if(Effects[p]->OBME.IsLoaded() && Effects[p]->OBME->EFII.IsLoaded())
                SaveHandler.writeSubRecord('IIFE', Effects[p]->OBME->EFII.value, Effects[p]->OBME->EFII.GetSize());
            if(Effects[p]->OBME.IsLoaded() && Effects[p]->OBME->EFIX.IsLoaded())
                SaveHandler.writeSubRecord('XIFE', Effects[p]->OBME->EFIX.value, Effects[p]->OBME->EFIX.GetSize());
            }
    if(Effects.size() && OBME.IsLoaded())
        SaveHandler.writeSubRecord('XXFE', NULL, 0);
    if(OBME.IsLoaded() && OBME->DATX.IsLoaded())
        SaveHandler.writeSubRecord('XTAD', OBME->DATX.value, OBME->DATX.GetSize());
    return -1;
    }

bool ENCHRecord::operator ==(const ENCHRecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        FULL.equals(other.FULL) &&
        ENIT == other.ENIT &&
        OBME == other.OBME &&
        Effects.size() == other.Effects.size())
        {
        //Record order doesn't matter on effects, so equality testing isn't easy
        //The proper solution would be to check each effect against every other effect to see if there's a one-to-one match
        //Fix-up later
        for(UINT32 x = 0; x < Effects.size(); ++x)
            if(*Effects[x] != *other.Effects[x])
                return false;
        return true;
        }

    return false;
    }

bool ENCHRecord::operator !=(const ENCHRecord &other) const
    {
    return !(*this == other);
    }