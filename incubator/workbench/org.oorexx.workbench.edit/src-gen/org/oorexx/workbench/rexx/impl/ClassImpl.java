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

import org.oorexx.workbench.rexx.ClassContent;
import org.oorexx.workbench.rexx.RexxPackage;
import org.oorexx.workbench.rexx.VISIBILITY;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>Class</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.impl.ClassImpl#getMeta <em>Meta</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.ClassImpl#getSubclass <em>Subclass</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.ClassImpl#getMixin <em>Mixin</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.ClassImpl#getVisibility <em>Visibility</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.ClassImpl#getInherit <em>Inherit</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.ClassImpl#getContent <em>Content</em>}</li>
 * </ul>
 * </p>
 *
 * @generated
 */
public class ClassImpl extends DirectivesImpl implements org.oorexx.workbench.rexx.Class
{
  /**
   * The default value of the '{@link #getMeta() <em>Meta</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMeta()
   * @generated
   * @ordered
   */
  protected static final String META_EDEFAULT = null;

  /**
   * The cached value of the '{@link #getMeta() <em>Meta</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMeta()
   * @generated
   * @ordered
   */
  protected String meta = META_EDEFAULT;

  /**
   * The default value of the '{@link #getSubclass() <em>Subclass</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getSubclass()
   * @generated
   * @ordered
   */
  protected static final String SUBCLASS_EDEFAULT = null;

  /**
   * The cached value of the '{@link #getSubclass() <em>Subclass</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getSubclass()
   * @generated
   * @ordered
   */
  protected String subclass = SUBCLASS_EDEFAULT;

  /**
   * The default value of the '{@link #getMixin() <em>Mixin</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMixin()
   * @generated
   * @ordered
   */
  protected static final String MIXIN_EDEFAULT = null;

  /**
   * The cached value of the '{@link #getMixin() <em>Mixin</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMixin()
   * @generated
   * @ordered
   */
  protected String mixin = MIXIN_EDEFAULT;

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
   * The cached value of the '{@link #getInherit() <em>Inherit</em>}' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getInherit()
   * @generated
   * @ordered
   */
  protected EList<String> inherit;

  /**
   * The cached value of the '{@link #getContent() <em>Content</em>}' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getContent()
   * @generated
   * @ordered
   */
  protected EList<ClassContent> content;

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected ClassImpl()
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
    return RexxPackage.Literals.CLASS;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public String getMeta()
  {
    return meta;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setMeta(String newMeta)
  {
    String oldMeta = meta;
    meta = newMeta;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.CLASS__META, oldMeta, meta));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public String getSubclass()
  {
    return subclass;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setSubclass(String newSubclass)
  {
    String oldSubclass = subclass;
    subclass = newSubclass;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.CLASS__SUBCLASS, oldSubclass, subclass));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public String getMixin()
  {
    return mixin;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setMixin(String newMixin)
  {
    String oldMixin = mixin;
    mixin = newMixin;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.CLASS__MIXIN, oldMixin, mixin));
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
      eNotify(new ENotificationImpl(this, Notification.SET, RexxPackage.CLASS__VISIBILITY, oldVisibility, visibility));
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<String> getInherit()
  {
    if (inherit == null)
    {
      inherit = new EDataTypeEList<String>(String.class, this, RexxPackage.CLASS__INHERIT);
    }
    return inherit;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<ClassContent> getContent()
  {
    if (content == null)
    {
      content = new EObjectContainmentEList<ClassContent>(ClassContent.class, this, RexxPackage.CLASS__CONTENT);
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
      case RexxPackage.CLASS__CONTENT:
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
      case RexxPackage.CLASS__META:
        return getMeta();
      case RexxPackage.CLASS__SUBCLASS:
        return getSubclass();
      case RexxPackage.CLASS__MIXIN:
        return getMixin();
      case RexxPackage.CLASS__VISIBILITY:
        return getVisibility();
      case RexxPackage.CLASS__INHERIT:
        return getInherit();
      case RexxPackage.CLASS__CONTENT:
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
      case RexxPackage.CLASS__META:
        setMeta((String)newValue);
        return;
      case RexxPackage.CLASS__SUBCLASS:
        setSubclass((String)newValue);
        return;
      case RexxPackage.CLASS__MIXIN:
        setMixin((String)newValue);
        return;
      case RexxPackage.CLASS__VISIBILITY:
        setVisibility((VISIBILITY)newValue);
        return;
      case RexxPackage.CLASS__INHERIT:
        getInherit().clear();
        getInherit().addAll((Collection<? extends String>)newValue);
        return;
      case RexxPackage.CLASS__CONTENT:
        getContent().clear();
        getContent().addAll((Collection<? extends ClassContent>)newValue);
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
      case RexxPackage.CLASS__META:
        setMeta(META_EDEFAULT);
        return;
      case RexxPackage.CLASS__SUBCLASS:
        setSubclass(SUBCLASS_EDEFAULT);
        return;
      case RexxPackage.CLASS__MIXIN:
        setMixin(MIXIN_EDEFAULT);
        return;
      case RexxPackage.CLASS__VISIBILITY:
        setVisibility(VISIBILITY_EDEFAULT);
        return;
      case RexxPackage.CLASS__INHERIT:
        getInherit().clear();
        return;
      case RexxPackage.CLASS__CONTENT:
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
      case RexxPackage.CLASS__META:
        return META_EDEFAULT == null ? meta != null : !META_EDEFAULT.equals(meta);
      case RexxPackage.CLASS__SUBCLASS:
        return SUBCLASS_EDEFAULT == null ? subclass != null : !SUBCLASS_EDEFAULT.equals(subclass);
      case RexxPackage.CLASS__MIXIN:
        return MIXIN_EDEFAULT == null ? mixin != null : !MIXIN_EDEFAULT.equals(mixin);
      case RexxPackage.CLASS__VISIBILITY:
        return visibility != VISIBILITY_EDEFAULT;
      case RexxPackage.CLASS__INHERIT:
        return inherit != null && !inherit.isEmpty();
      case RexxPackage.CLASS__CONTENT:
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
    result.append(" (meta: ");
    result.append(meta);
    result.append(", subclass: ");
    result.append(subclass);
    result.append(", mixin: ");
    result.append(mixin);
    result.append(", visibility: ");
    result.append(visibility);
    result.append(", inherit: ");
    result.append(inherit);
    result.append(')');
    return result.toString();
  }

} //ClassImpl
