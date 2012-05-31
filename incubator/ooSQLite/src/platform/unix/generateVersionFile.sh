#! /bin/sh
#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 2012-2012 Rexx Language Association. All rights reserved.    */
#/*                                                                            */
#/* This program and the accompanying materials are made available under       */
#/* the terms of the Common Public License v1.0 which accompanies this         */
#/* distribution. A copy is also available at the following address:           */
#/* http://www.oorexx.org/license.html                                         */
#/*                                                                            */
#/* Redistribution and use in source and binary forms, with or                 */
#/* without modification, are permitted provided that the following            */
#/* conditions are met:                                                        */
#/*                                                                            */
#/* Redistributions of source code must retain the above copyright             */
#/* notice, this list of conditions and the following disclaimer.              */
#/* Redistributions in binary form must reproduce the above copyright          */
#/* notice, this list of conditions and the following disclaimer in            */
#/* the documentation and/or other materials provided with the distribution.   */
#/*                                                                            */
#/* Neither the name of Rexx Language Association nor the names                */
#/* of its contributors may be used to endorse or promote products             */
#/* derived from this software without specific prior written permission.      */
#/*                                                                            */
#/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
#/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
#/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
#/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
#/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
#/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
#/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
#/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
#/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
#/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
#/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
#/*                                                                            */
#/*----------------------------------------------------------------------------*/

SVN_VER=`svnversion`

. src/ooSQLite.ver

echo OOSQLITE_MAJOR=$OOSQLITE_MAJOR > src/ooSQLite.ver.incl
echo OOSQLITE_MINOR=$OOSQLITE_MINOR >> src/ooSQLite.ver.incl
echo OOSQLITE_MOD_LVL=$OOSQLITE_MOD_LVL >> src/ooSQLite.ver.incl
echo OOSQLITE_BLD_LVL=$SVN_VER >> src/ooSQLite.ver.incl
echo OOSQLITE_VER_STR="${OOSQLITE_MAJOR}.${OOSQLITE_MINOR}.${OOSQLITE_MOD_LVL}.${SVN_VER}" >> src/ooSQLite.ver.incl
echo OOSQLITE_COPY_YEAR=$OOSQLITE_COPY_YEAR >> src/ooSQLite.ver.incl
echo SVN_VER=$SVN_VER >> src/ooSQLite.ver.incl

