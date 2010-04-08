/**
 * <copyright>
 * </copyright>
 *
 */
package org.oorexx.workbench.rexx;

import org.eclipse.emf.ecore.EAttribute;
import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EEnum;
import org.eclipse.emf.ecore.EPackage;
import org.eclipse.emf.ecore.EReference;

/**
 * <!-- begin-user-doc -->
 * The <b>Package</b> for the model.
 * It contains accessors for the meta objects to represent
 * <ul>
 *   <li>each class,</li>
 *   <li>each feature of each class,</li>
 *   <li>each enum,</li>
 *   <li>and each data type</li>
 * </ul>
 * <!-- end-user-doc -->
 * @see org.oorexx.workbench.rexx.RexxFactory
 * @model kind="package"
 * @generated
 */
public interface RexxPackage extends EPackage
{
  /**
   * The package name.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  String eNAME = "rexx";

  /**
   * The package namespace URI.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  String eNS_URI = "http://www.oorexx.org/workbench/Rexx";

  /**
   * The package namespace name.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  String eNS_PREFIX = "rexx";

  /**
   * The singleton instance of the package.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  RexxPackage eINSTANCE = org.oorexx.workbench.rexx.impl.RexxPackageImpl.init();

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.FileImpl <em>File</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.FileImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getFile()
   * @generated
   */
  int FILE = 0;

  /**
   * The feature id for the '<em><b>Instructions</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int FILE__INSTRUCTIONS = 0;

  /**
   * The feature id for the '<em><b>Requires</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int FILE__REQUIRES = 1;

  /**
   * The feature id for the '<em><b>Methods</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int FILE__METHODS = 2;

  /**
   * The feature id for the '<em><b>Directives</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int FILE__DIRECTIVES = 3;

  /**
   * The number of structural features of the '<em>File</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int FILE_FEATURE_COUNT = 4;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.InstructionImpl <em>Instruction</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.InstructionImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getInstruction()
   * @generated
   */
  int INSTRUCTION = 1;

  /**
   * The number of structural features of the '<em>Instruction</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int INSTRUCTION_FEATURE_COUNT = 0;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.AssignmentImpl <em>Assignment</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.AssignmentImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAssignment()
   * @generated
   */
  int ASSIGNMENT = 2;

  /**
   * The feature id for the '<em><b>Target</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ASSIGNMENT__TARGET = INSTRUCTION_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Expression</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ASSIGNMENT__EXPRESSION = INSTRUCTION_FEATURE_COUNT + 1;

  /**
   * The number of structural features of the '<em>Assignment</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ASSIGNMENT_FEATURE_COUNT = INSTRUCTION_FEATURE_COUNT + 2;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.IfImpl <em>If</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.IfImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getIf()
   * @generated
   */
  int IF = 3;

  /**
   * The feature id for the '<em><b>Expression</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int IF__EXPRESSION = INSTRUCTION_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Top Instr</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int IF__TOP_INSTR = INSTRUCTION_FEATURE_COUNT + 1;

  /**
   * The feature id for the '<em><b>Bottom Instr</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int IF__BOTTOM_INSTR = INSTRUCTION_FEATURE_COUNT + 2;

  /**
   * The number of structural features of the '<em>If</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int IF_FEATURE_COUNT = INSTRUCTION_FEATURE_COUNT + 3;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.DoImpl <em>Do</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.DoImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getDo()
   * @generated
   */
  int DO = 4;

  /**
   * The feature id for the '<em><b>Instrs</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int DO__INSTRS = INSTRUCTION_FEATURE_COUNT + 0;

  /**
   * The number of structural features of the '<em>Do</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int DO_FEATURE_COUNT = INSTRUCTION_FEATURE_COUNT + 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.CallImpl <em>Call</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.CallImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getCall()
   * @generated
   */
  int CALL = 5;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CALL__NAME = INSTRUCTION_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Args</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CALL__ARGS = INSTRUCTION_FEATURE_COUNT + 1;

  /**
   * The number of structural features of the '<em>Call</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CALL_FEATURE_COUNT = INSTRUCTION_FEATURE_COUNT + 2;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ArgImpl <em>Arg</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ArgImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getArg()
   * @generated
   */
  int ARG = 6;

  /**
   * The feature id for the '<em><b>Args</b></em>' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ARG__ARGS = INSTRUCTION_FEATURE_COUNT + 0;

  /**
   * The number of structural features of the '<em>Arg</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ARG_FEATURE_COUNT = INSTRUCTION_FEATURE_COUNT + 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.UseArgImpl <em>Use Arg</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.UseArgImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getUseArg()
   * @generated
   */
  int USE_ARG = 7;

  /**
   * The feature id for the '<em><b>Strict</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int USE_ARG__STRICT = INSTRUCTION_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Args</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int USE_ARG__ARGS = INSTRUCTION_FEATURE_COUNT + 1;

  /**
   * The number of structural features of the '<em>Use Arg</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int USE_ARG_FEATURE_COUNT = INSTRUCTION_FEATURE_COUNT + 2;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.UseArgsImpl <em>Use Args</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.UseArgsImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getUseArgs()
   * @generated
   */
  int USE_ARGS = 8;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int USE_ARGS__NAME = 0;

  /**
   * The feature id for the '<em><b>Default</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int USE_ARGS__DEFAULT = 1;

  /**
   * The number of structural features of the '<em>Use Args</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int USE_ARGS_FEATURE_COUNT = 2;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ArgumentsImpl <em>Arguments</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ArgumentsImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getArguments()
   * @generated
   */
  int ARGUMENTS = 9;

  /**
   * The feature id for the '<em><b>Arguments</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ARGUMENTS__ARGUMENTS = 0;

  /**
   * The number of structural features of the '<em>Arguments</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ARGUMENTS_FEATURE_COUNT = 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.RaiseImpl <em>Raise</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.RaiseImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getRaise()
   * @generated
   */
  int RAISE = 10;

