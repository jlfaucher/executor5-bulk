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

import org.eclipse.emf.ecore.impl.MinimalEObjectImpl;

import org.eclipse.emf.ecore.util.EObjectContainmentEList;
import org.eclipse.emf.ecore.util.InternalEList;

import org.oorexx.workbench.rexx.Directives;
import org.oorexx.workbench.rexx.File;
import org.oorexx.workbench.rexx.Instruction;
import org.oorexx.workbench.rexx.Method;
import org.oorexx.workbench.rexx.Requires;
import org.oorexx.workbench.rexx.RexxPackage;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>File</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * <ul>
 *   <li>{@link org.oorexx.workbench.rexx.impl.FileImpl#getInstructions <em>Instructions</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.FileImpl#getRequires <em>Requires</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.FileImpl#getMethods <em>Methods</em>}</li>
 *   <li>{@link org.oorexx.workbench.rexx.impl.FileImpl#getDirectives <em>Directives</em>}</li>
 * </ul>
 * </p>
 *
 * @generated
 */
public class FileImpl extends MinimalEObjectImpl.Container implements File
{
  /**
   * The cached value of the '{@link #getInstructions() <em>Instructions</em>}' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getInstructions()
   * @generated
   * @ordered
   */
  protected EList<Instruction> instructions;

  /**
   * The cached value of the '{@link #getRequires() <em>Requires</em>}' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getRequires()
   * @generated
   * @ordered
   */
  protected EList<Requires> requires;

  /**
   * The cached value of the '{@link #getMethods() <em>Methods</em>}' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getMethods()
   * @generated
   * @ordered
   */
  protected EList<Method> methods;

  /**
   * The cached value of the '{@link #getDirectives() <em>Directives</em>}' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getDirectives()
   * @generated
   * @ordered
   */
  protected EList<Directives> directives;

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected FileImpl()
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
    return RexxPackage.Literals.FILE;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<Instruction> getInstructions()
  {
    if (instructions == null)
    {
      instructions = new EObjectContainmentEList<Instruction>(Instruction.class, this, RexxPackage.FILE__INSTRUCTIONS);
    }
    return instructions;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<Requires> getRequires()
  {
    if (requires == null)
    {
      requires = new EObjectContainmentEList<Requires>(Requires.class, this, RexxPackage.FILE__REQUIRES);
    }
    return requires;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<Method> getMethods()
  {
    if (methods == null)
    {
      methods = new EObjectContainmentEList<Method>(Method.class, this, RexxPackage.FILE__METHODS);
    }
    return methods;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public EList<Directives> getDirectives()
  {
    if (directives == null)
    {
      directives = new EObjectContainmentEList<Directives>(Directives.class, this, RexxPackage.FILE__DIRECTIVES);
    }
    return directives;
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
      case RexxPackage.FILE__INSTRUCTIONS:
        return ((InternalEList<?>)getInstructions()).basicRemove(otherEnd, msgs);
      case RexxPackage.FILE__REQUIRES:
        return ((InternalEList<?>)getRequires()).basicRemove(otherEnd, msgs);
      case RexxPackage.FILE__METHODS:
        return ((InternalEList<?>)getMethods()).basicRemove(otherEnd, msgs);
      case RexxPackage.FILE__DIRECTIVES:
        return ((InternalEList<?>)getDirectives()).basicRemove(otherEnd, msgs);
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
      case RexxPackage.FILE__INSTRUCTIONS:
        return getInstructions();
      case RexxPackage.FILE__REQUIRES:
        return getRequires();
      case RexxPackage.FILE__METHODS:
        return getMethods();
      case RexxPackage.FILE__DIRECTIVES:
        return getDirectives();
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
      case RexxPackage.FILE__INSTRUCTIONS:
        getInstructions().clear();
        getInstructions().addAll((Collection<? extends Instruction>)newValue);
        return;
      case RexxPackage.FILE__REQUIRES:
        getRequires().clear();
        getRequires().addAll((Collection<? extends Requires>)newValue);
        return;
      case RexxPackage.FILE__METHODS:
        getMethods().clear();
        getMethods().addAll((Collection<? extends Method>)newValue);
        return;
      case RexxPackage.FILE__DIRECTIVES:
        getDirectives().clear();
        getDirectives().addAll((Collection<? extends Directives>)newValue);
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
      case RexxPackage.FILE__INSTRUCTIONS:
        getInstructions().clear();
        return;
      case RexxPackage.FILE__REQUIRES:
        getRequires().clear();
        return;
      case RexxPackage.FILE__METHODS:
        getMethods().clear();
        return;
      case RexxPackage.FILE__DIRECTIVES:
        getDirectives().clear();
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
      case RexxPackage.FILE__INSTRUCTIONS:
        return instructions != null && !instructions.isEmpty();
      case RexxPackage.FILE__REQUIRES:
        return requires != null && !requires.isEmpty();
      case RexxPackage.FILE__METHODS:
        return methods != null && !methods.isEmpty();
      case RexxPackage.FILE__DIRECTIVES:
        return directives != null && !directives.isEmpty();
    }
    return super.eIsSet(featureID);
  }

} //FileImpl
