#!/bin/bash
while :
do
        if mosquitto_sub -t wash -C 2 | grep "wash"; then
                echo "Wash"
                cat wash.txt | sendmail -t
        fi
done
