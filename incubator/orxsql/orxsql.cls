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


/*----------------------------------------------------------------------------*/
/* Class: database                                                            */
/*----------------------------------------------------------------------------*/

::class database public

-- This is just a base class to hold all the constants used by the dbconnx and
-- SQLstatement classes.

-- environment attributes
::constant SQL_ATTR_OUTPUT_NTS          10001    -- probably not needed for 00Rexx

-- connection attributes
::constant SQL_ATTR_ACCESS_MODE         101
::constant SQL_ATTR_AUTOCOMMIT          102
::constant SQL_ATTR_LOGIN_TIMEOUT       103
::constant SQL_ATTR_TRACE               104
::constant SQL_ATTR_TRACEFILE           105
::constant SQL_ATTR_TRANSLATE_LIB       106
::constant SQL_ATTR_TRANSLATE_OPTION    107
::constant SQL_ATTR_TXN_ISOLATION       108
::constant SQL_ATTR_CURRENT_CATALOG     109
::constant SQL_ATTR_ODBC_CURSORS        110
::constant SQL_ATTR_QUIET_MODE          111
::constant SQL_ATTR_PACKET_SIZE         112
::constant SQL_ATTR_CONNECTION_TIMEOUT  113
::constant SQL_ATTR_DISCONNECT_BEHAVIOR 114
::constant SQL_ATTR_DRIVER_THREADING    1028
::constant SQL_ATTR_ENLIST_IN_DTC       1207
::constant SQL_ATTR_ENLIST_IN_XA        1208
::constant SQL_ATTR_CONNECTION_DEAD     1209

-- getinfo types
::constant SQL_ACCESSIBLE_PROCEDURES            20
::constant SQL_MAX_DRIVER_CONNECTIONS          0
::constant SQL_MAXIMUM_DRIVER_CONNECTIONS       0
::constant SQL_MAX_CONCURRENT_ACTIVITIES       1
::constant SQL_MAXIMUM_CONCURRENT_ACTIVITIES    1
::constant SQL_DATA_SOURCE_NAME                2
::constant SQL_FETCH_DIRECTION                 8
::constant SQL_SERVER_NAME                     13
::constant SQL_SEARCH_PATTERN_ESCAPE           14
::constant SQL_DBMS_NAME                       17
::constant SQL_DBMS_VER                        18
::constant SQL_ACCESSIBLE_TABLES               19
::constant SQL_CURSOR_COMMIT_BEHAVIOR          23
::constant SQL_DATA_SOURCE_READ_ONLY           25
::constant SQL_DEFAULT_TXN_ISOLATION           26
::constant SQL_IDENTIFIER_CASE                 28
::constant SQL_IDENTIFIER_QUOTE_CHAR           29
::constant SQL_MAX_COLUMN_NAME_LEN             30
::constant SQL_MAXIMUM_COLUMN_NAME_LENGTH       30
::constant SQL_MAX_CURSOR_NAME_LEN             31
::constant SQL_MAXIMUM_CURSOR_NAME_LENGTH       31
::constant SQL_MAX_SCHEMA_NAME_LEN             32
::constant SQL_MAXIMUM_SCHEMA_NAME_LENGTH       32
::constant SQL_MAX_CATALOG_NAME_LEN            34
::constant SQL_MAXIMUM_CATALOG_NAME_LENGTH      34
::constant SQL_MAX_TABLE_NAME_LEN              35
::constant SQL_SCROLL_CONCURRENCY              43
::constant SQL_TXN_CAPABLE                     46
::constant SQL_TRANSACTION_CAPABLE              46
::constant SQL_USER_NAME                       47
::constant SQL_TXN_ISOLATION_OPTION            72
::constant SQL_TRANSACTION_ISOLATION_OPTION 72
::constant SQL_INTEGRITY                       73
::constant SQL_GETDATA_EXTENSIONS              81
::constant SQL_NULL_COLLATION                  85
::constant SQL_ALTER_TABLE                     86
::constant SQL_ORDER_BY_COLUMNS_IN_SELECT      90
::constant SQL_SPECIAL_CHARACTERS              94
::constant SQL_MAX_COLUMNS_IN_GROUP_BY         97
::constant SQL_MAXIMUM_COLUMNS_IN_GROUP_BY      97
::constant SQL_MAX_COLUMNS_IN_INDEX            98
::constant SQL_MAXIMUM_COLUMNS_IN_INDEX     98
::constant SQL_MAX_COLUMNS_IN_ORDER_BY         99
::constant SQL_MAXIMUM_COLUMNS_IN_ORDER_BY      99
::constant SQL_MAX_COLUMNS_IN_SELECT           100
::constant SQL_MAXIMUM_COLUMNS_IN_SELECT        100
::constant SQL_MAX_COLUMNS_IN_TABLE            101
::constant SQL_MAX_INDEX_SIZE                  102
::constant SQL_MAXIMUM_INDEX_SIZE               102
::constant SQL_MAX_ROW_SIZE                    104
::constant SQL_MAXIMUM_ROW_SIZE             104
::constant SQL_MAX_STATEMENT_LEN               105
::constant SQL_MAXIMUM_STATEMENT_LENGTH     105
::constant SQL_MAX_TABLES_IN_SELECT            106
::constant SQL_MAXIMUM_TABLES_IN_SELECT     106
::constant SQL_MAX_USER_NAME_LEN               107
::constant SQL_MAXIMUM_USER_NAME_LENGTH     107
::constant SQL_OUTER_JOIN_CAPABILITIES          115

