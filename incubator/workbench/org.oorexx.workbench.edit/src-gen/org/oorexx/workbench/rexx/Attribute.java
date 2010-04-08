/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;

import org.eclipse.emf.common.util.EList;

/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Attribute</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.Attribute#getVisibility <em>Visibility</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Attribute#isClass <em>Class</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Attribute#getExpose <em>Expose</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Attribute#getContent <em>Content</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getAttribute()
 * @model
 * @generated
 */
public interface Attribute extends ClassContent
{
  /**
   * Returns the value of the '<em><b>Visibility</b></em>' attribute.
   * The literals are from the enumeration {@link org.oorexx.workbench.rexx.VISIBILITY}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Visibility</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Visibility</em>' attribute.
   * @see org.oorexx.workbench.rexx.VISIBILITY
   * @see #setVisibility(VISIBILITY)
   * @see org.oorexx.workbench.rexx.RexxPackage#getAttribute_Visibility()
   * @model
   * @generated
   */
  VISIBILITY getVisibility();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Attribute#getVisibility <em>Visibility</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Visibility</em>' attribute.
   * @see org.oorexx.workbench.rexx.VISIBILITY
   * @see #getVisibility()
   * @generated
   */
  void setVisibility(VISIBILITY value);

  /**
   * Returns the value of the '<em><b>Class</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Class</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Class</em>' attribute.
   * @see #setClass(boolean)
   * @see org.oorexx.workbench.rexx.RexxPackage#getAttribute_Class()
   * @model
   * @generated
   */
  boolean isClass();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Attribute#isClass <em>Class</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Class</em>' attribute.
   * @see #isClass()
   * @generated
   */
  void setClass(boolean value);

  /**
   * Returns the value of the '<em><b>Expose</b></em>' attribute list.
   * The list contents are of type {@link java.lang.String}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Expose</em>' attribute list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Expose</em>' attribute list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getAttribute_Expose()
   * @model unique="false"
   * @generated
   */
  EList<String> getExpose();

  /**
   * Returns the value of the '<em><b>Content</b></em>' containment reference list.
   * The list contents are of type {@link org.oorexx.workbench.rexx.Instruction}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Content</em>' containment reference list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Content</em>' containment reference list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getAttribute_Content()
   * @model containment="true"
   * @generated
   */
  EList<Instruction> getContent();

} // Attribute
