package rexxeditor.scanner.tokenRules;

import rexxeditor.scanner.RexxLine;
import rexxeditor.scanner.RexxToken;
import rexxeditor.scanner.RexxTokenList;
import rexxeditor.scanner.RexxVariableMethodContainer;

public class ClassDefinitionRule implements IRexxTokenRule{
	RexxTokenList fTokenList;
	RexxVariableMethodContainer fContainer;
	
	public ClassDefinitionRule(RexxTokenList tokenList, RexxVariableMethodContainer varMetContainer) {
		fTokenList = tokenList;
		fContainer = varMetContainer;
	}
	
	public RexxToken evaluate(RexxToken token, RexxLine line) {
		
		if (token.getToken().equals(fTokenList.word) && line.getPosition()>0) {
			int position = line.getPrevTokenPos(line.getPosition(), true);
			if (position>-1) {
				if (line.get(position).getToken().equals(fTokenList.CLASSToken)) {
					fContainer.addClass(token.getOffset(), token.getLength());
					token.setToken(fTokenList.method);
					return token;
				}
				if (line.get(position).getToken().equals(fTokenList.METHODToken)) {
					fContainer.addMethod(token.getOffset(), token.getLength());
					token.setToken(fTokenList.method);
					return token;
				}
				if (line.get(position).getToken().equals(fTokenList.REQUIRESToken)) {
					fContainer.addRequires(token.getOffset(), token.getLength());
					token.setToken(fTokenList.method);
					return token;
				}
				if (line.get(position).getToken().equals(fTokenList.ROUTINEToken)) {
					fContainer.addRoutine(token.getOffset(), token.getLength());
					token.setToken(fTokenList.method);
					return token;
				}
			
			}
		}
		return RexxToken.NOTEXIST;
	}
}
