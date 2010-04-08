/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;

import org.eclipse.emf.common.util.EList;

/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Do</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.Do#getInstrs <em>Instrs</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getDo()
 * @model
 * @generated
 */
public interface Do extends Instruction
{
  /**
   * Returns the value of the '<em><b>Instrs</b></em>' containment reference list.
   * The list contents are of type {@link org.oorexx.workbench.rexx.Instruction}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Instrs</em>' containment reference list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Instrs</em>' containment reference list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getDo_Instrs()
   * @model containment="true"
   * @generated
   */
  EList<Instruction> getInstrs();

} // Do
