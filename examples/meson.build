example_hcl_config = executable(
  'test-hcl-config',
  ['test-hcl-config.c'],
  dependencies: [libslate_deps],
  link_with: [libslate],
  install: false,
)

# Example plugins
subdir('plugins')
