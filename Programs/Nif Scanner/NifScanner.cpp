#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "NifScanner.h"
#include "obj/NiObject.h"
#include "obj/NiAVObject.h"
#include "obj/NiNode.h"
#include "obj/NiProperty.h"
#include "obj/NiMaterialProperty.h"
#include "obj/NiTexturingProperty.h"
#include "obj/NiWireframeProperty.h"
#include "obj/NiAlphaProperty.h"
#include "obj/NiSourceTexture.h"
#include "obj/NiTriBasedGeom.h"
#include "obj/NiTriBasedGeomData.h"
#include "obj/NiBinaryExtraData.h"
#include "obj/NiTriStripsData.h"

#include <strstream>
#include <vector>

static LoadReturn loadedResult;
static vector<Subset> nodes;
static vector<NiTriBasedGeomRef> TriStrips;
static NiAVObjectRef rootObj;
static NifInfo rootInfo;
static char loadLog[10240];

SubsetInfo::SubsetInfo() { ZeroMemory(this,sizeof(SubsetInfo)); }
ExportedNode::ExportedNode() { ZeroMemory(this,sizeof(ExportedNode)); }
Subset::Subset(SubsetInfo si, ExportedNode d) { info=si; data=d; }
LoadReturn::LoadReturn() { ZeroMemory(this, sizeof(LoadReturn)); }

static void SearchShapes( NiAVObjectRef rootObj, vector<NiTriBasedGeomRef>* SubsetNodes ) {
    //Check if this object is derived from NiTriBasedGeom
    NiTriBasedGeomRef niGeom = DynamicCast<NiTriBasedGeom>( rootObj );
    if(niGeom) {
		SubsetNodes->push_back(niGeom);
	} else {
		//Check if this object derives from NiNode and, thus, may have children
		NiNodeRef niNode = DynamicCast<NiNode>( rootObj );
		if(niNode) {
			//Call this function for any children
			vector<NiAVObjectRef> children = niNode->GetChildren();
			for(uint i = 0; i < children.size(); i++) {
				SearchShapes( children[i], SubsetNodes );
			}
		}
	}
}

