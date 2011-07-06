#pragma once

//define or undef as desired
#undef CBASH_PROFILING
#undef CBASH_USE_LOGGING

//These require CBASH_PROFILING to be defined
#ifdef CBASH_PROFILING
    //define or undef as desired
    #define CBASH_CALLTIMING
    #define CBASH_CALLCOUNT
    #undef CBASH_TRACE
#else
    //don't touch
    #undef CBASH_CALLTIMING
    #undef CBASH_CALLCOUNT
    #undef CBASH_TRACE
#endif

#ifdef CBASH_TRACE
    #define TRACE_FUNC printf("%s\n", __FUNCTION__)
#else
    #define TRACE_FUNC
#endif

#ifdef CBASH_PROFILING
    #define PROFILE_FUNC TRACE_FUNC;COUNT_FUNC;TIME_FUNC;
#else
    #define PROFILE_FUNC
#endif

#ifdef CBASH_CALLTIMING
    #include <windows.h>
    #include <map>

    static std::map<char *, double> CallTime;

    typedef struct
        {
        LARGE_INTEGER start;
        LARGE_INTEGER stop;
        } stopWatch;

    class CStopWatch
        {
        private:
            stopWatch timer;
            LARGE_INTEGER frequency;
            char *FunctionName;

        public:
            CStopWatch(char *FName):FunctionName(FName)
                {
                timer.start.QuadPart=0;
                timer.stop.QuadPart=0; 
                QueryPerformanceFrequency(&frequency);
                QueryPerformanceCounter(&timer.start);
                }

            ~CStopWatch()
                {
                QueryPerformanceCounter(&timer.stop);
                LARGE_INTEGER time;
                time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart;
                CallTime[FunctionName] = CallTime[FunctionName] + ((double)time.QuadPart / (double)frequency.QuadPart);
                }
        };
    #define TIME_FUNC CStopWatch cbtimer(__FUNCTION__)
#else
    #define TIME_FUNC
#endif

#ifdef CBASH_CALLCOUNT
    #include <map>
    static std::map<char *, unsigned long> CallCount;

    class CCounter
        {
        private:
            unsigned long total;
            char *FunctionName;

        public:
            CCounter(char *FName):FunctionName(FName),total(CallCount[FName])
                {
                total++;
                CallCount[FName] = total;
                }
        };

    #define COUNT_FUNC CCounter cbcounter(__FUNCTION__)
#else
    #define COUNT_FUNC
#endif

#ifdef CBASH_USE_LOGGING
    #ifndef CLOGGER
        #define CLOGGER    boost::log::sources::severity_logger< severity_level > lg
    #endif
#endif

#ifndef MAJOR_VERSION
    #define MAJOR_VERSION    0
#endif

#ifndef MINOR_VERSION
    #define MINOR_VERSION    5
#endif

#ifndef REVISION_VERSION
    #define REVISION_VERSION    3
#endif

#ifndef NULL
    #ifdef __cplusplus
        #define NULL    0
    #else
        #define NULL    ((void *)0)
    #endif
#endif

#ifndef PRINT_RECORD_IDENTIFIERS
    #define PRINT_RECORD_IDENTIFIERS printf("CollectionID: %08X, ModID: %08X, RecordID: %08X\n", CollectionID, ModID, RecordID)
#endif

#ifndef PRINT_FIELD_IDENTIFIERS
    #define PRINT_FIELD_IDENTIFIERS printf("FieldID: %i, ListIndex: %i, ListFieldID: %i, ListX2Index: %i, ListX2FieldID: %i, ListX3Index: %i, ListX3FieldID: %i \n", FieldID, ListIndex, ListFieldID, ListX2Index, ListX2FieldID, ListX3Index, ListX3FieldID)
#endif

#ifndef PRINT_EXCEPTION
    #define PRINT_EXCEPTION(ex) printf("%s error\n  %s\n", __FUNCTION__, ex.what())
#endif

#ifndef PRINT_ERROR
    #define PRINT_ERROR printf("%s error\n  Unhandled Exception\n", __FUNCTION__)
#endif

#ifndef NUMTHREADS
    #define NUMTHREADS    boost::thread::hardware_concurrency()
#endif

#ifndef BUFFERSIZE
    #define BUFFERSIZE    65536
#endif

#ifndef END_HARDCODED_IDS
    #define END_HARDCODED_IDS    0x00000800
#endif

#ifndef EXPORT_FUNC
    #define EXPORT_FUNC extern "C" __declspec(dllexport)
#endif

#ifndef UINT8
    #define UINT8 unsigned char
#endif

#ifndef UINT8ARRAY
    #define UINT8ARRAY unsigned char *
#endif

#ifndef UINT16
    #define UINT16 unsigned short
#endif

#ifndef UINT32
    #define UINT32 unsigned long
#endif

#ifndef SINT8
    #define SINT8 signed char
#endif

#ifndef SINT16
    #define SINT16 signed short
#endif

#ifndef SINT32
    #define SINT32 signed long
#endif

#ifndef UINT32ARRAY
    #define UINT32ARRAY unsigned long *
#endif

#ifndef SINT32ARRAY
    #define SINT32ARRAY signed long *
#endif

#ifndef FLOAT32
    #define FLOAT32 float
#endif

#ifndef STRING
    #define STRING char *
#endif

#ifndef STRINGARRAY
    #define STRINGARRAY char **
#endif

#ifndef MODIDARRAY
    #define MODIDARRAY ModFile **
#endif

#ifndef RECORDIDARRAY
    #define RECORDIDARRAY Record **
#endif

#ifndef FORMID
    #define FORMID unsigned long
#endif

#ifndef MGEFCODE
    #define MGEFCODE unsigned long
#endif

#ifndef FORMID_OR_UINT32
    #define FORMID_OR_UINT32 unsigned long
#endif

#ifndef MGEFCODE_OR_UINT32
    #define MGEFCODE_OR_UINT32 unsigned long
#endif

#ifndef FORMID_OR_MGEFCODE_OR_ACTORVALUE_OR_UINT32
    #define FORMID_OR_MGEFCODE_OR_ACTORVALUE_OR_UINT32 unsigned long
#endif

#ifndef ACTORVALUE
    #define ACTORVALUE unsigned long
#endif

#ifndef FORMIDARRAY
    #define FORMIDARRAY unsigned long *
#endif

#ifndef MGEFCODEARRAY
    #define MGEFCODEARRAY unsigned long *
#endif

#ifndef DEFAULTED_FIELD_IDENTIFIERS
    #define DEFAULTED_FIELD_IDENTIFIERS const UINT32 FieldID=0, const UINT32 ListIndex=0, const UINT32 ListFieldID=0, const UINT32 ListX2Index=0, const UINT32 ListX2FieldID=0, const UINT32 ListX3Index=0, const UINT32 ListX3FieldID=0
#endif

#ifndef FIELD_IDENTIFIERS
    #define FIELD_IDENTIFIERS const UINT32 FieldID, const UINT32 ListIndex, const UINT32 ListFieldID, const UINT32 ListX2Index, const UINT32 ListX2FieldID, const UINT32 ListX3Index, const UINT32 ListX3FieldID
#endif