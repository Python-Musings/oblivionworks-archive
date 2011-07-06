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
#include "RACERecord.h"
#include <vector>

RACERecord::RACESKILL::RACESKILL():
    value(-1),
    boost(0)
    {
    //
    }

RACERecord::RACESKILL::~RACESKILL()
    {
    //
    }

bool RACERecord::RACESKILL::operator ==(const RACESKILL &other) const
    {
    return (value == other.value &&
            boost == other.boost);
    }

bool RACERecord::RACESKILL::operator !=(const RACESKILL &other) const
    {
    return !(*this == other);
    }

RACERecord::RACEDATA::RACEDATA():
    maleHeight(1.0f),
    femaleHeight(1.0f),
    maleWeight(1.0f),
    femaleWeight(1.0f),
    flags(0)
    {
    memset(&unused1, 0x00, 2);
    }

RACERecord::RACEDATA::~RACEDATA()
    {
    //
    }

bool RACERecord::RACEDATA::operator ==(const RACEDATA &other) const
    {
    return (skills[0] == other.skills[0] &&
            skills[1] == other.skills[1] &&
            skills[2] == other.skills[2] &&
            skills[3] == other.skills[3] &&
            skills[4] == other.skills[4] &&
            skills[5] == other.skills[5] &&
            skills[6] == other.skills[6] &&
            AlmostEqual(maleHeight,other.maleHeight,2) &&
            AlmostEqual(femaleHeight,other.femaleHeight,2) &&
            AlmostEqual(maleWeight,other.maleWeight,2) &&
            AlmostEqual(femaleWeight,other.femaleWeight,2) &&
            flags == other.flags);
    }

bool RACERecord::RACEDATA::operator !=(const RACEDATA &other) const
    {
    return !(*this == other);
    }

RACERecord::RACEVNAM::RACEVNAM():
    maleVoice(0),
    femaleVoice(0)
    {
    //
    }

RACERecord::RACEVNAM::~RACEVNAM()
    {
    //
    }

bool RACERecord::RACEVNAM::operator ==(const RACEVNAM &other) const
    {
    return (maleVoice == other.maleVoice &&
            femaleVoice == other.femaleVoice);
    }

bool RACERecord::RACEVNAM::operator !=(const RACEVNAM &other) const
    {
    return !(*this == other);
    }

RACERecord::RACEDNAM::RACEDNAM():
    defaultHairMale(0),
    defaultHairFemale(0)
    {
    //
    }

RACERecord::RACEDNAM::~RACEDNAM()
    {
    //
    }

bool RACERecord::RACEDNAM::operator ==(const RACEDNAM &other) const
    {
    return (defaultHairMale == other.defaultHairMale &&
            defaultHairFemale == other.defaultHairFemale);
    }

bool RACERecord::RACEDNAM::operator !=(const RACEDNAM &other) const
    {
    return !(*this == other);
    }

RACERecord::RACECNAM::RACECNAM():
    defaultHairColor(0)
    {
    //
    }

RACERecord::RACECNAM::~RACECNAM()
    {
    //
    }

bool RACERecord::RACECNAM::operator ==(const RACECNAM &other) const
    {
    return (defaultHairColor == other.defaultHairColor);
    }

bool RACERecord::RACECNAM::operator !=(const RACECNAM &other) const
    {
    return !(*this == other);
    }

RACERecord::RACEPNAM::RACEPNAM():
    mainClamp(0.0f)
    {
    //
    }

RACERecord::RACEPNAM::~RACEPNAM()
    {
    //
    }

bool RACERecord::RACEPNAM::operator ==(const RACEPNAM &other) const
    {
    return (AlmostEqual(mainClamp,other.mainClamp,2));
    }

bool RACERecord::RACEPNAM::operator !=(const RACEPNAM &other) const
    {
    return !(*this == other);
    }

RACERecord::RACEUNAM::RACEUNAM():
    faceClamp(0.0f)
    {
    //
    }

RACERecord::RACEUNAM::~RACEUNAM()
    {
    //
    }

bool RACERecord::RACEUNAM::operator ==(const RACEUNAM &other) const
    {
    return (AlmostEqual(faceClamp,other.faceClamp,2));
    }

bool RACERecord::RACEUNAM::operator !=(const RACEUNAM &other) const
    {
    return !(*this == other);
    }

RACERecord::RACEATTR::RACEATTR():maleStrength(50), maleIntelligence(50),
    maleWillpower(50), maleAgility(50),
    maleSpeed(50), maleEndurance(50),
    malePersonality(50), maleLuck(50),
    femaleStrength(50), femaleIntelligence(50),
    femaleWillpower(50), femaleAgility(50),
    femaleSpeed(50), femaleEndurance(50),
    femalePersonality(50), femaleLuck(50)
    {
    //
    }

RACERecord::RACEATTR::~RACEATTR()
    {
    //
    }

bool RACERecord::RACEATTR::operator ==(const RACEATTR &other) const
    {
    return (maleStrength == other.maleStrength &&
            maleIntelligence == other.maleIntelligence &&
            maleWillpower == other.maleWillpower &&
            maleAgility == other.maleAgility &&
            maleSpeed == other.maleSpeed &&
            maleEndurance == other.maleEndurance &&
            malePersonality == other.malePersonality &&
            maleLuck == other.maleLuck &&
            femaleStrength == other.femaleStrength &&
            femaleIntelligence == other.femaleIntelligence &&
            femaleWillpower == other.femaleWillpower &&
            femaleAgility == other.femaleAgility &&
            femaleSpeed == other.femaleSpeed &&
            femaleEndurance == other.femaleEndurance &&
            femalePersonality == other.femalePersonality &&
            femaleLuck == other.femaleLuck);
    }

