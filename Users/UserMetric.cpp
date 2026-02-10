#include "UserMetric.h"

#include <stdlib.h>

UserMetric::UserMetric(User* user)
{
	m_user = user;

	m_bufferSize = 1024 * 4 * sizeof(uint);
	m_buffer = (uint*)malloc(m_bufferSize);
	m_bufferPos = 0;
}

UserMetric::~UserMetric()
{
	if (m_buffer)
		free(m_buffer);
}

void UserMetric::RecordMetric(uint objId, 
							  uint time, 
							  uint metricType, 
							  uint metricDetails)
{
	if (m_bufferPos >= m_bufferSize)
	{
		m_bufferSize *= 2;
		m_buffer = (uint*)realloc(m_buffer, m_bufferSize);
	}

	m_buffer[m_bufferPos++] = objId;
	m_buffer[m_bufferPos++] = time;
	m_buffer[m_bufferPos++] = metricType;
	m_buffer[m_bufferPos++] = metricDetails;
}