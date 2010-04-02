package rexxeditor.editors;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.eclipse.jface.preference.IPreferenceStore;
import org.eclipse.jface.resource.StringConverter;
import org.eclipse.jface.text.TextAttribute;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.Token;
import org.eclipse.jface.util.PropertyChangeEvent;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.swt.widgets.Display;

import rexxeditor.scanner.RexxTokenList;

public class ColorManager {

	protected Map fColorTable = new HashMap(10);

	private Map tokenTable = new HashMap(10);

	private final IPreferenceStore preferenceStore;

	public ColorManager(IPreferenceStore store) {
		this.preferenceStore = store;
	}

	public void dispose() {
		Iterator e = fColorTable.values().iterator();
		while (e.hasNext())
			((Color) e.next()).dispose();
	}

	public Color getColor(String prefKey) {
		Color color = (Color) fColorTable.get(prefKey);
		if (color == null) {
			String colorName = preferenceStore.getString(prefKey);
			color = new Color(Display.getCurrent(), StringConverter
					.asRGB(colorName));
		}
		return color;
	}

	public Color getColor(RGB rgb) {
		Color color = (Color) fColorTable.get(rgb);
		if (color == null) {
			color = new Color(Display.getCurrent(), rgb);
			fColorTable.put(rgb, color);
		}
		return color;
	}

	public IToken getToken(String prefKey) {
		Token token = (Token) tokenTable.get(prefKey);
		if (token == null) {
			String colorName = preferenceStore.getString(prefKey);
			RGB rgb = StringConverter.asRGB(colorName);
			token = new Token(new TextAttribute(getColor(rgb)));
			tokenTable.put(prefKey, token);
		}
		return token;
	}

	public boolean affectsTextPresentation(PropertyChangeEvent event) {
		Token token = (Token) tokenTable.get(event.getProperty());
		return (token != null);
	}

	public void handlePreferenceStoreChanged(PropertyChangeEvent event) {
		String prefKey = event.getProperty();
		Color color = (Color) fColorTable.get(prefKey);
		if (color != null) {
			String colorName = preferenceStore.getString(prefKey);
			color = new Color(Display.getCurrent(), StringConverter
					.asRGB(colorName));
		}
	}
}
