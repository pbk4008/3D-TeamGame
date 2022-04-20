#pragma once

/* ���� �� �����Ӥ��� ���� �� �ִ� ��, ������ȯ������� �����ϳ�. */

#include "SingleTon.h"

class CPipeLine : public CSingleTon<CPipeLine>
{
	friend CSingleTon;
public:
	NO_COPY(CPipeLine);
	explicit CPipeLine();
	virtual ~CPipeLine() = default;
public:
	HRESULT Add_Camera(const wstring& pCameraTag);
	void Update_PipeLine();
	void Delete_Camera();
	_fmatrix Get_Transform(const wstring& pCameraTag, TRANSFORMSTATEMATRIX eType);
	_fvector Get_CamPosition(const wstring& pCameraTag);
	void Set_Transform(const wstring& pCameraTag, TRANSFORMSTATEMATRIX eType, _fmatrix TransformMatrix);
	_matrix Get_ViewMatrix() { return XMLoadFloat4x4(&m_matView); }
	
	HRESULT	SetPreViewProtj(const wstring& cametag);
	_fmatrix GetPreViewProtj(_fmatrix world);
	//�⺻ ī�޶� ����
	HRESULT Change_BaseCamera(const wstring& pCameraTag);
public:
	_uint getCameraCount() { return (_uint)m_mapPipeLine.size(); }
	const wstring& getBaseCamera();
private:
	CAMERA* Find_Camera(const wstring& pCameraTag);
	_int Find_Index(const wstring& pCameraTag);
private:
	//map<wstring, CAMERA*> m_mapPipeLine;
	vector<pair<wstring, CAMERA*>>	m_mapPipeLine;
	_float4x4						m_matPreView;
	_float4x4						m_matPreProj;

	_float4x4	m_matView;
public:
	virtual void Free() override;
};

