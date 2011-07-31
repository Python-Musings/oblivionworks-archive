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
// Collection.cpp
#include "Collection.h"
#include <direct.h>
#include <sys/utime.h>
//#include <boost/threadpool.hpp>

bool compModRecordPair(std::pair<ModFile *, Record *> *&lhs, std::pair<ModFile *, Record *> *&rhs)
    {
    return lhs->first->ModID < rhs->first->ModID;
    }

bool sortMod(ModFile *lhs, ModFile *rhs)
    {
    //Esp's sort after esm's
    //Non-existent esms sort before existing esps
    //Non-existent esms retain their relative load order
    //Existing esps sort by modified date
    //New esps load last
    if(lhs->TES4.IsESM())
        {
        if(rhs->TES4.IsESM())
            {
            if(lhs->ModTime == 0)
                {
                if(rhs->ModTime == 0)
                    return true;
                return false;
                }
            if(rhs->ModTime == 0)
                return true;
            return lhs->ModTime < rhs->ModTime;
            }
        return true;
        }
    if(rhs->TES4.IsESM())
        return false;
    if(lhs->Flags.IsIgnoreExisting || lhs->ModTime == 0)
        {
        if(rhs->Flags.IsIgnoreExisting || rhs->ModTime == 0)
            return true;
        return false;
        }
    if(rhs->Flags.IsIgnoreExisting || rhs->ModTime == 0)
        return false;
    return lhs->ModTime < rhs->ModTime;
    }

Collection::Collection(STRING const &ModsPath, UINT32 _CollectionType):
    ModsDir(NULL),
    IsLoaded(false),
    CollectionType((collectionType)_CollectionType)
    {
    ModsDir = new char[strlen(ModsPath)+1];
    strcpy_s(ModsDir, strlen(ModsPath)+1, ModsPath);
    }

Collection::~Collection()
    {
    delete []ModsDir;
    for(UINT32 p = 0; p < ModFiles.size(); p++)
        delete ModFiles[p];
    for(UINT32 p = 0; p < Expanders.size(); p++)
        delete Expanders[p];
    //LoadOrder255 is shared with ModFiles, so no deleting
    }

SINT32 Collection::AddMod(STRING const &_FileName, ModFlags &flags)
    {
    _chdir(ModsDir);
    //Mods may not be added after collection is loaded.
    //Prevent loading mods more than once

    STRING ModName = DeGhostModName(_FileName);

    if(IsLoaded || IsModAdded(ModName ? ModName : _FileName))
        {
        delete []ModName;
        return -1;
        }
    STRING FileName = new char[strlen(_FileName) + 1];
    strcpy_s(FileName, strlen(_FileName) + 1, _FileName);
    ModName = ModName ? ModName : FileName;

    switch(CollectionType)
        {
        case eTES4:
            ModFiles.push_back(new TES4File(FileName, ModName, flags.GetFlags()));
            break;
        case eFO3:
            printf("Unimplemented\n");
            delete []ModName;
            throw 1;
            break;
        case eFNV:
            printf("Unimplemented\n");
            delete []ModName;
            throw 1;
            break;
        default:
            delete []ModName;
            break;
        }
    return 1;
    }

bool Collection::IsModAdded(STRING const &ModName)
    {
    for(UINT32 p = 0;p < ModFiles.size();p++)
        if(_stricmp(ModName, ModFiles[p]->ReadHandler.getModName()) == 0)
            return true;
    return false;
    }

