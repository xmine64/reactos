add_executable(controls
               main.c
               navhost.c
               pagehost.c
               pages/button.c
               pages/edit.c
               pages/combo.c
               pages/icontitle.c
               pages/listbox.c
               pages/mdi.c
               pages/menu.c
               pages/scrollbar.c)

set_module_type(controls win32gui UNICODE)
add_importlibs(controls gdi32 user32 msvcrt kernel32 ntdll)
add_rostests_file(TARGET controls SUBDIR suppl)
