package rexxeditor.scanner.rules;

import org.eclipse.jface.text.rules.ICharacterScanner;
import org.eclipse.jface.text.rules.IRule;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.Token;

public class RexxNestedCommentRule implements IRule {
	IToken fToken;
	
	
	public RexxNestedCommentRule (IToken token) {
		fToken=token;
	}
	
	
	public IToken evaluate(ICharacterScanner scanner) {
		int openTags;
		int c= scanner.read();
		if (c=='/') {
			c=scanner.read();
			if (c=='*') {
				openTags=1;
				do {
					c=scanner.read();
					if (c=='/') {
						c=scanner.read();
						if (c=='*') 
							openTags++;
						else
							scanner.unread();
					}
					if (c=='*') {
						c=scanner.read();
						if (c=='/') {
							if (openTags==1) {
								return fToken;
							} else
								openTags--;
						}
						scanner.unread();
					}
				} while (c!=ICharacterScanner.EOF);
				scanner.unread();
				return fToken;
			}
			scanner.unread();
		}
		scanner.unread();
		return Token.UNDEFINED;
	}
}
