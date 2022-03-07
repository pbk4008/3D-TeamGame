#include "pch.h"
#include "ToolTerrain.h"
#include "ToolCell.h"

CToolTerrain::CToolTerrain()
	: m_pBuffer(nullptr)
	, m_pSrcTexture(nullptr)
	, m_iTextureCount(0)
	, m_bSameHeight(false)
	, m_iHitIndex(-1)
	, m_iDrawCheck(2)
	, m_bUsingNaviTool(false)
{
	ZeroMemory(&m_tMouseDesc, sizeof(MOUSEDESC));
}

CToolTerrain::CToolTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pBuffer(nullptr)
	, m_pSrcTexture(nullptr)
	, m_iTextureCount(0)
	, m_bSameHeight(false)
	, m_iHitIndex(-1)
	, m_iDrawCheck(2)
	, m_bUsingNaviTool(false)
{
	ZeroMemory(&m_tMouseDesc, sizeof(MOUSEDESC));
}

HRESULT CToolTerrain::Init_ToolTerrain()
{
	m_bActive = true;
	m_pComponentIndex = new CGameObject::COMINDEX[(_uint)COMPONENTID::COMPONENT_END];

	if (FAILED(CGameObject::Init_Clone(nullptr)))
		return E_FAIL;

	m_pComponentIndex[(_uint)COMPONENTID::VIBUFFER_TERRAIN].iStartIndex = 0;
	CVIBuffer_Terrain::TERRAINDESC pTerrinDesc = { true, L"../bin/Resource/Texture/Terrain/Filter/Height.bmp" };
	if (FAILED(Add_Component<CVIBuffer_Terrain>(0, COMPONENTID::VIBUFFER_TERRAIN, m_pComponentIndex[(_uint)COMPONENTID::VIBUFFER_TERRAIN].iStartIndex, &m_pBuffer, &pTerrinDesc)))
		return E_FAIL;

	_tchar strArg[MAX_PATH] = L"Terrain000.jpg";
	m_pComponentIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex = 0;
	if (FAILED(Add_Component<CTexture>(0, COMPONENTID::TEXTURE, m_pComponentIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex, &m_pSrcTexture, strArg)))
		return E_FAIL;

	lstrcpy(strArg, L"TerrainFilter");
	CTexture* pTexture = nullptr;
	m_pComponentIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex = 0;
	if (FAILED(Add_Component<CTexture>(0, COMPONENTID::TEXTURE, m_pComponentIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex, &pTexture,strArg)))
		return E_FAIL;

	m_vecTexture.emplace_back(pTexture);
	lstrcpy(strArg, L"Terrain001.jpg");
	m_pComponentIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex = 0;
	if (FAILED(Add_Component<CTexture>(0, COMPONENTID::TEXTURE, m_pComponentIndex[(_uint)COMPONENTID::TEXTURE].iStartIndex, &pTexture, strArg)))
		return E_FAIL;

	m_vecTexture.emplace_back(pTexture);

	m_pComponentIndex[(_uint)COMPONENTID::NAVIMESH].iStartIndex = 0;
	if (FAILED(Add_Component<CNaviMesh>(0, COMPONENTID::NAVIMESH, m_pComponentIndex[(_uint)COMPONENTID::NAVIMESH].iStartIndex, &m_pNaviMesh)))
		return E_FAIL;

	return S_OK;
}

_int CToolTerrain::Update_Terrain(_float fDeltaTime)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY,this);

	return 0;
}

