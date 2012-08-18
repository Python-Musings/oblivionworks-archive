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
// CBash.h
//#define _UNICODE
#include "MacroDefinitions.h"
#include "Collection.h"

//Exported Functions
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Version info functions
EXPORT_FUNC UINT32 GetVersionMajor();
EXPORT_FUNC UINT32 GetVersionMinor();
EXPORT_FUNC UINT32 GetVersionRevision();
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Logging action functions
#ifdef CBASH_USE_LOGGING
    EXPORT_FUNC SINT32 SetLogging(Collection *CollectionID, SINT32 (*_LoggingCallback)(const STRING), UINT32 LoggingLevel, UINT32 LoggingFlags);
#endif
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Collection action functions
EXPORT_FUNC Collection * CreateCollection(STRING const ModsPath, const UINT32 CollectionType);
EXPORT_FUNC SINT32 DeleteCollection(Collection *CollectionID);
EXPORT_FUNC SINT32 LoadCollection(Collection *CollectionID);
EXPORT_FUNC SINT32 UnloadCollection(Collection *CollectionID);
EXPORT_FUNC SINT32 DeleteAllCollections();
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Mod action functions
EXPORT_FUNC SINT32 AddMod(Collection *CollectionID, STRING const ModName, const UINT32 ModFlagsField);
EXPORT_FUNC SINT32 LoadMod(Collection *CollectionID, ModFile *ModID);
EXPORT_FUNC SINT32 UnloadMod(Collection *CollectionID, ModFile *ModID);
EXPORT_FUNC SINT32 CleanModMasters(Collection *CollectionID, ModFile *ModID);
EXPORT_FUNC SINT32 RemoveModMasters(Collection *CollectionID, ModFile *ModID, STRINGARRAY MasterNames, UINT32 MasterCount);
EXPORT_FUNC SINT32 SaveMod(Collection *CollectionID, ModFile *ModID, const bool CloseCollection);
////////////////////////////////////////////////////////////////////////
//Mod info functions
EXPORT_FUNC SINT32 GetAllNumMods(Collection *CollectionID);
EXPORT_FUNC SINT32 GetAllModIDs(Collection *CollectionID, MODIDARRAY ModIDs);
EXPORT_FUNC SINT32 GetLoadOrderNumMods(Collection *CollectionID);
EXPORT_FUNC SINT32 GetLoadOrderModIDs(Collection *CollectionID, MODIDARRAY ModIDs);
EXPORT_FUNC STRING GetFileNameByID(Collection *CollectionID, ModFile *ModID);
EXPORT_FUNC STRING GetFileNameByLoadOrder(Collection *CollectionID, const UINT32 ModIndex);
EXPORT_FUNC STRING GetModNameByID(Collection *CollectionID, ModFile *ModID);
EXPORT_FUNC STRING GetModNameByLoadOrder(Collection *CollectionID, const UINT32 ModIndex);
EXPORT_FUNC ModFile * GetModIDByName(Collection *CollectionID, STRING const ModName);
EXPORT_FUNC ModFile * GetModIDByLoadOrder(Collection *CollectionID, const UINT32 ModIndex);
EXPORT_FUNC SINT32 GetModLoadOrderByName(Collection *CollectionID, STRING const ModName);
EXPORT_FUNC SINT32 GetModLoadOrderByID(Collection *CollectionID, ModFile *ModID);
EXPORT_FUNC STRING GetLongIDName(Collection *CollectionID, ModFile *ModID, const UINT8 ModIndex);
//EXPORT_FUNC SINT32 GetShortIDIndex(Collection *CollectionID, const SINT32 ModID, STRING const ModName);
EXPORT_FUNC UINT32 IsModEmpty(Collection *CollectionID, ModFile *ModID);
EXPORT_FUNC SINT32 GetModNumTypes(Collection *CollectionID, ModFile *ModID);
EXPORT_FUNC void   GetModTypes(Collection *CollectionID, ModFile *ModID, UINT32ARRAY RecordTypes);
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Record action functions
EXPORT_FUNC Record * CreateRecord(Collection *CollectionID, ModFile *ModID, const UINT32 RecordType, const FORMID RecordFormID, STRING const RecordEditorID, Record *ParentID, const UINT32 CreateFlags);
EXPORT_FUNC SINT32 DeleteRecord(Collection *CollectionID, ModFile *ModID, Record *RecordID, Record *ParentID);
EXPORT_FUNC Record * CopyRecord(Collection *CollectionID, ModFile *ModID, Record *RecordID, ModFile *DestModID, Record *DestParentID, const FORMID DestRecordFormID, STRING const DestRecordEditorID, const UINT32 CreateFlags);
EXPORT_FUNC SINT32 UnloadRecord(Collection *CollectionID, ModFile *ModID, Record *RecordID);
EXPORT_FUNC SINT32 SetRecordIdentifiers(Collection *CollectionID, ModFile *ModID, Record *RecordID, const FORMID FormID, STRING const EditorID);
////////////////////////////////////////////////////////////////////////
//Record info functions
EXPORT_FUNC Record * GetRecordID(Collection *CollectionID, ModFile *ModID, const FORMID RecordFormID, STRING const RecordEditorID);
EXPORT_FUNC SINT32 GetNumRecords(Collection *CollectionID, ModFile *ModID, const UINT32 RecordType);
EXPORT_FUNC SINT32 GetRecordIDs(Collection *CollectionID, ModFile *ModID, const UINT32 RecordType, RECORDIDARRAY RecordIDs);
EXPORT_FUNC SINT32 IsRecordWinning(Collection *CollectionID, ModFile *ModID, Record *RecordID, const bool GetExtendedConflicts);
EXPORT_FUNC SINT32 GetNumRecordConflicts(Collection *CollectionID, Record *RecordID, const bool GetExtendedConflicts);
EXPORT_FUNC SINT32 GetRecordConflicts(Collection *CollectionID, Record *RecordID, MODIDARRAY ModIDs, RECORDIDARRAY RecordIDs, const bool GetExtendedConflicts);
EXPORT_FUNC SINT32 GetRecordHistory(Collection *CollectionID, ModFile *ModID, Record *RecordID, MODIDARRAY ModIDs, RECORDIDARRAY RecordIDs);
EXPORT_FUNC SINT32 CompareIdenticalRecords(Record *RecordID1, Record *RecordID2);
////////////////////////////////////////////////////////////////////////
//Mod or Record action functions
EXPORT_FUNC SINT32 UpdateReferences(Collection *CollectionID, ModFile *ModID, Record *RecordID, const FORMID FormIDToReplace, const FORMID ReplacementFormID);
////////////////////////////////////////////////////////////////////////
//Mod or Record info functions
EXPORT_FUNC SINT32 GetNumReferences(Collection *CollectionID, ModFile *ModID, Record *RecordID, const FORMID FormIDToMatch);
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Field action functions
EXPORT_FUNC void   SetField(Collection *CollectionID, ModFile *ModID, Record *RecordID, FIELD_IDENTIFIERS, void *FieldValue, const UINT32 ArraySize);
EXPORT_FUNC void   DeleteField(Collection *CollectionID, ModFile *ModID, Record *RecordID, FIELD_IDENTIFIERS);
////////////////////////////////////////////////////////////////////////
//Field info functions
EXPORT_FUNC UINT32 GetFieldAttribute(Collection *CollectionID, ModFile *ModID, Record *RecordID, FIELD_IDENTIFIERS, const UINT32 WhichAttribute);
EXPORT_FUNC void * GetField(Collection *CollectionID, ModFile *ModID, Record *RecordID, FIELD_IDENTIFIERS, void **FieldValues);
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////