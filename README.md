# NewVersionRPC


Se non è presente gmake nel pc, allora inizialmente utilizzare il comando:

sudo ln -s /usr/bin/make /usr/bin/gmake

Creare una cartella denominata "bin" nella HOME

Creare una cartella "data" dove andare a mettere i dati della stazione di test

Creare una cartella "output" dove verrà creato il file root per l'analisi

Modificare PATH-TO-ROOT nel Makefile e in compile.sh. 

Lanciare il compilatore ./compile.sh

Modificare PATH_TO_ROOT nel run.sh 

Lanciare il programma ./run.sh NUMERORUN