package rexxeditor.scanner.rules;

import org.eclipse.jface.text.rules.ICharacterScanner;
import org.eclipse.jface.text.rules.IRule;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.IWordDetector;
import org.eclipse.jface.text.rules.Token;

public class RexxGeneralWordRule implements IRule {
	protected IToken fToken;
	protected IWordDetector fDetector;
	
	public RexxGeneralWordRule(IToken token, IWordDetector detector ) {
		fToken=token;
		fDetector=detector;
	}
	
	public IToken evaluate(ICharacterScanner scanner) {
		char c = (char)scanner.read();
		if (fDetector.isWordStart(c)) {
				do {
					c= (char)scanner.read();
				} while (c != ICharacterScanner.EOF && fDetector.isWordPart(c));
				scanner.unread();
				return fToken;
		}
		scanner.unread();
		
		return Token.UNDEFINED;
	}
	

}
