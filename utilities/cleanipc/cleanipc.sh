#!/bin/bash

ipcs -m | grep "0x" | awk '{ system("ipcrm -m" $2) }'
ipcs -s | grep "0x" | awk '{ system("ipcrm -s" $2) }'
ipcs -q | grep "0x" | awk '{ system("ipcrm -q" $2) }'
