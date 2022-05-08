/*
   name:    listProgIds.rex
   version: 1.1
   date:    2022-04-28
   changed: 2022-05-08, ---rgf: - indicate bitness of ProgID

   author:  Rony G. Flatscher
   purpose: list all ProgIDs, optionally with their CLSID

   usage:   listProgIds.rex
            listProgIds.rex needle
            listProgIds.rex 1
            listProgIds.rex 1 needle
            ... needle:   optional: a string, list ProgIdDs that contain this string (caseless comparison)
            ... 1         optional: supply CLSID

   license: CPL 1.0, Apache License 2.0
*/
parse arg indicator needle
bCLSID=(indicator=1)
if \bCLSID then needle=indicator

bTiming=.false -- .true -- determines whether timings are shown
time0=.DateTime~new

.clsid~analyze_and_build	   -- let the class method query and analyze the registry,
all_progid= .clsid~all_progid	-- get ProgID directory

time1=.DateTime~new
if bTiming then say "analyzing registry time spent:" pp(time1-time0)
   -- sort all ProgID indixes caselessly
allProgIds   = all_progid~allIndexes~sortWith(.CaselessComparator~new)

time2=.DateTime~new
items=allProgIds~items
if bTiming then
do
   say "sorting" items "took:" pp(time2-time1)
   say
end
prefix=.rexxinfo~architecture"-bit #"
len=items~length
do counter i idx over allProgIds    -- idx is the ProgID
   if needle<>"", idx~caselessPos(needle)=0 then iterate

   o=all_progid~at(idx)    -- get ooRexx classid object
   hint=checkProgId(idx)   -- supply hint whether ProgID adhers to MS specs
   if bCLSID then
      say prefix i~right(len)"/"items":" "ProgId:" pp(idx)~left(70) hint~left(5) "CLSID:" o~clsid
   else
      say prefix i~right(len)"/"items":" "ProgId:" pp(idx)~left(70) hint~left(5)
end
time3=.DateTime~new
if bTiming then
do
   say "output took:                 " pp(time3-time2)
   say "total time:                  " pp(time3-time0)
end



::requires "reg_classids4ole.cls" -- get support for analyzing the registry

::routine pp
  return "["arg(1)"]"

::routine checkProgId         -- checks whether ProgID adheres to Microsoft's specifications
   use arg progid
   -- check validity of ProgId according to: <https://docs.microsoft.com/en-us/windows/win32/com/-progid--key>:
   --     length<=39, no punctuation (except dots), not start with a digit, ..
   -- also discussion at: <https://stackoverflow.com/questions/1754429/what-happens-if-i-violate-the-requirements-imposed-on-progids>
   hint=""
      -- too long or empty string?
   if progid~length >39 then hint="l"
   else if progid="" then hint="l"
      -- illegal character?
   if verify(progid,"ABCDEFGHIJKLMNOPQRSTUVWXYZ.abcdefghijklmnopqrstuvwxyz0123456789")>0 then hint||="c"
      -- must not start with a digit
   if datatype(progid[1],"W") then hint="d"
   if hint<>"" then return "*"hint"*"

   return hint

