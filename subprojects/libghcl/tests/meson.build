# Test suite for libghcl

test_sources = [
  'test-value.c',
  'test-block.c',
  'test-document.c',
  'test-lexer.c',
  'test-parser.c',
]

foreach test_source : test_sources
  test_name = test_source.split('.')[0]
  test_exe = executable(
    test_name,
    test_source,
    dependencies: [libghcl_dep],
    install: false,
  )
  test(test_name, test_exe)
endforeach
