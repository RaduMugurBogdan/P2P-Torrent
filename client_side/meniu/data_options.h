
#define UPL_FLAG 1
#define DOW_FLAG 2

//functia este activa ca freind pentru clasa client
void delete_from_server(files_list* list,int server_conn_socket){
    delete_file_ref_input input;
    strcpy(input.file_name,list->file_value.file_name);
    call_server(server_conn_socket,DELETE_FILE_REF,(void*)&input);
}
void insert_to_server(files_list* list,int server_conn_socket){
    insert_file_ref_input input;
    strcpy(input.file_name,list->file_value.file_name);
    call_server(server_conn_socket,INSERT_FILE_REF,(void*)&input);
}

void sub_menu(files_list* list,int server_conn_socket,int flag){
    int mm_ans;    
    char mm_ans_str[10];
    bool act_flag=true;
    while(act_flag==true){
        system("clear");
        cout<<"Data access menu"<<endl;
        cout<<"\t1.Set Upload access"<<endl;
        cout<<"\t2.Set Download access"<<endl;
        cout<<"\t3.Back to menu"<<endl;
        cout<<"Insert the file code:";
        cin.get(mm_ans_str,sizeof(mm_ans_str));
        cin.get();
        mm_ans=atoi(mm_ans_str);
        switch(mm_ans){
            case 0:{
                cout<<"Unknown command"<<endl;
                cin.ignore();
                break;
            }
            case 1:{
                system("clear");
                cout<<"Set Uploade option [insert true/false/exit]:\n"<<endl;
                cin.get(mm_ans_str,sizeof(mm_ans_str));
                cin.get();
                int input_size=strlen(mm_ans_str);
                for(int i=0;i<input_size;i++){
                    mm_ans_str[i]=toupper(mm_ans_str[i]);        
                }
                if(strcmp("TRUE",mm_ans_str)==0){
                    if(flag==UPL_FLAG){
                        if(list->file_value.file_access_upload==false){
                            list->file_value.file_access_upload=true;
                            insert_to_server(list,server_conn_socket);
                        }
                    }else{
                        list->file_value.file_access_upload=true;
                    }
                }else if(strcmp("FALSE",mm_ans_str)==0){
                    if(flag==UPL_FLAG){
                        if(list->file_value.file_access_upload=true){
                            list->file_value.file_access_upload=false;
                            delete_from_server(list,server_conn_socket);
                        }
                    }else{
                        list->file_value.file_access_upload=false;
                    }    
                }else if(strcmp("EXIT",mm_ans_str)==0){
                    act_flag=false;
                }else{
                    cout<<"Unknown answare"<<endl;
                    act_flag=false;
                    cin.ignore();
                }
                break;
            }   
            case 2:{
                system("clear");
                cout<<"Set Download option [insert true/false/exit]:\n"<<endl;
                cin.get(mm_ans_str,sizeof(mm_ans_str));
                cin.get();
                int input_size=strlen(mm_ans_str);
                for(int i=0;i<input_size;i++){
                    mm_ans_str[i]=toupper(mm_ans_str[i]);        
                }
                if(strcmp("TRUE",mm_ans_str)==0){
                    list->file_value.file_access_download=true;
                }else if(strcmp("FALSE",mm_ans_str)==0){
                    list->file_value.file_access_download=false;
                }else if(strcmp("EXIT",mm_ans_str)==0){
                    act_flag=false;
                }else{
                    cout<<"Unknown answare"<<endl;
                    act_flag=false;
                    cin.ignore();
                }
                break;
            }     
            case 3:{
                act_flag=false;
                break;
            }
        };
    }
}       

