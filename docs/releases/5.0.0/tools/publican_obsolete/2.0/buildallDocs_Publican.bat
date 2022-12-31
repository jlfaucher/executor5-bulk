@echo.
@echo Starting to launch documentation builds: %time%
@echo.

:: 2 docs Not working
:: start buildDoc buildmachine
start buildDoc ooconsole
start buildDoc oodguide
start buildDoc oodialog
:: start buildDoc oorexx
start buildDoc oosqlite
start buildDoc ootest
start buildDoc orxncurses
start buildDoc readme
start buildDoc rexxextensions
start buildDoc rexxgtk
start buildDoc rexxpg
start buildDoc rexxref
start buildDoc rxftp
start buildDoc rxmath
start buildDoc rxsock
start buildDoc unixextensions
start buildDoc winextensions

@echo.
@echo Finished launching documentation builds: %time%
@echo.
