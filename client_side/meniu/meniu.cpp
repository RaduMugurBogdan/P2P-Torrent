#include<iostream>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<vector>
#include<sys/stat.h>
using namespace std;
#include"../sys_man/file_state/my_dates.h"
#include"../sys_man/file_state/tmp_files_data.h"
#include"../sys_man/file_state/sys_man.h"
#include"../client_kernel/call_server.h"
#include"../client_kernel/download_process.h"
#include"../client_kernel/upload_process.h"
#include"../client_kernel/serv_connection.h"
#include"../client_kernel/client_kernel.h"
#include"../sys_man/insert_file/insert_file.h"
#include"show_local_files.h"
#include"data_options.h"



class main_menu{
    private:
        char server_ip_address[20];
        short server_port_number;

        short upload_port=2036;
        short download_port=2096;

        int server_conn_socket;

        pthread_t ck_thread;

        void show_main_menu(){

            //init_lists
            //generate_files(info_files_path,10);
            pass_files(info_files_path,download_list,upload_list);
            this->server_conn_socket=-1;
            while(this->server_conn_socket==-1){
                cout<<"IP ADDRESS:";cin.get(this->server_ip_address,20);
                cout<<"PORT:";cin>>this->server_port_number;
                cin.get();
                this->server_conn_socket=server_connection(this->server_ip_address,this->server_port_number);
                if(this->server_conn_socket==-1){
                        cout<<"SERVER CONNECTION ERROR -> INVALID INPUT DATA"<<endl;
                        cin.get();
                        system("clear");
                }
            }
            

            client_kernel_param ck_param;
            ck_param.server_conn_socket=this->server_conn_socket;
            
            ck_param.upload_server_port=this->upload_port;//port server de upload
            ck_param.download_server_port=this->download_port;//port client download

            ck_param.app_control.upload_control=new bool;
            
            ck_param.app_control.download_control=new bool;
            *(ck_param.app_control.upload_control)=true;
            *(ck_param.app_control.download_control)=true;
            
            
            if(pthread_create(&ck_thread,NULL,client_kernel,(void*)&ck_param)==-1){
                cout<<"CLIENT KERNEL THREAD ERROR"<<endl;
            }
            sleep(2);
            send_files_to_server(this->server_conn_socket,upload_list);
            system("clear");

            char mm_ans_str[10];
            int mm_ans;
            bool general_aplication_flag=true;
            while(general_aplication_flag==true){   
                cout<<"\n\t\t\tMAIN MENU\t\t\t"<<endl;
                cout<<"\n-------------------------------------------------------------\n"<<endl;
                cout<<"1.Show active files list in network"<<endl;
                cout<<"2.Show local files"<<endl;
                cout<<"3.Download data"<<endl;
                cout<<"4.Upload data"<<endl;
                cout<<"5.Show local files states"<<endl;
                cout<<"6.Uploading/Doanloading data opntions"<<endl;
                cout<<"7.Exit"<<endl;
                cout<<"\n-------------------------------------------------------------\n"<<endl;
                //input side
                cout<<"Insert the command code: ";
                cin.get(mm_ans_str,sizeof(mm_ans_str));
                cin.get();
                //--------------------------------------
                //compute input side
                mm_ans=atoi(mm_ans_str);
                switch(mm_ans){
                    case 0:{
                        cout<<endl<<"Unknown value."<<endl;
                        cin.ignore();
                        system("clear");
                        break;
                    }
                    case 1:{
                        get_files_list_output* output_test=(get_files_list_output*)call_server(this->server_conn_socket,GET_FILES_LIST,NULL);
                        system("clear");

                        if(output_test!=NULL){
                            get_files_list_output output=*((get_files_list_output*)output_test);    
                            if(strcmp(output.files_list[0],"")==0){
                                cout<<"EMPTY LIST"<<endl;
                            }else{    
                                int index=0;
                                while(strcmp(output.files_list[index],"")!=0){
                                    cout<<index+1<<". "<<output.files_list[index]<<endl;
                                    index++;
                                }
                                cout<<"\n-----------------------------------------------\n"<<endl;
                            }
                        }else{
                            cout<<"EMPTY LIST"<<endl;
                        }    
                        cin.ignore();
                        system("clear");
                        break;
                    };
                    case 2:{
                        system("clear");
                        show_local_files();
                        cin.ignore();
                        system("clear");
                        break;
                    };
                    case 3:{
                        get_files_list_output* test_output=(get_files_list_output*)call_server(this->server_conn_socket,GET_FILES_LIST,(void*)NULL);
                        if(test_output!=NULL){   
                            get_files_list_output output=*(test_output);
                            system("clear");
                            if(strcmp(output.files_list[0],"")==0){
                                cout<<"FILES NOT FOUND"<<endl;
                            }else{    
                                int index=0;
                                while(strcmp(output.files_list[index],"")!=0){
                                    cout<<index+1<<". "<<output.files_list[index]<<endl;
                                    index++;
                                }
                                cout<<"\n-----------------------------------------------\n"<<endl;
                                int file_index;
                                cout<<"Introduce file index: "<<endl;cin>>file_index;
                                if(file_index>0 && file_index<=index){
                                    file_index--;
                                    if(upload_list.search_for_file(output.files_list[file_index])==true || download_list.search_for_file(output.files_list[file_index])){
                                        cout<<endl;
                                        cout<<"File already exists in system"<<endl;
                                        cin.ignore();
                                    }else{
                                        file_data new_file;
                                        insert_file(output.files_list[file_index],new_file);
                                        char insert_file_path[200];
                                        strcpy(insert_file_path,files_down_data_path);
                                        strcat(insert_file_path,output.files_list[file_index]);
                                        open(insert_file_path,O_CREAT,0664);
                                        new_file.num_of_peers_d=0;
                                        download_list.insert(new_file);
                                    }
                                }else{
                                    cout<<endl;
                                    cout<<"WRONG FILE INDEX"<<endl;
                                }
                            }
                        }else{
                            cout<<"Empty list"<<endl;
                        }
                        cin.ignore();
                        system("clear");
                        break;
                    };
                    case 4:{
                        system("clear");
                        char file_path[200];
                        cout<<"Insert new file in netowrk"<<endl;
                        cout<<"Insert file path:"<<endl;
                        cin.get(file_path,200);cin.get();
                        struct stat my_file_stat;
                        if(stat(file_path,&my_file_stat)!=-1 && S_ISREG(my_file_stat.st_mode)==true){
                            char* file_name;
                            file_name=basename(file_path);
                            if(upload_list.search_for_file(file_name)==true){
                                cout<<"THE FILE ALREADY EXISTS IN SYSTEM\n"<<endl;
                            }else{
                                int rev_index=strlen(file_path);
                                while(file_path[rev_index]!='/' && rev_index>=0){
                                    rev_index--;
                                }
                                file_path[rev_index]='\0';
                                file_data my_file_data;
                                my_file_data.c_file_size=my_file_data.file_size=my_file_stat.st_size;
                                if(insert_extern_file(file_path,file_name,my_file_data)==true){
                                    upload_list.insert(my_file_data);
                                    send_file_to_server(this->server_conn_socket,file_name);
                                }
                            }
                        }else{
                            cout<<"Path not found"<<endl;
                        }
                        cin.ignore();
                        system("clear");
                        break;
                    };
                    case 5:{
                        show_local_files_details(download_list,upload_list);
                        cin.ignore();
                        system("clear");
                        break;
                    };
                    case 6:{
                        data_options_menu(this->server_conn_socket);
                        break;
                    };
                    case 7:{
                        general_aplication_flag=false;
                        *(ck_param.app_control.upload_control)=false;
                        *(ck_param.app_control.download_control)=false;
                        close_upload_thread(this->upload_port);
                        save_lists(download_list,upload_list);
                        break;
                    }




                    /***********************************************************/
                   /*
                    case 8:{
                        inc_upl_no_input input;
                        strcpy(input.file_name,"p_thread");
                        inc_upl_no_output* output=NULL; 
                        output=(inc_upl_no_output*)call_server(this->server_conn_socket,INC_UPL_NO,(void*)&input);
                        if(output!=NULL){
                            cout<<"server-ul a trimis "<<output->answare<<endl;
                            cin.ignore();
                        }else{
                            cout<<"problema de la server"<<endl;
                            cin.ignore();
                        }
                    break;
                    };   
                    case 9:{
                        dec_upl_no_input input;
                        strcpy(input.file_name,"p_thread");
                        dec_upl_no_output* output=NULL;
                        output=(dec_upl_no_output*)call_server(this->server_conn_socket,DEC_UPL_NO,(void*)&input);
                        if(output!=NULL){
                            cout<<"server-ul a trimis "<<output->answare<<endl;
                            cin.ignore();
                        }else{
                            cout<<"problema de la server"<<endl;
                            cin.ignore();
                        }
                        break;
                    }
                    */
                    /*********************************************************/



                }
                system("clear");
            }
            pthread_join(ck_thread,NULL);
            close(this->server_conn_socket);
        }
    public:
        main_menu(){
            this->show_main_menu();          
        }    
};

void load_menu(){
    main_menu main_menu_object;
}

int main(){
    load_menu();

    return 0;
}


