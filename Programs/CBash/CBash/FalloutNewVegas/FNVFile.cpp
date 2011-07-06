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
// FNVFile.cpp
#include "../Common.h"
#include "../GenericRecord.h"
#include "FNVFile.h"

FNVFile::FNVFile(STRING FileName, STRING ModName, const UINT32 _flags):
    ModFile(FileName, ModName, _flags)
    {
    //
    }

FNVFile::~FNVFile()
    {
    //
    }

SINT32 FNVFile::LoadTES4()
    {
    if(TES4.IsLoaded() || !Open())
        return 0;
    #ifdef CBASH_USE_LOGGING
        CLOGGER;
        BOOST_LOG_FUNCTION();
        BOOST_LOG_SEV(lg, trace) << "LoadTES4: " << FileName;
    #endif

    ReadHandler.set_used(4);
    UINT32 recSize = 0;
    ReadHandler.read(&recSize, 4);
    ReadHandler.read(&TES4.flags, 4);
    ReadHandler.read(&TES4.formID, 4);
    ReadHandler.read(&TES4.flagsUnk, 4);
    if(TES4.IsLoaded())
        printf("_fIsLoaded Flag used!!!!: %08X\n", TES4.flags);

    //Normally would read the record with the read method
    //However, that requires recData to be set on the record
    // and that can only be set by the constructor
    //TES4 is constructed when the modfile is created
    // so the info isn't available then.
    //Must make sure this mimics the read method as needed
    TES4.ParseRecord(ReadHandler.getBuffer(20), recSize);
    TES4.IsLoaded(true);
    TES4.IsChanged(true);
    ReadHandler.set_used(recSize);
    return 1;
    }

SINT32 FNVFile::Load(RecordOp &indexer, std::vector<FormIDResolver *> &Expanders)
    {
    enum IgTopRecords {
        };
    if(!ReadHandler.IsOpen() || Flags.LoadedGRUPs)
        return 0;

    Flags.LoadedGRUPs = true;
    UINT32 GRUPSize;
    UINT32 GRUPLabel;
    boost::unordered_set<UINT32> UsedFormIDs;

    RecordReader fullReader(FormIDHandler, Expanders);
    RecordOp skipReader;

    RecordProcessor processor_min(ReadHandler, FormIDHandler, skipReader, Flags, UsedFormIDs);
    RecordProcessor processor_full(ReadHandler, FormIDHandler, fullReader, Flags, UsedFormIDs);

    RecordProcessor &processor = Flags.IsFullLoad ? processor_full : processor_min;

    while(!ReadHandler.eof()){
        ReadHandler.set_used(4); //Skip "GRUP"
        ReadHandler.read(&GRUPSize, 4);
        ReadHandler.read(&GRUPLabel, 4);
        ReadHandler.set_used(4); //Skip type (tops will all == 0)
        //printf("%c%c%c%c\n", ((char *)&GRUPLabel)[0], ((char *)&GRUPLabel)[1], ((char *)&GRUPLabel)[2], ((char *)&GRUPLabel)[3]);
        switch(GRUPLabel)
            {
            //ADD DEFINITIONS HERE

            default:
                printf("FileName = %s\n", ReadHandler.getFileName());
                if(GRUPLabel == 0 && GRUPSize == 0)
                    {
                    printf("  Bad file structure, zeros found past end of groups\n");
                    return 1;
                    }
                else
                    {
                    printf("  Unimplemented GRUP = ");
                    for(int x = 0;x < 4;x++)
                        printf("%c", ((STRING)&GRUPLabel)[x]);
                    printf("\n");
                    }
                if(GRUPSize == 0)
                    {
                    printf("  Unable to continue loading.\n");
                    return 1;
                    }
                else
                    {
                    printf("  Attempting to skip and continue loading.\n");
                    ReadHandler.set_used(GRUPSize - 16); //Skip type (tops will all == 0)
                    }
                break;
            }
        };
    return 1;
    }

