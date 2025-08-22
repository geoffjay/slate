test_env = environment()
test_env.set('G_TEST_SRCDIR', meson.current_source_dir())
test_env.set('G_TEST_BUILDDIR', meson.current_build_dir())

test_cargs = [
  '-DG_LOG_DOMAIN="libslate-tests"',
]

# Core tests
test_buildable = executable(
  'test-buildable',
  ['test-buildable.c'],
  dependencies: [libslate_deps],
  link_with: [libslate],
  c_args: test_cargs,
  install: false,
)

test('buildable', test_buildable, env: test_env)

# Config tests
test_config = executable(
  'test-config',
  ['test-config.c'],
  dependencies: [libslate_deps],
  link_with: [libslate],
  c_args: test_cargs,
  install: false,
)

test('config', test_config, env: test_env)

# UI tests
test_enums = executable(
  'test-enums',
  ['test-enums.c'],
  dependencies: [libslate_deps],
  link_with: [libslate],
  c_args: test_cargs,
  install: false,
)

test('enums', test_enums, env: test_env)

test_box = executable(
  'test-box',
  ['test-box.c'],
  dependencies: [libslate_deps],
  link_with: [libslate],
  c_args: test_cargs,
  install: false,
)

test('box', test_box, env: test_env)
