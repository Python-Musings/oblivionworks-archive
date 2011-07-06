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
// Visitors.h
#include "GenericRecord.h"

class FormIDMasterUpdater : public FormIDOp
    {
    private:
        FormIDHandlerClass &FormIDHandler;
        const UINT8 &ExpandedIndex;
        const UINT8 &CollapsedIndex;
        const UINT8 (&ExpandTable)[256];
        const UINT8 (&CollapseTable)[256];

    public:
        FormIDMasterUpdater(FormIDHandlerClass &_FormIDHandler);
        ~FormIDMasterUpdater();

        bool Accept(UINT32 &curFormID);
        bool AcceptMGEF(UINT32 &curMgefCode);
    };

class FormIDMatchCounter : public FormIDOp
    {
    private:
        const UINT32 &FormIDToMatch;

    public:
        FormIDMatchCounter(const UINT32 &_FormIDToMatch);
        ~FormIDMatchCounter();

        bool Accept(UINT32 &curFormID);
        bool AcceptMGEF(UINT32 &curMgefCode);
    };

class RecordUnloader : public RecordOp
    {
    public:
        RecordUnloader();
        ~RecordUnloader();

        bool Accept(Record *&curRecord);
    };

class RecordIDRetriever : public RecordOp
    {
    private:
        RECORDIDARRAY RecordIDs;

    public:
        RecordIDRetriever(RECORDIDARRAY _RecordIDs);
        ~RecordIDRetriever();

        bool Accept(Record *&curRecord);
    };

class RecordMasterChecker : public RecordOp
    {
    private:
        class FormIDMasterChecker : public FormIDOp
            {
            private:
                const UINT8 (&CollapseTable)[256];
                const UINT8 &MasterIndex;

            public:
                FormIDMasterChecker(const UINT8 (&_CollapseTable)[256], const UINT8 &_MasterIndex);
                ~FormIDMasterChecker();

                bool Accept(UINT32 &curFormID);
                bool AcceptMGEF(UINT32 &curMgefCode);
            } checker;

        RecordReader reader;

    public:
        RecordMasterChecker(FormIDHandlerClass &_FormIDHandler, std::vector<FormIDResolver *> &_Expanders, const UINT8 &_MasterIndex);
        ~RecordMasterChecker();

        bool Accept(Record *&curRecord);
    };

class RecordFormIDSwapper : public RecordOp
    {
    private:
        class FormIDSwapper : public FormIDOp
            {
            private:
                const UINT32 &FormIDToMatch;
                const UINT32 &FormIDToSwap;
                FormIDMasterUpdater checker;

            public:
                FormIDSwapper(const UINT32 &_FormIDToMatch, const UINT32 &_FormIDToSwap, FormIDHandlerClass &_FormIDHandler);
                ~FormIDSwapper();

                bool Accept(UINT32 &curFormID);
                bool AcceptMGEF(UINT32 &curMgefCode);
            } swapper;

        RecordReader reader;

    public:
        RecordFormIDSwapper(const UINT32 &_FormIDToMatch, const UINT32 &_FormIDToSwap, FormIDHandlerClass &_FormIDHandler, std::vector<FormIDResolver *> &_Expanders);
        ~RecordFormIDSwapper();

        bool Accept(Record *&curRecord);
    };

class RecordDeleter : public RecordOp
    {
    private:
        EditorID_Map &EditorID_ModFile_Record;
        FormID_Map &FormID_ModFile_Record;
        Record *RecordToDelete;

    public:
        RecordDeleter(Record *_RecordToDelete, EditorID_Map &_EditorID_ModFile_Record, FormID_Map &_FormID_ModFile_Record);
        ~RecordDeleter();

        bool Accept(Record *&curRecord);
    };

class RecordIndexer : public RecordOp
    {
    private:
        EditorID_Map &EditorID_ModFile_Record;
        FormID_Map &FormID_ModFile_Record;
        ModFile *curModFile;

    public:
        RecordIndexer(ModFile *_curModFile, EditorID_Map &_EditorID_Map, FormID_Map &_FormID_Map);
        RecordIndexer(EditorID_Map &_EditorID_Map, FormID_Map &_FormID_Map);
        ~RecordIndexer();

        bool Accept(Record *&curRecord);

        void SetModFile(ModFile *_curModFile);
    };