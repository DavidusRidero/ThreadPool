#include <bits/stdc++.h>

// Detaching a thread hands it over to the standard library.
// Can't touch it in any way. Also called Daemon threads,
// after the UNIX daemon processes.

void edit_document (std::string const& filename) {
    open_document_and_display_gui(filename);
    while (!done_editing) {
        user_command cmd = get_user_input();

        if (cmd.type == open_new_document) {
            std::string const new_name = get_filename_from_user();
            std::thread thread(edit_document,new_name);
            thread.detach();
        }
        else
            process_user_input();
    }
}