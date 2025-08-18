# Examples for libghcl

example_sources = [
  'slate-example.c',
]

foreach example_source : example_sources
  example_name = example_source.split('.')[0]
  executable(
    example_name,
    example_source,
    dependencies: [libghcl_dep],
    install: false,
  )
endforeach
