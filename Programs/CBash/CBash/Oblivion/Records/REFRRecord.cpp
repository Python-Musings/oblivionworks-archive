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
#include "REFRRecord.h"

REFRRecord::REFRXTEL::REFRXTEL():
    destinationFid(0)
    {
    //
    }

REFRRecord::REFRXTEL::~REFRXTEL()
    {
    //
    }

bool REFRRecord::REFRXTEL::operator ==(const REFRXTEL &other) const
    {
    return (destinationFid == other.destinationFid &&
            destination == other.destination);
    }

bool REFRRecord::REFRXTEL::operator !=(const REFRXTEL &other) const
    {
    return !(*this == other);
    }

REFRRecord::REFRXLOC::REFRXLOC():
    level(0xFF), //Level actually defaults to 0, but this makes it write out if set to 0
    key(0),
    flags(0)
    {
    memset(&unused1, 0x00, 3);
    memset(&unused2, 0x00, 4);
    memset(&unused3, 0x00, 3);
    }

REFRRecord::REFRXLOC::~REFRXLOC()
    {
    //
    }

bool REFRRecord::REFRXLOC::operator ==(const REFRXLOC &other) const
    {
    return (level == other.level &&
            key == other.key &&
            flags == other.flags);
    }

bool REFRRecord::REFRXLOC::operator !=(const REFRXLOC &other) const
    {
    return !(*this == other);
    }

REFRRecord::REFRXSED::REFRXSED():
    seed(0),
    isOffset(true)
    {
    //
    }

REFRRecord::REFRXSED::~REFRXSED()
    {
    //
    }

bool REFRRecord::REFRXSED::operator ==(const REFRXSED &other) const
    {
    if(!isOffset)
        {
        if(!other.isOffset)
            return (seed == other.seed);
        }
    else if(other.isOffset)
        return offset == other.offset;
    return false;
    }

bool REFRRecord::REFRXSED::operator !=(const REFRXSED &other) const
    {
    return !(*this == other);
    }

REFRRecord::REFRXCHG::REFRXCHG():
    charge(0.0f)
    {
    //
    }

REFRRecord::REFRXCHG::~REFRXCHG()
    {
    //
    }

bool REFRRecord::REFRXCHG::operator ==(const REFRXCHG &other) const
    {
    return (AlmostEqual(charge,other.charge,2));
    }

bool REFRRecord::REFRXCHG::operator !=(const REFRXCHG &other) const
    {
    return !(*this == other);
    }

REFRRecord::REFRXHLT::REFRXHLT():
    health(0)
    {
    //
    }

REFRRecord::REFRXHLT::~REFRXHLT()
    {
    //
    }

bool REFRRecord::REFRXHLT::operator ==(const REFRXHLT &other) const
    {
    return (health == other.health);
    }

bool REFRRecord::REFRXHLT::operator !=(const REFRXHLT &other) const
    {
    return !(*this == other);
    }

REFRRecord::REFRXLCM::REFRXLCM():
    levelMod(0)
    {
    //
    }

REFRRecord::REFRXLCM::~REFRXLCM()
    {
    //
    }

bool REFRRecord::REFRXLCM::operator ==(const REFRXLCM &other) const
    {
    return (levelMod == other.levelMod);
    }

bool REFRRecord::REFRXLCM::operator !=(const REFRXLCM &other) const
    {
    return !(*this == other);
    }

REFRRecord::REFRXCNT::REFRXCNT():
    count(0)
    {
    //
    }

REFRRecord::REFRXCNT::~REFRXCNT()
    {
    //
    }

bool REFRRecord::REFRXCNT::operator ==(const REFRXCNT &other) const
    {
    return (count == other.count);
    }

bool REFRRecord::REFRXCNT::operator !=(const REFRXCNT &other) const
    {
    return !(*this == other);
    }

REFRRecord::REFRTNAM::REFRTNAM():
    markerType(0),
    unused1(0)
    {
    //
    }

REFRRecord::REFRTNAM::~REFRTNAM()
    {
    //
    }

bool REFRRecord::REFRTNAM::operator ==(const REFRTNAM &other) const
    {
    return (markerType == other.markerType);
    }

bool REFRRecord::REFRTNAM::operator !=(const REFRTNAM &other) const
    {
    return !(*this == other);
    }

