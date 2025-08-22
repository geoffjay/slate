# Concept

Slate is an application framework for building desktop applications. It is
designed to be simple to use, configurable, and highly extensible.

## History

Slate is the spiritual successor to [Dactl](https://github.com/coanda/dactl).
Dactl was a GTK3/Vala application framework that was designed to be simple to
implement new measurement and control applications. It used a custom XML-based
configuration format, a custom widget library, and a custom event system.
Plugins could be developed using either C or Vala and loaded using a custom
plugin system. Over time, Dactl became more difficult to maintain and lacked
vital features such as testing and a plugin system that supported languages
other than C and Vala. Another drawback of Dactl was that it was only tested on
Linux, and had several dependencies that were not available on other platforms.

## Goals

- Simple to use
- Very well documented
- Extensible using a plugin system that exposes all functionality
- Fully tested
- Improved configuration format using HCL
- Support for macOS and Windows
- CI/CD pipeline using GitHub Actions

## Design

Slate uses Gtk4 and the Adwaita library for the UI. The main application is
written in Vala and uses the GObject type system, while libraries should be
written in C and also use the GObject type system. The application is designed
to be extensible using a plugin system that exposes all functionality, this will
be accomplished using the libpeas library.

### Application

These are the main components of the application, this is not an exhaustive list
of all the components that will be used in the application. It is here as a
starting point to define the application architecture.

#### Header Bar

The header bar is the top bar of the application. It contains the application
title, a close project button, and any critical actions that should be available
to the user.

#### Slideout Sidebar

The sidebar is the right sidebar of the application and contains information
about selected and focused objects. It should be implemented as a slideout panel
that can be opened and closed.

#### Preferences & Settings

The preferences and settings dialogs should be implemented as a dialog
that can be opened and closed.

#### Main Content Area

The main content area is the center of the application and contains the main
content of the application. This content should be implemented as a stack of
widgets that can be switched between. This will support pages for different
purposes such as:

- Configuration builder and editor
- Dashboard for rendering configuration projects
- Plugins
- Help and documentation

##### Dashboard

The dashboard is the main content area of the application and contains the
dashboard for rendering configuration projects. It should be implemented as a
stack of widgets that can be switched between.

#### Configuration Builder and Editor

The configuration builder and editor is the main content area of the application
and contains the configuration builder and editor. It should allow a user to
edit the configuration of the project in a `GtkSourceView` widget, as well as
graphically edit the configuration of the project in a `GtkGraph` widget. The
graphical editor should be able to represent the configuration as a graph of
nodes and edges, where each node represents a configuration option and each edge
represents a dependency between configuration options. Selecting a node should
open or focus the sidebar with the configuration options for that node.
Selecting an edge should open or focus the sidebar with the configuration
options for that edge.

#### Status Bar

The status bar is the bottom bar of the application and contains information
about the current state of the application.

#### Menu

The menu is the top bar of the application and contains the application menu.
It should be implemented as a menu that can be opened and closed on all
operating system platforms. The menu should be implemented as a tree of items
that can be expanded and collapsed.

### Plugin System

The plugin system should expose the following functionality:

#### Header Bar

Plugins should be able to modify the title, subtitle, and icons in the header
bar. They should also be able to hide the header bar if desired.

#### Slideout Sidebar

Plugins should be able to add items to the slideout sidebar.

#### Preferences & Settings

Plugins should be able to add items to the preferences and settings dialogs.

#### Main Content Area

Plugins should be able to add pages to the main content area.

##### Dashboard

Plugins should be able to provide UI components that can be used in the dashboard.

#### Status Bar

Plugins should be able to add items to the status bar.

#### Menu

Plugins should be able to add items to the menu.

## Configuration

Slate uses HCL for the configuration format. HCL is a simple, human-readable,
and flexible configuration language. It is designed to be easy to read and
write.

## Differences from Dactl

- Slate uses Gtk4 and the Adwaita library for the UI.
- Slate uses the libpeas library for the plugin system.
- Slate uses HCL for the configuration format.
- Dactl focused on data acquisition and control, Slate will be more general and use "measurement" and "control" as synonyms. It will avoid "analog" and "digital", for example `DactlAiControl` will be created as `SlateMeasurementControl` or possibly `SlateUiMeasurement`.
