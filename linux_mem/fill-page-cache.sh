#!/bin/bash

echo "Filling the page cache with random stuff from the filesystem..."

find /usr -type f -exec cat {} \; > /dev/null 2>&1

