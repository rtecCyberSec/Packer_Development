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
        cargo \
 && apt autoremove -y \
 && rm -rf /var/lib/apt/lists/*
 && curl https://nim-lang.org/choosenim/init.sh -sSf | sh
 
 LABEL org.opencontainers.image.source=https://github.com/rtecCyberSec/Packer_Development
