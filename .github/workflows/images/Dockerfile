FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates curl git cppcheck zip unzip tar pkg-config \
    libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev \
    build-essential cmake wget lsb-release wget gnupg software-properties-common \
    && wget -qO - https://apt.llvm.org/llvm.sh | bash -s -- 20 \
    && apt-get install -y clang-20 clang-tidy-20 clang-format-20 \
    && ln -s /usr/bin/clang-format-20 /usr/bin/clang-format \
    && ln -s /usr/bin/clang-tidy-20 /usr/bin/clang-tidy \
    && rm -rf /var/lib/apt/lists/*
    
WORKDIR /app