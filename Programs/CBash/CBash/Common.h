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

// Common.h
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include <errno.h>
#include <exception>
#include <vector>
#include <boost/unordered_set.hpp>
#include <map>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "MacroDefinitions.h"

#ifdef CBASH_USE_LOGGING
    #include <boost/shared_ptr.hpp>
    #include <boost/log/common.hpp>
    #include <boost/log/filters.hpp>
    #include <boost/log/formatters.hpp>
    #include <boost/log/attributes.hpp>
    #include <boost/log/sinks/sync_frontend.hpp>
    #include <boost/log/sinks/basic_sink_backend.hpp>
    #include <boost/log/sources/logger.hpp>

    // Here we define our application severity levels.
    enum severity_level
    {
        trace,
        normal,
        notification,
        warning,
        error,
        critical
    };
#endif

enum API_FieldTypes {
    UNKNOWN_FIELD = 0,
    MISSING_FIELD,
    JUNK_FIELD,
    BOOL_FIELD,
    SINT8_FIELD,
    UINT8_FIELD,
    SINT16_FIELD,
    UINT16_FIELD,
    SINT32_FIELD,
    UINT32_FIELD,
    FLOAT32_FIELD,
    RADIAN_FIELD,
    FORMID_FIELD,
    MGEFCODE_FIELD,
    ACTORVALUE_FIELD,
    FORMID_OR_UINT32_FIELD,
    UINT8_OR_UINT32_FIELD,
    UNKNOWN_OR_FORMID_OR_UINT32_FIELD,
    UNKNOWN_OR_SINT32_FIELD,
    MGEFCODE_OR_UINT32_FIELD,
    FORMID_OR_MGEFCODE_OR_ACTORVALUE_OR_UINT32_FIELD,
    RESOLVED_MGEFCODE_FIELD,
    STATIC_MGEFCODE_FIELD,
    RESOLVED_ACTORVALUE_FIELD,
    STATIC_ACTORVALUE_FIELD,
    CHAR_FIELD,
    CHAR4_FIELD,
    STRING_FIELD,
    ISTRING_FIELD,
    STRING_OR_FLOAT32_OR_SINT32_FIELD,
    LIST_FIELD,
    PARENTRECORD_FIELD,
    SUBRECORD_FIELD,
    SINT8_FLAG_FIELD,
    SINT8_TYPE_FIELD,
    SINT8_FLAG_TYPE_FIELD,
    SINT8_ARRAY_FIELD,
    UINT8_FLAG_FIELD,
    UINT8_TYPE_FIELD,
    UINT8_FLAG_TYPE_FIELD,
    UINT8_ARRAY_FIELD,
    SINT16_FLAG_FIELD,
    SINT16_TYPE_FIELD,
    SINT16_FLAG_TYPE_FIELD,
    SINT16_ARRAY_FIELD,
    UINT16_FLAG_FIELD,
    UINT16_TYPE_FIELD,
    UINT16_FLAG_TYPE_FIELD,
    UINT16_ARRAY_FIELD,
    SINT32_FLAG_FIELD,
    SINT32_TYPE_FIELD,
    SINT32_FLAG_TYPE_FIELD,
    SINT32_ARRAY_FIELD,
    UINT32_FLAG_FIELD,
    UINT32_TYPE_FIELD,
    UINT32_FLAG_TYPE_FIELD,
    UINT32_ARRAY_FIELD,
    FLOAT32_ARRAY_FIELD,
    RADIAN_ARRAY_FIELD,
    FORMID_ARRAY_FIELD,
    FORMID_OR_UINT32_ARRAY_FIELD,
    MGEFCODE_OR_UINT32_ARRAY_FIELD,
    STRING_ARRAY_FIELD,
    ISTRING_ARRAY_FIELD,
    SUBRECORD_ARRAY_FIELD,
    UNDEFINED_FIELD
    };

