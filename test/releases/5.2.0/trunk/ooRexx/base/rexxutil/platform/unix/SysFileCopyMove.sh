#!/bin/sh

# Test SysFileCopy and SysFileMove


# These directories must be on different mounts, to test the move across filesystems
d1="/tmp/testSysFile"
d2="/local/tmp/testSysFile"

# Error codes
SUCCESS=0
ENOENT=2
EACCESS=13
EEXIST=17
EXDEV=18
EISDIR=21

echo
echo "Test SysFileCopy and SysFileMove."
echo "The following directories will be removed :"
echo "$d1"
echo "$d2"
echo
read -p "Press <Return> to continue or <Ctrl-C> to abort."

create_files()
{
    # Parameters :
    # $1 : directory path
    # $2 : directory mode ("rw" or "r" or "w" or whatever supported by chmod)
    
    # Each generated file has a specific timestamp : f1=01010001, f2=01010002, etc...
    # Each generated file has a symbolic link (add "l") at then end of the filename
    # f0rw
    # f1rw
    # f2r
    # f3w
    # f4

    d=$1
    d_mode=$2
    
    mkdir -p "$d" || (echo "Can't create $d" ; exit 1)
    
    # read-write file

    f="$d/f0rw"
    echo "$f : readable, writable" > "$f"
    touch -t 01010000 "$f"
    chmod a-rwx,u+rw "$f"

    l="${f}l"
    ln -s "$f" "$l"

    f="$d/f1rw"
    echo "$f : readable, writable" > "$f"
    touch -t 01010001 "$f"
    chmod a-rwx,u+rw "$f"

    l="${f}l"
    ln -s "$f" "$l"

    # read-only file

    f="$d/f2r"
    echo "$f : readable, not writable" > "$f"
    touch -t 01010002 "$f"
    chmod a-rwx,u+r "$f"

    l="${f}l"
    ln -s "$f" "$l"

    # write-only file

    f="$d/f3w"
    echo "$f : not readable, writable" > "$f"
    touch -t 01010003 "$f"
    chmod a-rwx,u+w "$f"

    l="${f}l"
    ln -s "$f" "$l"

    # no access file

    f="$d/f4"
    echo "$f : not readable, not writable" > "$f"
    touch -t 01010004 "$f"
    chmod a-rwx "$f"

    l="${f}l"
    ln -s "$f" "$l"
    
    # Set directory mode (do it at the end)
    chmod "$d_mode" "$d"
}

run_rexx()
{
    # Parameters :
    # $1 : SysFileCopy or SysFileMove
    # $2 : file1
    # $3 : file2
    # $4 : expected error code
    
    command=$1
    source=$2
    target=$3
    expected_error=$4
    
    testCount=$(($testCount+1))
    test -e "$target"
    target_exists=$? # will be used in case of error, to ensure that the target is still here
    rexx_command="$command(\"$source\", \"$target\")"
    rexx_command_verbose="error = $rexx_command ; call charout , SysGetErrorText(error)' ('error') --> ' ; return error"
    echo -n "$rexx_command : "
    rexx -e "$rexx_command_verbose"
    error=$?
    if [ $error -ne $expected_error ]
    then
        echo "KO : expected error code $expected_error"
        errorCount=$(($errorCount+1))
    elif [ $error -eq 0 ]
    then
        if [ $command = "SysFileCopy" ]
        then
            # No error has been returned, we can verify that the files contents are identical.
            # But we can do that only if both files are readable...
            if [ -r "$source" -a -r "$target" ]
            then
                diff "$source" "$target" > /dev/null 2>&1
                if [ $? -ne 0 ]
                then
                    echo "KO : files contents are different"
                    errorCount=$(($errorCount+1))
                else
                    echo "OK"
                fi
            elif [ ! -e "$source" ]
            then
                echo "KO : no error returned, but the source has been removed"
                errorCount=$(($errorCount+1))
            elif [ ! -e "$target" ]
            then
                echo "KO : no error returned, but the target has been removed"
                errorCount=$(($errorCount+1))
            else
                echo "OK (but contents not compared, at least one of the files is not readable)"
            fi
        else
            # No error has been returned, we can't verify that the file contents is ok because 
            # the source file is no longer available after the move. But we can verify that the
            # source is not longer here, and that the target exists.
            if [ -e "$source" ]
            then
                echo "KO : the source file is still here"
                errorCount=$(($errorCount+1))
            elif [ ! -e "$target" ]
            then
                echo "KO : the target file is not here"
                errorCount=$(($errorCount+1))
            else
                echo "OK (but contents not verified)"
            fi
        fi
    else
        # An error has been returned, and this is the expected behavior
        # But... There was a bug in the early implementation of SysFileMove : 
        # When the move was cross mount, and the source was not deletable,
        # then the target was deleted (was due to a bad decision to undo the move).
        # So if the target existed before the move, then ensure that it is still here after the move...
        if [ $command = "SysFileMove" -a $target_exists -eq 0 -a ! -e "$target" ] # remember : true == 0
        then
            echo "KO : the move has failed (that was expected) but the target has been removed (that's not good)"
            errorCount=$(($errorCount+1))
        else
            echo "OK"
        fi
    fi
}