  /**
   * The feature id for the '<em><b>Mayor</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int RAISE__MAYOR = INSTRUCTION_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Minor</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int RAISE__MINOR = INSTRUCTION_FEATURE_COUNT + 1;

  /**
   * The number of structural features of the '<em>Raise</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int RAISE_FEATURE_COUNT = INSTRUCTION_FEATURE_COUNT + 2;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ExpressionImpl <em>Expression</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ExpressionImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getExpression()
   * @generated
   */
  int EXPRESSION = 11;

  /**
   * The number of structural features of the '<em>Expression</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int EXPRESSION_FEATURE_COUNT = 0;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ArrayAccessImpl <em>Array Access</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ArrayAccessImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getArrayAccess()
   * @generated
   */
  int ARRAY_ACCESS = 12;

  /**
   * The feature id for the '<em><b>Target</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ARRAY_ACCESS__TARGET = EXPRESSION_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Index</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ARRAY_ACCESS__INDEX = EXPRESSION_FEATURE_COUNT + 1;

  /**
   * The number of structural features of the '<em>Array Access</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ARRAY_ACCESS_FEATURE_COUNT = EXPRESSION_FEATURE_COUNT + 2;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.fnCallImpl <em>fn Call</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.fnCallImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getfnCall()
   * @generated
   */
  int FN_CALL = 13;

  /**
   * The feature id for the '<em><b>Target</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int FN_CALL__TARGET = EXPRESSION_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Args</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int FN_CALL__ARGS = EXPRESSION_FEATURE_COUNT + 1;

  /**
   * The number of structural features of the '<em>fn Call</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int FN_CALL_FEATURE_COUNT = EXPRESSION_FEATURE_COUNT + 2;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.MessageImpl <em>Message</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.MessageImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getMessage()
   * @generated
   */
  int MESSAGE = 14;

  /**
   * The feature id for the '<em><b>Target</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int MESSAGE__TARGET = INSTRUCTION_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Discard</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int MESSAGE__DISCARD = INSTRUCTION_FEATURE_COUNT + 1;

  /**
   * The feature id for the '<em><b>Message</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int MESSAGE__MESSAGE = INSTRUCTION_FEATURE_COUNT + 2;

  /**
   * The feature id for the '<em><b>Scope</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int MESSAGE__SCOPE = INSTRUCTION_FEATURE_COUNT + 3;

  /**
   * The feature id for the '<em><b>Args</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int MESSAGE__ARGS = INSTRUCTION_FEATURE_COUNT + 4;

  /**
   * The feature id for the '<em><b>Value</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int MESSAGE__VALUE = INSTRUCTION_FEATURE_COUNT + 5;

  /**
   * The number of structural features of the '<em>Message</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int MESSAGE_FEATURE_COUNT = INSTRUCTION_FEATURE_COUNT + 6;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.NegateImpl <em>Negate</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.NegateImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getNegate()
   * @generated
   */
  int NEGATE = 15;

  /**
   * The feature id for the '<em><b>Expression</b></em>' containment reference.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int NEGATE__EXPRESSION = EXPRESSION_FEATURE_COUNT + 0;

  /**
   * The number of structural features of the '<em>Negate</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int NEGATE_FEATURE_COUNT = EXPRESSION_FEATURE_COUNT + 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ConstImpl <em>Const</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ConstImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getConst()
   * @generated
   */
  int CONST = 16;

  /**
   * The feature id for the '<em><b>Value</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CONST__VALUE = EXPRESSION_FEATURE_COUNT + 0;

  /**
   * The number of structural features of the '<em>Const</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CONST_FEATURE_COUNT = EXPRESSION_FEATURE_COUNT + 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.DirectivesImpl <em>Directives</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.DirectivesImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getDirectives()
   * @generated
   */
  int DIRECTIVES = 17;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int DIRECTIVES__NAME = 0;

  /**
   * The number of structural features of the '<em>Directives</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int DIRECTIVES_FEATURE_COUNT = 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ClassImpl <em>Class</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ClassImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getClass_()
   * @generated
   */
  int CLASS = 18;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS__NAME = DIRECTIVES__NAME;

  /**
   * The feature id for the '<em><b>Meta</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS__META = DIRECTIVES_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Subclass</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS__SUBCLASS = DIRECTIVES_FEATURE_COUNT + 1;

  /**
   * The feature id for the '<em><b>Mixin</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS__MIXIN = DIRECTIVES_FEATURE_COUNT + 2;

  /**
   * The feature id for the '<em><b>Visibility</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS__VISIBILITY = DIRECTIVES_FEATURE_COUNT + 3;

  /**
   * The feature id for the '<em><b>Inherit</b></em>' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS__INHERIT = DIRECTIVES_FEATURE_COUNT + 4;

  /**
   * The feature id for the '<em><b>Content</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS__CONTENT = DIRECTIVES_FEATURE_COUNT + 5;

  /**
   * The number of structural features of the '<em>Class</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS_FEATURE_COUNT = DIRECTIVES_FEATURE_COUNT + 6;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ClassContentImpl <em>Class Content</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ClassContentImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getClassContent()
   * @generated
   */
  int CLASS_CONTENT = 19;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS_CONTENT__NAME = 0;

  /**
   * The number of structural features of the '<em>Class Content</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CLASS_CONTENT_FEATURE_COUNT = 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.MethodImpl <em>Method</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.MethodImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getMethod()
   * @generated
   */
  int METHOD = 20;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int METHOD__NAME = CLASS_CONTENT__NAME;

  /**
   * The feature id for the '<em><b>Visibility</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int METHOD__VISIBILITY = CLASS_CONTENT_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Class</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int METHOD__CLASS = CLASS_CONTENT_FEATURE_COUNT + 1;

  /**
   * The feature id for the '<em><b>Expose</b></em>' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int METHOD__EXPOSE = CLASS_CONTENT_FEATURE_COUNT + 2;

  /**
   * The feature id for the '<em><b>Content</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int METHOD__CONTENT = CLASS_CONTENT_FEATURE_COUNT + 3;

  /**
   * The number of structural features of the '<em>Method</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int METHOD_FEATURE_COUNT = CLASS_CONTENT_FEATURE_COUNT + 4;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.AttributeImpl <em>Attribute</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.AttributeImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAttribute()
   * @generated
   */
  int ATTRIBUTE = 21;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE__NAME = CLASS_CONTENT__NAME;

