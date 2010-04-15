/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2009-2010 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/* Authors;                                                                   */
/*       W. David Ashley <dashley@us.ibm.com>                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "nixclib.h"


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Function:      SysSignal
 *
 * Change a signal behavior.
 *
 * @param signum  The signal number.
 *
 * @param sigact  The new signal action.
 *
 * @return        Function return code
 */
RexxRoutine2(int,
             SysSignal,
             int, signum,
             CSTRING, sigact)
{

    if (strlen(sigact) == 0) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysSignal"));
        return 0;
    }
    // According to POSIX we should only allow the following values for this function
    else if (*sigact == 'D' || *sigact == 'd') {
        signal(signum, SIG_DFL);
        return 0;
    }
    else if (*sigact == 'I' || *sigact == 'i') {
        signal(signum, SIG_IGN);
        return 0;
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysSignal"));
    return 0;
}

/**
 * Method:        SysUname
 *
 * Return the uname information.
 *
 * @param ichar   The option string.
 *
 * @return        Option information
 */
RexxRoutine1(RexxObjectPtr,
             SysUname,
             OPTIONAL_CSTRING, ichar)
{
    struct utsname name;

    uname(&name);
    if (ichar == NULL || *ichar == 'S' || *ichar == 's') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(name.sysname);
    }
    else if (*ichar == 'N' || *ichar == 'n') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(name.nodename);
    }
    else if (*ichar == 'R' || *ichar == 'r') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(name.release);
    }
    else if (*ichar == 'V' || *ichar == 'v') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(name.version);
    }
    else if (*ichar == 'M' || *ichar == 'm') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(name.machine);
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysUname"));
    return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
}

/**
 * Method:        SysSetsid
 *
 * Set the Session id.
 *
 * @return        Process id
 */
RexxRoutine0(RexxObjectPtr,          
             SysSetsid)
{
    pid_t pid;

    pid = setsid();
    return context->WholeNumberToObject((wholenumber_t)pid);
}

/**
 * Method:        SysGetsid
 *
 * Get the Session id.
 *
 * @param pid     The pid.
 *
 * @return        Session id
 */
RexxRoutine1(RexxObjectPtr,          
             SysGetsid,
             int, pid)
{
    pid = getsid((pid_t)pid);
    return context->WholeNumberToObject((wholenumber_t)pid);
}

/**
 * Method:        SysSetuid
 *
 * Set the userid.
 *
 * @param uid     The userid.
 *
 * @return        0 or -1
 */
RexxRoutine1(RexxObjectPtr,          
             SysSetuid,
             int, uid)
{

    return context->WholeNumberToObject((wholenumber_t)setuid((uid_t)uid));
}

/**
 * Method:        SysGetuid
 *
 * Get the userid.
 *
 * @return        uid
 */
RexxRoutine0(RexxObjectPtr,          
             SysGetuid)
{

    return context->WholeNumberToObject((wholenumber_t)getuid());
}

/**
 * Method:        SysSeteuid
 *
 * Set the effective userid.
 *
 * @param uid     The userid.
 *
 * @return        0 or -1
 */
RexxRoutine1(RexxObjectPtr,          
             SysSeteuid,
             int, uid)
{

    return context->WholeNumberToObject((wholenumber_t)seteuid((uid_t)uid));
}

/**
 * Method:        SysGeteuid
 *
 * Get the effective userid.
 *
 * @return        uid
 */
RexxRoutine0(RexxObjectPtr,          
             SysGeteuid)
{

    return context->WholeNumberToObject((wholenumber_t)geteuid());
}

/**
 * Method:        SysSetgid
 *
 * Set the groupid.
 *
 * @param gid     The groupid.
 *
 * @return        0 or -1
 */
RexxRoutine1(RexxObjectPtr,          
             SysSetgid,
             int, gid)
{

    return context->WholeNumberToObject((wholenumber_t)setgid((gid_t)gid));
}

/**
 * Method:        SysGetgid
 *
 * Get the groupid.
 *
 * @return        0 or -1
 */
RexxRoutine0(RexxObjectPtr,          
             SysGetgid)
{

    return context->WholeNumberToObject((wholenumber_t)getgid());
}

/**
 * Method:        SysSetegid
 *
 * Set the effective groupid.
 *
 * @param gid     The groupid.
 *
 * @return        0 or -1
 */
