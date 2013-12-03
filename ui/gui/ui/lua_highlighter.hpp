
/*
 * Special thanks for code to luaeditor (https://code.google.com/p/luaeditor)
 */

#ifndef LUA_HIGHLIGHTER_HPP
#define LUA_HIGHLIGHTER_HPP

#include "all.hpp"

class QLuaHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
	
    struct HighlightingRule
    {
		QRegExp pattern;
		QTextCharFormat format;
    };

	QVector<HighlightingRule> highlightingRules;
	QRegExp commentStartExpression, commentEndExpression;
    QTextCharFormat keywordFormat, singleLineCommentFormat, multiLineCommentFormat, quotationFormat, functionFormat, umlaut, numbers;

	protected:
		
		void highlightBlock(const QString & txt);

	public:
		
		QBrush clrUmlauts, clrNumbers, clrSingleComment, clrMultiComment, clrDoubleQuote, clrSingeleQuote, clrFunction;

		QLuaHighlighter();
};

#endif

