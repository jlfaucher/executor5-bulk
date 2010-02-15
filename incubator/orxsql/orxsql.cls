/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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


::class database public

-- connection attributes
::constant SQL_ATTR_OUTPUT_NTS          10001    -- probably not needed for 00Rexx

-- connection attributes
::constant SQL_ATTR_ACCESS_MODE		    101
::constant SQL_ATTR_AUTOCOMMIT			102
::constant SQL_ATTR_CONNECTION_TIMEOUT	113
::constant SQL_ATTR_CURRENT_CATALOG	    109
::constant SQL_ATTR_DISCONNECT_BEHAVIOR	114
::constant SQL_ATTR_ENLIST_IN_DTC		1207
::constant SQL_ATTR_ENLIST_IN_XA		1208
::constant SQL_ATTR_LOGIN_TIMEOUT		103
::constant SQL_ATTR_ODBC_CURSORS		110
::constant SQL_ATTR_PACKET_SIZE		    112
::constant SQL_ATTR_QUIET_MODE			111
::constant SQL_ATTR_TRACE				104
::constant SQL_ATTR_TRACEFILE			105
::constant SQL_ATTR_TRANSLATE_LIB		106
::constant SQL_ATTR_TRANSLATE_OPTION	107
::constant SQL_ATTR_TXN_ISOLATION		108
::constant SQL_ATTR_CONNECTION_DEAD	    1209
::constant SQL_ATTR_DRIVER_THREADING	1028

::attribute SQLRETURN

::method init EXTERNAL "Library orxsql OrxDB_dbinit"
::method getenvattr EXTERNAL "Library orxsql OrxDB_get_env_attr"
::method setenvattr EXTERNAL "Library orxsql OrxDB_set_env_attr"
::method connect EXTERNAL "Library orxsql OrxDB_connect"
::method disconnect EXTERNAL "Library orxsql OrxDB_close"
::method get_connect_attr EXTERNAL "Library orxsql OrxDB_get_connx_attr"
::method set_connect_attr EXTERNAL "Library orxsql OrxDB_set_connx_attr"









--
-- ::method prepare EXTERNAL "Library orxsql OrxDB_prepare"
-- ::method execute EXTERNAL "Library orxsql OrxDB_dbexecute"
-- ::method begin EXTERNAL "Library orxsql OrxDB_begin"
-- ::method commit EXTERNAL "Library orxsql OrxDB_commit"
-- ::method rollback EXTERNAL "Library orxsql OrxDB_rollback"
-- ::method check_liveness EXTERNAL "Library orxsql OrxDB_check_liveness"
-- ::method version class EXTERNAL "Library orxsql OrxDB_version"
--
-- ::method init EXTERNAL "Library orxsql OrxDB_stmtinit"
-- ::method destroy EXTERNAL "Library orxsql OrxDB_destroy"
-- ::method execute EXTERNAL "Library orxsql OrxDB_stmtexecute"
-- ::method fetch EXTERNAL "Library orxsql OrxDB_fetch"
-- ::method describe_col EXTERNAL "Library orxsql OrxDB_describe_col"
-- ::method get_col_data EXTERNAL "Library orxsql OrxDB_get_col_data"
-- ::method get_attr EXTERNAL "Library orxsql OrxDB_get_stmt_attr"
-- ::method set_attr EXTERNAL "Library orxsql OrxDB_set_stmt_attr"

