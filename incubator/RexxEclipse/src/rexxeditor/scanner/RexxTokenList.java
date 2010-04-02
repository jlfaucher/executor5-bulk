package rexxeditor.scanner;

import org.eclipse.jface.resource.StringConverter;
import org.eclipse.jface.text.TextAttribute;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.Token;
import org.eclipse.swt.SWT;
import rexxeditor.editors.ColorManager;
import rexxeditor.editors.IRexxColorConstants;
import rexxeditor.preferences.IRexxPreferenceFields;

public class RexxTokenList {
	public  IToken word;
	
	public  IToken symbol;
	public  IToken equalsSymbol;
	public 	IToken pointSymbol;
	public  IToken openBraceSymbol;
	public  IToken closeBraceSymbol;
	public  IToken tildeSymbol;
	
	public  IToken newLine;
	public  IToken keyword;
	public  IToken variable;
	public  IToken method;
	public  IToken environmentSymbol;
	public  IToken rexxComment;
	public  IToken rexxTag;
	
	public IToken CLASSToken;
	public IToken METHODToken;
	public IToken REQUIRESToken;
	public IToken ROUTINEToken;
	
	public IToken ADDRESSKeyword;
	public IToken VALUEKeyword;
	public IToken PARSEKeyword;
	public IToken UPPERKeyword;
	public IToken LOWERKeyword;
	public IToken ARGKeyword;
	public IToken CALLKeyword;
	public IToken OFFKeyword;
	public IToken ONKeyword;
	public IToken ANYKeyword;
	public IToken ERRORKeyword;
	public IToken FAILUREKeyword;
	public IToken HALTKeyword;
	public IToken NOTREADYKeyword;
	public IToken USERKeyword;
	public IToken NAMEKeyword;
	public IToken ENDKeyword;
	public IToken FOREVERKeyword;
	public IToken TOKeyword;
	public IToken OVERKeyword;
	public IToken BYKeyword;
	public IToken FORKeyword;
	public IToken DOKeyword;
	public IToken WHILEKeyword;
	public IToken UNTILKeyword;
	public IToken DROPKeyword;
	public IToken EXITKeyword;
	public IToken EXPOSEKeyword;
	public IToken FORWARDKeyword;
	public IToken CONTINUEKeyword;
	public IToken ARRAYKeyword;
	public IToken ARGUMENTSKeyword;
	public IToken MESSAGEKeyword;
	public IToken CLASSKeyword;
	public IToken GUARDKeyword;
	public IToken WHENKeyword;
	public IToken IFKeyword;
	public IToken ELSEKeyword;
	public IToken THENKeyword;
	public IToken INTERPRETKeyword;
	public IToken ITERATEKeyword;
	public IToken LEAVEKeyword;
	public IToken NOPKeyword;
	public IToken NUMERICKeyword;
	public IToken DIGITSKeyword;
	public IToken FORMKeyword;
	public IToken FUZZKeyword;
	public IToken SCIENTIFICKeyword;
	public IToken ENGINEERINGKeyword;
	public IToken CASELESSKeyword;
	public IToken LINEINKeyword;
	public IToken PULLKeyword;
	public IToken SOURCEKeyword;
	public IToken VARKeyword;
	public IToken VERSIONKeyword;
	public IToken PROCEDUREKeyword;
	public IToken PUSHKeyword;
	public IToken QUEUEKeyword;
	public IToken RAISEKeyword;
	public IToken SYNTAXKeyword;
	public IToken PROPAGATEKeyword;
	public IToken REPLYKeyword;
	public IToken RETURNKeyword;
	public IToken SAYKeyword;
	public IToken SELECTKeyword;
	public IToken SIGNALKeyword;
	public IToken TRACEKeyword;
	public IToken USEKeyword;
	public IToken ADDITIONALKeyword;
	public IToken DESCRIPTIONKeyword;
	
