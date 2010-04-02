package rexxeditor.launcher;

import java.util.StringTokenizer;

import org.eclipse.cdt.core.ErrorParserManager;
import org.eclipse.cdt.core.IErrorParser;
import org.eclipse.cdt.core.IMarkerGenerator;
import org.eclipse.core.resources.IFile;

public class ObjectRexxErrorParser implements IErrorParser {

	public boolean processLine(String arg0, ErrorParserManager arg1) {
		System.out.print(arg0.substring(7,9));
		if ( arg0 . startsWith("REX") && arg0.substring(7,9) == "E: ") 
		{
			String rexxerr, severitystr , filestr , linestr , message;
			StringTokenizer tokenizer = new StringTokenizer(arg0, ":");
			try 
			{
				rexxerr = tokenizer.nextToken(); 
				severitystr = tokenizer.nextToken(" ").substring(2).trim(); 
				String running = tokenizer.nextToken("running");
				System.out.println(running);
				
				filestr = tokenizer.nextToken(" ").substring (2). trim (); 
				linestr = tokenizer.nextToken(",").substring (2). trim ();
				message = tokenizer.nextToken("\r\n").substring(2).trim(); 
						int severity = ( severitystr . equals("Error") 
						? IMarkerGenerator.SEVERITY_ERROR_RESOURCE 
						: IMarkerGenerator.SEVERITY_WARNING); 
				int lineno = Integer. parseInt ( linestr . substring (5)); 
				IFile file = arg1.findFilePath( filestr ); 
				arg1.generateMarker(file, lineno , message, severity , null ); 
			} 
			catch (Throwable x) {;}	
		}
		return false;
	}

}
