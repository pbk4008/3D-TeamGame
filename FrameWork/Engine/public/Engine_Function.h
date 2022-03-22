#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	// 템플릿은 기능의 정해져있으나 자료형은 정해져있지 않은 것
	// 기능을 인스턴스화 하기 위하여 만들어두는 틀

	template<typename T>
	void	Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete [] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_AddRef(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)		
			dwRefCnt = pInstance->AddRef();	

		return dwRefCnt;
	}

	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}

		return dwRefCnt;
	}
	template<typename T>
	void Safe_PxRelease(T& pInstance)
	{
		if (pInstance)
			pInstance->release();
	}

	// Functor
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const wstring& pTag) : m_pTargetTag(pTag){}
		~CTag_Finder(void) {}

	public:
		template<typename T> 
		bool		operator()(const T& pair)
		{
			if (m_pTargetTag==pair.first)
				return true;
			
			return false;
		}

	private:
		wstring		m_pTargetTag = nullptr;
	};

	static const _float3 QuaternionToEuler(const _float4& _q)
	{
		//_float3 euler;

		//float unit = (_q.x * _q.x) + (_q.y * _q.y) + (_q.z * _q.z) + (_q.w * _q.w);

		//float test = _q.x * _q.w - _q.y * _q.z;

		//if (test > 0.49999f * unit)
		//{
		//	euler.x = (_float)M_PI / 2;
		//	euler.y = 2.0f * atan2f(_q.y, _q.x);
		//	euler.z = 0;
		//}
		//else if (test < -0.49999f * unit)
		//{
		//	euler.x = -(_float)M_PI / 2;
		//	euler.y = -2.0f * atan2f(_q.y, _q.x);
		//	euler.z = 0;
		//}
		//else
		//{
		//	euler.x = asinf(2.0f * (_q.w * _q.x - _q.y * _q.z));
		//	euler.y = atan2f(2.0f * _q.w * _q.y + 2.0f * _q.z * _q.x, 1 - 2.0f * (_q.x * _q.x + _q.y * _q.y));
		//	euler.z = atan2f(2.0f * _q.w * _q.z + 2.0f * _q.x * _q.y, 1 - 2.0f * (_q.z * _q.z + _q.x * _q.x));
		//}

		//euler.x = XMConvertToDegrees(euler.x);
		//euler.y = XMConvertToDegrees(euler.y);
		//euler.z = XMConvertToDegrees(euler.z);

		//euler.x = fmodf(euler.x, 360.0f);
		//euler.y = fmodf(euler.y, 360.0f);
		//euler.z = fmodf(euler.z, 360.0f);
		double roll, pitch, yaw;

		// roll (x-axis rotation)
		double sinR_cosP = 2 * (_q.w * _q.x + _q.y * _q.z);
		double cosR_cosP = 1 - 2 * (_q.x * _q.x + _q.y * _q.y);
		roll = atan2(sinR_cosP, cosR_cosP);

		// pitch (y-axis rotation)
		double sinP = 2 * (_q.w * _q.y - _q.z * _q.x);
		if (std::abs(sinP) >= 1)
			pitch = copysign(M_PI * 0.5f, sinP);	// use 90 degrees if out of range
		else
			pitch = asin(sinP);

		// yaw (z-axis rotation)
		double sinY_cosP = 2 * (_q.w * _q.z + _q.x * _q.y);
		double cosY_cosP = 1 - 2 * (_q.y * _q.y + _q.z * _q.z);
		yaw = atan2(sinY_cosP, cosY_cosP);

		return _float3(XMConvertToDegrees((_float)roll), XMConvertToDegrees((_float)pitch), XMConvertToDegrees((_float)yaw));
	}
}

#endif // Engine_Function_h__
