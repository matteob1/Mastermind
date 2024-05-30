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
#include <ctype.h>  //per funzione tolower
#include <time.h>

#define FACILE 10
#define INTERMEDIA 8
#define DIFFICILE 6

#define LUNGHEZZA_ID 11
#define COLORI 8
#define ESC_CHAR 0x1B

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

void clear_input_buffer() { // semplice funzione che serve per ipulire il buffer
    int ch;
    do {
        ch = getchar();
    } while (ch != '\n' && ch != EOF);
}

int strcasecmp(char stringa1[], char stringa2[]) {
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
    while (stringa1[x] != '\0' && stringa2[x] != '\0') {
        if (stringa1[x] != stringa2[x]) {
            return 1;
        }
        x++;
    }
    return 0; // Le stringhe sono uguali
}

void verifica_id(giocatore *player, char id[], char nomefile[]) // se ritorna 1 l'utente esiste già invece se ritorna 2 è stato creato un nuovo account
{

  FILE *fp;
  char id_tmp[LUNGHEZZA_ID];
  int trovato = 0;

  fp = fopen(nomefile, "r"); // apertura file in modalità lettura

  printf("ho aperto il file\n");

  if (fp == NULL)
  {
    printf("errore apertura file \n");
    exit(1);
  }
  // Leggi il file blocco per blocco

  while (fscanf(fp, "id= %s\n", id_tmp) == 1 && !feof(fp))
  { // legge finchè non trova un campo id e finchè non trova il campo eof (end of file)
    if (strcasecmp(id, id_tmp) == 0)
    { // chiamo la funzione di confronto non case sensitive, se il valore di ritorno è 0 allora leggo e salvo dati utente
      // Trovato l'utente, leggi le informazioni

      printf("ho trovato l'id\n");

      strcpy(player->id, id_tmp); // siccome non è possibile passare direttamente l'array id_tmp all'id del player lo copiamo con strcpy
      fscanf(fp, "games-played= %d\n", &player->partite_giocate);
      fscanf(fp, "games-won= %d\n", &player->partite_vinte);
      fscanf(fp, "points= %d\n", &player->punti);
      fscanf(fp, "tutorial= %d\n", &player->tutorial);

      printf("ho salvato i dati\n");

      fclose(fp);

      printf("Utente trovato, sono stati recuperati i dati delle sessioni precedenti\n");
      getchar();
      system("clear");
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
    printf("L'id utente da te inserito non è presente nella memoria.\n Vuoi creare un nuovo profilo?\n y/n\n\n");
    consenso = getchar();
    if (consenso == 'y')
    {
      printf("ho aperto il file\n");

      fp = fopen(nomefile, "a"); // apertura file in modalità append
      strcpy(player->id, id);
      fprintf(fp, "id= %10s\n", id);
      player->partite_giocate = 0;
      player->partite_vinte = 0;
      player->punti = 0;
      fprintf(fp, "games-played= %d\n", 0);
      fprintf(fp, "games-won= %d\n", 0);
      fprintf(fp, "points= %d\n", 0);
      fprintf(fp, "tutorial= %d\n", 1);
      fprintf(fp, ".\n");
      fclose(fp);
      system("clear");
      printf("Il tuo nuovo profilo utente è stato creato correttamente!!\n");
      clear_input_buffer();
    }
    else
    {
      system("clear");
      printf("Alla prossima!!\n");
      exit(1);
    }
  }
}

int settings_partita(int *lunghezza_codice, int *difficolta, giocatore *player) // se ritorna 0 c'è un problema, se ritorna 1 tutto ok e difficoltà settata, se ritorna 2 vuole vedere le regole
{
  char consenso;
  char tmp[20];
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
    clear_input_buffer();
    printf("\n\n Ora devi scegliere la difficoltà di gioco.\n FACILE = 10 TENTATIVI\n INTERMEDIA = 8 TENTATIVI\n DIFFICILE = 6 TENTATIVI\n");
    scanf(" %11s", tmp);
  

   while (strcasecmp(tmp, "FACILE") != 0 || strcasecmp(tmp, "INTERMEDIA") != 0 || strcasecmp(tmp, "DIFFICILE") != 0)
    {
      printf("Input non valido! Riprovare:\n");
      scanf(" %11s", tmp);
      clear_input_buffer();
    }

    if (strcasecmp(tmp, "FACILE") == 1)
    {
      *difficolta = FACILE;
      return 1;
    }

    if (strcasecmp(tmp, "INTERMEDIA") == 1)
    {
      *difficolta = INTERMEDIA;
      return 1;
    }

    if (strcasecmp(tmp, "DIFFICILE") == 1)
    {
      *difficolta = DIFFICILE;
      return 1;
    }
  }

  return 0;
}

void genera_codice(int *codice, int *lunghezza)
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

void visualizza_regole(char nomefile[], giocatore *player)
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

  
  printf("\n\nPremi 'esc' per uscire:\n");
  while ((carattere = getchar()) != ESC_CHAR) {
        printf("volevi dire 'esc' ?\n");
  }
}

int menu()
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
    printf("\n Scelta: ");
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
  char id_utente[LUNGHEZZA_ID]; // Buffer per 10 caratteri + terminatore nullo (\0) che viene aggiunto automaticamente alla fine dello scanf

  int *codice;

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