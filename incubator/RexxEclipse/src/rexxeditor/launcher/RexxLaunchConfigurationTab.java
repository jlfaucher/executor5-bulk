package rexxeditor.launcher;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.ui.AbstractLaunchConfigurationTab;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.ElementTreeSelectionDialog;
import org.eclipse.ui.model.WorkbenchContentProvider;
import org.eclipse.ui.model.WorkbenchLabelProvider;
import org.eclipse.ui.views.navigator.ResourceSorter;

import rexxeditor.Activator;
import rexxeditor.preferences.IRexxPreferenceFields;

public class RexxLaunchConfigurationTab extends AbstractLaunchConfigurationTab {
	// Project UI widgets.
	private Label fRexxInterpreterLabel;
	private Text fRexxInterpreterText;
	private Button fInterpreterBrowse;
	
	
	private Label fRexxFileLabel;
	private Text fRexxFileText;
	private Button fFileWorkspaceBrowse;
	
	private Label fRexxProjectLabel;
	private Text fRexxProjectText;
	private Button fProjectWorkspaceBrowse;
	
	private Label fRexxParamLabel;
	private Text fRexxParamText;
	
	protected String fName;
	String file;
	
	
	public RexxLaunchConfigurationTab() {
		super();
		fName = "General ooRexx Configuration";
	}
	
	private Label createVerticalSpacer(Composite comp) {
		return new Label(comp, SWT.NONE);
	}
	
