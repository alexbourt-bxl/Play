#include "OpGradient.h"

void OpGradient::Update_Linear(int* image, int width, int height)
{
	double dx = m_p1.X - m_p0.X;
	double dy = m_p1.Y - m_p0.Y;

	double f0 = dx * m_p0.X + dy * m_p0.Y;
	double f1 = dx * m_p1.X + dy * m_p1.Y;
	double _1_f1f0 = 1 / (f1 - f0);

	assert(f1 > f0);

	double f, fn;
	double fy;
	int* line;
	int* pixel;

	int cFirst = (Color8)m_colors[0];
	int cLast = (Color8)m_colors[m_colors.Count - 1];
	int cPrev = 0, cNext = 0;

	for (int y = 0; y < height; y++)
	{
		fy = (double)y;
		line = image + y * width;

		for (int x = 0; x < width; x++)
		{
			f = dx * (double)x + dy * fy;
			fn = (f - f0) * _1_f1f0;

			pixel = line + x;

			if (f <= f0)
				*pixel = cFirst;
			else if (f >= f1)
				*pixel = cLast;
			else
			{
				if (fn <= m_positions[0])
					*pixel = cFirst;
				else if (fn >= m_positions[m_positions.Count - 1])
					*pixel = cLast;
				else
				{
					for (int i = 0; i < m_positions.Count - 1; i++)
					{
						if (fn >= m_positions[i] && fn <= m_positions[i + 1]) 
						{
							cPrev = (Color8)m_colors[i];	
							cNext = (Color8)m_colors[i + 1];
							fn = (fn - m_positions[i]) / (m_positions[i + 1] - m_positions[i]);
							break;
						}
					}

					*pixel = Color8::Lerp(cPrev, cNext, Math::ToInt8(fn));
				}
			}
		}
	}
}

void OpGradient::Update_Radial(int* image, int width, int height)
{
	double d = Math::GetDistance(m_p0, m_p1);
	double _1_d = 1 / d;

	double f, fn;
	double dy;
	int* line;
	int* pixel;

	int cFirst = (Color8)m_colors[0];
	int cLast = (Color8)m_colors[m_colors.Count - 1];
	int cPrev = 0, cNext = 0;

	for (int y = 0; y < height; y++)
	{
		dy = (double)y;
		line = image + y * width;

		for (int x = 0; x < width; x++)
		{
			f = Math::GetDistance(m_p0, FPoint((double)x, dy));

			pixel = line + x;

			if (f < d)
			{
				fn = f * _1_d;

				if (fn <= m_positions[0])
					*pixel = cFirst;
				else if (fn >= m_positions[m_positions.Count - 1])
					*pixel = cLast;
				else
				{
					for (int i = 0; i < m_positions.Count - 1; i++)
					{
						if (fn >= m_positions[i] && fn <= m_positions[i + 1]) 
						{
							cNext = (Color8)m_colors[i + 1];
							cPrev = (Color8)m_colors[i];	
							fn = (fn - m_positions[i]) / (m_positions[i + 1] - m_positions[i]);
							break;
						}
					}

					*pixel = Color8::Lerp(cPrev, cNext, Math::ToInt8(fn));
				}
			}
			else
			{
				*pixel = cLast;
			}
		}
	}
}

void OpGradient::Update_Angle(int* image, int width, int height)
{
	double ra = Math::GetAngle(m_p0, m_p1); // reference angle

	double r, fn;
	double dy;
	int* line;
	int* pixel;

	double _1_PI2 = 1 / (PI * 2);

	int cFirst = (Color8)m_colors[0];
	int cLast = (Color8)m_colors[m_colors.Count - 1];
	int cPrev = 0, cNext = 0;

	for (int y = 0; y < height; y++)
	{
		dy = (double)y;
		line = image + y * width;

		for (int x = 0; x < width; x++)
		{
			r = Math::GetAngle(m_p0, FPoint((double)x, dy));
			fn = (r - ra) * _1_PI2;

			pixel = line + x;

			if (fn < 0)	fn += 1; // compensate <0 angles

			if (fn <= m_positions[0])
				*pixel = cFirst;
			else if (fn >= m_positions[m_positions.Count - 1])
				*pixel = cLast;
			else
			{
				for (int i = 0; i < m_positions.Count - 1; i++)
				{
					if (fn >= m_positions[i] && fn <= m_positions[i + 1]) 
					{
						cNext = (Color8)m_colors[i + 1];
						cPrev = (Color8)m_colors[i];	
						fn = (fn - m_positions[i]) / (m_positions[i + 1] - m_positions[i]);
						break;
					}
				}

				*pixel = Color8::Lerp(cPrev, cNext, Math::ToInt8(fn));
			}
		}
	}
}