#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CCollision
{
public:
	explicit CCollision();
	virtual ~CCollision() = default;

public:
	CGameObject* pGameObject = nullptr;
};

END