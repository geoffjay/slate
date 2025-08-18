using Adw;
using Gtk;

public class Preferences : Adw.PreferencesDialog {
    public Preferences(Gtk.Window? active_window) {
        Object(title: "Preferences");

        var builder = new Gtk.Builder.from_resource("/org/gnome/Slate/ui/preferences.ui");
        var prefs = (Adw.PreferencesDialog) builder.get_object("preferences_dialog");

        // Present the preferences dialog, associated with the active window if any.
        prefs.present(active_window);
    }
}
