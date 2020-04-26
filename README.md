# MeteoVache plug-in for OpenCPN

meteovache_pi is a small weather plug-in for OpenCPN. It provides weather forecasts anywhere the in the World as long as you have an Internet connection.

![Sample picture](files/meteovache_en_256.png)

## Description

meteovache_pi provides weather forecasts based on the following meteorological models :

- "Arome" : high resolution model (0.05°) from [Météo France](http://www.meteofrance.com) (Coverage : France)
- "Arpege HD" : regional model (0.1°) from [Météo France](http://www.meteofrance.com) (Coverage : Europe)
- "Icon Eu" : regional model (0.125°) from [Deutscher Wetterdienst](https://www.dwd.de/EN) (Coverage Europe)
- "Arpege" : global model (0.5°) from [Météo France](http://www.meteofrance.com) (Coverage : World)

meteovache_pi needs an Internet connection to retrieve weather data from MeteoVache server. It has been specifically designed to minimize the amount of data required to get a forecast at a given location. A typical request weights between 1 and 1.5kb, making it suitable for very low bandwidth Internet connections like Edge GSM.

## Author

* **Ronan Demoment** - [Rodemfr](https://github.com/Rodemfr)

## License

This program is distributed under the terms of the GPL v2. - see the [gpl.txt](cmake/gpl.txt) file for details

## Download

Latest release is v0.5.2

Pre-compiled binaries and source code are available here :

https://github.com/Rodemfr/meteovache_pi/releases/tag/v0.5.2

Windows and Linux x64 are supported. Raspberry Pi version will come later.

## Acknowledgments

* Thanks to @OpenCPN team for providing a comprehensive set of templates and examples to develop plug-ins and to maintain this wonderful Open Source application.

## Release history

**v0.5.2**
* Added an option to remove plug-in icon from tool-bar
* Minor translation fixes

**v0.5**
* Fixed issues in networking thread in case of long server response
* Added a status test in report window to inform on the status of the last forecast request (success, on-going, failed)
* Fixed bad backgorund color in case of dark theme under Debian
* Added display of time zone in weather report
* Added possibility to select select locale/system time zone or UTC in preference dialog

**v0.4**
* Added a button to manually save weather forecast in a text file (normal or column format)
* Added options in the preference panel to enable automatic save of requested forecast (normal, column, with or without ZIP compression)
* Added experimental ARMHF build for Raspberry PI (untested, may not work at all)

**v0.3**
* Added a preference panel with the possibility to change display units
* Fixed negative temperature bug
* Plug-in now compiles standalone
* Windows installer is now generated
* On server side : added Arpege model with world wide coverage

**v0.2.1**
* First binary release for Windows x86 and Linux x64

