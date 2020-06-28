CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(meteovache_pi)

SET(PACKAGE_NAME meteovache_pi)
SET(VERBOSE_NAME MeteoVache)
SET(TITLE_NAME METEOVACHE)
SET(CPACK_PACKAGE_CONTACT "Ronan Demoment")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "5")
SET(VERSION_PATCH "2")
SET(VERSION_DATE "25/04/2020")

IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0011 NEW)
	CMAKE_POLICY(SET CMP0003 NEW)
	CMAKE_POLICY(SET CMP0043 NEW)
	CMAKE_POLICY(SET CMP0072 NEW)
ENDIF(COMMAND cmake_policy)

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_VERBOSE_MAKEFILE ON)
#SET(PROFILING 1)

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_MV
    src/DateTime.h
    src/DateTime.cpp
    src/Forecast.h
    src/Forecast.cpp
    src/JobQueue.h
    src/JobQueue.cpp
    src/MeteoVacheClient.h
    src/MeteoVacheClient.cpp
    src/MeteoVachePlugin.h
    src/MeteoVachePlugin.cpp
    src/NetworkThread.h
    src/NetworkThread.cpp
    src/Icons.h
    src/Icons.cpp
    src/ReportWindow.h
    src/ReportWindow.cpp
    src/PreferenceDialog.h
    src/PreferenceDialog.cpp
    src/SpotForecasts.h
    src/SpotForecasts.cpp
    src/ConfigContainer.h
    src/ConfigContainer.cpp
    src/ForecastDisplay.h
    src/ForecastDisplay.cpp
    src/ColorGradient.h
    src/ColorGradient.cpp
	)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_MV})
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${wxWidgets_LIBRARIES} )

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
