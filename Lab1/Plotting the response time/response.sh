#!/bin/bash
sudo ./t1rm &
sleep 12
sudo pkill -SIGINT t1rm
sleep 5
sudo pkill -SIGKILL t1rm