bool RACERecord::RACEATTR::operator !=(const RACEATTR &other) const
    {
    return !(*this == other);
    }

bool RACERecord::RACEMODEL::operator ==(const RACEMODEL &other) const
    {
    return (MODB == other.MODB &&
            MODL.equalsi(other.MODL) &&
            MODT == other.MODT &&
            ICON.equalsi(other.ICON));
    }

bool RACERecord::RACEMODEL::operator !=(const RACEMODEL &other) const
    {
    return !(*this == other);
    }

RACERecord::RACESNAM::RACESNAM()
    {
    memset(&SNAM, 0x00, 2);
    }

RACERecord::RACESNAM::~RACESNAM()
    {
    //
    }

bool RACERecord::RACESNAM::operator ==(const RACESNAM &other) const
    {
    return (SNAM[0] == other.SNAM[0] &&
            SNAM[1] == other.SNAM[1]);
    }

bool RACERecord::RACESNAM::operator !=(const RACESNAM &other) const
    {
    return !(*this == other);
    }

RACERecord::RACERecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

RACERecord::RACERecord(RACERecord *srcRecord):
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

    SPLO.resize(srcRecord->SPLO.size());
    for(UINT32 x = 0; x < srcRecord->SPLO.size(); x++)
        SPLO[x] = srcRecord->SPLO[x];

    XNAM.resize(srcRecord->XNAM.size());
    for(UINT32 x = 0; x < srcRecord->XNAM.size(); x++)
        {
        XNAM[x] = new ReqSubRecord<GENXNAM>;
        *XNAM[x] = *srcRecord->XNAM[x];
        }

    DATA = srcRecord->DATA;

    VNAM = srcRecord->VNAM;
    DNAM = srcRecord->DNAM;

    CNAM = srcRecord->CNAM;
    PNAM = srcRecord->PNAM;
    UNAM = srcRecord->UNAM;
    ATTR = srcRecord->ATTR;

    if(srcRecord->MOD0.IsLoaded())
        {
        MOD0.Load();
        MOD0->MODB = srcRecord->MOD0->MODB;
        MOD0->MODL = srcRecord->MOD0->MODL;
        MOD0->MODT = srcRecord->MOD0->MODT;
        MOD0->ICON = srcRecord->MOD0->ICON;
        }

    if(srcRecord->MOD1.IsLoaded())
        {
        MOD1.Load();
        MOD1->MODB = srcRecord->MOD1->MODB;
        MOD1->MODL = srcRecord->MOD1->MODL;
        MOD1->MODT = srcRecord->MOD1->MODT;
        MOD1->ICON = srcRecord->MOD1->ICON;
        }

    if(srcRecord->MOD2.IsLoaded())
        {
        MOD2.Load();
        MOD2->MODB = srcRecord->MOD2->MODB;
        MOD2->MODL = srcRecord->MOD2->MODL;
        MOD2->MODT = srcRecord->MOD2->MODT;
        MOD2->ICON = srcRecord->MOD2->ICON;
        }

    if(srcRecord->MOD3.IsLoaded())
        {
        MOD3.Load();
        MOD3->MODB = srcRecord->MOD3->MODB;
        MOD3->MODL = srcRecord->MOD3->MODL;
        MOD3->MODT = srcRecord->MOD3->MODT;
        MOD3->ICON = srcRecord->MOD3->ICON;
        }

    if(srcRecord->MOD4.IsLoaded())
        {
        MOD4.Load();
        MOD4->MODB = srcRecord->MOD4->MODB;
        MOD4->MODL = srcRecord->MOD4->MODL;
        MOD4->MODT = srcRecord->MOD4->MODT;
        MOD4->ICON = srcRecord->MOD4->ICON;
        }

    if(srcRecord->MOD5.IsLoaded())
        {
        MOD5.Load();
        MOD5->MODB = srcRecord->MOD5->MODB;
        MOD5->MODL = srcRecord->MOD5->MODL;
        MOD5->MODT = srcRecord->MOD5->MODT;
        MOD5->ICON = srcRecord->MOD5->ICON;
        }

    if(srcRecord->MOD6.IsLoaded())
        {
        MOD6.Load();
        MOD6->MODB = srcRecord->MOD6->MODB;
        MOD6->MODL = srcRecord->MOD6->MODL;
        MOD6->MODT = srcRecord->MOD6->MODT;
        MOD6->ICON = srcRecord->MOD6->ICON;
        }

    if(srcRecord->MOD7.IsLoaded())
        {
        MOD7.Load();
        MOD7->MODB = srcRecord->MOD7->MODB;
        MOD7->MODL = srcRecord->MOD7->MODL;
        MOD7->MODT = srcRecord->MOD7->MODT;
        MOD7->ICON = srcRecord->MOD7->ICON;
        }

    if(srcRecord->MOD8.IsLoaded())
        {
        MOD8.Load();
        MOD8->MODB = srcRecord->MOD8->MODB;
        MOD8->MODL = srcRecord->MOD8->MODL;
        MOD8->MODT = srcRecord->MOD8->MODT;
        MOD8->ICON = srcRecord->MOD8->ICON;
        }

    if(srcRecord->MMODL.IsLoaded())
        {
        MMODL.Load();
        MMODL->MODB = srcRecord->MMODL->MODB;
        MMODL->MODL = srcRecord->MMODL->MODL;
        MMODL->MODT = srcRecord->MMODL->MODT;
        }

    MICON0 = srcRecord->MICON0;
    MICON1 = srcRecord->MICON1;
    MICON2 = srcRecord->MICON2;
    MICON3 = srcRecord->MICON3;
    MICON4 = srcRecord->MICON4;

    if(srcRecord->FMODL.IsLoaded())
        {
        FMODL.Load();
        FMODL->MODB = srcRecord->FMODL->MODB;
        FMODL->MODL = srcRecord->FMODL->MODL;
        FMODL->MODT = srcRecord->FMODL->MODT;
        }

    FICON0 = srcRecord->FICON0;
    FICON1 = srcRecord->FICON1;
    FICON2 = srcRecord->FICON2;
    FICON3 = srcRecord->FICON3;
    FICON4 = srcRecord->FICON4;
    HNAM = srcRecord->HNAM;
    ENAM = srcRecord->ENAM;
    FGGS = srcRecord->FGGS;
    FGGA = srcRecord->FGGA;
    FGTS = srcRecord->FGTS;
    SNAM = srcRecord->SNAM;
    }

