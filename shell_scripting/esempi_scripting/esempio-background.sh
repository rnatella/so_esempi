#!/bin/bash

URL="https://people.sc.fsu.edu/~jburkardt/data/jpg/"

mkdir -p ./immagini/

wget -r --no-parent -nH --cut-dirs=3 -P ./immagini/ $URL

for F in immagini/*.jpg
do
  echo "Conversione di $F"
  convert "$F" "$F.png" &
done

jobs
wait

echo "All images have been converted."

