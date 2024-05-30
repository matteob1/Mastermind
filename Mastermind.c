/*
  Regole del gioco Mastermind:

  Mastermind è un gioco di decodifica in cui un giocatore (il codificatore) crea un codice segreto,
  e l'altro giocatore (il decodificatore) cerca di indovinarlo entro un certo numero di tentativi.

  1. Il codificatore crea un codice segreto di lunghezza fissa, composto da una sequenza di elementi.
     Gli elementi possono essere colori, lettere o numeri e possono ripetersi.

  2. Il decodificatore fa una serie di tentativi per cercare di indovinare il codice segreto.
     Ogni tentativo è una sequenza della stessa lunghezza del codice segreto.

  3. Dopo ogni tentativo, il codificatore fornisce un feedback sotto forma di due numeri:
     - Punteggi neri: indicano quanti elementi del tentativo sono corretti sia per valore che per posizione.
     - Punteggi bianchi: indicano quanti elementi del tentativo sono corretti per valore ma non per posizione.

  4. Il gioco continua finché il decodificatore non indovina esattamente il codice segreto o finché non esaurisce
     i tentativi a disposizione.

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

  id= id utente // max 10 caratteri
  games-played= n\n
  games-won= n\n
  points= n\n
  tutorial= 0/1\n
  .\n // separatore
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FACILE 10
#define INTERMEDIA 8
#define DIFFICILE 6

#define LUNGHEZZA_ID 11

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

typedef struct struttura_giocatore
{
  char id[LUNGHEZZA_ID];
  int partite_giocate;
  int partite_vinte;
  int punti;
  int tutorial;
} giocatore;

void clear_input_buffer()
{ // semplice funzione che serve per ipulire il buffer
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF)
}

int strcasecmp(const char *a, const char *b) // la funzione è da rifare
{ 
  while (*a && *b)
  { 
    if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
    {                                                                 
      return tolower((unsigned char)*a) - tolower((unsigned char)*b); 
    }
    a++;
    b++;
  }
  return tolower((unsigned char)*a) - tolower((unsigned char)*b); 
}

void verifica_id(struct giocatore *player, char id[11], char nomefile[]) // se ritorna 1 l'utente esiste già invece se ritorna 2 è stato creato un nuovo account
{

  FILE *fp;
  char id_tmp[LUNGHEZZA_ID];

  fp = fopen(nomefile, "r"); // apertura file in modalità lettura

  if (fp == NULL)
  {
    printf("errore apertura file \n");
    exit(1);
  }
  // Leggi il file blocco per blocco
  int trovato = 0;
  while (fscanf(fp, "id= %10s\n", id_tmp) == 1 && !feof(fp))
  { // legge finchè non trova un campo id e finchè non trova il campo eof (end of file)
    if (strcasecmp(id, id_tmp) == 0)
    { // chiamo la funzione di confronto non case sensitive, se il valore di ritorno è 0 allora leggo e salvo dati utente
      // Trovato l'utente, leggi le informazioni
      strcpy(player->id, id_tmp); // siccome non è possibile passare direttamente l'array id_tmp all'id del player lo copiamo con strcpy
      fscanf(fp, "games-played= %d\n", &player->partitr_giocate);
      fscanf(fp, "games-won= %d\n", &player->partite_vinte);
      fscanf(fp, "points= %d\n", &player->punti);
      fscanf(fp, "tutorial= %d\n", &player->tutorial);
      fclose(fp);
      printf("Utente trovato, sono stati recuperati i dati delle sessioni precedenti\n");
      trovato = 1;
      break;
    }
    else
    {
      // Salta il blocco di 3 righe e il separatore "."
      fscanf(fp, "games-played= %*d\n"); // nella fscanf il "*"" indica che vuoi saltare il valore letto e non assegnarlo a nessuna variabile.
      fscanf(fp, "games-won= %*d\n");
      fscanf(fp, "points= %*d\n");
      fscanf(fp, "tutorial= %*d\n");
      while (fgetc(fp) != '.' && !feof(fp))
      {
        fgetc(fp); // Legge il carattere '\n' dopo il '.'
      }
    }
  }
  fclose(fp);
  if (trovato == 0)
  {
    // arrivati a questo punto vuol dire che l'utente non esiste e bisogna crearlo
    char consenso;
    printf("L'id utente da te inserito non è presente nella memoria.\n Vuoi creare un nuovo profilo?\n y/n");
    consenso = getchar();
    if (consenso == 'y')
    {
      fp = fopen(nomefile, "a"); // apertura file in modalità append
      strcpy(player->id, id);
      fprintf("id= %10s\n", id);
      player.partite_giocate, player.partite_vinte, player.punti = 0;
      fprintf("games-played= %d\n", 0);
      fprintf("games-won= %d\n", 0);
      fprintf("points= %d\n", 0);
      fprintf("tutorial= %d\n", 1);
      fprintf(".\n");
      fclose(fp);
      system("clear");
      printf("Il tuo nuovo profilo utente è stato creato correttamente!!");
      clear_input_buffer();
    }
    else
    {
      system("clear");
      printf("Alla prossima!!");
      exit(1);
    }
  }
}

int settings_partita(int *lunghezza_codice, int *difficoltà, struct giocatore *player) // se ritorna 0 c'è un problema, se ritorna 1 tutto ok e difficoltà settata, se ritorna 2 vuole vedere le regole
{
  char consenso;
  char[12] tmp;
  system("clear");

  if (player->tutorial == 1)
  {
    printf("Non hai mai visto le regole, vuoi vederle?\n y/n\n");
    consenso = getchar();
    clear_input_buffer();

    while (consenso != 'y' && consenso != 'n')
    {
      printf("Scelta non valida! Riprovare\n");
      consenso = getchar();
      clear_input_buffer();
    }
  }

  if (consenso == 'y')
  {
    clear_input_buffer();  // ancora player.tutorial vale 1 dovrà essere portato a 0 dalla funzione per vedere le regole
    return 2;
  }

  if (consenso == 'n')
  {
    printf("ok! procediamo...");
    player->tutorial = 0;
    printf("Devi scegliere se vuoi il codice segreto da 4, 6 o 8 elementi:\n");
    scanf(" %d", *lunghezza_codice);
    clear_input_buffer();
    printf("\n\n Ora devi scegliere la difficoltà di gioco.\n FACILE = 10 TENTATIVI\n INTERMEDIA = 8 TENTATIVI\n DIFFICILE = 6 TENTATIVI\n");
    scanf(" %11s", tmp);
    clear_input_buffer();

    while (strcasecmp(tmp, "FACILE") != 1 || strcasecmp(tmp, "INTERMEDIA") != 1 || strcasecmp(tmp, "DIFFICILE") != 1)
    {
      printf("Input non valido! Riprovare:\n");
      scanf(" %11s", tmp);
      clear_input_buffer();
    }

    if (strcasecmp(tmp, "FACILE") == 1)
    {
      *difficoltà = FACILE;
      return 1;
    }

    if (strcasecmp(tmp, "INTERMEDIA") == 1)
    {
      *difficoltà = INTERMEDIA;
      return 1;
    }

    if (strcasecmp(tmp, "DIFFICILE") == 1)
    {
      *difficoltà = DIFFICILE;
      return 1;
    }
  }

  return 0;
}

int menu()
{
  int scelta;
  printf("****** BENVENUTO IN MASTERMIND ******\n\n");
  printf("[1] INIZIA UNA NUOVA PARTITA\n");
  printf("[2] REGOLE DEL GIOCO\n");
  printf("[3] STORICO DEL GIOCATORE\n");
  printf("[0] ESCI DAL GIOCO\n");
  printf("\n Scelta: ");

  scanf("%d", &scelta);

  while (scleta < 0 || scelta > 3)
  {
    printf("SCELTA NON VALIDA RIPROVARE!\n");
    printf("\n Scelta: ");
    getchar();
    scanf("%d", &scelta);
  }

  return scelta;
}

int main()
{
  system("clear");

  struct giocatore *player;                                      // Dichiarazione del puntatore alla struttura
  player = (struct giocatore *)malloc(sizeof(struct giocatore)); // Allocazione di memoria per la struttura

  int difficoltà, lunghezza_codice, input_utente;
  char id_utente[LUNGHEZZA_ID]; // Buffer per 10 caratteri + terminatore nullo (\0) che viene aggiunto automaticamente alla fine dello scanf

  printf("****** BENVENUTO IN MASTERMIND ******\n INSERISCI IL TUO ID UTENTE:\n");
  // Usa fgets per leggere la stringa, inclusi gli spazi
  if (fgets(id_utente, sizeof(id_utente), stdin) != NULL)
  // Il terzo parametro di fgets in questo caso è 'stdin' e sta ad indicare che l'input proviene dallo standard input, cioè dalla tastiera.
  {
    // Rimuove il newline finale, se presente
    id_utente[strcspn(id_utente, "\n")] = '\0'; /* la funzione strcspn scansiona
    la stringa passata nel primo parametro fino a quando non trova corrispondenza con almeno uno dei caratteri contenuti nella stringa passata
    come secondo parametro ("\n"). Ha come valore di ritorno il contatore di posizioni analizzate a partire da zero.
    Quindi abbiamo preso l'elemento dell'array "id_utente" nella posizione del carattere \n (se presente) e lo sostituiamo con il carattere nullo*/
  }

  /*printf("Stringa letta: '%s'\n", id_utente);*/

  clear_input_buffer();
  verifica_id(player, id_utente, "data.txt");

  

    while (1)
    {
      input_utente = menu();

      switch (input_utente)
      {
      case 0:
        system("clear");
        exit(0);
        break;

      case 1:
        clear_input_buffer();
        int settings= settings_partita(&lunghezza_codice,&difficoltà,player);
        if(settings == 2){
          //funzione che chiama la lettura delle regole ed imposta tutorial a 0
        }
        if(settings == 1){
          //funzione di gioco
        }
        break;

      case 2:
        clear_input_buffer();
        system("clear");
        //funzione che chiama la lettura delle regole ed imposta tutorial a 0
        break;
      
      case 3:
      clear_input_buffer();
        //storico del giocatore
        break;

      default:
        break;
      }
    }
  return 0;
}