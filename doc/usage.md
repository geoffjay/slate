# Slate User Guide

Slate is a configurable dashboard application for GNOME that allows you to create custom monitoring and control interfaces. This guide covers installation, basic usage, and advanced features.

## Installation

### From Source

1. **Prerequisites**

   Ensure you have the required dependencies installed:

   ```bash
   # On Ubuntu/Debian
   sudo apt install build-essential meson ninja-build valac
   sudo apt install libgtk-4-dev libadwaita-1-dev libglib2.0-dev
   sudo apt install libgio2.0-dev libsqlite3-dev libgee-0.8-dev libpeas-2-dev

   # On Fedora
   sudo dnf install meson ninja-build vala
   sudo dnf install gtk4-devel libadwaita-devel glib2-devel
   sudo dnf install gio2-devel sqlite-devel libgee-devel libpeas2-devel
   ```

2. **Build and Install**

   ```bash
   git clone https://github.com/geoffjay/slate.git
   cd slate
   meson setup builddir
   meson compile -C builddir
   sudo meson install -C builddir
   ```

3. **Build Documentation** (Optional)

   ```bash
   meson setup builddir -Dgtk_doc=true
   meson compile -C builddir
   ```

### From Package Manager

_Package manager installation will be available in future releases._

## Getting Started

### First Launch

When you first launch Slate, it will create a default configuration directory at `~/.config/slate/`. If no configuration file is found, Slate will start with a minimal default interface.

### Creating Your First Dashboard

1. **Create a Configuration File**

   Create `~/.config/slate/config.hcl` with a basic dashboard:

   ```hcl
   app = "My First Dashboard"
   startup_page = "main"
   dark_theme = false

   page "main" {
     title = "Main Dashboard"

     object "box" {
       id = "main-container"
       orientation = "vertical"
       spacing = 10

       object "box" {
         id = "header"
         orientation = "horizontal"
         spacing = 5

         # Add your widgets here
       }
     }
   }
   ```

2. **Launch Slate**

   ```bash
   slate
   ```

   Or specify a custom configuration file:

   ```bash
   slate --config /path/to/your/config.hcl
   ```

## User Interface

### Main Window

The Slate main window consists of:

- **Header Bar**: Contains the application menu, page navigation, and window controls
- **Content Area**: Displays the current page with your configured widgets
- **Status Bar**: Shows connection status and system information (when enabled)

### Navigation

- **Page Switching**: Use the page switcher in the header bar or keyboard shortcuts
- **Keyboard Shortcuts**:
  - `Ctrl+1`, `Ctrl+2`, etc.: Switch to page by number
  - `Ctrl+,`: Open preferences
  - `Ctrl+Q`: Quit application
  - `F11`: Toggle fullscreen mode

### Application Menu

Access the application menu from the header bar to:

- Open preferences
- Reload configuration
- View about information
- Access help documentation

## Working with Widgets

### Charts and Graphs

Slate supports various chart types for data visualization:

#### Strip Charts

- Real-time line graphs for continuous data
- Configurable axes, colors, and update rates
- Support for multiple traces per chart

#### Polar Charts

- Circular plots for directional or cyclic data
- Ideal for compass headings, wind direction, etc.

### Control Widgets

#### Analog Input Displays

- Show current values from analog sensors
- Configurable units and formatting
- Real-time updates

#### Log Viewers

- Display system logs or application messages
- Filtering and search capabilities
- Automatic scrolling and archiving

#### Tree Views

- Hierarchical data display
- Expandable/collapsible nodes
- Custom column configurations

### Layout Containers

#### Boxes

- Horizontal and vertical layout containers
- Configurable spacing and margins
- Support for nested layouts

## Data Sources and Plugins

### Built-in Data Sources

Slate includes several built-in data source types:

- **File-based**: Read data from text files or CSV
- **Network**: TCP/UDP socket connections
- **System**: CPU, memory, and system metrics
- **Database**: SQLite and other database connections

### Plugin System

Extend Slate's capabilities with plugins:

1. **Arduino UDP Plugin**: Connect to Arduino devices via UDP
2. **Modbus Plugin**: Industrial Modbus RTU/TCP communication
3. **MQTT Plugin**: IoT device communication via MQTT
4. **Custom Plugins**: Develop your own using the plugin API

### Configuring Data Sources

Data sources are configured in your HCL configuration file:

