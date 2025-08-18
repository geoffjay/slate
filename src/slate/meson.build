sources = [
  'app.vala',
  'preferences.vala',
  'db.vala',
  'ui.vala',
]

slate_c_args = [
  '-DG_LOG_DOMAIN="slate"',
]

executable(
  'slate',
  sources,
  resources,
  install: false,
  dependencies: lib_dependencies,
  c_args: slate_c_args,
)