SINT32 Collection::SaveMod(ModFile *&curModFile, bool CloseCollection)
    {
    if(!curModFile->Flags.IsSaveable)
        return -1;

    //Saves to a temp file, then if successful, renames any existing files, and then renames temp file to ModName
    if(CloseCollection)
        {
        //clear up some memory
        EditorID_ModFile_Record.clear();
        FormID_ModFile_Record.clear();
        ExtendedEditorID_ModFile_Record.clear();
        ExtendedFormID_ModFile_Record.clear();
        }

    _chdir(ModsDir);

    char tName[L_tmpnam_s];
    errno_t err;
    time_t ltime;
    struct tm currentTime;
    struct stat oTimes;
    struct _utimbuf originalTimes;

    static time_t lastSave = time(NULL);

    STRING backupName = NULL;
    UINT32 bakAttempts = 0, bakSize = 0;

    err = tmpnam_s(tName, L_tmpnam_s);
    if (err)
        {
        printf("Error occurred creating unique filename.\n");
        return -1;
        }
    tName[0] = 'x';

    try
        {
        //Save the mod to temp file, using FileBuffer to write in chunks
        curModFile->Save(tName, Expanders, CloseCollection);

        //Rename any existing files to a datestamped backup
        time(&ltime);
        if(ltime - lastSave < 60)
            ltime = lastSave + 60;
        lastSave =  ltime;

        err = _localtime64_s(&currentTime, &ltime);
        if (err)
            {
            printf(" _localtime64_s failed due to invalid arguments.");
            return -1;
            }
        originalTimes.actime = ltime;
        originalTimes.modtime = ltime;

        if(curModFile->ReadHandler.exists())
            {
            stat(curModFile->ReadHandler.getFileName(), &oTimes);
            originalTimes.actime = oTimes.st_atime;
            originalTimes.modtime = oTimes.st_mtime;

            bakSize = (UINT32)strlen(curModFile->ReadHandler.getFileName()) + (UINT32)strlen(".bak.XXXX_XX_XX_XX_XX_XX") + 1;
            backupName = new char[bakSize];
            strcpy_s(backupName, bakSize, curModFile->ReadHandler.getFileName());
            strftime(backupName + strlen(curModFile->ReadHandler.getFileName()), bakSize, ".bak.%Y_%m_%d_%H_%M_%S", &currentTime );

            //If the backup name already exists, wait in 1 second increments until a free name is available
            //If 10 tries pass, then give up.
            bakAttempts = 0;
            while(FileExists(backupName))
                {
                if(bakAttempts > 10)
                    break;
                bakAttempts++;
                currentTime.tm_min++;
                mktime(&currentTime);
                strftime(backupName + strlen(curModFile->ReadHandler.getFileName()), bakSize, ".bak.%Y_%m_%d_%H_%M_%S", &currentTime);
                };
            err = rename(curModFile->ReadHandler.getFileName(), backupName);
            if(err != 0)
                printf("Error renaming \"%s\" to \"%s\"\n", curModFile->ReadHandler.getFileName(), backupName);
            delete []backupName;
            }

        //Rename temp file to the original ModName
        //If it fails, try to save it to a datestamped .new extension and inform the failure
        err = rename(tName, curModFile->ReadHandler.getFileName());
        if(err != 0)
            {
            bakSize = (UINT32)strlen(curModFile->ReadHandler.getFileName()) + (UINT32)strlen(".new.XXXX_XX_XX_XX_XX_XX") + 1;
            backupName = new char[bakSize];

            strcpy_s(backupName, bakSize, curModFile->ReadHandler.getFileName());
            strftime(backupName+strlen(curModFile->ReadHandler.getFileName()), bakSize, ".new.%Y_%m_%d_%H_%M_%S", &currentTime );

            //If the backup name already exists, wait in 1 second increments until a free name is available
            //If 10 tries pass, then give up.
            bakAttempts = 0;
            while(FileExists(backupName))
                {
                if(bakAttempts > 10)
                    break;
                bakAttempts++;
                currentTime.tm_min++;
                mktime(&currentTime);
                strftime(backupName + strlen(curModFile->ReadHandler.getFileName()), bakSize, ".new.%Y_%m_%d_%H_%M_%S", &currentTime);
                };

            err = rename(tName, backupName);
            if(err != 0)
                {
                printf("Error renaming \"%s\" to \"%s\"\n", tName, backupName);
                delete []backupName;
                return -1;
                }
            else
                {
                printf("Error renaming \"%s\" to \"%s\"\n", tName, curModFile->ReadHandler.getFileName());
                printf("Renamed \"%s\" to \"%s\"\n", tName, backupName);
                _utime(backupName, &originalTimes);
                delete []backupName;
                return 0;
                }
            }
        else
            _utime(curModFile->ReadHandler.getFileName(), &originalTimes);
        }
    catch(...)
        {
        printf("Error saving: %s\n", curModFile->ReadHandler.getFileName());
        if(FileExists(tName))
            {
            remove(tName);
            printf("  Temp file %s removed.\n", tName);
            }
        throw 1;
        return -1;
        }
    return 0;
    }

