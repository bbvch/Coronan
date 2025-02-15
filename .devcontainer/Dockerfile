FROM ubuntu:24.04

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

ARG CLANG_VERSION=18
ARG GCC_VERSION=14
ARG UBUNTU_BASE=noble

RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends \
    make \
    build-essential \
    ninja-build \
    ccache \
    clang-18 \
    clang-tidy-18 \
    clang-format-18 \
    clangd-18 \
    dpkg \
    pkg-config \
    doxygen  \
    lcov \
    git \
    python3-full \
    gcc-14 \
    && rm -rf /var/lib/apt/lists/*

# Install locales package and generate en_US.UTF-8 locale
RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends locales \
    && locale-gen en_US.UTF-8 \
    && update-locale LANG=en_US.UTF-8 \
    && rm -rf /var/lib/apt/lists/*

# Set environment variables
ENV LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8


# Install locales and set to english
ENV LANG en_US.UTF-8
RUN echo $LANG UTF-8 > /etc/locale.gen

ENV VIRTUAL_ENV=/opt/venv
RUN python3 -m venv $VIRTUAL_ENV
ENV PATH="$VIRTUAL_ENV/bin:$PATH"

#install cmake
RUN pip3 install cmake==3.31.4
#install conan
RUN pip3 install conan==2.11
RUN pip3  install python-dateutil==2.9.0

# pre-commit
RUN pip3 install pre-commit==4.0.1
RUN pip3 install yamlfmt==1.1.1

# cmake-format
RUN pip3 install cmake-format==0.6.13

# gcovr
RUN pip3 install gcovr==8.2

# docu stuff
RUN pip3 install Sphinx==8.1.3
RUN pip3 install sphinx_rtd_theme==3.0.2
RUN pip3 install breathe==4.35.0
RUN pip3 install sphinx_mdinclude==0.6.2


#install qt
RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends \
    software-properties-common  \
    qt6-base-dev \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    libqt6widgets6 \
    libqt6charts6 \
    libqt6charts6-dev \
    libqt6opengl6 \
    libqt6opengl6-dev \
    libxcb-xinerama0-dev \
    '^libxcb.*-dev' \
    libgles2-mesa-dev \
    libx11-xcb-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxrender-dev \
    libxi-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    perl \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# lcov and doxygen
RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends \
    lcov \
    doxygen \
    graphviz \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends \
    ccache \
    ninja-build \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install gcc and dependencies
RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends \
    g++-${GCC_VERSION} \
    gcc-${GCC_VERSION} \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} 100
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 100
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-${GCC_VERSION} 90
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-${GCC_VERSION} 90
RUN update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-${GCC_VERSION} 100


# Install Clang and dependencies
RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends \
    clang-${CLANG_VERSION} \
    clang-tidy-${CLANG_VERSION} \
    clang-format-${CLANG_VERSION} \
    llvm-${CLANG_VERSION} \
    llvm-${CLANG_VERSION}-dev \
    llvm-${CLANG_VERSION}-runtime \
    llvm \
    libc++1 \
    libc++abi1 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${CLANG_VERSION} 100
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${CLANG_VERSION} 100
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang-${CLANG_VERSION} 100
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-${CLANG_VERSION} 100
RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-${CLANG_VERSION} 100
RUN update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-${CLANG_VERSION} 100
