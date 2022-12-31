::  Steps to build ooRexx Documentation
::  (c) P.O. Jonsson 2020-02-23
::  Batfiles built from ooRexx Makefiles in Documentation subfolders
::  
::  Create the workbench if not already done
::  mkdir workspace
::  cd workspace
::  mkdir oorexxDocs
:: 
::  Move into the document directory to build and
::  Issue the build command; Notice the common content that is needed for all books

@echo on

if [%1]==[] echo usage: buildDoc product_name & goto :exit

set PRODUCT=%1
set PATH0=C:\Users\po\workspace\
set PATH1=%PATH0%oorexxDocSVN\
set PATH2=%PATH1%%PRODUCT%\
set PATH3=%PATH2%tmp\
set PATH4=%PATH0%oorexxDocs\


:: Update the documentation
cd %PATH1%
svn update

if exist %PATH3% ( RMDIR /S /Q %PATH3% )

@echo.
@echo Starting to build documentation for %PRODUCT%: %time%
@echo.

cd %PATH2%
publican build --formats=pdf,html --langs=en-US --Common_Content=%PATH1%
::publican build --formats=pdf --langs=en-US --Common_Content=%PATH1%
move /y %PATH3%en-US\pdf\ooRexx*.pdf %PATH4%PDF\%PRODUCT%.pdf
copy /y/v %PATH4%PDF\%PRODUCT%.pdf C:\Users\po\Dropbox\ooRexxandBSF\ooRexxDocs\oorexxDocs\PDF\

7z a -r %PATH4%HTML\%PRODUCT%.zip %PATH3%en-US\html\*.*
7z a -r C:\Users\po\Dropbox\ooRexxandBSF\ooRexxDocs\oorexxDocs\HTML\%PRODUCT%.zip %PATH3%en-US\html\*.*

if exist %PATH3% ( RMDIR /S /Q %PATH3% )

cd %PATH0%

@echo Build finished for %PRODUCT%: %time%

:exit