SINT32 Collection::Load()
    {
    #ifdef CBASH_USE_LOGGING
        CLOGGER;
        BOOST_LOG_FUNCTION();
        BOOST_LOG_SEV(lg, trace) << "Load flags = " << LoadFlags;
    #endif

    ModFile *curModFile = NULL;
    RecordIndexer indexer(EditorID_ModFile_Record, FormID_ModFile_Record);
    RecordIndexer extended_indexer(ExtendedEditorID_ModFile_Record, ExtendedFormID_ModFile_Record);
    bool Preloading = false;

    if(IsLoaded)
        return 0;
    try
        {
        _chdir(ModsDir);
        //Brute force approach to loading all masters
        //Could be done more elegantly with recursion
        //printf("Before Preloading\n");
        do {
            Preloading = false;
            for(UINT32 p = 0; p < (UINT32)ModFiles.size(); ++p)
                {
                curModFile = ModFiles[p];
                curModFile->LoadTES4();
                if(curModFile->Flags.IsAddMasters)
                    {
                    //Any new mods loaded this way inherit their flags
                    ModFlags preloadFlags(curModFile->Flags.GetFlags());
                    preloadFlags.IsNoLoad = !curModFile->Flags.IsLoadMasters;
                    //preloadFlags.IsInLoadOrder = !preloadFlags.IsNoLoad;
                    for(UINT8 x = 0; x < curModFile->TES4.MAST.size(); ++x)
                        Preloading = (AddMod(curModFile->TES4.MAST[x].value, preloadFlags) == 0 || Preloading);
                    }
                }
        }while(Preloading);
        //printf("Load order before sort\n");
        //for(UINT32 x = 0; x < ModFiles.size(); ++x)
        //    printf("%02X: %s\n", x, ModFiles[x]->FileName);
        //printf("\n");
        std::_Insertion_sort(ModFiles.begin(), ModFiles.end(), sortMod);
        std::vector<STRING> strLoadOrder255;
        std::vector<STRING> strTempLoadOrder;
        std::vector< std::vector<STRING> > strAllLoadOrder;
        LoadOrder255.clear(); //shouldn't be needed
        //printf("Load order:\n");
        for(UINT32 p = 0; p < (UINT32)ModFiles.size(); ++p)
            {
            curModFile = ModFiles[p];
            curModFile->ModID = p;
            //printf("ModID %02X: %s", p, curModFile->ReadHandler.getFileName());
            if(curModFile->Flags.IsInLoadOrder)
                {
                if(LoadOrder255.size() >= 255)
                    throw std::exception("Tried to load more than 255 mods.");
                LoadOrder255.push_back(curModFile);
                strLoadOrder255.push_back(curModFile->ReadHandler.getModName());
                //printf(" , OrderID %02X", LoadOrder255.size() - 1);
                }
            else if(!curModFile->Flags.IsIgnoreAbsentMasters) //every mod not in the std load order exists as if it and its masters are the only ones loaded
                {//No need to sort since the masters should be in order well enough
                for(UINT8 x = 0; x < curModFile->TES4.MAST.size(); ++x)
                    strTempLoadOrder.push_back(curModFile->TES4.MAST[x].value);
                strAllLoadOrder.push_back(strTempLoadOrder);
                strTempLoadOrder.clear();
                }
            //printf("\n");
            }
        //printf("Load Set\n");
        UINT8 expandedIndex = 0;
        UINT32 x = 0;
        for(UINT32 p = 0; p < (UINT32)ModFiles.size(); ++p)
            {
            curModFile = ModFiles[p];
            //Loads GRUP and Record Headers.  Fully loads GMST records.
            if(curModFile->Flags.IsInLoadOrder || curModFile->Flags.IsIgnoreAbsentMasters)
                curModFile->FormIDHandler.SetLoadOrder(strLoadOrder255);
            else
                {
                curModFile->FormIDHandler.SetLoadOrder(strAllLoadOrder[x]);
                ++x;
                }
            if(curModFile->Flags.IsSkipNewRecords)
                curModFile->FormIDHandler.CreateFormIDLookup(0xFF);
            else
                {
                if(curModFile->Flags.IsInLoadOrder)
                    {
                    curModFile->FormIDHandler.CreateFormIDLookup(expandedIndex);
                    ++expandedIndex;
                    }
                else
                    curModFile->FormIDHandler.CreateFormIDLookup(curModFile->TES4.MAST.size());
                }
            Expanders.push_back(new FormIDResolver(curModFile->FormIDHandler.ExpandTable, curModFile->FormIDHandler.FileStart, curModFile->FormIDHandler.FileEnd));
            if(curModFile->Flags.IsExtendedConflicts)
                {
                extended_indexer.SetModFile(curModFile);
                curModFile->Load(extended_indexer, Expanders);
                }
            else
                {
                indexer.SetModFile(curModFile);
                curModFile->Load(indexer, Expanders);
                }
            }
        //printf("Loaded\n");
        strAllLoadOrder.clear();
        IsLoaded = true;
        }
    catch(std::exception& e)
        {
        printf("Failed Load %s\n", e.what());
        IsLoaded = false;
        throw;
        return -1;
        }
    catch(...)
        {
        printf("Failed Load?\n");
        IsLoaded = false;
        throw;
        return -1;
        }
    return 0;
    }

