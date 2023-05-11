/*
 * Copyright (c) 2001-2012 Mark Hessling <mark@rexx.org> All rights reserved.
 *
 * This program and the accompanying materials are made available under
 * the terms of the Common Public License v1.0 which accompanies this
 * distribution. A copy is also available at the following address:
 * http://www.opensource.org/licenses/cpl1.0.php
 */
/*
 * For diffs between libcurl versions:
 * http://upstream-tracker.org/versions/libcurl.html
 */
/*
 * Not implemented:
 *   setopt()
 *      CURLOPT_SEEKFUNCTION - 7.18.0
 *      CURLOPT_SEEKDATA - 7.18.0
 *      CURLOPT_SSH_KEYFUNCTION - 7.19.6
 *      CURLOPT_SSH_KEYDATA - 7.19.6
 *      CURLOPT_INTERLEAVEFUNCTION - 7.20.0
 *      CURLOPT_INTERLEAVEDATA - 7.20.0
 *      CURLOPT_CLOSESOCKETFUNCTION - 7.21.7
 *      CURLOPT_CLOSESOCKETDATA - 7.21.7
 *   curl_easy_pause() not implemented
 *
 *   getopt()
 *      CURLINFO_CERTINFO - 7.19.1
 */
/*
 * CURLOPT options to add:
 * 7.33.0 XOAUTH2_BEARER                x      x
 *        DNS_INTERFACE
 *        DNS_LOCAL_IP4
 *        DNS_LOCAL_IP6
 * 7.34.0 LOGIN_OPTIONS                 x      x
 * 7.36.0 SSL_ENABLE_NPN
 *        SSL_ENABLE_ALPN
 *        EXPECT_100_TIMEOUT_MS
 * 7.37.0 PROXYHEADER
 *        HEADEROPT
 * 7.39.0 PINNEDPUBLICKEY
 * 7.40.0 UNIX_SOCKET_PATH
 * 7.41.0 SSL_VERIFYSTATUS              x      x
 */

/*
 * Error handling:
 *                              error_rexxcurl       error_curl
 *
 * ------------------------------------------------------------
 * wrong # params           ******* syntax error **********
 * internal error                   set                 N/A
 * cURL runtime errors              -1                  set
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# include "defines.h"
#endif

#if defined(__EMX__) && defined(USE_OS2REXX)
# define INCL_DOS
# define INCL_WIN
# define INCL_GPI
# include "rxpack.h"
# include "rexxcurl.h"
#else
# include "rexxcurl.h"
# include "rxpack.h"
#endif

char *RxPackageName = "rexxcurl";

#define LIBCURL_SUPPORTED_VERSION LIBCURL_VERSION_NUM

#define API_REXXCALLBACK_PRESENT 1
#define API_REXXCALLBACK_MISSING 0

#if defined(WIN32)
HMODULE handle=NULL;

typedef CURL * T_CURL_EASY_INIT(void);
T_CURL_EASY_INIT *p_curl_easy_init;

typedef CURLcode T_CURL_EASY_SETOPT(CURL *curl, CURLoption option, ...);
T_CURL_EASY_SETOPT *p_curl_easy_setopt;

typedef void T_CURL_EASY_RESET(CURL *curl );
T_CURL_EASY_RESET *p_curl_easy_reset;

typedef CURLcode T_CURL_EASY_PERFORM(CURL *curl);
T_CURL_EASY_PERFORM *p_curl_easy_perform;

typedef void T_CURL_EASY_CLEANUP(CURL *curl);
T_CURL_EASY_CLEANUP *p_curl_easy_cleanup;

typedef CURLcode T_CURL_EASY_GETINFO(CURL *curl, CURLINFO info, ...);
T_CURL_EASY_GETINFO *p_curl_easy_getinfo;

typedef CURLFORMcode T_CURL_FORMADD(struct curl_httppost **httppost, struct curl_httppost **last_post, ...);
T_CURL_FORMADD *p_curl_formadd;

typedef void T_CURL_FORMFREE(struct curl_httppost *form);
T_CURL_FORMFREE *p_curl_formfree;

typedef void T_CURL_GLOBAL_CLEANUP(void);
T_CURL_GLOBAL_CLEANUP *p_curl_global_cleanup;

typedef void T_CURL_GLOBAL_INIT(long flags);
T_CURL_GLOBAL_INIT *p_curl_global_init;

typedef struct curl_slist *T_CURL_SLIST_APPEND(struct curl_slist *, const char *);
T_CURL_SLIST_APPEND *p_curl_slist_append;

typedef void T_CURL_SLIST_FREE_ALL(struct curl_slist *);
T_CURL_SLIST_FREE_ALL *p_curl_slist_free_all;

typedef char *T_CURL_VERSION(void);
T_CURL_VERSION *p_curl_version;

typedef curl_version_info_data *T_CURL_VERSION_INFO(CURLversion);
T_CURL_VERSION_INFO *p_curl_version_info;

typedef char *T_CURL_EASY_ESCAPE(CURL *curl, char *url, int length);
T_CURL_EASY_ESCAPE *p_curl_easy_escape;

typedef void T_CURL_FREE(void *ptr);
T_CURL_FREE *p_curl_free;

typedef char *T_CURL_EASY_UNESCAPE(CURL *curl, char *url, int inlength, int *outlength);
T_CURL_EASY_UNESCAPE *p_curl_easy_unescape;

# define CURL_EASY_INIT ((*p_curl_easy_init))
# define CURL_EASY_SETOPT ((*p_curl_easy_setopt))
# define CURL_EASY_RESET ((*p_curl_easy_reset))
# define CURL_EASY_PERFORM ((*p_curl_easy_perform))
# define CURL_EASY_CLEANUP ((*p_curl_easy_cleanup))
# define CURL_EASY_GETINFO ((*p_curl_easy_getinfo))
# define CURL_FORMADD ((*p_curl_formadd))
# define CURL_FORMFREE ((*p_curl_formfree))
# define CURL_GLOBAL_INIT ((*p_curl_global_init))
# define CURL_GLOBAL_CLEANUP ((*p_curl_global_cleanup))
# define CURL_SLIST_APPEND ((*p_curl_slist_append))
# define CURL_SLIST_FREE_ALL ((*p_curl_slist_free_all))
# define CURL_VERSION ((*p_curl_version))
# define CURL_VERSION_INFO ((*p_curl_version_info))
# define CURL_EASY_ESCAPE ((*p_curl_easy_escape))
# define CURL_EASY_UNESCAPE ((*p_curl_easy_unescape))
# define CURL_FREE ((*p_curl_free))
#else
# define CURL_EASY_INIT curl_easy_init
# define CURL_EASY_SETOPT curl_easy_setopt
# define CURL_EASY_RESET curl_easy_reset
# define CURL_EASY_PERFORM curl_easy_perform
# define CURL_EASY_CLEANUP curl_easy_cleanup
# define CURL_EASY_GETINFO curl_easy_getinfo
# define CURL_FORMADD curl_formadd
# define CURL_FORMFREE curl_formfree
# define CURL_GLOBAL_INIT curl_global_init
# define CURL_GLOBAL_CLEANUP curl_global_cleanup
# define CURL_SLIST_APPEND curl_slist_append
# define CURL_SLIST_FREE_ALL curl_slist_free_all
# define CURL_VERSION curl_version
# define CURL_VERSION_INFO curl_version_info
# define CURL_EASY_ESCAPE curl_easy_escape
# define CURL_EASY_UNESCAPE curl_easy_unescape
# define CURL_FREE curl_free
#endif

#define INTERR_CURL_ERROR            1
#define INTERR_CURL_ERROR_STRING     "Error from cURL"
#define INTERR_INVALID_NUMBER        2
#define INTERR_INVALID_NUMBER_STRING "Invalid Number"
#define INTERR_INVALID_OPTION        3
#define INTERR_INVALID_OPTION_STRING "Invalid Option"
#define INTERR_NO_MEMORY             4
#define INTERR_NO_MEMORY_STRING      "Out of memory"
#define INTERR_INVALID_HANDLE        5
#define INTERR_INVALID_HANDLE_STRING "Invalid cURL handle"
#define INTERR_INVALID_FILE          6
#define INTERR_INVALID_FILE_STRING   "Invalid filename"
#define INTERR_INVALID_BOOL          7
#define INTERR_INVALID_BOOL_STRING   "Invalid boolean"
#define INTERR_INVALID_STEM          8
#define INTERR_INVALID_STEM_STRING   "Expecting a stem as parameter"
#define INTERR_INVALID_VARIABLE      9
#define INTERR_INVALID_VARIABLE_STRING "Invalid variable:"
#define INTERR_READONLY_VARIABLE     10
#define INTERR_READONLY_VARIABLE_STRING "Cannot set readonly variable"
#define INTERR_TOO_FEW_ARGS          11
#define INTERR_TOO_FEW_ARGS_STRING   "Too few arguments supplied"
#define INTERR_MANDATORY_FIELD       12
#define INTERR_MANDATORY_FIELD_STRING   "Field must be specified"
#define INTERR_WRITING_TEMP_FILE     13
#define INTERR_WRITING_TEMP_FILE_STRING "Error writing to temporary file"
#define INTERR_STEM_VALUE     14
#define INTERR_STEM_VALUE_STRING "Stem values must be in form of name=value"

/*
 * The following are option types for setopt()
 * For those items that are not "basic" types, there is another
 * table below which contains the definitions of the sub-options
 */
#define RXCURLOPT_STRING        1  /* string */
#define RXCURLOPT_LIST          2  /* stem for slist */
#define RXCURLOPT_LONG          3  /* 32bit number */
#define RXCURLOPT_OUTFILE       4  /* output file */
#define RXCURLOPT_INFILE        5  /* input file - passed to cURL as FILE **/
#define RXCURLOPT_BOOL          6  /* bool */
#define RXCURLOPT_POLICY        7  /* policy */
#define RXCURLOPT_POST_DATA     8  /* stem for httppostdata */
#define RXCURLOPT_POST_FIELDS   9  /* stem for httppostfields */
#define RXCURLOPT_OUTSTEM      10  /* stem for outstem */
#define RXCURLOPT_HEADERSTEM   11  /* stem for headerstem */
#define RXCURLOPT_PROXYTYPE    12  /* proxytype */
#define RXCURLOPT_HTTP_VERSION 13  /* http_version */
#define RXCURLOPT_NETRC        14  /* netrc */
#define RXCURLOPT_TIMECOND     15  /* timecondition */
#define RXCURLOPT_IPRESOLVE    16  /* ipresolve */
#define RXCURLOPT_BITMAP       17  /* generic bitmap */
#define RXCURLOPT_BITMAP_AUTH  18  /* authorisation bitmap */
#define RXCURLOPT_LONGLONG     19  /* 64bit number */
#define RXCURLOPT_CALLBACK     20  /* libcurl callback */
#define RXCURLOPT_FTPSSL       21  /* ftp ssl */
#define RXCURLOPT_FTPSSLAUTH   22  /* ftp ssl auth */
#define RXCURLOPT_POST_FORM    23  /* option for processing after curl_formadd() calls */
#define RXCURLOPT_FTPSSLCCC    24  /* ftp ssl cc */
#define RXCURLOPT_INFILENAME   25  /* input file - validated as existing filename - passed to cURL as filename */
#define RXCURLOPT_BITMAP_SSHAUTH 26 /* SSH authorisation bitmap */
#define RXCURLOPT_FTP_CREATE_MISSING_DIRS  27 /* FTP create missing dirs */
#define RXCURLOPT_RTSP_REQUEST 28  /* RTSP Request options */
#define RXCURLOPT_PROTOCOLS    29  /* options for protocols */
#define RXCURLOPT_GSSAPI_DELEGATION 30  /* options for gssapi delegation */
#define RXCURLOPT_SSL_OPTIONS 31  /* options for SSL */
#define RXCURLOPT_BITMAP_REDIR 32  /* post redirection bitmap */
#define RXCURLOPT_BITMAP_TLSAUTH 33 /* TLS authorisation types */
#define RXCURLOPT_INSTEM        34 /* input stem - passed to cURL as FILE **/
#define RXCURLOPT_ERRFILE       35 /* error file */

#define RXCURLINFO_STRING       1  /* string */
#define RXCURLINFO_LONG         2  /* number */
#define RXCURLINFO_DOUBLE       3  /* double */
#define RXCURLINFO_LIST         4  /* stem for slist */
#define RXCURLINFO_CERTINFO     5  /* certinfo struct */
#define RXCURLINFO_BITMAP       6  /* bitmap input, word list output based on curlsetopt sub-options*/

static char *curl_errors[] =
{
   "OK",                      /* 0 */
   "UNSUPPORTED_PROTOCOL",    /* 1 */
   "FAILED_INIT",             /* 2 */
   "URL_MALFORMAT",           /* 3 */
#if LIBCURL_VERSION_NUM >= 0x071505
   "NOT_BUILT_IN",            /* 4 */
#else
   "URL_MALFORMAT_USER",      /* 4 */
#endif
   "COULDNT_RESOLVE_PROXY",   /* 5 */
   "COULDNT_RESOLVE_HOST",    /* 6 */
   "COULDNT_CONNECT",         /* 7 */
   "FTP_WEIRD_SERVER_REPLY",  /* 8 */
   "FTP_ACCESS_DENIED",       /* 9 */
#if LIBCURL_VERSION_NUM >= 0x071800
   "FTP_ACCEPT_FAILED",       /* 10 */
#else
   "FTP_USER_PASSWORD_INCORRECT", /* 10 */
#endif
   "FTP_WEIRD_PASS_REPLY",    /* 11 */
#if LIBCURL_VERSION_NUM >= 0x071800
   "FTP_ACCEPT_TIMEOUT",      /* 12 */
#else
   "FTP_WEIRD_USER_REPLY",    /* 12 */
#endif
   "FTP_WEIRD_PASV_REPLY",    /* 13 */
   "FTP_WEIRD_227_FORMAT",    /* 14 */
   "FTP_CANT_GET_HOST",       /* 15 */
   "FTP_CANT_RECONNECT",      /* 16 */
   "FTP_COULDNT_SET_BINARY",  /* 17 */
   "PARTIAL_FILE",            /* 18 */
   "FTP_COULDNT_RETR_FILE",   /* 19 */
   "FTP_WRITE_ERROR",         /* 20 */
   "FTP_QUOTE_ERROR",         /* 21 */
   "HTTP_NOT_FOUND",          /* 22 */
   "WRITE_ERROR",             /* 23 */
   "MALFORMAT_USER",          /* 24 - user name is illegally specified */
   "FTP_COULDNT_STOR_FILE",   /* 25 - failed FTP upload */
   "READ_ERROR",              /* 26 - could open/read from file */
   "OUT_OF_MEMORY",           /* 27 */
   "OPERATION_TIMEOUTED",     /* 28 - the timeout time was reached */
   "FTP_COULDNT_SET_ASCII",   /* 29 - TYPE A failed */
   "FTP_PORT_FAILED",         /* 30 - FTP PORT operation failed */
   "FTP_COULDNT_USE_REST",    /* 31 - the REST command failed */
   "FTP_COULDNT_GET_SIZE",    /* 32 - the SIZE command failed */
   "HTTP_RANGE_ERROR",        /* 33 - RANGE "command" didn"'t work */
   "HTTP_POST_ERROR",         /* 34 */
   "SSL_CONNECT_ERROR",       /* 35 - wrong when connecting with SSL */
   "FTP_BAD_DOWNLOAD_RESUME", /* 36 - couldn"'t resume download */
   "FILE_COULDNT_READ_FILE",  /* 37 */
   "LDAP_CANNOT_BIND",        /* 38 */
   "LDAP_SEARCH_FAILED",      /* 39 */
   "LIBRARY_NOT_FOUND",       /* 40 */
   "FUNCTION_NOT_FOUND",      /* 41 */
   "ABORTED_BY_CALLBACK",     /* 42 */
   "BAD_FUNCTION_ARGUMENT",   /* 43 */
   "BAD_CALLING_ORDER",       /* 44 */
   "HTTP_PORT_FAILED",        /* 45 - HTTP Interface operation failed */
   "BAD_PASSWORD_ENTERED",    /* 46 - my_getpass() returns fail */
   "TOO_MANY_REDIRECTS ",     /* 47 - catch endless re-direct loops */
#if LIBCURL_VERSION_NUM >= 0x071505
   "UNKNOWN_OPTION",          /* 48 - User specified an unknown option */
#else
   "UNKNOWN_TELNET_OPTION",   /* 48 - User specified an unknown option */
#endif
   "TELNET_OPTION_SYNTAX ",   /* 49 - Malformed telnet option */
   "OBSOLETE",                /* 50 - removed after 7.7.3 */
   "SSL_PEER_CERTIFICATE",    /* 51 - peer"'s certificate wasn't ok */
   "GOT_NOTHING",             /* 52 - when this is a specific error */
   "SSL_ENGINE_NOTFOUND",     /* 53 - SSL crypto engine not found */
   "SSL_ENGINE_SETFAILED",    /* 54 - can not set SSL crypto engine as default */
   "SEND_ERROR",              /* 55 - failed sending network data */
   "RECV_ERROR",              /* 56 - failure in receiving network data */
   "SHARE_IN_USE",            /* 57 - share is in use */
   "SSL_CERTPROBLEM",         /* 58 - problem with the local certificate */
   "SSL_CIPHER",              /* 59 - couldn't use specified cipher */
   "SSL_CACERT",              /* 60 - problem with the CA cert (path?) */
   "BAD_CONTENT_ENCODING",    /* 61 - Unrecognized transfer encoding */
   "LDAP_INVALID_URL",        /* 62 - Invalid LDAP URL */
   "FILESIZE_EXCEEDED",       /* 63 - Maximum file size exceeded */
   "USE_SSL_FAILED",          /* 64 - Requested USE SSL level failed */
   "SEND_FAIL_REWIND",        /* 65 - When doing a send operation curl had to rewind the data to retransmit, but the rewinding operation failed */
   "SSL_ENGINE_INITFAILED",   /* 66 - Initiating the SSL Engine failed */
   "LOGIN_DENIED",            /* 67 - The remote server denied curl to login (Added in 7.13.1) */
   "TFTP_NOTFOUND",           /* 68 - file not found on server */
   "TFTP_PERM",               /* 69 - permission problem on server */
   "TFTP_DISKFULL",           /* 70 - out of disk space on server */
   "TFTP_ILLEGAL",            /* 71 - Illegal TFTP operation */
   "TFTP_UNKNOWNID",          /* 72 - Unknown transfer ID */
   "TFTP_EXISTS",             /* 73 - File already exists */
   "TFTP_NOSUCHUSER",         /* 74 - No such user */
   "CONV_FAILED",             /* 75 - conversion failed */
   "CONV_REQD",               /* 76 - caller must register conversion callbacks using curl_easy_setopt options CURLOPT_CONV_FROM_NETWORK_FUNCTION,CURLOPT_CONV_TO_NETWORK_FUNCTION, and CURLOPT_CONV_FROM_UTF8_FUNCTION */
   "SSL_CACERT_BADFILE",      /* 77 - could not load CACERT file, missing or wrong format */
   "REMOTE_FILE_NOT_FOUND",   /* 78 - remote file not found */
   "SSH",                     /* 79 - error from the SSH layer, somewhat generic so the error message will be of interest when this has happened */
   "SSL_SHUTDOWN_FAILED",     /* 80 - Failed to shut down the SSL */
#if LIBCURL_VERSION_NUM >= 0x071202
   "AGAIN",                   /* 81 - socket is not ready for send/recv, wait till it's ready and try again (Added in 7.18.2) */
#endif
#if LIBCURL_VERSION_NUM >= 0x071300
   "SSL_CRL_BADFILE",         /* 82 - could not load CRL file, missing or wrong format (Added in 7.19.0) */
   "SSL_ISSUER_ERROR",        /* 83 - Issuer check failed.  (Added in 7.19.0) */
#endif
#if LIBCURL_VERSION_NUM >= 0x071400
   "FTP_PRET_FAILED",         /* 84 - a PRET command failed */
   "RTSP_CSEQ_ERROR",         /* 85 - mismatch of RTSP CSeq numbers */
   "RTSP_SESSION_ERROR",      /* 86 - mismatch of RTSP Session Identifiers */
#endif
#if LIBCURL_VERSION_NUM >= 0x071500
   "FTP_BAD_FILE_LIST",       /* 87 - unable to parse FTP file list */
   "CHUNK_FAILED",            /* 88 - chunk callback reported error */
#endif
   NULL
};
static char *curl_formadd_errors[] =
{
   "OK",                  /* 0 */
   "MEMORY",              /* 1 */
   "OPTION_TWICE",        /* 2 */
   "NULL",                /* 3 */
   "UNKNOWN_OPTION",      /* 4 */
   "INCOMPLETE",          /* 5 */
   "ILLEGAL_ARRAY",       /* 6 */
   "DISABLED",            /* 7 */
};

typedef struct
{
   char *name;
   unsigned long number;
   int optiontype; /* things like STEM, STRING, INT */
   char *newname;  /* name of new value if this setting is deprecated, NULL means it is still valid */
} curl_options;

