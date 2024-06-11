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
#define VITTORIA 10

#define LUNGHEZZA_ID 11
#define COLORI 5
#define ESC 27
#define ROW 256


#define CLEAN_BUFFER  do { int ch; while ( (ch = getchar()) != EOF && ch != '\n' ) {} } while (0)
#define CLEAN_SCREEN system("clear")


#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define COLOR_RESET "\e[0m"


#define ROSSO_VAL 1
#define VERDE_VAL 2
#define BLU_VAL 3
#define GIALLO_VAL 4
#define BIANCO_VAL 5


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

void salvataggio_dati(char nomefile[], giocatore *player) {

    FILE *fp;
    fp = fopen(nomefile,"r+"); //r+ apre il file in lettura e scrittura senza cancellare i dati esistenti (il puntatore è all'inizio del file

    if (fp == NULL) {
        printf("ERRORE NELL'APERTURA DEL FILE!\n");
        exit(1);
    }

    char id_letto[LUNGHEZZA_ID];

    int pos = 0;
    while (fscanf(fp, "%[^\n]", id_letto) != EOF) { //finchè non è alla fine del file legge fino alla fine della riga considerando gli spazi
        fgetc(fp); //consuma il carattere \n che fa andare il puntatore a capo
        pos += sizeof(id_letto);
        if (insensitive_compare(id_letto, player->id) == 1) {
            fseek(fp, 0, SEEK_CUR);
            fprintf(fp, "%d %d %d %d", player->partite_giocate, player->partite_vinte, player->punti, player->tutorial);
            fclose(fp);
        }
    }
}


