/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;

import org.eclipse.emf.ecore.EObject;

/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Requires</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.Requires#getName <em>Name</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getRequires()
 * @model
 * @generated
 */
public interface Requires extends EObject
{
  /**
   * Returns the value of the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Name</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Name</em>' attribute.
   * @see #setName(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getRequires_Name()
   * @model
   * @generated
   */
  String getName();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Requires#getName <em>Name</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Name</em>' attribute.
   * @see #getName()
   * @generated
   */
  void setName(String value);

} // Requires
