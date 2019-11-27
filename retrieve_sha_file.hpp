namespace retrieve_sha_file
{
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
void retrieve_sha_file(string s, string vno)
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

    string vcpath = mygit_path + vno + "/index.txt";
    //cout << vcpath << endl;
    map<string, string> filemap = get_map_from(vcpath);
    string ans = "";
    for (auto x : filemap)
    {
        if (x.second == s)
        {
            ans = x.first;
            break;
        }
    }
    if (ans == "")
    {
        cout << "file with given sha not found" << endl;
    }
    else
    {
        cout << ans << endl;
    }
}
}






