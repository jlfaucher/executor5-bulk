package rexxeditor.scanner;

import java.util.ArrayList;
import org.eclipse.jface.text.rules.IToken;
import rexxeditor.scanner.tokenRules.ClassDefinitionRule;
import rexxeditor.scanner.tokenRules.IRexxTokenRule;
import rexxeditor.scanner.tokenRules.KeywordRule;
import rexxeditor.scanner.tokenRules.MethodRule;
import rexxeditor.scanner.tokenRules.VariableDefinitionRule;
import rexxeditor.scanner.tokenRules.VariableRule;

public class RexxLine {
	protected ArrayList fTokenList = new ArrayList();
	protected RexxTokenList tokenList;
	protected int fPosition=0;
	IRexxTokenRule rules[];
	
	
	public RexxLine(RexxTokenList tokenList, RexxVariableMethodContainer varMetContainer) {
		rules = new IRexxTokenRule[5];
		this.tokenList = tokenList;
		rules[0] = new KeywordRule(tokenList);
		rules[1] = new VariableDefinitionRule(tokenList, varMetContainer);
		rules[2] = new MethodRule(tokenList);
		rules[3] = new ClassDefinitionRule(tokenList, varMetContainer);
		rules[4] = new VariableRule(tokenList, varMetContainer);
	}
	
	public void changeToken(int position, RexxToken token) {
		fTokenList.remove(position);
		fTokenList.add(position, token);
	}
	
	public void add(RexxToken token) {
		for (int i=0;i<rules.length;i++) {
			RexxToken token2;
			token2 = rules[i].evaluate(token, this);
			if (!token2.notExists()) {
				fTokenList.add(fPosition,token2);
				fPosition++;
				return;
			}
		}

		fTokenList.add(fPosition,token);
		fPosition++;
	}
	
	public int getPosition() {
		return fPosition;
	}
	
	public int getPrevTokenPos(int position,boolean whitespace) {
		position--;
		IToken tempToken= this.get(position).getToken();
		if (whitespace==true) {
			while (tempToken.isWhitespace() || tempToken.equals(tokenList.rexxComment)) {
				position--;
				if (position>=0)
					tempToken = this.get(position).getToken();
				else
					break;
				
			}		
		} else {
			while (tempToken.equals(tokenList.rexxComment)) {
				position--;
				if (position>=0)
					tempToken = this.get(position).getToken();
				else
					break;
				}					
		}
		return position;
	}
	
	public int getFirstTokenPos() {
		int i=0;
		IToken tempToken = this.get(i).getToken();
		while (tempToken.isWhitespace() || tempToken.equals(tokenList.rexxComment)) {
			i++;
			if (i<fPosition)
				tempToken = this.get(i).getToken();
			else
				break;
			
		}
		return i;
	}
	
	public RexxToken get(int i) {
		RexxToken token = (RexxToken)fTokenList.get(i);
		return token;
	}
	
	public int getLength() {
		return fTokenList.size();
	}
	
	public void reset() {
		fPosition=0;
		fTokenList.clear();
	}
	
	public String toString() {
		String result="";
		
		for (int i=0;i<fTokenList.size();i++) {
			RexxToken token = (RexxToken) fTokenList.get(i);
			result+="index"+i;
			result+="type"+token.getToken().toString();
			result+=" token offset:"+token.getOffset();
			result+="\n";
		}
		
		return result;
	}
	
}
