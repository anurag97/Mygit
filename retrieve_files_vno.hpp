namespace gitret_files{
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <dirent.h>
#define VERSION_FILE "version_no.txt"
#define INDEX_FILE "index.txt"
using namespace std;
string vc;
string vcpath;
string indexpath;
map<string, string> filemap;
string current_path_global;


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
//2 -> vno >= currVer, 1 -> vno<currver and succ terminated

int ret_files_func(int vno,int curr_ver)
{
    string scwd="";
    string retrieve_path="";
    string source_path="";
    string dest_path="";
    int stat = 0;
    

    if (vno >= curr_ver)
    {
        return 2;
    }

    
    char cwd[PATH_MAX]; //defined in limits
    string mygit_path = "";

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        mygit_path = cwd;
        scwd=cwd;
        
        mygit_path += "/.mygit/";
        retrieve_path=cwd;
        retrieve_path += "/.mygit/";
        retrieve_path+=to_string(vno);
        retrieve_path+="/";
       
        cout<<"retrieve_path::"<<retrieve_path<<endl;
    }
    else
    {
        perror("unable to get current working directory");
        exit(-2);
    }

    //rm -r dir_name
   
  ;
    for (int i = vno + 1; i <= curr_ver; i++)
    {
         string cmd = "rm -r " + mygit_path;
        cmd = cmd + to_string(i);
        cout << "cmd to be executed" << endl;
        system(cmd.c_str());
    }



    string versionpathtxt = mygit_path + "version_no.txt";
    cout << "version_path  : " << versionpathtxt << endl;
    ofstream ofile(versionpathtxt, ios::trunc);
    ofile << vno;
    
    ofile.close();


    vector<string> todel = get_files_from(scwd);
    for(auto x:todel)
    {
        string cmd = "rm " + scwd + "/" + x;
        system(cmd.c_str());
        
    }









    //copying files from passed version no to cwd
    //-------------------------------------------

    // int to_ver_no = curr_ver-1;
    // string lastverno = to_string(to_ver_no);
    mygit_path = "";
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        mygit_path = cwd;
        mygit_path += "/.mygit/";
    }
    else
    {
        perror("Unable to get current working directory!\n");
        exit(1);
    }


    vcpath = mygit_path + VERSION_FILE;
   
    fstream vcfile(vcpath, std::ios_base::in);
    vcfile >> vc;
    indexpath = mygit_path + vc + "/" + INDEX_FILE;

    //current version is the version saved in version_no.txt

    string currverpath = mygit_path+vc;

//now we copy specific files which are in index of mentioned verison from global  
  filemap = get_map_from(indexpath);
  current_path_global = mygit_path+"global/";
  for (auto i=filemap.begin();i!=filemap.end();i++){
      string ifirst = i->first;
      string sourcefilepath = current_path_global+ifirst;
      string destfilepath = currverpath+"/"+ifirst;

    ifstream ifile(sourcefilepath, ios::in); 
    ofstream ofile(destfilepath, ios::out); 
     if (!ifile.is_open()) {
        cout << "file not found"; 
    } 
    else { 
        ofile << ifile.rdbuf(); 
    } 
  }




//run patch command for all patch fikes

for (auto i=filemap.begin();i!=filemap.end();i++){
      string ifirst = i->first;
      string sourcefilepath = currverpath+"/"+ifirst;
      string patchfilepath = currverpath+"/"+ifirst+".patch";

   string cmd = "patch "+sourcefilepath + " " + patchfilepath;
   system(cmd.c_str()); 
   
  }
  
  
  
  //remove patch files
for (auto i=filemap.begin();i!=filemap.end();i++){
      string ifirst = i->first;
      string patchfilepath = currverpath+"/"+ifirst+".patch";
      remove(patchfilepath.c_str());
  }



//remove all files existing in cwd


  DIR* folder = opendir(scwd.c_str());
  struct dirent   *next_file;
  char fpathlocal[PATH_MAX];
     while ( (next_file = readdir(folder)) != NULL )
    {
        sprintf(fpathlocal, "%s/%s", scwd.c_str(), next_file->d_name);
        if(fpathlocal == "a.out")continue;
        remove(fpathlocal);
    }
    closedir(folder);




for (auto i=filemap.begin();i!=filemap.end();i++){
      string ifirst = i->first;
      if(ifirst=="index.txt") continue;
      string sourcefilepath = currverpath+"/"+ifirst;
      string cwds = cwd;
      string destfilepath = cwds+"/"+ifirst;

    ifstream ifile(sourcefilepath, ios::in); 
    ofstream ofile(destfilepath, ios::out); 
     if (!ifile.is_open()) {
        cout << "file not found"; 
    } 
    else { 
        ofile << ifile.rdbuf(); 
    } 
  }
    


    return 1;
}


}
}


