#!/bin/bash
#
# rc.local
#
# This script is executed at the end of each multiuser runlevel.
# Make sure that the script will "exit 0" on success or any other value on error.
#
# In order to enable or disable this script  just change the execution bits.
#
# By default this script does nothing.
# Print the IP address
_IP=$(hostname -I)||true
if ["$_IP"]; then
  printf "My IP address is %s\n" "$_IP"
fi
# RPi Network Conf Bootstrapper
createAdHocNetwork(){
echo "Creating ad-hoc network"
ifconfig wlan0 down
iwconfig wlan0 mode ad-hoc
iwconfig wlan0 key aaaaa11111 #WEP key you can change this but keep it 10 digits
iwconfig wlan0 essid SportsOClock #SSID set this to whatever you want
ifconfig wlan0 10.0.0.200 netmask 255.255.255.0 up
/usr/sbin/dhcpd wlan0
echo "Ad-hoc network created"
echo "The server ip is 10.0.0.200"
}
echo "================================="
echo "RPi Network Conf Bootstrapper 0.2"
echo "================================="
echo "Scanning for known WiFi networks"
connected=false
for i in 1 #Initially was for i in 1 2 3 but found it took too long to failover to adhoc
do
if connected=false
then
echo "Starting supplicant for WPA/WPA2"
wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf> /dev/null 2>&1
echo "Obtaining IP from DHCP"
if dhclient -1 wlan0
then
echo "Connected to WiFi"
connected=true
iwconfig 2> /dev/null | grep ESSID > /etc/wpa_supplicant/my_current_ssid.txt
cat /etc/wpa_supplicant/my_current_ssid.txt
echo "My IP address is:"
hostname -I
break
else
echo "DHCP server did not respond with an IP lease (DHCPOFFER)"
wpa_cli terminate
break
fi
else
echo "No SSIDs listed in your wpa_supplicant.conf file were in range"
fi
done
if ! $connected; then
createAdHocNetwork
fi
exit 0