UINT32 FNVFile::GetNumRecords(const UINT32 &RecordType)
    {
    return 0; //Not Implemented
    switch(RecordType)
        {
        default:
            printf("Error counting records: %c%c%c%c\n", ((char *)&RecordType)[0], ((char *)&RecordType)[1], ((char *)&RecordType)[2], ((char *)&RecordType)[3]);
            break;
        }
    return 0;
    }

Record * FNVFile::CreateRecord(const UINT32 &RecordType, STRING const &RecordEditorID, Record *&SourceRecord, Record *&ParentRecord, CreateRecordOptions &options)
    {
    if(Flags.IsNoLoad)
        return NULL;
    return NULL; //Not Implemented

    Record *newRecord = NULL;

    switch(RecordType)
        {
        case 'BOLG':
            GLOB.Records.push_back(new GLOBRecord((GLOBRecord *)SourceRecord));
            newRecord = GLOB.Records.back();
            break;

        default:
            printf("Error creating record: %c%c%c%c\n", ((char *)&RecordType)[0], ((char *)&RecordType)[1], ((char *)&RecordType)[2], ((char *)&RecordType)[3]);
            break;
        }
    return newRecord;
    }

SINT32 FNVFile::CleanMasters(std::vector<FormIDResolver *> &Expanders)
    {
    if(Flags.IsNoLoad)
        return -1;
    return -1; //Not Implemented
    UINT32 cleaned = 0;
    //FormIDHandlerClass TempHandler(FileName, TES4.MAST, TES4.HEDR.value.nextObject);
    //TempHandler.SetLoadOrder(FormIDHandler.LoadOrder255);
    //TempHandler.CreateFormIDLookup(FormIDHandler.ExpandedIndex);
    std::vector<UINT32> ToRemove;
    ToRemove.reserve(TES4.MAST.size());

    for(UINT32 p = 0; p < (UINT8)TES4.MAST.size();++p)
        {
        RecordMasterChecker checker(FormIDHandler, Expanders, p);

        //printf("Checking: %s\n", TES4.MAST[p].value);

        //printf("ToRemove: %s\n", TES4.MAST[p].value);
        ToRemove.push_back(p);
        ++cleaned;
        }
    if(cleaned)
        {
        for(SINT32 p = (SINT32)ToRemove.size() - 1; p >= 0; --p)
            TES4.MAST.erase(TES4.MAST.begin() + ToRemove[p]);
        FormIDHandler.UpdateFormIDLookup();
        }
    return cleaned;
    }

SINT32 FNVFile::Save(STRING const &SaveName, std::vector<FormIDResolver *> &Expanders, bool CloseMod)
    {
    if(!Flags.IsSaveable)
        return -1;
    return -1; //Not Implemented

    _FileHandler SaveHandler(SaveName, BUFFERSIZE);
    if(SaveHandler.open_ReadWrite() == -1)
        throw std::exception("Unable to open temporary file for writing\n");

    UINT32 formCount = 0;
    FormIDResolver expander(FormIDHandler.ExpandTable, FormIDHandler.FileStart, FormIDHandler.FileEnd);
    FormIDResolver collapser(FormIDHandler.CollapseTable, FormIDHandler.FileStart, FormIDHandler.FileEnd);
    //RecordReader reader(FormIDHandler);
    const bool bMastersChanged = FormIDHandler.MastersChanged();

    TES4.Write(SaveHandler, bMastersChanged, expander, collapser, Expanders);

    //ADD DEFINITIONS HERE


    //update formCount. Cheaper to go back and write it at the end than to calculate it before any writing.
    SaveHandler.writeAt(30, &formCount, 4);
    SaveHandler.close();
    if(CloseMod)
        Close();
    return 0;
    }

void FNVFile::VisitAllRecords(RecordOp &op)
    {
    if(Flags.IsNoLoad)
        return;
    return; //Not Implemented

    //This visits every record and subrecord

    return;
    }

void FNVFile::VisitRecords(const UINT32 &TopRecordType, const UINT32 &RecordType, RecordOp &op, bool DeepVisit)
    {
    if(Flags.IsNoLoad)
        return;
    return; //Not Implemented

    //This visits only the top records specified.
    switch(TopRecordType)
        {

        default:
            printf("Error visiting records: %i\n", RecordType);
            break;
        }
    return;
    }