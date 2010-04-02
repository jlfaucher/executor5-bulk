package rexxeditor.scanner.rules;

import org.eclipse.jface.text.rules.WordRule;

import rexxeditor.scanner.RexxTokenList;

public class ConfigureKeywordRule {
	public static WordRule configure(WordRule rule, RexxTokenList tokenList) {
		
		

		
		rule.addWord("ADDRESS", tokenList.ADDRESSKeyword);
		rule.addWord("VALUE", tokenList.VALUEKeyword);
		rule.addWord("PARSE", tokenList.PARSEKeyword);
		rule.addWord("UPPER", tokenList.UPPERKeyword);
		rule.addWord("LOWER", tokenList.LOWERKeyword);
		rule.addWord("ARG", tokenList.ARGKeyword);
		rule.addWord("CALL", tokenList.CALLKeyword);
		rule.addWord("OFF", tokenList.OFFKeyword);
		rule.addWord("ON", tokenList.ONKeyword);
		rule.addWord("ANY", tokenList.ANYKeyword);
		rule.addWord("ERROR", tokenList.ERRORKeyword);
		rule.addWord("FAILURE", tokenList.FAILUREKeyword);
		rule.addWord("HALT", tokenList.HALTKeyword);
		rule.addWord("NOTREADY", tokenList.NOTREADYKeyword);
		rule.addWord("USER", tokenList.USERKeyword);
		rule.addWord("NAME", tokenList.NAMEKeyword);
		rule.addWord("END", tokenList.ENDKeyword);
		rule.addWord("FOREVER", tokenList.FOREVERKeyword);
		rule.addWord("TO", tokenList.TOKeyword);
		rule.addWord("BY", tokenList.BYKeyword);
		rule.addWord("FOR", tokenList.FORKeyword);
		rule.addWord("DO", tokenList.DOKeyword);
		rule.addWord("WHILE", tokenList.WHILEKeyword);
		rule.addWord("UNTIL", tokenList.UNTILKeyword);
		rule.addWord("DROP", tokenList.DROPKeyword);
		rule.addWord("EXIT", tokenList.EXITKeyword);
		rule.addWord("EXPOSE", tokenList.EXPOSEKeyword);
		rule.addWord("FORWARD", tokenList.FORWARDKeyword);
		rule.addWord("CONTINUE", tokenList.CONTINUEKeyword);
		rule.addWord("ARRAY", tokenList.ARRAYKeyword);
		rule.addWord("ARGUMENTS", tokenList.ARGUMENTSKeyword);
		rule.addWord("MESSAGE", tokenList.MESSAGEKeyword);
		rule.addWord("CLASS", tokenList.CLASSKeyword);
		rule.addWord("GUARD", tokenList.GUARDKeyword);
		rule.addWord("WHEN", tokenList.WHENKeyword);
		rule.addWord("IF", tokenList.IFKeyword);
		rule.addWord("ELSE", tokenList.ELSEKeyword);
		rule.addWord("THEN", tokenList.THENKeyword);
		rule.addWord("INTERPRET", tokenList.INTERPRETKeyword);
		rule.addWord("ITERATE", tokenList.ITERATEKeyword);
		rule.addWord("LEAVE", tokenList.LEAVEKeyword);
		rule.addWord("NOP", tokenList.NOPKeyword);
		rule.addWord("NUMERIC", tokenList.NUMERICKeyword);
		rule.addWord("DIGITS", tokenList.DIGITSKeyword);
		rule.addWord("FORM", tokenList.FORMKeyword);
		rule.addWord("FUZZ", tokenList.FUZZKeyword);
		rule.addWord("SCIENTIFIC", tokenList.SCIENTIFICKeyword);
		rule.addWord("ENGINEERING", tokenList.ENGINEERINGKeyword);
		rule.addWord("CASELESS", tokenList.CASELESSKeyword);
		rule.addWord("LINEIN", tokenList.LINEINKeyword);
		rule.addWord("PULL", tokenList.PULLKeyword);
		rule.addWord("SOURCE", tokenList.SOURCEKeyword);
		rule.addWord("VAR", tokenList.VARKeyword);
		rule.addWord("OVER", tokenList.OVERKeyword);
		rule.addWord("VERSION", tokenList.VERSIONKeyword);
		rule.addWord("PROCEDURE", tokenList.PROCEDUREKeyword);
		rule.addWord("EXPOSE", tokenList.EXPOSEKeyword);
		rule.addWord("PUSH", tokenList.PUSHKeyword);
		rule.addWord("QUEUE", tokenList.QUEUEKeyword);
		rule.addWord("RAISE", tokenList.RAISEKeyword);
		rule.addWord("SYNTAX", tokenList.SYNTAXKeyword);
		rule.addWord("PROPAGATE", tokenList.PROPAGATEKeyword);
		rule.addWord("REPLY", tokenList.REPLYKeyword);
		rule.addWord("RETURN", tokenList.RETURNKeyword);
		rule.addWord("SAY", tokenList.SAYKeyword);
		rule.addWord("SELECT", tokenList.SELECTKeyword);
		rule.addWord("SIGNAL", tokenList.SIGNALKeyword);
		rule.addWord("TRACE", tokenList.TRACEKeyword);
		rule.addWord("USE", tokenList.USEKeyword);
		rule.addWord("ADDITIONAL",tokenList.ADDITIONALKeyword);
		rule.addWord("DESCRIPTION",tokenList.DESCRIPTIONKeyword);
		
		rule.addWord("::CLASS", tokenList.CLASSToken);
		rule.addWord("::METHOD", tokenList.METHODToken);
		rule.addWord("::REQUIRES", tokenList.REQUIRESToken);
		rule.addWord("::ROUTINE", tokenList.ROUTINEToken);
		

		return rule;
	}

}
