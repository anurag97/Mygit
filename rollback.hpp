
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

int roll_back(int vno,int curr_ver)
{
    string scwd="";
    string retrieve_path="";
    string source_path="";
    string dest_path="";
    int stat = 0;
    //int curr_ver = atoi(retrieve_ver_no());
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
   
    //cout<<cmd<<endl;
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
    //cout << path << endl;
    ofile.close();


    vector<string> todel = get_files_from(scwd);
    for(auto x:todel)
    {
        string cmd = "rm " + scwd + "/" + x;
        system(cmd.c_str());
        //cout<<cmd<<endl;
    }
    //copying files from passed version no to cwd
    //-------------------------------------------

    dest_path=cwd;
    vector<string> tocopyagain=get_files_from(retrieve_path);
    for(auto x:tocopyagain)
    {
       
       source_path="";
        source_path=retrieve_path;
       source_path+=x;

        dest_path="";
        dest_path=cwd;
        dest_path+="/";

       dest_path+=x;
        //cout<<source_path<<endl;
        //cout<<dest_path<<endl;
       string command="cp ";
       command+=source_path;
       command+=" ";
       command+=dest_path;
       //cout<<command<<endl;
       system(command.c_str());

       // string cmd = "rm " + scwd + "/" + x;
       // system(cmd.c_str());
       // cout<<x<<endl;
    }
    

    




    return 1;
}
