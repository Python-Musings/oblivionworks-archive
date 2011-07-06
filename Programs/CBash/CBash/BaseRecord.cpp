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

#include "Common.h"
#include "BaseRecord.h"
#include "zlib/zlib.h"

FormIDOp::FormIDOp():
    count(0),
    stop(false)
    {
    //
    }

FormIDOp::~FormIDOp()
    {
    //
    }

UINT32 FormIDOp::GetCount()
    {
    return count;
    }

void FormIDOp::ResetCount()
    {
    count = 0;
    }

bool FormIDOp::Stop()
    {
    return stop;
    }

bool FormIDOp::GetResult()
    {
    return result;
    }

FormIDResolver::FormIDResolver(const UINT8 (&_ResolveTable)[255]):
    FormIDOp(),
    ResolveTable(_ResolveTable)
    {
    //
    }

FormIDResolver::~FormIDResolver()
    {
    //
    }

bool FormIDResolver::Accept(UINT32 &curFormID)
    {
    if(curFormID)
        curFormID = (ResolveTable[curFormID >> 24] << 24 ) | (curFormID & 0x00FFFFFF);
    return stop;
    }

bool FormIDResolver::AcceptMGEF(UINT32 &curMgefCode)
    {
    if(curMgefCode)
        curMgefCode = (ResolveTable[curMgefCode & 0x000000FF]) | (curMgefCode & 0xFFFFFF00);
    return stop;
    }

FormIDMasterUpdater::FormIDMasterUpdater(FormIDHandlerClass &_FormIDHandler):
    FormIDOp(),
    FormIDHandler(_FormIDHandler)
    {
    //
    }

FormIDMasterUpdater::~FormIDMasterUpdater()
    {
    //
    }

bool FormIDMasterUpdater::Accept(UINT32 &curFormID)
    {
    UINT32 modIndex = curFormID >> 24;

    //If formID is not set, or the formID belongs to the engine, or the formID belongs to the mod, or if the master is already present, do nothing
    if((curFormID == 0) || (curFormID < END_HARDCODED_IDS) || (modIndex == FormIDHandler.ExpandedIndex) || (FormIDHandler.CollapseTable[modIndex] != FormIDHandler.CollapsedIndex))
        return stop;

    //If the modIndex doesn't match to a loaded mod, it gets assigned to the mod that it is in.
    if(modIndex >= FormIDHandler.LoadOrder255.size())
        {
        FormIDResolver collapser(FormIDHandler.CollapseTable);
        collapser.Accept(curFormID);
        FormIDResolver expander(FormIDHandler.ExpandTable);
        expander.Accept(curFormID);
        return stop;
        }

    FormIDHandler.AddMaster(FormIDHandler.LoadOrder255[modIndex]);
    ++count;
    return stop;
    }

bool FormIDMasterUpdater::AcceptMGEF(UINT32 &curMgefCode)
    {
    UINT32 modIndex = curMgefCode << 24;

    //If formID is not set, or the formID belongs to the engine, or the formID belongs to the mod, or if the master is already present, do nothing
    if((curMgefCode == 0) || (curMgefCode < END_HARDCODED_IDS) || (modIndex == FormIDHandler.ExpandedIndex) || (FormIDHandler.CollapseTable[modIndex] != FormIDHandler.CollapsedIndex))
        return stop;

    //If the modIndex doesn't match to a loaded mod, it gets assigned to the mod that it is in.
    if(modIndex >= FormIDHandler.LoadOrder255.size())
        {
        FormIDResolver collapser(FormIDHandler.CollapseTable);
        collapser.AcceptMGEF(curMgefCode);
        FormIDResolver expander(FormIDHandler.ExpandTable);
        expander.AcceptMGEF(curMgefCode);
        return stop;
        }

    FormIDHandler.AddMaster(FormIDHandler.LoadOrder255[modIndex]);
    ++count;
    return stop;
    }

