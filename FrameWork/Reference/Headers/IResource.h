#ifndef IResource_h__
#define IResource_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL IResource
{
	friend class CResourceManager;

public:
	explicit IResource(void);
	virtual ~IResource(void) = default;

protected:
	virtual void NativeConstruct(std::wstring _fg) PURE;
	
public:
	const std::wstring& GetName(void) { return m_name; }
	void SetStatic(_bool Static) { m_bStatic = Static; }

protected:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	std::wstring m_name;
	std::wstring m_filePath;
	_bool m_bStatic;

protected:
	virtual void Free(void) PURE;
};

END
#endif // IResource_h__
