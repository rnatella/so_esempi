#!/bin/bash

VAR="ciao mondo"

if [ "$VAR" == "" ]
then
    echo "La variabile è vuota"
else
    echo "La variabile è: $VAR"
fi

