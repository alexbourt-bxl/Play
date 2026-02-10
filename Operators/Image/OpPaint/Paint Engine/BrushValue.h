#ifndef BRUSH_VALUE_H
#define BRUSH_VALUE_H

class BrushValue
	//: public Object//, public ISerializable
{
public:
	// The modifiers are in % of the set value
	double	Heavy;
	double	Light;
	double	Slow;
	double	Fast;
	double	Vertical;
	double	Tilted;

public:
	BrushValue(/*UINT valueId*/);
	BrushValue(const BrushValue& val);
	BrushValue(double value);
	
	operator double();
	BrushValue& operator = (double value);

	virtual double GetAdjusted(double pressure, double speed, double tilt);

	//int	GetSizeInBuffer();
	//void	WriteToBuffer(char* buffer, int* pos);
	//bool	ReadFromBuffer(char* buffer, int* pos);

protected:
	double	m_value;
};

#endif