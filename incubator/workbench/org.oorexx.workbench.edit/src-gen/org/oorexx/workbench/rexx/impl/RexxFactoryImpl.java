/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx.impl;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EDataType;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

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
import org.oorexx.workbench.rexx.RexxFactory;
import org.oorexx.workbench.rexx.RexxPackage;
import org.oorexx.workbench.rexx.Routine;
import org.oorexx.workbench.rexx.UseArg;
import org.oorexx.workbench.rexx.UseArgs;
import org.oorexx.workbench.rexx.VISIBILITY;
import org.oorexx.workbench.rexx.fnCall;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model <b>Factory</b>.
 * <!-- end-user-doc -->
 * @generated
 */
public class RexxFactoryImpl extends EFactoryImpl implements RexxFactory
{
  /**
   * Creates the default factory implementation.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public static RexxFactory init()
  {
    try
    {
      RexxFactory theRexxFactory = (RexxFactory)EPackage.Registry.INSTANCE.getEFactory("http://www.oorexx.org/workbench/Rexx"); 
      if (theRexxFactory != null)
      {
        return theRexxFactory;
      }
    }
    catch (Exception exception)
    {
      EcorePlugin.INSTANCE.log(exception);
    }
    return new RexxFactoryImpl();
  }

  /**
   * Creates an instance of the factory.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public RexxFactoryImpl()
  {
    super();
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public EObject create(EClass eClass)
  {
    switch (eClass.getClassifierID())
    {
      case RexxPackage.FILE: return createFile();
      case RexxPackage.INSTRUCTION: return createInstruction();
      case RexxPackage.ASSIGNMENT: return createAssignment();
      case RexxPackage.IF: return createIf();
      case RexxPackage.DO: return createDo();
      case RexxPackage.CALL: return createCall();
      case RexxPackage.ARG: return createArg();
      case RexxPackage.USE_ARG: return createUseArg();
      case RexxPackage.USE_ARGS: return createUseArgs();
      case RexxPackage.ARGUMENTS: return createArguments();
      case RexxPackage.RAISE: return createRaise();
      case RexxPackage.EXPRESSION: return createExpression();
      case RexxPackage.ARRAY_ACCESS: return createArrayAccess();
      case RexxPackage.FN_CALL: return createfnCall();
      case RexxPackage.MESSAGE: return createMessage();
      case RexxPackage.NEGATE: return createNegate();
      case RexxPackage.CONST: return createConst();
      case RexxPackage.DIRECTIVES: return createDirectives();
      case RexxPackage.CLASS: return createClass();
      case RexxPackage.CLASS_CONTENT: return createClassContent();
      case RexxPackage.METHOD: return createMethod();
      case RexxPackage.ATTRIBUTE: return createAttribute();
      case RexxPackage.ATTRIBUTE_SET: return createAttributeSet();
      case RexxPackage.ATTRIBUTE_GET: return createAttributeGet();
      case RexxPackage.ATTRIBUTE_NORMAL: return createAttributeNormal();
      case RexxPackage.CONSTANT: return createConstant();
      case RexxPackage.ROUTINE: return createRoutine();
      case RexxPackage.REQUIRES: return createRequires();
      case RexxPackage.OPTIONS: return createOptions();
      case RexxPackage.EXPR_VAR: return createExprVar();
      default:
        throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
    }
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public Object createFromString(EDataType eDataType, String initialValue)
  {
    switch (eDataType.getClassifierID())
    {
      case RexxPackage.VISIBILITY:
        return createVISIBILITYFromString(eDataType, initialValue);
      default:
        throw new IllegalArgumentException("The datatype '" + eDataType.getName() + "' is not a valid classifier");
    }
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public String convertToString(EDataType eDataType, Object instanceValue)
  {
    switch (eDataType.getClassifierID())
    {
      case RexxPackage.VISIBILITY:
        return convertVISIBILITYToString(eDataType, instanceValue);
      default:
        throw new IllegalArgumentException("The datatype '" + eDataType.getName() + "' is not a valid classifier");
    }
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public File createFile()
  {
    FileImpl file = new FileImpl();
    return file;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Instruction createInstruction()
  {
    InstructionImpl instruction = new InstructionImpl();
    return instruction;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Assignment createAssignment()
  {
    AssignmentImpl assignment = new AssignmentImpl();
    return assignment;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public If createIf()
  {
    IfImpl if_ = new IfImpl();
    return if_;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Do createDo()
  {
    DoImpl do_ = new DoImpl();
    return do_;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Call createCall()
  {
    CallImpl call = new CallImpl();
    return call;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Arg createArg()
  {
    ArgImpl arg = new ArgImpl();
    return arg;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public UseArg createUseArg()
  {
    UseArgImpl useArg = new UseArgImpl();
    return useArg;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public UseArgs createUseArgs()
  {
    UseArgsImpl useArgs = new UseArgsImpl();
    return useArgs;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Arguments createArguments()
  {
    ArgumentsImpl arguments = new ArgumentsImpl();
    return arguments;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Raise createRaise()
  {
    RaiseImpl raise = new RaiseImpl();
    return raise;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Expression createExpression()
  {
    ExpressionImpl expression = new ExpressionImpl();
    return expression;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public ArrayAccess createArrayAccess()
  {
    ArrayAccessImpl arrayAccess = new ArrayAccessImpl();
    return arrayAccess;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public fnCall createfnCall()
  {
    fnCallImpl fnCall = new fnCallImpl();
    return fnCall;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Message createMessage()
  {
    MessageImpl message = new MessageImpl();
    return message;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Negate createNegate()
  {
    NegateImpl negate = new NegateImpl();
    return negate;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Const createConst()
  {
    ConstImpl const_ = new ConstImpl();
    return const_;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Directives createDirectives()
  {
    DirectivesImpl directives = new DirectivesImpl();
    return directives;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public org.oorexx.workbench.rexx.Class createClass()
  {
    ClassImpl class_ = new ClassImpl();
    return class_;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public ClassContent createClassContent()
  {
    ClassContentImpl classContent = new ClassContentImpl();
    return classContent;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Method createMethod()
  {
    MethodImpl method = new MethodImpl();
    return method;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Attribute createAttribute()
  {
    AttributeImpl attribute = new AttributeImpl();
    return attribute;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public AttributeSet createAttributeSet()
  {
    AttributeSetImpl attributeSet = new AttributeSetImpl();
    return attributeSet;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public AttributeGet createAttributeGet()
  {
    AttributeGetImpl attributeGet = new AttributeGetImpl();
    return attributeGet;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public AttributeNormal createAttributeNormal()
  {
    AttributeNormalImpl attributeNormal = new AttributeNormalImpl();
    return attributeNormal;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Constant createConstant()
  {
    ConstantImpl constant = new ConstantImpl();
    return constant;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Routine createRoutine()
  {
    RoutineImpl routine = new RoutineImpl();
    return routine;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Requires createRequires()
  {
    RequiresImpl requires = new RequiresImpl();
    return requires;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public Options createOptions()
  {
    OptionsImpl options = new OptionsImpl();
    return options;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public ExprVar createExprVar()
  {
    ExprVarImpl exprVar = new ExprVarImpl();
    return exprVar;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public VISIBILITY createVISIBILITYFromString(EDataType eDataType, String initialValue)
  {
    VISIBILITY result = VISIBILITY.get(initialValue);
    if (result == null) throw new IllegalArgumentException("The value '" + initialValue + "' is not a valid enumerator of '" + eDataType.getName() + "'");
    return result;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public String convertVISIBILITYToString(EDataType eDataType, Object instanceValue)
  {
    return instanceValue == null ? null : instanceValue.toString();
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public RexxPackage getRexxPackage()
  {
    return (RexxPackage)getEPackage();
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @deprecated
   * @generated
   */
  @Deprecated
  public static RexxPackage getPackage()
  {
    return RexxPackage.eINSTANCE;
  }

} //RexxFactoryImpl
