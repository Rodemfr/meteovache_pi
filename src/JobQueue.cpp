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

#include "JobQueue.h"

#include "wx/wx.h"

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

DEFINE_EVENT_TYPE(wxEVT_THREAD_JOB_ONGOING)
DEFINE_EVENT_TYPE(wxEVT_THREAD_JOB_COMPLETED)
DEFINE_EVENT_TYPE(wxEVT_THREAD_JOB_FAILED)

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

JobRequest::JobRequest() : cmd(CMD_UNDEFINED), latitude(0.0f), longitude(0.0f)
{
}

JobRequest::JobRequest(JobCommand cmd)
{
    this->cmd       = cmd;
    this->latitude  = 0.0f;
    this->longitude = 0.0f;
}

JobRequest::JobRequest(JobCommand cmd, float latitude, float longitude)
{
    this->cmd       = cmd;
    this->latitude  = latitude;
    this->longitude = longitude;
}

JobRequest::~JobRequest()
{
}

JobQueue::JobQueue(wxEvtHandler *pParent) : parentEvtHandler(pParent)
{
}

void JobQueue::AddJobRequest(const JobRequest &job)
{
    mapMutex.Lock();
    jobMap.insert(std::make_pair(0, job));
    mapMutex.Unlock();
    queueCount.Post();
}

void JobQueue::GetNextJob(JobRequest *jobRequest)
{
    JobRequest element;

    queueCount.Wait();
    mapMutex.Lock();
    element = (jobMap.begin())->second;
    jobMap.erase(jobMap.begin());
    mapMutex.Unlock();

    *jobRequest = element;
}

bool JobQueue::GetNextJobTimeout(JobRequest *jobRequest, unsigned long timeOut)
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
    }
    else
    {
        return false;
    }
}

void JobQueue::ReportResult(const JobRequest::JobCommand &cmd, JobResult result)
{

    if (result == JOB_SUCCESSFUL)
    {
        wxCommandEvent evt(wxEVT_THREAD_JOB_COMPLETED);
        evt.SetInt(cmd);
        wxQueueEvent(parentEvtHandler, evt.Clone());
    }
    else if (result == JOB_FAILED)
    {
        wxCommandEvent evt(wxEVT_THREAD_JOB_FAILED);
        evt.SetInt(cmd);
        wxQueueEvent(parentEvtHandler, evt.Clone());
    }
    else if (result == JOB_ONGOING)
    {
        wxCommandEvent evt(wxEVT_THREAD_JOB_ONGOING);
        evt.SetInt(cmd);
        wxQueueEvent(parentEvtHandler, evt.Clone());
    }
    wxWakeUpIdle();
}