void data_options_menu_upl(int server_conn_socket){
    pthread_mutex_t my_lock_var=upload_list.get_lock_var();
    pthread_mutex_lock(&my_lock_var);
    bool act_flag=true;
    int mm_ans;    
    char mm_ans_str[10];

    while(act_flag==true){
        files_list* fl_iterator=upload_list.get_files_list();
        int files_n=upload_list.get_files_number();
        int file_index=1;
        system("clear");
        cout<<"Uploaded files \n";
        while(fl_iterator!=NULL){
            cout<<"\t"<<file_index<<"."<<fl_iterator->file_value.file_name<<endl;
            cout<<"\t\t"<<"download access:"<<fl_iterator->file_value.file_access_download<<endl;
            cout<<"\t\t"<<"uploadload access:"<<fl_iterator->file_value.file_access_upload<<endl;
            fl_iterator=fl_iterator->next_elem;
            file_index++;
            cout<<endl<<endl;
        }        
        cout<<"\n------------------------------------------------\n"<<endl;
        cout<<"Insert the file code:";
        cin.get(mm_ans_str,sizeof(mm_ans_str));
        cin.get();
        if(strcmp("exit",mm_ans_str)==0){
            break;
        }else{
            mm_ans=atoi(mm_ans_str);
            if(mm_ans==0){
                cout<<"Unknown command"<<endl;
                cin.ignore();
            }else{
                if(mm_ans>0 && mm_ans<=files_n){
                    
                    fl_iterator=upload_list.get_files_list();
                    for(int i=1;i<mm_ans;i++){
                        fl_iterator=fl_iterator->next_elem;
                    }                    
                    sub_menu(fl_iterator,server_conn_socket,UPL_FLAG);
                }else{
                    cout<<"Unknown command"<<endl;
                    cin.ignore();
                }
            }
        }
                   
    }
    pthread_mutex_unlock(&my_lock_var);
}




void data_options_menu_down(int server_conn_socket){
    pthread_mutex_t my_lock_var=download_list.get_lock_var();
    pthread_mutex_lock(&my_lock_var);
    bool act_flag=true;
    int mm_ans;    
    char mm_ans_str[10];

    while(act_flag==true){
        files_list* fl_iterator=download_list.get_files_list();
        int files_n=download_list.get_files_number();
        int file_index=1;
        system("clear");
        cout<<"Downloaded files \n";
        while(fl_iterator!=NULL){
            cout<<"\t"<<file_index<<"."<<fl_iterator->file_value.file_name<<endl;
            cout<<"\t\t"<<"download access:"<<fl_iterator->file_value.file_access_download<<endl;
            cout<<"\t\t"<<"uploadload access:"<<fl_iterator->file_value.file_access_upload<<endl;
            fl_iterator=fl_iterator->next_elem;
            file_index++;
            cout<<endl<<endl;
        }        
        cout<<"\n------------------------------------------------\n"<<endl;
        cout<<"Insert the file code:";
        cin.get(mm_ans_str,sizeof(mm_ans_str));
        cin.get();
        if(strcmp("exit",mm_ans_str)==0){
            break;
        }else{
            mm_ans=atoi(mm_ans_str);
            if(mm_ans==0){
                cout<<"Unknown command"<<endl;
                cin.ignore();
            }else{
                if(mm_ans>0 && mm_ans<=files_n){
                    
                    fl_iterator=download_list.get_files_list();
                    for(int i=1;i<mm_ans;i++){
                        fl_iterator=fl_iterator->next_elem;
                    }                    
                    sub_menu(fl_iterator,server_conn_socket,DOW_FLAG);
                }else{
                    cout<<"Unknown command"<<endl;
                    cin.ignore();
                }
            }
        }
                   
    }
    pthread_mutex_unlock(&my_lock_var);
}

void data_options_menu(int server_conn_socket){
    bool act_val=true;
    while(act_val==true){
        system("clear");
        cout<<"DATA OPTIONS MENU"<<endl;
        cout<<"1.Uploaded files options"<<endl;
        cout<<"2.Downloaded files options"<<endl;
        cout<<"3.Back to main menu"<<endl;
        char mm_ans_str[10];
        int mm_ans;
        cout<<"Insert the command code: ";
        cin.get(mm_ans_str,sizeof(mm_ans_str));
        cin.get();
        mm_ans=atoi(mm_ans_str);
        
        switch(mm_ans){
            case 0:{
                cout<<"Unknown command"<<endl;
                cin.ignore();
                break;
            }
            case 1:{
                data_options_menu_upl(server_conn_socket);
                break;
            }
            case 2:{
                break;
                data_options_menu_down(server_conn_socket);
            }
            case 3:{
                act_val=false;
                break;
            }

        };
    }

    system("clear");

}