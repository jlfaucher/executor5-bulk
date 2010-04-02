package rexxeditor.launcher;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorPart;

import rexxeditor.Activator;
import rexxeditor.preferences.IRexxPreferenceFields;

public class RexxLaunchShortcut implements ILaunchShortcut {

	public void launch(ISelection selection, String mode) {
		if (selection instanceof IStructuredSelection) {
			IStructuredSelection ss = (IStructuredSelection)selection;
			if (!ss.isEmpty()) {
				Object obj = ss.getFirstElement();
				if (obj instanceof IResource) {
					IResource i = (IResource) obj;
					String file = ((IFile) i).getLocation().toOSString();
					IProject pro = i.getProject();
					String project = pro.getLocation().toOSString();
					String workingDirectory = pro.getLocation().toString();
					if (workingDirectory != "" && workingDirectory != null) {
						workingDirectory += IPath.SEPARATOR;
					}
					launch(workingDirectory,file,project, mode);
					
																	
				}
			}
		}
				
	}

	public void launch(IEditorPart editor, String mode) {
		if (editor != null) {
			IEditorInput input = editor.getEditorInput();
			IFile f = (IFile) input.getAdapter(IFile.class);
			
			
			String file = f.getLocation().toOSString();
			IProject pro = f.getProject();
			String project = pro.getLocation().toOSString();
			String workingDirectory = pro.getLocation().toString();
			if (workingDirectory != "" && workingDirectory != null) {
				workingDirectory += IPath.SEPARATOR;
			}
			launch(workingDirectory,file,project, mode);
		}

	}
	
	private void launch(String workingDir, String file, String project, String mode) {
		/*String interPath = Activator.getDefault().getPreferenceStore().getString(IRexxPreferenceFields.PREF_REXX_PATH);
		if (interPath != "") {
			try {
	            ILaunchConfiguration config = findLaunchConfiguration("run", mode);
	            if (config != null) {
	             config.launch(mode, null);
	            }
	        } catch (CoreException e) {
	           
	        }
		}*/
	}

}
