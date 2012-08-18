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
// ModFile.h
#include "Common.h"
#include "GenericRecord.h"
#include "GRUPRecord.h"
#include "Oblivion/Records/TES4Record.h" //Is shared across all mod types

class ModFile
    {
    public:
        _FileHandler ReadHandler;
        FormIDHandlerClass FormIDHandler;
        ModFlags Flags;
        time_t ModTime;
        UINT32 ModID;

        TES4Record TES4;

        ModFile(STRING FileName, STRING ModName, const UINT32 _flags);
        virtual ~ModFile();

        bool operator <(ModFile &other);
        bool operator >(ModFile &other);

        bool   Open();
        bool   Close();

        virtual SINT32   LoadTES4() abstract {};
        virtual SINT32   Load(RecordOp &indexer, std::vector<FormIDResolver *> &Expanders) abstract {};
        virtual UINT32   GetNumRecords(const UINT32 &RecordType) abstract {};
        virtual Record * CreateRecord(const UINT32 &RecordType, STRING const &RecordEditorID, Record *&SourceRecord, Record *&ParentRecord, CreateRecordOptions &options) abstract {};
        virtual SINT32   CleanMasters(std::vector<FormIDResolver *> &Expanders) abstract {};
		virtual SINT32   CleanMasters2(std::vector<STRING> &MasterNames) abstract {};
        virtual SINT32   Save(STRING const &SaveName, std::vector<FormIDResolver *> &Expanders, bool CloseMod) abstract {};

        virtual void     VisitAllRecords(RecordOp &op) abstract {};
        virtual void     VisitRecords(const UINT32 &TopRecordType, const UINT32 &RecordType, RecordOp &op, bool DeepVisit=false) abstract {};
    };