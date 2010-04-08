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

import org.eclipse.emf.ecore.util.EDataTypeEList;
import org.eclipse.emf.ecore.util.EObjectContainmentEList;
import org.eclipse.emf.ecore.util.InternalEList;

import org.oorexx.workbench.rexx.Instruction;
import org.oorexx.workbench.rexx.Method;
import org.oorexx.workbench.rexx.RexxPackage;
import org.oorexx.workbench.rexx.VISIBILITY;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>Method</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.impl.MethodImpl#getVisibility <em>Visibility</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.MethodImpl#isClass <em>Class</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.MethodImpl#getExpose <em>Expose</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.MethodImpl#getContent <em>Content</em>}</li>
 * </ul>
 * </p>
 *
 * @generated
 */
public class MethodImpl extends ClassContentImpl implements Method
{
  /**
   * The default value of the '{@link #getVisibility() <em>Visibility</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getVisibility()
   * @generated
   * @ordered
   */
  protected static final VISIBILITY VISIBILITY_EDEFAULT = VISIBILITY.PRIVATE;

  /**
   * The cached value of the '{@link #getVisibility() <em>Visibility</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getVisibility()
   * @generated
   * @ordered
   */
  protected VISIBILITY visibility = VISIBILITY_EDEFAULT;

  /**
   * The default value of the '{@link #isClass() <em>Class</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #isClass()
   * @generated
   * @ordered
   */
  protected static final boolean CLASS_EDEFAULT = false;

  /**
   * The cached value of the '{@link #isClass() <em>Class</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #isClass()
   * @generated
   * @ordered
   */
  protected boolean class_ = CLASS_EDEFAULT;

  /**
   * The cached value of the '{@link #getExpose() <em>Expose</em>}' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getExpose()
   * @generated
   * @ordered
   */
  protected EList<String> expose;

  /**
   * The cached value of the '{@link #getContent() <em>Content</em>}' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getContent()
   * @generated
   * @ordered
   */
  protected EList<Instruction> content;

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected MethodImpl()
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
    return RexxPackage.Literals.METHOD;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public VISIBILITY getVisibility()
  {
    return visibility;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setVisibility(VISIBILITY newVisibility)
  {
    VISIBILITY oldVisibility = visibility;
    visibility = newVisibility == null ? VISIBILITY_EDEFAULT : newVisibility;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.METHOD__VISIBILITY, oldVisibility, visibility));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public boolean isClass()
  {
    return class_;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setClass(boolean newClass)
  {
    boolean oldClass = class_;
    class_ = newClass;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.METHOD__CLASS, oldClass, class_));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<String> getExpose()
  {
    if (expose == null)
    {
      expose = new EDataTypeEList<String>(String.class, this, RexxPackage.METHOD__EXPOSE);
    }
    return expose;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<Instruction> getContent()
  {
    if (content == null)
    {
      content = new EObjectContainmentEList<Instruction>(Instruction.class, this, RexxPackage.METHOD__CONTENT);
    }
    return content;
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
      case RexxPackage.METHOD__CONTENT:
        return ((InternalEList<?>)getContent()).basicRemove(otherEnd, msgs);
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
      case RexxPackage.METHOD__VISIBILITY:
        return getVisibility();
      case RexxPackage.METHOD__CLASS:
        return isClass();
      case RexxPackage.METHOD__EXPOSE:
        return getExpose();
      case RexxPackage.METHOD__CONTENT:
        return getContent();
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
      case RexxPackage.METHOD__VISIBILITY:
        setVisibility((VISIBILITY)newValue);
        return;
      case RexxPackage.METHOD__CLASS:
        setClass((Boolean)newValue);
        return;
      case RexxPackage.METHOD__EXPOSE:
        getExpose().clear();
        getExpose().addAll((Collection<? extends String>)newValue);
        return;
      case RexxPackage.METHOD__CONTENT:
        getContent().clear();
        getContent().addAll((Collection<? extends Instruction>)newValue);
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
      case RexxPackage.METHOD__VISIBILITY:
        setVisibility(VISIBILITY_EDEFAULT);
        return;
      case RexxPackage.METHOD__CLASS:
        setClass(CLASS_EDEFAULT);
        return;
      case RexxPackage.METHOD__EXPOSE:
        getExpose().clear();
        return;
      case RexxPackage.METHOD__CONTENT:
        getContent().clear();
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
      case RexxPackage.METHOD__VISIBILITY:
        return visibility != VISIBILITY_EDEFAULT;
      case RexxPackage.METHOD__CLASS:
        return class_ != CLASS_EDEFAULT;
      case RexxPackage.METHOD__EXPOSE:
        return expose != null && !expose.isEmpty();
      case RexxPackage.METHOD__CONTENT:
        return content != null && !content.isEmpty();
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
    result.append(" (visibility: ");
    result.append(visibility);
    result.append(", class: ");
    result.append(class_);
    result.append(", expose: ");
    result.append(expose);
    result.append(')');
    return result.toString();
  }

} //MethodImpl
