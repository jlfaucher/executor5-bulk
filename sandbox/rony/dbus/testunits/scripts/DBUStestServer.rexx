-- invoked by the test script in its setup method
-- replys object types with dedicated methods
-- replys objext by guessing the object type

objectPath  = "/oorexx/dbus/ooTestService"
busName     =  "oorexx.dbus.ooTestService"
interface   =  "oorexx.dbus.ooTestService"

conn=.dbus~session         -- get the session bus

signal on syntax name halt -- make sure message loop gets stopped
signal on halt             -- intercept ctl-c or closing terminal in which Rexx runs

res=conn~busName("request", busName)
if res<>.dbus.dir~primaryOwner & res<>.dbus.dir~alreadyOwner then   -- o.k., wait for clients
do
   say "res="res "problem with requesting the bus name" busName", aborting ..."
   exit -1
end

-- the first four classes are DBUsServiceObjects
testserver=.TestService~new     -- provides xml data
testserver2=.TestService2~new   -- loads xml data from file
testserver3=.TestService3~new   -- created on the fly
testserver4=.TestService4~new   -- with introspect method
testserver5=.TestService5~new   -- with method unknown return
-- the following classes are DBusServers
testserver6 = .TestServer~new    -- "tcp:host=localhost,port=23000,family=ipv4;"
testserver7 = .TestServer~new    -- "unix:path=/tmp/dbus-test" 


conn~serviceObject('add', objectPath, testserver)
conn~serviceObject('add', "/oorexx/dbus/ooTestService2", testserver2)
conn~serviceObject('add', "/oorexx/dbus/ooTestService3", testserver3)
conn~serviceObject('add', "/oorexx/dbus/ooTestService4", testserver4)
conn~serviceObject('add', "/oorexx/dbus/ooTestService5", testserver5)

-- necessary for DBus debuggers else service4 is not visible, but activation deactivates others
-- conn~serviceObject('add', "default", .IDBusPathMaker~new("/oorexx/dbus/ooTestService4"))
   .IDBusPathMaker~publishAllServiceObjects(conn)           -- makes services visible for debuggers
   
privateaddress = "tcp:host=localhost,port=23000,family=ipv4;unix:path=/tmp/dbus-test;"
privateaddress2 = "unix:path=/tmp/dbus-test" 

privateserver =.DBUSServer~new(privateaddress,testserver6,)
privateserver~allowAnonymous=.true
privateserver~startup
  
privateserver2 =.DBUSServer~new(privateaddress2,testserver7,)
privateserver2~allowAnonymous=.true
privateserver2~startup   
   

-- emit signal to the testgroup that the server setup is finished
  conn~message("signal", objectPath, interface, "Ready", "sb", "started", .true)
.local~dbus.shutdown = .false
-- wait until method Shutdown is called and .local~shutdown is set to -true   

say "Testserver is up"  
  
 /* 
do forever
if dbus.shutdown then leave
end
  say "ending"
halt:
 conn~message("signal", objectPath, interface, "Exit", "s", "Goodbye!")
   conn~close       --  stop message loop
   privateserver~shutdown
   privateserver2~shutdown
   exit -1  
*/

::requires 'dbus.cls'         -- get dbus support for ooRexx

