#include "niflib.h"
#include <d3d9.h>
using namespace Niflib;

#define uint unsigned int

#define SAFERELEASEP(a) { if(a) { delete   a; } a=0; }
#define SAFERELEASEA(a) { if(a) { delete[] a; } a=0; }

#define IDCHECK(a,b) if(a<0||a>=nodes.size()) return false; b; return true;
#define _LOG(a) { strcat_s(loadLog, a "\r\n"); }
#define _ERROR(a) { strcat_s(loadLog, a "\r\n"); return false; }

struct SubsetInfo {
	Vector3 Center;
	float Radius;
	float alpha;
	int vCount;
	int iCount;
	BYTE containsTexCoords;
	BYTE containsNormals;
	BYTE containsColor;
	BYTE containsTangentBinormal;
	BYTE containsTexture;
	BYTE containsIndicies;
	BYTE applyMode;
	BYTE wireframe;
	BYTE hasalpha;

	SubsetInfo();
};

struct ConvertedVertex {
	float Position[3];
	float Tangent[3];
	float Binormal[3];
	float Normal[3];
	float TexCoords[2];
	float Color[4];
};

struct Material {
    float Diffuse[4];
    float Ambient[4];
    float Specular[4];
    float Emissive[4];
    float Power;
};

struct ExportedNode {
	Matrix44 transform;
	ConvertedVertex* vBuffer;
	unsigned short* iBuffer;
	char* tex;
	Material mat;

	ExportedNode();
};

struct Subset {
	SubsetInfo info;
	ExportedNode data;

	Subset(SubsetInfo si, ExportedNode d);
};

struct LoadReturn {
	int Subsets;
	int FailedSubsets;
	float maxsize;
	char* log;

	LoadReturn();
};