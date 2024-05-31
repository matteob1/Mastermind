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
#include <ctype.h> //per funzione tolower
#include <time.h>

#define FACILE 10
#define INTERMEDIA 8
#define DIFFICILE 6

#define LUNGHEZZA_ID 10
#define COLORI 8

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

void clear_input_buffer() //funziona
{ // semplice funzione che serve per ipulire il buffer
  int ch;
  do
  {
    ch = getchar();
  } while (ch != '\n' && ch != EOF);
}

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
  while (stringa1[x] != '\0' && stringa2[x] != '\0')
  {
    if (stringa1[x] != stringa2[x])
    {
      return 0;
    }
    x++;
  }
  return 1; // Le stringhe sono uguali
}

void verifica_id() //da rifare completamente
{
  return;
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
    clear_input_buffer();
    consenso = getchar();

    while (consenso != 'y' && consenso != 'n')
    {
      printf("Scelta non valida! Riprovare\n");
      clear_input_buffer();
      consenso = getchar();
    }
  }

  if (consenso == 'y')
  {
    clear_input_buffer(); // ancora player.tutorial vale 1 dovrà essere portato a 0 dalla funzione per vedere le regole
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
      clear_input_buffer();
      scanf(" %d", &lunghezza_codice);
    }

    printf("\n\n Ora devi scegliere la difficoltà di gioco.\n FACILE = 10 TENTATIVI\n INTERMEDIA = 8 TENTATIVI\n DIFFICILE = 6 TENTATIVI\n");
    clear_input_buffer();
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
      clear_input_buffer();
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

void visualizza_regole(char nomefile[], giocatore *player) // funziona
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
  getchar();
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

  struct giocatore *player;           // Dichiarazione del puntatore alla struttura
  player = malloc(sizeof(giocatore)); // Allocazione di memoria per la struttura

  int difficolta, lunghezza_codice, input_utente, settings;
  char id_utente[LUNGHEZZA_ID];
  char appoggio[256];
  char esc_char = 27;
  int *codice;

  printf("****** BENVENUTO IN MASTERMIND ******\n INSERISCI IL TUO ID UTENTE:\n");

  while (fgets(appoggio, 256, stdin))
  {
    while (appoggio[0] == '\n' || strlen(appoggio) > LUNGHEZZA_ID || appoggio[0] == esc_char)
    {
      printf("input non valido, riprova\n");
      fgets(appoggio, sizeof(appoggio), stdin);
    }
    break;
  }

  appoggio[strcspn(appoggio, "\n")] = '\0'; // Rimuove il newline finale, se presente */
  strcpy(id_utente, appoggio);

  /* la funzione strcspn scansiona
 la stringa passata nel primo parametro fino a quando non trova corrispondenza con almeno uno dei caratteri contenuti nella stringa passata
 come secondo parametro ("\n"). Ha come valore di ritorno il contatore di posizioni analizzate a partire da zero.
 Quindi abbiamo preso l'elemento dell'array "id_utente" nella posizione del carattere \n (se presente) e lo sostituiamo con il carattere nullo*/


  while (1)
  {
    // system("clear");
    input_utente = menu();

    switch (input_utente)
    {
    case 0:
      system("clear");
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
      clear_input_buffer();
      getchar();
      clear_input_buffer();

      break;

    case 2:
      clear_input_buffer();
      system("clear");
      visualizza_regole("regole.txt", player);
      break;

    case 3:
      clear_input_buffer();
      // storico del giocatore
      break;

    default:
      break;
    }
  }
  return 0;
}