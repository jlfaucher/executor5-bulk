package rexxeditor.scanner.rules;

import org.eclipse.jface.text.rules.IWhitespaceDetector;



public class RexxWhitespaceDetector implements IWhitespaceDetector {


	//Rule for tokenizing whitespaces
	public boolean isWhitespace( char c) {
		if (c == ' ' || c == '\t')
			return true;
		
		return false;
		
	}
}