# Make clean directories for tests
chmod -R a+rwx "$d1" > /dev/null 2>&1
chmod -R a+rwx "$d2" > /dev/null 2>&1
rm -fr "$d1"  || { echo "aborting" ; exit 1 ; }
rm -fr "$d2" || { echo "aborting" ; exit 1 ; }
mkdir -p "$d1" || { echo "aborting" ; exit 1 ; }
mkdir -p "$d2" || { echo "aborting" ; exit 1 ; }

# Ensure that you have normal rights (if you are root, for example, then plenty of asserts would be wrong)
dir1="$d1/copy00rwx"
create_files "$dir1" "a-rwx,u+rwx"
cp "$dir1/f0rw" "$dir1/f2r" > /dev/null 2>&1 && { echo "You have special rights (you can write in a read-only file), aborting" ; exit 1 ; }
cp "$dir1/f3w" "$dir1/f1rw" > /dev/null 2>&1 && { echo "You have special rights (you can read in a write-only file), aborting" ; exit 1 ; }
dir1="$d1/copy00rx"
create_files "$dir1" "a-rwx,u+rx"
rm -f "$dir1/f0rw" > /dev/null 2>&1 && { echo "You have special rights (you can remove a file in a read-only directory), aborting" ; exit 1 ; }
dir2="$d2/copy00rwx"
create_files "$dir2" "a-rwx,u+rwx"
cp "$dir2/f0rw" "$dir2/f2r" > /dev/null 2>&1 && { echo "You have special rights (you can write in a read-only file), aborting" ; exit 1 ; }
cp "$dir2/f3w" "$dir2/f1rw" > /dev/null 2>&1 && { echo "You have special rights (you can read in a write-only file), aborting" ; exit 1 ; }
dir2="$d2/copy00rx"
create_files "$dir2" "a-rwx,u+rx"
rm -f "$dir2/f0rw" > /dev/null 2>&1 && { echo "You have special rights (you can remove a file in a read-only directory), aborting" ; exit 1 ; }

testCount=0
errorCount=0

################
#              #
# Copy invalid #
#              #
################

echo
echo "Copy non existent file"
run_rexx SysFileCopy "$d1/non_existent" "$d1/should_not_be_generated" $ENOENT

echo
echo "Copy on itself, not allowed"
dir1="$d1/copy01rwx"
create_files "$dir1" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir1/f1rw" $EEXIST
run_rexx SysFileCopy "$dir1/f2r" "$dir1/f2r" $EEXIST
run_rexx SysFileCopy "$dir1/f3w" "$dir1/f3w" $EEXIST
run_rexx SysFileCopy "$dir1/f4" "$dir1/f4" $EEXIST