bool REFRRecord::REFRMAPMARKER::operator ==(const REFRMAPMARKER &other) const
    {
    return (FNAM == other.FNAM &&
            FULL.equals(other.FULL) &&
            TNAM == other.TNAM);
    }

bool REFRRecord::REFRMAPMARKER::operator !=(const REFRMAPMARKER &other) const
    {
    return !(*this == other);
    }

REFRRecord::REFRRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

REFRRecord::REFRRecord(REFRRecord *srcRecord):
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
    NAME = srcRecord->NAME;
    XTEL = srcRecord->XTEL;
    XLOC = srcRecord->XLOC;
    if(srcRecord->Ownership.IsLoaded())
        {
        Ownership.Load();
        Ownership->XOWN = srcRecord->Ownership->XOWN;
        Ownership->XRNK = srcRecord->Ownership->XRNK;
        Ownership->XGLB = srcRecord->Ownership->XGLB;
        }
    XESP = srcRecord->XESP;
    XTRG = srcRecord->XTRG;
    XSED = srcRecord->XSED;
    XLOD = srcRecord->XLOD;
    XCHG = srcRecord->XCHG;
    XHLT = srcRecord->XHLT;
    if(srcRecord->XPCI.IsLoaded())
        {
        XPCI.Load();
        XPCI->XPCI = srcRecord->XPCI->XPCI;
        XPCI->FULL = srcRecord->XPCI->FULL;
        }
    XLCM = srcRecord->XLCM;
    XRTM = srcRecord->XRTM;
    XACT = srcRecord->XACT;
    XCNT = srcRecord->XCNT;
    if(srcRecord->Marker.IsLoaded())
        {
        Marker.Load();
        Marker->FNAM = srcRecord->Marker->FNAM;
        Marker->FULL = srcRecord->Marker->FULL;
        Marker->TNAM = srcRecord->Marker->TNAM;
        }
    if(srcRecord->IsOpenByDefault())//bool ONAM; //Open by Default, empty marker, written whenever fOpenByDefault is true
        IsOpenByDefault(true);
    XSCL = srcRecord->XSCL;
    XSOL = srcRecord->XSOL;
    DATA = srcRecord->DATA;
    }

REFRRecord::~REFRRecord()
    {
    //
    }

bool REFRRecord::VisitFormIDs(FormIDOp &op)
    {
    if(!IsLoaded())
        return false;

    op.Accept(NAME.value.fid);
    if(XTEL.IsLoaded())
        op.Accept(XTEL->destinationFid);
    if(XLOC.IsLoaded())
        op.Accept(XLOC->key);
    if(Ownership.IsLoaded())
        {
        if(Ownership->XOWN.IsLoaded())
            op.Accept(Ownership->XOWN.value.fid);
        if(Ownership->XGLB.IsLoaded())
            op.Accept(Ownership->XGLB->fid);
        }
    if(XESP.IsLoaded())
        op.Accept(XESP->parent);
    if(XTRG.IsLoaded())
        op.Accept(XTRG->fid);
    if(XPCI.IsLoaded() && XPCI->XPCI.IsLoaded())
        op.Accept(XPCI->XPCI->fid);
    if(XRTM.IsLoaded())
        op.Accept(XRTM->fid);

    return op.Stop();
    }

bool REFRRecord::IsOppositeParent()
    {
    if(!XESP.IsLoaded()) return false;
    return (XESP->flags & fIsOppositeParent) != 0;
    }

void REFRRecord::IsOppositeParent(bool value)
    {
    if(!XESP.IsLoaded()) return;
    if(value)
        XESP->flags |= fIsOppositeParent;
    else
        XESP->flags &= ~fIsOppositeParent;
    }

bool REFRRecord::IsParentFlagMask(UINT8 Mask, bool Exact)
    {
    if(!XESP.IsLoaded()) return false;
    return Exact ? ((XESP->flags & Mask) == Mask) : ((XESP->flags & Mask) != 0);
    }

void REFRRecord::SetParentFlagMask(UINT8 Mask)
    {
    XESP.Load();
    XESP->flags = Mask;
    }

bool REFRRecord::IsVisible()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->FNAM.value.flags & fVisible) != 0;
    }

void REFRRecord::IsVisible(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->FNAM.value.flags |= fVisible;
    else
        Marker->FNAM.value.flags &= ~fVisible;
    }