HRESULT CToolTerrain::Render_GameObject()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	_matrix matWorld = XMMatrixTranspose(m_pTransform->getWorldMatrix());
	_matrix matView = XMMatrixTranspose(pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_VIEW));
	_matrix matProj = XMMatrixTranspose(pInstance->GetTransform(L"MainCamera", TRANSMATRIXSTATE::D3DT_PROJ));
	_vector vCamerPos = pInstance->Get_CamPos(L"MainCamera");

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &matView, sizeof(_matrix));
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &matProj, sizeof(_matrix));

	_float fRadius = 0;
	m_pBuffer->SetUp_TextureOnShader("g_DiffuseSourTexture", m_pSrcTexture, 0);
	if (m_iDrawCheck == 0)
	{
		fRadius = m_tMouseDesc.fRadius;
		m_pBuffer->SetUp_ValueOnShader("g_vBrushPos", &m_tMouseDesc.vPos, sizeof(_float3));
		m_pBuffer->SetUp_ValueOnShader("g_bSameHeight", &m_bSameHeight, sizeof(_bool));
	}
	m_pBuffer->SetUp_ValueOnShader("g_fRadius", &fRadius, sizeof(_float));
	m_pBuffer->SetUp_TextureOnShader("g_FilterTexture", m_vecTexture[0], 0);
	m_pBuffer->SetUp_TextureOnShader("g_DiffuseDestTexture", m_vecTexture[1], 0);

	m_pBuffer->Render(0);

	RELEASE_INSTANCE(CGameInstance);

	if(m_bUsingNaviTool)
		m_pNaviMesh->Render(m_pTransform->getWorldMatrix());
	
	return S_OK;
}