echo
echo "Copy on a directory is not allowed, must always specify a target filename"
mkdir "$dir1/directory"
run_rexx SysFileCopy "$dir1/f1rw" "$dir1/directory" $EISDIR
run_rexx SysFileCopy "$dir1/f2r" "$dir1/directory" $EISDIR
run_rexx SysFileCopy "$dir1/f3w" "$dir1/directory" $EACCESS # source not readable
run_rexx SysFileCopy "$dir1/f4" "$dir1/directory" $EACCESS # source not readable

##################################
#                                #
# Copy on same mount, in RWX dir #
#                                #
##################################

echo
echo "Copy to new file on same mount, in RWX dir"
dir1="$d1/copy02rwx"
create_files "$dir1" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir1/f1rw_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir1/f2r_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir1/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir1/f4_copy" $EACCESS

echo
echo "Copy to existing file on same mount, in RWX dir"
dir1="$d1/copy03rwx"
create_files "$dir1" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f0rw" "$dir1/f1rw" $SUCCESS
run_rexx SysFileCopy "$dir1/f0rw" "$dir1/f2r" $EACCESS
run_rexx SysFileCopy "$dir1/f0rw" "$dir1/f3w" $SUCCESS
run_rexx SysFileCopy "$dir1/f0rw" "$dir1/f4" $EACCESS

echo
echo "Copy from symbolic link on same mount, in RWX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy04rwx"
create_files "$dir1" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f1rwl" "$dir1/f1rw_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f2rl" "$dir1/f2r_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f3wl" "$dir1/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4l" "$dir1/f4_copy" $EACCESS

echo
echo "Copy to symbolic link on same mount, in RWX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy05rwx"
create_files "$dir1" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f0rw" "$dir1/f1rwl" $SUCCESS
run_rexx SysFileCopy "$dir1/f0rw" "$dir1/f2rl" $EACCESS
run_rexx SysFileCopy "$dir1/f0rw" "$dir1/f3wl" $SUCCESS
run_rexx SysFileCopy "$dir1/f0rw" "$dir1/f4l" $EACCESS

#############################################
#                                           #
# Copy on same mount, from RX dir to WX dir #
#                                           #
#############################################

echo
echo "Copy to new file on same mount, from RX dir to WX dir"
dir1="$d1/copy06rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d1/copy06wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4_copy" $EACCESS

echo
echo "Copy to existing file on same mount, from RX dir to WX dir"
dir1="$d1/copy07rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d1/copy07wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r" $EACCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4" $EACCESS

echo
echo "Copy from symbolic link on same mount, from RX dir to WX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy08rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d1/copy08wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileCopy "$dir1/f1rwl" "$dir2/f1rw_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f2rl" "$dir2/f2r_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f3wl" "$dir2/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4l" "$dir2/f4_copy" $EACCESS

echo
echo "Copy to symbolic link on same mount, from RX dir to WX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy09rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d1/copy09wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rwl" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2rl" $EACCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3wl" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4l" $EACCESS

#############################################
#                                           #
# Copy on same mount, from WX dir to RX dir #
#                                           #
#############################################

echo
echo "Copy to new file on same mount, from WX dir to RX dir"
dir1="$d1/copy10wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d1/copy10rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw_copy" $EACCESS # can't create target
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r_copy" $EACCESS # can't create target
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w_copy" $EACCESS # can't read source and can't create target
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4_copy" $EACCESS # can't read source and can't create target

echo
echo "Copy to existing file on same mount, from WX dir to RX dir"
dir1="$d1/copy11wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d1/copy11rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw" $SUCCESS # yes, source is readable despite the fact the directory is not readable
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r" $EACCESS # target is not writable
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w" $EACCESS # source is not readable
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4" $EACCESS # source is not readable

