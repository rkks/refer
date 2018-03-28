#!/bin/bash
#
# setup a simple access point using the madwifi (atheros) drivers
# use mac adresses instead of a key (simply using keys is easy! ;-)

{
ifconfig ath_br down
ifconfig ath0 down

brctl delbr ath_br
wlanconfig ath0 destroy

while iptables -D FORWARD -j ath_check; do :; done
while iptables -D INPUT -j ath_check; do :; done
iptables -F ath_check
iptables -X ath_check
} &> /dev/null

iptables -N ath_check
iptables -A ath_check -i ath0 -m mac --mac-source 00:0f:b5:4b:b8:57 -j RETURN # laptop-chan
iptables -A ath_check -i ath0 -m mac --mac-source 00:17:f2:e9:00:20 -j RETURN # lena macbook
iptables -A ath_check -i ath0 -m mac --mac-source 00:14:51:ee:75:c2 -j RETURN # ak macbook
iptables -A ath_check -i ath0 -j DROP
iptables -A FORWARD -j ath_check
iptables -A INPUT -j ath_check

wlanconfig ath0 create wlandev wifi0 wlanmode ap
iwconfig ath0 essid patanjali key off
ifconfig ath0 up

brctl addbr ath_br
brctl addif ath_br ath0
brctl addif ath_br eth0
ifconfig ath_br up

ip a flush dev eth0
ip a add dev ath_br 192.168.0.30/24
ip r add default via 192.168.0.1

