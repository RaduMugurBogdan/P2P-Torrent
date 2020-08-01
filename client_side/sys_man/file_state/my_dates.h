

/*
    definitiile perzente in acest fisier sunt uitile aplicatiei client
pentru incarcarea in memorie a datelor despre fisierele prezente pe dispozitiv

*/
/*
#include<iostream>
#include<string.h>
using namespace std;
*/

struct file_data{
    char tmp_file_name[100];//numele fisierului 
    char file_path[200];//adresa fisierului descarcat
    char file_name[100];//numele fisierului
    bool file_stat;//starea fsierului (descarcat/sau nu);
    bool file_access_upload;//permisiune de acces pentru upload
    bool file_access_download;//permisiune pentru continuarea
    long long file_size;//dimensiunea fisierului
    long long c_file_size;//dimensiunea fisierului pe perioada descarcarii
    int num_of_peers_d;//numarul de clienti utilizati pentru download de procesul curent(momentan optional)
    int num_of_peers_u;//numarul de clienti care utilizeaza procesul curent pentru descarcarea unui fisier
};

struct files_list{
    file_data file_value;
    files_list* next_elem;
};

class main_list{
    private:
        int files_number=0;//number of files in list
        files_list* fl_list=NULL;
        pthread_mutex_t my_lock;
    public:
        int get_files_number(){
            return this->files_number;
        }
        pthread_mutex_t get_lock_var(){
            return this->my_lock;
        }
        void show_files_list(){
            pthread_mutex_lock(&my_lock);
            cout<<"\n----------------------------------------\n"<<endl;
            files_list* iterator=this->fl_list;
            while(iterator!=NULL){
                cout<<iterator->file_value.file_name<<endl;
                iterator=iterator->next_elem;
            }
            cout<<"\n----------------------------------------\n"<<endl;
            pthread_mutex_unlock(&my_lock);
        }
        
        files_list* get_files_list(){
            return this->fl_list;
        }

        file_data* get_file_stat_ref(const char* file_name){
            pthread_mutex_lock(&my_lock);
            files_list* iterator=this->fl_list;
            while(iterator!=NULL){
                if(strcmp(iterator->file_value.file_name,file_name)==0){
                    pthread_mutex_unlock(&my_lock);
                    return &(iterator->file_value);
                }
                iterator=iterator->next_elem;
            }
            pthread_mutex_unlock(&my_lock);
            return NULL;
        }

        bool search_for_file(const char* file_name){
            pthread_mutex_lock(&my_lock);
            files_list* iterator=this->fl_list;
            while(iterator!=NULL){
                if(strcmp(iterator->file_value.file_name,file_name)==0){
                     pthread_mutex_unlock(&my_lock);
                    return true;
                }else{
                    iterator=iterator->next_elem;
                }
            }
            pthread_mutex_unlock(&my_lock);
            return false;
        }   
        bool check_state(const char*file_name){  ///functia este utila pe partea de server ,atunci cand sunt inserate noi fisiere ,ca acestea sa nu se repete
            pthread_mutex_lock(&my_lock);
            files_list* aux_pointer=this->fl_list;
            while(aux_pointer!=NULL){
                if(strcmp(aux_pointer->file_value.file_name,file_name)==0){
                    pthread_mutex_unlock(&my_lock);
                    return aux_pointer->file_value.file_stat;
                }
                aux_pointer=aux_pointer->next_elem;
            }
            return false;
            pthread_mutex_unlock(&my_lock);
        }

        void insert(file_data ins_value){
             pthread_mutex_lock(&my_lock);
            if(this->fl_list==NULL){
                this->fl_list=new files_list;
                this->fl_list->file_value=ins_value;
                fl_list->next_elem=NULL;
            }else{
                files_list* aux_list=new files_list;
                aux_list->file_value=ins_value;
                aux_list->next_elem=this->fl_list;
                this->fl_list=aux_list;
            }
            this->files_number++;
            pthread_mutex_unlock(&my_lock);
        }
        
        files_list* remove_file(files_list* del_object){
            pthread_mutex_lock(&my_lock);
            files_list* list_iterator=this->fl_list;
            files_list* last_step=this->fl_list;
            while(list_iterator!=NULL && list_iterator!=del_object){
                last_step=list_iterator;
                list_iterator=list_iterator->next_elem;
            }
            if(list_iterator==del_object){
                if(del_object==fl_list){
                    this->fl_list=this->fl_list->next_elem;
                }else{
                    last_step->next_elem=del_object->next_elem;
                }
                pthread_mutex_unlock(&my_lock);
                return del_object;
            }else{
                pthread_mutex_unlock(&my_lock);
                return NULL;
            }
        }


        bool delete_el(unsigned int index){
            pthread_mutex_lock(&my_lock);
            if(index==0 || index>this->files_number){
                return false;
            }else{
                if(this->files_number!=1){
                    files_list* del_object;
                    files_list* aux_fl=this->fl_list;
                    for(int i=1;i<index;i++){
                        aux_fl=aux_fl->next_elem;
                    }
                    del_object=aux_fl->next_elem;
                    aux_fl->next_elem=aux_fl->next_elem->next_elem;
                    delete del_object;
                }else{
                    delete this->fl_list;
                    this->fl_list=NULL;
                }
                
                this->files_number--;
                pthread_mutex_unlock(&my_lock);
                return true;
            }
        }  
        main_list(){
            //empty block
        }

};



