/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx.impl;

import java.util.Collection;

import org.eclipse.emf.common.notify.NotificationChain;

import org.eclipse.emf.common.util.EList;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.util.EObjectContainmentEList;
import org.eclipse.emf.ecore.util.InternalEList;

import org.oorexx.workbench.rexx.Do;
import org.oorexx.workbench.rexx.Instruction;
import org.oorexx.workbench.rexx.RexxPackage;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>Do</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.impl.DoImpl#getInstrs <em>Instrs</em>}</li>
 * </ul>
 * </p>
 *
 * @generated
 */
public class DoImpl extends InstructionImpl implements Do
{
  /**
   * The cached value of the '{@link #getInstrs() <em>Instrs</em>}' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getInstrs()
   * @generated
   * @ordered
   */
  protected EList<Instruction> instrs;

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected DoImpl()
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
    return RexxPackage.Literals.DO;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<Instruction> getInstrs()
  {
    if (instrs == null)
    {
      instrs = new EObjectContainmentEList<Instruction>(Instruction.class, this, RexxPackage.DO__INSTRS);
    }
    return instrs;
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
      case RexxPackage.DO__INSTRS:
        return ((InternalEList<?>)getInstrs()).basicRemove(otherEnd, msgs);
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
      case RexxPackage.DO__INSTRS:
        return getInstrs();
    }
    return super.eGet(featureID, resolve, coreType);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @SuppressWarnings("unchecked")
  @Override
  public void eSet(int featureID, Object newValue)
  {
    switch (featureID)
    {
      case RexxPackage.DO__INSTRS:
        getInstrs().clear();
        getInstrs().addAll((Collection<? extends Instruction>)newValue);
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
      case RexxPackage.DO__INSTRS:
        getInstrs().clear();
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
      case RexxPackage.DO__INSTRS:
        return instrs != null && !instrs.isEmpty();
    }
    return super.eIsSet(featureID);
  }

} //DoImpl
