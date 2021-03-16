FROM ubuntu:bionic as cmake_builder

ARG cmake_version=3.20
ARG cmake_build=0-rc4

RUN apt-get update
RUN apt-get install -y wget
RUN wget https://cmake.org/files/v$cmake_version/cmake-$cmake_version.$cmake_build-linux-x86_64.sh
RUN mkdir /opt/cmake
RUN sh cmake-$cmake_version.$cmake_build-linux-x86_64.sh --skip-license --prefix=/opt/cmake

FROM bbvch/qt:5.14.2 as qt_builder

FROM gitpod/workspace-full-vnc:commit-0d86f18fcb06832838d2cb77636d2c1a6a7dbe6c

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

USER root
#install cmake
COPY --from=cmake_builder /opt/cmake /opt/cmake
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

#install qt
COPY --from=qt_builder /usr/local/Qt /usr/local/Qt/5.14.2
ENV Qt5_DIR=/usr/local/Qt/5.14.2
# Install some fonts
RUN wget https://altushost-swe.dl.sourceforge.net/project/dejavu/dejavu/2.37/dejavu-fonts-ttf-2.37.tar.bz2 \
&& tar -xf dejavu-fonts-ttf-2.37.tar.bz2 && rm dejavu-fonts-ttf-2.37.tar.bz2 \
 && mv dejavu-fonts-ttf-2.37/ttf /usr/local/Qt/5.14.2/lib/fonts

#install qt dependencies
RUN sed -Ei 's/^# deb-src /deb-src /' /etc/apt/sources.list \
 && apt-get update
RUN apt-get -qq build-dep -y qt5-default
RUN apt-get -qq install -y --no-install-recommends libxcb-xinerama0-dev \
 && apt-get -qq install -y --no-install-recommends '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev \
 && apt-get -qq install -y --no-install-recommends perl

# lcov and doxygen
RUN apt-get -qq install -y --no-install-recommends lcov doxygen graphviz

# install clang-11. gitpod/workspace-full-vnc has clang-13 installes which is not supported by conan yet.
RUN apt-get -qq install -y --no-install-recommends clang-11 clang-10
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-11 100
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-11 100

RUN apt-get -qq install -y --no-install-recommends ninja-build

RUN apt-get clean \
 && rm -rf /var/lib/apt/lists/*

USER gitpod

# conan
RUN pip3 install conan==1.34.0
RUN conan profile new default --detect

# cmake-format
RUN pip3 install cmake-format==0.6.13

# pre-commit
RUN pip3 install pre-commit==2.11.1
