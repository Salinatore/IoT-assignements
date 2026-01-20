# Real-Time Water Level Monitor

- CUS: formato da 3 componenti le connessioni, gli handler e il modello.
    - connessioni: Questi moduli si occupano di gestire le connessioni, quindi l'invio e l'arrivo dei messaggi.
    - handler: Si occupa di costruire e fare parsing dei messaggi seguendo una logica predefinita, per poi inviarli tramite le connessioni.
    - modello: 
    Questo è il modulo più importante: contiene una rappresentazione completa dello stato del sistema.
    Il modello incapsula l’oggetto Stato, che mantiene lo stato attuale del sistema ed è responsabile della gestione delle transizioni. Inoltre, il modulo ha il compito di aggiornare gli altri sottosistemi e di ricevere le richieste di cambio di modalità. Tutte le richieste di cambiamento passano attraverso il modello, che ne verifica la correttezza. 
- DBS
- TMS
- WCS