-- bit masks/values
::constant SQL_AF_AVG                                   1
::constant SQL_AF_COUNT                                 2
::constant SQL_AF_MAX                                   4
::constant SQL_AF_MIN                                   8
::constant SQL_AF_SUM                                   16
::constant SQL_AF_DISTINCT                              32
::constant SQL_AF_ALL                                   64
::constant SQL_AD_CONSTRAINT_NAME_DEFINITION            1
::constant SQL_AD_ADD_DOMAIN_CONSTRAINT                 2
::constant SQL_AD_DROP_DOMAIN_CONSTRAINT                4
::constant SQL_AD_ADD_DOMAIN_DEFAULT                    8
::constant SQL_AD_DROP_DOMAIN_DEFAULT                   16
::constant SQL_AD_ADD_CONSTRAINT_INITIALLY_DEFERRED     32
::constant SQL_AD_ADD_CONSTRAINT_INITIALLY_IMMEDIATE    64
::constant SQL_AD_ADD_CONSTRAINT_DEFERRABLE             128
::constant SQL_AD_ADD_CONSTRAINT_NON_DEFERRABLE         256
::constant SQL_AT_ADD_COLUMN                            1
::constant SQL_AT_DROP_COLUMN                           2
::constant SQL_AT_ADD_CONSTRAINT                        8
::constant SQL_AT_ADD_COLUMN_SINGLE                     32
::constant SQL_AT_ADD_COLUMN_DEFAULT                    64
::constant SQL_AT_ADD_COLUMN_COLLATION                  128
::constant SQL_AT_SET_COLUMN_DEFAULT                    256
::constant SQL_AT_DROP_COLUMN_DEFAULT                   512
::constant SQL_AT_DROP_COLUMN_CASCADE                   1024
::constant SQL_AT_DROP_COLUMN_RESTRICT                  2048
::constant SQL_AT_ADD_TABLE_CONSTRAINT                  4096
::constant SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE         8192
::constant SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT        16384
::constant SQL_AT_CONSTRAINT_NAME_DEFINITION            32768
::constant SQL_AT_CONSTRAINT_INITIALLY_DEFERRED         65536
::constant SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE        131072
::constant SQL_AT_CONSTRAINT_DEFERRABLE                 262144
::constant SQL_AT_CONSTRAINT_NON_DEFERRABLE             524288
::constant SQL_AM_CONNECTION                            1
::constant SQL_AM_STATEMENT                             2
::constant SQL_BRC_PROCEDURES                           1
::constant SQL_BRC_EXPLICIT                             2
::constant SQL_BRC_ROLLED_UP                            4
::constant SQL_BS_SELECT_EXPLICIT                       1
::constant SQL_BS_ROW_COUNT_EXPLICIT                    2
::constant SQL_BS_SELECT_PROC                           4
::constant SQL_BS_ROW_COUNT_PROC                        8
::constant SQL_BP_CLOSE                                 1
::constant SQL_BP_DELETE                                2
::constant SQL_BP_DROP                                  4
::constant SQL_BP_TRANSACTION                           8
::constant SQL_BP_UPDATE                                16
::constant SQL_BP_OTHER_HSTMT                           32
::constant SQL_BP_SCROLL                                64
::constant SQL_CL_START                                 1
::constant SQL_CL_END                                   2
::constant SQL_QL_START                                 1
::constant SQL_QL_END                                   2
::constant SQL_CU_DML_STATEMENTS                        1
::constant SQL_CU_PROCEDURE_INVOCATION                  2
::constant SQL_CU_TABLE_DEFINITION                      4
::constant SQL_CU_INDEX_DEFINITION                      8
::constant SQL_CU_PRIVILEGE_DEFINITION                  16
::constant SQL_CB_NULL                                  0
::constant SQL_CB_NON_NULL                              1
::constant SQL_CB_DELETE                                0
::constant SQL_CB_CLOSE                                 1
::constant SQL_CB_PRESERVE                              2
::constant SQL_CVT_CHAR                                 1
::constant SQL_CVT_NUMERIC                              2
::constant SQL_CVT_DECIMAL                              4
::constant SQL_CVT_INTEGER                              8
::constant SQL_CVT_SMALLINT                             16
::constant SQL_CVT_FLOAT                                32
::constant SQL_CVT_REAL                                 64
::constant SQL_CVT_DOUBLE                               128
::constant SQL_CVT_VARCHAR                              256
::constant SQL_CVT_LONGVARCHAR                          512
::constant SQL_CVT_BINARY                               1024
::constant SQL_CVT_VARBINARY                            2048
::constant SQL_CVT_BIT                                  4096
::constant SQL_CVT_TINYINT                              8192
::constant SQL_CVT_BIGINT                               16384
::constant SQL_CVT_DATE                                 32768
::constant SQL_CVT_TIME                                 65536
::constant SQL_CVT_TIMESTAMP                            131072
::constant SQL_CVT_LONGVARBINARY                        262144
::constant SQL_CVT_INTERVAL_YEAR_MONTH                  524288
::constant SQL_CVT_INTERVAL_DAY_TIME                    1048576
::constant SQL_CVT_WCHAR                                2097152
::constant SQL_CVT_WLONGVARCHAR                         4194304
::constant SQL_CVT_WVARCHAR                             8388608
::constant SQL_CVT_GUID                                 16777216
::constant SQL_FN_CVT_CONVERT                           1
::constant SQL_FN_CVT_CAST                              2
::constant SQL_CN_NONE                                  0
::constant SQL_CN_DIFFERENT                             1
::constant SQL_CN_ANY                                   2
::constant SQL_CA_CREATE_ASSERTION                      1
::constant SQL_CA_CONSTRAINT_INITIALLY_DEFERRED         16
::constant SQL_CA_CONSTRAINT_INITIALLY_IMMEDIATE        32
::constant SQL_CA_CONSTRAINT_DEFERRABLE                 64
::constant SQL_CA_CONSTRAINT_NON_DEFERRABLE             128
::constant SQL_CCS_CREATE_CHARACTER_SET                 1
::constant SQL_CCS_COLLATE_CLAUSE                       2
::constant SQL_CCS_LIMITED_COLLATION                    4
::constant SQL_CCOL_CREATE_COLLATION                    1
::constant SQL_CDO_CREATE_DOMAIN                        1
::constant SQL_CDO_DEFAULT                              2
::constant SQL_CDO_CONSTRAINT                           4
::constant SQL_CDO_COLLATION                            8
::constant SQL_CDO_CONSTRAINT_NAME_DEFINITION           16
::constant SQL_CDO_CONSTRAINT_INITIALLY_DEFERRED        32
::constant SQL_CDO_CONSTRAINT_INITIALLY_IMMEDIATE       64
::constant SQL_CDO_CONSTRAINT_DEFERRABLE                128
::constant SQL_CDO_CONSTRAINT_NON_DEFERRABLE            256
::constant SQL_CS_CREATE_SCHEMA                         1
::constant SQL_CS_AUTHORIZATION                         2
::constant SQL_CS_DEFAULT_CHARACTER_SET                 4
::constant SQL_CT_CREATE_TABLE                          1
::constant SQL_CT_COMMIT_PRESERVE                       2
::constant SQL_CT_COMMIT_DELETE                         4
::constant SQL_CT_GLOBAL_TEMPORARY                      8
::constant SQL_CT_LOCAL_TEMPORARY                       16
::constant SQL_CT_CONSTRAINT_INITIALLY_DEFERRED         32
::constant SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE        64
::constant SQL_CT_CONSTRAINT_DEFERRABLE                 128
::constant SQL_CT_CONSTRAINT_NON_DEFERRABLE             256
::constant SQL_CT_COLUMN_CONSTRAINT                     512
::constant SQL_CT_COLUMN_DEFAULT                        1024
::constant SQL_CT_COLUMN_COLLATION                      2048
::constant SQL_CT_TABLE_CONSTRAINT                      4096
::constant SQL_CT_CONSTRAINT_NAME_DEFINITION            8192
::constant SQL_CTR_CREATE_TRANSLATION                   1
::constant SQL_CV_CREATE_VIEW                           1
::constant SQL_CV_CHECK_OPTION                          2
::constant SQL_CV_CASCADED                              4
::constant SQL_CV_LOCAL                                 8
::constant SQL_DL_SQL92_DATE                            1
::constant SQL_DL_SQL92_TIME                            2
::constant SQL_DL_SQL92_TIMESTAMP                       4
::constant SQL_DL_SQL92_INTERVAL_YEAR                   8
::constant SQL_DL_SQL92_INTERVAL_MONTH                  16
::constant SQL_DL_SQL92_INTERVAL_DAY                    32
::constant SQL_DL_SQL92_INTERVAL_HOUR                   64
::constant SQL_DL_SQL92_INTERVAL_MINUTE                 128
::constant SQL_DL_SQL92_INTERVAL_SECOND                 256
::constant SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH          512
::constant SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR            1024
::constant SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE          2048
::constant SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND          4096
::constant SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE         8192
::constant SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND         16384
::constant SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND       65536
::constant SQL_UNSPECIFIED                              0
::constant SQL_INSENSITIVE                              1
::constant SQL_SENSITIVE                                2
::constant SQL_DI_CREATE_INDEX                          1
::constant SQL_DI_DROP_INDEX                            2
::constant SQL_TXN_READ_UNCOMMITTED                     1
::constant SQL_TRANSACTION_READ_UNCOMMITTED             1
::constant SQL_TXN_READ_COMMITTED                       2
::constant SQL_TRANSACTION_READ_COMMITTED               2
::constant SQL_TXN_REPEATABLE_READ                      4
::constant SQL_TRANSACTION_REPEATABLE_READ              4
::constant SQL_TXN_SERIALIZABLE                         8
::constant SQL_TRANSACTION_SERIALIZABLE                 8
::constant SQL_DA_DROP_ASSERTION                        1
::constant SQL_DCS_DROP_CHARACTER_SET                   1
::constant SQL_DC_DROP_COLLATION                        1
::constant SQL_DD_DROP_DOMAIN                           1
::constant SQL_DD_RESTRICT                              2
::constant SQL_DD_CASCADE                               4
::constant SQL_DT_DROP_TABLE                            1
::constant SQL_DT_RESTRICT                              2
::constant SQL_DT_CASCADE                               4
::constant SQL_DTR_DROP_TRANSLATION                     1
::constant SQL_DV_DROP_VIEW                             1
::constant SQL_DV_RESTRICT                              2
::constant SQL_DV_CASCADE                               4
::constant SQL_CA1_NEXT                                 1
::constant SQL_CA1_ABSOLUTE                             2
::constant SQL_CA1_RELATIVE                             4
::constant SQL_CA1_BOOKMARK                             8
::constant SQL_CA1_LOCK_NO_CHANGE                       64
::constant SQL_CA1_LOCK_EXCLUSIVE                       128
::constant SQL_CA1_LOCK_UNLOCK                          256
::constant SQL_CA1_POS_POSITION                         512
::constant SQL_CA1_POS_UPDATE                           1024
::constant SQL_CA1_POS_DELETE                           2048
::constant SQL_CA1_POS_REFRESH                          4096
::constant SQL_CA1_POSITIONED_UPDATE                    8192
::constant SQL_CA1_POSITIONED_DELETE                    16384
::constant SQL_CA1_SELECT_FOR_UPDATE                    32768
::constant SQL_CA1_BULK_ADD                             65536
::constant SQL_CA1_BULK_UPDATE_BY_BOOKMARK              131072
::constant SQL_CA1_BULK_DELETE_BY_BOOKMARK              262144
::constant SQL_CA1_BULK_FETCH_BY_BOOKMARK               524288
::constant SQL_CA2_READ_ONLY_CONCURRENCY                1
::constant SQL_CA2_LOCK_CONCURRENCY                     2
::constant SQL_CA2_OPT_ROWVER_CONCURRENCY               4
::constant SQL_CA2_OPT_VALUES_CONCURRENCY               8
::constant SQL_CA2_SENSITIVITY_ADDITIONS                16
::constant SQL_CA2_SENSITIVITY_DELETIONS                32
::constant SQL_CA2_SENSITIVITY_UPDATES                  64
::constant SQL_CA2_MAX_ROWS_SELECT                      128
::constant SQL_CA2_MAX_ROWS_INSERT                      256
::constant SQL_CA2_MAX_ROWS_DELETE                      512
::constant SQL_CA2_MAX_ROWS_UPDATE                      1024
::constant SQL_CA2_MAX_ROWS_CATALOG                     2048
::constant SQL_CA2_MAX_ROWS_AFFECTS_ALL                 3968
::constant SQL_CA2_CRC_EXACT                            4096
::constant SQL_CA2_CRC_APPROXIMATE                      8192
::constant SQL_CA2_SIMULATE_NON_UNIQUE                  16384
::constant SQL_CA2_SIMULATE_TRY_UNIQUE                  32768
::constant SQL_CA2_SIMULATE_UNIQUE                      65536
::constant SQL_FILE_NOT_SUPPORTED                       0
::constant SQL_FILE_TABLE                               1
::constant SQL_FILE_QUALIFIER                           2
::constant SQL_FILE_CATALOG                             2
::constant SQL_GD_ANY_COLUMN                            1
::constant SQL_GD_ANY_ORDER                             2
::constant SQL_GD_BLOCK                                 4
::constant SQL_GD_BOUND                                 8
::constant SQL_GB_NOT_SUPPORTED                         0
::constant SQL_GB_GROUP_BY_EQUALS_SELECT                1
::constant SQL_GB_GROUP_BY_CONTAINS_SELECT              2
::constant SQL_GB_NO_RELATION                           3
::constant SQL_GB_COLLATE                               4
::constant SQL_IC_UPPER                                 1
::constant SQL_IC_LOWER                                 2
::constant SQL_IC_SENSITIVE                             3
::constant SQL_IC_MIXED                                 4
::constant SQL_IK_NONE                                  0
::constant SQL_IK_ASC                                   1
::constant SQL_IK_DESC                                  2
::constant SQL_IK_ALL                                   3
::constant SQL_ISV_ASSERTIONS                           1
::constant SQL_ISV_CHARACTER_SETS                       2
::constant SQL_ISV_CHECK_CONSTRAINTS                    4
::constant SQL_ISV_COLLATIONS                           8
::constant SQL_ISV_COLUMN_DOMAIN_USAGE                  16
::constant SQL_ISV_COLUMN_PRIVILEGES                    32
::constant SQL_ISV_COLUMNS                              64
::constant SQL_ISV_CONSTRAINT_COLUMN_USAGE              128
::constant SQL_ISV_CONSTRAINT_TABLE_USAGE               256
::constant SQL_ISV_DOMAIN_CONSTRAINTS                   512
::constant SQL_ISV_DOMAINS                              1024
::constant SQL_ISV_KEY_COLUMN_USAGE                     2048
::constant SQL_ISV_REFERENTIAL_CONSTRAINTS              4096
::constant SQL_ISV_SCHEMATA                             8192
::constant SQL_ISV_SQL_LANGUAGES                        16384
::constant SQL_ISV_TABLE_CONSTRAINTS                    32768
::constant SQL_ISV_TABLE_PRIVILEGES                     65536
::constant SQL_ISV_TABLES                               131072
::constant SQL_ISV_TRANSLATIONS                         262144
::constant SQL_ISV_USAGE_PRIVILEGES                     524288
::constant SQL_ISV_VIEW_COLUMN_USAGE                    1048576
::constant SQL_ISV_VIEW_TABLE_USAGE                     2097152
::constant SQL_ISV_VIEWS                                4194304
::constant SQL_IS_INSERT_LITERALS                       1
::constant SQL_IS_INSERT_SEARCHED                       2
::constant SQL_IS_SELECT_INTO                           4
::constant SQL_NNC_NULL                                 0
::constant SQL_NNC_NON_NULL                             1
::constant SQL_NC_START                                 2
::constant SQL_NC_END                                   4
::constant SQL_FN_STR_CONCAT                            1
::constant SQL_FN_STR_INSERT                            2
::constant SQL_FN_STR_LEFT                              4
::constant SQL_FN_STR_LTRIM                             8
::constant SQL_FN_STR_LENGTH                            16
::constant SQL_FN_STR_LOCATE                            32
::constant SQL_FN_STR_LCASE                             64
::constant SQL_FN_STR_REPEAT                            128
::constant SQL_FN_STR_REPLACE                           256
::constant SQL_FN_STR_RIGHT                             512
::constant SQL_FN_STR_RTRIM                             1024
::constant SQL_FN_STR_SUBSTRING                         2048
::constant SQL_FN_STR_UCASE                             4096
::constant SQL_FN_STR_ASCII                             8192
::constant SQL_FN_STR_CHAR                              16384
::constant SQL_FN_STR_DIFFERENCE                        32768
::constant SQL_FN_STR_LOCATE_2                          65536
::constant SQL_FN_STR_SOUNDEX                           131072
::constant SQL_FN_STR_SPACE                             262144
::constant SQL_FN_STR_BIT_LENGTH                        524288
::constant SQL_FN_STR_CHAR_LENGTH                       1048576
::constant SQL_FN_STR_CHARACTER_LENGTH                  2097152
::constant SQL_FN_STR_OCTET_LENGTH                      4194304
::constant SQL_FN_STR_POSITION                          8388608
::constant SQL_FN_NUM_ABS                               1
::constant SQL_FN_NUM_ACOS                              2
::constant SQL_FN_NUM_ASIN                              4
::constant SQL_FN_NUM_ATAN                              8
::constant SQL_FN_NUM_ATAN2                             16
::constant SQL_FN_NUM_CEILING                           32
::constant SQL_FN_NUM_COS                               64
::constant SQL_FN_NUM_COT                               128
::constant SQL_FN_NUM_EXP                               256
::constant SQL_FN_NUM_FLOOR                             512
::constant SQL_FN_NUM_LOG                               1024
::constant SQL_FN_NUM_MOD                               2048
::constant SQL_FN_NUM_SIGN                              4096
::constant SQL_FN_NUM_SIN                               8192
::constant SQL_FN_NUM_SQRT                              16384
::constant SQL_FN_NUM_TAN                               32768
::constant SQL_FN_NUM_PI                                65536
::constant SQL_FN_NUM_RAND                              131072
::constant SQL_FN_NUM_DEGREES                           262144
::constant SQL_FN_NUM_LOG10                             524288
::constant SQL_FN_NUM_POWER                             1048576
::constant SQL_FN_NUM_RADIANS                           2097152
::constant SQL_FN_NUM_ROUND                             4194304
::constant SQL_FN_NUM_TRUNCATE                          8388608
::constant SQL_FN_TD_NOW                                1
::constant SQL_FN_TD_CURDATE                            2
::constant SQL_FN_TD_DAYOFMONTH                         4
::constant SQL_FN_TD_DAYOFWEEK                          8
::constant SQL_FN_TD_DAYOFYEAR                          16
::constant SQL_FN_TD_MONTH                              32
::constant SQL_FN_TD_QUARTER                            64
::constant SQL_FN_TD_WEEK                               128
::constant SQL_FN_TD_YEAR                               256
::constant SQL_FN_TD_CURTIME                            512
::constant SQL_FN_TD_HOUR                               1024
::constant SQL_FN_TD_MINUTE                             2048
::constant SQL_FN_TD_SECOND                             4096
::constant SQL_FN_TD_TIMESTAMPADD                       8192
::constant SQL_FN_TD_TIMESTAMPDIFF                      16384
::constant SQL_FN_TD_DAYNAME                            32768
::constant SQL_FN_TD_MONTHNAME                          65536
::constant SQL_FN_TD_CURRENT_DATE                       131072
::constant SQL_FN_TD_CURRENT_TIME                       262144
::constant SQL_FN_TD_CURRENT_TIMESTAMP                  524288
::constant SQL_FN_TD_EXTRACT                            1048576
::constant SQL_FN_SYS_USERNAME                          1
::constant SQL_FN_SYS_DBNAME                            2
::constant SQL_FN_SYS_IFNULL                            4
::constant SQL_FN_TSI_FRAC_SECOND                       1        
::constant SQL_FN_TSI_SECOND                            2
::constant SQL_FN_TSI_MINUTE                            4
::constant SQL_FN_TSI_HOUR                              8
::constant SQL_FN_TSI_DAY                               16
::constant SQL_FN_TSI_WEEK                              32
::constant SQL_FN_TSI_MONTH                             64
::constant SQL_FN_TSI_QUARTER                           128
::constant SQL_FN_TSI_YEAR                              256
::constant SQL_OIC_CORE                                 1
::constant SQL_OIC_LEVEL1                               2
::constant SQL_OIC_LEVEL2                               3
::constant SQL_OJ_CAPABILITIES                          115
::constant SQL_OJ_LEFT                                  1
::constant SQL_OJ_RIGHT                                 2
::constant SQL_OJ_FULL                                  4
::constant SQL_OJ_NESTED                                8
::constant SQL_OJ_NOT_ORDERED                           16
::constant SQL_OJ_INNER                                 32
::constant SQL_OJ_ALL_COMPARISON_OPS                    64
::constant SQL_PARC_BATCH                               1
::constant SQL_PARC_NO_BATCH                            2
::constant SQL_PAS_BATCH                                1
::constant SQL_PAS_NO_BATCH                             2       
::constant SQL_PAS_NO_SELECT                            3
::constant SQL_POS_POSITION                             1
::constant SQL_POS_REFRESH                              2
::constant SQL_POS_UPDATE                               4
::constant SQL_POS_DELETE                               8
::constant SQL_POS_ADD                                  16
::constant SQL_SU_DML_STATEMENTS                        1
::constant SQL_SU_PROCEDURE_INVOCATION                  2
::constant SQL_SU_TABLE_DEFINITION                      4
::constant SQL_SU_INDEX_DEFINITION                      8
::constant SQL_SU_PRIVILEGE_DEFINITION                  16
::constant SQL_SO_FORWARD_ONLY                          1
::constant SQL_SO_KEYSET_DRIVEN                         2
::constant SQL_SO_DYNAMIC                               4
::constant SQL_SO_MIXED                                 8
::constant SQL_SO_STATIC                                16
::constant SQL_SC_SQL92_ENTRY                           1
::constant SQL_SC_FIPS127_2_TRANSITIONAL                2
::constant SQL_SC_SQL92_INTERMEDIATE                    4 
::constant SQL_SC_SQL92_FULL                            8
::constant SQL_SDF_CURRENT_DATE                         1
::constant SQL_SDF_CURRENT_TIME                         2
::constant SQL_SDF_CURRENT_TIMESTAMP                    4
::constant SQL_SFKD_CASCADE                             1
::constant SQL_SFKD_NO_ACTION                           2
::constant SQL_SFKD_SET_DEFAULT                         4
::constant SQL_SFKD_SET_NULL                            8
::constant SQL_SFKU_CASCADE                             1
::constant SQL_SFKU_NO_ACTION                           2
::constant SQL_SFKU_SET_DEFAULT                         4
::constant SQL_SFKU_SET_NULL                            8
::constant SQL_SG_USAGE_ON_DOMAIN                       1
::constant SQL_SG_USAGE_ON_CHARACTER_SET                2
::constant SQL_SG_USAGE_ON_COLLATION                    4
::constant SQL_SG_USAGE_ON_TRANSLATION                  8
::constant SQL_SG_WITH_GRANT_OPTION                     16
::constant SQL_SG_DELETE_TABLE                          32
::constant SQL_SG_INSERT_TABLE                          64
::constant SQL_SG_INSERT_COLUMN                         128
::constant SQL_SG_REFERENCES_TABLE                      256
::constant SQL_SG_REFERENCES_COLUMN                     512
::constant SQL_SG_SELECT_TABLE                          1024
::constant SQL_SG_UPDATE_TABLE                          2048
::constant SQL_SG_UPDATE_COLUMN                         4096
::constant SQL_SNVF_BIT_LENGTH                          1
::constant SQL_SNVF_CHAR_LENGTH                         2
::constant SQL_SNVF_CHARACTER_LENGTH                    4
::constant SQL_SNVF_EXTRACT                             8
::constant SQL_SNVF_OCTET_LENGTH                        16
::constant SQL_SNVF_POSITION                            32
::constant SQL_SP_EXISTS                                1
::constant SQL_SP_ISNOTNULL                             2
::constant SQL_SP_ISNULL                                4
::constant SQL_SP_MATCH_FULL                            8
::constant SQL_SP_MATCH_PARTIAL                         16
::constant SQL_SP_MATCH_UNIQUE_FULL                     32
::constant SQL_SP_MATCH_UNIQUE_PARTIAL                  64
::constant SQL_SP_OVERLAPS                              128
::constant SQL_SP_UNIQUE                                256
::constant SQL_SP_LIKE                                  512
::constant SQL_SP_IN                                    1024
::constant SQL_SP_BETWEEN                               2048
::constant SQL_SP_COMPARISON                            4096
::constant SQL_SP_QUANTIFIED_COMPARISON                 8192
::constant SQL_SRJO_CORRESPONDING_CLAUSE                1
::constant SQL_SRJO_CROSS_JOIN                          2
::constant SQL_SRJO_EXCEPT_JOIN                         4
::constant SQL_SRJO_FULL_OUTER_JOIN                     8
::constant SQL_SRJO_INNER_JOIN                          16
::constant SQL_SRJO_INTERSECT_JOIN                      32
::constant SQL_SRJO_LEFT_OUTER_JOIN                     64
::constant SQL_SRJO_NATURAL_JOIN                        128
::constant SQL_SRJO_RIGHT_OUTER_JOIN                    256
::constant SQL_SRJO_UNION_JOIN                          512
::constant SQL_SR_USAGE_ON_DOMAIN                       1
::constant SQL_SR_USAGE_ON_CHARACTER_SET                2
::constant SQL_SR_USAGE_ON_COLLATION                    4
::constant SQL_SR_USAGE_ON_TRANSLATION                  8
::constant SQL_SR_GRANT_OPTION_FOR                      16
::constant SQL_SR_CASCADE                               32
::constant SQL_SR_RESTRICT                              64
::constant SQL_SR_DELETE_TABLE                          128
::constant SQL_SR_INSERT_TABLE                          256
::constant SQL_SR_INSERT_COLUMN                         512
::constant SQL_SR_REFERENCES_TABLE                      1024
::constant SQL_SR_REFERENCES_COLUMN                     2048
::constant SQL_SR_SELECT_TABLE                          4096
::constant SQL_SR_UPDATE_TABLE                          8192
::constant SQL_SR_UPDATE_COLUMN                         16384
::constant SQL_SRVC_VALUE_EXPRESSION                    1
::constant SQL_SRVC_NULL                                2
::constant SQL_SRVC_DEFAULT                             4
::constant SQL_SRVC_ROW_SUBQUERY                        8
::constant SQL_SSF_CONVERT                              1
::constant SQL_SSF_LOWER                                2
::constant SQL_SSF_UPPER                                4
::constant SQL_SSF_SUBSTRING                            8
::constant SQL_SSF_TRANSLATE                            16
::constant SQL_SSF_TRIM_BOTH                            32
::constant SQL_SSF_TRIM_LEADING                         64
::constant SQL_SSF_TRIM_TRAILING                        128
::constant SQL_SVE_CASE                                 1
::constant SQL_SVE_CAST                                 2
::constant SQL_SVE_COALESCE                             4
::constant SQL_SVE_NULLIF                               8
::constant SQL_SCC_XOPEN_CLI_VERSION1                   1
::constant SQL_SCC_ISO92_CLI                            2
::constant SQL_SQ_COMPARISON                            1
::constant SQL_SQ_EXISTS                                2
::constant SQL_SQ_IN                                    4
::constant SQL_SQ_QUANTIFIED                            8
::constant SQL_SQ_CORRELATED_SUBQUERIES                 16
::constant SQL_TC_NONE                                  0
::constant SQL_TC_DML                                   1
::constant SQL_TC_ALL                                   2
::constant SQL_TC_DDL_COMMIT                            3
::constant SQL_TC_DDL_IGNORE                            4
::constant SQL_U_UNION                                  1
::constant SQL_U_UNION_ALL                              2