FormIDMatchCounter::FormIDMatchCounter(const UINT32 &_FormIDToMatch):
    FormIDOp(),
    FormIDToMatch(_FormIDToMatch)
    {
    //
    }

FormIDMatchCounter::~FormIDMatchCounter()
    {
    //
    }

bool FormIDMatchCounter::Accept(UINT32 &curFormID)
    {
    if(curFormID == FormIDToMatch)
        ++count;
    return stop;
    }

bool FormIDMatchCounter::AcceptMGEF(UINT32 &curMgefCode)
    {
    if(curMgefCode == FormIDToMatch)
        ++count;
    return stop;
    }

RecordOp::RecordOp():
    count(0)
    {
    //
    }

RecordOp::~RecordOp()
    {
    //
    }

UINT32 RecordOp::GetCount()
    {
    return count;
    }

void RecordOp::ResetCount()
    {
    count = 0;
    }

bool RecordOp::Stop()
    {
    return stop;
    }

bool RecordOp::GetResult()
    {
    return result;
    }

RecordReader::RecordReader(FormIDHandlerClass &_FormIDHandler):
    RecordOp(),
    expander(_FormIDHandler.ExpandTable)
    {
    //
    }

RecordReader::~RecordReader()
    {
    //
    }

bool RecordReader::Accept(Record **curRecord)
    {
    result = (*curRecord)->Read();
    if(result)
        {
        (*curRecord)->VisitFormIDs(expander);
        ++count;
        }
    return stop;
    }

RecordUnloader::RecordUnloader():
    RecordOp()
    {
    //
    }

RecordUnloader::~RecordUnloader()
    {
    //
    }

bool RecordUnloader::Accept(Record **curRecord)
    {
    if((*curRecord)->recData != NULL)
        {
        (*curRecord)->Unload();
        ++count;
        }
    return stop;
    }

FormIDRecordRetriever::FormIDRecordRetriever(UINT32 **_RecordFIDs):
    RecordOp(),
    RecordFIDs(_RecordFIDs)
    {
    //
    }

FormIDRecordRetriever::~FormIDRecordRetriever()
    {
    //
    }

bool FormIDRecordRetriever::Accept(Record **curRecord)
    {
    RecordFIDs[count] = &(*curRecord)->formID;
    ++count;
    return stop;
    }

EditorIDRecordRetriever::EditorIDRecordRetriever(STRINGARRAY _RecordEditorIDs):
    RecordOp(),
    RecordEditorIDs(_RecordEditorIDs)
    {
    //
    }

EditorIDRecordRetriever::~EditorIDRecordRetriever()
    {
    //
    }

bool EditorIDRecordRetriever::Accept(Record **curRecord)
    {
    STRING editorID = (STRING )(*curRecord)->GetField(5);
    if(editorID != NULL)
        {
        RecordEditorIDs[count] = editorID;
        ++count;
        }
    return stop;
    }

RecordMasterChecker::FormIDMasterChecker::FormIDMasterChecker(const UINT8 (&_ExpandTable)[255], const UINT8 &_MasterIndex):
    FormIDOp(),
    ExpandTable(_ExpandTable),
    MasterIndex(_MasterIndex)
    {
    //
    }

RecordMasterChecker::FormIDMasterChecker::~FormIDMasterChecker()
    {
    //
    }

bool RecordMasterChecker::FormIDMasterChecker::Accept(UINT32 &curFormID)
    {
    //Short-circuit the function if the master has already been found to be in use
    if(result)
        return result;

    //Any formID <= 0x800 is hardcoded in the engine and doesn't 'belong' to a mod.
    if(curFormID < END_HARDCODED_IDS)
        return false;

    //Check if the master index is in use
    result = ExpandTable[MasterIndex] == (curFormID >> 24);

    return result;
    }

bool RecordMasterChecker::FormIDMasterChecker::AcceptMGEF(UINT32 &curMgefCode)
    {
    //Short-circuit the function if the master has already been found to be in use
    if(result)
        return result;

    //Check if the master index is in use
    result = ExpandTable[MasterIndex] == (curMgefCode & 0x000000FF);

    return result;
    }

