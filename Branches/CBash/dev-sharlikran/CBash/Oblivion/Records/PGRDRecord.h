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

class PGRDRecord : public Record
    {
    private:
        struct PGRDDATA
            {
            UINT16  count;

            PGRDDATA();
            ~PGRDDATA();

            bool operator ==(const PGRDDATA &other) const;
            bool operator !=(const PGRDDATA &other) const;
            };

        //struct PGRDPGRR
        //    {
        //    unsigned short start, end;
        //    PGRDPGRR():start(0), end(0) {}
        //    #ifdef _DEBUG
        //    void Debug(SINT32 debugLevel, size_t &indentation)
        //        {
        //        if(debugLevel > 3)
        //            {
        //            indentation += 2;
        //            PrintIndent(indentation);
        //            printf("start = %u\n", start);
        //            PrintIndent(indentation);
        //            printf("end   = %u\n", end);
        //            indentation -= 2;
        //            }
        //        }
        //    #endif
        //    };

        struct PGRDPGRI
            {
            UINT16  point;
            UINT8   unused1[2];
            FLOAT32 x, y, z;

            PGRDPGRI();
            ~PGRDPGRI();

            bool operator ==(const PGRDPGRI &other) const;
            bool operator !=(const PGRDPGRI &other) const;
            };

        struct PGRDPGRL
            {
            std::vector<UINT32> points;

            PGRDPGRL();
            ~PGRDPGRL();

            bool operator ==(const PGRDPGRL &other) const;
            bool operator !=(const PGRDPGRL &other) const;
            };

    public:
        ReqSubRecord<PGRDDATA> DATA;
        std::vector<GENPGRP> PGRP;
        RawRecord PGAG;
        RawRecord PGRR;
        std::vector<PGRDPGRI> PGRI;
        std::vector<PGRDPGRL *> PGRL;

        PGRDRecord(unsigned char *_recData=NULL);
        PGRDRecord(PGRDRecord *srcRecord);
        ~PGRDRecord();

        bool   VisitFormIDs(FormIDOp &op);

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

        bool operator ==(const PGRDRecord &other) const;
        bool operator !=(const PGRDRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(PGRDRecord *)other;}
    };