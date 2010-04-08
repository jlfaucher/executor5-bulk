/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;

import org.eclipse.emf.common.util.EList;

/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Use Arg</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.UseArg#isStrict <em>Strict</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.UseArg#getArgs <em>Args</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getUseArg()
 * @model
 * @generated
 */
public interface UseArg extends Instruction
{
  /**
   * Returns the value of the '<em><b>Strict</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Strict</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Strict</em>' attribute.
   * @see #setStrict(boolean)
   * @see org.oorexx.workbench.rexx.RexxPackage#getUseArg_Strict()
   * @model
   * @generated
   */
  boolean isStrict();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.UseArg#isStrict <em>Strict</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Strict</em>' attribute.
   * @see #isStrict()
   * @generated
   */
  void setStrict(boolean value);

  /**
   * Returns the value of the '<em><b>Args</b></em>' containment reference list.
   * The list contents are of type {@link org.oorexx.workbench.rexx.UseArgs}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Args</em>' containment reference list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Args</em>' containment reference list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getUseArg_Args()
   * @model containment="true"
   * @generated
   */
  EList<UseArgs> getArgs();

} // UseArg
