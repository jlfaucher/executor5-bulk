/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx.util;

import java.util.List;

import org.eclipse.emf.ecore.EClass;
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
 * The <b>Switch</b> for the model's inheritance hierarchy.
 * It supports the call {@link #doSwitch(EObject) doSwitch(object)}
 * to invoke the <code>caseXXX</code> method for each class of the model,
 * starting with the actual class of the object
 * and proceeding up the inheritance hierarchy
 * until a non-null result is returned,
 * which is the result of the switch.
 * <!-- end-user-doc -->
 * @see org.oorexx.workbench.rexx.RexxPackage
 * @generated
 */
public class RexxSwitch<T>
{
  /**
   * The cached model package
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected static RexxPackage modelPackage;

  /**
   * Creates an instance of the switch.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public RexxSwitch()
  {
    if (modelPackage == null)
    {
      modelPackage = RexxPackage.eINSTANCE;
    }
  }

  /**
   * Calls <code>caseXXX</code> for each class of the model until one returns a non null result; it yields that result.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the first non-null result returned by a <code>caseXXX</code> call.
   * @generated
   */
  public T doSwitch(EObject theEObject)
  {
    return doSwitch(theEObject.eClass(), theEObject);
  }

  /**
   * Calls <code>caseXXX</code> for each class of the model until one returns a non null result; it yields that result.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the first non-null result returned by a <code>caseXXX</code> call.
   * @generated
   */
  protected T doSwitch(EClass theEClass, EObject theEObject)
  {
    if (theEClass.eContainer() == modelPackage)
    {
      return doSwitch(theEClass.getClassifierID(), theEObject);
    }
    else
    {
      List<EClass> eSuperTypes = theEClass.getESuperTypes();
      return
        eSuperTypes.isEmpty() ?
          defaultCase(theEObject) :
          doSwitch(eSuperTypes.get(0), theEObject);
    }
  }

  /**
   * Calls <code>caseXXX</code> for each class of the model until one returns a non null result; it yields that result.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the first non-null result returned by a <code>caseXXX</code> call.
   * @generated
   */
  protected T doSwitch(int classifierID, EObject theEObject)
  {
    switch (classifierID)
    {
      case RexxPackage.FILE:
      {
        File file = (File)theEObject;
        T result = caseFile(file);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.INSTRUCTION:
      {
        Instruction instruction = (Instruction)theEObject;
        T result = caseInstruction(instruction);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ASSIGNMENT:
      {
        Assignment assignment = (Assignment)theEObject;
        T result = caseAssignment(assignment);
        if (result == null) result = caseInstruction(assignment);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.IF:
      {
        If if_ = (If)theEObject;
        T result = caseIf(if_);
        if (result == null) result = caseInstruction(if_);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.DO:
      {
        Do do_ = (Do)theEObject;
        T result = caseDo(do_);
        if (result == null) result = caseInstruction(do_);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.CALL:
      {
        Call call = (Call)theEObject;
        T result = caseCall(call);
        if (result == null) result = caseInstruction(call);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ARG:
      {
        Arg arg = (Arg)theEObject;
        T result = caseArg(arg);
        if (result == null) result = caseInstruction(arg);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.USE_ARG:
      {
        UseArg useArg = (UseArg)theEObject;
        T result = caseUseArg(useArg);
        if (result == null) result = caseInstruction(useArg);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.USE_ARGS:
      {
        UseArgs useArgs = (UseArgs)theEObject;
        T result = caseUseArgs(useArgs);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ARGUMENTS:
      {
        Arguments arguments = (Arguments)theEObject;
        T result = caseArguments(arguments);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.RAISE:
      {
        Raise raise = (Raise)theEObject;
        T result = caseRaise(raise);
        if (result == null) result = caseInstruction(raise);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.EXPRESSION:
      {
        Expression expression = (Expression)theEObject;
        T result = caseExpression(expression);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ARRAY_ACCESS:
      {
        ArrayAccess arrayAccess = (ArrayAccess)theEObject;
        T result = caseArrayAccess(arrayAccess);
        if (result == null) result = caseExpression(arrayAccess);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.FN_CALL:
      {
        fnCall fnCall = (fnCall)theEObject;
        T result = casefnCall(fnCall);
        if (result == null) result = caseExpression(fnCall);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.MESSAGE:
      {
        Message message = (Message)theEObject;
        T result = caseMessage(message);
        if (result == null) result = caseInstruction(message);
        if (result == null) result = caseExpression(message);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.NEGATE:
      {
        Negate negate = (Negate)theEObject;
        T result = caseNegate(negate);
        if (result == null) result = caseExpression(negate);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.CONST:
      {
        Const const_ = (Const)theEObject;
        T result = caseConst(const_);
        if (result == null) result = caseExpression(const_);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.DIRECTIVES:
      {
        Directives directives = (Directives)theEObject;
        T result = caseDirectives(directives);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.CLASS:
      {
        org.oorexx.workbench.rexx.Class class_ = (org.oorexx.workbench.rexx.Class)theEObject;
        T result = caseClass(class_);
        if (result == null) result = caseDirectives(class_);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.CLASS_CONTENT:
      {
        ClassContent classContent = (ClassContent)theEObject;
        T result = caseClassContent(classContent);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.METHOD:
      {
        Method method = (Method)theEObject;
        T result = caseMethod(method);
        if (result == null) result = caseClassContent(method);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ATTRIBUTE:
      {
        Attribute attribute = (Attribute)theEObject;
        T result = caseAttribute(attribute);
        if (result == null) result = caseClassContent(attribute);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ATTRIBUTE_SET:
      {
        AttributeSet attributeSet = (AttributeSet)theEObject;
        T result = caseAttributeSet(attributeSet);
        if (result == null) result = caseAttribute(attributeSet);
        if (result == null) result = caseClassContent(attributeSet);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ATTRIBUTE_GET:
      {
        AttributeGet attributeGet = (AttributeGet)theEObject;
        T result = caseAttributeGet(attributeGet);
        if (result == null) result = caseAttribute(attributeGet);
        if (result == null) result = caseClassContent(attributeGet);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ATTRIBUTE_NORMAL:
      {
        AttributeNormal attributeNormal = (AttributeNormal)theEObject;
        T result = caseAttributeNormal(attributeNormal);
        if (result == null) result = caseAttribute(attributeNormal);
        if (result == null) result = caseClassContent(attributeNormal);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.CONSTANT:
      {
        Constant constant = (Constant)theEObject;
        T result = caseConstant(constant);
        if (result == null) result = caseClassContent(constant);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.ROUTINE:
      {
        Routine routine = (Routine)theEObject;
        T result = caseRoutine(routine);
        if (result == null) result = caseDirectives(routine);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.REQUIRES:
      {
        Requires requires = (Requires)theEObject;
        T result = caseRequires(requires);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.OPTIONS:
      {
        Options options = (Options)theEObject;
        T result = caseOptions(options);
        if (result == null) result = caseDirectives(options);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      case RexxPackage.EXPR_VAR:
      {
        ExprVar exprVar = (ExprVar)theEObject;
        T result = caseExprVar(exprVar);
        if (result == null) result = caseExpression(exprVar);
        if (result == null) result = defaultCase(theEObject);
        return result;
      }
      default: return defaultCase(theEObject);
    }
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>File</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>File</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseFile(File object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Instruction</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Instruction</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseInstruction(Instruction object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Assignment</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Assignment</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseAssignment(Assignment object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>If</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>If</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseIf(If object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Do</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Do</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseDo(Do object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Call</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Call</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseCall(Call object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Arg</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Arg</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseArg(Arg object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Use Arg</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Use Arg</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseUseArg(UseArg object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Use Args</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Use Args</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseUseArgs(UseArgs object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Arguments</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Arguments</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseArguments(Arguments object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Raise</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Raise</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseRaise(Raise object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Expression</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Expression</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseExpression(Expression object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Array Access</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Array Access</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseArrayAccess(ArrayAccess object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>fn Call</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>fn Call</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T casefnCall(fnCall object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Message</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Message</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseMessage(Message object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Negate</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Negate</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseNegate(Negate object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Const</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Const</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseConst(Const object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Directives</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Directives</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseDirectives(Directives object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Class</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Class</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseClass(org.oorexx.workbench.rexx.Class object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Class Content</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Class Content</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseClassContent(ClassContent object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Method</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Method</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseMethod(Method object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Attribute</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Attribute</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseAttribute(Attribute object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Attribute Set</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Attribute Set</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseAttributeSet(AttributeSet object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Attribute Get</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Attribute Get</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseAttributeGet(AttributeGet object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Attribute Normal</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Attribute Normal</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseAttributeNormal(AttributeNormal object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Constant</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Constant</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseConstant(Constant object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Routine</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Routine</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseRoutine(Routine object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Requires</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Requires</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseRequires(Requires object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Options</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Options</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseOptions(Options object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>Expr Var</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>Expr Var</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
   * @generated
   */
  public T caseExprVar(ExprVar object)
  {
    return null;
  }

  /**
   * Returns the result of interpreting the object as an instance of '<em>EObject</em>'.
   * <!-- begin-user-doc -->
   * This implementation returns null;
   * returning a non-null result will terminate the switch, but this is the last case anyway.
   * <!-- end-user-doc -->
   * @param object the target of the switch.
   * @return the result of interpreting the object as an instance of '<em>EObject</em>'.
   * @see #doSwitch(org.eclipse.emf.ecore.EObject)
   * @generated
   */
  public T defaultCase(EObject object)
  {
    return null;
  }

} //RexxSwitch