-- first class with introspection helper and SetInfo implemented as method, MethodName and Info as attributes
::class TestService subclass DBusServiceObjectProxy
::attribute Info
::attribute MethodName
::method init
  expose introspectData Info

  node=.IntrospectHelper~new      -- create root node (could supply a name)
  if=node~addInterface("org.freedesktop.DBus.Introspectable")
  if~addMethod("Introspect",,"s")  -- name, in-signature, out-signature
  if=node~addInterface("org.freedesktop.DBus.Properties")
  if~addMethod("Get","ss","v")
  if~addMethod("Set","ssv","")
   -- tell about the properties that are available (implemented as ::attributes)
    
  if=node~addInterface("oorexx.dbus.ooTestService")
  if~addMethod("ReplyArray","as","as")
  if~addMethod("ReplyBoolean","b","b")
  if~addMethod("ReplyByte","y","y")
  if~addMethod("ReplyDouble","d","d")
  if~addMethod("ReplyInt16","n","n")
  if~addMethod("ReplyInt32","i","i")
  if~addMethod("ReplyInt64","x","x")
  if~addMethod("ReplyObjpath","o","o")
  if~addMethod("ReplySignature","g","g")
  if~addMethod("ReplyString","s","s")
  if~addMethod("ReplyUint16","q","q")
  if~addMethod("ReplyUint32","u","u")
  if~addMethod("ReplyUint64","t","t")
  if~addMethod("ReplyUnix_fd","h","h")
  if~addMethod("ReplyStruct","(si)","(si)")
  if~addMethod("ReplyArrayofByte","ay","ay")
  if~addMethod("ReplyArrayofDouble","ad","ad")
  if~addMethod("ReplyArrayofInt16","an","an")
  if~addMethod("ReplyArrayofInt32","ai","ai")
  if~addMethod("ReplyArrayofInt64","ax","ax")
  if~addMethod("ReplyArrayofobjpath","ao","ao")
  if~addMethod("ReplyArrayofSignature","ag","ag")
  if~addMethod("ReplyArrayofString","as","as")
  if~addMethod("ReplyArrayofUint16","aq","aq")
  if~addMethod("ReplyArrayofUint32","au","au")
  if~addMethod("ReplyArrayofUint64","at","at")
  if~addMethod("ReplyArrayofUnix_fd","ah","ah")
  if~addMethod("ReplyArrayofVariant","av","av")
  if~addMethod("ReplyArrayofStrucure","a(s)","a(s)")
  if~addMethod("ReplyArrayofBoolean","ab","ab")
  if~addMethod("ReplyDict","a{si}","a{si}")
  if~addMethod("ReplyDictofStrings","a{sv}","a{sv}")
  if~addMethod("TestReplySlotDir","s","a{sv}")
  if~addMethod("TestError",,'s')
  if~addMethod("TestError2",,'s')
  if~addMethod("TestError3",,'s')
  if~addMethod("TestError4",,'s')
  if~addMethod("TestError5",,'s')
  if~addMethod("Shutdown",,)
  if~addSignal("Ready",'sb')
  if~addSignal("Exit",)
  if~addProperty("Info", "s", "read")
  if~addProperty("SetInfo", "s", "write")
  if~addProperty("MethodName", "s", "readwrite")
  
 info="Property via ::attribute directive"
 introspectData=node~makeString       -- turn definitions into xml-string
 self~init:super(introspectData)     -- define interface data for clients 
  
::method SetInfo        -- the property set method                       
    expose info
    use arg info
  
::class TestService2 subclass DBusServiceObjectProxy
::attribute info
::method init                            -- constructor
 expose info 
 idata='<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN"'  -
        '"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">               '  -
        '<node>'-