void verifica_id(char nomefile[], char id_inserito[], giocatore *player, char input[]) //finita e testata FUNZIONA
{
    char id_letto[LUNGHEZZA_ID];


    FILE *fp = fopen(nomefile, "r");


    if (fp == NULL) {
        printf("ERRORE NELL'APERTURA DEL FILE!\n");
        exit(1);
    }

    while (fscanf(fp, "%[^\n]", id_letto) !=
           EOF) { //finchè non è alla fine del file legge fino alla fine della riga considerando gli spazi
        fgetc(fp); //consuma il carattere \n che fa andare il puntatore a capo
        if (insensitive_compare(id_letto, id_inserito) == 1) {
            printf("Id trovato!\nCaricamento dei dati in memoria...\n");
            strcpy(player->id, id_letto);
            fscanf(fp, "%d %d %d %d", &player->partite_giocate, &player->partite_vinte, &player->punti,&player->tutorial);
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
    char facile[] = "FACILE";
    char intermedia[] = "INTERMEDIA";
    char difficile[] = "DIFFICILE";


    if (player->tutorial == 1) {
        printf("Non hai mai visto le regole, vuoi vederle?\n\n y/n\n\n");
        CLEAN_BUFFER;

        scanf("%1[^\n]", input);
        while (*input != 'y' && *input != 'n' && *input != 'Y' && *input != 'N') {
            //printf("input= |%s|\n",input);
            printf("Input non valido,riprova\n\n");
            CLEAN_BUFFER;
            scanf("%1[^\n]", input);
        }
    }

    if (*input == 'y' || *input == 'Y') {
        player->tutorial = 0;
        CLEAN_BUFFER; // ancora player.tutorial vale 1 dovrà essere portato a 0 dalla funzione per vedere le regole
        return 2;
    }

    printf("ok! procediamo...\n");
    player->tutorial = 0;
    salvataggio_dati("data.txt",player);

    CLEAN_SCREEN;
    printf("Scegli la lunghezza del codice segreto. (8-6-4)\n\n\n");
    scanf(" %1d", lunghezza_codice);
    while (*lunghezza_codice != 4 && *lunghezza_codice != 6 && *lunghezza_codice != 8) {
        printf("Input non valido,riprova\n\n");
        CLEAN_BUFFER;
        scanf(" %1d", lunghezza_codice);
    }

    CLEAN_SCREEN;
    printf("Scegli la difficoltà di gioco.\n\n\nFACILE = 10 TENTATIVI\nINTERMEDIA = 8 TENTATIVI\nDIFFICILE = 6 TENTATIVI\n\n");
    CLEAN_BUFFER;


    scanf("%s", input);
    while (insensitive_compare(input, facile) != 1 && insensitive_compare(input, intermedia) != 1
           && insensitive_compare(input, difficile) != 1) {
        printf("Input non valido,riprova\n\n");
        CLEAN_BUFFER;
        scanf("%s", input);
    }


    if (insensitive_compare(facile, input) == 1) {
        *difficolta = FACILE;
        return 1;
    } else if (insensitive_compare(intermedia, input) == 1) {
        *difficolta = INTERMEDIA;
        return 1;
    } else if (insensitive_compare(difficile, input) == 1) {
        *difficolta = DIFFICILE;
        return 1;
    }
    return 0;
}

int *genera_codice(int *codice, int *lunghezza_codice) //Funziona
{
    if (codice == NULL) // se l'array codice non è stato inizializzato (quindi se è la prima partita della sessione)
    {
        codice = (int *) malloc(sizeof(int) * (*lunghezza_codice));
        // a memoria all'array in base alla lunghezza scelta dall'utente
    }

    srand(time(
            NULL)); // inizializzo generatore di n umeri casuali con tempo nullo per garantire valori diversi ad ogni esecuzione
    for (int i = 0; i < (*lunghezza_codice); i++) {
        codice[i] = (rand() % COLORI) + 1;
    }
    return codice;
}

void visualizza_regole(char nomefile[], giocatore *player) // da testare e coreggere
{
    int carattere, scelta;
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
    while (scelta != 0) {
        printf("Input non valido,riprova\n");
        CLEAN_BUFFER;
        scanf("%d", &scelta);
    }
}

void dati_giocatore(giocatore *player) { //funziona
    int scelta;

    CLEAN_SCREEN;
    printf("Ecco i tuoi dati %s\n\n", player->id);
    printf("Partite giocate= %d\n", player->partite_giocate);
    printf("Partite vinte= %d\n", player->partite_vinte);
    printf("Punti= %d\n", player->punti);

    printf("\n\nInserisci 0 per tornare  al menù:\n");
    scanf("%1d", &scelta);
    while (scelta != 0) {
        printf("Input non valido,riprova\n");
        CLEAN_BUFFER;
        scanf("%1 d", &scelta);
    }

}

void render_matrice(int **matrix, int row, int col) //funziona
{
    for (int i = 0; i < (row); i++) { //scorro le righe
        for (int j = 0; j < (col); j++) { //scorro le colonne
            switch (matrix[i][j]) {
                case 0:
                    printf("|0| ");
                    break;
                case ROSSO_VAL:
                    printf(RED "|ROSSO| " COLOR_RESET);
                    break;
                case VERDE_VAL:
                    printf(GREEN "|VERDE| " COLOR_RESET);
                    break;
                case BLU_VAL:
                    printf(BLUE "|BLU| " COLOR_RESET);
                    break;
                case GIALLO_VAL:
                    printf(YELLOW "|GIALLO| " COLOR_RESET);
                    break;
                case BIANCO_VAL:
                    printf("|BIANCO| ");
                    break;
                default:
                    printf("Valore sconosciuto\n");
                    exit(1);
                    break;
            }
        }
        printf("\n");
    }
}

void render_codice(int *array, int *lunghezza) {
    for (int i = 0; i < *lunghezza; i++) {
        switch (array[i]) {
            case ROSSO_VAL:
                printf(RED "|ROSSO| " COLOR_RESET);
                break;
            case VERDE_VAL:
                printf(GREEN "|VERDE| " COLOR_RESET);
                break;
            case BLU_VAL:
                printf(BLUE "|BLU| " COLOR_RESET);
                break;
            case GIALLO_VAL:
                printf(YELLOW "|GIALLO| " COLOR_RESET);
                break;
            case BIANCO_VAL:
                printf("|BIANCO| ");
                break;
            default:
                break;
        }
    }
}


void game(giocatore *player, int *codice, char input[], char nomefile[], int *difficolta, int *lunghezza_codice) {  //non funziona come dovrebbe

    player->partite_giocate++;
    int *input_codice = (int *) malloc((*lunghezza_codice) * sizeof(int));
    int tentativo = 0;
    int elementi_tentativo;
    int punteggi_verdi; //indicano quanti elementi del tentativo sono corretti sia per valore che per posizione.
    int punteggi_neri; //indicano quanti elementi del tentativo sono corretti per valore ma non per posizione.

    int *matched_codice = (int *) malloc((*lunghezza_codice) * sizeof(int));
    int *matched_input = (int *) malloc((*lunghezza_codice) * sizeof(int));


    char rosso[] = "rosso";
    char verde[] = "verde";
    char blu[] = "blu";
    char giallo[] = "giallo";
    char bianco[] = "bianco";



    //matrice
    // Allocazione memoria alla matrice
    int **matrix = (int **) malloc((*difficolta) * sizeof(int *)); //allocazione righe
    for (int i = 0; i < (*difficolta); i++) {
        matrix[i] = (int *) malloc((*lunghezza_codice) * sizeof(int)); //allocazione delle collone per riga
    }

    //srand(time(NULL));
    // Inizializzo la matrice
    for (int row = 0; row < (*difficolta); row++) { //scorro le righe
        for (int col = 0; col < (*lunghezza_codice); col++) { //scorro le colonne
            matrix[row][col] = 0;
        }

    }


    while (tentativo < (*difficolta)) {
        CLEAN_SCREEN;
        printf("Inserisci i colori UNO ALLA VOLTA:\n\n");
        printf(" %s, %s, %s, %s, %s\n",rosso,verde,blu,giallo,bianco);
        printf("\n\n");
        render_matrice(matrix, (*difficolta), (*lunghezza_codice));
        printf("\n\n");
        printf(GREEN "PUNTEGGI VERDI: %d\n", punteggi_verdi);
        printf(COLOR_RESET);
        printf("PUNTEGGI NERI: %d\n", punteggi_neri);
        printf("\n\n");

        elementi_tentativo = 0;
        punteggi_verdi = 0;
        punteggi_neri = 0;

        // Inizializzo matched_codice e matched_input a 0
        for (int i = 0; i < *lunghezza_codice; i++) {
            matched_codice[i] = 0;
            matched_input[i] = 0;
        }

        while (elementi_tentativo < (*lunghezza_codice)) {
            scanf("%s", input);
            while (insensitive_compare(input, rosso) != 1 && insensitive_compare(input, verde) != 1
                   && insensitive_compare(input, blu) != 1 && insensitive_compare(input, giallo) != 1
                   && insensitive_compare(input, bianco) != 1) {
                printf("Input non valido, riprova\n");
                CLEAN_BUFFER;
                scanf("%s", input);
            }
            if (insensitive_compare(input, rosso) == 1) {
                input_codice[elementi_tentativo] = ROSSO_VAL;
            } else if (insensitive_compare(input, verde) == 1) {
                input_codice[elementi_tentativo] = VERDE_VAL;
            } else if (insensitive_compare(input, blu) == 1) {
                input_codice[elementi_tentativo] = BLU_VAL;
            } else if (insensitive_compare(input, giallo) == 1) {
                input_codice[elementi_tentativo] = GIALLO_VAL;
            } else if (insensitive_compare(input, bianco) == 1) {
                input_codice[elementi_tentativo] = BIANCO_VAL;
            }
            elementi_tentativo++;
        }

        // Controllo per punteggi verdi
        for (int i = 0; i < *lunghezza_codice; i++) {
            if (codice[i] == input_codice[i]) {
                punteggi_verdi++;
                matched_codice[i] = 1;
                matched_input[i] = 1;
            }
        }

        // Punteggi bianchi (dopo quelli verdi)
        for (int i = 0; i < *lunghezza_codice; i++) {
            if (!matched_codice[i]) {
                for (int j = 0; j < *lunghezza_codice; j++) {
                    if (!matched_input[j] && codice[i] == input_codice[j]) {
                        punteggi_neri++;
                        matched_input[j] = 1;
                        break;
                    }
                }
            }
        }


        render_matrice(matrix, (*difficolta), (*lunghezza_codice));
        printf("\n\n");



        if (punteggi_verdi==(*lunghezza_codice) && punteggi_neri == 0) {
            CLEAN_SCREEN;
            printf("Congratulazioni hai vinto!!\n");
            printf("Codice da te inserito:  ");
            render_codice(input_codice,lunghezza_codice);
            printf("\n\n");

            printf("Codice segreto:");
            render_codice(codice,lunghezza_codice);
            printf("\n\n");
            player->partite_vinte++;
            player->punti+= VITTORIA;
            salvataggio_dati("data.txt",player);
            CLEAN_BUFFER;
            getchar();
            return;
        }

        for (int col = 0; col < (*lunghezza_codice); col++) { //scorro le colonne
            matrix[((*difficolta) - 1) - tentativo][col] = input_codice[col];
        }

        tentativo++;
    }
    render_matrice(matrix, (*difficolta), (*lunghezza_codice));
    printf("********** Game Over **********\n");
    printf("\n\n");
    salvataggio_dati("data.txt",player);
    CLEAN_BUFFER;
    getchar();
}


int menu()  //funziona
{
    int scelta = 5;
    printf("****** BENVENUTO IN MASTERMIND ******\n\n");
    printf("[1] INIZIA UNA NUOVA PARTITA\n");
    printf("[2] REGOLE DEL GIOCO\n");
    printf("[3] DATI DEL GIOCATORE\n");
    printf("[0] ESCI DAL GIOCO\n");
    printf("\nScelta: ");


    scanf("%1d", &scelta);
    while (scelta < 0 || scelta > 3) {
        printf("SCELTA NON VALIDA RIPROVARE!\n");
        printf("\nScelta: ");
        CLEAN_BUFFER;
        scanf("%1d", &scelta);
    }

    return scelta;
}

int main() {
    CLEAN_SCREEN;

    giocatore *player = (giocatore *) malloc(sizeof(giocatore));

    int difficolta, lunghezza_codice, scelta_menu, settings;
    char id_utente[LUNGHEZZA_ID];
    char input[ROW];
    int *codice;

    printf("****** BENVENUTO IN MASTERMIND ******\n\n\nINSERISCI IL TUO ID UTENTE:\n\n");

    fgets(input, sizeof(input), stdin);
    while (input[0] == '\n' || strlen(input) > LUNGHEZZA_ID || input[0] == ESC) {
        printf("input non valido, riprova\n");
        fgets(input, sizeof(input), stdin);
    }

    input[strcspn(input, "\n")] = '\0'; // Rimuove il newline finale, se presente
    strcpy(id_utente, input);
    /*
    la funzione strcspn scansiona
    la stringa passata nel primo parametro fino a quando non trova corrispondenza con almeno uno dei caratteri contenuti nella stringa passata
    come secondo parametro ("\n"). Ha come valore di ritorno il contatore di posizioni analizzate a partire da zero.
    Quindi abbiamo preso l'elemento dell'array "id_utente" nella posizione del carattere \n (se presente) e lo sostituiamo con il carattere nullo
    */

    verifica_id("data.txt", id_utente, player, input);


    while (1) {
        CLEAN_SCREEN;

        scelta_menu = menu();
        switch (scelta_menu) {
            case 0:
                CLEAN_SCREEN;
                free(player);
                exit(0);
                break;

            case 1:
                CLEAN_SCREEN;
                settings = settings_partita(&lunghezza_codice, &difficolta, player, input, "data.txt");

                if (settings == 2) {
                    visualizza_regole("regole.txt", player);
                }

                if (settings == 1) {
                    codice = genera_codice(codice, &lunghezza_codice);
                    game(player, codice, input, "data.txt", &difficolta, &lunghezza_codice);
                }


                break;

            case 2:
                CLEAN_SCREEN;
                visualizza_regole("regole.txt", player);
                break;

            case 3:
                CLEAN_SCREEN;
                dati_giocatore(player);
                break;

            default:
                break;
        }
    }
}