package rexxeditor.scanner.rules;

import org.eclipse.jface.text.rules.ICharacterScanner;
import org.eclipse.jface.text.rules.IRule;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.Token;

public class RexxEndLineCommentRule implements IRule {
	IToken fToken;
	
	public RexxEndLineCommentRule(IToken token) {
		fToken=token;
	}
	
	public IToken evaluate(ICharacterScanner scanner) {
		int c = scanner.read();
		if (c=='-') {
			c=scanner.read();
			if (c=='-') {
				do {
					c=scanner.read();
				} while (c!='\n' && c!='\r' && c!=ICharacterScanner.EOF);
				scanner.unread();
				return fToken;
			}
			scanner.unread();
		}
		scanner.unread();
		return Token.UNDEFINED;
	}
}
