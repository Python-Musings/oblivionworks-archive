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

class SOUNRecord : public Record
    {
    private:
        struct SOUNSNDX
            {
            UINT8   minDistance, maxDistance;
            SINT8   freqAdjustment;
            UINT8   unused1;
            UINT16  flags;
            UINT8   unused2[2];
            SINT16  staticAtten;
            UINT8   stopTime,startTime;

            SOUNSNDX();
            ~SOUNSNDX();

            bool operator ==(const SOUNSNDX &other) const;
            bool operator !=(const SOUNSNDX &other) const;
            };

        enum flagsFlags
            {
            fIsRandomFrequencyShift = 0x00000001,
            fIsPlayAtRandom         = 0x00000002,
            fIsEnvironmentIgnored   = 0x00000004,
            fIsRandomLocation       = 0x00000008,
            fIsLoop                 = 0x00000010,
            fIsMenuSound            = 0x00000020,
            fIs2D                   = 0x00000040,
            fIs360LFE               = 0x00000080
            };

    public:
        StringRecord EDID;
        StringRecord FNAM;
        ReqSubRecord<SOUNSNDX> SNDX;

        SOUNRecord(unsigned char *_recData=NULL);
        SOUNRecord(SOUNRecord *srcRecord);
        ~SOUNRecord();

        bool   IsRandomFrequencyShift();
        void   IsRandomFrequencyShift(bool value);
        bool   IsPlayAtRandom();
        void   IsPlayAtRandom(bool value);
        bool   IsEnvironmentIgnored();
        void   IsEnvironmentIgnored(bool value);
        bool   IsRandomLocation();
        void   IsRandomLocation(bool value);
        bool   IsLoop();
        void   IsLoop(bool value);
        bool   IsMenuSound();
        void   IsMenuSound(bool value);
        bool   Is2D();
        void   Is2D(bool value);
        bool   Is360LFE();
        void   Is360LFE(bool value);
        bool   IsFlagMask(unsigned short Mask, bool Exact=false);
        void   SetFlagMask(unsigned short Mask);

        UINT32 GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, UINT32 WhichAttribute=0);
        void * GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool   SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, UINT32 ArraySize=0);
        void   DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        UINT32 GetSize(bool forceCalc=false);
        UINT32 GetType();
        STRING GetStrType();

        SINT32 ParseRecord(unsigned char *buffer, const UINT32 &recSize);
        SINT32 Unload();
        SINT32 WriteRecord(_FileHandler &SaveHandler);

        bool operator ==(const SOUNRecord &other) const;
        bool operator !=(const SOUNRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(SOUNRecord *)other;}
    };