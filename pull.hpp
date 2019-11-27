namespace gitpull
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

vector<string> diff_get_files_from(string path)
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
        if (fname == "index.txt" || fname == "." || fname == "..")
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

int pull()
{
    string mygit_path1 = "";
    string vc;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        mygit_path1 = cwd;
        mygit_path1 += "/.mygit/";
    }
    else
    {
        perror("Unable to get current working directory!\n");
        exit(1);
    }
    string vcpath = mygit_path1 + VERSION_FILE;
    fstream vcfile(vcpath, std::ios_base::in);
    vcfile >> vc;
    vcfile.close();
    string copy_src_path_dir = mygit_path1 + vc;
    cout << "copying all files from " << copy_src_path_dir << endl;
    vector<string> filelist = diff_get_files_from(copy_src_path_dir);
    string copy_des_path_dir = string(cwd) + "/pull";
    if (mkdir(copy_des_path_dir.c_str(), 0777) == -1 && errno != EEXIST)
    {
        cerr << "unable to make pull directory " << strerror(errno) << endl;
        return 0;
    }
    for (auto f : filelist)
    {
        cout << "file going to be copied : " << f << endl;
        string src = copy_src_path_dir + "/" + f;
        string des = copy_des_path_dir + "/" + f;
        string cmd = "cp " + src + " " + des;
        cout << "copying " << src << " to" << des << endl;
        system(cmd.c_str());
    }
    return 0;
}
}





