SINT32 Collection::Unload()
    {
    Record *curRecord = NULL;
    for(FormID_Iterator it = FormID_ModFile_Record.begin(); it != FormID_ModFile_Record.end(); ++it)
        {
        curRecord = it->second.second;
        if(curRecord != NULL && !curRecord->IsChanged())
            curRecord->Unload();
        }
    for(EditorID_Iterator it = EditorID_ModFile_Record.begin(); it != EditorID_ModFile_Record.end(); ++it)
        {
        curRecord = it->second.second;
        if(curRecord != NULL && !curRecord->IsChanged())
            curRecord->Unload();
        }
    for(FormID_Iterator it = ExtendedFormID_ModFile_Record.begin(); it != ExtendedFormID_ModFile_Record.end(); ++it)
        {
        curRecord = it->second.second;
        if(curRecord != NULL && !curRecord->IsChanged())
            curRecord->Unload();
        }
    for(EditorID_Iterator it = ExtendedEditorID_ModFile_Record.begin(); it != ExtendedEditorID_ModFile_Record.end(); ++it)
        {
        curRecord = it->second.second;
        if(curRecord != NULL && !curRecord->IsChanged())
            curRecord->Unload();
        }
    return 0;
    }

FormID_Iterator Collection::LookupRecord(ModFile *&curModFile, const FORMID &RecordFormID, Record *&curRecord)
    {
    curRecord = NULL;
    FormID_Range range = curModFile->Flags.IsExtendedConflicts ? ExtendedFormID_ModFile_Record.equal_range(RecordFormID) : FormID_ModFile_Record.equal_range(RecordFormID);
    for(; range.first != range.second; ++range.first)
        if(range.first->second.first == curModFile)
            {
            curRecord = range.first->second.second;
            return range.first;
            }
    return curModFile->Flags.IsExtendedConflicts ? ExtendedFormID_ModFile_Record.end() : FormID_ModFile_Record.end();
    }

EditorID_Iterator Collection::LookupRecord(ModFile *&curModFile, STRING const &RecordEditorID, Record *&curRecord)
    {
    curRecord = NULL;
    EditorID_Range range = curModFile->Flags.IsExtendedConflicts ? ExtendedEditorID_ModFile_Record.equal_range(RecordEditorID) : EditorID_ModFile_Record.equal_range(RecordEditorID);
    for(; range.first != range.second; ++range.first)
        if(range.first->second.first == curModFile)
            {
            curRecord = range.first->second.second;
            return range.first;
            }
    return curModFile->Flags.IsExtendedConflicts ? ExtendedEditorID_ModFile_Record.end() : EditorID_ModFile_Record.end();
    }

UINT32 Collection::IsRecordWinning(ModFile *&curModFile, Record *&curRecord, const bool GetExtendedConflicts)
    {
    bool bIsWinning = false;
    if(curModFile->Flags.IsExtendedConflicts && !GetExtendedConflicts)
        return false;

    std::vector<UINT32> sortedConflicts;
    sortedConflicts.reserve(300);  //Enough for the worst case of all 255 loaded mods having conflicts, plus some for extended conflict mods

    if(curRecord->IsKeyedByEditorID())
        {
        STRING RecordEditorID = (STRING)curRecord->GetField(4);
        if(RecordEditorID != NULL)
            {
            for(EditorID_Range range = EditorID_ModFile_Record.equal_range(RecordEditorID); range.first != range.second; ++range.first)
                if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                    sortedConflicts.push_back(range.first->second.first->ModID);
            if(GetExtendedConflicts)
                {
                for(EditorID_Range range = ExtendedEditorID_ModFile_Record.equal_range(RecordEditorID); range.first != range.second; ++range.first)
                    if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                        sortedConflicts.push_back(range.first->second.first->ModID);
                }
            }
        }
    else
        {
        for(FormID_Range range = FormID_ModFile_Record.equal_range(curRecord->formID); range.first != range.second; ++range.first)
            if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                sortedConflicts.push_back(range.first->second.first->ModID);
        if(GetExtendedConflicts)
            {
            for(FormID_Range range = ExtendedFormID_ModFile_Record.equal_range(curRecord->formID); range.first != range.second; ++range.first)
                if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                    sortedConflicts.push_back(range.first->second.first->ModID);
            }
        }

    if(sortedConflicts.size())
        {
        std::sort(sortedConflicts.begin(), sortedConflicts.end());
        bIsWinning = sortedConflicts.back() == curModFile->ModID;
        sortedConflicts.clear();
        }
    return bIsWinning;
    }

