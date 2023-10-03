#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Autonomie Autonomie;
struct Autonomie{
    Autonomie *left;
    Autonomie *right;
    Autonomie *parent;
    
    Autonomie *root_ricariche;
    
    int key;
    int macchine;
    int conta;
};

typedef struct matr matr;
struct matr{
    int key;
    int max;
};


typedef struct info info;
struct info{
    int path;
    int from;
    int stazioni;
};

//                                                              INSERT
Autonomie* createnode(int key, Autonomie* parent){
    Autonomie* NNode= malloc(sizeof(Autonomie));
    NNode->right=NULL;
    NNode->left=NULL;
    NNode->root_ricariche=NULL;
    NNode->parent=NULL;
    NNode->key=key;
    NNode->parent=parent;
    NNode->conta=0;
    NNode->macchine=0;
return NNode;
}

Autonomie* Insert(Autonomie* root, int key, Autonomie* parent) {
Autonomie* temp = root;
    while(temp != NULL){
        parent=temp;
        if(temp->key>key)
            temp=temp->left;
        else if(temp->key<key)
            temp=temp->right;
        else if(temp->key==key){
            temp->conta++;
            return root;
        }
    }
 Autonomie* NewNode = createnode(key, parent); 
    if(parent == NULL)
        return NewNode; // Se l'albero era vuoto, il nuovo nodo è la radice
    else if(key < parent->key) 
        parent->left = NewNode;
    else    
        parent->right = NewNode;
return root;
} 

//                                                              FIX UP
int search(Autonomie* node, int key){
    while(node!=NULL){
        if(node->key==key)          
            return 0;           //nodo trovato, non aggiunto.
        else if(node->key<key)
            node=node->right;
        else if(node->key>key)
            node=node->left;
    }
    return 1;                   //nodo non trovato, può essere aggiunto
} 

Autonomie* nodo(Autonomie* node, int key){ //come search ma restituisce il nodo puntato 
    while(node!=NULL){
        if(node->key==key)          
            return node;           
        else if(node->key<key)
            node=node->right;
        else if(node->key>key)
            node=node->left;
    }
    return NULL;  
}


//                                                              DELETE

Autonomie* Ricerca(Autonomie* node){
   if(node->parent==NULL)
    return node;
return Ricerca(node->parent);
}

//O(h)=(ln(n))
Autonomie* min(Autonomie* node){
if(node->left==NULL)
    return node;
return min(node->left);
}

//O(h)=(ln(n))
Autonomie* max(Autonomie* node){
if(node->right==NULL)
    return node;
return max(node->right);
}

//O(1)
Autonomie* Delete(Autonomie* node,Autonomie* root){
Autonomie* var=NULL;
    if(node->right!=NULL){
        var=min(node->right);
        node->key=var->key;
        node->conta=var->conta;
        var->conta=0;
        node->root_ricariche=var->root_ricariche;
    }
    else if(node->left!=NULL){
        var=max(node->left);
        node->key=var->key;
        node->conta=var->conta;
        var->conta=0;
        node->root_ricariche=var->root_ricariche;
    }
    else{                                       //caso in cui il node che si chiama è direttamente una foglia                              
        if(node->parent==NULL){
            if(node->conta)
                node->conta--;
            else{
                free(node);
                return NULL;
            }
        return node;
        }
        else if(node->parent->right==node)
            node->parent->right=NULL;
        else    
            node->parent->left=NULL;
        root=Ricerca(node);
            if(node->conta)                     //Per eliminare la ripetizione, IMPORTANTISSIMA!
                node->conta--;
            else
                free(node);
        return root;                            //qua deve tornare root     
    }
    if(var->left==NULL && var->right==NULL){
        if(var->parent->right==var)
            var->parent->right=NULL;
        else    
            var->parent->left=NULL;
        if(var->conta)                          //IMPORTANTISSIMA!!!
            var->conta--;
        else
            free(var);
    }    
    else                                        //se non è una foglia si richiama la funzione 
        Delete(var,root);
return root;
}

//O(lgn)
Autonomie* SearchDelete(Autonomie* node, int key){ 
Autonomie* backup=node;
    while(node!=NULL){
        if(node->key==key){                     //caso di successo
            if(node->conta==0)
                return Delete(node,backup);
            else{
                node->conta--;
                return backup;
            }
        }             
        else if(node->key<key)
            node=node->right;
        else if(node->key>key)
            node=node->left;
    }
        return backup;
}

