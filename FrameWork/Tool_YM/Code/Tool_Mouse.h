#ifndef Tool_Mouse_h__
#define Tool_Mouse_h__

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CRenderer;
class CTexture;

END

class CTool_Mouse final : public CGameObject
{
protected:
	explicit CTool_Mouse(void);
	virtual ~CTool_Mouse(void) = default;

public:
	virtual HRESULT NativeConstruct_Prototype() ;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta) override;

public:
	static CTool_Mouse* Create(void);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free(void) override;
};

#endif // Tool_Mouse_h__