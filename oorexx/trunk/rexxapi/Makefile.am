#----------------------------------------------------------------------------
#
# Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.
#
# This program and the accompanying materials are made available under
# the terms of the Common Public License v1.0 which accompanies this
# distribution. A copy is also available at the following address:
# http://www.ibm.com/developerworks/oss/CPLv1.0.htm
#
# Redistribution and use in source and binary forms, with or
# without modification, are permitted provided that the following
# conditions are met:
#
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in
# the documentation and/or other materials provided with the distribution.
#
# Neither the name of Rexx Language Association nor the names
# of its contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#----------------------------------------------------------------------------


MAINTAINERCLEANFILES = Makefile.in

# get version details from our external file
include $(top_srcdir)/oorexx.ver

INCLUDES = @OOREXX_INCLUDES@ \
           -I$(top_srcdir)/rexxapi \
           -I$(top_srcdir)/rexxapi/api \
           -I$(top_srcdir)/rexxapi/api/platform/unix \
           -I$(top_srcdir)/rexxapi/daemon \
           -I$(top_srcdir)/rexxapi/daemon/platform/unix \
           -I$(top_srcdir)/shared \
           -I$(top_srcdir)/shared/unix


#################### librexxapi.la #########################

pkglib_LTLIBRARIES = librexxapi.la

#
# Sources for librexxapi.la
#
librexxapi_la_SOURCES = Encodings.hpp \
                        ServiceException.hpp \
                        ServiceMessage.cpp \
                        ServiceMessage.hpp \
                        api/ClientMessage.cpp \
                        api/ClientMessage.hpp \
                        api/LocalAPIContext.cpp \
                        api/LocalAPIContext.hpp \
                        api/LocalAPIManager.cpp \
                        api/LocalAPIManager.hpp \
                        api/LocalAPISubsystem.hpp \
                        api/LocalMacroSpaceManager.cpp \
                        api/LocalMacroSpaceManager.hpp \
                        api/LocalQueueManager.cpp \
                        api/LocalQueueManager.hpp \
                        api/LocalRegistrationManager.cpp \
                        api/LocalRegistrationManager.hpp \
                        api/MacroSpaceApi.cpp \
                        api/QueuesAPI.cpp \
                        api/RegistrationAPI.cpp \
                        api/RexxAPI.h \
                        api/RexxLegacyAPI.cpp \
                        api/RexxLegacyAPI.hpp \
                        api/platform/unix/SysLocalAPIManager.cpp \
                        api/platform/unix/SysLocalAPIManager.hpp

#
# C++ compiler flags for librexxapi.la sources
#
librexxapi_la_CXXFLAGS = -DORX_VER=$(MAJOR) -DORX_REL=$(MINOR) \
                         -DORX_MOD=$(MOD_LVL)

#
# linker flags for librexxapi.la objects
#
librexxapi_la_LDFLAGS = -version-info @OOREXX_LIB_VERSION@ -no-undefined \
                        @OOREXX_LIBS@

#
# additional libraries for librexxapi.la objects
#
librexxapi_la_LIBADD  = -lrxshared


#################### rxapi #########################

bin_PROGRAMS = rxapi

#
# Sources for rxapi
#
rxapi_SOURCES = Encodings.hpp \
                ServiceException.hpp \
                ServiceMessage.cpp \
                ServiceMessage.hpp \
                daemon/APIServer.cpp \
                daemon/APIServer.hpp \
                daemon/APIServerInstance.cpp \
                daemon/APIServerInstance.hpp \
                daemon/MacroSpaceManager.cpp \
                daemon/MacroSpaceManager.hpp \
                daemon/QueueManager.cpp \
                daemon/QueueManager.hpp \
                daemon/RegistrationManager.cpp \
                daemon/RegistrationManager.hpp \
                daemon/platform/unix/APIService.cpp

#
# C++ compiler flags for rxapi sources
#
rxapi_CXXFLAGS = -DORX_VER=$(MAJOR) -DORX_REL=$(MINOR) \
                 -DORX_MOD=$(MOD_LVL)

#
# additional libraries for rxapi objects
#
rxapi_LDADD  = -lrxshared -lrexxapi @OOREXX_LIBS@

#
# linker flags for rxapi objects
#
rxapi_LDFLAGS =


