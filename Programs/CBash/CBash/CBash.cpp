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
// CBash.cpp
//#define CBASH_PYTHON
//#ifdef CBASH_PYTHON
//   #include "Python.h"
//    //#define BOOST_PYTHON_STATIC_LIB
//    //#include <boost/python.hpp>
//#endif

#include "CBash.h"
#include <vector>
//#include "mmgr.h"

#ifdef CBASH_USE_LOGGING
    //#include <boost/log/utility/init/common_attributes.hpp>
    //#include <boost/log/attributes/current_process_id.hpp>

    namespace logging = boost::log;
    namespace flt = boost::log::filters;
    namespace fmt = boost::log::formatters;
    namespace sinks = boost::log::sinks;
    namespace attrs = boost::log::attributes;
    namespace src = boost::log::sources;
    namespace keywords = boost::log::keywords;

    // The backend performs a callback to the specified function
    // Primarily used to send messages back to python as they occur

    class callback_backend : public sinks::basic_formatting_sink_backend< char, char > // Character type
        {
        public:
            typedef SINT32 (*CallbackFunc)(const STRING);

        private:
            // The callback for every logged file
            CallbackFunc const LoggingCallback;

        public:
            // The function consumes the log records that come from the frontend
            void do_consume(record_type const& rec, target_string_type const& formatted_message);

            // The constructor initializes the internal data
            explicit callback_backend(CallbackFunc _LoggingCallback):LoggingCallback(_LoggingCallback)
                {
                //
                }
        };

    // The method puts the formatted message to the callback
    void callback_backend::do_consume(record_type const& rec, target_string_type const& formatted_message)
        {
        //printf("%s\n", formatted_message.c_str());
        LoggingCallback(formatted_message.c_str());
        }
#endif

static std::vector<Collection *> Collections;
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Internal Functions
inline void ValidatePointer(const void *testPointer)
    {
    if(testPointer == NULL)
        throw Ex_NULL();
    }

ModFile *ValidateModName(Collection *curCollection, STRING const ModName)
    {
    ValidatePointer(ModName);
    STRING NonGhostName = DeGhostModName(ModName);
    STRING const &CompName = NonGhostName ? NonGhostName : ModName;
    //ModFiles will never contain null pointers
    for(UINT32 x = 0; x < curCollection->ModFiles.size();++x)
        if(_stricmp(CompName, curCollection->ModFiles[x]->ReadHandler.getModName()) == 0)
            {
            delete []NonGhostName;
            return curCollection->ModFiles[x];
            }
    delete []NonGhostName;
    throw Ex_INVALIDMODINDEX();
    return NULL;
    }

ModFile *ValidateLoadOrderIndex(Collection *curCollection, const UINT32 ModIndex)
    {
    //ModFiles will never contain null pointers
    if(ModIndex >= curCollection->LoadOrder255.size())
        throw Ex_INVALIDMODINDEX();
    return curCollection->LoadOrder255[ModIndex];
    }

ModFile *ValidateLoadOrderIndex(Collection *curCollection, STRING const ModName)
    {
    ValidatePointer(ModName);
    STRING NonGhostName = DeGhostModName(ModName);
    STRING const &CompName = NonGhostName ? NonGhostName : ModName;
    //ModFiles will never contain null pointers
    for(UINT32 x = 0; x < curCollection->LoadOrder255.size();++x)
        if(_stricmp(CompName, curCollection->LoadOrder255[x]->ReadHandler.getModName()) == 0)
            {
            delete []NonGhostName;
            return curCollection->LoadOrder255[x];
            }
    delete []NonGhostName;
    throw Ex_INVALIDMODINDEX();
    return NULL;
    }

//Exported Functions
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Exported Python
//#ifdef CBASH_PYTHON

//PyMODINIT_FUNC
//init_CBashTest(void)

//class CBashUINT32
//    {
//    private:
//        UINT32 _FieldID;
//    public:
//        CBashUINT32(UINT32 FieldID):_FieldID(FieldID) {}
//        UINT32 get()
//            {
//            return 0;
//            }
//        UINT32 set()
//            {
//            return 0;
//            }
//    };

//BOOST_PYTHON_MODULE(CBashTest)
//    {
//    using namespace boost::python;
//
//    class_<CBashUINT32>("CBashUINT32", init<UINT32>())
//        .def("__get__", &CBashUINT32::get)
//        .def("__set__", &CBashUINT32::set);
//    }