  /**
   * The feature id for the '<em><b>Visibility</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE__VISIBILITY = CLASS_CONTENT_FEATURE_COUNT + 0;

  /**
   * The feature id for the '<em><b>Class</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE__CLASS = CLASS_CONTENT_FEATURE_COUNT + 1;

  /**
   * The feature id for the '<em><b>Expose</b></em>' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE__EXPOSE = CLASS_CONTENT_FEATURE_COUNT + 2;

  /**
   * The feature id for the '<em><b>Content</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE__CONTENT = CLASS_CONTENT_FEATURE_COUNT + 3;

  /**
   * The number of structural features of the '<em>Attribute</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_FEATURE_COUNT = CLASS_CONTENT_FEATURE_COUNT + 4;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.AttributeSetImpl <em>Attribute Set</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.AttributeSetImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAttributeSet()
   * @generated
   */
  int ATTRIBUTE_SET = 22;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_SET__NAME = ATTRIBUTE__NAME;

  /**
   * The feature id for the '<em><b>Visibility</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_SET__VISIBILITY = ATTRIBUTE__VISIBILITY;

  /**
   * The feature id for the '<em><b>Class</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_SET__CLASS = ATTRIBUTE__CLASS;

  /**
   * The feature id for the '<em><b>Expose</b></em>' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_SET__EXPOSE = ATTRIBUTE__EXPOSE;

  /**
   * The feature id for the '<em><b>Content</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_SET__CONTENT = ATTRIBUTE__CONTENT;

  /**
   * The number of structural features of the '<em>Attribute Set</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_SET_FEATURE_COUNT = ATTRIBUTE_FEATURE_COUNT + 0;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.AttributeGetImpl <em>Attribute Get</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.AttributeGetImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAttributeGet()
   * @generated
   */
  int ATTRIBUTE_GET = 23;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_GET__NAME = ATTRIBUTE__NAME;

  /**
   * The feature id for the '<em><b>Visibility</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_GET__VISIBILITY = ATTRIBUTE__VISIBILITY;

  /**
   * The feature id for the '<em><b>Class</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_GET__CLASS = ATTRIBUTE__CLASS;

  /**
   * The feature id for the '<em><b>Expose</b></em>' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_GET__EXPOSE = ATTRIBUTE__EXPOSE;

  /**
   * The feature id for the '<em><b>Content</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_GET__CONTENT = ATTRIBUTE__CONTENT;

  /**
   * The number of structural features of the '<em>Attribute Get</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_GET_FEATURE_COUNT = ATTRIBUTE_FEATURE_COUNT + 0;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.AttributeNormalImpl <em>Attribute Normal</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.AttributeNormalImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAttributeNormal()
   * @generated
   */
  int ATTRIBUTE_NORMAL = 24;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_NORMAL__NAME = ATTRIBUTE__NAME;

  /**
   * The feature id for the '<em><b>Visibility</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_NORMAL__VISIBILITY = ATTRIBUTE__VISIBILITY;

  /**
   * The feature id for the '<em><b>Class</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_NORMAL__CLASS = ATTRIBUTE__CLASS;

  /**
   * The feature id for the '<em><b>Expose</b></em>' attribute list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_NORMAL__EXPOSE = ATTRIBUTE__EXPOSE;

  /**
   * The feature id for the '<em><b>Content</b></em>' containment reference list.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_NORMAL__CONTENT = ATTRIBUTE__CONTENT;

  /**
   * The number of structural features of the '<em>Attribute Normal</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ATTRIBUTE_NORMAL_FEATURE_COUNT = ATTRIBUTE_FEATURE_COUNT + 0;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ConstantImpl <em>Constant</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ConstantImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getConstant()
   * @generated
   */
  int CONSTANT = 25;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CONSTANT__NAME = CLASS_CONTENT__NAME;

  /**
   * The number of structural features of the '<em>Constant</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int CONSTANT_FEATURE_COUNT = CLASS_CONTENT_FEATURE_COUNT + 0;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.RoutineImpl <em>Routine</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.RoutineImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getRoutine()
   * @generated
   */
  int ROUTINE = 26;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ROUTINE__NAME = DIRECTIVES__NAME;

  /**
   * The number of structural features of the '<em>Routine</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int ROUTINE_FEATURE_COUNT = DIRECTIVES_FEATURE_COUNT + 0;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.RequiresImpl <em>Requires</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.RequiresImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getRequires()
   * @generated
   */
  int REQUIRES = 27;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int REQUIRES__NAME = 0;

  /**
   * The number of structural features of the '<em>Requires</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int REQUIRES_FEATURE_COUNT = 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.OptionsImpl <em>Options</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.OptionsImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getOptions()
   * @generated
   */
  int OPTIONS = 28;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int OPTIONS__NAME = DIRECTIVES__NAME;

  /**
   * The number of structural features of the '<em>Options</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int OPTIONS_FEATURE_COUNT = DIRECTIVES_FEATURE_COUNT + 0;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.impl.ExprVarImpl <em>Expr Var</em>}' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.impl.ExprVarImpl
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getExprVar()
   * @generated
   */
  int EXPR_VAR = 29;

  /**
   * The feature id for the '<em><b>Name</b></em>' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int EXPR_VAR__NAME = EXPRESSION_FEATURE_COUNT + 0;

  /**
   * The number of structural features of the '<em>Expr Var</em>' class.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   * @ordered
   */
  int EXPR_VAR_FEATURE_COUNT = EXPRESSION_FEATURE_COUNT + 1;

