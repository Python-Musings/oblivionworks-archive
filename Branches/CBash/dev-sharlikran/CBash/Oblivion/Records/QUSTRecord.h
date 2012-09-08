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

class QUSTRecord : public Record
    {
    private:
        struct QUSTDATA
            {
            UINT8   flags, priority;

            QUSTDATA();
            ~QUSTDATA();

            bool operator ==(const QUSTDATA &other) const;
            bool operator !=(const QUSTDATA &other) const;
            };

        struct QUSTEntry
            {
            ReqSubRecord<GENFLAG> QSDT;
            std::vector<ReqSubRecord<GENCTDA> *> CTDA;
            StringRecord CNAM;
            ReqSubRecord<GENSCHR> SCHR;
            RawRecord SCDA;
            NonNullStringRecord SCTX;
            std::vector<ReqSubRecord<GENSCR_> *> SCR_;

            enum entriesFlags
                {
                fIsCompletes = 0x00000001
                };

            QUSTEntry();
            ~QUSTEntry();

            bool IsCompletes();
            void IsCompletes(bool value);
            bool IsFlagMask(UINT8 Mask, bool Exact=false);
            void SetFlagMask(UINT8 Mask);

            bool operator ==(const QUSTEntry &other) const;
            bool operator !=(const QUSTEntry &other) const;
            };

        struct QUSTINDX
            {
            UINT16  stage;

            QUSTINDX();
            ~QUSTINDX();

            bool operator ==(const QUSTINDX &other) const;
            bool operator !=(const QUSTINDX &other) const;
            };

        struct QUSTStage
            {
            ReqSubRecord<QUSTINDX> INDX;
            std::vector<QUSTEntry *> Entries;

            QUSTStage();
            ~QUSTStage();

            bool operator ==(const QUSTStage &other) const;
            bool operator !=(const QUSTStage &other) const;
            };

        struct QUSTQSTA
            {
            UINT32  targetId;
            UINT8   flags, unused1[3];

            QUSTQSTA();
            ~QUSTQSTA();

            bool operator ==(const QUSTQSTA &other) const;
            bool operator !=(const QUSTQSTA &other) const;
            };

        struct QUSTTarget
            {
            ReqSubRecord<QUSTQSTA> QSTA;
            std::vector<ReqSubRecord<GENCTDA> *> CTDA;

            enum targetsFlags
                {
                fIsIgnoresLocks = 0x00000001
                };

            QUSTTarget();
            ~QUSTTarget();

            bool IsIgnoresLocks();
            void IsIgnoresLocks(bool value);
            bool IsFlagMask(UINT8 Mask, bool Exact=false);
            void SetFlagMask(UINT8 Mask);

            bool operator ==(const QUSTTarget &other) const;
            bool operator !=(const QUSTTarget &other) const;
            };

        enum flagsFlags
            {
            fIsStartEnabled   = 0x00000001,
            fIsRepeatedTopics = 0x00000004,
            fIsRepeatedStages = 0x00000008
            };

    public:
        StringRecord EDID;
        OptSubRecord<GENFID> SCRI;
        StringRecord FULL;
        StringRecord ICON;
        ReqSubRecord<QUSTDATA> DATA;
        std::vector<ReqSubRecord<GENCTDA> *> CTDA;
        std::vector<QUSTStage *> Stages;
        std::vector<QUSTTarget *> Targets;

        QUSTRecord(unsigned char *_recData=NULL);
        QUSTRecord(QUSTRecord *srcRecord);
        ~QUSTRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsStartEnabled();
        void   IsStartEnabled(bool value);
        bool   IsRepeatedTopics();
        void   IsRepeatedTopics(bool value);
        bool   IsRepeatedStages();
        void   IsRepeatedStages(bool value);
        bool   IsFlagMask(UINT8 Mask, bool Exact=false);
        void   SetFlagMask(UINT8 Mask);

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

        bool operator ==(const QUSTRecord &other) const;
        bool operator !=(const QUSTRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(QUSTRecord *)other;}
    };