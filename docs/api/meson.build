# API Documentation Generation
# Uses gi-docgen to generate comprehensive API documentation

if get_option('gtk_doc')
  # Generate documentation for libslate
  libslate_doc = gnome.gidocgen(
    'libslate-1.0',
    sources: [
      libslate_gir[0],
    ],
    dependencies: [
      libslate_dep,
    ],
    build_by_default: true,
    install: true,
    install_dir: get_option('datadir') / 'doc' / 'slate',
  )

  # Generate documentation for plugin interfaces
  plugin_doc = gnome.gidocgen(
    'slate-plugins-1.0',
    sources: [],
    dependencies: [
      libslate_dep,
    ],
    build_by_default: true,
    install: true,
    install_dir: get_option('datadir') / 'doc' / 'slate-plugins',
  )
endif
