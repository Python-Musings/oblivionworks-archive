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

class WTHRRecord : public Record
    {
    private:
        struct WTHRColors
            {
            GENCLR rise;
            GENCLR day;
            GENCLR set;
            GENCLR night;

            bool operator ==(const WTHRColors &other) const;
            bool operator !=(const WTHRColors &other) const;
            };

        struct WTHRNAM0
            {
            WTHRColors upperSky;
            WTHRColors fog;
            WTHRColors lowerClouds;
            WTHRColors ambient;
            WTHRColors sunlight;
            WTHRColors sun;
            WTHRColors stars;
            WTHRColors lowerSky;
            WTHRColors horizon;
            WTHRColors upperClouds;

            bool operator ==(const WTHRNAM0 &other) const;
            bool operator !=(const WTHRNAM0 &other) const;
            };

        struct WTHRFNAM
            {
            FLOAT32 fogDayNear, fogDayFar, fogNightNear, fogNightFar;

            WTHRFNAM();
            ~WTHRFNAM();

            bool operator ==(const WTHRFNAM &other) const;
            bool operator !=(const WTHRFNAM &other) const;
            };

        struct WTHRHNAM
            {
            FLOAT32 eyeAdaptSpeed, blurRadius, blurPasses, emissiveMult,
                    targetLum, upperLumClamp, brightScale, brightClamp,
                    lumRampNoTex, lumRampMin, lumRampMax, sunlightDimmer,
                    grassDimmer, treeDimmer;

            WTHRHNAM();
            ~WTHRHNAM();

            bool operator ==(const WTHRHNAM &other) const;
            bool operator !=(const WTHRHNAM &other) const;
            };

        struct WTHRDATA
            {
            UINT8   windSpeed, lowerCloudSpeed, upperCloudSpeed,
                    transDelta, sunGlare, sunDamage, rainFadeIn,
                    rainFadeOut, boltFadeIn, boltFadeOut, boltFrequency,
                    weatherType, boltRed, boltGreen, boltBlue;

            WTHRDATA();
            ~WTHRDATA();

            bool operator ==(const WTHRDATA &other) const;
            bool operator !=(const WTHRDATA &other) const;
            };

        struct WTHRSNAM
            {
            FORMID  sound;
            UINT32  type;

            enum eSoundType
                {
                eDefault = 0,
                ePrecip  = 1,
                eWind    = 2,
                eThunder = 3
                };

            WTHRSNAM();
            ~WTHRSNAM();

            bool IsDefault();
            void IsDefault(bool value);
            bool IsPrecip();
            void IsPrecip(bool value);
            bool IsPrecipitation();
            void IsPrecipitation(bool value);
            bool IsWind();
            void IsWind(bool value);
            bool IsThunder();
            void IsThunder(bool value);
            bool IsType(UINT32 Type);
            void SetType(UINT32 Type);

            bool operator ==(const WTHRSNAM &other) const;
            bool operator !=(const WTHRSNAM &other) const;
            };

        enum eWeatherType //actually flags, but all are exclusive(except unknowns)...so like a Type
            { //Manual hackery will make the CS think it is multiple types. It isn't known how the game would react.
            eNone     = 0x00,
            ePleasant = 0x01,
            eCloudy   = 0x02,
            eRainy    = 0x04,
            eSnow     = 0x08,
            fUnk1     = 0x40,
            fUnk2     = 0x80
            };

    public:
        StringRecord EDID;
        StringRecord CNAM;
        StringRecord DNAM;
        OptSubRecord<GENMODEL> MODL;
        ReqSubRecord<WTHRNAM0> NAM0;
        ReqSubRecord<WTHRFNAM> FNAM;
        ReqSubRecord<WTHRHNAM> HNAM;
        ReqSubRecord<WTHRDATA> DATA;
        std::vector<ReqSubRecord<WTHRSNAM> *> Sounds;

        WTHRRecord(unsigned char *_recData=NULL);
        WTHRRecord(WTHRRecord *srcRecord);
        ~WTHRRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsPleasant();
        void   IsPleasant(bool value);
        bool   IsCloudy();
        void   IsCloudy(bool value);
        bool   IsRainy();
        void   IsRainy(bool value);
        bool   IsSnow();
        void   IsSnow(bool value);
        bool   IsNone();
        void   IsNone(bool value);
        bool   IsUnk1();
        void   IsUnk1(bool value);
        bool   IsUnk2();
        void   IsUnk2(bool value);
        bool   IsType(UINT8 Type);
        void   SetType(UINT8 Type);
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

        bool operator ==(const WTHRRecord &other) const;
        bool operator !=(const WTHRRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(WTHRRecord *)other;}
    };