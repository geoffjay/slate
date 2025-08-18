project(
  'libghcl',
  'c',
  version: '0.1.0',
  default_options: [
    'warning_level=3',
    'c_std=c11',
  ],
)

# Dependencies
glib_dep = dependency('glib-2.0', version: '>= 2.66')
gobject_dep = dependency('gobject-2.0', version: '>= 2.66')

# Include directories
libghcl_inc = include_directories('src')

# Source files
libghcl_sources = files(
  'src/hcl-block.c',
  'src/hcl-document.c',
  'src/hcl-enums.c',
  'src/hcl-lexer.c',
  'src/hcl-parser.c',
  'src/hcl-value.c',
)

# Headers
libghcl_headers = files(
  'src/hcl-block.h',
  'src/hcl-document.h',
  'src/hcl-enums.h',
  'src/hcl-lexer.h',
  'src/hcl-parser.h',
  'src/hcl-value.h',
  'src/hcl.h',
)

# Library target
libghcl = static_library(
  'ghcl',
  libghcl_sources,
  dependencies: [glib_dep, gobject_dep],
  include_directories: [libghcl_inc],
  install: false,
)

# Dependency for using libghcl
libghcl_dep = declare_dependency(
  link_with: libghcl,
  include_directories: [libghcl_inc],
  dependencies: [glib_dep, gobject_dep],
)

# Tests
if get_option('tests').enabled()
  subdir('tests')
endif

# Examples
subdir('examples')