```hcl
plugin "my-arduino" {
  type = "arduino-udp"

  object "plugin-control" {
    id = "arduino-controller"
    port = 3000
    enabled = true
    refs = ["/arduino/temp", "/arduino/humidity"]
  }
}
```

## Preferences and Settings

### Application Preferences

Access preferences via `Ctrl+,` or the application menu:

- **Appearance**: Theme selection, font sizes
- **Behavior**: Auto-save, update intervals
- **Data Sources**: Connection settings, timeouts
- **Plugins**: Enable/disable plugins, plugin settings

### Per-Dashboard Settings

Some settings can be configured per dashboard in the HCL file:

```hcl
app = "Industrial Monitor"
dark_theme = true
log_level = "warning:3"
auto_save_interval = 300  # seconds
```

## Advanced Usage

### Multiple Configurations

Manage multiple dashboard configurations:

```bash
# Use different config files
slate --config ~/dashboards/production.hcl
slate --config ~/dashboards/testing.hcl

# Set via environment variable
export SLATE_CONFIG=~/dashboards/production.hcl
slate
```

### Fullscreen Mode

Run Slate in fullscreen mode for dedicated display systems:

```bash
slate --fullscreen
```

Or toggle fullscreen with `F11` during runtime.

### Command Line Options

```bash
slate [OPTIONS] [CONFIG_FILE]

Options:
  -c, --config FILE       Use specific configuration file
  -f, --fullscreen        Start in fullscreen mode
  -v, --validate-config   Validate configuration and exit
  -d, --debug             Enable debug logging
  -h, --help              Show help information
  --version               Show version information
```

### Logging and Debugging

Enable detailed logging for troubleshooting:

```bash
# Enable debug logging
slate --debug

# Set log level in configuration
log_level = "*:debug"

# View logs
journalctl --user -f -u slate
```

## Troubleshooting

### Common Issues

#### Configuration Errors

- **Syntax errors**: Check HCL syntax, missing quotes or braces
- **Invalid references**: Ensure data reference paths are correct
- **Duplicate IDs**: All widget IDs must be unique

#### Connection Problems

- **Plugin not loading**: Check plugin dependencies and permissions
- **Data not updating**: Verify data source connections and network settings
- **Permission denied**: Ensure Slate has access to required resources

#### Performance Issues

- **High CPU usage**: Reduce chart update rates, limit data points
- **Memory leaks**: Check for circular references in configuration
- **Slow startup**: Reduce number of plugins or data sources

### Getting Help

1. **Documentation**: Check the built-in help system (`F1`)
2. **Configuration validation**: Use `slate --validate-config`
3. **Debug logging**: Enable with `--debug` flag
4. **Community**: Visit the project repository for issues and discussions

### Reporting Bugs

When reporting bugs, please include:

- Slate version (`slate --version`)
- Operating system and version
- Configuration file (sanitized)
- Steps to reproduce
- Error messages or logs

## Integration Examples

### Home Automation

Monitor and control smart home devices:

```hcl
page "home" {
  title = "Home Automation"

  object "box" {
    id = "sensors"
    orientation = "horizontal"

    object "ai" {
      id = "temperature"
      ref = "/home/living_room/temperature"
      title = "Living Room"
      units = "°C"
    }

    object "ai" {
      id = "humidity"
      ref = "/home/living_room/humidity"
      title = "Humidity"
      units = "%"
    }
  }
}
```

### Industrial Monitoring

Create industrial control interfaces:

```hcl
page "production" {
  title = "Production Line"

  object "stripchart" {
    id = "pressure-chart"
    title = "System Pressure"

    object "chart-axis" {
      id = "pressure-axis"
      label = "Pressure (PSI)"
      min = 0.0
      max = 150.0

      object "trace" {
        id = "line1-pressure"
        ref = "/plc/line1/pressure"
        color = "red"
      }
    }
  }
}
```

### Environmental Monitoring

Track environmental conditions:

```hcl
plugin "weather-station" {
  type = "arduino-udp"

  object "plugin-control" {
    id = "weather-controller"
    port = 8080
    refs = [
      "/weather/temperature",
      "/weather/humidity",
      "/weather/wind_speed",
      "/weather/wind_direction"
    ]
  }
}
```

This comprehensive guide should help you get started with Slate and create effective dashboard applications for your monitoring and control needs.
