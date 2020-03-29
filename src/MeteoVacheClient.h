/*
 * MeteoVacheClient.h
 *
 *  Created on: 10 nov. 2019
 *      Author: ronan
 */

#ifndef METEOVACHECLIENT_H_
#define METEOVACHECLIENT_H_

#include "wx/wx.h"
#include "wx/socket.h"
#include "SpotForecasts.h"

#define DEFAULT_METEOVACHE_SERVER_NAME "vendredi.homelinux.net"
#define DEFAULT_METEOVACHE_SERVER_PORT 31837
#define MVC_MAX_RESPONSE_LENGTH 8192

class MeteoVacheClient {
public:
	MeteoVacheClient();
	virtual ~MeteoVacheClient();

	bool downloadAllForecasts(float latitude, float longitude, SpotForecasts &spotHandler);

private:
	wxIPV4address localIpAddr, serverIpAddr;
	wxDatagramSocket *localSocket;
	char gzippedResponse[MVC_MAX_RESPONSE_LENGTH];
	char serverResponse[MVC_MAX_RESPONSE_LENGTH];

	unsigned int uncompressBuffer(void *inputBuffer, unsigned int inputLength, void *outputBuffer, unsigned int outputLength);
};

#endif /* METEOVACHECLIENT_H_ */