Autonomie* Eliminazione(Autonomie* node){
   if(node!=NULL){
    Eliminazione(node->left);
    Eliminazione(node->right);
        if(node->parent!=NULL){
            if(node->parent->left==node)
                node->parent->left=NULL;
            else
                node->parent->right=NULL;
            
            free(node);        
        }
    }
    return NULL;
}

//                                      RICERCA PERCORSO
 
void inorder(Autonomie* root,int start,int end,matr* array,int *count) {
Autonomie* temp=NULL;
  if(root != NULL){
        inorder(root->left,start,end,array,count);
        
        if(root->key>=start && root->key<=end){
            (*count)++;
            array[*count-1].key=root->key;
            if(root->root_ricariche!=NULL){
                temp=max(root->root_ricariche);
                array[*count-1].max=temp->key;
            }
            else{
                array[*count-1].max=0;
            }
                
        }
        inorder(root->right,start,end,array,count);
    }
}

int contatore(Autonomie* root, int start, int end,int *flag){
if (root != NULL){
    contatore(root->left,start,end,flag);     
        if(root->key>=start && root->key<=end)
            (*flag)++;   
    contatore(root->right,start,end,flag);
    }
return *flag;
}



int main(){
Autonomie* root=NULL;
Autonomie* NewNode=NULL;
Autonomie* test=NULL;
matr *array=NULL;
    
char command[19];
int dist, num_auto,flag;
int autonomie[511];//stazione non può avere più di 512 veicoli 
    while (scanf("%s", command)!=EOF) {
        if(strcmp(command, "aggiungi-stazione")==0){
            if(scanf("%d %d", &dist, &num_auto)!=EOF){
                if(search(root,dist)){   
                    printf("aggiunta\n");
                    root=Insert(root,dist,NULL);//inserimento stazione.
                    flag=1;
                }           //NewNode ora avrà il nodo di nostro interesse, quello dove lavoreremo.
                else{
                    printf("non aggiunta\n");
                    flag=0;
                }
                if(num_auto!=0){
                    NewNode=nodo(root,dist);
                    for(int i = 0; i <= num_auto; i++){
                       if(scanf("%d", &autonomie[i])!=1){}
                        
                    }
                            
                            if(flag){
                                for(int i = 0; i < num_auto && NewNode->macchine <= 512; i++) {
                                    NewNode->macchine++;
                                    NewNode->root_ricariche = Insert(NewNode->root_ricariche, autonomie[i], NULL); //sarebbe il root delle stazioni
                                }
                            }      
                }
            }
        }
        else if(strcmp(command,"aggiungi-auto")==0){
            if(scanf("%d %d", &dist, &num_auto)!=EOF){     //in questo caso num_auto è direttamente l'autonomia
                NewNode=nodo(root, dist);
                if(NewNode!=NULL && NewNode->macchine<=512){
                    printf("aggiunta\n");
                    NewNode->root_ricariche=Insert(NewNode->root_ricariche,num_auto,NULL);
                    NewNode->macchine++;
                }
                else
                    printf("non aggiunta\n"); 
            }
        }

        else if(strcmp(command, "demolisci-stazione")==0){ 
            if(scanf("%d", &dist)!=EOF){
                NewNode=nodo(root, dist);
                if(NewNode!=NULL){  
                    printf("demolita\n");
                    NewNode->root_ricariche=Eliminazione(NewNode->root_ricariche); //elimino tutte le autonomie
                    root=SearchDelete(root,NewNode->key); //elimino nodo stazione
                    NewNode->macchine=0;  
                }
                else    
                    printf("non demolita\n");
            }
        }

        else if(strcmp(command, "rottama-auto")==0){
            if(scanf("%d %d",&dist, &num_auto)!=EOF){//dist è la stazione, num_auto la stazione da eliminare 
                NewNode=nodo(root,dist);
                if(NewNode!=NULL){
                    test=nodo(NewNode->root_ricariche,num_auto);//NEL CASO DOVESSERO ESSERCI PROBLEMI DI TEMPO SI PUÒ SOSTIUIRE TEST CON UN FLAG GLOBALE.
                    if(test!=NULL){
                        printf("rottamata\n");
                        NewNode->root_ricariche=SearchDelete(NewNode->root_ricariche,num_auto);
                        NewNode->macchine--;
                    }
                    else                        
                        printf("non rottamata\n");                       
                }
                else    
                    printf("non rottamata\n");
            }
        }

        else if(strcmp(command, "pianifica-percorso")==0){
            if(scanf("%d %d", &dist, &num_auto)!=EOF){//partenza e arrivo
            int flag=0;
            if(dist<num_auto){
                flag=contatore(root,dist,num_auto,&flag);
                array=(matr*)malloc((flag)*sizeof(matr));
                flag=0;
                inorder(root,dist,num_auto,array,&flag);
            }
            else{
                flag=contatore(root,num_auto,dist,&flag);
                array=(matr*)malloc((flag)*sizeof(matr));
                flag=0;
                inorder(root,num_auto,dist,array,&flag);
            }
            
//          Ho poco tempo e non posso stare a debuggare la matrice nella funz, scrivo tutto qua
                                    //Ricerca
    info *lista;
    lista=(info*)malloc((flag)*sizeof(info));
        short int uscita=1;

        for(int i=0;i<flag;i++){
            lista[i].path=__INT_MAX__;
            lista[i].from=-1;
            lista[i].stazioni=__INT_MAX__;
        }

//                                                                        Da Sx a Dx

if(dist<num_auto){
        lista[0].path=0;//si inizializza la prima stazione a 0

        for(int i=0;i<flag-1 && uscita>0;i++){                                  //per non leggere anche l'ultima stazione
            for(int j=i+1;j<flag;j++){                              //così inizia direttamente dalla stazione successiva  
                if(array[i].key+array[i].max>=array[j].key && lista[j].path>lista[i].path+j){ 
// 1)Fondamentale per capire se c'è il percorso da un nodo ad un altro      2)Se il percorso vecchio è maggiore di quello nuovo
                    lista[j].from=i;                                //l'autonomia più vicina all'origine
                    lista[j].path=lista[i].path+j;                  //aggiornamento lunghezza percorso     
                }
                else if(array[i].key+array[i].max<array[j].key){//abbiamo superato il nostro max, non ha più senso continuare
                    if(i==0 && j==1)
                        uscita=0;
                    break;
                }
            }
        }   //non gestisco bene l'uscita se c'è qualcosa che non va, sistemare bene sta situzione qua.

//                                              STAMPA PERCORSO PRIMO CASO
    int percorso[flag];
    int count=0;        

    if(lista[flag-1].from == -1)
        printf("nessun percorso\n");
    else{
        int i=flag-1;
        count=0;
        percorso[count]=i;
        count++;
        do{
            percorso[count]=lista[i].from;
            count++;
            i=lista[i].from;
        }while(i>0);

        count--;
        if(array[percorso[count]].key!=dist)
            printf("nessun percorso\n");
        else{
            while(count>=0){ //                                     stampa stazioni!
                printf("%d ",array[percorso[count]].key);
                count--;
            }
            printf("\n");
        }
    }
}   
//                                                                   Da Dx a Sx
else{
    lista[flag- 1].path=0;
    lista[flag-1].stazioni=0;
         for(int i=flag-1;i>=0;i--){

            for(int j=i-1;j>=0;j--){
                if(array[i].key-array[i].max<=array[j].key && lista[j].path>=lista[i].path+j  && lista[i].stazioni<lista[j].stazioni ){
            
                    lista[j].from=i;
                    lista[j].path=lista[i].path+j;
                    lista[j].stazioni=lista[i].stazioni+1;
                    
                }
                else if(array[i].key-array[i].max<=array[j].key && i<lista[j].from && (lista[i].stazioni+1)==lista[j].stazioni){
                    lista[j].from=i;
                }
                else if(array[i].key-array[i].max>array[j].key){
                    break;
                }
                else if(lista[i].path==(lista[i].path+j) && lista[j].from>i && lista[j].stazioni==(lista[i].stazioni+1)){
        //prima condizione per vedere se è l'elemento 0 dell'array, 2) per vedere se è più vicino allo 0   
                lista[j].from=i;
                }
            }
        }
//                                              STAMPA PERCORSO SECONDO CASO 
    int i=0,count=0;
    int percorso[flag];
        
    if(lista[0].from == -1) //si parte dalla fine che è il nodo 0 e si va a ritroso
        printf("nessun percorso\n");
    else{
        percorso[count]=0;
        count++;
        do{
            percorso[count]=lista[i].from;
            count++;
            i=lista[i].from;
        }while(i>0);// alla fine di questo ciclo l'ultimo elemento dell'array sarà quello iniziale, dist!(quello più grande)
count-=2;
        
        if(array[percorso[count]].key!=dist)
            printf("nessun percorso\n");
        else{
            while(count>=0){ //                                     stampa stazioni!
                printf("%d ",array[percorso[count]].key);
                count--;
            }
            printf("\n");
        }
    }
}             
                                    //fine ricerca                           
        free(array);
        }
    }
    }
return 0;
}