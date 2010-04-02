package rexxeditor.scanner.rules;

import java.util.HashMap;
import java.util.Map;
import org.eclipse.jface.text.rules.ICharacterScanner;
import org.eclipse.jface.text.rules.IRule;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.Token;
import rexxeditor.scanner.RexxTokenList;

public class RexxSymbolRule implements IRule {

	protected Map fSymbols= new HashMap();

	
	public RexxSymbolRule(RexxTokenList tokenList) {	
	
		fSymbols.put("+", tokenList.symbol);
		fSymbols.put("-", tokenList.symbol);
		fSymbols.put("*", tokenList.symbol);
		fSymbols.put("/", tokenList.symbol);
		fSymbols.put("=", tokenList.equalsSymbol);
		fSymbols.put("(", tokenList.openBraceSymbol);
		fSymbols.put(")", tokenList.symbol);
		fSymbols.put("~", tokenList.tildeSymbol);
		fSymbols.put(".", tokenList.pointSymbol);
		fSymbols.put(",", tokenList.symbol);
		fSymbols.put("|", tokenList.symbol);
		fSymbols.put(":", tokenList.equalsSymbol);
		fSymbols.put("<", tokenList.symbol);
		fSymbols.put(">", tokenList.symbol);
		fSymbols.put("!", tokenList.symbol);
		fSymbols.put(";", tokenList.symbol);
		
	}


	
	
	public IToken evaluate(ICharacterScanner scanner) {
		int c= scanner.read();

		IToken token= (IToken) fSymbols.get(Character.toString((char)c));
		if (token != null)
			return token;

		scanner.unread();
		return Token.UNDEFINED;
	}
}


