package rexxeditor.editors;

import java.util.ArrayList;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.jface.util.PropertyChangeEvent;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.editors.text.TextEditor;
import org.eclipse.ui.views.contentoutline.IContentOutlinePage;

import rexxeditor.Activator;
import rexxeditor.contentoutliner.RexxContentOutlinePage;

public class RexxEditor extends TextEditor {
	private RexxContentOutlinePage fOutlinePage;
	private ColorManager colorManager;


	public RexxEditor() {
		super();
		colorManager = new ColorManager(Activator.getDefault().getPreferenceStore());
		setSourceViewerConfiguration(new RexxConfiguration(colorManager, this));
		setPreferenceStore(Activator.getDefault().getPreferenceStore());
	}
	
	public void dispose() {
		colorManager.dispose();
		if (fOutlinePage != null)
			fOutlinePage.setInput(null);
		super.dispose();
	}
	
	protected void handlePreferenceStoreChanged(
			   PropertyChangeEvent event)
			{	
			   colorManager.handlePreferenceStoreChanged(event);
			   super.handlePreferenceStoreChanged(event);
			}
	
	public Object getAdapter(Class required) {
		if (IContentOutlinePage.class.equals(required)) {
			if (fOutlinePage == null) {
				fOutlinePage= new RexxContentOutlinePage(getDocumentProvider(), this);
				if (getEditorInput() != null)
					fOutlinePage.setInput(getEditorInput());
			}
			return fOutlinePage;
		}
		return super.getAdapter(required);
	}
	public void update(ArrayList updateList) {

		if (fOutlinePage != null) {
			fOutlinePage.update(updateList);
		}
	}
	public void doSetInput(IEditorInput input) throws CoreException {
		super.doSetInput(input);

		if (fOutlinePage != null)
			fOutlinePage.setInput(input);
	}
}