//#endif
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Exported DLL
//Version info functions
UINT32 GetVersionMajor()
    {
    return MAJOR_VERSION;
    }

UINT32 GetVersionMinor()
    {
    return MINOR_VERSION;
    }

UINT32 GetVersionRevision()
    {
    return REVISION_VERSION;
    }
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Logging action functions
#ifdef CBASH_USE_LOGGING
    // This function registers my_backend sink in the logging library
    // Complete sink type
    typedef sinks::synchronous_sink< callback_backend > sink_t;

    void init_logging(SINT32 (*_LoggingCallback)(const STRING))
    {
        boost::shared_ptr< logging::core > core = logging::core::get();

        attrs::named_scope Scope;
        core->add_thread_attribute("Scope", Scope);

        // Also let's add some commonly used attributes, like timestamp and record counter.
        //logging::add_common_attributes();

        attrs::counter< unsigned int > RecordID(1);

        // Since we intend to count all logging records ever made by the application,
        // this attribute should clearly be global.
        core->add_global_attribute("RecordID", RecordID);

        // One can construct backend separately and pass it to the frontend
        boost::shared_ptr< callback_backend > backend(new callback_backend(_LoggingCallback));
        boost::shared_ptr< sink_t > sink1(new sink_t(backend));
        core->add_sink(sink1);

        boost::shared_ptr< sink_t > sink2(new sink_t(backend));
        core->add_sink(sink2);

        sink2->set_filter
            (
                flt::attr< severity_level >("Severity", std::nothrow) >= warning
            );

        sink2->locked_backend()->set_formatter
            (
                fmt::stream
                //<< "PID = " << fmt::attr< boost::log::aux::process::id >("ProcessID") // First an attribute "RecordID" is written to the log
                << fmt::attr("RecordID") << ": " // First an attribute "RecordID" is written to the log
                //<< fmt::if_(flt::has_attr("ThreadID"))
                //    [
                //        fmt::stream << "<" << fmt::attr< attrs::current_thread_id::value_type >("ThreadID") // First an attribute "RecordID" is written to the log
                //    ]
                 << fmt::attr< severity_level >("Severity", std::nothrow) << " - "
                << fmt::date_time< boost::posix_time::ptime >("TimeStamp", "%m.%d.%Y %H:%M:%S.%f") << "]"
                << "] [" // then this delimiter separates it from the rest of the line
                << fmt::if_(flt::has_attr("Tag"))
                   [
                       fmt::stream << fmt::attr< std::string >("Tag") << "] [" // yet another delimiter
                   ]
                << fmt::named_scope("Scope", keywords::iteration = fmt::forward) << "] "
                << fmt::message() // here goes the log record text
            );
    }

    SINT32 SetLogging(Collection *CollectionID, SINT32 (*_LoggingCallback)(const STRING), UINT32 LoggingLevel, UINT32 LoggingFlags)
        {
        try
            {
            ValidatePointer(_LoggingCallback);
            if(LoggingFlags == 0)
                {
                init_logging(_LoggingCallback);
                }
            else
                {
                BOOST_LOG_NAMED_SCOPE("SetLogging scope");
                src::logger lg;
                BOOST_LOG(lg) << "Hello World";

                // Now, let's try logging with severity
                src::severity_logger< severity_level > slg;

                BOOST_LOG_SEV(slg, normal) << "A normal severity message, will not pass to the file";
                BOOST_LOG_SEV(slg, warning) << "A warning severity message, will pass to the file";
                BOOST_LOG_SEV(slg, error) << "An error severity message, will pass to the file";

                // Ok, remember the "Tag" attribute we added in the formatter? It is absent in these
                // two lines above, so it is empty in the output. Let's try to tag some log records with it.
                {
                    BOOST_LOG_NAMED_SCOPE("Tagging scope");

                    // Here we add a temporary attribute to the logger lg.
                    // Every log record being written in the current scope with logger lg
                    // will have a string attribute "Tag" with value "Tagged line" attached.
                    BOOST_LOG_SCOPED_LOGGER_TAG(lg, "Tag", std::string, "Tagged line");

                    // The above line is roughly equivalent to the following:
                    // attrs::constant< std::string > TagAttr("Tagged line");
                    // logging::scoped_attribute _ =
                    //     logging::add_scoped_logger_attribute(lg, "Tag", TagAttr);

                    // Now these lines will be highlighted with the tag
                    BOOST_LOG(lg) << "Some tagged log line";
                    BOOST_LOG(lg) << "Another tagged log line";
                }

                // And this line is not highlighted anymore
                BOOST_LOG(lg) << "Now the tag is removed";
                }
            //LOG("Level = %03i, Flags = %08X\n", LoggingLevel, LoggingFlags)
            }
        catch(std::exception &ex)
            {
            PRINT_EXCEPTION(ex);
            printf("\n\n");
            return -1;
            }
        catch(...)
            {
            PRINT_ERROR;
            printf("\n\n");
            return -1;
            }
        return 0;
        }
