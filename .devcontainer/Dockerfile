# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.205.2/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version (use Debian 11/9, Ubuntu 18.04/21.04 on local arm64/Apple Silicon): debian-11, debian-10, debian-9, ubuntu-21.04, ubuntu-20.04, ubuntu-18.04
ARG VARIANT="bullseye"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

ENV USER vscode

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    arduino \
    clang-format \
    usbutils \
    udev

RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/local/bin sh

# Install udev rules for Arduino devices
RUN printf 'ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"\nATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"\nATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"\nATTRS{idVendor}=="067b", ATTRS{idProduct}=="2303", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"\n' > /etc/udev/rules.d/98-esp32.rules
RUN curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules -o /etc/udev/rules.d/99-platformio-udev.rules

# Restart udev service to use the newly installed udev rules
RUN service udev restart

# Give our non root user the groups necessary for udev
RUN usermod -a -G dialout $USER
RUN usermod -a -G plugdev $USER
