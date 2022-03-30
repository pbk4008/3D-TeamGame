#include "..\public\Base.h"

CBase::CBase()
	: m_dwRefCnt(0)
{
}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;	
}

unsigned long CBase::Release()
{
	/* �������� ���Ҷ� �����Ѵ�. */
	if ( 0 == m_dwRefCnt)
	{
		Free();
		delete this;

		return 0;
	}
	/* �����Ѵ�. */
	else
		return m_dwRefCnt--;

	return 0;
}

