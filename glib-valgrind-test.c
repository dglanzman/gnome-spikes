#include <glib.h>
#include <gio/gio.h>

int main() {
    GFileIOStream * stream;
    GError * err = NULL;
    GFile * tmp_file = g_file_new_tmp(NULL, &stream, &err);
    g_clear_error(&err);

    g_object_unref(stream);
    g_file_delete(tmp_file, NULL, NULL);
    g_object_unref(tmp_file);
    return 0;
}
