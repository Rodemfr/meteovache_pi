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

#include <MeteoVacheThread.h>
#include <MVReportFrame.h>
#include <JobQueue.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

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