static bool ExportShape( NiTriBasedGeomRef niGeom, float* maxsize ) {
	ExportedNode node;
	SubsetInfo info;

	//Get data object (NiTriBasedGeomData) from geometry node (NiTriBasedGeom)
	NiTriBasedGeomDataRef niGeomData = DynamicCast<NiTriBasedGeomData>(niGeom->GetData());
	if(!niGeomData) _ERROR("Could not get NiTriBasedGeomData from subset");

	//Get number of indicies
	vector<Triangle> tris = niGeomData->GetTriangles();
	info.iCount=tris.size();

	//Get number of verticies
	info.vCount=niGeomData->GetVertexCount();
	if(info.vCount==0) _ERROR("Subset had no verticies");

	//Calculate mesh bounding sphere
	float radius = niGeomData->GetRadius();
	Vector3 center = niGeomData->GetCenter();
	radius = radius + sqrt(center.x*center.x + center.y*center.y + center.z*center.z);
	if(radius>*maxsize) *maxsize=radius;
	info.Radius=radius;
	info.Center=center;

	//Now that we're sure this mesh is valid, start the conversion
	if(info.iCount>0) {
		info.containsIndicies=true;
		node.iBuffer=new unsigned short[info.iCount*3];
		for(int i=0;i<info.iCount;i++) {
			node.iBuffer[i*3+0]=tris[i].v1;
			node.iBuffer[i*3+1]=tris[i].v2;
			node.iBuffer[i*3+2]=tris[i].v3;
		}
	}

	//Vertex data
	vector<Vector3> positions = niGeomData->GetVertices();
	vector<Vector3> normals = niGeomData->GetNormals();
	vector<Color4> colors = niGeomData->GetColors();

	//Verticies
	node.vBuffer=new ConvertedVertex[info.vCount];
	for(int i=0;i<info.vCount;i++) {
		node.vBuffer[i].Position[0]=positions[i].x;
		node.vBuffer[i].Position[1]=positions[i].y;
		node.vBuffer[i].Position[2]=positions[i].z;
	}
	if(niGeomData->GetUVSetCount()>0) {
		vector<TexCoord> texCoords = niGeomData->GetUVSet(0);
		for(int i=0;i<info.vCount;i++) {
			node.vBuffer[i].TexCoords[0]=texCoords[i].u;
			node.vBuffer[i].TexCoords[1]=texCoords[i].v;
		}
		info.containsTexCoords=true;
	}
	if(normals.size()>0) {
		for(int i=0;i<info.vCount;i++) {
			node.vBuffer[i].Normal[0]=normals[i].x;
			node.vBuffer[i].Normal[1]=normals[i].y;
			node.vBuffer[i].Normal[2]=normals[i].z;
		}
		info.containsNormals=true;
	} else {
		for(int i=0;i<info.vCount;i++) {
			node.vBuffer[i].Normal[0]=0;
			node.vBuffer[i].Normal[1]=0;
			node.vBuffer[i].Normal[2]=1;
		}
	}
	if(colors.size()>0) {
		for(int i=0;i<info.vCount;i++) {
			node.vBuffer[i].Color[0]=colors[i].r;
			node.vBuffer[i].Color[1]=colors[i].g;
			node.vBuffer[i].Color[2]=colors[i].b;
			node.vBuffer[i].Color[3]=colors[i].a;
		}
		info.containsColor=true;
	} else {
		for(int i=0;i<info.vCount;i++) {
			node.vBuffer[i].Color[0]=1;
			node.vBuffer[i].Color[1]=1;
			node.vBuffer[i].Color[2]=1;
			node.vBuffer[i].Color[3]=1;
		}
	}

	list<NiExtraDataRef> extraData = niGeom->GetExtraData();
	//bool foundExtraData=false;
	if(extraData.size()>0) {
		NiBinaryExtraDataRef data = DynamicCast<NiBinaryExtraData>(extraData.back());
		if(data) {
			vector<BYTE> bytes=data->GetData();
			if(bytes.size()==info.vCount*6*4) {
				/*for(int i=0;i<info.vCount;i++) {
					node.vBuffer[i].Tangent[0]=*(float*)&bytes[i*6*4+0];
					node.vBuffer[i].Tangent[1]=*(float*)&bytes[i*6*4+4];
					node.vBuffer[i].Tangent[2]=*(float*)&bytes[i*6*4+8];
					node.vBuffer[i].Binormal[0]=*(float*)&bytes[i*6*4+12];
					node.vBuffer[i].Binormal[1]=*(float*)&bytes[i*6*4+16];
					node.vBuffer[i].Binormal[2]=*(float*)&bytes[i*6*4+20];
				}*/
				//foundExtraData=true;
				info.containsTangentBinormal=true;
			}
		}
	}
	/*if(!foundExtraData) {
		for(int i=0;i<info.vCount;i++) {
			node.vBuffer[i].Tangent[0]=1;
			node.vBuffer[i].Tangent[1]=0;
			node.vBuffer[i].Tangent[2]=0;
			node.vBuffer[i].Binormal[0]=0;
			node.vBuffer[i].Binormal[1]=1;
			node.vBuffer[i].Binormal[2]=0;
		}
	}*/

	//Get material
	NiMaterialPropertyRef niMat = DynamicCast<NiMaterialProperty>(niGeom->GetPropertyByType(NiMaterialProperty::TYPE));
	bool foundMaterial=false;
	if(niMat) {
		node.mat.Ambient[0]=niMat->GetAmbientColor().r;
		node.mat.Ambient[1]=niMat->GetAmbientColor().g;
		node.mat.Ambient[2]=niMat->GetAmbientColor().b;
		node.mat.Diffuse[0]=niMat->GetDiffuseColor().r;
		node.mat.Diffuse[1]=niMat->GetDiffuseColor().g;
		node.mat.Diffuse[2]=niMat->GetDiffuseColor().b;
		node.mat.Emissive[0]=niMat->GetEmissiveColor().r;
		node.mat.Emissive[1]=niMat->GetEmissiveColor().g;
		node.mat.Emissive[2]=niMat->GetEmissiveColor().b;
		node.mat.Specular[0]=niMat->GetSpecularColor().r;
		node.mat.Specular[1]=niMat->GetSpecularColor().g;
		node.mat.Specular[2]=niMat->GetSpecularColor().b;
		node.mat.Power=niMat->GetGlossiness();
		info.alpha=niMat->GetTransparency();
	} else {
		node.mat.Ambient[0]=1;
		node.mat.Ambient[1]=1;
		node.mat.Ambient[2]=1;
		node.mat.Diffuse[0]=1;
		node.mat.Diffuse[1]=1;
		node.mat.Diffuse[2]=1;
		node.mat.Emissive[0]=0;
		node.mat.Emissive[1]=0;
		node.mat.Emissive[2]=0;
		node.mat.Specular[0]=0;
		node.mat.Specular[1]=0;
		node.mat.Specular[2]=0;
		node.mat.Power=100;
		info.alpha=1;
	}
	node.mat.Ambient[3]=1;
	node.mat.Diffuse[3]=1;
	node.mat.Emissive[3]=1;
	node.mat.Specular[3]=1;

	//A couple of misc properties
	NiWireframePropertyRef niWireframe = DynamicCast<NiWireframeProperty>(niGeom->GetPropertyByType(NiWireframeProperty::TYPE));
	if(niWireframe) {
		if(!niWireframe->GetFlags()) info.wireframe=false;
		else info.wireframe=true;
	} else info.wireframe=false;

	NiAlphaPropertyRef niAlpha = DynamicCast<NiAlphaProperty>(niGeom->GetPropertyByType(NiAlphaProperty::TYPE));
	if(niAlpha) info.hasalpha=1; else info.hasalpha=0;

	//Get texture file path
	NiTexturingPropertyRef niTexProp = DynamicCast<NiTexturingProperty>(niGeom->GetPropertyByType( NiTexturingProperty::TYPE ));
	if(niTexProp&&niTexProp->GetTextureCount()) {
		TexDesc texDesc=niTexProp->GetTexture(0);
		NiSourceTextureRef niSrcTex = texDesc.source;
		if(niSrcTex && niSrcTex->IsTextureExternal() ) {
			string s=niSrcTex->GetTextureFileName();
			for(uint i=0;i<s.size();i++) {
				if(s[i]=='/') s[i]='\\';
			}
			if(s[0]!='\0') {
				node.tex=new char[s.length()+1];
				strcpy(node.tex,s.c_str());
				info.containsTexture=true;
				info.applyMode=niTexProp->GetApplyMode();
			}
		}
	}

	//Get transformation of mesh as 4x4 radius
	node.transform = niGeom->GetWorldTransform();

	info.iCount*=3;
	nodes.push_back(Subset(info, node));
	TriStrips.push_back(niGeom);
	_LOG("Subset loaded sucessfully");
	return true;
}

