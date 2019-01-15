#!/bin/bash

sleep 60; 
LD_LIBRARY_PATH=/home/pi/Desktop/ADSRT_2018/ /home/pi/Desktop/ADSRT_2018/informe -b basedades.db -r 1422047@campus.euss.org -d 1421184@campus.euss.org)&
touch /tmp/cron.EXECUTAT
