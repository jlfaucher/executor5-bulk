/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx.util;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

import org.oorexx.workbench.rexx.Arg;
import org.oorexx.workbench.rexx.Arguments;
import org.oorexx.workbench.rexx.ArrayAccess;
import org.oorexx.workbench.rexx.Assignment;
import org.oorexx.workbench.rexx.Attribute;
import org.oorexx.workbench.rexx.AttributeGet;
import org.oorexx.workbench.rexx.AttributeNormal;
import org.oorexx.workbench.rexx.AttributeSet;
import org.oorexx.workbench.rexx.Call;
import org.oorexx.workbench.rexx.ClassContent;
import org.oorexx.workbench.rexx.Const;
import org.oorexx.workbench.rexx.Constant;
import org.oorexx.workbench.rexx.Directives;
import org.oorexx.workbench.rexx.Do;
import org.oorexx.workbench.rexx.ExprVar;
import org.oorexx.workbench.rexx.Expression;
import org.oorexx.workbench.rexx.File;
import org.oorexx.workbench.rexx.If;
import org.oorexx.workbench.rexx.Instruction;
import org.oorexx.workbench.rexx.Message;
import org.oorexx.workbench.rexx.Method;
import org.oorexx.workbench.rexx.Negate;
import org.oorexx.workbench.rexx.Options;
import org.oorexx.workbench.rexx.Raise;
import org.oorexx.workbench.rexx.Requires;
import org.oorexx.workbench.rexx.RexxPackage;
import org.oorexx.workbench.rexx.Routine;
import org.oorexx.workbench.rexx.UseArg;
import org.oorexx.workbench.rexx.UseArgs;
import org.oorexx.workbench.rexx.fnCall;

/**
 * <!-- begin-user-doc -->
 * The <b>Adapter Factory</b> for the model.
 * It provides an adapter <code>createXXX</code> method for each class of the model.
 * <!-- end-user-doc -->
 * @see org.oorexx.workbench.rexx.RexxPackage
 * @generated
 */
public class RexxAdapterFactory extends AdapterFactoryImpl
{
  /**
   * The cached model package.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected static RexxPackage modelPackage;

  /**
   * Creates an instance of the adapter factory.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public RexxAdapterFactory()
  {
    if (modelPackage == null)
    {
      modelPackage = RexxPackage.eINSTANCE;
    }
  }

  /**
   * Returns whether this factory is applicable for the type of the object.
   * <!-- begin-user-doc -->
   * This implementation returns <code>true</code> if the object is either the model's package or is an instance object of the model.
   * <!-- end-user-doc -->
   * @return whether this factory is applicable for the type of the object.
   * @generated
   */
  @Override
  public boolean isFactoryForType(Object object)
  {
    if (object == modelPackage)
    {
      return true;
    }
    if (object instanceof EObject)
    {
      return ((EObject)object).eClass().getEPackage() == modelPackage;
    }
    return false;
  }

