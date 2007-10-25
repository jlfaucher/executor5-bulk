<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY dbstyle SYSTEM "/usr/share/sgml/docbook/dsssl-stylesheets/html/docbook.dsl" CDATA DSSSL>
]>

<style-sheet>
<style-specification use="docbook">
<style-specification-body>

;;###########################################################################
;;#
;;#  Customizations to the standard DocBook HTML Stylesheet
;;#  for the Open Object Rexx Documentation
;;#
;;#  by W. David Ashley
;;#
;;###########################################################################



;;Contents for the recto titlepage (the main page)
(define (set-titlepage-recto-elements)
  (list (normalize "title")
	(normalize "subtitle")
	(normalize "edition")
	(normalize "releaseinfo")
	(normalize "pubdate")
	(normalize "graphic")
	(normalize "mediaobject")
	(normalize "corpauthor")
	(normalize "authorgroup")
	(normalize "author")
	(normalize "editor")))
(define (book-titlepage-recto-elements)
  (list (normalize "title")
	(normalize "subtitle")
	(normalize "edition")
	(normalize "releaseinfo")
	(normalize "pubdate")
	(normalize "graphic")
	(normalize "mediaobject")
	(normalize "corpauthor")
	(normalize "authorgroup")
	(normalize "author")
	(normalize "editor")))


</style-specification-body>
</style-specification>

<external-specification id="docbook" document="dbstyle">
</style-sheet>