CToolTerrain* CToolTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CToolTerrain* pInstance = new CToolTerrain(pDevice, pDeviceContext);
	if (FAILED(pInstance->Init_ToolTerrain()))
	{
		MSG_BOX(L"CToolTerrain Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CToolTerrain::setMouseDesc(MOUSEDESC pDesc, _int iHitIndex)
{
	memcpy(&m_tMouseDesc, &pDesc, sizeof(MOUSEDESC));

	m_iHitIndex = iHitIndex;
}

void CToolTerrain::Check_Draw(_uint iCheck)
{
	m_iDrawCheck = iCheck;
	switch (m_iDrawCheck)
	{
	case 0://Filter
		if (!m_bSameHeight)
		{
			if (FAILED(Draw_HeightMap()))
				return;
		}
		else
		{
			if (FAILED(Draw_SameHeight()))
				return;
		}
		break;
	case 1://No_Draw
		break;
	}
}

HRESULT CToolTerrain::Change_TerrainTexture(const _tchar* pTextureTag)
{
	if (!m_pSrcTexture)
		return E_FAIL;

	if (FAILED(m_pSrcTexture->Change_Texture(pTextureTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolTerrain::Save_HeightMap(const _tchar* pFilePath)
{
	VTXTERRAIN* pVertices = (VTXTERRAIN*)(m_pBuffer->getVertices());
	vector<_ulong> vecVerticesY;
	_uint iVtxX = m_pBuffer->getVerteicesX();
	_uint iVtxZ = m_pBuffer->getVerteicesZ();

	vecVerticesY.emplace_back((_ulong)iVtxX);
	vecVerticesY.emplace_back((_ulong)iVtxZ);

	for (_uint i = 0; i < iVtxZ; i++)
	{
		for (_uint j = 0; j < iVtxX; j++)
		{
			_uint iIndex = i * iVtxX + j;
			vecVerticesY.emplace_back((_ulong)pVertices[iIndex].vPosition.y);
		}
	}
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pInstance->SaveFile<_ulong>(&vecVerticesY, pFilePath)))
	{
		MSG_BOX(L"HeightMap SaveFail");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolTerrain::Load_HeighMap(const _tchar* pFilePath)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	vector<_ulong> vecVerticesY;
	if (FAILED(pInstance->LoadFile<_ulong>(vecVerticesY, pFilePath)))
	{
		MSG_BOX(L"HeightMap Load Fail");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	_uint iVtxX = vecVerticesY[0];
	_uint iVtxZ = vecVerticesY[1];

	VTXTERRAIN* pTerrain = (VTXTERRAIN*)m_pBuffer->getVertices();

	for (_uint i = 0; i < iVtxX; i++)
	{
		for (_uint j = 0; j < iVtxZ; j++)
		{
			_uint iIndex = i * iVtxX + j;
			pTerrain[iIndex].vPosition.y = vecVerticesY[2+iIndex];
		}
	}
	m_pBuffer->Update_Buffer();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolTerrain::Add_Cell(CToolCell* pCell)
{
	_vector vCellsPoints[3];
	for (_uint i = 0; i < 3; i++)
		vCellsPoints[i]=pCell->getPos(i);

	m_pNaviMesh->Add_Cell(vCellsPoints);

	return S_OK;
}

HRESULT CToolTerrain::Delete_Cell(_uint iIndex)
{
	return 	m_pNaviMesh->Delete_Cell(iIndex);
}

HRESULT CToolTerrain::Change_CellPos(_uint iIndex, _fvector vPos)
{
	_uint iCellNum = iIndex / 3;
	_uint iPointNum = iIndex % 3;

	_float fY = XMVectorGetY(vPos);
	fY += 0.3f;
	_vector vTmpPos = XMVectorSetY(vPos, fY);
	m_pNaviMesh->MovePoint(iCellNum, iPointNum, vTmpPos);
	return S_OK;
}

HRESULT CToolTerrain::Draw_HeightMap()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->GetMouseState(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		VTXTERRAIN* pTerrain = (VTXTERRAIN*)(m_pBuffer->getVertices());
		_uint iVtxX = m_pBuffer->getVerteicesX();
		_uint iVtxZ = m_pBuffer->getVerteicesZ();

		for (_uint i = 0; i < iVtxZ; i++)
		{
			for (_uint j = 0; j < iVtxX; j++)
			{
				_uint iIndex = i * iVtxX + j;
				_vector vTerrainPos = XMLoadFloat3(&pTerrain[iIndex].vPosition);
				_float vTerrainPosX = XMVectorGetX(vTerrainPos);
				_float vTerrainPosZ = XMVectorGetZ(vTerrainPos);
				if (pow((vTerrainPosX - m_tMouseDesc.vPos.x), 2) + pow((vTerrainPosZ - m_tMouseDesc.vPos.z), 2) < pow(m_tMouseDesc.fRadius, 2))
				{
					_vector vMouse = XMLoadFloat3(&m_tMouseDesc.vPos);
					_float fLen = XMVectorGetX(XMVector3Length(vMouse - vTerrainPos));
					_float fRatio = 1-(fLen / m_tMouseDesc.fRadius);
					if (fRatio < 0)
						fRatio = 0.f;

					pTerrain[iIndex].vPosition.y += 0.1*fRatio;
					if (FAILED(m_pBuffer->Update_Buffer()))
					{
						RELEASE_INSTANCE(CGameInstance);
						return E_FAIL;
					}
				}
			}
		}

	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolTerrain::Draw_SameHeight()
{
	if (m_iHitIndex < 0)
		return E_FAIL;
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->GetMouseState(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		VTXTERRAIN* pTerrain = (VTXTERRAIN*)(m_pBuffer->getVertices());
		_uint iVtxX = m_pBuffer->getVerteicesX();
		_uint iVtxZ = m_pBuffer->getVerteicesZ();

		for (_uint i = 0; i < iVtxZ; i++)
		{
			for (_uint j = 0; j < iVtxX; j++)
			{
				_uint iIndex = i * iVtxX + j;
				_vector vTerrainPos = XMLoadFloat3(&pTerrain[iIndex].vPosition);
				_float vTerrainPosX = XMVectorGetX(vTerrainPos);
				_float vTerrainPosZ = XMVectorGetZ(vTerrainPos);
				if (vTerrainPosX > m_tMouseDesc.vPos.x - m_tMouseDesc.fRadius &&
					vTerrainPosX < m_tMouseDesc.vPos.x + m_tMouseDesc.fRadius &&
					vTerrainPosZ > m_tMouseDesc.vPos.z - m_tMouseDesc.fRadius &&
					vTerrainPosZ < m_tMouseDesc.vPos.z + m_tMouseDesc.fRadius)
				{
					pTerrain[iIndex].vPosition.y = pTerrain[m_iHitIndex].vPosition.y;
					if (FAILED(m_pBuffer->Update_Buffer()))
						return E_FAIL;
				}
			}
		}

	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CToolTerrain::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pSrcTexture);
	
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();
	m_vecTexture.shrink_to_fit();

	Safe_Release(m_pNaviMesh);
}
