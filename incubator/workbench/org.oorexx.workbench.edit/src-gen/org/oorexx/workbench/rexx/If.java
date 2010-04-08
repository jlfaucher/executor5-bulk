/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;


/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>If</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.If#getExpression <em>Expression</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.If#getTopInstr <em>Top Instr</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.If#getBottomInstr <em>Bottom Instr</em>}</li>
 * </ul>
 * </p>
 *
 * @see org.oorexx.workbench.rexx.RexxPackage#getIf()
 * @model
 * @generated
 */
public interface If extends Instruction
{
  /**
   * Returns the value of the '<em><b>Expression</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Expression</em>' containment reference isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Expression</em>' containment reference.
   * @see #setExpression(Expression)
   * @see org.oorexx.workbench.rexx.RexxPackage#getIf_Expression()
   * @model containment="true"
   * @generated
   */
  Expression getExpression();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.If#getExpression <em>Expression</em>}' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Expression</em>' containment reference.
   * @see #getExpression()
   * @generated
   */
  void setExpression(Expression value);

  /**
   * Returns the value of the '<em><b>Top Instr</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Top Instr</em>' containment reference isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Top Instr</em>' containment reference.
   * @see #setTopInstr(Instruction)
   * @see org.oorexx.workbench.rexx.RexxPackage#getIf_TopInstr()
   * @model containment="true"
   * @generated
   */
  Instruction getTopInstr();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.If#getTopInstr <em>Top Instr</em>}' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Top Instr</em>' containment reference.
   * @see #getTopInstr()
   * @generated
   */
  void setTopInstr(Instruction value);

  /**
   * Returns the value of the '<em><b>Bottom Instr</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <p>
   * If the meaning of the '<em>Bottom Instr</em>' containment reference isn't clear,
   * there really should be more of a description here...
   * </p>
   * <!-- end-user-doc -->
   * @return the value of the '<em>Bottom Instr</em>' containment reference.
   * @see #setBottomInstr(Instruction)
   * @see org.oorexx.workbench.rexx.RexxPackage#getIf_BottomInstr()
   * @model containment="true"
   * @generated
   */
  Instruction getBottomInstr();

  /**
   * Sets the value of the '{@link org.oorexx.workbench.rexx.If#getBottomInstr <em>Bottom Instr</em>}' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param value the new value of the '<em>Bottom Instr</em>' containment reference.
   * @see #getBottomInstr()
   * @generated
   */
  void setBottomInstr(Instruction value);

} // If
