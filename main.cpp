

//Author :-> Aman Izardar
#include <bits/stdc++.h>
#include <filesystem>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define ll long long
#define ff first
#define ss second
#define pb push_back
#define mp make_pair
#define phi 3.1415926536
#define mod 1000000007
using namespace std;

// Function Declaratiom
void Normalmode(string);
void backtocanonical();
void noncanonicalmode();
int getch(void);
void Print_files_caller(int, int);
void Get_files_metadata(string cwd);

//
int screenrows,screencols;
string root;
stack<string> lefts;
stack<string> rights;
vector<struct dirent> curr_files;
vector<string> cmd;

//Code Section : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : :
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define ENTER_KEY 13
#define BACKSPACE 127
#define HOME 72
#define KEY_ESC 27
int curr_cursor = 0;
int tempcur = 1;


// Window Size

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}


// End of Window size



// This is The cmd mode



void refresh()
{
    // cout << "\033[2J\033[1;1H"; // Clearing the screen;
    cout << string(22, '\n'); // Clearing the screen;
    Get_files_metadata(lefts.top());
    int s = 0;
    int e = 0;

    if (curr_files.size() >= 20)
        e = 19;
    else
        e = curr_files.size() - 1;

    
    printf("%c[%d;%df", 0x1B, 0, 0); // cursor at 0,0;
    Print_files_caller(s, e);
    cout << "\033[9999;1H"; //Set Cursor to bottom left
}

bool searchfun(string cwd, string filename)
{

    const char *c = cwd.c_str();
    struct stat s;

    DIR *dir;
    struct dirent *diread;

    if ((dir = opendir(c)) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {

            if (strcmp(diread->d_name, filename.c_str()) == 0)
            {
                return true;
            }

            if (strcmp(diread->d_name, ".") == 0)
            {
                continue;
            }
            if (strcmp(diread->d_name, "..") == 0)
            {
                continue;
            }

            string newdir = cwd + "/" + diread->d_name;
            const char *path = newdir.c_str();

            if (stat(path, &s) == 0)
            {
                if (s.st_mode & S_IFDIR)
                {
                    // string newdir=cwd+"/"+diread->d_name;
                    // cout<<"I'm a Dir  "<<newdir<<"\n";
                    if (searchfun(newdir, filename))
                        return true;
                }
                else
                {
                    // cout<<"I'm a file  "<<diread->d_name<<"\n";
                }
            }
        }
        closedir(dir);
    }
    return false;
}

bool search(string filename)
{
    string cwd = lefts.top();
    return searchfun(cwd, filename);
}

void rename1(string source, string dest)
{
    string sfp;
    string dfp;

    if (source[0] == '/')
    {
        sfp = source;
    }
    else if (source[0] == '~')
    {
        source.erase(0, 1);
        sfp = root + "/" + source;
    }
    else
    {
        if (source[0] == '.')
        {
            source.erase(0, 1);
        }

        sfp = lefts.top() + "/" + source;
    }

    if (dest[0] == '/')
    {
        dfp = dest;
    }
    else if (dest[0] == '~')
    {
        dest.erase(0, 1);
        dfp = root + "/" + dest;
    }
    else
    {
        if (dest[0] == '.')
        {
            dest.erase(0, 1);
        }

        dfp = lefts.top() + "/" + dest;
    }

    const char *oldpath = sfp.c_str();
    const char *newpath = dfp.c_str();

    if (rename(oldpath, newpath) == 0)
    {
        refresh();
        cout << "Renamed Successfully\n";
    }
    else
    {
        cout << "Renamed Failed\n";
    }
}

void move(string source, string dest)
{
    string sfp;
    string dfp;

    if (source[0] == '/')
    {
        sfp = source;
    }
    else if (source[0] == '~')
    {
        source.erase(0, 1);
        sfp = root + "/" + source;
    }
    else
    {
        if (source[0] == '.')
        {
            source.erase(0, 1);
        }

        sfp = lefts.top() + "/" + source;
    }

    if (dest[0] == '/')
    {
        dfp = dest;
    }
    else if (dest[0] == '~')
    {
        dest.erase(0, 1);
        dfp = root + "/" + dest;
    }
    else
    {
        if (dest[0] == '.')
        {
            dest.erase(0, 1);
        }

        dfp = lefts.top() + "/" + dest;
    }

    const char *oldpath = sfp.c_str();
    const char *newpath = dfp.c_str();

    string name;
    int i = 0;
    for (i = source.length() - 1; i >= 0; i--)
    {
        if (source[i] == '/')
            break;
    }
    i++;
    for (i; i < source.length(); i++)
    {
        name += source[i];
    }
    // cout << name << "\n";

    dfp += "/" + name;
    newpath = dfp.c_str();

    // cout << dfp << "\n";

    if (rename(oldpath, newpath) == 0)
    {
        refresh();
        cout << "Moved Successfully\n";
    }
    else
    {
        cout << "Please Enter Valid Command\n";
    }
}

