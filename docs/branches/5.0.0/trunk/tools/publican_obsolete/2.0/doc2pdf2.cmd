@if not defined echo @echo off

set docpath=ooRexxDocSVN

rem Build an ooRexx document PDF
if not defined docpath (
    echo You must specify the path to the ooRexx docs.
    echo Please run the DOCPATH command first.
    goto :eof
)

:: Uppdate SVN, assume docs in /ooRexxDocSVN
@echo Uppdating documentation using SVN: %time%
:: cd %docpath%
:: svn update
:: cd ..

@echo Starting to build %1: %time%

if [%1] NEQ [] call docprep %1
@echo Starting to build fo file for %1: %time%
call doc2fo
@echo Starting to build pdf file for %1: %time%
call fo2pdf

@echo Build finished for %1: %time%

:: Copy to Dropbox
copy /y/v out-%1\%1.pdf C:\Users\po\Dropbox\ooRexxandBSF\ooRexxDocs\oorexxDocs\PDF\