UINT32 Collection::GetNumRecordConflicts(Record *&curRecord, const bool GetExtendedConflicts)
    {
    UINT32 count = 0;
    if(curRecord->IsKeyedByEditorID())
        {
        STRING RecordEditorID = (STRING)curRecord->GetField(4);
        if(RecordEditorID != NULL)
            {
            count = EditorID_ModFile_Record.count(RecordEditorID);
            if(GetExtendedConflicts)
                count += ExtendedEditorID_ModFile_Record.count(RecordEditorID);
            }
        }
    else
        {
        count = FormID_ModFile_Record.count(curRecord->formID);
        if(GetExtendedConflicts)
            count += ExtendedFormID_ModFile_Record.count(curRecord->formID);
        }
    return count;
    }

SINT32 Collection::GetRecordConflicts(Record *&curRecord, MODIDARRAY ModIDs, RECORDIDARRAY RecordIDs, const bool GetExtendedConflicts)
    {
    std::vector<std::pair<ModFile *, Record *> *> sortedConflicts;
    sortedConflicts.reserve(300);  //Enough for the worst case of all 255 loaded mods having conflicts, plus some for extended conflict mods
    //std::set<UINT32> conflicts;
    if(curRecord->IsKeyedByEditorID())
        {
        STRING RecordEditorID = (STRING)curRecord->GetField(4);
        if(RecordEditorID != NULL)
            {
            for(EditorID_Range range = EditorID_ModFile_Record.equal_range(RecordEditorID); range.first != range.second; ++range.first)
                if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                    sortedConflicts.push_back(&range.first->second);
            if(GetExtendedConflicts)
                {
                for(EditorID_Range range = ExtendedEditorID_ModFile_Record.equal_range(RecordEditorID); range.first != range.second; ++range.first)
                    if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                        sortedConflicts.push_back(&range.first->second);
                }
            }
        }
    else
        {
        for(FormID_Range range = FormID_ModFile_Record.equal_range(curRecord->formID); range.first != range.second; ++range.first)
            if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                sortedConflicts.push_back(&range.first->second);
        if(GetExtendedConflicts)
            {
            for(FormID_Range range = ExtendedFormID_ModFile_Record.equal_range(curRecord->formID); range.first != range.second; ++range.first)
                if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                    sortedConflicts.push_back(&range.first->second);
            }
        }

    UINT32 y = sortedConflicts.size();
    if(y)
        {
        std::sort(sortedConflicts.begin(), sortedConflicts.end(), compModRecordPair);
        for(UINT32 x = 0; x < y; ++x)
            {
            //if(y > 1) printf("sortedConflicts[%d] = %d\n", x, sortedConflicts[x]->ModID);
            ModIDs[x] = sortedConflicts[y - (x + 1)]->first;
            RecordIDs[x] = sortedConflicts[y - (x + 1)]->second;
            }
        //if(y > 1) 
        //    for(UINT32 x = 0; x < y; ++x)
        //        {
        //        printf("ModIDs[%d] = %d\n", x, ModIDs[x]->ModID);
        //        }
        sortedConflicts.clear();
        }
    return y;
    }

