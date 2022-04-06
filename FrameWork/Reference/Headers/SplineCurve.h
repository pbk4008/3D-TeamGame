#ifndef SplineCurve_h__
#define SplineCurve_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CSplineCurve
{
public:
	CSplineCurve(void);
	~CSplineCurve(void);

public:
	void AddPoint(_fvector _point);
	void SubPoint(_int _index);
	void DeletePoints(void);
	void DrawCurve(void);
	_fvector GetPoint(_float _ratio);
	_fvector GetPoint(_int _index, _float _ratio);
	list<_vector>* GetCurve(void) { return &m_line; }

private:
	void CalcDistanceRatio(void);

private:
	vector<_vector> m_points;
	list<_vector>   m_line;
	vector<_float>  m_distanceRatio;
};

END
#endif // SplineCurve_h__
