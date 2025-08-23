using Adw;
using Gtk;

public class Preferences : Object {
    private Adw.PreferencesDialog dialog;
    private SlateSettings settings;

    // UI elements
    private Adw.SwitchRow auto_open_last_project_row;
    private Adw.SpinRow recent_projects_limit_row;
    private Adw.SwitchRow show_project_actions_row;
    private Adw.ComboRow header_bar_style_row;
    private Adw.SwitchRow show_debug_info_row;
    private Adw.ComboRow log_level_row;

    public Preferences(Gtk.Window? active_window) {
        settings = SlateSettings.get_default();

        var builder = new Gtk.Builder.from_resource("/org/gnome/Slate/ui/preferences.ui");
        dialog = (Adw.PreferencesDialog) builder.get_object("preferences_dialog");

        // Get UI elements
        auto_open_last_project_row = (Adw.SwitchRow) builder.get_object("auto_open_last_project_row");
        recent_projects_limit_row = (Adw.SpinRow) builder.get_object("recent_projects_limit_row");
        show_project_actions_row = (Adw.SwitchRow) builder.get_object("show_project_actions_row");
        header_bar_style_row = (Adw.ComboRow) builder.get_object("header_bar_style_row");
        show_debug_info_row = (Adw.SwitchRow) builder.get_object("show_debug_info_row");
        log_level_row = (Adw.ComboRow) builder.get_object("log_level_row");

        setup_bindings();
        load_current_values();

        // Present the preferences dialog
        dialog.present(active_window);
    }

    private void setup_bindings() {
        // Bind settings to UI elements using GSettings binding
        settings.bind_property("auto-open-last-project", auto_open_last_project_row, "active");
        settings.bind_property("recent-projects-limit", recent_projects_limit_row, "value");
        settings.bind_property("show-project-actions", show_project_actions_row, "active");
        settings.bind_property("show-debug-info", show_debug_info_row, "active");

        // Handle combo rows manually since they need index mapping
        header_bar_style_row.notify["selected"].connect(on_header_bar_style_changed);
        log_level_row.notify["selected"].connect(on_log_level_changed);
    }

    private void load_current_values() {
        // Set combo row selections based on current settings
        var header_style = settings.header_bar_style;
        switch (header_style) {
            case "default":
                header_bar_style_row.selected = 0;
                break;
            case "compact":
                header_bar_style_row.selected = 1;
                break;
            case "expanded":
                header_bar_style_row.selected = 2;
                break;
        }

        var log_level = settings.log_level;
        switch (log_level) {
            case "error":
                log_level_row.selected = 0;
                break;
            case "warning":
                log_level_row.selected = 1;
                break;
            case "info":
                log_level_row.selected = 2;
                break;
            case "debug":
                log_level_row.selected = 3;
                break;
        }
    }

    private void on_header_bar_style_changed() {
        string[] styles = {"default", "compact", "expanded"};
        var selected = header_bar_style_row.selected;
        if (selected < styles.length) {
            settings.header_bar_style = styles[selected];
        }
    }

    private void on_log_level_changed() {
        string[] levels = {"error", "warning", "info", "debug"};
        var selected = log_level_row.selected;
        if (selected < levels.length) {
            settings.log_level = levels[selected];
        }
    }
}