SINT32 Collection::GetRecordHistory(ModFile *&curModFile, Record *&curRecord, MODIDARRAY ModIDs, RECORDIDARRAY RecordIDs)
    {
    if(curModFile->Flags.IsExtendedConflicts)
        return -1;
    std::vector<std::pair<ModFile *, Record *> *> sortedHistory;
    sortedHistory.reserve(10);
    UINT8 curCollapsedIndex = curModFile->FormIDHandler.CollapsedIndex;
    const UINT8 (&CollapseTable)[256] = curModFile->FormIDHandler.CollapseTable;

    if(curRecord->IsKeyedByEditorID())
        {
        STRING RecordEditorID = (STRING)curRecord->GetField(4);
        if(RecordEditorID != NULL)
            {
            for(EditorID_Range range = EditorID_ModFile_Record.equal_range(RecordEditorID); range.first != range.second; ++range.first)
                if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                    if(CollapseTable[range.first->second.first->FormIDHandler.ExpandedIndex] != curCollapsedIndex)
                        sortedHistory.push_back(&range.first->second);
            }
        }
    else
        {
        for(FormID_Range range = FormID_ModFile_Record.equal_range(curRecord->formID); range.first != range.second; ++range.first)
            {
            if(range.first->second.first->Flags.IsInLoadOrder || range.first->second.first->Flags.IsIgnoreAbsentMasters)
                if(CollapseTable[range.first->second.first->FormIDHandler.ExpandedIndex] != curCollapsedIndex)
                    sortedHistory.push_back(&range.first->second);
            }
        }

    UINT32 y = sortedHistory.size();
    if(y)
        {
        std::sort(sortedHistory.begin(), sortedHistory.end(), compModRecordPair);
        for(UINT32 x = 0; x < y; ++x)
            {
            //if(y > 1) printf("sortedHistory[%d] = %d\n", x, sortedHistory[x].first->ModID);
            ModIDs[x] = sortedHistory[x]->first;
            RecordIDs[x] = sortedHistory[x]->second;
            }
        //if(y > 1) 
        //    for(UINT32 x = 0; x < y; ++x)
        //        {
        //        printf("ModIDs[%d] = %d\n", x, ModIDs[x]->ModID);
        //        }
        sortedHistory.clear();
        }
    return y;
    }

UINT32 Collection::NextFreeExpandedFormID(ModFile *&curModFile, UINT32 depth)
    {
    UINT32 curFormID = curModFile->FormIDHandler.NextExpandedFormID();
    FormID_Range range = curModFile->Flags.IsExtendedConflicts ? ExtendedFormID_ModFile_Record.equal_range(curFormID) : FormID_ModFile_Record.equal_range(curFormID);
    //FormID doesn't exist in any mod, so it's free for use
    if(range.first == range.second)
        return curFormID;
    //The formID already exists, so try again (either in that mod, or being injected into that mod)
    //Wrap around and recycle any freed ids.
    if(depth < 0x00FFFFFF)
        return NextFreeExpandedFormID(curModFile, ++depth);
    //All formIDs are in use. Unlikely to ever occur.
    return 0;
    }

Record * Collection::CreateRecord(ModFile *&curModFile, const UINT32 &RecordType, FORMID RecordFormID, STRING const &RecordEditorID, const FORMID &ParentFormID, UINT32 CreateFlags)
    {
    if(!curModFile->Flags.IsInLoadOrder)
        return NULL;

    CreateRecordOptions options(CreateFlags);
    Record *DummyRecord = NULL;
    Record *ParentRecord = NULL;

    //See if the requested record already exists
    if(RecordFormID != 0)
        LookupRecord(curModFile, RecordFormID, DummyRecord);
    else if(RecordEditorID != NULL)
        LookupRecord(curModFile, RecordEditorID, DummyRecord);
    if(DummyRecord != NULL)
        return DummyRecord;

    //Lookup the required data, and ensure it exists
    if(ParentFormID)
        {
        LookupRecord(curModFile, ParentFormID, ParentRecord);
        if(ParentRecord == NULL)
            return NULL;
        }

    //Create the new record
    Record *curRecord = curModFile->CreateRecord(RecordType, RecordEditorID, DummyRecord, ParentRecord, options);
    if(curRecord == NULL)
        return NULL;

    //See if an existing record was returned instead of a new record
    if(curRecord->formID != 0)
        return curRecord;

    //Assign the new record a formID
    //If keyed by editor id, assign a new formID so that FormIDMasterUpdater doesn't add unneeded masters
    curRecord->formID = (RecordFormID == 0 || curRecord->IsKeyedByEditorID()) ? NextFreeExpandedFormID(curModFile) : RecordFormID;

    //Index the new record
    RecordIndexer indexer(curModFile, curModFile->Flags.IsExtendedConflicts ? ExtendedEditorID_ModFile_Record: EditorID_ModFile_Record, curModFile->Flags.IsExtendedConflicts ? ExtendedFormID_ModFile_Record: FormID_ModFile_Record);
    indexer.Accept(curRecord);

    return curRecord;
    }

