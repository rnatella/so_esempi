#!/bin/bash

wget -q -r --no-parent -nH --cut-dirs=3 \
		https://people.sc.fsu.edu/~jburkardt/data/jpg/

for F in *.jpg
do 
  convert "$F" "$F.png" &
done

jobs
wait

echo "All images have been converted."