bool REFRRecord::IsCanTravelTo()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->FNAM.value.flags & fCanTravelTo) != 0;
    }

void REFRRecord::IsCanTravelTo(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->FNAM.value.flags |= fCanTravelTo;
    else
        Marker->FNAM.value.flags &= ~fCanTravelTo;
    }

bool REFRRecord::IsMapFlagMask(UINT8 Mask, bool Exact)
    {
    if(!Marker.IsLoaded()) return false;
    return Exact ? ((Marker->FNAM.value.flags & Mask) == Mask) : ((Marker->FNAM.value.flags & Mask) != 0);
    }

void REFRRecord::SetMapFlagMask(UINT8 Mask)
    {
    Marker.Load();
    Marker->FNAM.value.flags = Mask;
    }

bool REFRRecord::IsUseDefault()
    {
    if(!XACT.IsLoaded()) return false;
    return (XACT->flags & fUseDefault) != 0;
    }

void REFRRecord::IsUseDefault(bool value)
    {
    if(!XACT.IsLoaded()) return;
    if(value)
        XACT->flags |= fUseDefault;
    else
        XACT->flags &= ~fUseDefault;
    }

bool REFRRecord::IsActivate()
    {
    if(!XACT.IsLoaded()) return false;
    return (XACT->flags & fActivate) != 0;
    }

void REFRRecord::IsActivate(bool value)
    {
    if(!XACT.IsLoaded()) return;
    if(value)
        XACT->flags |= fActivate;
    else
        XACT->flags &= ~fActivate;
    }

bool REFRRecord::IsOpen()
    {
    if(!XACT.IsLoaded()) return false;
    return (XACT->flags & fOpen) != 0;
    }

void REFRRecord::IsOpen(bool value)
    {
    if(!XACT.IsLoaded()) return;
    if(value)
        XACT->flags |= fOpen;
    else
        XACT->flags &= ~fOpen;
    }

bool REFRRecord::IsOpenByDefault()
    {
    if(!XACT.IsLoaded()) return false;
    return (XACT->flags & fOpenByDefault) != 0;
    }

void REFRRecord::IsOpenByDefault(bool value)
    {
    if(!XACT.IsLoaded()) return;
    if(value)
        XACT->flags |= fOpenByDefault;
    else
        XACT->flags &= ~fOpenByDefault;
    }

bool REFRRecord::IsActionFlagMask(UINT32 Mask, bool Exact)
    {
    if(!XACT.IsLoaded()) return false;
    return Exact ? ((XACT->flags & Mask) == Mask) : ((XACT->flags & Mask) != 0);
    }

void REFRRecord::SetActionFlagMask(UINT32 Mask)
    {
    XACT.Load();
    XACT->flags = Mask;
    }

bool REFRRecord::IsLeveledLock()
    {
    if(!XLOC.IsLoaded()) return false;
    return (XLOC->flags & fLeveledLock) != 0;
    }

void REFRRecord::IsLeveledLock(bool value)
    {
    if(!XLOC.IsLoaded()) return;
    if(value)
        XLOC->flags |= fLeveledLock;
    else
        XLOC->flags &= ~fLeveledLock;
    }

bool REFRRecord::IsLockFlagMask(UINT8 Mask, bool Exact)
    {
    if(!XLOC.IsLoaded()) return false;
    return Exact ? ((XLOC->flags & Mask) == Mask) : ((XLOC->flags & Mask) != 0);
    }

void REFRRecord::SetLockFlagMask(UINT8 Mask)
    {
    XLOC.Load();
    XLOC->flags = Mask;
    }

bool REFRRecord::IsNoMarker()
    {
    if(!Marker.IsLoaded()) return true;
    return (Marker->TNAM.value.markerType == eMarkerNone);
    }

void REFRRecord::IsNoMarker(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eMarkerNone;
    else if(IsNoMarker())
        Marker->TNAM.value.markerType = eCamp;
    }

bool REFRRecord::IsCamp()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eCamp);
    }

void REFRRecord::IsCamp(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eCamp;
    else if(IsCamp())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsCave()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eCave);
    }

void REFRRecord::IsCave(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eCave;
    else if(IsCave())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsCity()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eCity);
    }

void REFRRecord::IsCity(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eCity;
    else if(IsCity())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsElvenRuin()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eElvenRuin);
    }

