#!/bin/bash

wget https://github.com/acaudwell/Logstalgia/raw/master/data/example.log

cat example.log | cut -d ' ' -f 1 | sort | uniq -c | sort -rn | head -3

