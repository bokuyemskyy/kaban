FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates curl git cppcheck zip unzip tar pkg-config \
    libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev \
    build-essential cmake wget lsb-release gnupg software-properties-common \
    && wget -qO - https://apt.llvm.org/llvm.sh | bash -s -- 21 \
    && apt-get install -y clang-21 clang-tidy-21 clang-format-21 \
    && ln -sf /usr/bin/clang-format-21 /usr/bin/clang-format \
    && ln -sf /usr/bin/clang-tidy-21 /usr/bin/clang-tidy \
    && rm -rf /var/lib/apt/lists/*
    
WORKDIR /app