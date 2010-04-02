package rexxeditor.scanner.tokenRules;

import rexxeditor.scanner.RexxLine;
import rexxeditor.scanner.RexxToken;
import rexxeditor.scanner.RexxTokenList;

public class KeywordRule implements IRexxTokenRule {
	RexxTokenList fTokenList;

	public KeywordRule(RexxTokenList tokenList) {
		fTokenList = tokenList;
	}

	public RexxToken evaluate(RexxToken token, RexxLine line) {
		
		if (line.getPosition() != 0 && fTokenList.isKeyword(token.getToken())) {
			


				int position = line.getPrevTokenPos( line.getPosition(), true);
				if (position == -1) {
					if (fTokenList.isLineStartKeyword(token.getToken())) {
					return token;
					}
				}
				if (position >= 0) {
					if (token.getToken().equals(fTokenList.VALUEKeyword)) {

						if (line.get(position).getToken().equals(
								fTokenList.ADDRESSKeyword)
								|| line.get(position).getToken().equals(
										fTokenList.TRACEKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.OFFKeyword)
							|| token.getToken().equals(fTokenList.ONKeyword)) {

						if (line.get(position).getToken().equals(
								fTokenList.CALLKeyword)
								|| line.get(position).getToken().equals(
										fTokenList.GUARDKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.ANYKeyword)
							|| token.getToken().equals(fTokenList.HALTKeyword)
							|| token.getToken().equals(
									fTokenList.NOTREADYKeyword)
							|| token.getToken().equals(fTokenList.WHENKeyword)) {
						if (line.get(position).getToken().equals(
								fTokenList.ONKeyword)
								|| line.get(position).getToken().equals(
										fTokenList.OFFKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.ERRORKeyword)
							|| token.getToken().equals(
									fTokenList.FAILUREKeyword)
							|| token.getToken().equals(fTokenList.USERKeyword)) {
						if (line.get(position).getToken().equals(
								fTokenList.ONKeyword)
								|| line.get(position).getToken().equals(
										fTokenList.OFFKeyword)
								|| line.get(position).getToken().equals(
										fTokenList.RAISEKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.NAMEKeyword)) {
						if (line.get(position).getToken().equals(
								fTokenList.ANYKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.FOREVERKeyword)
							|| token.getToken().equals(fTokenList.WHILEKeyword)
							|| token.getToken().equals(fTokenList.UNTILKeyword)) {
						if (line.get(position).getToken().equals(
								fTokenList.DOKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.DIGITSKeyword)
							|| token.getToken().equals(fTokenList.FORMKeyword)
							|| token.getToken().equals(fTokenList.FUZZKeyword)) {
						if (line.get(position).getToken().equals(
								fTokenList.NUMERICKeyword)) {
							return token;
						}

					}
					if (token.getToken().equals(fTokenList.SCIENTIFICKeyword)) {

						if (line.get(position).getToken().equals(
								fTokenList.DIGITSKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.ENGINEERINGKeyword)
							|| token.getToken().equals(fTokenList.VALUEKeyword)) {

						if (line.get(position).getToken().equals(
								fTokenList.FORMKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.EXPOSEKeyword)) {
						if (line.get(position).getToken().equals(
								fTokenList.PROCEDUREKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.SYNTAXKeyword)
							|| token.getToken().equals(
									fTokenList.PROPAGATEKeyword)) {

						if (line.get(position).getToken().equals(
								fTokenList.RAISEKeyword)) {
							return token;
						}
					}
				}
				position = line.getFirstTokenPos();
				if (position < line.getPosition() && line.getPosition()!=-1 && position!=-1) {
					if (token.getToken().equals(fTokenList.CONTINUEKeyword)
							|| token.getToken().equals(
									fTokenList.ARGUMENTSKeyword)
							|| token.getToken().equals(fTokenList.ARRAYKeyword)
							|| token.getToken().equals(fTokenList.CLASSKeyword)
							|| token.getToken().equals(fTokenList.TOKeyword)) {
						if (line.get(position).getToken().equals(
								fTokenList.FORWARDKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.OVERKeyword)
							|| token.getToken().equals(fTokenList.TOKeyword)
							|| token.getToken().equals(fTokenList.BYKeyword)
							|| token.getToken().equals(fTokenList.FORKeyword)) {

						if (line.get(position).getToken().equals(
								fTokenList.DOKeyword)) {
							return token;
						}
					}
					if (token.getToken().equals(fTokenList.THENKeyword)) {

						if (line.get(position).getToken().equals(
								fTokenList.IFKeyword)) {
							return token;
						}

					}
					if (token.getToken().equals(fTokenList.UPPERKeyword)
							|| token.getToken().equals(fTokenList.LOWERKeyword)
							|| token.getToken().equals(fTokenList.ARGKeyword)
							|| token.getToken()
									.equals(fTokenList.LINEINKeyword)
							|| token.getToken().equals(fTokenList.PULLKeyword)
							|| token.getToken()
									.equals(fTokenList.SOURCEKeyword)
							|| token.getToken().equals(fTokenList.VALUEKeyword)
							|| token.getToken().equals(fTokenList.VARKeyword)
							|| token.getToken().equals(
									fTokenList.VERSIONKeyword)
							|| token.getToken().equals(
									fTokenList.VERSIONKeyword)) {
						if (line.get(position).getToken().equals(
								fTokenList.PARSEKeyword)) {
							return token;
						}

					}
					if (token.getToken().equals(fTokenList.ADDITIONALKeyword)
							|| token.getToken().equals(
									fTokenList.DESCRIPTIONKeyword)
							|| token.getToken().equals(fTokenList.ARRAYKeyword)
							|| token.getToken()
									.equals(fTokenList.RETURNKeyword)) {

						if (line.get(position).getToken().equals(
								fTokenList.RAISEKeyword)) {
							return token;
						}

					}
				}
				token.setToken(fTokenList.word);
				return token;
			}

		
		return RexxToken.NOTEXIST;
	}

}
