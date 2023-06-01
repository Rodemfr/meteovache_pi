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

#ifndef _METEOVACHECLIENT_H_
#define _METEOVACHECLIENT_H_

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <wx/wx.h>
#include <wx/socket.h>
#include "SpotForecasts.h"

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

#define DEFAULT_METEOVACHE_SERVER_NAME "meteovache.dyndns.org"
#define DEFAULT_METEOVACHE_SERVER_PORT 31837
// Maximum size of server's response to a forecast request
#define MVC_MAX_RESPONSE_LENGTH 8192

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class MeteoVacheClient {
public:
	MeteoVacheClient();
	virtual ~MeteoVacheClient();

	// Download all available forecasts at a given location
	bool DownloadAllForecasts(float latitudeDeg, float longitudeDeg, SpotForecasts &spotHandler);

private:
	wxIPV4address localIpAddr, serverIpAddr;       // Addresses of the local and distant sockets
	wxDatagramSocket *localSocket;                 // Local socket object for communication with MeteoVache server
	char gzippedResponse[MVC_MAX_RESPONSE_LENGTH]; // Buffer storing compressed response from server
	char serverResponse[MVC_MAX_RESPONSE_LENGTH];  // Buffer storing uncompressed response from server

	// Uncompress a gzipped response from the server
	unsigned int UncompressBuffer(void *inputBuffer, unsigned int inputLength, void *outputBuffer, unsigned int outputLength);
};

#endif /* _METEOVACHECLIENT_H_ */
