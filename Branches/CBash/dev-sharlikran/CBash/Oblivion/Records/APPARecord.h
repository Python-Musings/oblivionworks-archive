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
#include <vector>

class APPARecord : public Record
    {
    private:
        #pragma pack(push)
        #pragma pack(1)
        struct APPADATA
            {
            UINT8   apparatus;
            UINT32  value;
            FLOAT32 weight, quality;

            APPADATA();
            ~APPADATA();

            bool operator ==(const APPADATA &other) const;
            bool operator !=(const APPADATA &other) const;
            };
        #pragma pack(pop)

        enum eApparatusType
            {
            eMortarPestle = 0,
            eAlembic      = 1,
            eCalcinator   = 2,
            eRetort       = 3
            };

    public:
        StringRecord EDID;
        StringRecord FULL;
        OptSubRecord<GENMODEL> MODL;
        StringRecord ICON;
        OptSubRecord<GENFID> SCRI;
        ReqSubRecord<APPADATA> DATA;

        APPARecord(unsigned char *_recData=NULL);
        APPARecord(APPARecord *srcRecord);
        ~APPARecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsMortarPestle();
        void   IsMortarPestle(bool value);
        bool   IsAlembic();
        void   IsAlembic(bool value);
        bool   IsCalcinator();
        void   IsCalcinator(bool value);
        bool   IsRetort();
        void   IsRetort(bool value);
        bool   IsType(UINT8 Type);
        void   SetType(UINT8 Type);

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

        bool operator ==(const APPARecord &other) const;
        bool operator !=(const APPARecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(APPARecord *)other;}
    };