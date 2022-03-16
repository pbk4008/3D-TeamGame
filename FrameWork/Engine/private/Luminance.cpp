#include "Luminance.h"
#include "GameInstance.h"
#include "VIBuffer_RectViewPort.h"

CLuminance::CLuminance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CLuminance::InitLuminance()
{
	_uint		iViewportIndex = 1;
	ZeroMemory(&m_viewport, sizeof(m_viewport));

	m_pDeviceContext->RSGetViewports(&iViewportIndex, &m_viewport);

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, m_viewport.Width, m_viewport.Height, TEXT("../../Reference/ShaderFile/Shader_Luminance.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

HRESULT CLuminance::DownSampling(CTarget_Manager* pTargetMgr)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_LumGray"))))
		return E_FAIL;

	_float2 vTexUV = { 1.f / m_viewport.Width, 1.f / m_viewport.Height };

	_vector	offset[4];
	ZeroMemory(offset, sizeof(_vector) * 4);
	offset[0] = { -0.5f * vTexUV.x , 0.5f * vTexUV.y , 0, 0 };
	offset[1] = { 0.5f * vTexUV.x , 0.5f * vTexUV.y , 0, 0 };
	offset[2] = { -0.5f * vTexUV.x , -0.5f * vTexUV.y , 0, 0 };
	offset[3] = { 0.5f * vTexUV.x , -0.5f * vTexUV.y , 0, 0 };

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_GrayOffset", offset, sizeof(_vector) * 4)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_HDRDiffuse"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(243.f, 234.f);
	m_pVIBuffer->Render(0);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//---------------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum1"))))
		return E_FAIL;

	_vector DSOffset[9];
	_int idx1 = 0;
	for (_int i = -1; i < 2; i++)
	{
		for (_int j = -1; j < 2; j++)
			DSOffset[idx1++] = { (_float)i / (_float)243.f,(_float)j / (_float)243.f, 0 , 0 };
	}
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_DSOffset", DSOffset, sizeof(_vector) * 9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_LumGray"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(81.f, 81.f);
	m_pVIBuffer->Render(1);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//---------------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum2"))))
		return E_FAIL;

	_int idx2 = 0;
	for (_int i = -1; i < 2; i++)
	{
		for (_int j = -1; j < 2; j++)
			DSOffset[idx2++] = { (_float)i / (_float)81.f,(_float)j / (_float)81.f, 0 , 0 };
	}
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_DSOffset", DSOffset, sizeof(_vector) * 9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum1"))))
		return E_FAIL;


	m_pVIBuffer->Buffer_Resize(27.f, 27.f);
	m_pVIBuffer->Render(1);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//---------------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum3"))))
		return E_FAIL;

	_int idx3 = 0;
	for (_int i = -1; i < 2; i++)
	{
		for (_int j = -1; j < 2; j++)
			DSOffset[idx3++] = { (_float)i / (_float)9.f,(_float)j / (_float)9.f, 0 , 0 };
	}
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_DSOffset", DSOffset, sizeof(_vector) * 9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum2"))))
		return E_FAIL;


	m_pVIBuffer->Buffer_Resize(9.f, 9.f);
	m_pVIBuffer->Render(1);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//---------------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum4"))))
		return E_FAIL;

	_int idx4 = 0;
	for (_int i = -1; i < 2; i++)
	{
		for (_int j = -1; j < 2; j++)
			DSOffset[idx4++] = { (_float)i / (_float)3.f,(_float)j / (_float)3.f, 0 , 0 };
	}
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_DSOffset", DSOffset, sizeof(_vector) * 9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum3"))))
		return E_FAIL;


	m_pVIBuffer->Buffer_Resize(3.f, 3.f);
	m_pVIBuffer->Render(1);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//---------------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum5"))))
		return E_FAIL;

	_int idx5 = 0;
	for (_int i = -1; i < 2; i++)
	{
		for (_int j = -1; j < 2; j++)
			DSOffset[idx5++] = { (_float)i / (_float)1.f,(_float)j / (_float)1.f, 0 , 0 };
	}
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_DSOffset", DSOffset, sizeof(_vector) * 9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum4"))))
		return E_FAIL;


	m_pVIBuffer->Buffer_Resize(1.f, 1.f);
	m_pVIBuffer->Render(1);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(m_viewport.Width, m_viewport.Height);

	return S_OK;
}

#ifdef _DEBUG
#pragma region Luminance DebugTarget
HRESULT CLuminance::DownSamplingDebug(CTarget_Manager* pTargetMgr)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum1Db"))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum1"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(81.f, 81.f);
	m_pVIBuffer->Render(2);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//----------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum2Db"))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum2"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(27.f, 27.f);
	m_pVIBuffer->Render(2);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//----------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum3Db"))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum3"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(9.f, 9.f);
	m_pVIBuffer->Render(2);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//----------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum4Db"))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum4"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(3.f, 3.f);
	m_pVIBuffer->Render(2);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	//----------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Lum5Db"))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_Lum5"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(1.f, 1.f);
	m_pVIBuffer->Render(2);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(m_viewport.Width, m_viewport.Height);

	return S_OK;
}
#pragma endregion
#endif // _DEBUG


CLuminance* CLuminance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLuminance* pinstance = new CLuminance(pDevice, pDeviceContext);

	if (FAILED(pinstance->InitLuminance()))
	{
		MSGBOX("Failed To Creating Luminance");
		Safe_Release(pinstance);
	}

	return pinstance;
}

void CLuminance::Free()
{
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}