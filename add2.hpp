namespace gitadd
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
string current_path_global;

char cwd[PATH_MAX];
string mygit_path1;
string vcpath;
string vc;
string indexpath;
vector<string> filelist;
map<string, string> filemap;
vector<string> untracked;
vector<string> modified;
vector<string> deleted;
int numoffiles;


bool is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

string get_sha(string file_name)
{
    string f1 = "sha1sum " + file_name + " > temp.txt";
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
    }
    return sha;
}

int process_untracked(vector<string> &untrackedv)
{

    for (int i = 0; i < untrackedv.size(); i++)
    {
        string filename = untrackedv[i];

        //copy file from local folder to current version folder
        string cwd1 = cwd;
        string inppath = cwd1 + "/" + filename;
        string filesha = get_sha(inppath);
        string outpath = mygit_path1 + vc + "/" + filename;

        //
        string command_copy = "cp ";
        command_copy = command_copy + inppath + " ";
        command_copy = command_copy + cwd1 + "/.mygit/global/" + filename;
        //cout << command_copy << endl;
        system(command_copy.c_str());
        /*
        copying the untracked files to global folder
        */

        //

        ifstream fin;
        ofstream fout;

        fin.open(inppath, ios::in);
        fout.open(outpath, ios::out);
        string line;

        while (fin)
        {
            getline(fin, line);
            fout << line << endl;
        }
        fin.close();
        fout.close();

        filemap[filename] = filesha;
    }
    //copying untracked files to global , globalindex[untrackedfiles version_no]
    //updating entry in global_index.txt file for untracked files only
    string globindexpath = string(cwd) + "/.mygit/global_index.txt";
    cout << "global path : " << globindexpath << endl;
    ofstream globindexfile(globindexpath, ios::app);
    for (int i = 0; i < untrackedv.size(); i++)
    {
        string filename = untrackedv[i];
        string cwd1 = cwd;
        string inppath = cwd1 + "/" + filename;
        string outpath = current_path_global + "/" + filename;
        globindexfile << filename << " " << vc << "\n";

        ifstream fin;
        ofstream fout;

        fin.open(inppath, ios::in);
        fout.open(outpath, ios::out);
        string line;

        while (fin)
        {
            getline(fin, line);
            fout << line << endl;
        }
        fin.close();
        fout.close();
    }
    globindexfile.close();
    //
    ofstream fout1;
    fout1.open(indexpath, ios::out);
    for (auto itr = filemap.begin(); itr != filemap.end(); itr++)
    {
        string line;
        line += itr->first + " " + itr->second;
        fout1 << line << endl;
    }
    fout1.close();

    return 1;
}
int process_modified(vector<string> modifiedv)
{
    for (int i = 0; i < modifiedv.size(); i++)
    {
        string filename = modifiedv[i];
        string cwd1 = cwd;
        string inpath = cwd1 + "/" + filename;
        string outpath = mygit_path1 + vc + "/" + filename;
        ifstream fin(inpath);
        ofstream fout(outpath);

        string line;
        while (fin)
        {
            getline(fin, line);
            fout << line << endl;
        }
        fin.close();
        fout.close();
        string filesha = get_sha(filename);
        filemap[filename] = filesha;
    }

    ofstream fout1;
    fout1.open(indexpath, ios::out);
    for (auto itr = filemap.begin(); itr != filemap.end(); itr++)
    {
        string line;
        line += itr->first + " " + itr->second;
        fout1 << line << endl;
    }
    fout1.close();

    return 1;
}

int process_deleted(vector<string> deletedv)
{
    string inpath = mygit_path1 + vc + "/";
    
    
    for (int i = 0; i < deletedv.size(); i++)
    {
        string filename = inpath + deletedv[i];
        filemap.erase(deletedv[i]);
       // cout<<"path to delete:"<<filename<<endl;
        remove(filename.c_str());
    }




    ofstream fout1;
    fout1.open(indexpath, ios::trunc | ios::app);
    for (auto itr = filemap.begin(); itr != filemap.end(); itr++)
    {
        string line;
        line += itr->first + " " + itr->second;
        fout1 << line << endl;
    }
    fout1.close();

    return 1;
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
    return toret;}

map<string, string> get_map_from(string path)
{
    map<string, string> toret;
    fstream f(path, std::ios_base::in);
    if (f.is_open())
    {
        string line;
        while (getline(f, line))
        {
            stringstream ss(line);
            string filename = "";
            string sha = "";
            ss >> filename;
            ss >> sha;
            toret[filename] = sha;
        }
    }
    else
    {
        perror("Unable to open file information get_map_from function");
        exit(1);
    }
    return toret;
}

int add()
{
    mygit_path1 = "";
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
    vcpath = mygit_path1 + VERSION_FILE;
    fstream vcfile(vcpath, std::ios_base::in);
    vcfile >> vc;
    ;
    indexpath = mygit_path1 + vc + "/" + INDEX_FILE;
    filelist = get_files_from(cwd);
    numoffiles = filelist.size();
    filemap = get_map_from(indexpath);

    for (int i = 0; i < numoffiles; i++)
    {
        string shaf = get_sha(filelist[i]);
        if (filemap.find(filelist[i]) == filemap.end())
        {
            untracked.push_back(filelist[i]);
        }
        else if (filemap[filelist[i]] != shaf)
        {
            modified.push_back(filelist[i]);
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

    if (modified.size() != 0)
    {
        cout << "Modified:" << endl;
        for (int i = 0; i < modified.size(); i++)
        {
            cout << modified[i] << endl;
        }
    }
    if (untracked.size() != 0)
    {
        cout << "Untracked:" << endl;
        for (int i = 0; i < untracked.size(); i++)
        {
            cout << untracked[i] << endl;
        }
    }
    if (deleted.size() != 0)
    {
        cout << "Deleted:" << endl;
        for (int i = 0; i < deleted.size(); i++)
        {
            cout << deleted[i] << endl;
        }
    }

    int pmodified = 0, puntracked = 0, pdeleted = 0;

    if (modified.size() != 0)
    {
        pmodified = process_modified(modified);
        if (pmodified)
        {
            modified.clear();
        }
    }
    if (untracked.size() != 0)
    {
        puntracked = process_untracked(untracked);
        if (puntracked)
            untracked.clear();
        if (untracked.size())
            cout << "untracked clear!" << endl;
    }
    if (deleted.size() != 0)
    {
        pdeleted = process_deleted(deleted);
        if (pdeleted)
            deleted.clear();
    }

    if (modified.size() == 0 && untracked.size() == 0 && deleted.size() == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

} // namespace gitadd



