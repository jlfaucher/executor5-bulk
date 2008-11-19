<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/common"
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
     exclude-result-prefixes="exsl"
     version="1.0">

<xsl:import href="/usr/share/sgml/docbook/xsl-stylesheets/fo/docbook.xsl" />
<xsl:import href="../shared/oorexx.fo.titlepage.xsl" />

<xsl:template match="symbol[@role = 'symbolfont']">
  <fo:inline font-family="Symbol">
     <xsl:call-template name="inline.charseq"/>
  </fo:inline>
</xsl:template>

<!-- Style sheet parameters for the ooRexx documents -->
<!-- Page Layout -->
<xsl:param name="draft.mode" select="yes" />
<!-- Chapter and Section Numbering -->
<xsl:param name="section.autolabel" select="1" />
<xsl:param name="section.label.includes.component.label" select="1" />
<!-- PDF Bookmarks -->
<xsl:param name="passivetex.extensions" select="0" />
<!-- Variable List Formatting -->
<!-- Extra Vertical Space -->
<!-- Cross Reference Page Numbers -->
<!-- DocBook Icon Graphics -->
<!-- Printing One Chapter -->

</xsl:stylesheet>
