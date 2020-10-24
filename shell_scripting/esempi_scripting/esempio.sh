#!/bin/bash

wget -q http://www.hoepliscuola.it/download/2842/la-divina-commedia.txt

echo "Il numero di linee della Divina Commedia è circa:"
cat la-divina-commedia.txt | wc -l

echo "Il numero di caratteri è circa:"
cat la-divina-commedia.txt | wc -c

