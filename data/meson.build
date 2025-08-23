# Compile Blueprint files to GtkBuilder XML under the build dir
blueprints = custom_target(
  'blueprints',
  input: files(
    'ui/main.blp',
    'ui/preferences.blp',
    'ui/project_dashboard.blp',
  ),
  output: 'ui',
  command: [
    py,
    blueprint_compiler_prog,
    'batch-compile',
    '@OUTPUT@',
    '@CURRENT_SOURCE_DIR@/ui',
    '@INPUT@',
  ],
)

# Compile and embed resources, looking for compiled UI in the build dir
resources = gnome.compile_resources(
  'slate-resources',
  'org.gnome.Slate.gresource.xml',
  source_dir: meson.current_build_dir(),
  c_name: 'slate',
  dependencies: blueprints,
)

# Include gsettings
subdir('gsettings')
