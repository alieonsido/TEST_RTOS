#!/bin/bash
sudo ./t1rm &
sudo ./t3rm &
sleep 10
sudo kill -SIGINT t1rm
sudo kill -SIGINT t3rm
sleep 10
sudo kill -SIGINT t1rm
sudo kill -SIGINT t3rm
sleep 5
sudo kill -SIGKILL t1rm
sudo kill -SIGKILL t3rm