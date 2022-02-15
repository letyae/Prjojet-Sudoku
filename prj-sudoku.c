#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

typedef struct{
	int row;
	int column;
	}parameters;

parameters *new_param(int l,int c)
{
	parameters *p=(parameters*)malloc(sizeof(parameters));
	p->row=l;
	p->column=c;
	return p;
}





int sudoku[9][9]={{6,2,4,5,3,9,1,8,7},
		  {5,1,9,7,2,8,6,3,4},
		  {8,3,7,6,1,4,2,9,5},
		  {1,4,3,8,6,5,7,2,9},
		  {9,5,8,2,4,7,3,6,1},
		  {7,6,2,3,9,1,4,5,8},
		  {3,7,1,9,5,6,8,4,2},
		  {4,9,6,1,8,2,5,7,3},
		  {2,8,5,4,7,3,9,1,6}};

/*  verifie si les valeurs du sudoku appartiennent à [1,9] */

int verifi(int r){
	if(r==1 || r==2|| r==3 || r==4 || r==5 || r==6|| r==7 || r==8 ||r==9)
	   return 1;
	else return 0;   
}


int resultat[27]; /* tableau de recuperation des etats de validation */

/*creation d'un mutex pour rendre atomique l'accès au tableau sudoku qui est une donne partagé par  plusieurs threads */

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

/*la fonction verifi_col() verifie la validite des colonnes */
 
void *verifi_col(void *arg){
	parameters *p=(parameters*)arg;
	int c=p->column,i,r=0,s=0,j,x;
	int tab[9]={1,2,3,4,5,6,7,8,9};

	for(i=0;i<9;i++)
	{
		/*l'acces au sudoku est proteger par un mutex*/

		pthread_mutex_lock(&mutex);
		r=sudoku[i][c];

	    /*sortit de la section critique*/
	    pthread_mutex_unlock(&mutex);

      x=verifi(r);   /* verifie si r appartient à [0,9]    */

      if (x==1){ 

           for(i=0;i<9;i++){
              if (r==tab[j]){

                 tab[j]=-1;                 
                 break;
               } /* chaque  valeur de la colonne fixé du sudoku retrouver pour la 1ere fois dans 
                tab est vidée */
            }
        }
    }

/* verification du contenu de tab */

for(i=0;i<9;i++){
	if(tab[i]==-1)
	 s++;
	  else
	  break;
}


    if(s==9){
		s=1;
		pthread_exit((void*) s);    /*fonction de sortit du thread avec pour argument la valeur de sortit du thread qui signifie que tout s'est bien passe */
	}

	else if(s<9){
	 s=0;
	 pthread_exit((void*) s);   /* fonction de sortit du thread avec pour argument la valeur de sortit du thread qui signifie que la colonne est invalid */
	}
}

/*fonction pour verifier la validite d'une ligne */

void *verifi_lign(void *arg)
{
	parameters *p=(parameters*)arg;
	int l=p->row,i,j,r=0,s=0,x;
	int tab[9]={1,2,3,4,5,6,7,8,9};
	for(i=0;i<9;i++)
	{
		pthread_mutex_lock(&mutex);
		r=sudoku[l][i];
		pthread_mutex_unlock(&mutex);

		x=verifi(r);   /* verifie si r appartient à [0,9] */
        if (x==1){ 
      
		        for(j=0;j<9;j++){

		             if(r==tab[j]){
		     		 tab[j]=-1;
		    		 break;
		        	}
		        }
	        }
	}


 for(i=0;i<9;i++){
	if(tab[i]==-1)
	 s++;
	else
	 break;
 }


  if(s==9){
		s=1;
		pthread_exit((void*)s);    /*fonction de sortit du thread avec pour argument la valeur de sortit du thread qui signifie que tout s'est bien passe */
	}

	else if(s<9){
	 s=0;
	 pthread_exit((void*) s);   /* fonction de sortit du thread avec pour argument la valeur de sortit du thread qui signifie que la colonne est invalid */
	}
}




/*FONCTION DE THREAD POUR CONTROLER LA VALIDITE D'UNE SOUS-MATRICE 3X3 */

void *verifi_grille(void *arg)
{
	parameters *p=(parameters*)arg;
	int l=p->row;
	int c=p->column,i,j,r=0,s=0,k;
	int tab[9]={1,2,3,4,5,6,7,8,9};

	for(i=l;i<l+3;i++)
	{
		for(j=c;j<c+3;j++)
		{
			pthread_mutex_lock(&mutex);
			r=sudoku[i][j];
			pthread_mutex_unlock(&mutex);

			for(k=0;k<9;k++){
				if(r==tab[k]){
					tab[k]=-1;					
					break;
				}	
			}		
		}
	}


 for(i=0;i<9;i++){
	if(tab[i]==-1)
	  s++;
	  else
	  break;
 }

	if(s==9){
		s=1;
		pthread_exit((void*) s);
	}

	else if(s<9){
		s=0;
		pthread_exit((void*) s);
	}
}



