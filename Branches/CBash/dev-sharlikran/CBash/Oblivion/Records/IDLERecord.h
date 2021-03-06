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
#include "..\..\Common.h"
#include "..\..\GenericRecord.h"
#include <vector>

class IDLERecord : public Record
    {
    private:
        struct IDLEANAM
            {
            UINT8   group;

            IDLEANAM();
            ~IDLEANAM();

            bool operator ==(const IDLEANAM &other) const;
            bool operator !=(const IDLEANAM &other) const;
            };

        struct IDLEDATA
            {
            FORMID  parent, prevId;

            IDLEDATA();
            ~IDLEDATA();

            bool operator ==(const IDLEDATA &other) const;
            bool operator !=(const IDLEDATA &other) const;
            };

        enum groupType
            {
            eLowerBody   = 0x00,
            eLeftArm     = 0x01,
            eLeftHand    = 0x02,
            eRightArm    = 0x03,
            eSpecialIdle = 0x04,
            eWholeBody   = 0x05,
            eUpperBody   = 0x06
            };

        enum groupFlag
            {
            fIsNotReturnFile = 0x080
            };

    public:
        StringRecord EDID;
        OptSubRecord<GENMODEL> MODL;
        std::vector<ReqSubRecord<GENCTDA> *> CTDA;
        ReqSubRecord<IDLEANAM> ANAM;
        ReqSubRecord<IDLEDATA> DATA;

        IDLERecord(unsigned char *_recData=NULL);
        IDLERecord(IDLERecord *srcRecord);
        ~IDLERecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsLowerBody();
        void   IsLowerBody(bool value);
        bool   IsLeftArm();
        void   IsLeftArm(bool value);
        bool   IsLeftHand();
        void   IsLeftHand(bool value);
        bool   IsRightArm();
        void   IsRightArm(bool value);
        bool   IsSpecialIdle();
        void   IsSpecialIdle(bool value);
        bool   IsWholeBody();
        void   IsWholeBody(bool value);
        bool   IsUpperBody();
        void   IsUpperBody(bool value);
        bool   IsType(UINT8 Type);
        void   SetType(UINT8 Type);
        bool   IsNotReturnFile();
        void   IsNotReturnFile(bool value);
        bool   IsReturnFile();
        void   IsReturnFile(bool value);
        bool   IsFlagMask(UINT8 Mask, bool Exact=false);
        void   SetFlagMask(UINT8 Mask);

        UINT32 GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, UINT32 WhichAttribute=0);
        void * GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool   SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, UINT32 ArraySize=0);
        void   DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        UINT32 GetSize(bool forceCalc=false);
        UINT32 GetType();
        STRING GetStrType();

        SINT32 ParseRecord(unsigned char *buffer, const UINT32 &recSize);
        SINT32 Unload();
        SINT32 WriteRecord(_FileHandler &SaveHandler);

        bool operator ==(const IDLERecord &other) const;
        bool operator !=(const IDLERecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(IDLERecord *)other;}
    };