echo
echo "Copy from symbolic link on same mount, from WX dir to RX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy12wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d1/copy12rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileCopy "$dir1/f1rwl" "$dir2/f1rw_copy" $EACCESS # can't create target
run_rexx SysFileCopy "$dir1/f2rl" "$dir2/f2r_copy" $EACCESS # can't create target
run_rexx SysFileCopy "$dir1/f3wl" "$dir2/f3w_copy" $EACCESS # can't read source and can't create target
run_rexx SysFileCopy "$dir1/f4l" "$dir2/f4_copy" $EACCESS # can't read source and can't create target

echo
echo "Copy to symbolic link on same mount, from WX dir to RX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy13wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d1/copy13rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rwl" $SUCCESS # yes, source is readable despite the fact the directory is not readable
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2rl" $EACCESS # target is not writable
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3wl" $EACCESS # source is not readable
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4l" $EACCESS # source is not readable

###############################################
#                                             #
# Copy across mounts, from RWX dir to RWX dir #
#                                             #
###############################################

echo
echo "Copy to new file across different mounts, from RWX dir to RWX dir"
dir1="$d1/copy14rwx"
create_files "$dir1" "a-rwx,u+rwx"
dir2="$d2/copy14rwx"
create_files "$dir2" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4_copy" $EACCESS

echo
echo "Copy to existing file across different mounts, from RWX dir to RWX dir"
dir1="$d1/copy15rwx"
create_files "$dir1" "a-rwx,u+rwx"
dir2="$d2/copy15rwx"
create_files "$dir2" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r" $EACCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4" $EACCESS

echo
echo "Copy from symbolic link across different mounts, from RWX dir to RWX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy16rwx"
create_files "$dir1" "a-rwx,u+rwx"
dir2="$d2/copy16rwx"
create_files "$dir2" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f1rwl" "$dir2/f1rw_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f2rl" "$dir2/f2r_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f3wl" "$dir2/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4l" "$dir2/f4_copy" $EACCESS

echo
echo "Copy to symbolic link across different mounts, from RWX dir to RWX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy17rwx"
create_files "$dir1" "a-rwx,u+rwx"
dir2="$d2/copy17rwx"
create_files "$dir2" "a-rwx,u+rwx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rwl" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2rl" $EACCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3wl" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4l" $EACCESS

#############################################
#                                           #
# Copy across mounts, from RX dir to WX dir #
#                                           #
#############################################

echo
echo "Copy to new file across different mounts, from RX dir to WX dir"
dir1="$d1/copy18rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d2/copy18wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4_copy" $EACCESS

echo
echo "Copy to existing file across different mounts, from RX dir to WX dir"
dir1="$d1/copy19rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d2/copy19wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r" $EACCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4" $EACCESS

echo
echo "Copy from symbolic link across different mounts, from RX dir to WX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy20rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d2/copy20wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileCopy "$dir1/f1rwl" "$dir2/f1rw_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f2rl" "$dir2/f2r_copy" $SUCCESS
run_rexx SysFileCopy "$dir1/f3wl" "$dir2/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4l" "$dir2/f4_copy" $EACCESS

echo
echo "Copy to symbolic link across different mounts, from RX dir to WX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy21rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d2/copy21wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rwl" $SUCCESS
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2rl" $EACCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3wl" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4l" $EACCESS

#############################################
#                                           #
# Copy across mounts, from WX dir to RX dir #
#                                           #
#############################################

echo
echo "Copy to new file across different mounts, from WX dir to RX dir"
dir1="$d1/copy22wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d2/copy22rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw_copy" $EACCESS # can't create target
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r_copy" $EACCESS # can't create target
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w_copy" $EACCESS # can't read source and can't create target
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4_copy" $EACCESS # can't read source and can't create target

echo
echo "Copy to existing file across different mounts, from WX dir to RX dir"
dir1="$d1/copy23wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d2/copy23rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rw" $SUCCESS # yes, source is readable
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2r" $EACCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3w" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4" $EACCESS

