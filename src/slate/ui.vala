using Adw;
using Gtk;
using GLib;

public delegate void NameCallback(string? name);

public class MainWindowController : Object {
    private Gtk.Application app;
    private Gtk.ApplicationWindow window;
    private Gtk.Box content_box;
    private Gtk.Stack main_stack;
    private Gtk.Stack projects_stack;
    private Gtk.ListBox projects_list;
    private Gtk.Label project_title;
    private Gtk.Label project_path;
    private Gtk.Widget empty_state;
    private Gtk.Widget projects_page;
    private Gtk.Widget dashboard_page;
    private Gtk.Widget projects_scroller;
    private Slate.HeaderBar slate_header_bar;

    public MainWindowController(Gtk.Application app) {
        this.app = app;
        var builder = new Gtk.Builder.from_resource("/org/gnome/Slate/ui/main.ui");
        window = (Gtk.ApplicationWindow) builder.get_object("main_window");
        content_box = (Gtk.Box) builder.get_object("content_box");
        window.set_child(content_box);
        window.set_application(app);

        main_stack = (Gtk.Stack) builder.get_object("main_stack");
        projects_stack = (Gtk.Stack) builder.get_object("projects_stack");
        projects_list = (Gtk.ListBox) builder.get_object("projects_list");
        project_title = (Gtk.Label) builder.get_object("project_title");
        project_path = (Gtk.Label) builder.get_object("project_path");
        empty_state = (Gtk.Widget) builder.get_object("empty_state");
        projects_page = (Gtk.Widget) builder.get_object("projects_page");
        dashboard_page = (Gtk.Widget) builder.get_object("dashboard_page");
        projects_scroller = (Gtk.Widget) builder.get_object("projects_scroller");
        slate_header_bar = (Slate.HeaderBar) builder.get_object("slate_header_bar");

        // Set up project action buttons
        setup_header_bar_actions();

        projects_list.row_activated.connect((row) => {
            var item = row as ProjectRow;
            if (item != null) {
                show_dashboard(item.project);
            }
        });

        refresh_projects();
    }

    private void setup_header_bar_actions() {
        // Create project action buttons
        var create_button = new Gtk.Button.from_icon_name("list-add-symbolic");
        create_button.set_tooltip_text(_("Create Project"));
        create_button.set_action_name("app.create-project");

        var add_button = new Gtk.Button.from_icon_name("document-open-symbolic");
        add_button.set_tooltip_text(_("Add Existing Project"));
        add_button.set_action_name("app.add-project");

        // Add buttons to header bar
        slate_header_bar.add_end_widget(create_button);
        slate_header_bar.add_end_widget(add_button);

        // Connect to close project signal
        slate_header_bar.close_project_requested.connect(() => {
            show_projects();
        });
    }

    public void present() {
        window.present();
    }

    public void refresh_projects() {
        // Clear existing rows - GTK4 ListBox approach
        while (true) {
            var row = projects_list.get_row_at_index(0);
            if (row == null) break;
            projects_list.remove(row);
        }
        try {
            var projects = Project.all();
            if (projects.size == 0) {
                projects_stack.set_visible_child(empty_state);
            } else {
                foreach (var p in projects) {
                    projects_list.append(new ProjectRow(p));
                }
                projects_stack.set_visible_child(projects_scroller);
            }
            show_projects();
        } catch (DbError e) {
            critical("Database error loading projects: %s", e.message);
        } catch (Error e) {
            critical("Failed to load projects: %s", e.message);
        }
    }

    public void show_dashboard(Project project) {
        project_title.set_label(project.name);
        project_path.set_label(project.path);

        // Update header bar for project view
        slate_header_bar.set_project_title(project.name);
        slate_header_bar.set_project_subtitle(project.path);
        slate_header_bar.set_show_project_actions(true);

        main_stack.set_visible_child(dashboard_page);
    }

    public void show_projects() {
        // Update header bar for projects view
        slate_header_bar.set_project_title("Slate");
        slate_header_bar.set_project_subtitle(null);
        slate_header_bar.set_show_project_actions(false);

        main_stack.set_visible_child(projects_page);
    }
}

public class ProjectRow : Gtk.ListBoxRow {
    public Project project { get; private set; }

    public ProjectRow(Project project) {
        Object(selectable: true, activatable: true);

        this.project = project;
        var box = new Gtk.Box(Gtk.Orientation.VERTICAL, 2);
        var title = new Gtk.Label(project.name);
        title.set_xalign(0.0f);
        title.add_css_class("title-4");
        var path = new Gtk.Label(project.path);
        path.set_xalign(0.0f);
        path.add_css_class("dim-label");
        box.append(title);
        box.append(path);
        set_child(box);
    }
}

public class ProjectDialogs : Object {
    public static async string? select_directory(Gtk.Window transient_for) {
        var dialog = new Gtk.FileDialog();
        dialog.set_title("Select Project Folder");
        try {
            var file = yield dialog.select_folder(transient_for, null);
            if (file != null) {
                return file.get_path();
            }
        } catch (Error e) {
            debug("Folder selection cancelled or failed: %s", e.message);
        }
        return null;
    }

    public static void prompt_for_name(Gtk.Window transient_for, string default_name, owned NameCallback on_done) {
        var dialog = new Adw.AlertDialog("Project Name", "Enter a name for your project");
        var entry = new Gtk.Entry();
        entry.set_text(default_name);
        dialog.set_extra_child(entry);
        dialog.add_response("cancel", "Cancel");
        dialog.add_response("ok", "Create");
        dialog.set_default_response("ok");
        dialog.set_close_response("cancel");
        dialog.set_response_appearance("ok", Adw.ResponseAppearance.SUGGESTED);
        dialog.response.connect((id) => {
            string? result = null;
            if (id == "ok") {
                result = entry.get_text();
            }
            on_done(result);
        });
        dialog.present(transient_for);
    }
}

