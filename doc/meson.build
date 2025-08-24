if get_option('gtk_doc')
  toml_conf = configuration_data()
  toml_conf.set('VERSION', meson.project_version())

  slate_toml = configure_file(input: 'slate.toml.in', output: 'slate.toml', configuration: toml_conf)

  # Find gi-docgen program (will use subproject if not found system-wide)
  gidocgen = find_program('gi-docgen', required: get_option('gtk_doc'), native: true)

  docs_dir = get_option('datadir') / 'doc'

  # Only build documentation if we have both introspection and gi-docgen
  if get_option('introspection') and gidocgen.found()
    # Get the GIR file path
    gir_file = meson.current_build_dir() / '../src/libslate/Slate-1.0.gir'

    custom_target(
      'slate-doc',
      input: [slate_toml, gir_file],
      output: 'slate-1.0',
      command: [
        gidocgen,
        'generate',
        '--quiet',
        '--add-include-path=@0@'.format(meson.current_build_dir() / '../src/libslate'),
        '--config=@INPUT0@',
        '--output-dir=@OUTPUT@',
        '--no-namespace-dir',
        '--content-dir=@0@'.format(meson.current_source_dir()),
        '@INPUT1@',
      ],
      depend_files: ['configuration.md', 'usage.md'],
      build_by_default: true,
      install: true,
      install_dir: docs_dir,
    )

    message('Documentation will be built with gi-docgen')
  else
    if not get_option('introspection')
      warning('GObject introspection disabled, skipping API documentation')
    endif
    if not gidocgen.found()
      warning('gi-docgen not found, skipping API documentation')
    endif
  endif
endif
