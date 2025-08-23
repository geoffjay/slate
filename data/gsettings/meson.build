gsettings_schema = 'org.gnome.Slate.gschema.xml'

# Install the schema file
install_data(
  gsettings_schema,
  install_dir: get_option('datadir') / 'glib-2.0' / 'schemas',
)

# Compile schemas for local testing
gnome = import('gnome')
gnome.compile_schemas(build_by_default: true, depend_files: gsettings_schema)