int main()
{
	int *tab2=malloc(27*sizeof(int)),i,j,cpt=0;
	
	/* entrer des valeurs du sudoku */	

	printf("Entrer données dans le sudoku :\n");
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){

		     do{
			     scanf("%d",&sudoku[i][j]);
			   } while(sudoku[i][j]>9);
	    }
	}


	pthread_t *thread=malloc(27*sizeof(pthread_t)); /*reservation d'espace pour un tableau de 27 threads */


	/* creation de 9 threads pour verifier la validite des 9 lignes */

	tab2[0]=pthread_create(&thread[0],NULL,verifi_lign,(void*)(new_param(0,0)));
	tab2[1]=pthread_create(&thread[1],NULL,verifi_lign,(void*)(new_param(1,0)));
	tab2[2]=pthread_create(&thread[2],NULL,verifi_lign,(void*)(new_param(2,0)));
	tab2[3]=pthread_create(&thread[3],NULL,verifi_lign,(void*)(new_param(3,0)));
	tab2[4]=pthread_create(&thread[4],NULL,verifi_lign,(void*)(new_param(4,0)));
	tab2[5]=pthread_create(&thread[5],NULL,verifi_lign,(void*)(new_param(5,0)));
	tab2[6]=pthread_create(&thread[6],NULL,verifi_lign,(void*)(new_param(6,0)));
	tab2[7]=pthread_create(&thread[7],NULL,verifi_lign,(void*)(new_param(7,0)));
	tab2[8]=pthread_create(&thread[8],NULL,verifi_lign,(void*)(new_param(8,0)));

	/*creation de 9 threads pour verifier la validite des 9 colonnes*/

	  tab2[9]=pthread_create(&thread[9],NULL,verifi_col,(void*)(new_param(0,0)));
	tab2[10]=pthread_create(&thread[10],NULL,verifi_col,(void*)(new_param(0,1)));
	tab2[11]=pthread_create(&thread[11],NULL,verifi_col,(void*)(new_param(0,2)));
	tab2[12]=pthread_create(&thread[12],NULL,verifi_col,(void*)(new_param(0,3)));
	tab2[13]=pthread_create(&thread[13],NULL,verifi_col,(void*)(new_param(0,4)));
	tab2[14]=pthread_create(&thread[14],NULL,verifi_col,(void*)(new_param(0,5)));
	tab2[15]=pthread_create(&thread[15],NULL,verifi_col,(void*)(new_param(0,6)));
	tab2[16]=pthread_create(&thread[16],NULL,verifi_col,(void*)(new_param(0,7)));
	tab2[17]=pthread_create(&thread[17],NULL,verifi_col,(void*)(new_param(0,8)));

	/*creation de 9 threads pour verifier la validite des 9 sous  grille 3*3 */

    tab2[18]=pthread_create(&thread[18],NULL,verifi_grille,(void*)(new_param(0,0)));
	tab2[19]=pthread_create(&thread[19],NULL,verifi_grille,(void*)(new_param(0,3)));
	tab2[20]=pthread_create(&thread[20],NULL,verifi_grille,(void*)(new_param(0,6)));
	tab2[21]=pthread_create(&thread[21],NULL,verifi_grille,(void*)(new_param(3,0)));
	tab2[22]=pthread_create(&thread[22],NULL,verifi_grille,(void*)(new_param(3,3)));
	tab2[23]=pthread_create(&thread[23],NULL,verifi_grille,(void*)(new_param(3,6)));
	tab2[24]=pthread_create(&thread[24],NULL,verifi_grille,(void*)(new_param(6,0)));
	tab2[25]=pthread_create(&thread[25],NULL,verifi_grille,(void*)(new_param(6,3)));
	tab2[26]=pthread_create(&thread[26],NULL,verifi_grille,(void*)(new_param(6,6)));

 for(i=0;i<27;i++){
		if(tab2[i]!=0)
		{
			fprintf(stderr,"Erreur lors de la création du thread %d \n",thread[i]);
			exit(1);
		}
 }		
			
		
 /* Synchronisation et recuperation des valeurs de retours des threads avec la fonction pthread_join(..,..) */


 for(i=0;i<27;i++){

  /*   pthread_join()  s'assure que le ieme thread est termine et recupere dans resultat[i] sa valeur de retour */

  pthread_join(thread[i],(void*) &resultat[i]);

 }		



 /* AFFICHAGE DES RESULTATS */

 for(i=0;i<27;i++){

     if(resultat[i]==0)

     { printf("sudoku invalid");
       break;
     }

   else
    cpt++;
}

 if(cpt==27)
 printf("THE sudoku game is valide");
		
	
return 0;
}