RACERecord::~RACERecord()
    {
    for(UINT32 x = 0; x < XNAM.size(); x++)
        delete XNAM[x];
    }

bool RACERecord::IsPlayable()
    {
    return (DATA.value.flags & fIsPlayable) != 0;
    }

void RACERecord::IsPlayable(bool value)
    {
    if(value)
        DATA.value.flags |= fIsPlayable;
    else
        DATA.value.flags &= ~fIsPlayable;
    }

bool RACERecord::IsFlagMask(UINT32 Mask, bool Exact)
    {
    return Exact ? ((DATA.value.flags & Mask) == Mask) : ((DATA.value.flags & Mask) != 0);
    }

void RACERecord::SetFlagMask(UINT32 Mask)
    {
    DATA.value.flags = Mask;
    }

bool RACERecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    for(UINT32 x = 0; x < SPLO.size(); x++)
        op.Accept(SPLO[x]);
    for(UINT32 x = 0; x < XNAM.size(); x++)
        op.Accept(XNAM[x]->value.faction);
    if(VNAM.IsLoaded())
        {
        op.Accept(VNAM.value.femaleVoice);
        op.Accept(VNAM.value.maleVoice);
        }
    if(DNAM.IsLoaded())
        {
        op.Accept(DNAM.value.defaultHairFemale);
        op.Accept(DNAM.value.defaultHairMale);
        }
    for(UINT32 x = 0; x < HNAM.size(); x++)
        op.Accept(HNAM[x]);
    for(UINT32 x = 0; x < ENAM.size(); x++)
        op.Accept(ENAM[x]);

    return op.Stop();
    }

