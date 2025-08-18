using Adw;
using GLib;
using Gtk;

public class Application {
    private static async void create_project_flow(Adw.Application app, MainWindowController controller) {
        var win = app.get_active_window();
        var dir = yield ProjectDialogs.select_directory(win);
        if (dir == null) return;
        var default_name = Path.get_basename(dir);
        ProjectDialogs.prompt_for_name(win, default_name, (name) => {
            if (name == null || name.strip().length == 0) return;
            try {
                var p = Project.create(name.strip(), dir);
                controller.show_dashboard(p);
                controller.refresh_projects();
            } catch (Error e) {
                critical("Failed to create project: %s", e.message);
            }
        });
    }

    private static async void add_project_flow(Adw.Application app, MainWindowController controller) {
        var win = app.get_active_window();
        var dir = yield ProjectDialogs.select_directory(win);
        if (dir == null) return;
        try {
            var existing = Project.find_by_path(dir);
            Project p;
            if (existing != null) {
                p = existing;
            } else {
                var default_name = Path.get_basename(dir);
                p = Project.create(default_name, dir);
            }
            controller.show_dashboard(p);
            controller.refresh_projects();
        } catch (Error e) {
            critical("Failed to add project: %s", e.message);
        }
    }
    public static int main(string[] argv) {
        Adw.init();

        var app = new Adw.Application("org.gnome.Slate", ApplicationFlags.DEFAULT_FLAGS);

        // Add a cross-platform Preferences action. On macOS this will be invoked
        // from the system "Preferences…" menu item; on Linux it is available as
        // the application action and via the accelerator.
        var preferences_action = new GLib.SimpleAction("preferences", null);
        preferences_action.activate.connect((_param) => {
            var active = app.get_active_window();
            new Preferences(active);
        });
        app.add_action(preferences_action);

        // Common shortcut: Cmd+, on macOS and Ctrl+, elsewhere
        string[] prefs_accels = { "<Primary>comma" };
        app.set_accels_for_action("app.preferences", prefs_accels);

        // Provide a menubar entry so macOS shows "Preferences…" in the
        // application menu. Harmless on Linux.
        var menubar = new GLib.Menu();
        menubar.append("Preferences…", "app.preferences");
        app.set_menubar(menubar);

        app.activate.connect(() => {
            try {
                SlateDb.get_default().open_and_migrate();
            } catch (Error e) {
                critical("Database init failed: %s", e.message);
            }

            var controller = new MainWindowController(app);
            controller.present();
            // Bind actions
            var create_action = new GLib.SimpleAction("create-project", null);
            create_action.activate.connect((_p) => {
                create_project_flow.begin(app, controller);
            });
            app.add_action(create_action);

            var add_action = new GLib.SimpleAction("add-project", null);
            add_action.activate.connect((_p) => {
                add_project_flow.begin(app, controller);
            });
            app.add_action(add_action);

            var close_project_action = new GLib.SimpleAction("close-project", null);
            close_project_action.activate.connect((_p) => {
                controller.show_projects();
            });
            app.add_action(close_project_action);
        });

        return app.run(argv);
    }
}