'<interface name="org.freedesktop.DBus.Introspectable">'-
'<method name="Introspect">'-
'<arg name="data" direction="out" type="s"/>'-
'</method>'-
'</interface>'-
'<interface name="oorexx.dbus.ooTestService">'-
'<method name="ReplyArray">'-
'<arg name="input" direction="in" type="as"/>'-
'<arg name="output" direction="out" type="as"/>'-
'</method>'-
'<method name="ReplyBoolean">'-
'<arg name="inp" direction="in" type="b"/>'-
'<arg name="out" direction="out" type="b"/>'-
'</method>'-
'<method name="ReplyByte">'-
'<arg name="input" direction="in" type="y"/>'-
'<arg name="output" direction="out" type="y"/>'-
'</method>'-
'<method name="ReplyDouble">'-
'<arg name="input" direction="in" type="d"/>'-
'<arg name="output" direction="out" type="d"/>'-
'</method>'-
'<method name="ReplyInt16">'-
'<arg name="input" direction="in" type="n"/>'-
'<arg name="output" direction="out" type="n"/>'-
'</method>'-
'<method name="ReplyInt32">'-
'<arg name="input" direction="in" type="i"/>'-
'<arg name="output" direction="out" type="i"/>'-
'</method>'-
'<method name="ReplyInt64">'-
'<arg name="input" direction="in" type="x"/>'-
'<arg name="output" direction="out" type="x"/>'-
'</method>'-
'<method name="ReplyObjpath">'-
'<arg name="input" direction="in" type="o"/>'-
'<arg name="output" direction="out" type="o"/>'-
'</method>'-
'<method name="ReplySignature">'-
'<arg name="input" direction="in" type="g"/>'-
'<arg name="output" direction="out" type="g"/>'-
'</method>'-
'<method name="ReplyString">'-
'<arg name="input" direction="in" type="s"/>'-
'<arg name="output" direction="out" type="s"/>'-
'</method>'-
'<method name="ReplyUint16">'-
'<arg name="input" direction="in" type="q"/>'-
'<arg name="output" direction="out" type="q"/>'-
'</method>'-
'<method name="ReplyUint32">'-
'<arg name="input" direction="in" type="u"/>'-
'<arg name="output" direction="out" type="u"/>'-
'</method>'-
'<method name="ReplyUint64">'-
'<arg name="input" direction="in" type="t"/>'-
'<arg name="output" direction="out" type="t"/>'-
'</method>'-
'<method name="ReplyUnix_fd">'-
'<arg name="input" direction="in" type="h"/>'-
'<arg name="output" direction="out" type="h"/>'-
'</method>'-
'<method name="ReplyVariant">'-
'<arg name="input" direction="in" type="v"/>'-
'<arg name="output" direction="out" type="v"/>'-
'</method>'-
'<method name="ReplyStruct">'-
'<arg name="input" direction="in" type="(si)"/>'-
'<arg name="output" direction="out" type="(si)"/>'-
'</method>'-
'<method name="ReplyArrayofBoolean">'-
'<arg name="input" direction="in" type="ab"/>'-
'<arg name="output" direction="out" type="ab"/>'-
'</method>'-
'<method name="ReplyArrayofByte">'-
'<arg name="input" direction="in" type="ay"/>'-
'<arg name="output" direction="out" type="ay"/>'-
'</method>'-
'<method name="ReplyArrayofDouble">'-
'<arg name="input" direction="in" type="ad"/>'-
'<arg name="output" direction="out" type="ad"/>'-
'</method>'-
'<method name="ReplyArrayofInt16">'-
'<arg name="input" direction="in" type="an"/>'-
'<arg name="output" direction="out" type="an"/>'-
'</method>'-
'<method name="ReplyArrayofInt32">'-
'<arg name="input" direction="in" type="ai"/>'-
'<arg name="output" direction="out" type="ai"/>'-
'</method>'-
'<method name="ReplyArrayofInt64">'-
'<arg name="input" direction="in" type="ax"/>'-
'<arg name="output" direction="out" type="ax"/>'-
'</method>'-
'<method name="ReplyArrayofobjpath">'-
'<arg name="input" direction="in" type="ao"/>'-
'<arg name="output" direction="out" type="ao"/>'-
'</method>'-
'<method name="ReplyArrayofSignature">'-
'<arg name="input" direction="in" type="ag"/>'-
'<arg name="output" direction="out" type="ag"/>'-
'</method>'-
'<method name="ReplyArrayofString">'-
'<arg name="input" direction="in" type="as"/>'-
'<arg name="output" direction="out" type="as"/>'-
'</method>'-
'<method name="ReplyArrayofUint16">'-
'<arg name="input" direction="in" type="aq"/>'-
'<arg name="output" direction="out" type="aq"/>'-
'</method>'-
'<method name="ReplyArrayofUint32">'-
'<arg name="input" direction="in" type="au"/>'-
'<arg name="output" direction="out" type="au"/>'-
'</method>'-
'<method name="ReplyArrayofUint64">'-
'<arg name="input" direction="in" type="at"/>'-
'<arg name="output" direction="out" type="at"/>'-
'</method>'-
'<method name="ReplyArrayofUnix_fd">'-
'<arg name="input" direction="in" type="ah"/>'-
'<arg name="output" direction="out" type="ah"/>'-
'</method>'-
'<method name="ReplyArrayofVariant">'-
'<arg name="input" direction="in" type="av"/>'-
'<arg name="output" direction="out" type="av"/>'-
'</method>'-
'<method name="ReplyArrayofStrucure">'-
'<arg name="input" direction="in" type="a(s)"/>'-
'<arg name="output" direction="out" type="a(s)"/>'-
'</method>'-
'<method name="ReplyDict">'-
'<arg name="input" direction="in" type="a{si}"/>'-
'<arg name="output" direction="out" type="a{si}"/>'-
'</method>'-
'<method name="ReplyDictofStringVariant">'-
'<arg name="input" direction="in" type="a{sv}"/>'-
'<arg name="output" direction="out" type="a{sv}"/>'-
'</method>'-
'<method name="TestReplySlotDir">'-
'<arg name="input" direction="in" type="s"/>'-
'<arg name="output" direction="out" type="a{sv}"/>'-
'</method>'-
'<method name="TestError">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="TestError2">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="TestError3">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="TestError4">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="TestError5">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="Shutdown">'-
'</method>'-
'<signal name="Ready">'-
'<arg name="started" type="sb"/>'-
'</signal>'-
'<signal name="Exit">'-
'<arg name="result" type="s"/>'-
'</signal>'-
'<property name="Info" access="read" type="s"/>'-
'<property name="SetInfo" access="write" type="s"/>'-
'<property name="MethodName" access="readwrite" type="s"/>'-
'</interface>'-
'<interface name="org.freedesktop.DBus.Properties">' -
'<method name="Get">' -
'<arg name="interface_name" type="s" direction="in"/>' -
'<arg name="property_name" type="s" direction="in"/>' -
'<arg name="value" type="v" direction="out"/>'-
'</method>' -
'<method name="Set">' -
'<arg name="interface_name" type="s" direction="in"/>' -
'<arg name="property_name" type="s" direction="in"/>' -
'<arg name="value" type="v" direction="in"/>' -
'</method>' -
' </interface>' -
'</node>'

  self~init:super(idata)  -- let DBusServiceObject initialize
  info="property method return"

