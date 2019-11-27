namespace gitpush
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

vector<string> get_files_from(string path)
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
        if (fname == "." || fname == ".." || fname == ".mygit" || fname == "a.out" || fname == ".vscode" || fname == "mygit")
        {
            continue;
        }
        else
        {
            toret.push_back(fname);
        }
    }
    closedir(dir);
    return toret;
}

void cp(string src, string des)
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
    fin.close();
    fout.close();
}

int push()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Unable to get current working directory!\n");
        exit(1);
    }
    string push_path_dir = string(cwd) + "/push";
    int mkdir_stat = mkdir(push_path_dir.c_str(), 0777);
    if (mkdir_stat != 0 && errno != EEXIST)
    {
        cerr << "unable to make push directory " << strerror(errno) << endl;
        return 0;
    }
    string pull_path_dir = string(cwd) + "/pull";
    vector<string> filelist = get_files_from(pull_path_dir);
    for (auto f : filelist)
    {
        string src = pull_path_dir + "/" + f;
        string des = push_path_dir + "/" + f;
        cp(src, des);
    }
    return 0;
}
}















