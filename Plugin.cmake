# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
    "meteovache_pi/meteovache_pi-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "meteovache_pi/meteovache_pi-beta"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "meteovache_pi/meteovache_pi"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

#
#
# -------  Plugin setup --------
#
set(PKG_NAME meteovache_pi)
set(PKG_VERSION  1.1.4)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME MeteoVache)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME MeteoVache) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "MeteoVache weather plug-in for OpenCPN")
set(PKG_DESCRIPTION [=[MeteoVache is a local weather plug-in for OpenCPN. It provides weather forecasts everywhere in the world even with a very low bandwidth Internet connection.]=])

set(PKG_AUTHOR "Ronan Demoment")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/Rodemfr/meteovache_pi)
set(PKG_INFO_URL https://github.com/Rodemfr/meteovache_pi)

set(SRC
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

include_directories(${CMAKE_SOURCE_DIR}/src)

if (WIN32)
  add_compile_definitions(__WIN32__)
endif (WIN32)

if (APPLE)
  add_compile_definitions(__MACOS__)
endif (APPLE)

set(PKG_API_LIB api-16)  #  A dir in opencpn-libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
endmacro ()
