#ifndef UI_Texture_h__
#define UI_Texture_h__

#include "IResource.h"

BEGIN(Engine)

class ENGINE_DLL UI_Texture : public IResource
{
	friend class CResourceManager;

private:
	explicit UI_Texture(void);
	~UI_Texture(void);

private:
	// IResource을(를) 통해 상속됨
	virtual void NativeConstruct(std::wstring _fg) override;
	virtual void Free(void) override;

public:
	void Set_Texture(ID3D11ShaderResourceView* _image);
	ID3D11ShaderResourceView* Get_Texture(size_t _index);

private:
	vector< ID3D11ShaderResourceView*> m_textures;
};

END

#endif // UI_Texture_h__