  /**
   * The meta object id for the '{@link org.oorexx.workbench.rexx.VISIBILITY <em>VISIBILITY</em>}' enum.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see org.oorexx.workbench.rexx.VISIBILITY
   * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getVISIBILITY()
   * @generated
   */
  int VISIBILITY = 30;


  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.File <em>File</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>File</em>'.
   * @see org.oorexx.workbench.rexx.File
   * @generated
   */
  EClass getFile();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.File#getInstructions <em>Instructions</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Instructions</em>'.
   * @see org.oorexx.workbench.rexx.File#getInstructions()
   * @see #getFile()
   * @generated
   */
  EReference getFile_Instructions();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.File#getRequires <em>Requires</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Requires</em>'.
   * @see org.oorexx.workbench.rexx.File#getRequires()
   * @see #getFile()
   * @generated
   */
  EReference getFile_Requires();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.File#getMethods <em>Methods</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Methods</em>'.
   * @see org.oorexx.workbench.rexx.File#getMethods()
   * @see #getFile()
   * @generated
   */
  EReference getFile_Methods();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.File#getDirectives <em>Directives</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Directives</em>'.
   * @see org.oorexx.workbench.rexx.File#getDirectives()
   * @see #getFile()
   * @generated
   */
  EReference getFile_Directives();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Instruction <em>Instruction</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Instruction</em>'.
   * @see org.oorexx.workbench.rexx.Instruction
   * @generated
   */
  EClass getInstruction();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Assignment <em>Assignment</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Assignment</em>'.
   * @see org.oorexx.workbench.rexx.Assignment
   * @generated
   */
  EClass getAssignment();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Assignment#getTarget <em>Target</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Target</em>'.
   * @see org.oorexx.workbench.rexx.Assignment#getTarget()
   * @see #getAssignment()
   * @generated
   */
  EAttribute getAssignment_Target();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.Assignment#getExpression <em>Expression</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Expression</em>'.
   * @see org.oorexx.workbench.rexx.Assignment#getExpression()
   * @see #getAssignment()
   * @generated
   */
  EReference getAssignment_Expression();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.If <em>If</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>If</em>'.
   * @see org.oorexx.workbench.rexx.If
   * @generated
   */
  EClass getIf();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.If#getExpression <em>Expression</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Expression</em>'.
   * @see org.oorexx.workbench.rexx.If#getExpression()
   * @see #getIf()
   * @generated
   */
  EReference getIf_Expression();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.If#getTopInstr <em>Top Instr</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Top Instr</em>'.
   * @see org.oorexx.workbench.rexx.If#getTopInstr()
   * @see #getIf()
   * @generated
   */
  EReference getIf_TopInstr();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.If#getBottomInstr <em>Bottom Instr</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Bottom Instr</em>'.
   * @see org.oorexx.workbench.rexx.If#getBottomInstr()
   * @see #getIf()
   * @generated
   */
  EReference getIf_BottomInstr();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Do <em>Do</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Do</em>'.
   * @see org.oorexx.workbench.rexx.Do
   * @generated
   */
  EClass getDo();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.Do#getInstrs <em>Instrs</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Instrs</em>'.
   * @see org.oorexx.workbench.rexx.Do#getInstrs()
   * @see #getDo()
   * @generated
   */
  EReference getDo_Instrs();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Call <em>Call</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Call</em>'.
   * @see org.oorexx.workbench.rexx.Call
   * @generated
   */
  EClass getCall();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Call#getName <em>Name</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Name</em>'.
   * @see org.oorexx.workbench.rexx.Call#getName()
   * @see #getCall()
   * @generated
   */
  EAttribute getCall_Name();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.Call#getArgs <em>Args</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Args</em>'.
   * @see org.oorexx.workbench.rexx.Call#getArgs()
   * @see #getCall()
   * @generated
   */
  EReference getCall_Args();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Arg <em>Arg</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Arg</em>'.
   * @see org.oorexx.workbench.rexx.Arg
   * @generated
   */
  EClass getArg();

