/*
  Implementazione del gioco:

  - La lunghezza del codice segreto verrà scelta dall'utente prima di iniziare una partita (4 - 6 - 8).
  - L'utente potrà scegliere la difficoltà (Facile - Intermedia - Difficile):
    * Facile: 10 tentativi
    * Intermedia: 8 tentativi
    * Difficile: 6 tentativi

  - Durante la partita, l'utente avrà la possibilità di abbandonare il gioco e tornare al menù principale.

  - Se un utente vince la partita, verrà incrementato il contatore delle partite vinte dall'inizio della sessione e riceverà
    10 punti per ogni partita vinta. Se perde, comparirà una scritta "Game Over" e potrà scegliere se riprovare la partita con
    gli stessi settaggi della precedente oppure tornare al menù principale.

  - Il punteggio verrà salvato su un file di testo che conterrà l'ID del giocatore. Questo ID verrà richiesto durante la fase
    di scelta dei settaggi pre-partita. Se il nome utente è già presente nel file, verrà comunicato all'utente e si
    incrementeranno i valori di partite giocate, partite vinte e il punteggio.

  La struttura dei dati contenuti nel file per singolo giocatore sarà la seguente:
  xxxx (id utente)
  n n n n  (partite giocate, partite vinte, punti, tutorial)
  . (separatore)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define FACILE 10
#define INTERMEDIA 8
#define DIFFICILE 6

#define LUNGHEZZA_ID 11
#define COLORI 8
#define ESC 27
#define ROW 256
#define CLEAN_BUFFER  while ((getchar()) != '\n')

/*
#define ROSSO 1
#define VERDE 2
#define BLU 3
#define GIALLO 4
#define MARRONE 5
#define ARANCIONE 6
#define NERO 7
#define BIANCO 8
*/

typedef struct Sgiocatore {
    char id[LUNGHEZZA_ID];
    int partite_giocate;
    int partite_vinte;
    int punti;
    int tutorial;
} giocatore;


int insensitive_compare(char stringa1[], char stringa2[]) //funziona
{
    int status = 0;
    // Converti stringa1 in minuscolo
    for (int i = 0; stringa1[i] != '\0'; i++) {
        stringa1[i] = tolower(stringa1[i]);
    }

    // Converti stringa2 in minuscolo
    for (int i = 0; stringa2[i] != '\0'; i++) {
        stringa2[i] = tolower(stringa2[i]);
    }

    // Confronta le stringhe e restituisci il risultato
    int x = 0;
    while (stringa1[x] != '\0' || stringa2[x] != '\0') {
        if (stringa1[x] != stringa2[x]) {
            return 0;
        }
        x++;
    }
    return 1; // Le stringhe sono uguali
}

void verifica_id(char nomefile[], char id_inserito[], giocatore *player, char input[]) //finita e testata FUNZIONA
{
    char id_letto[LUNGHEZZA_ID];


    FILE *fp = fopen(nomefile, "r");
    printf("ho aperto il file\n");

    if (fp == NULL) {
        printf("ERRORE NELL'APERTURA DEL FILE!\n");
        exit(1);
    }
    int res = 0;

    while (fscanf(fp, "%[^\n]", id_letto) != EOF) { //finchè non è alla fine del file legge fino alla fine della riga considerando gli spazi
        fgetc(fp); //consuma il carattere \n che fa andare il puntatore a capo
        printf("id letto= |%s| \n", id_letto);
        if (insensitive_compare(id_letto, id_inserito) == 1) {
            printf("Id trovato!\nCaricamento dei dati in memoria...\n");
            strcpy(player->id, id_letto);
            printf("player->id= %s\n", player->id);
            fscanf(fp, "%d %d %d %d", &player->partite_giocate, &player->partite_vinte, &player->punti,&player->tutorial);
            //printf("dati salvati:\n giocate= |%d|\n vinte=|%d|\n punti=|%d|\n tutorial=|%d|\n",
            // player->partite_giocate,player->partite_vinte,player->punti,player->tutorial);

            printf("Caricamento terminato!\n");
            fclose(fp);
            return;
        } //questo while è stato testato e funziona
    }


    //se l'id non viene trovato
    printf("L'id da te inserito non è presente in memoria. Vuoi crearlo?\n y/n\n");


    scanf("%1[^\n]", input);
    while (*input != 'y' && *input != 'n' && *input != 'Y' && *input != 'N') {
        //printf("input= |%s|\n",buffer);
        printf("Input non valido,riprova\n");
        CLEAN_BUFFER;
        scanf("%1[^\n]", input);
    }
    //printf("input= |%s|", input);

    if (*input == 'y' || *input == 'Y') {
        fp = fopen(nomefile, "a");

        if (fp == NULL) {
            printf("ERRORE NELL'APERTURA DEL FILE!\n");
            exit(1);
        }

        strcpy(player->id, id_inserito);
        player->partite_giocate = 0;
        player->partite_vinte = 0;
        player->punti = 0;
        player->tutorial = 1;

        fprintf(fp, "%s\n", id_inserito);
        fprintf(fp, "%d %d %d %d\n", player->partite_giocate, player->partite_vinte, player->punti, player->tutorial);
        fprintf(fp, ".\n");
        fclose(fp);
        printf("Il tuo profilo è stato creato correttamente!\n");
    } else if (*input == 'n' || *input == 'N') {
        printf("Alla prossima!!\n");
        free(player);
        exit(0);
    }

}

