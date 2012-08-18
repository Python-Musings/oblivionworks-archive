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

class DIALRecord : public Record
    {
    private:
        enum eDialogType
            {
            eTopic        = 0,
            eConversation = 1,
            eCombat       = 2,
            ePersuasion   = 3,
            eDetection    = 4,
            eService      = 5,
            eMisc         = 6
            };

    public:
        StringRecord EDID;
        std::vector<FORMID> QSTI;
        std::vector<FORMID> QSTR;
        StringRecord FULL;
        ReqSubRecord<GENFLAG> DATA;
        std::vector<Record *> INFO;

        DIALRecord(unsigned char *_recData=NULL);
        DIALRecord(DIALRecord *srcRecord);
        ~DIALRecord();

        bool   HasSubRecords();
        bool   VisitSubRecords(const UINT32 &RecordType, RecordOp &op);
        bool   VisitFormIDs(FormIDOp &op);

        bool   IsTopic();
        void   IsTopic(bool value);
        bool   IsConversation();
        void   IsConversation(bool value);
        bool   IsCombat();
        void   IsCombat(bool value);
        bool   IsPersuasion();
        void   IsPersuasion(bool value);
        bool   IsDetection();
        void   IsDetection(bool value);
        bool   IsService();
        void   IsService(bool value);
        bool   IsMisc();
        void   IsMisc(bool value);
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

        bool operator ==(const DIALRecord &other) const;
        bool operator !=(const DIALRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(DIALRecord *)other;}
    };