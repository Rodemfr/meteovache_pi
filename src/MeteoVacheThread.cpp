/*
 * MeteoVacheThread.cpp
 *
 *  Created on: 14 oct. 2019
 *      Author: ronan
 */

#include "MeteoVacheThread.h"
#include "MVReportFrame.h"
#include "JobQueue.h"

MeteoVacheThread::MeteoVacheThread(
		MVReportFrame *handler,
		JobQueue *jobQueue) :
		wxThread(wxTHREAD_DETACHED)
{
	pHandler = handler;
	this->jobQueue = jobQueue;
	meteoVacheClient = new MeteoVacheClient();
}

MeteoVacheThread::~MeteoVacheThread()
{
	delete meteoVacheClient;
}

wxThread::ExitCode MeteoVacheThread::Entry()
{
	JobRequest job;

	while (TestDestroy() == false)
	{
		if (pHandler->jobQueue->getNextJobTimeout(&job, 500) == true)
		{
			if (job.cmd == JobRequest::CMD_GET_ALL_FORECASTS_AT_LOCATION)
			{
				meteoVacheClient->downloadAllForecasts(job.latitude, job.longitude, pHandler->spotForecast);
				jobQueue->reportResult(job.cmd, JobQueue::JOB_SUCCESSFUL);
			}
		}
	}

	return (wxThread::ExitCode) 0;
}
