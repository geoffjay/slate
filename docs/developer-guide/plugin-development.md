# Plugin Development Guide

This guide covers how to develop plugins for Slate using the libpeas-based plugin system.

## Overview

Slate uses libpeas-2 for its plugin architecture, providing a flexible and secure way to extend the application. Plugins can extend various aspects of Slate:

- **Header Bar Extensions**: Add buttons and widgets to the header bar
- **Dashboard Widgets**: Create custom dashboard components
- **Configuration Extensions**: Add custom configuration options
- **Data Sources**: Provide data for visualization

## Plugin Structure

A typical Slate plugin consists of:

```
my-plugin/
├── meson.build                 # Build configuration
├── my-plugin.plugin.in         # Plugin metadata
├── my-plugin.c                 # Main plugin implementation
├── my-plugin.h                 # Plugin header
├── my-plugin-widget.c          # Custom widgets (optional)
├── my-plugin-widget.h
├── my-plugin.gresource.xml     # UI resources
└── ui/
    └── my-plugin-widget.ui     # UI definitions
```

## Basic Plugin Implementation

### 1. Plugin Metadata (`my-plugin.plugin.in`)

```ini
[Plugin]
Module=my-plugin
Name=@PLUGIN_NAME@
Description=@PLUGIN_DESCRIPTION@
Version=@PLUGIN_VERSION@
Authors=@PLUGIN_AUTHORS@
Website=@PLUGIN_WEBSITE@
Copyright=Copyright © 2025 @PLUGIN_AUTHORS@
Builtin=false
Hidden=false

# Plugin capabilities
Capabilities=header-bar-extension,dashboard-widget

# Dependencies
Depends=slate-core>=1.0

# Loader information
Loader=C
```

### 2. Plugin Implementation

```c
#include <slate.h>
#include <libpeas.h>

struct _MyPlugin
{
  PeasExtensionBase parent_instance;
  gboolean active;
};

static void slate_plugin_interface_init (SlatePluginInterfaceInterface *iface);
static void slate_header_bar_extension_interface_init (SlateHeaderBarExtensionInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE (MyPlugin, my_plugin, PEAS_TYPE_EXTENSION_BASE,
                               G_IMPLEMENT_INTERFACE (SLATE_TYPE_PLUGIN_INTERFACE, slate_plugin_interface_init)
                               G_IMPLEMENT_INTERFACE (SLATE_TYPE_HEADER_BAR_EXTENSION, slate_header_bar_extension_interface_init))

/* Plugin Interface Implementation */
static void
my_plugin_activate (SlatePluginInterface *plugin)
{
  MyPlugin *self = MY_PLUGIN (plugin);
  g_debug ("My plugin activated");
  self->active = TRUE;
  g_signal_emit_by_name (plugin, "ready");
}

static void
my_plugin_deactivate (SlatePluginInterface *plugin)
{
  MyPlugin *self = MY_PLUGIN (plugin);
  g_debug ("My plugin deactivated");
  self->active = FALSE;
}

/* Header Bar Extension Implementation */
static GtkWidget *
my_plugin_create_header_widget (SlateHeaderBarExtension *extension)
{
  GtkWidget *button = gtk_button_new_with_label ("My Plugin");
  gtk_widget_set_tooltip_text (button, "My plugin button");
  return button;
}

/* Plugin Entry Point */
G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
  peas_object_module_register_extension_type (module,
                                              SLATE_TYPE_PLUGIN_INTERFACE,
                                              MY_TYPE_PLUGIN);

  peas_object_module_register_extension_type (module,
                                              SLATE_TYPE_HEADER_BAR_EXTENSION,
                                              MY_TYPE_PLUGIN);
}
```

### 3. Build Configuration (`meson.build`)

```meson
plugin_sources = files(
  'my-plugin.c',
  'my-plugin-widget.c',
)

# Plugin shared library
my_plugin = shared_library(
  'my-plugin',
  plugin_sources,
  dependencies: [
    dependency('gtk4'),
    dependency('libadwaita-1'),
    dependency('libpeas-2'),
    libslate_dep,
  ],
  install: true,
  install_dir: get_option('libdir') / 'slate' / 'plugins',
)

# Plugin metadata
plugin_data = configuration_data()
plugin_data.set('PLUGIN_NAME', 'My Plugin')
plugin_data.set('PLUGIN_DESCRIPTION', 'An example plugin')
plugin_data.set('PLUGIN_VERSION', '1.0.0')
plugin_data.set('PLUGIN_AUTHORS', 'Your Name')
plugin_data.set('PLUGIN_WEBSITE', 'https://example.com')

configure_file(
  input: 'my-plugin.plugin.in',
  output: 'my-plugin.plugin',
  configuration: plugin_data,
  install: true,
  install_dir: get_option('libdir') / 'slate' / 'plugins',
)
```