#endif
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Collection action functions
Collection * CreateCollection(STRING const ModsPath, const UINT32 CollectionType)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModsPath);
        for(UINT32 p = 0; p < Collections.size(); ++p)
            {
            if(Collections[p] == NULL)
                {
                Collections[p] = new Collection(ModsPath, CollectionType);
                return Collections[p];
                }
            }
        Collections.push_back(new Collection(ModsPath, CollectionType));
        return Collections.back();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

SINT32 DeleteCollection(Collection *CollectionID)
    {
    PROFILE_FUNC
    
    try
        {
        ValidatePointer(CollectionID);
        for(UINT32 p = 0; p < Collections.size(); ++p)
            {
            if(Collections[p] == CollectionID)
                {
                delete Collections[p];
                Collections[p] = NULL;
                }
            }
        for(UINT32 p = 0; p < Collections.size(); ++p)
            {
            if(Collections[p] != NULL)
                return 0;
            }
        Collections.clear();

        #ifdef CBASH_CALLCOUNT
            printf("counts = [");
            for(std::map<char *, unsigned long>::iterator it = CallCount.begin(); it != CallCount.end(); ++it)
                printf("(%d, '%s'),", it->second, it->first);
            printf("]\n");
            #ifndef CBASH_CALLTIMING
                CallCount.clear();
            #endif
        #endif
        #ifdef CBASH_CALLTIMING
            double TotTime = 0.0f;
            printf("times = [");
            for(std::map<char *, double>::iterator it = CallTime.begin(); it != CallTime.end(); ++it)
                {
                TotTime += it->second;
                printf("(%.10f, '%s'),", it->second, it->first);
                }
            printf("]\n\n");
            #ifdef CBASH_CALLCOUNT
                printf("Time in CBash = %.10f\n\n", TotTime);
                printf("Time in CBash = sum(total time)\n");
                printf("Total %% = Time in CBash / total time * 100%%\n");
                printf("avg execution = total time / times called\n\n");
                printf("Total %%,avg execution,total time,function,times called\n");
                std::multimap<double, char *> CBashProfiling;
                for(std::map<char *, double>::iterator it = CallTime.begin(); it != CallTime.end(); ++it)
                    CBashProfiling.insert(std::make_pair((it->second / TotTime) * 100, it->first));
                for(std::multimap<double, char *>::iterator it = CBashProfiling.end(); it != CBashProfiling.begin();)
                    {
                    --it;
                    if(it->first < 10.0)
                        printf("0");
                    printf("%02.2f%%,%.10f,%.10f,%s,%d\n", it->first, CallTime[it->second] / CallCount[it->second], CallTime[it->second], it->second, CallCount[it->second]);
                    }
                printf("\n\n");
                CBashProfiling.clear();
                CallCount.clear();
            #endif
            CallTime.clear();
        #endif
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 LoadCollection(Collection *CollectionID)
    {
    PROFILE_FUNC
    
    #ifdef CBASH_USE_LOGGING
        CLOGGER;
        BOOST_LOG_FUNCTION();
        BOOST_LOG_SEV(lg, trace) << "CollectionID = " << CollectionID;
    #endif
    try
        {
        ValidatePointer(CollectionID);
        CollectionID->Load();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 UnloadCollection(Collection *CollectionID)
    {
    PROFILE_FUNC
    
    try
        {
        ValidatePointer(CollectionID);
        CollectionID->Unload();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 DeleteAllCollections()
    {
    PROFILE_FUNC
    
    try
        {
        for(UINT32 p = 0; p < Collections.size(); ++p)
            delete Collections[p];
        Collections.clear();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Mod action functions
SINT32 AddMod(Collection *CollectionID, STRING const ModName, const UINT32 ModFlagsField)
    {
    PROFILE_FUNC
    
    #ifdef CBASH_USE_LOGGING
        CLOGGER;
        BOOST_LOG_FUNCTION();
        BOOST_LOG_SEV(lg, trace) << "Adding " << ModName;
    #endif
    ModFlags flags(ModFlagsField);

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModName);
        return CollectionID->AddMod(ModName, flags);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        }
    return -1;
    }

SINT32 LoadMod(Collection *CollectionID, ModFile *ModID)
    {
    PROFILE_FUNC
    
    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        RecordReader reader(ModID->FormIDHandler, CollectionID->Expanders);
        ModID->VisitAllRecords(reader);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 UnloadMod(Collection *CollectionID, ModFile *ModID)
    {
    PROFILE_FUNC
    
    try
        {
        ValidatePointer(ModID);
        RecordUnloader unloader;
        ModID->VisitAllRecords(unloader);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 CleanModMasters(Collection *CollectionID, ModFile *ModID)
    {
    PROFILE_FUNC
    
    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        return ModID->CleanMasters(CollectionID->Expanders);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return 0;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return 0;
        }
    return 0;
    }

SINT32 RemoveModMasters(Collection *CollectionID, ModFile *ModID, STRINGARRAY MasterNames, UINT32 MasterCount)
    {
    PROFILE_FUNC
    
    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
		std::vector<STRING> masters;
		for (UINT32 i = 0; i < MasterCount; ++i)
		{
			masters.push_back(MasterNames[i]);
		}
        return ModID->CleanMasters2(masters);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return 0;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return 0;
        }
    return 0;
    }

SINT32 SaveMod(Collection *CollectionID, ModFile *ModID, const bool CloseCollection)
    {

    try
        {
        //Profiling is in try block so that the timer gets destructed before DeleteCollection is called
        PROFILE_FUNC
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        CollectionID->SaveMod(ModID, CloseCollection);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }

    if(CloseCollection)
        DeleteCollection(CollectionID);
    return 0;
    }
////////////////////////////////////////////////////////////////////////
//Mod info functions
SINT32 GetAllNumMods(Collection *CollectionID)
    {
    PROFILE_FUNC
    
    try
        {
        ValidatePointer(CollectionID);
        return CollectionID->ModFiles.size();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 GetAllModIDs(Collection *CollectionID, MODIDARRAY ModIDs)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        UINT32 numMods = CollectionID->ModFiles.size();
        for(UINT32 x = 0; x < numMods; ++x)
            ModIDs[x] = CollectionID->ModFiles[x];
        return 0;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 GetLoadOrderNumMods(Collection *CollectionID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        return CollectionID->LoadOrder255.size();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 GetLoadOrderModIDs(Collection *CollectionID, MODIDARRAY ModIDs)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        UINT32 numMods = CollectionID->LoadOrder255.size();
        for(UINT32 x = 0; x < numMods; ++x)
            ModIDs[x] = CollectionID->LoadOrder255[x];
        return 0;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

STRING GetFileNameByID(Collection *CollectionID, ModFile *ModID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModID);
        return ModID->ReadHandler.getFileName();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

STRING GetFileNameByLoadOrder(Collection *CollectionID, const UINT32 ModIndex)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        return ValidateLoadOrderIndex(CollectionID, ModIndex)->ReadHandler.getFileName();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

STRING GetModNameByID(Collection *CollectionID, ModFile *ModID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModID);
        return ModID->ReadHandler.getModName();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

STRING GetModNameByLoadOrder(Collection *CollectionID, const UINT32 ModIndex)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        return ValidateLoadOrderIndex(CollectionID, ModIndex)->ReadHandler.getModName();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

ModFile * GetModIDByName(Collection *CollectionID, STRING const ModName)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        return ValidateModName(CollectionID, ModName);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

ModFile * GetModIDByLoadOrder(Collection *CollectionID, const UINT32 ModIndex)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        return ValidateLoadOrderIndex(CollectionID, ModIndex);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

SINT32 GetModLoadOrderByName(Collection *CollectionID, STRING const ModName)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        return ValidateLoadOrderIndex(CollectionID, ModName)->FormIDHandler.ExpandedIndex;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

SINT32 GetModLoadOrderByID(Collection *CollectionID, ModFile *ModID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModID);
        if(ModID->Flags.IsInLoadOrder)
            return ModID->FormIDHandler.ExpandedIndex;
        return -1;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

STRING GetLongIDName(Collection *CollectionID, ModFile *ModID, const UINT8 ModIndex)
    {
    PROFILE_FUNC

    if(ModIndex == 0xFF)
        return NULL;
    try
        {
        ValidatePointer(ModID);
        UINT8 CollapsedIndex = ModID->FormIDHandler.CollapseTable[ModIndex];
        if(CollapsedIndex >= ModID->TES4.MAST.size())
            return ModID->ReadHandler.getModName();
        return ModID->TES4.MAST[CollapsedIndex].value;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

//SINT32 GetShortIDIndex(Collection *CollectionID, const SINT32 ModID, STRING const ModName)
//    {
//    if(ModID == -1)
//        return GetModLoadOrderByName(CollectionID, ModName);
//    try
//        {
//        ModFile *curModFile = ValidateModID(ValidateCollectionID(CollectionID), ModID);
//        for(UINT16 x = 0; x < curModFile->TES4.MAST.size(); ++x)
//            if(_stricmp(curModFile->TES4.MAST[x].value, ModName) == 0)
//                return curModFile->FormIDHandler.ExpandTable[(UINT8)x] << 24;
//        printf("GetShortIDIndex: Error\n  %s not found in %s's master list!\n", ModName, curModFile->ReadHandler.getModName());
//        return -1;
//        }
//    catch(std::exception &ex)
//        {
//        PRINT_EXCEPTION(ex);
//        printf("\n\n");
//        return -1;
//        }
//    catch(...)
//        {
//        PRINT_ERROR;
//        printf("\n\n");
//        return -1;
//        }
//    return -1;
//    }

UINT32 IsModEmpty(Collection *CollectionID, ModFile *ModID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModID);
        return ModID->FormIDHandler.IsEmpty;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return 0;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return 0;
        }
    return 0;
    }

SINT32 GetModNumTypes(Collection *CollectionID, ModFile *ModID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModID);
        if(!ModID->Flags.IsTrackNewTypes)
            return -1;

        return ModID->FormIDHandler.NewTypes.size();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

void GetModTypes(Collection *CollectionID, ModFile *ModID, UINT32ARRAY RecordTypes)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModID);
        if(!ModID->Flags.IsTrackNewTypes)
            return;

        UINT32 x = 0;
        for(boost::unordered_set<UINT32>::iterator it = ModID->FormIDHandler.NewTypes.begin(); it != ModID->FormIDHandler.NewTypes.end(); ++it, ++x)
            RecordTypes[x] = *it;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        }
    return;
    }
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Record action functions
Record * CreateRecord(Collection *CollectionID, ModFile *ModID, const UINT32 RecordType, const FORMID RecordFormID, STRING const RecordEditorID, Record *ParentID, const UINT32 CreateFlags)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);

        return CollectionID->CreateRecord(ModID, RecordType, RecordFormID, RecordEditorID, (ParentID != NULL ? ParentID->formID: NULL), CreateFlags);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return 0;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return 0;
        }
    return 0;
    }

SINT32 DeleteRecord(Collection *CollectionID, ModFile *ModID, Record *RecordID, Record *ParentID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        return CollectionID->DeleteRecord(ModID, RecordID, ParentID);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

Record * CopyRecord(Collection *CollectionID, ModFile *ModID, Record *RecordID, ModFile *DestModID, Record *DestParentID, const FORMID DestRecordFormID, STRING const DestRecordEditorID, const UINT32 CreateFlags)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        ValidatePointer(RecordID);
        ValidatePointer(DestModID);
        return CollectionID->CopyRecord(ModID, RecordID, DestModID, (DestParentID != NULL) ? DestParentID->formID : NULL, DestRecordFormID, DestRecordEditorID, CreateFlags);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        //PRINT_RECORD_IDENTIFIERS;
        printf("\n\n");
        return 0;
        }
    catch(...)
        {
        PRINT_ERROR;
        //PRINT_RECORD_IDENTIFIERS;
        printf("\n\n");
        return 0;
        }
    return 0;
    }

SINT32 UnloadRecord(Collection *CollectionID, ModFile *ModID, Record *RecordID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(RecordID);
        return RecordID->IsChanged() ? 0 : RecordID->Unload();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

SINT32 SetRecordIdentifiers(Collection *CollectionID, ModFile *ModID, Record *RecordID, const FORMID FormID, STRING const EditorID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        ValidatePointer(RecordID);
        return CollectionID->SetRecordIDs(ModID, RecordID, FormID, EditorID);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }
////////////////////////////////////////////////////////////////////////
//Record info functions
Record * GetRecordID(Collection *CollectionID, ModFile *ModID, const FORMID RecordFormID, STRING const RecordEditorID)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        Record * curRecord = NULL;
        if(RecordFormID == 0 && RecordEditorID == 0)
            return &ModID->TES4;
        if(RecordFormID != 0)
            CollectionID->LookupRecord(ModID, RecordFormID, curRecord);
        if(curRecord != NULL)
            return curRecord;
        if(RecordEditorID != 0)
            CollectionID->LookupRecord(ModID, RecordEditorID, curRecord);
        return curRecord;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }

SINT32 GetNumRecords(Collection *CollectionID, ModFile *ModID, const UINT32 RecordType)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModID);
        return ModID->GetNumRecords(RecordType);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

SINT32 GetRecordIDs(Collection *CollectionID, ModFile *ModID, const UINT32 RecordType, RECORDIDARRAY RecordIDs)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(ModID);
        RecordIDRetriever retriever(RecordIDs);
        ModID->VisitRecords(RecordType, NULL, retriever, false);
        return retriever.GetCount();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

SINT32 IsRecordWinning(Collection *CollectionID, ModFile *ModID, Record *RecordID, const bool GetExtendedConflicts)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        return CollectionID->IsRecordWinning(ModID, RecordID, GetExtendedConflicts);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

SINT32 GetNumRecordConflicts(Collection *CollectionID, Record *RecordID, const bool GetExtendedConflicts)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        return CollectionID->GetNumRecordConflicts(RecordID, GetExtendedConflicts);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

SINT32 GetRecordConflicts(Collection *CollectionID, Record *RecordID, MODIDARRAY ModIDs, RECORDIDARRAY RecordIDs, const bool GetExtendedConflicts)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        return CollectionID->GetRecordConflicts(RecordID, ModIDs, RecordIDs, GetExtendedConflicts);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

SINT32 GetRecordHistory(Collection *CollectionID, ModFile *ModID, Record *RecordID, MODIDARRAY ModIDs, RECORDIDARRAY RecordIDs)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        return CollectionID->GetRecordHistory(ModID, RecordID, ModIDs, RecordIDs);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }

SINT32 CompareIdenticalRecords(Record *RecordID1, Record *RecordID2)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(RecordID1);
		ValidatePointer(RecordID2);
		// Get correct class pointers
		if (RecordID1->GetType() == RecordID2->GetType())
			return RecordID1->equals(RecordID2);
		return false;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return 0;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return 0;
    }

