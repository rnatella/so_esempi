#!/bin/bash

# Occorre avere installato il pacchetto "pdftk-java":
#   sudo apt install pdftk-java

awk -F, '{print $3}' list.txt | while read PDF
do 
	I=$(($I+1))

	wget -O "OSTEP-$I-$PDF"  "http://pages.cs.wisc.edu/~remzi/OSTEP/$PDF"
done

pdftk OSTEP-*.pdf cat output output.pdf

export PAGE=1

ls -v OSTEP-*.pdf | while read PDF
do
	HEAD=`pdftotext $PDF -| head -2 |sed 'N;s/\n/ /'`
	NUM=`pdftk $PDF dump_data | grep NumberOfPages | awk '{print $2}' | tr -dc '[0-9]'`
	echo "$HEAD/$PAGE"
	
	if [[ $HEAD == *"Part"* ]]
	then
		HEAD=`pdftotext -f 3 $PDF -|head -2|sed 'N;s/\n/ /'`
		echo "$HEAD/"$(($PAGE+2))
	fi
	
	PAGE=$(($PAGE+$NUM))

done | tee toc.txt

java -jar ./jpdfbookmarks-2.5.2/jpdfbookmarks.jar  --apply toc.txt --out OSTEP.pdf output.pdf


rm -f OSTEP-*.pdf
rm -f toc.txt
rm -f output.pdf

