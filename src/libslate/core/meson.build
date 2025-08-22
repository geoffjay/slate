core_headers = [
  'slate-buildable.h',
  'slate-config.h',
]

core_sources = [
  'slate-buildable.c',
  'slate-config.c',
]

libslate_core_sources = files(core_sources)
libslate_core_headers = files(core_headers)

libslate_headers += libslate_core_headers
libslate_sources += libslate_core_sources

install_headers(core_headers, subdir: join_paths(libslate_header_subdir, 'core'))
