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

class EFSHRecord : public Record
    {
    private:
        struct EFSHDATA
            {
            UINT8   flags, unused1[3];
            UINT32  memSBlend, memBlendOp, memZFunc;
            GENCLR  fill;
            FLOAT32 fillAIn, fillAFull, fillAOut, fillAPRatio, fillAAmp,
                    fillAFreq, fillAnimSpdU, fillAnimSpdV, edgeOff;
            GENCLR  edge;
            FLOAT32 edgeAIn, edgeAFull, edgeAOut, edgeAPRatio, edgeAAmp,
                    edgeAFreq,  fillAFRatio, edgeAFRatio;
            UINT32  memDBlend, partSBlend, partBlendOp, partZFunc, partDBlend;
            FLOAT32 partBUp, partBFull, partBDown, partBFRatio, partBPRatio, partLTime,
                    partLDelta, partNSpd, partNAcc, partVel1, partVel2, partVel3,
                    partAcc1, partAcc2, partAcc3, partKey1, partKey2, partKey1Time,
                    partKey2Time;
            GENCLR  key1;
            GENCLR  key2;
            GENCLR  key3;
            FLOAT32 key1A, key2A, key3A, key1Time, key2Time, key3Time;

            EFSHDATA();
            ~EFSHDATA();

            bool operator ==(const EFSHDATA &other) const;
            bool operator !=(const EFSHDATA &other) const;
            };

        enum flagsFlags
            {
            fIsNoMemShader  = 0x00000001,
            fIsNoPartShader = 0x00000008,
            fIsEdgeInverse  = 0x00000010,
            fIsMemSkinOnly  = 0x00000020
            };

    public:
        StringRecord EDID;
        StringRecord ICON;
        StringRecord ICO2;
        ReqSubRecord<EFSHDATA> DATA;

        EFSHRecord(unsigned char *_recData=NULL);
        EFSHRecord(EFSHRecord *srcRecord);
        ~EFSHRecord();

        bool   IsNoMemShader();
        void   IsNoMemShader(bool value);
        bool   IsNoMembraneShader();
        void   IsNoMembraneShader(bool value);
        bool   IsNoPartShader();
        void   IsNoPartShader(bool value);
        bool   IsNoParticleShader();
        void   IsNoParticleShader(bool value);
        bool   IsEdgeInverse();
        void   IsEdgeInverse(bool value);
        bool   IsEdgeEffectInverse();
        void   IsEdgeEffectInverse(bool value);
        bool   IsMemSkinOnly();
        void   IsMemSkinOnly(bool value);
        bool   IsMembraneShaderSkinOnly();
        void   IsMembraneShaderSkinOnly(bool value);
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

        bool operator ==(const EFSHRecord &other) const;
        bool operator !=(const EFSHRecord &other) const;
		virtual bool equals(const Record *other) const {return *this == *(EFSHRecord *)other;}
    };