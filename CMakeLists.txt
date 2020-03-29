##---------------------------------------------------------------------------
## Author:      nohal aka. Pavel Kalian
## Copyright:   
## License:     wxWidgets License
##---------------------------------------------------------------------------
 
# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
IF (COMMAND cmake_policy)
  if (POLICY CMP0043)
    CMAKE_POLICY(SET CMP0043 NEW)
  endif (POLICY CMP0043)
ENDIF (COMMAND cmake_policy)

PROJECT(meteovache_pi)

SET(PACKAGE_NAME meteovache_pi)
SET(VERBOSE_NAME MeteoVache)
SET(TITLE_NAME METEOVACHE)
SET(CPACK_PACKAGE_CONTACT "Ronan Demoment")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "2")
SET(VERSION_PATCH "1")
SET(VERSION_DATE "29/03/2020")

IF( NOT CMAKE_BUILD_TYPE )
  SET( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
       FORCE )
ENDIF( NOT CMAKE_BUILD_TYPE )

MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_MACOSX_RPATH "ON")

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
    src/MeteoVacheThread.h
    src/MeteoVacheThread.cpp
    src/icons.h
    src/icons.cpp
    src/MVReportFrame.h
    src/MVReportFrame.cpp
    src/PointForecast.h
    src/PointForecast.cpp
    src/RegionForecast.h
    src/RegionForecast.cpp
    src/SpotForecasts.h
    src/SpotForecasts.cpp
	)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_MV})

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang|AppleClang")
    target_compile_options(${PACKAGE_NAME} PRIVATE
        -Wno-unknown-pragmas
        -Wno-overloaded-virtual
    )
endif ()

FIND_PACKAGE(ZLIB REQUIRED)
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${wxWidgets_LIBRARIES} ${ZLIB_LIBRARY})

IF (QT_ANDROID )
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${OCPN_Core_LIBRARIES} )
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} "-lgnustl_shared")
ENDIF(QT_ANDROID)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