enum TopTypes {
    eTop,
    eWorld,
    eInteriorBlock,
    eInteriorSubBlock,
    eExteriorBlock,
    eExteriorSubBlock,
    eCellChildren,
    eTopicChildren,
    eCellPersistent,
    eCellTemporary,
    eCellVWD
    };

enum varType {
    eNONE,
    eUINT32,
    eFORMID
    };

class Ex_NULL : public std::exception
    {
    public:
        const char * __CLR_OR_THIS_CALL what() const;
    };

class Ex_INVALIDINDEX : public std::exception
    {
    public:
        const char * __CLR_OR_THIS_CALL what() const;
    };

class Ex_INVALIDCOLLECTIONINDEX : public std::exception
    {
    public:
        const char * __CLR_OR_THIS_CALL what() const;
    };

class Ex_INVALIDMODINDEX : public std::exception
    {
    public:
        const char * __CLR_OR_THIS_CALL what() const;
    };

class Ex_INVALIDRECORDINDEX : public std::exception
    {
    public:
        const char * __CLR_OR_THIS_CALL what() const;
    };

class ModFile;
class Record;
class StringRecord;

struct sameStr
    {
    bool operator()( const STRING s1, const STRING s2 ) const;
    };

typedef std::multimap<UINT32, std::pair<ModFile *, Record *> >   FormID_Map;
typedef std::multimap<STRING, std::pair<ModFile *, Record *>, sameStr> EditorID_Map;

typedef FormID_Map::iterator FormID_Iterator;
typedef EditorID_Map::iterator EditorID_Iterator;

typedef std::pair<FormID_Iterator, FormID_Iterator> FormID_Range;
typedef std::pair<EditorID_Iterator, EditorID_Iterator> EditorID_Range;

typedef std::pair<varType, varType> FunctionArguments;

typedef std::map<UINT32, FunctionArguments>::value_type Function_ArgumentsType;
typedef std::map<UINT32, STRING>::value_type Function_NameType;
typedef std::map<UINT32, std::vector<UINT32>>::value_type RecordType_PossibleGroupsType;

typedef std::map<UINT32, std::vector<UINT32>>::const_iterator RecordType_PossibleGroups_Iterator;
typedef std::map<UINT32, FunctionArguments>::const_iterator Function_Arguments_Iterator;
typedef std::map<UINT32, STRING>::const_iterator ID_Name_Iterator;

extern const std::map<UINT32, std::vector<UINT32>> RecordType_PossibleGroups;
extern const std::map<UINT32, FunctionArguments> Function_Arguments;
extern const std::map<UINT32, STRING> Function_Name;
extern const std::map<UINT32, STRING> Comparison_Name;
extern const std::map<UINT32, STRING> IDLEGroup_Name;
extern const std::map<UINT32, STRING> PACKAIType_Name;
extern const std::map<UINT32, STRING> PACKLocType_Name;
extern const std::map<UINT32, STRING> PACKTargetType_Name;
extern const std::map<UINT32, STRING> HardCodedFormID_EditorID;

inline void _readBuffer(void *_DstBuf, const unsigned char *_SrcBuf, const UINT32 &_MaxCharCount, UINT32 &_BufPos)
    {
    memcpy(_DstBuf, _SrcBuf + _BufPos, _MaxCharCount);
    _BufPos += _MaxCharCount;
    }

STRING DeGhostModName(STRING const ModName);
bool FileExists(STRING const FileName);
bool AlmostEqual(FLOAT32 A, FLOAT32 B, SINT32 maxUlps);

