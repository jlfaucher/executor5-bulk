#!/usr/bin/rexx
/* this script demonstrates some functionalities of UDISK2
   start with switch -r for registration of a new USB device
   start with switch -f for definition of the backup file
   Once configured, start the script without switch and plug in an USB stick*/

parse arg switch1 switch2

say '#'~copies(51)
say '#### automatic backup program using dbusoorexx ####'
say '#'~copies(51)
say 

registermode = .false

if (switch1 = '-r' | switch2 = '-r') then registermode=.true
if (switch1 = '-f' | switch2 = '-f') then do
  fc = .bsf~new('javax.swing.JFileChooser')
  fc~setDialogTitle('please select file to backup')
    if fc~showOpenDialog(.nil)=0 then do
      call writebackupfile(fc~getSelectedFile~getCanonicalPath)
      end
    else say 'cancelled no (new) backupfile defined'
end


filename = getbackupfilename()
if (filename=.nil) then exit 0 
else say 'Backup file:' filename

signal on halt

if (stream('backup.device','c','query exists')\="") then do
    if (\registermode) then do
say 'Ready, ... waiting for registered USB-devices'
  end
else say 'Ready, ... waiting for USB-devices to register'
  end
  else do
   if (registermode) then say 'Please attach device to register'
  else do
  say 'There is no USB device registered'
  say 'Please start the program again with the switch -r and plug in the device'
    exit -1
  end
  end
  
conn=.dbus~system                   -- get the system connection
listener=.rexxSignalListener~new(registermode)    -- create an instance of our signal handler
conn~listener('add',listener) --,interface='org.freedesktop.DBus.ObjectManager')       -- add the listener to the connection
conn~match('add',"type='signal',interface='org.freedesktop.DBus.ObjectManager'",.true)   
  
parse pull quit                  -- wait until enter is pressed

halt:
conn~close
exit -1

::class RexxSignalListener
::attribute drive
::attribute registermode
::attribute infoready
::attribute spath
::method init
  expose infoready registermode
  use arg registermode 
  infoready=.false
  stickregistered=.false
  
::method InterfacesAdded
  expose drive infoready registermode spath
  use arg path, dict, slotdir
  do index over dict
     if (index='org.freedesktop.UDisks2.Block') then do
       if (dict[index]['IdUsage'] = "filesystem") then do 
           drive = dict[index]['Drive']
           spath = path
           infoready = .true
       end 
        else infoready = .false
     end
  end

 if (infoready) then do
 id=.dbus~system~getObject("org.freedesktop.UDisks2",drive)~Id
infoready = .false

if  usbRegistered(id) then do
 if (registermode) then say 'this device is already registered,' -
           'please start program without switch, or add another device'
 else call copytoUSB(path)
end
else do
if (registermode) then call registerUSB(id)
else say 'Device added, but not a backup device, start with switch -r to register'
end
 end

::method unknown                   -- necessary to intercept unknown signals
  use arg methName
 
::routine loadfile                 -- loads the file where registered usb devices are stored
  devices =.list~new               -- returns a list of all device IDs 
  do  while lines('backup.device')\==0                                             
  devices~append(linein('backup.device'))
  end
  return devices

::routine usbRegistered  -- returns true if the id in the parameter is already registered
 use arg id
 devices=loadfile()
  if (devices~hasItem(id)) then return .true
   else return .false
  
::routine registerUSB
  use arg id
  if (lineout('backup.device',id)==0) then do
   say 'device:' id 'added sucessfully'
   say 'upon next start of the program, this device is used for backup'
  end  

::routine copytoUSB -- the backup file is zipped and copied to the usb device
  use arg path       -- path of the usb device's filesystem
  mountpath=""
  udisk = .dbus~system~getObject("org.freedesktop.UDisks2",path)
  say "wait a little bit if automounted"
  call syssleep 3
  mountpoints = udisk~MountPoints
  say 'registered device plugged in, starting the backup'
  if (mountpoints~size>1) then do
    do i over mountpoints
 -- mountpath = mountpath || d2c(i)            -- not necessary
    mountpath = mountpath || i
  end
 end                              -- if not mounted automatically
  else mountpath=udisk~Mount(.directory~new~~put(.array~of(),""))
  name = date(s)"_"time(s) || '.zip'
  --cmd = 'zip -j' mountpath'/'date(s)"_"time(s) getbackupfilename()
  cmd = 'zip -j' name getbackupfilename()
  cmd2= 'cp' name mountpath ||'/'|| name
  say cmd
  say cmd2
  cmd                   -- executes zipping
  cmd2                  -- executes copying
  
::routine writebackupfile       -- stores the full path of the file to backup
  use arg filename
  file=.stream~new('backup.file')~~open('both replace') 
  file~lineout(filename)
  file~close
  
::routine getbackupfilename
  file=.stream~new('backup.file')
  if file~query("exists")="" then do
    say 'Sorry, you have to define a file for backup first'
    say 'Please start the program again with switch -f' 
  return .nil
  end
   name = linein('backup.file')
  file~close
  return name
  
::requires DBUS.CLS         -- get access to DBus