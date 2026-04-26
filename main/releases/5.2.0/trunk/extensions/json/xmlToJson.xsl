<?xml version="1.0" encoding="UTF-8"?>
<!--
  xmlToJson.xsl - XSLT 1.0 stylesheet to transform json.cls XML output
  back to JSON text.

  Handles both XML flavours produced by json.cls:
    - XSD flavour: elements in namespace "urn:json:xml:1.0"
    - DTD flavour: elements with no namespace

  Usage with xsltproc:
    xsltproc xmlToJson.xsl data.xml

  Copyright (c) 2024-2026 Rexx Language Association. All rights reserved.

  This program and the accompanying materials are made available under
  the terms of the Common Public License v1.0 which accompanies this
  distribution. A copy is also available at the following address:
  https://www.oorexx.org/license.html

  Redistribution and use in source and binary forms, with or
  without modification, are permitted provided that the following
  conditions are met:

  Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the distribution.

  Neither the name of Rexx Language Association nor the names
  of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-->
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:j="urn:json:xml:1.0">

  <xsl:output method="text" encoding="UTF-8"/>
  <xsl:strip-space elements="*"/>
  <xsl:preserve-space elements="j:string string j:n n"/>

  <!-- Double-quote character stored in a variable so it can be used
       in XPath expressions without quoting ambiguity (XSLT 1.0). -->
  <xsl:variable name="quot" select="'&quot;'"/>

  <!-- ================================================================ -->
  <!-- Root: match <json> (either namespace or no namespace)            -->
  <!-- ================================================================ -->
  <xsl:template match="/j:json | /json">
    <xsl:apply-templates select="*"/>
    <xsl:text>&#10;</xsl:text>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Object                                                           -->
  <!-- ================================================================ -->
  <xsl:template match="j:object | object">
    <xsl:text>{</xsl:text>
    <xsl:for-each select="j:entry | entry">
      <xsl:if test="position() > 1">
        <xsl:text>,</xsl:text>
      </xsl:if>
      <!-- Key -->
      <xsl:text>"</xsl:text>
      <xsl:call-template name="escape-json-string">
        <xsl:with-param name="val" select="j:n | n"/>
      </xsl:call-template>
      <xsl:text>":</xsl:text>
      <!-- Value -->
      <xsl:apply-templates select="(j:value | value)/*"/>
    </xsl:for-each>
    <xsl:text>}</xsl:text>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Array                                                            -->
  <!-- ================================================================ -->
  <xsl:template match="j:array | array">
    <xsl:text>[</xsl:text>
    <xsl:for-each select="j:item | item">
      <xsl:if test="position() > 1">
        <xsl:text>,</xsl:text>
      </xsl:if>
      <xsl:apply-templates select="*"/>
    </xsl:for-each>
    <xsl:text>]</xsl:text>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- String                                                           -->
  <!-- ================================================================ -->
  <xsl:template match="j:string | string">
    <xsl:text>"</xsl:text>
    <xsl:call-template name="escape-json-string">
      <xsl:with-param name="val" select="."/>
    </xsl:call-template>
    <xsl:text>"</xsl:text>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Number                                                           -->
  <!-- ================================================================ -->
  <xsl:template match="j:number | number">
    <xsl:value-of select="."/>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Boolean                                                          -->
  <!-- ================================================================ -->
  <xsl:template match="j:boolean | boolean">
    <xsl:value-of select="."/>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Null                                                             -->
  <!-- ================================================================ -->
  <xsl:template match="j:null | null">
    <xsl:text>null</xsl:text>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Named templates                                                  -->
  <!-- ================================================================ -->

  <!-- Escape a string for JSON double-quoting.
       Handles: backslash, double-quote, tab, newline, carriage return.
       JSON escape sequences placed by xmlEscape (\b, \f, \uHHHH) are
       passed through as-is since they represent XML-illegal characters
       that were re-encoded for safe XML transport. -->
  <xsl:template name="escape-json-string">
    <xsl:param name="val"/>
    <xsl:choose>
      <xsl:when test="$val = ''"/>
      <xsl:when test="contains($val, '\')">
        <!-- Emit everything before the backslash (escaped) -->
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-before($val, '\')"/>
        </xsl:call-template>
        <!-- Check what follows the backslash -->
        <xsl:variable name="after" select="substring-after($val, '\')"/>
        <xsl:variable name="next" select="substring($after, 1, 1)"/>
        <xsl:choose>
          <!-- JSON escape sequences from xmlEscape: pass through -->
          <xsl:when test="$next = 'b' or $next = 'f' or $next = 'r'">
            <xsl:text>\</xsl:text>
            <xsl:value-of select="$next"/>
            <xsl:call-template name="escape-json-string">
              <xsl:with-param name="val" select="substring($after, 2)"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$next = 'u'">
            <!-- \uHHHH — pass through the 5 chars (u + 4 hex digits) -->
            <xsl:text>\u</xsl:text>
            <xsl:value-of select="substring($after, 2, 4)"/>
            <xsl:call-template name="escape-json-string">
              <xsl:with-param name="val" select="substring($after, 6)"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$next = '\'">
            <!-- \\ (escaped backslash from xmlEscape) — emit as JSON \\ -->
            <xsl:text>\\</xsl:text>
            <xsl:call-template name="escape-json-string">
              <xsl:with-param name="val" select="substring($after, 2)"/>
            </xsl:call-template>
          </xsl:when>
          <!-- Lone backslash (should not occur) — double it for safety -->
          <xsl:otherwise>
            <xsl:text>\\</xsl:text>
            <xsl:call-template name="escape-json-string">
              <xsl:with-param name="val" select="$after"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <!-- No backslash — handle other characters -->
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="$val"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Escape a string fragment that contains no backslashes.
       Handles: double-quote, tab, newline, carriage return. -->
  <xsl:template name="escape-json-string-no-bs">
    <xsl:param name="val"/>
    <xsl:choose>
      <xsl:when test="$val = ''"/>
      <xsl:when test="contains($val, $quot)">
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-before($val, $quot)"/>
        </xsl:call-template>
        <xsl:text>\"</xsl:text>
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-after($val, $quot)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($val, '&#9;')">
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-before($val, '&#9;')"/>
        </xsl:call-template>
        <xsl:text>\t</xsl:text>
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-after($val, '&#9;')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($val, '&#10;')">
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-before($val, '&#10;')"/>
        </xsl:call-template>
        <xsl:text>\n</xsl:text>
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-after($val, '&#10;')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($val, '&#13;')">
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-before($val, '&#13;')"/>
        </xsl:call-template>
        <xsl:text>\r</xsl:text>
        <xsl:call-template name="escape-json-string-no-bs">
          <xsl:with-param name="val" select="substring-after($val, '&#13;')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$val"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
