/*************************************************************************************************************
* In questo file verranno incluse le funzioni necessarie a costruire un albero binario,
* e gestirlo tramite
* i puntatori che ogni radice ha verso il figlio destro e il figlio sinistro.
* L'interfaccia è abbastanza semplice, una volta avviato si arriva ad un menu.
* Si consiglia per compilarlo sotto *nix di usare "gcc -o btree binarytree.c" ,
* Per avviarlo invece, come suggerisce l'ERRORE di parametri
* sarà utile seguire la forma ./btree <nomefile>, in cui nomefile può anche contenere il path completo.
* i dati presenti in nomefile devono essere degli interi separati da spazi.
**************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BinaryTree.h"

FILE *intfile;

/*   old style delcaration ANSI C : works fine, but too many things in one shot.
typedef struct T{  //Comincio a definire la struttura che mi servirà
    int value;  //Come posso notare ho il valore attuale e due puntatori: uno al figlio sinistro
    struct T *T_l, *T_r;  //E l'altro al figlio destro
}*tree, NodeSize;
*/

struct T
{ // struttura dati del generico nodo.
    int value;  // valore del nodo attuale e due puntatori, ai due figli : uno al sinistro ed uno al destro.
    struct T *T_l;  // figlio sinistro
    struct T *T_r;  // figlio destro
};

typedef T *tree; // global type : a node pointer, which will be used to access the entire tree.
typedef T NodeSize; // global types : the generic node itsself( not a pointer to), used to get the sizeof(Node).


tree mergetree(int el, tree t1, tree t2)
{  //Mergetree unisce due alberi
    tree t0 = (tree)malloc(sizeof(NodeSize));
    t0->T_l = t1;
    t0->T_r = t2;
    t0->value = el;
    return(t0);
}


tree createleaf(int el)
{// Ogni foglia è formata da un valore e due puntatori nulli
    return mergetree(el, NULL, NULL);
}

int isvoidtree(tree t)
{  //Verifico che un albero sia vuoto
    int result = (t == NULL);  //Se non c'è nulla è ovvio che è un albero vuoto...
    return result;
}


tree leftchild(tree t)
{  //Restituisce il figlio sinistro accedendo alla struttura tree
    return t->T_l;
}


tree rightchild(tree t)
{ // Restituisce il figlio destro, accedendo alla struttura tree
    return t->T_r;
}


int root(tree t)
{ // Restituisce la radice, sempre facendo accesso alla struttura
    return t->value;
}


tree insert(int el, tree t)
{  //Si inserisce un intero el, nell'albero t
    if(isvoidtree(t))
    {  //Se l'albero è vuoto, allora verrà creata una foglia
        return createleaf(el);
    }
    if (root(t)>=el)
    {  //Altrimenti si procede da direttive, ovvero se il valore della radice è >= dell'elemento
        return mergetree(root(t), insert(el,leftchild(t)), rightchild(t));  //Andrà a sinistra
    }
    if (root(t)<el)
    { // Se la radice è invece minore dell'elemento, verrà inserita a destra.
        return mergetree(root(t), leftchild(t), insert(el, rightchild(t)));
    }else
    {
        return t;
    }
}// END insert



int mintree(tree t)
{ // Trovo il minimo per dicotomia: sapendo che più mi muovo in basso ed a sinistra, più ho un numero piccolo.
    if( isvoidtree( leftchild(t)) )
    {// più mi muovo in basso ed a sinistra, più ho un numero piccolo.
        return root(t);
    }
    else
    {
        return mintree(leftchild(t));  //Ripeto la procedura ricorsivamente.
    }
}// END mintree


int maxtree(tree t)
{ // Trovo il massimo per dicotomia: sapendo che più mi muovo in basso ed a destra, più ho un numero grande.
    if( isvoidtree( rightchild(t)))
    {
        return root(t);  //Come per il minimo, solo che in questo caso
    }
    else
    {
        return maxtree(rightchild(t));  //Mi muovo in basso a destra
    }
}// END maxtree


void showtree(tree t)
{
    if( isvoidtree(t)==false)
    {
        showtree(leftchild(t));
        printf("%d ", root(t));
        showtree(rightchild(t));
    }
}// END showtree



// was main
int TreeManagement( char *dataFilePath )
{
    if( NULL==dataFilePath)
    {  //Controllo che ci siano tutti i parametri
        printf("ERRORE: Per avviare il programma la sintassi è ./btree <file>\n");
        return(1);
    }// else can continue.

    if((intfile = fopen(dataFilePath, "r")) == NULL)
    {  //Apro il file che mi serve
        printf("ERRORE: Non riesco ad aprire il file %s\n", dataFilePath );
        return(2);
    }// else can continue.

    printf("    *   Ho aperto il file %s.\n", dataFilePath );

    int num; //Scansiono il file di interi
    tree albero = NULL;  //Inizializzo l'albero vuoto

    while(fscanf(intfile,"%d", &num) != EOF)
    {// insert in the RAM-binary-tree all of the file contained in "dataFilePath".
        albero=insert(num,albero);
    }// END insert in the RAM-binary-tree all of the file contained in "dataFilePath".

    printf("    *   Ho costruito l'albero binario\n\n");
    printf("Cosa vuoi fare adesso?\n");
    printf("[s]tampare l'albero\n");
    printf("Cercare il [m]inimo\n");
    printf("Cercare il [M]assimo\n");
    printf("[u]scire.\n\n");

    printf(">");
    char tmp;

    while((tmp = getchar()) != 'u')
    {
        switch (tmp)
        {
            case 's':  //Serve a mostrare l'albero
                 showtree(albero);
                 printf("\n");
            break;

            case 'm':  //Stampa a video il valore minimo
                 printf("Il valore minimo dell'albero binario e' %d\n", mintree(albero));
            break;

            case 'M':  //Stampa a video il valore massimo
                 printf("Il valore massimo dell'albero binario e' %d\n", maxtree(albero));
            break;

            default:
                printf(">");
            break;
        }
    }
    fclose(intfile);
    // ready
    return(0);
}// end
