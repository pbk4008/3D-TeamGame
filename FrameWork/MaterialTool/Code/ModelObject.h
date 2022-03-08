#pragma once
#ifndef __MODELOBJECT_H__
#define __MODELOBJECT_H__

#include "GameObject.h"
BEGIN(Engine)
class CModel;
END
BEGIN(Tool)
class CModelObject final : public CGameObject
{
public:
	typedef  struct tagModelObecjtDesc
	{
		wstring wstrFbxName;
		wstring wstrFbxFolder;
	}MODELOBJDESC;
private:
	explicit CModelObject();
	explicit CModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CModelObject(const CModelObject& rhs);
	virtual ~CModelObject()= default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double dDeltaTime);
	virtual _int LateTick(_double dDeltaTime);
	virtual HRESULT Render();
public:
	_bool Check_Name(const wstring& pName);
	HRESULT Check_Texture(list<wstring>& pTextrue);
	void Focusing();
	void Picking_Face(_fvector vRayPos, _fvector vRayDir);
public:
	void setDraw(_bool bCheck) { m_bDraw = bCheck; }
public:
	static CModelObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	CModel* m_pModel;
	wstring m_tModelName;
	wstring m_tModelFolder;
	_bool m_bDraw;
};
END
#endif
