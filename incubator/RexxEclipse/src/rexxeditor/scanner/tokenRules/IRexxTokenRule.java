package rexxeditor.scanner.tokenRules;

import rexxeditor.scanner.RexxLine;
import rexxeditor.scanner.RexxToken;

public interface IRexxTokenRule {
	
	public RexxToken evaluate(RexxToken token, RexxLine line);
}
