# Slate Configuration Guide

Slate uses HCL (HashiCorp Configuration Language) for its configuration files. This provides a human-readable and flexible way to define dashboard layouts, widgets, and application settings.

## Configuration File Structure

A Slate configuration file consists of several main sections:

### Application Settings

The application settings define global properties for your Slate instance:

```hcl
# Application properties
app = "My Dashboard"
admin = false
startup_page = "main"
dark_theme = true
log_level = "*:5"
```

#### Available Application Properties

- `app` (string): The name of your application/dashboard
- `admin` (boolean): Enable administrative features (default: false)
- `startup_page` (string): The ID of the page to show on startup
- `dark_theme` (boolean): Use dark theme by default (default: false)
- `log_level` (string): Logging level configuration (format: "\*:level" where level is 0-5)

### Pages

Pages are the main containers for your dashboard content. Each page can contain multiple widgets and layouts:

```hcl
page "main" {
  title = "Main Dashboard"

  # Page content goes here
}
```

#### Page Properties

- `title` (string): Display title for the page
- Content is defined using nested `object` blocks

### Objects and Widgets

Objects are the building blocks of your dashboard. They can be containers (like boxes) or widgets (like charts):

#### Box Container

Boxes are used to organize and layout other widgets:

```hcl
object "box" {
  id = "main-container"
  orientation = "horizontal"  # or "vertical"
  spacing = 5
  homogeneous = false
  expand = true
  fill = true

  # Margins
  margin_top = 5
  margin_right = 5
  margin_bottom = 5
  margin_left = 5

  # Child objects go here
}
```

##### Box Properties

- `id` (string, required): Unique identifier for the box
- `orientation` (string): Layout direction - "horizontal" or "vertical"
- `spacing` (integer): Space between child widgets in pixels
- `homogeneous` (boolean): Whether all children should have equal size
- `expand` (boolean): Whether the box should expand to fill available space
- `fill` (boolean): Whether the box should fill the allocated space
- `margin_top`, `margin_right`, `margin_bottom`, `margin_left` (integer): Margins in pixels

#### Chart Widgets

Charts display data in various visual formats:

##### Strip Chart

```hcl
object "stripchart" {
  id = "flow-chart"
  title = "Liquid Flow Rate"
  height_min = 200
  width_min = 300
  show_grid = true
  show_title = true
  points_per_second = 10

  object "chart-axis" {
    id = "y-axis"
    label = "Flow Rate (L/min)"
    min = 0.0
    max = 100.0

    object "trace" {
      id = "flow-trace"
      ref = "/sensors/flow01"
      color = "blue"
      line_width = 2
    }
  }
}
```

##### Polar Chart

```hcl
object "polar-chart" {
  id = "direction-chart"
  title = "Wind Direction"
  refresh_ms = 1000
  height_min = 200
  width_min = 200
  show_grid = true
  show_grid_border = true
  show_title = true

  object "polar-axis" {
    id = "angle-axis"
    label = "Degrees"
    min = 0.0
    max = 360.0

    object "trace" {
      id = "wind-trace"
      ref = "/sensors/wind_direction"
      color = "green"
    }
  }
}
```

##### Chart Properties

**Common Chart Properties:**

- `id` (string, required): Unique identifier
- `title` (string): Chart title
- `height_min`, `width_min` (integer): Minimum dimensions in pixels
- `show_grid` (boolean): Display grid lines
- `show_title` (boolean): Display the chart title
- `refresh_ms` (integer): Refresh interval in milliseconds

**Strip Chart Specific:**

- `points_per_second` (integer): Data point sampling rate

**Polar Chart Specific:**

- `show_grid_border` (boolean): Show border around the grid

**Chart Axis Properties:**

- `id` (string, required): Unique identifier for the axis
- `label` (string): Axis label text
- `min`, `max` (float): Axis range

**Trace Properties:**

- `id` (string, required): Unique identifier for the trace
- `ref` (string, required): Data reference path
- `color` (string): Color name or hex code
- `line_width` (integer): Line thickness in pixels

#### Control Widgets

##### Analog Input Control

```hcl
object "ai" {
  id = "temperature-input"
  ref = "/sensors/temp01"
}
```

##### Log Widget

```hcl
object "log" {
  id = "system-log"
  ref = "/logs/system"
}
```

##### Tree Widget

```hcl
object "tree" {
  id = "data-tree"
  width_min = 200
  height_min = 300
  show_header = true
  show_grid = true
}
```

### Plugins

Plugins extend Slate's functionality with additional data sources and controls:

```hcl
plugin "arduino-sensor" {
  type = "arduino-udp"

  object "plugin-control" {
    id = "arduino-controller"
    parent = "sensor-container"
    port = 3000
    enabled = true
    refs = [
      "/arduino/sensor01",
      "/arduino/sensor02",
      "/arduino/sensor03"
    ]
  }
}
```

#### Plugin Properties

- Plugin name (string): Unique identifier for the plugin instance
- `type` (string, required): Plugin type identifier
- `object "plugin-control"`: Plugin-specific configuration

**Plugin Control Properties:**

- `id` (string, required): Unique identifier
- `parent` (string): Parent container ID
- `port` (integer): Network port (for network plugins)
- `enabled` (boolean): Whether the plugin is active
- `refs` (array of strings): Data reference paths managed by this plugin

## Data References

Data references (`ref` properties) use a hierarchical path system to identify data sources:

- `/sensors/temperature01` - Temperature sensor data
- `/logs/system` - System log messages
- `/arduino/analog_input_00` - Arduino analog input channel 0

The exact reference format depends on your data sources and plugin configuration.

## Configuration File Location

Slate looks for configuration files in the following locations:

1. `~/.config/slate/config.hcl` (user-specific)
2. `/etc/slate/config.hcl` (system-wide)
3. File specified via command line argument

## Validation and Debugging

Slate validates your configuration on startup and will report any errors. Common issues include:

- **Missing required properties**: Ensure all required fields are specified
- **Invalid references**: Check that `ref` paths point to valid data sources
- **Duplicate IDs**: All `id` values must be unique within the configuration
- **Invalid property values**: Check data types and allowed values

Use the `--validate-config` command line option to check your configuration without starting the application:

```bash
slate --validate-config /path/to/config.hcl
```

## Example Configurations

See the `examples/` directory for complete configuration examples:

- `examples/sample-config.hcl` - Basic dashboard with charts and controls
- `examples/sensor-dashboard.hcl` - Environmental sensor monitoring
- `examples/industrial-hmi.hcl` - Industrial control interface

## Migration from XML

If you're migrating from an XML-based configuration system, the HCL format provides equivalent functionality with improved readability and flexibility. The main differences are:

- Attributes become properties with `=` assignment
- Nested elements become nested blocks with `{}`
- More flexible data types and expressions
- Better support for comments and documentation