  /**
   * The switch that delegates to the <code>createXXX</code> methods.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected RexxSwitch<Adapter> modelSwitch =
    new RexxSwitch<Adapter>()
    {
      @Override
      public Adapter caseFile(File object)
      {
        return createFileAdapter();
      }
      @Override
      public Adapter caseInstruction(Instruction object)
      {
        return createInstructionAdapter();
      }
      @Override
      public Adapter caseAssignment(Assignment object)
      {
        return createAssignmentAdapter();
      }
      @Override
      public Adapter caseIf(If object)
      {
        return createIfAdapter();
      }
      @Override
      public Adapter caseDo(Do object)
      {
        return createDoAdapter();
      }
      @Override
      public Adapter caseCall(Call object)
      {
        return createCallAdapter();
      }
      @Override
      public Adapter caseArg(Arg object)
      {
        return createArgAdapter();
      }
      @Override
      public Adapter caseUseArg(UseArg object)
      {
        return createUseArgAdapter();
      }
      @Override
      public Adapter caseUseArgs(UseArgs object)
      {
        return createUseArgsAdapter();
      }
      @Override
      public Adapter caseArguments(Arguments object)
      {
        return createArgumentsAdapter();
      }
      @Override
      public Adapter caseRaise(Raise object)
      {
        return createRaiseAdapter();
      }
      @Override
      public Adapter caseExpression(Expression object)
      {
        return createExpressionAdapter();
      }
      @Override
      public Adapter caseArrayAccess(ArrayAccess object)
      {
        return createArrayAccessAdapter();
      }
      @Override
      public Adapter casefnCall(fnCall object)
      {
        return createfnCallAdapter();
      }
      @Override
      public Adapter caseMessage(Message object)
      {
        return createMessageAdapter();
      }
      @Override
      public Adapter caseNegate(Negate object)
      {
        return createNegateAdapter();
      }
      @Override
      public Adapter caseConst(Const object)
      {
        return createConstAdapter();
      }
      @Override
      public Adapter caseDirectives(Directives object)
      {
        return createDirectivesAdapter();
      }
      @Override
      public Adapter caseClass(org.oorexx.workbench.rexx.Class object)
      {
        return createClassAdapter();
      }
      @Override
      public Adapter caseClassContent(ClassContent object)
      {
        return createClassContentAdapter();
      }
      @Override
      public Adapter caseMethod(Method object)
      {
        return createMethodAdapter();
      }
      @Override
      public Adapter caseAttribute(Attribute object)
      {
        return createAttributeAdapter();
      }
      @Override
      public Adapter caseAttributeSet(AttributeSet object)
      {
        return createAttributeSetAdapter();
      }
      @Override
      public Adapter caseAttributeGet(AttributeGet object)
      {
        return createAttributeGetAdapter();
      }
      @Override
      public Adapter caseAttributeNormal(AttributeNormal object)
      {
        return createAttributeNormalAdapter();
      }
      @Override
      public Adapter caseConstant(Constant object)
      {
        return createConstantAdapter();
      }
      @Override
      public Adapter caseRoutine(Routine object)
      {
        return createRoutineAdapter();
      }
      @Override
      public Adapter caseRequires(Requires object)
      {
        return createRequiresAdapter();
      }
      @Override
      public Adapter caseOptions(Options object)
      {
        return createOptionsAdapter();
      }
      @Override
      public Adapter caseExprVar(ExprVar object)
      {
        return createExprVarAdapter();
      }
      @Override
      public Adapter defaultCase(EObject object)
      {
        return createEObjectAdapter();
      }
    };

  /**
   * Creates an adapter for the <code>target</code>.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @param target the object to adapt.
   * @return the adapter for the <code>target</code>.
   * @generated
   */
  @Override
  public Adapter createAdapter(Notifier target)
  {
    return modelSwitch.doSwitch((EObject)target);
  }


  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.File <em>File</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.File
   * @generated
   */
  public Adapter createFileAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Instruction <em>Instruction</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Instruction
   * @generated
   */
  public Adapter createInstructionAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Assignment <em>Assignment</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Assignment
   * @generated
   */
  public Adapter createAssignmentAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.If <em>If</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.If
   * @generated
   */
  public Adapter createIfAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Do <em>Do</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Do
   * @generated
   */
  public Adapter createDoAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Call <em>Call</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Call
   * @generated
   */
  public Adapter createCallAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Arg <em>Arg</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Arg
   * @generated
   */
  public Adapter createArgAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.UseArg <em>Use Arg</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.UseArg
   * @generated
   */
  public Adapter createUseArgAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.UseArgs <em>Use Args</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.UseArgs
   * @generated
   */
  public Adapter createUseArgsAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Arguments <em>Arguments</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Arguments
   * @generated
   */
  public Adapter createArgumentsAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Raise <em>Raise</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Raise
   * @generated
   */
  public Adapter createRaiseAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Expression <em>Expression</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Expression
   * @generated
   */
  public Adapter createExpressionAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.ArrayAccess <em>Array Access</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.ArrayAccess
   * @generated
   */
  public Adapter createArrayAccessAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.fnCall <em>fn Call</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.fnCall
   * @generated
   */
  public Adapter createfnCallAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Message <em>Message</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Message
   * @generated
   */
  public Adapter createMessageAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Negate <em>Negate</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Negate
   * @generated
   */
  public Adapter createNegateAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Const <em>Const</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Const
   * @generated
   */
  public Adapter createConstAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Directives <em>Directives</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Directives
   * @generated
   */
  public Adapter createDirectivesAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Class <em>Class</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Class
   * @generated
   */
  public Adapter createClassAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.ClassContent <em>Class Content</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.ClassContent
   * @generated
   */
  public Adapter createClassContentAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Method <em>Method</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Method
   * @generated
   */
  public Adapter createMethodAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Attribute <em>Attribute</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Attribute
   * @generated
   */
  public Adapter createAttributeAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.AttributeSet <em>Attribute Set</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.AttributeSet
   * @generated
   */
  public Adapter createAttributeSetAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.AttributeGet <em>Attribute Get</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.AttributeGet
   * @generated
   */
  public Adapter createAttributeGetAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.AttributeNormal <em>Attribute Normal</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.AttributeNormal
   * @generated
   */
  public Adapter createAttributeNormalAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Constant <em>Constant</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Constant
   * @generated
   */
  public Adapter createConstantAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Routine <em>Routine</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Routine
   * @generated
   */
  public Adapter createRoutineAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Requires <em>Requires</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Requires
   * @generated
   */
  public Adapter createRequiresAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.Options <em>Options</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.Options
   * @generated
   */
  public Adapter createOptionsAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for an object of class '{@link org.oorexx.workbench.rexx.ExprVar <em>Expr Var</em>}'.
   * <!-- begin-user-doc -->
   * This default implementation returns null so that we can easily ignore cases;
   * it's useful to ignore a case when inheritance will catch all the cases anyway.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @see org.oorexx.workbench.rexx.ExprVar
   * @generated
   */
  public Adapter createExprVarAdapter()
  {
    return null;
  }

  /**
   * Creates a new adapter for the default case.
   * <!-- begin-user-doc -->
   * This default implementation returns null.
   * <!-- end-user-doc -->
   * @return the new adapter.
   * @generated
   */
  public Adapter createEObjectAdapter()
  {
    return null;
  }

} //RexxAdapterFactory
