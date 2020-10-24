#!/bin/bash

VAR="$1"

if [ "$VAR" == "" ]
then
    echo "La variabile è vuota"
else
    echo "La variabile è: $VAR"
fi

