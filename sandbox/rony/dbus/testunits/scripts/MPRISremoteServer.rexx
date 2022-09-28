-- this script runs on the target computer and offers services for the main computer
-- the intention is to start the script and make the computer available for connection
-- through the main computer (vlc is started via command line interface, as no gui is necessary)

-- ip address is hardcoded in this example, has to be replaced
ownaddress="tcp:host=192.168.0.15,port=23000,family=ipv4;" 
vlcservice =.VlcService~new 
server =.DBUSServer~new(ownaddress,vlcservice) 
server~allowAnonymous=.true
server~startup
signal on halt                                 -- intercept ctl-c (jump to label "halt:")

say 'This script starts the vlc mediaplayer without graphical userinterface'
say 'Connection to this service is possible via:'  server~serverAddress
say 'Press return to quit'
parse pull enter

halt:                                          -- a ctl-c causes a jump to this label
say "shutdown server, shutdown vlc mediaplayer and closing all connections to clients .." 
server~shutdown
vlcservice~Quitvlc
.dbus~session~close

::requires "dbus.cls" -- get dbus support for ooRexx 

::class VlcService subclass DBusServiceObject 
::attribute vlc
::method init
  expose vlc
  idata='<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN"'  -
        '"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">'  -
        '<node>'  -
           '<interface name="org.freedesktop.DBus.Introspectable">'  -
             '<method name="Introspect">'  -
              '<arg name="data" direction="out" type="s"/>'  -
             '</method>'  -
          '</interface>'  -
          '<interface name="oorexx.vlc">'  -
            '<method name="Play">'  -
              '<arg name=   "file"   direction= "in" type="s"/>'  -
              '<arg name= "position" direction= "in" type="x"/>'  -
              '<arg name=  "result"  direction="out" type="b"/>'  -
             '</method>'  -
            '<method name="Stop">' -
           '<arg name="data" direction="out" type="as"/>'  -
               '</method>'  -
          '</interface>' -
         '</node>'

  self~init:super(idata) -- let superclass (DBusServiceObject) initialize 
   
  procId=getVlcId()      -- get the procid of the newest vlc, create an instance if necessary
  if procId="" then      -- no vlc instance found (and could not be created)
  do
    say "no vlc instance available, could not create one either, aborting ..."
    exit -1
  end
  -- MPRIS2 does not necessarily need a proc ID unless multiple instances are running  
  busname="org.mpris.MediaPlayer2.vlc.instance"procId          -- create MPRIS2 bus object
  objPath="/org/mpris/MediaPlayer2"                            -- define object path
  vlc = .dbus~session~getObject(busname,objPath)               -- get the vlc proxy object

::method Stop
  expose vlc
  url = vlc~Metadata['xesam:url']
  say 'Url to return =' url
  position = vlc~Position
  vlc~Stop
  return .array~of(url, position)
 
::method Play
  expose vlc
  use arg file, position
  say "file to play:" file "at position:" position
  vlc~OpenUri(file)
  call syssleep 1             -- let vlc time to load the file
  url = vlc~Metadata['xesam:url']
  vlc~Seek(position)
  if file=url then return .true  --if file loaded sucessfully
  else return .false

::method Quitvlc
  expose vlc
  vlc~Quit
 
::routine getVlcId       -- return process id of newest vlc instance of current user; if there is none, start an instance
  cmd='pgrep -n -x -u "$USER" vlc | rxqueue'   -- get procid and queue it via Rexx queue
  proc=getProc(cmd)           -- get vlc's proc id
  if proc="" then             -- found no vlc instance for this user
  do
     "cvlc --control dbus &"   -- let the shell create a new instance of vlc
     call syssleep 1.5         -- wait a little to let the new instance request a busname
     proc=getProc(cmd)        -- get vlc's proc id
  end
  return proc                 -- return the proc id

getProc: procedure            -- do the work: execute the command, parse its output
  parse arg cmd
  cmd                         -- let the shell execute the passed command
  proc=""
  do while queued()>0
     parse pull proc          -- pull the procid from the external Rexx queue, make sure its emptied
  end
  return proc