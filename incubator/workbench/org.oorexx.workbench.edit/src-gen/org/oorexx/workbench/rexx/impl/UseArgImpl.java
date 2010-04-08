/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx.impl;

import java.util.Collection;

import org.eclipse.emf.common.notify.Notification;
import org.eclipse.emf.common.notify.NotificationChain;

import org.eclipse.emf.common.util.EList;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.ENotificationImpl;

import org.eclipse.emf.ecore.util.EObjectContainmentEList;
import org.eclipse.emf.ecore.util.InternalEList;

import org.oorexx.workbench.rexx.RexxPackage;
import org.oorexx.workbench.rexx.UseArg;
import org.oorexx.workbench.rexx.UseArgs;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>Use Arg</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.impl.UseArgImpl#isStrict <em>Strict</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.UseArgImpl#getArgs <em>Args</em>}</li>
 * </ul>
 * </p>
 *
 * @generated
 */
public class UseArgImpl extends InstructionImpl implements UseArg
{
  /**
   * The default value of the '{@link #isStrict() <em>Strict</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #isStrict()
   * @generated
   * @ordered
   */
  protected static final boolean STRICT_EDEFAULT = false;

  /**
   * The cached value of the '{@link #isStrict() <em>Strict</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #isStrict()
   * @generated
   * @ordered
   */
  protected boolean strict = STRICT_EDEFAULT;

  /**
   * The cached value of the '{@link #getArgs() <em>Args</em>}' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getArgs()
   * @generated
   * @ordered
   */
  protected EList<UseArgs> args;

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected UseArgImpl()
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
    return RexxPackage.Literals.USE_ARG;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public boolean isStrict()
  {
    return strict;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setStrict(boolean newStrict)
  {
    boolean oldStrict = strict;
    strict = newStrict;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.USE_ARG__STRICT, oldStrict, strict));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<UseArgs> getArgs()
  {
    if (args == null)
    {
      args = new EObjectContainmentEList<UseArgs>(UseArgs.class, this, RexxPackage.USE_ARG__ARGS);
    }
    return args;
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
      case RexxPackage.USE_ARG__ARGS:
        return ((InternalEList<?>)getArgs()).basicRemove(otherEnd, msgs);
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
      case RexxPackage.USE_ARG__STRICT:
        return isStrict();
      case RexxPackage.USE_ARG__ARGS:
        return getArgs();
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
      case RexxPackage.USE_ARG__STRICT:
        setStrict((Boolean)newValue);
        return;
      case RexxPackage.USE_ARG__ARGS:
        getArgs().clear();
        getArgs().addAll((Collection<? extends UseArgs>)newValue);
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
      case RexxPackage.USE_ARG__STRICT:
        setStrict(STRICT_EDEFAULT);
        return;
      case RexxPackage.USE_ARG__ARGS:
        getArgs().clear();
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
      case RexxPackage.USE_ARG__STRICT:
        return strict != STRICT_EDEFAULT;
      case RexxPackage.USE_ARG__ARGS:
        return args != null && !args.isEmpty();
    }
    return super.eIsSet(featureID);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public String toString()
  {
    if (eIsProxy()) return super.toString();

    StringBuffer result = new StringBuffer(super.toString());
    result.append(" (strict: ");
    result.append(strict);
    result.append(')');
    return result.toString();
  }

} //UseArgImpl