## Extension Points

### Header Bar Extensions

Header bar extensions allow plugins to add widgets to the application header bar:

```c
static GtkWidget *
my_plugin_create_header_widget (SlateHeaderBarExtension *extension)
{
  GtkWidget *button = gtk_button_new_with_label ("Action");
  g_signal_connect (button, "clicked", G_CALLBACK (on_action_clicked), extension);
  return button;
}

static int
my_plugin_get_priority (SlateHeaderBarExtension *extension)
{
  return 50; // Medium priority
}

static const char *
my_plugin_get_position (SlateHeaderBarExtension *extension)
{
  return "end"; // Place in end section
}
```

### Dashboard Widgets

Create custom dashboard widgets by extending `SlateDashboardCard`:

```c
struct _MyDashboardWidget
{
  SlateDashboardCard parent_instance;
  GtkLabel *data_label;
  char *data_source;
};

G_DEFINE_FINAL_TYPE (MyDashboardWidget, my_dashboard_widget, SLATE_TYPE_DASHBOARD_CARD)

static void
my_dashboard_widget_init (MyDashboardWidget *self)
{
  slate_dashboard_card_set_title (SLATE_DASHBOARD_CARD (self), "My Data");
  slate_dashboard_card_set_subtitle (SLATE_DASHBOARD_CARD (self), "Custom widget");

  self->data_label = GTK_LABEL (gtk_label_new ("No data"));
  slate_dashboard_card_set_content (SLATE_DASHBOARD_CARD (self), GTK_WIDGET (self->data_label));
}
```

## Best Practices

### 1. Resource Management

- Always clean up resources in the `deactivate` method
- Use `g_autoptr` and `g_autofree` for automatic cleanup
- Disconnect signal handlers when deactivating

### 2. Error Handling

- Use `g_debug`, `g_warning`, and `g_error` for logging
- Handle errors gracefully and provide user feedback
- Validate input parameters with `g_return_if_fail`

### 3. UI Integration

- Follow GNOME Human Interface Guidelines
- Use appropriate CSS classes for styling
- Ensure accessibility with proper labels and roles

### 4. Performance

- Avoid blocking operations in the main thread
- Use `GTask` for asynchronous operations
- Cache expensive computations

## Testing Plugins

### 1. Unit Testing

Create unit tests for your plugin logic:

```c
static void
test_my_plugin_creation (void)
{
  MyPlugin *plugin = my_plugin_new ();
  g_assert_nonnull (plugin);
  g_assert_false (my_plugin_is_active (plugin));
  g_object_unref (plugin);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);
  g_test_add_func ("/my-plugin/creation", test_my_plugin_creation);
  return g_test_run ();
}
```

### 2. Integration Testing

Test your plugin with the actual Slate application:

```bash
# Install plugin to development location
meson install -C builddir

# Run Slate with plugin debugging
SLATE_DEBUG_PLUGINS=1 ./builddir/src/slate/slate
```

## Distribution

### 1. Plugin Packages

Plugins can be distributed as:

- Shared libraries with metadata files
- Flatpak extensions
- System packages (RPM, DEB)

### 2. Plugin Registry

Consider submitting your plugin to the Slate plugin registry for easy discovery and installation.

## Example Plugins

See the `examples/plugins/` directory for complete plugin examples:

- **hello-world**: Basic plugin with header bar button and dashboard widget
- **data-logger**: Advanced plugin demonstrating data collection and visualization
- **configuration-editor**: Plugin that extends the configuration system

## API Reference

For detailed API documentation, see:

- [libslate API Reference](../api/libslate-1.0/)
- [Plugin Interfaces Reference](../api/slate-plugins-1.0/)

## Support

- **Documentation**: https://slate.example.com/docs
- **Issues**: https://github.com/geoffjay/slate/issues
- **Discussions**: https://github.com/geoffjay/slate/discussions
