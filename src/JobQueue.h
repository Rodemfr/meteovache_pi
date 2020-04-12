/***************************************************************************
 *                                                                         *
 * Project:  meteovache_pi                                                 *
 * Purpose:  Weather forecast plugin for OpenCPN                           *
 * Author:   Ronan Demoment                                                *
 *                                                                         *
 ***************************************************************************
 *   Copyright (C) 2020 by Ronan Demoment                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#ifndef _JOBQUEUE_H_
#define _JOBQUEUE_H_

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <map>
#include <wx/event.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

DECLARE_EVENT_TYPE(wxEVT_THREAD_JOB_ONGOING, -1)
DECLARE_EVENT_TYPE(wxEVT_THREAD_JOB_COMPLETED, -1)
DECLARE_EVENT_TYPE(wxEVT_THREAD_JOB_FAILED, -1)

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class JobRequest
{
public:
	enum JobCommand
	{
		CMD_GET_ALL_FORECASTS_AT_LOCATION = 0,
		CMD_UNDEFINED
	};

	JobCommand cmd;
	float latitude;
	float longitude;

	JobRequest();
	JobRequest(JobCommand cmd);
	JobRequest(JobCommand cmd, float latitude, float longitude);
	virtual ~JobRequest();
};

class JobQueue
{

public:
	enum JobResult
	{
		JOB_SUCCESSFUL = 0,
		JOB_FAILED,
		JOB_ONGOING
	};

	JobQueue(wxEvtHandler *pParent);

	void AddJobRequest(const JobRequest &job);
	void GetNextJob(JobRequest *jobRequest);
	bool GetNextJobTimeout(JobRequest *jobRequest, unsigned long timeOut);
	void ReportResult(const JobRequest::JobCommand &cmd, JobResult result);

private:
	wxEvtHandler *parentEvtHandler;
	std::map<int, JobRequest> jobMap;
	wxMutex mapMutex;
	wxSemaphore queueCount;
};

#endif /* _JOBQUEUE_H_ */
