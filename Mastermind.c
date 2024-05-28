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

  . // separatore
  id= id utente // max 10 caratteri
  games-played= n
  games-won= n
  points= n
*/



#include <stdio.h>
#include <stdlib.h>

#define FACILE 8
#define INTERMEDIA 6
#define DIFFICILE 4


int main (){



    return 0;
}