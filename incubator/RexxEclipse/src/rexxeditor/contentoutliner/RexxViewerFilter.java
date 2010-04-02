package rexxeditor.contentoutliner;

import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.StructuredViewer;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerFilter;


/**
 * A Rexx viewer filter is used by a structured viewer to
 * extract a subset of elements provided by its content provider.
 * 
 * @see IStructuredContentProvider
 * @see StructuredViewer
 */
public class RexxViewerFilter extends ViewerFilter {

	protected String category;
	protected String categoryArray[];
	
    /**
     * Creates a new viewer filter with the given category array.
     * 
     * @param c an array of categories
     */
	public RexxViewerFilter (String[] c) {
		categoryArray = c;
		category = "";
	}
	
	/**
	 * Selects the given category <code>a</code>.
	 * 
	 * @param a a category name
	 */
	public void selectCategory(String a) {
		for (int i=0;i<categoryArray.length;i++) {
			if (categoryArray[i].contentEquals(a)) {category=a;}
		}
	}
	
	/**
	 * Erases a category.
	 *
	 */
	public void eraseCategory() {
		category="";
	}
	
	/**
	 * Returns the current category.
	 *
	 * 
	 * @return	the category
	 */
	public String getCurrentCategory() {
		return category;
	}
	
    /**
     * Returns whether the given element makes it through this filter.
     *
     * @param viewer the viewer
     * @param parentElement the parent element
     * @param element the element
     * @return <code>true</code> if element is included in the
     *   filtered set, and <code>false</code> if excluded
     */
	public boolean select(Viewer viewer, Object parentElement, Object element) {
		if (category != "") {
			
			RexxContentElement e = (RexxContentElement) element;
			if (category.contentEquals(e.getCategory().toUpperCase())) {
				
				return true;
			}
			return false;
		}
		return true;
	}

}