	public TextAttribute procs;
	public TextAttribute methodVar;
	public TextAttribute symb;
	public TextAttribute comment;
	public TextAttribute string;
	public TextAttribute normal;
	
	
	public void setTokens(ColorManager manager) {

		procs = new TextAttribute(manager
				.getColor(IRexxPreferenceFields.PREF_PROC_INSTR));
		methodVar = new TextAttribute(manager
				.getColor(IRexxPreferenceFields.PREF_VAR_COLOR), manager
				.getColor(IRexxColorConstants.WHITE), SWT.BOLD);
		symb = new TextAttribute(manager
				.getColor(IRexxPreferenceFields.PREF_REXX_SYMBOL));
		comment = new TextAttribute(manager
				.getColor(IRexxPreferenceFields.PREF_REXX_COMMENT));
		string = new TextAttribute(manager
				.getColor(IRexxPreferenceFields.PREF_STRING_COLOR));
		normal = new TextAttribute(manager.getColor(IRexxPreferenceFields.PREF_DEFAULT_COLOR));
		
		word = manager.getToken(IRexxPreferenceFields.PREF_DEFAULT_COLOR);
		
		symbol = new Token(symb);
		equalsSymbol = new Token(symb);
		pointSymbol = new Token(symb);
		openBraceSymbol = new Token(symb);
		tildeSymbol = new Token(symb);
		
		newLine = new Token(symb);
		//keyword = new Token(procs);
		variable = new Token(methodVar);
		method = new Token(methodVar);
		rexxComment = new Token(comment);
		rexxTag = new Token(string);
		environmentSymbol = new Token(environmentSymbol);
		
		ADDITIONALKeyword = new Token(procs);
		DESCRIPTIONKeyword = new Token(procs);
		ADDRESSKeyword = new Token(procs);
		VALUEKeyword = new Token(procs);
		PARSEKeyword = new Token(procs);
		UPPERKeyword = new Token(procs);
		LOWERKeyword = new Token(procs);
		ARGKeyword = new Token(procs);
		CALLKeyword = new Token(procs);
		OFFKeyword = new Token(procs);
		ONKeyword = new Token(procs);
		ANYKeyword = new Token(procs);
		ERRORKeyword = new Token(procs);
		FAILUREKeyword = new Token(procs);
		HALTKeyword = new Token(procs);
		NOTREADYKeyword = new Token(procs);
		USERKeyword = new Token(procs);
		NAMEKeyword = new Token(procs);
		ENDKeyword = new Token(procs);
		FOREVERKeyword = new Token(procs);
		TOKeyword = new Token(procs);
		BYKeyword = new Token(procs);
		FORKeyword = new Token(procs);
		DOKeyword = new Token(procs);
		WHILEKeyword = new Token(procs);
		UNTILKeyword = new Token(procs);
		DROPKeyword = new Token(procs);
		EXITKeyword = new Token(procs);
		EXPOSEKeyword = new Token(procs);
		FORWARDKeyword = new Token(procs);
		CONTINUEKeyword = new Token(procs);
		ARRAYKeyword = new Token(procs);
		OVERKeyword = new Token(procs);
		ARGUMENTSKeyword = new Token(procs);
		MESSAGEKeyword = new Token(procs);
		CLASSKeyword = new Token(procs);
		GUARDKeyword = new Token(procs);
		WHENKeyword = new Token(procs);
		IFKeyword = new Token(procs);
		ELSEKeyword = new Token(procs);
		THENKeyword = new Token(procs);
		INTERPRETKeyword = new Token(procs);
		ITERATEKeyword = new Token(procs);
		LEAVEKeyword = new Token(procs);
		NOPKeyword = new Token(procs);
		NUMERICKeyword = new Token(procs);
		DIGITSKeyword = new Token(procs);
		FORMKeyword = new Token(procs);
		FUZZKeyword = new Token(procs);
		SCIENTIFICKeyword = new Token(procs);
		ENGINEERINGKeyword = new Token(procs);
		CASELESSKeyword = new Token(procs);
		LINEINKeyword = new Token(procs);
		PULLKeyword = new Token(procs);
		SOURCEKeyword = new Token(procs);
		VARKeyword = new Token(procs);
		VERSIONKeyword = new Token(procs);
		PROCEDUREKeyword = new Token(procs);
		PUSHKeyword = new Token(procs);
		QUEUEKeyword = new Token(procs);
		RAISEKeyword = new Token(procs);
		SYNTAXKeyword = new Token(procs);
		PROPAGATEKeyword = new Token(procs);
		REPLYKeyword = new Token(procs);
		RETURNKeyword = new Token(procs);
		SAYKeyword = new Token(procs);
		SELECTKeyword = new Token(procs);
		SIGNALKeyword = new Token(procs);
		TRACEKeyword = new Token(procs);
		USEKeyword = new Token(procs);
		
		CLASSToken = new Token(procs);
		METHODToken = new Token(procs);
		REQUIRESToken = new Token(procs);
		ROUTINEToken = new Token(procs);
	}
	
	public RexxTokenList(ColorManager manager) {
		this.setTokens(manager);
	}
	