/*----------------------------------------------------------------------------*/
/* Class: dbconnx                                                             */
/*----------------------------------------------------------------------------*/

::class dbconnx public subclass database

::attribute SQLRETURN

::method init EXTERNAL "Library orxsql OrxDB_dbinit"
::method get_env_attr EXTERNAL "Library orxsql OrxDB_get_env_attr"
::method set_env_attr EXTERNAL "Library orxsql OrxDB_set_env_attr"
::method connect EXTERNAL "Library orxsql OrxDB_connect"
::method disconnect EXTERNAL "Library orxsql OrxDB_close"
::method get_connect_attr EXTERNAL "Library orxsql OrxDB_get_connx_attr"
::method set_connect_attr EXTERNAL "Library orxsql OrxDB_set_connx_attr"
::method commit EXTERNAL "Library orxsql OrxDB_commit"
::method rollback EXTERNAL "Library orxsql OrxDB_rollback"
::method isConnected EXTERNAL "Library orxsql OrxDB_check_connx"
::method get_info EXTERNAL "Library orxsql OrxDB_get_info"
::method destroy EXTERNAL "Library orxsql OrxDB_destroy"
::method version class EXTERNAL "Library orxsql OrxDB_version"
::method isBitSet class EXTERNAL "Library orxsql OrxDB_isbitset"


/*----------------------------------------------------------------------------*/
/* Class: SQLstatement                                                        */
/*----------------------------------------------------------------------------*/

::class SQLstatement public subclass database

::method init EXTERNAL "Library orxsql OrxDB_stmtinit"
::method destroy EXTERNAL "Library orxsql OrxDB_stmtdestroy"
::method isBitSet class EXTERNAL "Library orxsql OrxDB_isbitset"


-- ::method prepare EXTERNAL "Library orxsql OrxDB_prepare"
-- ::method execute EXTERNAL "Library orxsql OrxDB_dbexecute"
-- ::method destroy EXTERNAL "Library orxsql OrxDB_destroy"
-- ::method execute EXTERNAL "Library orxsql OrxDB_stmtexecute"
-- ::method fetch EXTERNAL "Library orxsql OrxDB_fetch"
-- ::method describe_col EXTERNAL "Library orxsql OrxDB_describe_col"
-- ::method get_col_data EXTERNAL "Library orxsql OrxDB_get_col_data"
-- ::method get_attr EXTERNAL "Library orxsql OrxDB_get_stmt_attr"
-- ::method set_attr EXTERNAL "Library orxsql OrxDB_set_stmt_attr"

