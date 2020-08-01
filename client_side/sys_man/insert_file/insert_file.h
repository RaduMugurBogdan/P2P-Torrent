

const char* info_files_path="../DATA/DN_FILES/";
const char* files_down_data_path="../DATA/FILES_DATA/";


//const char* info_files_path="/home/oem/Desktop/main_network_project/client_side/DATA/DN_FILES/";//adresa folderului unde sunt pastrate fisierele cu informatii
//const char* files_down_data_path="/home/oem/Desktop/main_network_project/client_side/DATA/FILES_DATA/";
//trebuie sa aducem modificari structurii file_data sa inseram un camp pentru retinerea adresei fisierului in retea
/*
struct file_data{
    char tmp_file_name[100];//numele fisierului 
    char file_path[200];//adresa fisierului de implementat
    char file_name[100];//numele fisierului
    bool file_stat;//starea fsierului (descarcat/sau nu);
    bool file_access_upload;//permisiune de acces pentru upload
    bool file_access_download;//permisiune pentru continuarea
    long long file_size;//dimensiunea fisierului
    long long c_file_size;//dimensiunea fisierului pe perioada descarcarii
    int num_of_peers_d;//numarul de clienti utilizati pentru download de procesul curent(momentan optional)
    int num_of_peers_u;//numarul de clienti care utilizeaza procesul curent pentru descarcarea unui fisier
};
*/

void get_basename(char* file_path,char* basename){
    int string_size=strlen(basename);
    int file_path_size=strlen(file_path);
    for(int i=file_path_size-string_size;i<file_path_size;i++){
        basename[string_size-(file_path_size-i)]=file_path[i];
    }
}

bool insert_extern_file(const char*file_address,const char* file_name,file_data &my_file_info){
    struct stat my_file_stat;
    if(stat(file_address,&my_file_stat)!=-1){
        char full_tmp_path[200];
        const char *tmp_file_basename;
        strcpy(full_tmp_path,info_files_path);
        strcat(full_tmp_path,"dexXXXXXX");
        int file_d=mkstemp(full_tmp_path);
        char* basename=new char[strlen("dexXXXXXX")];
        strcpy(basename,"dexXXXXXX");
        get_basename(full_tmp_path,basename);
        strcpy(my_file_info.file_path,file_address);//adresa folderrului care contine documentul
        strcpy(my_file_info.tmp_file_name,basename);//numele fisierului temporar nou creat
        strcpy(my_file_info.file_name,file_name);//numele fisiserului 
         
        //my_file_info.file_size//aceste doua functii au fost apelate anterior de catre functia care a apelat
        // my_file_info.c_file_size
        my_file_info.file_stat=true;
        my_file_info.file_access_download=true;
        my_file_info.file_access_upload=true;

        write_file_state_2(file_d,my_file_info);
       
        delete basename;
        return true;
    }else{
        cout<<"FILE_NOT_FOUND"<<endl;
        return false;
    }
}

bool insert_file(const char* file_name,file_data &my_file_info){
    struct stat my_file_stat;
    char* temp_path=new char[strlen(info_files_path)+strlen("dexXXXXXX")];
    strcpy(temp_path,info_files_path);
    strcat(temp_path,"dexXXXXXX");
    int file_d=mkstemp(temp_path);
    char* basename=new char[strlen("dexXXXXXX")];
    strcpy(basename,"dexXXXXXX");
    get_basename(temp_path,basename);
    strcpy(my_file_info.file_name,file_name);
    strcpy(my_file_info.tmp_file_name,basename);
    strcpy(my_file_info.file_path,files_down_data_path);
    my_file_info.file_access_download=true;
    my_file_info.file_access_upload=true;
    my_file_info.file_size=0;//dimensiunea fisierului
    my_file_info.c_file_size=0;//initial zero
    //acestea trebuie setate inainte de trimiterea lor in aceasta functie s

    write_file_state_2(file_d,my_file_info);
    cout<<"Fisier inserat"<<endl;
    return true;
}



/*
int main(){

        file_data file_info;
        for(int i=1;i<=5;i++){
            insert_file("/home/oem/Desktop/dlm.txt","dlm.txt",file_info);
        }



    return 0;
}


*/