  /**
   * Returns the meta object for the attribute list '{@link org.oorexx.workbench.rexx.Arg#getArgs <em>Args</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute list '<em>Args</em>'.
   * @see org.oorexx.workbench.rexx.Arg#getArgs()
   * @see #getArg()
   * @generated
   */
  EAttribute getArg_Args();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.UseArg <em>Use Arg</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Use Arg</em>'.
   * @see org.oorexx.workbench.rexx.UseArg
   * @generated
   */
  EClass getUseArg();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.UseArg#isStrict <em>Strict</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Strict</em>'.
   * @see org.oorexx.workbench.rexx.UseArg#isStrict()
   * @see #getUseArg()
   * @generated
   */
  EAttribute getUseArg_Strict();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.UseArg#getArgs <em>Args</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Args</em>'.
   * @see org.oorexx.workbench.rexx.UseArg#getArgs()
   * @see #getUseArg()
   * @generated
   */
  EReference getUseArg_Args();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.UseArgs <em>Use Args</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Use Args</em>'.
   * @see org.oorexx.workbench.rexx.UseArgs
   * @generated
   */
  EClass getUseArgs();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.UseArgs#getName <em>Name</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Name</em>'.
   * @see org.oorexx.workbench.rexx.UseArgs#getName()
   * @see #getUseArgs()
   * @generated
   */
  EAttribute getUseArgs_Name();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.UseArgs#getDefault <em>Default</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Default</em>'.
   * @see org.oorexx.workbench.rexx.UseArgs#getDefault()
   * @see #getUseArgs()
   * @generated
   */
  EReference getUseArgs_Default();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Arguments <em>Arguments</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Arguments</em>'.
   * @see org.oorexx.workbench.rexx.Arguments
   * @generated
   */
  EClass getArguments();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.Arguments#getArguments <em>Arguments</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Arguments</em>'.
   * @see org.oorexx.workbench.rexx.Arguments#getArguments()
   * @see #getArguments()
   * @generated
   */
  EReference getArguments_Arguments();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Raise <em>Raise</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Raise</em>'.
   * @see org.oorexx.workbench.rexx.Raise
   * @generated
   */
  EClass getRaise();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Raise#getMayor <em>Mayor</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Mayor</em>'.
   * @see org.oorexx.workbench.rexx.Raise#getMayor()
   * @see #getRaise()
   * @generated
   */
  EAttribute getRaise_Mayor();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Raise#getMinor <em>Minor</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Minor</em>'.
   * @see org.oorexx.workbench.rexx.Raise#getMinor()
   * @see #getRaise()
   * @generated
   */
  EAttribute getRaise_Minor();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Expression <em>Expression</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Expression</em>'.
   * @see org.oorexx.workbench.rexx.Expression
   * @generated
   */
  EClass getExpression();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.ArrayAccess <em>Array Access</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Array Access</em>'.
   * @see org.oorexx.workbench.rexx.ArrayAccess
   * @generated
   */
  EClass getArrayAccess();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.ArrayAccess#getTarget <em>Target</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Target</em>'.
   * @see org.oorexx.workbench.rexx.ArrayAccess#getTarget()
   * @see #getArrayAccess()
   * @generated
   */
  EAttribute getArrayAccess_Target();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.ArrayAccess#getIndex <em>Index</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Index</em>'.
   * @see org.oorexx.workbench.rexx.ArrayAccess#getIndex()
   * @see #getArrayAccess()
   * @generated
   */
  EReference getArrayAccess_Index();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.fnCall <em>fn Call</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>fn Call</em>'.
   * @see org.oorexx.workbench.rexx.fnCall
   * @generated
   */
  EClass getfnCall();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.fnCall#getTarget <em>Target</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Target</em>'.
   * @see org.oorexx.workbench.rexx.fnCall#getTarget()
   * @see #getfnCall()
   * @generated
   */
  EAttribute getfnCall_Target();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.fnCall#getArgs <em>Args</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Args</em>'.
   * @see org.oorexx.workbench.rexx.fnCall#getArgs()
   * @see #getfnCall()
   * @generated
   */
  EReference getfnCall_Args();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Message <em>Message</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Message</em>'.
   * @see org.oorexx.workbench.rexx.Message
   * @generated
   */
  EClass getMessage();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Message#getTarget <em>Target</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Target</em>'.
   * @see org.oorexx.workbench.rexx.Message#getTarget()
   * @see #getMessage()
   * @generated
   */
  EAttribute getMessage_Target();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Message#isDiscard <em>Discard</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Discard</em>'.
   * @see org.oorexx.workbench.rexx.Message#isDiscard()
   * @see #getMessage()
   * @generated
   */
  EAttribute getMessage_Discard();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Message#getMessage <em>Message</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Message</em>'.
   * @see org.oorexx.workbench.rexx.Message#getMessage()
   * @see #getMessage()
   * @generated
   */
  EAttribute getMessage_Message();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Message#getScope <em>Scope</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Scope</em>'.
   * @see org.oorexx.workbench.rexx.Message#getScope()
   * @see #getMessage()
   * @generated
   */
  EAttribute getMessage_Scope();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.Message#getArgs <em>Args</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Args</em>'.
   * @see org.oorexx.workbench.rexx.Message#getArgs()
   * @see #getMessage()
   * @generated
   */
  EReference getMessage_Args();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.Message#getValue <em>Value</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Value</em>'.
   * @see org.oorexx.workbench.rexx.Message#getValue()
   * @see #getMessage()
   * @generated
   */
  EReference getMessage_Value();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Negate <em>Negate</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Negate</em>'.
   * @see org.oorexx.workbench.rexx.Negate
   * @generated
   */
  EClass getNegate();

  /**
   * Returns the meta object for the containment reference '{@link org.oorexx.workbench.rexx.Negate#getExpression <em>Expression</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference '<em>Expression</em>'.
   * @see org.oorexx.workbench.rexx.Negate#getExpression()
   * @see #getNegate()
   * @generated
   */
  EReference getNegate_Expression();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Const <em>Const</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Const</em>'.
   * @see org.oorexx.workbench.rexx.Const
   * @generated
   */
  EClass getConst();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Const#getValue <em>Value</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Value</em>'.
   * @see org.oorexx.workbench.rexx.Const#getValue()
   * @see #getConst()
   * @generated
   */
  EAttribute getConst_Value();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Directives <em>Directives</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Directives</em>'.
   * @see org.oorexx.workbench.rexx.Directives
   * @generated
   */
  EClass getDirectives();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Directives#getName <em>Name</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Name</em>'.
   * @see org.oorexx.workbench.rexx.Directives#getName()
   * @see #getDirectives()
   * @generated
   */
  EAttribute getDirectives_Name();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Class <em>Class</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Class</em>'.
   * @see org.oorexx.workbench.rexx.Class
   * @generated
   */
  EClass getClass_();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Class#getMeta <em>Meta</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Meta</em>'.
   * @see org.oorexx.workbench.rexx.Class#getMeta()
   * @see #getClass_()
   * @generated
   */
  EAttribute getClass_Meta();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Class#getSubclass <em>Subclass</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Subclass</em>'.
   * @see org.oorexx.workbench.rexx.Class#getSubclass()
   * @see #getClass_()
   * @generated
   */
  EAttribute getClass_Subclass();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Class#getMixin <em>Mixin</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Mixin</em>'.
   * @see org.oorexx.workbench.rexx.Class#getMixin()
   * @see #getClass_()
   * @generated
   */
  EAttribute getClass_Mixin();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Class#getVisibility <em>Visibility</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Visibility</em>'.
   * @see org.oorexx.workbench.rexx.Class#getVisibility()
   * @see #getClass_()
   * @generated
   */
  EAttribute getClass_Visibility();

  /**
   * Returns the meta object for the attribute list '{@link org.oorexx.workbench.rexx.Class#getInherit <em>Inherit</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute list '<em>Inherit</em>'.
   * @see org.oorexx.workbench.rexx.Class#getInherit()
   * @see #getClass_()
   * @generated
   */
  EAttribute getClass_Inherit();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.Class#getContent <em>Content</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Content</em>'.
   * @see org.oorexx.workbench.rexx.Class#getContent()
   * @see #getClass_()
   * @generated
   */
  EReference getClass_Content();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.ClassContent <em>Class Content</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Class Content</em>'.
   * @see org.oorexx.workbench.rexx.ClassContent
   * @generated
   */
  EClass getClassContent();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.ClassContent#getName <em>Name</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Name</em>'.
   * @see org.oorexx.workbench.rexx.ClassContent#getName()
   * @see #getClassContent()
   * @generated
   */
  EAttribute getClassContent_Name();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Method <em>Method</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Method</em>'.
   * @see org.oorexx.workbench.rexx.Method
   * @generated
   */
  EClass getMethod();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Method#getVisibility <em>Visibility</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Visibility</em>'.
   * @see org.oorexx.workbench.rexx.Method#getVisibility()
   * @see #getMethod()
   * @generated
   */
  EAttribute getMethod_Visibility();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Method#isClass <em>Class</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Class</em>'.
   * @see org.oorexx.workbench.rexx.Method#isClass()
   * @see #getMethod()
   * @generated
   */
  EAttribute getMethod_Class();

