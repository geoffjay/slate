using GLib;

public errordomain DbError {
    FAILED
}

public class SlateDb : Object {
    private Sqlite.Database connection;
    private string database_path;

    public static SlateDb? instance = null;

    public static SlateDb get_default() {
        if (instance == null) {
            instance = new SlateDb();
        }
        return instance;
    }

    private SlateDb() {
        var data_dir = Path.build_filename(Environment.get_user_data_dir(), "org.gnome.Slate");
        if (DirUtils.create_with_parents(data_dir, 493) != 0) {
            warning("Failed creating data dir: %s", data_dir);
        }
        database_path = Path.build_filename(data_dir, "slate.db");
    }

    public void open_and_migrate() throws Error {
        int rc = Sqlite.Database.open(database_path, out connection);
        if (rc != Sqlite.OK) {
            throw new DbError.FAILED("Failed to open database: %d", rc);
        }

        // Ensure foreign keys are on
        exec("PRAGMA foreign_keys = ON;");

        // Create migrations table if missing
        exec("CREATE TABLE IF NOT EXISTS schema_migrations (version INTEGER PRIMARY KEY);");

        // Run migrations
        var migrations = get_migrations();
        foreach (var migration in migrations) {
            if (!is_version_applied(migration.version)) {
                debug("Applying migration %d", migration.version);
                exec(migration.sql);
                exec("INSERT INTO schema_migrations (version) VALUES (%d);".printf(migration.version));
            }
        }
    }

    private class Migration : Object {
        public int version;
        public string sql;
        public Migration(int version, string sql) {
            this.version = version;
            this.sql = sql;
        }
    }

    private Migration[] get_migrations() {
        var list = new Gee.ArrayList<Migration>();
        // v1: projects table
        list.add(new Migration(1, "\nBEGIN;\n\nCREATE TABLE IF NOT EXISTS projects (\n  id INTEGER PRIMARY KEY AUTOINCREMENT,\n  name TEXT NOT NULL,\n  path TEXT NOT NULL UNIQUE,\n  created_at TEXT NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')),\n  updated_at TEXT NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now'))\n);\n\nCREATE INDEX IF NOT EXISTS idx_projects_path ON projects(path);\n\nCOMMIT;\n"));
        return (Migration[]) list.to_array();
    }

    private bool is_version_applied(int version) {
        Sqlite.Statement stmt;
        int rc = connection.prepare_v2("SELECT 1 FROM schema_migrations WHERE version = ?1 LIMIT 1;", -1, out stmt);
        if (rc != Sqlite.OK) {
            warning("prepare failed: %d", rc);
            return false;
        }
        stmt.bind_int(1, version);
        rc = stmt.step();
        bool applied = (rc == Sqlite.ROW);
        return applied;
    }

    public void exec(string sql) throws Error {
        int rc = connection.exec(sql);
        if (rc != Sqlite.OK) {
            throw new DbError.FAILED("SQL exec error: %d", rc);
        }
    }

    public Sqlite.Statement prepare(string sql) throws Error {
        Sqlite.Statement stmt;
        int rc = connection.prepare_v2(sql, -1, out stmt);
        if (rc != Sqlite.OK) {
            throw new DbError.FAILED("SQL prepare error: %d", rc);
        }
        return stmt;
    }
}

public class Project : Object {
    public int id { get; set; default = 0; }
    public string name { get; set; default = ""; }
    public string path { get; set; default = ""; }

    public static Gee.ArrayList<Project> all() throws Error {
        var db = SlateDb.get_default();
        var projects = new Gee.ArrayList<Project>();
        var stmt = db.prepare("SELECT id, name, path FROM projects ORDER BY name ASC;");
        while (stmt.step() == Sqlite.ROW) {
            var p = new Project();
            p.id = stmt.column_int(0);
            p.name = (string) stmt.column_text(1);
            p.path = (string) stmt.column_text(2);
            projects.add(p);
        }
        return projects;
    }

    public static Project create(string name, string path) throws Error {
        var db = SlateDb.get_default();
        var stmt = db.prepare("INSERT INTO projects (name, path) VALUES (?1, ?2);");
        stmt.bind_text(1, name);
        stmt.bind_text(2, path);
        var rc = stmt.step();
        if (rc != Sqlite.DONE) {
            throw new DbError.FAILED("Insert failed: %d", rc);
        }
        var p = new Project();
        p.id = (int) last_insert_rowid();
        p.name = name;
        p.path = path;
        return p;
    }

    public static Project? find_by_path(string path) throws Error {
        var db = SlateDb.get_default();
        var stmt = db.prepare("SELECT id, name, path FROM projects WHERE path = ?1 LIMIT 1;");
        stmt.bind_text(1, path);
        Project? p = null;
        if (stmt.step() == Sqlite.ROW) {
            p = new Project();
            p.id = stmt.column_int(0);
            p.name = (string) stmt.column_text(1);
            p.path = (string) stmt.column_text(2);
        }
        return p;
    }

    private static int64 last_insert_rowid() {
        // Access underlying handle via a quick SELECT last_insert_rowid();
        var db = SlateDb.get_default();
        try {
            var stmt = db.prepare("SELECT last_insert_rowid();");
            int64 id = 0;
            if (stmt.step() == Sqlite.ROW) {
                id = stmt.column_int64(0);
            }
            return id;
        } catch (Error e) {
            warning("Failed to get last insert rowid: %s", e.message);
            return 0;
        }
    }
}