::method SetInfo       
  expose info
  use arg info
  
::method MethodName        /* the Rexx attribute is a 'readwrite' property!   */
  expose mname

  if arg()=2 then    /* one argument (plus slotDir gives 2 !), then behave as setter method  */
     use arg mname    /* fetch argument and save with attribute          */
  else               /* no argument, then behave as a getter method     */
     return mname     /* return current attribute                        */
  
-- third iteration with external xml file specified, all attributes are provided through methods
::class TestService3 subclass DBusServiceObjectProxy
::method init                       -- constructor
  expose Info
  idata='ooTestDBusService.xml'     -- filename that contains the introspect.xml data
  self~init:super(idata)  -- let DBusServiceObject initialize
  Info="TestserverXMLFile_Propertymethod"

::method MethodName
  expose methodcalled 
  if arg()=2 then          /* one argument (plus slotDir) --> behave as setter method  */
   use arg methodcalled    /* fetch argument and save with attribute          */
  else return methodcalled
  
::method Info       
  expose info
  return info
 
::method SetInfo    
  expose info
  use arg info
  
-- fourth iteration without DBusServiceObject and own Introspect method
::class TestService4 
::attribute info
::method Introspect        /* return the introspection data for this service object */
  return '<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN"'  -
        '"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">               '  -
        '<node>'-