static curl_options RexxCurlOptions[] =
{
#if LIBCURL_VERSION_NUM >= 0x071506
   { "ACCEPTENCODING"  ,CURLOPT_ACCEPT_ENCODING ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071800
   { "ACCEPTTIMEOUTMS" ,CURLOPT_ACCEPTTIMEOUT_MS,RXCURLOPT_LONG        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071300
   { "ADDRESSSCOPE"    ,CURLOPT_ADDRESS_SCOPE   ,RXCURLOPT_LONG        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071100
   { "APPEND"          ,CURLOPT_APPEND          ,RXCURLOPT_BOOL        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "BUFFERSIZE"      ,CURLOPT_BUFFERSIZE      ,RXCURLOPT_LONG        , NULL },
#endif
   { "CAINFO"          ,CURLOPT_CAINFO          ,RXCURLOPT_INFILENAME  , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "CAPATH"          ,CURLOPT_CAPATH          ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071301
   { "CERTINFO"        ,CURLOPT_CERTINFO        ,RXCURLOPT_LONG        , NULL },
#endif
#if LIBCURL_VERSION_NUM < 0x072600
   { "CLOSEPOLICY"     ,CURLOPT_CLOSEPOLICY     ,RXCURLOPT_POLICY      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070f02
   { "CONNECTONLY"     ,CURLOPT_CONNECT_ONLY    ,RXCURLOPT_LONG        , NULL },
#endif
   { "CONNECTTIMEOUT"  ,CURLOPT_CONNECTTIMEOUT  ,RXCURLOPT_LONG        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071002
   { "CONNECTTIMEOUTMS",CURLOPT_CONNECTTIMEOUT_MS ,RXCURLOPT_LONG      , NULL },
#endif
   { "COOKIE"          ,CURLOPT_COOKIE          ,RXCURLOPT_STRING      , NULL },
   { "COOKIEFILE"      ,CURLOPT_COOKIEFILE      ,RXCURLOPT_INFILENAME  , NULL },
#if LIBCURL_VERSION_NUM >= 0x070900
   { "COOKIEJAR"       ,CURLOPT_COOKIEJAR       ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070e01
   { "COOKIELIST"      ,CURLOPT_COOKIELIST      ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "COOKIESESSION"   ,CURLOPT_COOKIESESSION   ,RXCURLOPT_BOOL        , NULL },
#endif
   { "CRLF"            ,CURLOPT_CRLF            ,RXCURLOPT_BOOL        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071300
   { "CRLFILE"         ,CURLOPT_CRLFILE         ,RXCURLOPT_STRING      , NULL },
#endif
   { "CUSTOMREQUEST"   ,CURLOPT_CUSTOMREQUEST   ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x071100
   { "DIRLISTONLY"     ,CURLOPT_DIRLISTONLY     ,RXCURLOPT_BOOL        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "DNSCACHETIMEOUT" ,CURLOPT_DNS_CACHE_TIMEOUT,RXCURLOPT_LONG       , NULL },
#if LIBCURL_VERSION_NUM >= 0x071800
   { "DNSSERVERS"      ,CURLOPT_DNS_SERVERS     ,RXCURLOPT_STRING      , NULL },
#endif
   { "DNSUSEGLOBALCACHE",CURLOPT_DNS_USE_GLOBAL_CACHE,RXCURLOPT_BOOL   , NULL },
#endif
   { "EGDSOCKET"       ,CURLOPT_EGDSOCKET       ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a02
# if LIBCURL_VERSION_NUM >= 0x071506
   { "ENCODING"        ,CURLOPT_ACCEPT_ENCODING ,RXCURLOPT_STRING      , "ACCEPTENCODING" },
# else
   { "ENCODING"        ,CURLOPT_ENCODING        ,RXCURLOPT_STRING      , NULL },
# endif
#endif
   { "ERRFILE"         ,CURLOPT_STDERR          ,RXCURLOPT_ERRFILE     , NULL },
   { "FAILONERROR"     ,CURLOPT_FAILONERROR     ,RXCURLOPT_BOOL        , NULL },
   { "FILETIME"        ,CURLOPT_FILETIME        ,RXCURLOPT_BOOL        , NULL },
   { "FOLLOWLOCATION"  ,CURLOPT_FOLLOWLOCATION  ,RXCURLOPT_BOOL        , NULL },
   { "FORBIDREUSE"     ,CURLOPT_FORBID_REUSE    ,RXCURLOPT_BOOL        , NULL },
   { "FRESHCONNECT"    ,CURLOPT_FRESH_CONNECT   ,RXCURLOPT_BOOL        , NULL },
#if LIBCURL_VERSION_NUM >= 0x070e05
   { "FTPALTERNATIVETOUSER", CURLOPT_FTP_ALTERNATIVE_TO_USER, RXCURLOPT_STRING , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071100
   { "FTPAPPEND"       ,CURLOPT_APPEND          ,RXCURLOPT_BOOL        , "APPEND" },
#else
   { "FTPAPPEND"       ,CURLOPT_FTPAPPEND       ,RXCURLOPT_BOOL        , NULL },
#endif
   { "FTPCMDS"         ,CURLOPT_QUOTE           ,RXCURLOPT_LIST        , NULL },
   { "FTPCMDSAFTER"    ,CURLOPT_POSTQUOTE       ,RXCURLOPT_LIST        , NULL },
   { "FTPCMDSBEFORE"   ,CURLOPT_PREQUOTE        ,RXCURLOPT_LIST        , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a07
# if LIBCURL_VERSION_NUM >= 0x071304
   { "FTPCREATEMISSINGDIRS", CURLOPT_FTP_CREATE_MISSING_DIRS, RXCURLOPT_FTP_CREATE_MISSING_DIRS , NULL },
# else
   { "FTPCREATEMISSINGDIRS", CURLOPT_FTP_CREATE_MISSING_DIRS, RXCURLOPT_LONG , NULL },
# endif
#endif
   { "FTPCRLF"         ,CURLOPT_CRLF            ,RXCURLOPT_BOOL        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071100
   { "FTPLISTONLY"     ,CURLOPT_DIRLISTONLY     ,RXCURLOPT_BOOL        , "DIRLISTONLY" },
#else
   { "FTPLISTONLY"     ,CURLOPT_FTPLISTONLY     ,RXCURLOPT_BOOL        , NULL },
#endif
   { "FTPPORT"         ,CURLOPT_FTPPORT         ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a08
   { "FTPRESPONSETIMEOUT", CURLOPT_FTP_RESPONSE_TIMEOUT, RXCURLOPT_LONG, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070e00
   { "FTPSKIPPASVIP"   ,CURLOPT_FTP_SKIP_PASV_IP,RXCURLOPT_BOOL        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070b00
# if LIBCURL_VERSION_NUM >= 0x071100
   { "FTPSSL"          ,CURLOPT_USE_SSL         ,RXCURLOPT_FTPSSL      , "USESSL" },
# else
   { "FTPSSL"          ,CURLOPT_FTP_SSL         ,RXCURLOPT_FTPSSL      , NULL },
# endif
#endif
#if LIBCURL_VERSION_NUM >= 0x070c02
   { "FTPSSLAUTH"      ,CURLOPT_FTPSSLAUTH      ,RXCURLOPT_FTPSSLAUTH  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071001
   { "FTPSSLCCC"       ,CURLOPT_FTP_SSL_CCC     ,RXCURLOPT_FTPSSLCCC   , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a05
   { "FTPUSEEPRT"      ,CURLOPT_FTP_USE_EPRT    ,RXCURLOPT_BOOL        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "FTPUSEEPSV"      ,CURLOPT_FTP_USE_EPSV    ,RXCURLOPT_BOOL        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071400
   { "FTPUSEPRET"      ,CURLOPT_FTP_USE_PRET    ,RXCURLOPT_BOOL        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071600
   { "GSSAPIDELEGATION",CURLOPT_GSSAPI_DELEGATION,RXCURLOPT_GSSAPI_DELEGATION , NULL },
#endif
   { "HEADER"          ,CURLOPT_HEADER          ,RXCURLOPT_BOOL        , NULL },

#if LIBCURL_VERSION_NUM >= 0x072600
   { "HEADERFILE"      ,CURLOPT_HEADERDATA      ,RXCURLOPT_OUTFILE     , NULL },
   { "HEADERSTEM"      ,CURLOPT_HEADERDATA      ,RXCURLOPT_HEADERSTEM  , NULL },
#else
   { "HEADERFILE"      ,CURLOPT_WRITEHEADER     ,RXCURLOPT_OUTFILE     , NULL },
   { "HEADERSTEM"      ,CURLOPT_WRITEHEADER     ,RXCURLOPT_HEADERSTEM  , NULL },
#endif

#if LIBCURL_VERSION_NUM >= 0x070a03
   { "HTTP200ALIASES"  ,CURLOPT_HTTP200ALIASES  ,RXCURLOPT_LIST        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a06
   { "HTTPAUTH"        ,CURLOPT_HTTPAUTH        ,RXCURLOPT_BITMAP_AUTH , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071002
   { "HTTPCONTENTDECODING",CURLOPT_HTTP_CONTENT_DECODING,RXCURLOPT_BOOL , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070801
   { "HTTPGET"         ,CURLOPT_HTTPGET         ,RXCURLOPT_BOOL        , NULL },
#endif
   { "HTTPHEADER"      ,CURLOPT_HTTPHEADER      ,RXCURLOPT_LIST        , NULL },
   { "HTTPPOST"        ,CURLOPT_POST            ,RXCURLOPT_BOOL        , NULL },
   { "HTTPPOSTDATA"    ,CURLOPT_HTTPPOST        ,RXCURLOPT_POST_DATA   , NULL },
   { "HTTPPOSTFIELDS"  ,CURLOPT_POSTFIELDS      ,RXCURLOPT_POST_FIELDS , NULL },
   { "HTTPPOSTFORM"    ,CURLOPT_HTTPPOST        ,RXCURLOPT_POST_FORM   , NULL },
   { "HTTPPROXYTUNNEL" ,CURLOPT_HTTPPROXYTUNNEL ,RXCURLOPT_BOOL        , NULL },
   { "HTTPPUT"         ,CURLOPT_PUT             ,RXCURLOPT_BOOL        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071002
   { "HTTPTRANSFERDECODING",CURLOPT_HTTP_TRANSFER_DECODING,RXCURLOPT_BOOL , NULL },
#endif
   { "HTTPVERSION"     ,CURLOPT_HTTP_VERSION    ,RXCURLOPT_HTTP_VERSION, NULL },
#if LIBCURL_VERSION_NUM >= 0x070e01
   { "IGNORECONTENTLENGTH",CURLOPT_IGNORE_CONTENT_LENGTH,RXCURLOPT_LONG, NULL },
#endif

#if LIBCURL_VERSION_NUM >= 0x072600
   { "INFILE"          ,CURLOPT_READDATA        ,RXCURLOPT_INFILE      , NULL },
   { "INSTEM"          ,CURLOPT_READDATA        ,RXCURLOPT_INSTEM      , NULL },
#else
   { "INFILE"          ,CURLOPT_INFILE          ,RXCURLOPT_INFILE      , NULL },
   { "INSTEM"          ,CURLOPT_INFILE          ,RXCURLOPT_INSTEM      , NULL },
#endif

   { "INTERFACE"       ,CURLOPT_INTERFACE       ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a08
   { "IPRESOLVE"       ,CURLOPT_IPRESOLVE       ,RXCURLOPT_IPRESOLVE   , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071300
   { "ISSUERCERT"      ,CURLOPT_ISSUERCERT      ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071100
   { "KEYPASSWD"       ,CURLOPT_KEYPASSWD       ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071100
   { "KRB4LEVEL"       ,CURLOPT_KRBLEVEL        ,RXCURLOPT_STRING      , "KRBLEVEL" },
   { "KRBLEVEL"        ,CURLOPT_KRBLEVEL        ,RXCURLOPT_STRING      , NULL },
#else
   { "KRB4LEVEL"       ,CURLOPT_KRB4LEVEL       ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070f02
   { "LOCALPORT"       ,CURLOPT_LOCALPORT       ,RXCURLOPT_LONG        , NULL },
   { "LOCALPORTRANGE"  ,CURLOPT_LOCALPORTRANGE  ,RXCURLOPT_LONG        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x072200
   { "LOGINOPTIONS"    ,CURLOPT_LOGIN_OPTIONS   ,RXCURLOPT_STRING      , NULL },
#endif
   { "LOWSPEEDLIMIT"   ,CURLOPT_LOW_SPEED_LIMIT ,RXCURLOPT_LONG        , NULL },
   { "LOWSPEEDTIME"    ,CURLOPT_LOW_SPEED_TIME  ,RXCURLOPT_LONG        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071900
   { "MAILAUTH"        ,CURLOPT_MAIL_AUTH       ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071400
   { "MAILFROM"        ,CURLOPT_MAIL_FROM       ,RXCURLOPT_STRING      , NULL },
   { "MAILRCPT"        ,CURLOPT_MAIL_RCPT       ,RXCURLOPT_LIST        , NULL },
#endif
   { "MAXCONNECTS"     ,CURLOPT_MAXCONNECTS     ,RXCURLOPT_LONG        , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a08
   { "MAXFILESIZE"     ,CURLOPT_MAXFILESIZE     ,RXCURLOPT_LONGLONG    , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070e05
   { "MAXRECVSPEEDLARGE", CURLOPT_MAX_RECV_SPEED_LARGE, RXCURLOPT_LONGLONG , NULL },
#endif
   { "MAXREDIRS"       ,CURLOPT_MAXREDIRS       ,RXCURLOPT_LONG        , NULL },
#if LIBCURL_VERSION_NUM >= 0x070e05
   { "MAXSENDSPEEDLARGE", CURLOPT_MAX_SEND_SPEED_LARGE, RXCURLOPT_LONGLONG , NULL },
#endif
   { "NETRC"           ,CURLOPT_NETRC           ,RXCURLOPT_NETRC       , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a09
   { "NETRCFILE"       ,CURLOPT_NETRC_FILE      ,RXCURLOPT_INFILENAME  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071004
   { "NEWDIRECTORYPERMS" ,CURLOPT_NEW_DIRECTORY_PERMS ,RXCURLOPT_LONG  , NULL },
#endif
   { "NOBODY"          ,CURLOPT_NOBODY          ,RXCURLOPT_BOOL        , NULL },
#if LIBCURL_VERSION_NUM >= 0x070900
   { "NOPROGRESS"      ,CURLOPT_NOPROGRESS      ,RXCURLOPT_BOOL        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071304
   { "NOPROXY"         ,CURLOPT_NOPROXY         ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "NOSIGNAL"        ,CURLOPT_NOSIGNAL        ,RXCURLOPT_BOOL        , NULL },
#endif

#if LIBCURL_VERSION_NUM >= 0x072600
   { "OUTFILE"         ,CURLOPT_WRITEDATA       ,RXCURLOPT_OUTFILE     , NULL },
   { "OUTSTEM"         ,CURLOPT_WRITEDATA       ,RXCURLOPT_OUTSTEM     , NULL },
#else
   { "OUTFILE"         ,CURLOPT_FILE            ,RXCURLOPT_OUTFILE     , NULL },
   { "OUTSTEM"         ,CURLOPT_FILE            ,RXCURLOPT_OUTSTEM     , NULL },
#endif

#if LIBCURL_VERSION_NUM >= 0x071301
   { "PASSWORD"        ,CURLOPT_PASSWORD        ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071101
# if LIBCURL_VERSION_NUM >= 0x071301
   { "POST301"         ,CURLOPT_POSTREDIR       ,RXCURLOPT_BITMAP_REDIR, "POSTREDIR" },
# else
   { "POST301"         ,CURLOPT_POST301         ,RXCURLOPT_BOOL        , NULL },
# endif
#endif

/* don't explicitly allow this as an option   { "POSTFIELDSIZE"   ,CURLOPT_POSTFIELDSIZE   ,RXCURLOPT_LONG        , NULL }, */

#if LIBCURL_VERSION_NUM >= 0x070a02
   { "POSTQUOTE"       ,CURLOPT_POSTQUOTE       ,RXCURLOPT_LIST        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071301
   { "POSTREDIR"       ,CURLOPT_POSTREDIR       ,RXCURLOPT_BITMAP_REDIR, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "PREQUOTE"        ,CURLOPT_PREQUOTE        ,RXCURLOPT_LIST        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a03
   { "PRIVATE"         ,CURLOPT_PRIVATE         ,RXCURLOPT_STRING      , NULL },
#endif
#if defined( REXXCALLBACK )
   { "PROGRESSFUNCTION",CURLOPT_PROGRESSFUNCTION,RXCURLOPT_CALLBACK    , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071304
   { "PROTOCOLS"       ,CURLOPT_PROTOCOLS       ,RXCURLOPT_PROTOCOLS   , NULL },
#endif
   { "PROXY"           ,CURLOPT_PROXY           ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a07
   { "PROXYAUTH"       ,CURLOPT_PROXYAUTH       ,RXCURLOPT_BITMAP_AUTH , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071301
   { "PROXYPASSWORD"   ,CURLOPT_PROXYPASSWORD   ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070900
   { "PROXYPORT"       ,CURLOPT_PROXYPORT       ,RXCURLOPT_LONG        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071200
   { "PROXYTRANSFERMODE",CURLOPT_PROXY_TRANSFER_MODE,RXCURLOPT_LONG    , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "PROXYTYPE"       ,CURLOPT_PROXYTYPE       ,RXCURLOPT_PROXYTYPE   , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071301
   { "PROXYUSERNAME"   ,CURLOPT_PROXYUSERNAME   ,RXCURLOPT_STRING      , NULL },
#endif
   { "PROXYUSERPWD"    ,CURLOPT_PROXYUSERPWD    ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "QUOTE"           ,CURLOPT_QUOTE           ,RXCURLOPT_LIST        , NULL },
#endif
   { "RANDOMFILE"      ,CURLOPT_RANDOM_FILE     ,RXCURLOPT_STRING      , NULL },
   { "RANGE"           ,CURLOPT_RANGE           ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x071304
   { "REDIRPROTOCOLS"  ,CURLOPT_REDIR_PROTOCOLS ,RXCURLOPT_PROTOCOLS   , NULL },
#endif
   { "REFERER"         ,CURLOPT_REFERER         ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x071503
   { "RESOLVE"         ,CURLOPT_RESOLVE         ,RXCURLOPT_LIST        , NULL },
#endif
   { "RESUMEFROM"      ,CURLOPT_RESUME_FROM     ,RXCURLOPT_LONGLONG    , NULL },
#if LIBCURL_VERSION_NUM >= 0x071400
   { "RTSPCLIENTCSEQ"  ,CURLOPT_RTSP_CLIENT_CSEQ,RXCURLOPT_LONG        , NULL },
   { "RTSPHEADER"      ,CURLOPT_RTSPHEADER      ,RXCURLOPT_LIST        , NULL },
   { "RTSPREQUEST"     ,CURLOPT_RTSP_REQUEST    ,RXCURLOPT_RTSP_REQUEST, NULL },
   { "RTSPSERVERCSEQ"  ,CURLOPT_RTSP_SERVER_CSEQ,RXCURLOPT_LONG        , NULL },
   { "RTSPSESSIONID"   ,CURLOPT_RTSP_SESSION_ID ,RXCURLOPT_STRING      , NULL },
   { "RTSPSTREAMURI"   ,CURLOPT_RTSP_STREAM_URI ,RXCURLOPT_STRING      , NULL },
   { "RTSPTRANSPORT"   ,CURLOPT_RTSP_TRANSPORT  ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071304
   { "SOCKS5GSSAPINEC" ,CURLOPT_SOCKS5_GSSAPI_NEC, RXCURLOPT_LONG      , NULL },
   { "SOCKS5GSSAPISERVICE", CURLOPT_SOCKS5_GSSAPI_SERVICE, RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070d00 && LIBCURL_VERSION_NUM < 0x071000
   { "SOURCEPOSTQUOTE" ,CURLOPT_SOURCE_POSTQUOTE,RXCURLOPT_LIST        , NULL },
   { "SOURCEPREQUOTE"  ,CURLOPT_SOURCE_PREQUOTE ,RXCURLOPT_LIST        , NULL },
   { "SOURCEQUOTE"     ,CURLOPT_SOURCE_QUOTE    ,RXCURLOPT_LIST        , NULL },
   { "SOURCEURL"       ,CURLOPT_SOURCE_URL      ,RXCURLOPT_STRING      , NULL },
   { "SOURCEUSERPWD"   ,CURLOPT_SOURCE_USERPWD  ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071001
   { "SSHAUTHTYPES"    ,CURLOPT_SSH_AUTH_TYPES  ,RXCURLOPT_BITMAP_SSHAUTH , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071101
   { "SSHHOSTPUBLICKEYMD5", CURLOPT_SSH_HOST_PUBLIC_KEY_MD5, RXCURLOPT_STRING , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071306
# if defined( REXXCALLBACK )
   { "SSHKEYFUNCTION"  ,CURLOPT_SSH_KEYFUNCTION, RXCURLOPT_CALLBACK    , NULL },
# endif
   { "SSHKNOWNHOSTS"   ,CURLOPT_SSH_KNOWNHOSTS  ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071001
   { "SSHPRIVATEKEYFILE", CURLOPT_SSH_PRIVATE_KEYFILE, RXCURLOPT_INFILENAME , NULL },
   { "SSHPUBLICKEYFILE" , CURLOPT_SSH_PUBLIC_KEYFILE, RXCURLOPT_INFILENAME , NULL },
#endif
   { "SSLCERT"         ,CURLOPT_SSLCERT         ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x071100
   { "SSLCERTPASSWD"   ,CURLOPT_KEYPASSWD       ,RXCURLOPT_STRING      , "KEYPASSWD" },
#else
   { "SSLCERTPASSWD"   ,CURLOPT_SSLCERTPASSWD   ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "SSLCERTTYPE"     ,CURLOPT_SSLCERTTYPE     ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070900
   { "SSLCIPHERLIST"   ,CURLOPT_SSL_CIPHER_LIST ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "SSLENGINE"       ,CURLOPT_SSLENGINE       ,RXCURLOPT_STRING      , NULL },
   { "SSLENGINEDEFAULT",CURLOPT_SSLENGINE_DEFAULT,RXCURLOPT_STRING     , NULL },
   { "SSLKEY"          ,CURLOPT_SSLKEY          ,RXCURLOPT_STRING      , NULL },
# if LIBCURL_VERSION_NUM >= 0x071100
   { "SSLKEYPASSWD"    ,CURLOPT_KEYPASSWD       ,RXCURLOPT_STRING      , "KEYPASSWD" },
# else
   { "SSLKEYPASSWD"    ,CURLOPT_SSLKEYPASSWD    ,RXCURLOPT_STRING      , NULL },
# endif
   { "SSLKEYTYPE"      ,CURLOPT_SSLKEYTYPE      ,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071900
   { "SSLOPTIONS"      ,CURLOPT_SSL_OPTIONS     ,RXCURLOPT_SSL_OPTIONS , NULL },
#endif
   { "SSLPEERCERT"     ,CURLOPT_CAINFO          ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x071000
   { "SSLSESSIONIDCACHE", CURLOPT_SSL_SESSIONID_CACHE, RXCURLOPT_BOOL  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070801
   { "SSLVERIFYHOST"   ,CURLOPT_SSL_VERIFYHOST  ,RXCURLOPT_LONG        , NULL },
#endif
   { "SSLVERIFYPEER"   ,CURLOPT_SSL_VERIFYPEER  ,RXCURLOPT_BOOL        , NULL },
#if LIBCURL_VERSION_NUM >= 0x072A00
   { "SSLVERIFYSTATUS" ,CURLOPT_SSL_VERIFYSTATUS,RXCURLOPT_BOOL        , NULL },
#endif
   { "SSLVERSION"      ,CURLOPT_SSLVERSION      ,RXCURLOPT_LONG        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071900
   { "TCPKEEPALIVE"     ,CURLOPT_TCP_KEEPALIVE  ,RXCURLOPT_BOOL        , NULL },
   { "TCPKEEPIDLE"      ,CURLOPT_TCP_KEEPIDLE   ,RXCURLOPT_LONG        , NULL },
   { "TCPKEEPINTVL"     ,CURLOPT_TCP_KEEPINTVL  ,RXCURLOPT_LONG        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070d00
   { "TCPNODELAY"       ,CURLOPT_TCP_NODELAY    ,RXCURLOPT_BOOL        , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071304
   { "TFTPBLKSIZE"      ,CURLOPT_TFTP_BLKSIZE   ,RXCURLOPT_LONG        , NULL },
#endif
   { "TIMECONDITION"   ,CURLOPT_TIMECONDITION   ,RXCURLOPT_TIMECOND    , NULL },
   { "TIMEOUT"         ,CURLOPT_TIMEOUT         ,RXCURLOPT_LONG        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071002
   { "TIMEOUTMS"       ,CURLOPT_TIMEOUT_MS      ,RXCURLOPT_LONG        , NULL },
#endif
   { "TIMEVALUE"       ,CURLOPT_TIMEVALUE       ,RXCURLOPT_LONG        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071504
   { "TLSAUTHPASSWORD" ,CURLOPT_TLSAUTH_PASSWORD,RXCURLOPT_STRING      , NULL },
   { "TLSAUTHTYPE"     ,CURLOPT_TLSAUTH_TYPE    ,RXCURLOPT_BITMAP_TLSAUTH, NULL },
   { "TLSAUTHUSERNAME" ,CURLOPT_TLSAUTH_USERNAME,RXCURLOPT_STRING      , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071506
   { "TRANSFERENCODING",CURLOPT_TRANSFER_ENCODING,RXCURLOPT_BOOL      , NULL },
#endif
   { "TRANSFERTEXT"    ,CURLOPT_TRANSFERTEXT    ,RXCURLOPT_BOOL        , NULL },
#if LIBCURL_VERSION_NUM >= 0x070908
   { "UNRESTRICTEDAUTH",CURLOPT_UNRESTRICTED_AUTH  ,RXCURLOPT_BOOL     , NULL },
#endif
   { "UPLOAD"          ,CURLOPT_UPLOAD          ,RXCURLOPT_BOOL        , NULL },
   { "URL"             ,CURLOPT_URL             ,RXCURLOPT_STRING      , NULL },
   { "USERAGENT"       ,CURLOPT_USERAGENT       ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x071301
   { "USERNAME"        ,CURLOPT_USERNAME        ,RXCURLOPT_STRING      , NULL },
#endif
   { "USERPWD"         ,CURLOPT_USERPWD         ,RXCURLOPT_STRING      , NULL },
#if LIBCURL_VERSION_NUM >= 0x071100
   { "USESSL"          ,CURLOPT_USE_SSL         ,RXCURLOPT_FTPSSL      , NULL },
#endif
   { "VERBOSE"         ,CURLOPT_VERBOSE         ,RXCURLOPT_BOOL        , NULL },
#if LIBCURL_VERSION_NUM >= 0x071500
/* not implemented; need lots of callbacks */
/*   { "WILDCARDMATCH"   ,CURLOPT_WILDCARDMATCH   ,RXCURLOPT_BOOL        , NULL },*/
#endif
#if LIBCURL_VERSION_NUM >= 0x072100
   { "XOAUTH2BEARER"   ,CURLOPT_XOAUTH2_BEARER  ,RXCURLOPT_STRING      , NULL },
#endif
   { NULL              ,0                       ,0                     , NULL }
};

static curl_options RexxCurlGetinfos[] =
{
#if LIBCURL_VERSION_NUM >= 0x071300
   { "APPCONNECT_TIME"        ,CURLINFO_APPCONNECT_TIME        ,RXCURLINFO_DOUBLE, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071301
   { "CERTINFO"               ,CURLINFO_CERTINFO               ,RXCURLINFO_CERTINFO, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071304
   { "CONDITION_UNMET"        ,CURLINFO_CONDITION_UNMET        ,RXCURLINFO_LONG  , NULL },
#endif
   { "CONNECT_TIME"           ,CURLINFO_CONNECT_TIME           ,RXCURLINFO_DOUBLE, NULL },
   { "CONTENT_LENGTH_DOWNLOAD",CURLINFO_CONTENT_LENGTH_DOWNLOAD,RXCURLINFO_DOUBLE, NULL },
   { "CONTENT_LENGTH_UPLOAD"  ,CURLINFO_CONTENT_LENGTH_UPLOAD  ,RXCURLINFO_DOUBLE, NULL },
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "CONTENT_TYPE"           ,CURLINFO_CONTENT_TYPE           ,RXCURLINFO_STRING, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070e01
   { "COOKIELIST"             ,CURLINFO_COOKIELIST             ,RXCURLINFO_LIST  , NULL },
#endif
   { "EFFECTIVE_URL"          ,CURLINFO_EFFECTIVE_URL          ,RXCURLINFO_STRING, NULL },
   { "FILE_TIME"              ,CURLINFO_FILETIME               ,RXCURLINFO_LONG  , NULL },
#if LIBCURL_VERSION_NUM >= 0x070f04
   { "FTP_ENTRY_PATH"         ,CURLINFO_FTP_ENTRY_PATH         ,RXCURLINFO_STRING, NULL },
#endif
   { "HEADER_SIZE"            ,CURLINFO_HEADER_SIZE            ,RXCURLINFO_LONG  , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a08
   { "HTTPAUTH_AVAIL"         ,CURLINFO_HTTPAUTH_AVAIL         ,RXCURLINFO_BITMAP, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a07
   { "HTTP_CODE"              ,CURLINFO_RESPONSE_CODE          ,RXCURLINFO_LONG  , "RESPONSE_CODE" },
#else
   { "HTTP_CODE"              ,CURLINFO_HTTP_CODE              ,RXCURLINFO_LONG  , NULL },
#endif
   { "HTTP_CONNECTCODE"       ,CURLINFO_HTTP_CONNECTCODE       ,RXCURLINFO_LONG  , NULL },
#if LIBCURL_VERSION_NUM >= 0x070f02
   { "LASTSOCKET"             ,CURLINFO_LASTSOCKET             ,RXCURLINFO_LONG  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071500
   { "LOCAL_IP"               ,CURLINFO_LOCAL_IP               ,RXCURLINFO_STRING, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071500
   { "LOCAL_PORT"             ,CURLINFO_LOCAL_PORT             ,RXCURLINFO_LONG  , NULL },
#endif
   { "NAMELOOKUP_TIME"        ,CURLINFO_NAMELOOKUP_TIME        ,RXCURLINFO_DOUBLE, NULL },
#if LIBCURL_VERSION_NUM >= 0x070c03
   { "NUM_CONNECTS"           ,CURLINFO_NUM_CONNECTS           ,RXCURLINFO_LONG  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070c02
   { "OS_ERRNO"               ,CURLINFO_OS_ERRNO               ,RXCURLINFO_LONG  , NULL },
#endif
   { "PRETRANSFER_TIME"       ,CURLINFO_PRETRANSFER_TIME       ,RXCURLINFO_DOUBLE, NULL },
#if LIBCURL_VERSION_NUM >= 0x071300
   { "PRIMARY_IP"             ,CURLINFO_PRIMARY_IP             ,RXCURLINFO_STRING, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071500
   { "PRIMARY_PORT"           ,CURLINFO_PRIMARY_PORT           ,RXCURLINFO_LONG  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a03
   { "PRIVATE"                ,CURLINFO_PRIVATE                ,RXCURLINFO_STRING, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a08
   { "PROXYAUTH_AVAIL"        ,CURLINFO_PROXYAUTH_AVAIL        ,RXCURLINFO_BITMAP, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070907
   { "REDIRECT_COUNT"         ,CURLINFO_REDIRECT_COUNT         ,RXCURLINFO_LONG  , NULL },
   { "REDIRECT_TIME"          ,CURLINFO_REDIRECT_TIME          ,RXCURLINFO_DOUBLE, NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071202
   { "REDIRECT_URL"           ,CURLINFO_REDIRECT_URL           ,RXCURLINFO_STRING, NULL },
#endif
   { "REQUEST_SIZE"           ,CURLINFO_REQUEST_SIZE           ,RXCURLINFO_LONG  , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a08
   { "RESPONSE_CODE"          ,CURLINFO_RESPONSE_CODE          ,RXCURLINFO_LONG  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071400
   { "RTSP_CLIENT_CSEQ"       ,CURLINFO_RTSP_CLIENT_CSEQ       ,RXCURLINFO_LONG  , NULL },
   { "RTSP_CSEQ_RECV"         ,CURLINFO_RTSP_CSEQ_RECV         ,RXCURLINFO_LONG  , NULL },
   { "RTSP_SERVER_CSEQ"       ,CURLINFO_RTSP_SERVER_CSEQ       ,RXCURLINFO_LONG  , NULL },
   { "RTSP_SESSION_ID"        ,CURLINFO_RTSP_SESSION_ID        ,RXCURLINFO_STRING, NULL },
#endif
   { "SIZE_DOWNLOAD"          ,CURLINFO_SIZE_DOWNLOAD          ,RXCURLINFO_DOUBLE, NULL },
   { "SIZE_UPLOAD"            ,CURLINFO_SIZE_UPLOAD            ,RXCURLINFO_DOUBLE, NULL },
   { "SPEED_DOWNLOAD"         ,CURLINFO_SPEED_DOWNLOAD         ,RXCURLINFO_DOUBLE, NULL },
   { "SPEED_UPLOAD"           ,CURLINFO_SPEED_UPLOAD           ,RXCURLINFO_DOUBLE, NULL },
#if LIBCURL_VERSION_NUM >= 0x070c03
   { "SSL_ENGINES"            ,CURLINFO_SSL_ENGINES            ,RXCURLINFO_LIST  , NULL },
#endif
   { "SSL_VERIFYRESULT"       ,CURLINFO_SSL_VERIFYRESULT       ,RXCURLINFO_LONG  , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "STARTTRANSFER_TIME"     ,CURLINFO_STARTTRANSFER_TIME     ,RXCURLINFO_DOUBLE, NULL },
#endif
   { "TOTAL_TIME"             ,CURLINFO_TOTAL_TIME             ,RXCURLINFO_DOUBLE, NULL },
   { NULL                     ,0                               ,0                , NULL }
};

static curl_options RexxCurlSubOptions[] =
{
   { "OLDEST"                 ,CURLCLOSEPOLICY_OLDEST             ,RXCURLOPT_POLICY , NULL },
   { "LEAST_RECENTLY_USED"    ,CURLCLOSEPOLICY_LEAST_RECENTLY_USED,RXCURLOPT_POLICY , NULL },
   { "LEAST_TRAFFIC"          ,CURLCLOSEPOLICY_LEAST_TRAFFIC      ,RXCURLOPT_POLICY , NULL },
   { "SLOWEST"                ,CURLCLOSEPOLICY_SLOWEST            ,RXCURLOPT_POLICY , NULL },
   { "CALLBACK"               ,CURLCLOSEPOLICY_CALLBACK           ,RXCURLOPT_POLICY , NULL },
   { "NONE"                   ,CURL_HTTP_VERSION_NONE             ,RXCURLOPT_HTTP_VERSION , NULL },
   { "VERSION_1_0"            ,CURL_HTTP_VERSION_1_0              ,RXCURLOPT_HTTP_VERSION , NULL },
   { "VERSION_1_1"            ,CURL_HTTP_VERSION_1_1              ,RXCURLOPT_HTTP_VERSION , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a08
   { "OPTIONAL"               ,CURL_NETRC_OPTIONAL                ,RXCURLOPT_NETRC , NULL },
   { "IGNORED"                ,CURL_NETRC_IGNORED                 ,RXCURLOPT_NETRC , NULL },
   { "REQUIRED"               ,CURL_NETRC_REQUIRED                ,RXCURLOPT_NETRC , NULL },
#endif
   { "IFMODSINCE"             ,CURL_TIMECOND_IFMODSINCE           ,RXCURLOPT_TIMECOND , NULL },
   { "IFUNMODSINCE"           ,CURL_TIMECOND_IFUNMODSINCE         ,RXCURLOPT_TIMECOND , NULL },
   { "LASTMOD"                ,CURL_TIMECOND_LASTMOD              ,RXCURLOPT_TIMECOND , NULL },
#if LIBCURL_VERSION_NUM >= 0x070a02
   { "HTTP"                   ,CURLPROXY_HTTP                     ,RXCURLOPT_PROXYTYPE , NULL },
   { "SOCKS4"                 ,CURLPROXY_SOCKS4                   ,RXCURLOPT_PROXYTYPE , NULL },
   { "SOCKS5"                 ,CURLPROXY_SOCKS5                   ,RXCURLOPT_PROXYTYPE , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071304
   { "HTTP_1_0"               ,CURLPROXY_HTTP_1_0                 ,RXCURLOPT_PROXYTYPE , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071200
   { "SOCKS4A"                ,CURLPROXY_SOCKS4A                  ,RXCURLOPT_PROXYTYPE , NULL },
   { "SOCKS5_HOSTNAME"        ,CURLPROXY_SOCKS5_HOSTNAME          ,RXCURLOPT_PROXYTYPE , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a08
   { "WHATEVER"               ,CURL_IPRESOLVE_WHATEVER            ,RXCURLOPT_IPRESOLVE , NULL },
   { "V4"                     ,CURL_IPRESOLVE_V4                  ,RXCURLOPT_IPRESOLVE , NULL },
   { "V6"                     ,CURL_IPRESOLVE_V6                  ,RXCURLOPT_IPRESOLVE , NULL },
   { "IPRESOLVE_WHATEVER"     ,CURL_IPRESOLVE_WHATEVER            ,RXCURLOPT_IPRESOLVE , "WHATEVER" },
   { "IPRESOLVE_V4"           ,CURL_IPRESOLVE_V4                  ,RXCURLOPT_IPRESOLVE , "V4"       },
   { "IPRESOLVE_V6"           ,CURL_IPRESOLVE_V6                  ,RXCURLOPT_IPRESOLVE , "V6"       },
#endif
#if LIBCURL_VERSION_NUM >= 0x070a06
   { "BASIC"                  ,CURLAUTH_BASIC                     ,RXCURLOPT_BITMAP_AUTH , NULL },
   { "DIGEST"                 ,CURLAUTH_DIGEST                    ,RXCURLOPT_BITMAP_AUTH , NULL },
   { "AUTH_BASIC"             ,CURLAUTH_BASIC                     ,RXCURLOPT_BITMAP_AUTH , "BASIC"  },
   { "AUTH_DIGEST"            ,CURLAUTH_DIGEST                    ,RXCURLOPT_BITMAP_AUTH , "DIGEST" },
# if LIBCURL_VERSION_NUM >= 0x071303
   { "DIGEST_IE"              ,CURLAUTH_DIGEST_IE                 ,RXCURLOPT_BITMAP_AUTH , NULL },
# endif
   { "GSSNEGOTIATE"           ,CURLAUTH_GSSNEGOTIATE              ,RXCURLOPT_BITMAP_AUTH , NULL },
   { "NTLM"                   ,CURLAUTH_NTLM                      ,RXCURLOPT_BITMAP_AUTH , NULL },
   { "AUTH_GSSNEGOTIATE"      ,CURLAUTH_GSSNEGOTIATE              ,RXCURLOPT_BITMAP_AUTH , "GSSNEGOTIATE" },
   { "AUTH_NTLM"              ,CURLAUTH_NTLM                      ,RXCURLOPT_BITMAP_AUTH , "NTLM"         },
# if LIBCURL_VERSION_NUM >= 0x071600
   { "NTLM_WB"                ,CURLAUTH_NTLM_WB                   ,RXCURLOPT_BITMAP_AUTH , NULL },
# endif
   { "ANY"                    ,CURLAUTH_ANY                       ,RXCURLOPT_BITMAP_AUTH , NULL },
   { "ANYSAFE"                ,CURLAUTH_ANYSAFE                   ,RXCURLOPT_BITMAP_AUTH , NULL },
   { "AUTH_ANY"               ,CURLAUTH_ANY                       ,RXCURLOPT_BITMAP_AUTH , "ANY"     },
   { "AUTH_ANYSAFE"           ,CURLAUTH_ANYSAFE                   ,RXCURLOPT_BITMAP_AUTH , "ANYSAFE" },
#endif
# if LIBCURL_VERSION_NUM >= 0x071503
   { "ONLY"                   ,CURLAUTH_ONLY                      ,RXCURLOPT_BITMAP_AUTH , NULL },
   { "AUTH_ONLY"              ,CURLAUTH_ONLY                      ,RXCURLOPT_BITMAP_AUTH , "ONLY" },
#endif
#if LIBCURL_VERSION_NUM >= 0x071001
   { "PUBLICKEY"              ,CURLSSH_AUTH_PUBLICKEY             ,RXCURLOPT_BITMAP_SSHAUTH , NULL },
   { "PASSWORD"               ,CURLSSH_AUTH_PASSWORD              ,RXCURLOPT_BITMAP_SSHAUTH , NULL },
   { "HOST"                   ,CURLSSH_AUTH_HOST                  ,RXCURLOPT_BITMAP_SSHAUTH , NULL },
   { "KEYBOARD"               ,CURLSSH_AUTH_KEYBOARD              ,RXCURLOPT_BITMAP_SSHAUTH , NULL },
   { "ANY"                    ,CURLSSH_AUTH_ANY                   ,RXCURLOPT_BITMAP_SSHAUTH , NULL },
   { "AUTH_PUBLICKEY"         ,CURLSSH_AUTH_PUBLICKEY             ,RXCURLOPT_BITMAP_SSHAUTH , "PUBLICKEY" },
   { "AUTH_PASSWORD"          ,CURLSSH_AUTH_PASSWORD              ,RXCURLOPT_BITMAP_SSHAUTH , "PASSWORD"  },
   { "AUTH_HOST"              ,CURLSSH_AUTH_HOST                  ,RXCURLOPT_BITMAP_SSHAUTH , "HOST"      },
   { "AUTH_KEYBOARD"          ,CURLSSH_AUTH_KEYBOARD              ,RXCURLOPT_BITMAP_SSHAUTH , "KEYBOARD"  },
   { "AUTH_ANY"               ,CURLSSH_AUTH_ANY                   ,RXCURLOPT_BITMAP_SSHAUTH , "ANY"       },
#endif
#if LIBCURL_VERSION_NUM >= 0x070b00
# if LIBCURL_VERSION_NUM >= 0x071100
   { "NONE"                   ,CURLUSESSL_NONE                    ,RXCURLOPT_FTPSSL , NULL },
   { "TRY"                    ,CURLUSESSL_TRY                     ,RXCURLOPT_FTPSSL , NULL },
   { "CONTROL"                ,CURLUSESSL_CONTROL                 ,RXCURLOPT_FTPSSL , NULL },
   { "ALL"                    ,CURLUSESSL_ALL                     ,RXCURLOPT_FTPSSL , NULL },
# else
   { "NONE"                   ,CURLFTPSSL_NONE                    ,RXCURLOPT_FTPSSL , NULL },
   { "TRY"                    ,CURLFTPSSL_TRY                     ,RXCURLOPT_FTPSSL , NULL },
   { "CONTROL"                ,CURLFTPSSL_CONTROL                 ,RXCURLOPT_FTPSSL , NULL },
   { "ALL"                    ,CURLFTPSSL_ALL                     ,RXCURLOPT_FTPSSL , NULL },
# endif
#endif
#if LIBCURL_VERSION_NUM >= 0x070c02
   { "DEFAULT"                ,CURLFTPAUTH_DEFAULT                ,RXCURLOPT_FTPSSLAUTH , NULL },
   { "SSL"                    ,CURLFTPAUTH_SSL                    ,RXCURLOPT_FTPSSLAUTH , NULL },
   { "TLS"                    ,CURLFTPAUTH_TLS                    ,RXCURLOPT_FTPSSLAUTH , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071001
   { "NONE"                   ,CURLFTPSSL_CCC_NONE                ,RXCURLOPT_FTPSSLCCC , NULL },
   { "PASSIVE"                ,CURLFTPSSL_CCC_PASSIVE             ,RXCURLOPT_FTPSSLCCC , NULL },
   { "ACTIVE"                 ,CURLFTPSSL_CCC_ACTIVE              ,RXCURLOPT_FTPSSLCCC , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071304
   { "CREATE_DIR_NONE"        ,CURLFTP_CREATE_DIR_NONE            ,RXCURLOPT_FTP_CREATE_MISSING_DIRS , NULL },
   { "CREATE_DIR"             ,CURLFTP_CREATE_DIR                 ,RXCURLOPT_FTP_CREATE_MISSING_DIRS , NULL },
   { "CREATE_DIR_RETRY"       ,CURLFTP_CREATE_DIR_RETRY           ,RXCURLOPT_FTP_CREATE_MISSING_DIRS , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071400
   { "OPTIONS "              ,CURL_RTSPREQ_ANNOUNCE              ,RXCURLOPT_RTSP_REQUEST , NULL },
   { "DESCRIBE"              ,CURL_RTSPREQ_SETUP                 ,RXCURLOPT_RTSP_REQUEST , NULL },
   { "ANNOUNCE"              ,CURL_RTSPREQ_PLAY                  ,RXCURLOPT_RTSP_REQUEST , NULL },
   { "SETUP"                 ,CURL_RTSPREQ_PAUSE                 ,RXCURLOPT_RTSP_REQUEST , NULL },
   { "PLAY"                  ,CURL_RTSPREQ_TEARDOWN              ,RXCURLOPT_RTSP_REQUEST , NULL },
   { "PAUSE"                 ,CURL_RTSPREQ_GET_PARAMETER         ,RXCURLOPT_RTSP_REQUEST , NULL },
   { "TEARDOWN"              ,CURL_RTSPREQ_SET_PARAMETER         ,RXCURLOPT_RTSP_REQUEST , NULL },
   { "GET_PARAMETER"         ,CURL_RTSPREQ_RECORD                ,RXCURLOPT_RTSP_REQUEST , NULL },
   { "SET_PARAMETER"         ,CURL_RTSPREQ_RECEIVE               ,RXCURLOPT_RTSP_REQUEST , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071304
   { "ALL"                    ,CURLPROTO_ALL                      ,RXCURLOPT_PROTOCOLS    , NULL },
   { "HTTP"                   ,CURLPROTO_HTTP                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "HTTPS"                  ,CURLPROTO_HTTPS                    ,RXCURLOPT_PROTOCOLS    , NULL },
   { "FTP"                    ,CURLPROTO_FTP                      ,RXCURLOPT_PROTOCOLS    , NULL },
   { "FTPS"                   ,CURLPROTO_FTPS                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "SCP"                    ,CURLPROTO_SCP                      ,RXCURLOPT_PROTOCOLS    , NULL },
   { "SFTP"                   ,CURLPROTO_SFTP                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "TELNET"                 ,CURLPROTO_TELNET                   ,RXCURLOPT_PROTOCOLS    , NULL },
   { "LDAP"                   ,CURLPROTO_LDAP                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "LDAPS"                  ,CURLPROTO_LDAPS                    ,RXCURLOPT_PROTOCOLS    , NULL },
   { "DICT"                   ,CURLPROTO_DICT                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "FILE"                   ,CURLPROTO_FILE                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "TFTP"                   ,CURLPROTO_TFTP                     ,RXCURLOPT_PROTOCOLS    , NULL },
#endif
# if LIBCURL_VERSION_NUM >= 0x071400
   { "IMAP"                   ,CURLPROTO_IMAP                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "IMAPS"                  ,CURLPROTO_IMAPS                    ,RXCURLOPT_PROTOCOLS    , NULL },
   { "POP3"                   ,CURLPROTO_POP3                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "POP3S"                  ,CURLPROTO_POP3S                    ,RXCURLOPT_PROTOCOLS    , NULL },
   { "SMTP"                   ,CURLPROTO_SMTP                     ,RXCURLOPT_PROTOCOLS    , NULL },
   { "SMTPS"                  ,CURLPROTO_SMTPS                    ,RXCURLOPT_PROTOCOLS    , NULL },
   { "RTSP"                   ,CURLPROTO_RTSP                     ,RXCURLOPT_PROTOCOLS    , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071600
   { "FLAG"                   ,CURLGSSAPI_DELEGATION_FLAG         ,RXCURLOPT_GSSAPI_DELEGATION , NULL },
   { "NONE"                   ,CURLGSSAPI_DELEGATION_NONE         ,RXCURLOPT_GSSAPI_DELEGATION , NULL },
   { "POLICY_FLAG"            ,CURLGSSAPI_DELEGATION_POLICY_FLAG  ,RXCURLOPT_GSSAPI_DELEGATION , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071900
   { "ALLOW_BEAST"            ,CURLSSLOPT_ALLOW_BEAST             ,RXCURLOPT_SSL_OPTIONS  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x072C00
   { "NO_REVOKE"              ,CURLSSLOPT_NO_REVOKE               ,RXCURLOPT_SSL_OPTIONS  , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071301
   { "GET_ALL"                ,CURL_REDIR_GET_ALL                 ,RXCURLOPT_BITMAP_REDIR , NULL },
   { "POST_301"               ,CURL_REDIR_POST_301                ,RXCURLOPT_BITMAP_REDIR , NULL },
   { "POST_302"               ,CURL_REDIR_POST_302                ,RXCURLOPT_BITMAP_REDIR , NULL },
# if LIBCURL_VERSION_NUM >= 0x071a00
   { "POST_303"               ,CURL_REDIR_POST_303                ,RXCURLOPT_BITMAP_REDIR , NULL },
# endif
   { "POST_ALL"               ,CURL_REDIR_POST_ALL                ,RXCURLOPT_BITMAP_REDIR , NULL },
#endif
#if LIBCURL_VERSION_NUM >= 0x071504
   { "NONE"                   ,CURL_TLSAUTH_NONE                  ,RXCURLOPT_BITMAP_TLSAUTH,NULL },
   { "SRP"                    ,CURL_TLSAUTH_SRP                   ,RXCURLOPT_BITMAP_TLSAUTH,NULL },
#endif
   { NULL                     ,0                                  ,0 , NULL }
};

#define NUMBER_REXXCURL_OPTIONS (sizeof(RexxCurlOptions)/sizeof(curl_options))
static curl_version_info_data *version_info = NULL;

RxPackageConstantDef RexxCURLConstants[] =
{
   { "DIRSEP"   , 1, 0 , FILE_SEPARATOR_STR ,0.0, 0 },
   { "KHMATCH_MISMATCH", 0, CURLKHMATCH_MISMATCH, NULL, 0.0, 0 },
   { "KHMATCH_MISSING", 0, CURLKHMATCH_MISSING, NULL, 0.0, 0 },
   { "KHMATCH_OK", 0, CURLKHMATCH_OK, NULL, 0.0, 0 },
   { "KHSTAT_DEFER", 0, CURLKHSTAT_DEFER, NULL, 0.0, 0 },
   { "KHSTAT_FINE", 0, CURLKHSTAT_FINE, NULL, 0.0, 0 },
   { "KHSTAT_FINE_ADD_TO_FILE", 0, CURLKHSTAT_FINE_ADD_TO_FILE, NULL, 0.0, 0 },
   { "KHSTAT_REJECT", 0, CURLKHSTAT_REJECT, NULL, 0.0, 0 },
   { "KHTYPE_DSS", 0, CURLKHTYPE_DSS, NULL, 0.0, 0 },
   { "KHTYPE_RSA", 0, CURLKHTYPE_RSA, NULL, 0.0, 0 },
   { "KHTYPE_RSA1", 0, CURLKHTYPE_RSA1, NULL, 0.0, 0 },
   { "KHTYPE_UNKNOWN", 0, CURLKHTYPE_UNKNOWN, NULL, 0.0, 0 },
   { "PATHSEP"  , 1, 0 , PATH_SEPARATOR_STR ,0.0, 0 },
   { NULL       , 0, 0 , NULL               ,0.0, 0 },
};

rxfunc( CurlLoadFuncs );
rxfunc( CurlDropFuncs );
rxfunc( CurlInit );
rxfunc( CurlSetopt );
rxfunc( CurlPerform );
rxfunc( CurlGetinfo );
rxfunc( CurlCleanup );
rxfunc( CurlReset );
rxfunc( CurlVariable );
rxfunc( CurlQueryFunction );
rxfunc( CurlFormAdd );
rxfunc( CurlFormFree );
rxfunc( CurlEscape );
rxfunc( CurlUnescape );

/*-----------------------------------------------------------------------------
 * Table of CURL Functions. Used to install/de-install functions.
 * If you change this table, don't forget to change the table at the end
 * of this file.
 *----------------------------------------------------------------------------*/
RexxFunction RexxCURLFunctions[] = {
   { "CURLINIT"         ,CurlInit        ,"CurlInit"        , 1  },
   { "CURLCLEANUP"      ,CurlCleanup     ,"CurlCleanup"     , 1  },
   { "CURLSETOPT"       ,CurlSetopt      ,"CurlSetopt"      , 1  },
   { "CURLRESET"        ,CurlReset       ,"CurlReset"       , 1  },
   { "CURLPERFORM"      ,CurlPerform     ,"CurlPerform"     , 1  },
   { "CURLGETINFO"      ,CurlGetinfo     ,"CurlGetinfo"     , 1  },
   { "CURLVARIABLE"     ,CurlVariable    ,"CurlVariable"    , 1  },
   { "CURLQUERYFUNCTION",CurlQueryFunction,"CurlQueryFunction", 1  },
   { "CURLFORMADD"      ,CurlFormAdd     ,"CurlFormAdd"     , 1  },
   { "CURLFORMFREE"     ,CurlFormFree    ,"CurlFormFree"    , 1  },
   { "CURLESCAPE"       ,CurlEscape      ,"CurlEscape"      , 1  },
   { "CURLUNESCAPE"     ,CurlUnescape    ,"CurlUnescape"    , 1  },
   { "CURLDROPFUNCS"    ,CurlDropFuncs   ,"CurlDropFuncs"   , 1  },
   { "CURLLOADFUNCS"    ,CurlLoadFuncs   ,"CurlLoadFuncs"   , 0  }, /* Don't load for DLL */
   { NULL, NULL, NULL,0 }
};


#define DEFAULT_REXXCURL_ERROR        "CURLERROR."
#define INTERRM_PREFIX        "INTERRM"
#define INTCODE_PREFIX        "INTCODE"
#define CURLERRM_PREFIX       "CURLERRM"
#define CURLCODE_PREFIX       "CURLCODE"

typedef struct
{
   RxPackageGlobalDataDef *RxPackageGlobalData;
   RXSTRING str; /* str.strlength identifies how many bytes remaining */
   unsigned long index; /* where to start copying data from */
} instem_option;

typedef struct
{
   int g_rexxcurl_error;
   CURLcode g_curl_error;
   char curl_error[CURL_ERROR_SIZE+1];
   char rexxcurl_error_prefix[350];
   int outstem_index;
   int outstem_tail;
   char *outstem_strptr;
   char *outstem_line_terminator;
   int outstem_strlength;
   int headerstem_index;
   int headerstem_tail;
   int progress_index;
   rx_long_long max_long;
   char UsedOptions[NUMBER_REXXCURL_OPTIONS];
   FILE *FilePtrs[NUMBER_REXXCURL_OPTIONS];
   char *StringPtrs[NUMBER_REXXCURL_OPTIONS];
   instem_option *instem_options[NUMBER_REXXCURL_OPTIONS];
   struct curl_slist *SListPtrs[NUMBER_REXXCURL_OPTIONS];
   struct curl_httppost *HttpPostFirstPtrs[NUMBER_REXXCURL_OPTIONS];
   struct curl_httppost *HttpPostLastPtrs[NUMBER_REXXCURL_OPTIONS];
   RxPackageGlobalDataDef *RxPackageGlobalData;
} REXXCURLDATA;

static int have_rexxcallback = API_REXXCALLBACK_MISSING;
static int g_global_init_called = 0;

/*
 * If we don't have the RexxFreeMemory() API, then emulate it
 */
#ifndef REXXFREEMEMORY
static APIRET APIENTRY RexxFreeMemory( PVOID ptr )
{
#if defined( WIN32 )
   GlobalFree( ptr );
#elif defined( __OS2__ )
   DosFreeMem( ptr );
#else
   free( ptr );
#endif
   return 0;
}
# define REXXFREEMEMORY
#endif

/*
 * If we don't have the RexxAllocateMemory() API, then emulate it
 */
#ifndef REXXALLOCATEMEMORY
static PVOID APIENTRY RexxAllocateMemory( ULONG size )
{
   PVOID ret;
# if defined( WIN32 )
   ret = (PVOID)( GlobalLock( GlobalAlloc ( GMEM_FIXED, size ) ) );
   return ret;
# elif defined( __OS2__ )
   if ( ( BOOL )DosAllocMem( &ret, size, fPERM|PAG_COMMIT ) )
      return NULL;
   else
      return ret;
# else
   ret = (PVOID)malloc( size );
   return ret;
# endif
}
# define REXXALLOCATEMEMORY
#endif

#if defined(WIN32)
static CURLcode win32_init(void);
static void win32_cleanup(void);

void RexxCURLLoadCURL()
{
   char LoadError[256];
   if ( handle == NULL )
   {
      handle = LoadLibrary( REXXCURL_LIBCURL_DLL_NAME ) ;
      if ( handle == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to load cURL dll \"%s\": LoadLibrary() failed: ", REXXCURL_LIBCURL_DLL_NAME  );
         exit(1);
      }

      p_curl_easy_init = (T_CURL_EASY_INIT *)GetProcAddress( (HMODULE)handle, "curl_easy_init" );
      if ( p_curl_easy_init == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: %s", "curl_easy_init", LoadError );
         exit(1);
      }
      p_curl_easy_setopt = (T_CURL_EASY_SETOPT *)GetProcAddress( (HMODULE)handle, "curl_easy_setopt" );
      if ( p_curl_easy_setopt == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_easy_setopt" );
         exit(1);
      }
      p_curl_easy_reset = (T_CURL_EASY_SETOPT *)GetProcAddress( (HMODULE)handle, "curl_easy_reset" );
      if ( p_curl_easy_reset == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_easy_reset" );
         exit(1);
      }
      p_curl_easy_getinfo = (T_CURL_EASY_GETINFO *)GetProcAddress( (HMODULE)handle, "curl_easy_getinfo" );
      if ( p_curl_easy_getinfo == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_easy_getinfo" );
         exit(1);
      }
      p_curl_easy_perform = (T_CURL_EASY_PERFORM *)GetProcAddress( (HMODULE)handle, "curl_easy_perform" );
      if ( p_curl_easy_perform == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_easy_perform" );
         exit(1);
      }
      p_curl_easy_cleanup = (T_CURL_EASY_CLEANUP *)GetProcAddress( (HMODULE)handle, "curl_easy_cleanup" );
      if ( p_curl_easy_cleanup == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_easy_cleanup" );
         exit(1);
      }
      p_curl_formadd = (T_CURL_FORMADD *)GetProcAddress( (HMODULE)handle, "curl_formadd" );
      if ( p_curl_formadd == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_formadd" );
         exit(1);
      }
      p_curl_formfree = (T_CURL_FORMFREE *)GetProcAddress( (HMODULE)handle, "curl_formfree" );
      if ( p_curl_formfree == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_formfree" );
         exit(1);
      }
      p_curl_global_init = (T_CURL_GLOBAL_INIT *)GetProcAddress( (HMODULE)handle, "curl_global_init" );
      if ( p_curl_global_init == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_global_init" );
         exit(1);
      }
      p_curl_global_cleanup = (T_CURL_GLOBAL_CLEANUP *)GetProcAddress( (HMODULE)handle, "curl_global_cleanup" );
      if ( p_curl_global_cleanup == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_global_cleanup" );
         exit(1);
      }
      p_curl_slist_append = (T_CURL_SLIST_APPEND *)GetProcAddress( (HMODULE)handle, "curl_slist_append" );
      if ( p_curl_slist_append == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_slist_append" );
         exit(1);
      }
      p_curl_slist_free_all = (T_CURL_SLIST_FREE_ALL *)GetProcAddress( (HMODULE)handle, "curl_slist_free_all" );
      if ( p_curl_slist_free_all == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_slist_free_all" );
         exit(1);
      }
      p_curl_version = (T_CURL_VERSION *)GetProcAddress( (HMODULE)handle, "curl_version" );
      if ( p_curl_version == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_version" );
         exit(1);
      }
      p_curl_version_info = (T_CURL_VERSION_INFO *)GetProcAddress( (HMODULE)handle, "curl_version_info" );
      if ( p_curl_version_info == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_version_info" );
         exit(1);
      }
      p_curl_easy_escape = (T_CURL_EASY_ESCAPE *)GetProcAddress( (HMODULE)handle, "curl_easy_escape" );
      if ( p_curl_easy_escape == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_easy_escape" );
         exit(1);
      }
      p_curl_easy_unescape = (T_CURL_EASY_UNESCAPE *)GetProcAddress( (HMODULE)handle, "curl_easy_unescape" );
      if ( p_curl_easy_unescape == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_easy_unescape" );
         exit(1);
      }
      p_curl_free = (T_CURL_FREE *)GetProcAddress( (HMODULE)handle, "curl_free" );
      if ( p_curl_free == NULL )
      {
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL );
         fprintf( stderr, "Failed to find \"%s\" in external library: GetProcAddress() failed: ", "curl_free" );
         exit(1);
      }
   }
}

void RexxCURLUnloadCURL(void)
{
   if ( handle )
   {
      FreeLibrary( handle );
      handle = NULL;
   }
}
#endif

void RexxCURLSetVersionInfoConstants( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   const char * const*prot;
   char name[350];
   char value[100];
   char *support;
   int namelen;
   int valuelen;
   int count;

   InternalTrace( RxPackageGlobalData, "RexxCURLSetVersionInfoConstants", NULL );

   namelen = sprintf( name, "%sVERSION", RxGetConstantPrefix( RxPackageGlobalData ) );
   SetRexxVariable( RxPackageGlobalData, name, namelen, (char *)version_info->version, (version_info->version) ? strlen( version_info->version ) : 0 );
   namelen = sprintf( name, "%sVERSION_NUM", RxGetConstantPrefix( RxPackageGlobalData ) );
   valuelen = sprintf( value, "%d", version_info->version_num );
   SetRexxVariable( RxPackageGlobalData, name, namelen, value, valuelen );

   namelen = sprintf( name, "%sHOST", RxGetConstantPrefix( RxPackageGlobalData ) );
   SetRexxVariable( RxPackageGlobalData, name, namelen, (char *)version_info->host, (version_info->host) ? strlen( version_info->host ) : 0 );

   namelen = sprintf( name, "%sSSL_VERSION", RxGetConstantPrefix( RxPackageGlobalData ) );
   SetRexxVariable( RxPackageGlobalData, name, namelen, (char *)version_info->ssl_version, (version_info->ssl_version) ? strlen( version_info->ssl_version ) : 0 );

   namelen = sprintf( name, "%sLIBZ_VERSION", RxGetConstantPrefix( RxPackageGlobalData ) );
   SetRexxVariable( RxPackageGlobalData, name, namelen, (char *)version_info->libz_version, (version_info->libz_version) ? strlen( version_info->libz_version ) : 0 );

   namelen = sprintf( name, "%sARES", RxGetConstantPrefix( RxPackageGlobalData ) );
   SetRexxVariable( RxPackageGlobalData, name, namelen, (char *)version_info->ares, (version_info->ares) ? strlen( version_info->ares ) : 0 );
   namelen = sprintf( name, "%sARES_NUM", RxGetConstantPrefix( RxPackageGlobalData ) );
   valuelen = sprintf( value, "%d", version_info->ares_num );
   SetRexxVariable( RxPackageGlobalData, name, namelen, value, valuelen );

   namelen = sprintf( name, "%sLIBIDN", RxGetConstantPrefix( RxPackageGlobalData ) );
   SetRexxVariable( RxPackageGlobalData, name, namelen, (char *)version_info->libidn, (version_info->libidn) ? strlen( version_info->libidn ) : 0 );

   support = "";
   valuelen = 0;
   namelen = sprintf( name, "%sLIBSSH_VERSION", RxGetConstantPrefix( RxPackageGlobalData ) );
#if LIBCURL_VERSION_NUM >= 0x071001
   support = (char *)version_info->libssh_version;
   valuelen = (version_info->libssh_version) ? strlen( version_info->libssh_version ) : 0;
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, valuelen );

   namelen = sprintf( name, "%sICONV_VER_NUM", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
   valuelen = 1;
#if LIBCURL_VERSION_NUM >= 0x070e05
   valuelen = sprintf( value, "%d", version_info->iconv_ver_num );
   support = value;
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, valuelen );
   /*
    * Determine supported features
    */
   namelen = sprintf( name, "%sSUPPORTS_IPV6", RxGetConstantPrefix( RxPackageGlobalData ) );
   SetRexxVariable( RxPackageGlobalData, name, namelen, ( version_info->features & CURL_VERSION_IPV6 ) ? "1" : "0", 1 );
   namelen = sprintf( name, "%sSUPPORTS_KERBEROS4", RxGetConstantPrefix( RxPackageGlobalData ) );
   SetRexxVariable( RxPackageGlobalData, name, namelen, ( version_info->features & CURL_VERSION_KERBEROS4 ) ? "1" : "0", 1 );

   namelen = sprintf( name, "%sSUPPORTS_SSL", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070a00
   support = ( version_info->features & CURL_VERSION_SSL ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_LIBZ", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070a00
   support = ( version_info->features & CURL_VERSION_LIBZ ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_NTLM", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070a06
   support = ( version_info->features & CURL_VERSION_NTLM ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_GSSNEGOTIATE", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070a06
   support = ( version_info->features & CURL_VERSION_GSSNEGOTIATE ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_DEBUG", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070a06
   support = ( version_info->features & CURL_VERSION_DEBUG ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_CURLDEBUG", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x071306
   support = ( version_info->features & CURL_VERSION_CURLDEBUG ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_ASYNCHDNS", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070a07
   support = ( version_info->features & CURL_VERSION_ASYNCHDNS ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_SPNEGO", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070a08
   support = ( version_info->features & CURL_VERSION_SPNEGO ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_LARGEFILE", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070b01
   support = ( version_info->features & CURL_VERSION_LARGEFILE ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_IDN", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070c00
   support = ( version_info->features & CURL_VERSION_IDN ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_SSPI", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070d02
   support = ( version_info->features & CURL_VERSION_SSPI ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_CONV", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x070e04
   support = ( version_info->features & CURL_VERSION_CONV ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_TLSAUTH_SRP", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x071504
   support = ( version_info->features & CURL_VERSION_TLSAUTH_SRP ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );

   namelen = sprintf( name, "%sSUPPORTS_NTLM_WB", RxGetConstantPrefix( RxPackageGlobalData ) );
   support = "0";
#if LIBCURL_VERSION_NUM >= 0x071600
   support = ( version_info->features & CURL_VERSION_NTLM_WB ) ? "1" : "0";
#endif
   SetRexxVariable( RxPackageGlobalData, name, namelen, support, 1 );
   /*
    * Set supported protocols
    */
   prot = version_info->protocols;
   count = 1;
   while ( *prot )
   {
      namelen = sprintf( name, "%sPROTOCOLS.%d", RxGetConstantPrefix( RxPackageGlobalData ), count );
      SetRexxVariable( RxPackageGlobalData, name, namelen, (char *)*prot, strlen( *prot ) );
      prot++;
      count++;
   }
   namelen = sprintf( name, "%sPROTOCOLS.0", RxGetConstantPrefix( RxPackageGlobalData ) );
   valuelen = sprintf( value, "%d", count-1 );
   SetRexxVariable( RxPackageGlobalData, name, namelen, value, valuelen );
}

/*
 * These functions used by loader.c to obtain package-specific info
 * There probably is no need for this function at all as the INI EXIT should
 * do all the work!  FIX ME
 */
int RexxCURLInitialiser( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   InternalTrace( RxPackageGlobalData, "RexxCURLInitialiser", NULL );
#if 0

#if defined(WIN32)
   RexxCURLLoadCURL();
#endif
   /*
    * Check the version of the library we have loaded at runtime against the version we support;
    * not necessarily the version we built with.
    * We crash and burn if the version at runtime is < the supported version
    */
   version_info = CURL_VERSION_INFO(CURLVERSION_NOW);
   if ( version_info )
   {
      if ( version_info->version_num < LIBCURL_SUPPORTED_VERSION )
      {
         fprintf(stderr,"WARNING! Incompatible version of cURL found. The version of cURL found: %s is less than the supported version: %s.\n", version_info->version, LIBCURL_VERSION );
      }
      /*
       * Set our version info variables
       */
      RexxCURLSetVersionInfoConstants( RxPackageGlobalData );
   }
#endif
   return 0;
}

int RexxCURLTerminator( RxPackageGlobalDataDef *RxPackageGlobalData  )
{
   InternalTrace( RxPackageGlobalData , "RexxCURLTerminator", NULL );
   CURL_GLOBAL_CLEANUP();
#if defined(WIN32)
   RexxCURLUnloadCURL();
//   win32_cleanup();
#endif
   return 0;
}

/*
 * Every package MUST have one of these, otherwise ooRexx will crash
 */
REH_RETURN RexxCURLInitHandler( REH_ARG0_TYPE ExitNum, REH_ARG1_TYPE Subfun, REH_ARG2_TYPE PBlock )
{
   unsigned long tmp_long;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   InternalTrace(RxPackageGlobalData ,"RexxCURLInitHandler", "ExitNum %ld Subfun %ld",ExitNum, Subfun);
#if defined(WIN32)
   /*
    * Load the entry points for the libcurl DLL
    */
   RexxCURLLoadCURL();
#endif
   /*
    * Determine if the Rexx interpreter has RexxCallBack()
    * If USE_REGINA then we have it.
    * If USE_REXXTRANS then we MIGHT have it; we need to call RxTransHaveRexxCallBack()
    * to check.
    * If REXXCALLBACK defined, then we also have it.
    * All other situations, we DON'T have it.
    * We need to determine this BEFORE calling SetPackageConstants()
    */
#if defined( USE_REGINA )
   have_rexxcallback = API_REXXCALLBACK_PRESENT;
#elif defined ( USE_REXXTRANS )
   if ( RexxTransHaveRexxCallBack() )
      have_rexxcallback = API_REXXCALLBACK_PRESENT;
#elif defined( REXXCALLBACK )
      have_rexxcallback = API_REXXCALLBACK_PRESENT;
#else
   /*
    * The default value for HAVE_REXXCALLBACK variable is not set, so we don't have to change anything
    */
#endif
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;
#if defined(WIN32_NOT_REQUIRED)
   if ( win32_init() )
      return 1;
#endif
   if ( !g_global_init_called )
   {
      g_global_init_called = 1;
      CURL_GLOBAL_INIT(CURL_GLOBAL_ALL);
   }
   strcpy( RexxCURLData->rexxcurl_error_prefix, DEFAULT_REXXCURL_ERROR );
   /*
    * Get ourselves the maximum value of an unsigned long into max_long
    * to use when determining if we need to use long longs later
    */
   memset( &tmp_long, 0xFF, sizeof(tmp_long) );
   RexxCURLData->max_long = (rx_long_long)(tmp_long/2);
   SetPackageConstants( RxPackageGlobalData, RexxCURLConstants, 0 );
   /*
    * Check the version of the library we have loaded at runtime against the version we support;
    * not necessarily the version we built with.
    * We crash and burn if the version at runtime is < the supported version
    */
   version_info = CURL_VERSION_INFO(CURLVERSION_NOW);
   if ( version_info )
   {
      if ( version_info->version_num < LIBCURL_SUPPORTED_VERSION )
      {
         fprintf(stderr,"WARNING! Incompatible version of cURL found. The version of cURL found: %s is less than the supported version: %s.\n", version_info->version, LIBCURL_VERSION );
      }
      /*
       * Set our version info variables
       */
      RexxCURLSetVersionInfoConstants( RxPackageGlobalData );
   }
   return 0L;
}

LONG APIENTRY RexxCURLTermHandler( LONG ExitNum, LONG Subfun, PEXIT PBlock )
{
   DEBUGDUMP(fprintf(stderr,"%s-%d: In rexxcurlTermHandler() ExitNum %ld Subfun %ld\n",__FILE__,__LINE__,ExitNum, Subfun);)
   return 0L;
}

RexxSubcomHandler *getRexxCURLSubcomHandler( void )
{
   return NULL;
}

RexxExitHandler *getRexxCURLInitHandler( void )
{
   return RexxCURLInitHandler;
}

RexxExitHandler *getRexxCURLTermHandler( void )
{
   return RexxCURLTermHandler;
}

RexxFunction *getRexxCURLFunctions( void )
{
   return RexxCURLFunctions;
}

RxPackageConstantDef *getRexxCURLConstants( void )
{
   return RexxCURLConstants;
}

PackageInitialiser *getRexxCURLInitialiser( void )
{
   return RexxCURLInitialiser;
}

PackageTerminator *getRexxCURLTerminator( void )
{
   return RexxCURLTerminator;
}

void *getRexxCURLFunctionAddress( char *name )
{
   int i, size = sizeof( RexxCURLFunctions ) / sizeof( RexxFunction ) ;

   for (i = 0; i < size && RexxCURLFunctions[i].InternalName; i++)
   {
      if ( strcmp( RexxCURLFunctions[i].InternalName, name) == 0 )
         return RexxCURLFunctions[i].EntryPoint;
   }
   return NULL;
}

void *INIT_RXPACKAGE ( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   /*
    * This needs to allocate memory for RexxCURLData structure,
    */
   RxPackageGlobalData->RXPACKAGE_tsd = (REXXCURLDATA *)MALLOC_TSD( RxPackageGlobalData, sizeof(REXXCURLDATA) );
   if ( RxPackageGlobalData->RXPACKAGE_tsd )
      memset( RxPackageGlobalData->RXPACKAGE_tsd , 0, sizeof(REXXCURLDATA) );
   /*
    * TODO
    * If any global variable that require initialisation or allocation of memory, add them here
    */
   return (void *)RxPackageGlobalData->RXPACKAGE_tsd;
}

void TERM_RXPACKAGE ( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   /*
    * This needs to free memory allocated in INIT_RXPACKAGE()
    * and call any database-specific termination code
    */
   /*
    * Get the TSD if required
   REXXCURLDATA *RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;
    */
   /*
    * Free any items in the RexxCURLData struct that were allocated in INIT_RXPACKAGE
    */
   /*
    * Free the RexxCURLData struct
    */
   FREE_TSD( RxPackageGlobalData, RxPackageGlobalData->RXPACKAGE_tsd );
   RxPackageGlobalData = NULL;
}

#if defined(WIN32)
static void win32_cleanup(void)
{
  WSACleanup();
}

static CURLcode win32_init(void)
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  wVersionRequested = MAKEWORD(1, 1);

  err = WSAStartup(wVersionRequested, &wsaData);

  if (err != 0)
    /* Tell the user that we couldn't find a useable */
    /* winsock.dll.     */
    return 1;

  /* Confirm that the Windows Sockets DLL supports 1.1.*/
  /* Note that if the DLL supports versions greater */
  /* than 1.1 in addition to 1.1, it will still return */
  /* 1.1 in wVersion since that is the version we */
  /* requested. */

  if ( LOBYTE( wsaData.wVersion ) != 1 ||
       HIBYTE( wsaData.wVersion ) != 1 ) {
    /* Tell the user that we couldn't find a useable */

    /* winsock.dll. */
    WSACleanup();
    return 1;
  }
  return 0; /* 0 is ok */
}
#endif

/*-----------------------------------------------------------------------------
 * This function is the callback for PROGRESSFUNCTION option.
 * We need the client data pointer to get the TSD.
 *----------------------------------------------------------------------------*/
int rexxcurl_progress_callback( void *clientp, double dltotal, double dlnow, double ultotal, double ulnow )
{
   int num_args = 4;
   char rx_dltotal[20];
   char rx_dlnow[20];
   char rx_ultotal[20];
   char rx_ulnow[20];
   PRXSTRING argv;
   RXSTRING RetStr;
   int rc;
   SHORT rcode=0;
   REXXCURLDATA *RexxCURLData;
   RxPackageGlobalDataDef *RxPackageGlobalData = (RxPackageGlobalDataDef *)clientp;

   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( have_rexxcallback )
   {
      argv = (PRXSTRING)malloc( num_args*sizeof(RXSTRING) );
      if ( argv == NULL )
         return -1;
      sprintf( rx_dltotal, "%0.f", dltotal );
      sprintf( rx_dlnow, "%0.f", dlnow );
      sprintf( rx_ultotal, "%0.f", ultotal );
      sprintf( rx_ulnow, "%0.f", ulnow );
      MAKERXSTRING( argv[0], rx_dltotal, strlen( rx_dltotal ) );
      MAKERXSTRING( argv[1], rx_dlnow, strlen( rx_dlnow ) );
      MAKERXSTRING( argv[2], rx_ultotal, strlen( rx_ultotal ) );
      MAKERXSTRING( argv[3], rx_ulnow, strlen( rx_ulnow ) );

      MAKERXSTRING( RetStr, NULL, 0 );
#if defined( REXXCALLBACK )
      rc = RexxCallBack( RexxCURLData->StringPtrs[RexxCURLData->progress_index], num_args, argv, &rcode, &RetStr );
#else
      rc = 0;
#endif
      if ( RetStr.strptr )
         RexxFreeMemory( RetStr.strptr );
      free( argv );
      /* free_cb_argv( curr );*/
   }
   return rcode;
}

/*-----------------------------------------------------------------------------
 * This function is the callback for SSHKEYFUNCTION option.
 * We need the client data pointer to get the TSD.
 * Args to Rexx procedure:
 * knownkeystring - value of key
 * knownkeylength - length of known key; 0 indicates base64 encoding, otherwise raw
 * knownkeytype   - KHTYPE_UNKNOWN, KHTYPE_RSA, KHTYPE_RSA1, KHTYPE_DSS
 * foundkeystring - value of key
 * foundkeylength - length of found key; 0 indicates base64 encoding, otherwise raw
 * foundkeytype   - KHTYPE_UNKNOWN, KHTYPE_RSA, KHTYPE_RSA1, KHTYPE_DSS
 * libcurlviewofkey - KHMATCH_OK (match), KHMATCH_MISMATCH (host found, key mismatch!), KHMATCH_MISSING (no matching host/key found)
 *----------------------------------------------------------------------------*/
int rexxcurl_sshkey_callback( CURL *easy,     /* easy handle; ignored */
                              const struct curl_khkey *knownkey, /* known */
                              const struct curl_khkey *foundkey, /* found */
                              enum curl_khmatch match, /* libcurl's view on the keys */
                              void *clientp) /* custom pointer passed from app */
{
   int num_args = 7;
   char rx_knownkeylength[20];
   char rx_knownkeytype[20];
   char rx_foundkeylength[20];
   char rx_foundkeytype[20];
   char rx_libcurlviewofkey[20];
   int rx_knownkeylength_len;
   int rx_knownkeytype_len;
   int rx_foundkeylength_len;
   int rx_foundkeytype_len;
   int rx_libcurlviewofkey_len;
   PRXSTRING argv;
   RXSTRING RetStr;
   int rc;
   SHORT rcode=0;
   REXXCURLDATA *RexxCURLData;
   RxPackageGlobalDataDef *RxPackageGlobalData = (RxPackageGlobalDataDef *)clientp;

   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( have_rexxcallback )
   {
      argv = (PRXSTRING)malloc( num_args*sizeof(RXSTRING) );
      if ( argv == NULL )
         return -1;

      rx_knownkeylength_len = sprintf( rx_knownkeylength, "%lu", knownkey->len );
      rx_knownkeytype_len = sprintf( rx_knownkeytype, "%u", knownkey->keytype );
      rx_foundkeylength_len = sprintf( rx_foundkeylength, "%lu", foundkey->len );
      rx_foundkeytype_len = sprintf( rx_foundkeytype, "%u", foundkey->keytype );
      rx_libcurlviewofkey_len = sprintf( rx_libcurlviewofkey, "%u", match );

      if ( knownkey->len )
      {
         MAKERXSTRING( argv[0], knownkey->key, knownkey->len );
      }
      else
      {
         MAKERXSTRING( argv[0], knownkey->key, strlen( knownkey->key ) );
      }
      MAKERXSTRING( argv[1], rx_knownkeylength, rx_knownkeylength_len );
      MAKERXSTRING( argv[2], rx_knownkeytype, rx_knownkeytype_len );
      if ( foundkey->len )
      {
         MAKERXSTRING( argv[3], foundkey->key, foundkey->len );
      }
      else
      {
         MAKERXSTRING( argv[3], foundkey->key, strlen( foundkey->key ) );
      }
      MAKERXSTRING( argv[4], rx_foundkeylength, rx_foundkeylength_len );
      MAKERXSTRING( argv[5], rx_foundkeytype, rx_foundkeytype_len );
      MAKERXSTRING( argv[6], rx_libcurlviewofkey, rx_libcurlviewofkey_len );

      MAKERXSTRING( RetStr, NULL, 0 );
#if defined( REXXCALLBACK )
      rc = RexxCallBack( RexxCURLData->StringPtrs[RexxCURLData->progress_index], num_args, argv, &rcode, &RetStr );
#else
      rc = 0;
#endif
      if ( RetStr.strptr )
         RexxFreeMemory( RetStr.strptr );
      free( argv );
      /* free_cb_argv( curr );*/
   }
   return rcode;
}

/*-----------------------------------------------------------------------------
 * This function returns the size of the specified file. Returns -1 if the
 * file does not exist or is not a "normal" file eg a directory
 *----------------------------------------------------------------------------*/
rx_long_long get_file_size( RxPackageGlobalDataDef *RxPackageGlobalData, char *fn )
{
   struct stat stat_buf;
   size_t rc;
   rx_long_long fs;

   rc = stat( fn, &stat_buf ) ;
   if (rc == 0)
   {
      if ( (stat_buf.st_mode & S_IFMT) == S_IFDIR)
         fs = -1;
      else
         fs = (rx_long_long)stat_buf.st_size;
   }
   else
      fs = (rx_long_long)rc;
   InternalTrace( RxPackageGlobalData, "get_file_size", "Size of file: %s is: %ld", fn, fs );
   return fs;
}

/*-----------------------------------------------------------------------------
 * This function is necessary for Win32/64 platform
 * To use InternalTrace we would need to get TSD each time; very inefficient
 *----------------------------------------------------------------------------*/
size_t file_write_function( void *ptr, size_t size, size_t nmemb, void *stream )
{
   return fwrite( ptr, size, nmemb, stream );
}

/*-----------------------------------------------------------------------------
 * This function is necessary for Win32/64 platform
 * To use InternalTrace we would need to get TSD each time; very inefficient
 *----------------------------------------------------------------------------*/
int debug_function( CURL *handle, curl_infotype type, char *data, size_t size, void *userdata )
{
   FILE *fp = (FILE *)userdata;
   int c;
   char *prefix = "*<><><>";
   if ( type == CURLINFO_TEXT )
      fprintf( fp, "text: %c %.*s", prefix[type],size, data );
   if ( type == CURLINFO_HEADER_IN )
      fprintf( fp, "header_in: %c %.*s", prefix[type],size, data );
   if ( type == CURLINFO_HEADER_OUT )
      fprintf( fp, "header_out:%c %.*s", prefix[type],size, data );
   if ( type == CURLINFO_SSL_DATA_IN )
   {
      fprintf( fp, "ssl_data_in: " );
      for ( c = 0; c < size; c++ )
      {
         char x = (data[c] >= 0x20) ? data[c] : '.';
         fputc( x, fp);
      }
      fputc('\n', fp); /* newline */
   }
   if ( type == CURLINFO_SSL_DATA_OUT )
   {
      fprintf( fp, "ssl_data_out: " );
      for ( c = 0; c < size; c++ )
      {
         char x = (data[c] >= 0x20) ? data[c] : '.';
         fputc( x, fp);
      }
      fputc('\n', fp); /* newline */
   }
   return 0;
}


/*-----------------------------------------------------------------------------
 * This function is necessary for Win32/64 platform
 * To use InternalTrace we would need to get TSD each time; very inefficient
 *----------------------------------------------------------------------------*/
size_t file_read_function( void *ptr, size_t size, size_t nmemb, void *stream )
{
   return fread( ptr, size, nmemb, stream );
}

size_t stem_read_function( void *ptr, size_t size, size_t nmemb, void *data )
{
   instem_option *opt = (instem_option *)data;
   int count = size*nmemb; /* requested size to return */
   char *str;
   if ( opt->str.strlength )
   {
      int copy_this_much = opt->str.strlength;
      if ( copy_this_much > count)
      {
         copy_this_much = count;
      }
      InternalTrace( opt->RxPackageGlobalData, "stem_read_function", "Requested size: %ld Remaining: %ld Returning count: %ld", count, opt->str.strlength, copy_this_much );
      str = opt->str.strptr + opt->index;

      memcpy(ptr, str, copy_this_much);
      opt->str.strlength -= copy_this_much;
      opt->index += copy_this_much;
      return copy_this_much;
   }
   InternalTrace( opt->RxPackageGlobalData, "stem_read_function", "Complete!" );
   return 0;
}

/*-----------------------------------------------------------------------------
 * This function creates a compound Rexx variable with the supplied name
 * and vale.
 *----------------------------------------------------------------------------*/
int create_rexx_compound( RxPackageGlobalDataDef *RxPackageGlobalData, char *stem, int tail, char *value, int valuelen )
{
   char name[350];
   int namelen;

   namelen = sprintf( name, "%s%d", stem, tail );
   SetRexxVariable( RxPackageGlobalData, name, namelen, value, valuelen );
   return 0;
}
/*-----------------------------------------------------------------------------
 * This function writes the output from the site to a stem. Called from
 * OUTSTEM option
 *----------------------------------------------------------------------------*/
size_t outstem_write_function( void *ptr, size_t size, size_t nmemb, void *stream )
{
   size_t num_bytes=size*nmemb;
   size_t rc;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();

   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   InternalTrace( RxPackageGlobalData, "outstem_write_function", "Size of chunk to write: %ld", num_bytes );
   if ( RexxCURLData->outstem_strlength)
      RexxCURLData->outstem_strptr = (char *)realloc( RexxCURLData->outstem_strptr, RexxCURLData->outstem_strlength + num_bytes + 1);
   else
      RexxCURLData->outstem_strptr = (char *)malloc( RexxCURLData->outstem_strlength + num_bytes + 1);

   if ( RexxCURLData->outstem_strptr == NULL )
   {
      rc = -1;
   }
   else
   {
      memcpy( RexxCURLData->outstem_strptr+RexxCURLData->outstem_strlength, ptr, num_bytes );
      RexxCURLData->outstem_strlength += num_bytes;
      RexxCURLData->outstem_strptr[RexxCURLData->outstem_strlength] = '\0';
      rc = num_bytes;
   }
   InternalTrace( RxPackageGlobalData, "outstem_write_function", "Returned with: %ld", num_bytes );
   return rc;
}
/*-----------------------------------------------------------------------------
 * This function creates the compound variables for the stem.
 *----------------------------------------------------------------------------*/
int outstem_create( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   char *tmp,*ptr;
   char *eol;
   int eollen;
   REXXCURLDATA *RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->outstem_strptr )
   {
      eol = RexxCURLData->outstem_line_terminator;
      eollen = strlen( eol );
      ptr = RexxCURLData->outstem_strptr;
      tmp = strstr( ptr, eol );

      while( tmp != NULL )
      {
         *tmp = '\0';
         create_rexx_compound( RxPackageGlobalData, RexxCURLData->StringPtrs[RexxCURLData->outstem_index], ++(RexxCURLData->outstem_tail), (char *)ptr, strlen( ptr ) );
         ptr = tmp+eollen;
         tmp = strstr( ptr, eol );
      }
      if ( *ptr != '\0' )
      {
         create_rexx_compound( RxPackageGlobalData, RexxCURLData->StringPtrs[RexxCURLData->outstem_index], ++(RexxCURLData->outstem_tail), (char *)ptr, strlen( ptr ) );
      }
   }

   return 0;
}
/*-----------------------------------------------------------------------------
 * This function writes the output from the site to a stem. Called from
 * HEADERSTEM option
 *----------------------------------------------------------------------------*/
size_t headerstem_write_function( void *ptr, size_t size, size_t nmemb, void *stream )
{
   size_t num_bytes=size*nmemb;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;
   /*
    * We are guaranteed that each header is complete and terminated with CRLF, so simply
    * set the length of the variable value to 2 less than actual length
    */
   create_rexx_compound( RxPackageGlobalData , RexxCURLData->StringPtrs[RexxCURLData->headerstem_index], ++(RexxCURLData->headerstem_tail), (char *)ptr, num_bytes - 2 );
   /*
    * Tell cURL we've read ALL bytes
    */
   return num_bytes;
}

/*-----------------------------------------------------------------------------
 * Clear the cURL error message.
 *----------------------------------------------------------------------------*/
void ClearCURLError( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   char var[350];
   int varlen;
   REXXCURLDATA *RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   InternalTrace( RxPackageGlobalData, "ClearCURLError", NULL );
   /*
    * Set CURLERROR.CURLERRM variable
    */
   varlen = sprintf( var,"%s%s", RexxCURLData->rexxcurl_error_prefix, CURLERRM_PREFIX );
   (void)SetRexxVariable( RxPackageGlobalData, var, varlen, "", 0 );
   /*
    * Set CURLERROR.CURLCODE variable
    */
   varlen = sprintf( var,"%s%s", RexxCURLData->rexxcurl_error_prefix, CURLCODE_PREFIX );
   (void)SetRexxVariable( RxPackageGlobalData, var, varlen, "0", 1 );
   RexxCURLData->g_curl_error = 0;
   return;
}

/*-----------------------------------------------------------------------------
 * Set the cURL error message.
 *----------------------------------------------------------------------------*/
void SetCURLError( RxPackageGlobalDataDef *RxPackageGlobalData, CURLcode curlcode, char *curlmsg)
{
   char var[350];
   char msg[350];
   int varlen;
   int msglen;
   REXXCURLDATA *RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   InternalTrace( RxPackageGlobalData, "SetCURLError", "%d,%s", curlcode, curlmsg );

   RexxCURLData->g_curl_error = curlcode;
   /*
    * Set CURLERROR.CURLERRM variable
    */
   varlen = sprintf( var,"%s%s", RexxCURLData->rexxcurl_error_prefix, CURLERRM_PREFIX );
   (void)SetRexxVariable( RxPackageGlobalData, var, varlen, curlmsg, strlen(curlmsg) );
   /*
    * Set CURLERROR.CURLCODE variable
    */
   msglen = sprintf( msg, "%d", RexxCURLData->g_curl_error );
   varlen = sprintf( var,"%s%s", RexxCURLData->rexxcurl_error_prefix, CURLCODE_PREFIX );
   (void)SetRexxVariable( RxPackageGlobalData, var, varlen, msg, msglen );
   return;
}

/*-----------------------------------------------------------------------------
 * Clear the internal error message.
 *----------------------------------------------------------------------------*/
static int ClearIntError( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   char var[350];
   int varlen;
   REXXCURLDATA *RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   InternalTrace( RxPackageGlobalData, "ClearIntError", NULL );

   RexxCURLData->g_rexxcurl_error = 0;
   /*
    * Set CURLERROR.INTERRM variable
    */
   varlen = sprintf( var,"%s%s", RexxCURLData->rexxcurl_error_prefix, INTERRM_PREFIX );
   (void)SetRexxVariable( RxPackageGlobalData, var, varlen, "", 0 );
   /*
    * Set CURLERROR.INTCODE variable
    */
   varlen = sprintf( var,"%s%s", RexxCURLData->rexxcurl_error_prefix, INTCODE_PREFIX );
   (void)SetRexxVariable( RxPackageGlobalData, var, varlen, "0", 1 );

   return( RexxCURLData->g_rexxcurl_error );
}

/*-----------------------------------------------------------------------------
 * Set the internal error message.
 *----------------------------------------------------------------------------*/
static int SetIntError( RxPackageGlobalDataDef *RxPackageGlobalData, char *fn, int lineno, int errcode, char *errmsg )
{
   char msg[350];
   char var[350];
   int msglen;
   int varlen;
   REXXCURLDATA *RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   InternalTrace( RxPackageGlobalData, "SetIntError", "%s,%d,%d,%s", fn, lineno, errcode, errmsg );

   RexxCURLData->g_rexxcurl_error = -errcode;

   /*
    * Set CURLERROR.INTERRM variable
    */
   if ( RxGetRunFlags( RxPackageGlobalData ) & MODE_INTERNAL )
      msglen = sprintf(msg, "Rexx/CURL-%02d: %s [%s:%d]", errcode, errmsg, fn, lineno);
   else
      msglen = sprintf(msg, "REXX/CURL-%02d: %s", errcode, errmsg);
   varlen = sprintf( var,"%s%s", RexxCURLData->rexxcurl_error_prefix, INTERRM_PREFIX );
   (void)SetRexxVariable( RxPackageGlobalData, var, varlen, msg, msglen );

   /*
    * Set CURLERROR.INTCODE variable
    */
   msglen = sprintf( msg, "%d", RexxCURLData->g_rexxcurl_error );
   varlen = sprintf( var,"%s%s", RexxCURLData->rexxcurl_error_prefix, INTCODE_PREFIX );
   (void)SetRexxVariable( RxPackageGlobalData, var, varlen, msg, msglen );

   return( RexxCURLData->g_rexxcurl_error );
}

static void init_options( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   int i;
   REXXCURLDATA *RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   InternalTrace( RxPackageGlobalData, "init_options", NULL );

   for ( i = 0; i < NUMBER_REXXCURL_OPTIONS; i++ )
   {
      RexxCURLData->FilePtrs[i] = NULL;
      RexxCURLData->StringPtrs[i] = NULL;
      RexxCURLData->SListPtrs[i] = NULL;
      RexxCURLData->HttpPostFirstPtrs[i] = NULL;
      RexxCURLData->HttpPostLastPtrs[i] = NULL;
      RexxCURLData->UsedOptions[i] = '\0';
   }
}

static void reset_options( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   int i;
   REXXCURLDATA *RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   for ( i = 0; i < NUMBER_REXXCURL_OPTIONS; i++ )
   {
      if ( RexxCURLData->FilePtrs[i] )
      {
         fclose( RexxCURLData->FilePtrs[i] );
         RexxCURLData->FilePtrs[i] = NULL;
      }
      if ( RexxCURLData->StringPtrs[i] )
      {
         free( RexxCURLData->StringPtrs[i] );
         RexxCURLData->StringPtrs[i] = NULL;
      }
      if ( RexxCURLData->SListPtrs[i] )
      {
         CURL_SLIST_FREE_ALL( RexxCURLData->SListPtrs[i] );
         RexxCURLData->SListPtrs[i] = NULL;
      }
      if ( RexxCURLData->HttpPostFirstPtrs[i] )
      {
         CURL_FORMFREE( RexxCURLData->HttpPostFirstPtrs[i] );
         RexxCURLData->HttpPostFirstPtrs[i] = NULL;
         RexxCURLData->HttpPostLastPtrs[i] = NULL;
      }
      RexxCURLData->UsedOptions[i] = '\0';
   }
}

static int find_option( RxPackageGlobalDataDef *RxPackageGlobalData, char *str, int len )
{
   register int i = 0;

   for ( i = 0; RexxCurlOptions[i].name != NULL; i++ )
   {
      if ( memcmpi( RxPackageGlobalData, str, RexxCurlOptions[i].name, len ) == 0 )
      {
         /*
          * If this is a deprecated option name, print a warning with the new name
          */
         if ( RexxCurlOptions[i].newname )
         {
            fprintf( stderr,"WARNING: The option \"%s\" is deprecated. Use option \"%s\" instead.\n", RexxCurlOptions[i].name, RexxCurlOptions[i].newname );
         }
         return i;
      }
   }
   return ( -1 );
}

static int find_getinfo( RxPackageGlobalDataDef *RxPackageGlobalData, char *str, int len )
{
   register int i = 0;

   for ( i = 0; RexxCurlGetinfos[i].name != NULL; i++ )
   {
      if ( memcmpi( RxPackageGlobalData, str, RexxCurlGetinfos[i].name, len ) == 0 )
      {
         /*
          * If this is a deprecated option name, print a warning with the new name
          */
         if ( RexxCurlGetinfos[i].newname )
         {
            fprintf( stderr,"WARNING: The info option \"%s\" is deprecated. Use info option \"%s\" instead.\n", RexxCurlGetinfos[i].name, RexxCurlGetinfos[i].newname );
         }
         return i;
      }
   }
   return ( -1 );
}

static int find_suboption( RxPackageGlobalDataDef *RxPackageGlobalData, char *str, int len, int optiontype )
{
   register int i = 0;

   for ( i = 0; RexxCurlSubOptions[i].name != NULL; i++ )
   {
      if ( memcmpi( RxPackageGlobalData, str, RexxCurlSubOptions[i].name, len ) == 0
      &&   RexxCurlSubOptions[i].optiontype == optiontype )
      {
         /*
          * If this is a deprecated option name, print a warning with the new name
          */
         if ( RexxCurlSubOptions[i].newname )
         {
            fprintf( stderr,"WARNING: The sub-option \"%s\" is deprecated. Use sub-option \"%s\" instead.\n", RexxCurlSubOptions[i].name, RexxCurlSubOptions[i].newname );
         }
         return i;
      }
   }
   return ( -1 );
}

/*====== Here come the real interface functions to curl ======*/

rxfunc( CurlInit )
{
   CURL *curl;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   ClearCURLError( RxPackageGlobalData );
   ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 0, 0 ) )
      return( 1 );
   /*
    * Do some initialisations
    */
   init_options( RxPackageGlobalData );
   curl = CURL_EASY_INIT();
   InternalTrace( RxPackageGlobalData, "CurlInit", "After curl_easy_init" );
   /*
    * Determine if the Rexx interpreter has RexxCallBack()
    * If USE_REGINA then we have it.
    * If USE_REXXTRANS then we MIGHT have it; we need to call RxTransHaveRexxCallBack()
    * to check.
    * If REXXCALLBACK defined, then we also have it.
    * All other situations, we DON'T have it.
    * We need to determine this BEFORE calling SetPackageConstants()
    */
#if defined( USE_REGINA )
   have_rexxcallback = 1;
   InternalTrace( RxPackageGlobalData, "CurlInit", "RexxCallback available: Regina" );
#elif defined ( USE_REXXTRANS )
   if ( RexxTransHaveRexxCallBack() )
      have_rexxcallback = 1;
   InternalTrace( RxPackageGlobalData, "CurlInit", "RexxCallback %s: RexxTrans", have_rexxcallback ? "available" : "unavailable" );
#elif defined( REXXCALLBACK )
   have_rexxcallback = 1;
   InternalTrace( RxPackageGlobalData, "CurlInit", "RexxCallback available: Other Interpreter" );
#else
   have_rexxcallback = 0;
   InternalTrace( RxPackageGlobalData, "CurlInit", "RexxCallback unavailable: Other Interpreter" );
#endif
   return RxReturnPointer( RxPackageGlobalData, retstr, (void *)curl ) ;
}

rxfunc( CurlCleanup )
{
   long curl;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 1, 1 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnNumber( RxPackageGlobalData, retstr, INTERR_INVALID_HANDLE ) ;
   }
   CURL_EASY_CLEANUP( (CURL *)curl );
   reset_options( RxPackageGlobalData );
   memset( RexxCURLData->UsedOptions, 0, sizeof(char)*NUMBER_REXXCURL_OPTIONS );
   return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
}

rxfunc( CurlSetopt )
{
   ULONG rc = 0L;
   long curl;
   int i,j,opt,sub_opt;
   long long_opt;
   rx_long_long longlong_opt;
   rx_long_long file_size;
   CURLcode curl_rc;
   CURLFORMcode curlformadd_rc;
   RXSTRING value;
   int ltlen;
   int count = 0;
   char *ltstr;
   char *tmp;
   char eol[2]={10,0};
   char tmp_format[100];
   char *opentype="wb";
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 2, 0 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   opt = find_option( RxPackageGlobalData, argv[1].strptr, argv[1].strlength );
   if ( opt == (-1) )
   {
      char buf[1024];
      sprintf( buf, "%s: %s", INTERR_INVALID_OPTION_STRING, argv[1].strptr );
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_OPTION, buf );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   /*
    * HTTPPOSTDATA does NOT require any further arguments, so only if it is NOT
    * this option, check for the mandatory 3rd argument
    */
   if ( RexxCurlOptions[opt].optiontype != RXCURLOPT_POST_FORM )
   {
      if ( my_checkparam( RxPackageGlobalData, name, argc, 3, 0 ) )
         return( 1 );
   }

   switch( RexxCurlOptions[opt].optiontype )
   {
      case RXCURLOPT_STRING:
         if ( RexxCURLData->StringPtrs[opt] )
         {
            free( RexxCURLData->StringPtrs[opt] );
            RexxCURLData->StringPtrs[opt] = NULL;
         }
         RexxCURLData->StringPtrs[opt] = (char *)malloc( argv[2].strlength + 1 );
         if ( RexxCURLData->StringPtrs[opt] == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            break;
         }
         memcpy( RexxCURLData->StringPtrs[opt], argv[2].strptr, argv[2].strlength );
         RexxCURLData->StringPtrs[opt][argv[2].strlength] = '\0';
         /*
          * Do any extra processing here for some options
          */
         switch( RexxCurlOptions[opt].number )
         {
            default:
               break;
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->StringPtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_OUTFILE:
         /* check if APPEND passed as optional argument */
         if ( argc == 4
         &&   argv[3].strlength )
         {
            if ( memcmpi( RxPackageGlobalData, argv[3].strptr, "APPEND", 6 ) == 0 )
               opentype = "ab";
         }
         /* parameter must be the name of a file to write to or blank to turn it off */
         if ( argv[2].strlength )
         {
            RexxCURLData->FilePtrs[opt] = fopen( argv[2].strptr, opentype );
            if ( RexxCURLData->FilePtrs[opt] == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_FILE, INTERR_INVALID_FILE_STRING );
               break;
            }
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->FilePtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         switch ( RexxCurlOptions[opt].number )
         {
#if LIBCURL_VERSION_NUM >= 0x072600
            case CURLOPT_WRITEDATA:
            case CURLOPT_HEADERDATA:
#else
            case CURLOPT_FILE:
            case CURLOPT_WRITEHEADER:
#endif
               if ( argv[2].strlength )
               {
                  /*
                   * Always set the write function if we have an output file
                   */
                  rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_WRITEFUNCTION, &file_write_function );
                  if ( curl_rc != CURLE_OK )
                  {
                     SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                     SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                     break;
                  }
               }
               break;
            default:
               break;
         }
         break;
      case RXCURLOPT_ERRFILE:
         /* check if APPEND passed as optional argument */
         if ( argc == 4
         &&   argv[3].strlength )
         {
            if ( memcmpi( RxPackageGlobalData, argv[3].strptr, "APPEND", 6 ) == 0 )
               opentype = "ab";
         }
         /* parameter must be the name of a file to write to or blank to turn it off */
         if ( argv[2].strlength )
         {
            RexxCURLData->FilePtrs[opt] = fopen( argv[2].strptr, opentype );
            if ( RexxCURLData->FilePtrs[opt] == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_FILE, INTERR_INVALID_FILE_STRING );
               break;
            }
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->FilePtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_DEBUGDATA, RexxCURLData->FilePtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         switch ( RexxCurlOptions[opt].number )
         {
            case CURLOPT_STDERR:
               if ( argv[2].strlength )
               {
                  /*
                   * Always set the write function if we have an output file
                   */
                  rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_DEBUGFUNCTION, &debug_function );
                  if ( curl_rc != CURLE_OK )
                  {
                     SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                     SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                     break;
                  }
               }
               break;
            default:
               break;
         }
         break;

      case RXCURLOPT_INFILE:
         /* parameter must be the name of a file to read from or blank to turn it off */
         if ( argv[2].strlength )
         {
            RexxCURLData->FilePtrs[opt] = fopen( argv[2].strptr, "rb" );
            if ( RexxCURLData->FilePtrs[opt] == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_FILE, INTERR_INVALID_FILE_STRING );
               break;
            }
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->FilePtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         /*
          * Now do some extra stuff depending on the option specified...
          */
         switch ( RexxCurlOptions[opt].number )
         {
#if LIBCURL_VERSION_NUM >= 0x072600
            case CURLOPT_READDATA:
#else
            case CURLOPT_INFILE:
#endif
               /*
                * Always set the file size for the specified INFILE
                */
               if ( argv[2].strlength )
               {
                  file_size = get_file_size( RxPackageGlobalData, argv[2].strptr );
               }
               else
               {
                  file_size = 0;
               }
               if ( file_size == -1 )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_FILE, INTERR_INVALID_FILE_STRING );
                  break;
               }
               /*
                * Determine which option to use based on file size
                */
#if LIBCURL_VERSION_NUM >= 0x070b00
               if ( file_size > RexxCURLData->max_long )
               {
                  rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_INFILESIZE_LARGE, file_size );
               }
               else
#endif
               {
                  rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_INFILESIZE, (long)file_size );
               }
               if ( curl_rc != CURLE_OK )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                  SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                  break;
               }
               if ( argv[2].strlength )
               {
                  /*
                   * Always set the read function if we have an input file. This is because
                   * Win32/64 port requires it!
                   */
                  rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_READFUNCTION, &file_read_function );
                  if ( curl_rc != CURLE_OK )
                  {
                     SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                     SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                     break;
                  }
               }
               break;
            default:
               break;
         }

         break;
      case RXCURLOPT_INFILENAME:
         if ( argv[2].strlength == 0
         &&   RexxCurlOptions[opt].number == CURLOPT_COOKIEFILE )
         {
            /* parameter can be blank for COOKIEFILE */
         }
         else
         {
            /* parameter must be the name of a file that we can read; so open it */
            RexxCURLData->FilePtrs[opt] = fopen( argv[2].strptr, "rb" );
            if ( RexxCURLData->FilePtrs[opt] == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_FILE, INTERR_INVALID_FILE_STRING );
               break;
            }
            /* close the file and set ptr to NULL */
            fclose( RexxCURLData->FilePtrs[opt] );
            break;
         }
         RexxCURLData->FilePtrs[opt] = NULL;

         /* copy the arg to pass to cURL */
         if ( RexxCURLData->StringPtrs[opt] )
            free( RexxCURLData->StringPtrs[opt] );
         RexxCURLData->StringPtrs[opt] = (char *)malloc( argv[2].strlength + 1 );
         if ( RexxCURLData->StringPtrs[opt] == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            break;
         }
         memcpy( RexxCURLData->StringPtrs[opt], argv[2].strptr, argv[2].strlength );
         RexxCURLData->StringPtrs[opt][argv[2].strlength] = '\0';
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->StringPtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }

         break;
      case RXCURLOPT_INSTEM:
         /*
          * Check that arg[2] is a stem, then convert the stem into
          * a string of values
          */
         if ( argv[2].strlength
         &&   argv[2].strptr[argv[2].strlength-1] != '.' )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         if ( GetRexxVariableInteger( RxPackageGlobalData, argv[2].strptr, &sub_opt, 0 ) == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         /*
          * Reset the rxstring ptr if previously set
          */
         if ( RexxCURLData->instem_options[opt] )
         {
            if (RexxCURLData->instem_options[opt]->str.strptr)
            {
               free( RexxCURLData->instem_options[opt]->str.strptr );
            }
            RexxCURLData->instem_options[opt]->str.strlength = 0;
            RexxCURLData->instem_options[opt]->str.strptr = NULL;
            RexxCURLData->instem_options[opt]->str.strlength = 0;
            RexxCURLData->instem_options[opt]->index = 0;
            free( RexxCURLData->instem_options[opt] );
            RexxCURLData->instem_options[opt] = NULL;
         }
         /*
          * Allocate new instem_option
          */
         RexxCURLData->instem_options[opt] = (instem_option *)malloc( sizeof(instem_option) );
         if ( RexxCURLData->instem_options[opt] == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            break;
         }

         for ( i = 1; i <= sub_opt; i++ )
         {
            if ( GetRexxVariable( RxPackageGlobalData, argv[2].strptr, &value, i ) == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
            if ( RexxCURLData->instem_options[opt]->str.strptr == NULL )
            {
               RexxCURLData->instem_options[opt]->str.strptr = (char *)malloc( value.strlength + 2 );
               if ( RexxCURLData->instem_options[opt]->str.strptr == NULL )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
                  break;
               }
               strcpy( RexxCURLData->instem_options[opt]->str.strptr, "" );
               RexxCURLData->instem_options[opt]->str.strlength = 0;
               RexxCURLData->instem_options[opt]->index = 0;
               RexxCURLData->instem_options[opt]->RxPackageGlobalData = RxPackageGlobalData;
//               tmp = RexxCURLData->RxStrings[opt].strptr;
            }
            else
            {
               RexxCURLData->instem_options[opt]->str.strptr = (char *)realloc( RexxCURLData->instem_options[opt]->str.strptr, RexxCURLData->instem_options[opt]->str.strlength + value.strlength + 2);
               if ( RexxCURLData->instem_options[opt]->str.strptr == NULL )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
                  break;
               }
            }
            strcat( RexxCURLData->instem_options[opt]->str.strptr, value.strptr );
            RexxCURLData->instem_options[opt]->str.strlength += value.strlength;
            /*
             * Free the memory allocated in GetRexxVariable()
             */
            FREE_TSD( RxPackageGlobalData, value.strptr );
         }
         InternalTrace( RxPackageGlobalData, "CurlSetopt(RXCURLOPT_INSTEM)", "Value: <%s>", (RexxCURLData->instem_options[opt]->str.strptr) ? RexxCURLData->instem_options[opt]->str.strptr : "" );

         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_POSTFIELDSIZE, RexxCURLData->instem_options[opt]->str.strlength );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_READFUNCTION, stem_read_function );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_READDATA, RexxCURLData->instem_options[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_LONG:
         if ( RxStrToLong( RxPackageGlobalData, &argv[2], &long_opt ) != 0 )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_NUMBER, INTERR_INVALID_NUMBER_STRING );
            break;
         }
         FunctionTrace( RxPackageGlobalData, "CurlSetopt", "Setting LONG value: %ld", long_opt );
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, long_opt );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_LONGLONG:
      {
         CURLoption opt_num;
         int opt_long_long = 0;

         /*
          * Do we try and compare the passed value against MAX_LONG and call
          * the LONG option setting instead of the LARGE option???????
          */
         opt_num = RexxCurlOptions[opt].number;
         switch ( RexxCurlOptions[opt].number )
         {
#if LIBCURL_VERSION_NUM >= 0x070b00
            case CURLOPT_RESUME_FROM:
               opt_num = CURLOPT_RESUME_FROM_LARGE;
               opt_long_long = 1;
               break;
#endif
#if LIBCURL_VERSION_NUM >= 0x070a09
            case CURLOPT_MAXFILESIZE:
               opt_num = CURLOPT_MAXFILESIZE_LARGE;
               opt_long_long = 1;
               break;
#endif
#if LIBCURL_VERSION_NUM >= 0x070e05
            case CURLOPT_MAX_SEND_SPEED_LARGE:
               opt_long_long = 1;
               break;
#endif
#if LIBCURL_VERSION_NUM >= 0x070e05
            case CURLOPT_MAX_RECV_SPEED_LARGE:
               opt_long_long = 1;
               break;
#endif
            default:
               break;
         }
         if ( opt_long_long )
         {
            if ( RxStrToLongLong( RxPackageGlobalData, &argv[2], &longlong_opt ) != 0 )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_NUMBER, INTERR_INVALID_NUMBER_STRING );
               break;
            }
            sprintf( tmp_format, "Setting LONGLONG value: %s", RX_LL_FORMAT );
            FunctionTrace( RxPackageGlobalData, "CurlSetopt", tmp_format, longlong_opt );
            rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, opt_num, longlong_opt );
         }
         else
         {
            if ( RxStrToLong( RxPackageGlobalData, &argv[2], &long_opt ) != 0 )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_NUMBER, INTERR_INVALID_NUMBER_STRING );
               break;
            }
            FunctionTrace( RxPackageGlobalData, "CurlSetopt", "Setting LONG(LONG) value: %ld", long_opt );
            rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, opt_num, long_opt );
         }
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      }
      case RXCURLOPT_BOOL:
         if ( RxStrToBool( RxPackageGlobalData, &argv[2], (unsigned long *)&long_opt ) != 0 )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_BOOL, INTERR_INVALID_BOOL_STRING );
            break;
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, long_opt );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_BITMAP_AUTH:
      case RXCURLOPT_BITMAP_REDIR:
      case RXCURLOPT_BITMAP_SSHAUTH:
      case RXCURLOPT_FTPSSL:
      case RXCURLOPT_FTPSSLAUTH:
      case RXCURLOPT_FTPSSLCCC:
      case RXCURLOPT_SSL_OPTIONS:
         long_opt = 0L;
         for ( i = 2; i < (int)argc; i++ )
         {
            sub_opt = find_suboption( RxPackageGlobalData, argv[i].strptr, argv[i].strlength, RexxCurlOptions[opt].optiontype );
            if ( sub_opt == (-1) )
            {
               char buf[1024];
               sprintf( buf, "%s: %s", INTERR_INVALID_OPTION_STRING, argv[i].strptr );
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_OPTION, buf );
               break;
            }
            long_opt |= RexxCurlSubOptions[sub_opt].number;
         }
         if ( sub_opt == (-1) )
            break;
         FunctionTrace( RxPackageGlobalData, "CurlSetopt", "Setting BITMAP value: %x", long_opt );
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, long_opt );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_POLICY:
      case RXCURLOPT_PROXYTYPE:
      case RXCURLOPT_HTTP_VERSION:
      case RXCURLOPT_NETRC:
      case RXCURLOPT_TIMECOND:
      case RXCURLOPT_IPRESOLVE:
      case RXCURLOPT_FTP_CREATE_MISSING_DIRS:
      case RXCURLOPT_RTSP_REQUEST:
      case RXCURLOPT_PROTOCOLS:
         /*
          * This handles sub-options that result in a long value passed to CURL_EASY_SETOPT()
          */
         sub_opt = find_suboption( RxPackageGlobalData, argv[2].strptr, argv[2].strlength, RexxCurlOptions[opt].optiontype );
         if ( sub_opt == (-1) )
         {
            char buf[1024];
            sprintf( buf, "%s: %s", INTERR_INVALID_OPTION_STRING, argv[2].strptr );
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_OPTION, buf );
            break;
         }
         long_opt = RexxCurlSubOptions[sub_opt].number;
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, long_opt );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_LIST:
         /*
          * Check that arg[2] is a stem, then convert the stem into
          * a cURL linked list
          */
         if ( argv[2].strptr[argv[2].strlength-1] != '.' )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
/* can't do this on ooRexx 4.x as strptr is const         argv[2].strptr[argv[2].strlength] = '\0'; */
         if ( GetRexxVariableInteger( RxPackageGlobalData, argv[2].strptr, &sub_opt, 0 ) == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         if ( RexxCURLData->SListPtrs[opt] )
         {
            CURL_SLIST_FREE_ALL( RexxCURLData->SListPtrs[opt] );
            RexxCURLData->SListPtrs[opt] = NULL;
         }
         for ( i = 1; i <= sub_opt; i++ )
         {
            if ( GetRexxVariable( RxPackageGlobalData, argv[2].strptr, &value, i ) == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
            InternalTrace( RxPackageGlobalData, "CurlSetopt(RXCURLOPT_LIST)", "Variable: <%s%d> Value: <%s>", argv[2].strptr, i, value.strptr );
            RexxCURLData->SListPtrs[opt] = CURL_SLIST_APPEND( RexxCURLData->SListPtrs[opt], value.strptr );
            /*
             * Free the memory allocated in GetRexxVariable()
             */
            FREE_TSD( RxPackageGlobalData, value.strptr );
            if ( RexxCURLData->SListPtrs[opt] == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->SListPtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_POST_DATA:
         /*
          * Check that arg[2] is a stem, then convert the stem into a name/value pair for formdata
          */
         if ( argv[2].strlength
         &&   argv[2].strptr[argv[2].strlength-1] != '.' )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
/* can't do this on ooRexx 4.x as strptr is const         argv[2].strptr[argv[2].strlength] = '\0'; */
         if ( GetRexxVariableInteger( RxPackageGlobalData, argv[2].strptr, &sub_opt, 0 ) == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         if ( RexxCURLData->HttpPostFirstPtrs[opt] )
         {
            CURL_FORMFREE( RexxCURLData->HttpPostFirstPtrs[opt] );
         }
         RexxCURLData->HttpPostFirstPtrs[opt] = NULL;
         RexxCURLData->HttpPostLastPtrs[opt] = NULL;
         for ( i = 1; i <= sub_opt; i++ )
         {
            char *fd_name=NULL, *fd_value=NULL;
            if ( GetRexxVariable( RxPackageGlobalData, argv[2].strptr, &value, i ) == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
            fd_name = value.strptr;
            for ( j = 0; j < value.strlength; j++ )
            {
               if ( value.strptr[j] == '=' )
               {
                  value.strptr[j] = '\0';
                  fd_value = &value.strptr[j+1];
                  break;
               }
            }
            if ( fd_value == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_STEM_VALUE, INTERR_STEM_VALUE_STRING );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
            InternalTrace( RxPackageGlobalData, "CurlSetopt(RXCURLOPT_POST_DATA)", "Variable: <%s%d> Name: <%s> Value: <%s>", argv[2].strptr, i, fd_name, fd_value );
            rc = curlformadd_rc = CURL_FORMADD( &RexxCURLData->HttpPostFirstPtrs[opt], &RexxCURLData->HttpPostLastPtrs[opt], CURLFORM_COPYNAME, fd_name, CURLFORM_COPYCONTENTS, fd_value, CURLFORM_CONTENTSLENGTH, strlen(fd_value), CURLFORM_END );
            /*
             * Free the memory allocated in GetRexxVariable()
             */
            FREE_TSD( RxPackageGlobalData, value.strptr );
            if ( curlformadd_rc != CURL_FORMADD_OK )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
               SetCURLError( RxPackageGlobalData,  (CURLcode)curlformadd_rc, curl_formadd_errors[rc] );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->HttpPostFirstPtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_POST_FORM:
         /*
          * Always look for HTTPPOSTFORM option
          */
         opt = find_option( RxPackageGlobalData, "HTTPPOSTFORM", 12 );
         if ( opt == (-1) )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_OPTION, INTERR_INVALID_OPTION_STRING );
            return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
         }
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->HttpPostFirstPtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_POST_FIELDS:
         /*
          * Check that arg[2] is a stem, then convert the stem into
          * a string of values
          */
         if ( argv[2].strlength
         &&   argv[2].strptr[argv[2].strlength-1] != '.' )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
/* can't do this on ooRexx 4.x as strptr is const         argv[2].strptr[argv[2].strlength] = '\0'; */
         if ( GetRexxVariableInteger( RxPackageGlobalData, argv[2].strptr, &sub_opt, 0 ) == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         /*
          * Reset the string ptr if previously set
          */
         if ( RexxCURLData->StringPtrs[opt] )
         {
            free( RexxCURLData->StringPtrs[opt] );
            RexxCURLData->StringPtrs[opt] = NULL;
         }
         for ( i = 1; i <= sub_opt; i++ )
         {
            if ( GetRexxVariable( RxPackageGlobalData, argv[2].strptr, &value, i ) == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
            if ( RexxCURLData->StringPtrs[opt] == NULL )
            {
               RexxCURLData->StringPtrs[opt] = (char *)malloc( value.strlength + 2 );
               if ( RexxCURLData->StringPtrs[opt] == NULL )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
                  break;
               }
               strcpy( RexxCURLData->StringPtrs[opt], "" );
               tmp = RexxCURLData->StringPtrs[opt];
            }
            else
            {
               RexxCURLData->StringPtrs[opt] = (char *)realloc( RexxCURLData->StringPtrs[opt], strlen( RexxCURLData->StringPtrs[opt]) + value.strlength + 2 );
               if ( RexxCURLData->StringPtrs[opt] == NULL )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
                  break;
               }
            }
            /*
             * Change of behaviour!  The data should posted shold NOT be changed from what
             * the user specifies. To support previous versions, allow optional 3rd arg of
             * '&' to prepend '&' between stem values
             */
            if ( argc > 3
            &&   argv[3].strlength > 0 )
            {
               /* Concatenate argv[3].strptr and the new value to the previous string value
                * Don't prepend for first option
                */
               if ( i != 1 )
               {
                  strcat( RexxCURLData->StringPtrs[opt], argv[3].strptr );
                  count += argv[3].strlength;
               }
            }
            strcat( RexxCURLData->StringPtrs[opt], value.strptr );
            count += value.strlength;
            /*
             * Free the memory allocated in GetRexxVariable()
             */
            FREE_TSD( RxPackageGlobalData, value.strptr );
         }
         InternalTrace( RxPackageGlobalData, "CurlSetopt(RXCURLOPT_POST_FIELDS)", "Value: <%s> Length: %d", (RexxCURLData->StringPtrs[opt]) ? RexxCURLData->StringPtrs[opt] : "", count );
         /*
          * Set the length of the generated string...
          */
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_POSTFIELDSIZE, count );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         /*
          * Set the string
          */
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, RexxCurlOptions[opt].number, RexxCURLData->StringPtrs[opt] );
//         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_COPYPOSTFIELDS, RexxCURLData->StringPtrs[opt] );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_OUTSTEM:
         /*
          * Check that arg[2] is a stem, then setup the write callback
          */
         if ( argv[2].strlength
         &&   argv[2].strptr[argv[2].strlength-1] != '.' )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         /*
          * Save the stem name for the write function...
          */
         if ( RexxCURLData->StringPtrs[opt] )
            free( RexxCURLData->StringPtrs[opt] );
         RexxCURLData->StringPtrs[opt] = (char *)malloc( argv[2].strlength + 1 );
         if ( RexxCURLData->StringPtrs[opt] == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            break;
         }
         memcpy( RexxCURLData->StringPtrs[opt], argv[2].strptr, argv[2].strlength );
         RexxCURLData->StringPtrs[opt][argv[2].strlength] = '\0';
         /*
          * Set these globals to allow the write function to know which
          * compound variable to write.
          */
         RexxCURLData->outstem_index = opt;
         RexxCURLData->outstem_tail = 0;
         if ( RexxCURLData->outstem_strptr )
            free( RexxCURLData->outstem_strptr );
         RexxCURLData->outstem_strlength = 0;
         if ( RexxCURLData->outstem_line_terminator )
            free( RexxCURLData->outstem_line_terminator );
         RexxCURLData->outstem_line_terminator = 0;
         /*
          * If we have the optional line terminator (argument 3), allocate space here
          * and copy it.  No terminator, default to LF (which was original terminator)
          */
         if ( argc == 4
         &&   argv[3].strlength )
         {
            ltlen = argv[3].strlength;
            ltstr = argv[3].strptr;
         }
         else
         {
            ltlen = strlen( eol );
            ltstr = eol;
         }
         RexxCURLData->outstem_line_terminator = malloc( ltlen + 1 );
         if ( RexxCURLData->outstem_line_terminator == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            break;
         }
         strcpy( RexxCURLData->outstem_line_terminator, ltstr );
         /*
          * Set our write function to store the content
          */
         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_WRITEFUNCTION, &outstem_write_function );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_HEADERSTEM:
         /*
          * Check that arg[2] is a stem, then setup the write callback
          */
         if ( argv[2].strlength
         &&   argv[2].strptr[argv[2].strlength-1] != '.' )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         /*
          * Save the stem name for the write function...
          */
         if ( RexxCURLData->StringPtrs[opt] )
            free( RexxCURLData->StringPtrs[opt] );
         RexxCURLData->StringPtrs[opt] = (char *)malloc( argv[2].strlength + 1 );
         if ( RexxCURLData->StringPtrs[opt] == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            break;
         }
         memcpy( RexxCURLData->StringPtrs[opt], argv[2].strptr, argv[2].strlength );
         RexxCURLData->StringPtrs[opt][argv[2].strlength] = '\0';
         /*
          * Set these globals to allow the write function to know which
          * compound variable to write.
          */
         RexxCURLData->headerstem_index = opt;
         RexxCURLData->headerstem_tail = 0;

         rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_HEADERFUNCTION, &headerstem_write_function );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         break;
      case RXCURLOPT_CALLBACK:
         if ( RexxCURLData->StringPtrs[opt] )
            free( RexxCURLData->StringPtrs[opt] );
         RexxCURLData->StringPtrs[opt] = (char *)malloc( argv[2].strlength + 1 );
         if ( RexxCURLData->StringPtrs[opt] == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            break;
         }
         memcpy( RexxCURLData->StringPtrs[opt], argv[2].strptr, argv[2].strlength );
         RexxCURLData->StringPtrs[opt][argv[2].strlength] = '\0';
         /*
          * Do any extra processing here for some options
          */
         switch( RexxCurlOptions[opt].number )
         {
            case CURLOPT_PROGRESSFUNCTION:
               rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_NOPROGRESS, 0 );
               if ( curl_rc != CURLE_OK )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                  SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                  break;
               }
               rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_PROGRESSFUNCTION, &rexxcurl_progress_callback );
               if ( curl_rc != CURLE_OK )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                  SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                  break;
               }
               rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_PROGRESSDATA, RxPackageGlobalData );
               if ( curl_rc != CURLE_OK )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                  SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                  break;
               }
               RexxCURLData->progress_index = opt;
               break;
            case CURLOPT_SSH_KEYFUNCTION:
               rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_SSH_KEYFUNCTION, &rexxcurl_sshkey_callback );
               if ( curl_rc != CURLE_OK )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                  SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                  break;
               }
               rc = curl_rc = CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_SSH_KEYDATA, RxPackageGlobalData );
               if ( curl_rc != CURLE_OK )
               {
                  SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
                  SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
                  break;
               }
               RexxCURLData->progress_index = opt;
               break;
            default:
               break;
         }
         break;
      default:
         /* error */
         break;
   }
   return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
}

rxfunc( CurlReset )
{
   ULONG rc = 0L;
   long curl;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 1, 1 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   CURL_EASY_RESET( (CURL *)curl );
   return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
}

/*
 * Equivalent of curl_formadd()
 * Args:
 *   curl handle
 *   option - COPYCONTENTS or FILECONTENTS
 *   name of section
 *   data for COPYCONTENTS or multiple content type/filename for FILECONTENTS
 *
 *   call curlformadd curl, 'COPYCONTENTS', 'htmlcode', 'text/html', mydata
 *   call curlformadd curl, 'FILE', 'moreimages', 'image/jpeg', 'pic1.jpg', , 'pic2.jpg'
 *   content.1 = 'image/jpeg'; filename.1 = 'pic1.jpg'
 *   content.2 = 'text/html' ; filename.2 = 'index.html'
 *   content.2 = ''          ; filename.2 = 'pic2.jpg'
 *   content.0 = 3; filename.0 = 3
 *   call curlformadd curl, 'FILE', 'imagesandhtml', 'content.', 'filename.'
 *   call curlsetopt curl, 'HTTPPOSTFORM'
 *   call curlperform curl
 *   call curlformfree curl


array:
  i=0 content.1 -> cf[0].option = CONTENTYTPE; cf[0].value = content.1
  i=0 filename.1 -> cf[1].option = CONTENTYTPE; cf[1].value = filename.1
  i=1 content.2 -> cf[2].option = CONTENTYTPE; cf[2].value = content.1
  i=1 filename.2 -> cf[3].option = CONTENTYTPE; cf[3].value = filename.1
 */
rxfunc( CurlFormAdd )
{
   ULONG rc = 0L;
   long curl;
   int i,opt;
   CURLFORMcode curl_rc;
   int count,idx,num_buffers,contenttype;
   struct curl_forms *cf;
   RXSTRING *values;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 5, 0 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   /*
    * Always look for HTTPPOSTFORM option
    */
   opt = find_option( RxPackageGlobalData, "HTTPPOSTFORM", 12 );
   if ( opt == (-1) )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_OPTION, INTERR_INVALID_OPTION_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   if ( memcmpi( RxPackageGlobalData, "COPYCONTENTS", argv[1].strptr, argv[1].strlength ) == 0 )
   {
      if ( !RXVALIDSTRING( argv[2] ) )
      {
         SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_MANDATORY_FIELD, INTERR_MANDATORY_FIELD_STRING );
         return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
      }
      InternalTrace( RxPackageGlobalData, "CurlFormAdd(COPYCONTENTS)", "Name: <%s> ContentType: <%s> Value: <%s>", argv[2].strptr, argv[3].strptr, argv[4].strptr );
      rc = curl_rc = CURL_FORMADD( &RexxCURLData->HttpPostFirstPtrs[opt],
                                   &RexxCURLData->HttpPostLastPtrs[opt],
                                   CURLFORM_COPYNAME,
                                   argv[2].strptr,
                                   CURLFORM_NAMELENGTH,
                                   argv[2].strlength,
                                   CURLFORM_CONTENTTYPE,
                                   argv[3].strptr,
                                   CURLFORM_COPYCONTENTS,
                                   argv[4].strptr,
                                   CURLFORM_CONTENTSLENGTH,
                                   argv[4].strlength,
                                   CURLFORM_END );
      if ( curl_rc != CURLE_OK )
      {
         SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
         SetCURLError( RxPackageGlobalData,  (CURLcode)curl_rc, curl_formadd_errors[rc] );
         return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
      }
   }
   else if ( memcmpi( RxPackageGlobalData, "FILE", argv[1].strptr, argv[1].strlength ) == 0 )
   {
      /*
       * Check if we are using the array option
       */
      if ( argc == 5
      &&   argv[3].strptr[argv[3].strlength-1] == '.'
      &&   argv[4].strptr[argv[4].strlength-1] == '.' )
      {
         /*
          * arg[3] is array of contenttype, arg[4] is array of filenames
          */
         if ( GetRexxVariableInteger( RxPackageGlobalData, argv[3].strptr, &count, 0 ) == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
         }
         num_buffers = 1 + (count * 2);
         /*
          * Get an array of curl_forms structures; two for each stem item
          */
         cf = (struct curl_forms *)malloc( num_buffers*sizeof(struct curl_forms) );
         if ( cf == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
         }
         /*
          * Get an array of RXSTRINGs; one for each stem item
          */
         values = (RXSTRING *)malloc( count*sizeof(RXSTRING) );
         if ( values == NULL )
         {
            free( cf );
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
         }

         for ( idx = 0, i = 0; i < count; i++ )
         {
            /*
             * Get the contenttype stem value
             */
            if ( GetRexxVariable( RxPackageGlobalData, argv[3].strptr, &values[i], i+1 ) == NULL )
            {
               free( cf );
               free( values );
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
            /*
             * If the contenttype value is empty, do not add the option
             */
            if ( values[i].strlength )
            {
               cf[idx].value = values[i].strptr;
               cf[idx].option = CURLFORM_CONTENTTYPE;
               InternalTrace( RxPackageGlobalData, "CurlFormAdd", "Setting Contenttype %d Option %d Value: [%s](%d)",
                              i, idx, cf[idx].value, cf[idx].option );
               idx++;
            }
            /*
             * Get the filename stem value
             */
            if ( GetRexxVariable( RxPackageGlobalData, argv[4].strptr, &values[i], i+1 ) == NULL )
            {
               free( cf );
               free( values );
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
               return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
            }
            cf[idx].option = CURLFORM_FILE;
            cf[idx].value = values[i].strptr;
            InternalTrace( RxPackageGlobalData, "CurlFormAdd", "Setting Filename %d Option %d Value: [%s](%d)",
                           i, idx, cf[idx].value, cf[idx].option );
            idx++;
         }
         cf[idx].option = CURLFORM_END;
         cf[idx].value = NULL;
         InternalTrace( RxPackageGlobalData, "CurlFormAdd", "Executing curl_formadd. Name: [%s]%d",
                           argv[2].strptr, argv[2].strlength );
         rc = curl_rc = CURL_FORMADD( &RexxCURLData->HttpPostFirstPtrs[opt],
                                      &RexxCURLData->HttpPostLastPtrs[opt],
                                      CURLFORM_COPYNAME,
                                      argv[2].strptr,
                                      CURLFORM_NAMELENGTH,
                                      argv[2].strlength,
                                      CURLFORM_ARRAY,
                                      cf,
                                      CURLFORM_END );
         /*
          * Free the memory allocated in GetRexxVariable()
          */
         for ( i = 0 ; i < count ; i++ )
         {
            FREE_TSD( RxPackageGlobalData, values[i].strptr );
         }
         free( cf );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  (CURLcode)curl_rc, curl_formadd_errors[rc] );
            return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
         }
      }
      else
      {
         /*
          * We can have any number of pairs of options after the 3rd mandatory option
          */
         if ( argc % 2 == 0 )
         {
            return 1;
         }
         /*
          * Number of file pairs
          */
         count = ( argc - 3 ) / 2;
         num_buffers = 1 + (count * 2);
         /*
          * Get an array of curl_forms structures; one for each stem item
          */
         cf = (struct curl_forms *)malloc( num_buffers*sizeof(struct curl_forms) );
         if ( cf == NULL )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
         }
         /*
          * Get an array of RXSTRINGs; one for each stem item
          */
         values = (RXSTRING *)malloc( count*sizeof(RXSTRING) );
         if ( values == NULL )
         {
            free( cf );
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
            return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
         }
         contenttype = 1;
         for ( idx = 0, i = 3; i < argc; i++ )
         {
            /*
             * If the contenttype value is empty, do not add the option
             */
            if ( contenttype )
            {
               if ( RXVALIDSTRING( argv[i] ) )
               {
                  cf[idx].value = argv[i].strptr;
                  cf[idx].option = CURLFORM_CONTENTTYPE;
                  InternalTrace( RxPackageGlobalData, "CurlFormAdd", "Setting Contenttype %d Option %d Value: [%s](%d)",
                                 i, idx, cf[idx].value, cf[idx].option );
                  idx++;
               }
               contenttype = 0;
            }
            else
            {
               cf[idx].option = CURLFORM_FILE;
               cf[idx].value = argv[i].strptr;
               InternalTrace( RxPackageGlobalData, "CurlFormAdd", "Setting Filename %d Option %d Value: [%s](%d)",
                              i, idx, cf[idx].value, cf[idx].option );
               idx++;
               contenttype = 1;
            }
         }
         cf[idx].option = CURLFORM_END;
         cf[idx].value = NULL;
         InternalTrace( RxPackageGlobalData, "CurlFormAdd", "Executing curl_formadd. Name: [%s]%d",
                           argv[2].strptr, argv[2].strlength );
         rc = curl_rc = CURL_FORMADD( &RexxCURLData->HttpPostFirstPtrs[opt],
                                      &RexxCURLData->HttpPostLastPtrs[opt],
                                      CURLFORM_COPYNAME,
                                      argv[2].strptr,
                                      CURLFORM_NAMELENGTH,
                                      argv[2].strlength,
                                      CURLFORM_ARRAY,
                                      cf,
                                      CURLFORM_END );
         /*
          * Free the memory allocated in GetRexxVariable()
          */
         for ( i = 0 ; i < count ; i++ )
         {
            FREE_TSD( RxPackageGlobalData, values[i].strptr );
         }
         free( cf );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  (CURLcode)curl_rc, curl_formadd_errors[rc] );
            return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
         }
      }
   }
   else
   {
      char buf[1024];
      sprintf( buf, "%s: %s", INTERR_INVALID_OPTION_STRING, argv[1].strptr );
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_OPTION, buf );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
}

rxfunc( CurlPerform )
{
   long curl;
   CURLcode rc;
   char value[20]; /* big enough for an int */
   int valuelen,i;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 1, 1 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   /*
    * Set the CURLOPT_ERRORBUFFER here to ensure we get a string
    * error if something goes wrong.
    */
   strcpy( RexxCURLData->curl_error, "" );
   CURL_EASY_SETOPT( (CURL *)curl, CURLOPT_ERRORBUFFER, RexxCURLData->curl_error );
   InternalTrace( RxPackageGlobalData, "CurlPerform", "Set ERRORBUFFER OK" );
   RexxCURLData->g_curl_error = rc = CURL_EASY_PERFORM( (CURL *)curl );
   if ( rc != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
      SetCURLError( RxPackageGlobalData,  rc, (char *)RexxCURLData->curl_error );
   }
   InternalTrace( RxPackageGlobalData, "CurlPerform", "curl_easy_perform exited with %d", rc );
   /*
    * For those options that return their data in a stem, we need to set
    * the stem.0 value to the number of stem variables actually created.
    */
   if ( RexxCURLData->outstem_index )
   {
      InternalTrace( RxPackageGlobalData, "CurlPerform", "Using OUTSTEM" );
      outstem_create( RxPackageGlobalData );
      valuelen = sprintf( value, "%d", RexxCURLData->outstem_tail );
      create_rexx_compound( RxPackageGlobalData, RexxCURLData->StringPtrs[RexxCURLData->outstem_index], 0, value, valuelen );
      RexxCURLData->outstem_index = RexxCURLData->outstem_tail = RexxCURLData->outstem_strlength = 0;
      if ( RexxCURLData->outstem_strptr )
      {
         free( RexxCURLData->outstem_strptr );
         RexxCURLData->outstem_strptr = NULL;
         RexxCURLData->outstem_strlength = 0;
      }
      if ( RexxCURLData->outstem_line_terminator )
      {
         free( RexxCURLData->outstem_line_terminator );
         RexxCURLData->outstem_line_terminator = NULL;
      }
   }
   if ( RexxCURLData->headerstem_index )
   {
      InternalTrace( RxPackageGlobalData, "CurlPerform", "Using HEADERSTEM" );
      valuelen = sprintf( value, "%d", RexxCURLData->headerstem_tail );
      create_rexx_compound( RxPackageGlobalData, RexxCURLData->StringPtrs[RexxCURLData->headerstem_index], 0, value, valuelen );
      RexxCURLData->headerstem_index = RexxCURLData->headerstem_tail = 0;
   }
   /*
    * If OUTFILE or INFILE is used, close it here and set the FILE * to NULL
    */
   for ( i = 0; i < NUMBER_REXXCURL_OPTIONS; i++ )
   {
      if ( ( RexxCurlOptions[i].optiontype == RXCURLOPT_OUTFILE
         ||  RexxCurlOptions[i].optiontype == RXCURLOPT_INFILE )
      &&   RexxCURLData->FilePtrs[i] )
      {
         fclose( RexxCURLData->FilePtrs[i] );
         RexxCURLData->FilePtrs[i] = NULL;
      }
   }
   return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
}

rxfunc( CurlGetinfo )
{
   ULONG rc = 0L;
   CURLcode curl_rc;
   long curl;
   int opt,suboption;
   int idx = 0;
   char *return_string = NULL;
   double return_double;
   long return_long;
   struct curl_slist *TmpSListPtrs = NULL;
   struct curl_slist *OrigSListPtrs=NULL;
#if LIBCURL_VERSION_NUM >= 0x071301
   struct curl_certinfo *TmpCertInfoPtrs = NULL;
#endif
   char value[20]; /* big enough for an int */
   int valuelen,i;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 2, 3 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   opt = find_getinfo( RxPackageGlobalData, argv[1].strptr, argv[1].strlength );
   if ( opt == (-1) )
   {
      char buf[1024];
      sprintf( buf, "%s: %s", INTERR_INVALID_OPTION_STRING, argv[1].strptr );
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_OPTION, buf );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   switch( RexxCurlGetinfos[opt].optiontype )
   {
      case RXCURLINFO_STRING:
         rc = curl_rc = CURL_EASY_GETINFO( (CURL *)curl, RexxCurlGetinfos[opt].number, &return_string );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         return RxReturnString( RxPackageGlobalData, retstr, return_string );
         break;
      case RXCURLINFO_DOUBLE:
         rc = curl_rc = CURL_EASY_GETINFO( (CURL *)curl, RexxCurlGetinfos[opt].number, &return_double );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         return RxReturnDouble( RxPackageGlobalData, retstr, return_double );
         break;
      case RXCURLINFO_LONG:
         rc = curl_rc = CURL_EASY_GETINFO( (CURL *)curl, RexxCurlGetinfos[opt].number, &return_long );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         return RxReturnNumber( RxPackageGlobalData, retstr, return_long );
         break;
      case RXCURLINFO_LIST:
         if ( argc != 3 )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_TOO_FEW_ARGS, INTERR_TOO_FEW_ARGS_STRING );
            break;
         }
         /*
          * Check that we have a stem
          */
         if ( argv[2].strptr[argv[2].strlength-1] != '.' )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         rc = curl_rc = CURL_EASY_GETINFO( (CURL *)curl, RexxCurlGetinfos[opt].number, &TmpSListPtrs );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         OrigSListPtrs = TmpSListPtrs;
         for ( i = 1; TmpSListPtrs; i++, TmpSListPtrs=TmpSListPtrs->next )
         {
            create_rexx_compound( RxPackageGlobalData, argv[2].strptr, i, TmpSListPtrs->data, strlen( TmpSListPtrs->data ) );
         }
         valuelen = sprintf( value, "%d", i-1 );
         create_rexx_compound( RxPackageGlobalData, argv[2].strptr, 0, value, valuelen );
         if ( OrigSListPtrs )
         {
            CURL_SLIST_FREE_ALL( OrigSListPtrs );
         }
         return RxReturnNumber( RxPackageGlobalData, retstr, i-1 );
         break;
#if LIBCURL_VERSION_NUM >= 0x071301
      case RXCURLINFO_CERTINFO:
         if ( argc != 3 )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_TOO_FEW_ARGS, INTERR_TOO_FEW_ARGS_STRING );
            break;
         }
         /*
          * Check that we have a stem
          */
         if ( argv[2].strptr[argv[2].strlength-1] != '.' )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_STEM, INTERR_INVALID_STEM_STRING );
            break;
         }
         rc = curl_rc = CURL_EASY_GETINFO( (CURL *)curl, RexxCurlGetinfos[opt].number, &TmpCertInfoPtrs );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         if ( TmpCertInfoPtrs )
         {
            for ( i = 0; i < TmpCertInfoPtrs->num_of_certs; i++ )
            {
               TmpSListPtrs = TmpCertInfoPtrs->certinfo[i];
               for ( ; TmpSListPtrs; TmpSListPtrs=TmpSListPtrs->next )
               {
                  idx++;
                  create_rexx_compound( RxPackageGlobalData, argv[2].strptr, idx, TmpSListPtrs->data, strlen( TmpSListPtrs->data ) );
               }
               /* don't free the slist!! */
            }
         }
         valuelen = sprintf( value, "%d", idx );
         create_rexx_compound( RxPackageGlobalData, argv[2].strptr, 0, value, valuelen );
         /* don't free the TmpCertInfoPtrs */
         return RxReturnNumber( RxPackageGlobalData, retstr, idx );
         break;
#endif
      case RXCURLINFO_BITMAP:
         suboption = 0;
         rc = curl_rc = CURL_EASY_GETINFO( (CURL *)curl, RexxCurlGetinfos[opt].number, &return_long );
         if ( curl_rc != CURLE_OK )
         {
            SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_CURL_ERROR, INTERR_CURL_ERROR_STRING );
            SetCURLError( RxPackageGlobalData,  curl_rc, curl_errors[rc] );
            break;
         }
         switch( RexxCurlGetinfos[opt].number )
         {
#if LIBCURL_VERSION_NUM >= 0x070a08
            case CURLINFO_HTTPAUTH_AVAIL:
            case CURLINFO_PROXYAUTH_AVAIL:
               suboption = RXCURLOPT_BITMAP_AUTH;
               break;
#endif
            default:
               break;
         }
         if ( suboption != 0 )
         {
            valuelen = 0;
            /*
             * Get the total length of our return string
             */
            for ( idx = 0; RexxCurlSubOptions[idx].name != NULL; idx++ )
            {
               if ( RexxCurlSubOptions[idx].optiontype == suboption
               &&   RexxCurlSubOptions[idx].newname == NULL
               &&   ( RexxCurlSubOptions[idx].number & return_long ) == RexxCurlSubOptions[idx].number )
                  valuelen = valuelen + 1 + strlen( RexxCurlSubOptions[idx].name );
            }
            /* allocate our return string */
            return_string = RexxAllocateMemory( valuelen + 1 );
            if ( return_string == NULL )
            {
               SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
               break;
            }
            /*
             * Append each value to our return string
             */
            strcpy( return_string, "" );
            for ( idx = 0; RexxCurlSubOptions[idx].name != NULL; idx++ )
            {
               if ( RexxCurlSubOptions[idx].optiontype == suboption
               &&   RexxCurlSubOptions[idx].newname == NULL
               &&   ( RexxCurlSubOptions[idx].number & return_long ) == RexxCurlSubOptions[idx].number )
               {
                  strcat( return_string, RexxCurlSubOptions[idx].name );
                  strcat( return_string, " " );
               }
            }
            return RxReturnStringAndFree( RxPackageGlobalData, retstr, return_string, 1 );
         }
         break;
      default:
         /* error */
         break;
   }
   return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
}

rxfunc( CurlFormFree )
{
   long curl;
   int opt;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 1, 1 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   /*
    * Always look for HTTPPOSTFORM option
    */
   opt = find_option( RxPackageGlobalData, "HTTPPOSTFORM", 12 );
   if ( opt == (-1) )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_OPTION, INTERR_INVALID_OPTION_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   if ( RexxCURLData->HttpPostFirstPtrs[opt] )
   {
      CURL_FORMFREE( RexxCURLData->HttpPostFirstPtrs[opt] );
   }
   RexxCURLData->HttpPostFirstPtrs[opt] = NULL;
   RexxCURLData->HttpPostLastPtrs[opt] = NULL;
   return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
}

rxfunc( CurlEscape )
{
   long curl;
   int valuelen;
   char *url, *return_string;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 2, 2 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnNumber( RxPackageGlobalData, retstr, INTERR_INVALID_HANDLE ) ;
   }

   url = CURL_EASY_ESCAPE( (CURL *)curl, (char *)RXSTRPTR( argv[1] ), RXSTRLEN( argv[1] ) );
   if ( url == NULL )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   valuelen = strlen( url );
   return_string = RexxAllocateMemory( valuelen + 1 );
   if ( return_string == NULL )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   strcpy( return_string, url );
   CURL_FREE( url );
   return RxReturnStringAndFree( RxPackageGlobalData, retstr, return_string, 1 ) ;
}

rxfunc( CurlUnescape )
{
   long curl;
   int valuelen;
   char *url, *return_string;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( RexxCURLData->g_curl_error) ClearCURLError( RxPackageGlobalData );
   if ( RexxCURLData->g_rexxcurl_error) ClearIntError( RxPackageGlobalData );
   if ( my_checkparam( RxPackageGlobalData, name, argc, 2, 2 ) )
      return( 1 );
   if ( RxStrToLong( RxPackageGlobalData, &argv[0], &curl ) != 0 )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_HANDLE, INTERR_INVALID_HANDLE_STRING );
      return RxReturnNumber( RxPackageGlobalData, retstr, INTERR_INVALID_HANDLE ) ;
   }

   url = CURL_EASY_UNESCAPE( (CURL *)curl, (char *)RXSTRPTR( argv[1] ), RXSTRLEN( argv[1] ), &valuelen );
   if ( url == NULL )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   return_string = RexxAllocateMemory( valuelen + 1 );
   if ( return_string == NULL )
   {
      SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_NO_MEMORY, INTERR_NO_MEMORY_STRING );
      return RxReturnString( RxPackageGlobalData, retstr, "" ) ;
   }
   strcpy( return_string, url );
   CURL_FREE( url );
   return RxReturnStringAndFree( RxPackageGlobalData, retstr, return_string, 1 ) ;
}

/*
 * If an argument is passed, check if it is a valid function name
 * If no argument is passed, return all function names
 * Curlqueryfunction( var|stem, opt )
 * opt is R(registered), A(vailable)
 *
 * return 0 is successful - stem or func found
 * return 1 if func not found
 */
rxfunc(CurlQueryFunction)
{
   ULONG rc = 0L;
   int i,len,tmplen,idx;
   char *stem;
   char optionchar;
   char varname[255], value[10];
   char stem_name[255];
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();
   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );
   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( my_checkparam( RxPackageGlobalData, name, argc, 1, 2 ) )
      return( 1 );

   stem = RXSTRPTR(argv[0]);
   len = RXSTRLEN(argv[0]);

   if ( argc == 1 )
      optionchar = 'R';
   else
      optionchar = toupper( *argv[1].strptr );

   if ( stem[len-1] != '.' )
   {
      /*
       * Check that the specified function name is available/registered
       */
      rc = 1L; /* not found by default */
      for ( i = 0; RexxCURLFunctions[i].ExternalName != NULL; i++ )
      {
         tmplen = strlen( (char *)RexxCURLFunctions[i].ExternalName );
         if ( tmplen == len
         &&   memcmpi( RxPackageGlobalData, (char *)RexxCURLFunctions[i].ExternalName, stem, len ) == 0 )
         {
            /*
             * Function is available...
             */
            rc = 0L;
            if ( RexxQueryFunction( RexxCURLFunctions[i].InternalName )
            &&   optionchar == 'R' )
            {
               /*
                * Function is not registered and we wanted a registered function
                */
               rc = 1L;
            }
            break;
         }
      }
   }
   else
   {
      strcpy( stem_name, stem );
      make_upper( RxPackageGlobalData, stem_name );
      idx = 0;
      for ( i = 0; RexxCURLFunctions[i].ExternalName != NULL; i++ )
      {
         if ( RexxQueryFunction( RexxCURLFunctions[i].InternalName )
         &&   optionchar == 'R' )
         {
            /*
             * Function is not registered, and we wanted only registered functions
             */
            ;
         }
         else
         {
            idx++;
            len = sprintf( varname, "%s%u", stem_name, idx );
            if ( SetRexxVariable( RxPackageGlobalData, varname, len, (char *)RexxCURLFunctions[i].ExternalName, strlen( RexxCURLFunctions[i].ExternalName ) ) == 1 )
               return(1);
         }
      }
      i = sprintf( value, "%u", idx);
      len = sprintf(varname, "%s0", stem_name);
      rc = SetRexxVariable( RxPackageGlobalData, varname, len, value, i );
   }
   return RxReturnNumber( NULL, retstr, rc );
}

rxfunc( CurlVariable )
{
   RX_ULONG rc = 0L;
   char buf[250];
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();

   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );

   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( my_checkparam( RxPackageGlobalData, (char *)name, argc, 1, 2 ) )
      return( 1 );
   if ( RXSTRLEN( argv[0] ) == 9 && memcmpi( RxPackageGlobalData, "DEBUGFILE", RXSTRPTR( argv[0] ), RXSTRLEN( argv[0] ) ) == 0 )
   {
      if ( argc == 1 )
         return RxReturnString( RxPackageGlobalData, retstr, RxGetTraceFile( RxPackageGlobalData ) );
      else
      {
         rc = RxSetTraceFile( RxPackageGlobalData, (char *)RXSTRPTR( argv[1] ) );
         return RxReturnNumber( RxPackageGlobalData, retstr, rc );
      }
   }
   else if ( RXSTRLEN( argv[0] ) == 5 && memcmpi( RxPackageGlobalData, "DEBUG", RXSTRPTR( argv[0] ), RXSTRLEN( argv[0] ) ) == 0 )
   {
      if ( argc == 1 )
      {
         sprintf( buf, "%d", RxGetRunFlags( RxPackageGlobalData ) );
         return RxReturnString( RxPackageGlobalData, retstr, buf );
      }
      else
      {
         if ( RxStrToULong( RxPackageGlobalData, &argv[1], (RX_ULONG *)&rc ) == -1 )
            return RxReturnString( RxPackageGlobalData, retstr, "ERROR: Invalid DEBUG value. Cannot set variable; DEBUG" );
         RxSetRunFlags ( RxPackageGlobalData, rc );
         return RxReturnNumber( RxPackageGlobalData, retstr, 0 );
      }
   }
   else if ( RXSTRLEN( argv[0] ) == 7 && memcmpi( RxPackageGlobalData, "VERSION", RXSTRPTR( argv[0] ), RXSTRLEN( argv[0] ) ) == 0 )
   {
      if ( argc == 1 )
      {
         sprintf( buf, "%s %s %s %s", RxPackageName, REXXCURL_VERSION, REXXCURL_DATE, CURL_VERSION() );
         return RxReturnString( RxPackageGlobalData, retstr, buf );
      }
      else
         return RxReturnString( RxPackageGlobalData, retstr, "ERROR: Cannot set variable; VERSION" );
   }
   else if ( RXSTRLEN( argv[0] ) == 5 && memcmpi( RxPackageGlobalData, "ERROR", RXSTRPTR( argv[0] ), RXSTRLEN( argv[0] ) ) == 0 )
   {
      if ( argc == 1 )
      {
         return RxReturnString( RxPackageGlobalData, retstr, RexxCURLData->rexxcurl_error_prefix );
      }
      else
      {
         memcpy( RexxCURLData->rexxcurl_error_prefix, argv[1].strptr, argv[1].strlength );
         RexxCURLData->rexxcurl_error_prefix[argv[1].strlength] = '\0';
         return RxReturnString( RxPackageGlobalData, retstr, "" );
      }
   }
   else if ( RXSTRLEN(argv[0]) == 14 && memcmpi( RxPackageGlobalData, "CONSTANTPREFIX", RXSTRPTR(argv[0]), RXSTRLEN(argv[0]) ) == 0 )
   {
      if ( argc == 1 )
      {
         return RxReturnString( RxPackageGlobalData, retstr, RxGetConstantPrefix( RxPackageGlobalData ) );
      }
      else
      {
         rc = RxSetConstantPrefix( RxPackageGlobalData, (char *)argv[1].strptr );
         /*
          * Now we have a new prefix, we should reset all our constants...
          */
         SetPackageConstants( RxPackageGlobalData, RexxCURLConstants, 1 );
         /*
          * ...and our version info variables
          */
         RexxCURLSetVersionInfoConstants( RxPackageGlobalData );
         return RxReturnNumber( RxPackageGlobalData, retstr, rc );
      }
   }
   sprintf( buf, "%s %s", INTERR_INVALID_VARIABLE_STRING, argv[0].strptr );
   SetIntError( RxPackageGlobalData, __FILE__, __LINE__, INTERR_INVALID_VARIABLE, buf );
   return RxReturnString( RxPackageGlobalData, retstr, "" );
}

/*====== Routines for handling registration of functions ======*/


rxfunc( CurlLoadFuncs )
{
   int rc = 0L;

   /*
    * Get thread-safe-data for DLL. EXE global data is not thread-safe
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = GLOBAL_ENTRY_POINT();
   InitRxPackage( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, &rc );
   if ( !QueryRxFunction( RxPackageGlobalData, "CURLINIT" ) )
   {
      /*
       * Register all external functions
       */
      if ( !rc )
      {
         rc = RegisterRxFunctions( RxPackageGlobalData, RexxCURLFunctions, RxPackageName );
      }
   }
   rc = RexxCURLInitHandler( 0, 0, NULL );
   return RxReturnNumber( RxPackageGlobalData, retstr, rc );
}

rxfunc( CurlDropFuncs )
{
   int rc=0;
   int unload=0;
   REXXCURLDATA *RexxCURLData;
   /*
    * Get thread-safe-data
    */
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();

   RxPackageGlobalData = FunctionPrologue( RxPackageGlobalData, RexxCURLInitialiser, RXPACKAGE_CONSTANT_PREFIX, (char*)name, argc, argv );

   RexxCURLData=(REXXCURLDATA *)RxPackageGlobalData->RXPACKAGE_tsd;

   if ( my_checkparam( RxPackageGlobalData, (char *)name, argc, 0, 1 ) )
      return( 1 );
   if ( argv[0].strlength == 6
   &&   memcmpi( RxPackageGlobalData, argv[0].strptr, "UNLOAD", 6 ) == 0 )
      unload = 1;
   (void)TermRxPackage( RxPackageGlobalData, RexxCURLTerminator, RexxCURLFunctions, RxPackageName, unload );
   return RxReturnNumber( NULL, retstr, rc );
}



/*
 * The following functions are used in rxpackage.c
 */

/*-----------------------------------------------------------------------------
 * Print a usage message.
 *----------------------------------------------------------------------------*/
void RexxCURLUsage

#ifdef HAVE_PROTO
   (void)
#else
   ()
#endif

{
   char buf[1024];
   (void)fprintf(stderr,
      "\nVersion: %s %s %s\nUsing: %s\n\nUsage:   %s [-h]\n         %s [-idvf<trace file>] [Rexx program name]\n\n",
      RxPackageName,
      REXXCURL_VERSION,
      REXXCURL_DATE,
      RxGetRexxInterpreterVersion( buf ),
      RxPackageName,
      RxPackageName);
}


#if defined(USE_REXX6000)
/*
 * This function is used as the entry point for the REXX/6000
 * interpreter
 * If you change this table, don't forget to change the table at the
 * start of this file.
 */
USHORT InitFunc( RXFUNCBLOCK **FuncBlock )
{
   static RXFUNCBLOCK funcarray[] =
   {
      { NULL, NULL, NULL }
   } ;
   *FuncBlock = funcarray;
   return (USHORT)0;
}
#endif