Record * Collection::CopyRecord(ModFile *&curModFile, Record *&curRecord, ModFile *&DestModFile, const FORMID &DestParentFormID, const FORMID &DestRecordFormID, STRING const &DestRecordEditorID, UINT32 CreateFlags)
    {
    if(!curModFile->Flags.IsInLoadOrder && !curModFile->Flags.IsIgnoreAbsentMasters)
        {
        printf("Source mod is not in the load order and is not ignoring absent masters\n");
        return NULL;
        }

    CreateRecordOptions options(CreateFlags);

    Record *ParentRecord = NULL;
    Record *RecordCopy = NULL;

    if(options.SetAsOverride)
        {
        //See if its trying to copy a record that already exists in the destination mod
        if(curRecord->IsKeyedByEditorID())
            LookupRecord(DestModFile, (STRING)curRecord->GetField(4), RecordCopy);
        else
            LookupRecord(DestModFile, curRecord->formID, RecordCopy);

        if(RecordCopy != NULL)
            return RecordCopy;
        }

    if(DestParentFormID)
        {
        //See if the parent record already exists in the destination mod
        LookupRecord(DestModFile, DestParentFormID, ParentRecord);
        if(ParentRecord == NULL)
            {
            //If it doesn't, try and create it.
            CreateRecordOptions parentOptions;
            parentOptions.SetAsOverride = options.SetAsOverride;
            LookupRecord(curModFile, DestParentFormID, ParentRecord);
            if(ParentRecord == NULL)
                {
                printf("Unable to find destination parent record\n");
                return NULL;
                }
            ParentRecord = CopyRecord(curModFile, ParentRecord, DestModFile, 0, 0, 0, parentOptions.GetFlags());
            if(ParentRecord == NULL)
                {
                printf("Unable to find destination parent record\n");
                return NULL;
                }
            }
        }

    if(curModFile == DestModFile && options.SetAsOverride)
        {
        printf("Source and Destination mods are the same (so can't copy override records to).\n");
        return NULL;
        }

    if(!DestModFile->Flags.IsInLoadOrder && !options.SetAsOverride)
        {
        printf("Destination mod not in load order (so can't copy new records to)\n");
        return NULL;
        }

    //Create the record copy
    RecordCopy = DestModFile->CreateRecord(curRecord->GetType(), DestRecordEditorID ? DestRecordEditorID : (STRING)curRecord->GetField(4), curRecord, ParentRecord, options);
    if(RecordCopy == NULL)
        {
        printf("Unable to copy record\n");
        return NULL;
        }

    //See if an existing record was returned instead of the requested copy
    if(RecordCopy->formID != curRecord->formID)
        return RecordCopy;

    //Give the record a new formID if it isn't an override record
    if(!options.SetAsOverride)
        RecordCopy->formID = DestRecordFormID ? DestRecordFormID : NextFreeExpandedFormID(DestModFile);
    else if(RecordCopy->IsKeyedByEditorID()) //Assign the formID to the destination mod so that FormIDMasterUpdater doesn't add unneeded masters
        RecordCopy->formID = NextFreeExpandedFormID(DestModFile);

    //See if the destination mod masters need updating
    //Ensure the record has been fully read
    //Uses the source mod's formID resolution tables
    RecordReader reader(curModFile->FormIDHandler, Expanders);
    reader.Accept(RecordCopy);

    //Then the destination mod's tables get used so that they can be updated
    FormIDMasterUpdater checker(DestModFile->FormIDHandler);
    checker.Accept(RecordCopy->formID);
    RecordCopy->VisitFormIDs(checker);

    //Index the record
    RecordIndexer indexer(DestModFile, DestModFile->Flags.IsExtendedConflicts ? ExtendedEditorID_ModFile_Record: EditorID_ModFile_Record, DestModFile->Flags.IsExtendedConflicts ? ExtendedFormID_ModFile_Record: FormID_ModFile_Record);
    indexer.Accept(RecordCopy);

    if(reader.GetResult()) //If the record was read, go ahead and unload it
        RecordCopy->Unload();
    return RecordCopy;
    }

