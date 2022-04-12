#ifndef SingleImage_h__
#define SingleImage_h__

#include "Component.h"

BEGIN(Engine)
class UI_Texture;
class CVIBuffer_Rect;
class CVI_UIBuffer;
class CRenderer;
END

BEGIN(Client)
class CSingleImage :  public CComponent
{
public:
	struct Desc
	{
		_float2 fOffsetPos = { 0.f, 0.f };
		_float2 fOffsetScale = { 1.f, 1.f };
		_float4 fColor = { 1.f, 1.f, 1.f, 1.f }; /* RGBA */
		std::wstring textureName = L"default";
		std::wstring renderTargetName = L"";
		ID3D11ShaderResourceView* pTexture = nullptr;
		CRenderer* pRenderer = nullptr;
		CTransform* pTransform = nullptr;
		CGameObject* pCreator = nullptr;
	};

private:
	explicit CSingleImage(void) = default;
	explicit CSingleImage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CSingleImage(const CSingleImage& rhs);
	virtual ~CSingleImage(void) = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render(CTransform* _sender);

public:
	void SetTexture(std::wstring name);
	void SetColor(const _fvector& color)			{ XMStoreFloat4(&m_fColor, color); }
	const _fvector& GetColor(void)					{ return XMLoadFloat4(&m_fColor); }
	void SetOffsetPos(const _float2& offsetPos)		{ m_fOffsetPosition = offsetPos; }
	void SetOffsetScale(const _float2& offsetScale) { m_fOffsetScale = offsetScale; }

private:
	ID3D11ShaderResourceView* m_pImage = nullptr;
	_float4 m_fColor;
	_float2 m_fOffsetPosition = { 0.f, 0.f };
	_float2 m_fOffsetScale = { 1.f, 1.f };

private:
	CVIBuffer_Rect* m_pBuffer;
	CRenderer*		m_pRenderer = nullptr;
	CTransform*		m_pTransform = nullptr;
	CGameObject*	m_pCreator = nullptr;

public:
	static CSingleImage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END
#endif // SingleImage_h__
