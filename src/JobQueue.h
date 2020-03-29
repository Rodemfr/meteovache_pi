/*
 * JobQueue.h
 *
 *  Created on: 10 nov. 2019
 *      Author: ronan
 */

#ifndef JOBQUEUE_H_
#define JOBQUEUE_H_

#include <map>
#include <wx/event.h>

DECLARE_EVENT_TYPE(wxEVT_THREAD_JOB_COMPLETED, -1)
DECLARE_EVENT_TYPE(wxEVT_THREAD_JOB_FAILED, -1)

class JobRequest
{
public:
	enum JobCommand
	{
		CMD_GET_ALL_FORECASTS_AT_LOCATION = 0, CMD_UNDEFINED
	};

	JobCommand cmd;
	float latitude;
	float longitude;

	JobRequest();
	JobRequest(
			JobCommand cmd);
	JobRequest(
			JobCommand cmd,
			float latitude,
			float longitude);
	virtual ~JobRequest();
};

class JobQueue
{

public:
	enum JobResult
	{
		JOB_SUCCESSFUL = 0, JOB_FAILED
	};

	JobQueue(
			wxEvtHandler *pParent);

	void addJob(
			const JobRequest &job);
	void getNextJob(
			JobRequest *jobRequest);
	bool getNextJobTimeout(
			JobRequest *jobRequest,
			unsigned long timeOut);
	void reportResult(
			const JobRequest::JobCommand &cmd,
			JobResult result);

private:
	wxEvtHandler *parentEvtHandler;
	std::map<int, JobRequest> jobMap;
	wxMutex mapMutex;
	wxSemaphore queueCount;
};

#endif /* JOBQUEUE_H_ */
