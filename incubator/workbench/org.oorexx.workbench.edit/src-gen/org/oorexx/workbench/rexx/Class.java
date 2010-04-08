/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;

import org.eclipse.emf.common.util.EList;

/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Class</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.Class#getMeta <em>Meta</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Class#getSubclass <em>Subclass</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Class#getMixin <em>Mixin</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Class#getVisibility <em>Visibility</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Class#getInherit <em>Inherit</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Class#getContent <em>Content</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getClass_()
 * @model
 * @generated
 */
public interface Class extends Directives
{
  /**
   * Returns the value of the '<em><b>Meta</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Meta</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Meta</em>' attribute.
   * @see #setMeta(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getClass_Meta()
   * @model
   * @generated
   */
  String getMeta();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Class#getMeta <em>Meta</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Meta</em>' attribute.
   * @see #getMeta()
   * @generated
   */
  void setMeta(String value);

  /**
   * Returns the value of the '<em><b>Subclass</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Subclass</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Subclass</em>' attribute.
   * @see #setSubclass(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getClass_Subclass()
   * @model
   * @generated
   */
  String getSubclass();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Class#getSubclass <em>Subclass</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Subclass</em>' attribute.
   * @see #getSubclass()
   * @generated
   */
  void setSubclass(String value);

  /**
   * Returns the value of the '<em><b>Mixin</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Mixin</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Mixin</em>' attribute.
   * @see #setMixin(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getClass_Mixin()
   * @model
   * @generated
   */
  String getMixin();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Class#getMixin <em>Mixin</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Mixin</em>' attribute.
   * @see #getMixin()
   * @generated
   */
  void setMixin(String value);

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
   * @see org.oorexx.workbench.rexx.RexxPackage#getClass_Visibility()
   * @model
   * @generated
   */
  VISIBILITY getVisibility();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Class#getVisibility <em>Visibility</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Visibility</em>' attribute.
   * @see org.oorexx.workbench.rexx.VISIBILITY
   * @see #getVisibility()
   * @generated
   */
  void setVisibility(VISIBILITY value);

  /**
   * Returns the value of the '<em><b>Inherit</b></em>' attribute list.
   * The list contents are of type {@link java.lang.String}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Inherit</em>' attribute list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Inherit</em>' attribute list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getClass_Inherit()
   * @model unique="false"
   * @generated
   */
  EList<String> getInherit();

  /**
   * Returns the value of the '<em><b>Content</b></em>' containment reference list.
   * The list contents are of type {@link org.oorexx.workbench.rexx.ClassContent}.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Content</em>' containment reference list isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Content</em>' containment reference list.
   * @see org.oorexx.workbench.rexx.RexxPackage#getClass_Content()
   * @model containment="true"
   * @generated
   */
  EList<ClassContent> getContent();

} // Class