UINT32 RACERecord::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 226;
    //TotSize += 96; //NAM0, INDX 0, INDX 1, INDX 2, INDX 3, INDX 4, INDX 5, INDX 6, INDX 7, INDX 8
    //TotSize += 62; //NAM1, MNAM, INDX 0, INDX 1, INDX 2, INDX 3, INDX 4
    //TotSize += 56; //FNAM, INDX 0, INDX 1, INDX 2, INDX 3, INDX 4
    //TotSize += 6; //ENAM, written even if empty
    //TotSize += 6; //HNAM, written even if empty

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

    if(SPLO.size())
        TotSize += (UINT32)SPLO.size() * (sizeof(UINT32) + 6);

    for(UINT32 p = 0; p < XNAM.size(); p++)
        TotSize += XNAM[p]->GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    if(VNAM.IsLoaded())
        TotSize += VNAM.GetSize() + 6;

    if(DNAM.IsLoaded())
        TotSize += DNAM.GetSize() + 6;

    if(CNAM.IsLoaded())
        TotSize += CNAM.GetSize() + 6;

    if(PNAM.IsLoaded())
        TotSize += PNAM.GetSize() + 6;

    if(UNAM.IsLoaded())
        TotSize += UNAM.GetSize() + 6;

    if(ATTR.IsLoaded())
        TotSize += ATTR.GetSize() + 6;

    if(MOD0.IsLoaded() && MOD0->MODL.IsLoaded())
        {
        cSize = MOD0->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD0->MODB.IsLoaded())
            TotSize += MOD0->MODB.GetSize() + 6;

        if(MOD0->MODT.IsLoaded())
            {
            cSize = MOD0->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD0->ICON.IsLoaded())
            {
            cSize = MOD0->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD1.IsLoaded() && MOD1->MODL.IsLoaded())
        {
        cSize = MOD1->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD1->MODB.IsLoaded())
            TotSize += MOD1->MODB.GetSize() + 6;

        if(MOD1->MODT.IsLoaded())
            {
            cSize = MOD1->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD1->ICON.IsLoaded())
            {
            cSize = MOD1->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD2.IsLoaded() && MOD2->MODL.IsLoaded())
        {
        cSize = MOD2->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD2->MODB.IsLoaded())
            TotSize += MOD2->MODB.GetSize() + 6;

        if(MOD2->MODT.IsLoaded())
            {
            cSize = MOD2->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD2->ICON.IsLoaded())
            {
            cSize = MOD2->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD3.IsLoaded() && MOD3->MODL.IsLoaded())
        {
        cSize = MOD3->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD3->MODB.IsLoaded())
            TotSize += MOD3->MODB.GetSize() + 6;

        if(MOD3->MODT.IsLoaded())
            {
            cSize = MOD3->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD3->ICON.IsLoaded())
            {
            cSize = MOD3->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD4.IsLoaded() && MOD4->MODL.IsLoaded())
        {
        cSize = MOD4->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD4->MODB.IsLoaded())
            TotSize += MOD4->MODB.GetSize() + 6;

        if(MOD4->MODT.IsLoaded())
            {
            cSize = MOD4->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD4->ICON.IsLoaded())
            {
            cSize = MOD4->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD5.IsLoaded() && MOD5->MODL.IsLoaded())
        {
        cSize = MOD5->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD5->MODB.IsLoaded())
            TotSize += MOD5->MODB.GetSize() + 6;

        if(MOD5->MODT.IsLoaded())
            {
            cSize = MOD5->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD5->ICON.IsLoaded())
            {
            cSize = MOD5->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD6.IsLoaded() && MOD6->MODL.IsLoaded())
        {
        cSize = MOD6->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD6->MODB.IsLoaded())
            TotSize += MOD6->MODB.GetSize() + 6;

        if(MOD6->MODT.IsLoaded())
            {
            cSize = MOD6->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD6->ICON.IsLoaded())
            {
            cSize = MOD6->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD7.IsLoaded() && MOD7->MODL.IsLoaded())
        {
        cSize = MOD7->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD7->MODB.IsLoaded())
            TotSize += MOD7->MODB.GetSize() + 6;

        if(MOD7->MODT.IsLoaded())
            {
            cSize = MOD7->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD7->ICON.IsLoaded())
            {
            cSize = MOD7->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MOD8.IsLoaded() && MOD8->MODL.IsLoaded())
        {
        cSize = MOD8->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MOD8->MODB.IsLoaded())
            TotSize += MOD8->MODB.GetSize() + 6;

        if(MOD8->MODT.IsLoaded())
            {
            cSize = MOD8->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(MOD8->ICON.IsLoaded())
            {
            cSize = MOD8->ICON.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MMODL.IsLoaded() && MMODL->MODL.IsLoaded())
        {
        cSize = MMODL->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(MMODL->MODB.IsLoaded())
            TotSize += MMODL->MODB.GetSize() + 6;

        if(MMODL->MODT.IsLoaded())
            {
            cSize = MMODL->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(MICON0.IsLoaded())
        {
        cSize = MICON0.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(MICON1.IsLoaded())
        {
        cSize = MICON1.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(MICON2.IsLoaded())
        {
        cSize = MICON2.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(MICON3.IsLoaded())
        {
        cSize = MICON3.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(MICON4.IsLoaded())
        {
        cSize = MICON4.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(FMODL.IsLoaded() && FMODL->MODL.IsLoaded())
        {
        cSize = FMODL->MODL.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;

        if(FMODL->MODB.IsLoaded())
            TotSize += FMODL->MODB.GetSize() + 6;

        if(FMODL->MODT.IsLoaded())
            {
            cSize = FMODL->MODT.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        }

    if(FICON0.IsLoaded())
        {
        cSize = FICON0.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(FICON1.IsLoaded())
        {
        cSize = FICON1.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(FICON2.IsLoaded())
        {
        cSize = FICON2.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(FICON3.IsLoaded())
        {
        cSize = FICON3.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(FICON4.IsLoaded())
        {
        cSize = FICON4.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    if(HNAM.size())
        {
        cSize = (sizeof(UINT32) * (UINT32)HNAM.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize;
        }

    if(ENAM.size())
        {
        cSize = (sizeof(UINT32) * (UINT32)ENAM.size());
        if(cSize > 65535) cSize += 10;
        TotSize += cSize;
        }

    if(FGGS.IsLoaded())
        TotSize += FGGS.GetSize() + 6;

    if(FGGA.IsLoaded())
        TotSize += FGGA.GetSize() + 6;

    if(FGTS.IsLoaded())
        TotSize += FGTS.GetSize() + 6;

    if(SNAM.IsLoaded())
        TotSize += SNAM.GetSize() + 6;

    return TotSize;
    }

UINT32 RACERecord::GetType()
    {
    return 'ECAR';
    }


STRING RACERecord::GetStrType()
    {
    return "RACE";
    }

SINT32 RACERecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    SINT32 curNAM = -1;
    SINT32 curINDX = -1;
    UINT32 curPos = 0;
    FORMID curFormID = 0;
    ReqSubRecord<GENXNAM> *newXNAM = NULL;
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
            case 'OLPS':
                _readBuffer(&curFormID,buffer,subSize,curPos);
                SPLO.push_back(curFormID);
                break;
            case 'MANX':
                newXNAM = new ReqSubRecord<GENXNAM>;
                newXNAM->Read(buffer, subSize, curPos);
                XNAM.push_back(newXNAM);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            case 'MANV':
                if(VNAM.Read(buffer, subSize, curPos))
                    {
                    if(formID == VNAM.value.maleVoice)
                        VNAM.value.maleVoice = 0;
                    if(formID == VNAM.value.femaleVoice)
                        VNAM.value.femaleVoice = 0;
                    }
                break;
            case 'MAND':
                DNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANC':
                CNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANP':
                PNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANU':
                UNAM.Read(buffer, subSize, curPos);
                break;
            case 'RTTA':
                ATTR.Read(buffer, subSize, curPos);
                break;
            case '0MAN':
                curNAM = 0;
                curINDX = -1;
                break;
            case '1MAN':
                curNAM = 1;
                break;
            case 'XDNI':
                _readBuffer(&curINDX,buffer,subSize,curPos);
                break;
            case 'MANM':
                curNAM = 2;
                curINDX = -1;
                break;
            case 'MANF':
                curNAM = 3;
                curINDX = -1;
                break;
            case 'LDOM':
                switch(curNAM)
                    {
                    case 0:
                        switch(curINDX)
                            {
                            case 0:
                                MOD0.Load();
                                MOD0->MODL.Read(buffer, subSize, curPos);
                                break;
                            case 1:
                                MOD1.Load();
                                MOD1->MODL.Read(buffer, subSize, curPos);
                                break;
                            case 2:
                                MOD2.Load();
                                MOD2->MODL.Read(buffer, subSize, curPos);
                                break;
                            case 3:
                                MOD3.Load();
                                MOD3->MODL.Read(buffer, subSize, curPos);
                                break;
                            case 4:
                                MOD4.Load();
                                MOD4->MODL.Read(buffer, subSize, curPos);
                                break;
                            case 5:
                                MOD5.Load();
                                MOD5->MODL.Read(buffer, subSize, curPos);
                                break;
                            case 6:
                                MOD6.Load();
                                MOD6->MODL.Read(buffer, subSize, curPos);
                                break;
                            case 7:
                                MOD7.Load();
                                MOD7->MODL.Read(buffer, subSize, curPos);
                                break;
                            case 8:
                                MOD8.Load();
                                MOD8->MODL.Read(buffer, subSize, curPos);
                                break;
                            default:
                                //ERROR
                                //printf("FileName = %s\n", FileName);
                                printf("  RACE: %08X - Unexpected INDX %I under NAM%01I: %i\n", formID, curINDX, curNAM);
                                printf("  CurPos = %04x\n\n", curPos - 6);
                                curPos = recSize;
                                break;
                            }
                        break;
                    case 2:
                        MMODL.Load();
                        MMODL->MODL.Read(buffer, subSize, curPos);
                        break;
                    case 3:
                        FMODL.Load();
                        FMODL->MODL.Read(buffer, subSize, curPos);
                        break;
                    default:
                        //ERROR
                        //printf("FileName = %s\n", FileName);
                        printf("  RACE: %08X - Unexpected NAM%01I: %i\n", formID, curNAM);
                        printf("  CurPos = %04x\n\n", curPos - 6);
                        curPos = recSize;
                        break;
                    }
                break;
            case 'NOCI':
                switch(curNAM)
                    {
                    case 0:
                        switch(curINDX)
                            {
                            case 0:
                                MOD0.Load();
                                MOD0->ICON.Read(buffer, subSize, curPos);
                                break;
                            case 1:
                                MOD1.Load();
                                MOD1->ICON.Read(buffer, subSize, curPos);
                                break;
                            case 2:
                                MOD2.Load();
                                MOD2->ICON.Read(buffer, subSize, curPos);
                                break;
                            case 3:
                                MOD3.Load();
                                MOD3->ICON.Read(buffer, subSize, curPos);
                                break;
                            case 4:
                                MOD4.Load();
                                MOD4->ICON.Read(buffer, subSize, curPos);
                                break;
                            case 5:
                                MOD5.Load();
                                MOD5->ICON.Read(buffer, subSize, curPos);
                                break;
                            case 6:
                                MOD6.Load();
                                MOD6->ICON.Read(buffer, subSize, curPos);
                                break;
                            case 7:
                                MOD7.Load();
                                MOD7->ICON.Read(buffer, subSize, curPos);
                                break;
                            case 8:
                                MOD8.Load();
                                MOD8->ICON.Read(buffer, subSize, curPos);
                                break;
                            default:
                                //ERROR
                                //printf("FileName = %s\n", FileName);
                                printf("  RACE: %08X - Unexpected INDX %I under NAM%01I: %i\n", formID, curINDX, curNAM);
                                printf("  CurPos = %04x\n\n", curPos - 6);
                                curPos = recSize;
                                break;
                            }
                        break;
                    case 2:
                        switch(curINDX)
                            {
                            case 0:
                                MICON0.Read(buffer, subSize, curPos);
                                break;
                            case 1:
                                MICON1.Read(buffer, subSize, curPos);
                                break;
                            case 2:
                                MICON2.Read(buffer, subSize, curPos);
                                break;
                            case 3:
                                MICON3.Read(buffer, subSize, curPos);
                                break;
                            case 4:
                                MICON4.Read(buffer, subSize, curPos);
                                break;
                            default:
                                //ERROR
                                //printf("FileName = %s\n", FileName);
                                printf("  RACE: %08X - Unexpected ICON under NAM%01I: %i\n", formID, curNAM);
                                printf("  CurPos = %04x\n\n", curPos - 6);
                                curPos = recSize;
                                break;
                            }
                        break;
                    case 3:
                        switch(curINDX)
                            {
                            case 0:
                                FICON0.Read(buffer, subSize, curPos);
                                break;
                            case 1:
                                FICON1.Read(buffer, subSize, curPos);
                                break;
                            case 2:
                                FICON2.Read(buffer, subSize, curPos);
                                break;
                            case 3:
                                FICON3.Read(buffer, subSize, curPos);
                                break;
                            case 4:
                                FICON4.Read(buffer, subSize, curPos);
                                break;
                            default:
                                //ERROR
                                //printf("FileName = %s\n", FileName);
                                printf("  RACE: %08X - Unexpected ICON under NAM%01i: indx = %i\n", formID, curNAM, curINDX);
                                printf("  CurPos = %04x\n\n", curPos - 6);
                                curPos = recSize;
                                break;
                            }
                        break;
                    default:
                        //ERROR
                        //printf("FileName = %s\n", FileName);
                        printf("  RACE: %08X - Unexpected ICON under NAM%01i\n", formID, curNAM);
                        printf("  CurPos = %04x\n\n", curPos - 6);
                        curPos = recSize;
                        break;
                    }
                break;
            case 'BDOM':
                switch(curNAM)
                    {
                    case 0:
                        switch(curINDX)
                            {
                            case 0:
                                MOD0.Load();
                                MOD0->MODB.Read(buffer, subSize, curPos);
                                break;
                            case 1:
                                MOD1.Load();
                                MOD1->MODB.Read(buffer, subSize, curPos);
                                break;
                            case 2:
                                MOD2.Load();
                                MOD2->MODB.Read(buffer, subSize, curPos);
                                break;
                            case 3:
                                MOD3.Load();
                                MOD3->MODB.Read(buffer, subSize, curPos);
                                break;
                            case 4:
                                MOD4.Load();
                                MOD4->MODB.Read(buffer, subSize, curPos);
                                break;
                            case 5:
                                MOD5.Load();
                                MOD5->MODB.Read(buffer, subSize, curPos);
                                break;
                            case 6:
                                MOD6.Load();
                                MOD6->MODB.Read(buffer, subSize, curPos);
                                break;
                            case 7:
                                MOD7.Load();
                                MOD7->MODB.Read(buffer, subSize, curPos);
                                break;
                            case 8:
                                MOD8.Load();
                                MOD8->MODB.Read(buffer, subSize, curPos);
                                break;
                            default:
                                //ERROR
                                //printf("FileName = %s\n", FileName);
                                printf("  RACE: %08X - Unexpected INDX %I under NAM%01I: %i\n", formID, curINDX, curNAM);
                                printf("  CurPos = %04x\n\n", curPos - 6);
                                curPos = recSize;
                                break;
                            }
                        break;
                    case 2:
                        MMODL.Load();
                        MMODL->MODB.Read(buffer, subSize, curPos);
                        break;
                     case 3:
                        FMODL.Load();
                        FMODL->MODB.Read(buffer, subSize, curPos);
                        break;
                    default:
                        //ERROR
                        //printf("FileName = %s\n", FileName);
                        printf("  RACE: %08X - Unexpected MODB under NAM%01I: %i\n", formID, curNAM);
                        printf("  CurPos = %04x\n\n", curPos - 6);
                        curPos = recSize;
                        break;
                    }
                break;
            case 'TDOM':
                switch(curNAM)
                    {
                    case 0:
                        switch(curINDX)
                            {
                            case 0:
                                MOD0.Load();
                                MOD0->MODT.Read(buffer, subSize, curPos);
                                break;
                            case 1:
                                MOD1.Load();
                                MOD1->MODT.Read(buffer, subSize, curPos);
                                break;
                            case 2:
                                MOD2.Load();
                                MOD2->MODT.Read(buffer, subSize, curPos);
                                break;
                            case 3:
                                MOD3.Load();
                                MOD3->MODT.Read(buffer, subSize, curPos);
                                break;
                            case 4:
                                MOD4.Load();
                                MOD4->MODT.Read(buffer, subSize, curPos);
                                break;
                            case 5:
                                MOD5.Load();
                                MOD5->MODT.Read(buffer, subSize, curPos);
                                break;
                            case 6:
                                MOD6.Load();
                                MOD6->MODT.Read(buffer, subSize, curPos);
                                break;
                            case 7:
                                MOD7.Load();
                                MOD7->MODT.Read(buffer, subSize, curPos);
                                break;
                            case 8:
                                MOD8.Load();
                                MOD8->MODT.Read(buffer, subSize, curPos);
                                break;
                            default:
                                //ERROR
                                //printf("FileName = %s\n", FileName);
                                printf("  RACE: %08X - Unexpected INDX %I under NAM%01I: %i\n", formID, curINDX, curNAM);
                                printf("  CurPos = %04x\n\n", curPos - 6);
                                curPos = recSize;
                                break;
                            }
                        break;
                    case 2:
                        MMODL.Load();
                        MMODL->MODT.Read(buffer, subSize, curPos);
                        break;
                    case 3:
                        FMODL.Load();
                        FMODL->MODT.Read(buffer, subSize, curPos);
                        break;
                    default:
                        //ERROR
                        //printf("FileName = %s\n", FileName);
                        printf("  RACE: %08X - Unexpected MODT under NAM%01I: %i\n", formID, curNAM);
                        printf("  CurPos = %04x\n\n", curPos - 6);
                        curPos = recSize;
                        break;
                    }
                break;
            case 'MANH':
                if(subSize % sizeof(UINT32) == 0)
                    {
                    if(subSize == 0)
                        break;
                    HNAM.resize(subSize / sizeof(UINT32));
                    _readBuffer(&HNAM[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized HNAM size: %i\n", subSize);
                    curPos += subSize;
                    }
                break;
            case 'MANE':
                if(subSize % sizeof(UINT32) == 0)
                    {
                    if(subSize == 0)
                        break;
                    ENAM.resize(subSize / sizeof(UINT32));
                    _readBuffer(&ENAM[0], buffer, subSize, curPos);
                    }
                else
                    {
                    printf("  Unrecognized ENAM size: %i\n", subSize);
                    curPos += subSize;
                    }
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
            case 'MANS':
                SNAM.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  RACE: Unknown subType = %04X\n", subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 RACERecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    FULL.Unload();
    DESC.Unload();

    SPLO.clear();

    for(UINT32 x = 0; x < XNAM.size(); x++)
        delete XNAM[x];
    XNAM.clear();

    DATA.Unload();

    VNAM.Unload();
    DNAM.Unload();

    CNAM.Unload();
    PNAM.Unload();
    UNAM.Unload();
    ATTR.Unload();

    MOD0.Unload();
    MOD1.Unload();
    MOD2.Unload();
    MOD3.Unload();
    MOD4.Unload();
    MOD5.Unload();
    MOD6.Unload();
    MOD7.Unload();
    MOD8.Unload();

    MMODL.Unload();

    MICON0.Unload();
    MICON1.Unload();
    MICON2.Unload();
    MICON3.Unload();
    MICON4.Unload();

    FMODL.Unload();

    FICON0.Unload();
    FICON1.Unload();
    FICON2.Unload();
    FICON3.Unload();
    FICON4.Unload();
    HNAM.clear();
    ENAM.clear();
    FGGS.Unload();
    FGGA.Unload();
    FGTS.Unload();
    SNAM.Unload();
    return 1;
    }

SINT32 RACERecord::WriteRecord(_FileHandler &SaveHandler)
    {
    UINT32 curINDX = 0;
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    if(FULL.IsLoaded())
        SaveHandler.writeSubRecord('LLUF', FULL.value, FULL.GetSize());
    if(DESC.IsLoaded())
        SaveHandler.writeSubRecord('CSED', DESC.value, DESC.GetSize());

    for(UINT32 p = 0; p < SPLO.size(); p++)
        SaveHandler.writeSubRecord('OLPS', &SPLO[p], sizeof(UINT32));

    for(UINT32 p = 0; p < XNAM.size(); p++)
        if(XNAM[p]->IsLoaded())
            SaveHandler.writeSubRecord('MANX', &XNAM[p]->value, XNAM[p]->GetSize());

    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());
    if(VNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANV', &VNAM.value, VNAM.GetSize());
    if(DNAM.IsLoaded())
        SaveHandler.writeSubRecord('MAND', &DNAM.value, DNAM.GetSize());
    if(CNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANC', &CNAM.value, CNAM.GetSize());
    if(PNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANP', PNAM.value, PNAM.GetSize());
    if(UNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANU', UNAM.value, UNAM.GetSize());
    if(ATTR.IsLoaded())
        SaveHandler.writeSubRecord('RTTA', &ATTR.value, ATTR.GetSize());

    SaveHandler.writeSubRecord('0MAN', NULL, 0);

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD0.IsLoaded() && MOD0->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD0->MODL.value, MOD0->MODL.GetSize());
        if(MOD0->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD0->MODB.value, MOD0->MODB.GetSize());
        if(MOD0->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD0->MODT.value, MOD0->MODT.GetSize());
        if(MOD0->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD0->ICON.value, MOD0->ICON.GetSize());
        }
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD1.IsLoaded() && MOD1->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD1->MODL.value, MOD1->MODL.GetSize());
        if(MOD1->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD1->MODB.value, MOD1->MODB.GetSize());
        if(MOD1->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD1->MODT.value, MOD1->MODT.GetSize());
        if(MOD1->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD1->ICON.value, MOD1->ICON.GetSize());
        }
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD2.IsLoaded() && MOD2->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD2->MODL.value, MOD2->MODL.GetSize());
        if(MOD2->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD2->MODB.value, MOD2->MODB.GetSize());
        if(MOD2->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD2->MODT.value, MOD2->MODT.GetSize());
        if(MOD2->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD2->ICON.value, MOD2->ICON.GetSize());
        }
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD3.IsLoaded() && MOD3->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD3->MODL.value, MOD3->MODL.GetSize());
        if(MOD3->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD3->MODB.value, MOD3->MODB.GetSize());
        if(MOD3->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD3->MODT.value, MOD3->MODT.GetSize());
        if(MOD3->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD3->ICON.value, MOD3->ICON.GetSize());
        }
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD4.IsLoaded() && MOD4->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD4->MODL.value, MOD4->MODL.GetSize());
        if(MOD4->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD4->MODB.value, MOD4->MODB.GetSize());
        if(MOD4->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD4->MODT.value, MOD4->MODT.GetSize());
        if(MOD4->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD4->ICON.value, MOD4->ICON.GetSize());
        }
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD5.IsLoaded() && MOD5->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD5->MODL.value, MOD5->MODL.GetSize());
        if(MOD5->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD5->MODB.value, MOD5->MODB.GetSize());
        if(MOD5->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD5->MODT.value, MOD5->MODT.GetSize());
        if(MOD5->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD5->ICON.value, MOD5->ICON.GetSize());
        }
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD6.IsLoaded() && MOD6->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD6->MODL.value, MOD6->MODL.GetSize());
        if(MOD6->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD6->MODB.value, MOD6->MODB.GetSize());
        if(MOD6->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD6->MODT.value, MOD6->MODT.GetSize());
        if(MOD6->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD6->ICON.value, MOD6->ICON.GetSize());
        }
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD7.IsLoaded() && MOD7->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD7->MODL.value, MOD7->MODL.GetSize());
        if(MOD7->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD7->MODB.value, MOD7->MODB.GetSize());
        if(MOD7->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD7->MODT.value, MOD7->MODT.GetSize());
        if(MOD7->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD7->ICON.value, MOD7->ICON.GetSize());
        }
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MOD8.IsLoaded() && MOD8->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MOD8->MODL.value, MOD8->MODL.GetSize());
        if(MOD8->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MOD8->MODB.value, MOD8->MODB.GetSize());
        if(MOD8->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MOD8->MODT.value, MOD8->MODT.GetSize());
        if(MOD8->ICON.IsLoaded())
            SaveHandler.writeSubRecord('NOCI', MOD8->ICON.value, MOD8->ICON.GetSize());
        }

    curINDX = 0;
    SaveHandler.writeSubRecord('1MAN', NULL, 0);
    SaveHandler.writeSubRecord('MANM', NULL, 0);
    if(MMODL.IsLoaded() && MMODL->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', MMODL->MODL.value, MMODL->MODL.GetSize());
        if(MMODL->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &MMODL->MODB.value, MMODL->MODB.GetSize());
        if(MMODL->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', MMODL->MODT.value, MMODL->MODT.GetSize());
        }

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MICON0.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', MICON0.value, MICON0.GetSize());
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MICON1.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', MICON1.value, MICON1.GetSize());
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MICON2.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', MICON2.value, MICON2.GetSize());
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MICON3.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', MICON3.value, MICON3.GetSize());
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(MICON4.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', MICON4.value, MICON4.GetSize());

    curINDX = 0;
    SaveHandler.writeSubRecord('MANF', NULL, 0);
    if(FMODL.IsLoaded() && FMODL->MODL.IsLoaded())
        {
        SaveHandler.writeSubRecord('LDOM', FMODL->MODL.value, FMODL->MODL.GetSize());
        if(FMODL->MODB.IsLoaded())
            SaveHandler.writeSubRecord('BDOM', &FMODL->MODB.value, FMODL->MODB.GetSize());
        if(FMODL->MODT.IsLoaded())
            SaveHandler.writeSubRecord('TDOM', FMODL->MODT.value, FMODL->MODT.GetSize());
        }

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(FICON0.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', FICON0.value, FICON0.GetSize());
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(FICON1.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', FICON1.value, FICON1.GetSize());
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(FICON2.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', FICON2.value, FICON2.GetSize());
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(FICON3.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', FICON3.value, FICON3.GetSize());
    curINDX++;

    SaveHandler.writeSubRecord('XDNI', &curINDX, 4);
    if(FICON4.IsLoaded())
        SaveHandler.writeSubRecord('NOCI', FICON4.value, FICON4.GetSize());

    if(HNAM.size())
        SaveHandler.writeSubRecord('MANH', &HNAM[0], (UINT32)HNAM.size() * sizeof(UINT32));
    else
        SaveHandler.writeSubRecord('MANH', NULL, 0);

    if(ENAM.size())
        SaveHandler.writeSubRecord('MANE', &ENAM[0], (UINT32)ENAM.size() * sizeof(UINT32));
    else
        SaveHandler.writeSubRecord('MANE', NULL, 0);

    if(FGGS.IsLoaded())
        SaveHandler.writeSubRecord('SGGF', FGGS.value, FGGS.GetSize());
    if(FGGA.IsLoaded())
        SaveHandler.writeSubRecord('AGGF', FGGA.value, FGGA.GetSize());
    if(FGTS.IsLoaded())
        SaveHandler.writeSubRecord('STGF', FGTS.value, FGTS.GetSize());

    if(SNAM.IsLoaded())
        SaveHandler.writeSubRecord('MANS', &SNAM.value, SNAM.GetSize());

    return -1;
    }

bool RACERecord::operator ==(const RACERecord &other) const
    {
    if(EDID.equalsi(other.EDID) &&
        FULL.equals(other.FULL) &&
        DESC.equals(other.DESC) &&
        DATA == other.DATA &&
        VNAM == other.VNAM &&
        DNAM == other.DNAM &&
        CNAM == other.CNAM &&
        PNAM == other.PNAM &&
        UNAM == other.UNAM &&
        ATTR == other.ATTR &&
        MOD0 == other.MOD0 &&
        MOD1 == other.MOD1 &&
        MOD2 == other.MOD2 &&
        MOD3 == other.MOD3 &&
        MOD4 == other.MOD4 &&
        MOD5 == other.MOD5 &&
        MOD6 == other.MOD6 &&
        MOD7 == other.MOD7 &&
        MOD8 == other.MOD8 &&
        MMODL == other.MMODL &&
        MICON0.equalsi(other.MICON0) &&
        MICON1.equalsi(other.MICON1) &&
        MICON2.equalsi(other.MICON2) &&
        MICON3.equalsi(other.MICON3) &&
        MICON4.equalsi(other.MICON4) &&
        FMODL == other.FMODL &&
        FICON0.equalsi(other.FICON0) &&
        FICON1.equalsi(other.FICON1) &&
        FICON2.equalsi(other.FICON2) &&
        FICON3.equalsi(other.FICON3) &&
        FICON4.equalsi(other.FICON4) &&
        FGGS == other.FGGS &&
        FGGA == other.FGGA &&
        FGTS == other.FGTS &&
        SNAM == other.SNAM &&
        SPLO.size() == other.SPLO.size() &&
        XNAM.size() == other.XNAM.size() &&
        HNAM.size() == other.HNAM.size() &&
        ENAM.size() == other.ENAM.size())
        {
        //Record order doesn't matter on spells, so equality testing isn't easy
        //The proper solution would be to check each spell against every other spell to see if there's a one-to-one match
        //Perhaps using a disjoint set
        //Fix-up later
        for(UINT32 x = 0; x < SPLO.size(); ++x)
		{
			bool good = false;
			for(UINT32 y = 0; y < SPLO.size(); ++y)
			{
				if (SPLO[x] == other.SPLO[y])
				{
					good = true;
					break;
				}
			}
			if (!good)
				return false;
		}

        //Not sure if record order matters on relations, so equality testing is a guess
        //Fix-up later
        for(UINT32 x = 0; x < XNAM.size(); ++x)
            if(*XNAM[x] != *other.XNAM[x])
                return false;

        //Record order doesn't matter on hairs, so equality testing isn't easy
        //The proper solution would be to check each spell against every other spell to see if there's a one-to-one match
        //Perhaps using a disjoint set
        //Fix-up later
        for(UINT32 x = 0; x < HNAM.size(); ++x)
		{
			bool good = false;
			for(UINT32 y = 0; y < HNAM.size(); ++y)
			{
				if(HNAM[x] == other.HNAM[y])
				{
					good = true;
					break;
				}
			}
			if (!good)
				return false;
		}

        //Record order doesn't matter on eyes, so equality testing isn't easy
        //The proper solution would be to check each spell against every other spell to see if there's a one-to-one match
        //Perhaps using a disjoint set
        //Fix-up later
        for(UINT32 x = 0; x < ENAM.size(); ++x)
		{
			bool good = false;
			for(UINT32 y = 0; y < ENAM.size(); ++y)
			{
				if(ENAM[x] == other.ENAM[y])
				{
					good = true;
					break;
				}
			}
			if (!good)
				return false;
		}

        return true;
        }

    return false;
    }

bool RACERecord::operator !=(const RACERecord &other) const
    {
    return !(*this == other);
    }