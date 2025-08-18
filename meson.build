# Project Declaration
project(
  'org.gnome.Slate',
  ['vala', 'c'],
  version: '0.1.0',
  meson_version: '>= 1.4.0',
  default_options: [
    'c_std=gnu11',
    'cpp_std=gnu++2a',
    'warning_level=2',
    'channel=other',
  ],
)

version_split = meson.project_version().split('.')
MAJOR_VERSION = version_split[0]
MINOR_VERSION = version_split[1]
MICRO_VERSION = version_split[2]

api_version = '1.0'

status = [
  '',
  '',
  'Slate @0@'.format(meson.project_version()),
  '',
  '',
]

# Warning message
if (version_split[1].to_int() % 2 == 1)
  status += [
    'You are building a debug version of Slate. There may be more bugs in',
    'this version that you are comfortable with.',
    '',
    '',
  ]
endif

status += [
  'Version ............................... : @0@'.format(meson.project_version()),
  'Build Type ............................ : @0@'.format(get_option('buildtype')),
  '',
  'Prefix ................................ : @0@'.format(get_option('prefix')),
  'Libdir ................................ : @0@'.format(join_paths(get_option('prefix'), get_option('libdir'))),
  '',
  '',
]

c_flags = [
  '-fstack-protector',
  '-fPIC',
  '-pedantic',
  '-Wstrict-prototypes',
  '-Wundef',
  '-Werror-implicit-function-declaration',
  '-Wformat',
  '-Wformat-security',
  '-Werror=format-security',
  '-Wconversion',
  '-Wunused-variable',
  '-Wunreachable-code',
  '-Wall',
  '-W',
]

add_global_arguments(c_flags, language: 'c')

gnome = import('gnome')
py = import('python').find_installation('python3.13', required: true)
blueprint_compiler_prog = find_program('blueprint-compiler', required: true)

# Subprojects
libghcl_subproject = subproject('libghcl')
libghcl_dep = libghcl_subproject.get_variable('libghcl_dep')

lib_dependencies = [
  dependency('gtk4'),
  dependency('libadwaita-1'),
  dependency('gio-2.0'),
  dependency('sqlite3'),
  dependency('gee-0.8'),
  libghcl_dep,
]

subdir('data')
subdir('src')
subdir('tests')

message('\n'.join(status))
