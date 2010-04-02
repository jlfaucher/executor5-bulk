package rexxeditor.scanner.rules;

import java.util.HashMap;
import java.util.Map;
import org.eclipse.jface.text.Assert;
import org.eclipse.jface.text.rules.ICharacterScanner;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.IWordDetector;
import org.eclipse.jface.text.rules.Token;
import org.eclipse.jface.text.rules.WordRule;

public class RexxKeywordRule extends WordRule {
	protected Map fWords= new HashMap();
	private StringBuffer fBuffer= new StringBuffer();	
	
	
	
	public void addWord(String word, IToken token) {
		Assert.isNotNull(word);
		Assert.isNotNull(token);

		fWords.put(word, token);
	}
	

	
	public RexxKeywordRule (IWordDetector wordDetector, IToken token) {

		super(wordDetector,token);

	}
	
	public IToken evaluate(ICharacterScanner scanner) {
		int c= scanner.read();
		if (fDetector.isWordStart((char) c)) {
			if (fColumn == UNDEFINED || (fColumn == scanner.getColumn() - 1)) {

				fBuffer.setLength(0);
				do {
					fBuffer.append((char) c);
					c= scanner.read();
				} while (c != ICharacterScanner.EOF && fDetector.isWordPart((char) c));
				scanner.unread();
				
				IToken token= (IToken) fWords.get(fBuffer.toString().toUpperCase());
				if (token != null)
					return token;

				if (fDefaultToken.isUndefined())
					unreadBuffer(scanner);

				return fDefaultToken;
			}
		}

		scanner.unread();
		return Token.UNDEFINED;
	}
}
