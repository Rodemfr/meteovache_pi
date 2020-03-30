#ifndef DATETIME_H_
#define DATETIME_H_

#include <stdint.h>
#include <time.h>
#include <string>

class DateTime {

private:
	time_t timeValue;

public:
	DateTime();
	virtual ~DateTime();

	bool ParseGmtIso8601(std::string timeString);
	std::string FormatGmtIso8601();
	void SetToCurrentTime();

	uint32_t GetLocalYear();
	uint32_t GetLocalMonth();
	std::string GetLocalMonthName();
	uint32_t GetLocalDay();
	std::string GetLocalDayName();
	uint32_t GetLocalHour();
	uint32_t GetLocalMinute();

	uint32_t GetGmtYear();
	uint32_t GetGmtMonth();
	std::string GetGmtMonthName();
	uint32_t GetGmtDay();
	std::string GetGmtDayName();
	uint32_t GetGmtHour();
	uint32_t GetGmtMinute();

	void AddHours(int hours);
	bool IsLaterThan(DateTime &dateTime);

	uint32_t GetTimeCode();
	void SetTimeCode(uint32_t code);

private:
	std::string GetMonthName(uint32_t month);
	std::string GetDayName(uint32_t day);
};

#endif /* DATETIME_H_ */
