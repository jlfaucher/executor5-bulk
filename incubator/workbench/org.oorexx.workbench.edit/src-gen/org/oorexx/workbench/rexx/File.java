/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;

import org.eclipse.emf.common.util.EList;

import org.eclipse.emf.ecore.EObject;

/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>File</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.File#getInstructions <em>Instructions</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.File#getRequires <em>Requires</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.File#getMethods <em>Methods</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.File#getDirectives <em>Directives</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getFile()
 * @model
 * @generated
 */
public interface File extends EObject
{
  /**
   * Returns the value of the '<em><b>Instructions</b></em>' containment reference list.
   * The list contents are of type {@link org.oorexx.workbench.rexx.Instruction}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Instructions</em>' containment reference list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Instructions</em>' containment reference list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getFile_Instructions()
   * @model containment="true"
   * @generated
   */
  EList<Instruction> getInstructions();

  /**
   * Returns the value of the '<em><b>Requires</b></em>' containment reference list.
   * The list contents are of type {@link org.oorexx.workbench.rexx.Requires}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Requires</em>' containment reference list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Requires</em>' containment reference list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getFile_Requires()
   * @model containment="true"
   * @generated
   */
  EList<Requires> getRequires();

  /**
   * Returns the value of the '<em><b>Methods</b></em>' containment reference list.
   * The list contents are of type {@link org.oorexx.workbench.rexx.Method}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Methods</em>' containment reference list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Methods</em>' containment reference list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getFile_Methods()
   * @model containment="true"
   * @generated
   */
  EList<Method> getMethods();

  /**
   * Returns the value of the '<em><b>Directives</b></em>' containment reference list.
   * The list contents are of type {@link org.oorexx.workbench.rexx.Directives}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Directives</em>' containment reference list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Directives</em>' containment reference list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getFile_Directives()
   * @model containment="true"
   * @generated
   */
  EList<Directives> getDirectives();

} // File