BYTE _stdcall ResaveNif(const char* outpath, BYTE* shaders) {
	for(uint i=0;i<TriStrips.size();i++) {
		NiTexturingPropertyRef niTexProp = DynamicCast<NiTexturingProperty>(TriStrips[i]->GetPropertyByType( NiTexturingProperty::TYPE ));
		if(niTexProp&&niTexProp->GetTextureCount()) {
			niTexProp->SetApplyMode((ApplyMode)shaders[i]);
		}
	}
	WriteNifTree(outpath, rootObj, rootInfo);
	return 0;
}

static NiAVObjectRef LoadStreamNif(const char* data, int size) {
	istrstream s(data, size);
	NiAVObjectRef rootObj = DynamicCast<NiAVObject>(ReadNifTree( s, &rootInfo ));
	return rootObj;
}
static void Unload() {
	rootObj=0;
	for(uint i=0;i<nodes.size();i++) {
		SAFERELEASEA(nodes[i].data.iBuffer);
		SAFERELEASEA(nodes[i].data.vBuffer);
		SAFERELEASEA(nodes[i].data.tex);
	}
	nodes.clear();
	TriStrips.clear();
	loadLog[0]='\0';
}
LoadReturn _stdcall Load(char* data, int size) {
	Unload();
	vector<NiTriBasedGeomRef> SubsetNodes;
	int numNodes;

	LoadReturn lr=LoadReturn();
	lr.Subsets=-1;
	lr.log=loadLog;

	_LOG("Parsing nif data");
	try {
		rootObj=LoadStreamNif(data, size);
		if(!rootObj) {
			_LOG("No data found in nif file");
			return lr;
		}
	} catch(...) {
		_LOG("An exception was thrown while loading the nif");
		return lr;
	}
	
	_LOG("Searching for renderable nodes");
	try {
		SearchShapes(rootObj, &SubsetNodes);
		numNodes=SubsetNodes.size();
		if(!numNodes) {
			_LOG("No renderable nodes found");
			return lr;
		}
	} catch(...) {
		_LOG("An exception was thrown while searching for renderable nodes");
		return lr;
	}
	
	_LOG("Exporting renderable nodes");
	for(int i = 0;i<numNodes;i++) {
		try {
			if(!ExportShape(SubsetNodes[i], &lr.maxsize)) lr.FailedSubsets++;
		} catch(...) {
			_LOG("An exception was thrown while exporting a node");
			lr.FailedSubsets++;
		}
	}

	if(nodes.size()==0) {
		_LOG("No renderable nodes were able to be converted");
		return lr;
	}

	_LOG("Nif load complete");
	lr.Subsets=nodes.size();
	loadedResult=lr;
	return lr;
}

