package rexxeditor.scanner.rules;

import org.eclipse.jface.text.rules.ICharacterScanner;
import org.eclipse.jface.text.rules.IRule;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.Token;

public class RexxNewLineRule implements IRule {
	protected IToken fToken;
	protected IToken fNoNewLineToken;
	
	public RexxNewLineRule(IToken token,IToken noNewLine) {
		fToken = token;
		fNoNewLineToken = noNewLine;
	}
	
	public IToken evaluate(ICharacterScanner scanner) {
		char c=(char)scanner.read();
		
		if (c=='-' || c==',') {
			c=(char)scanner.read();
			if (c=='\n' || c=='\r')
				return fNoNewLineToken;
			
				scanner.unread();
				scanner.unread();
				return Token.UNDEFINED;
			
			
		}
		if (c=='\n' || c=='\r' || c==';')
			return fToken;
		scanner.unread();
		return Token.UNDEFINED;
	}
}
