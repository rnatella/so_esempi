#!/bin/bash

wget -q http://www.hoepliscuola.it/download/non-esiste.txt

if [ $? -ne 0 ]
then
    echo "Download fallito :-("
else
    echo "Download riuscito!"
fi

