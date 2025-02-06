FROM gitpod/workspace-full-vnc:2025-01-15-08-55-28

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

USER root

#install qt
RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends software-properties-common  \
    && apt-get -qq install -y --no-install-recommends qt6-base-dev qt6-tools-dev qt6-tools-dev-tools libqt6charts6 libqt6charts6-dev libqt6opengl6 libqt6opengl6-dev \
    && apt-get -qq install -y --no-install-recommends libxcb-xinerama0-dev \
    && apt-get -qq install -y --no-install-recommends '^libxcb.*-dev' libgles2-mesa-dev libx11-xcb-dev libgl1-mesa-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev \
    && apt-get -qq install -y --no-install-recommends perl \
    && apt-get clean && rm -rf /var/lib/apt/lists/*


# lcov and doxygen
RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends lcov doxygen graphviz \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update \
    && apt-get -qq install -y --no-install-recommends ccache ninja-build \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN pyenv install 3.12.7 && pyenv global 3.12.7

#install cmake
RUN pip3 install cmake==3.31.4
#install conan
RUN pip3 install conan==2.11
RUN pip3  install python-dateutil

# pre-commit
RUN pip3 install pre-commit==4.0.1
RUN pip3 install yamlfmt==1.1.1

USER gitpod


# cmake-format
RUN pip3 install cmake-format==0.6.13

# gcovr
RUN pip3 install gcovr==8.2

# docu stuff
RUN pip3 install Sphinx==8.1.3
RUN pip3 install sphinx_rtd_theme==3.0.2
RUN pip3 install breathe==4.35.0
RUN pip3 install m2r2==0.3.3

RUN echo 'export PIP_USER=false' >> ~/.bashrc
