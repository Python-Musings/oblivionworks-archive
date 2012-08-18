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

class TES4Record : public Record
    {
    private:
        struct TES4HEDR
            {
            FLOAT32 version;
            UINT32  numRecords, nextObject;

            TES4HEDR(FLOAT32 _version=0.8f, UINT32 _numRecords=0, UINT32 _nextObject=END_HARDCODED_IDS);
            ~TES4HEDR();

            bool operator ==(const TES4HEDR &other) const;
            bool operator !=(const TES4HEDR &other) const;
            };

        struct TES4DATA //Placeholder for writing. Otherwise not used.
            {
            UINT32 unk1, unk2;

            TES4DATA();
            ~TES4DATA();
            };

    public:
        ReqSubRecord<TES4HEDR> HEDR;
        RawRecord OFST;
        RawRecord DELE;
        StringRecord CNAM;
        StringRecord SNAM;
        std::vector<StringRecord> MAST;

        TES4Record(unsigned char *_recData=NULL);
        TES4Record(TES4Record *srcRecord);
        ~TES4Record();

        bool IsESM();
        void IsESM(bool value);

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

        bool operator ==(const TES4Record &other) const;
        bool operator !=(const TES4Record &other) const;
		virtual bool equals(const Record *other) const {return *this == *(TES4Record *)other;}
    };