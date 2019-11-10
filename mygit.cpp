#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include<dirent.h>

using namespace std;
#include "status.hpp"
//#include "rollback.hpp"
#include "add2.hpp"
#include"gitlog.hpp"
#include"commit.hpp"
#include"rollback.hpp"
#include"retrieve_ver_no.hpp"
#include "retrieve_sha_file.hpp"

namespace retrieve_sha_file
{

void retrieve_sha_file();
} // namespace retrieve_ver_no




namespace retrieve_ver_no
{
string retrieve_ver_no();
} // namespace retrieve_ver_no


namespace gitstatus{
    int status();
}
namespace gitadd{
    int add();
}
namespace gitlog{
    void writeinlog();
    void printlog();
}
namespace gitCommit
{
    int commit();
}
using namespace gitstatus;
using namespace gitadd;
using namespace gitlog;
using namespace gitCommit;
using namespace retrieve_ver_no;




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

    ofstream flog;
    flog.open("mygit_log.txt", ios::out);
    flog.close();


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
             
               string temp2 = "Mygit initialised!!";
               writeinlog(temp2);
               cout<<temp2<<endl;
                exit(EXIT_SUCCESS);
            }
            else
            {
                string temp =  "Sorry! Error in initialisation!!";
                writeinlog(temp);
                cout<<temp<<endl;
                exit(EXIT_SUCCESS);
            }
        }
        else if (cmd == "status")
        {
            if(gitstatus::status()){
            string temp =  "Status executed!";
            writeinlog(temp);
            cout<<temp<<endl;
            
            exit(EXIT_SUCCESS);
            }
            else {
               // string temp = "Error in status!";
                //writeinlog(temp);
                //cout<<temp<<endl;
                exit(EXIT_SUCCESS);
            }
        }
        else if(cmd == "add"){
             if(gitadd::add()){
             string temp = "Mygit add executed";
             
             
            exit(EXIT_SUCCESS);
             }
        }
        else if(cmd == "log"){
            gitlog::printlog();
            exit(EXIT_SUCCESS);
        }
        else if(cmd == "commit")
		{
			gitCommit::commit();
			exit(EXIT_SUCCESS);
		}
        else if(cmd == "retrieve_ver_no")
        {
            retrieve_ver_no::retrieve_ver_no();
            exit(EXIT_SUCCESS);

        }
        else if(cmd == "retrieve")
        { 
            string cmd1=argv[2];
            if(cmd1 != "-a")
            {
                retrieve_sha_file::retrieve_sha_file(argv[2],argv[3]);
                exit(EXIT_SUCCESS);
            }
            else
            {
                string ver_no=retrieve_ver_no::retrieve_ver_no();
                cout<<"ver_no"<<endl;
                int curr_ver=atoi(ver_no.c_str());
                roll_back(atoi(argv[3]),curr_ver);
                exit(EXIT_SUCCESS);
            }
        }
        else
        {
            cout<<"Wrong Command:"<<endl;
        }
        
    }
}
}
