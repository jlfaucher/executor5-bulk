              Readme for oorxwiki
	      -------------------

The oorxwiki package is a functioning wiki written in ooRexx. It is currently
in a very beta state and lacks a lot of the functionality that a complete wiki
package would be expected to provide. But it does supply a code base that can
be used to create your own wiki environment.

The ooRexx code for oorxwiki is composed of 4 files:

wiki.rex - This file can be invoked from the Apache web server as either a CGI
    program or via Mod_Rexx. It is the main driver for all wiki processing.

oorxwiki.cls - This is the wiki class file. It processes wiki files that are in
    MediaWiki format to produce HTML. MediaWiki is the same wiki format that
    Wikipedia uses.

templates.rex - This file contains the routines for producing different parts
    of the output HTML page. You should modify these routines for your own
    environment and HTML page look-and-feel.

oorxwikiconf.rex - This file contains the configuration information for the
    wiki.rex program. It includes information such as the location of wiki
    namespaces, the names of image files, etc. Breaking this information out
    using this technique allows for easy configuration changes when setting up
    wiki.rex in your environment.

There is also a subdirectory tree supplied with a number of wiki samples using
a snapshot of the www.oorexx.org website as an example.

To install oorxwiki there are 4 simple steps to perform.

1. Copy the subdirectory with the wiki.rex program in it (including the child
   subdirectories) to the DocumentRoot of your Apache server.

2. If you intend to run wiki.rex as a CGI program you will need to add the
   ExecCGI option to the Options directive inside the Directory directive
   for the DocumentRoot. This step is not necessary if you will be using
   Mod_Rexx to invoke wiki.rex.

3. If you intend to run wiki.rex as a CGI program you will need to add a new
   AddHandler directive like this one.

	  AddHandler cgi-script .rex

   This step is not necessary if you will be using Mod_Rexx to invoke wiki.rex.

4. Restart your Apache server.

To fetch your first wiki page you can then issue the following

          http://your.server.com/wiki.rex

This will display the wiki page located at /wikidb/wikins/Main_Page (unless you
modified the namespace oorxwiki configuration information). Editing of the
wiki page is fully supported in this version of oorxwiki but versioning of the
page is not supported yet.

Have fun!
W. David Ashley

