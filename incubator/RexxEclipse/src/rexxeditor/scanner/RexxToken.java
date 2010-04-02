package rexxeditor.scanner;

import org.eclipse.jface.text.rules.IToken;

public class RexxToken {
	protected int fOffset;
	protected int fLength;
	protected IToken fToken;
	private int fType;
	private static final int T_UNDEFINED= 0;
	
	public static final RexxToken NOTEXIST = new RexxToken(T_UNDEFINED);
	
	private RexxToken(int i) {
		fType=i;
	}
	
	public boolean notExists() {
		return (fType == T_UNDEFINED);
	}
	
	public RexxToken(IToken token, int offset, int length) {
		fOffset = offset;
		fLength = length;
		fToken = token;
	}

	public int getLength() {
		return fLength;
	}

	public int getOffset() {
		return fOffset;
	}

	public IToken getToken() {
		return fToken;
	}

	public void setToken(IToken token) {
		fToken = token;
	}
	
	
}
