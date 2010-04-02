package rexxeditor;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import org.eclipse.jface.preference.IPreferenceStore;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.ImageRegistry;
import org.eclipse.jface.resource.StringConverter;
import org.eclipse.ui.plugin.AbstractUIPlugin;
import org.osgi.framework.BundleContext;

import rexxeditor.editors.IRexxColorConstants;
import rexxeditor.preferences.IRexxPreferenceFields;

/**
 * The main plugin class to be used in the desktop.
 */
public class Activator extends AbstractUIPlugin {

	// The shared instance.
	private static Activator plugin;

	/**
	 * The constructor.
	 */
	public Activator() {
		plugin = this;

	}

	protected void initializeImageRegistry(ImageRegistry reg) {

		try {
			reg.put("method", this.getImageDescriptor("icons/jmeth_obj.gif"));
			reg.put("class", this.getImageDescriptor("icons/classes.gif"));
			reg.put("requires", this.getImageDescriptor("icons/jload_obj.gif"));
			reg.put("variable", this.getImageDescriptor("icons/brkpi_obj.gif"));

		} catch (RuntimeException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	/**
	 * This method is called upon plug-in activation
	 */
	public void start(BundleContext context) throws Exception {
		super.start(context);
	}

	/**
	 * This method is called when the plug-in is stopped
	 */
	public void stop(BundleContext context) throws Exception {
		super.stop(context);
		plugin = null;
	}

	/**
	 * Returns the shared instance.
	 * 
	 * @return the shared instance.
	 */
	public static Activator getDefault() {
		return plugin;
	}

	/**
	 * Returns an image descriptor for the image file at the given plug-in
	 * relative path.
	 * 
	 * @param path
	 *            the path
	 * @return the image descriptor
	 */
	public static ImageDescriptor getImageDescriptor(String path) {
		return AbstractUIPlugin.imageDescriptorFromPlugin("RexxEditor", path);
	}

	protected void initializeDefaultPreferences(IPreferenceStore store) {

		// Colors for syntax highlighting.
		store.setDefault(IRexxPreferenceFields.PREF_REXX_COMMENT,
				StringConverter.asString(IRexxColorConstants.REXX_COMMENT));
		store.setDefault(IRexxPreferenceFields.PREF_REXX_SYMBOL,
				StringConverter.asString(IRexxColorConstants.REXX_SYMBOL));
		store.setDefault(IRexxPreferenceFields.PREF_DEFAULT_COLOR,
				StringConverter.asString(IRexxColorConstants.DEFAULT));
		store.setDefault(IRexxPreferenceFields.PREF_VAR_COLOR, StringConverter
				.asString(IRexxColorConstants.REXX_VAR));
		store.setDefault(IRexxPreferenceFields.PREF_PROC_INSTR, StringConverter
				.asString(IRexxColorConstants.PROC_INSTR));
		store.setDefault(IRexxPreferenceFields.PREF_STRING_COLOR,
				StringConverter.asString(IRexxColorConstants.REXX_STR));
		store.setDefault(IRexxPreferenceFields.PREF_REXX_PATH, "");

	}
}
