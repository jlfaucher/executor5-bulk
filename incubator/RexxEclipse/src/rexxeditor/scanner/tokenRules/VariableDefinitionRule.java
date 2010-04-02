package rexxeditor.scanner.tokenRules;

import org.eclipse.jface.text.TextAttribute;
import rexxeditor.scanner.RexxLine;
import rexxeditor.scanner.RexxToken;
import rexxeditor.scanner.RexxTokenList;
import rexxeditor.scanner.RexxVariableMethodContainer;

public class VariableDefinitionRule implements IRexxTokenRule {
	RexxTokenList fTokenList;
	RexxVariableMethodContainer fContainer;
	
	public VariableDefinitionRule(RexxTokenList tokenList, RexxVariableMethodContainer varMetContainer) {
		fTokenList = tokenList;
		fContainer = varMetContainer;
	}
	
	public RexxToken evaluate(RexxToken token, RexxLine line) {
		if (token.getToken().equals(fTokenList.equalsSymbol) && line.getPosition()>0) {
			int position = line.getPrevTokenPos(line.getPosition(), true);
			int beginPosition;
			if (position>0) {
				beginPosition =  line.getPrevTokenPos(position, true);

			} else if (position==0) {
				beginPosition = -1;
			} else {
				return RexxToken.NOTEXIST;
			}
			if (beginPosition==-1) {
				TextAttribute attribute = (TextAttribute)line.get(position).getToken().getData();
				if (attribute.equals(fTokenList.normal) || attribute.equals(fTokenList.procs)) {
					
					RexxToken deleteToken = line.get(position);
					fContainer.addVariable(deleteToken.getOffset(), deleteToken.getLength());
					line.changeToken(position, new RexxToken(fTokenList.variable,deleteToken.getOffset(),deleteToken.getLength()));
					
				}
			}	
			
		}
		return RexxToken.NOTEXIST;
	}
	
}