echo
echo "Copy from symbolic link across different mounts, from WX dir to RX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy24wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d2/copy24rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileCopy "$dir1/f1rwl" "$dir2/f1rw_copy" $EACCESS # can't create target
run_rexx SysFileCopy "$dir1/f2rl" "$dir2/f2r_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f3wl" "$dir2/f3w_copy" $EACCESS
run_rexx SysFileCopy "$dir1/f4l" "$dir2/f4_copy" $EACCESS

echo
echo "Copy to symbolic link across different mounts, from WX dir to RX dir"
echo "The symbolic links are always dereferenced"
dir1="$d1/copy25wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d2/copy25rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileCopy "$dir1/f1rw" "$dir2/f1rwl" $SUCCESS # yes, source is readable
run_rexx SysFileCopy "$dir1/f2r" "$dir2/f2rl" $EACCESS
run_rexx SysFileCopy "$dir1/f3w" "$dir2/f3wl" $EACCESS
run_rexx SysFileCopy "$dir1/f4" "$dir2/f4l" $EACCESS

################
#              #
# Move invalid #
#              #
################

echo
echo "Move from non existent file"
run_rexx SysFileMove "$d1/non_existent" "$d1/should_not_be_generated" $ENOENT

echo
echo "Move on itself, not allowed"
dir1="$d1/move01rwx"
create_files "$dir1" "a-rwx,u+rwx"
run_rexx SysFileMove "$dir1/f1rw" "$dir1/f1rw" $EEXIST
run_rexx SysFileMove "$dir1/f2r" "$dir1/f2r" $EEXIST
run_rexx SysFileMove "$dir1/f3w" "$dir1/f3w" $EEXIST
run_rexx SysFileMove "$dir1/f4" "$dir1/f4" $EEXIST

echo
echo "Move on a directory is not allowed, must always specify a target filename"
mkdir "$dir1/directory"
run_rexx SysFileMove "$dir1/f1rw" "$dir1/directory" $EISDIR
run_rexx SysFileMove "$dir1/f2r" "$dir1/directory" $EISDIR
run_rexx SysFileMove "$dir1/f3w" "$dir1/directory" $EISDIR
run_rexx SysFileMove "$dir1/f4" "$dir1/directory" $EISDIR

##################################
#                                #
# Move on same mount, in RWX dir #
#                                #
##################################

echo
echo "Move to new file on same mount, in RWX dir"
dir1="$d1/move02rwx"
create_files "$dir1" "a-rwx,u+rwx"
run_rexx SysFileMove "$dir1/f1rw" "$dir1/f1rw_moved" $SUCCESS
run_rexx SysFileMove "$dir1/f2r" "$dir1/f2r_moved" $SUCCESS
run_rexx SysFileMove "$dir1/f3w" "$dir1/f3w_moved" $SUCCESS # source not readable but it works thanks to rename()
run_rexx SysFileMove "$dir1/f4" "$dir1/f4_moved" $SUCCESS # source not readable but it works thanks to rename()

echo
echo "Move to existing file on same mount, in RWX dir"
dir1="$d1/move03rwx"
create_files "$dir1" "a-rwx,u+rwx"
cp --preserve=all "$dir1/f0rw" "$dir1/f0rw_clone1"
cp --preserve=all "$dir1/f0rw" "$dir1/f0rw_clone2"
cp --preserve=all "$dir1/f0rw" "$dir1/f0rw_clone3"
cp --preserve=all "$dir1/f0rw" "$dir1/f0rw_clone4"
run_rexx SysFileMove "$dir1/f0rw_clone1" "$dir1/f1rw" $SUCCESS
run_rexx SysFileMove "$dir1/f0rw_clone2" "$dir1/f2r" $SUCCESS # target not writable but it works thanks to rename()
run_rexx SysFileMove "$dir1/f0rw_clone3" "$dir1/f3w" $SUCCESS
run_rexx SysFileMove "$dir1/f0rw_clone4" "$dir1/f4" $SUCCESS # target not writable but it works thanks to rename()