RecordMasterChecker::RecordMasterChecker(FormIDHandlerClass &_FormIDHandler, const UINT8 &_MasterIndex):
    RecordOp(),
    reader(_FormIDHandler),
    checker(_FormIDHandler.ExpandTable, _MasterIndex)
    {
    //
    }

RecordMasterChecker::~RecordMasterChecker()
    {
    //
    }

bool RecordMasterChecker::Accept(Record **curRecord)
    {
    if(stop)
        return stop;

    //Ensure the record is read
    reader.Accept(curRecord);

    //Perform the swap
    (*curRecord)->VisitFormIDs(checker);
    stop = checker.GetResult();

    //If the record was read unload it again
    if(reader.GetResult())
        (*curRecord)->Unload();

    return stop;
    }

RecordFormIDSwapper::FormIDSwapper::FormIDSwapper(const UINT32 &_FormIDToMatch, const UINT32 &_FormIDToSwap, FormIDHandlerClass &_FormIDHandler):
    FormIDOp(),
    FormIDToMatch(_FormIDToMatch),
    FormIDToSwap(_FormIDToSwap),
    checker(_FormIDHandler)
    {
    //
    }

RecordFormIDSwapper::FormIDSwapper::~FormIDSwapper()
    {
    //
    }

bool RecordFormIDSwapper::FormIDSwapper::Accept(UINT32 &curFormID)
    {
    result = curFormID == FormIDToMatch;
    if(result)
        {
        curFormID = FormIDToSwap;
        checker.Accept(curFormID);
        ++count;
        }
    return stop;
    }

bool RecordFormIDSwapper::FormIDSwapper::AcceptMGEF(UINT32 &curMgefCode)
    {
    result = curMgefCode == FormIDToMatch;
    if(result)
        {
        curMgefCode = FormIDToSwap;
        checker.Accept(curMgefCode);
        ++count;
        }
    return stop;
    }

RecordFormIDSwapper::RecordFormIDSwapper(const UINT32 &_FormIDToMatch, const UINT32 &_FormIDToSwap, FormIDHandlerClass &_FormIDHandler):
    RecordOp(),
    reader(_FormIDHandler),
    swapper(_FormIDToMatch, _FormIDToSwap, _FormIDHandler)
    {
    //
    }

RecordFormIDSwapper::~RecordFormIDSwapper()
    {
    //
    }

bool RecordFormIDSwapper::Accept(Record **curRecord)
    {
    //Ensure the record is read
    reader.Accept(curRecord);

    //Perform the swap
    stop = (*curRecord)->VisitFormIDs(swapper);

    //Check if anything was swapped
    if(count != swapper.GetCount())
        {
        //If so, mark the record as changed
        (*curRecord)->recData = NULL;
        count = swapper.GetCount();
        }

    //If the record was read, but not changed, unload it again
    if(reader.GetResult() && (*curRecord)->recData != NULL)
        (*curRecord)->Unload();

    return stop;
    }

Record::Record(bool _IsNewRecord):
    flags(0),
    formID(0),
    flagsUnk(0),
    recData(NULL)
    {
    IsLoaded(_IsNewRecord);
    }

Record::~Record()
    {
    //
    }

SINT32 Record::GetOtherFieldType(const UINT32 Field)
    {
    return UNKNOWN_FIELD;
    }

SINT32 Record::GetFieldType(const UINT32 Field)
    {
    switch(Field)
        {
        case 0: //recType
            return GetType();
        //case 1: //size
        //    return UINT_FIELD;
        case 2: //flags1
            return UINT_FIELD;
        case 3: //fid
            return FID_FIELD;
        case 4: //flags2
            return UINT_FIELD;
        case 5: //eid
            return STRING_FIELD;
        default:
            return GetOtherFieldType(Field);
        }
    }

void * Record::GetOtherField(const UINT32 Field)
    {
    return NULL;
    }

