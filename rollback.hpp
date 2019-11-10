

//2 -> vno >= currVer, 1 -> vno<currver and succ terminated

int roll_back(int vno,int curr_ver)
{
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
        mygit_path += "/.mygit/";
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
    return 1;
}

