#!/bin/bash

echo "Flushing the page cache..."
echo "(note: run with sudo)"

sync
echo 3 > /proc/sys/vm/drop_caches