void * Record::GetField(const UINT32 Field)
    {
    switch(Field)
        {
        //case 0: //recType
        //    return &Header.Header.type;
        //case 1: //size
        //    return &Header.Header.size;
        case 2: //flags1
            return &flags;
        case 3: //fid
            return &formID;
        case 4: //flags2
            return &flagsUnk;
        default:
            return GetOtherField(Field);
        }
    }

void Record::SetOtherField(const UINT32 Field, UINT32 FieldValue)
    {
    return;
    }

void Record::SetField(const UINT32 Field, UINT32 FieldValue)
    {
    switch(Field)
        {
        case 2: //flags1
            SetHeaderFlagMask(FieldValue);
            return;
        //case 3: //fid
        //    formID = FieldValue;
        //    return;
        case 4: //flags2
            flagsUnk = FieldValue;
            return;
        default:
            SetOtherField(Field, FieldValue);
            return;
        }
    }

void Record::SetField(const UINT32 Field, STRINGARRAY FieldValue, UINT32 nSize)
    {
    return;
    }

void Record::SetField(const UINT32 Field, UINT8ARRAY FieldValue, UINT32 nSize)
    {
    return;
    }

void Record::SetField(const UINT32 Field, FLOAT32 FieldValue)
    {
    return;
    }

void Record::SetField(const UINT32 Field, SINT32 FieldValue)
    {
    return;
    }

void Record::SetField(const UINT32 Field, STRING FieldValue)
    {
    return;
    }

void Record::SetField(const UINT32 Field, SINT8 FieldValue)
    {
    return;
    }

void Record::SetField(const UINT32 Field, UINT8 FieldValue)
    {
    return;
    }

void Record::SetField(const UINT32 Field, UINT32 FieldValue[], UINT32 nSize)
    {
    return;
    }

void Record::SetField(const UINT32 Field, SINT16 FieldValue)
    {
    return;
    }

void Record::SetField(const UINT32 Field, UINT16 FieldValue)
    {
    return;
    }

UINT32 Record::GetFieldArraySize(const UINT32 Field)
    {
    return 0;
    }

UINT32 Record::GetListSize(const UINT32 Field)
    {
    return 0;
    }

UINT32 Record::GetListX2Size(const UINT32 subField, const UINT32 listIndex, const UINT32 listField)
    {
    return 0;
    }

UINT32 Record::GetListX3Size(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field)
    {
    return 0;
    }

UINT32 Record::GetListArraySize(const UINT32 subField, const UINT32 listIndex, const UINT32 listField)
    {
    return 0;
    }

UINT32 Record::GetListX2ArraySize(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field)
    {
    return 0;
    }

UINT32 Record::GetListX3ArraySize(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field)
    {
    return 0;
    }

SINT32 Record::GetListFieldType(const UINT32 subField, const UINT32 listField)
    {
    return UNKNOWN_FIELD;
    }

SINT32 Record::GetListX2FieldType(const UINT32 subField, const UINT32 listField, const UINT32 listX2Field)
    {
    return UNKNOWN_FIELD;
    }

SINT32 Record::GetListX3FieldType(const UINT32 subField, const UINT32 listField, const UINT32 listX2Field, const UINT32 listX3Field)
    {
    return UNKNOWN_FIELD;
    }

void * Record::GetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField)
    {
    return NULL;
    }

void * Record::GetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field)
    {
    return NULL;
    }

void * Record::GetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field)
    {
    return NULL;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, UINT8ARRAY FieldValue, UINT32 nSize)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, STRINGARRAY FieldValue, UINT32 nSize)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, SINT8 FieldValue)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, UINT8 FieldValue)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, STRING FieldValue)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, SINT16 FieldValue)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, UINT16 FieldValue)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, SINT32 FieldValue)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, UINT32 FieldValue)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, UINT32 FieldValue[], UINT32 nSize)
    {
    return;
    }