echo
echo "Move from symbolic link on same mount, in RWX dir"
echo "The symbolic links are not dereferenced"
dir1="$d1/move04rwx"
create_files "$dir1" "a-rwx,u+rwx"
run_rexx SysFileMove "$dir1/f1rwl" "$dir1/f1rw_moved" $SUCCESS
run_rexx SysFileMove "$dir1/f2rl" "$dir1/f2r_moved" $SUCCESS 
run_rexx SysFileMove "$dir1/f3wl" "$dir1/f3w_moved" $SUCCESS # referenced source not readable but it works thanks to rename()
run_rexx SysFileMove "$dir1/f4l" "$dir1/f4_moved" $SUCCESS # referenced source not readable but it works thanks to rename()

echo
echo "Move to symbolic link on same mount, in RWX dir"
echo "The symbolic links are not dereferenced"
dir1="$d1/move05rwx"
create_files "$dir1" "a-rwx,u+rwx"
cp --preserve=all "$dir1/f0rw" "$dir1/f0rw_clone1"
cp --preserve=all "$dir1/f0rw" "$dir1/f0rw_clone2"
cp --preserve=all "$dir1/f0rw" "$dir1/f0rw_clone3"
cp --preserve=all "$dir1/f0rw" "$dir1/f0rw_clone4"
run_rexx SysFileMove "$dir1/f0rw_clone1" "$dir1/f1rwl" $SUCCESS
run_rexx SysFileMove "$dir1/f0rw_clone2" "$dir1/f2rl" $SUCCESS # target not writable but it works thanks to rename()
run_rexx SysFileMove "$dir1/f0rw_clone3" "$dir1/f3wl" $SUCCESS
run_rexx SysFileMove "$dir1/f0rw_clone4" "$dir1/f4l" $SUCCESS # target not writable but it works thanks to rename()

############################################
#                                          #
# Move on same mount, from RX dir to WX dir#
#                                          #
############################################

echo
echo "Move to new file on same mount, from RX dir to WX dir"
dir1="$d1/move06rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d1/move06wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw_moved" $EACCESS
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r_moved" $EACCESS
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w_moved" $EACCESS
run_rexx SysFileMove "$dir1/f4" "$dir2/f4_moved" $EACCESS

echo
echo "Move to existing file on same mount, from RX dir to WX dir"
dir1="$d1/move07rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d1/move07wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw" $EACCESS
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r" $EACCESS
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w" $EACCESS
run_rexx SysFileMove "$dir1/f4" "$dir2/f4" $EACCESS

echo
echo "Move from symbolic link on same mount, from RX dir to WX dir"
echo "The symbolic links are not dereferenced"
dir1="$d1/move08rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d1/move08wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileMove "$dir1/f1rwl" "$dir2/f1rw" $EACCESS
run_rexx SysFileMove "$dir1/f2rl" "$dir2/f2r" $EACCESS 
run_rexx SysFileMove "$dir1/f3wl" "$dir2/f3w" $EACCESS
run_rexx SysFileMove "$dir1/f4l" "$dir2/f4" $EACCESS

echo
echo "Move to symbolic link on same mount, from RX dir to WX dir"
echo "The symbolic links are not dereferenced"
dir1="$d1/move09rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d1/move09wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rwl" $EACCESS
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2rl" $EACCESS
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3wl" $EACCESS
run_rexx SysFileMove "$dir1/f4" "$dir2/f4l" $EACCESS

############################################
#                                          #
# Move on same mount, from WX dir to RX dir#
#                                          #
############################################

echo
echo "Move to new file on same mount, from WX dir to RX dir"
dir1="$d1/move10wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d1/move10rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw_moved" $EACCESS
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r_moved" $EACCESS
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w_moved" $EACCESS
run_rexx SysFileMove "$dir1/f4" "$dir2/f4_moved" $EACCESS