RexxRoutine1(RexxObjectPtr,          
             SysSetegid,
             int, gid)
{

    return context->WholeNumberToObject((wholenumber_t)setegid((gid_t)gid));
}

/**
 * Method:        SysGetegid
 *
 * Get the effective groupid.
 *
 * @return        0 or -1
 */
RexxRoutine0(RexxObjectPtr,          
             SysGetegid)
{

    return context->WholeNumberToObject((wholenumber_t)getegid());
}

/**
 * Method:        SysSetpgrp
 *
 * Set the pgrp.
 *
 * @return        0 or -1
 */
RexxRoutine0(RexxObjectPtr,          
             SysSetpgrp)
{

    return context->WholeNumberToObject((wholenumber_t)setpgrp());
}

/**
 * Method:        SysGetpgrp
 *
 * Get the pgrp.
 *
 * @return        0 or -1
 */
RexxRoutine0(RexxObjectPtr,          
             SysGetpgrp)
{

    return context->WholeNumberToObject((wholenumber_t)getpgrp());
}

/**
 * Method:        SysSetpgid
 *
 * Set the pgid.
 *
 * @param pid1    The pid to set.
 *
 * @param pid2    The new pid.
 *
 * @return        0 or -1
 */
RexxRoutine2(RexxObjectPtr,          
             SysSetpgid,
             int, pid1,
             int, pid2)
{

    return context->WholeNumberToObject((wholenumber_t)setpgid((pid_t)pid1, (pid_t)pid2));
}

/**
 * Method:        SysGetpgid
 *
 * Get the pgid.
 *
 * @param pid1    The pid to query.
 *
 * @return        The pid.
 */
RexxRoutine1(RexxObjectPtr,          
             SysGetpgid,
             int, pid)
{

    return context->WholeNumberToObject((wholenumber_t)getpgid((pid_t)pid));
}

/**
 * Method:        SysGetpid
 *
 * Get the pid.
 *
 * @return        pid
 */
RexxRoutine0(RexxObjectPtr,          
             SysGetpid)
{
    pid_t pid = getpid();
    return context->WholeNumberToObject((wholenumber_t)pid);
}

/**
 * Method:        SysGetppid
 *
 * Get the ppid.
 *
 * @return        pid
 */
RexxRoutine0(RexxObjectPtr,          
             SysGetppid)
{
    pid_t pid = getppid();
    return context->WholeNumberToObject((wholenumber_t)pid);
}

/**
 * Method:        SysGettid
 *
 * Get the tid.
 *
 * @return        tid
 */
RexxRoutine0(RexxObjectPtr,          
             SysGettid)
{
    pid_t tid = syscall(SYS_gettid);
    return context->WholeNumberToObject((wholenumber_t)tid);
}

/**
 * Method:        SysSymlink
 *
 * Create a symbolic link.
 *
 * @param path1   The source path.
 *
 * @param path2   The target path.
 *
 * @return        int error code
 */
RexxRoutine2(int,          
             SysSymlink,
             CSTRING, path1,
             CSTRING, path2)
{
    return symlink(path1, path2);
}

/**
 * Method:        SysLink
 *
 * Create a hard link.
 *
 * @param path1   The source path.
 *
 * @param path2   The target path.
 *
 * @return        int error code
 */
RexxRoutine2(int,          
             SysLink,
             CSTRING, path1,
             CSTRING, path2)
{
    return link(path1, path2);
}

/**
 * Method:        SysUnlink
 *
 * Remove a hard or soft link.
 *
 * @param path1   The source path.
 *
 * @return        int error code
 */
RexxRoutine1(int,          
             SysUnlink,
             CSTRING, path1)
{
    return unlink(path1);
}

/**
 * Method:        SysChown
 *
 * Change the owner and group of a file.
 *
 * @param path1   The file path.
 *
 * @param uid     The new userid. 
 *
 * @param gid     The new groupid.
 *
 * @return        int error code
 */
RexxRoutine3(int,          
             SysChown,
             CSTRING, path1,
             int, uid,
             int, gid)
{
    return chown(path1, (uid_t)uid, (gid_t)gid);
}

/**
 * Method:        SysChroot
 *
 * Changes the root directory of the calling process.
 *
 * @param path1   The new root path.
 *
 * @return        int error code
 */