  /**
   * Returns the meta object for the attribute list '{@link org.oorexx.workbench.rexx.Method#getExpose <em>Expose</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute list '<em>Expose</em>'.
   * @see org.oorexx.workbench.rexx.Method#getExpose()
   * @see #getMethod()
   * @generated
   */
  EAttribute getMethod_Expose();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.Method#getContent <em>Content</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Content</em>'.
   * @see org.oorexx.workbench.rexx.Method#getContent()
   * @see #getMethod()
   * @generated
   */
  EReference getMethod_Content();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Attribute <em>Attribute</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Attribute</em>'.
   * @see org.oorexx.workbench.rexx.Attribute
   * @generated
   */
  EClass getAttribute();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Attribute#getVisibility <em>Visibility</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Visibility</em>'.
   * @see org.oorexx.workbench.rexx.Attribute#getVisibility()
   * @see #getAttribute()
   * @generated
   */
  EAttribute getAttribute_Visibility();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Attribute#isClass <em>Class</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Class</em>'.
   * @see org.oorexx.workbench.rexx.Attribute#isClass()
   * @see #getAttribute()
   * @generated
   */
  EAttribute getAttribute_Class();

  /**
   * Returns the meta object for the attribute list '{@link org.oorexx.workbench.rexx.Attribute#getExpose <em>Expose</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute list '<em>Expose</em>'.
   * @see org.oorexx.workbench.rexx.Attribute#getExpose()
   * @see #getAttribute()
   * @generated
   */
  EAttribute getAttribute_Expose();

  /**
   * Returns the meta object for the containment reference list '{@link org.oorexx.workbench.rexx.Attribute#getContent <em>Content</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the containment reference list '<em>Content</em>'.
   * @see org.oorexx.workbench.rexx.Attribute#getContent()
   * @see #getAttribute()
   * @generated
   */
  EReference getAttribute_Content();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.AttributeSet <em>Attribute Set</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Attribute Set</em>'.
   * @see org.oorexx.workbench.rexx.AttributeSet
   * @generated
   */
  EClass getAttributeSet();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.AttributeGet <em>Attribute Get</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Attribute Get</em>'.
   * @see org.oorexx.workbench.rexx.AttributeGet
   * @generated
   */
  EClass getAttributeGet();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.AttributeNormal <em>Attribute Normal</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Attribute Normal</em>'.
   * @see org.oorexx.workbench.rexx.AttributeNormal
   * @generated
   */
  EClass getAttributeNormal();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Constant <em>Constant</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Constant</em>'.
   * @see org.oorexx.workbench.rexx.Constant
   * @generated
   */
  EClass getConstant();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Routine <em>Routine</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Routine</em>'.
   * @see org.oorexx.workbench.rexx.Routine
   * @generated
   */
  EClass getRoutine();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Requires <em>Requires</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Requires</em>'.
   * @see org.oorexx.workbench.rexx.Requires
   * @generated
   */
  EClass getRequires();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.Requires#getName <em>Name</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Name</em>'.
   * @see org.oorexx.workbench.rexx.Requires#getName()
   * @see #getRequires()
   * @generated
   */
  EAttribute getRequires_Name();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.Options <em>Options</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Options</em>'.
   * @see org.oorexx.workbench.rexx.Options
   * @generated
   */
  EClass getOptions();

  /**
   * Returns the meta object for class '{@link org.oorexx.workbench.rexx.ExprVar <em>Expr Var</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for class '<em>Expr Var</em>'.
   * @see org.oorexx.workbench.rexx.ExprVar
   * @generated
   */
  EClass getExprVar();

  /**
   * Returns the meta object for the attribute '{@link org.oorexx.workbench.rexx.ExprVar#getName <em>Name</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for the attribute '<em>Name</em>'.
   * @see org.oorexx.workbench.rexx.ExprVar#getName()
   * @see #getExprVar()
   * @generated
   */
  EAttribute getExprVar_Name();

  /**
   * Returns the meta object for enum '{@link org.oorexx.workbench.rexx.VISIBILITY <em>VISIBILITY</em>}'.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the meta object for enum '<em>VISIBILITY</em>'.
   * @see org.oorexx.workbench.rexx.VISIBILITY
   * @generated
   */
  EEnum getVISIBILITY();

  /**
   * Returns the factory that creates the instances of the model.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @return the factory that creates the instances of the model.
   * @generated
   */
  RexxFactory getRexxFactory();

  /**
   * <!-- begin-user-doc -->
   * Defines literals for the meta objects that represent
   * <ul>
   *   <li>each class,</li>
   *   <li>each feature of each class,</li>
   *   <li>each enum,</li>
   *   <li>and each data type</li>
   * </ul>
   * <!-- end-user-doc -->
   * @generated
   */
  interface Literals
  {
    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.FileImpl <em>File</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.FileImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getFile()
     * @generated
     */
    EClass FILE = eINSTANCE.getFile();

    /**
     * The meta object literal for the '<em><b>Instructions</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference FILE__INSTRUCTIONS = eINSTANCE.getFile_Instructions();

    /**
     * The meta object literal for the '<em><b>Requires</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference FILE__REQUIRES = eINSTANCE.getFile_Requires();

    /**
     * The meta object literal for the '<em><b>Methods</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference FILE__METHODS = eINSTANCE.getFile_Methods();

    /**
     * The meta object literal for the '<em><b>Directives</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference FILE__DIRECTIVES = eINSTANCE.getFile_Directives();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.InstructionImpl <em>Instruction</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.InstructionImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getInstruction()
     * @generated
     */
    EClass INSTRUCTION = eINSTANCE.getInstruction();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.AssignmentImpl <em>Assignment</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.AssignmentImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAssignment()
     * @generated
     */
    EClass ASSIGNMENT = eINSTANCE.getAssignment();

