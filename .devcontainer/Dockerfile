FROM ubuntu:22.04

RUN apt update \
 && apt install -y \
        git \
        clangd \
        clang-format \
        python-is-python3 \
        gcc-mingw-w64 \
        nasm \
        python3 \
 && apt autoremove -y \
 && rm -rf /var/lib/apt/lists/*
 
 LABEL org.opencontainers.image.source=https://github.com/rtecCyberSec/Packer_Development
