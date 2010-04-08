lexer grammar InternalRexx;
@header {
package org.oorexx.workbench.ui.contentassist.antlr.internal;

// Hack: Use our own Lexer superclass by means of import. 
// Currently there is no other way to specify the superclass for the lexer.
import org.eclipse.xtext.ui.editor.contentassist.antlr.internal.Lexer;
}

T60 : 'private' ;
T61 : 'public' ;
T62 : '=' ;
T63 : 'if' ;
T64 : 'then' ;
T65 : 'else' ;
T66 : 'do' ;
T67 : 'end' ;
T68 : 'call' ;
T69 : 'arg' ;
T70 : ',' ;
T71 : 'use' ;
T72 : 'raise' ;
T73 : 'syntax' ;
T74 : '.' ;
T75 : '[' ;
T76 : ']' ;
T77 : '(' ;
T78 : ')' ;
T79 : '~' ;
T80 : ':' ;
T81 : '\\' ;
T82 : '::class' ;
T83 : 'metaclass' ;
T84 : 'subclass' ;
T85 : 'mixinclass' ;
T86 : 'inherit' ;
T87 : '::method' ;
T88 : 'expose' ;
T89 : '::attribute' ;
T90 : 'set' ;
T91 : 'get' ;
T92 : '::constant' ;
T93 : '::routine' ;
T94 : '::requires' ;
T95 : '::options' ;
T96 : 'strict' ;
T97 : 'class' ;

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5127
RULE_WS : (' '|'\t')+;

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5129
RULE_ML_COMMENT : '/*' ( options {greedy=false;} : . )*'*/';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5131
RULE_SL_COMMENT : '--' ~(('\r'|'\n'))*;

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5133
RULE_TERM : (RULE_NL|';')+;

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5135
RULE_NL : '\r'? '\n';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5137
RULE_STRING : ('"' ~('"')* '"'|'\'' (~('\'')|'\'\'')* '\'');

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5139
RULE_STRING_HEX : RULE_STRING ('X'|'x');

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5141
RULE_STRING_BOOL : RULE_STRING ('B'|'b');

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5143
RULE_SYMBOL_VAR : ('A'..'Z'|'a'..'z'|'_'|'?'|'!') ('A'..'Z'|'a'..'z'|'0'..'9'|'_'|'.'|'!'|'?')*;

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5145
RULE_SYMBOL_CLS : '.' RULE_SYMBOL_VAR;

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5147
RULE_SYMBOL_NUM : ('0'..'9')+;

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5149
RULE_OP_PLUS : '+';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5151
RULE_OP_MINUS : '-';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5153
RULE_OP_SLASH : '/';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5155
RULE_OP_PERCENT : '%';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5157
RULE_OP_STAR : '*';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5159
RULE_OP_PIPE : '|';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5161
RULE_OP_ET : '&';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5163
RULE_OP_NEG : '\u00AC';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5165
RULE_OP_GT : '>';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5167
RULE_OP_LT : '<';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5169
RULE_OP_GEQ : '>=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5171
RULE_OP_LEQ : '<=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5173
RULE_OP_BSGT : '\\>';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5175
RULE_OP_BSLT : '\\<';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5177
RULE_OP_BSEQ : '\\=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5179
RULE_OP_GTLT : '><';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5181
RULE_OP_LTGT : '<>';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5183
RULE_OP_EQ2 : '==';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5185
RULE_OP_BSEQ2 : '\\==';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5187
RULE_OP_SLASH2 : '//';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5189
RULE_OP_AND2 : '&&';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5191
RULE_OP_PIPE2 : '||';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5193
RULE_OP_STAR2 : '**';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5195
RULE_OP_NGT : '\u00AC>';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5197
RULE_OP_NLT : '\u00AC<';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5199
RULE_OP_NEQ : '\u00AC=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5201
RULE_OP_NEQ2 : '\u00AC==';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5203
RULE_OP_GT2 : '>>';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5205
RULE_OP_LT2 : '<<';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5207
RULE_OP_GT2EQ : '>>=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5209
RULE_OP_BSLT2 : '\\<<';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5211
RULE_OP_NLT2 : '\u00AC<<';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5213
RULE_OP_BSGT2 : '\\>>';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5215
RULE_OP_NGT2 : '\u00AC>>';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5217
RULE_OP_LT2EQ : '<<=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5219
RULE_OP_PLUSEQ : '+=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5221
RULE_OP_MINUSEQ : '-=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5223
RULE_OP_STAREQ : '*=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5225
RULE_OP_SLASHEQ : '/=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5227
RULE_OP_PERCENTEQ : '%=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5229
RULE_OP_SLAHS2EQ : '//=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5231
RULE_OP_PIPE2EQ : '||=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5233
RULE_OP_ANDEQ : '&=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5235
RULE_OP_PIPEEQ : '|=';

// $ANTLR src "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g" 5237
RULE_OP_AND2EQ : '&&=';