RexxRoutine1(int,          
             SysChroot,
             CSTRING, path1)
{
    return chroot(path1);
}

/**
 * Method:        SysUmask
 *
 * Sets the calling process umask.
 *
 * @param nmask   The new umask.
 *
 * @return        the old umansk value
 */
RexxRoutine1(int,          
             SysUmask,
             int, nmask)
{
    return umask((mode_t)nmask);
}

/**
 * Method:        SysGetpwnam
 *
 * Return user name information from the passwd file.
 *
 * @param user    The user name.
 *
 * @param ichar   The option string.
 *
 * @return        Option information
 */
RexxRoutine2(RexxObjectPtr,
             SysGetpwnam,
             CSTRING, user,
             CSTRING, ichar)
{
    if (strlen(user) == 0 || strlen(ichar) != 1) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetpwnam"));
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    struct passwd *pw = getpwnam(user);
    if (pw == NULL) {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    else if (*ichar == 'N' || *ichar == 'n') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(pw->pw_name);
    }
    else if (*ichar == 'U' || *ichar == 'u') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)pw->pw_uid);
    }
    else if (*ichar == 'G' || *ichar == 'g') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)pw->pw_gid);
    }
    else if (*ichar == 'R' || *ichar == 'r') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(pw->pw_gecos);
    }
    else if (*ichar == 'D' || *ichar == 'd') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(pw->pw_dir);
    }
    else if (*ichar == 'S' || *ichar == 's') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(pw->pw_shell);
    }
    else if (*ichar == 'P' || *ichar == 'p') {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetpwnam"));
    return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
}

/**
 * Method:        SysGetpwuid
 *
 * Return user name information from the passwd file.
 *
 * @param user    The uid.
 *
 * @param ichar   The option string.
 *
 * @return        Option information
 */
RexxRoutine2(RexxObjectPtr,
             SysGetpwuid,
             int, uid,
             CSTRING, ichar)
{
    if (strlen(ichar) != 1) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetpwuid"));
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    struct passwd *pw = getpwuid((uid_t)uid);
    if (pw == NULL) {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    else if (*ichar == 'N' || *ichar == 'n') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(pw->pw_name);
    }
    else if (*ichar == 'U' || *ichar == 'u') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)pw->pw_uid);
    }
    else if (*ichar == 'G' || *ichar == 'g') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)pw->pw_gid);
    }
    else if (*ichar == 'R' || *ichar == 'r') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(pw->pw_gecos);
    }
    else if (*ichar == 'D' || *ichar == 'd') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(pw->pw_dir);
    }
    else if (*ichar == 'S' || *ichar == 's') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(pw->pw_shell);
    }
    else if (*ichar == 'P' || *ichar == 'p') {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetpwnam"));
    return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
}

/**
 * Method:        SysGetgrnam
 *
 * Return group name information from the group file.
 *
 * @param user    The group name.
 *
 * @param ichar   The option string.
 *
 * @return        Option information
 */
RexxRoutine2(RexxObjectPtr,
             SysGetgrnam,
             CSTRING, grpname,
             CSTRING, ichar)
{
    if (strlen(grpname) == 0 || strlen(ichar) != 1) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetgrnam"));
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    struct group *gr = getgrnam(grpname);
    if (*ichar == 'N' || *ichar == 'n') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(gr->gr_name);
    }
    else if (*ichar == 'P' || *ichar == 'p') {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    else if (*ichar == 'G' || *ichar == 'g') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)gr->gr_gid);
    }
    else if (*ichar == 'M' || *ichar == 'm') {
        RexxArrayObject arr = context->NewArray(1);
        char **members = gr->gr_mem;
        while (*members != NULL) {
           context->ArrayAppendString(arr, *members, strlen(*members));
           members++;
        }
        return (RexxObjectPtr)arr;
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetgrnam"));
    return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
}

/**
 * Method:        SysGetgrgid
 *
 * Return group name information from the group file.
 *
 * @param user    The gid.
 *
 * @param ichar   The option string.
 *
 * @return        Option information
 */
