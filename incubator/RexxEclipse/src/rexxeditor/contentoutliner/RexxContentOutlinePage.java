package rexxeditor.contentoutliner;

import java.util.ArrayList;
import java.util.List;
import org.eclipse.jface.text.BadLocationException;
import org.eclipse.jface.text.BadPositionCategoryException;
import org.eclipse.jface.text.DefaultPositionUpdater;
import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.IPositionUpdater;
import org.eclipse.jface.text.Position;
import org.eclipse.jface.viewers.DoubleClickEvent;
import org.eclipse.jface.viewers.IDoubleClickListener;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerComparator;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.ui.texteditor.IDocumentProvider;
import org.eclipse.ui.texteditor.ITextEditor;
import org.eclipse.ui.views.contentoutline.ContentOutlinePage;

/**
 * An implementation of the abstract base class <code>ContentOutlinePage</code>
 * for content outline pages.
 * <p>
 * Internally, each content outline page consists of a standard tree viewer;
 * selections made in the tree viewer are reported as selection change events by
 * the page (which is a selection provider). The tree viewer is not created
 * until <code>createPage</code> is called; consequently, subclasses must
 * extend <code>createControl</code> to configure the tree viewer with a
 * proper content provider, label provider, and input element.
 */
public class RexxContentOutlinePage extends ContentOutlinePage implements
		IDoubleClickListener {
	public class ContentProvider implements ITreeContentProvider {
		protected final static String SEGMENTS = "__rexx_segments";

		protected List fContent = new ArrayList();

		IPositionUpdater fPositionUpdater = new DefaultPositionUpdater(ContentProvider.SEGMENTS);

		/**
		 * Returns the elements of the given input element.
		 * <p>
		 * 
		 * @param inputElement
		 *            the input element
		 * @return an array of elements
		 */
		public Object[] getElements(Object inputElement) {

			return fContent.toArray();
		}

		/**
		 * Returns whether the given element has children.
		 * <p>
		 * Intended as an optimization for when the viewer does not need the
		 * actual children. Clients may be able to implement this more
		 * efficiently than <code>getChildren</code>.
		 * </p>
		 * 
		 * @param element
		 *            the element
		 * @return <code>true</code> if the given element has children, and
		 *         <code>false</code> if it has no children
		 */
		public boolean hasChildren(Object element) {
			return element == fInput;
		}

		/**
		 * Returns the parent for the given element, or <code>null</code>
		 * indicating that the parent can't be computed. In this case the
		 * tree-structured viewer can't expand a given node correctly if
		 * requested.
		 * 
		 * @param element
		 *            the element
		 * @return the parent element, or <code>null</code> if it has none or
		 *         if the parent cannot be computed
		 */
		public Object getParent(Object element) {
			if (element instanceof RexxContentElement)
				return fInput;
			return null;
		}

		/**
		 * Returns the child elements of the given parent element.
		 * <p>
		 * The difference between this method and
		 * <code>IStructuredContentProvider.getElements</code> is that
		 * <code>getElements</code> is called to obtain the tree viewer's root
		 * elements, whereas <code>getChildren</code> is used to obtain the
		 * children of a given parent element in the tree (including a root).
		 * </p>
		 * The result is not modified by the viewer.
		 * 
		 * @param element
		 *            the parent element
		 * @return an array of child elements
		 */
		public Object[] getChildren(Object element) {
			if (element == fInput)
				return fContent.toArray();
			return new Object[0];
		}

		/**
		 * Removes the segments from the old input and adds ones to the new
		 * input.
		 * 
		 * @param viewer
		 *            the viewer
		 * @param oldInput
		 *            the old input object
		 * @param newInput
		 *            the new input object
		 */
		public void inputChanged(Viewer viewer, Object oldInput, Object newInput) {
			if (oldInput != null) {
				IDocument document = fDocumentProvider.getDocument(oldInput);
				if (document != null) {
					try {
						document.removePositionCategory(ContentProvider.SEGMENTS);
					} catch (BadPositionCategoryException x) {
					}
					document.removePositionUpdater(fPositionUpdater);
				}
			}
			fContent.clear();
			if (newInput != null) {
				IDocument document = fDocumentProvider.getDocument(newInput);
				if (document != null) {
					document.addPositionCategory(ContentProvider.SEGMENTS);
					document.addPositionUpdater(fPositionUpdater);
					parse(document);
				}
			}

		}

		/**
		 * Checks whether an object is deleted.
		 * 
		 * @param element
		 *            the element
		 * @return returns always <code>false</code>
		 */
		public boolean isDeleted(Object element) {
			return false;
		}

		/**
		 * Adds segments to the given document with the help of the
		 * <code>offset</code>, <code>name</code> and <code>length</code>
		 * arguments from the <code>RexxContentElement</code> class.
		 * 
		 * @param document
		 *            the document
		 */
		protected void parse(IDocument document) {
			for (int i = 0; i < elementList.size(); i++) {
				RexxContentElement element = (RexxContentElement) elementList
						.get(i);

				Position p = new Position(element.getOffset(), element
						.getLength());
				try {
					document.addPosition(ContentProvider.SEGMENTS, p);
					fContent.add(new RexxContentElement(element.getName(),
							element.getOffset(), element.getLength(), element
									.getCategory()));

				} catch (BadLocationException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (BadPositionCategoryException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}

		/**
		 * Deletes an element if it is not <code>null</code>.
		 */
		public void dispose() {
			if (fContent != null) {
				fContent.clear();
				fContent = null;
			}
		}
	}

	protected Object fInput;

	protected Combo comboBar;

	protected IDocumentProvider fDocumentProvider;

	protected ITextEditor fTextEditor;

	protected RexxViewerFilter filter;

	ArrayList elementList;

	/**
	 * Sets the input.
	 * 
	 * @param input
	 *            the input
	 */
	public void setInput(Object input) {
		fInput = input;
		update();
	}

	/**
	 * Create a new Rexx content outline page. Additionally sets the document
	 * provider and the editor for later querying.
	 * 
	 * @param provider
	 *            the document provider
	 * @param editor
	 *            the text editor
	 */
	public RexxContentOutlinePage(IDocumentProvider provider, ITextEditor editor) {
		super();
		elementList = new ArrayList();
		fDocumentProvider = provider;
		fTextEditor = editor;
		String[] cat = new String[4];
		cat[0] = "VARIABLE";
		cat[1] = "CLASS";
		cat[2] = "METHOD";
		cat[3] = "REQUIRES";
		filter = new RexxViewerFilter(cat);
	}

	/**
	 * The <code>RexxContentOutlinePage</code> implementation of this
	 * <code>IContentOutlinePage</code> method creates a tree viewer.
	 * <p>
	 * The treeviewer is set up with a <code>RexxLabelProvider</code> and the
	 * <code>ContentProvider</code>
	 * </p>
	 * 
	 * @param parent
	 *            the parent
	 */
	@Override
	public void createControl(Composite parent) {

		super.createControl(parent);

		TreeViewer viewer = getTreeViewer();
		viewer.setContentProvider(new ContentProvider());
		viewer.setLabelProvider(new RexxLabelProvider());
		viewer.addSelectionChangedListener(this);
		viewer.setComparator(new ViewerComparator());
		viewer.addDoubleClickListener(this);

		viewer.addFilter(filter);

		if (fInput != null)
			viewer.setInput(fInput);
	}

	/**
	 * Notifies of a double click and sets a <code>RexxViewerFilter</code>
	 * configures with the category of the element that was double-clicked.
	 * 
	 * @param event
	 *            event object describing the double-click
	 */
	public void doubleClick(DoubleClickEvent event) {
		ISelection selection = event.getSelection();
		if (!selection.isEmpty()) {
			RexxContentElement element = (RexxContentElement) ((IStructuredSelection) selection)
					.getFirstElement();
			if (filter.getCurrentCategory().contentEquals(
					element.getCategory().toUpperCase()))
				filter.eraseCategory();
			else
				filter.selectCategory(element.getCategory().toUpperCase());
			TreeViewer viewer = getTreeViewer();
			viewer.refresh();
		}

	}

	/**
	 * Fires a selection changed event.
	 * <p>
	 * The editor highlights the characters from the offset to the length of the
	 * selected element in the text.
	 * </p>
	 * 
	 * @param event
	 *            the new selection
	 */
	@Override
	public void selectionChanged(SelectionChangedEvent event) {

		super.selectionChanged(event);

		ISelection selection = event.getSelection();
		if (selection.isEmpty())
			fTextEditor.resetHighlightRange();
		else {
			RexxContentElement element = (RexxContentElement) ((IStructuredSelection) selection)
					.getFirstElement();
			int start = element.getOffset();
			int length = element.getLength();
			try {
				fTextEditor.setHighlightRange(start, length, true);
			} catch (IllegalArgumentException x) {
				fTextEditor.resetHighlightRange();
			}
		}
	}

	/**
	 * Updates the <code>TreeViewer</code> with the <code>ArrayList</code>
	 * from the <code>RexxVariableMethodContainer</code>.
	 * 
	 * @param updateList
	 */
	public void update(ArrayList updateList) {
		elementList = updateList;
		TreeViewer viewer = getTreeViewer();
		viewer.setInput(elementList);

		update();
	}

	/**
	 * Update the <code>TreeViewer</code>.
	 * 
	 */
	public void update() {
		TreeViewer viewer = getTreeViewer();

		if (viewer != null) {
			Control control = viewer.getControl();
			if (control != null && !control.isDisposed()) {
				control.setRedraw(false);
				viewer.setInput(fInput);
				viewer.expandAll();
				control.setRedraw(true);
			}
		}
	}
}
