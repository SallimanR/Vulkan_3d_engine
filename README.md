# Building requirements:
- C++ 20
- compiler: GCC or Clang
- cmake
- just (optional for convenience)

## Development:
```sh
just setup
just build
just run
```

### You can use RenderDoc for GPU pipeline debugging. For Linux config see [RenderDoc-example.cap](./RenderDoc-example.cap)

## Release
```sh
just setup-release
just build-release
just run-release
```

# Learning:
- [3b1b linear algebra course](https://www.youtube.com/watch?v=fNk_zzaMoSs&list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab)
- [Vulkan guide](https://vkguide.dev/)
- [Vulkan documentation (official)](https://docs.vulkan.org/)