void copy(string source, string dest)
{
    static int flag = 0;
    static int flag2 = 0;

    string sfp;
    string dfp;

    if (source[0] == '/')
    {
        sfp = source;
    }
    else if (source[0] == '~')
    {
        source.erase(0, 1);
        sfp = root + "/" + source;
    }
    else
    {
        if (source[0] == '.')
        {
            source.erase(0, 1);
        }

        sfp = lefts.top() + "/" + source;
    }

    if (dest[0] == '/')
    {
        dfp = dest;
    }
    else if (dest[0] == '~')
    {
        dest.erase(0, 1);
        dfp = root + "/" + dest;
    }
    else
    {
        if (dest[0] == '.')
        {
            dest.erase(0, 1);
        }

        dfp = lefts.top() + "/" + dest;
    }

    const char *oldpath = sfp.c_str();
    const char *newpath = dfp.c_str();

    struct stat s;
    if (stat(oldpath, &s) == 0)
    {
        if (s.st_mode & S_IFDIR) //it's a directory
        {
            flag = 1;
            // newfolder(oldpath,dest);

            string name;
            int i = 0;
            for (i = source.length() - 1; i >= 0; i--)
            {
                if (source[i] == '/')
                    break;
            }
            i++;
            for (i; i < source.length(); i++)
            {
                name += source[i];
            }

            dfp += "/" + name;
            newpath = dfp.c_str();

            mkdir(newpath, O_RDWR | O_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);

            DIR *dir;
            struct dirent *diread;

            if ((dir = opendir(oldpath)) != nullptr)
            {
                while ((diread = readdir(dir)) != nullptr)
                {
                    if (strcmp(diread->d_name, ".") == 0)
                    {
                        continue;
                    }
                    if (strcmp(diread->d_name, "..") == 0)
                    {
                        continue;
                    }

                    string filedir = sfp + "/" + diread->d_name;
                    copy(filedir, dfp);
                }
                closedir(dir);
            }
        }

        else if (s.st_mode & S_IFREG) //it's a file
        {
            string name;
            int i = 0;
            for (i = source.length() - 1; i >= 0; i--)
            {
                if (source[i] == '/')
                    break;
            }
            i++;
            for (i; i < source.length(); i++)
            {
                name += source[i];
            }

            dfp += "/" + name;
            newpath = dfp.c_str();

            int fd1 = open(oldpath, O_RDONLY);
            int fd2 = open(newpath, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

            if (fd1 == -1)
            {
                cout << "Error can not open the source file\n";
                return;
            }
            if (fd2 == -1)
            {
                cout << "Error can not open the Dest. file\n";
                return;
            }

            while (1)
            {
                char buf[1030];
                char *c = buf;
                int n = read(fd1, c, 1024);
                if (n <= 0)
                    break;
                int k = write(fd2, c, n);
            }

            int c1 = close(fd1);
            int c2 = close(fd2);

            if (c1 == -1 or c2 == -1)
            {
                cout << "Error Can not Close Source File\n";
            }
            if (c2 == -1)
            {
                cout << "Error Can not Close Dest. File\n";
            }

            if (flag == 0)
            {
                refresh();
                cout << "File Copied Successfully\n";
            }

            return;
        }
        else
        {
            cout << "Error\n";
        }
    }
    else
    {
        cout << "Error\n";
    }
    if (flag == 1 and flag2 == 0)
    {
        flag2 = 1;
        refresh();
        cout << "Dir. Copied successfully\n";
    }
}

void delete_file(string source)
{
    string sfp;
    if (source[0] == '/')
    {
        sfp = source;
    }
    else if (source[0] == '~')
    {
        source.erase(0, 1);
        sfp = root + "/" + source;
    }
    else
    {
        if (source[0] == '.')
        {
            source.erase(0, 1);
        }

        sfp = lefts.top() + "/" + source;
    }

    const char *oldpath = sfp.c_str();

    if (remove(oldpath) == 0)
    {
        refresh();
        cout << "File Deleted Successfully\n";
    }
    else
    {
        cout << "Please Enter a Valid File Name\n";
    }
}

void delete_dir(string source)
{
    string sfp;
    if (source[0] == '/')
    {
        sfp = source;
    }
    else if (source[0] == '~')
    {
        source.erase(0, 1);
        sfp = root + "/" + source;
    }
    else
    {
        if (source[0] == '.')
        {
            source.erase(0, 1);
        }

        sfp = lefts.top() + "/" + source;
    }

    const char *oldpath = sfp.c_str();

    DIR *dir;
    struct dirent *diread;

    if ((dir = opendir(oldpath)) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            if (strcmp(diread->d_name, ".") == 0)
            {
                continue;
            }
            if (strcmp(diread->d_name, "..") == 0)
            {
                continue;
            }

            struct stat s;
            string filename = sfp + "/" + diread->d_name;
            const char *ffilename = filename.c_str();

            if (stat(ffilename, &s) == 0)
            {
                if (s.st_mode & S_IFDIR) //it's a directory
                {
                    delete_dir(filename);
                }

                else if (s.st_mode & S_IFREG) //it's a file
                {
                    remove(ffilename);
                }
                else
                {
                    refresh();
                    cout << "Error\n";
                }
            }
            else
            {
                refresh();
                cout << "Error\n";
            }
        }
        closedir(dir);
        if (remove(oldpath) == 0)
        {
            refresh();
            cout << "Dir. Deleted Successfully\n";
        }
        else
        {
            refresh();
            cout << "Please Enter a Valid Directory Path\n";
        }
    }
}

