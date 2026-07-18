cc := env("CC", "clang")
cxx := env("CXX", "clang++")
linker_flags := env("LINKER_FLAGS", "-fuse-ld=mold")
march := env("MARCH", "native")
lto_flag := env("LTO_FLAG", "-flto")

# ======== DEVELOPMENT =========
setup:
    cmake --preset linux-debug

build:
    ninja -C build-linux-debug

run:
    ./build-linux-debug/bin/vulkan-tutorial

# ======== RELEASE =========
setup-release:
    cmake -G Ninja -S . -B build-release \
    -DCMAKE_C_COMPILER={{ cc }} \
    -DCMAKE_CXX_COMPILER={{ cxx }} \
    -DCMAKE_C_FLAGS="-march={{ march }} {{ lto_flag }}" \
    -DCMAKE_CXX_FLAGS="-march={{ march }} {{ lto_flag }}" \
    -DCMAKE_EXE_LINKER_FLAGS="{{ linker_flags }}" \
    -DCMAKE_SHARED_LINKER_FLAGS="{{ linker_flags }}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

build-release:
    ninja -C build-release

run-release:
    ./build-release/bin/vulkan-tutorial