void Record::SetListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, FLOAT32 FieldValue)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, UINT8ARRAY FieldValue, UINT32 nSize)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, STRINGARRAY FieldValue, UINT32 nSize)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, SINT8 FieldValue)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, UINT8 FieldValue)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, STRING FieldValue)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, SINT16 FieldValue)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, UINT16 FieldValue)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, SINT32 FieldValue)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, UINT32 FieldValue)
    {
    return;
    }

void Record::SetListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, FLOAT32 FieldValue)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, UINT8ARRAY FieldValue, UINT32 nSize)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, STRINGARRAY FieldValue, UINT32 nSize)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, SINT8 FieldValue)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, UINT8 FieldValue)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, STRING FieldValue)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, SINT16 FieldValue)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, UINT16 FieldValue)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, SINT32 FieldValue)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, UINT32 FieldValue)
    {
    return;
    }

void Record::SetListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, FLOAT32 FieldValue)
    {
    return;
    }

void Record::GetFieldArray(const UINT32 Field, void **FieldValues)
    {
    *FieldValues = NULL;
    return;
    }

void Record::GetListArray(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, void **FieldValues)
    {
    *FieldValues = NULL;
    return;
    }

void Record::GetListX2Array(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, void **FieldValues)
    {
    *FieldValues = NULL;
    return;
    }

void Record::GetListX3Array(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field, void **FieldValues)
    {
    *FieldValues = NULL;
    return;
    }

SINT32 Record::CreateListElement(const UINT32 subField)
    {
    return -1;
    }

SINT32 Record::CreateListX2Element(const UINT32 subField, const UINT32 listIndex, const UINT32 listField)
    {
    return -1;
    }

SINT32 Record::CreateListX3Element(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field)
    {
    return -1;
    }

SINT32 Record::DeleteListElement(const UINT32 subField)
    {
    return -1;
    }

SINT32 Record::DeleteListX2Element(const UINT32 subField, const UINT32 listIndex, const UINT32 listField)
    {
    return -1;
    }

SINT32 Record::DeleteListX3Element(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field)
    {
    return -1;
    }

SINT32 Record::DeleteField(const UINT32 Field)
    {
    return -1;
    }

SINT32 Record::DeleteListField(const UINT32 subField, const UINT32 listIndex, const UINT32 listField)
    {
    return -1;
    }

SINT32 Record::DeleteListX2Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field)
    {
    return -1;
    }

SINT32 Record::DeleteListX3Field(const UINT32 subField, const UINT32 listIndex, const UINT32 listField, const UINT32 listX2Index, const UINT32 listX2Field, const UINT32 listX3Index, const UINT32 listX3Field)
    {
    return -1;
    }

UINT32 Record::GetParentType()
    {
    return eUnknown;
    }

bool Record::IsKeyedByEditorID()
    {
    return false;
    }

bool Record::HasSubRecords()
    {
    return false;
    }

bool Record::VisitSubRecords(const UINT32 &RecordType, RecordOp &op)
    {
    return false;
    }

bool Record::VisitFormIDs(FormIDOp &op)
    {
    return false;
    }

bool Record::Read()
    {
    if(IsLoaded() || recData == NULL)
        return false;
    UINT32 recSize = GetSize();
    if (IsCompressed())
        {
        unsigned char localBuffer[BUFFERSIZE];
        unsigned char *buffer = NULL;
        UINT32 expandedRecSize = *(UINT32*)recData;
        if(expandedRecSize >= BUFFERSIZE)
            buffer = new unsigned char[expandedRecSize];
        else
            buffer = &localBuffer[0];
        uncompress(buffer, (uLongf*)&expandedRecSize, &recData[4], recSize - 4);
        ParseRecord(buffer, expandedRecSize);
        if(buffer != &localBuffer[0])
            delete [] buffer;
        }
    else
        ParseRecord(recData, recSize);
    return true;
    }

