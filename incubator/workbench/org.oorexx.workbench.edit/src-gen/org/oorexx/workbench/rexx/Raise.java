/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;


/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Raise</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.Raise#getMayor <em>Mayor</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Raise#getMinor <em>Minor</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getRaise()
 * @model
 * @generated
 */
public interface Raise extends Instruction
{
  /**
   * Returns the value of the '<em><b>Mayor</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Mayor</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Mayor</em>' attribute.
   * @see #setMayor(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getRaise_Mayor()
   * @model
   * @generated
   */
  String getMayor();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Raise#getMayor <em>Mayor</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Mayor</em>' attribute.
   * @see #getMayor()
   * @generated
   */
  void setMayor(String value);

  /**
   * Returns the value of the '<em><b>Minor</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Minor</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Minor</em>' attribute.
   * @see #setMinor(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getRaise_Minor()
   * @model
   * @generated
   */
  String getMinor();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Raise#getMinor <em>Minor</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Minor</em>' attribute.
   * @see #getMinor()
   * @generated
   */
  void setMinor(String value);

} // Raise
