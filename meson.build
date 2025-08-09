# Project Declaration  
project(
  'org.slate.app',
  ['vala', 'c'],
  version: '0.0.1'
)

gnome = import('gnome')
py = import('python').find_installation('python3.13', required: true)
blueprint_compiler_prog = find_program('blueprint-compiler', required: true)

lib_dependencies = [  
    dependency('gtk4'),  
    dependency('libadwaita-1')  
]  

# Compile Blueprint files to GtkBuilder XML under the build dir
blueprints = custom_target(
  'blueprints',
  input: files('data/ui/main.blp'),
  output: 'ui',
  command: [
    py,
    blueprint_compiler_prog,
    'batch-compile',
    '@OUTPUT@',
    '@CURRENT_SOURCE_DIR@/data/ui',
    '@INPUT@'
  ]
)

# Compile and embed resources, looking for compiled UI in the build dir
resources = gnome.compile_resources(
  'slate-resources',
  'data/org.slate.app.gresource.xml',
  source_dir: meson.current_build_dir(),
  c_name: 'slate',
  dependencies: blueprints
)

executable(
  'slate',
  'src/app.vala',
  resources,
  dependencies: lib_dependencies,
  install: false
)
