libslate_header_subdir = join_paths('slate', 'libslate')
libslate_header_dir = get_option('includedir') + '/libslate'

libslate_generated_sources = []
libslate_generated_headers = []
libslate_sources = []
libslate_headers = files('slate.h')

libslate_reqs = [
  'glib-2.0',
  'gtk4',
  'libadwaita-1',
  'gio-2.0',
  'libxml-2.0',
]

libslate_deps = [
  dependency('glib-2.0', version: '>= 2.70.0'),
  dependency('gtk4', version: '>= 4.0.0'),
  dependency('libadwaita-1', version: '>= 1.0.0'),
  dependency('gio-2.0', version: '>= 2.70.0'),
  dependency('libxml-2.0', version: '>= 2.9.0'),
]

subdir('core')
subdir('ui')

sources = libslate_sources + libslate_headers + libslate_generated_sources + libslate_generated_headers

libslate = library(
  'slate-' + api_version,
  sources,
  dependencies: libslate_deps,
  include_directories: include_directories('.'),
  install: true,
  version: meson.project_version(),
  soversion: '0',
)

# Generate pkg-config file
pkgconfig = import('pkgconfig')
pkgconfig.generate(
  libraries: [libslate],
  subdirs: ['libslate-@0@'.format(api_version)],
  version: meson.project_version(),
  name: 'libslate',
  filebase: 'libslate-@0@'.format(api_version),
  description: 'Library for Slate application plugins and components',
  requires: libslate_reqs,
  install_dir: join_paths(get_option('libdir'), 'pkgconfig'),
)

link_libslate = declare_dependency(
  link_with: libslate,
  dependencies: libslate_deps,
  include_directories: [include_directories('.')],
)

# Install main header
install_headers(['slate.h'], subdir: libslate_header_subdir)

# Expose this directory for use with includes
dir_libslate = meson.current_source_dir()