UINT32 Record::Write(_FileHandler &SaveHandler, FormIDHandlerClass &FormIDHandler)
    {
    //if masters have changed, all formIDs have to be updated...
    //so the record can't just be written as is.
    UINT32 recSize = 0;
    if(!FormIDHandler.MastersChanged() && recData != NULL)
        {
        recSize = GetSize();
        SaveHandler.write(recData - 20, recSize + 20);
        Unload();
        return recSize + 20;
        }
    //RecordReader reader(FormIDHandler);
    //reader.Accept((Record **)&this);
    FormIDResolver expander(FormIDHandler.ExpandTable);
    if(Read())
        VisitFormIDs(expander);
    if(IsDeleted())
        recSize = 0;
    else
        recSize = GetSize(true);
    FormIDResolver collapser(FormIDHandler.CollapseTable);
    collapser.Accept(formID);
    VisitFormIDs(collapser);

    IsLoaded(false);
    UINT32 recType = GetType();
    SaveHandler.write(&recType, 4);
    SaveHandler.write(&recSize, 4);
    SaveHandler.write(&flags, 4);
    SaveHandler.write(&formID, 4);
    SaveHandler.write(&flagsUnk, 4);
    IsLoaded(true);

    if(!IsDeleted())
        {
        //IsCompressed(true); //Test code
        if(IsCompressed())
            {
            //printf("Compressed: %08X\n", formID);
            UINT32 recStart = SaveHandler.tell();
            UINT32 compSize = compressBound(recSize);
            unsigned char *compBuffer = new unsigned char[compSize + 4];
            SaveHandler.reserveBuffer(compSize + 4);
            WriteRecord(SaveHandler);
            memcpy(compBuffer, &recSize, 4);
            if(SaveHandler.IsCached(recStart) && ((SaveHandler.UnusedCache() + recSize) >= compSize))
                compress2(compBuffer + 4, &compSize, SaveHandler.getBuffer(recStart), recSize, 6);
            else
                {
                SaveHandler.flush();
                printf("Not in cache, written improperly!\n  Size: %u\n", recSize);
                return recSize + 20;
                }
            SaveHandler.set_used((compSize + 4) - recSize);
            recSize = compSize + 4;
            SaveHandler.writeAt(recStart - 16, &recSize, 4);
            SaveHandler.writeAt(recStart, compBuffer, recSize);
            delete []compBuffer;
            }
        else
            WriteRecord(SaveHandler);
        }
    expander.Accept(formID);
    if(recData == NULL)
        VisitFormIDs(expander);
    else
        Unload();
    return recSize + 20;
    }

bool Record::IsDeleted() const
    {
    return (flags & fIsDeleted) != 0;
    }

void Record::IsDeleted(bool value)
    {
    if(value)
        flags |= fIsDeleted;
    else
        flags &= ~fIsDeleted;
    }

bool Record::IsBorderRegion()
    {
    return (flags & fIsBorderRegion) != 0;
    }

void Record::IsBorderRegion(bool value)
    {
    if(value)
        flags |= fIsBorderRegion;
    else
        flags &= ~fIsBorderRegion;
    }

bool Record::IsTurnOffFire()
    {
    return (flags & fTurnOffFire) != 0;
    }

void Record::IsTurnOffFire(bool value)
    {
    if(value)
        flags |= fTurnOffFire;
    else
        flags &= ~fTurnOffFire;
    }

bool Record::IsCastsShadows()
    {
    return (flags & fCastsShadows) != 0;
    }

void Record::IsCastsShadows(bool value)
    {
    if(value)
        flags |= fCastsShadows;
    else
        flags &= ~fCastsShadows;
    }

bool Record::IsQuestOrPersistent()
    {
    return (flags & fIsQuestOrPersistent) != 0;
    }

void Record::IsQuestOrPersistent(bool value)
    {
    if(value)
        flags |= fIsQuestOrPersistent;
    else
        flags &= ~fIsQuestOrPersistent;
    }

bool Record::IsQuest()
    {
    return (flags & fIsQuestOrPersistent) != 0;
    }

void Record::IsQuest(bool value)
    {
    if(value)
        flags |= fIsQuestOrPersistent;
    else
        flags &= ~fIsQuestOrPersistent;
    }

