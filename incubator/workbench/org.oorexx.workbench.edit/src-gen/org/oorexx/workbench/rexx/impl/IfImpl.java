/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx.impl;

import org.eclipse.emf.common.notify.Notification;
import org.eclipse.emf.common.notify.NotificationChain;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.ENotificationImpl;

import org.oorexx.workbench.rexx.Expression;
import org.oorexx.workbench.rexx.If;
import org.oorexx.workbench.rexx.Instruction;
import org.oorexx.workbench.rexx.RexxPackage;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>If</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.impl.IfImpl#getExpression <em>Expression</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.IfImpl#getTopInstr <em>Top Instr</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.IfImpl#getBottomInstr <em>Bottom Instr</em>}</li>
 * </ul>
 * </p>
 *
 * @generated
 */
public class IfImpl extends InstructionImpl implements If
{
  /**
   * The cached value of the '{@link #getExpression() <em>Expression</em>}' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getExpression()
   * @generated
   * @ordered
   */
  protected Expression expression;

  /**
   * The cached value of the '{@link #getTopInstr() <em>Top Instr</em>}' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getTopInstr()
   * @generated
   * @ordered
   */
  protected Instruction topInstr;

  /**
   * The cached value of the '{@link #getBottomInstr() <em>Bottom Instr</em>}' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getBottomInstr()
   * @generated
   * @ordered
   */
  protected Instruction bottomInstr;

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected IfImpl()
  {
    super();
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  protected EClass eStaticClass()
  {
    return RexxPackage.Literals.IF;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Expression getExpression()
  {
    return expression;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public NotificationChain basicSetExpression(Expression newExpression, NotificationChain msgs)
  {
    Expression oldExpression = expression;
    expression = newExpression;
    if (eNotificationRequired())
    {
      ENotificationImpl notification = new ENotificationImpl(this, Notification.SET, RexxPackage.IF__EXPRESSION, oldExpression, newExpression);
      if (msgs == null) msgs = notification; else msgs.add(notification);
    }
    return msgs;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setExpression(Expression newExpression)
  {
    if (newExpression != expression)
    {
      NotificationChain msgs = null;
      if (expression != null)
        msgs = ((InternalEObject)expression).eInverseRemove(this, EOPPOSITE_FEATURE_BASE - RexxPackage.IF__EXPRESSION, null, msgs);
      if (newExpression != null)
        msgs = ((InternalEObject)newExpression).eInverseAdd(this, EOPPOSITE_FEATURE_BASE - RexxPackage.IF__EXPRESSION, null, msgs);
      msgs = basicSetExpression(newExpression, msgs);
      if (msgs != null) msgs.dispatch();
    }
    else if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.IF__EXPRESSION, newExpression, newExpression));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Instruction getTopInstr()
  {
    return topInstr;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public NotificationChain basicSetTopInstr(Instruction newTopInstr, NotificationChain msgs)
  {
    Instruction oldTopInstr = topInstr;
    topInstr = newTopInstr;
    if (eNotificationRequired())
    {
      ENotificationImpl notification = new ENotificationImpl(this, Notification.SET, RexxPackage.IF__TOP_INSTR, oldTopInstr, newTopInstr);
      if (msgs == null) msgs = notification; else msgs.add(notification);
    }
    return msgs;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setTopInstr(Instruction newTopInstr)
  {
    if (newTopInstr != topInstr)
    {
      NotificationChain msgs = null;
      if (topInstr != null)
        msgs = ((InternalEObject)topInstr).eInverseRemove(this, EOPPOSITE_FEATURE_BASE - RexxPackage.IF__TOP_INSTR, null, msgs);
      if (newTopInstr != null)
        msgs = ((InternalEObject)newTopInstr).eInverseAdd(this, EOPPOSITE_FEATURE_BASE - RexxPackage.IF__TOP_INSTR, null, msgs);
      msgs = basicSetTopInstr(newTopInstr, msgs);
      if (msgs != null) msgs.dispatch();
    }
    else if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.IF__TOP_INSTR, newTopInstr, newTopInstr));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Instruction getBottomInstr()
  {
    return bottomInstr;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public NotificationChain basicSetBottomInstr(Instruction newBottomInstr, NotificationChain msgs)
  {
    Instruction oldBottomInstr = bottomInstr;
    bottomInstr = newBottomInstr;
    if (eNotificationRequired())
    {
      ENotificationImpl notification = new ENotificationImpl(this, Notification.SET, RexxPackage.IF__BOTTOM_INSTR, oldBottomInstr, newBottomInstr);
      if (msgs == null) msgs = notification; else msgs.add(notification);
    }
    return msgs;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setBottomInstr(Instruction newBottomInstr)
  {
    if (newBottomInstr != bottomInstr)
    {
      NotificationChain msgs = null;
      if (bottomInstr != null)
        msgs = ((InternalEObject)bottomInstr).eInverseRemove(this, EOPPOSITE_FEATURE_BASE - RexxPackage.IF__BOTTOM_INSTR, null, msgs);
      if (newBottomInstr != null)
        msgs = ((InternalEObject)newBottomInstr).eInverseAdd(this, EOPPOSITE_FEATURE_BASE - RexxPackage.IF__BOTTOM_INSTR, null, msgs);
      msgs = basicSetBottomInstr(newBottomInstr, msgs);
      if (msgs != null) msgs.dispatch();
    }
    else if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.IF__BOTTOM_INSTR, newBottomInstr, newBottomInstr));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public NotificationChain eInverseRemove(InternalEObject otherEnd, int featureID, NotificationChain msgs)
  {
    switch (featureID)
    {
      case RexxPackage.IF__EXPRESSION:
        return basicSetExpression(null, msgs);
      case RexxPackage.IF__TOP_INSTR:
        return basicSetTopInstr(null, msgs);
      case RexxPackage.IF__BOTTOM_INSTR:
        return basicSetBottomInstr(null, msgs);
    }
    return super.eInverseRemove(otherEnd, featureID, msgs);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public Object eGet(int featureID, boolean resolve, boolean coreType)
  {
    switch (featureID)
    {
      case RexxPackage.IF__EXPRESSION:
        return getExpression();
      case RexxPackage.IF__TOP_INSTR:
        return getTopInstr();
      case RexxPackage.IF__BOTTOM_INSTR:
        return getBottomInstr();
    }
    return super.eGet(featureID, resolve, coreType);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public void eSet(int featureID, Object newValue)
  {
    switch (featureID)
    {
      case RexxPackage.IF__EXPRESSION:
        setExpression((Expression)newValue);
        return;
      case RexxPackage.IF__TOP_INSTR:
        setTopInstr((Instruction)newValue);
        return;
      case RexxPackage.IF__BOTTOM_INSTR:
        setBottomInstr((Instruction)newValue);
        return;
    }
    super.eSet(featureID, newValue);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public void eUnset(int featureID)
  {
    switch (featureID)
    {
      case RexxPackage.IF__EXPRESSION:
        setExpression((Expression)null);
        return;
      case RexxPackage.IF__TOP_INSTR:
        setTopInstr((Instruction)null);
        return;
      case RexxPackage.IF__BOTTOM_INSTR:
        setBottomInstr((Instruction)null);
        return;
    }
    super.eUnset(featureID);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public boolean eIsSet(int featureID)
  {
    switch (featureID)
    {
      case RexxPackage.IF__EXPRESSION:
        return expression != null;
      case RexxPackage.IF__TOP_INSTR:
        return topInstr != null;
      case RexxPackage.IF__BOTTOM_INSTR:
        return bottomInstr != null;
    }
    return super.eIsSet(featureID);
  }

} //IfImpl
