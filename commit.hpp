namespace gitCommit
{
#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#include <sys/stat.h>
using namespace std;

//

bool is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

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
//

int commit()
{
    char buff[PATH_MAX];
    getcwd(buff, sizeof(buff));
    string path(buff);
    string global_path_commit = path; //variable ./mygit/global
    string versionpathtxt = path + "/.mygit/version_no.txt";
    ifstream ifile(versionpathtxt.c_str());
    int version;
    ifile >> version;
    //cout << version;
    version = version + 1;
    ifile.close();
    ofstream ofile(versionpathtxt, ios::trunc);
    ofile << version;
    //cout << path << endl;
    string newDir = path + "/.mygit/";
    newDir = newDir + to_string(version);
    int check = mkdir(newDir.c_str(), 0777);
    if (!check)
        printf("Directory created\n");
    else
    {
        printf("Unable to create directory\n");
        exit(1);
    }
    string command = "cp -r";
    command = command + " " + path + "/.mygit/";
    int x = version - 1;
    command = command + to_string(x);
    command = command + "/. ";
    command = command + path + "/.mygit/";
    command = command + to_string(version);
    cout << command << endl;
    system(command.c_str());

    global_path_commit = global_path_commit + "/.mygit/global/";
    vector<string> files = get_files_from(path); //current cwd
    for (int i = 0; i < files.size(); i++)
    {
        string command_commit = "diff " + global_path_commit + files[i];
        command_commit = command_commit + " " + files[i];

        command_commit = command_commit + " >> " + path + "/.mygit/" + to_string(version - 1) + "/" + files[i] + ".patch";
        int a = system(command_commit.c_str());
        //cout << "a:" << a << endl;
        //cout << command_commit << endl;
        // cout << endl;
    }

    //remove files.delete files after creating patch for it
    for (int i = 0; i < files.size(); i++)
    {
        if(files[i] == "index.txt") continue;
        string command_remove = "rm ";
        command_remove = command_remove + path + "/.mygit/" + to_string(version - 1) + "/" + files[i];
        // cout << command_remove << endl;
        system(command_remove.c_str());
        // cout << endl;
    }
    /*
    the above function deletes the file 
    */

    return 0;
}
} // namespace gitCommit
  // namespace gitCommit



