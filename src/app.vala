public class Application {
    public static int main(string[] argv) {
        Adw.init();

        var app = new Adw.Application("org.slate.app", ApplicationFlags.DEFAULT_FLAGS);

        app.activate.connect(() => {
            var builder = new Gtk.Builder.from_resource("/org/slate/app/ui/main.ui");
            var window = (Gtk.ApplicationWindow) builder.get_object("main_window");
            var close_button = (Gtk.Button) builder.get_object("close_button");
            close_button.clicked.connect(() => window.close());
            window.set_application(app);
            window.present();
        });

        return app.run(argv);
    }
}