void REFRRecord::IsElvenRuin(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eElvenRuin;
    else if(IsElvenRuin())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsFortRuin()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eFortRuin);
    }

void REFRRecord::IsFortRuin(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eFortRuin;
    else if(IsFortRuin())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsMine()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eMine);
    }

void REFRRecord::IsMine(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eMine;
    else if(IsMine())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsLandmark()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eLandmark);
    }

void REFRRecord::IsLandmark(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eLandmark;
    else if(IsLandmark())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsTavern()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eTavern);
    }

void REFRRecord::IsTavern(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eTavern;
    else if(IsTavern())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsSettlement()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eSettlement);
    }

void REFRRecord::IsSettlement(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eSettlement;
    else if(IsSettlement())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsDaedricShrine()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eDaedricShrine);
    }

void REFRRecord::IsDaedricShrine(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eDaedricShrine;
    else if(IsDaedricShrine())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsOblivionGate()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eOblivionGate);
    }

void REFRRecord::IsOblivionGate(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eOblivionGate;
    else if(IsOblivionGate())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsUnknownDoorIcon()
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == eUnknownDoorIcon);
    }

void REFRRecord::IsUnknownDoorIcon(bool value)
    {
    if(!Marker.IsLoaded()) return;
    if(value)
        Marker->TNAM.value.markerType = eUnknownDoorIcon;
    else if(IsUnknownDoorIcon())
        Marker->TNAM.value.markerType = eMarkerNone;
    }

bool REFRRecord::IsMarkerType(UINT8 Type)
    {
    if(!Marker.IsLoaded()) return false;
    return (Marker->TNAM.value.markerType == Type);
    }

void REFRRecord::SetMarkerType(UINT8 Type)
    {
    Marker.Load();
    Marker->TNAM.value.markerType = Type;
    }

bool REFRRecord::IsNoSoul()
    {
    if(!XSOL.IsLoaded()) return true;
    return (XSOL->flags == eNone);
    }

void REFRRecord::IsNoSoul(bool value)
    {
    if(!XSOL.IsLoaded()) return;
    if(value)
        XSOL->flags = eNone;
    else if(IsNoSoul())
        XSOL->flags = ePetty;
    }

bool REFRRecord::IsPettySoul()
    {
    if(!XSOL.IsLoaded()) return false;
    return (XSOL->flags == ePetty);
    }

void REFRRecord::IsPettySoul(bool value)
    {
    if(!XSOL.IsLoaded()) return;
    if(value)
        XSOL->flags = ePetty;
    else if(IsPettySoul())
        XSOL->flags = eNone;
    }

bool REFRRecord::IsLesserSoul()
    {
    if(!XSOL.IsLoaded()) return false;
    return (XSOL->flags == eLesser);
    }

void REFRRecord::IsLesserSoul(bool value)
    {
    if(!XSOL.IsLoaded()) return;
    if(value)
        XSOL->flags = eLesser;
    else if(IsLesserSoul())
        XSOL->flags = eNone;
    }

bool REFRRecord::IsCommonSoul()
    {
    if(!XSOL.IsLoaded()) return false;
    return (XSOL->flags == eCommon);
    }

void REFRRecord::IsCommonSoul(bool value)
    {
    if(!XSOL.IsLoaded()) return;
    if(value)
        XSOL->flags = eCommon;
    else if(IsCommonSoul())
        XSOL->flags = eNone;
    }

bool REFRRecord::IsGreaterSoul()
    {
    if(!XSOL.IsLoaded()) return false;
    return (XSOL->flags == eGreater);
    }

void REFRRecord::IsGreaterSoul(bool value)
    {
    if(!XSOL.IsLoaded()) return;
    if(value)
        XSOL->flags = eGreater;
    else if(IsGreaterSoul())
        XSOL->flags = eNone;
    }

bool REFRRecord::IsGrandSoul()
    {
    if(!XSOL.IsLoaded()) return false;
    return (XSOL->flags == eGrand);
    }

void REFRRecord::IsGrandSoul(bool value)
    {
    if(!XSOL.IsLoaded()) return;
    if(value)
        XSOL->flags = eGrand;
    else if(IsGrandSoul())
        XSOL->flags = eNone;
    }

bool REFRRecord::IsSoul(UINT8 Type)
    {
    if(!XSOL.IsLoaded()) return false;
    return (XSOL->flags == Type);
    }

