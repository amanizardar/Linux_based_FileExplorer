# Linux File Explorer.
### CAPABLE OF DOING FILE OPERATION AND EXPLORATION. RUNS IN TWO MODES :-

    1.NORMAL MODE
    2.COMMAND MODE

***

                             
##Assumptions:-

\~ means Home, Means the Dir from where we started our application/File_Explorer.
. Means Currnt working Directory.
/ Gives path from the root of the system.

Before each command in command mode :- 
Press q to exit, 
esc to goto normal mode , 
Enter to type the commands.


##Normal Mode 

This is the default mode of our application.
it works in non-canonical mode.
for each of the dir. it prints all the contents like files name, file size ,Ownership and Last modified time.
Handles the vertical overflow by k and l keys.
on Pressing Enter on an Entry if it is Directory then its content will be displayed or if it is a file then in a new window that file will be opened in default application.

Left arrow - it will take or display the content of the previous directory if exists.
Right arrow - it will take or display the content of the Next directory if exists.
up arrow - Navigate up.
Down arrow - Navigate Down.
Backspace - Go to the parent Directory.
Home/h - Go to the home dir/(/home/user).
q - Exit the application.
':' - Enter in the Command Mode.


##Command Mode 

This mode is the Second Mode of our Application.
Works in Canonical mode.
Before Each command :-
Press q to Exit.
Press Esc to return to the Normal Mode
Press Enter To Type the commands.


### These are the commands allowed :-

- search <file/dir Path>
- goto <file/dir Path>
- create_file
- create_dir <dir. name>
- rename <old file/dir name> <new file/dir name>
- move <source_file(s)/dir(s) path> <destination_directory>
- copy <source_file(s)/dir(s) path> <destination_directory>
- delete_file
- delete_dir
- mydir //Extra cmd to print cwd