    /**
     * The meta object literal for the '<em><b>Target</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute ASSIGNMENT__TARGET = eINSTANCE.getAssignment_Target();

    /**
     * The meta object literal for the '<em><b>Expression</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference ASSIGNMENT__EXPRESSION = eINSTANCE.getAssignment_Expression();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.IfImpl <em>If</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.IfImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getIf()
     * @generated
     */
    EClass IF = eINSTANCE.getIf();

    /**
     * The meta object literal for the '<em><b>Expression</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference IF__EXPRESSION = eINSTANCE.getIf_Expression();

    /**
     * The meta object literal for the '<em><b>Top Instr</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference IF__TOP_INSTR = eINSTANCE.getIf_TopInstr();

    /**
     * The meta object literal for the '<em><b>Bottom Instr</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference IF__BOTTOM_INSTR = eINSTANCE.getIf_BottomInstr();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.DoImpl <em>Do</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.DoImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getDo()
     * @generated
     */
    EClass DO = eINSTANCE.getDo();

    /**
     * The meta object literal for the '<em><b>Instrs</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference DO__INSTRS = eINSTANCE.getDo_Instrs();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.CallImpl <em>Call</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.CallImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getCall()
     * @generated
     */
    EClass CALL = eINSTANCE.getCall();

    /**
     * The meta object literal for the '<em><b>Name</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute CALL__NAME = eINSTANCE.getCall_Name();

    /**
     * The meta object literal for the '<em><b>Args</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference CALL__ARGS = eINSTANCE.getCall_Args();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ArgImpl <em>Arg</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ArgImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getArg()
     * @generated
     */
    EClass ARG = eINSTANCE.getArg();

    /**
     * The meta object literal for the '<em><b>Args</b></em>' attribute list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute ARG__ARGS = eINSTANCE.getArg_Args();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.UseArgImpl <em>Use Arg</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.UseArgImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getUseArg()
     * @generated
     */
    EClass USE_ARG = eINSTANCE.getUseArg();

    /**
     * The meta object literal for the '<em><b>Strict</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute USE_ARG__STRICT = eINSTANCE.getUseArg_Strict();

    /**
     * The meta object literal for the '<em><b>Args</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference USE_ARG__ARGS = eINSTANCE.getUseArg_Args();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.UseArgsImpl <em>Use Args</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.UseArgsImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getUseArgs()
     * @generated
     */
    EClass USE_ARGS = eINSTANCE.getUseArgs();

    /**
     * The meta object literal for the '<em><b>Name</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute USE_ARGS__NAME = eINSTANCE.getUseArgs_Name();

    /**
     * The meta object literal for the '<em><b>Default</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference USE_ARGS__DEFAULT = eINSTANCE.getUseArgs_Default();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ArgumentsImpl <em>Arguments</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ArgumentsImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getArguments()
     * @generated
     */
    EClass ARGUMENTS = eINSTANCE.getArguments();

    /**
     * The meta object literal for the '<em><b>Arguments</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference ARGUMENTS__ARGUMENTS = eINSTANCE.getArguments_Arguments();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.RaiseImpl <em>Raise</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.RaiseImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getRaise()
     * @generated
     */
    EClass RAISE = eINSTANCE.getRaise();

    /**
     * The meta object literal for the '<em><b>Mayor</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute RAISE__MAYOR = eINSTANCE.getRaise_Mayor();

    /**
     * The meta object literal for the '<em><b>Minor</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute RAISE__MINOR = eINSTANCE.getRaise_Minor();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ExpressionImpl <em>Expression</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ExpressionImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getExpression()
     * @generated
     */
    EClass EXPRESSION = eINSTANCE.getExpression();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ArrayAccessImpl <em>Array Access</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ArrayAccessImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getArrayAccess()
     * @generated
     */
    EClass ARRAY_ACCESS = eINSTANCE.getArrayAccess();

    /**
     * The meta object literal for the '<em><b>Target</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute ARRAY_ACCESS__TARGET = eINSTANCE.getArrayAccess_Target();

    /**
     * The meta object literal for the '<em><b>Index</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference ARRAY_ACCESS__INDEX = eINSTANCE.getArrayAccess_Index();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.fnCallImpl <em>fn Call</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.fnCallImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getfnCall()
     * @generated
     */
    EClass FN_CALL = eINSTANCE.getfnCall();

    /**
     * The meta object literal for the '<em><b>Target</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute FN_CALL__TARGET = eINSTANCE.getfnCall_Target();

    /**
     * The meta object literal for the '<em><b>Args</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference FN_CALL__ARGS = eINSTANCE.getfnCall_Args();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.MessageImpl <em>Message</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.MessageImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getMessage()
     * @generated
     */
    EClass MESSAGE = eINSTANCE.getMessage();

    /**
     * The meta object literal for the '<em><b>Target</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute MESSAGE__TARGET = eINSTANCE.getMessage_Target();

    /**
     * The meta object literal for the '<em><b>Discard</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute MESSAGE__DISCARD = eINSTANCE.getMessage_Discard();

    /**
     * The meta object literal for the '<em><b>Message</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute MESSAGE__MESSAGE = eINSTANCE.getMessage_Message();

    /**
     * The meta object literal for the '<em><b>Scope</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute MESSAGE__SCOPE = eINSTANCE.getMessage_Scope();

    /**
     * The meta object literal for the '<em><b>Args</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference MESSAGE__ARGS = eINSTANCE.getMessage_Args();

    /**
     * The meta object literal for the '<em><b>Value</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference MESSAGE__VALUE = eINSTANCE.getMessage_Value();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.NegateImpl <em>Negate</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.NegateImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getNegate()
     * @generated
     */
    EClass NEGATE = eINSTANCE.getNegate();

