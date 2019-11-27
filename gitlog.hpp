namespace gitlog{
#include<bits/stdc++.h>
#include<fstream>
#include<ctime>
using namespace std;

string logpath;
char cwd[PATH_MAX];
string getcurrtime(){
    time_t curr_time;
    curr_time = time(NULL);
    tm *tm_local = localtime(&curr_time);
    string t ="";
    string thour =  to_string(tm_local->tm_hour);
    string tmin = to_string(tm_local->tm_min);
    string tsec = to_string(tm_local->tm_sec);
    t += thour+":"+tmin + ":"+tsec;
    return t;

}
void setlogpath(){
  
    if(getcwd(cwd,sizeof(cwd))!=NULL){
        logpath=cwd;
        logpath+="/.mygit/";
        logpath += "gitlog.txt";
    }
    else {
        perror("Unable to get current working directory!\n");
        exit(1);
    }
}

void writeinlog(string logdetails){
std::ofstream flog;
setlogpath();
cout<<logpath<<endl;
flog.open(logpath,std::ios::app);
string currtime = getcurrtime();
cout<<currtime<<endl;
flog<<currtime<<":"<<logdetails<<endl;
flog.close();
}

void printlog(){
    ifstream flog ;
    string line;
    setlogpath();
    flog.open(logpath,ios::in);

    while(flog){
        getline(flog , line);

        cout<<line<<endl;
    }
    flog.close();
}


}
}


