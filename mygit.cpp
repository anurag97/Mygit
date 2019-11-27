#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <dirent.h>
#include "push.hpp"
#include "pull.hpp"
#include "merge.hpp"
using namespace std;
#include "status.hpp"
//#include "rollback.hpp"
#include "add2.hpp"
#include "gitlog.hpp"
#include "commit.hpp"
#include "rollback.hpp"
#include "retrieve_ver_no.hpp"
#include "retrieve_sha_file.hpp"
#include "retrieve_files_vno.hpp"
//string current_path_global;

namespace retrieve_sha_file
{

void retrieve_sha_file();
} // namespace retrieve_sha_file

namespace gitret_files{
    int ret_files_func();
}
namespace retrieve_ver_no
{
string retrieve_ver_no();
} // namespace retrieve_ver_no
namespace gitrollback{
    int roll_back( );
}
namespace gitstatus
{
int status();
}
namespace gitadd
{
int add();
}
namespace gitpush
{
    int push();
}
namespace gitpull
{
    int pull();
}
namespace gitmerge
{
    int merge();
}
namespace gitlog
{
void writeinlog();
void printlog();
} // namespace gitlog
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

    char buffer[PATH_MAX];
    getcwd(buffer, sizeof(buffer));
    string current_path(buffer);

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

    if (mkdir("global", 0777) == -1)
    {
        cerr << "Error2: " << strerror(errno) << endl;
        return 0;
    }

    if (chdir("global") == -1)
    {
        cerr << "Error2: " << strerror(errno) << endl;
        return 0;
    }
    vector<string> files = gitadd::get_files_from(current_path);
    for (int i = 0; i < files.size(); i++)
    {
        string filename = files[i];

        //copy file from local folder to current version folder
        string cwd1 = current_path;
        string inppath = cwd1 + "/" + filename;

        char temp_global[PATH_MAX];
        getcwd(temp_global, sizeof(temp_global));
        current_path_global = temp_global;
        //changes made here made current_path_global is a global variable
        string outpath = current_path_global + "/" + filename;

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
    string gitpath = current_path + "/.mygit";
    if (chdir(gitpath.c_str()) == -1)
    {
        cerr << "Error2: " << strerror(errno) << endl;
        return 0;
    }
    // ofstream globfile("global_index.txt", ios::out);
    // if (globfile)
    // {
    //     int i = 0;
    //     for (; i <= files.size() - 1; i++)
    //     {
    //         string s = files[i];
    //         s = s + " 0\n";
    //         globfile << s;
    //     }
    // }
    // globfile.close();

    /*
    comment above section of code because we are already changing 
    in untracked module,so when untracked get called,all entries save to 
    global_index.txt
    */

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

        cmd = argv[1];

        //cout << argv[1] << endl;
        if (cmd == "init")
        {
            if (init())
            {

                string temp2 = "Mygit initialised!!";
                writeinlog(temp2);
                cout << temp2 << endl;
                exit(EXIT_SUCCESS);
            }
            else
            {
                string temp = "Sorry! Error in initialisation!!";
                writeinlog(temp);
                cout << temp << endl;
                exit(EXIT_SUCCESS);
            }
        }
        else if (cmd == "status")
        {
            if (gitstatus::status())
            {
                string temp = "Status executed!";
                writeinlog(temp);
                cout << temp << endl;
                exit(EXIT_SUCCESS);
            }
            else
            {
                string temp = "Error in status!";
                writeinlog(temp);
                cout<<temp<<endl;
                exit(EXIT_SUCCESS);
            }
        }
        else if (cmd == "add")
        {
            if (gitadd::add())
            {
                string temp = "Mygit add executed";
                writeinlog(temp);
                cout << temp << endl;
                exit(EXIT_SUCCESS);
            }
        }
        else if (cmd == "log")
        {
            gitlog::printlog();
             string temp = "Mygit log executed";
                writeinlog(temp);
                cout << temp << endl;
                exit(EXIT_SUCCESS);
            
        }
        else if (cmd == "commit")
        {
            gitCommit::commit();
            string temp = "Mygit commit executed";
                writeinlog(temp);
                cout << temp << endl;
            exit(EXIT_SUCCESS);
        }
        else if (cmd == "retrieve_ver_no")
        {
            retrieve_ver_no::retrieve_ver_no();
            string temp = "Mygit version retrieve executed";
                writeinlog(temp);
                cout << temp << endl;
            exit(EXIT_SUCCESS);
        }
        else if (cmd == "retrieve")
        {
            string cmd1 = argv[2];
            if (cmd1 != "-a")
            {
                retrieve_sha_file::retrieve_sha_file(argv[2], argv[3]);
                string temp = "Mygit retrieve sha vno executed";
                writeinlog(temp);
                cout << temp << endl;
                exit(EXIT_SUCCESS);
            }
            else
            {
               
           string ver_no = retrieve_ver_no::retrieve_ver_no();
           cout<<"ver_no "<<endl;
           int curr_ver = atoi(ver_no.c_str());
               
               string vrno = argv[3];
               int vno = stoi(vrno);
                gitret_files::ret_files_func(vno,curr_ver);
                string temp = "Mygit retrieve -a vno executed";
                writeinlog(temp);
                cout << temp << endl;
                exit(EXIT_SUCCESS);
            }
        }
        else if (cmd == "rollback"){
           string ver_no = retrieve_ver_no::retrieve_ver_no();
           cout<<"ver_no "<<ver_no<<endl;
         int curr_ver = atoi(ver_no.c_str());
           cout<<"aa"<<endl;
           gitrollback::roll_back(curr_ver);
           string temp = "Mygit rollback executed";
                writeinlog(temp);
                cout << temp << endl;
           exit(EXIT_SUCCESS); 
        }
          else if ( cmd == "diff" )
        {
            string inp1=argv[2];
            string inp2=argv[3];
            string command_diff = "diff "+ inp1 +" "+inp2;
            system(command_diff.c_str());
            // cout<<"inp1 "<<inp1<<endl;
            // cout<<"inp2 "<<inp2<<endl;
            string temp = "Mygit diff executed";
                writeinlog(temp);
                cout << temp << endl;
             exit(EXIT_SUCCESS);
        }
        else if( cmd == "push")
        {
            gitpush::push();
            exit(EXIT_SUCCESS);
        }
        else if( cmd == "pull")
        {
            gitpull::pull();
            exit(EXIT_SUCCESS);
        }
        else if(cmd == "merge")
        {
            gitmerge::merge();
            exit(EXIT_SUCCESS);
        }
        else
        {
            cout << "Wrong Command" << endl;
            exit(EXIT_SUCCESS);
        }
    
}
}


