FROM gitpod/workspace-full

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

#install qt
COPY --from=bbvch/qt:5.14.2 /usr/local/Qt /usr/local/Qt
ENV Qt5_DIR=/usr/local/Qt

#install qt dependencies
RUN apt-get -qq install -y mesa-common-dev libglu1-mesa-dev libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxkbcommon-x11-dev

RUN pip3 install conan
RUN conan profile new default --detect
