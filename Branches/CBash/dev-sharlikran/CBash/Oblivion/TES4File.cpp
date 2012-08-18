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
// TES4File.cpp
#include "../Common.h"
#include "../GenericRecord.h"
#include "TES4File.h"

TES4File::TES4File(STRING FileName, STRING ModName, const UINT32 _flags):
    ModFile(FileName, ModName, _flags)
    {
    //
    }

TES4File::~TES4File()
    {
    //
    }

SINT32 TES4File::LoadTES4()
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

SINT32 TES4File::Load(RecordOp &indexer, std::vector<FormIDResolver *> &Expanders)
    {
    enum IgTopRecords {
        eIgGMST=0x54535D47,
        eIgGLOB=0x424F5C47,
        eIgCLAS=0x53415C43,
        eIgFACT=0x54435146,
        eIgHAIR=0x52495148,
        eIgEYES=0x53456945,
        eIgRACE=0x45435152,
        eIgSOUN=0x4E555F53,
        eIgSKIL=0x4C495B53,
        eIgMGEF=0x4645574D,
        eIgSCPT=0x54505353,
        eIgLTEX=0x5845644C,
        eIgENCH=0x48435E45,
        eIgSPEL=0x4C456053,
        eIgBSGN=0x4E476342,
        eIgACTI=0x49545341,
        eIgAPPA=0x41506041,
        eIgARMO=0x4F4D6241,
        eIgBOOK=0x4B4F5F42,
        eIgCLOT=0x544F5C43,
        eIgCONT=0x544E5F43,
        eIgDOOR=0x524F5F44,
        eIgINGR=0x52475E49,
        eIgLIGH=0x4847594C,
        eIgMISC=0x4353594D,
        eIgSTAT=0x54416453,
        eIgGRAS=0x53416247,
        eIgTREE=0x45456254,
        eIgFLOR=0x524F5C46,
        eIgFURN=0x4E526546,
        eIgWEAP=0x50415557,
        eIgAMMO=0x4F4D5D41,
        eIgNPC_=0x5F43604E,
        eIgCREA=0x41456243,
        eIgLVLC=0x434C664C,
        eIgSLGM=0x4D475C53,
        eIgKEYM=0x4D59554B,
        eIgALCH=0x48435C41,
        eIgSBSP=0x50535253,
        eIgSGST=0x54535753,
        eIgLVLI=0x494C664C,
        eIgWTHR=0x52486457,
        eIgCLMT=0x544D5C43,
        eIgREGN=0x4E475552,
        eIgCELL=0x4C4C5543,
        eIgWRLD=0x444C6257,
        eIgDIAL=0x4C415944,
        eIgQUST=0x54536551,
        eIgIDLE=0x454C5449,
        eIgPACK=0x4B435150,
        eIgCSTY=0x59546343,
        eIgLSCR=0x5243634C,
        eIgLVSP=0x5053664C,
        eIgANIO=0x4F495E41,
        eIgWATR=0x52545157,
        eIgEFSH=0x48535645
        };
    if(!ReadHandler.IsOpen() || Flags.LoadedGRUPs)
        return 0;
    //printf("%08X\n", Flags.GetFlags());
    //printf("Loading %s\n", FileName);
    //for(UINT32 x = 0; x < FormIDHandler.LoadOrder255.size(); ++x)
    //    printf("Master in load order %02X: %s\n", x, FormIDHandler.LoadOrder255[x]);
    Flags.LoadedGRUPs = true;
    UINT32 GRUPSize;
    UINT32 GRUPLabel;
    boost::unordered_set<UINT32> UsedFormIDs;

    RecordReader fullReader(FormIDHandler, Expanders);
    RecordOp skipReader;

    RecordProcessor processor_min(ReadHandler, FormIDHandler, skipReader, Flags, UsedFormIDs);
    RecordProcessor processor_full(ReadHandler, FormIDHandler, fullReader, Flags, UsedFormIDs);

    RecordProcessor &processor = Flags.IsFullLoad ? processor_full : processor_min;

    //RecordProcessFunc RecordProcessor = (Flags.IsSkipNewRecords || Flags.IsTrackNewTypes) ? (Flags.IsSkipNewRecords ? &ProcessRecord_SkipNew : &ProcessRecord_TrackNew) : &ProcessRecord_Base;

    while(!ReadHandler.eof()){
        ReadHandler.set_used(4); //Skip "GRUP"
        ReadHandler.read(&GRUPSize, 4);
        ReadHandler.read(&GRUPLabel, 4);
        ReadHandler.set_used(4); //Skip type (tops will all == 0)
        //printf("%c%c%c%c\n", ((char *)&GRUPLabel)[0], ((char *)&GRUPLabel)[1], ((char *)&GRUPLabel)[2], ((char *)&GRUPLabel)[3]);
        switch(GRUPLabel)
            {
            //ADD DEFINITIONS HERE
            case eIgGMST:
            case 'TSMG':
                ReadHandler.read(&GMST.stamp, 4);
                GMST.Skim(ReadHandler, GRUPSize, processor_full, indexer);
                break;
            case eIgGLOB:
            case 'BOLG':
                ReadHandler.read(&GLOB.stamp, 4);
                GLOB.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgCLAS:
            case 'SALC':
                ReadHandler.read(&CLAS.stamp, 4);
                CLAS.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgFACT:
            case 'TCAF':
                ReadHandler.read(&FACT.stamp, 4);
                FACT.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgHAIR:
            case 'RIAH':
                ReadHandler.read(&HAIR.stamp, 4);
                HAIR.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgEYES:
            case 'SEYE':
                ReadHandler.read(&EYES.stamp, 4);
                EYES.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgRACE:
            case 'ECAR':
                ReadHandler.read(&RACE.stamp, 4);
                RACE.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgSOUN:
            case 'NUOS':
                ReadHandler.read(&SOUN.stamp, 4);
                SOUN.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgSKIL:
            case 'LIKS':
                ReadHandler.read(&SKIL.stamp, 4);
                SKIL.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgMGEF:
            case 'FEGM':
                ReadHandler.read(&MGEF.stamp, 4);
                MGEF.Skim(ReadHandler, GRUPSize, processor_full, indexer);
                break;
            case eIgSCPT:
            case 'TPCS':
                ReadHandler.read(&SCPT.stamp, 4);
                SCPT.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgLTEX:
            case 'XETL':
                ReadHandler.read(&LTEX.stamp, 4);
                LTEX.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgENCH:
            case 'HCNE':
                ReadHandler.read(&ENCH.stamp, 4);
                ENCH.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgSPEL:
            case 'LEPS':
                ReadHandler.read(&SPEL.stamp, 4);
                SPEL.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgBSGN:
            case 'NGSB':
                ReadHandler.read(&BSGN.stamp, 4);
                BSGN.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgACTI:
            case 'ITCA':
                ReadHandler.read(&ACTI.stamp, 4);
                ACTI.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgAPPA:
            case 'APPA':
                ReadHandler.read(&APPA.stamp, 4);
                APPA.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgARMO:
            case 'OMRA':
                ReadHandler.read(&ARMO.stamp, 4);
                ARMO.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgBOOK:
            case 'KOOB':
                ReadHandler.read(&BOOK.stamp, 4);
                BOOK.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgCLOT:
            case 'TOLC':
                ReadHandler.read(&CLOT.stamp, 4);
                CLOT.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgCONT:
            case 'TNOC':
                ReadHandler.read(&CONT.stamp, 4);
                CONT.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgDOOR:
            case 'ROOD':
                ReadHandler.read(&DOOR.stamp, 4);
                DOOR.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgINGR:
            case 'RGNI':
                ReadHandler.read(&INGR.stamp, 4);
                INGR.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgLIGH:
            case 'HGIL':
                ReadHandler.read(&LIGH.stamp, 4);
                LIGH.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgMISC:
            case 'CSIM':
                ReadHandler.read(&MISC.stamp, 4);
                MISC.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgSTAT:
            case 'TATS':
                ReadHandler.read(&STAT.stamp, 4);
                STAT.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgGRAS:
            case 'SARG':
                ReadHandler.read(&GRAS.stamp, 4);
                GRAS.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgTREE:
            case 'EERT':
                ReadHandler.read(&TREE.stamp, 4);
                TREE.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgFLOR:
            case 'ROLF':
                ReadHandler.read(&FLOR.stamp, 4);
                FLOR.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgFURN:
            case 'NRUF':
                ReadHandler.read(&FURN.stamp, 4);
                FURN.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgWEAP:
            case 'PAEW':
                ReadHandler.read(&WEAP.stamp, 4);
                WEAP.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgAMMO:
            case 'OMMA':
                ReadHandler.read(&AMMO.stamp, 4);
                AMMO.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgNPC_:
            case '_CPN':
                ReadHandler.read(&NPC_.stamp, 4);
                NPC_.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgCREA:
            case 'AERC':
                ReadHandler.read(&CREA.stamp, 4);
                CREA.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgLVLC:
            case 'CLVL':
                ReadHandler.read(&LVLC.stamp, 4);
                LVLC.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgSLGM:
            case 'MGLS':
                ReadHandler.read(&SLGM.stamp, 4);
                SLGM.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgKEYM:
            case 'MYEK':
                ReadHandler.read(&KEYM.stamp, 4);
                KEYM.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgALCH:
            case 'HCLA':
                ReadHandler.read(&ALCH.stamp, 4);
                ALCH.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgSBSP:
            case 'PSBS':
                ReadHandler.read(&SBSP.stamp, 4);
                SBSP.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgSGST:
            case 'TSGS':
                ReadHandler.read(&SGST.stamp, 4);
                SGST.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgLVLI:
            case 'ILVL':
                ReadHandler.read(&LVLI.stamp, 4);
                LVLI.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgWTHR:
            case 'RHTW':
                ReadHandler.read(&WTHR.stamp, 4);
                WTHR.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgCLMT:
            case 'TMLC':
                ReadHandler.read(&CLMT.stamp, 4);
                CLMT.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgREGN:
            case 'NGER':
                ReadHandler.read(&REGN.stamp, 4);
                REGN.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgCELL:
            case 'LLEC':
                ReadHandler.read(&CELL.stamp, 4);
                CELL.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgWRLD:
            case 'DLRW':
                ReadHandler.read(&WRLD.stamp, 4);
                WRLD.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgDIAL:
            case 'LAID':
                ReadHandler.read(&DIAL.stamp, 4);
                DIAL.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgQUST:
            case 'TSUQ':
                ReadHandler.read(&QUST.stamp, 4);
                QUST.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgIDLE:
            case 'ELDI':
                ReadHandler.read(&IDLE.stamp, 4);
                IDLE.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgPACK:
            case 'KCAP':
                ReadHandler.read(&PACK.stamp, 4);
                PACK.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgCSTY:
            case 'YTSC':
                ReadHandler.read(&CSTY.stamp, 4);
                CSTY.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgLSCR:
            case 'RCSL':
                ReadHandler.read(&LSCR.stamp, 4);
                LSCR.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgLVSP:
            case 'PSVL':
                ReadHandler.read(&LVSP.stamp, 4);
                LVSP.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgANIO:
            case 'OINA':
                ReadHandler.read(&ANIO.stamp, 4);
                ANIO.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgWATR:
            case 'RTAW':
                ReadHandler.read(&WATR.stamp, 4);
                WATR.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
            case eIgEFSH:
            case 'HSFE':
                ReadHandler.read(&EFSH.stamp, 4);
                EFSH.Skim(ReadHandler, GRUPSize, processor, indexer);
                break;
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

UINT32 TES4File::GetNumRecords(const UINT32 &RecordType)
    {
    switch(RecordType)
        {
        case 'TSMG':
            return (UINT32)GMST.Records.size();
        case 'BOLG':
            return (UINT32)GLOB.Records.size();
        case 'SALC':
            return (UINT32)CLAS.Records.size();
        case 'TCAF':
            return (UINT32)FACT.Records.size();
        case 'RIAH':
            return (UINT32)HAIR.Records.size();
        case 'SEYE':
            return (UINT32)EYES.Records.size();
        case 'ECAR':
            return (UINT32)RACE.Records.size();
        case 'NUOS':
            return (UINT32)SOUN.Records.size();
        case 'LIKS':
            return (UINT32)SKIL.Records.size();
        case 'FEGM':
            return (UINT32)MGEF.Records.size();
        case 'TPCS':
            return (UINT32)SCPT.Records.size();
        case 'XETL':
            return (UINT32)LTEX.Records.size();
        case 'HCNE':
            return (UINT32)ENCH.Records.size();
        case 'LEPS':
            return (UINT32)SPEL.Records.size();
        case 'NGSB':
            return (UINT32)BSGN.Records.size();
        case 'ITCA':
            return (UINT32)ACTI.Records.size();
        case 'APPA':
            return (UINT32)APPA.Records.size();
        case 'OMRA':
            return (UINT32)ARMO.Records.size();
        case 'KOOB':
            return (UINT32)BOOK.Records.size();
        case 'TOLC':
            return (UINT32)CLOT.Records.size();
        case 'TNOC':
            return (UINT32)CONT.Records.size();
        case 'ROOD':
            return (UINT32)DOOR.Records.size();
        case 'RGNI':
            return (UINT32)INGR.Records.size();
        case 'HGIL':
            return (UINT32)LIGH.Records.size();
        case 'CSIM':
            return (UINT32)MISC.Records.size();
        case 'TATS':
            return (UINT32)STAT.Records.size();
        case 'SARG':
            return (UINT32)GRAS.Records.size();
        case 'EERT':
            return (UINT32)TREE.Records.size();
        case 'ROLF':
            return (UINT32)FLOR.Records.size();
        case 'NRUF':
            return (UINT32)FURN.Records.size();
        case 'PAEW':
            return (UINT32)WEAP.Records.size();
        case 'OMMA':
            return (UINT32)AMMO.Records.size();
        case '_CPN':
            return (UINT32)NPC_.Records.size();
        case 'AERC':
            return (UINT32)CREA.Records.size();
        case 'CLVL':
            return (UINT32)LVLC.Records.size();
        case 'MGLS':
            return (UINT32)SLGM.Records.size();
        case 'MYEK':
            return (UINT32)KEYM.Records.size();
        case 'HCLA':
            return (UINT32)ALCH.Records.size();
        case 'PSBS':
            return (UINT32)SBSP.Records.size();
        case 'TSGS':
            return (UINT32)SGST.Records.size();
        case 'ILVL':
            return (UINT32)LVLI.Records.size();
        case 'RHTW':
            return (UINT32)WTHR.Records.size();
        case 'TMLC':
            return (UINT32)CLMT.Records.size();
        case 'NGER':
            return (UINT32)REGN.Records.size();
        case 'LLEC':
            return (UINT32)CELL.Records.size();
        case 'DLRW':
            return (UINT32)WRLD.Records.size();
        case 'LAID':
            return (UINT32)DIAL.Records.size();
        ///////////////////////////////////////////////
        //SubRecords are counted via GetFieldAttribute API function
        //case 'DRGP':
        //    break;
        //case 'DNAL':
        //    break;
        //case 'RHCA':
        //    break;
        //case 'ERCA':
        //    break;
        //case 'RFER':
        //    break;
        //case 'DAOR':
        //    break;
        //case 'OFNI':
        //    break;
        ///////////////////////////////////////////////
        case 'TSUQ':
            return (UINT32)QUST.Records.size();
        case 'ELDI':
            return (UINT32)IDLE.Records.size();
        case 'KCAP':
            return (UINT32)PACK.Records.size();
        case 'YTSC':
            return (UINT32)CSTY.Records.size();
        case 'RCSL':
            return (UINT32)LSCR.Records.size();
        case 'PSVL':
            return (UINT32)LVSP.Records.size();
        case 'OINA':
            return (UINT32)ANIO.Records.size();
        case 'RTAW':
            return (UINT32)WATR.Records.size();
        case 'HSFE':
            return (UINT32)EFSH.Records.size();
        default:
            printf("Error counting records: %c%c%c%c\n", ((char *)&RecordType)[0], ((char *)&RecordType)[1], ((char *)&RecordType)[2], ((char *)&RecordType)[3]);
            break;
        }
    return 0;
    }

Record * TES4File::CreateRecord(const UINT32 &RecordType, STRING const &RecordEditorID, Record *&SourceRecord, Record *&ParentRecord, CreateRecordOptions &options)
    {
    if(Flags.IsNoLoad)
        return NULL;

    Record *newRecord = NULL;

    switch(RecordType)
        {
        case 'TSMG':
            if(RecordEditorID == NULL && SourceRecord == NULL)
                return NULL;

            GMST.Records.push_back(new GMSTRecord((GMSTRecord *)SourceRecord));
            newRecord = GMST.Records.back();

            if(RecordEditorID != NULL)
                {
                ((GMSTRecord *)newRecord)->EDID.Copy(RecordEditorID);
                ((GMSTRecord *)newRecord)->DATA.format = ((GMSTRecord *)newRecord)->EDID.value[0];
                }
            break;
        case 'BOLG':
            GLOB.Records.push_back(new GLOBRecord((GLOBRecord *)SourceRecord));
            newRecord = GLOB.Records.back();
            break;
        case 'SALC':
            CLAS.Records.push_back(new CLASRecord((CLASRecord *)SourceRecord));
            newRecord = CLAS.Records.back();
            break;
        case 'TCAF':
            FACT.Records.push_back(new FACTRecord((FACTRecord *)SourceRecord));
            newRecord = FACT.Records.back();
            break;
        case 'RIAH':
            HAIR.Records.push_back(new HAIRRecord((HAIRRecord *)SourceRecord));
            newRecord = HAIR.Records.back();
            break;
        case 'SEYE':
            EYES.Records.push_back(new EYESRecord((EYESRecord *)SourceRecord));
            newRecord = EYES.Records.back();
            break;
        case 'ECAR':
            RACE.Records.push_back(new RACERecord((RACERecord *)SourceRecord));
            newRecord = RACE.Records.back();
            break;
        case 'NUOS':
            SOUN.Records.push_back(new SOUNRecord((SOUNRecord *)SourceRecord));
            newRecord = SOUN.Records.back();
            break;
        case 'LIKS':
            SKIL.Records.push_back(new SKILRecord((SKILRecord *)SourceRecord));
            newRecord = SKIL.Records.back();
            break;
        case 'FEGM':
            if(RecordEditorID == NULL && SourceRecord == NULL)
                return NULL;

            MGEF.Records.push_back(new MGEFRecord((MGEFRecord *)SourceRecord));
            newRecord = MGEF.Records.back();

            if(RecordEditorID != NULL)
                {
                if(((MGEFRecord *)newRecord)->OBME.IsLoaded())
                    {
                    memcpy(&(((MGEFRecord *)newRecord)->OBME->EDDX.value.mgefCode)[0], RecordEditorID, 4);
                    ((MGEFRecord *)newRecord)->OBME->EDDX.value.mgefCode[4] = 0x00;
                    }
                else
                    ((MGEFRecord *)newRecord)->EDID.Copy(RecordEditorID);
                }
            break;
        case 'TPCS':
            SCPT.Records.push_back(new SCPTRecord((SCPTRecord *)SourceRecord));
            newRecord = SCPT.Records.back();
            break;
        case 'XETL':
            LTEX.Records.push_back(new LTEXRecord((LTEXRecord *)SourceRecord));
            newRecord = LTEX.Records.back();
            break;
        case 'HCNE':
            ENCH.Records.push_back(new ENCHRecord((ENCHRecord *)SourceRecord));
            newRecord = ENCH.Records.back();
            break;
        case 'LEPS':
            SPEL.Records.push_back(new SPELRecord((SPELRecord *)SourceRecord));
            newRecord = SPEL.Records.back();
            break;
        case 'NGSB':
            BSGN.Records.push_back(new BSGNRecord((BSGNRecord *)SourceRecord));
            newRecord = BSGN.Records.back();
            break;
        case 'ITCA':
            ACTI.Records.push_back(new ACTIRecord((ACTIRecord *)SourceRecord));
            newRecord = ACTI.Records.back();
            break;
        case 'APPA':
            APPA.Records.push_back(new APPARecord((APPARecord *)SourceRecord));
            newRecord = APPA.Records.back();
            break;
        case 'OMRA':
            ARMO.Records.push_back(new ARMORecord((ARMORecord *)SourceRecord));
            newRecord = ARMO.Records.back();
            break;
        case 'KOOB':
            BOOK.Records.push_back(new BOOKRecord((BOOKRecord *)SourceRecord));
            newRecord = BOOK.Records.back();
            break;
        case 'TOLC':
            CLOT.Records.push_back(new CLOTRecord((CLOTRecord *)SourceRecord));
            newRecord = CLOT.Records.back();
            break;
        case 'TNOC':
            CONT.Records.push_back(new CONTRecord((CONTRecord *)SourceRecord));
            newRecord = CONT.Records.back();
            break;
        case 'ROOD':
            DOOR.Records.push_back(new DOORRecord((DOORRecord *)SourceRecord));
            newRecord = DOOR.Records.back();
            break;
        case 'RGNI':
            INGR.Records.push_back(new INGRRecord((INGRRecord *)SourceRecord));
            newRecord = INGR.Records.back();
            break;
        case 'HGIL':
            LIGH.Records.push_back(new LIGHRecord((LIGHRecord *)SourceRecord));
            newRecord = LIGH.Records.back();
            break;
        case 'CSIM':
            MISC.Records.push_back(new MISCRecord((MISCRecord *)SourceRecord));
            newRecord = MISC.Records.back();
            break;
        case 'TATS':
            STAT.Records.push_back(new STATRecord((STATRecord *)SourceRecord));
            newRecord = STAT.Records.back();
            break;
        case 'SARG':
            GRAS.Records.push_back(new GRASRecord((GRASRecord *)SourceRecord));
            newRecord = GRAS.Records.back();
            break;
        case 'EERT':
            TREE.Records.push_back(new TREERecord((TREERecord *)SourceRecord));
            newRecord = TREE.Records.back();
            break;
        case 'ROLF':
            FLOR.Records.push_back(new FLORRecord((FLORRecord *)SourceRecord));
            newRecord = FLOR.Records.back();
            break;
        case 'NRUF':
            FURN.Records.push_back(new FURNRecord((FURNRecord *)SourceRecord));
            newRecord = FURN.Records.back();
            break;
        case 'PAEW':
            WEAP.Records.push_back(new WEAPRecord((WEAPRecord *)SourceRecord));
            newRecord = WEAP.Records.back();
            break;
        case 'OMMA':
            AMMO.Records.push_back(new AMMORecord((AMMORecord *)SourceRecord));
            newRecord = AMMO.Records.back();
            break;
        case '_CPN':
            NPC_.Records.push_back(new NPC_Record((NPC_Record *)SourceRecord));
            newRecord = NPC_.Records.back();
            break;
        case 'AERC':
            CREA.Records.push_back(new CREARecord((CREARecord *)SourceRecord));
            newRecord = CREA.Records.back();
            break;
        case 'CLVL':
            LVLC.Records.push_back(new LVLCRecord((LVLCRecord *)SourceRecord));
            newRecord = LVLC.Records.back();
            break;
        case 'MGLS':
            SLGM.Records.push_back(new SLGMRecord((SLGMRecord *)SourceRecord));
            newRecord = SLGM.Records.back();
            break;
        case 'MYEK':
            KEYM.Records.push_back(new KEYMRecord((KEYMRecord *)SourceRecord));
            newRecord = KEYM.Records.back();
            break;
        case 'HCLA':
            ALCH.Records.push_back(new ALCHRecord((ALCHRecord *)SourceRecord));
            newRecord = ALCH.Records.back();
            break;
        case 'PSBS':
            SBSP.Records.push_back(new SBSPRecord((SBSPRecord *)SourceRecord));
            newRecord = SBSP.Records.back();
            break;
        case 'TSGS':
            SGST.Records.push_back(new SGSTRecord((SGSTRecord *)SourceRecord));
            newRecord = SGST.Records.back();
            break;
        case 'ILVL':
            LVLI.Records.push_back(new LVLIRecord((LVLIRecord *)SourceRecord));
            newRecord = LVLI.Records.back();
            break;
        case 'RHTW':
            WTHR.Records.push_back(new WTHRRecord((WTHRRecord *)SourceRecord));
            newRecord = WTHR.Records.back();
            break;
        case 'TMLC':
            CLMT.Records.push_back(new CLMTRecord((CLMTRecord *)SourceRecord));
            newRecord = CLMT.Records.back();
            break;
        case 'NGER':
            REGN.Records.push_back(new REGNRecord((REGNRecord *)SourceRecord));
            newRecord = REGN.Records.back();
            break;
        case 'LLEC':
            if(ParentRecord == NULL)
                {
                CELL.Records.push_back(new CELLRecord((CELLRecord *)SourceRecord));
                newRecord = CELL.Records.back();

                ((CELLRecord *)newRecord)->IsInterior(true);
                }
            else
                {
                if(ParentRecord->GetType() != 'DLRW')
                    return NULL;

                if(options.CopyWorldCellStatus)
                    {
                    if(((WRLDRecord *)((CELLRecord *)SourceRecord)->Parent)->CELL->formID == SourceRecord->formID)
                        options.SetAsWorldCell = true;
                    else
                        options.SetAsWorldCell = false;
                    }

                if(options.SetAsWorldCell)
                    {
                    //If a world cell already exists, return it instead of making a new one
                    if(((WRLDRecord *)ParentRecord)->CELL != NULL)
                        return ((WRLDRecord *)ParentRecord)->CELL;

                    ((WRLDRecord *)ParentRecord)->CELL = new CELLRecord((CELLRecord *)SourceRecord);
                    newRecord = ((WRLDRecord *)ParentRecord)->CELL;
                    }
                else
                    {
                    ((WRLDRecord *)ParentRecord)->CELLS.push_back(new CELLRecord((CELLRecord *)SourceRecord));
                    newRecord = ((WRLDRecord *)ParentRecord)->CELLS.back();
                    }

                ((CELLRecord *)newRecord)->IsInterior(false);
                ((CELLRecord *)newRecord)->Parent = ParentRecord;
                }
            break;
        case 'DLRW':
            WRLD.Records.push_back(new WRLDRecord((WRLDRecord *)SourceRecord));
            newRecord = WRLD.Records.back();
            break;
        case 'LAID':
            DIAL.Records.push_back(new DIALRecord((DIALRecord *)SourceRecord));
            newRecord = DIAL.Records.back();
            break;
        case 'DRGP':
            if(ParentRecord == NULL || ParentRecord->GetType() != 'LLEC')
                return NULL;

            //If a cell pathgrid already exists, return it instead of making a new one
            if(((CELLRecord *)ParentRecord)->PGRD != NULL)
                return ((CELLRecord *)ParentRecord)->PGRD;

            ((CELLRecord *)ParentRecord)->PGRD = new PGRDRecord((PGRDRecord *)SourceRecord);
            newRecord = ((CELLRecord *)ParentRecord)->PGRD;
            break;
        case 'DNAL':
            if(ParentRecord == NULL || ParentRecord->GetType() != 'LLEC')
                return NULL;

            //If a cell land already exists, return it instead of making a new one
            if(((CELLRecord *)ParentRecord)->LAND != NULL)
                return ((CELLRecord *)ParentRecord)->LAND;

            ((CELLRecord *)ParentRecord)->LAND = new LANDRecord((LANDRecord *)SourceRecord);
            newRecord = ((CELLRecord *)ParentRecord)->LAND;
            break;
        case 'RHCA':
            if(ParentRecord == NULL || ParentRecord->GetType() != 'LLEC')
                return NULL;

            ((CELLRecord *)ParentRecord)->ACHR.push_back(new ACHRRecord((ACHRRecord *)SourceRecord));
            newRecord = ((CELLRecord *)ParentRecord)->ACHR.back();
            break;
        case 'ERCA':
            if(ParentRecord == NULL || ParentRecord->GetType() != 'LLEC')
                return NULL;

            ((CELLRecord *)ParentRecord)->ACRE.push_back(new ACRERecord((ACRERecord *)SourceRecord));
            newRecord = ((CELLRecord *)ParentRecord)->ACRE.back();
            break;
        case 'RFER':
            if(ParentRecord == NULL || ParentRecord->GetType() != 'LLEC')
                return NULL;
            ((CELLRecord *)ParentRecord)->REFR.push_back(new REFRRecord((REFRRecord *)SourceRecord));
            newRecord = ((CELLRecord *)ParentRecord)->REFR.back();
            break;
        case 'DAOR':
            if(ParentRecord == NULL || ParentRecord->GetType() != 'DLRW')
                return NULL;

            //If a world road already exists, return it instead of making a new one
            if(((WRLDRecord *)ParentRecord)->ROAD != NULL)
                return ((WRLDRecord *)ParentRecord)->ROAD;

            ((WRLDRecord *)ParentRecord)->ROAD = new ROADRecord((ROADRecord *)SourceRecord);
            newRecord = ((WRLDRecord *)ParentRecord)->ROAD;
            break;
        case 'OFNI':
            if(ParentRecord == NULL || ParentRecord->GetType() != 'LAID')
                return NULL;

            ((DIALRecord *)ParentRecord)->INFO.push_back(new INFORecord((INFORecord *)SourceRecord));
            newRecord = ((DIALRecord *)ParentRecord)->INFO.back();
            break;
        case 'TSUQ':
            QUST.Records.push_back(new QUSTRecord((QUSTRecord *)SourceRecord));
            newRecord = QUST.Records.back();
            break;
        case 'ELDI':
            IDLE.Records.push_back(new IDLERecord((IDLERecord *)SourceRecord));
            newRecord = IDLE.Records.back();
            break;
        case 'KCAP':
            PACK.Records.push_back(new PACKRecord((PACKRecord *)SourceRecord));
            newRecord = PACK.Records.back();
            break;
        case 'YTSC':
            CSTY.Records.push_back(new CSTYRecord((CSTYRecord *)SourceRecord));
            newRecord = CSTY.Records.back();
            break;
        case 'RCSL':
            LSCR.Records.push_back(new LSCRRecord((LSCRRecord *)SourceRecord));
            newRecord = LSCR.Records.back();
            break;
        case 'PSVL':
            LVSP.Records.push_back(new LVSPRecord((LVSPRecord *)SourceRecord));
            newRecord = LVSP.Records.back();
            break;
        case 'OINA':
            ANIO.Records.push_back(new ANIORecord((ANIORecord *)SourceRecord));
            newRecord = ANIO.Records.back();
            break;
        case 'RTAW':
            WATR.Records.push_back(new WATRRecord((WATRRecord *)SourceRecord));
            newRecord = WATR.Records.back();
            break;
        case 'HSFE':
            EFSH.Records.push_back(new EFSHRecord((EFSHRecord *)SourceRecord));
            newRecord = EFSH.Records.back();
            break;
        default:
            printf("Error creating record: %c%c%c%c\n", ((char *)&RecordType)[0], ((char *)&RecordType)[1], ((char *)&RecordType)[2], ((char *)&RecordType)[3]);
            break;
        }
    return newRecord;
    }

SINT32 TES4File::CleanMasters(std::vector<FormIDResolver *> &Expanders)
    {
    if(Flags.IsNoLoad)
        return -1;

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
        if(GMST.VisitRecords(NULL, checker, false)) continue;
        if(GLOB.VisitRecords(NULL, checker, false)) continue;
        if(CLAS.VisitRecords(NULL, checker, false)) continue;
        if(FACT.VisitRecords(NULL, checker, false)) continue;
        if(HAIR.VisitRecords(NULL, checker, false)) continue;
        if(EYES.VisitRecords(NULL, checker, false)) continue;
        if(RACE.VisitRecords(NULL, checker, false)) continue;
        if(SOUN.VisitRecords(NULL, checker, false)) continue;
        if(SKIL.VisitRecords(NULL, checker, false)) continue;
        if(MGEF.VisitRecords(NULL, checker, false)) continue;
        if(SCPT.VisitRecords(NULL, checker, false)) continue;
        if(LTEX.VisitRecords(NULL, checker, false)) continue;
        if(ENCH.VisitRecords(NULL, checker, false)) continue;
        if(SPEL.VisitRecords(NULL, checker, false)) continue;
        if(BSGN.VisitRecords(NULL, checker, false)) continue;
        if(ACTI.VisitRecords(NULL, checker, false)) continue;
        if(APPA.VisitRecords(NULL, checker, false)) continue;
        if(ARMO.VisitRecords(NULL, checker, false)) continue;
        if(BOOK.VisitRecords(NULL, checker, false)) continue;
        if(CLOT.VisitRecords(NULL, checker, false)) continue;
        if(CONT.VisitRecords(NULL, checker, false)) continue;
        if(DOOR.VisitRecords(NULL, checker, false)) continue;
        if(INGR.VisitRecords(NULL, checker, false)) continue;
        if(LIGH.VisitRecords(NULL, checker, false)) continue;
        if(MISC.VisitRecords(NULL, checker, false)) continue;
        if(STAT.VisitRecords(NULL, checker, false)) continue;
        if(GRAS.VisitRecords(NULL, checker, false)) continue;
        if(TREE.VisitRecords(NULL, checker, false)) continue;
        if(FLOR.VisitRecords(NULL, checker, false)) continue;
        if(FURN.VisitRecords(NULL, checker, false)) continue;
        if(WEAP.VisitRecords(NULL, checker, false)) continue;
        if(AMMO.VisitRecords(NULL, checker, false)) continue;
        if(NPC_.VisitRecords(NULL, checker, false)) continue;
        if(CREA.VisitRecords(NULL, checker, false)) continue;
        if(LVLC.VisitRecords(NULL, checker, false)) continue;
        if(SLGM.VisitRecords(NULL, checker, false)) continue;
        if(KEYM.VisitRecords(NULL, checker, false)) continue;
        if(ALCH.VisitRecords(NULL, checker, false)) continue;
        if(SBSP.VisitRecords(NULL, checker, false)) continue;
        if(SGST.VisitRecords(NULL, checker, false)) continue;
        if(LVLI.VisitRecords(NULL, checker, false)) continue;
        if(WTHR.VisitRecords(NULL, checker, false)) continue;
        if(CLMT.VisitRecords(NULL, checker, false)) continue;
        if(REGN.VisitRecords(NULL, checker, false)) continue;
        if(CELL.VisitRecords(NULL, checker, true)) continue;
        if(WRLD.VisitRecords(NULL, checker, true)) continue;
        if(DIAL.VisitRecords(NULL, checker, true)) continue;
        if(QUST.VisitRecords(NULL, checker, false)) continue;
        if(IDLE.VisitRecords(NULL, checker, false)) continue;
        if(PACK.VisitRecords(NULL, checker, false)) continue;
        if(CSTY.VisitRecords(NULL, checker, false)) continue;
        if(LSCR.VisitRecords(NULL, checker, false)) continue;
        if(LVSP.VisitRecords(NULL, checker, false)) continue;
        if(ANIO.VisitRecords(NULL, checker, false)) continue;
        if(WATR.VisitRecords(NULL, checker, false)) continue;
        if(EFSH.VisitRecords(NULL, checker, false)) continue;
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

SINT32 TES4File::CleanMasters2(std::vector<STRING> &MasterNames)
{
    if(Flags.IsNoLoad)
        return -1;

    UINT32 cleaned = 0;
    std::vector<UINT32> ToRemove;
    ToRemove.reserve(TES4.MAST.size());

    for(UINT32 p = 0; p < (UINT8)TES4.MAST.size();++p)
    {
		for(UINT32 i = 0; i < MasterNames.size(); ++i)
		{
			if (TES4.MAST[p].equalsi(StringRecord(MasterNames[i])))
			{
				ToRemove.push_back(p);
				++cleaned;
				break;
			}
		}
	}
    if(cleaned)
        {
        for(SINT32 p = (SINT32)ToRemove.size() - 1; p >= 0; --p)
            TES4.MAST.erase(TES4.MAST.begin() + ToRemove[p]);
        FormIDHandler.UpdateFormIDLookup();
        }
    return cleaned;
    }

SINT32 TES4File::Save(STRING const &SaveName, std::vector<FormIDResolver *> &Expanders, bool CloseMod)
    {
    if(!Flags.IsSaveable)
        return -1;

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
    formCount += GMST.WriteGRUP('TSMG', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += GLOB.WriteGRUP('BOLG', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += CLAS.WriteGRUP('SALC', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += FACT.WriteGRUP('TCAF', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += HAIR.WriteGRUP('RIAH', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += EYES.WriteGRUP('SEYE', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += RACE.WriteGRUP('ECAR', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += SOUN.WriteGRUP('NUOS', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += SKIL.WriteGRUP('LIKS', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += MGEF.WriteGRUP('FEGM', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += SCPT.WriteGRUP('TPCS', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += LTEX.WriteGRUP('XETL', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += ENCH.WriteGRUP('HCNE', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += SPEL.WriteGRUP('LEPS', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += BSGN.WriteGRUP('NGSB', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += ACTI.WriteGRUP('ITCA', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += APPA.WriteGRUP('APPA', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += ARMO.WriteGRUP('OMRA', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += BOOK.WriteGRUP('KOOB', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += CLOT.WriteGRUP('TOLC', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += CONT.WriteGRUP('TNOC', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += DOOR.WriteGRUP('ROOD', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += INGR.WriteGRUP('RGNI', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += LIGH.WriteGRUP('HGIL', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += MISC.WriteGRUP('CSIM', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += STAT.WriteGRUP('TATS', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += GRAS.WriteGRUP('SARG', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += TREE.WriteGRUP('EERT', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += FLOR.WriteGRUP('ROLF', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += FURN.WriteGRUP('NRUF', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += WEAP.WriteGRUP('PAEW', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += AMMO.WriteGRUP('OMMA', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += NPC_.WriteGRUP('_CPN', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += CREA.WriteGRUP('AERC', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += LVLC.WriteGRUP('CLVL', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += SLGM.WriteGRUP('MGLS', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += KEYM.WriteGRUP('MYEK', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += ALCH.WriteGRUP('HCLA', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += SBSP.WriteGRUP('PSBS', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += SGST.WriteGRUP('TSGS', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += LVLI.WriteGRUP('ILVL', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += WTHR.WriteGRUP('RHTW', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += CLMT.WriteGRUP('TMLC', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += REGN.WriteGRUP('NGER', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += CELL.WriteGRUP(SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += WRLD.WriteGRUP(SaveHandler, FormIDHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += DIAL.WriteGRUP(SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += QUST.WriteGRUP('TSUQ', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += IDLE.WriteGRUP('ELDI', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += PACK.WriteGRUP('KCAP', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += CSTY.WriteGRUP('YTSC', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += LSCR.WriteGRUP('RCSL', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += LVSP.WriteGRUP('PSVL', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += ANIO.WriteGRUP('OINA', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += WATR.WriteGRUP('RTAW', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);
    formCount += EFSH.WriteGRUP('HSFE', SaveHandler, Expanders, expander, collapser, bMastersChanged, CloseMod);

    //update formCount. Cheaper to go back and write it at the end than to calculate it before any writing.
    SaveHandler.writeAt(30, &formCount, 4);
    SaveHandler.close();
    if(CloseMod)
        Close();
    return 0;
    }

void TES4File::VisitAllRecords(RecordOp &op)
    {
    if(Flags.IsNoLoad)
        return;

    //This visits every record and subrecord
    GMST.VisitRecords(NULL, op, true);
    GLOB.VisitRecords(NULL, op, true);
    CLAS.VisitRecords(NULL, op, true);
    FACT.VisitRecords(NULL, op, true);
    HAIR.VisitRecords(NULL, op, true);
    EYES.VisitRecords(NULL, op, true);
    RACE.VisitRecords(NULL, op, true);
    SOUN.VisitRecords(NULL, op, true);
    SKIL.VisitRecords(NULL, op, true);
    MGEF.VisitRecords(NULL, op, true);
    SCPT.VisitRecords(NULL, op, true);
    LTEX.VisitRecords(NULL, op, true);
    ENCH.VisitRecords(NULL, op, true);
    SPEL.VisitRecords(NULL, op, true);
    BSGN.VisitRecords(NULL, op, true);
    ACTI.VisitRecords(NULL, op, true);
    APPA.VisitRecords(NULL, op, true);
    ARMO.VisitRecords(NULL, op, true);
    BOOK.VisitRecords(NULL, op, true);
    CLOT.VisitRecords(NULL, op, true);
    CONT.VisitRecords(NULL, op, true);
    DOOR.VisitRecords(NULL, op, true);
    INGR.VisitRecords(NULL, op, true);
    LIGH.VisitRecords(NULL, op, true);
    MISC.VisitRecords(NULL, op, true);
    STAT.VisitRecords(NULL, op, true);
    GRAS.VisitRecords(NULL, op, true);
    TREE.VisitRecords(NULL, op, true);
    FLOR.VisitRecords(NULL, op, true);
    FURN.VisitRecords(NULL, op, true);
    WEAP.VisitRecords(NULL, op, true);
    AMMO.VisitRecords(NULL, op, true);
    NPC_.VisitRecords(NULL, op, true);
    CREA.VisitRecords(NULL, op, true);
    LVLC.VisitRecords(NULL, op, true);
    SLGM.VisitRecords(NULL, op, true);
    KEYM.VisitRecords(NULL, op, true);
    ALCH.VisitRecords(NULL, op, true);
    SBSP.VisitRecords(NULL, op, true);
    SGST.VisitRecords(NULL, op, true);
    LVLI.VisitRecords(NULL, op, true);
    WTHR.VisitRecords(NULL, op, true);
    CLMT.VisitRecords(NULL, op, true);
    REGN.VisitRecords(NULL, op, true);
    CELL.VisitRecords(NULL, op, true);
    WRLD.VisitRecords(NULL, op, true);
    DIAL.VisitRecords(NULL, op, true);
    QUST.VisitRecords(NULL, op, true);
    IDLE.VisitRecords(NULL, op, true);
    PACK.VisitRecords(NULL, op, true);
    CSTY.VisitRecords(NULL, op, true);
    LSCR.VisitRecords(NULL, op, true);
    LVSP.VisitRecords(NULL, op, true);
    ANIO.VisitRecords(NULL, op, true);
    WATR.VisitRecords(NULL, op, true);
    EFSH.VisitRecords(NULL, op, true);
    return;
    }

void TES4File::VisitRecords(const UINT32 &TopRecordType, const UINT32 &RecordType, RecordOp &op, bool DeepVisit)
    {
    if(Flags.IsNoLoad)
        return;

    //This visits only the top records specified.
    switch(TopRecordType)
        {
        case 'TSMG':
            GMST.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'BOLG':
            GLOB.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'SALC':
            CLAS.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'TCAF':
            FACT.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'RIAH':
            HAIR.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'SEYE':
            EYES.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'ECAR':
            RACE.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'NUOS':
            SOUN.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'LIKS':
            SKIL.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'FEGM':
            MGEF.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'TPCS':
            SCPT.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'XETL':
            LTEX.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'HCNE':
            ENCH.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'LEPS':
            SPEL.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'NGSB':
            BSGN.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'ITCA':
            ACTI.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'APPA':
            APPA.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'OMRA':
            ARMO.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'KOOB':
            BOOK.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'TOLC':
            CLOT.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'TNOC':
            CONT.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'ROOD':
            DOOR.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'RGNI':
            INGR.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'HGIL':
            LIGH.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'CSIM':
            MISC.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'TATS':
            STAT.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'SARG':
            GRAS.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'EERT':
            TREE.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'ROLF':
            FLOR.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'NRUF':
            FURN.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'PAEW':
            WEAP.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'OMMA':
            AMMO.VisitRecords(RecordType, op, DeepVisit);
            break;
        case '_CPN':
            NPC_.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'AERC':
            CREA.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'CLVL':
            LVLC.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'MGLS':
            SLGM.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'MYEK':
            KEYM.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'HCLA':
            ALCH.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'PSBS':
            SBSP.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'TSGS':
            SGST.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'ILVL':
            LVLI.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'RHTW':
            WTHR.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'TMLC':
            CLMT.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'NGER':
            REGN.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'LLEC':
            CELL.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'DLRW':
            WRLD.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'LAID':
            DIAL.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'TSUQ':
            QUST.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'ELDI':
            IDLE.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'KCAP':
            PACK.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'YTSC':
            CSTY.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'RCSL':
            LSCR.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'PSVL':
            LVSP.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'OINA':
            ANIO.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'RTAW':
            WATR.VisitRecords(RecordType, op, DeepVisit);
            break;
        case 'HSFE':
            EFSH.VisitRecords(RecordType, op, DeepVisit);
            break;
        default:
            printf("Error visiting records: %i\n", RecordType);
            break;
        }
    return;
    }