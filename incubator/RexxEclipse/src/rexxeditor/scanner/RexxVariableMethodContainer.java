package rexxeditor.scanner;

import java.util.ArrayList;
import org.eclipse.jface.text.BadLocationException;
import org.eclipse.jface.text.IDocument;
import rexxeditor.contentoutliner.RexxContentElement;

public class RexxVariableMethodContainer {
	ArrayList fVariableList;

	
	IDocument fDocument;
	
	public RexxVariableMethodContainer () {
		fVariableList = new ArrayList();
	}
	
	public ArrayList getVariableList() {
		return fVariableList;
	}
	
	public void update(IDocument document) {
		fVariableList = new ArrayList();
		fDocument = document;
	}
	
	public void addVariable(int offset, int length) {
		try {
			String variable = fDocument.get(offset, length).toUpperCase();
			fVariableList.add(new RexxContentElement(variable, offset, length,"variable"));
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void addClass(int offset, int length) {
		try {
			String Class = fDocument.get(offset, length).toUpperCase();
			fVariableList.add(new RexxContentElement(Class, offset, length,"class"));
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void addMethod(int offset, int length) {
		try {
			String method = fDocument.get(offset, length).toUpperCase();
			fVariableList.add(new RexxContentElement(method, offset, length,"method"));
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void addRequires(int offset, int length) {
		try {
			String requires = fDocument.get(offset, length).toUpperCase();
			fVariableList.add(new RexxContentElement(requires, offset, length,"requires"));
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void addRoutine(int offset, int length) {
		try {
			String routine = fDocument.get(offset, length).toUpperCase();
			fVariableList.add(new RexxContentElement(routine, offset, length,routine));
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public boolean isVariable(int offset, int length) {
		String variable="";
		try {
			variable = fDocument.get(offset, length).toUpperCase();
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		for (int i=0;i<fVariableList.size();i++) {
			RexxContentElement element = (RexxContentElement)fVariableList.get(i);
			if (element.getCategory().equals("variable")) {
				if (variable.equals(element.getName()))
					return true;
			}
		}
		return false;
	}
	
}