SINT32 Collection::DeleteRecord(ModFile *&curModFile, Record *&curRecord, Record *&ParentRecord)
    {
    RecordDeleter deleter(curRecord, curModFile->Flags.IsExtendedConflicts ? ExtendedEditorID_ModFile_Record: EditorID_ModFile_Record, curModFile->Flags.IsExtendedConflicts ? ExtendedFormID_ModFile_Record: FormID_ModFile_Record);

    UINT32 RecordType = curRecord->GetType();
    UINT32 TopType = ParentRecord != NULL ? (ParentRecord->GetParentType() ? ParentRecord->GetParentType() : ParentRecord->GetType()) : RecordType;

    curModFile->VisitRecords(TopType, RecordType, deleter, true);
    return deleter.GetCount();
    }

SINT32 Collection::SetRecordIDs(ModFile *&curModFile, Record *&RecordID, const FORMID &FormID, STRING const &EditorID)
    {
    bool bChangingFormID = RecordID->formID != FormID;
    bool bChangingEditorID = false;
    STRING RecordEditorID = (STRING)RecordID->GetField(4);
    if(RecordEditorID == NULL)
        {
        if(EditorID != NULL)
            bChangingEditorID = true;
        }
    else
        {
        if(EditorID == NULL)
            bChangingEditorID = true;
        else if(_stricmp(RecordEditorID, EditorID) != 0)
            bChangingEditorID = true;
        }
    
    if(!(bChangingFormID || bChangingEditorID))
        return -1;

    FormID_Map &curFormID_Map = curModFile->Flags.IsExtendedConflicts ? ExtendedFormID_ModFile_Record: FormID_ModFile_Record;
    EditorID_Map &curEditorID_Map = curModFile->Flags.IsExtendedConflicts ? ExtendedEditorID_ModFile_Record: EditorID_ModFile_Record;

    bool bDeIndexed = false;

    Record *curRecord = NULL;

    //If the FormID is already in use by the mod, do nothing.
    if(bChangingFormID && FormID != 0)
        LookupRecord(curModFile, FormID, curRecord);

    //If the EditorID is already in use by the mod, do nothing.
    //This only cares about records keyed by editorID since they'd get the most confused
    if(bChangingEditorID && EditorID != NULL)
        LookupRecord(curModFile, EditorID, curRecord);

    if(curRecord != NULL)
        return -1;

    //Lookup the required data, and ensure it exists
    //De-index the record
    FormID_Iterator formID_it = LookupRecord(curModFile, RecordID->formID, curRecord);
    if(curRecord != NULL || formID_it != curFormID_Map.end())
        {
        //Not allowed to set the FormID to 0 if the record is keyed by it
        bChangingFormID = bChangingFormID && FormID != 0;
        if(bChangingFormID)
            {
            curFormID_Map.erase(formID_it);
            bDeIndexed = true;
            }
        }
    else
        {
        EditorID_Iterator editorID_it = LookupRecord(curModFile, RecordEditorID, curRecord);
        if(curRecord != NULL || editorID_it != curEditorID_Map.end())
            {
            //Not allowed to delete the EditorID if the record is keyed by it
            bChangingEditorID = bChangingEditorID && EditorID != NULL;
            if(bChangingEditorID)
                {
                curEditorID_Map.erase(editorID_it);
                bDeIndexed = true;
                }
            }
        else
            return -1;
        }

    //Update the formID
    if(bChangingFormID)
        {
        curRecord->formID = FormID;
        FormIDMasterUpdater checker(curModFile->FormIDHandler);
        checker.Accept(curRecord->formID);
        }

    //Update the editorID
    if(bChangingEditorID)
        {
        //Ensure the record is fully loaded, otherwise any changes could be lost when the record is later loaded
        RecordReader reader(curModFile->FormIDHandler, Expanders);
        reader.Accept(curRecord);
        curRecord->SetField(4, 0, 0, 0, 0, 0, 0, (void *)EditorID, 0);
        curRecord->IsChanged(true);
        }

    //Re-index the record
    if(bDeIndexed)
        {
        RecordIndexer indexer(curModFile, curEditorID_Map, curFormID_Map);
        indexer.Accept(curRecord);
        }
    return (bChangingFormID || bChangingEditorID) ? 1 : -1;
    }