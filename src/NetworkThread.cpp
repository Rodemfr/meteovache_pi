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

#include "NetworkThread.h"
#include "JobQueue.h"
#include "ReportWindow.h"

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

NetworkThread::NetworkThread(SpotForecasts *spotForecast, JobQueue *jobQueue)
    : wxThread(wxTHREAD_DETACHED), exitThread(false), condition(mutex), finished(false)
{
    this->spotForecast = spotForecast;
    this->jobQueue     = jobQueue;
    meteoVacheClient   = new MeteoVacheClient();
}

NetworkThread::~NetworkThread()
{
    delete meteoVacheClient;
}

void NetworkThread::RequestEnd()
{
    exitThread = true;

    mutex.Lock();
    if (!finished)
        condition.Wait();

    mutex.Unlock();
}

wxThread::ExitCode NetworkThread::Entry()
{
    JobRequest job;
    int        retries;

    // We check at each loop if the thread has been requested to be deleted
    while ((TestDestroy() == false) && (exitThread == false))
    {
        // GetNextJobTimeout is a blocking function but we limit the blocking time to 500ms to avoid
        // blocking OpenCPN for too long when exiting the application (OpenCPN will indirectly wait
        // for the end of this thread when stopping the plug-in
        if (jobQueue->GetNextJobTimeout(&job, 500) == true)
        {
            if (job.cmd == JobRequest::CMD_GET_ALL_FORECASTS_AT_LOCATION)
            {
                // Network retries are handled at this level, once again to allow checking for thread
                // deletion. This avoids blocking OpenCPN exit but also allows to send a status event
                // to the ReportWindow which will display the corresponding message to the user
                retries = 0;
                while ((retries < MAX_NETWORK_RETRIES) &&
                       (meteoVacheClient->DownloadAllForecasts(job.latitude, job.longitude, *spotForecast) == false))
                {
                    // If we are here, then the server did not respond in one second to the first request
                    retries++;
                    // Send an "on going" event to the ReportWindow
                    jobQueue->ReportResult(job.cmd, JobQueue::JOB_ONGOING);
                    // Check if thread deletion was requested meanwhile
                    if (TestDestroy())
                    {
                        return (wxThread::ExitCode)0;
                    }
                }
                // Check if request succeeded or if we reach retry limit
                if (retries < MAX_NETWORK_RETRIES)
                {
                    jobQueue->ReportResult(job.cmd, JobQueue::JOB_SUCCESSFUL);
                }
                else
                {
                    jobQueue->ReportResult(job.cmd, JobQueue::JOB_FAILED);
                }
            }
        }
    }

    mutex.Lock();
    finished = true;
    condition.Signal();
    mutex.Unlock();

    return (wxThread::ExitCode)0;
}
