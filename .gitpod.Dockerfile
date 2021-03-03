FROM ubuntu:bionic as qt_builder

ARG QT_VERSION=5.14.2

# Build and install Qt from source
RUN apt-get update
RUN apt-get -qq  install -y build-essential
RUN apt-get -qq  install -y python python3
RUN apt-get -qq  install -y git
RUN apt-get -qq install -y perl 
RUN apt-get -qq install -y mesa-common-dev libglu1-mesa-dev libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxkbcommon-x11-dev
RUN git clone https://code.qt.io/qt/qt5.git
WORKDIR qt5
RUN git checkout v${QT_VERSION}
RUN perl init-repository
WORKDIR ./qt-build
RUN ../configure -qt-xcb -qt-zlib -qt-libjpeg -qt-libpng -qt-freetype -qt-pcre -qt-harfbuzz -opensource -nomake examples -nomake tests -confirm-license -prefix "/usr/local/Qt"
RUN make -s -j$(nproc)
RUN make install
WORKDIR /
RUN rm -rf qt

FROM gitpod/workspace-full

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

#install qt
COPY --from=qt_builder /usr/local/Qt /usr/local/Qt
ENV Qt5_DIR=/usr/local/Qt

#install qt dependencies
RUN apt-get -qq install -y mesa-common-dev libglu1-mesa-dev libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxkbcommon-x11-dev

RUN pip3 install conan
RUN conan profile new default --detect
