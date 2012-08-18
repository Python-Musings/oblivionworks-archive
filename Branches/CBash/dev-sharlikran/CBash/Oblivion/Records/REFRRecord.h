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
#pragma once
#include "..\..\Common.h"
#include "..\..\GenericRecord.h"

class REFRRecord : public Record
    {
    private:
        struct REFRXTEL
            {
            FORMID  destinationFid;
            GENPOSDATA destination;

            REFRXTEL();
            ~REFRXTEL();

            bool operator ==(const REFRXTEL &other) const;
            bool operator !=(const REFRXTEL &other) const;
            };

        struct REFRXLOC
            {
            UINT8   level, unused1[3];
            FORMID  key;
            UINT8   unused2[4], flags, unused3[3];

            REFRXLOC();
            ~REFRXLOC();

            bool operator ==(const REFRXLOC &other) const;
            bool operator !=(const REFRXLOC &other) const;
            };

        struct REFRXSED
            {
            union
                {
                UINT32  seed;
                UINT8   offset;
                };
            bool isOffset;

            REFRXSED();
            ~REFRXSED();

            bool operator ==(const REFRXSED &other) const;
            bool operator !=(const REFRXSED &other) const;
            };

        struct REFRXCHG
            {
            FLOAT32 charge;

            REFRXCHG();
            ~REFRXCHG();

            bool operator ==(const REFRXCHG &other) const;
            bool operator !=(const REFRXCHG &other) const;
            };

        struct REFRXHLT
            {
            SINT32  health;

            REFRXHLT();
            ~REFRXHLT();

            bool operator ==(const REFRXHLT &other) const;
            bool operator !=(const REFRXHLT &other) const;
            };

        struct REFRXLCM
            {
            SINT32  levelMod;

            REFRXLCM();
            ~REFRXLCM();

            bool operator ==(const REFRXLCM &other) const;
            bool operator !=(const REFRXLCM &other) const;
            };

        struct REFRXCNT
            {
            SINT32  count;

            REFRXCNT();
            ~REFRXCNT();

            bool operator ==(const REFRXCNT &other) const;
            bool operator !=(const REFRXCNT &other) const;
            };

        struct REFRTNAM
            {
            UINT8   markerType, unused1;

            REFRTNAM();
            ~REFRTNAM();

            bool operator ==(const REFRTNAM &other) const;
            bool operator !=(const REFRTNAM &other) const;
            };

        struct REFRMAPMARKER
            {
            ReqSubRecord<GENFLAG> FNAM;
            StringRecord FULL;
            ReqSubRecord<REFRTNAM> TNAM;

            bool operator ==(const REFRMAPMARKER &other) const;
            bool operator !=(const REFRMAPMARKER &other) const;
            };

        enum flagsFlags
            {
            fIsOppositeParent = 0x00000001
            };

        enum fnamFlags
            {
            fVisible     = 0x00000001,
            fCanTravelTo = 0x00000002
            };

        enum xactFlags
            {
            fUseDefault    = 0x00000001,
            fActivate      = 0x00000002,
            fOpen          = 0x00000004,
            fOpenByDefault = 0x00000008
            };

        enum xlocFlags
            {
            fLeveledLock = 0x00000004
            };

        enum markerTypeTypes
            {
            eMarkerNone      = 0,
            eCamp            = 1,
            eCave            = 2,
            eCity            = 3,
            eElvenRuin       = 4,
            eFortRuin        = 5,
            eMine            = 6,
            eLandmark        = 7,
            eTavern          = 8,
            eSettlement      = 9,
            eDaedricShrine   = 10,
            eOblivionGate    = 11,
            eUnknownDoorIcon = 12
            };

        enum eSoulType
            {
            eNone    = 0,
            ePetty   = 1,
            eLesser  = 2,
            eCommon  = 3,
            eGreater = 4,
            eGrand   = 5
            };

    public:
        StringRecord EDID;
        ReqSubRecord<GENFID> NAME;
        OptSubRecord<REFRXTEL> XTEL;
        SemiOptSubRecord<REFRXLOC> XLOC;
        OptSubRecord<GENXOWN> Ownership;
        OptSubRecord<GENXESP> XESP;
        OptSubRecord<GENFID> XTRG;
        SemiOptSubRecord<REFRXSED> XSED;
        OptSubRecord<GENXLOD> XLOD;
        OptSubRecord<REFRXCHG> XCHG;
        OptSubRecord<REFRXHLT> XHLT;
        OptSubRecord<GENXPCI> XPCI;
        OptSubRecord<REFRXLCM> XLCM;
        OptSubRecord<GENFID> XRTM;
        OptSubRecord<GENUFLAG> XACT;
        OptSubRecord<REFRXCNT> XCNT;
        OptSubRecord<REFRMAPMARKER> Marker;
        //bool ONAM; //Open by Default, empty marker, written whenever fOpenByDefault is true
        OptSubRecord<GENXSCL> XSCL;
        OptSubRecord<GENFLAG> XSOL;
        ReqSubRecord<GENPOSDATA> DATA;

        REFRRecord(unsigned char *_recData=NULL);
        REFRRecord(REFRRecord *srcRecord);
        ~REFRRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsOppositeParent();
        void   IsOppositeParent(bool value);
        bool   IsParentFlagMask(UINT8 Mask, bool Exact=false);
        void   SetParentFlagMask(UINT8 Mask);

        bool   IsVisible();
        void   IsVisible(bool value);
        bool   IsCanTravelTo();
        void   IsCanTravelTo(bool value);
        bool   IsMapFlagMask(UINT8 Mask, bool Exact=false);
        void   SetMapFlagMask(UINT8 Mask);

        bool   IsUseDefault();
        void   IsUseDefault(bool value);
        bool   IsActivate();
        void   IsActivate(bool value);
        bool   IsOpen();
        void   IsOpen(bool value);
        bool   IsOpenByDefault();
        void   IsOpenByDefault(bool value);
        bool   IsActionFlagMask(UINT32 Mask, bool Exact=false);
        void   SetActionFlagMask(UINT32 Mask);

        bool   IsLeveledLock();
        void   IsLeveledLock(bool value);
        bool   IsLockFlagMask(UINT8 Mask, bool Exact=false);
        void   SetLockFlagMask(UINT8 Mask);

        bool   IsNoMarker();
        void   IsNoMarker(bool value);
        bool   IsCamp();
        void   IsCamp(bool value);
        bool   IsCave();
        void   IsCave(bool value);
        bool   IsCity();
        void   IsCity(bool value);
        bool   IsElvenRuin();
        void   IsElvenRuin(bool value);
        bool   IsFortRuin();
        void   IsFortRuin(bool value);
        bool   IsMine();
        void   IsMine(bool value);
        bool   IsLandmark();
        void   IsLandmark(bool value);
        bool   IsTavern();
        void   IsTavern(bool value);
        bool   IsSettlement();
        void   IsSettlement(bool value);
        bool   IsDaedricShrine();
        void   IsDaedricShrine(bool value);
        bool   IsOblivionGate();
        void   IsOblivionGate(bool value);
        bool   IsUnknownDoorIcon();
        void   IsUnknownDoorIcon(bool value);
        bool   IsMarkerType(UINT8 Type);
        void   SetMarkerType(UINT8 Type);

        bool   IsNoSoul();
        void   IsNoSoul(bool value);
        bool   IsPettySoul();
        void   IsPettySoul(bool value);
        bool   IsLesserSoul();
        void   IsLesserSoul(bool value);
        bool   IsCommonSoul();
        void   IsCommonSoul(bool value);
        bool   IsGreaterSoul();
        void   IsGreaterSoul(bool value);
        bool   IsGrandSoul();
        void   IsGrandSoul(bool value);
        bool   IsSoul(UINT8 Type);
        void   SetSoul(UINT8 Type);

        UINT32 GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, UINT32 WhichAttribute=0);
        void * GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool   SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, UINT32 ArraySize=0);
        void   DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        UINT32 GetSize(bool forceCalc=false);
        UINT32 GetType();
        STRING GetStrType();
        UINT32 GetParentType();

        SINT32 ParseRecord(unsigned char *buffer, const UINT32 &recSize);
        SINT32 Unload();
        SINT32 WriteRecord(_FileHandler &SaveHandler);

        bool operator ==(const REFRRecord &other) const;
        bool operator !=(const REFRRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(REFRRecord *)other;}
    };