RexxRoutine2(RexxObjectPtr,
             SysGetgrgid,
             int, gid,
             CSTRING, ichar)
{
    if (strlen(ichar) != 1) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetgrgid"));
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    struct group *gr = getgrgid(gid);
    if (*ichar == 'N' || *ichar == 'n') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(gr->gr_name);
    }
    else if (*ichar == 'P' || *ichar == 'p') {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    else if (*ichar == 'G' || *ichar == 'g') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)gr->gr_gid);
    }
    else if (*ichar == 'M' || *ichar == 'm') {
        RexxArrayObject arr = context->NewArray(1);
        char **members = gr->gr_mem;
        while (*members != NULL) {
           context->ArrayAppendString(arr, *members, strlen(*members));
           members++;
        }
        return (RexxObjectPtr)arr;
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetgrnam"));
    return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
}

/**
 * Method:        SysStat
 *
 * Return information from the stat API.
 *
 * @param user    The file name.
 *
 * @param ichar   The option string.
 *
 * @return        Option information
 */
RexxRoutine2(RexxObjectPtr,
             SysStat,
             CSTRING, fname,
             CSTRING, ichar)
{
    struct tm *ftime;
    struct stat64 mystat;
    char buf[32];  // used for both the file times and the permissions

    if (strlen(fname) == 0 || strlen(ichar) != 1) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysStat"));
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    int retc = stat64(fname, &mystat);
    if (retc != 0) {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    else if (*ichar == 'D' || *ichar == 'd') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)mystat.st_dev);
    }
    else if (*ichar == 'I' || *ichar == 'i') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)mystat.st_ino);
    }
    else if (*ichar == 'P' || *ichar == 'p') {
        // 1- file type
        if (S_IFDIR & mystat.st_mode == S_IFDIR) {
            strcpy(buf, "d");
        }
        else if (S_IFCHR & mystat.st_mode == S_IFCHR) {
            strcpy(buf, "c");
        }
        else if (S_IFBLK & mystat.st_mode == S_IFBLK) {
            strcpy(buf, "b");
        }
        else if (S_IFIFO & mystat.st_mode == S_IFIFO) {
            strcpy(buf, "p");
        }
        else if (S_IFREG & mystat.st_mode == S_IFREG) {
            strcpy(buf, "-");
        }
        else if (S_IFLNK & mystat.st_mode == S_IFLNK) {
            strcpy(buf, "l");
        }
        else if (S_IFSOCK & mystat.st_mode == S_IFSOCK) {
            strcpy(buf, "s");
        }
        else strcpy(buf, "-");  
        // 2 - user read
        if (S_IRUSR & mystat.st_mode) {
            strcat(buf, "r");
        }
        else strcat(buf, "-");
        // 3 - user write
        if (S_IWUSR & mystat.st_mode) {
            strcat(buf, "w");
        }
        else strcat(buf, "-");
        // 4 - user execute
        if ((S_IXUSR & mystat.st_mode) && (S_ISUID & mystat.st_mode) &&
            (S_ISGID & mystat.st_mode)) {
            strcat(buf, "s");
        }
        else if (!(S_IXUSR & mystat.st_mode) && (S_ISUID & mystat.st_mode) &&
                 (S_ISGID & mystat.st_mode)) {
            strcat(buf, "S");
        }
        else if ((S_IXUSR & mystat.st_mode) && (S_ISVTX & mystat.st_mode)) {
            strcat(buf, "t");
        }
        else if (!(S_IXUSR & mystat.st_mode) && (S_ISVTX & mystat.st_mode)) {
            strcat(buf, "T");
        }
        else if (S_IXUSR & mystat.st_mode) {
            strcat(buf, "x");
        }
        else strcat(buf, "-");
        // 5 -group read
        if (S_IRGRP & mystat.st_mode) {
            strcat(buf, "r");
        }
        else strcat(buf, "-");
        // 6 - group write
        if (S_IWGRP & mystat.st_mode) {
            strcat(buf, "w");
        }
        else strcat(buf, "-");
        // 7 - group execute
        if ((S_IXGRP & mystat.st_mode) && (S_ISUID & mystat.st_mode) &&
            (S_ISGID & mystat.st_mode)) {
            strcat(buf, "s");
        }
        else if (!(S_IXGRP & mystat.st_mode) && (S_ISUID & mystat.st_mode) &&
                 (S_ISGID & mystat.st_mode)) {
            strcat(buf, "S");
        }
        else if ((S_IXGRP & mystat.st_mode) && (S_ISVTX & mystat.st_mode)) {
            strcat(buf, "t");
        }
        else if (!(S_IXGRP & mystat.st_mode) && (S_ISVTX & mystat.st_mode)) {
            strcat(buf, "T");
        }
        else if (S_IXGRP & mystat.st_mode) {
            strcat(buf, "x");
        }
        else strcat(buf, "-");
        // 8 - other read
        if (S_IROTH & mystat.st_mode) {
            strcat(buf, "r");
        }
        else strcat(buf, "-");
        // 9 - other write
        if (S_IWOTH & mystat.st_mode) {
            strcat(buf, "w");
        }
        else strcat(buf, "-");
        // 10 - other execute
        if ((S_IXOTH & mystat.st_mode) && (S_ISUID & mystat.st_mode) &&
            (S_ISGID & mystat.st_mode)) {
            strcat(buf, "s");
        }
        else if (!(S_IXOTH & mystat.st_mode) && (S_ISUID & mystat.st_mode) &&
                 (S_ISGID & mystat.st_mode)) {
            strcat(buf, "S");
        }
        else if ((S_IXOTH & mystat.st_mode) && (S_ISVTX & mystat.st_mode)) {
            strcat(buf, "t");
        }
        else if (!(S_IXOTH & mystat.st_mode) && (S_ISVTX & mystat.st_mode)) {
            strcat(buf, "T");
        }
        else if (S_IXOTH & mystat.st_mode) {
            strcat(buf, "x");
        }
        else strcat(buf, "-");
        return (RexxObjectPtr)context->NewStringFromAsciiz(buf);
    }
    else if (*ichar == 'N' || *ichar == 'n') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)mystat.st_nlink);
    }
    else if (*ichar == 'U' || *ichar == 'u') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)mystat.st_uid);
    }
    else if (*ichar == 'G' || *ichar == 'g') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)mystat.st_gid);
    }
    else if (*ichar == 'R' || *ichar == 'r') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)mystat.st_rdev);
    }
    else if (*ichar == 'S' || *ichar == 's') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)mystat.st_size);
    }
    else if (*ichar == 'A' || *ichar == 'a') {
        ftime = localtime(&mystat.st_atime);
        strftime(buf, sizeof(buf), "%F %T", ftime);
        return (RexxObjectPtr)context->NewStringFromAsciiz(buf);
    }
    else if (*ichar == 'M' || *ichar == 'm') {
        ftime = localtime(&mystat.st_mtime);
        strftime(buf, sizeof(buf), "%F %T", ftime);
        return (RexxObjectPtr)context->NewStringFromAsciiz(buf);
    }
    else if (*ichar == 'C' || *ichar == 'c') {
        ftime = localtime(&mystat.st_ctime);
        strftime(buf, sizeof(buf), "%F %T", ftime);
        return (RexxObjectPtr)context->NewStringFromAsciiz(buf);
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysStat"));
    return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
}

