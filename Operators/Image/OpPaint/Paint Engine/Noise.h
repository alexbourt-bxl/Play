#ifndef NOISE_H
#define NOISE_H

#include <Math\Math.h>

#define FLOAT(a) ((double)(a & 0xFFFF) / (double)0xFFFF)

class Noise
{
public:
	enum _Interpolation
	{
		Step   = 0,
		Linear = 1,
		Cosine = 2,
		Cubic  = 3
	};

	class _Seed
	{
	public:
		uint Initial;
		uint Current;
		
	public:
		_Seed();

		_Seed& operator = (uint seed);

		uint Rotate();
		void Reset();
	};

public:
	_Interpolation Interpolation;
	bool   Clip;

	_Seed  Seed;

	double v0;
	double v1;
	double v2;
	double v3;

	double _step;

public:
	Noise(uint seed = 0x8CA2); // 0x92D68CA2
	~Noise();

	double	Next(double scale = 1.0f);
	void	Reset();
};

#endif