	public boolean isLineStartKeyword (IToken token) {
		if (token.equals(IFKeyword) || 
				token.equals(DOKeyword) ||
				token.equals(ADDRESSKeyword) ||
				token.equals(ARGKeyword) ||
				token.equals(CALLKeyword) ||
				token.equals(DROPKeyword) ||
				token.equals(EXITKeyword) ||
				token.equals(EXPOSEKeyword) ||
				token.equals(FORWARDKeyword) ||
				token.equals(GUARDKeyword) ||
				token.equals(INTERPRETKeyword) ||
				token.equals(ITERATEKeyword) ||
				token.equals(LEAVEKeyword) ||
				token.equals(NOPKeyword) ||
				token.equals(NUMERICKeyword) ||
				token.equals(PARSEKeyword) ||
				token.equals(PROCEDUREKeyword) ||
				token.equals(PULLKeyword) ||
				token.equals(PUSHKeyword) ||
				token.equals(QUEUEKeyword) ||
				token.equals(RAISEKeyword) ||
				token.equals(REPLYKeyword) ||
				token.equals(RETURNKeyword) ||
				token.equals(SAYKeyword) ||
				token.equals(SELECTKeyword) ||
				token.equals(SIGNALKeyword) ||
				token.equals(ELSEKeyword) ||
				token.equals(ENDKeyword) ||
				token.equals(TRACEKeyword) ||
				token.equals(USEKeyword)) {
			return true;
		}
		return false;
	}
	
	public boolean isKeyword(IToken token) {
		if (token.equals(ADDRESSKeyword) ||
		token.equals(VALUEKeyword) ||
		token.equals(PARSEKeyword) ||
		token.equals(UPPERKeyword) ||
		token.equals(LOWERKeyword) ||
		token.equals(ARGKeyword) ||
		token.equals(CALLKeyword) ||
		token.equals(OFFKeyword) ||
		token.equals(ONKeyword) ||
		token.equals(ANYKeyword) ||
		token.equals(ERRORKeyword) ||
		token.equals(FAILUREKeyword) ||
		token.equals(HALTKeyword) ||
		token.equals(NOTREADYKeyword) ||
		token.equals(USERKeyword) ||
		token.equals(NAMEKeyword) ||
		token.equals(ENDKeyword) ||
		token.equals(FOREVERKeyword) ||
		token.equals(TOKeyword) ||
		token.equals(OVERKeyword) ||
		token.equals(BYKeyword) ||
		token.equals(FORKeyword) ||
		token.equals(DOKeyword) ||
		token.equals(WHILEKeyword) ||
		token.equals(UNTILKeyword) ||
		token.equals(DROPKeyword) ||
		token.equals(EXITKeyword) ||
		token.equals(EXPOSEKeyword) ||
		token.equals(FORWARDKeyword) ||
		token.equals(CONTINUEKeyword) ||
		token.equals(ARRAYKeyword) ||
		token.equals(ARGUMENTSKeyword) ||
		token.equals(MESSAGEKeyword) ||
		token.equals(CLASSKeyword) ||
		token.equals(GUARDKeyword) ||
		token.equals(WHENKeyword) ||
		token.equals(IFKeyword) ||
		token.equals(ELSEKeyword) ||
		token.equals(THENKeyword) ||
		token.equals(INTERPRETKeyword) ||
		token.equals(ITERATEKeyword) ||
		token.equals(LEAVEKeyword) ||
		token.equals(NOPKeyword) ||
		token.equals(NUMERICKeyword) ||
		token.equals(DIGITSKeyword) ||
		token.equals(FORMKeyword) ||
		token.equals(FUZZKeyword) ||
		token.equals(SCIENTIFICKeyword) ||
		token.equals(ENGINEERINGKeyword) ||
		token.equals(CASELESSKeyword) ||
		token.equals(LINEINKeyword) ||
		token.equals(PULLKeyword) ||
		token.equals(SOURCEKeyword) ||
		token.equals(VARKeyword) ||
		token.equals(VERSIONKeyword) ||
		token.equals(PROCEDUREKeyword) ||
		token.equals(PUSHKeyword) ||
		token.equals(QUEUEKeyword) ||
		token.equals(RAISEKeyword) ||
		token.equals(SYNTAXKeyword) ||
		token.equals(PROPAGATEKeyword) ||
		token.equals(REPLYKeyword) ||
		token.equals(RETURNKeyword) ||
		token.equals(SAYKeyword) ||
		token.equals(SELECTKeyword) ||
		token.equals(SIGNALKeyword) ||
		token.equals(TRACEKeyword) ||
		token.equals(USEKeyword) ||
		token.equals(ADDITIONALKeyword) ||
		token.equals(DESCRIPTIONKeyword)) {
			return true;
		}
		
		return false;
	}
}