/**
 * Method:        SysAccess
 *
 * Return access information.
 *
 * @param file    The file specification.
 *
 * @param ichar   The integer option.
 *
 * @return        0 = access allowed.
 */
RexxRoutine2(int,
             SysAccess,
             CSTRING, file,
             int, option)
{

    return access(file, option);
}

/**
 * Method:        SysEuidaccess
 *
 * Return access information using the effective userid.
 *
 * @param file    The file specification.
 *
 * @param ichar   The integer option.
 *
 * @return        0 = access allowed.
 */
RexxRoutine2(int,
             SysEuidaccess,
             CSTRING, file,
             int, option)
{

    return euidaccess(file, option);
}

/**
 * Method:        SysGetservbyname
 *
 * Return service information from the services file.
 *
 * @param name    The service name.
 *
 * @param proto   The service protocol.
 *
 * @param option  The option string.
 *
 * @return        Option information
 */
RexxRoutine3(RexxObjectPtr,
             SysGetservbyname,
             CSTRING, name,
             CSTRING, proto,
             CSTRING, ichar)
{
    if (strlen(name) == 0 || strlen(proto) == 0 || strlen(ichar) != 1) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetservbyname"));
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    struct servent *se = getservbyname(name, proto);
    if (*ichar == 'N' || *ichar == 'n') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(se->s_name);
    }
    else if (*ichar == 'P' || *ichar == 'p') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)se->s_port);
    }
    else if (*ichar == 'A' || *ichar == 'a') {
        RexxArrayObject arr = context->NewArray(1);
        char **members = se->s_aliases;
        while (*members != NULL) {
           context->ArrayAppendString(arr, *members, strlen(*members));
           members++;
        }
        return (RexxObjectPtr)arr;
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetservbyname"));
    return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
}

