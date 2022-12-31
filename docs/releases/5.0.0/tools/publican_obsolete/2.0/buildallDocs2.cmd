:: Build all manuals for ooRexx 5.0.0

@echo.
@echo Starting to launch documentation builds: %time%
@echo.

:: call doc2pdf2 buildmachine
:: call doc2pdf2 oorexx

call doc2pdf2 readme
call doc2pdf2 rexxextensions
call doc2pdf2 rexxgtk
call doc2pdf2 rexxpg
call doc2pdf2 rexxref
call doc2pdf2 rxftp
call doc2pdf2 rxmath
call doc2pdf2 rxsock
call doc2pdf2 unixextensions
call doc2pdf2 winextensions
call doc2pdf2 ooconsole
call doc2pdf2 oodguide
call doc2pdf2 oodialog
call doc2pdf2 oosqlite
call doc2pdf2 ootest
call doc2pdf2 orxncurses

@echo.
@echo Finished building documentation: %time%
@echo.
