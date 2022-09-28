#!/usr/bin/rexx
/* demonstrates how IP address is aquired from org.freedesktop.NetworkManager
   converts decimal IP address to decimal-dot representation */

signal on halt

busname='org.freedesktop.NetworkManager'
conn=.dbus~system                                  
nwmanager = conn~getObject(busname,'/org/freedesktop/NetworkManager')                        
activeconnectionspaths = nwmanager~ActiveConnections
-- get the path to the first active connection
activeconnection = conn~getObject(busname, activeconnectionspaths[1])
-- get the device path to the first active connection
activedevicespath = activeconnection~Devices
-- get the first active device
activedevice = conn~getObject(busname,activedevicespath[1])

-- Ip4Address returns an UINT32 decimal address
decaddress = activedevice~Ip4Address
numeric digits 20
hexaddress = d2x(decaddress)

ipaddr= x2d(SUBSTR(hexaddress,7 ,2 ))||'.'||x2d(SUBSTR(hexaddress,5 ,2 ))||'.' -
           x2d(SUBSTR(hexaddress,3 ,2 ))||'.'||x2d(SUBSTR(hexaddress,1 ,2 ))

ipaddress = space(ipaddr,0)   -- removes empty spaces from the string 

say 'Your IP address is:' ipaddress

halt:
conn~close

::requires DBUS.CLS         -- get access to DBus