'<interface name="org.freedesktop.DBus.Introspectable">'-
'<method name="Introspect">'-
'<arg name="data" direction="out" type="s"/>'-
'</method>'-
'</interface>'-
'<interface name="oorexx.dbus.ooTestService">'-
'<method name="ReplyArray">'-
'<arg name="input" direction="in" type="as"/>'-
'<arg name="output" direction="out" type="as"/>'-
'</method>'-
'<method name="ReplyBoolean">'-
'<arg name="inp" direction="in" type="b"/>'-
'<arg name="out" direction="out" type="b"/>'-
'</method>'-
'<method name="ReplyByte">'-
'<arg name="input" direction="in" type="y"/>'-
'<arg name="output" direction="out" type="y"/>'-
'</method>'-
'<method name="ReplyDouble">'-
'<arg name="input" direction="in" type="d"/>'-
'<arg name="output" direction="out" type="d"/>'-
'</method>'-
'<method name="ReplyInt16">'-
'<arg name="input" direction="in" type="n"/>'-
'<arg name="output" direction="out" type="n"/>'-
'</method>'-
'<method name="ReplyInt32">'-
'<arg name="input" direction="in" type="i"/>'-
'<arg name="output" direction="out" type="i"/>'-
'</method>'-
'<method name="ReplyInt64">'-
'<arg name="input" direction="in" type="x"/>'-
'<arg name="output" direction="out" type="x"/>'-
'</method>'-
'<method name="ReplyObjpath">'-
'<arg name="input" direction="in" type="o"/>'-
'<arg name="output" direction="out" type="o"/>'-
'</method>'-
'<method name="ReplySignature">'-
'<arg name="input" direction="in" type="g"/>'-
'<arg name="output" direction="out" type="g"/>'-
'</method>'-
'<method name="ReplyString">'-
'<arg name="input" direction="in" type="s"/>'-
'<arg name="output" direction="out" type="s"/>'-
'</method>'-
'<method name="ReplyUint16">'-
'<arg name="input" direction="in" type="q"/>'-
'<arg name="output" direction="out" type="q"/>'-
'</method>'-
'<method name="ReplyUint32">'-
'<arg name="input" direction="in" type="u"/>'-
'<arg name="output" direction="out" type="u"/>'-
'</method>'-
'<method name="ReplyUint64">'-
'<arg name="input" direction="in" type="t"/>'-
'<arg name="output" direction="out" type="t"/>'-
'</method>'-
'<method name="ReplyUnix_fd">'-
'<arg name="input" direction="in" type="h"/>'-
'<arg name="output" direction="out" type="h"/>'-
'</method>'-
'<method name="ReplyVariant">'-
'<arg name="input" direction="in" type="v"/>'-
'<arg name="output" direction="out" type="v"/>'-
'</method>'-
'<method name="ReplyStruct">'-
'<arg name="input" direction="in" type="(si)"/>'-
'<arg name="output" direction="out" type="(si)"/>'-
'</method>'-
'<method name="ReplyArrayofBoolean">'-
'<arg name="input" direction="in" type="ab"/>'-
'<arg name="output" direction="out" type="ab"/>'-
'</method>'-
'<method name="ReplyArrayofByte">'-
'<arg name="input" direction="in" type="ay"/>'-
'<arg name="output" direction="out" type="ay"/>'-
'</method>'-
'<method name="ReplyArrayofDouble">'-
'<arg name="input" direction="in" type="ad"/>'-
'<arg name="output" direction="out" type="ad"/>'-
'</method>'-
'<method name="ReplyArrayofInt16">'-
'<arg name="input" direction="in" type="an"/>'-
'<arg name="output" direction="out" type="an"/>'-
'</method>'-
'<method name="ReplyArrayofInt32">'-
'<arg name="input" direction="in" type="ai"/>'-
'<arg name="output" direction="out" type="ai"/>'-
'</method>'-
'<method name="ReplyArrayofInt64">'-
'<arg name="input" direction="in" type="ax"/>'-
'<arg name="output" direction="out" type="ax"/>'-
'</method>'-
'<method name="ReplyArrayofobjpath">'-
'<arg name="input" direction="in" type="ao"/>'-
'<arg name="output" direction="out" type="ao"/>'-
'</method>'-
'<method name="ReplyArrayofSignature">'-
'<arg name="input" direction="in" type="ag"/>'-
'<arg name="output" direction="out" type="ag"/>'-
'</method>'-
'<method name="ReplyArrayofString">'-
'<arg name="input" direction="in" type="as"/>'-
'<arg name="output" direction="out" type="as"/>'-
'</method>'-
'<method name="ReplyArrayofUint16">'-
'<arg name="input" direction="in" type="aq"/>'-
'<arg name="output" direction="out" type="aq"/>'-
'</method>'-
'<method name="ReplyArrayofUint32">'-
'<arg name="input" direction="in" type="au"/>'-
'<arg name="output" direction="out" type="au"/>'-
'</method>'-
'<method name="ReplyArrayofUint64">'-
'<arg name="input" direction="in" type="at"/>'-
'<arg name="output" direction="out" type="at"/>'-
'</method>'-
'<method name="ReplyArrayofUnix_fd">'-
'<arg name="input" direction="in" type="ah"/>'-
'<arg name="output" direction="out" type="ah"/>'-
'</method>'-
'<method name="ReplyArrayofVariant">'-
'<arg name="input" direction="in" type="av"/>'-
'<arg name="output" direction="out" type="av"/>'-
'</method>'-
'<method name="ReplyArrayofStrucure">'-
'<arg name="input" direction="in" type="a(s)"/>'-
'<arg name="output" direction="out" type="a(s)"/>'-
'</method>'-
'<method name="ReplyDict">'-
'<arg name="input" direction="in" type="a{si}"/>'-
'<arg name="output" direction="out" type="a{si}"/>'-
'</method>'-
'<method name="ReplyDictofStringVariant">'-
'<arg name="input" direction="in" type="a{sv}"/>'-
'<arg name="output" direction="out" type="a{sv}"/>'-
'</method>'-
'<method name="TestReplySlotDir">'-
'<arg name="input" direction="in" type="s"/>'-
'<arg name="output" direction="out" type="a{sv}"/>'-
'</method>'-
'<method name="TestError">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="TestError2">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="TestError3">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="TestError4">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="TestError5">'  -
'<arg name="result" direction="out" type="s"/>'  -
'</method>'  -
'<method name="Shutdown">'-
'</method>'-
'<signal name="Ready">'-
'<arg name="started" type="sb"/>'-
'</signal>'-
'<signal name="Exit">'-
'<arg name="result" type="s"/>'-
'</signal>'-
'<property name="Info" access="read" type="s"/>'-
'<property name="SetInfo" access="write" type="s"/>'-
'<property name="MethodName" access="readwrite" type="s"/>'-
'</interface>'-
'<interface name="org.freedesktop.DBus.Properties">' -
'<method name="Get">' -
'<arg name="interface_name" type="s" direction="in"/>' -
'<arg name="property_name" type="s" direction="in"/>' -
'<arg name="value" type="v" direction="out"/>'-
'</method>' -
'<method name="Set">' -
'<arg name="interface_name" type="s" direction="in"/>' -
'<arg name="property_name" type="s" direction="in"/>' -
'<arg name="value" type="v" direction="in"/>' -
'</method>' -
' </interface>' -
'</node>'

