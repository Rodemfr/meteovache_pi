#ifndef DATETIME_H_
#define DATETIME_H_

#include <time.h>

class DateTime
{

private:
	time_t timeValue;
	static char *dayNames[7];
	static char *monthNames[12];

public:
	DateTime();
	virtual ~DateTime();

	char* parseGmtIso8601(const char *timeString);
	void formatGmtIso8601(char *timeString, int maxLength);
	void setToCurrentTime();

	int getLocalYear();
	int getLocalMonth();
	const char* getLocalMonthName();
	int getLocalDay();
	const char* getLocalDayName();
	int getLocalHour();
	int getLocalMinute();

	int getGmtYear();
	int getGmtMonth();
	const char* getGmtMonthName();
	int getGmtDay();
	const char* getGmtDayName();
	int getGmtHour();
	int getGmtMinute();

	void addHours(int hours);
	int isLaterThan(DateTime *dateTime);

	unsigned int getTimeCode();
	void setTimeCode(unsigned int code);
};

#endif /* DATETIME_H_ */