    /**
     * The meta object literal for the '<em><b>Expression</b></em>' containment reference feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference NEGATE__EXPRESSION = eINSTANCE.getNegate_Expression();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ConstImpl <em>Const</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ConstImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getConst()
     * @generated
     */
    EClass CONST = eINSTANCE.getConst();

    /**
     * The meta object literal for the '<em><b>Value</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute CONST__VALUE = eINSTANCE.getConst_Value();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.DirectivesImpl <em>Directives</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.DirectivesImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getDirectives()
     * @generated
     */
    EClass DIRECTIVES = eINSTANCE.getDirectives();

    /**
     * The meta object literal for the '<em><b>Name</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute DIRECTIVES__NAME = eINSTANCE.getDirectives_Name();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ClassImpl <em>Class</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ClassImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getClass_()
     * @generated
     */
    EClass CLASS = eINSTANCE.getClass_();

    /**
     * The meta object literal for the '<em><b>Meta</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute CLASS__META = eINSTANCE.getClass_Meta();

    /**
     * The meta object literal for the '<em><b>Subclass</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute CLASS__SUBCLASS = eINSTANCE.getClass_Subclass();

    /**
     * The meta object literal for the '<em><b>Mixin</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute CLASS__MIXIN = eINSTANCE.getClass_Mixin();

    /**
     * The meta object literal for the '<em><b>Visibility</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute CLASS__VISIBILITY = eINSTANCE.getClass_Visibility();

    /**
     * The meta object literal for the '<em><b>Inherit</b></em>' attribute list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute CLASS__INHERIT = eINSTANCE.getClass_Inherit();

    /**
     * The meta object literal for the '<em><b>Content</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference CLASS__CONTENT = eINSTANCE.getClass_Content();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ClassContentImpl <em>Class Content</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ClassContentImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getClassContent()
     * @generated
     */
    EClass CLASS_CONTENT = eINSTANCE.getClassContent();

    /**
     * The meta object literal for the '<em><b>Name</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute CLASS_CONTENT__NAME = eINSTANCE.getClassContent_Name();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.MethodImpl <em>Method</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.MethodImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getMethod()
     * @generated
     */
    EClass METHOD = eINSTANCE.getMethod();

    /**
     * The meta object literal for the '<em><b>Visibility</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute METHOD__VISIBILITY = eINSTANCE.getMethod_Visibility();

    /**
     * The meta object literal for the '<em><b>Class</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute METHOD__CLASS = eINSTANCE.getMethod_Class();

    /**
     * The meta object literal for the '<em><b>Expose</b></em>' attribute list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute METHOD__EXPOSE = eINSTANCE.getMethod_Expose();

    /**
     * The meta object literal for the '<em><b>Content</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference METHOD__CONTENT = eINSTANCE.getMethod_Content();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.AttributeImpl <em>Attribute</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.AttributeImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAttribute()
     * @generated
     */
    EClass ATTRIBUTE = eINSTANCE.getAttribute();

    /**
     * The meta object literal for the '<em><b>Visibility</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute ATTRIBUTE__VISIBILITY = eINSTANCE.getAttribute_Visibility();

    /**
     * The meta object literal for the '<em><b>Class</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute ATTRIBUTE__CLASS = eINSTANCE.getAttribute_Class();

    /**
     * The meta object literal for the '<em><b>Expose</b></em>' attribute list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute ATTRIBUTE__EXPOSE = eINSTANCE.getAttribute_Expose();

    /**
     * The meta object literal for the '<em><b>Content</b></em>' containment reference list feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EReference ATTRIBUTE__CONTENT = eINSTANCE.getAttribute_Content();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.AttributeSetImpl <em>Attribute Set</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.AttributeSetImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAttributeSet()
     * @generated
     */
    EClass ATTRIBUTE_SET = eINSTANCE.getAttributeSet();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.AttributeGetImpl <em>Attribute Get</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.AttributeGetImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAttributeGet()
     * @generated
     */
    EClass ATTRIBUTE_GET = eINSTANCE.getAttributeGet();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.AttributeNormalImpl <em>Attribute Normal</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.AttributeNormalImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getAttributeNormal()
     * @generated
     */
    EClass ATTRIBUTE_NORMAL = eINSTANCE.getAttributeNormal();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ConstantImpl <em>Constant</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ConstantImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getConstant()
     * @generated
     */
    EClass CONSTANT = eINSTANCE.getConstant();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.RoutineImpl <em>Routine</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.RoutineImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getRoutine()
     * @generated
     */
    EClass ROUTINE = eINSTANCE.getRoutine();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.RequiresImpl <em>Requires</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.RequiresImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getRequires()
     * @generated
     */
    EClass REQUIRES = eINSTANCE.getRequires();

    /**
     * The meta object literal for the '<em><b>Name</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute REQUIRES__NAME = eINSTANCE.getRequires_Name();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.OptionsImpl <em>Options</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.OptionsImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getOptions()
     * @generated
     */
    EClass OPTIONS = eINSTANCE.getOptions();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.impl.ExprVarImpl <em>Expr Var</em>}' class.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.impl.ExprVarImpl
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getExprVar()
     * @generated
     */
    EClass EXPR_VAR = eINSTANCE.getExprVar();

    /**
     * The meta object literal for the '<em><b>Name</b></em>' attribute feature.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @generated
     */
    EAttribute EXPR_VAR__NAME = eINSTANCE.getExprVar_Name();

    /**
     * The meta object literal for the '{@link org.oorexx.workbench.rexx.VISIBILITY <em>VISIBILITY</em>}' enum.
     * <!-- begin-user-doc -->
     * <!-- end-user-doc -->
     * @see org.oorexx.workbench.rexx.VISIBILITY
     * @see org.oorexx.workbench.rexx.impl.RexxPackageImpl#getVISIBILITY()
     * @generated
     */
    EEnum VISIBILITY = eINSTANCE.getVISIBILITY();

  }

} //RexxPackage