class _FileHandler
    {
    private:
        boost::interprocess::mapped_region *m_region;
        boost::interprocess::file_mapping *f_map;
        unsigned char *_Buffer;
        UINT32 _BufSize;
        UINT32 _BufPos;
        UINT32 _BufEnd;
        UINT32 _TotalWritten;
        int fh;
        STRING FileName;
        STRING ModName;

    public:
        _FileHandler(STRING _FileName, STRING _ModName);
        _FileHandler(STRING _FileName, UINT32 nSize);
        ~_FileHandler();

        SINT32 open_ReadOnly();
        SINT32 open_ReadWrite();
        STRING const getFileName();
        STRING const getModName();
        bool   IsGhosted();
        bool   IsOpen();
        time_t mtime();
        bool   exists();
        bool   eof();
        UINT32 tell();
        UINT32 set_used(SINT32 _Used);
        void   read(void *_DestBuf, UINT32 _MaxCharCount);
        unsigned char *getBuffer(UINT32 _Offset);
        UINT32 getBufferSize();
        UINT32 write(const void *_SrcBuf, UINT32 _MaxCharCount);
        void   writeSubRecord(UINT32 _Type, const void *_SrcBuf, UINT32 _MaxCharCount);
        UINT32 writeAt(UINT32 _Offset, const void *_SrcBuf, UINT32 _MaxCharCount);
        UINT32 UnusedCache();
        bool   IsCached(UINT32 _Offset);
        SINT32 close();
        void   reserveBuffer(UINT32 nSize);
        void   flush();
    };

class FormIDHandlerClass
    {
    public:
        std::vector<StringRecord> &MAST;
        std::vector<STRING> LoadOrder255;
        boost::unordered_set<UINT32> NewTypes;
        UINT32 &nextObject;
        UINT8  ExpandedIndex;
        UINT8  CollapsedIndex;
        UINT8  ExpandTable[256];
        UINT8  CollapseTable[256];
        bool   IsEmpty;
        bool   bMastersChanged;
        unsigned char * FileStart;
        unsigned char * FileEnd;

        FormIDHandlerClass(std::vector<StringRecord> &_MAST, UINT32 &_NextObject);
        ~FormIDHandlerClass();

        void   SetLoadOrder(std::vector<STRING> &cLoadOrder);
        UINT32 NextExpandedFormID();
        void   CreateFormIDLookup(const UINT8 expandedIndex);
        void   UpdateFormIDLookup();
        void   AddMaster(STRING const curMaster);
        bool   MastersChanged();
        bool   IsNewRecord(const UINT32 *&RecordFormID);
        bool   IsNewRecord(const UINT32 &RecordFormID);
        bool   IsValid(const unsigned char *_SrcBuf);
    };

class CreateRecordOptions
    {
    private:
        enum createFlags
            {
            fSetAsOverride       = 0x00000001,
            fSetAsWorldCell      = 0x00000002,
            fCopyWorldCellStatus = 0x00000004
            };

    public:
        CreateRecordOptions();
        CreateRecordOptions(UINT32 nFlags);
        ~CreateRecordOptions();

        bool SetAsOverride;
        bool SetAsWorldCell;
        bool CopyWorldCellStatus;

        UINT32 GetFlags();
    };

