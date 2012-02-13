#!/bin/bash
ifconfig |grep "inet "| grep -v "127.0.0.1"|cut -d " " -f 10 > my_ip
