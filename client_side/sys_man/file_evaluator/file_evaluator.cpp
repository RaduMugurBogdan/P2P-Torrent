#include<iostream>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include"../file_state/my_dates.h"
using namespace std;


/*
struct file_data{
    char tmp_file_name[100]//numele fisierului 
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


bool gen_file_eval(const char* file_root,const char* file_name,const char* stat_file_root){//primul parametru este adresa fisierului pe care dorim sa il inseram
                                                                       //al doilea parametru este adresa folderului unde sunt fisierele cu informatii
                                                             //pentru a crea fisierul pe baza structurii date          
    struct stat my_file_stat;
    if(stat(file_root,&my_file_stat)==-1){
        cout<<"THE FILE DOSEN'T EXIST OR THE PATH IS WRONG"<<endl;
        exit(-1);
    }else{
        file_data my_file;
        strcpy(my_file.file_path,file_root);
        my_file.file_stat=true;
        my_file.file_access_upload=true;
        my_file.file_access_download=true;
        my_file.file_size=my_file_stat.st_size;//dimensiunea fisierului
        my_file.c_file_size=my_file_stat.st_size;//dimensiunea curenta a fisieruli (este egala cu cea a campului file_size)
        //my_file.num_of_peers_d=0;//nu sunt utile campurile 
        //my_file.num_of_peers_u=0;//ulterior fisierul va trebui mentionat serverului,iar aceasta valoare este 
    }
}









bool insert_file(){
    return true;
}




int main(){
        struct stat my_stat_file;
        stat("test.txt",&my_stat_file);
        cout<<"dimensiunea fisierului este "<<my_stat_file.st_size<<endl;
        cout<<"\n---------------------------------\n"<<endl;
    return 0;
}