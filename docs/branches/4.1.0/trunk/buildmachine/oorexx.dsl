<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY dbstyle SYSTEM "/usr/share/sgml/docbook/dsssl-stylesheets/print/docbook.dsl" CDATA DSSSL>
]>

<style-sheet>
<style-specification use="docbook">
<style-specification-body>

;;###########################################################################
;;#
;;#  Customizations to the standard DocBook Print Stylesheet
;;#  for the Open Object Rexx Documentation
;;#
;;#  by W. David Ashley
;;#
;;###########################################################################



;;Enumerate sections (E.g, 1.1, 1.1.1, 1.2, etc.)
(define %section-autolabel%
 #t)

;; If '%two-side%' is true, headers and footers are alternated
(define %two-side%
 #t)

;;Contents for the recto titlepage (the main page)
(define (set-titlepage-recto-elements)
  (list (normalize "title")
	(normalize "subtitle")
	(normalize "edition")
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
	(normalize "pubdate")
	(normalize "graphic")
	(normalize "mediaobject")
	(normalize "corpauthor")
	(normalize "authorgroup")
	(normalize "author")
	(normalize "editor")))

(define %mono-font-family%
  ;; This change eliminates curly double-quotes from program listings but
  ;; still leaves curly single-quotes intact. This means that all mono-space
  ;; font sections (like programlisting) need to use the apos entity for
  ;; single quotes. Otherwise the character used is not comapaable with Windows.
  "Computer-Modern-Typewriter")


</style-specification-body>
</style-specification>

<external-specification id="docbook" document="dbstyle">
</style-sheet>
