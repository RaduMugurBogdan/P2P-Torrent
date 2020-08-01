#include<string.h>
#include<iostream>
using namespace std;

class files_path{
    private:
        char tmp_files_path[200]="";
        char down_files_path[200]="";
    public:
    const char* get_tmp_files_path(){
        return this->tmp_files_path;
    }
    void set_tmp_files_path(const char* path){
        strcpy(this->tmp_files_path,path);
    }
    const char* get_down_files_path(){
        return this->down_files_path;
    }
    void set_down_files_path(const char* path){
       strcpy(this->down_files_path,path);
    }

    void set_default_values(){
        const char* download_files_path="/home/oem/Desktop/main_network_project/client_side/DATA/FILES_DATA/";
        strcpy(this->down_files_path,down_files_path);

        const char* tmp_files_path="/home/oem/Desktop/main_network_project/client_side/DATA/DN_FILES";
        strcpy(this->tmp_files_path,tmp_files_path);
    }

};





