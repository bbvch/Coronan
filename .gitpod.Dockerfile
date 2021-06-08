FROM ubuntu:bionic as cmake_builder

RUN apt-get update
RUN apt-get install -y --no-install-recommends wget
RUN mkdir /opt/cmake
RUN wget --no-check-certificate https://github.com/Kitware/CMake/releases/download/v3.20.3/cmake-3.20.3-linux-x86_64.sh \
 && sh ./cmake-3.20.3-linux-x86_64.sh --skip-license --prefix=/opt/cmake

FROM gitpod/workspace-full-vnc

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

USER root

#install qt
RUN apt-get -qq install -y --no-install-recommends software-properties-common
RUN add-apt-repository ppa:beineri/opt-qt-5.15.2-focal
RUN apt-get update
RUN apt-get -qq install -y --no-install-recommends qt515base qt515charts-no-lgpl qt515tools
RUN apt-get -qq install -y --no-install-recommends libxcb-xinerama0-dev \
 && apt-get -qq install -y --no-install-recommends '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev \
 && apt-get -qq install -y --no-install-recommends perl

#install cmake
COPY --from=cmake_builder /opt/cmake /opt/cmake
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

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
RUN pip3 install conan==1.37.1
RUN conan profile new default --detect

# cmake-format
RUN pip3 install cmake-format==0.6.13

# pre-commit
RUN pip3 install pre-commit==2.13.0
RUN echo 'export PIP_USER=false' >> ~/.bashrc
