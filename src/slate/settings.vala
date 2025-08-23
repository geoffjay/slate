using GLib;

/**
 * SlateSettings manages application settings using GSettings
 */
public class SlateSettings : Object {
    private static SlateSettings? instance = null;
    private Settings settings;

    // Window state properties
    public int window_width {
        get { return settings.get_int("window-width"); }
        set { settings.set_int("window-width", value); }
    }

    public int window_height {
        get { return settings.get_int("window-height"); }
        set { settings.set_int("window-height", value); }
    }

    public bool window_maximized {
        get { return settings.get_boolean("window-maximized"); }
        set { settings.set_boolean("window-maximized", value); }
    }

        // Project settings
    public string last_project_path {
        owned get { return settings.get_string("last-project-path"); }
        set { settings.set_string("last-project-path", value); }
    }

    public bool auto_open_last_project {
        get { return settings.get_boolean("auto-open-last-project"); }
        set { settings.set_boolean("auto-open-last-project", value); }
    }

    public int recent_projects_limit {
        get { return settings.get_int("recent-projects-limit"); }
        set { settings.set_int("recent-projects-limit", value); }
    }

    // UI preferences
    public bool show_project_actions {
        get { return settings.get_boolean("show-project-actions"); }
        set { settings.set_boolean("show-project-actions", value); }
    }

        public string header_bar_style {
        owned get { return settings.get_string("header-bar-style"); }
        set { settings.set_string("header-bar-style", value); }
    }

        // Plugin settings
    public string[] enabled_plugins {
        owned get { return settings.get_strv("enabled-plugins"); }
        set { settings.set_strv("enabled-plugins", value); }
    }

    public string[] plugin_search_paths {
        owned get { return settings.get_strv("plugin-search-paths"); }
        set { settings.set_strv("plugin-search-paths", value); }
    }

    // Development settings
    public bool show_debug_info {
        get { return settings.get_boolean("show-debug-info"); }
        set { settings.set_boolean("show-debug-info", value); }
    }

        public string log_level {
        owned get { return settings.get_string("log-level"); }
        set { settings.set_string("log-level", value); }
    }

    private SlateSettings() {
        settings = new Settings("org.gnome.Slate");
    }

    public static SlateSettings get_default() {
        if (instance == null) {
            instance = new SlateSettings();
        }
        return instance;
    }

    /**
     * Connect to a setting change signal
     */
    public void connect_changed(string key, owned SourceFunc callback) {
        settings.changed[key].connect(() => {
            callback();
        });
    }

    /**
     * Bind a setting to a GObject property
     */
    public new void bind_property(string key, Object object, string property, SettingsBindFlags flags = SettingsBindFlags.DEFAULT) {
        settings.bind(key, object, property, flags);
    }

    /**
     * Reset all settings to defaults
     */
    public void reset_all() {
        var keys = settings.settings_schema.list_keys();
        foreach (string key in keys) {
            settings.reset(key);
        }
    }

    /**
     * Reset a specific setting to default
     */
    public void reset_key(string key) {
        settings.reset(key);
    }
}