echo
echo "Move to existing file on same mount, from WX dir to RX dir"
dir1="$d1/move11wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d1/move11rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r" $EACCESS # target not writable
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w" $EACCESS
run_rexx SysFileMove "$dir1/f4" "$dir2/f4" $EACCESS

echo
echo "Move from symbolic link on same mount, from WX dir to RX dir"
echo "The symbolic links are not dereferenced"
dir1="$d1/move12wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d1/move12rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileMove "$dir1/f1rwl" "$dir2/f1rw" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f2rl" "$dir2/f2r" $EACCESS # target not writable
run_rexx SysFileMove "$dir1/f3wl" "$dir2/f3w" $EACCESS # referenced source not readable
run_rexx SysFileMove "$dir1/f4l" "$dir2/f4" $EACCESS # referenced source not readable

echo
echo "Move to symbolic link on same mount, from WX dir to RX dir"
echo "The symbolic links are not dereferenced"
dir1="$d1/move13wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d1/move13rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rwl" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2rl" $EACCESS # target not writable
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3wl" $EACCESS
run_rexx SysFileMove "$dir1/f4" "$dir2/f4l" $EACCESS

###############################################
#                                             #
# Move across mounts, from RWX dir to RWX dir #
#                                             #
###############################################

echo
echo "Move to new file across different mounts, from RWX dir to RWX dir"
dir1="$d1/move14rwx"
create_files "$dir1" "a-rwx,u+rwx"
dir2="$d2/move14rwx"
create_files "$dir2" "a-rwx,u+rwx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw_moved" $SUCCESS
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r_moved" $SUCCESS
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w_moved" $EACCESS # source not readable, move by copy+unlink doesn't work
run_rexx SysFileMove "$dir1/f4" "$dir2/f4_moved" $EACCESS # source not readable, move by copy+unlink doesn't work

echo
echo "Move to existing file across different mounts, from RWX dir to RWX dir"
dir1="$d1/move15rwx"
create_files "$dir1" "a-rwx,u+rwx"
dir2="$d2/move15rwx"
create_files "$dir2" "a-rwx,u+rwx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw" $SUCCESS
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r" $EACCESS # target not writable, move by copy+unlink doesn't work
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w" $EACCESS # source not readable, move by copy+unlink doesn't work
run_rexx SysFileMove "$dir1/f4" "$dir2/f4" $EACCESS # source not readable, target not writable, move by copy+unlink doesn't work

echo
echo "Move from symbolic link across different mounts, from RWX dir to RWX dir"
echo "The symbolic links are not dereferenced"
echo "NOTE : always fails when target exists (like the Windows version of SysFileMove)"
dir1="$d1/move16rwx"
create_files "$dir1" "a-rwx,u+rwx"
dir2="$d2/move16rwx"
create_files "$dir2" "a-rwx,u+rwx"
run_rexx SysFileMove "$dir1/f1rwl" "$dir2/f1rw" $EEXIST
run_rexx SysFileMove "$dir1/f2rl" "$dir2/f2r" $EEXIST
run_rexx SysFileMove "$dir1/f3wl" "$dir2/f3w" $EEXIST
run_rexx SysFileMove "$dir1/f4l" "$dir2/f4" $EEXIST

echo
echo "Move to symbolic link across different mounts, from RWX dir to RWX dir"
echo "The symbolic links are not dereferenced"
echo "NOTE : always fails when target exists (like the Windows version of SysFileMove)"
dir1="$d1/move17rwx"
create_files "$dir1" "a-rwx,u+rwx"
dir2="$d2/move17rwx"
create_files "$dir2" "a-rwx,u+rwx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rwl" $EEXIST
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2rl" $EEXIST
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3wl" $EEXIST
run_rexx SysFileMove "$dir1/f4" "$dir2/f4l" $EEXIST

#############################################
#                                           #
# Move across mounts, from RX dir to WX dir #
#                                           #
#############################################

