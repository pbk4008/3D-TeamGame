#include "..\public\SplineCurve.h"

CSplineCurve::CSplineCurve(void)
{
	m_points.reserve(16);
}

CSplineCurve::~CSplineCurve(void)
{
}

void CSplineCurve::AddPoint(_fvector _point)
{
	m_points.emplace_back(_point);

	if (m_points.size() > 3)
	{
		CalcDistanceRatio();
	}
}

void CSplineCurve::CalcDistanceRatio(void)
{
	_float totalDistance = 0;
	m_distanceRatio.clear();
	vector<_float> distanceList;
	for (int i = 1; i < m_points.size() - 2; ++i)
	{
		totalDistance += Engine::Distance(m_points[i], m_points[i + 1]);
		distanceList.emplace_back(totalDistance);
	}

	for (auto& distance : distanceList)
	{
		m_distanceRatio.emplace_back(distance / totalDistance);
	}
}

_fvector CSplineCurve::GetPoint(_int _index, _float _ratio)
{
	_vector point;
	
	point = XMVectorCatmullRom(
		m_points[_index - 2],
		m_points[_index - 1],
		m_points[_index],
		m_points[_index + 1],
		_ratio);

	return point;
}

_fvector CSplineCurve::GetPoint(_float _ratio)
{
	_ratio = Engine::Clamp(_ratio, 0.f, 1.f);

	_int index = 0;
	_vector point;

	for (_int i = 0; i < m_distanceRatio.size(); ++i)
	{
		if (_ratio <= m_distanceRatio[i])
		{
			_float total;
			_float patial;
			if (0 == i)
			{
				total = m_distanceRatio[i];
				patial = _ratio;
			}
			else
			{
				total = m_distanceRatio[i] - m_distanceRatio[i - 1];
				patial = _ratio - m_distanceRatio[i - 1];
			}

			point = GetPoint(i + 2, patial / total);
			break;
		}
	}
	return point;
}


void CSplineCurve::SubPoint(_int _index)
{
	if (m_points.size() <= _index)
		return;

	m_points.erase(m_points.begin() + _index);
}

void CSplineCurve::DrawCurve(void)
{
#ifdef _DEBUG
	if (m_points.size() < 3)
	{
		assert(L"Spline Curve must have points at least 3! " && 0);
		return;
	}
#endif // _DEBUG

	_int index = 0;
	m_line.clear();

	/* first curve -> first_p, p0, p1, p2 */
	for (_int i = 0; i < 10; ++i)
	{
		_vector out;
		out = XMVectorCatmullRom(
			m_points[0],
			m_points[0],
			m_points[1],
			m_points[2],
			i * 0.1f);
		m_line.emplace_back(out);
	}

	/* middle curves */
	for (; index < m_points.size() - 3; ++index)
	{
		for (_int i = 0; i < 10; ++i)
		{
			_vector out;
			out = XMVectorCatmullRom(
				m_points[index],
				m_points[index + 1],
				m_points[index + 2],
				m_points[index + 3],
				i * 0.1f);
			m_line.emplace_back(out);
		}
	}

	/* last curve  pn-2, pn-1, pn, lastp */
	for (_int i = 0; i < 11; ++i)
	{
		_vector out;
		out = XMVectorCatmullRom(
			m_points[index],
			m_points[index + 1],
			m_points[index + 2],
			m_points[index + 2],
			i * 0.1f);
		m_line.emplace_back(out);
	}
}


void CSplineCurve::DeletePoints(void)
{
	m_points.clear();
}


