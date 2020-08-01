

void show_file_state(file_data state){
    cout<<"\n------------------------------\n"<<endl;
    cout<<"file_path="<<state.file_path<<endl;
    cout<<"tmp_file_name="<<state.tmp_file_name<<endl;
    cout<<"file_name="<<state.file_name<<endl;
    cout<<"file_stat="<<state.file_stat<<endl;
    cout<<"file_access_upload="<<state.file_access_upload<<endl;
    cout<<"file_access_download="<<state.file_access_download<<endl;
    cout<<"file_size="<<state.file_size<<endl;
    cout<<"c_file_size="<<state.c_file_size<<endl;
    cout<<"num_of_peers_d="<<state.num_of_peers_d<<endl;
    cout<<"num_of_peers_u="<<state.num_of_peers_u<<endl;
    cout<<"\n------------------------------\n"<<endl;
}


bool write_file_state_2(int file_d,file_data state){
    if(file_d!=-1){    
        int tmp_file_name_size=strlen(state.tmp_file_name)+1;
        int file_name_size=strlen(state.file_name)+1;
        int file_path_size=strlen(state.file_path)+1;
        write(file_d,&file_path_size,sizeof(int));//write sizeof file_path    
        write(file_d,state.file_path,file_path_size);//write file_path_name
        write(file_d,&tmp_file_name_size,sizeof(int));//write tmp_file_name string size
        write(file_d,state.tmp_file_name,tmp_file_name_size);//write tmp_file_name  string
        write(file_d,&file_name_size,sizeof(int));//write file_name string size
        write(file_d,state.file_name,file_name_size);//write file_name string
        write(file_d,&state.file_stat,sizeof(bool));//write file state
        write(file_d,&state.file_access_upload,sizeof(bool));//write upload permision access
        write(file_d,&state.file_access_download,sizeof(bool));//write download permision access
        write(file_d,&state.file_size,sizeof(long long));//write file_size
        write(file_d,&state.c_file_size,sizeof(long long));//write curent_file_size
        write(file_d,&state.num_of_peers_d,sizeof(int));//write dowload peers 
        write(file_d,&state.num_of_peers_u,sizeof(int));//wrtie upload peers
        close(file_d);
        return true;
    }else{
        return false;
    }   
}

bool write_file_state(file_data state,const char* file_name){
    int file_d=open(file_name,O_WRONLY|O_CREAT,0664);
    if(file_d!=-1){ 
        int tmp_file_name_size=strlen(state.tmp_file_name)+1;
        int file_name_size=strlen(state.file_name)+1;
        int file_path_size=strlen(state.file_path)+1;
        write(file_d,&file_path_size,sizeof(int));//write sizeof file_path    
        write(file_d,state.file_path,file_path_size);//write file_path_name
        write(file_d,&tmp_file_name_size,sizeof(int));//write tmp_file_name string size
        write(file_d,state.tmp_file_name,tmp_file_name_size);//write tmp_file_name  string
        write(file_d,&file_name_size,sizeof(int));//write file_name string size
        write(file_d,state.file_name,file_name_size);//write file_name string
        write(file_d,&state.file_stat,sizeof(bool));//write file state
        write(file_d,&state.file_access_upload,sizeof(bool));//write upload permision access
        write(file_d,&state.file_access_download,sizeof(bool));//write download permision access
        write(file_d,&state.file_size,sizeof(long long));//write file_size
        write(file_d,&state.c_file_size,sizeof(long long));//write curent_file_size
        write(file_d,&state.num_of_peers_d,sizeof(int));//write dowload peers 
        write(file_d,&state.num_of_peers_u,sizeof(int));//wrtie upload peers
        close(file_d);
        return true;
    }else{
        return false;
    }   
}

bool read_file_state(file_data &state,const char* file_name){
    int file_d=open(file_name,O_RDONLY);
    if(file_d!=-1){    
        int tmp_file_name_size;
        int file_name_size;
        int file_path_size;
        read(file_d,&file_path_size,sizeof(int));//read the size of the path
        read(file_d,state.file_path,file_path_size);//read the file path
        read(file_d,&tmp_file_name_size,sizeof(int));//read tmp_file_name string size
        read(file_d,state.tmp_file_name,tmp_file_name_size);//read tmp_file_name string
        read(file_d,&file_name_size,sizeof(int));//read file_name string size
        read(file_d,state.file_name,file_name_size);//read file_name string
        read(file_d,&state.file_stat,sizeof(bool));//read file_stat
        read(file_d,&state.file_access_upload,sizeof(bool));//read file_access_upload
        read(file_d,&state.file_access_download,sizeof(bool));//read file_access_download
        read(file_d,&state.file_size,sizeof(long long));//read file_size
        read(file_d,&state.c_file_size,sizeof(long long));//read curent file size
        read(file_d,&state.num_of_peers_d,sizeof(int));//read download peers
        read(file_d,&state.num_of_peers_u,sizeof(int));//read upload peers
        close(file_d);
        return true;
    }else{
        return false;
    }
}

void pass_files(const char* dir_path,main_list &download_list,main_list &upload_list){//functia care citeste datele din fiecare fisier si le insereaza intr-o list de fisiere 
    file_data my_state;
    DIR* my_dir=NULL;
    dirent* c_dir;
    my_dir=opendir(dir_path);
    char sec_path[200];
    char file_data_ref[200];
    struct stat f_stat;
    if(my_dir!=NULL){   
        while((c_dir=readdir(my_dir))!=NULL){
            if(strcmp(c_dir->d_name,"..")!=0 && strcmp(c_dir->d_name,".")!=0){
                strcpy(sec_path,dir_path);
                strcat(sec_path,"/");
                strcat(sec_path,c_dir->d_name);
                read_file_state(my_state,sec_path);
                strcpy(file_data_ref,my_state.file_path);
                strcat(file_data_ref,"/");
                strcat(file_data_ref,my_state.file_name);
                if(stat(file_data_ref,&f_stat)!=-1){
                    if(upload_list.search_for_file(my_state.file_name)!=true && download_list.search_for_file(my_state.file_name)!=true){
                        my_state.c_file_size=f_stat.st_size;
                        my_state.file_size=my_state.c_file_size;
                        if(my_state.file_stat==false &&  my_state.c_file_size<my_state.file_size){
                            download_list.insert(my_state);
                        }else{
                            upload_list.insert(my_state);
                        }
                    }
                }
            }
        }
    }else{
        cout<<"PATH INEXISTENT"<<endl;
    }
    closedir(my_dir);
}
