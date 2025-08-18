# libghcl - GLib HCL Parser Library

A GLib/GObject-compatible implementation of the HashiCorp Configuration Language (HCL) parser for the Slate project.

## Overview

libghcl provides a C library for parsing HCL configuration files into GObject structures. It implements the [HCL syntax specification](https://github.com/hashicorp/hcl/blob/main/hclsyntax/spec.md) and is designed to integrate seamlessly with GLib-based applications.

## Features

- **Complete HCL Syntax Support**: Supports all HCL data types (strings, numbers, booleans, lists, objects)
- **Block-based Configuration**: Parse HCL blocks with attributes and nested structures
- **GObject Integration**: All types are proper GObjects with reference counting
- **Error Handling**: Comprehensive error reporting with location information
- **Memory Safe**: Uses GLib memory management and automatic cleanup
- **Extensible**: Designed for easy integration into larger applications

## Supported HCL Syntax

```hcl
# Comments (both # and // styles)
application "myapp" {
  title = "My Application"
  debug = true
  port = 8080

  features = ["auth", "logging", "metrics"]

  database {
    host = "localhost"
    port = 5432
    ssl = true

    connection_pool = {
      max_connections = 10
      idle_timeout = 300
    }
  }
}
```

## Core Types

### HclValue

Represents any HCL value (string, number, boolean, list, object):

```c
HclValue *value = hcl_value_new_string("hello");
HclValue *list = hcl_value_new_list();
hcl_value_list_add_item(list, value);
```

### HclBlock

Represents an HCL configuration block:

```c
HclBlock *block = hcl_block_new("application", "myapp");
hcl_block_set_attribute(block, "port", hcl_value_new_int(8080));
```

### HclDocument

Container for the entire parsed HCL document:

```c
HclDocument *doc = hcl_parse_string(config_text, &error);
GList *blocks = hcl_document_get_blocks_by_type(doc, "application");
```

### HclParser

The main parser interface:

```c
HclParser *parser = hcl_parser_new();
HclDocument *doc = hcl_parser_parse_file(parser, "config.hcl", &error);
```

## Usage Example

```c
#include <hcl.h>

int main() {
  const char *config =
    "application \"myapp\" {\n"
    "  port = 8080\n"
    "  debug = true\n"
    "}\n";

  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) doc = hcl_parse_string(config, &error);

  if (!doc) {
    g_printerr("Parse error: %s\n", error->message);
    return 1;
  }

  GList *blocks = hcl_document_get_blocks_by_type(doc, "application");
  if (blocks) {
    HclBlock *app_block = blocks->data;
    HclValue *port = hcl_block_get_attribute(app_block, "port");

    if (port && hcl_value_is_number(port)) {
      g_print("Port: %" G_GINT64_FORMAT "\n", hcl_value_get_int(port));
    }
  }

  g_list_free(blocks);
  return 0;
}
```

## Building

libghcl uses the Meson build system and is designed as a subproject:

```bash
# From the main project directory
meson setup builddir
meson compile -C builddir

# Run tests
meson test -C builddir --suite libghcl
```

## Integration with Slate

For the Slate project, libghcl enables declarative configuration of dashboards and UI components:

```hcl
application "slate-dashboard" {
  title = "Production Monitor"

  dashboard {
    box "main" {
      orientation = "vertical"

      box "header" {
        height = "auto"
        # Header components
      }

      box "content" {
        height = "expand"
        orientation = "horizontal"

        # Content components
      }
    }
  }
}
```

## Dependencies

- GLib 2.66+
- GObject 2.66+

## License

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

## Contributing

This library is part of the Slate project. Please follow the Slate project's contribution guidelines.
