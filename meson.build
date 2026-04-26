project('vulkan-tutorial', 'cpp',
version : '0.1.0',
default_options : ['warning_level=3'])

cc = meson.get_compiler('cpp')

vulkan_dep = dependency('vulkan')
glfw_dep = dependency('glfw3')

sources = files(
  'src/main.cpp',
  'src/platform/lve_window.cpp',
  'src/platform/lve_app_base.cpp'
)

exe = executable('app', 
  sources,
  dependencies: [vulkan_dep, glfw_dep],
)