/**
 * Method:        SysGetservbyport
 *
 * Return service information from the services file.
 *
 * @param port    The service port.
 *
 * @param proto   The service protocol.
 *
 * @param option  The option string.
 *
 * @return        Option information
 */
RexxRoutine3(RexxObjectPtr,
             SysGetservbyport,
             int, port,
             CSTRING, proto,
             CSTRING, ichar)
{
    if (port == 0 || strlen(proto) == 0 || strlen(ichar) != 1) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetservbyname"));
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    struct servent *se = getservbyport(port, proto);
    if (ichar == NULL) {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    else if (*ichar == 'N' || *ichar == 'n') {
        return (RexxObjectPtr)context->NewStringFromAsciiz(se->s_name);
    }
    else if (*ichar == 'P' || *ichar == 'p') {
        return (RexxObjectPtr)context->WholeNumberToObject((wholenumber_t)se->s_port);
    }
    else if (*ichar == 'A' || *ichar == 'a') {
        RexxArrayObject arr = context->NewArray(1);
        char **members = se->s_aliases;
        while (*members != NULL) {
           context->ArrayAppendString(arr, *members, strlen(*members));
           members++;
        }
        return (RexxObjectPtr)arr;
    }
    context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysGetservbyname"));
    return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
}

/**
 * Method:        SysWordexp
 *
 * Return an array of files matching the input expression.
 *
 * @param inexp   The input expression.
 *
 * @return        Array of file names.
 */
RexxRoutine1(RexxObjectPtr,
             SysWordexp,
             CSTRING, inexp)
{
    wordexp_t p;
    char **w;
    if (strlen(inexp) == 0) {
        context->RaiseException1(40001, (RexxObjectPtr) context->NewStringFromAsciiz("SysWordexp"));
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    RexxArrayObject arr = context->NewArray(1);

    wordexp(inexp, &p, 0);

    w = p.we_wordv;
    for (int i = 0; i < p.we_wordc; i++) {
       context->ArrayAppendString(arr, w[i], strlen(w[i]));
    }
    wordfree(&p);
    return (RexxObjectPtr)arr;
}

/**
 * Method:        SysSetxattr
 *
 * Set a file extended attribute.
 *
 * @param fname   The file name.
 *
 * @param name    The extended attribute name.
 *
 * @param val     The extended attribute value.
 *
 * @return        0 or -1.
 */
RexxRoutine3(int,
             SysSetxattr,
             CSTRING, fname,
             CSTRING, name,
             CSTRING, val)
{
    return setxattr(fname, name, val, strlen(val), 0);
}

/**
 * Method:        SysGetxattr
 *
 * Get a file extended attribute.
 *
 * @param fname   The file name.
 *
 * @param name    The extended attribute name.
 *
 * @return        0 or -1.
 */
RexxRoutine2(RexxObjectPtr,
             SysGetxattr,
             CSTRING, fname,
             CSTRING, name)
{
    ssize_t sz;
    char *buf;

    sz = getxattr(fname, name, NULL, 0);
    if (sz == -1) {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    buf = (char *)alloca(sz + 1);
    getxattr(fname, name, buf, sizeof(buf));

    return (RexxObjectPtr)context->NewStringFromAsciiz(buf);
}

/**
 * Method:        SysListxattr
 *
 * List a file's extended attribute(s).
 *
 * @param fname   The file name.
 *
 * @return        0 or -1.
 */
RexxRoutine1(RexxObjectPtr,
             SysListxattr,
             CSTRING, fname)
{
    ssize_t sz;
    char *buf, *name;
    
    sz = listxattr(fname, NULL, 0);
    if (sz == -1) {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    buf = (char *)alloca(sz + 1);
    listxattr(fname, buf, sizeof(buf));

    // create a Rexx array of the xattr names
    RexxArrayObject arr = context->NewArray(1);
    if (sz > 0) {
        while (sz > 1) {
            name = buf;
            context->ArrayAppendString(arr, name, strlen(name));
            buf = buf + strlen(name) + 1;
            sz -= strlen(name) + 1;
        }
    }

    return (RexxObjectPtr)arr;
}

/**
 * Method:        SysRemovexattr
 *
 * Remove an extended attribute.
 *
 * @param fname   The file name.
 *
 * @param name    The extended attribute name.
 *
 * @return        0 or -1.
 */
RexxRoutine2(int,
             SysRemovexattr,
             CSTRING, name,
             CSTRING, fname)
{
    

    return removexattr(fname, name);
}


// initialize the libvirt library
static void orxnixclib_loader(RexxThreadContext *context) {
   }


// build the actual entry list
RexxRoutineEntry orxnixclib_routines[] = {
    REXX_TYPED_ROUTINE(SysSignal, SysSignal),
    REXX_TYPED_ROUTINE(SysUname, SysUname),
    REXX_TYPED_ROUTINE(SysSetsid, SysSetsid),
    REXX_TYPED_ROUTINE(SysGetsid, SysGetsid),
    REXX_TYPED_ROUTINE(SysSetuid, SysSetuid),
    REXX_TYPED_ROUTINE(SysGetuid, SysGetuid),
    REXX_TYPED_ROUTINE(SysSeteuid, SysSeteuid),
    REXX_TYPED_ROUTINE(SysGeteuid, SysGeteuid),
    REXX_TYPED_ROUTINE(SysSetgid, SysSetgid),
    REXX_TYPED_ROUTINE(SysGetgid, SysGetgid),
    REXX_TYPED_ROUTINE(SysSetegid, SysSetegid),
    REXX_TYPED_ROUTINE(SysGetegid, SysGetegid),
    REXX_TYPED_ROUTINE(SysSetpgrp, SysSetpgrp),
    REXX_TYPED_ROUTINE(SysGetpgrp, SysGetpgrp),
    REXX_TYPED_ROUTINE(SysSetpgid, SysSetpgid),
    REXX_TYPED_ROUTINE(SysGetpgid, SysGetpgid),
    REXX_TYPED_ROUTINE(SysGetpid, SysGetpid),
    REXX_TYPED_ROUTINE(SysGetppid, SysGetppid),
    REXX_TYPED_ROUTINE(SysGettid, SysGettid),
    REXX_TYPED_ROUTINE(SysSymlink, SysSymlink),
    REXX_TYPED_ROUTINE(SysLink, SysLink),
    REXX_TYPED_ROUTINE(SysUnlink, SysUnlink),
    REXX_TYPED_ROUTINE(SysChown, SysChown),
    REXX_TYPED_ROUTINE(SysChroot, SysChroot),
    REXX_TYPED_ROUTINE(SysUmask, SysUmask),
    REXX_TYPED_ROUTINE(SysGetpwnam, SysGetpwnam),
    REXX_TYPED_ROUTINE(SysGetpwuid, SysGetpwuid),
    REXX_TYPED_ROUTINE(SysGetgrnam, SysGetgrnam),
    REXX_TYPED_ROUTINE(SysGetgrgid, SysGetgrgid),
    REXX_TYPED_ROUTINE(SysStat, SysStat),
    REXX_TYPED_ROUTINE(SysAccess, SysAccess),
    REXX_TYPED_ROUTINE(SysEuidaccess, SysEuidaccess),
    REXX_TYPED_ROUTINE(SysGetservbyname, SysGetservbyname),
    REXX_TYPED_ROUTINE(SysGetservbyport, SysGetservbyport),
    REXX_TYPED_ROUTINE(SysWordexp, SysWordexp),
    REXX_TYPED_ROUTINE(SysSetxattr, SysSetxattr),
    REXX_TYPED_ROUTINE(SysGetxattr, SysGetxattr),
    REXX_TYPED_ROUTINE(SysListxattr, SysListxattr),
    REXX_TYPED_ROUTINE(SysRemovexattr, SysRemovexattr),
    REXX_LAST_ROUTINE()
};


// build the actual entry list
RexxMethodEntry orxnixclib_methods[] = {
    REXX_LAST_METHOD()
};


RexxPackageEntry orxnixclib_package_entry = {
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "OrxNixCLib",                        // name of the package
    "1.0.0",                             // package information
    orxnixclib_loader,                   // load function
    NULL,                                // unload function
    orxnixclib_routines,                 // the exported routines
    orxnixclib_methods                   // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(orxnixclib);
