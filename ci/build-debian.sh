set -xe

sudo apt update
sudo sudo apt-get install build-essential cmake gettext git-core gpsd gpsd-clients libgps-dev wx-common libwxgtk3.0-gtk3-dev libglu1-mesa-dev libgtk2.0-dev libgtk-3-dev wx3.0-headers libbz2-dev libtinyxml-dev libsndfile1-dev libportaudio2 portaudio19-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev libelf-dev libsqlite3-dev libusb-1.0-0-dev libudev-dev libjpeg-dev wx3.0-headers libwxgtk-webview3.0-gtk3-dev rpm

rm -rf build && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
make -sj2 package
