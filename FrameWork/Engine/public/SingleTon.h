#pragma once
#ifndef __SINGLETON_H__
#define __SINGLETON_H__
#include "Base.h"
BEGIN(Engine)
#ifdef ENGINE_EXPORTS
template<typename T>
class ENGINE_DLL CSingleTon abstract : public CBase
#else
template<typename T>
class CSingleTon abstract : public CBase
#endif
{
protected:
	NO_COPY(CSingleTon);
	explicit  CSingleTon() {};
	virtual  ~CSingleTon() = default;
public:
	static T* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new T;
		return m_pInstance;
	}
	static _ulong DestroyInstance()
	{
		return Safe_Release(m_pInstance);
	}
private:
	static T* m_pInstance;
};
template<typename T>
T* CSingleTon<T>::m_pInstance = nullptr;
END
#endif