int settings_partita(int *lunghezza_codice, int *difficolta, giocatore *player, char input[], char nomefile[]) //FUNZIONA
{
    char facile []="FACILE";
    char intermedia []="INTERMEDIA";
    char difficile []="DIFFICILE";

    system("clear");

    if (player->tutorial == 1) {
        printf("player->tutorial |%d|\n",player->tutorial);
        printf("Non hai mai visto le regole, vuoi vederle?\n y/n\n");
        CLEAN_BUFFER;

        scanf("%1[^\n]", input);
        while (*input != 'y' && *input != 'n' && *input != 'Y' && *input != 'N') {
            //printf("input= |%s|\n",input);
            printf("Input non valido,riprova\n");
            CLEAN_BUFFER;
            scanf("%1[^\n]", input);
        }
        printf("input= |%s|", input);


    }

    if (*input == 'y' || *input == 'Y') {
        player->tutorial=0;
        CLEAN_BUFFER; // ancora player.tutorial vale 1 dovrà essere portato a 0 dalla funzione per vedere le regole
        return 2;
    }

    printf("ok! procediamo...");
    player->tutorial = 0;
    FILE *fp;
    fp=fopen(nomefile,"r+"); //r+ apre il file in lettura e scrittura senza cancellare i dati esistenti (il puntatore è all'inizio del file

    if (fp == NULL) {
        printf("ERRORE NELL'APERTURA DEL FILE!\n");
        exit(1);
    }

    char id_letto [LUNGHEZZA_ID];

    int pos =0;
    while (fscanf(fp, "%[^\n]", id_letto) != EOF) { //finchè non è alla fine del file legge fino alla fine della riga considerando gli spazi
        fgetc(fp); //consuma il carattere \n che fa andare il puntatore a capo
        pos+=sizeof (id_letto);
        printf("id letto= |%s| \n", id_letto);
        if (insensitive_compare(id_letto, player->id) == 1) {
            printf("Id trovato!\nCaricamento dei dati in memoria...\n");
            //printf("posizione del puntatore = |%ld| , seek_set= |%d|\n", ftell(fp), SEEK_SET);
            getchar();
            printf("pos = |%d|\n",pos);
            fseek(fp,0,SEEK_CUR);
            fprintf(fp, "%d %d %d %d", player->partite_giocate, player->partite_vinte, player->punti,player->tutorial);
            printf("dati salvati:\n giocate= |%d|\n vinte=|%d|\n punti=|%d|\n tutorial=|%d|\n",player->partite_giocate,player->partite_vinte,player->punti,player->tutorial);
            printf("Salvataggio terminato!\n");
            fclose(fp);
        } //questo while è stato testato e funziona
    }



    printf("Scegli la lunghezza del codice segreto. (8-6-4)\n");
    scanf(" %1d", lunghezza_codice);
    while (*lunghezza_codice != 4 && *lunghezza_codice != 6 && *lunghezza_codice != 8 ) {
        printf("Input non valido,riprova\n");
        CLEAN_BUFFER;
        scanf(" %1d", lunghezza_codice);
    }

    printf("\n\n Ora devi scegliere la difficoltà di gioco.\nFACILE = 10 TENTATIVI\nINTERMEDIA = 8 TENTATIVI\nDIFFICILE = 6 TENTATIVI\n");
    CLEAN_BUFFER;


    scanf("%s",input);
   while(insensitive_compare(input,facile) !=1 && insensitive_compare(input,intermedia) != 1
                                                            && insensitive_compare(input,difficile) != 1){
       printf("Input non valido,riprova\n");
       CLEAN_BUFFER;
       scanf("%s",input);
   }


    if (insensitive_compare(facile, input) == 1) {
        *difficolta = FACILE;
        printf("difficoltà settata |%d|\n",*difficolta);
        return 1;
    } else if (insensitive_compare(intermedia, input) == 1) {
        *difficolta = INTERMEDIA;
        printf("difficoltà settata |%d|\n",*difficolta);
        return 1;
    } else if (insensitive_compare(difficile, input) == 1) {
        *difficolta = DIFFICILE;
        printf("difficoltà settata |%d|\n",*difficolta);
        return 1;
    }
    return 0;
}

int * genera_codice(int *codice, int *lunghezza_codice) //Funziona
{

    if (codice == NULL) // se l'array codice non è stato inizializzato (quindi se è la prima partita della sessione)
    {
        printf("sto per allocare memoria al codice poichè è nullo\n");
        codice = (int *) malloc(sizeof(int)*(*lunghezza_codice));
        printf("fatto\n");// alloc
        // a memoria all'array in base alla lunghezza scelta dall'utente
    }

    srand(time(NULL)); // inizializzo generatore di n umeri casuali con tempo nullo per garantire valori diversi ad ogni esecuzione
    printf("inizializzato srand\n");
    for(int i=0; i < (*lunghezza_codice) ;i++){
        codice[i]=rand() % (COLORI +1);
        printf("posizione =|%d| valore casuale |%d|\n",i,codice[i]);
    }

    printf("ho finito il for\n");

    return codice;
}

