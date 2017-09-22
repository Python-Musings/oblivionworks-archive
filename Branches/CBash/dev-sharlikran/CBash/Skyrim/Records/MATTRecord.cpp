/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is CBash code.
 *
 * The Initial Developer of the Original Code is
 * Waruddar.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * Ethatron
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#include "..\..\Common.h"
#include "MATTRecord.h"

namespace Sk
{
MATTRecord::MATTRecord(unsigned char *_recData):
    TES5Record(_recData)
    {
    //
    }

MATTRecord::MATTRecord(MATTRecord *srcRecord):
    TES5Record()
    {
    if(srcRecord == NULL)
        return;

    flags = srcRecord->flags;
    formID = srcRecord->formID;
    flagsUnk = srcRecord->flagsUnk;
    formVersion = srcRecord->formVersion;
    versionControl2[0] = srcRecord->versionControl2[0];
    versionControl2[1] = srcRecord->versionControl2[1];

    recData = srcRecord->recData;
    if(!srcRecord->IsChanged())
        return;

    EDID = srcRecord->EDID;
    MNAM = srcRecord->MNAM;
    CNAM = srcRecord->CNAM;
    BNAM = srcRecord->BNAM;
    FNAM = srcRecord->FNAM;
    HNAM = srcRecord->HNAM;
    PNAM = srcRecord->PNAM;
    return;
    }

MATTRecord::~MATTRecord()
    {
    //
    }

UINT32 MATTRecord::GetType()
    {
    return REV32(MATT);
    }

STRING MATTRecord::GetStrType()
    {
    return "MATT";
    }

SINT32 MATTRecord::ParseRecord(unsigned char *buffer, unsigned char *end_buffer, bool CompressedOnDisk)
    {
    UINT32 subType = 0;
    UINT32 subSize = 0;
    while(buffer < end_buffer){
        subType = *(UINT32 *)buffer;
        buffer += 4;
        switch(subType)
            {
            case REV32(XXXX):
                buffer += 2;
                subSize = *(UINT32 *)buffer;
                buffer += 4;
                subType = *(UINT32 *)buffer;
                buffer += 6;
                break;
            default:
                subSize = *(UINT16 *)buffer;
                buffer += 2;
                break;
            }
        switch(subType)
            {
	    case REV32(EDID): // MaterialInsect
                EDID.Read(buffer, subSize, CompressedOnDisk);
                break;
            case REV32(MNAM): // Insect
                MNAM.Read(buffer, subSize, CompressedOnDisk);
                break;
	    case REV32(CNAM): // 12 bytes, f1 f0 f0 3e - b9 b8 b8 3d - b9 b8 b8 3d, flags?
                CNAM.Read(buffer, subSize, CompressedOnDisk);
                break;
            case REV32(BNAM): // 4 bytes, 00 00 80 3e, flags
                BNAM.Read(buffer, subSize, CompressedOnDisk);
                break;
            case REV32(FNAM): // 4 bytes, 02 00 00 00, flags
                FNAM.Read(buffer, subSize, CompressedOnDisk);
                break;
            case REV32(HNAM): // 4 bytes, 86 a2 05 00, formID -> IPDS (PHYBodyMedium), Havok NAMe
                HNAM.Read(buffer, subSize);
		break;
	    case REV32(PNAM): // 4 bytes, 47 2f 01 00, formID -> MATT, Parent NAMe
		PNAM.Read(buffer, subSize);
		break;
            default:
                //printer("FileName = %s\n", FileName);
                printer("  MATT: %08X - Unknown subType = %04x [%c%c%c%c]\n", formID, subType, (subType >> 0) & 0xFF, (subType >> 8) & 0xFF, (subType >> 16) & 0xFF, (subType >> 24) & 0xFF);
                CBASH_CHUNK_DEBUG
                printer("  Size = %i\n", subSize);
                printer("  CurPos = %04x\n\n", buffer - 6);
                buffer = end_buffer;
                break;
            }
        };
    return 0;
    }

SINT32 MATTRecord::Unload()
    {
    IsChanged(false);
    IsLoaded(false);
    EDID.Unload();
    MNAM.Unload();
    CNAM.Unload();
    BNAM.Unload();
    FNAM.Unload();
    HNAM.Unload();
    PNAM.Unload();
    return 1;
    }

SINT32 MATTRecord::WriteRecord(FileWriter &writer)
    {
    WRITE(EDID);
    WRITE(MNAM);
    WRITE(CNAM);
    WRITE(BNAM);
    WRITE(FNAM);
    WRITE(HNAM);
    WRITE(PNAM);
    return -1;
    }

bool MATTRecord::operator ==(const MATTRecord &other) const
    {
    return (MNAM.equalsi(other.MNAM) &&
            CNAM == other.CNAM &&
            BNAM == other.BNAM &&
            FNAM == other.FNAM &&
	    HNAM == other.HNAM &&
	    PNAM == other.PNAM);
    }

bool MATTRecord::operator !=(const MATTRecord &other) const
    {
    return !(*this == other);
    }

bool MATTRecord::equals(Record *other)
    {
    return *this == *(MATTRecord *)other;
    }
}