	public void createControl(Composite parent) {
		Font font = parent.getFont();
		
		Composite comp = new Composite(parent, SWT.NONE);
		setControl(comp);		
		GridLayout topLayout = new GridLayout();
		comp.setLayout(topLayout);		
		GridData gd;
		
		createVerticalSpacer(comp);
		
		Composite projComp = new Composite(comp, SWT.NONE);
		GridLayout projLayout = new GridLayout();
		projLayout.numColumns = 3;
		projLayout.marginHeight = 0;
		projLayout.marginWidth = 0;
		projComp.setLayout(projLayout);
		gd = new GridData(GridData.FILL_HORIZONTAL);
		projComp.setLayoutData(gd);
		projComp.setFont(font);
		
		fRexxInterpreterLabel = new Label(projComp, SWT.NONE);
		
		fRexxInterpreterLabel.setText("Rexx Interpreter Path: ");
		gd = new GridData();
		gd.horizontalSpan = 3;
		fRexxInterpreterLabel.setLayoutData(gd);
		fRexxInterpreterLabel.setFont(font);
		
		fRexxInterpreterText = new Text(projComp, SWT.SINGLE | SWT.BORDER);
		gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = 2;
		fRexxInterpreterText.setLayoutData(gd);
		fRexxInterpreterText.setFont(font);
		this.fRexxInterpreterText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent evt) {
				updateLaunchConfigurationDialog();
			}
		});
		
		fInterpreterBrowse = createPushButton(projComp, "Browse", null);
        fInterpreterBrowse.addSelectionListener(new SelectionAdapter() {
            public void widgetSelected(SelectionEvent e) {
                String filePath = fRexxInterpreterText.getText();
                FileDialog dialog = new FileDialog(getShell(), SWT.OPEN);
                filePath = dialog.open();
                if (filePath != null) {
                	fRexxInterpreterText.setText(filePath);
                }
            }
        });
        
        Label spacer = createVerticalSpacer(projComp);
		gd = new GridData();
		gd.horizontalSpan = 3;
		spacer.setLayoutData(gd);
		
		fRexxFileLabel = new Label(projComp, SWT.NONE);
		fRexxFileLabel.setText("File to launch:");
		gd = new GridData();
		gd.horizontalSpan = 3;
		fRexxFileLabel.setLayoutData(gd);
		fRexxFileLabel.setFont(font);

		fRexxFileText = new Text(projComp, SWT.SINGLE | SWT.BORDER);
		gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = 2;
		fRexxFileText.setLayoutData(gd);
		fRexxFileText.setFont(font);
		this.fRexxFileText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent evt) {
				updateLaunchConfigurationDialog();
			}
		});
		
        fFileWorkspaceBrowse = createPushButton(projComp, "Workspace", null); 
        fFileWorkspaceBrowse.addSelectionListener(new SelectionAdapter() {
            public void widgetSelected(SelectionEvent e) {
                ElementTreeSelectionDialog dialog = new ElementTreeSelectionDialog(getShell(), new WorkbenchLabelProvider(), new WorkbenchContentProvider());
                dialog.setTitle("Select a Rexx file"); 
                dialog.setMessage("Select a Rexx file to launch:"); 
                dialog.setInput(ResourcesPlugin.getWorkspace().getRoot()); 
                dialog.setSorter(new ResourceSorter(ResourceSorter.NAME));
                if (dialog.open() == IDialogConstants.OK_ID) {
                    IResource resource = (IResource) dialog.getFirstResult();
                    String arg = "";
                    if (resource instanceof IFile)
                    	arg = resource.getLocation().toOSString();
                    fRexxFileText.setText(arg);
                }
            }
        });
        
        Label spacer2 = createVerticalSpacer(projComp);
		gd = new GridData();
		gd.horizontalSpan = 3;
		spacer.setLayoutData(gd);
		
		fRexxProjectLabel = new Label(projComp, SWT.NONE);
		fRexxProjectLabel.setText("Working directory:");
		gd = new GridData();
		gd.horizontalSpan = 3;
		fRexxProjectLabel.setLayoutData(gd);
		fRexxProjectLabel.setFont(font);

		fRexxProjectText = new Text(projComp, SWT.SINGLE | SWT.BORDER);
		gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = 2;
		fRexxProjectText.setLayoutData(gd);
		fRexxProjectText.setFont(font);
		this.fRexxProjectText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent evt) {
				updateLaunchConfigurationDialog();
			}
		});
		
		fProjectWorkspaceBrowse = createPushButton(projComp, "Workspace", null); 
		fProjectWorkspaceBrowse.addSelectionListener(new SelectionAdapter() {
            public void widgetSelected(SelectionEvent e) {
                ElementTreeSelectionDialog dialog = new ElementTreeSelectionDialog(getShell(), new WorkbenchLabelProvider(), new WorkbenchContentProvider());
                dialog.setTitle("Select a Working Directory"); 
                dialog.setMessage("Select a Project as Working Directory:"); 
                dialog.setInput(ResourcesPlugin.getWorkspace().getRoot()); 
                dialog.setSorter(new ResourceSorter(ResourceSorter.NAME));
                if (dialog.open() == IDialogConstants.OK_ID) {
                    IResource resource = (IResource) dialog.getFirstResult();
                    String arg = "";
                    if (resource instanceof IProject)
                    	arg = resource.getLocation().toOSString();
                    else 
                    	arg = resource.getProject().getLocation().toOSString();
                  
                    fRexxProjectText.setText(arg);
                }
            }
        });
		
		Label spacer3 = createVerticalSpacer(projComp);
			gd = new GridData();
			gd.horizontalSpan = 3;
			spacer.setLayoutData(gd);
			
			fRexxParamLabel = new Label(projComp, SWT.NONE);
			fRexxParamLabel.setText("Parameters:");
			gd = new GridData();
			gd.horizontalSpan = 3;
			fRexxParamLabel.setLayoutData(gd);
			fRexxParamLabel.setFont(font);

			fRexxParamText = new Text(projComp, SWT.SINGLE | SWT.BORDER);
			gd = new GridData(GridData.FILL_HORIZONTAL);
			gd.horizontalSpan = 2;
			fRexxParamText.setLayoutData(gd);
			fRexxParamText.setFont(font);
			this.fRexxParamText.addModifyListener(new ModifyListener() {
				public void modifyText(ModifyEvent evt) {
					updateLaunchConfigurationDialog();
				}
			});
		
				
	}

	public String getName() {
		return fName;
	}

	public void initializeFrom(ILaunchConfiguration configuration) {
		try {
			String interpreter = configuration.getAttribute(IRexxLaunchConstants.REXX_LAUNCH_INTERPRETER, "");
			fRexxInterpreterText.setText(interpreter);	
			String selectedFile = getSelectedFile(configuration.getWorkingCopy()).getLocation().toOSString();
			fRexxFileText.setText(selectedFile);
			String workingDir = getSelectedProject(configuration.getWorkingCopy()).getLocation().toOSString();
			fRexxProjectText.setText(workingDir);
		} catch (CoreException ce) {
			  
		}

	}

	public void performApply(ILaunchConfigurationWorkingCopy configuration) {
		
		configuration.setAttribute(IRexxLaunchConstants.REXX_LAUNCH_STRING_FILE, fRexxFileText.getText());
		configuration.setAttribute(IRexxLaunchConstants.REXX_LAUNCH_INTERPRETER, fRexxInterpreterText.getText());
		configuration.setAttribute(IRexxLaunchConstants.REXX_LAUNCH_WORKING_DIRECTORY, fRexxProjectText.getText());
		configuration.setAttribute(IRexxLaunchConstants.REXX_LAUNCH_PARAMETERS, fRexxParamText.getText());
	}

	public void setDefaults(ILaunchConfigurationWorkingCopy configuration) {
		configuration.setAttribute(IRexxLaunchConstants.REXX_LAUNCH_STRING_FILE, getSelectedFile(configuration).getLocation().toOSString());
		
		String interpPath = Activator.getDefault().getPreferenceStore().getString(IRexxPreferenceFields.PREF_REXX_PATH);
		configuration.setAttribute(IRexxLaunchConstants.REXX_LAUNCH_INTERPRETER, interpPath);
		
		setWorkingDirectory(getSelectedProject(configuration),configuration);
	}
	
	private IFile getSelectedFile(ILaunchConfigurationWorkingCopy config) {
		IWorkbenchWindow window = PlatformUI.getWorkbench().getActiveWorkbenchWindow();
		if (window != null) {
			IWorkbenchPage page = window.getActivePage();
			if (page != null) {
				ISelection selection = page.getSelection();
				if (selection instanceof IStructuredSelection) {
					IStructuredSelection ss = (IStructuredSelection)selection;
					if (!ss.isEmpty()) {
						Object obj = ss.getFirstElement();
						if (obj instanceof IResource) {
							IResource i = (IResource) obj;
							return (IFile)i;
							//return i.getLocation().toOSString();
																			
						}
					}
				}
				// If the editor has the focus...
				IEditorPart part = page.getActiveEditor();
				if (part != null) {
					IEditorInput input = part.getEditorInput();
					IFile file = (IFile) input.getAdapter(IFile.class);
					return file;
				}				
			}
		}
		return null;
							
	}
	
	private IProject getSelectedProject(ILaunchConfigurationWorkingCopy config) {
		IWorkbenchWindow window = PlatformUI.getWorkbench().getActiveWorkbenchWindow();
		if (window != null) {
			IWorkbenchPage page = window.getActivePage();
			if (page != null) {
				ISelection selection = page.getSelection();
				if (selection instanceof IStructuredSelection) {
					IStructuredSelection ss = (IStructuredSelection)selection;
					if (!ss.isEmpty()) {
						Object obj = ss.getFirstElement();
						if (obj instanceof IResource) {
							IResource i = (IResource) obj;
							IProject pro = i.getProject();
							return pro;														
						}
					}
				}
				// If the editor has the focus...
				IEditorPart part = page.getActiveEditor();
				if (part != null) {
					IEditorInput input = part.getEditorInput();
					IFile file = (IFile) input.getAdapter(IFile.class);
					return file.getProject();
				}				
			}
		}
		return null;							
	}
	
	private void setWorkingDirectory(IResource r, ILaunchConfigurationWorkingCopy config) {
		String workingDirectory = r.getLocation().toString();
		if (workingDirectory != "" && workingDirectory != null) {
			workingDirectory += IPath.SEPARATOR;
		}
		config.setAttribute(IRexxLaunchConstants.REXX_LAUNCH_WORKING_DIRECTORY,workingDirectory);
	}

}
