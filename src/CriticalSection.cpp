/*
 * CriticalSection.cpp
 *
 *  Created on: 12.08.2011
 *      Author: outz
 */

#include "CriticalSection.h"

namespace webzavod {

CriticalSection::CriticalSection()
{
	pthread_mutex_init(&mutex, NULL);
}

CriticalSection::~CriticalSection()
{
	pthread_mutex_destroy(&mutex);
}

void CriticalSection::Enter()
{
	pthread_mutex_lock(&mutex);
}

void CriticalSection::Leave()
{
	pthread_mutex_unlock(&mutex);
}

}