void REFRRecord::SetSoul(UINT8 Type)
    {
    XSOL.Load();
    XSOL->flags = Type;
    }

UINT32 REFRRecord::GetSize(bool forceCalc)
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

    if(NAME.IsLoaded())
        TotSize += NAME.GetSize() + 6;

    if(XTEL.IsLoaded())
        TotSize += XTEL.GetSize() + 6;

    if(XLOC.IsLoaded())
        TotSize += XLOC.GetSize() + 6;

    if(Ownership.IsLoaded() && Ownership->XOWN.IsLoaded())
        {
        TotSize += Ownership->XOWN.GetSize() + 6;
        if(Ownership->XRNK.IsLoaded())
            TotSize += Ownership->XRNK.GetSize() + 6;
        if(Ownership->XGLB.IsLoaded())
            TotSize += Ownership->XGLB.GetSize() + 6;
        }

    if(XESP.IsLoaded())
        TotSize += XESP.GetSize() + 6;

    if(XTRG.IsLoaded())
        TotSize += XTRG.GetSize() + 6;

    if(XSED.IsLoaded())
        if(XSED->isOffset)
            TotSize += 7;
        else TotSize += 10;

    if(XLOD.IsLoaded())
        TotSize += XLOD.GetSize() + 6;

    if(XCHG.IsLoaded())
        TotSize += XCHG.GetSize() + 6;

    if(XHLT.IsLoaded())
        TotSize += XHLT.GetSize() + 6;

    if(XPCI.IsLoaded() && XPCI->XPCI.IsLoaded())
        {
        TotSize += XPCI->XPCI.GetSize() + 6;

        if(XPCI->FULL.IsLoaded())
            {
            cSize = XPCI->FULL.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }
        else TotSize += 7;
        }

    if(XLCM.IsLoaded())
        TotSize += XLCM.GetSize() + 6;

    if(XRTM.IsLoaded())
        TotSize += XRTM.GetSize() + 6;

    if(XACT.IsLoaded())
        TotSize += XACT.GetSize() + 6;

    if(XCNT.IsLoaded())
        TotSize += XCNT.GetSize() + 6;

    if(Marker.IsLoaded())
        {
        TotSize += 6;

        if(Marker->FNAM.IsLoaded())
            TotSize += Marker->FNAM.GetSize() + 6;

        if(Marker->FULL.IsLoaded())
            {
            cSize = Marker->FULL.GetSize();
            if(cSize > 65535) cSize += 10;
            TotSize += cSize += 6;
            }

        if(Marker->TNAM.IsLoaded())
            TotSize += Marker->TNAM.GetSize() + 6;
        }

    if(IsOpenByDefault()) //ONAM
        TotSize += 6;

    if(XSCL.IsLoaded())
        TotSize += XSCL.GetSize() + 6;

    if(XSOL.IsLoaded())
        TotSize += XSOL.GetSize() + 6;

    if(DATA.IsLoaded())
        TotSize += DATA.GetSize() + 6;

    return TotSize;
    }

UINT32 REFRRecord::GetType()
    {
    return 'RFER';
    }

STRING REFRRecord::GetStrType()
    {
    return "REFR";
    }

UINT32 REFRRecord::GetParentType()
    {
    return 'LLEC';
    }

