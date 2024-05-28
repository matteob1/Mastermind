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
  games-played= n
  games-won= n
  points= n
  . // separatore
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FACILE 8
#define INTERMEDIA 6
#define DIFFICILE 4
#define ROSSO 1
#define VERDE 2
#define BLU 3
#define GIALLO 4
#define MARRONE 5
#define ARANCIONE 6
#define NERO 7
#define BIANCO 8

typedef struct struttura_giocatore
{
  char id[11];
  int partite_giocate;
  int partite_vinte;
  int punti
  
} giocatore;



int strcasecmp(const char *a, const char *b) { //funzione che dati due puntatori a costanti a e b li confronta in maniera non case sensitive
    while (*a && *b) { //Il ciclo while (*a && *b) continua finché entrambe le stringhe non raggiungono il terminatore di stringa ('\0') 
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {  //con le funzioni tolower porto i caratteri in minuscolo e se sono diversi (hanno un valore ascii diverso)
            return tolower((unsigned char)*a) - tolower((unsigned char)*b);// restituisco il valore della loro differenza
        }
        a++; //carattere successivo di a (incremento del puntatore)
        b++; //carattere successivo di b (incremento del puntatore)
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b); // restituisco il valore della loro differenza (se uguali 0)
}


void verifica_id (giocatore player, char id [11], char nomefile[]){

  FILE *fp;
  char id_tmp [11];
  fp = fopen(nomefile,"r");//apertura file in modalità lettura
   if (fp == NULL) {
		printf("errore apertura file \n");
		exit(1);
	}
     // Leggi il file blocco per blocco
    while (fscanf(fp, "id= %10s\n", id_tmp) == 1 && !feof(fp)) { // legge finchè non trova un campo id e finchè non trova il campo eof (end of file)
        if (strcasecmp(id, id_tmp) == 0) { //chiamo la funzione di confronto non case sensitive, se il valore di ritorno è 0 allora leggo e salvo dati utente
            // Trovato l'utente, leggi le informazioni
            strcpy(player.id,id_tmp); // siccome non è possibile passare direttamente l'array id_tmp all'id del player lo copiamo con strcpy
            fscanf(fp, "games-played= %d\n", &player.partite_giocate);
            fscanf(fp, "games-won= %d\n", &player.partite_vinte);
            fscanf(fp, "points= %d\n", &player.punti);
            fclose(fp);
            printf("Utente trovato, sono stati recuperati i dati delle sessioni precedenti\n");

        } else {
            // Salta il blocco di 3 righe e il separatore "."
            fscanf(fp, "games-played= %*d\n");  // nella fscanf il "*"" indica che vuoi saltare il valore letto e non assegnarlo a nessuna variabile.
            fscanf(fp, "games-won= %*d\n");
            fscanf(fp, "points= %*d\n");
            while (fgetc(fp) != '.' && !feof(fp)); 
            fgetc(fp); // Legge il carattere '\n' dopo il '.'
        }
    }
    fclose(fp);

    // arrivati a questo punto vuol dire che l'utente non esiste e bisogna crearlo
    char consenso;
    printf("L'id utente da te inserito non è presente nella memoria.\n Vuoi creare un nuovo profilo?\n y/n");
    if(getc(consenso)== 'y'){
      fp=fopen(nomefile,"a"); //apertura file in modalità append
      strcpy(player.id,id);
      fprintf("id= %10s\n",id);
      player.partite_giocate, player.partite_vinte, player.punti = 0;
      fprintf("games-played= %d\n",0);
      fprintf("games-won= %d\n",0);
      fprintf("points= %d\n",0);
      fprintf(".\n");
      fclose(fp);
      system(clear);
      printf("Il tuo nuovo profilo utente è stato creato correttamente!!");
    }
    else {
      system(clear);
      printf("Alla prossima!!");
      exit(1);
    }
}



int main()
{
  giocatore player;
  char id_utente[11]; // Buffer per 10 caratteri + terminatore nullo (\0) che viene aggiunto automaticamente alla fine dello scanf
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

  return 0;
}