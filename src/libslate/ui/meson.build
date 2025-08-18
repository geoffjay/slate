# Import required modules
fs = import('fs')

# UI blueprint files
ui_blueprint_files = [
  'data/ui/box.blp',
]

ui_compiled_files = []

# Compile blueprint files to UI files
foreach ui_file : ui_blueprint_files
  ui_file_base = fs.stem(ui_file.split('/')[-1])
  ui_compiled = custom_target(
    ui_file_base + '-ui',
    input: ui_file,
    output: ui_file_base + '.ui',
    command: [
      blueprint_compiler_prog,
      'compile',
      '--output', '@OUTPUT@',
      '@INPUT@',
    ],
    install: false,
  )
  ui_compiled_files += ui_compiled
endforeach

# GResource generation
gresource = join_paths(meson.current_source_dir(), 'data', 'org.gnome.libslate-ui.gresource.xml')

ui_resources = gnome.compile_resources(
  'libslate-ui-resources',
  gresource,
  source_dir: meson.current_build_dir(),
  dependencies: ui_compiled_files,
  c_name: 'libslate_ui',
)

ui_headers = [
  'slate-enums.h',
  'slate-widget.h',
  'slate-utility.h',
  'slate-box.h',
]

ui_sources = [
  'slate-enums.c',
  'slate-widget.c',
  'slate-utility.c',
  'slate-box.c',
]

libslate_generated_sources += [ui_resources[0]]
libslate_generated_headers += [ui_resources[1]]

libslate_ui_sources = files(ui_sources)
libslate_ui_headers = files(ui_headers)

libslate_headers += libslate_ui_headers
libslate_sources += libslate_ui_sources

install_headers(ui_headers, subdir: join_paths(libslate_header_subdir, 'ui'))