::method SetInfo     /* the property set method                         */
  expose info
  use arg info
  
::method MethodName        /* the Rexx attribute is a 'readwrite' property!   */
  expose mname
  if arg()=2 then    /* one argument (plus slotDir gives 2 !), then behave as setter method  */
   use arg mname    /* fetch argument and save with attribute          */
  else               /* no argument, then behave as a getter method     */
   return mname     /* return current attribute                        */
  
::method Get                   
  expose MethodName info 
  use arg caller, propertyname 
  if (propertyname='Info') then return info
  if (propertyname='MethodName') then return MethodName
  
::method Set     /* the property set method                         */
  expose MethodName info 
  use arg caller, propertyname, values
  if (propertyname='info' | propertyname='SetInfo') then self~SetInfo(values)
  if (propertyname='MethodName') then MethodName = values
  
::method ReplyArray
  use arg input
  return input
 
::method ReplyBoolean
  use arg input
  return input
  
::method ReplyByte
  use arg input
  return input
  
::method ReplyDouble
  use arg input
  return input 
  
::method ReplyInt16
  use arg input
  return input 
  
::method ReplyInt32
  use arg input
  return input
  
::method ReplyInt64
  use arg input
  return input
  
::method ReplyObjpath
  use arg input
  return input 
  
::method ReplySignature
  use arg input
  return input
  
::method ReplyString
  use arg input
  return input
  
::method ReplyUint16
  use arg input
  return input
  
::method ReplyUint32
  use arg input
  return input
  
::method ReplyUint64
  use arg input
  return input
 
::method ReplyUnix_fd
  use arg input
  return input
  
::method ReplyVariant
  use arg input
  return input
  
::method ReplyStruct
  use arg input
  return input
  
::method ReplyDict
  use arg input
  return input
  
::method ReplyDictofStrings      -- returns a directory ("map"/"dict") 
  use arg input
  return input