////////////////////////////////////////////////////////////////////////
//Mod or Record action functions
SINT32 UpdateReferences(Collection *CollectionID, ModFile *ModID, Record *RecordID, const FORMID FormIDToReplace, const FORMID ReplacementFormID)
    {
    PROFILE_FUNC

    //Sanity check.
    if(FormIDToReplace == ReplacementFormID)
        return -1;

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        RecordFormIDSwapper swapper(FormIDToReplace, ReplacementFormID, ModID->FormIDHandler, CollectionID->Expanders);

        if(RecordID != NULL) //Swap possible uses of FormIDToReplace in a specific record only
            {
            swapper.Accept(RecordID);
            }
        else //Swap all possible uses of FormIDToReplace
            {
            RecordType_PossibleGroups_Iterator curTypes = RecordType_PossibleGroups.find(RecordID->GetType());
            if(curTypes != RecordType_PossibleGroups.end())
                {
                for(std::vector<UINT32>::const_iterator x = curTypes->second.begin(); x != curTypes->second.end(); ++x)
                    ModID->VisitRecords(*x, NULL, swapper, true);
                }
            }
        return swapper.GetCount();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }
////////////////////////////////////////////////////////////////////////
//Mod or Record info functions
SINT32 GetNumReferences(Collection *CollectionID, ModFile *ModID, Record *RecordID, const FORMID FormIDToMatch)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        ValidatePointer(RecordID);
        
        //Ensure the record is fully loaded
        RecordReader reader(ModID->FormIDHandler, CollectionID->Expanders);
        reader.Accept(RecordID);

        FormIDMatchCounter counter(FormIDToMatch);
        RecordID->VisitFormIDs(counter);
        return counter.GetCount();
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        printf("\n\n");
        return -1;
        }
    catch(...)
        {
        PRINT_ERROR;
        printf("\n\n");
        return -1;
        }
    return -1;
    }
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//Field action functions
void SetField(Collection *CollectionID, ModFile *ModID, Record *RecordID, FIELD_IDENTIFIERS, void *FieldValue, const UINT32 ArraySize)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        ValidatePointer(RecordID);
        
        //Ensure the record is fully loaded
        RecordReader reader(ModID->FormIDHandler, CollectionID->Expanders);
        reader.Accept(RecordID);

        if(RecordID->SetField(FieldID, ListIndex, ListFieldID, ListX2Index, ListX2FieldID, ListX3Index, ListX3FieldID, FieldValue, ArraySize))
            {
            //returns true if formIDs need to be checked
            //Update the master list if needed
            FormIDMasterUpdater checker(ModID->FormIDHandler);
            //checker.Accept(curRecord->formID); //FormID can only be changed through SetRecordIDs, so no need to check
            RecordID->VisitFormIDs(checker);
            }

        RecordID->IsChanged(true);
        return;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("ArraySize: %i\n\n", ArraySize);
        }
    catch(...)
        {
        PRINT_ERROR;
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("ArraySize: %i\n\n", ArraySize);
        }
    }