SINT32 REFRRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    UINT32 lastRecord = 0;
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
            case 'EMAN':
                NAME.Read(buffer, subSize, curPos);
                break;
            case 'LETX':
                XTEL.Read(buffer, subSize, curPos);
                break;
            case 'COLX':
                switch(subSize)
                    {
                    case 12: //unused2 is absent, so shift the values read into it
                        XLOC.Read(buffer, subSize, curPos);
                        XLOC->flags = XLOC->unused2[0];
                        XLOC->unused3[0] = XLOC->unused2[1];
                        XLOC->unused3[1] = XLOC->unused2[2];
                        XLOC->unused3[2] = XLOC->unused2[3];
                        memset(&XLOC->unused2[0], 0x00, 4);
                        break;
                    default:
                        XLOC.Read(buffer, subSize, curPos);
                        break;
                    }
                break;
            case 'NWOX':
                Ownership.Load();
                Ownership->XOWN.Read(buffer, subSize, curPos);
                break;
            case 'KNRX':
                Ownership.Load();
                Ownership->XRNK.Read(buffer, subSize, curPos);
                break;
            case 'BLGX':
                Ownership.Load();
                Ownership->XGLB.Read(buffer, subSize, curPos);
                break;
            case 'PSEX':
                XESP.Read(buffer, subSize, curPos);
                break;
            case 'GRTX':
                XTRG.Read(buffer, subSize, curPos);
                break;
            case 'DESX':
                switch(subSize)
                    {
                    case 1:
                        //if it's a single byte then it's an offset into the list of seed values in the TREE record
                        XSED.Load();
                        XSED->isOffset = true;
                        //XSED.size = 1;
                        _readBuffer(&XSED->offset, buffer, 1, curPos);
                        break;
                    case 4:
                        //if it's 4 byte it's the seed value directly
                        XSED.Load();
                        XSED->isOffset = false;
                        //XSED.size = 4;
                        _readBuffer(&XSED->seed, buffer, 4, curPos);
                        break;
                    default:
                        printf("  REFR: %08X - Unknown XSED size = %u\n", formID, subSize);
                        printf("  CurPos = %04x\n\n", curPos - 6);
                        curPos += subSize;
                        break;
                    }
                break;
            case 'DOLX':
                XLOD.Read(buffer, subSize, curPos);
                break;
            case 'GHCX':
                XCHG.Read(buffer, subSize, curPos);
                break;
            case 'TLHX':
                XHLT.Read(buffer, subSize, curPos);
                break;
            case 'ICPX':
                XPCI.Load();
                XPCI->XPCI.Read(buffer, subSize, curPos);
                lastRecord = 'ICPX';
                break;
            case 'LLUF':
                switch(lastRecord)
                    {
                    case 'ICPX':
                        XPCI.Load();
                        XPCI->FULL.Read(buffer, subSize, curPos);
                        break;
                    case 'KRMX':
                        Marker->FULL.Read(buffer, subSize, curPos);
                        break;
                    default:
                        printf("  REFR: %08X - Unexpected FULL record\n", formID);
                        printf("  Size = %i\n", subSize);
                        printf("  CurPos = %04x\n\n", curPos - 6);
                        curPos += subSize;
                        break;
                    }
                break;
            case 'MCLX':
                XLCM.Read(buffer, subSize, curPos);
                break;
            case 'MTRX':
                XRTM.Read(buffer, subSize, curPos);
                break;
            case 'TCAX':
                XACT.Read(buffer, subSize, curPos);
                break;
            case 'TNCX':
                XCNT.Read(buffer, subSize, curPos);
                break;
            case 'KRMX':
                Marker.Load();
                curPos += subSize;
                lastRecord = 'KRMX';
                break;
            case 'MANF':
                Marker.Load();
                Marker->FNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANT':
                Marker.Load();
                Marker->TNAM.Read(buffer, subSize, curPos);
                break;
            case 'MANO':
                IsOpenByDefault(true);
                curPos += subSize;
                break;
            case 'LCSX':
                XSCL.Read(buffer, subSize, curPos);
                break;
            case 'LOSX':
                XSOL.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.Read(buffer, subSize, curPos);
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  REFR: %08X - Unknown subType = %04x\n", formID, subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    return 0;
    }

SINT32 REFRRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    NAME.Unload();
    XTEL.Unload();
    XLOC.Unload();
    Ownership.Unload();
    XESP.Unload();
    XTRG.Unload();
    XSED.Unload();
    XLOD.Unload();
    XCHG.Unload();
    XHLT.Unload();
    XPCI.Unload();
    XLCM.Unload();
    XRTM.Unload();
    XACT.Unload();
    XCNT.Unload();
    Marker.Unload();
    XSCL.Unload();
    XSOL.Unload();
    DATA.Unload();
    return 1;
    }

