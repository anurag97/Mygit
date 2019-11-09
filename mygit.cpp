#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include<dirent.h>
using namespace std;
#include "status.hpp"
#include "add2.hpp"
namespace gitstatus{
    int status();
}
namespace gitadd{
    int add();
}
using namespace gitstatus;
using namespace gitadd;

int init()
{
    bool gitdircreate = false;
    bool gitchdirmygit = false;
    bool gitv0create = false;
    bool gitchdirv0 = false;
    bool gitcreateindexv0 = false;

    if (mkdir(".mygit", 0777) == -1)
    {
        cerr << "Error1: " << strerror(errno) << endl;
        return 0;
    }
    else
    {
        gitdircreate = true;
        // cout<<".Mygit directory created"<<endl;
    }

    if (chdir(".mygit") == -1)
    {
        cerr << "Error2: " << strerror(errno) << endl;
        return 0;
    }
    else
    {
        gitchdirmygit = true;
        // cout<<"Moved to .mygit directory!!"<<endl;
    }

    ofstream fout;
    fout.open("version_no.txt", ios::out);
    if (fout)
    {
        string num = "0";
        fout << num << endl;
    }
    fout.close();

    if (mkdir("0", 0777) == -1)
    {
        cerr << "Error3: " << strerror(errno) << endl;
        return 0;
    }
    else
    {
        gitv0create = true;
        // cout<<"version0 directory created"<<endl;
    }

    if (chdir("0") == -1)
    {
        cerr << "Error4: " << strerror(errno) << endl;
        return 0;
    }
    else
    {
        gitchdirv0 = true;
        // cout<<"Moved to .version0 directory!!"<<endl;
    }

    ofstream fout2;
    fout2.open("index.txt");
    //  cout<<"Created index.txt in v0 file"<<endl;
    gitcreateindexv0 = true;
    fout.close();

    if (gitcreateindexv0 && gitdircreate && gitv0create && gitchdirmygit && gitchdirv0)
        return 1;
    else
        return 0;
};

// void add(){

// };

int main(int argc, char *argv[])
{
    string cmd;

    while (true)
    {
        cmd = argv[1];
        //cout << argv[1] << endl;
        if (cmd == "init")
        {
            if (init())
            {
                cout << "Mygit initialised!!\n";
                exit(EXIT_SUCCESS);
            }
            else
            {
                cout << "Sorry! Error in initialisation!!" << endl;
                exit(EXIT_SUCCESS);
            }
        }
        else if (cmd == "status")
        {
            gitstatus::status();
            exit(EXIT_SUCCESS);
        }
        else if(cmd == "add"){
             gitadd::add();
            exit(EXIT_SUCCESS);
        }
    }
}