class ModFlags
    {
    private:
        //MinLoad and FullLoad are exclusive
        // If both are set, FullLoad takes priority
        // If neither is set, the mod isn't loaded

        //SkipNewRecords causes any new record to be ignored when the mod is loaded
        // This may leave broken records behind (such as a quest override pointing to a new script that was ignored)
        // So it shouldn't be used if planning on copying records unless you either check that there are no new records being referenced

        //InLoadOrder makes the mod count towards the 255 limit and enables record creation and copying as new.
        // If it is false, it forces Saveable to be false.
        // Any mod with new records should have this set unless you're ignoring the new records.
        // It causes the mod to be reported by GetNumModIDs, GetModIDs

        //Saveable allows the mod to be saved.

        //AddMasters causes the mod's masters to be added to the load order
        // This is essential for most mod editing functions

        //LoadMasters causes the mod's masters to be loaded into memory after being added
        // This has no effect if AddMasters is false
        // This is required if you want to lookup overridden records

        //ExtendedConflicts causes any conflicting records to be ignored by most functions
        // IsRecordWinning, GetNumRecordConflicts, GetRecordConflicts will report the extended conflicts only if asked

        //TrackNewTypes causes the loader to track which record types in a mod are new and not overrides
        // Increases load time per mod.
        // It enables GetModNumTypes and GetModTypes for that mod.

        //IndexLANDs causes LAND records to have extra indexing.
        // Increases load time per mod.
        // It allows the safe editing of land records heights. 
        // Modifying one LAND may require changes in an adjacent LAND to prevent seams

        //FixupPlaceables moves any REFR,ACHR,ACRE records in a world cell to the actual cell they belong to.
        // Increases load time per mod.
        // Use if you're planning on iterating through every placeable in a specific cell 
        //   so that you don't have to check the world cell as well.

        //IgnoreAbsentMasters causes any records that override masters not in the load order to be dropped
        // If it is true, it forces IsAddMasters to be false.
        // Allows mods not in load order to copy records

        //Only the following combinations are tested via Bash:
        // Normal:  (fIsMinLoad or fIsFullLoad) + fIsInLoadOrder + fIsSaveable + fIsAddMasters + fIsLoadMasters
        // Dummy:    fIsAddMasters
        // Merged:  (fIsMinLoad or fIsFullLoad) + fIsSkipNewRecords + fIgnoreAbsentMasters
        // Scanned: (fIsMinLoad or fIsFullLoad) + fIsSkipNewRecords + fIsExtendedConflicts
        enum modFlags
            {
            fIsMinLoad             = 0x00000001,
            fIsFullLoad            = 0x00000002,
            fIsSkipNewRecords      = 0x00000004,
            fIsInLoadOrder         = 0x00000008,
            fIsSaveable            = 0x00000010,
            fIsAddMasters          = 0x00000020,
            fIsLoadMasters         = 0x00000040,
            fIsExtendedConflicts   = 0x00000080,
            fIsTrackNewTypes       = 0x00000100,
            fIsIndexLANDs          = 0x00000200,
            fIsFixupPlaceables     = 0x00000400,
            fIsIgnoreExisting      = 0x00000800,
            fIsIgnoreAbsentMasters = 0x00001000,
            };

    public:
        ModFlags();
        ModFlags(UINT32 _Flags);
        ~ModFlags();

        bool IsMinLoad;
        bool IsFullLoad;
        bool IsNoLoad;
        bool IsSkipNewRecords;
        bool IsInLoadOrder;
        bool IsSaveable;
        bool IsAddMasters;
        bool IsLoadMasters;
        bool IsExtendedConflicts;
        bool IsTrackNewTypes;
        bool IsIndexLANDs;
        bool IsFixupPlaceables;
        bool IsIgnoreExisting;
        bool IsIgnoreAbsentMasters;

        //For internal use, may not be set by constructor
        bool LoadedGRUPs;

        UINT32 GetFlags();
    };

class StringRecord
    {
    public:
        STRING value;

        StringRecord();
        StringRecord(const StringRecord &p);
        StringRecord(const STRING p);
        virtual ~StringRecord();

        virtual UINT32 GetSize() const;

        bool IsLoaded() const;
        void Load();
        void Unload();

        virtual bool Read(unsigned char *buffer, const UINT32 &subSize, UINT32 &curPos);

        void Copy(const StringRecord &FieldValue);
        void Copy(STRING FieldValue);

        bool equals(const StringRecord &other) const;
        bool equalsi(const StringRecord &other) const;
        StringRecord& operator = (const StringRecord &rhs);
    };

class NonNullStringRecord : public StringRecord
    {
    public:
        NonNullStringRecord();
        NonNullStringRecord(const NonNullStringRecord &p);
        ~NonNullStringRecord();

        UINT32 GetSize() const;

        bool Read(unsigned char *buffer, const UINT32 &subSize, UINT32 &curPos);
    };

