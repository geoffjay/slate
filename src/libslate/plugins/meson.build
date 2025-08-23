plugins_headers = files(
  'slate-header-bar-extension.h',
  'slate-plugin-interface.h',
  'slate-plugin-manager.h',
)

plugins_sources = files(
  'slate-header-bar-extension.c',
  'slate-plugin-interface.c',
  'slate-plugin-manager.c',
)

# Add to libslate sources and headers
libslate_sources += plugins_sources
libslate_headers += plugins_headers

# Install plugin headers
install_headers(plugins_headers, subdir: libslate_header_subdir / 'plugins')
