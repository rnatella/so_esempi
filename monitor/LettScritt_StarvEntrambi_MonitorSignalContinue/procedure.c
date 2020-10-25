#include "procedure.h"

#include <unistd.h>
#include <stdio.h>


int Leggi(struct LettScritt * ls) {

	int valore;

	enter_monitor( &(ls->m) );

	printf("Lettura - ingresso monitor\n");

	while( ls->numero_scrittori > 0 ) {

		printf("Sospensione - lettura\n");
		wait_condition( &(ls->m), VARCOND_LETTORI );
		printf("Riattivazione - lettura\n");
	}

	ls->numero_lettori++;

	printf("Numero lettori ++ : %d\n", ls->numero_lettori);

	leave_monitor( &(ls->m) );



	// ...operazione lenta...
	sleep(2);
	valore = ls->buffer;

	printf("Lettura - valore [%d]\n", valore);



	enter_monitor( &(ls->m) );

	ls->numero_lettori--;

	printf("Numero lettori -- : %d\n", ls->numero_lettori);


	if( ls->numero_lettori == 0 ) {

		printf("Lettura - signal su scrittori\n");

		signal_condition( &(ls->m), VARCOND_SCRITTORI );
	}

	leave_monitor( &(ls->m) );

	printf("Lettura - uscita monitor\n");

	return valore;

}


void Scrivi(struct LettScritt * ls, int valore) {

	enter_monitor( &(ls->m) );

	printf("Scrittura - ingresso monitor\n");

	while (ls->numero_lettori > 0 || ls->numero_scrittori > 0) {

		printf("Scrittura - sospensione\n");
		wait_condition( &(ls->m), VARCOND_SCRITTORI );
		printf("Scrittura - riattivazione\n");
	}

	ls->numero_scrittori++;

	printf("Numero scrittori ++ : %d\n", ls->numero_scrittori);

	leave_monitor( &(ls->m) );


	// ...operazione lenta...
	sleep(1);
	ls->buffer = valore;

	printf("Scrittura - valore [%d]\n", valore);



	enter_monitor( &(ls->m) );

	ls->numero_scrittori--;

	printf("Numero scrittori -- : %d\n", ls->numero_scrittori);


	if( queue_condition( &(ls->m), VARCOND_SCRITTORI ) ) {

		printf("Scrittura - signal su scrittori\n");

		signal_condition( &(ls->m), VARCOND_SCRITTORI );

	} else {

		/*
		 *  NOTA: questa soluzione è applicabile solo al caso di
		 *  monitor con semantica signal-and-continue (signal all).
		 */
		printf("Scrittura - signal all su lettori\n");
		
		signal_all( &(ls->m), VARCOND_LETTORI );
	 }
	

	leave_monitor( &(ls->m) );

	printf("Scrittura - uscita monitor\n");
}