void DeleteField(Collection *CollectionID, ModFile *ModID, Record *RecordID, FIELD_IDENTIFIERS)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        ValidatePointer(RecordID);
        
        //Ensure the record is fully loaded
        RecordReader reader(ModID->FormIDHandler, CollectionID->Expanders);
        reader.Accept(RecordID);

        RecordID->DeleteField(FieldID, ListIndex, ListFieldID, ListX2Index, ListX2FieldID, ListX3Index, ListX3FieldID);

        RecordID->IsChanged(true);
        return;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("\n\n");
        }
    catch(...)
        {
        PRINT_ERROR;
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("\n\n");
        }
    }
////////////////////////////////////////////////////////////////////////
//Field info functions
UINT32 GetFieldAttribute(Collection *CollectionID, ModFile *ModID, Record *RecordID, FIELD_IDENTIFIERS, const UINT32 WhichAttribute)
    {
    PROFILE_FUNC

    try
        {
        ValidatePointer(CollectionID);
        ValidatePointer(ModID);
        ValidatePointer(RecordID);

        if(WhichAttribute > 0)
            {
            //Any attribute other than type requires the record to be read
            //Ensure the record is fully loaded
            RecordReader reader(ModID->FormIDHandler, CollectionID->Expanders);
            reader.Accept(RecordID);
            }

        return RecordID->GetFieldAttribute(FieldID, ListIndex, ListFieldID, ListX2Index, ListX2FieldID, ListX3Index, ListX3FieldID, WhichAttribute);
        }
    catch(Ex_INVALIDRECORDINDEX &ex)
        {
        PRINT_EXCEPTION(ex);
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("WhichAttribute: %i\n\n", WhichAttribute);
        return UNKNOWN_FIELD;
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("WhichAttribute: %i\n\n", WhichAttribute);
        return UNKNOWN_FIELD;
        }
    catch(...)
        {
        PRINT_ERROR;
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("WhichAttribute: %i\n\n", WhichAttribute);
        return UNKNOWN_FIELD;
        }
    return UNKNOWN_FIELD;
    }

void * GetField(Collection *CollectionID, ModFile *ModID, Record *RecordID, FIELD_IDENTIFIERS, void **FieldValues)
    {
    PROFILE_FUNC

    try
        {
        //ValidatePointer(CollectionID);
        //ValidatePointer(ModID);
        //ValidatePointer(RecordID);
        
        //Ensure the record is fully loaded
        RecordReader reader(ModID->FormIDHandler, CollectionID->Expanders);
        reader.Accept(RecordID);

        return RecordID->GetField(FieldID, ListIndex, ListFieldID, ListX2Index, ListX2FieldID, ListX3Index, ListX3FieldID, FieldValues);
        }
    catch(std::exception &ex)
        {
        PRINT_EXCEPTION(ex);
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("\n\n");
        return NULL;
        }
    catch(...)
        {
        PRINT_ERROR;
        //PRINT_RECORD_IDENTIFIERS;
        PRINT_FIELD_IDENTIFIERS;
        printf("\n\n");
        return NULL;
        }
    return NULL;
    }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////