package rexxeditor.preferences;

import org.eclipse.jface.preference.ColorFieldEditor;
import org.eclipse.jface.preference.FieldEditorPreferencePage;
import org.eclipse.jface.preference.FileFieldEditor;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPreferencePage;

import rexxeditor.Activator;

public class RexxPreferencePage extends FieldEditorPreferencePage implements
		IWorkbenchPreferencePage {

	public RexxPreferencePage() {
		super(GRID);
	    setPreferenceStore(Activator.getDefault().getPreferenceStore());
	}
	
	@Override
	protected void createFieldEditors() {

		Group g = new Group(getFieldEditorParent(),SWT.SHADOW_ETCHED_IN);
		GridData gd = new GridData(GridData.HORIZONTAL_ALIGN_FILL);
		gd.horizontalSpan = 3;
		g.setLayoutData(gd);
		g.setText("Rexx editor settings:");

		Label l1= new Label(g, SWT.LEFT);
		l1.setText("");
		gd = new GridData();
		gd.horizontalSpan = 3;
		l1.setLayoutData(gd);

	 	addField(
			new ColorFieldEditor(
				IRexxPreferenceFields.PREF_REXX_COMMENT,
				"    &Comments:",
				g)) ;
		addField(
			new ColorFieldEditor(
				IRexxPreferenceFields.PREF_REXX_SYMBOL,
				"    &Symbols:",
				g));
		addField(
			new ColorFieldEditor(
				IRexxPreferenceFields.PREF_VAR_COLOR,
				"    &Variables:",
				g));			
		addField(
			new ColorFieldEditor(
				IRexxPreferenceFields.PREF_PROC_INSTR,
				"    &Keywords:",
				g));
		addField(
			new ColorFieldEditor(
				IRexxPreferenceFields.PREF_STRING_COLOR,
				"    &Strings:",
				g));			
		addField(
			new ColorFieldEditor(
				IRexxPreferenceFields.PREF_DEFAULT_COLOR,
				"    O&thers:",
				g));			

		Label l2= new Label(g, SWT.LEFT);
		l2.setText("");
		gd = new GridData();
		gd.horizontalSpan = 3;
		l2.setLayoutData(gd);

		Label l3= new Label(getFieldEditorParent(), SWT.LEFT);
		l3.setText("");
		gd = new GridData();
		gd.horizontalSpan = 3;
		l3.setLayoutData(gd);

		//DirectoryFieldEditor pathEditor =
		FileFieldEditor pathEditor =
			new FileFieldEditor(
				IRexxPreferenceFields.PREF_REXX_PATH,
				"&Rexx interpreter path: ",
				getFieldEditorParent());
		pathEditor.setEmptyStringAllowed(false);
		addField(pathEditor);

	}

	public void init(IWorkbench workbench) {
		

	}

}
