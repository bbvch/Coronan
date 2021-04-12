FROM gitpod/workspace-full-vnc:commit-0d86f18fcb06832838d2cb77636d2c1a6a7dbe6c

LABEL maintainer="Michel Estermann <estermann.michel@gmail.com>"

ENV USER gitpod
RUN /bin/bash -c "$(curl -fsSL https://nixos.org/nix/install)"

RUN echo 'if [ -e /home/gitpod/.nix-profile/etc/profile.d/nix.sh ]; then . /home/gitpod/.nix-profile/etc/profile.d/nix.sh; fi' >> /home/gitpod/.bashrc

SHELL ["/bin/bash", "-c"]

RUN . /home/gitpod/.nix-profile/etc/profile.d/nix.sh && nix-env -i direnv
RUN . /home/gitpod/.nix-profile/etc/profile.d/nix.sh && direnv hook bash >> ~/.bashrc