bool validpath(string source)
{
    string sfp;
    if (source[0] == '/')
    {
        sfp = source;
    }
    else if (source[0] == '~')
    {
        source.erase(0, 1);
        sfp = root + "/" + source;
    }
    else
    {
        if (source[0] == '.')
        {
            source.erase(0, 1);
        }

        sfp = lefts.top() + "/" + source;
    }

    struct stat s;

    if (!stat(sfp.c_str(), &s))
        return true;
    else
        return false;
}

void Commandmode()
{

    refresh();
    backtocanonical();
    cout << "\033[9999;1H"; //Set Cursor to bottom left

    string s;
    cout << "$ ";
    getline(cin, s);
    cmd.clear();
    string word;
    stringstream ss(s);

    while (ss >> word)
        cmd.push_back(word);

    if (cmd.size() == 0)
    {
        // continue;
        refresh();
        return;
    }

    else if (cmd[0] == "search")
    {
        refresh();
        if (cmd.size() > 2)
        {
            cout << "Please Enter a valid file/Dir Name\n";
            return;
        }
        else
        {
            if (search(cmd[1]))
                cout << "True\n";
            else
                cout << "False\n";
        }
    }

    else if (cmd[0] == "goto")
    {
        if (cmd.size() > 2)
        {
            cout << "Please Enter a valid Command";
            return;
        }
        else
        {
            if (cmd[1][0] == '/') //Goto absolute path
            {
                if (validpath(cmd[1]))
                {
                    lefts.push(cmd[1]);
                    refresh();
                    cout << "Path Changed Successfully\n";
                }
                else
                {
                    cout << "Please Enter a Valid Path\n";
                }
            }
            else if (cmd[1][0] == '~')
            {
                string sfp = cmd[1];
                sfp.erase(0, 1);
                sfp = root + "/" + sfp;
                if (validpath(sfp))
                {
                    lefts.push(sfp);
                    refresh();
                    cout << "Path Changed Successfully\n";
                }
                else
                {
                    cout << "Please Enter a Valid Path\n";
                }
            }

            else //Goto Relative Path
            {
                string sfp = cmd[1];
                string cwd = lefts.top();

                if (cmd[1][0] == '.' and cmd[1][1] != '.')
                {
                    sfp.erase(0, 1);
                }
                else if (cmd[1][0] == '.' and cmd[1][1] == '.')
                {

                    string cwd = lefts.top();

                    while (cwd[cwd.length() - 1] != '/')
                    {
                        cwd.pop_back();
                    }
                    cwd.pop_back();

                    if (validpath(cwd))
                    {
                        lefts.push(cwd);
                        refresh();
                        cout << "Path Changed Successfully\n";
                    }
                    else
                    {
                        cout << "Please Enter a Valid Path\n";
                    }
                    return;
                }

                cwd += "/";
                cwd += sfp;
                if (validpath(cwd))
                {
                    lefts.push(cwd);
                    refresh();
                    cout << "Path Changed Successfully\n";
                }
                else
                {
                    cout << "Please Enter a Valid Path\n";
                }
            }
        }
    }

    else if (cmd[0] == "mydir") //Extra cmd to print cwd
    {
        cout << lefts.top() << "\n";
    }

    else if (cmd[0] == "create_file")
    {
        if (cmd.size() != 3)
        {
            cout << "Please Provide a Valid Cmd\n";
        }
        else
        {
            //cmd[1] ==> filename
            //cmd[2] ==> path

            string filename = cmd[1];
            string path = cmd[2];

            if (cmd[2][0] == '/')
            {
                path += "/" + filename;
                const char *p = path.c_str();
                if (creat(p, O_CREAT | S_IRWXG | S_IRWXO | S_IRWXU) >= 0)
                {
                    refresh();
                    cout << "File Created Successfully\n";
                }
                else
                {
                    cout << "Please Enter Valid Command\n";
                }
            }
            else if (cmd[2][0] == '~')
            {
                path.erase(0, 1);
                string fullpath = root + "/" + path + "/" + filename;
                const char *p = fullpath.c_str();
                if (creat(p, O_CREAT | S_IRWXG | S_IRWXO | S_IRWXU) >= 0)
                {
                    refresh();
                    cout << "File Created Successfully\n";
                }
                else
                {
                    refresh();
                    cout << "Please Enter Valid Command";
                }
            }
            else
            {
                string cwd = lefts.top();
                string fullpath = cwd + "/" + path + "/" + filename;
                const char *p = fullpath.c_str();
                if (creat(p, O_CREAT | S_IRWXG | S_IRWXO | S_IRWXU) >= 0)
                {
                    refresh();
                    cout << "File Created Successfully\n";
                }
                else
                {
                    cout << "Please Enter Valid Command";
                }
            }
        }
    }

    else if (cmd[0] == "create_dir")
    {
        if (cmd.size() != 3)
        {
            cout << "Please Provide a Valid Cmd\n";
        }
        else
        {
            //cmd[1] ==> Dir name
            //cmd[2] ==> path

            string filename = cmd[1];
            string path = cmd[2];

            if (cmd[2][0] == '/')
            {
                path += "/" + filename;
                const char *p = path.c_str();
                if (mkdir(p, O_CREAT | S_IRWXU | S_IRWXG | S_IRWXG) == 0)
                {
                    refresh();
                    cout << "Dir. Created Successfully\n";
                }
                else
                {
                    cout << "Please Provide Valid Command\n";
                }
            }
            else if (cmd[2][0] == '~')
            {
                path.erase(0, 1);
                string fullpath = root + "/" + path + "/" + filename;
                const char *p = fullpath.c_str();
                if (mkdir(p, O_CREAT | S_IRWXU | S_IRWXG | S_IRWXG) == 0)
                {
                    refresh();
                    cout << "Dir. Created Successfully\n";
                }
                else
                {
                    cout << "Please Provide Valid Command\n";
                }
            }
            else
            {
                string cwd = lefts.top();
                string fullpath = cwd + "/" + path + "/" + filename;
                const char *p = fullpath.c_str();
                if (mkdir(p, O_CREAT | S_IRWXU | S_IRWXG | S_IRWXG) == 0)
                {
                    refresh();
                    cout << "Dir. Created Successfully\n";
                }
                else
                {
                    cout << "Please Provide Valid Command\n";
                }
            }
        }
    }

    else if (cmd[0] == "rename")
    {
        if (cmd.size() != 3)
        {
            cout << "Please Enter a Valid Command\n";
        }
        else
        {
            string source = cmd[1];
            string dest = cmd[2];

            rename1(source, dest);
        }
    }

    else if (cmd[0] == "move")
    {
        if (cmd.size() < 3)
        {
            cout << "Please Enter a Valid Command Thankyou\n";
        }
        else
        {
            string dest = cmd[cmd.size() - 1];

            for (int i = 1; i < cmd.size() - 1; i++)
            {
                move(cmd[i], dest);
            }
        }
    }

    else if (cmd[0] == "copy")
    {
        if (cmd.size() < 3)
        {
            cout << "Please Enter a Valid Command Thankyou\n";
        }
        else
        {
            string dest = cmd[cmd.size() - 1];

            for (int i = 1; i < cmd.size() - 1; i++)
            {
                refresh();
                copy(cmd[i], dest);
                
            }
        }
    }

    else if (cmd[0] == "delete_file")
    {
        if (cmd.size() != 2)
        {
            cout << "Enter a valid Command Please\n";
        }
        else
            delete_file(cmd[1]);
    }

    else if (cmd[0] == "delete_dir")
    {
        if (cmd.size() != 2)
        {
            cout << "Enter a valid Command Please\n";
        }
        else
            delete_dir(cmd[1]);
    }

    else
    {
        refresh();
        cout << "Enter a Valid Command Please\n";
        // refresh();
    }

    // cout << "\n";
}

