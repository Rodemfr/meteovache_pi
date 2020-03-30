#ifndef DATETIME_H_
#define DATETIME_H_

#include <time.h>
#include <string>

class DateTime {

private:
	time_t timeValue;
	static std::string dayNames[7];
	static std::string monthNames[12];

public:
	DateTime();
	virtual ~DateTime();

	char* parseGmtIso8601(const char *timeString);
	void formatGmtIso8601(char *timeString, int maxLength);
	void setToCurrentTime();

	int getLocalYear();
	int getLocalMonth();
	std::string getLocalMonthName();
	int getLocalDay();
	std::string getLocalDayName();
	int getLocalHour();
	int getLocalMinute();

	int getGmtYear();
	int getGmtMonth();
	std::string getGmtMonthName();
	int getGmtDay();
	std::string getGmtDayName();
	int getGmtHour();
	int getGmtMinute();

	void addHours(int hours);
	int isLaterThan(DateTime *dateTime);

	unsigned int getTimeCode();
	void setTimeCode(unsigned int code);
};

#endif /* DATETIME_H_ */
