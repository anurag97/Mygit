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

int commit()
{
    char buff[PATH_MAX];
    getcwd(buff, sizeof(buff));
    string path(buff);
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
    return 0;
}
} // namespace gitCommit
} // namespace gitCommit
