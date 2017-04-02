#!/bin/bash

# Determine OS platform
UNAME=$(uname | tr "[:upper:]" "[:lower:]")
# If Linux, try to determine specific distribution
if [ "$UNAME" == "linux" ]; then
    # If available, use LSB to identify distribution
    if [ -f /etc/lsb-release -o -d /etc/lsb-release.d ]; then
        export DISTRO=$(lsb_release -i | cut -d: -f2 | sed s/'^\t'//)
        DISTRO_VER=$(lsb_release -r | cut -d: -f2 | sed s/'^\t'//)
    # Otherwise, use release info file
    else
        export DISTRO=$(ls -d /etc/[A-Za-z]*[_-][rv]e[lr]* | grep -v "lsb" | cut -d'/' -f3 | cut -d'-' -f1 | cut -d'_' -f1)
        DISTRO_VER=$(cat /etc/*-release | grep "VERSION_ID" | cut -d'=' -f2 | tr -d '"')
    fi
fi
# For everything else (or if above failed), just use generic identifier
[ "$DISTRO" == "" ] && export DISTRO=$UNAME
unset UNAME
DISTRO=${DISTRO^^}
TMPDIR=/tmp

function install_repo_package() {
    if [ "$#" -ne 1 ]; then
        echo "The download path is required."
        exit 1 
    else
        dpath=$(basename "$1")
    fi
    pushd $TMPDIR
    wget -nc --no-check-certificate "$1"
    sudo yum install -y "$dpath"
    popd
    return 0
}

function install_package() {
    if [ "$#" -lt 1 ]; then
        echo "The download path is required."
        exit 1
    fi

    pushd $TMPDIR
    wget -nc --no-check-certificate "$1"
    tpath=$(basename "$1")
    dpath=$(tar --exclude='*/*' -tf "$tpath")
    tar xzf $tpath
    pushd $dpath
    if [ -e "bootstrap.sh" ]; then
        ./bootstrap.sh
        sudo ./b2 install
    else
        ./configure
        make
        sudo make install
    fi
    popd; popd
    return 0
}


## ------------------------------------------------
## UBUNTU
## ------------------------------------------------
if [ "$DISTRO" = "UBUNTU" ]; then
    sudo apt-get --ignore-missing -y install \
        git \
        g++ \
        cmake \
        libgflags-dev

## ------------------------------------------------
## FEDORA
## ------------------------------------------------
elif [[ "$DISTRO" == *"FEDORA"* ]]; then
    sudo dnf install -y git \
        gcc-c++ \
        cmake \
        gflags-devel 

## ------------------------------------------------
## UNKNOWN
## ------------------------------------------------
else
    echo "Unknown distribution '$DISTRO'"
    echo "Please contact our support team for additional help." \
         "Be sure to include the contents of this message"
    echo "Platform: $(uname -a)"
    echo
    echo "https://github.com/cmu-db/peloton/issues"
    echo
    exit 1
fi




