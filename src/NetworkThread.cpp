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

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <JobQueue.h>
#include <NetWorkThread.h>
#include <ReportWindow.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

#define MAX_NETWORK_RETRIES 10

/***************************************************************************/
/*                             Local types                                 */
/***************************************************************************/

/***************************************************************************/
/*                               Globals                                   */
/***************************************************************************/

/***************************************************************************/
/*                           Local prototypes                              */
/***************************************************************************/

/***************************************************************************/
/*                              Functions                                  */
/***************************************************************************/

NetworkThread::NetworkThread(ReportWindow *handler, JobQueue *jobQueue) :
		wxThread(wxTHREAD_DETACHED)
{
	pHandler = handler;
	this->jobQueue = jobQueue;
	meteoVacheClient = new MeteoVacheClient();
}

NetworkThread::~NetworkThread()
{
	delete meteoVacheClient;
}

wxThread::ExitCode NetworkThread::Entry()
{
	JobRequest job;
	int retries;

	while (TestDestroy() == false)
	{
		if (pHandler->jobQueue->GetNextJobTimeout(&job, 500) == true)
		{
			if (job.cmd == JobRequest::CMD_GET_ALL_FORECASTS_AT_LOCATION)
			{
				retries = 0;
				while ((retries < MAX_NETWORK_RETRIES) && (meteoVacheClient->DownloadAllForecasts(job.latitude, job.longitude, pHandler->spotForecast) == false))
				{
					retries++;
					jobQueue->ReportResult(job.cmd, JobQueue::JOB_ONGOING);
					if (TestDestroy()) {
						return (wxThread::ExitCode) 0;
					}
				}
				if (retries < MAX_NETWORK_RETRIES)
				{
					jobQueue->ReportResult(job.cmd, JobQueue::JOB_SUCCESSFUL);
				} else
				{
					jobQueue->ReportResult(job.cmd, JobQueue::JOB_FAILED);
				}
			}
		}
	}

	return (wxThread::ExitCode) 0;
}
