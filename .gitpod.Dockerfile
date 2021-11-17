FROM ubuntu:bionic as cmake_builder

RUN apt-get update \
 && apt-get install -y --no-install-recommends wget \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*
RUN mkdir /opt/cmake
RUN wget --no-check-certificate https://github.com/Kitware/CMake/releases/download/v3.21.4/cmake-3.21.4-linux-x86_64.sh \
 && sh ./cmake-3.21.4-linux-x86_64.sh --skip-license --prefix=/opt/cmake

FROM gitpod/workspace-full-vnc:commit-2f5689a36391544b3c592f987c5a6abe230f22fe

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

USER root

#install qt
RUN apt-get -qq install -y --no-install-recommends software-properties-common  \
 && add-apt-repository ppa:beineri/opt-qt-5.15.2-focal \
 && apt-get update \
 && apt-get -qq install -y --no-install-recommends qt515base qt515charts-no-lgpl qt515tools \
 && apt-get -qq install -y --no-install-recommends libxcb-xinerama0-dev \
 && apt-get -qq install -y --no-install-recommends '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev \
 && apt-get -qq install -y --no-install-recommends perl \
 && apt-get clean && rm -rf /var/lib/apt/lists/*

#install cmake
COPY --from=cmake_builder /opt/cmake /opt/cmake
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

# lcov and doxygen
RUN apt-get update \
 && apt-get -qq install -y --no-install-recommends lcov doxygen graphviz \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

# install clang-11. gitpod/workspace-full-vnc has clang-13 installes which is not supported by conan yet.
RUN apt-get update \
 && apt-get -qq install -y --no-install-recommends clang-11 clang-10 \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/* \
 && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-11 100 \
 && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-11 100

RUN apt-get update \
 && apt-get -qq install -y --no-install-recommends ninja-build \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

USER gitpod

# conan
RUN pip3 install conan>=1.42.1 \
 && conan profile new default --detect

# cmake-format
RUN pip3 install cmake-format>=0.6.13

# gcovr
RUN pip3 install gcovr>=5.0

# docu stuff
RUN pip3 install sphinx>=4.3.0
RUN pip3 install sphinx_rtd_theme>=1.0.0
RUN pip3 install breathe>=4.31.0

# pre-commit
RUN pip3 install pre-commit>=2.15.0
RUN echo 'export PIP_USER=false' >> ~/.bashrc
