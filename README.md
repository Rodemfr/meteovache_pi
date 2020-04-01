# MeteoVache plug-in for OpenCPN

meteovache_pi is a small weather plug-in for OpenCPN. It provides weather forecasts anywhere the world as long as you have an Internet connection.
MeteoVache has been specifically designed to be use-able with very low bandwidth Internet connections. It runs fine and fast with only a few kilobits per second.

![Sample picture](files/meteovache_en_256.png)

## Description

meteovache_pi provides weather forecast from a number of meteorological forecast models :

- "AROME" : high-resolution model (0.05°) from [Météo France](http://www.meteofrance.com) (Coverage : France)
- "ARPEGE HD" : regional model (0.1°) from [Météo France](http://www.meteofrance.com) (Coverage : Europe)
- "ICON EU" : regional model (0.125°) from [Deutscher Wetterdienst](https://www.dwd.de/EN) (Coverage Europe)
- "ARPEGE" : global model (0.5°) from [Météo France](http://www.meteofrance.com) (Coverage : World)

meteovache_pi needs an Internet connection to retrieve weather data from MeteoVache server. It has been especially designed to minimize the amount of data required to get a forecast at a given location. A typical request weights between 1 and 1.5kb, making it suitable for very low bandwidth Internet connections like Edge GSM.

## Author

* **R. Demoment** - [Rodemfr](https://github.com/Rodemfr)

## License

This program is distributed under the terms of the GPL v2. - see the [gpl.txt](cmake/gpl.txt) file for details

## Download

Latest release is v0.2.1

Pre-compiled binaries for OpenCPN 5.0 are available here :

- Win32 x86 : [meteovache_pi_v0.2.1_ocpn50_win32_x86.zip](https://github.com/Rodemfr/meteovache_pi/raw/master/files/binaries/opencpn50/win32_x86/meteovache_pi_v0.2.1_ocpn50_win32_x86.zip)
- Linux x64 : [meteovache_pi_v0.2.1_ocpn50_linux_x86.tar.gz](https://github.com/Rodemfr/meteovache_pi/raw/master/files/binaries/opencpn50/linux_x64/meteovache_pi_v0.2.1_ocpn50_linux_x86.tar.gz)

Installers are not available at this time. You will have to uncompress archives manually and to copy the various files in OpenCPN install directory. Archives have been made so that the folder structure inside OpenCPN install directory is preserved.

## Release history

**v0.2.1**
* First binary release for Windows x86 and Linux x64

## Acknowledgments

* Thanks to @OpenCPN team for providing a comprehensive set of templates and examples to develop plugins