::method ReplyArrayofByte		-- return array
  use arg input
  return input
 
::method ReplyArrayofArray		-- return array
  use arg input
  return input
 
::method ReplyArrayofBoolean		-- return array
  use arg input
  return input
  
::method ReplyArrayofDouble		-- return array
  use arg input
  return input 
  
::method ReplyArrayofInt16		-- return array
  use arg input
  return input  
  
::method ReplyArrayofInt32		-- return array
  use arg input
  return input  
  
::method ReplyArrayofInt64		-- return array
  use arg input
  return input  
 
::method ReplyArrayofUint16		-- return array
  use arg input
  return input  
  
::method ReplyArrayofUint32		-- return array
  use arg input
  return input  
  
::method ReplyArrayofUint64		-- return array
  use arg input
  return input  
 
::method ReplyArrayofUnix_fd		-- return arrayofUnix_fd
  use arg input
  return input  

::method ReplyArrayofVariant		-- return arrayofVariant
  use arg input
  return input  
  
::method ReplyArrayofStrucure		-- return arrayofStrucure
  use arg input
  return input  

::method ReplyArrayofSignature		-- return array of signature
  use arg input
  return input 
  
::method ReplyArrayofString		-- return arrayofString
  use arg input
  return input  
  
::method ReplyArrayofobjpath		-- return Arrayofobjpath
  use arg input
  return input  
  
::method ReplyArrayofDict		-- return arrayofDict
  use arg input
  return input  
  
::method ReplyDictofStringVariant  
  use arg input
  return input
  
::method  TestReplySlotDir
  use arg call, slotDir
  return slotDir
  
::method TestError
  signal on syntax
  say 1/0    -- provoke a Rexx runtime error
  return
syntax:raise propagate

::method TestError2
  signal on syntax
  call raiseDBusError "org.freedesktop.DBus.Error.TestErrorFromRexxServiceObject", "raiseDBusError was invoked intentionally"
  return
syntax:raise propagate

::method TestError3   -- leave out error name
  signal on syntax
  call raiseDBusError , "raiseDBusError was invoked intentionally"
  return
syntax:raise propagate

::method TestError4    -- leave out error message
  signal on syntax
  call raiseDBusError "org.freedesktop.DBus.Error.TestErrorFromRexxServiceObject"
  return
syntax:raise propagate

::method TestError5    -- leave out error name and error message
  call raiseDBusError
  
::method Shutdown
  say "stopping server"
  dbus.shutdown=.true
   
     
  -- testserver uses an xml file to return introspection data   
::class TestServer subclass DBusServiceObjectProxy
::method init                       -- constructor
  expose Info
  idata='ooTestDBusService.xml'     -- filename that contains the introspect.xml data
  self~init:super(idata)  -- let DBusServiceObject initialize
  Info="TestserverXMLFile_Server"

::method MethodName
  expose methodcalled 
  if arg()=2 then          /* one argument (plus slotDir) --> behave as setter method  */
   use arg methodcalled    /* fetch argument and save with attribute          */
  else return methodcalled
  
::method Info       
  expose info
  return info
 
::method SetInfo    
  expose info
  use arg info     
     
-- class containing all replymethods for the servers
::class DBusServiceObjectProxy subclass DBusServiceObject 
::method ReplyArray
  use arg input
  return input
 
::method ReplyBoolean
  use arg input
   return input
  
::method ReplyByte
  use arg input
  return input
  
::method ReplyDouble
  use arg input
  return input 
  
::method ReplyInt16
  use arg input
  return input 
  
::method ReplyInt32
  use arg input
  return input
  
::method ReplyInt64
  use arg input
  return input
  
::method ReplyObjpath
  use arg input
  return input 
  
::method ReplySignature
  use arg input
  return input
  
::method ReplyString
  use arg input
  return input
  
::method ReplyUint16
  use arg input
  return input
  
::method ReplyUint32
  use arg input
  return input
  
::method ReplyUint64
  use arg input
  return input
 
::method ReplyUnix_fd
  use arg input
  return input
  
::method ReplyVariant
  use arg input
  return input
  
::method ReplyStruct
  use arg input
  return input
  
