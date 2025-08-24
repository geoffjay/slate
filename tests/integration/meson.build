# Integration Tests
# These tests verify that different components work together correctly

integration_test_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1'),
  dependency('libpeas-2'),
  dependency('glib-2.0'),
  dependency('gobject-2.0'),
]

integration_tests = [
  'test-plugin-system',
  'test-dashboard-system',
]

foreach test_name : integration_tests
  test_exe = executable(
    test_name,
    test_name + '.c',
    dependencies: integration_test_deps,
    link_with: [libslate, libghcl],
    include_directories: [
      include_directories('../../src/libslate'),
      include_directories('../../subprojects/libghcl/src'),
    ],
    install: false,
  )

  test(
    test_name,
    test_exe,
    suite: 'integration',
    timeout: 30,
  )
endforeach
