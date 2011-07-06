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
#include "..\..\Common.h"
#include "GMSTRecord.h"

GMSTRecord::GMSTDATA::GMSTDATA(STRING _DATA):
    s(_DATA),
    format('s'),
    i(0)
    {
    //
    }

GMSTRecord::GMSTDATA::GMSTDATA(SINT32 _DATA):
    s(NULL),
    format('i'),
    i(_DATA)
    {
    //
    }

GMSTRecord::GMSTDATA::GMSTDATA(FLOAT32 _DATA):
    s(NULL),
    format('f'),
    f(_DATA)
    {
    //
    }

GMSTRecord::GMSTDATA::GMSTDATA():
    format(0),
    s(NULL),
    i(0)
    {
    //
    }

GMSTRecord::GMSTDATA::~GMSTDATA()
    {
    delete []s;
    }

bool GMSTRecord::GMSTDATA::operator ==(const GMSTDATA &other) const
    {
    if(format != other.format)
        return false;

    switch(format)
        {
        case 's':
            return strcmp(s, other.s) == 0;
        case 'i':
            return i == other.i;
        case 'f':
            return AlmostEqual(f, other.f, 2);
        default:
            return false;
        }
    }

bool GMSTRecord::GMSTDATA::operator !=(const GMSTDATA &other) const
    {
    return !(*this == other);
    }

GMSTRecord::GMSTRecord(unsigned char *_recData):
    Record(_recData)
    {
    //
    }

GMSTRecord::GMSTRecord(GMSTRecord *srcRecord):
    Record()
    {
    if(srcRecord == NULL)
        return;

    flags = srcRecord->flags;
    formID = srcRecord->formID;
    flagsUnk = srcRecord->flagsUnk;
    EDID = srcRecord->EDID;

    if(!srcRecord->IsChanged())
        {
        IsLoaded(false);
        recData = srcRecord->recData;
        return;
        }

    DATA.format = srcRecord->DATA.format;
    UINT32 vSize;
    switch(DATA.format)
        {
        case 'f':
            DATA.f = srcRecord->DATA.f;
            break;
        case 'i':
            DATA.i = srcRecord->DATA.i;
            break;
        case 's':
            vSize = (UINT32)strlen(srcRecord->DATA.s) + 1;
            DATA.s = new char [vSize];
            strcpy_s(DATA.s, vSize, srcRecord->DATA.s);
            break;
        default:
            break;
        }
    return;
    }

GMSTRecord::~GMSTRecord()
    {
    //
    }

UINT32 GMSTRecord::GetSize(bool forceCalc)
    {
    if(!forceCalc && !IsChanged())
        return *(UINT32*)&recData[-16];

    UINT32 cSize = 0;
    UINT32 TotSize = 0;

    if(EDID.IsLoaded())
        {
        cSize = EDID.GetSize();
        if(cSize > 65535) cSize += 10;
        TotSize += cSize += 6;
        }

    switch(DATA.format)
        {
        case 'i':
        case 'f':
            TotSize += 10;
            break;
        case 's':
            if(DATA.s != NULL)
                {
                cSize = (UINT32)strlen(DATA.s) + 1;
                if(cSize > 65535) cSize += 10;
                }
            else
                cSize = 1;
            TotSize += cSize += 6;
            break;
        default:
            printf("Unknown GMST format (%c) in GetSize(): %s\n", DATA.format, EDID.value);
            break;
        }

    return TotSize;
    }

UINT32 GMSTRecord::GetType()
    {
    return 'TSMG';
    }

STRING GMSTRecord::GetStrType()
    {
    return "GMST";
    }

bool GMSTRecord::IsKeyedByEditorID()
    {
    return true;
    }

SINT32 GMSTRecord::ParseRecord(unsigned char *buffer, const UINT32 &recSize)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    UINT32 curPos = 0;
    while(curPos < recSize){
        _readBuffer(&subType, buffer, 4, curPos);
        switch(subType)
            {
            case 'XXXX':
                curPos += 2;
                _readBuffer(&subSize, buffer, 4, curPos);
                _readBuffer(&subType, buffer, 4, curPos);
                curPos += 2;
                break;
            default:
                subSize = 0;
                _readBuffer(&subSize, buffer, 2, curPos);
                break;
            }
        switch(subType)
            {
            case 'DIDE':
                EDID.Read(buffer, subSize, curPos);
                break;
            case 'ATAD':
                DATA.format = EDID.value[0];
                switch(DATA.format)
                    {
                    case 's':
                        DATA.s = new char[subSize];
                        memcpy(DATA.s, buffer + curPos, subSize);
                        curPos += subSize;
                        break;
                    case 'i':
                        memcpy(&DATA.i, buffer + curPos, subSize);
                        curPos += subSize;
                        break;
                    case 'f':
                        memcpy(&DATA.f, buffer + curPos, subSize);
                        curPos += subSize;
                        break;
                    default:
                        //printf("FileName = %s\n", FileName);
                        printf("  GMST: %08X - Unknown type = %c\n", formID, DATA.format);
                        printf("  Size = %i\n", subSize);
                        printf("  CurPos = %04x\n\n", curPos - 6);
                        curPos = recSize;
                        break;
                    }
                break;
            default:
                //printf("FileName = %s\n", FileName);
                printf("  GMST: Unknown subType = %04X\n", subType);
                printf("  Size = %i\n", subSize);
                printf("  CurPos = %04x\n\n", curPos - 6);
                curPos = recSize;
                break;
            }
        };
    //GMSTs should always be loaded since they're keyed by editorID
    //By marking it as changed, it prevents the record from being unloaded
    //IsChanged(true);
    return 0;
    }

SINT32 GMSTRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    //Don't unload EDID since it is used to index the record
    if(DATA.format == 's')
        delete []DATA.s;
    DATA.i = 0;
    DATA.s = NULL;
    DATA.format = 0;
    return 1;
    }

SINT32 GMSTRecord::WriteRecord(_FileHandler &SaveHandler)
    {
    if(EDID.IsLoaded())
        SaveHandler.writeSubRecord('DIDE', EDID.value, EDID.GetSize());
    UINT8 null = 0;
    switch(DATA.format)
        {
        case 'i':
            SaveHandler.writeSubRecord('ATAD', &DATA.i, 4);
            break;
        case 'f':
            SaveHandler.writeSubRecord('ATAD', &DATA.f, 4);
            break;
        case 's':
            if(DATA.s != NULL)
                SaveHandler.writeSubRecord('ATAD', DATA.s, (UINT32)strlen(DATA.s) + 1);
            else
                SaveHandler.writeSubRecord('ATAD', &null, 1);
            break;
        default:
            printf("Unknown GMST format (%c) when writing: %s\n", DATA.format, EDID.value);
        }
    return -1;
    }

bool GMSTRecord::operator ==(const GMSTRecord &other) const
    {
    return (EDID.equalsi(other.EDID) &&
            DATA == other.DATA);
    }

bool GMSTRecord::operator !=(const GMSTRecord &other) const
    {
    return !(*this == other);
    }