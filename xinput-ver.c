#include <glib.h>
#include <gio/gio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
    int return_code = 0;
    GFileIOStream * stream;
    GError * err = NULL;

    // create a new temporary file to record output from xinput
    GFile * tmp_file = g_file_new_tmp(NULL, &stream, &err);
    if (err) {
        fprintf(stderr, "Unable to create temp file for communication with "
            "xinput: %s\n", err->message);
        g_error_free(err);
        return -1;
    }

    // get the filename and path (UTF-8) to redirect xinput info to
    char * filename = g_file_get_path(tmp_file);
    if (!filename) {
        fprintf(stderr, "Unable to get xinput temp filename\n");
        goto file_return;
    }

    // build the xinput command string using the temp filename
    size_t buf_len = strlen("xinput --version >> ");
    buf_len += strlen(filename);
    buf_len++; // null terminator
    char * command_buf = malloc(buf_len);
    if (!command_buf) {
        fprintf(stderr, "Malloc returned NULL at " __FILE__ ":%d\n", __LINE__);
        return_code = -1;
        goto filename_return;
    }
    errno = 0;
    snprintf(command_buf, buf_len, "xinput --version >> %s", filename);
    if (errno) {
        fprintf(stderr, "snprintf failed at " __FILE__ ":%d\n", __LINE__);
        return_code = -1;
        goto command_buf_return;
    }

    // launch xinput
    errno = 0;
    int ret = system((const char *)command_buf);
    if (errno) {
        fprintf(stderr, "failed to launch xinput, aborting\n");
        return_code = -1;
        goto command_buf_return;
    }

    // figure out how much output xinput produced from tmp_file filesize
    GFileInfo * info = g_file_query_info(tmp_file, G_FILE_ATTRIBUTE_STANDARD_SIZE, 
        G_FILE_QUERY_INFO_NONE, NULL, &err);
    if (err) {
        fprintf(stderr, "Unable to get temp file size: %s\n",
            err->message);
        return_code = -1;
        g_clear_error(&err);
        goto command_buf_return;
    }
    guint64 file_size = g_file_info_get_attribute_uint64(info,
        G_FILE_ATTRIBUTE_STANDARD_SIZE);
    g_object_unref(info);

    // read in xinput outputs file to a buffer
    char * output_buffer = malloc(file_size+1);
    if (!output_buffer) {
        fprintf(stderr, "Malloc returned NULL at " __FILE__ ":%d\n", __LINE__);
        return_code = -1;
        goto command_buf_return;
    }
    GInputStream * in_stream = g_io_stream_get_input_stream(G_IO_STREAM(stream));
    g_input_stream_read(in_stream, output_buffer, file_size, NULL, &err);
    if (err) {
        fprintf(stderr, "Unable to read xinput output from tmp file: %s\n",
            err->message);
        return_code = -1;
        g_clear_error(&err);
        goto output_buf_return;
    }
    output_buffer[file_size] = '\0';
    
    // print the output to standard out
    printf("\x1b[1m%s\x1b[0m", output_buffer);

stream_return:
    g_object_unref(stream);
output_buf_return:
    free(output_buffer);
command_buf_return:
    free(command_buf);
filename_return:
    g_free(filename);
file_return:
    g_file_delete(tmp_file, NULL, NULL);
    g_object_unref(tmp_file);
    return return_code;
}
