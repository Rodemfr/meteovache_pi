/*
 * MeteoVacheThread.h
 *
 *  Created on: 14 oct. 2019
 *      Author: ronan
 */

#ifndef METEOVACHETHREAD_H_
#define METEOVACHETHREAD_H_

#include "JobQueue.h"
#include "MeteoVacheClient.h"
#include <wx/thread.h>
#include <wx/event.h>

class MVReportFrame;

class MeteoVacheThread: public wxThread {
public:
	MeteoVacheThread(MVReportFrame *handler, JobQueue *jobQueue);
	virtual ~MeteoVacheThread();

private:
	virtual ExitCode Entry();
	MVReportFrame *pHandler;
	JobQueue *jobQueue;
	MeteoVacheClient *meteoVacheClient;
};

#endif /* METEOVACHETHREAD_H_ */
