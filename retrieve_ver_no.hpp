namespace retrieve_ver_no
{
string retrieve_ver_no()
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
    cout<<vc<<endl;
    return (vc);
}
}






