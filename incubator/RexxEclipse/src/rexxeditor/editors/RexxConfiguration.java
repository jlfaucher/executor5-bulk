package rexxeditor.editors;

import java.util.ArrayList;

import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.ITextDoubleClickStrategy;
import org.eclipse.jface.text.TextAttribute;
import org.eclipse.jface.text.presentation.IPresentationReconciler;
import org.eclipse.jface.text.presentation.PresentationReconciler;
import org.eclipse.jface.text.rules.DefaultDamagerRepairer;
import org.eclipse.jface.text.rules.Token;
import org.eclipse.jface.text.source.ISourceViewer;
import org.eclipse.jface.text.source.SourceViewerConfiguration;

import rexxeditor.preferences.IRexxPreferenceFields;
import rexxeditor.scanner.RexxLineScanner;

public class RexxConfiguration extends SourceViewerConfiguration {
	private RexxDoubleClickStrategy doubleClickStrategy;
	private RexxLineScanner scanner;
	private ColorManager colorManager;
	private RexxEditor editor;
	
	
	
	public RexxConfiguration(ColorManager colorManager, RexxEditor editor) {
		this.editor = editor;
		this.colorManager = colorManager;
	}
	public String[] getConfiguredContentTypes(ISourceViewer sourceViewer) {
		return new String[] {IDocument.DEFAULT_CONTENT_TYPE};
	}
	public ITextDoubleClickStrategy getDoubleClickStrategy(
		ISourceViewer sourceViewer,
		String contentType) {
		if (doubleClickStrategy == null)
			doubleClickStrategy = new RexxDoubleClickStrategy();
		return doubleClickStrategy;
	}

	public void updateList(ArrayList update) {
		editor.update(update);
	}
	
	protected RexxLineScanner getRexxScanner() {
		if (scanner == null) {
			scanner = new RexxLineScanner(colorManager, this);
			scanner.setDefaultReturnToken(
				new Token(
					new TextAttribute(
						colorManager.getColor(IRexxPreferenceFields.PREF_DEFAULT_COLOR))));
		}
		return scanner;
	}


	public IPresentationReconciler getPresentationReconciler(ISourceViewer sourceViewer) {
		PresentationReconciler reconciler = new RexxPresentationReconciler();

		DefaultDamagerRepairer dr = new DefaultDamagerRepairer(getRexxScanner());
		reconciler.setDamager(dr, IDocument.DEFAULT_CONTENT_TYPE);
		reconciler.setRepairer(dr, IDocument.DEFAULT_CONTENT_TYPE);

		return reconciler;
	}

}