void Commandterminal()
{
    // noncanonicalmode();
    // cout << "\033[2J\033[1;1H"; // Clearing the screen;
    // printf("%c[%d;%df", 0x1B, 1, 0); // cursor at 0,0;

    // Print_files_caller(0,19);
    refresh();

    cout << "\033[9999;1H"; //Set Cursor to bottom left

    int ch;

    while (1)
    {
        printf("%c[%d;%df", 0x1B, 25, 0); // cursor at 25,0;
        // printf("\x1B[31mPress q To Exit , Press Esc To Return To Normal Mode, Enter To type Commands\n\033[0m\t\t");
        printf("\033[3;104;30mPress q To Exit , Press Esc To Return To Normal Mode, Enter To type Commands\n\033[0m\t\t");
        // cout << "Press q To Exit , Press Esc To Return To Normal Mode, Enter To type Commands\n";
        cout << "\033[9999;1H"; //Set Cursor to bottom left
        ch = getch();
        if (ch == 'q')
        {
            exit(0);
        }
        else if (ch == 27)
        {
            Normalmode(lefts.top());
        }
        else
        {
            backtocanonical();
            Commandmode();
        }
    }
}

// End Of Cmd Mode

int getch(void)
{
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= static_cast<unsigned int>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void Get_files_metadata(string cwd)
{
    // curr_dir.clear();
    curr_files.clear();

    struct stat fileStat;
    DIR *dir;
    struct dirent *diread;

    const char *c = cwd.c_str();

    if ((dir = opendir(c)) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            // stat(diread->d_name, &fileStat);

            // char *dirr = (diread->d_name);
            // string dirrr = dirr;
            curr_files.push_back(*diread);
        }
        closedir(dir);
    }
}