bool Record::IsPersistent()
    {
    return (flags & fIsQuestOrPersistent) != 0;
    }

void Record::IsPersistent(bool value)
    {
    if(value)
        flags |= fIsQuestOrPersistent;
    else
        flags &= ~fIsQuestOrPersistent;
    }

bool Record::IsInitiallyDisabled()
    {
    return (flags & fIsInitiallyDisabled) != 0;
    }

void Record::IsInitiallyDisabled(bool value)
    {
    if(value)
        flags |= fIsInitiallyDisabled;
    else
        flags &= ~fIsInitiallyDisabled;
    }

bool Record::IsIgnored()
    {
    return (flags & fIsIgnored) != 0;
    }

void Record::IsIgnored(bool value)
    {
    if(value)
        flags |= fIsIgnored;
    else
        flags &= ~fIsIgnored;
    }

bool Record::IsVisibleWhenDistant()
    {
    return (flags & fIsVisibleWhenDistant) != 0;
    }

void Record::IsVisibleWhenDistant(bool value)
    {
    if(value)
        flags |= fIsVisibleWhenDistant;
    else
        flags &= ~fIsVisibleWhenDistant;
    }

bool Record::IsVWD()
    {
    return (flags & fIsVisibleWhenDistant) != 0;
    }

void Record::IsVWD(bool value)
    {
    if(value)
        flags |= fIsVisibleWhenDistant;
    else
        flags &= ~fIsVisibleWhenDistant;
    }

bool Record::IsTemporary()
    {
    return (flags & (fIsVisibleWhenDistant | fIsQuestOrPersistent)) != 0;
    }

void Record::IsTemporary(bool value)
    {
    if(value)
        {
        flags &= ~fIsVisibleWhenDistant;
        flags &= ~fIsQuestOrPersistent;
        }
    else
        flags |= fIsQuestOrPersistent;
    }

bool Record::IsDangerousOrOffLimits()
    {
    return (flags & fIsDangerousOrOffLimits) != 0;
    }

void Record::IsDangerousOrOffLimits(bool value)
    {
    if(value)
        flags |= fIsDangerousOrOffLimits;
    else
        flags &= ~fIsDangerousOrOffLimits;
    }

bool Record::IsDangerous()
    {
    return (flags & fIsDangerousOrOffLimits) != 0;
    }

void Record::IsDangerous(bool value)
    {
    if(value)
        flags |= fIsDangerousOrOffLimits;
    else
        flags &= ~fIsDangerousOrOffLimits;
    }

bool Record::IsOffLimits()
    {
    return (flags & fIsDangerousOrOffLimits) != 0;
    }

void Record::IsOffLimits(bool value)
    {
    if(value)
        flags |= fIsDangerousOrOffLimits;
    else
        flags &= ~fIsDangerousOrOffLimits;
    }

bool Record::IsCompressed()
    {
    return (flags & fIsCompressed) != 0;
    }

void Record::IsCompressed(bool value)
    {
    if(value)
        flags |= fIsCompressed;
    else
        flags &= ~fIsCompressed;
    }

bool Record::IsCantWait()
    {
    return (flags & fCantWait) != 0;
    }

void Record::IsCantWait(bool value)
    {
    if(value)
        flags |= fCantWait;
    else
        flags &= ~fCantWait;
    }

bool Record::IsHeaderFlagMask(UINT32 Mask, bool Exact)
    {
    if(Exact)
        return (flags & Mask) == Mask;
    else
        return (flags & Mask) != 0;
    }

void Record::SetHeaderFlagMask(UINT32 Mask)
    {
    bool loaded = IsLoaded();
    flags = Mask;
    IsLoaded(loaded);
    }

bool Record::IsLoaded()
    {
    return (flags & _fIsLoaded) != 0;
    }

void Record::IsLoaded(bool value)
    {
    if(value)
        flags |= _fIsLoaded;
    else
        flags &= ~_fIsLoaded;
    }