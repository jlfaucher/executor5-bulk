package rexxeditor.contentoutliner;


/**
 * A class storing a Rexx content element to be displayed in the content outliner.
 * <p>
 * This element stores a name, an offset relative to the beginning of a document 
 * in characters, a length and a category.
 * </p>
 * 
 * @author gulasch
 *
 */
public class RexxContentElement {
	/**
	 *  The name of the content element.
	 */
	protected String name;
	protected String category;
	protected int offset;
	protected int length;
	
	/**
	 * Constructor for a content element used in the outliner.
	 * 
	 * @param name
	 * 		The name to display in the content outliner.
	 * @param offset
	 * 		The offset in the document.
	 * @param length
	 * 		The length of the element.
	 * @param category
	 * 		The element category that is diplayed in the content outliner. 
	 * 		Should be either METHOD, CLASS, REQUIRES or VARIABLE.
	 */
	public RexxContentElement(String name, int offset, int length, String category) {
		this.name=name;
		this.offset =offset;
		this.length =length;
		this.category=category;
	}
	
	/**
	 * Returns the length of the content element.
	 * 
	 * @return 
	 * 		the length
	 */
	public int getLength() {
		return this.length;
	}
	
	/**
	 * Sets the length of the content element.
	 * 
	 * @param length
	 * 		the length
	 */
	public void setLength(int length) {
		this.length = length;
	}
	
	/**
	 * Returns the name of the content element.
	 * 
	 * @return 
	 * 		the name
	 */
	public String getName() {
		return this.name;
	}
	
	/**
	 * Sets the name of the content element.
	 * 
	 * @param name
	 * 		the name
	 */
	public void setName(String name) {
		this.name = name;
	}
	
	/**
	 * Returns the category of the content element.
	 * 
	 * @return 
	 * 		the category
	 */
	public String getCategory() {
		return this.category;
	}
	
	/**
	 * Returns the length of the content element.
	 * 
	 * @return 
	 * 		the offset
	 */
	public int getOffset() {
		return this.offset;
	}
	
	/**
	 * Sets the offset of the content element.
	 * 
	 * @param offset
	 * 		the offset
	 */
	public void setOffset(int offset) {
		this.offset = offset;
	}
	
	/**
	 * Returns the text that is displayed in the content outliner.
	 * 
	 * @return 
	 * 		A text in the format of 'category::name'.
	 */
	public String toString() {
		return this.category+"::"+this.name;
	}
}
