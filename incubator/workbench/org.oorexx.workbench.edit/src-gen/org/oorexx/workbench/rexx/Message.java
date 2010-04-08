/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;


/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Message</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.Message#getTarget <em>Target</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Message#isDiscard <em>Discard</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Message#getMessage <em>Message</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Message#getScope <em>Scope</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Message#getArgs <em>Args</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.Message#getValue <em>Value</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getMessage()
 * @model
 * @generated
 */
public interface Message extends Instruction, Expression
{
  /**
   * Returns the value of the '<em><b>Target</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Target</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Target</em>' attribute.
   * @see #setTarget(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getMessage_Target()
   * @model
   * @generated
   */
  String getTarget();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Message#getTarget <em>Target</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Target</em>' attribute.
   * @see #getTarget()
   * @generated
   */
  void setTarget(String value);

  /**
   * Returns the value of the '<em><b>Discard</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Discard</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Discard</em>' attribute.
   * @see #setDiscard(boolean)
   * @see org.oorexx.workbench.rexx.RexxPackage#getMessage_Discard()
   * @model
   * @generated
   */
  boolean isDiscard();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Message#isDiscard <em>Discard</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Discard</em>' attribute.
   * @see #isDiscard()
   * @generated
   */
  void setDiscard(boolean value);

  /**
   * Returns the value of the '<em><b>Message</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Message</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Message</em>' attribute.
   * @see #setMessage(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getMessage_Message()
   * @model
   * @generated
   */
  String getMessage();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Message#getMessage <em>Message</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Message</em>' attribute.
   * @see #getMessage()
   * @generated
   */
  void setMessage(String value);

  /**
   * Returns the value of the '<em><b>Scope</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Scope</em>' attribute isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Scope</em>' attribute.
   * @see #setScope(String)
   * @see org.oorexx.workbench.rexx.RexxPackage#getMessage_Scope()
   * @model
   * @generated
   */
  String getScope();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Message#getScope <em>Scope</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Scope</em>' attribute.
   * @see #getScope()
   * @generated
   */
  void setScope(String value);

  /**
   * Returns the value of the '<em><b>Args</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Args</em>' containment reference isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Args</em>' containment reference.
   * @see #setArgs(Arguments)
   * @see org.oorexx.workbench.rexx.RexxPackage#getMessage_Args()
   * @model containment="true"
   * @generated
   */
  Arguments getArgs();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Message#getArgs <em>Args</em>}' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Args</em>' containment reference.
   * @see #getArgs()
   * @generated
   */
  void setArgs(Arguments value);

  /**
   * Returns the value of the '<em><b>Value</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Value</em>' containment reference isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Value</em>' containment reference.
   * @see #setValue(Expression)
   * @see org.oorexx.workbench.rexx.RexxPackage#getMessage_Value()
   * @model containment="true"
   * @generated
   */
  Expression getValue();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.Message#getValue <em>Value</em>}' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Value</em>' containment reference.
   * @see #getValue()
   * @generated
   */
  void setValue(Expression value);

} // Message
