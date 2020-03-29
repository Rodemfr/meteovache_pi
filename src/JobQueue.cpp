/*
 * JobQueue.cpp
 *
 *  Created on: 10 nov. 2019
 *      Author: ronan
 */

#include <JobQueue.h>

DEFINE_EVENT_TYPE(wxEVT_THREAD_JOB_COMPLETED)
DEFINE_EVENT_TYPE(wxEVT_THREAD_JOB_FAILED)

JobRequest::JobRequest() :
		cmd(CMD_UNDEFINED), latitude(0.0f), longitude(0.0f)
{
}

JobRequest::JobRequest(
		JobCommand cmd)
{
	this->cmd = cmd;
	this->latitude = 0.0f;
	this->longitude = 0.0f;
}

JobRequest::JobRequest(
		JobCommand cmd,
		float latitude,
		float longitude)
{
	this->cmd = cmd;
	this->latitude = latitude;
	this->longitude = longitude;
}

JobRequest::~JobRequest()
{
}

JobQueue::JobQueue(
		wxEvtHandler *pParent) :
		parentEvtHandler(pParent)
{
}

void JobQueue::addJob(
		const JobRequest &job)
{
	wxMutexLocker lock(mapMutex);
	jobMap.insert(std::make_pair(0, job));
	queueCount.Post();
}

void JobQueue::getNextJob(
		JobRequest *jobRequest)
{
	JobRequest element;

	queueCount.Wait();
	mapMutex.Lock();
	element = (jobMap.begin())->second;
	jobMap.erase(jobMap.begin());
	mapMutex.Unlock();

	*jobRequest = element;
}

bool JobQueue::getNextJobTimeout(
		JobRequest *jobRequest,
		unsigned long timeOut)
{
	JobRequest element;

	if (queueCount.WaitTimeout(timeOut) == wxSEMA_NO_ERROR)
	{
		mapMutex.Lock();
		element = (jobMap.begin())->second;
		jobMap.erase(jobMap.begin());
		mapMutex.Unlock();

		*jobRequest = element;
		return true;
	} else
	{
		return false;
	}
}

void JobQueue::reportResult(
		const JobRequest::JobCommand &cmd,
		JobResult result)
{
	if (result == JOB_SUCCESSFUL)
	{
		wxCommandEvent evt(wxEVT_THREAD_JOB_COMPLETED);
		evt.SetInt(cmd);
		parentEvtHandler->AddPendingEvent(evt);
	} else if (result == JOB_FAILED)
	{
		wxCommandEvent evt(wxEVT_THREAD_JOB_FAILED);
		evt.SetInt(cmd);
		parentEvtHandler->AddPendingEvent(evt);
	}
}
