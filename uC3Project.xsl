<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:include href="file:uC3CmpKnlRX.xsl"/>
<xsl:include href="file:uC3CmpCpuRX600.xsl"/>
<xsl:include href="file:uNet3Cmp.xsl"/>
  <xsl:template match="/">
    <html>
      <head>
        <style type="text/css">
            table {border: 2px solid black;border-collapse: collapse;}
            th {background-color: yellow;}
            td,th {border: 1px solid grey;}
            h4 {margin-bottom: 3px;}
            hr {color: green;}
        </style>
      </head>
      <body>
        <h2>uC3/Configurator プロジェクトファイルの設定値一覧</h2>
        <xsl:apply-templates select="uC3Project" />
      </body>
    </html>
  </xsl:template>

  <xsl:template match="Plugins">
    <h3>[使用プラグイン]</h3>
    <table>
      <tr>
        <th>ファイル名</th>
      </tr>
      <xsl:apply-templates select="plugin"/>
    </table>
  </xsl:template>
  <xsl:template match="plugin">
    <tr>
      <td><xsl:value-of select="."/></td>
    </tr>
  </xsl:template>

</xsl:stylesheet>