void Print_Details(struct dirent *myfile)
{
    struct passwd *pw;
    struct group *gp;
    struct stat fileStat;
    char *c;
    int i;
    
    string path = lefts.top()+"/"+myfile->d_name;
    // cout<<path<<"\n";
    const char *path1=path.c_str();
    stat(path1, &fileStat);

    
    
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\t");

    printf("%ld ", fileStat.st_nlink);

    pw = getpwuid(fileStat.st_uid);
    if (pw)
        printf("%s ", pw->pw_name);
    else
        printf("%s", "NA");

    gp = getgrgid(fileStat.st_gid);
    if (gp)
        printf("%s  ", gp->gr_name);
    else
        printf("%s", "NA");

    printf("\t");

    c = ctime(&fileStat.st_mtime);
    if (c)
        for (i = 4; i <= 10; i++)
            printf("%c", c[i]);
    else
        printf("%s", "NA");

    printf("\t");

    double totsize = fileStat.st_size;
    int cc = 0;
    while (totsize > 1024)
    {
        totsize = totsize / 1024;
        cc++;
    }
    if (cc == 0)
    {
        printf("%10.1f B", totsize);
    }
    else if (cc == 1)
    {
        printf("%10.1f KB", totsize);
    }
    else if (cc == 2)
    {
        printf("%10.1f MB", totsize);
    }
    else if (cc == 3)
    {
        printf("%10.1f GB", totsize);
    }
    else
    {
        printf("%10.1f B", 0.0);
    }

    printf("\t");

    string name = myfile->d_name;
    if (name.length() > 14)
    {
        name = name.substr(0, 14);
        // name+="---";
    }
    printf(" %s\n", name.c_str());
}

