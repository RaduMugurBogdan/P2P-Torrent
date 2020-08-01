
extern main_list upload_list;
extern main_list download_list;



void send_file_to_server(int server_socket,const char* file_name){
    insert_file_ref_input input;
    strcpy(input.file_name,file_name);
    call_server(server_socket,INSERT_FILE_REF,(void*)&input);
}

void send_files_to_server(int server_conn_socket,main_list& upload_list){
    pthread_mutex_t my_lock=upload_list.get_lock_var();
    pthread_mutex_lock(&my_lock);
    files_list* fl_iterator=upload_list.get_files_list();
    insert_file_ref_input input;
    while(fl_iterator!=NULL){
        if(fl_iterator->file_value.file_access_upload==true){
            strcpy(input.file_name,fl_iterator->file_value.file_name);
            call_server(server_conn_socket,INSERT_FILE_REF,(void*)&input);   
        }
        fl_iterator=fl_iterator->next_elem;
    }
    pthread_mutex_unlock(&my_lock);
}

void save_lists(main_list& download_list,main_list& upload_list){
    pthread_mutex_t my_lock=upload_list.get_lock_var();
    pthread_mutex_lock(&my_lock);
    files_list* fl_iterator=download_list.get_files_list();
    char file_path[200];
    while(fl_iterator!=NULL){
        strcpy(file_path,info_files_path);
        strcat(file_path,fl_iterator->file_value.tmp_file_name);
        write_file_state(fl_iterator->file_value,file_path);
        fl_iterator=fl_iterator->next_elem;
    }
    pthread_mutex_unlock(&my_lock);
    my_lock=download_list.get_lock_var();
    pthread_mutex_lock(&my_lock);
    fl_iterator=upload_list.get_files_list();
    while(fl_iterator!=NULL){
        strcpy(file_path,info_files_path);
        strcat(file_path,fl_iterator->file_value.tmp_file_name);
        write_file_state(fl_iterator->file_value,file_path);
       // cout<<file_path<<" salvat"<<endl;
        fl_iterator=fl_iterator->next_elem;
    }
    pthread_mutex_unlock(&my_lock);
}


void show_local_files_details(main_list &download_list,main_list &upload_list){
    files_list* fl_iterator=NULL;
    file_data state;
    pthread_mutex_t my_lock_var=download_list.get_lock_var();
    pthread_mutex_lock(&my_lock_var);
    fl_iterator=upload_list.get_files_list();

    cout<<"Upload list"<<endl;
    while(fl_iterator!=NULL){
        state=fl_iterator->file_value;
        cout<<"\t\n------------------------------\n"<<endl;
        cout<<"\tfile_path="<<state.file_path<<endl;
        cout<<"\ttmp_file_name="<<state.tmp_file_name<<endl;
        cout<<"\tfile_name="<<state.file_name<<endl;
        cout<<"\tfile_stat="<<state.file_stat<<endl;
        cout<<"\tfile_access_upload="<<state.file_access_upload<<endl;
        cout<<"\tfile_access_download="<<state.file_access_download<<endl;
        cout<<"\tfile_size="<<state.file_size<<endl;
        cout<<"\tc_file_size="<<state.c_file_size<<endl;
        cout<<"\t\n------------------------------\n"<<endl;
        fl_iterator=fl_iterator->next_elem;
    }
    pthread_mutex_unlock(&my_lock_var);
    cout<<"\n--------------------------------------------------------------------\n";
    cout<<"--------------------------------------------------------------------\n"<<endl;
    cout<<"Download list"<<endl;
    pthread_mutex_lock(&my_lock_var);
    fl_iterator=download_list.get_files_list();
    while(fl_iterator!=NULL){
        state=fl_iterator->file_value;
        cout<<"\t\n------------------------------\n"<<endl;
        cout<<"\tfile_path="<<state.file_path<<endl;
        cout<<"\ttmp_file_name="<<state.tmp_file_name<<endl;
        cout<<"\tfile_name="<<state.file_name<<endl;
        cout<<"\tfile_stat="<<state.file_stat<<endl;
        cout<<"\tfile_access_upload="<<state.file_access_upload<<endl;
        cout<<"\tfile_access_download="<<state.file_access_download<<endl;
        cout<<"\tfile_size="<<state.file_size<<endl;
        cout<<"\tc_file_size="<<state.c_file_size<<endl;
        cout<<"\t\n------------------------------\n"<<endl;
        fl_iterator=fl_iterator->next_elem;
    }
    pthread_mutex_unlock(&my_lock_var);
}


void show_local_files(){
    pthread_mutex_t my_lock=upload_list.get_lock_var();
    pthread_mutex_lock(&my_lock);
    files_list* list_iterator=upload_list.get_files_list();
    cout<<"Uploading process files"<<endl;
    while(list_iterator!=NULL){
        cout<<endl;
        cout<<"\t\t"<<list_iterator->file_value.file_name<<endl;
        cout<<"\t\tFile_size:"<<list_iterator->file_value.file_size;
        cout<<endl;
        list_iterator=list_iterator->next_elem;
    }
    pthread_mutex_unlock(&my_lock);
    cout<<"\n--------------------------------------------------------------\n"<<endl;
    cout<<"Downloading process files"<<endl;
    my_lock=download_list.get_lock_var();
    pthread_mutex_lock(&my_lock);
    list_iterator=download_list.get_files_list();
    while(list_iterator!=NULL){
        cout<<endl;
        cout<<"\t\t"<<list_iterator->file_value.file_name<<endl;
        cout<<"\t\tFile_size: "<<list_iterator->file_value.file_size;
        cout<<endl;
        list_iterator=list_iterator->next_elem;
    }
    pthread_mutex_unlock(&my_lock);  
}