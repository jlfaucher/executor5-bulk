package rexxeditor.scanner.tokenRules;

import rexxeditor.scanner.RexxLine;
import rexxeditor.scanner.RexxToken;
import rexxeditor.scanner.RexxTokenList;
import rexxeditor.scanner.RexxVariableMethodContainer;

public class VariableRule  implements IRexxTokenRule {
	RexxTokenList fTokenList;
	RexxVariableMethodContainer fContainer;
	
	public VariableRule(RexxTokenList tokenList, RexxVariableMethodContainer varMetContainer) {
		fTokenList = tokenList;
		fContainer = varMetContainer;
	}
	
	
	
	public RexxToken evaluate(RexxToken token, RexxLine line) {
		if (token.getToken().equals(fTokenList.word)) {
			if (fContainer.isVariable(token.getOffset(), token.getLength())) {
				token.setToken(fTokenList.variable);
				return token;
			}
		}
		return RexxToken.NOTEXIST;
	}
}
