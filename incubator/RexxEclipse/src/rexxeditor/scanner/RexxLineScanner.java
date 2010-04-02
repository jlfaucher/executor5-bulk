package rexxeditor.scanner;

import java.util.ArrayList;
import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.rules.IRule;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.RuleBasedScanner;
import org.eclipse.jface.text.rules.SingleLineRule;
import org.eclipse.jface.text.rules.Token;
import org.eclipse.jface.text.rules.WhitespaceRule;
import rexxeditor.editors.ColorManager;
import rexxeditor.editors.RexxConfiguration;
import rexxeditor.scanner.rules.ConfigureKeywordRule;
import rexxeditor.scanner.rules.RexxEndLineCommentRule;
import rexxeditor.scanner.rules.RexxGeneralWordRule;
import rexxeditor.scanner.rules.RexxKeywordDetector;
import rexxeditor.scanner.rules.RexxKeywordRule;
import rexxeditor.scanner.rules.RexxNestedCommentRule;
import rexxeditor.scanner.rules.RexxNewLineRule;
import rexxeditor.scanner.rules.RexxSymbolRule;
import rexxeditor.scanner.rules.RexxWhitespaceDetector;

public class RexxLineScanner extends RuleBasedScanner {
	protected RexxLine fLine;

	protected RexxToken fCurrentToken;

	protected int fCurrentLinePosition = 0;

	RexxTokenList tokenList;

	RexxVariableMethodContainer varMetContainer;

	ArrayList variableList;

	RexxConfiguration configuration;

	public RexxLineScanner(ColorManager manager, RexxConfiguration configuration) {
		this.variableList = new ArrayList();
		this.configuration = configuration;
		varMetContainer = new RexxVariableMethodContainer();

		tokenList = new RexxTokenList(manager);

		fLine = new RexxLine(tokenList, varMetContainer);

		IRule[] rules = new IRule[9];

		rules[0] = new WhitespaceRule(new RexxWhitespaceDetector());

		rules[1] = new RexxNestedCommentRule(tokenList.rexxComment);

		rules[2] = new RexxEndLineCommentRule(tokenList.rexxComment);

		rules[3] = new SingleLineRule("\"", "\"", tokenList.rexxTag, '-', true);

		rules[4] = new SingleLineRule("'", "'", tokenList.rexxTag, '-', true);

		rules[5] = new RexxNewLineRule(tokenList.newLine, tokenList.symbol);

		rules[7] = new RexxSymbolRule(tokenList);

		rules[6] = ConfigureKeywordRule.configure(new RexxKeywordRule(
				new RexxKeywordDetector(), tokenList.word), tokenList);

		rules[8] = new RexxGeneralWordRule(tokenList.word,
				new RexxKeywordDetector());

		setRules(rules);
	}

	public int getTokenOffset() {
		return fCurrentToken.getOffset();
	}

	/*
	 * @see ITokenScanner#getTokenLength()
	 */
	public int getTokenLength() {
		return fCurrentToken.getLength();
	}

	private IToken scanLine() {

		if (fRules != null) {
			for (int i = 0; i < fRules.length; i++) {
				IToken token = (fRules[i].evaluate(this));

				if (!token.isUndefined()) {
					return token;
				}
			}
		}

		if (read() == EOF) {
			return Token.EOF;
		}
		return fDefaultReturnToken;
	}

	public IToken nextToken() {

		if (fCurrentLinePosition >= fLine.getLength()) {
			fLine.reset();
			//scan a new Line
			IToken token = fDefaultReturnToken;
			while (fOffset < fRangeEnd || !token.isEOF()) {
				int tokenOffset = fOffset;
				token = scanLine();
				fLine.add(new RexxToken(token, tokenOffset, fOffset
						- tokenOffset));

				if (token.equals(tokenList.newLine)) {
					break;
				}
			}
			//System.out.println(fLine.getLength() + ","+ fLine.toString());
			fCurrentLinePosition = 0;
		}

		fCurrentToken = fLine.get(fCurrentLinePosition);

		fCurrentLinePosition++;

		if (fCurrentToken.getToken().isEOF()) {
			if (!variableList.equals(varMetContainer.getVariableList())) {
				variableList = varMetContainer.getVariableList();
				configuration.updateList(variableList);
			}
		}

		return fCurrentToken.getToken();

	}

	public void setRange(IDocument document, int offset, int length) {
		//configuration.updateList(varMetContainer.getVariableList());
		varMetContainer.update(document);

		fDocument = document;
		fOffset = offset;
		fColumn = UNDEFINED;
		fRangeEnd = Math.min(fDocument.getLength(), offset + length-1);

		String[] delimiters = fDocument.getLegalLineDelimiters();
		fDelimiters = new char[delimiters.length][];
		for (int i = 0; i < delimiters.length; i++)
			fDelimiters[i] = delimiters[i].toCharArray();

		if (fDefaultReturnToken == null)
			fDefaultReturnToken = new Token(null);

	}

}
