project('vulkan-tutorial', 'cpp',
version : '0.1.0',
default_options : ['cpp_std=c++20', 'optimization=3', 'warning_level=3'])

cc = meson.get_compiler('cpp')

vulkan_dep = dependency('vulkan')
glfw_dep = dependency('glfw3')

engine_sources = files(
  'src/platform/lve_window.cpp',
  'src/graphics/vulkan/vk_device.cpp',
  'src/graphics/vulkan/vk_pipeline.cpp',
  'src/graphics/vulkan/vk_swapchain.cpp',
  'src/graphics/vulkan/vk_model.cpp',
  'src/renderer/lve_renderer.cpp',
)

exe_sources = files(
  'src/main.cpp',
  'src/vulkan_app.cpp',
  'src/render_system.cpp',
)

engine_lib = shared_library('engine',
  engine_sources,
  dependencies: [vulkan_dep, glfw_dep]
)

exe = executable('app', 
  exe_sources,
  link_with: engine_lib,
  dependencies: [vulkan_dep, glfw_dep],
)
