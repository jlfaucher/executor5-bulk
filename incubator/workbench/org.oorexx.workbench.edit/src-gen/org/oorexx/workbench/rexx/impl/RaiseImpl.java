/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx.impl;

import org.eclipse.emf.common.notify.Notification;

import org.eclipse.emf.ecore.EClass;

import org.eclipse.emf.ecore.impl.ENotificationImpl;

import org.oorexx.workbench.rexx.Raise;
import org.oorexx.workbench.rexx.RexxPackage;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>Raise</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.impl.RaiseImpl#getMayor <em>Mayor</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.RaiseImpl#getMinor <em>Minor</em>}</li>
 * </ul>
 * </p>
 *
 * @generated
 */
public class RaiseImpl extends InstructionImpl implements Raise
{
  /**
   * The default value of the '{@link #getMayor() <em>Mayor</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMayor()
   * @generated
   * @ordered
   */
  protected static final String MAYOR_EDEFAULT = null;

  /**
   * The cached value of the '{@link #getMayor() <em>Mayor</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMayor()
   * @generated
   * @ordered
   */
  protected String mayor = MAYOR_EDEFAULT;

  /**
   * The default value of the '{@link #getMinor() <em>Minor</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMinor()
   * @generated
   * @ordered
   */
  protected static final String MINOR_EDEFAULT = null;

  /**
   * The cached value of the '{@link #getMinor() <em>Minor</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMinor()
   * @generated
   * @ordered
   */
  protected String minor = MINOR_EDEFAULT;

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected RaiseImpl()
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
    return RexxPackage.Literals.RAISE;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public String getMayor()
  {
    return mayor;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setMayor(String newMayor)
  {
    String oldMayor = mayor;
    mayor = newMayor;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.RAISE__MAYOR, oldMayor, mayor));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public String getMinor()
  {
    return minor;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setMinor(String newMinor)
  {
    String oldMinor = minor;
    minor = newMinor;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.RAISE__MINOR, oldMinor, minor));
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
      case RexxPackage.RAISE__MAYOR:
        return getMayor();
      case RexxPackage.RAISE__MINOR:
        return getMinor();
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
      case RexxPackage.RAISE__MAYOR:
        setMayor((String)newValue);
        return;
      case RexxPackage.RAISE__MINOR:
        setMinor((String)newValue);
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
      case RexxPackage.RAISE__MAYOR:
        setMayor(MAYOR_EDEFAULT);
        return;
      case RexxPackage.RAISE__MINOR:
        setMinor(MINOR_EDEFAULT);
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
      case RexxPackage.RAISE__MAYOR:
        return MAYOR_EDEFAULT == null ? mayor != null : !MAYOR_EDEFAULT.equals(mayor);
      case RexxPackage.RAISE__MINOR:
        return MINOR_EDEFAULT == null ? minor != null : !MINOR_EDEFAULT.equals(minor);
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
    result.append(" (mayor: ");
    result.append(mayor);
    result.append(", minor: ");
    result.append(minor);
    result.append(')');
    return result.toString();
  }

} //RaiseImpl
