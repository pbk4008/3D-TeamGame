#include "DataLoader.h"
#include "GameInstance.h"

CDataLoader::CDataLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pDeviceContext);
}

HRESULT CDataLoader::LoadMapData(const wstring& filepath)
{
	//HANDLE hfile = CreateFile(filepath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	//if (INVALID_HANDLE_VALUE == hfile)
	//	return E_FAIL;

	g_pGameInstance->LoadFile<MESHDESC>(m_vecMesh, filepath);

	for (_int i = 0; i < m_vecMesh.size(); ++i)
	{
		wstring FileName = m_vecMesh[i].FileName;
		wstring FileFolder = m_vecMesh[i].FolderName;
		wstring Name = m_vecMesh[i].Name;
		wstring Tag = m_vecMesh[i].Tag;
		_uint   iType = m_vecMesh[i].iType;

		FILEINFO FileInfo;
		FileInfo.cstrFolder = FileFolder;
		FileInfo.cstrFileName = FileName;
		FileInfo.cstrFBX_Type = iType;

		auto TagFinder = find(m_vecProto.begin(), m_vecProto.end(), FileInfo.cstrFileName);

		if (TagFinder == m_vecProto.end())
		{
			if (FAILED(CreateProtoModel(FileInfo)))
			{
				MSGBOX("파일을 불러오는 도중 원본 생성에 실패했습니다.");
				return E_FAIL;
			}
			m_vecProto.push_back(FileInfo.cstrFileName);
		}
	}
	
	for (auto& iter : m_vecMesh)
		CreateCloneModel(iter);

	return S_OK;
}

HRESULT CDataLoader::CreateProtoModel(const FILEINFO& fileinfo)
{
	wstring strPath = L"../bin/Resources/FBX/";
	wstring strPullPath = strPath + fileinfo.cstrFolder + L'/';

	string PullPath;
	PullPath.assign(strPullPath.begin(), strPullPath.end());
	string FileName;
	FileName.assign(fileinfo.cstrFileName.begin(), fileinfo.cstrFileName.end());
	string FolderName;
	FolderName.assign(fileinfo.cstrFolder.begin(), fileinfo.cstrFolder.end());

	wstring Static_ShaderFilePath = L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl";
	wstring Anim_ShaderFilePath = L"../../Reference/ShaderFile/Shader_MeshAnim.hlsl";

	_matrix  PivotMatrix;

	if (0 == fileinfo.cstrFBX_Type)
	{
		PivotMatrix = /*XMMatrixIdentity();*/XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));

		if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, fileinfo.cstrFileName,
			CModel::Create(m_pDevice, m_pDeviceContext, PullPath.c_str(), FileName.c_str(),
				Static_ShaderFilePath, PivotMatrix, (CModel::TYPE)fileinfo.cstrFBX_Type))))
			return E_FAIL;
	}
	else
	{
		PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, fileinfo.cstrFileName,
			CModel::Create(m_pDevice, m_pDeviceContext, PullPath.c_str(), FileName.c_str(),
				Anim_ShaderFilePath, PivotMatrix, (CModel::TYPE)fileinfo.cstrFBX_Type))))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CDataLoader::CreateCloneModel(const MESHDESC& modeldesc)
{
	MODELDESC MeshDesc;

	MeshDesc.strFolder = modeldesc.FolderName;
	MeshDesc.strFileName = modeldesc.FileName;
	MeshDesc.strTag = modeldesc.Tag;
	MeshDesc.strName = modeldesc.Name;
	MeshDesc.fInitPos = modeldesc.fInitPos;
	MeshDesc.WorldMat = modeldesc.WorldMat;
	MeshDesc.iType = modeldesc.iType;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, MeshDesc.strTag, L"Prototype_GameObject_StaticMesh", &MeshDesc)))
		return E_FAIL;

	return S_OK;
}

CDataLoader* CDataLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CDataLoader* pInstance = new CDataLoader(pDevice, pDeviceContext);

	if (pInstance == nullptr)
	{
		MSGBOX("Failed to Creating CDataLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDataLoader::Free()
{

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
