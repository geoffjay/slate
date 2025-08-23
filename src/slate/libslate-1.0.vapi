[CCode (cheader_filename = "slate.h")]
namespace Slate {
    [CCode (cname = "SlateHeaderBar", type_id = "slate_header_bar_get_type ()")]
    public class HeaderBar : Gtk.Widget {
        [CCode (cname = "slate_header_bar_new")]
        public HeaderBar ();

        [CCode (cname = "slate_header_bar_set_project_title")]
        public void set_project_title (string? title);

        [CCode (cname = "slate_header_bar_get_project_title")]
        public unowned string? get_project_title ();

        [CCode (cname = "slate_header_bar_set_project_subtitle")]
        public void set_project_subtitle (string? subtitle);

        [CCode (cname = "slate_header_bar_get_project_subtitle")]
        public unowned string? get_project_subtitle ();

        [CCode (cname = "slate_header_bar_add_start_widget")]
        public void add_start_widget (Gtk.Widget widget);

        [CCode (cname = "slate_header_bar_add_end_widget")]
        public void add_end_widget (Gtk.Widget widget);

        [CCode (cname = "slate_header_bar_remove_widget")]
        public void remove_widget (Gtk.Widget widget);

        [CCode (cname = "slate_header_bar_set_show_project_actions")]
        public void set_show_project_actions (bool show);

        [CCode (cname = "slate_header_bar_get_show_project_actions")]
        public bool get_show_project_actions ();

        public string? project_title { get; set; }
        public string? project_subtitle { get; set; }
        public bool show_project_actions { get; set; }

        public signal void close_project_requested ();
    }
}
