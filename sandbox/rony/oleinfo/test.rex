strIE="InternetExplorer.Application"
ie=.oleObject~new(strIE)   -- create an instance of IE, returns an OLEObject
ie~visible=.true           -- make IE window visible

ie~navigate("https://www.RexxLA.org")
do while ie~busy           -- wait until page got fully loaded
   call sysSleep 0.01
end
doc=ie~document            -- get document object from IE

-- note: OLE objects do not always publish what they offer!
   -- create documentation for document object, full documentation, show in browser
call createOleInfo doc, "ie~document", .false, .true
   -- document Internetexplorer, full documentation (with constants), show in browser
call createOleInfo ie, strIE, .false, .true
   -- document Internetexplorer, reference documentation, show in browser
call createOleInfo ie, strIE, .true, .true

ie~quit                    -- quit (close) this IE instance

