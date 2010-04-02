package rexxeditor.scanner.tokenRules;

import org.eclipse.jface.text.rules.IToken;
import rexxeditor.scanner.RexxLine;
import rexxeditor.scanner.RexxToken;
import rexxeditor.scanner.RexxTokenList;

public class MethodRule implements IRexxTokenRule {
	RexxTokenList fTokenList;
	
	
	public MethodRule(RexxTokenList tokenList) {
		fTokenList = tokenList;
	}
	
	public RexxToken evaluate(RexxToken token, RexxLine line) {
		if (line.getPosition()!=0) {
		
			IToken fIToken = token.getToken();
			int position = line.getPosition()-1;
			
			if (fIToken.equals(fTokenList.openBraceSymbol)) {
				if (line.get(position).getToken().equals(fTokenList.word)) {
					RexxToken returnToken = line.get(position);
					returnToken.setToken(fTokenList.method);
				}
			} else if (fIToken.equals(fTokenList.tildeSymbol)) {
				IToken tempToken= line.get(position).getToken();
				while (tempToken.isWhitespace() || tempToken.equals(fTokenList.rexxComment)) {
					position--;
					if (position>=0)
						tempToken = line.get(position).getToken();
					else
						break;
					
				}
				if (position>-1 && tempToken.equals(fTokenList.word)) {
					RexxToken returnToken = line.get(position);
					returnToken.setToken(fTokenList.method);
					
				}
			} else if (fIToken.equals(fTokenList.word)) {
				if (line.get(position).getToken().equals(fTokenList.pointSymbol)) {
					token.setToken(fTokenList.method);
					return token;
					
				} else {
					IToken tempToken= line.get(position).getToken();
					while (tempToken.isWhitespace() || tempToken.equals(fTokenList.rexxComment)) {
						position--;
						if (position>=0)
							tempToken = line.get(position).getToken();
						else
							break;
						
					}
					if (position>-1 && tempToken.equals(fTokenList.tildeSymbol)) {
						token.setToken(fTokenList.method);
						return token;
						
					}
				}
			}
		
		}
		return RexxToken.NOTEXIST;
	}

}