void Print_files_caller(int start, int end)
{
    cout << "\033[2J\033[1;1H";      // Clearing the screen;
    printf("%c[%d;%df", 0x1B, 0, 0); // cursor at 0,0;

    




    for (int i = start; i <= end; i++)
    {
        Print_Details(&curr_files[i]);
    }

    printf("     \x1B[35m↑\033[0m\t");
    printf("\t      \x1B[35m↑\033[0m\t\t");
    printf("  \x1B[35m↑\033[0m\t\t");
    printf("  \x1B[35m↑\033[0m\t");
    printf("    \x1B[35m↑\033[0m\n");

    
    printf("\x1B[35mPermissions\033[0m\t");
    printf("\x1B[35mUser and Group \033[0m\t");
    printf("\x1B[35mLast Modified\033[0m\t");
    printf("\x1B[35mSize\033[0m\t");
    printf("\x1B[35m File Name\033[0m\n");



}

struct termios org;

void die(const char *s)
{
    perror(s);
    exit(1);
}

void backtocanonical()
{
    // cout << "\033[2J\033[1;1H"; // Clearing the screen;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &org) == -1)
        die("tcsetattr");
}

void noncanonicalmode()
{
    struct termios temp;
    // if (tcgetattr(STDIN_FILENO, &org) == -1)
    //     die("tcgetattr");

    temp = org;
    temp.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    temp.c_iflag &= ~(IXON | ICRNL);
    temp.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &temp) == -1)
        die("tcsetattr");
    atexit(backtocanonical);
}

void Raw_Mode(int vstart, int vend, int s, int e)
{

    noncanonicalmode();
    printf("%c[%d;%df", 0x1B, 1, 0); // cursor at 0,0;

    int ch;
    int flag = 1;
    while (flag)
    {
        ch = getch();

        if (ch == KEY_UP)
        {
            if (s >= 0 and e <= min(19, vend - 1))
            {
                if (tempcur != 1)
                {
                    tempcur--;
                    curr_cursor--;
                    printf("%c[%d;%df", 0x1B, tempcur, 0);
                    // moveCursor(tempcur,0);
                }
            }

            else
            {
                if (tempcur != 1)
                {
                    tempcur--;
                    curr_cursor = s + tempcur - 1;
                    printf("%c[%d;%df", 0x1B, tempcur, 0);
                    // moveCursor(tempcur,0);
                }
            }
        }

        else if (ch == KEY_DOWN)
        {
            if (s >= 0 and e <= min(19, vend - 1))
            {
                if (tempcur != min(20, vend + 1))
                {
                    tempcur++;
                    curr_cursor++;
                    printf("%c[%d;%df", 0x1B, tempcur, 0);
                }
            }
            else
            {
                if (tempcur != min(20, vend + 1))
                {
                    tempcur++;
                    curr_cursor = s + tempcur - 1;
                    printf("%c[%d;%df", 0x1B, tempcur, 0);
                }
            }
        }

        else if (ch == 'k' or ch == 'K')
        {
            if (s != 0)
            {
                e--;
                s--;

                Print_files_caller(s, e);
                //  runloop(st,en,cur_dir_path);
                if (s > 1)
                {
                    tempcur = 1;
                }
                curr_cursor = s + tempcur - 1;
                //  moveCursor(tempcur,0);
                printf("%c[%d;%df", 0x1B, tempcur, 0);
            }
        }
        else if (ch == 'l' or ch == 'L')
        {
            if (e != vend - 1)
            {
                e++;
                s++;
                // curs++;
                // tempcur++;
                Print_files_caller(s, e);
                //  runloop(st,en,cur_dir_path);
                if (e > min(19, vend - 1))
                {
                    tempcur = 1;
                }
                curr_cursor = s + tempcur - 1;

                printf("%c[%d;%df", 0x1B, tempcur, 0);
            }
        }

        else if (ch == ENTER_KEY)
        {

            struct dirent *myfile;
            myfile = &curr_files[curr_cursor];

            string cwf = lefts.top();
            cwf += "/";
            string filename = myfile->d_name;
            cwf += filename;
            const char *path = cwf.c_str();
            // cout<<cwf<<"\n";

            struct stat s;
            if (stat(path, &s) == 0)
            {
                if (s.st_mode & S_IFDIR)
                {
                    lefts.push(cwf);
                    flag = 0;
                    curr_cursor = 0;
                    tempcur = 1;
                    Normalmode(cwf);
                }
                else if (s.st_mode & S_IFREG)
                {
                    pid_t pid = fork();
                    if (pid == 0)
                    {
                        // child process
                        execlp("/usr/bin/xdg-open", "xdg-open", cwf.c_str(), (char *)0);
                        _exit(127);
                    };
                }
                else
                {
                    cout << "Anything else";
                }
            }
            else
            {
                cout << "Error";
            }
        }

        else if (ch == BACKSPACE)
        {
            
            string cwd = lefts.top();

            if(cwd=="/home")
            continue;
            while (!lefts.empty())
                lefts.pop();
            while (!rights.empty())
                rights.pop();

            while (cwd[cwd.length() - 1] != '/')
            {
                cwd.pop_back();
            }
            cwd.pop_back();
            

            // cout<<cwd;

            lefts.push(cwd);
            Normalmode(cwd); //Problem Here
        }

        else if (ch == KEY_LEFT)
        {
            if (lefts.size() > 1)
            {
                rights.push(lefts.top());
                lefts.pop();
                Normalmode(lefts.top());
            }
        }

        else if (ch == KEY_RIGHT)
        {
            if (rights.size() >=1)
            {
                lefts.push(rights.top());
                rights.pop();
                Normalmode(lefts.top());
            }
        }

        else if (ch == 'q')
        {
            exit(0);
        }

        else if (ch == HOME)
        {
            string home = "/home";
            lefts.push(home);
            Normalmode(home); //Problem
        }

        else if (ch == ':')
        {
            Commandterminal();
        }
    }
}

