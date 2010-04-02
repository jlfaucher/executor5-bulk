package rexxeditor.scanner.rules;

import org.eclipse.jface.text.rules.IWordDetector;

public class RexxKeywordDetector implements IWordDetector {

	
	
	public boolean isWordStart(char c) {		

		
		return Character.isLetter(c) || c==':' || Character.isDigit(c);
	}
	
	public boolean isWordPart(char c) {
		return Character.isLetter(c) || c==':' || Character.isDigit(c);
	}
}