echo
echo "Move to new file across different mounts, from RX dir to WX dir"
dir1="$d1/move18rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d2/move18wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw_moved" $EACCESS # can't unlink source, move by copy+unlink doesn't work
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r_moved" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w_moved" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f4" "$dir2/f4_moved" $EACCESS # can't unlink source

echo
echo "Move to existing file across different mounts, from RX dir to WX dir"
dir1="$d1/move19rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d2/move19wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f4" "$dir2/f4" $EACCESS # can't unlink source

echo
echo "Move from symbolic link across different mounts, from RX dir to WX dir"
echo "The symbolic links are not dereferenced"
dir1="$d1/move20rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d2/move20wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileMove "$dir1/f1rwl" "$dir2/f1rw" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f2rl" "$dir2/f2r" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f3wl" "$dir2/f3w" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f4l" "$dir2/f4" $EACCESS # can't unlink source

echo
echo "Move to symbolic link across different mounts, from RX dir to WX dir"
echo "The symbolic links are not dereferenced"
dir1="$d1/move21rx"
create_files "$dir1" "a-rwx,u+rx"
dir2="$d2/move21wx"
create_files "$dir2" "a-rwx,u+wx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rwl" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2rl" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3wl" $EACCESS # can't unlink source
run_rexx SysFileMove "$dir1/f4" "$dir2/f4l" $EACCESS # can't unlink source

#############################################
#                                           #
# Move across mounts, from WX dir to RX dir #
#                                           #
#############################################

echo
echo "Move to new file across different mounts, from WX dir to RX dir"
dir1="$d1/move22wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d2/move22rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw_moved" $EACCESS # can't create target
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r_moved" $EACCESS # can't create target
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w_moved" $EACCESS # source not readable, move by copy+unlink doesn't work
run_rexx SysFileMove "$dir1/f4" "$dir2/f4_moved" $EACCESS # source not readable, move by copy+unlink doesn't work

echo
echo "Move to existing file across different mounts, from WX dir to RX dir"
dir1="$d1/move23wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d2/move23rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rw" $SUCCESS
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2r" $EACCESS # target not writable, move by copy+unlink doesn't work
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3w" $EACCESS # source not readable
run_rexx SysFileMove "$dir1/f4" "$dir2/f4" $EACCESS # target not writable, move by copy+unlink doesn't work

echo
echo "Move from symbolic link across different mounts, from WX dir to RX dir"
echo "The symbolic links are not dereferenced"
echo "NOTE : always fails when target exists (like the Windows version of SysFileMove)"
dir1="$d1/move24wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d2/move24rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileMove "$dir1/f1rwl" "$dir2/f1rw" $EEXIST
run_rexx SysFileMove "$dir1/f2rl" "$dir2/f2r" $EEXIST
run_rexx SysFileMove "$dir1/f3wl" "$dir2/f3w" $EEXIST
run_rexx SysFileMove "$dir1/f4l" "$dir2/f4" $EEXIST

echo
echo "Move to symbolic link across different mounts, from WX dir to RX dir"
echo "The symbolic links are not dereferenced"
echo "NOTE : always fails when target exists (like the Windows version of SysFileMove)"
dir1="$d1/move25wx"
create_files "$dir1" "a-rwx,u+wx"
dir2="$d2/move25rx"
create_files "$dir2" "a-rwx,u+rx"
run_rexx SysFileMove "$dir1/f1rw" "$dir2/f1rwl" $EEXIST
run_rexx SysFileMove "$dir1/f2r" "$dir2/f2rl" $EEXIST
run_rexx SysFileMove "$dir1/f3w" "$dir2/f3wl" $EEXIST
run_rexx SysFileMove "$dir1/f4" "$dir2/f4l" $EEXIST

################
#              #
# End of tests #
#              #
################

echo
echo "testCount=$testCount"
echo "errorCount=$errorCount"
if [ $errorCount -eq 0 ]
then
    exit 0
else
    exit 1
fi
