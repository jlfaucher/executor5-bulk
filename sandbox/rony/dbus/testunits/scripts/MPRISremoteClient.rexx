signal on halt
signal on syntax name nodevice

/* this scripts lookups a bluetooth device and accordingly to the signal value of another device whithin its range
   does some actions.
   In this example a mediaplayer running on two systems gets started and stopped dependend of the position of a
   bluetooth device.
   The track gets stopped on the machine, where the signal is lower and will continue playing on the machine, where
   the signal is high.
   
   start to play a music file on the computer and switch on bluetooth (has to be discoverable) on a device carried with you.
*/   
  parse arg serveraddress
  if (serveraddress="") then do
  say "Please add server address as argument eg. 192.168.0.24"
  exit -1
  end
  conn = .dbus~system                           -- get a system-bus connection
  bluez = conn~getObject('org.bluez','/')       -- get the bluez dbus object
  adr = bluez~DefaultAdapter                    -- query default bluetooth adapter
  adapter = conn~getObject('org.bluez',adr)     -- get the default bluetooth adapter object

  conn~listener("add",.rexxSignalListener~new(serveraddress))  -- add the listener to the connection
  conn~match("add","type='signal',path="adr",interface='org.bluez.Adapter'")  -- specify what to dispatch
  adapter~StartDiscovery                        -- instruct bluetooth adapter to query for bluetooth devices
  say "running till enter is pressed"
  parse pull quit

halt:
  adapter~StopDiscovery
  conn~close
  say "end"
  exit -1

nodevice:
  additional = (Condition("ADDITIONAL"))[2]    -- additional information about the error
  say "Bluetooth adapter error --" additional  -- second value is the error name
  exit -1

::class RexxSignalListener
::attribute active
::attribute url
::attribute position
::attribute vlcremote
::attribute vlclocal

::method init
  expose active vlclocal vlcremote
  use arg serveraddress
  remoteconn = .dbus~new('tcp:host='serveraddress',port=23000,family=ipv4;unix:path=/tmp/dbus-test;')
  vlcremote = remoteconn~getObject('oorexx.vlc','/oorexx/vlc')
  vlclocal = .dbus~session~getObject('org.mpris.MediaPlayer2.vlc','/org/mpris/MediaPlayer2')
  if (vlclocal~PlaybackStatus\='Playing') then active = .false
  else active = .true             -- music plays therefore no action is required
  
  -- very simple implementation only listens if a device disappears

::method DeviceDisappeared
  expose active
  use arg i, directory
  name = directory['Name']
  say 'device' names 'out of range or turned off'
  self~switchPlaybackStatus(1)  -- true means switch to remote
 
::method DeviceFound
  expose active 
  use arg i, directory
  name = directory['Name']
  say 'device' name 'in range and/or turned on, signal strengh:' directory['RSSI']
  if \active then self~switchPlaybackStatus(0)  -- switch to local
    
::method switchPlaybackStatus
  expose vlclocal vlcremote active
  use arg toremote 
  if toremote then do
   active=.false
   url = vlclocal~Metadata['xesam:url']
   position = vlclocal~Position
  say 'playing 'url 'at' position 'on remote computer'
  vlclocal~Stop
  vlcremote~Play(url, position) 
  end 
  else do
   say 'start local player'
      ar = vlcremote~Stop
      url = ar[1]
      position = ar[2]
      say 'file to play:' url
      vlclocal~OpenUri(url)
      call syssleep 0.5
      if (vlclocal~Metadata['xesam:url']\=url) then do
      say 'Error loading music file! maybe' url 'is not available on this machine?'
      end 
      else do
      vlclocal~Seek(position)
      vlclocal~Play
      end
      active=.true
  end
  
::method unknown
::requires DBUS.CLS         -- get access to DBus