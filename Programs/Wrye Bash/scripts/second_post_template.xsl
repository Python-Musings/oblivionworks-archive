<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text"/>
<xsl:param name="release_num"/>
<xsl:param name="release_theme"/>

<xsl:template match="/">Here's a rundown of what the next release will contain, as well as a list of all known bugs and requested enhancements.  Where relevant, we have included links to sourceforge tracker artifacts.  If you have information or opinions pertaining to any particular bug or enhancement, please comment at the tracker link.  If you have screenshots or sample files, you can attach them to the bugs at the tracker links too (though size limits might apply).  Any information helps!

Users who have reported bugs and are updating from svn - check the following for any closed/fixed bugs (indicated with a [color="#FF8C00"][s]strikethrough[/s][/color]) - confirmation would be much appreciated.

[b]Upcoming release <xsl:value-of select="$release_num"/>[/b]: <xsl:value-of select="$release_theme"/>
[color=green][/color][color=orange]||||||||||||||||||||[/color] 0% Complete[spoiler][list]
[/list][/spoiler]

[b][url=https://sourceforge.net/tracker/?limit=25&amp;group_id=284958&amp;atid=1207901&amp;status=1&amp;submit=Filter]Additional known bugs[/url][/b]:[spoiler][list]<xsl:for-each select="//trackers/tracker[name='Bugs']/tracker_items/tracker_item[status_id=1]">
        <xsl:sort select="id" order="descending"/>
[*] [b][url=https://sourceforge.net/tracker/?func=detail&amp;aid=<xsl:value-of select="id"/>&amp;group_id=284958&amp;atid=1207901]Bug <xsl:value-of select="id"/>[/url][/b] <xsl:value-of select="summary"/>
    </xsl:for-each>
[/list][/spoiler]

[b][url=https://sourceforge.net/tracker/?limit=25&amp;group_id=284958&amp;atid=1207904&amp;status=1&amp;submit=Filter]Current enhancement requests[/url][/b]:[spoiler][list]<xsl:for-each select="//trackers/tracker[name='Enhancements']/tracker_items/tracker_item[status_id=1]">
        <xsl:sort select="id" order="descending"/>
[*] [b][url=https://sourceforge.net/tracker/?func=detail&amp;aid=<xsl:value-of select="id"/>&amp;group_id=284958&amp;atid=1207904]Enhancement <xsl:value-of select="id"/>[/url][/b] <xsl:value-of select="summary"/>
    </xsl:for-each>
[/list][/spoiler]
</xsl:template>
</xsl:stylesheet>