::method ReplyDict
  use arg input
  return input
  
::method ReplyDictofStrings      -- returns a directory ("map"/"dict") 
  use arg input
  return input

::method ReplyArrayofByte		-- return array
  use arg input
  return input
 
::method ReplyArrayofArray		-- return array
  use arg input
  return input
 
::method ReplyArrayofBoolean		-- return array
  use arg input
  return input
  
::method ReplyArrayofDouble		-- return array
  use arg input
  return input 
  
::method ReplyArrayofInt16		-- return array
  use arg input
  return input  
  
::method ReplyArrayofInt32		-- return array
  use arg input
  return input  
  
::method ReplyArrayofInt64		-- return array
  use arg input
  return input  
 
::method ReplyArrayofUint16		-- return array
  use arg input
  return input  
  
::method ReplyArrayofUint32		-- return array
  use arg input
  return input  
  
::method ReplyArrayofUint64		-- return array
  use arg input
  return input  
 
::method ReplyArrayofUnix_fd		-- return arrayofUnix_fd
  use arg input
  return input  

::method ReplyArrayofVariant		-- return arrayofVariant
  use arg input
  return input  
  
::method ReplyArrayofStrucure		-- return arrayofStrucure
  use arg input
  return input  

::method ReplyArrayofSignature		-- return array of signature
  use arg input
  return input 
  
::method ReplyArrayofString		-- return arrayofString
  use arg input
  return input  
  
::method ReplyArrayofobjpath		-- return Arrayofobjpath
  use arg input
  return input  
  
::method ReplyArrayofDict		-- return arrayofDict
  use arg input
  return input  
  
::method ReplyDictofStringVariant  
  use arg input
  return input

::method TestReplySlotDir
  use arg call, slotDir
  return slotDir  
  
::method TestError
  signal on syntax
  say 1/0    -- provoke a Rexx runtime error
  return
syntax:raise propagate

::method TestError2
  signal on syntax
  call raiseDBusError "org.freedesktop.DBus.Error.TestErrorFromRexxServiceObject", "raiseDBusError was invoked intentionally"
  return
syntax:raise propagate

::method TestError3   -- leave out error name
  signal on syntax
  call raiseDBusError , "raiseDBusError was invoked intentionally"
  return
syntax:raise propagate

::method TestError4    -- leave out error message
  signal on syntax
  call raiseDBusError "org.freedesktop.DBus.Error.TestErrorFromRexxServiceObject"
  return
syntax:raise propagate

::method TestError5    -- leave out error name and error message
  call raiseDBusError

::method Shutdown
  say "stopping server"
  dbus.shutdown = .true
    
 -- this class provides an mechanism that inspects the signature and returns the value 
::class TestService5 subclass DBusServiceObject
::method init                            -- constructor
 expose info 
 idata='<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN"'  -
        '"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">' -
        '<node>'-
'<interface name="org.freedesktop.DBus.Introspectable">'-
'<method name="Introspect">'-
'<arg name="data" direction="out" type="s"/>'-
'</method>'-
'</interface>'-
'<interface name="oorexx.dbus.ooTestService">'-
'<method name="Shutdown">'-
'</method>'-
' </interface>' -
'</node>'

  self~init:super(idata)  -- let DBusServiceObject initialize
  info="property method return"

::method SetInfo       
  expose info
  use arg info
  
::method MethodName        /* the Rexx attribute is a 'readwrite' property!   */
  expose mname

  if arg()=2 then    /* one argument (plus slotDir gives 2 !), then behave as setter method  */
   use arg mname    /* fetch argument and save with attribute          */
  else               /* no argument, then behave as a getter method     */
   return mname     /* return current attribute                        */
    
::method Shutdown
  say "stopping server"
  .dbus~session~close
  exit -1

::method unknown              -- this method will service all invocations
  use arg methName, methArgs
  slotDir=methArgs[methArgs~items]
 -- process arguments, create reply (return) value, by using the same 
 -- arguments (without slotDir argument) with the same signature
 
  resArr=.array~of("useThisSignature="slotDir~signature)
  resArr~append(methArgs~section(1,methArgs~size-1)) -- leave out the supplemental slotDir argument
  return resArr
  