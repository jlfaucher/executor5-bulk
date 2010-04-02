package rexxeditor.launcher;

import org.eclipse.debug.ui.AbstractLaunchConfigurationTabGroup;
import org.eclipse.debug.ui.CommonTab;
import org.eclipse.debug.ui.ILaunchConfigurationDialog;
import org.eclipse.debug.ui.ILaunchConfigurationTab;
import org.eclipse.swt.widgets.Text;

public class RexxLaunchConfigurationTabGroup extends
		AbstractLaunchConfigurationTabGroup {
	 

	public RexxLaunchConfigurationTabGroup() {
		super();
		fTabs = new ILaunchConfigurationTab[2];
		
	}
	
	public void createTabs(ILaunchConfigurationDialog dialog, String mode) {
		fTabs[0] = new RexxLaunchConfigurationTab();
		fTabs[1] = new CommonTab();
	}

}
