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

#ifndef _METEOVACHETHREAD_H_
#define _METEOVACHETHREAD_H_

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <JobQueue.h>
#include <MeteoVacheClient.h>
#include <wx/thread.h>
#include <wx/event.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class ReportWindow;

class NetworkThread: public wxThread {
public:
	NetworkThread(ReportWindow *handler, JobQueue *jobQueue);
	virtual ~NetworkThread();

private:
	virtual ExitCode Entry();
	ReportWindow *pHandler;
	JobQueue *jobQueue;
	MeteoVacheClient *meteoVacheClient;
};

#endif /* _METEOVACHETHREAD_H_ */
