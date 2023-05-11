/*
 * Copyright (c) 2001-2007 Mark Hessling <mark@rexx.org> All rights reserved.
 * Copyright (c) 2007 Rexx Language Association. All rights reserved.
 *
 * This program and the accompanying materials are made available under
 * the terms of the Common Public License v1.0 which accompanies this
 * distribution. A copy is also available at the following address:
 * http://www.oorexx.org/license.html
 */

#define RXPACKAGE_MAGIC_NUMBER    30502
#define RXPACKAGE_DEBUG_VAR       "REXXCURL_DEBUG"
#define RXPACKAGE_CONSTANT_PREFIX "!REXXCURL.!"

#define RXPACKAGENAME             "rexxcurl"
#define GETPACKAGEINITIALISER()   getRexxCURLInitialiser()
#define GETPACKAGETERMINATOR()    getRexxCURLTerminator()
#define GETPACKAGEFUNCTIONS()     getRexxCURLFunctions()
#define GETPACKAGESUBCOMHANDLER() getRexxCURLSubcomHandler()
#define GETPACKAGECONSTANTS()     getRexxCURLConstants()
#define GETPACKAGEINITHANDLER()   getRexxCURLInitHandler()
#define PACKAGEUSAGE()            RexxCURLUsage()