SINT32 REFRRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    char null = 0;

    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());

    if(NAME.IsLoaded())
        SaveHandler.writeSubRecord('EMAN', &NAME.value, NAME.GetSize());

    if(XTEL.IsLoaded())
        SaveHandler.writeSubRecord('LETX', XTEL.value, XTEL.GetSize());

    if(XLOC.IsLoaded())
        SaveHandler.writeSubRecord('COLX', XLOC.value, XLOC.GetSize());

    if(Ownership.IsLoaded() && Ownership->XOWN.IsLoaded())
        {
        SaveHandler.writeSubRecord('NWOX', &Ownership->XOWN.value, Ownership->XOWN.GetSize());
        if(Ownership->XRNK.IsLoaded())
            SaveHandler.writeSubRecord('KNRX', Ownership->XRNK.value, Ownership->XRNK.GetSize());
        if(Ownership->XGLB.IsLoaded())
            SaveHandler.writeSubRecord('BLGX', Ownership->XGLB.value, Ownership->XGLB.GetSize());
        }

    if(XESP.IsLoaded())
        SaveHandler.writeSubRecord('PSEX', XESP.value, XESP.GetSize());

    if(XTRG.IsLoaded())
        SaveHandler.writeSubRecord('GRTX', XTRG.value, XTRG.GetSize());

    if(XSED.IsLoaded())
        if(XSED->isOffset)
            SaveHandler.writeSubRecord('DESX', &XSED->offset, 1);
        else
            SaveHandler.writeSubRecord('DESX', &XSED->seed, 4);

    if(XLOD.IsLoaded())
        SaveHandler.writeSubRecord('DOLX', XLOD.value, XLOD.GetSize());

    if(XCHG.IsLoaded())
        SaveHandler.writeSubRecord('GHCX', XCHG.value, XCHG.GetSize());

    if(XHLT.IsLoaded())
        SaveHandler.writeSubRecord('TLHX', XHLT.value, XHLT.GetSize());

    if(XPCI.IsLoaded() && XPCI->XPCI.IsLoaded())
        {
        SaveHandler.writeSubRecord('ICPX', XPCI->XPCI.value, XPCI->XPCI.GetSize());
        if(XPCI->FULL.IsLoaded())
            SaveHandler.writeSubRecord('LLUF', XPCI->FULL.value, XPCI->FULL.GetSize());
        else
            SaveHandler.writeSubRecord('LLUF', &null, 1);
        }

    if(XLCM.IsLoaded())
        SaveHandler.writeSubRecord('MCLX', XLCM.value, XLCM.GetSize());

    if(XRTM.IsLoaded())
        SaveHandler.writeSubRecord('MTRX', XRTM.value, XRTM.GetSize());

    if(XACT.IsLoaded())
        SaveHandler.writeSubRecord('TCAX', XACT.value, XACT.GetSize());

    if(XCNT.IsLoaded())
        SaveHandler.writeSubRecord('TNCX', XCNT.value, XCNT.GetSize());

    if(Marker.IsLoaded())
        {
        SaveHandler.writeSubRecord('KRMX', NULL, 0);
        if(Marker->FNAM.IsLoaded())
            SaveHandler.writeSubRecord('MANF', &Marker->FNAM.value, Marker->FNAM.GetSize());
        if(Marker->FULL.IsLoaded())
            SaveHandler.writeSubRecord('LLUF', Marker->FULL.value, Marker->FULL.GetSize());
        if(Marker->TNAM.IsLoaded())
            SaveHandler.writeSubRecord('MANT', &Marker->TNAM.value, Marker->TNAM.GetSize());
        }

    if(IsOpenByDefault()) //ONAM
        SaveHandler.writeSubRecord('MANO', NULL, 0);

    if(XSCL.IsLoaded())
        SaveHandler.writeSubRecord('LCSX', XSCL.value, XSCL.GetSize());

    if(XSOL.IsLoaded())
        SaveHandler.writeSubRecord('LOSX', XSOL.value, XSOL.GetSize());

    if(DATA.IsLoaded())
        SaveHandler.writeSubRecord('ATAD', &DATA.value, DATA.GetSize());

    return -1;
    }

bool REFRRecord::operator ==(const REFRRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            NAME == other.NAME &&
            XTEL == other.XTEL &&
            XLOC == other.XLOC &&
            Ownership == other.Ownership &&
            XESP == other.XESP &&
            XTRG == other.XTRG &&
            XSED == other.XSED &&
            XLOD == other.XLOD &&
            XCHG == other.XCHG &&
            XHLT == other.XHLT &&
            XPCI == other.XPCI &&
            XLCM == other.XLCM &&
            XRTM == other.XRTM &&
            XACT == other.XACT &&
            XCNT == other.XCNT &&
            Marker == other.Marker &&
            XSCL == other.XSCL &&
            XSOL == other.XSOL &&
            DATA == other.DATA);
    }

bool REFRRecord::operator !=(const REFRRecord &other) const
    {
    return !(*this == other);
    }