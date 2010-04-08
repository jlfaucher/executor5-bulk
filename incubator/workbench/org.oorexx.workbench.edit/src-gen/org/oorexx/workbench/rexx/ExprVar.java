/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;


/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Expr Var</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.ExprVar#getName <em>Name</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getExprVar()
 * @model
 * @generated
 */
public interface ExprVar extends Expression
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
   * @see org.oorexx.workbench.rexx.RexxPackage#getExprVar_Name()
   * @model
   * @generated
   */
  String getName();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.ExprVar#getName <em>Name</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Name</em>' attribute.
   * @see #getName()
   * @generated
   */
  void setName(String value);

} // ExprVar
