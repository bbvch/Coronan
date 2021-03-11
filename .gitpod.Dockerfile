FROM gitpod/workspace-full-vnc

# More information: https://www.gitpod.io/docs/config-docker/

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

#install qt dependencies

RUN sudo cp /etc/apt/sources.list /etc/apt/sources.list~ \
 && sudo sed -Ei 's/^# deb-src /deb-src /' /etc/apt/sources.list \
 && sudo apt-get update
RUN sudo apt-get -qq build-dep -y qt5-default
RUN sudo apt-get -qq install -y libxcb-xinerama0-dev \
 && sudo apt-get -qq install -y '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev \
 && sudo apt-get -qq install -y perl 


#install qt
USER root
COPY --from=bbvch/qt:5.14.2 /usr/local/Qt /usr/local/Qt/5.14.2
ENV Qt5_DIR=/usr/local/Qt/5.14.2
# Install some fonts
RUN wget https://altushost-swe.dl.sourceforge.net/project/dejavu/dejavu/2.37/dejavu-fonts-ttf-2.37.tar.bz2 \
&& tar -xf dejavu-fonts-ttf-2.37.tar.bz2 && rm dejavu-fonts-ttf-2.37.tar.bz2 \
 && mv dejavu-fonts-ttf-2.37/ttf /usr/local/Qt/5.14.2/lib/fonts

USER gitpod
# lcov and doxygen
RUN sudo apt-get -qq install -y lcov doxygen graphviz

# conan
RUN pip3 install conan
RUN conan profile new default --detect

# cmake-format
RUN pip3 install cmake-format

# install clang-11. gitpod/workspace-full-vnc has clang-13 installes which is not supported by conan yet.
RUN sudo apt-get -qq install -y clang-11 clang-10

