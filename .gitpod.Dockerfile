FROM gitpod/workspace-full-vnc:commit-8fc141dbdd92030a435ead06617c6d37651d8312

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
RUN pip3 install cmake>=3.21.0

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