bool _stdcall GetSubsetMaterial(uint ID, Material* mat) { IDCHECK(ID, *mat=nodes[ID].data.mat) };
bool _stdcall GetSubsetVerticies(uint ID, ConvertedVertex* vert) { IDCHECK(ID, memcpy(vert, nodes[ID].data.vBuffer, nodes[ID].info.vCount*sizeof(ConvertedVertex))) };
bool _stdcall GetSubsetTransform(uint ID, Matrix44* mat) { IDCHECK(ID, *mat=nodes[ID].data.transform) };
bool _stdcall GetSubsetInfo(uint ID, SubsetInfo* mat) { IDCHECK(ID, *mat=nodes[ID].info) };
bool _stdcall GetSubsetTex(uint ID, char** tex) { IDCHECK(ID, *tex=nodes[ID].data.tex) };
bool _stdcall GetSubsetIndicies(uint ID, short* data) { IDCHECK(ID, memcpy(data, nodes[ID].data.iBuffer, nodes[ID].info.iCount*2)) };
char* _stdcall GenList(char* data, int size) {
	Unload();
	vector<NiTriBasedGeomRef> SubsetNodes;
	loadLog[0]='\0';

	try {
		rootObj=LoadStreamNif(data, size);
		if(!rootObj) return loadLog;
	} catch(...) {
		return loadLog;
	}

	try {
		SearchShapes(rootObj, &SubsetNodes);
	} catch(...) {
		return loadLog;
	}

	for(uint i = 0;i<SubsetNodes.size();i++) {
		try {
			NiTexturingPropertyRef niTexProp = DynamicCast<NiTexturingProperty>(SubsetNodes[i]->GetPropertyByType( NiTexturingProperty::TYPE ));
			if(niTexProp&&niTexProp->GetTextureCount()) {
				TexDesc texDesc=niTexProp->GetTexture(0);
				NiSourceTextureRef niSrcTex = texDesc.source;
				if(niSrcTex && niSrcTex->IsTextureExternal() ) {
					string s=niSrcTex->GetTextureFileName();
					if(s[0]!='\0') {
						strcat_s(loadLog, s.c_str());
						strcat_s(loadLog, "*");
					}
				}
			}
		} catch(...) { }
	}

	return loadLog;
}
/*BOOL _stdcall FixTexPaths(char* data, char size, char* temppath) {
	Unload();
	vector<NiTriBasedGeomRef> SubsetNodes;
	loadLog[0]='\0';

	try {
		rootObj=LoadStreamNif(data, size);
		if(!rootObj) return 0;
	} catch(...) {
		return 0;
	}

	try {
		SearchShapes(rootObj, &SubsetNodes);
	} catch(...) {
		return 0;
	}

	BYTE found=0;
	for(uint i = 0;i<SubsetNodes.size();i++) {
		try {
			NiTexturingPropertyRef niTexProp = DynamicCast<NiTexturingProperty>(SubsetNodes[i]->GetPropertyByType( NiTexturingProperty::TYPE ));
			if(niTexProp&&niTexProp->GetTextureCount()) {
				TexDesc texDesc=niTexProp->GetTexture(0);
				NiSourceTextureRef niSrcTex = texDesc.source;
				if(niSrcTex && niSrcTex->IsTextureExternal() ) {
					string s=niSrcTex->GetTextureFileName();
					for(uint i=0;i<s.size();i++) {
						if(s[i]=='/') {
							found=1;
							s[i]='\\';
						}
					}
				}
			}
		} catch(...) { }
	}

	if(found) {
		WriteNifTree(temppath, rootObj, rootInfo);
	}

	return found;
}*/