void Normalmode(string cwd)
{
    Get_files_metadata(cwd);

    int s = 0;
    int e = 0;

    if (curr_files.size() >= 20)
        e = 19;
    else
        e = curr_files.size() - 1;

    Print_files_caller(s, e);

    Raw_Mode(0, curr_files.size() - 1, s, e);
}

void solve()
{

    char buff[256];
    getcwd(buff, 256);
    // cout << "Current working directory: " << tmp << endl;
    string cwd = buff;
    root = cwd;
    lefts.push(cwd);
    // cout<<"Our cwd is : \n"<<cwd;

    Normalmode(cwd);


    //   if (getWindowSize(&screenrows, &screencols) == -1) die("getWindowSize");

         
    // printf("\x1B[31mTexting\033[0m\t\t");
    // printf("\x1B[32mTexting\033[0m\t\t");
    // printf("\x1B[33mTexting\033[0m\t\t");
    // printf("\x1B[34mTexting\033[0m\t\t");
    // printf("\x1B[35mTexting\033[0m\n");
    
    // printf("\x1B[36mTexting\033[0m\t\t");
    // printf("\x1B[36mTexting\033[0m\t\t");
    // printf("\x1B[36mTexting\033[0m\t\t");
    // printf("\x1B[37mTexting\033[0m\t\t");
    // printf("\x1B[93mTexting\033[0m\n");
    
    // printf("\033[3;42;30mTexting\033[0m\t\t");
    // printf("\033[3;43;30mTexting\033[0m\t\t");
    // printf("\033[3;44;30mTexting\033[0m\t\t");
    // printf("\033[3;104;30mTexting\033[0m\t\t");
    // printf("\033[3;100;30mTexting\033[0m\n");

    // printf("\033[3;47;35mTexting\033[0m\t\t");
    // printf("\033[2;47;35mTexting\033[0m\t\t");
    // printf("\033[1;47;35mTexting\033[0m\t\t");
    // printf("\t\t");
   


    //   cout<<"rows are = "<<screenrows<<"\n";
    //   cout<<"col are = "<<screencols<<"\n";

}


void handle_winch(int sig){
  signal(SIGWINCH, SIG_IGN);
  

printf("%c[%d;%df", 0x1B, 0, 0); // cursor at 0,0;
   refresh();
   

  signal(SIGWINCH, handle_winch);
}

int main()
{

    //  ios_base::sync_with_stdio(false);
    //  cin.tie(NULL);
    if (tcgetattr(STDIN_FILENO, &org) == -1)
        die("tcgetattr");

    ll t = 1;
    //cin>>t;
    signal(SIGWINCH, handle_winch);
    while (t--)
    {
        solve();
    }
    return 0;
}
