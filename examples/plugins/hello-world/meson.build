# Hello World Plugin Example
# Demonstrates basic plugin development for Slate

plugin_sources = files(
  'hello-world-plugin.c',
  'hello-world-widget.c',
)

plugin_headers = files(
  'hello-world-plugin.h',
  'hello-world-widget.h',
)

# Plugin shared library
hello_world_plugin = shared_library(
  'hello-world',
  plugin_sources,
  dependencies: [
    dependency('gtk4'),
    dependency('libadwaita-1'),
    dependency('libpeas-2'),
    dependency('glib-2.0'),
    dependency('gobject-2.0'),
    libghcl_dep,
  ],
  link_with: [libslate],
  include_directories: [
    include_directories('../../../src/libslate'),
  ],
  install: true,
  install_dir: get_option('libdir') / 'slate' / 'plugins',
)

# Plugin metadata file
plugin_data = configuration_data()
plugin_data.set('PLUGIN_NAME', 'Hello World')
plugin_data.set('PLUGIN_DESCRIPTION', 'A simple example plugin for Slate')
plugin_data.set('PLUGIN_VERSION', '1.0.0')
plugin_data.set('PLUGIN_AUTHORS', 'Slate Development Team')
plugin_data.set('PLUGIN_WEBSITE', 'https://github.com/geoffjay/slate')

configure_file(
  input: 'hello-world.plugin.in',
  output: 'hello-world.plugin',
  configuration: plugin_data,
  install: true,
  install_dir: get_option('libdir') / 'slate' / 'plugins',
)

# Plugin UI resources
plugin_ui_resources = gnome.compile_resources(
  'hello-world-resources',
  'hello-world.gresource.xml',
  source_dir: 'ui',
  c_name: 'hello_world',
)

# Add resources to plugin
hello_world_plugin_sources = [plugin_sources, plugin_ui_resources]
