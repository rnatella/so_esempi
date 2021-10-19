#!/bin/bash

URL=http://www.mirrorservice.org/sites/ftp.ibiblio.org/pub/docs/books/gutenberg/1/0/1/1012/1012-0.txt

wget -q $URL -O la-divina-commedia.txt

echo "Il numero di linee della Divina Commedia è circa:"
cat la-divina-commedia.txt | wc -l

echo "Il numero di caratteri è circa:"
cat la-divina-commedia.txt | wc -c

