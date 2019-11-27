namespace merge
{
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <dirent.h>
#include <iostream>
#define VERSION_FILE "version_no.txt"
#define INDEX_FILE "index.txt"
using namespace std;

bool is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void mv(string src, string des)
{
    ifstream fin;
    ofstream fout;
    fin.open(src, ios::in);
    fout.open(des, ios::out);
    string line;

    while (fin)
    {
        getline(fin, line);
        fout << line << endl;
    }
    if (remove(src.c_str()) == 0)
    {
        cout << src << " deleted successfully" << endl;
    }
    fin.close();
    fout.close();
}

vector<string> very_diff_get_files_from(string path)
{
    vector<string> toret;
    struct dirent *entry;
    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
    {
        cout << "Error in opening Directory";
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        //cout << entry->d_name << endl;
        string fname = entry->d_name;
        string filepath = path + "/" + fname;
        if (is_regular_file(filepath.c_str()))
        {
            if (fname == "." || fname == ".." || fname == ".mygit" || fname == "a.out" || fname == ".vscode" || fname == "mygit")
            {
                continue;
            }
            else
            {
                toret.push_back(fname);
            }
        }
    }
    closedir(dir);
    return toret;
}

int maerge()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Unable to get current working directory!\n");
        exit(1);
    }
    //removing files in cwd
    vector<string> file_in_cwd = very_diff_get_files_from(cwd);
    for (auto f : file_in_cwd)
    {
        string rmfile = string(cwd) + "/" + f;
        //remove(rmfile.c_str());
    }
    //moving files to cwd
    string push_path = string(cwd) + "/push";
    vector<string> file_in_push = very_diff_get_files_from(push_path);
    for (auto f : file_in_push)
    {
        string src = push_path + "/" + f;
        string des = string(cwd) + "/" + f;
        cout << "moving file " << src << " to " << des << endl;
        mv(src, des);
    }
    return 0;
}


}



