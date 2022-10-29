# Linux File Explorer.
Developed a terminal-based file explorer in non-canonical mode with a limited feature set. It provides normal mode and command mode.In normal mode, the application shows current directory contents. Users can navigate in directories using the arrow key and scrolling keys, also has access to forward-history, backward history, and parent directory, and can resize the window size.In command mode, the user can copy, create, delete, search, etc. by providing path and files/directories names. Commands are implemented through appropriate system calls.Technologies used : C++, POSIX System calls.

### CAPABLE OF DOING FILE OPERATION AND EXPLORATION. RUNS IN TWO MODES :-

    1.NORMAL MODE
    2.COMMAND MODE

***

                             
## Assumptions:-

\~ means Home, Means the Dir from where we started our application/File_Explorer.<br />
. Means Currnt working Directory.<br />
/ Gives path from the root of the system.<br />

Before each command in command mode :- <br />
Press q to exit, <br />
esc to goto normal mode , <br />
Enter to type the commands.<br />


## Normal Mode 

This is the default mode of our application.<br />
it works in non-canonical mode.<br />
for each of the dir. it prints all the contents like files name, file size ,Ownership and Last modified time.<br />
Handles the vertical overflow by k and l keys.<br />
on Pressing Enter on an Entry if it is Directory then its content will be displayed or if it is a file then in a new window that file will be opened in default application.<br />
<br />
Left arrow - it will take or display the content of the previous directory if exists.<br />
Right arrow - it will take or display the content of the Next directory if exists.<br />
up arrow - Navigate up.<br />
Down arrow - Navigate Down.<br />
Backspace - Go to the parent Directory.<br />
Home/h - Go to the home dir/(/home/user).<br />
q - Exit the application.<br />
':' - Enter in the Command Mode.<br />
<br />

## Command Mode 
This mode is the Second Mode of our Application.<br />
Works in Canonical mode.<br />
Before Each command :-<br />
Press q to Exit.<br />
Press Esc to return to the Normal Mode<br />
Press Enter To Type the commands.<br />


### These are the commands allowed :-<br />

- search <file/dir Path><br />
- goto <file/dir Path><br />
- create_file<br />
- create_dir <dir. name><br />
- rename <old file/dir name> <new file/dir name><br />
- move <source_file(s)/dir(s) path> <destination_directory><br />
- copy <source_file(s)/dir(s) path> <destination_directory><br />
- delete_file<br />
- delete_dir<br />
- mydir //Extra cmd to print cwd<br />



