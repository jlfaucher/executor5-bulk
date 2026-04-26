<?xml version="1.0" encoding="UTF-8"?>
<!--
  xmlToYaml.xsl - XSLT 1.0 stylesheet to transform yaml.cls XML output
  back to YAML text.

  Handles both XML flavours produced by yaml.cls:
    - XSD flavour: elements in namespace "urn:yaml:xml:1.0"
    - DTD flavour: elements with no namespace

  Usage with BSF4ooRexx (runXSLT.rxj):
    rexx runXSLT.rxj data.xml xmlToYaml.xsl

  Usage with xsltproc:
    xsltproc xmlToYaml.xsl data.xml

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
    xmlns:y="urn:yaml:xml:1.0">

  <xsl:output method="text" encoding="UTF-8"/>
  <xsl:strip-space elements="*"/>

  <!-- Double-quote character stored in a variable so it can be used
       in XPath expressions without quoting ambiguity (XSLT 1.0). -->
  <xsl:variable name="quot" select="'&quot;'"/>

  <!-- ================================================================ -->
  <!-- Root: match <yaml> (either namespace or no namespace)            -->
  <!-- ================================================================ -->
  <xsl:template match="/y:yaml | /yaml">
    <xsl:for-each select="y:document | document">
      <xsl:if test="position() > 1">
        <xsl:text>---&#10;</xsl:text>
      </xsl:if>
      <!-- Emit %YAML directive if present -->
      <xsl:if test="@yaml-version">
        <xsl:text>%YAML </xsl:text>
        <xsl:value-of select="@yaml-version"/>
        <xsl:text>&#10;</xsl:text>
      </xsl:if>
      <!-- Emit %TAG directives -->
      <xsl:for-each select="y:tag-directive | tag-directive">
        <xsl:text>%TAG </xsl:text>
        <xsl:value-of select="@handle"/>
        <xsl:text> </xsl:text>
        <xsl:value-of select="@prefix"/>
        <xsl:text>&#10;</xsl:text>
      </xsl:for-each>
      <!-- Emit --- separator when directives are present -->
      <xsl:if test="@yaml-version or y:tag-directive or tag-directive">
        <xsl:text>---&#10;</xsl:text>
      </xsl:if>
      <!-- Apply templates to content nodes only (skip tag-directive) -->
      <xsl:apply-templates select="*[not(self::y:tag-directive) and not(self::tag-directive)]">
        <xsl:with-param name="indent" select="0"/>
        <xsl:with-param name="context" select="'doc'"/>
      </xsl:apply-templates>
    </xsl:for-each>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Mapping                                                          -->
  <!-- ================================================================ -->
  <xsl:template match="y:mapping | mapping">
    <xsl:param name="indent" select="0"/>
    <xsl:param name="context" select="'doc'"/>

    <xsl:variable name="anchor" select="@anchor"/>

    <!-- Empty mapping -->
    <xsl:if test="count(y:entry | entry | y:merge | merge) = 0">
      <xsl:if test="$anchor">
        <xsl:text>&amp;</xsl:text>
        <xsl:value-of select="$anchor"/>
        <xsl:text> </xsl:text>
      </xsl:if>
      <xsl:text>{}&#10;</xsl:text>
    </xsl:if>

    <xsl:if test="count(y:entry | entry | y:merge | merge) > 0">
      <!-- Anchor at document top level (no caller to emit it) -->
      <xsl:if test="$anchor and $context = 'doc'">
        <xsl:text>&amp;</xsl:text>
        <xsl:value-of select="$anchor"/>
        <xsl:text>&#10;</xsl:text>
      </xsl:if>

      <!-- Emit merge keys first -->
      <xsl:variable name="merges" select="y:merge | merge"/>
      <xsl:if test="count($merges) > 0">
        <xsl:call-template name="spaces">
          <xsl:with-param name="n" select="$indent"/>
        </xsl:call-template>
        <xsl:choose>
          <xsl:when test="count($merges) = 1">
            <xsl:text>&lt;&lt;: *</xsl:text>
            <xsl:value-of select="$merges[1]/@anchor"/>
            <xsl:text>&#10;</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>&lt;&lt;: [</xsl:text>
            <xsl:for-each select="$merges">
              <xsl:if test="position() > 1">
                <xsl:text>, </xsl:text>
              </xsl:if>
              <xsl:text>*</xsl:text>
              <xsl:value-of select="@anchor"/>
            </xsl:for-each>
            <xsl:text>]&#10;</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>

      <xsl:for-each select="y:entry | entry">
        <xsl:variable name="key-node" select="(y:key | key)/*"/>
        <xsl:variable name="val-node" select="(y:value | value)/*"/>
        <xsl:variable name="key-text">
          <xsl:apply-templates select="$key-node" mode="scalar-value"/>
        </xsl:variable>
        <xsl:variable name="quoted-key">
          <xsl:call-template name="quote-key">
            <xsl:with-param name="key" select="$key-text"/>
            <xsl:with-param name="key-node" select="$key-node"/>
          </xsl:call-template>
        </xsl:variable>

        <!-- Indent + key -->
        <xsl:call-template name="spaces">
          <xsl:with-param name="n" select="$indent"/>
        </xsl:call-template>
        <xsl:value-of select="$quoted-key"/>
        <xsl:text>:</xsl:text>

        <xsl:choose>
          <!-- Value is a mapping or sequence with content -->
          <xsl:when test="(local-name($val-node) = 'mapping' and count($val-node/y:entry | $val-node/entry | $val-node/y:merge | $val-node/merge) > 0)
                       or (local-name($val-node) = 'sequence' and count($val-node/y:item | $val-node/item) > 0)">
            <!-- If the child has an anchor, emit it on the same line as the key -->
            <xsl:if test="$val-node/@anchor">
              <xsl:text> &amp;</xsl:text>
              <xsl:value-of select="$val-node/@anchor"/>
            </xsl:if>
            <xsl:text>&#10;</xsl:text>
            <xsl:apply-templates select="$val-node">
              <xsl:with-param name="indent" select="$indent + 2"/>
              <xsl:with-param name="context" select="'map-value'"/>
            </xsl:apply-templates>
          </xsl:when>
          <!-- Value is alias, scalar, or empty collection -->
          <xsl:otherwise>
            <xsl:text> </xsl:text>
            <xsl:apply-templates select="$val-node">
              <xsl:with-param name="indent" select="$indent + 2"/>
              <xsl:with-param name="context" select="'inline'"/>
            </xsl:apply-templates>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Sequence                                                         -->
  <!-- ================================================================ -->
  <xsl:template match="y:sequence | sequence">
    <xsl:param name="indent" select="0"/>
    <xsl:param name="context" select="'doc'"/>

    <xsl:variable name="anchor" select="@anchor"/>

    <!-- Empty sequence -->
    <xsl:if test="count(y:item | item) = 0">
      <xsl:if test="$anchor">
        <xsl:text>&amp;</xsl:text>
        <xsl:value-of select="$anchor"/>
        <xsl:text> </xsl:text>
      </xsl:if>
      <xsl:text>[]&#10;</xsl:text>
    </xsl:if>

    <xsl:if test="count(y:item | item) > 0">
      <!-- Anchor at document top level (callers handle it otherwise) -->
      <xsl:if test="$anchor and $context = 'doc'">
        <xsl:text>&amp;</xsl:text>
        <xsl:value-of select="$anchor"/>
        <xsl:text>&#10;</xsl:text>
      </xsl:if>

      <xsl:for-each select="y:item | item">
        <xsl:variable name="child" select="*"/>

        <xsl:call-template name="spaces">
          <xsl:with-param name="n" select="$indent"/>
        </xsl:call-template>
        <xsl:text>- </xsl:text>

        <xsl:choose>
          <!-- Item is a mapping with entries or merges: compact notation -->
          <xsl:when test="local-name($child) = 'mapping' and count($child/y:entry | $child/entry | $child/y:merge | $child/merge) > 0">
            <xsl:variable name="child-anchor" select="$child/@anchor"/>
            <xsl:if test="$child-anchor">
              <xsl:text>&amp;</xsl:text>
              <xsl:value-of select="$child-anchor"/>
              <xsl:text>&#10;</xsl:text>
              <xsl:call-template name="spaces">
                <xsl:with-param name="n" select="$indent + 2"/>
              </xsl:call-template>
            </xsl:if>
            <!-- First entry inline after "- " -->
            <xsl:for-each select="($child/y:entry | $child/entry)">
              <xsl:variable name="ekey-node" select="(y:key | key)/*"/>
              <xsl:variable name="eval-node" select="(y:value | value)/*"/>
              <xsl:variable name="ekey-text">
                <xsl:apply-templates select="$ekey-node" mode="scalar-value"/>
              </xsl:variable>
              <xsl:variable name="equoted-key">
                <xsl:call-template name="quote-key">
                  <xsl:with-param name="key" select="$ekey-text"/>
                  <xsl:with-param name="key-node" select="$ekey-node"/>
                </xsl:call-template>
              </xsl:variable>

              <xsl:if test="position() > 1">
                <xsl:call-template name="spaces">
                  <xsl:with-param name="n" select="$indent + 2"/>
                </xsl:call-template>
              </xsl:if>
              <xsl:value-of select="$equoted-key"/>
              <xsl:text>:</xsl:text>

              <xsl:choose>
                <xsl:when test="(local-name($eval-node) = 'mapping' and count($eval-node/y:entry | $eval-node/entry | $eval-node/y:merge | $eval-node/merge) > 0)
                             or (local-name($eval-node) = 'sequence' and count($eval-node/y:item | $eval-node/item) > 0)">
                  <xsl:text>&#10;</xsl:text>
                  <xsl:apply-templates select="$eval-node">
                    <xsl:with-param name="indent" select="$indent + 4"/>
                    <xsl:with-param name="context" select="'map-value'"/>
                  </xsl:apply-templates>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text> </xsl:text>
                  <xsl:apply-templates select="$eval-node">
                    <xsl:with-param name="indent" select="$indent + 4"/>
                    <xsl:with-param name="context" select="'inline'"/>
                  </xsl:apply-templates>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
          </xsl:when>
          <!-- Item is a non-empty sequence: nested on next line -->
          <xsl:when test="local-name($child) = 'sequence' and count($child/y:item | $child/item) > 0">
            <xsl:text>&#10;</xsl:text>
            <xsl:apply-templates select="$child">
              <xsl:with-param name="indent" select="$indent + 2"/>
              <xsl:with-param name="context" select="'seq-item'"/>
            </xsl:apply-templates>
          </xsl:when>
          <!-- Scalar, alias, or empty collection: inline -->
          <xsl:otherwise>
            <xsl:apply-templates select="$child">
              <xsl:with-param name="indent" select="$indent + 2"/>
              <xsl:with-param name="context" select="'inline'"/>
            </xsl:apply-templates>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Scalar                                                           -->
  <!-- ================================================================ -->
  <xsl:template match="y:scalar | scalar">
    <xsl:param name="indent" select="0"/>
    <xsl:param name="context" select="'doc'"/>

    <xsl:variable name="anchor" select="@anchor"/>
    <xsl:variable name="type" select="@type"/>

    <xsl:if test="$anchor">
      <xsl:text>&amp;</xsl:text>
      <xsl:value-of select="$anchor"/>
      <xsl:text> </xsl:text>
    </xsl:if>

    <xsl:choose>
      <xsl:when test="$type = 'null'">
        <xsl:text>null</xsl:text>
      </xsl:when>
      <xsl:when test="$type = 'bool'">
        <xsl:value-of select="."/>
      </xsl:when>
      <xsl:when test="$type = 'int' or $type = 'float'">
        <xsl:value-of select="."/>
      </xsl:when>
      <xsl:otherwise>
        <!-- String: may need quoting -->
        <xsl:variable name="val" select="."/>
        <xsl:choose>
          <xsl:when test="$val = ''">
            <xsl:text>""</xsl:text>
          </xsl:when>
          <xsl:when test="contains($val, '&#10;')">
            <!-- Multiline: use literal block scalar -->
            <xsl:text>|&#10;</xsl:text>
            <xsl:call-template name="block-scalar">
              <xsl:with-param name="text" select="$val"/>
              <xsl:with-param name="indent" select="$indent"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="quote-string">
              <xsl:with-param name="val" select="$val"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:if test="not(contains(., '&#10;')) or $type != 'str'">
      <xsl:text>&#10;</xsl:text>
    </xsl:if>
  </xsl:template>

  <!-- Scalar value extraction (for keys — no newline) -->
  <xsl:template match="y:scalar | scalar" mode="scalar-value">
    <xsl:value-of select="."/>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Alias                                                            -->
  <!-- ================================================================ -->
  <xsl:template match="y:alias | alias">
    <xsl:param name="indent" select="0"/>
    <xsl:param name="context" select="'doc'"/>
    <xsl:text>*</xsl:text>
    <xsl:value-of select="@anchor"/>
    <xsl:text>&#10;</xsl:text>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- Named templates                                                  -->
  <!-- ================================================================ -->

  <!-- Emit n spaces -->
  <xsl:template name="spaces">
    <xsl:param name="n" select="0"/>
    <xsl:if test="$n > 0">
      <xsl:text> </xsl:text>
      <xsl:call-template name="spaces">
        <xsl:with-param name="n" select="$n - 1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <!-- Quote a YAML key if it needs quoting -->
  <xsl:template name="quote-key">
    <xsl:param name="key"/>
    <xsl:param name="key-node"/>
    <xsl:variable name="needs-quote">
      <xsl:call-template name="needs-quoting">
        <xsl:with-param name="val" select="$key"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$needs-quote = 'yes'">
        <xsl:text>"</xsl:text>
        <xsl:call-template name="escape-string">
          <xsl:with-param name="val" select="$key"/>
        </xsl:call-template>
        <xsl:text>"</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$key"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Quote a YAML string value if needed -->
  <xsl:template name="quote-string">
    <xsl:param name="val"/>
    <xsl:variable name="needs-quote">
      <xsl:call-template name="needs-quoting">
        <xsl:with-param name="val" select="$val"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$needs-quote = 'yes'">
        <xsl:text>"</xsl:text>
        <xsl:call-template name="escape-string">
          <xsl:with-param name="val" select="$val"/>
        </xsl:call-template>
        <xsl:text>"</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$val"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Check if a string needs quoting -->
  <xsl:template name="needs-quoting">
    <xsl:param name="val"/>
    <xsl:choose>
      <!-- Empty string -->
      <xsl:when test="$val = ''">yes</xsl:when>
      <!-- Reserved words -->
      <xsl:when test="$val = 'null' or $val = 'Null' or $val = 'NULL' or $val = '~'">yes</xsl:when>
      <xsl:when test="$val = 'true' or $val = 'True' or $val = 'TRUE'">yes</xsl:when>
      <xsl:when test="$val = 'false' or $val = 'False' or $val = 'FALSE'">yes</xsl:when>
      <xsl:when test="$val = 'yes' or $val = 'Yes' or $val = 'YES'">yes</xsl:when>
      <xsl:when test="$val = 'no' or $val = 'No' or $val = 'NO'">yes</xsl:when>
      <xsl:when test="$val = 'on' or $val = 'On' or $val = 'ON'">yes</xsl:when>
      <xsl:when test="$val = 'off' or $val = 'Off' or $val = 'OFF'">yes</xsl:when>
      <!-- Looks like a number -->
      <xsl:when test="number($val) = number($val)">yes</xsl:when>
      <xsl:when test="$val = '.inf' or $val = '+.inf' or $val = '-.inf' or $val = '.nan'
                   or $val = '.Inf' or $val = '.INF' or $val = '.NaN' or $val = '.NAN'">yes</xsl:when>
      <!-- Starts with special YAML indicator character -->
      <xsl:when test="starts-with($val, '&amp;') or starts-with($val, '*')
                   or starts-with($val, '[') or starts-with($val, ']')
                   or starts-with($val, '{') or starts-with($val, '}')
                   or starts-with($val, '|') or starts-with($val, '>')
                   or starts-with($val, '!') or starts-with($val, '%')
                   or starts-with($val, '@') or starts-with($val, '`')
                   or starts-with($val, '#') or starts-with($val, ',')
                   or starts-with($val, '?') or starts-with($val, '-')
                   or starts-with($val, ':') or starts-with($val, &quot;'&quot;)">yes</xsl:when>
      <!-- Contains problematic sequences -->
      <xsl:when test="contains($val, ': ') or contains($val, ' #')">yes</xsl:when>
      <!-- Starts or ends with space -->
      <xsl:when test="starts-with($val, ' ') or substring($val, string-length($val)) = ' '">yes</xsl:when>
      <!-- Contains backslash or double quote -->
      <xsl:when test="contains($val, '\') or contains($val, $quot)">yes</xsl:when>
      <!-- Contains tab -->
      <xsl:when test="contains($val, '&#9;')">yes</xsl:when>
      <xsl:otherwise>no</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Escape a string for YAML double-quoting -->
  <xsl:template name="escape-string">
    <xsl:param name="val"/>
    <xsl:choose>
      <xsl:when test="$val = ''"/>
      <xsl:when test="contains($val, '\')">
        <xsl:call-template name="escape-string">
          <xsl:with-param name="val" select="substring-before($val, '\')"/>
        </xsl:call-template>
        <xsl:text>\\</xsl:text>
        <xsl:call-template name="escape-string">
          <xsl:with-param name="val" select="substring-after($val, '\')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($val, $quot)">
        <xsl:call-template name="escape-string">
          <xsl:with-param name="val" select="substring-before($val, $quot)"/>
        </xsl:call-template>
        <xsl:text>\"</xsl:text>
        <xsl:call-template name="escape-string">
          <xsl:with-param name="val" select="substring-after($val, $quot)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($val, '&#9;')">
        <xsl:call-template name="escape-string">
          <xsl:with-param name="val" select="substring-before($val, '&#9;')"/>
        </xsl:call-template>
        <xsl:text>\t</xsl:text>
        <xsl:call-template name="escape-string">
          <xsl:with-param name="val" select="substring-after($val, '&#9;')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$val"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Emit a block scalar (literal style) with proper indentation -->
  <xsl:template name="block-scalar">
    <xsl:param name="text"/>
    <xsl:param name="indent" select="0"/>
    <xsl:choose>
      <xsl:when test="contains($text, '&#10;')">
        <xsl:call-template name="spaces">
          <xsl:with-param name="n" select="$indent + 2"/>
        </xsl:call-template>
        <xsl:value-of select="substring-before($text, '&#10;')"/>
        <xsl:text>&#10;</xsl:text>
        <xsl:call-template name="block-scalar">
          <xsl:with-param name="text" select="substring-after($text, '&#10;')"/>
          <xsl:with-param name="indent" select="$indent"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$text != ''">
        <xsl:call-template name="spaces">
          <xsl:with-param name="n" select="$indent + 2"/>
        </xsl:call-template>
        <xsl:value-of select="$text"/>
        <xsl:text>&#10;</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
