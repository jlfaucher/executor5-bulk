package rexxeditor.launcher;

import java.io.File;
import java.text.MessageFormat;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.core.model.IProcess;
import org.eclipse.debug.core.model.LaunchConfigurationDelegate;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PlatformUI;

public class RexxLaunchConfigurationDelegate extends
		LaunchConfigurationDelegate {
	private String fRexxRuntime = "";
	private String fRexxCommand = "";
	private IProgressMonitor fMonitor;
	private ILaunchConfiguration fElement;
	private ILaunch fLaunch;
	private String fMode;
	private Process fInferiorProcess;
	private IProcess fEclipseProcHandle;
	

	public void launch(ILaunchConfiguration configuration, String mode,
			ILaunch launch, IProgressMonitor monitor) throws CoreException {
		
		try {

			fRexxRuntime = configuration.getAttribute(IRexxLaunchConstants.REXX_LAUNCH_INTERPRETER, "");

			if (monitor == null) {
				fMonitor = new NullProgressMonitor();
			} 
			else {
				fMonitor = monitor;
			}

			fElement = configuration;
			fMode = mode;
			fLaunch = launch;
			

			monitor.beginTask(MessageFormat.format("Launching {0}...", new String[] {configuration.getName()}), 3); //$NON-NLS-1$
			if (monitor.isCanceled()) {
				
				return;
			}
			String file =
				configuration.getAttribute(
					IRexxLaunchConstants.REXX_LAUNCH_STRING_FILE,
					"");
			
			String workingDirectory = 
				configuration.getAttribute(
					IRexxLaunchConstants.REXX_LAUNCH_WORKING_DIRECTORY,
					"");
			String params = 
				configuration.getAttribute(
						IRexxLaunchConstants.REXX_LAUNCH_PARAMETERS, 
						"");
					
			File fworkingDirectory = null;
			
			if (workingDirectory != "" && workingDirectory != null) {
				fworkingDirectory = new File(workingDirectory);
			}
			
					
			
			
			fInferiorProcess =
				DebugPlugin.exec(new String[] {fRexxRuntime, file, params}, fworkingDirectory);
			
			fEclipseProcHandle =  DebugPlugin.newProcess(launch,
			    fInferiorProcess,"Rexx Session");
			
			

			monitor.worked(1);
		}
		catch (CoreException e) {
						
		}
		

		
	}
	
	

}
