#!/usr/bin/rexx

say 'Calling SysUname'
say 'System name =' SysUname('S')
say 'Node name   =' SysUname('N')
say 'Release     =' SysUname('R')
say 'Version     =' SysUname('V')
say 'Machine     =' SysUname('M')
say '            =' SysUname() '(should be the same as the system name)'
say

say 'Calling SysGetpid'
pid = SysGetpid()
say 'pid =' pid
say

say 'Calling SysGetppid'
ppid = SysGetppid()
say 'ppid =' ppid
say

say 'Calling SysGettid'
tid = SysGettid()
say 'tid =' tid
say

say 'Calling SysGetsid'
say 'sid =' SysGetsid(pid)
say

say 'Calling SysGetuid'
uid = SysGetuid()
say 'uid =' uid
say

say 'Calling SysGeteuid'
euid = SysGeteuid()
say 'euid =' euid
say

say 'Calling SysGetgid'
gid = SysGetgid()
say 'gid =' gid
say

say 'Calling SysGetegid'
egid = SysGetegid()
say 'egid =' egid
say

say 'Calling SysGetpwuid ('uid')'
say 'Name      =' SysGetpwuid(uid, 'N')
say 'uid       =' SysGetpwuid(uid, 'U')
say 'gid       =' SysGetpwuid(uid, 'G')
say 'Real name =' SysGetpwuid(uid, 'R')
say 'Home dir  =' SysGetpwuid(uid, 'D')
say 'Shell     =' SysGetpwuid(uid, 'S')
say 'Password  = "'SysGetpwuid(uid, 'P')'" (should be the null string)'
say

say 'Calling SysGetpwnam ('SysGetpwuid(uid, 'N')')'
say 'Name      =' SysGetpwnam(SysGetpwuid(uid, 'N'), 'N')
say 'uid       =' SysGetpwnam(SysGetpwuid(uid, 'N'), 'U')
say 'gid       =' SysGetpwnam(SysGetpwuid(uid, 'N'), 'G')
say 'Real name =' SysGetpwnam(SysGetpwuid(uid, 'N'), 'R')
say 'Home dir  =' SysGetpwnam(SysGetpwuid(uid, 'N'), 'D')
say 'Shell     =' SysGetpwnam(SysGetpwuid(uid, 'N'), 'S')
say 'Password  = "'SysGetpwnam(SysGetpwuid(uid, 'N'), 'P')'" (should be the null string)'
say

say 'Calling SysGetgrgid ('gid')'
say 'Name      =' SysGetgrgid(gid, 'N')
say 'gid       =' SysGetgrgid(gid, 'G')
say 'Password  = "'SysGetgrgid(gid, 'P')'" (should be the null string)'
members = ''
mbrs = SysGetgrgid(gid, 'M')
do mbr over mbrs
   if members = '' then members = mbr
   else members = members',' mbr
   end
say 'members   =' members
say

say 'Calling SysGetgrnam ('SysGetgrgid(gid, 'N')')'
say 'Name      =' SysGetgrnam(SysGetgrgid(gid, 'N'), 'N')
say 'gid       =' SysGetgrnam(SysGetgrgid(gid, 'N'), 'G')
say 'Password  = "'SysGetgrnam(SysGetgrgid(gid, 'N'), 'P')'" (should be the null string)'
members = ''
mbrs = SysGetgrnam(SysGetgrgid(gid, 'N'), 'M')
do mbr over mbrs
   if members = '' then members = mbr
   else members = members',' mbr
   end
say 'members   =' members
say

say 'Calling SysGetgrnam (users)'
say 'Name      =' SysGetgrnam('users', 'N')
say 'gid       =' SysGetgrnam('users', 'G')
say 'Password  = "'SysGetgrnam('users', 'P')'" (should be the null string)'
members = ''
mbrs = SysGetgrnam('users', 'M')
do mbr over mbrs
   if members = '' then members = mbr
   else members = members',' mbr
   end
say 'members   =' members
say

parse source . . file
say 'Calling SysStat ('file')'
say 'Device =' SysStat(file, 'D')
say 'Inode =' SysStat(file, 'I')
say 'Permissions =' SysStat(file, 'P')
say 'Number of links =' SysStat(file, 'N')
statuid = SysStat(file, 'U')
say 'uid =' statuid '('SysGetpwuid(statuid, 'N')')'
statgid = SysStat(file, 'G')
say 'gid =' statgid '('SysGetgrgid(statgid, 'N')')'
say 'rdev =' SysStat(file, 'R')
say 'size =' SysStat(file, 'S')
say 'last access =' SysStat(file, 'A')
say 'last modification =' SysStat(file, 'M')
say 'last change =' SysStat(file, 'C')
say

say 'Calling SysAccess ('file')'
if SysAccess(file, 0) = 0 then say 'File exists.'
else say 'File does not exist.'
if SysAccess(file, 1) = 0 then say 'We have execute permission.'
else say 'We do not have execute permission.'
if SysAccess(file, 2) = 0 then say 'We have write permission.'
else say 'We do not have write permission.'
if SysAccess(file, 4) = 0 then say 'We have read permission.'
else say 'We do not have read permission.'

say 'Calling SysWordexp (~/*)'
arr = SysWordexp('~/*')
do ent over arr
   say ent
   end

parse source . . file
say 'Calling SysSetxattr {"'file'", "user.mime_type", "rexx/exec")'
say 'retc =' SysSetxattr(file, 'user.mime_type', 'rexx/exec')

say 'Calling SysGetxattr {"'file'", "user.mime_type")'
say 'user.mime_type =' SysGetxattr(file, 'user.mime_type')

say 'Calling SysListxattr {"'file'")'
arr = SysListxattr(file)
do name over arr
   say 'name =' name
   end

say 'Calling SysRemovexattr {"'file'", "user.mime_type")'
say 'retc =' SysRemovexattr(file, 'user.mime_type')

say 'Calling SysGetsizeofptr'
say 'Addressing pointer size =' SysGetsizeofptr()

say 'Calling SysGethostname'
say 'Nostname =' SysGethostname()

say 'Calling SysCrypt'
say SysCrypt("EncryptionString", "3A")

return

::requires 'rxunixsys' library

