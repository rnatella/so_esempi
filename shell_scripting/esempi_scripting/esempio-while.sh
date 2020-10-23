#!/bin/bash

ls | while read LINEA
do
	if [ -f "$LINEA" ]
	then
		echo "$LINEA è un file"
	fi

	if [ -d "$LINEA" ]
	then
		echo "$LINEA è una cartella"
	fi
done

