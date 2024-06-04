/*
  Implementazione del gioco:

  - La lunghezza del codice segreto verrà scelta dall'utente prima di iniziare una partita (4 - 6 - 8).
  - L'utente potrà scegliere la difficoltà (Facile - Intermedia - Difficile):
    * Facile: 8 tentativi
    * Intermedia: 6 tentativi
    * Difficile: 4 tentativi

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
#include <ctype.h> //per funzione tolower
#include <time.h>

#define FACILE 10
#define INTERMEDIA 8
#define DIFFICILE 6

#define LUNGHEZZA_ID 11
#define COLORI 8
#define ESC 27
#define ROW 256
#define CLEAN_BUFFER while ((getchar()) != '\n')

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

typedef struct Sgiocatore
{
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
  for (int i = 0; stringa1[i] != '\0'; i++)
  {
    stringa1[i] = tolower(stringa1[i]);
  }

  // Converti stringa2 in minuscolo
  for (int i = 0; stringa2[i] != '\0'; i++)
  {
    stringa2[i] = tolower(stringa2[i]);
  }

  // Confronta le stringhe e restituisci il risultato
  int x = 0;
  while (stringa1[x] != '\0' || stringa2[x] != '\0')
  {
    if (stringa1[x] != stringa2[x])
    {
      return 0;
    }
    x++;
  }
  return 1; // Le stringhe sono uguali
}

void verifica_id(char nomefile [], char id_inserito [], giocatore* player, char buffer[]) //finita e testata FUNZIONA
{
    char id_letto [LUNGHEZZA_ID];


    FILE * fp= fopen(nomefile,"r");
    printf("ho aperto il file\n");

    if(fp == NULL){
        printf("ERRORE NELL'APERTURA DEL FILE!\n");
        exit(1);
    }
    int res=0;

    while(fscanf(fp, "%[^\n]", id_letto) != EOF){ //finchè non è alla fine del file legge fino alla fine della riga considerando gli spazi
        fgetc(fp); //consuma il carattere \n che fa andare il puntatore a capo
        printf("id letto= |%s| \n",id_letto);
        if(insensitive_compare(id_letto,id_inserito)==1){
            printf("Id trovato!\nCaricamento dei dati in memoria...\n");
            strcpy(player->id,id_letto);
            printf("player->id= %s\n",player->id);
            fscanf(fp,"%d %d %d %d",&player->partite_giocate,&player->partite_vinte,&player->punti,&player->tutorial);
            //printf("dati salvati:\n giocate= |%d|\n vinte=|%d|\n punti=|%d|\n tutorial=|%d|\n",player->partite_giocate,player->partite_vinte,player->punti,player->tutorial);
            printf("Caricamento terminato!\n");
            fclose(fp);
           return;
        } //questo while è stato testato e funziona
    }


    //se l'id non viene trovato
    printf("L'id da te inserito non è presente in memoria. Vuoi crearlo?\n y/n\n");


    scanf("%1[^\n]",buffer);
    while(*buffer != 'y' && *buffer != 'n' && *buffer != 'Y' && *buffer != 'N'){
        //printf("input= |%s|\n",buffer);
        printf("Input non valido,riprova\n");
        CLEAN_BUFFER;
        scanf("%1[^\n]",buffer);
    }
    printf("input= |%s|",buffer);

    if(*buffer=='y' || *buffer=='Y'){
        fp=fopen(nomefile,"a");

        strcpy(player->id,id_inserito);
        player->partite_giocate=0;
        player->partite_vinte=0;
        player->punti=0;
        player->tutorial=1;

        fprintf(fp,"%s\n",id_inserito);
        fprintf(fp,"%d %d %d %d\n",player->partite_giocate,player->partite_vinte,player->punti,player->tutorial);
        fprintf(fp,".\n");
        fclose(fp);
        printf("Il tuo profilo è stato creato correttamente!\n");
    }
    else if(*buffer=='n' || *buffer=='N'){
        printf("Alla prossima!!\n");
        free(player);
        exit(0);
    }

}

int settings_partita(int *lunghezza_codice, int *difficolta, giocatore *player) //da testare

{
  char consenso;
  char tmp[12];
  int difficolta_scelta = 0;
  system("clear");

  if (player->tutorial == 1)
  {
    printf("Non hai mai visto le regole, vuoi vederle?\n y/n\n");
    CLEAN_BUFFER;
    consenso = getchar();

    while (consenso != 'y' && consenso != 'n')
    {
      printf("Scelta non valida! Riprovare\n");
      CLEAN_BUFFER;
      consenso = getchar();
    }
  }

  if (consenso == 'y')
  {
    CLEAN_BUFFER; // ancora player.tutorial vale 1 dovrà essere portato a 0 dalla funzione per vedere le regole
    return 2;
  }

  if (consenso == 'n')
  {
    printf("ok! procediamo...");
    player->tutorial = 0;
    printf("Devi scegliere se vuoi il codice segreto da 4, 6 o 8 elementi:\n");
    scanf(" %d", &lunghezza_codice);
    while (lunghezza_codice != 4 || lunghezza_codice != 6 || lunghezza_codice != 8)
    {
      printf("Scelta non valida! Riprovare\n");
      CLEAN_BUFFER;
      scanf(" %d", &lunghezza_codice);
    }

    printf("\n\n Ora devi scegliere la difficoltà di gioco.\n FACILE = 10 TENTATIVI\n INTERMEDIA = 8 TENTATIVI\n DIFFICILE = 6 TENTATIVI\n");
    CLEAN_BUFFER;
    scanf(" %11s", tmp);

    if (insensitive_compare("FACILE", tmp) == 1)
    {
      *difficolta = FACILE;
      difficolta_scelta = 1;
    }

    if (insensitive_compare("INTERMEDIA", tmp) == 1)
    {
      *difficolta = INTERMEDIA;
      difficolta_scelta = 1;
    }

    if (insensitive_compare("DIFFICILE", tmp) == 1)
    {
      *difficolta = DIFFICILE;
      difficolta_scelta = 1;
    }
    while (difficolta_scelta != 1)
    {
      printf("Scelta non valida! Riprovare\n");
      CLEAN_BUFFER;
      scanf(" %11s", tmp);
    }
  }

  return 0;
  // se ritorna 0 c'è un problema, se ritorna 1 tutto ok e difficoltà settata, se ritorna 2 vuole vedere le regole
}

void genera_codice(int *codice, int *lunghezza) //da testare
{
  int tmp;

  if (codice == NULL) // se l'array codice non è stato inizializzato (quindi se è la prima partita della sessione)
  {
    codice = malloc((sizeof(int)) * (*lunghezza)); // alloca memoria all'array in base alla lunghezza scelta dall'utente
  }

  srand(time(NULL)); // inizializzo generatore di n umeri casuali con tempo nullo per garantire valori diversi ad ogni esecuzione

  for (int i = 0; i < *lunghezza; i++)
  {
    tmp = ((rand() % 8) + 1); // valore casuale fra 1 e 8
    while (codice[i] == tmp)
    {
      tmp = ((rand() % COLORI) + 1);
    }
    codice[i] = tmp;
  }
}

void visualizza_regole(char nomefile[], giocatore *player) // da testare
{
  int carattere;
  system("clear");
  player->tutorial = 0;

  FILE *fp;
  fp = fopen(nomefile, "r");
  if (fp == NULL)
  {
    printf("errore apertura file \n");
    exit(1);
  }

  // Leggi e stampa il file carattere per carattere
  while ((carattere = fgetc(fp)) != EOF)
  {
    putchar(carattere); // stampa il carattere attraverso il valore ascii dello stesso
  }

  fclose(fp);

  printf("\n\nPremi un tasto qualsiasi per tornare al menù:\n");
  getchar();  //da modificare
  system("clear");
}

int menu()  //funziona ma da testare diversi tipi di input
{
  int scelta;
  printf("****** BENVENUTO IN MASTERMIND ******\n\n");
  printf("[1] INIZIA UNA NUOVA PARTITA\n");
  printf("[2] REGOLE DEL GIOCO\n");
  printf("[3] STORICO DEL GIOCATORE\n");
  printf("[0] ESCI DAL GIOCO\n");
  printf("\nScelta: ");

  scanf("%d", &scelta);

  while (scelta < 0 || scelta > 3)
  {
    printf("SCELTA NON VALIDA RIPROVARE!\n");
    printf("\nScelta: ");
    getchar();
    scanf("%d", &scelta);
  }

  return scelta;
}

int main()
{
  system("clear");


    giocatore *player = (giocatore *)malloc(sizeof(giocatore));

  int difficolta, lunghezza_codice, input_utente, settings;
  char id_utente[LUNGHEZZA_ID];
  char buffer[ROW];
  int *codice;

  printf("****** BENVENUTO IN MASTERMIND ******\n INSERISCI IL TUO ID UTENTE:\n");

    fgets(buffer, ROW, stdin);
    printf("ho letto\n");
    while (buffer[0] == '\n' || strlen(buffer) > LUNGHEZZA_ID || buffer[0] == ESC)
    {
      printf("input non valido, riprova\n");
      fgets(buffer, sizeof(buffer), stdin);
    }
    printf("sono dopo il while\n");

  buffer[strcspn(buffer, "\n")] = '\0'; // Rimuove il newline finale, se presente
    printf("ho rimosso il new line\n");
  strcpy(id_utente, buffer);
    printf("ho copiato id utente da buffer\n");

  /* la funzione strcspn scansiona
 la stringa passata nel primo parametro fino a quando non trova corrispondenza con almeno uno dei caratteri contenuti nella stringa passata
 come secondo parametro ("\n"). Ha come valore di ritorno il contatore di posizioni analizzate a partire da zero.
 Quindi abbiamo preso l'elemento dell'array "id_utente" nella posizione del carattere \n (se presente) e lo sostituiamo con il carattere nullo*/
    printf("sto per entrare in verifica id\n");
    verifica_id("data.txt", id_utente,player,buffer);
    printf("sono uscito da verifica id\n");
    getchar();
   CLEAN_BUFFER;

  while (1)
  {
    // system("clear");
    input_utente = menu();

    switch (input_utente)
    {
    case 0:
      system("clear");
      free(player);
      exit(0);
      break;

    case 1:
      settings = settings_partita(&lunghezza_codice, &difficolta, player);

      if (settings == 2)
      {
        visualizza_regole("regole.txt", player);
      }
      if (settings == 1)
      {
        genera_codice(codice, &lunghezza_codice);
        // funzione di gioco
      }

      printf("lungheza del codice= %d\n difficoltà= %d\n", lunghezza_codice, difficolta);
      CLEAN_BUFFER;
      getchar();
      CLEAN_BUFFER;

      break;

    case 2:
      CLEAN_BUFFER;
      system("clear");
      visualizza_regole("regole.txt", player);
      break;

    case 3:
      CLEAN_BUFFER;
      // storico del giocatore
      break;

    default:
      break;
    }
  }
  return 0;
}