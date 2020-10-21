#!/bin/bash

find . -type d -exec du -s -m {} \; > find.txt

cat find.txt | sort -n -r | head -10

