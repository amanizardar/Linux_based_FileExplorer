Aos Assignment 1 Submiision by
Aman Izardar
2021201028
aman.izardar@students.iiit.ac.in





Assumptions:-
~ means Home, Means the Dir from where we started our application/File_Explorer.
h 
. Means Currnt working Directory.
/ Gives path from the root of the system.
Before each command in command mode :- Press q to exit, esc to goto normal mode , Enter to type the commands.






Normal Mode :-


This is the default mode of our application.
it works in non-canonical mode.
for each of the dir. it prints all the contents like files name, file size ,Ownership and Last modified time.
Handles the vertical overflow by k and l keys.
on Pressing Enter on an Entry if it is Directory then its content will be displayed or if it is a file then in a new window that file will be opened in default application.

Left arrow   - it will take or display the content of the previous directory if exists.
Right arrow  - it will take or display the content of the Next directory if exists.
up arrow     - Navigate up.
Down arrow   - Navigate Down.
Backspace    - Go to the parent Directory.
Home/h       - Go to the home dir/(/home/user).
q            - Exit the application.
':'          - Enter in the Command Mode.





Command Mode :-




This mode is the Second Mode of our Application.
Works in Canonical mode.

Before Each command :-
Press q to Exit.
Press Esc to return to the Normal Mode
Press Enter To Type the commands.



These are the commands allowed :-

 1. search <file/dir Path>
 2. goto <file/dir Path>
 3. create_file <filename> <Destination dir path>
 4. create_dir <dir. name> <destination path>
 5. rename <old file/dir name> <new file/dir name>
 6. move <source_file(s)/dir(s) path> <destination_directory>
 7. copy <source_file(s)/dir(s) path> <destination_directory>
 8. delete_file <file path>
 9. delete_dir <dir path>
10. mydir //Extra cmd to print cwd

