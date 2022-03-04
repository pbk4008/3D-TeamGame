#pragma once

namespace Engine
{
	typedef struct tagEffectDesc
	{
		ID3D11InputLayout* pInputLayout = nullptr;
		ID3DX11EffectPass* pPass = nullptr;
	}EFFECTDESC;
	/* Ư���ð��뿡 ǥ���ؾ��� ������� ���� */
	typedef struct tagCamera
	{
		XMFLOAT4X4 matView;
		XMFLOAT4X4 matProj;
		XMFLOAT4 vCamPos;
	}CAMERA;
	typedef struct tagKeyFrame
	{
		double			Time;
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
	}KEYFRAME;

	typedef struct MeshMaterial
	{
		_tchar		pMeshTextureName[AI_TEXTURE_TYPE_MAX][MAX_PATH];
		class CTexture*		pMeshTexture[AI_TEXTURE_TYPE_MAX];
	}MESHMATERIAL;

	typedef struct tagBoneDesc
	{
		/* ������ ���� -> ������ ���ý����̽� ��ȯ�� ���� ����̴�. */
		XMFLOAT4X4		OffsetMatrix;
		class CHierarchyNode*	pNode;
	}BONEDESC;

	typedef struct tagVertex_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		//XMFLOAT3		vBiNormal;
	}VTXMESH;

	typedef struct tagVertex_Anim : public VTXMESH
	{
		XMUINT4			vBlendIndex; /* ���� ������ ���º�ȯ�� ���� ������ �ִ� ���� �ε��� (�ִ� �װ�) */
		XMFLOAT4		vBlendWeight; /* ������ �ִ� ���� ����ġ (����� �ִ밡 1 ) */
	}VTXMESH_ANIM;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType = TYPE_END;
		/* ���⼺ ������ ����. */
		XMFLOAT3		vDirection;
		XMFLOAT3		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	}LIGHTDESC;

	typedef struct tagMtrlDesc
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
		XMFLOAT4		vEmissive;
	}MTRLDESC;

	// (L.Diffuse * M.Diffuse) * (V.Shade + (L.Ambient * M.Ambient))

	typedef struct tagVertex_Matrix
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vPosition;		
	} VTXMATRIX;

	typedef struct tagVertex_Texture_Cube
	{
		XMFLOAT3		vPosition;		
		XMFLOAT3		vTexUV;
	} VTXCUBETEX;

	typedef struct tagVertex_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	} VTXTEX;

	typedef struct tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
	} VTXPOINT;

	typedef struct tagVertex_Color
	{
		XMFLOAT3		vPosition;
		XMFLOAT4		vColor;		
	} VTXCOL;

	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	} VTXNORTEX;

	typedef struct tagLineIndices_16
	{
		unsigned short		_0, _1;
	} LINEINDICES16;

	typedef struct tagFaceIndices_16
	{
		unsigned short		_0, _1, _2;
	} FACEINDICES16;

	typedef struct tagFaceIndices_32
	{
		unsigned long		_0, _1, _2;
	} FACEINDICES32;
}
