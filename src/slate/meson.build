sources = [
  'app.vala',
  'preferences.vala',
  'db.vala',
  'ui.vala',
  'settings.vala',
]

vapi_files = [
  'libslate-1.0.vapi',
]

slate_c_args = [
  '-DG_LOG_DOMAIN="slate"',
  '-DGETTEXT_PACKAGE="org.gnome.Slate"',
]

slate_dependencies = lib_dependencies + [link_libslate]

executable(
  'slate',
  sources + vapi_files,
  resources,
  install: false,
  dependencies: slate_dependencies,
  c_args: slate_c_args,
  vala_args: ['--vapidir', meson.current_source_dir(), '--pkg', 'libslate-1.0'],
)
