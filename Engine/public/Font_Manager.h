#pragma once

#include "Base.h"

BEGIN(Engine)

class CCustomFont;
class CFont_Manager final : public CBase
{
	DECLARE_SINGLETON(CFont_Manager)
public:
	explicit CFont_Manager();
	virtual ~CFont_Manager() = default;
public:
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontPath);
	HRESULT Render_Font(const _tchar* pFontTag, _fvector vColor, const _tchar* pString);
private:
	/*CCustomFont : 하나의 글꼴에 해당하는 폰트다.  */
	map<const _tchar*, CCustomFont*>			m_Fonts;
	typedef map<const _tchar*, CCustomFont*>	FONT;
public:
	virtual void Free();
};

END