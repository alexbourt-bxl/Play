#include "Noise.h"
#include <windows.h>
#include <math.h>

Noise::_Seed::_Seed()
{
	Reset();
}

Noise::_Seed& Noise::_Seed::operator = (uint seed)
{
	Initial = seed;
	Current = seed;

	return *this;
}

uint Noise::_Seed::Rotate()
{
	uint seed = Current;

	Current ^= (Current << 13);
	Current  = (Current >> 17);
	Current ^= (Current << 5);

	return seed;
}

void Noise::_Seed::Reset()
{
	Current = Initial;
}

Noise::Noise(uint seed)
{
	Interpolation = Noise::Cubic;
	Clip = false;
	
	_step = 0;

	Seed = seed;
	Reset();
}

Noise::~Noise()
{

}

double Noise::Next(double scale)
{
	// Changing the scale linearly only has a visible "regular" effect when 
	// the values are relatively small, so I set the scale as a power of PHI.
	scale = pow(PHI, scale) / PHI;
	scale = max(1, scale);

	double next = v1;

	if (_step >= 1)
	{
	 	// Get the next random value
	
		Seed.Rotate();

		v0 = v1;
	//------------------------
		v1 = v2;
		v2 = v3;
	//------------------------
		v3 = FLOAT(Seed.Current);

		_step -= 1;
	}

	// Get the next 'noise' value

	switch (Interpolation)
	{
		case Noise::Step:
		{
			next = v1;
			break;
		}
		case Noise::Linear:
		{
			next = v1 + _step * (v2 - v1);
			break;
		}
		case Noise::Cosine:
		{
			double ft = _step * PI;
			double f = (1 - cos(ft)) * 0.5;
			
			next = v1 + f * (v2 - v1);
			
			break;
		}
		case Cubic:
		{
			double p = (v3 - v2) - (v0 - v1);
			double q = (v0 - v1) - p;
			double r =  v2 - v0;
			double s =  v1;

			double step2 = _step * _step;
			double step3 = step2 * _step;

			double val = p * step3 + q * step2 + r * _step + s;

			next = Clip 
				? min(max(0, val), 1)
				: val;

			break;
		}
	}

	_step += 1 / scale;

	return next;
}

void Noise::Reset()
{
	Seed.Reset();

	v0 = FLOAT(Seed.Rotate());
	v1 = FLOAT(Seed.Rotate());
	v2 = FLOAT(Seed.Rotate());
	v3 = FLOAT(Seed.Rotate());

	_step = 0;
}