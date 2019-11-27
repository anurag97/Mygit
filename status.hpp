namespace gitstatus
{
#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#define VERSION_FILE "version_no.txt"
#define INDEX_FILE "index.txt"
using namespace std;

string get_sha(string file_name)
{
    string f1 = "sha1sum " + file_name + " > temp.txt";
    //cout<<f1<<endl;

    char buff[255];
    strcpy(buff, f1.c_str());
    system(buff);
    FILE *fp = fopen("temp.txt", "r+");
    char buffer[100];
    fread(buffer, sizeof(char), 100, fp);
    string input = buffer;
    stringstream ss(input);
    string sha;
    ss >> sha;
    fclose(fp);
    int status = remove("temp.txt");
    if (status)
    {
        cout << "fail to delete file" << endl;
        ;
    }
    return sha;
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
        if (fname == "." || fname == ".." || fname == ".mygit" || fname == "a.out" || fname == ".vscode" || fname=="mygit")
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

map<string, string> get_map_from(string path)
{
    map<string, string> toret;
    fstream f(path, std::ios_base::in);
    if (f.is_open())
    {
        string line;
        while (getline(f, line)) //reading line by line of file
        {
            stringstream ss(line); //tokenising using space filename sha
            string filename = "";
            string sha = "";
            ss >> filename;
            ss >> sha;
            toret[filename] = sha;
        }
    }
    else
    {
        perror("unable to open file inf get_map_from function");
        exit(1);
    }
    return toret;
}

int status()
{
    char cwd[PATH_MAX]; //defined in limits
    string mygit_path = "";
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        mygit_path = cwd;
        mygit_path += "/.mygit/";
    }
    else
    {
        perror("unable to get current working directory");
        exit(1);
    }

    //READING VERSION NUMBER AND MAP OF ALREADY ADDED FILES
    //-----------------------------------------------------

    string vcpath = mygit_path + VERSION_FILE;
    fstream vcfile(vcpath, std::ios_base::in);
    string vc;
    vcfile >> vc;
    //cout << vc << endl; /* code */
    vcfile.close();
    string indexpath = mygit_path + vc + "/" + INDEX_FILE;
    //cout << "indexpath : " << indexpath << endl;
    vector<string> filelist = get_files_from(cwd);
    int numoffiles = filelist.size();
    map<string, string> filemap = get_map_from(indexpath);
    vector<string> untracked; //new
    vector<string> modified;
    vector<string> deleted;
    for (int i = 0; i < numoffiles; i++)
    {
        string shaf = get_sha(filelist[i]);
        if (filemap.find(filelist[i]) == filemap.end())
        {
            //cout << "filelist[i]" << filelist[i] << endl;
            untracked.push_back(filelist[i]);
        }
        else
        {
            if (filemap[filelist[i]] != shaf)
            {
                modified.push_back(filelist[i]);
            }
        }
    }
    sort(filelist.begin(), filelist.end());
    for (auto f : filemap)
    {
        if (find(filelist.begin(), filelist.end(), f.first) == filelist.end())
        {
            deleted.push_back(f.first);
        }
    }
    cout << "no. of modified files " << modified.size() << endl;
    cout << "no. of untracked files " << untracked.size() << endl;
    cout << "no. of deleted files " << deleted.size() << endl;
    if (modified.size() == 0 && untracked.size() == 0 && deleted.size() == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
}






