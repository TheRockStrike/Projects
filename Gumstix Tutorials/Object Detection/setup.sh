#!/bin/bash
set -e

PROG=`basename $0`
WORKING_DIR=$PWD
START_TIME="$(date -u +%s)"

RED='\033[0;31m'
GREEN='\033[0;32m'

if [ -f /sys/module/tegra_fuse/parameters/tegra_chip_id ]; then
    case $(cat /sys/module/tegra_fuse/parameters/tegra_chip_id) in
        64)
            JETSON_BOARD="TK1" ;;
        33)
            JETSON_BOARD="TX1" ;;
        24)
            JETSON_BOARD="TX2" ;;
        *)
            JETSON_BOARD="UNKNOWN" ;;
    esac
fi

if [ "$JETSON_BOARD" = "TX2" ]; then

	git clone https://github.com/jetsonhacks/buildOpenCVTX2.git
	REPO_DIR="$PWD/buildOpenCVTX2/"

elif [ "$JETSON_BOARD" = "TX1" ]; then

	git clone https://github.com/jetsonhacks/buildOpenCVTX1.git
	REPO_DIR="$PWD/buildOpenCVTX1/"

else
	printf "${RED}Not running on a Jetson TX1 or TX2, aborting"
	exit 1
fi

sudo apt-get update
sudo apt-get -y upgrade

echo "${GREEN}Moving into" $REPO_DIR
cd "$REPO_DIR"
mkdir "opencv_build"
sudo ./buildOpenCV.sh -s opencv_build	
cd "$WORKING_DIR"

# Download newest version of protoc

wget -q -O tmp.html https://github.com/google/protobuf/releases
PROTOC_URL="https://github.com"$(egrep -o -m 1 /google/.*aarch_64.zip tmp.html)
rm tmp.html
wget -O protoc.zip "$PROTOC_URL" 

unzip protoc.zip -d protoc
rm protoc.zip

sudo mv protoc/bin/* /usr/local/bin/
sudo mv protoc3/include/* /usr/local/include/

# Install Python dependencies

sudo apt-get install python3-pip
sudo pip3 install Cython
sudo pip3 install pillow
sudo pip3 install lxml
sudo pip3 install jupyter
sudo apt-get install libfreetype6-dev pkg-config libpng12-dev 
sudo pip3 install matplotlib

# Download TF Models

git clone https://github.com/tensorflow/models.git
cd models/research
protoc object_detection/protos/*.proto --python_out=.

cd "$WORKING_DIR"
sudo rm -rf protoc # Remove the installation folders
sudo rm -rf buildOpenCVTX*

sudo apt-get update
sudo apt-get -y upgrade
sudo apt autoremove

sudo chown -R $USER: $HOME

END_TIME="$(date -u +%s)"
ELAPSED="$(($end_time-$start_time))"

printf "\n\n${GREEN}Dependencies installed successfully in $ELAPSED seconds\n"

