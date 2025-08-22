# Sample Slate Configuration in HCL
# This demonstrates the equivalent of the XML configuration from dactl

# Application properties
app = "Sample"
admin = false
startup_page = "pg0"
dark_theme = true
log_level = "*:5"

# Main dashboard page
page "pg0" {
  title = "Main Dashboard"

  object "box" {
    id = "box0"
    orientation = "horizontal"
    spacing = 5

    object "box" {
      id = "box0-0"
      homogeneous = false
      orientation = "vertical"
      spacing = 5
      expand = false
      fill = false

      object "log" {
        id = "log-ctl0"
        ref = "/logctl0/log0"
      }

      object "ai" {
        id = "ai-ctl0"
        ref = "/daqctl0/dev0/ai00"
      }

      object "tree" {
        id = "tree0"
        width_min = 200
        height_min = 200
        show_header = true
        show_grid = true
      }
    }

    object "box" {
      id = "box0-1"
      homogeneous = true
      orientation = "vertical"
      margin_top = 5
      margin_right = 5
      margin_bottom = 5
      margin_left = 5

      object "box" {
        id = "box0-1-0"
        homogeneous = true
        orientation = "vertical"

        object "stripchart" {
          id = "chart0"
          title = "Liquid Flow"
          height_min = 100
          width_min = 100
          show_grid = true
          show_title = true
          points_per_second = 10

          object "chart-axis" {
            id = "ax0"
            label = "Flow Rate (L/min)"
            min = 0.0
            max = 100.0

            object "trace" {
              id = "tr0"
              ref = "/daqctl0/dev0/ai00"
              color = "blue"
              line_width = 2
            }
          }
        }
      }
    }
  }
}

# Second page example
page "pg1" {
  title = "Heat Map"

  object "box" {
    id = "box0"
    orientation = "vertical"
    spacing = 5

    object "polar-chart" {
      id = "pg1chart0"
      title = "Bin Heat Map"
      refresh_ms = 30
      height_min = 100
      width_min = 100
      show_grid = true
      show_grid_border = true
      show_title = true

      object "polar-axis" {
        id = "ax0"
        label = "Angle"
        min = 0.0
        max = 360.0

        object "trace" {
          id = "tr0"
          ref = "/daqctl0/dev0/ai00"
          color = "red"
        }
      }
    }
  }
}

# Plugin configuration
plugin "arduino-udp0" {
  type = "arduino-udp"

  object "plugin-control" {
    id = "arduino-udp-ctl00"
    parent = "box0-0-2"
    port = 3000
    enabled = true
    refs = [
      "/udp00",
      "/udp01",
      "/udp02",
      "/udp03",
      "/udp04",
      "/udp05",
      "/udp06",
      "/udp07"
    ]
  }
}
