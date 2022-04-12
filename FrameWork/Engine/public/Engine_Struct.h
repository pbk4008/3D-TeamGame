#pragma once

namespace Engine
{
	typedef struct tagEffectDesc
	{
		ID3D11InputLayout* pInputLayout = nullptr;
		ID3DX11EffectPass* pPass = nullptr;
	}EFFECTDESC;
	/* 특정시간대에 표현해야할 뼈대들의 상태 */
	typedef struct tagCamera
	{
		XMFLOAT4X4 matView;
		XMFLOAT4X4 matProj;
		XMFLOAT4 vCamPos;
	}CAMERA;
	typedef struct MeshMaterial
	{
		_tchar		pMeshTextureName[AI_TEXTURE_TYPE_MAX][MAX_PATH];
		class CTexture* pMeshTexture[AI_TEXTURE_TYPE_MAX];
	}MESHMATERIAL;
	typedef struct tagKeyFrame
	{
		double			Time;
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
	}KEYFRAME;
	typedef struct tagBoneDesc
	{
		/* 뼈대의 로컬 -> 정점의 로컬스페이스 변환을 위한 행렬이다. */
		XMFLOAT4X4		OffsetMatrix;
		class CHierarchyNode*	pNode;
	}BONEDESC;
	
	typedef struct tagVertex_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMFLOAT3		vBiNormal;
	}VTXMESH;

	typedef struct tagVertex_Anim : public VTXMESH
	{
		XMUINT4			vBlendIndex; /* 현재 정점에 상태변환에 대한 영향을 주는 뼈의 인덱스 (최대 네개) */
		XMFLOAT4		vBlendWeight; /* 영향을 주는 뼈의 가중치 (영향력 최대가 1 ) */
	}VTXMESH_ANIM;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType = TYPE_END;
		/* 방향성 광원의 방향. */
		XMFLOAT3		vDirection;
		XMFLOAT3		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

		XMMATRIX		mLightWorld;
		XMMATRIX		mLightView;
		XMMATRIX		mLightProj;

		_float			mOrthinfo[5];
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

	typedef struct tagVertex_Particle
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vPosition;

		XMFLOAT4		vTime;

	} VTXPARTICLE;

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

	/// Compute Shader Only
	typedef struct tagCSRenderTarget
	{
		ID3D11Texture2D* CSrenderTargetTexutre = nullptr;
		ID3D11RenderTargetView* CSRTV = nullptr;
		ID3D11ShaderResourceView* CSSRV = nullptr;
		ID3D11UnorderedAccessView* CSUAV = nullptr;
	}CSRT;

	typedef struct tagThresholdParams
	{
		alignas(16) float threshold;
	}THRESHOLD;

	typedef struct tagComputeShader
	{
		ID3D10Blob* csBlob;
		ID3D11ComputeShader* csShader = nullptr;
	}CS;

#define	GAUSSIAN_RADIUS 7
	typedef struct BlurParams
	{
		alignas(16) float coefficients[GAUSSIAN_RADIUS + 1];
		int radius;
		int direction;
	}BLURPARAM;

	/* for. Trigger */
	typedef struct tagTrigger
	{
		TRIGGERTYPE eTrigger_Type;
		_float3		fTrigger_Point;
		_int		iIndex;
	}TRIGGER;

	/* for. Map_Object*/
	typedef struct tagMabObject
	{
		_float4x4 WorldMat;
	}MABOBJECT;


	// 공격 정보
	typedef struct tagAttackDesc
	{
		class CGameObject* pOwner = nullptr;
		class CGameObject* pHitObject = nullptr;
		// 1 : 약공격, 2 : 강공격, 3 : 넉백될 정도의 강려크한 공격
		_uint iLevel = 0;
		_float fDamage = 0.f;
	}ATTACKDESC;

	/* UI 리소스 관리 */
	typedef struct ResoucePath
	{
		std::wstring clientResourcePath = L"../bin/Resources/Texture/InventoryResources/Texture";
		std::wstring engineResourcePath = L"../../Engine/Resource";
	}RESOUCEPATH;

	typedef struct tagParryDesc
	{
		_uint iLevel = 0;
		class CGameObject* pOwner = nullptr;
	}PARRYDESC;

	// 쉐이더 재질 값, 이미션 컬러
	typedef struct ShaderConstantBuffer
	{
		_float metalic = 0.f;
		_float roughness = 0.f;
		_float ao = 0.f;
		_float4 color = _float4(0, 0, 0, 0);
		_float empower = 0.f;
	}SCB;
}
