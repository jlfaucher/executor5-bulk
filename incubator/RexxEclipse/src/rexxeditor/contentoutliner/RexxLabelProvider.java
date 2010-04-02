package rexxeditor.contentoutliner;

import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.Display;

import rexxeditor.Activator;


/**
 * A label provider for Rexx content elements.
 * <p>
 * This class provides a Rexx icon for the content outliner classes.
 * </p>
 * @author gulasch
 *
 */
public class RexxLabelProvider extends LabelProvider {
	/**
	 * Returns an icon from the image registry.
	 * 
	 * @param the element
	 */
	public Image getImage(Object element) {
		RexxContentElement e = (RexxContentElement)element;
		if (e!=null) {
			return Activator.getDefault().getImageRegistry().get(e.getCategory());
		}
		return null;
		
	}
	
}
