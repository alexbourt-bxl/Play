#ifndef USER_METRIC_H
#define USER_METRIC_H

#include <Devices\Mouse\Mouse.h>

const uint M_LButtonDown	= 0x01;
const uint M_LButtonUp		= 0x02;
const uint M_LClick			= 0x03;
const uint M_LDoubleClick	= 0x04;

class User;
class UserMetric
{
public:
	UserMetric(User* user);
	~UserMetric();

	void	RecordMetric(
				uint objId,
				uint time,
				uint metricType,
				uint metricDetails);

private:
	User*	m_user;

	uint	m_bufferSize;
	uint*	m_buffer;
	uint	m_bufferPos;

public:
	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
};

#endif