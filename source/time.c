#include <nds.h>
#include <time.h>

int getHour()
{
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);

	return timeStruct->tm_hour;
}