class RawRecord
    {
    public:
        UINT32 size;
        unsigned char *value;

        RawRecord();
        RawRecord(const RawRecord &p);
        ~RawRecord();

        UINT32 GetSize() const;

        bool IsLoaded() const;
        void Load();
        void Unload();

        bool Read(unsigned char *buffer, UINT32 subSize, UINT32 &curPos);

        void Copy(unsigned char *FieldValue, UINT32 nSize);

        RawRecord& operator = (const RawRecord &rhs);
        bool operator ==(const RawRecord &other) const;
        bool operator !=(const RawRecord &other) const;
    };

//Base record field. Vestigial.
//Used when it isn't known if the record is required or optional.
template<class T>
struct SubRecord
    {
    T value;
    bool isLoaded;

    SubRecord();
    ~SubRecord();

    UINT32 GetSize() const;

    bool IsLoaded() const;
    void Load();
    void Unload();

    bool Read(unsigned char *buffer, UINT32 subSize, UINT32 &curPos);

    T *operator->() const;
    bool operator ==(const SubRecord<T> &other) const;
    bool operator !=(const SubRecord<T> &other) const;
    };

//Used for subrecords that are required
//Even if not actually loaded from disk, they are always considered loaded even if they're explicitly unloaded.
//Unloading them simply resets the values to default.
template<class T>
struct ReqSubRecord
    {
    T value;

    ReqSubRecord();
    ~ReqSubRecord();

    UINT32 GetSize() const;

    bool IsLoaded() const;
    void Load();
    void Unload();

    bool Read(unsigned char *buffer, UINT32 subSize, UINT32 &curPos);

    T *operator->() const;
    bool operator ==(const ReqSubRecord<T> &other) const;
    bool operator !=(const ReqSubRecord<T> &other) const;
    };

//Used for subrecords that are optional
//Even if loaded, they are considered unloaded if they're equal to their defaults
template<class T>
struct OptSubRecord
    {
    T *value;

    OptSubRecord();
    ~OptSubRecord();

    UINT32 GetSize() const;

    bool IsLoaded() const;
    void Load();
    void Unload();

    bool Read(unsigned char *buffer, UINT32 subSize, UINT32 &curPos);

    T *operator->() const;
    OptSubRecord<T>& operator = (const OptSubRecord<T> &rhs);
    bool operator ==(const OptSubRecord<T> &other) const;
    bool operator !=(const OptSubRecord<T> &other) const;
    };

//Identical to OptSubRecord except for IsLoaded
//Once loaded, they are always considered loaded unless they're explicitly unloaded.
//They don't compare to the default value to see if they're
// still considered loaded.
template<class T>
struct SemiOptSubRecord
    {
    T *value;

    SemiOptSubRecord();
    ~SemiOptSubRecord();

    UINT32 GetSize() const;

    bool IsLoaded() const;
    void Load();
    void Unload();

    bool Read(unsigned char *buffer, UINT32 subSize, UINT32 &curPos);

    T *operator->() const;
    SemiOptSubRecord<T>& operator = (const SemiOptSubRecord<T> &rhs);
    bool operator ==(const SemiOptSubRecord<T> &other) const;
    bool operator !=(const SemiOptSubRecord<T> &other) const;
    };

//Hack to support OBMEEFIX
//Identical to OptSubRecord except for IsLoaded
//If value->efixOverrides is equal to 0, it unloads itself
template<class T>
struct OBMEEFIXSubRecord
    {
    T *value;

    OBMEEFIXSubRecord();
    ~OBMEEFIXSubRecord();

    UINT32 GetSize() const;

    bool IsLoaded();
    bool Internal_IsLoaded() const;
    void Load();
    void Unload();

    bool Read(unsigned char *buffer, UINT32 subSize, UINT32 &curPos);

    T *operator->() const;
    OBMEEFIXSubRecord<T>& operator = (const OBMEEFIXSubRecord<T> &rhs);
    bool operator ==(const OBMEEFIXSubRecord<T> &other) const;
    bool operator !=(const OBMEEFIXSubRecord<T> &other) const;
    };

//Cheap trick to separate the template methods into a separate file
//Notice that the file is NOT part of the project
#include "CommonTemplates.cpp"