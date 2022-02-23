#pragma once

#include "SingleTon.h"

BEGIN(Engine)

class CCustomFont;
class CFont_Manager final : public CSingleTon<CFont_Manager>
{
	friend CSingleTon;
public:
	NO_COPY(CFont_Manager);
	explicit CFont_Manager();
	virtual ~CFont_Manager() = default;
public:
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pFontTag, const wstring& pFontPath);
	HRESULT Render_Font(const wstring& pFontTag, _fvector vColor, const wstring& pString);
private:
	/*CCustomFont : 하나의 글꼴에 해당하는 폰트다.  */
	map<wstring, CCustomFont*>			m_Fonts;
	typedef map<wstring, CCustomFont*>	FONT;
public:
	virtual void Free();
};

END