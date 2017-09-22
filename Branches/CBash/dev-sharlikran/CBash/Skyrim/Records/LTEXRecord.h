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
#pragma once
#include "..\..\Common.h"
#include "..\..\GenericRecord.h"

namespace Sk
{
class LTEXRecord : public TES5Record //Landscape Texture
    {
    private:
        struct LTEXHNAM
            {
            UINT8   types, friction, restitution;

            LTEXHNAM();
            ~LTEXHNAM();

            bool operator ==(const LTEXHNAM &other) const;
            bool operator !=(const LTEXHNAM &other) const;
            };

        enum typesTypes
            {
            eStone = 0,
            eCloth,
            eDirt,
            eGlass,
            eGrass,
            eMetal,
            eOrganic,
            eSkin,
            eWater,
            eWood,
            eHeavyStone,
            eHeavyMetal,
            eHeavyWood,
            eChain,
            eSnow,
            };

    public:
        StringRecord EDID; //Editor ID
        StringRecord ICON; //Large Icon Filename
        StringRecord MICO; //Small Icon Filename
        OptSimpleSubRecord<FORMID> TNAM; //Texture (Skyrim) -> TXST
        ReqSubRecord<LTEXHNAM> HNAM; //Havok Data
        ReqSimpleSubRecord<UINT8> SNAM; //Texture Specular Exponent
	UnorderedSparseArray<FORMID> GNAM; //Grasses
        ReqSimpleSubRecord<FORMID> MNAM; //Material (Skyrim) -> MATT

        LTEXRecord(unsigned char *_recData=NULL);
        LTEXRecord(LTEXRecord *srcRecord);
        ~LTEXRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsStone();
        void   IsStone(bool value);
        bool   IsCloth();
        void   IsCloth(bool value);
        bool   IsDirt();
        void   IsDirt(bool value);
        bool   IsGlass();
        void   IsGlass(bool value);
        bool   IsGrass();
        void   IsGrass(bool value);
        bool   IsMetal();
        void   IsMetal(bool value);
        bool   IsOrganic();
        void   IsOrganic(bool value);
        bool   IsSkin();
        void   IsSkin(bool value);
        bool   IsWater();
        void   IsWater(bool value);
        bool   IsWood();
        void   IsWood(bool value);
        bool   IsHeavyStone();
        void   IsHeavyStone(bool value);
        bool   IsHeavyMetal();
        void   IsHeavyMetal(bool value);
        bool   IsHeavyWood();
        void   IsHeavyWood(bool value);
        bool   IsChain();
        void   IsChain(bool value);
        bool   IsSnow();
        void   IsSnow(bool value);

        bool   IsType(UINT8 Type);
        void   SetType(UINT8 Type);

        UINT32 GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, UINT32 WhichAttribute=0);
        void * GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool   SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, UINT32 ArraySize=0);
        void   DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        UINT32 GetType();
        STRING GetStrType();

        SINT32 ParseRecord(unsigned char *buffer, unsigned char *end_buffer, bool CompressedOnDisk=false);
        SINT32 Unload();
        SINT32 WriteRecord(FileWriter &writer);

        bool operator ==(const LTEXRecord &other) const;
        bool operator !=(const LTEXRecord &other) const;
        bool equals(Record *other);
    };
}