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

#include "MeteoVacheClient.h"
#include "Forecast.h"
#include "SpotForecasts.h"

#include <wx/mstream.h>
#include <wx/zstream.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

#define MV_CMD_REQUEST_ALL_FORECAST_AT_LOCATION 1

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

MeteoVacheClient::MeteoVacheClient() : serverIpOk(false)
{
    // Setup port address of distant server
    serverIpAddr.Service(DEFAULT_METEOVACHE_SERVER_PORT);

    // Create the local UDP socket
    localIpAddr.AnyAddress();
    localIpAddr.Service(0x8000);
    localSocket = new wxDatagramSocket(localIpAddr, wxSOCKET_BLOCK);
    // Set 1 second timeout
    localSocket->SetTimeout(1);
}

MeteoVacheClient::~MeteoVacheClient()
{
    localSocket->Destroy();
}

bool MeteoVacheClient::DownloadAllForecasts(float latitude, float longitude, SpotForecasts &spotForecast)
{
    char     requestBuffer[9];
    int      nbForecasts;
    Forecast forecast;
    wxUint32 responseLength;

    GetServerAddress();

    // Prepare REQUEST_ALL_FORECATS_AT_LOCATION :
    // 1 byte = command
    // 1 float = latitude (little endian)
    // 1 float = longitude (little endian)
    requestBuffer[0] = MV_CMD_REQUEST_ALL_FORECAST_AT_LOCATION;
    // TODO : Handle endianess
    *((float *)(requestBuffer + 1)) = latitude;
    *((float *)(requestBuffer + 5)) = longitude;

    localSocket->Discard();
    localSocket->SendTo(serverIpAddr, requestBuffer, sizeof(requestBuffer));
    localSocket->Read(gzippedResponse, sizeof(gzippedResponse));
    responseLength = localSocket->LastReadCount();
    if (localSocket->Error() || (responseLength == 0))
    {
        return (false);
    }
    UncompressBuffer(gzippedResponse, responseLength, serverResponse, sizeof(serverResponse));

    nbForecasts = serverResponse[0];
    spotForecast.Lock();
    spotForecast.Reset();
    spotForecast.SetPosition(latitude, longitude);

    int dataOffset = 1;
    for (int i = 0; i < nbForecasts; i++)
    {
        dataOffset += forecast.ReadBinary(serverResponse + dataOffset);
        spotForecast.Add(forecast);
    }

    spotForecast.Unlock();

    return (true);
}

void MeteoVacheClient::GetServerAddress()
{
    if (!serverIpOk)
    {
        // Setup IP address of distant server
        serverIpOk = serverIpAddr.Hostname(DEFAULT_METEOVACHE_SERVER_NAME);
        if (!serverIpOk)
        {
            // If DNS is not responding, use hard coded rollback address
            serverIpAddr.Hostname(ROLLBACK_METEOVACHE_SERVER_NAME);
        }
    }
}

unsigned int MeteoVacheClient::UncompressBuffer(void *inputBuffer, unsigned int inputLength, void *outputBuffer, unsigned int outputLength)
{
    wxMemoryInputStream inputStream(inputBuffer, inputLength);
    wxZlibInputStream   zlibStream(inputStream);

    zlibStream.Read(outputBuffer, outputLength);

    return (zlibStream.LastRead());
}
