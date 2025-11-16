# MeteoVache Plugin for OpenCPN

*MeteoVache* (also known as *meteovache_pi*) is a weather plugin for *OpenCPN*. It provides weather forecasts for any location worldwide, requiring an internet connection.

![Sample picture](files/meteovache_en_256.png)

## Description

*MeteoVache* provides weather forecasts based on the following meteorological models:

- "Arome" : high resolution model (0.05°) from [Météo France](http://www.meteofrance.com) (Coverage : France)
- "Arome Antilles" : high resolution model (0.05°) from [Météo France](http://www.meteofrance.com) (Coverage : Antilles)
- "Arome Océan Indien" : high resolution model (0.05°) from [Météo France](http://www.meteofrance.com) (Coverage : Indian Ocean)
- "Arome Guyane" : high resolution model (0.05°) from [Météo France](http://www.meteofrance.com) (Coverage : Guyana)
- "Arome Nouvelle Calédonie" : high resolution model (0.05°) from [Météo France](http://www.meteofrance.com) (Coverage : New Calédonia)
- "Arome Polynésie" : high resolution model (0.05°) from [Météo France](http://www.meteofrance.com) (Coverage : French Polynésia)
- "Icon D2" : high resolution model (0.02°) from [Deutscher Wetterdienst](https://www.dwd.de/EN) (Coverage : Germany)
- "HRDPS" : high resolution model (0.05°) from [Meteorological Service of Canada](https://meteo.gc.ca/) (Coverage : Canada)
- "Arpege HD" : regional model (0.1°) from [Météo France](http://www.meteofrance.com) (Coverage : Europe)
- "Icon Eu" : regional model (0.125°) from [Deutscher Wetterdienst](https://www.dwd.de/EN) (Coverage : Europe)
- "Arpege" : global model (0.25°) from [Météo France](http://www.meteofrance.com) (Coverage : World)

*MeteoVache* requires an internet connection to retrieve weather data from the MeteoVache server. It is specifically designed to minimize data consumption; a typical forecast request is only 500 to 1500 bytes. This makes it ideal for low-bandwidth connections, such as satellite or cellular networks (e.g., Edge/GSM).

## Author

* **Ronan Demoment** - [Rodemfr](https://github.com/Rodemfr)

## License

This program is distributed under the terms of the GPL v2. - see the [gpl.txt](cmake/gpl.txt) file for details

## Download

Since v1.1, *MeteoVache* uses *OpenCPN*'s managed plugin system, allowing you to download it directly from the *OpenCPN* user interface. Navigate to the *Plugins* settings page and update the master catalog. *MeteoVache* will be available for installation if your platform is supported.

## Acknowledgments

* Thanks to @OpenCPN team for providing a comprehensive set of templates and examples to develop plug-ins and to maintain this wonderful Open Source application.

## Release history

**v1.6.4** (Latest)
*   Fixed a potential infinite wait in the socket read loop, ensuring a clean exit from OpenCPN.

**v1.6.0**
*   Fixed a crash that occurred when using the plugin immediately after an update from the catalog.

**v1.5.4**
*   Added an option to use non-graphical wind arrows in weather reports.
*   Removed the commit hash from the version string in the plugin manager.
*   Fixed a DNS failure on some systems.

**v1.3.7**
*   Fixed translation issues in the preferences panel.
*   Improved thread handling.

**v1.2**
*   Fixed wxWidgets warnings.
*   Resolved macOS build errors.
*   Updated CI templates from `shipdriver_pi`.

**v1.1**
*   Implemented support for *OpenCPN*'s managed plugin system.
*   Added Spanish & Italian translations.
*   Added support for wxWidgets 3.2 and HiDPI displays.
*   Added support for dark color themes.
*   Fixed a crash on exit.
*   Other minor bugfixes.

**v0.8**
*   Wind direction is now displayed with graphical arrows.
*   Recompiled against OpenCPN 5.2.4 to fix a "small font issue" in the context menu.

**v0.7**
*   Improved weather report readability with color-coding.
*   Updated CMake files for arm64 compatibility.
*   Minor bug fixes.

**v0.5.2**
*   Added an option to hide the plugin icon from the toolbar.
*   Minor translation fixes.

**v0.5**
*   Fixed networking thread issues during long server responses.
*   Added a status indicator to the report window (success, ongoing, failed).
*   Fixed incorrect background color with dark themes on Debian.
*   Added time zone display to the weather report.
*   Added an option to select between local time zone and UTC.

**v0.4**
*   Added a button to manually save forecasts to a text file.
*   Added preferences for automatic forecast saving (with optional ZIP compression).
*   Added experimental ARMHF build for Raspberry Pi.

**v0.3**
*   Added a preferences panel to change display units.
*   Fixed a negative temperature display bug.
*   The plugin now compiles as a standalone project.
*   A Windows installer is now generated.
*   Server-side: Added Arpège model for worldwide coverage.

**v0.2.1**
*   First binary release for Windows (x86) and Linux (x64).