void visualizza_regole(char nomefile[], giocatore *player) // da testare e coreggere
{
    int carattere,scelta;

    system("clear");
    player->tutorial = 0;

    FILE *fp;
    fp = fopen(nomefile, "r");
    if (fp == NULL) {
        printf("ERRORE NELL'APERTURA DEL FILE!\n");
        exit(1);
    }

    // Leggi e stampa il file carattere per carattere
    while ((carattere = fgetc(fp)) != EOF) {
        putchar(carattere); // stampa il carattere attraverso il valore ascii dello stesso
    }

    fclose(fp);

    printf("\n\nInserisci 0 per tornare  al menù:\n");
    scanf("%d", &scelta);
    while (scelta!=0) {
        printf("Input non valido,riprova\n");
        CLEAN_BUFFER;
        scanf("%d", &scelta);
    }
}

void dati_giocatore(giocatore *player){ //funziona
    int scelta;

    system("clear");
    printf("Ecco i tuoi dati %s\n\n",player->id);
    printf("Partite giocate= %d\n",player->partite_giocate);
    printf("Partite vinte= %d\n",player->partite_vinte);
    printf("Punti= %d\n",player->punti);

    printf("\n\nInserisci 0 per tornare  al menù:\n");
    scanf("%d", &scelta);
    while (scelta!=0) {
        printf("Input non valido,riprova\n");
        CLEAN_BUFFER;
        scanf("%d", &scelta);
    }

}
int menu()  //funziona
{
    int scelta;
    printf("****** BENVENUTO IN MASTERMIND ******\n\n");
    printf("[1] INIZIA UNA NUOVA PARTITA\n");
    printf("[2] REGOLE DEL GIOCO\n");
    printf("[3] DATI DEL GIOCATORE\n");
    printf("[0] ESCI DAL GIOCO\n");
    printf("\nScelta: ");

    scanf("%d", &scelta);

    while (scelta < 0 || scelta > 3) {
        printf("SCELTA NON VALIDA RIPROVARE!\n");
        printf("\nScelta: ");
        getchar();
        scanf("%d", &scelta);
    }

    return scelta;
}

int main() {
    system("clear");

    giocatore *player = (giocatore *) malloc(sizeof(giocatore));

    int difficolta, lunghezza_codice, input_utente, settings;
    char id_utente[LUNGHEZZA_ID];
    char input[ROW];
    int *codice;

    printf("****** BENVENUTO IN MASTERMIND ******\n INSERISCI IL TUO ID UTENTE:\n");

    fgets(input, sizeof (input), stdin);
    printf("ho letto\n");
    while (input[0] == '\n' || strlen(input) > LUNGHEZZA_ID || input[0] == ESC) {
        printf("input non valido, riprova\n");
        fgets(input, sizeof(input), stdin);
    }
    printf("sono dopo il while\n");

    input[strcspn(input, "\n")] = '\0'; // Rimuove il newline finale, se presente
    printf("ho rimosso il new line\n");
    strcpy(id_utente, input);
    printf("ho copiato id utente da buffer\n");

    /* la funzione strcspn scansiona
   la stringa passata nel primo parametro fino a quando non trova corrispondenza con almeno uno dei caratteri contenuti nella stringa passata
   come secondo parametro ("\n"). Ha come valore di ritorno il contatore di posizioni analizzate a partire da zero.
   Quindi abbiamo preso l'elemento dell'array "id_utente" nella posizione del carattere \n (se presente) e lo sostituiamo con il carattere nullo*/


    printf("sto per entrare in verifica id\n");
    verifica_id("data.txt", id_utente, player, input);
    printf("sono uscito da verifica id\n");
    getchar();
    CLEAN_BUFFER;

    while (1) {
        system("clear");
        input_utente = menu();

        switch (input_utente) {
            case 0:
                system("clear");
                free(player);
                exit(0);
                break;

            case 1:
                settings = settings_partita(&lunghezza_codice, &difficolta, player, input, "data.txt");
                printf("Il risultato di settings_partita è |%d|\n",settings);
                getchar();

                if (settings == 2) {
                    visualizza_regole("regole.txt", player);
                }

                 if (settings == 1) {
                    codice=genera_codice(codice, &lunghezza_codice);
                    //printf("sono tornato da genera codice, il codice risultante è:\n");
                    printf("lunghezza codice=|%d|\n",lunghezza_codice);
                     printf("codice=");
                     for(int i=0; i < (lunghezza_codice) ;i++){
                         printf("|%d|",codice[i]);
                     }
                     printf("\n");
                    // funzione di gioco
                }


                break;

            case 2:
                CLEAN_BUFFER;
                system("clear");
                visualizza_regole("regole.txt", player);
                break;

            case 3:
                CLEAN_BUFFER;
                dati_giocatore(player);
                break;

            default:
                break;
        }
    }
}