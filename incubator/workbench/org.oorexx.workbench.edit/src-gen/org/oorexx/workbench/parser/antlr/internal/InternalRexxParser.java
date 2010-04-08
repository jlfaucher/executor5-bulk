package org.oorexx.workbench.parser.antlr.internal; 

import java.io.InputStream;
import org.eclipse.xtext.*;
import org.eclipse.xtext.parser.*;
import org.eclipse.xtext.parser.impl.*;
import org.eclipse.xtext.parsetree.*;
import org.eclipse.emf.ecore.util.EcoreUtil;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.common.util.Enumerator;
import org.eclipse.xtext.parser.antlr.AbstractInternalAntlrParser;
import org.eclipse.xtext.parser.antlr.XtextTokenStream;
import org.eclipse.xtext.parser.antlr.XtextTokenStream.HiddenTokens;
import org.eclipse.xtext.parser.antlr.AntlrDatatypeRuleToken;
import org.eclipse.xtext.conversion.ValueConverterException;
import org.oorexx.workbench.services.RexxGrammarAccess;



import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

public class InternalRexxParser extends AbstractInternalAntlrParser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "KEYWORD_38", "KEYWORD_35", "KEYWORD_36", "KEYWORD_37", "KEYWORD_32", "KEYWORD_33", "KEYWORD_34", "KEYWORD_30", "KEYWORD_31", "KEYWORD_27", "KEYWORD_28", "KEYWORD_29", "KEYWORD_23", "KEYWORD_24", "KEYWORD_25", "KEYWORD_26", "KEYWORD_21", "KEYWORD_22", "KEYWORD_18", "KEYWORD_19", "KEYWORD_20", "KEYWORD_13", "KEYWORD_14", "KEYWORD_15", "KEYWORD_16", "KEYWORD_17", "KEYWORD_11", "KEYWORD_12", "KEYWORD_1", "KEYWORD_2", "KEYWORD_3", "KEYWORD_4", "KEYWORD_5", "KEYWORD_6", "KEYWORD_7", "KEYWORD_8", "KEYWORD_9", "KEYWORD_10", "RULE_WS", "RULE_ML_COMMENT", "RULE_SL_COMMENT", "RULE_NL", "RULE_TERM", "RULE_STRING", "RULE_STRING_HEX", "RULE_STRING_BOOL", "RULE_SYMBOL_VAR", "RULE_SYMBOL_CLS", "RULE_SYMBOL_NUM", "RULE_OP_PLUS", "RULE_OP_MINUS", "RULE_OP_SLASH", "RULE_OP_PERCENT", "RULE_OP_STAR", "RULE_OP_PIPE", "RULE_OP_ET", "RULE_OP_NEG", "RULE_OP_GT", "RULE_OP_LT", "RULE_OP_GEQ", "RULE_OP_LEQ", "RULE_OP_BSGT", "RULE_OP_BSLT", "RULE_OP_BSEQ", "RULE_OP_GTLT", "RULE_OP_LTGT", "RULE_OP_EQ2", "RULE_OP_BSEQ2", "RULE_OP_SLASH2", "RULE_OP_AND2", "RULE_OP_PIPE2", "RULE_OP_STAR2", "RULE_OP_NGT", "RULE_OP_NLT", "RULE_OP_NEQ", "RULE_OP_NEQ2", "RULE_OP_GT2", "RULE_OP_LT2", "RULE_OP_GT2EQ", "RULE_OP_BSLT2", "RULE_OP_NLT2", "RULE_OP_BSGT2", "RULE_OP_NGT2", "RULE_OP_LT2EQ", "RULE_OP_PLUSEQ", "RULE_OP_MINUSEQ", "RULE_OP_STAREQ", "RULE_OP_SLASHEQ", "RULE_OP_PERCENTEQ", "RULE_OP_SLAHS2EQ", "RULE_OP_PIPE2EQ", "RULE_OP_ANDEQ", "RULE_OP_PIPEEQ", "RULE_OP_AND2EQ", "Tokens"
    };
    public static final int RULE_OP_NGT=76;
    public static final int RULE_SYMBOL_VAR=50;
    public static final int RULE_SYMBOL_CLS=51;
    public static final int RULE_OP_GT=61;
    public static final int RULE_OP_PIPE=58;
    public static final int RULE_OP_SLASH2=72;
    public static final int RULE_OP_PERCENTEQ=92;
    public static final int RULE_OP_BSEQ=67;
    public static final int RULE_OP_STAREQ=90;
    public static final int RULE_OP_BSEQ2=71;
    public static final int RULE_OP_STAR2=75;
    public static final int RULE_OP_GT2=80;
    public static final int RULE_OP_NGT2=86;
    public static final int KEYWORD_1=32;
    public static final int KEYWORD_2=33;
    public static final int RULE_OP_LEQ=64;
    public static final int KEYWORD_3=34;
    public static final int KEYWORD_4=35;
    public static final int KEYWORD_5=36;
    public static final int KEYWORD_6=37;
    public static final int KEYWORD_7=38;
    public static final int KEYWORD_8=39;
    public static final int KEYWORD_9=40;
    public static final int RULE_OP_SLAHS2EQ=93;
    public static final int RULE_OP_PIPE2=74;
    public static final int RULE_OP_EQ2=70;
    public static final int RULE_OP_NEQ2=79;
    public static final int RULE_OP_MINUS=54;
    public static final int KEYWORD_10=41;
    public static final int RULE_OP_GT2EQ=82;
    public static final int KEYWORD_11=30;
    public static final int KEYWORD_12=31;
    public static final int KEYWORD_13=25;
    public static final int RULE_OP_BSGT=65;
    public static final int KEYWORD_14=26;
    public static final int KEYWORD_15=27;
    public static final int KEYWORD_16=28;
    public static final int KEYWORD_17=29;
    public static final int KEYWORD_18=22;
    public static final int KEYWORD_19=23;
    public static final int RULE_OP_PLUSEQ=88;
    public static final int RULE_SYMBOL_NUM=52;
    public static final int RULE_OP_SLASH=55;
    public static final int KEYWORD_20=24;
    public static final int RULE_OP_PIPEEQ=96;
    public static final int KEYWORD_21=20;
    public static final int KEYWORD_22=21;
    public static final int KEYWORD_23=16;
    public static final int KEYWORD_24=17;
    public static final int RULE_OP_GEQ=63;
    public static final int KEYWORD_25=18;
    public static final int KEYWORD_26=19;
    public static final int KEYWORD_27=13;
    public static final int KEYWORD_28=14;
    public static final int RULE_SL_COMMENT=44;
    public static final int KEYWORD_29=15;
    public static final int RULE_OP_SLASHEQ=91;
    public static final int RULE_OP_GTLT=68;
    public static final int KEYWORD_30=11;
    public static final int KEYWORD_31=12;
    public static final int KEYWORD_32=8;
    public static final int RULE_STRING_BOOL=49;
    public static final int KEYWORD_33=9;
    public static final int KEYWORD_34=10;
    public static final int KEYWORD_35=5;
    public static final int KEYWORD_36=6;
    public static final int KEYWORD_37=7;
    public static final int KEYWORD_38=4;
    public static final int RULE_OP_NLT=77;
    public static final int RULE_OP_LT=62;
    public static final int RULE_OP_ANDEQ=95;
    public static final int RULE_OP_STAR=57;
    public static final int RULE_STRING=47;
    public static final int RULE_OP_PIPE2EQ=94;
    public static final int RULE_OP_NEG=60;
    public static final int RULE_OP_AND2=73;
    public static final int Tokens=98;
    public static final int RULE_OP_BSLT2=83;
    public static final int RULE_OP_NEQ=78;
    public static final int RULE_OP_MINUSEQ=89;
    public static final int RULE_ML_COMMENT=43;
    public static final int RULE_OP_ET=59;
    public static final int RULE_TERM=46;
    public static final int RULE_OP_LTGT=69;
    public static final int RULE_OP_LT2EQ=87;
    public static final int EOF=-1;
    public static final int RULE_NL=45;
    public static final int RULE_OP_AND2EQ=97;
    public static final int RULE_OP_PERCENT=56;
    public static final int RULE_OP_LT2=81;
    public static final int RULE_STRING_HEX=48;
    public static final int RULE_OP_BSLT=66;
    public static final int RULE_OP_PLUS=53;
    public static final int RULE_OP_NLT2=84;
    public static final int RULE_OP_BSGT2=85;
    public static final int RULE_WS=42;

        public InternalRexxParser(TokenStream input) {
            super(input);
        }
        

    public String[] getTokenNames() { return tokenNames; }
    public String getGrammarFileName() { return "../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g"; }




     	private RexxGrammarAccess grammarAccess;
     	
        public InternalRexxParser(TokenStream input, IAstFactory factory, RexxGrammarAccess grammarAccess) {
            this(input);
            this.factory = factory;
            registerRules(grammarAccess.getGrammar());
            this.grammarAccess = grammarAccess;
        }
        
        @Override
        protected InputStream getTokenFile() {
        	ClassLoader classLoader = getClass().getClassLoader();
        	return classLoader.getResourceAsStream("org/oorexx/workbench/parser/antlr/internal/InternalRexx.tokens");
        }
        
        @Override
        protected String getFirstRuleName() {
        	return "File";	
       	} 
       	   	
       	@Override
       	protected RexxGrammarAccess getGrammarAccess() {
       		return grammarAccess;
       	}



    // $ANTLR start entryRuleFile
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:72:1: entryRuleFile returns [EObject current=null] : iv_ruleFile= ruleFile EOF ;
    public final EObject entryRuleFile() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleFile = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:73:2: (iv_ruleFile= ruleFile EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:74:2: iv_ruleFile= ruleFile EOF
            {
             currentNode = createCompositeNode(grammarAccess.getFileRule(), currentNode); 
            pushFollow(FOLLOW_ruleFile_in_entryRuleFile67);
            iv_ruleFile=ruleFile();
            _fsp--;

             current =iv_ruleFile; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleFile77); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleFile


    // $ANTLR start ruleFile
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:81:1: ruleFile returns [EObject current=null] : ( () ( RULE_TERM )* ( (lv_instructions_2_0= ruleInstruction ) )* ( (lv_requires_3_0= ruleRequires ) )* ( (lv_methods_4_0= ruleMethod ) )* ( (lv_directives_5_0= ruleDirectives ) )* ) ;
    public final EObject ruleFile() throws RecognitionException {
        EObject current = null;

        EObject lv_instructions_2_0 = null;

        EObject lv_requires_3_0 = null;

        EObject lv_methods_4_0 = null;

        EObject lv_directives_5_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:86:6: ( ( () ( RULE_TERM )* ( (lv_instructions_2_0= ruleInstruction ) )* ( (lv_requires_3_0= ruleRequires ) )* ( (lv_methods_4_0= ruleMethod ) )* ( (lv_directives_5_0= ruleDirectives ) )* ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:87:1: ( () ( RULE_TERM )* ( (lv_instructions_2_0= ruleInstruction ) )* ( (lv_requires_3_0= ruleRequires ) )* ( (lv_methods_4_0= ruleMethod ) )* ( (lv_directives_5_0= ruleDirectives ) )* )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:87:1: ( () ( RULE_TERM )* ( (lv_instructions_2_0= ruleInstruction ) )* ( (lv_requires_3_0= ruleRequires ) )* ( (lv_methods_4_0= ruleMethod ) )* ( (lv_directives_5_0= ruleDirectives ) )* )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:87:2: () ( RULE_TERM )* ( (lv_instructions_2_0= ruleInstruction ) )* ( (lv_requires_3_0= ruleRequires ) )* ( (lv_methods_4_0= ruleMethod ) )* ( (lv_directives_5_0= ruleDirectives ) )*
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:87:2: ()
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:88:5: 
            {
             
                    temp=factory.create(grammarAccess.getFileAccess().getFileAction_0().getType().getClassifier());
                    current = temp; 
                    temp = null;
                    CompositeNode newNode = createCompositeNode(grammarAccess.getFileAccess().getFileAction_0(), currentNode.getParent());
                newNode.getChildren().add(currentNode);
                moveLookaheadInfo(currentNode, newNode);
                currentNode = newNode; 
                    associateNodeWithAstElement(currentNode, current); 
                

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:98:2: ( RULE_TERM )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==RULE_TERM) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:98:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleFile121); 
            	     
            	        createLeafNode(grammarAccess.getFileAccess().getTERMTerminalRuleCall_1(), null); 
            	        

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:102:3: ( (lv_instructions_2_0= ruleInstruction ) )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( ((LA2_0>=KEYWORD_22 && LA2_0<=KEYWORD_18)||LA2_0==KEYWORD_13||(LA2_0>=KEYWORD_17 && LA2_0<=KEYWORD_12)||LA2_0==RULE_STRING||LA2_0==RULE_SYMBOL_VAR) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:103:1: (lv_instructions_2_0= ruleInstruction )
            	    {
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:103:1: (lv_instructions_2_0= ruleInstruction )
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:104:3: lv_instructions_2_0= ruleInstruction
            	    {
            	     
            	    	        currentNode=createCompositeNode(grammarAccess.getFileAccess().getInstructionsInstructionParserRuleCall_2_0(), currentNode); 
            	    	    
            	    pushFollow(FOLLOW_ruleInstruction_in_ruleFile143);
            	    lv_instructions_2_0=ruleInstruction();
            	    _fsp--;


            	    	        if (current==null) {
            	    	            current = factory.create(grammarAccess.getFileRule().getType().getClassifier());
            	    	            associateNodeWithAstElement(currentNode.getParent(), current);
            	    	        }
            	    	        try {
            	    	       		add(
            	    	       			current, 
            	    	       			"instructions",
            	    	        		lv_instructions_2_0, 
            	    	        		"Instruction", 
            	    	        		currentNode);
            	    	        } catch (ValueConverterException vce) {
            	    				handleValueConverterException(vce);
            	    	        }
            	    	        currentNode = currentNode.getParent();
            	    	    

            	    }


            	    }
            	    break;

            	default :
            	    break loop2;
                }
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:126:3: ( (lv_requires_3_0= ruleRequires ) )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==KEYWORD_36) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:127:1: (lv_requires_3_0= ruleRequires )
            	    {
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:127:1: (lv_requires_3_0= ruleRequires )
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:128:3: lv_requires_3_0= ruleRequires
            	    {
            	     
            	    	        currentNode=createCompositeNode(grammarAccess.getFileAccess().getRequiresRequiresParserRuleCall_3_0(), currentNode); 
            	    	    
            	    pushFollow(FOLLOW_ruleRequires_in_ruleFile165);
            	    lv_requires_3_0=ruleRequires();
            	    _fsp--;


            	    	        if (current==null) {
            	    	            current = factory.create(grammarAccess.getFileRule().getType().getClassifier());
            	    	            associateNodeWithAstElement(currentNode.getParent(), current);
            	    	        }
            	    	        try {
            	    	       		add(
            	    	       			current, 
            	    	       			"requires",
            	    	        		lv_requires_3_0, 
            	    	        		"Requires", 
            	    	        		currentNode);
            	    	        } catch (ValueConverterException vce) {
            	    				handleValueConverterException(vce);
            	    	        }
            	    	        currentNode = currentNode.getParent();
            	    	    

            	    }


            	    }
            	    break;

            	default :
            	    break loop3;
                }
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:150:3: ( (lv_methods_4_0= ruleMethod ) )*
            loop4:
            do {
                int alt4=2;
                int LA4_0 = input.LA(1);

                if ( (LA4_0==KEYWORD_30) ) {
                    alt4=1;
                }


                switch (alt4) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:151:1: (lv_methods_4_0= ruleMethod )
            	    {
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:151:1: (lv_methods_4_0= ruleMethod )
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:152:3: lv_methods_4_0= ruleMethod
            	    {
            	     
            	    	        currentNode=createCompositeNode(grammarAccess.getFileAccess().getMethodsMethodParserRuleCall_4_0(), currentNode); 
            	    	    
            	    pushFollow(FOLLOW_ruleMethod_in_ruleFile187);
            	    lv_methods_4_0=ruleMethod();
            	    _fsp--;


            	    	        if (current==null) {
            	    	            current = factory.create(grammarAccess.getFileRule().getType().getClassifier());
            	    	            associateNodeWithAstElement(currentNode.getParent(), current);
            	    	        }
            	    	        try {
            	    	       		add(
            	    	       			current, 
            	    	       			"methods",
            	    	        		lv_methods_4_0, 
            	    	        		"Method", 
            	    	        		currentNode);
            	    	        } catch (ValueConverterException vce) {
            	    				handleValueConverterException(vce);
            	    	        }
            	    	        currentNode = currentNode.getParent();
            	    	    

            	    }


            	    }
            	    break;

            	default :
            	    break loop4;
                }
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:174:3: ( (lv_directives_5_0= ruleDirectives ) )*
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( ((LA5_0>=KEYWORD_32 && LA5_0<=KEYWORD_33)||LA5_0==KEYWORD_27) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:175:1: (lv_directives_5_0= ruleDirectives )
            	    {
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:175:1: (lv_directives_5_0= ruleDirectives )
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:176:3: lv_directives_5_0= ruleDirectives
            	    {
            	     
            	    	        currentNode=createCompositeNode(grammarAccess.getFileAccess().getDirectivesDirectivesParserRuleCall_5_0(), currentNode); 
            	    	    
            	    pushFollow(FOLLOW_ruleDirectives_in_ruleFile209);
            	    lv_directives_5_0=ruleDirectives();
            	    _fsp--;


            	    	        if (current==null) {
            	    	            current = factory.create(grammarAccess.getFileRule().getType().getClassifier());
            	    	            associateNodeWithAstElement(currentNode.getParent(), current);
            	    	        }
            	    	        try {
            	    	       		add(
            	    	       			current, 
            	    	       			"directives",
            	    	        		lv_directives_5_0, 
            	    	        		"Directives", 
            	    	        		currentNode);
            	    	        } catch (ValueConverterException vce) {
            	    				handleValueConverterException(vce);
            	    	        }
            	    	        currentNode = currentNode.getParent();
            	    	    

            	    }


            	    }
            	    break;

            	default :
            	    break loop5;
                }
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleFile


    // $ANTLR start entryRuleInstruction
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:206:1: entryRuleInstruction returns [EObject current=null] : iv_ruleInstruction= ruleInstruction EOF ;
    public final EObject entryRuleInstruction() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleInstruction = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:207:2: (iv_ruleInstruction= ruleInstruction EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:208:2: iv_ruleInstruction= ruleInstruction EOF
            {
             currentNode = createCompositeNode(grammarAccess.getInstructionRule(), currentNode); 
            pushFollow(FOLLOW_ruleInstruction_in_entryRuleInstruction245);
            iv_ruleInstruction=ruleInstruction();
            _fsp--;

             current =iv_ruleInstruction; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleInstruction255); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleInstruction


    // $ANTLR start ruleInstruction
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:215:1: ruleInstruction returns [EObject current=null] : ( (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage ) ( RULE_TERM )+ ) ;
    public final EObject ruleInstruction() throws RecognitionException {
        EObject current = null;

        EObject this_Assignment_0 = null;

        EObject this_Do_1 = null;

        EObject this_If_2 = null;

        EObject this_Call_3 = null;

        EObject this_Arg_4 = null;

        EObject this_UseArg_5 = null;

        EObject this_Raise_6 = null;

        EObject this_Message_7 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:220:6: ( ( (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage ) ( RULE_TERM )+ ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:221:1: ( (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage ) ( RULE_TERM )+ )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:221:1: ( (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage ) ( RULE_TERM )+ )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:221:2: (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage ) ( RULE_TERM )+
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:221:2: (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage )
            int alt6=8;
            switch ( input.LA(1) ) {
            case RULE_STRING:
                {
                int LA6_1 = input.LA(2);

                if ( (LA6_1==KEYWORD_10) ) {
                    alt6=8;
                }
                else if ( (LA6_1==KEYWORD_6) ) {
                    alt6=1;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("221:2: (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage )", 6, 1, input);

                    throw nvae;
                }
                }
                break;
            case RULE_SYMBOL_VAR:
                {
                int LA6_2 = input.LA(2);

                if ( (LA6_2==KEYWORD_10) ) {
                    alt6=8;
                }
                else if ( (LA6_2==KEYWORD_6) ) {
                    alt6=1;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("221:2: (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage )", 6, 2, input);

                    throw nvae;
                }
                }
                break;
            case KEYWORD_11:
                {
                alt6=2;
                }
                break;
            case KEYWORD_12:
                {
                alt6=3;
                }
                break;
            case KEYWORD_18:
                {
                alt6=4;
                }
                break;
            case KEYWORD_13:
                {
                alt6=5;
                }
                break;
            case KEYWORD_17:
                {
                alt6=6;
                }
                break;
            case KEYWORD_22:
                {
                alt6=7;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("221:2: (this_Assignment_0= ruleAssignment | this_Do_1= ruleDo | this_If_2= ruleIf | this_Call_3= ruleCall | this_Arg_4= ruleArg | this_UseArg_5= ruleUseArg | this_Raise_6= ruleRaise | this_Message_7= ruleMessage )", 6, 0, input);

                throw nvae;
            }

            switch (alt6) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:222:5: this_Assignment_0= ruleAssignment
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getInstructionAccess().getAssignmentParserRuleCall_0_0(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleAssignment_in_ruleInstruction303);
                    this_Assignment_0=ruleAssignment();
                    _fsp--;

                     
                            current = this_Assignment_0; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:232:5: this_Do_1= ruleDo
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getInstructionAccess().getDoParserRuleCall_0_1(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleDo_in_ruleInstruction330);
                    this_Do_1=ruleDo();
                    _fsp--;

                     
                            current = this_Do_1; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:242:5: this_If_2= ruleIf
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getInstructionAccess().getIfParserRuleCall_0_2(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleIf_in_ruleInstruction357);
                    this_If_2=ruleIf();
                    _fsp--;

                     
                            current = this_If_2; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 4 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:252:5: this_Call_3= ruleCall
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getInstructionAccess().getCallParserRuleCall_0_3(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleCall_in_ruleInstruction384);
                    this_Call_3=ruleCall();
                    _fsp--;

                     
                            current = this_Call_3; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 5 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:262:5: this_Arg_4= ruleArg
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getInstructionAccess().getArgParserRuleCall_0_4(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleArg_in_ruleInstruction411);
                    this_Arg_4=ruleArg();
                    _fsp--;

                     
                            current = this_Arg_4; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 6 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:272:5: this_UseArg_5= ruleUseArg
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getInstructionAccess().getUseArgParserRuleCall_0_5(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleUseArg_in_ruleInstruction438);
                    this_UseArg_5=ruleUseArg();
                    _fsp--;

                     
                            current = this_UseArg_5; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 7 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:282:5: this_Raise_6= ruleRaise
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getInstructionAccess().getRaiseParserRuleCall_0_6(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleRaise_in_ruleInstruction465);
                    this_Raise_6=ruleRaise();
                    _fsp--;

                     
                            current = this_Raise_6; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 8 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:292:5: this_Message_7= ruleMessage
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getInstructionAccess().getMessageParserRuleCall_0_7(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleMessage_in_ruleInstruction492);
                    this_Message_7=ruleMessage();
                    _fsp--;

                     
                            current = this_Message_7; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:300:2: ( RULE_TERM )+
            int cnt7=0;
            loop7:
            do {
                int alt7=2;
                int LA7_0 = input.LA(1);

                if ( (LA7_0==RULE_TERM) ) {
                    alt7=1;
                }


                switch (alt7) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:300:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleInstruction502); 
            	     
            	        createLeafNode(grammarAccess.getInstructionAccess().getTERMTerminalRuleCall_1(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt7 >= 1 ) break loop7;
                        EarlyExitException eee =
                            new EarlyExitException(7, input);
                        throw eee;
                }
                cnt7++;
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleInstruction


    // $ANTLR start entryRuleAssignment
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:312:1: entryRuleAssignment returns [EObject current=null] : iv_ruleAssignment= ruleAssignment EOF ;
    public final EObject entryRuleAssignment() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleAssignment = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:313:2: (iv_ruleAssignment= ruleAssignment EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:314:2: iv_ruleAssignment= ruleAssignment EOF
            {
             currentNode = createCompositeNode(grammarAccess.getAssignmentRule(), currentNode); 
            pushFollow(FOLLOW_ruleAssignment_in_entryRuleAssignment538);
            iv_ruleAssignment=ruleAssignment();
            _fsp--;

             current =iv_ruleAssignment; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAssignment548); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleAssignment


    // $ANTLR start ruleAssignment
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:321:1: ruleAssignment returns [EObject current=null] : ( ( (lv_target_0_0= ruleName ) ) KEYWORD_6 ( (lv_expression_2_0= ruleExpression ) ) ) ;
    public final EObject ruleAssignment() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_target_0_0 = null;

        EObject lv_expression_2_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:326:6: ( ( ( (lv_target_0_0= ruleName ) ) KEYWORD_6 ( (lv_expression_2_0= ruleExpression ) ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:327:1: ( ( (lv_target_0_0= ruleName ) ) KEYWORD_6 ( (lv_expression_2_0= ruleExpression ) ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:327:1: ( ( (lv_target_0_0= ruleName ) ) KEYWORD_6 ( (lv_expression_2_0= ruleExpression ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:327:2: ( (lv_target_0_0= ruleName ) ) KEYWORD_6 ( (lv_expression_2_0= ruleExpression ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:327:2: ( (lv_target_0_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:328:1: (lv_target_0_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:328:1: (lv_target_0_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:329:3: lv_target_0_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getAssignmentAccess().getTargetNameParserRuleCall_0_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleAssignment594);
            lv_target_0_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getAssignmentRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"target",
            	        		lv_target_0_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_6,FOLLOW_KEYWORD_6_in_ruleAssignment605); 

                    createLeafNode(grammarAccess.getAssignmentAccess().getEqualsSignKeyword_1(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:356:1: ( (lv_expression_2_0= ruleExpression ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:357:1: (lv_expression_2_0= ruleExpression )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:357:1: (lv_expression_2_0= ruleExpression )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:358:3: lv_expression_2_0= ruleExpression
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getAssignmentAccess().getExpressionExpressionParserRuleCall_2_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleExpression_in_ruleAssignment626);
            lv_expression_2_0=ruleExpression();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getAssignmentRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"expression",
            	        		lv_expression_2_0, 
            	        		"Expression", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleAssignment


    // $ANTLR start entryRuleIf
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:388:1: entryRuleIf returns [EObject current=null] : iv_ruleIf= ruleIf EOF ;
    public final EObject entryRuleIf() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleIf = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:389:2: (iv_ruleIf= ruleIf EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:390:2: iv_ruleIf= ruleIf EOF
            {
             currentNode = createCompositeNode(grammarAccess.getIfRule(), currentNode); 
            pushFollow(FOLLOW_ruleIf_in_entryRuleIf661);
            iv_ruleIf=ruleIf();
            _fsp--;

             current =iv_ruleIf; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleIf671); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleIf


    // $ANTLR start ruleIf
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:397:1: ruleIf returns [EObject current=null] : ( KEYWORD_12 ( (lv_expression_1_0= ruleExpression ) ) KEYWORD_20 ( RULE_TERM )* ( (lv_topInstr_4_0= ruleInstruction ) ) ( KEYWORD_19 ( RULE_TERM )* ( (lv_bottomInstr_7_0= ruleInstruction ) ) )? ) ;
    public final EObject ruleIf() throws RecognitionException {
        EObject current = null;

        EObject lv_expression_1_0 = null;

        EObject lv_topInstr_4_0 = null;

        EObject lv_bottomInstr_7_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:402:6: ( ( KEYWORD_12 ( (lv_expression_1_0= ruleExpression ) ) KEYWORD_20 ( RULE_TERM )* ( (lv_topInstr_4_0= ruleInstruction ) ) ( KEYWORD_19 ( RULE_TERM )* ( (lv_bottomInstr_7_0= ruleInstruction ) ) )? ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:403:1: ( KEYWORD_12 ( (lv_expression_1_0= ruleExpression ) ) KEYWORD_20 ( RULE_TERM )* ( (lv_topInstr_4_0= ruleInstruction ) ) ( KEYWORD_19 ( RULE_TERM )* ( (lv_bottomInstr_7_0= ruleInstruction ) ) )? )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:403:1: ( KEYWORD_12 ( (lv_expression_1_0= ruleExpression ) ) KEYWORD_20 ( RULE_TERM )* ( (lv_topInstr_4_0= ruleInstruction ) ) ( KEYWORD_19 ( RULE_TERM )* ( (lv_bottomInstr_7_0= ruleInstruction ) ) )? )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:404:2: KEYWORD_12 ( (lv_expression_1_0= ruleExpression ) ) KEYWORD_20 ( RULE_TERM )* ( (lv_topInstr_4_0= ruleInstruction ) ) ( KEYWORD_19 ( RULE_TERM )* ( (lv_bottomInstr_7_0= ruleInstruction ) ) )?
            {
            match(input,KEYWORD_12,FOLLOW_KEYWORD_12_in_ruleIf707); 

                    createLeafNode(grammarAccess.getIfAccess().getIfKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:408:1: ( (lv_expression_1_0= ruleExpression ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:409:1: (lv_expression_1_0= ruleExpression )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:409:1: (lv_expression_1_0= ruleExpression )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:410:3: lv_expression_1_0= ruleExpression
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getIfAccess().getExpressionExpressionParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleExpression_in_ruleIf728);
            lv_expression_1_0=ruleExpression();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getIfRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"expression",
            	        		lv_expression_1_0, 
            	        		"Expression", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_20,FOLLOW_KEYWORD_20_in_ruleIf739); 

                    createLeafNode(grammarAccess.getIfAccess().getThenKeyword_2(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:437:1: ( RULE_TERM )*
            loop8:
            do {
                int alt8=2;
                int LA8_0 = input.LA(1);

                if ( (LA8_0==RULE_TERM) ) {
                    alt8=1;
                }


                switch (alt8) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:437:2: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleIf749); 
            	     
            	        createLeafNode(grammarAccess.getIfAccess().getTERMTerminalRuleCall_3(), null); 
            	        

            	    }
            	    break;

            	default :
            	    break loop8;
                }
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:441:3: ( (lv_topInstr_4_0= ruleInstruction ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:442:1: (lv_topInstr_4_0= ruleInstruction )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:442:1: (lv_topInstr_4_0= ruleInstruction )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:443:3: lv_topInstr_4_0= ruleInstruction
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getIfAccess().getTopInstrInstructionParserRuleCall_4_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleInstruction_in_ruleIf771);
            lv_topInstr_4_0=ruleInstruction();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getIfRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"topInstr",
            	        		lv_topInstr_4_0, 
            	        		"Instruction", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:465:2: ( KEYWORD_19 ( RULE_TERM )* ( (lv_bottomInstr_7_0= ruleInstruction ) ) )?
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==KEYWORD_19) ) {
                alt10=1;
            }
            switch (alt10) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:466:2: KEYWORD_19 ( RULE_TERM )* ( (lv_bottomInstr_7_0= ruleInstruction ) )
                    {
                    match(input,KEYWORD_19,FOLLOW_KEYWORD_19_in_ruleIf783); 

                            createLeafNode(grammarAccess.getIfAccess().getElseKeyword_5_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:470:1: ( RULE_TERM )*
                    loop9:
                    do {
                        int alt9=2;
                        int LA9_0 = input.LA(1);

                        if ( (LA9_0==RULE_TERM) ) {
                            alt9=1;
                        }


                        switch (alt9) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:470:2: RULE_TERM
                    	    {
                    	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleIf793); 
                    	     
                    	        createLeafNode(grammarAccess.getIfAccess().getTERMTerminalRuleCall_5_1(), null); 
                    	        

                    	    }
                    	    break;

                    	default :
                    	    break loop9;
                        }
                    } while (true);

                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:474:3: ( (lv_bottomInstr_7_0= ruleInstruction ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:475:1: (lv_bottomInstr_7_0= ruleInstruction )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:475:1: (lv_bottomInstr_7_0= ruleInstruction )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:476:3: lv_bottomInstr_7_0= ruleInstruction
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getIfAccess().getBottomInstrInstructionParserRuleCall_5_2_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleInstruction_in_ruleIf815);
                    lv_bottomInstr_7_0=ruleInstruction();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getIfRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"bottomInstr",
                    	        		lv_bottomInstr_7_0, 
                    	        		"Instruction", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }


                    }
                    break;

            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleIf


    // $ANTLR start entryRuleDo
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:506:1: entryRuleDo returns [EObject current=null] : iv_ruleDo= ruleDo EOF ;
    public final EObject entryRuleDo() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleDo = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:507:2: (iv_ruleDo= ruleDo EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:508:2: iv_ruleDo= ruleDo EOF
            {
             currentNode = createCompositeNode(grammarAccess.getDoRule(), currentNode); 
            pushFollow(FOLLOW_ruleDo_in_entryRuleDo852);
            iv_ruleDo=ruleDo();
            _fsp--;

             current =iv_ruleDo; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleDo862); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleDo


    // $ANTLR start ruleDo
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:515:1: ruleDo returns [EObject current=null] : ( KEYWORD_11 ( RULE_TERM )+ ( (lv_instrs_2_0= ruleInstruction ) )+ KEYWORD_14 ) ;
    public final EObject ruleDo() throws RecognitionException {
        EObject current = null;

        EObject lv_instrs_2_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:520:6: ( ( KEYWORD_11 ( RULE_TERM )+ ( (lv_instrs_2_0= ruleInstruction ) )+ KEYWORD_14 ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:521:1: ( KEYWORD_11 ( RULE_TERM )+ ( (lv_instrs_2_0= ruleInstruction ) )+ KEYWORD_14 )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:521:1: ( KEYWORD_11 ( RULE_TERM )+ ( (lv_instrs_2_0= ruleInstruction ) )+ KEYWORD_14 )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:522:2: KEYWORD_11 ( RULE_TERM )+ ( (lv_instrs_2_0= ruleInstruction ) )+ KEYWORD_14
            {
            match(input,KEYWORD_11,FOLLOW_KEYWORD_11_in_ruleDo898); 

                    createLeafNode(grammarAccess.getDoAccess().getDoKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:526:1: ( RULE_TERM )+
            int cnt11=0;
            loop11:
            do {
                int alt11=2;
                int LA11_0 = input.LA(1);

                if ( (LA11_0==RULE_TERM) ) {
                    alt11=1;
                }


                switch (alt11) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:526:2: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleDo908); 
            	     
            	        createLeafNode(grammarAccess.getDoAccess().getTERMTerminalRuleCall_1(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt11 >= 1 ) break loop11;
                        EarlyExitException eee =
                            new EarlyExitException(11, input);
                        throw eee;
                }
                cnt11++;
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:530:3: ( (lv_instrs_2_0= ruleInstruction ) )+
            int cnt12=0;
            loop12:
            do {
                int alt12=2;
                int LA12_0 = input.LA(1);

                if ( ((LA12_0>=KEYWORD_22 && LA12_0<=KEYWORD_18)||LA12_0==KEYWORD_13||(LA12_0>=KEYWORD_17 && LA12_0<=KEYWORD_12)||LA12_0==RULE_STRING||LA12_0==RULE_SYMBOL_VAR) ) {
                    alt12=1;
                }


                switch (alt12) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:531:1: (lv_instrs_2_0= ruleInstruction )
            	    {
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:531:1: (lv_instrs_2_0= ruleInstruction )
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:532:3: lv_instrs_2_0= ruleInstruction
            	    {
            	     
            	    	        currentNode=createCompositeNode(grammarAccess.getDoAccess().getInstrsInstructionParserRuleCall_2_0(), currentNode); 
            	    	    
            	    pushFollow(FOLLOW_ruleInstruction_in_ruleDo930);
            	    lv_instrs_2_0=ruleInstruction();
            	    _fsp--;


            	    	        if (current==null) {
            	    	            current = factory.create(grammarAccess.getDoRule().getType().getClassifier());
            	    	            associateNodeWithAstElement(currentNode.getParent(), current);
            	    	        }
            	    	        try {
            	    	       		add(
            	    	       			current, 
            	    	       			"instrs",
            	    	        		lv_instrs_2_0, 
            	    	        		"Instruction", 
            	    	        		currentNode);
            	    	        } catch (ValueConverterException vce) {
            	    				handleValueConverterException(vce);
            	    	        }
            	    	        currentNode = currentNode.getParent();
            	    	    

            	    }


            	    }
            	    break;

            	default :
            	    if ( cnt12 >= 1 ) break loop12;
                        EarlyExitException eee =
                            new EarlyExitException(12, input);
                        throw eee;
                }
                cnt12++;
            } while (true);

            match(input,KEYWORD_14,FOLLOW_KEYWORD_14_in_ruleDo942); 

                    createLeafNode(grammarAccess.getDoAccess().getEndKeyword_3(), null); 
                

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleDo


    // $ANTLR start entryRuleCall
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:567:1: entryRuleCall returns [EObject current=null] : iv_ruleCall= ruleCall EOF ;
    public final EObject entryRuleCall() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleCall = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:568:2: (iv_ruleCall= ruleCall EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:569:2: iv_ruleCall= ruleCall EOF
            {
             currentNode = createCompositeNode(grammarAccess.getCallRule(), currentNode); 
            pushFollow(FOLLOW_ruleCall_in_entryRuleCall977);
            iv_ruleCall=ruleCall();
            _fsp--;

             current =iv_ruleCall; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleCall987); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleCall


    // $ANTLR start ruleCall
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:576:1: ruleCall returns [EObject current=null] : ( KEYWORD_18 ( (lv_name_1_0= ruleName ) ) ( (lv_args_2_0= ruleArguments ) ) ) ;
    public final EObject ruleCall() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;

        EObject lv_args_2_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:581:6: ( ( KEYWORD_18 ( (lv_name_1_0= ruleName ) ) ( (lv_args_2_0= ruleArguments ) ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:582:1: ( KEYWORD_18 ( (lv_name_1_0= ruleName ) ) ( (lv_args_2_0= ruleArguments ) ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:582:1: ( KEYWORD_18 ( (lv_name_1_0= ruleName ) ) ( (lv_args_2_0= ruleArguments ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:583:2: KEYWORD_18 ( (lv_name_1_0= ruleName ) ) ( (lv_args_2_0= ruleArguments ) )
            {
            match(input,KEYWORD_18,FOLLOW_KEYWORD_18_in_ruleCall1023); 

                    createLeafNode(grammarAccess.getCallAccess().getCallKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:587:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:588:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:588:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:589:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getCallAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleCall1044);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getCallRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:611:2: ( (lv_args_2_0= ruleArguments ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:612:1: (lv_args_2_0= ruleArguments )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:612:1: (lv_args_2_0= ruleArguments )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:613:3: lv_args_2_0= ruleArguments
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getCallAccess().getArgsArgumentsParserRuleCall_2_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleArguments_in_ruleCall1065);
            lv_args_2_0=ruleArguments();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getCallRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"args",
            	        		lv_args_2_0, 
            	        		"Arguments", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleCall


    // $ANTLR start entryRuleArg
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:643:1: entryRuleArg returns [EObject current=null] : iv_ruleArg= ruleArg EOF ;
    public final EObject entryRuleArg() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleArg = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:644:2: (iv_ruleArg= ruleArg EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:645:2: iv_ruleArg= ruleArg EOF
            {
             currentNode = createCompositeNode(grammarAccess.getArgRule(), currentNode); 
            pushFollow(FOLLOW_ruleArg_in_entryRuleArg1100);
            iv_ruleArg=ruleArg();
            _fsp--;

             current =iv_ruleArg; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleArg1110); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleArg


    // $ANTLR start ruleArg
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:652:1: ruleArg returns [EObject current=null] : ( () KEYWORD_13 ( ( (lv_args_2_0= ruleName ) ) ( KEYWORD_3 ( (lv_args_4_0= ruleName ) ) )* )? ) ;
    public final EObject ruleArg() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_args_2_0 = null;

        AntlrDatatypeRuleToken lv_args_4_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:657:6: ( ( () KEYWORD_13 ( ( (lv_args_2_0= ruleName ) ) ( KEYWORD_3 ( (lv_args_4_0= ruleName ) ) )* )? ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:658:1: ( () KEYWORD_13 ( ( (lv_args_2_0= ruleName ) ) ( KEYWORD_3 ( (lv_args_4_0= ruleName ) ) )* )? )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:658:1: ( () KEYWORD_13 ( ( (lv_args_2_0= ruleName ) ) ( KEYWORD_3 ( (lv_args_4_0= ruleName ) ) )* )? )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:658:2: () KEYWORD_13 ( ( (lv_args_2_0= ruleName ) ) ( KEYWORD_3 ( (lv_args_4_0= ruleName ) ) )* )?
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:658:2: ()
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:659:5: 
            {
             
                    temp=factory.create(grammarAccess.getArgAccess().getArgAction_0().getType().getClassifier());
                    current = temp; 
                    temp = null;
                    CompositeNode newNode = createCompositeNode(grammarAccess.getArgAccess().getArgAction_0(), currentNode.getParent());
                newNode.getChildren().add(currentNode);
                moveLookaheadInfo(currentNode, newNode);
                currentNode = newNode; 
                    associateNodeWithAstElement(currentNode, current); 
                

            }

            match(input,KEYWORD_13,FOLLOW_KEYWORD_13_in_ruleArg1155); 

                    createLeafNode(grammarAccess.getArgAccess().getArgKeyword_1(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:674:1: ( ( (lv_args_2_0= ruleName ) ) ( KEYWORD_3 ( (lv_args_4_0= ruleName ) ) )* )?
            int alt14=2;
            int LA14_0 = input.LA(1);

            if ( (LA14_0==RULE_STRING||LA14_0==RULE_SYMBOL_VAR) ) {
                alt14=1;
            }
            switch (alt14) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:674:2: ( (lv_args_2_0= ruleName ) ) ( KEYWORD_3 ( (lv_args_4_0= ruleName ) ) )*
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:674:2: ( (lv_args_2_0= ruleName ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:675:1: (lv_args_2_0= ruleName )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:675:1: (lv_args_2_0= ruleName )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:676:3: lv_args_2_0= ruleName
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getArgAccess().getArgsNameParserRuleCall_2_0_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleName_in_ruleArg1177);
                    lv_args_2_0=ruleName();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getArgRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		add(
                    	       			current, 
                    	       			"args",
                    	        		lv_args_2_0, 
                    	        		"Name", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }

                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:698:2: ( KEYWORD_3 ( (lv_args_4_0= ruleName ) ) )*
                    loop13:
                    do {
                        int alt13=2;
                        int LA13_0 = input.LA(1);

                        if ( (LA13_0==KEYWORD_3) ) {
                            alt13=1;
                        }


                        switch (alt13) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:699:2: KEYWORD_3 ( (lv_args_4_0= ruleName ) )
                    	    {
                    	    match(input,KEYWORD_3,FOLLOW_KEYWORD_3_in_ruleArg1189); 

                    	            createLeafNode(grammarAccess.getArgAccess().getCommaKeyword_2_1_0(), null); 
                    	        
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:703:1: ( (lv_args_4_0= ruleName ) )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:704:1: (lv_args_4_0= ruleName )
                    	    {
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:704:1: (lv_args_4_0= ruleName )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:705:3: lv_args_4_0= ruleName
                    	    {
                    	     
                    	    	        currentNode=createCompositeNode(grammarAccess.getArgAccess().getArgsNameParserRuleCall_2_1_1_0(), currentNode); 
                    	    	    
                    	    pushFollow(FOLLOW_ruleName_in_ruleArg1210);
                    	    lv_args_4_0=ruleName();
                    	    _fsp--;


                    	    	        if (current==null) {
                    	    	            current = factory.create(grammarAccess.getArgRule().getType().getClassifier());
                    	    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	    	        }
                    	    	        try {
                    	    	       		add(
                    	    	       			current, 
                    	    	       			"args",
                    	    	        		lv_args_4_0, 
                    	    	        		"Name", 
                    	    	        		currentNode);
                    	    	        } catch (ValueConverterException vce) {
                    	    				handleValueConverterException(vce);
                    	    	        }
                    	    	        currentNode = currentNode.getParent();
                    	    	    

                    	    }


                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop13;
                        }
                    } while (true);


                    }
                    break;

            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleArg


    // $ANTLR start entryRuleUseArg
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:735:1: entryRuleUseArg returns [EObject current=null] : iv_ruleUseArg= ruleUseArg EOF ;
    public final EObject entryRuleUseArg() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleUseArg = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:736:2: (iv_ruleUseArg= ruleUseArg EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:737:2: iv_ruleUseArg= ruleUseArg EOF
            {
             currentNode = createCompositeNode(grammarAccess.getUseArgRule(), currentNode); 
            pushFollow(FOLLOW_ruleUseArg_in_entryRuleUseArg1249);
            iv_ruleUseArg=ruleUseArg();
            _fsp--;

             current =iv_ruleUseArg; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleUseArg1259); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleUseArg


    // $ANTLR start ruleUseArg
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:744:1: ruleUseArg returns [EObject current=null] : ( () KEYWORD_17 ( (lv_strict_2_0= KEYWORD_25 ) )? KEYWORD_13 ( ( (lv_args_4_0= ruleUseArgs ) ) ( KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) ) )* )? ) ;
    public final EObject ruleUseArg() throws RecognitionException {
        EObject current = null;

        Token lv_strict_2_0=null;
        EObject lv_args_4_0 = null;

        EObject lv_args_6_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:749:6: ( ( () KEYWORD_17 ( (lv_strict_2_0= KEYWORD_25 ) )? KEYWORD_13 ( ( (lv_args_4_0= ruleUseArgs ) ) ( KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) ) )* )? ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:750:1: ( () KEYWORD_17 ( (lv_strict_2_0= KEYWORD_25 ) )? KEYWORD_13 ( ( (lv_args_4_0= ruleUseArgs ) ) ( KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) ) )* )? )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:750:1: ( () KEYWORD_17 ( (lv_strict_2_0= KEYWORD_25 ) )? KEYWORD_13 ( ( (lv_args_4_0= ruleUseArgs ) ) ( KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) ) )* )? )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:750:2: () KEYWORD_17 ( (lv_strict_2_0= KEYWORD_25 ) )? KEYWORD_13 ( ( (lv_args_4_0= ruleUseArgs ) ) ( KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) ) )* )?
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:750:2: ()
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:751:5: 
            {
             
                    temp=factory.create(grammarAccess.getUseArgAccess().getUseArgAction_0().getType().getClassifier());
                    current = temp; 
                    temp = null;
                    CompositeNode newNode = createCompositeNode(grammarAccess.getUseArgAccess().getUseArgAction_0(), currentNode.getParent());
                newNode.getChildren().add(currentNode);
                moveLookaheadInfo(currentNode, newNode);
                currentNode = newNode; 
                    associateNodeWithAstElement(currentNode, current); 
                

            }

            match(input,KEYWORD_17,FOLLOW_KEYWORD_17_in_ruleUseArg1304); 

                    createLeafNode(grammarAccess.getUseArgAccess().getUseKeyword_1(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:766:1: ( (lv_strict_2_0= KEYWORD_25 ) )?
            int alt15=2;
            int LA15_0 = input.LA(1);

            if ( (LA15_0==KEYWORD_25) ) {
                alt15=1;
            }
            switch (alt15) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:767:1: (lv_strict_2_0= KEYWORD_25 )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:767:1: (lv_strict_2_0= KEYWORD_25 )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:768:3: lv_strict_2_0= KEYWORD_25
                    {
                    lv_strict_2_0=(Token)input.LT(1);
                    match(input,KEYWORD_25,FOLLOW_KEYWORD_25_in_ruleUseArg1323); 

                            createLeafNode(grammarAccess.getUseArgAccess().getStrictStrictKeyword_2_0(), "strict"); 
                        

                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getUseArgRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode, current);
                    	        }
                    	        
                    	        try {
                    	       		set(current, "strict", true, "strict", lastConsumedNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	    

                    }


                    }
                    break;

            }

            match(input,KEYWORD_13,FOLLOW_KEYWORD_13_in_ruleUseArg1348); 

                    createLeafNode(grammarAccess.getUseArgAccess().getArgKeyword_3(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:793:1: ( ( (lv_args_4_0= ruleUseArgs ) ) ( KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) ) )* )?
            int alt17=2;
            int LA17_0 = input.LA(1);

            if ( (LA17_0==RULE_STRING||LA17_0==RULE_SYMBOL_VAR) ) {
                alt17=1;
            }
            switch (alt17) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:793:2: ( (lv_args_4_0= ruleUseArgs ) ) ( KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) ) )*
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:793:2: ( (lv_args_4_0= ruleUseArgs ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:794:1: (lv_args_4_0= ruleUseArgs )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:794:1: (lv_args_4_0= ruleUseArgs )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:795:3: lv_args_4_0= ruleUseArgs
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getUseArgAccess().getArgsUseArgsParserRuleCall_4_0_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleUseArgs_in_ruleUseArg1370);
                    lv_args_4_0=ruleUseArgs();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getUseArgRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		add(
                    	       			current, 
                    	       			"args",
                    	        		lv_args_4_0, 
                    	        		"UseArgs", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }

                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:817:2: ( KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) ) )*
                    loop16:
                    do {
                        int alt16=2;
                        int LA16_0 = input.LA(1);

                        if ( (LA16_0==KEYWORD_3) ) {
                            alt16=1;
                        }


                        switch (alt16) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:818:2: KEYWORD_3 ( (lv_args_6_0= ruleUseArgs ) )
                    	    {
                    	    match(input,KEYWORD_3,FOLLOW_KEYWORD_3_in_ruleUseArg1382); 

                    	            createLeafNode(grammarAccess.getUseArgAccess().getCommaKeyword_4_1_0(), null); 
                    	        
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:822:1: ( (lv_args_6_0= ruleUseArgs ) )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:823:1: (lv_args_6_0= ruleUseArgs )
                    	    {
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:823:1: (lv_args_6_0= ruleUseArgs )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:824:3: lv_args_6_0= ruleUseArgs
                    	    {
                    	     
                    	    	        currentNode=createCompositeNode(grammarAccess.getUseArgAccess().getArgsUseArgsParserRuleCall_4_1_1_0(), currentNode); 
                    	    	    
                    	    pushFollow(FOLLOW_ruleUseArgs_in_ruleUseArg1403);
                    	    lv_args_6_0=ruleUseArgs();
                    	    _fsp--;


                    	    	        if (current==null) {
                    	    	            current = factory.create(grammarAccess.getUseArgRule().getType().getClassifier());
                    	    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	    	        }
                    	    	        try {
                    	    	       		add(
                    	    	       			current, 
                    	    	       			"args",
                    	    	        		lv_args_6_0, 
                    	    	        		"UseArgs", 
                    	    	        		currentNode);
                    	    	        } catch (ValueConverterException vce) {
                    	    				handleValueConverterException(vce);
                    	    	        }
                    	    	        currentNode = currentNode.getParent();
                    	    	    

                    	    }


                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop16;
                        }
                    } while (true);


                    }
                    break;

            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleUseArg


    // $ANTLR start entryRuleUseArgs
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:854:1: entryRuleUseArgs returns [EObject current=null] : iv_ruleUseArgs= ruleUseArgs EOF ;
    public final EObject entryRuleUseArgs() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleUseArgs = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:855:2: (iv_ruleUseArgs= ruleUseArgs EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:856:2: iv_ruleUseArgs= ruleUseArgs EOF
            {
             currentNode = createCompositeNode(grammarAccess.getUseArgsRule(), currentNode); 
            pushFollow(FOLLOW_ruleUseArgs_in_entryRuleUseArgs1442);
            iv_ruleUseArgs=ruleUseArgs();
            _fsp--;

             current =iv_ruleUseArgs; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleUseArgs1452); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleUseArgs


    // $ANTLR start ruleUseArgs
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:863:1: ruleUseArgs returns [EObject current=null] : ( ( (lv_name_0_0= ruleName ) ) ( KEYWORD_6 ( (lv_default_2_0= ruleExpression ) ) )? ) ;
    public final EObject ruleUseArgs() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_0_0 = null;

        EObject lv_default_2_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:868:6: ( ( ( (lv_name_0_0= ruleName ) ) ( KEYWORD_6 ( (lv_default_2_0= ruleExpression ) ) )? ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:869:1: ( ( (lv_name_0_0= ruleName ) ) ( KEYWORD_6 ( (lv_default_2_0= ruleExpression ) ) )? )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:869:1: ( ( (lv_name_0_0= ruleName ) ) ( KEYWORD_6 ( (lv_default_2_0= ruleExpression ) ) )? )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:869:2: ( (lv_name_0_0= ruleName ) ) ( KEYWORD_6 ( (lv_default_2_0= ruleExpression ) ) )?
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:869:2: ( (lv_name_0_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:870:1: (lv_name_0_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:870:1: (lv_name_0_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:871:3: lv_name_0_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getUseArgsAccess().getNameNameParserRuleCall_0_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleUseArgs1498);
            lv_name_0_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getUseArgsRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_0_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:893:2: ( KEYWORD_6 ( (lv_default_2_0= ruleExpression ) ) )?
            int alt18=2;
            int LA18_0 = input.LA(1);

            if ( (LA18_0==KEYWORD_6) ) {
                alt18=1;
            }
            switch (alt18) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:894:2: KEYWORD_6 ( (lv_default_2_0= ruleExpression ) )
                    {
                    match(input,KEYWORD_6,FOLLOW_KEYWORD_6_in_ruleUseArgs1510); 

                            createLeafNode(grammarAccess.getUseArgsAccess().getEqualsSignKeyword_1_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:898:1: ( (lv_default_2_0= ruleExpression ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:899:1: (lv_default_2_0= ruleExpression )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:899:1: (lv_default_2_0= ruleExpression )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:900:3: lv_default_2_0= ruleExpression
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getUseArgsAccess().getDefaultExpressionParserRuleCall_1_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleExpression_in_ruleUseArgs1531);
                    lv_default_2_0=ruleExpression();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getUseArgsRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"default",
                    	        		lv_default_2_0, 
                    	        		"Expression", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }


                    }
                    break;

            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleUseArgs


    // $ANTLR start entryRuleArguments
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:930:1: entryRuleArguments returns [EObject current=null] : iv_ruleArguments= ruleArguments EOF ;
    public final EObject entryRuleArguments() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleArguments = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:931:2: (iv_ruleArguments= ruleArguments EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:932:2: iv_ruleArguments= ruleArguments EOF
            {
             currentNode = createCompositeNode(grammarAccess.getArgumentsRule(), currentNode); 
            pushFollow(FOLLOW_ruleArguments_in_entryRuleArguments1568);
            iv_ruleArguments=ruleArguments();
            _fsp--;

             current =iv_ruleArguments; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleArguments1578); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleArguments


    // $ANTLR start ruleArguments
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:939:1: ruleArguments returns [EObject current=null] : ( () ( ( (lv_arguments_1_0= ruleExpression ) ) ( KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) ) )* )? ) ;
    public final EObject ruleArguments() throws RecognitionException {
        EObject current = null;

        EObject lv_arguments_1_0 = null;

        EObject lv_arguments_3_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:944:6: ( ( () ( ( (lv_arguments_1_0= ruleExpression ) ) ( KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) ) )* )? ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:945:1: ( () ( ( (lv_arguments_1_0= ruleExpression ) ) ( KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) ) )* )? )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:945:1: ( () ( ( (lv_arguments_1_0= ruleExpression ) ) ( KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) ) )* )? )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:945:2: () ( ( (lv_arguments_1_0= ruleExpression ) ) ( KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) ) )* )?
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:945:2: ()
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:946:5: 
            {
             
                    temp=factory.create(grammarAccess.getArgumentsAccess().getArgumentsAction_0().getType().getClassifier());
                    current = temp; 
                    temp = null;
                    CompositeNode newNode = createCompositeNode(grammarAccess.getArgumentsAccess().getArgumentsAction_0(), currentNode.getParent());
                newNode.getChildren().add(currentNode);
                moveLookaheadInfo(currentNode, newNode);
                currentNode = newNode; 
                    associateNodeWithAstElement(currentNode, current); 
                

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:956:2: ( ( (lv_arguments_1_0= ruleExpression ) ) ( KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) ) )* )?
            int alt20=2;
            int LA20_0 = input.LA(1);

            if ( (LA20_0==KEYWORD_8||(LA20_0>=RULE_STRING && LA20_0<=RULE_SYMBOL_VAR)||LA20_0==RULE_SYMBOL_NUM) ) {
                alt20=1;
            }
            switch (alt20) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:956:3: ( (lv_arguments_1_0= ruleExpression ) ) ( KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) ) )*
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:956:3: ( (lv_arguments_1_0= ruleExpression ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:957:1: (lv_arguments_1_0= ruleExpression )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:957:1: (lv_arguments_1_0= ruleExpression )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:958:3: lv_arguments_1_0= ruleExpression
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getArgumentsAccess().getArgumentsExpressionParserRuleCall_1_0_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleExpression_in_ruleArguments1634);
                    lv_arguments_1_0=ruleExpression();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getArgumentsRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		add(
                    	       			current, 
                    	       			"arguments",
                    	        		lv_arguments_1_0, 
                    	        		"Expression", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }

                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:980:2: ( KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) ) )*
                    loop19:
                    do {
                        int alt19=2;
                        int LA19_0 = input.LA(1);

                        if ( (LA19_0==KEYWORD_3) ) {
                            alt19=1;
                        }


                        switch (alt19) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:981:2: KEYWORD_3 ( (lv_arguments_3_0= ruleExpression ) )
                    	    {
                    	    match(input,KEYWORD_3,FOLLOW_KEYWORD_3_in_ruleArguments1646); 

                    	            createLeafNode(grammarAccess.getArgumentsAccess().getCommaKeyword_1_1_0(), null); 
                    	        
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:985:1: ( (lv_arguments_3_0= ruleExpression ) )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:986:1: (lv_arguments_3_0= ruleExpression )
                    	    {
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:986:1: (lv_arguments_3_0= ruleExpression )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:987:3: lv_arguments_3_0= ruleExpression
                    	    {
                    	     
                    	    	        currentNode=createCompositeNode(grammarAccess.getArgumentsAccess().getArgumentsExpressionParserRuleCall_1_1_1_0(), currentNode); 
                    	    	    
                    	    pushFollow(FOLLOW_ruleExpression_in_ruleArguments1667);
                    	    lv_arguments_3_0=ruleExpression();
                    	    _fsp--;


                    	    	        if (current==null) {
                    	    	            current = factory.create(grammarAccess.getArgumentsRule().getType().getClassifier());
                    	    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	    	        }
                    	    	        try {
                    	    	       		add(
                    	    	       			current, 
                    	    	       			"arguments",
                    	    	        		lv_arguments_3_0, 
                    	    	        		"Expression", 
                    	    	        		currentNode);
                    	    	        } catch (ValueConverterException vce) {
                    	    				handleValueConverterException(vce);
                    	    	        }
                    	    	        currentNode = currentNode.getParent();
                    	    	    

                    	    }


                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop19;
                        }
                    } while (true);


                    }
                    break;

            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleArguments


    // $ANTLR start entryRuleRaise
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1017:1: entryRuleRaise returns [EObject current=null] : iv_ruleRaise= ruleRaise EOF ;
    public final EObject entryRuleRaise() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleRaise = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1018:2: (iv_ruleRaise= ruleRaise EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1019:2: iv_ruleRaise= ruleRaise EOF
            {
             currentNode = createCompositeNode(grammarAccess.getRaiseRule(), currentNode); 
            pushFollow(FOLLOW_ruleRaise_in_entryRuleRaise1706);
            iv_ruleRaise=ruleRaise();
            _fsp--;

             current =iv_ruleRaise; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleRaise1716); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleRaise


    // $ANTLR start ruleRaise
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1026:1: ruleRaise returns [EObject current=null] : ( KEYWORD_22 KEYWORD_26 ( (lv_mayor_2_0= RULE_SYMBOL_NUM ) ) ( KEYWORD_4 ( (lv_minor_4_0= RULE_SYMBOL_NUM ) ) ) ) ;
    public final EObject ruleRaise() throws RecognitionException {
        EObject current = null;

        Token lv_mayor_2_0=null;
        Token lv_minor_4_0=null;

         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1031:6: ( ( KEYWORD_22 KEYWORD_26 ( (lv_mayor_2_0= RULE_SYMBOL_NUM ) ) ( KEYWORD_4 ( (lv_minor_4_0= RULE_SYMBOL_NUM ) ) ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1032:1: ( KEYWORD_22 KEYWORD_26 ( (lv_mayor_2_0= RULE_SYMBOL_NUM ) ) ( KEYWORD_4 ( (lv_minor_4_0= RULE_SYMBOL_NUM ) ) ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1032:1: ( KEYWORD_22 KEYWORD_26 ( (lv_mayor_2_0= RULE_SYMBOL_NUM ) ) ( KEYWORD_4 ( (lv_minor_4_0= RULE_SYMBOL_NUM ) ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1033:2: KEYWORD_22 KEYWORD_26 ( (lv_mayor_2_0= RULE_SYMBOL_NUM ) ) ( KEYWORD_4 ( (lv_minor_4_0= RULE_SYMBOL_NUM ) ) )
            {
            match(input,KEYWORD_22,FOLLOW_KEYWORD_22_in_ruleRaise1752); 

                    createLeafNode(grammarAccess.getRaiseAccess().getRaiseKeyword_0(), null); 
                
            match(input,KEYWORD_26,FOLLOW_KEYWORD_26_in_ruleRaise1763); 

                    createLeafNode(grammarAccess.getRaiseAccess().getSyntaxKeyword_1(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1042:1: ( (lv_mayor_2_0= RULE_SYMBOL_NUM ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1043:1: (lv_mayor_2_0= RULE_SYMBOL_NUM )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1043:1: (lv_mayor_2_0= RULE_SYMBOL_NUM )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1044:3: lv_mayor_2_0= RULE_SYMBOL_NUM
            {
            lv_mayor_2_0=(Token)input.LT(1);
            match(input,RULE_SYMBOL_NUM,FOLLOW_RULE_SYMBOL_NUM_in_ruleRaise1780); 

            			createLeafNode(grammarAccess.getRaiseAccess().getMayorSYMBOL_NUMTerminalRuleCall_2_0(), "mayor"); 
            		

            	        if (current==null) {
            	            current = factory.create(grammarAccess.getRaiseRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode, current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"mayor",
            	        		lv_mayor_2_0, 
            	        		"SYMBOL_NUM", 
            	        		lastConsumedNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1066:2: ( KEYWORD_4 ( (lv_minor_4_0= RULE_SYMBOL_NUM ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1067:2: KEYWORD_4 ( (lv_minor_4_0= RULE_SYMBOL_NUM ) )
            {
            match(input,KEYWORD_4,FOLLOW_KEYWORD_4_in_ruleRaise1797); 

                    createLeafNode(grammarAccess.getRaiseAccess().getFullStopKeyword_3_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1071:1: ( (lv_minor_4_0= RULE_SYMBOL_NUM ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1072:1: (lv_minor_4_0= RULE_SYMBOL_NUM )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1072:1: (lv_minor_4_0= RULE_SYMBOL_NUM )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1073:3: lv_minor_4_0= RULE_SYMBOL_NUM
            {
            lv_minor_4_0=(Token)input.LT(1);
            match(input,RULE_SYMBOL_NUM,FOLLOW_RULE_SYMBOL_NUM_in_ruleRaise1814); 

            			createLeafNode(grammarAccess.getRaiseAccess().getMinorSYMBOL_NUMTerminalRuleCall_3_1_0(), "minor"); 
            		

            	        if (current==null) {
            	            current = factory.create(grammarAccess.getRaiseRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode, current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"minor",
            	        		lv_minor_4_0, 
            	        		"SYMBOL_NUM", 
            	        		lastConsumedNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	    

            }


            }


            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleRaise


    // $ANTLR start entryRuleExpression
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1103:1: entryRuleExpression returns [EObject current=null] : iv_ruleExpression= ruleExpression EOF ;
    public final EObject entryRuleExpression() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleExpression = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1104:2: (iv_ruleExpression= ruleExpression EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1105:2: iv_ruleExpression= ruleExpression EOF
            {
             currentNode = createCompositeNode(grammarAccess.getExpressionRule(), currentNode); 
            pushFollow(FOLLOW_ruleExpression_in_entryRuleExpression1855);
            iv_ruleExpression=ruleExpression();
            _fsp--;

             current =iv_ruleExpression; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleExpression1865); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleExpression


    // $ANTLR start ruleExpression
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1112:1: ruleExpression returns [EObject current=null] : ( ( () ( (lv_name_1_0= ruleName ) ) ) | this_fnCall_2= rulefnCall | this_Message_3= ruleMessage | this_ArrayAccess_4= ruleArrayAccess | this_Negate_5= ruleNegate | this_Const_6= ruleConst ) ;
    public final EObject ruleExpression() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;

        EObject this_fnCall_2 = null;

        EObject this_Message_3 = null;

        EObject this_ArrayAccess_4 = null;

        EObject this_Negate_5 = null;

        EObject this_Const_6 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1117:6: ( ( ( () ( (lv_name_1_0= ruleName ) ) ) | this_fnCall_2= rulefnCall | this_Message_3= ruleMessage | this_ArrayAccess_4= ruleArrayAccess | this_Negate_5= ruleNegate | this_Const_6= ruleConst ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1118:1: ( ( () ( (lv_name_1_0= ruleName ) ) ) | this_fnCall_2= rulefnCall | this_Message_3= ruleMessage | this_ArrayAccess_4= ruleArrayAccess | this_Negate_5= ruleNegate | this_Const_6= ruleConst )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1118:1: ( ( () ( (lv_name_1_0= ruleName ) ) ) | this_fnCall_2= rulefnCall | this_Message_3= ruleMessage | this_ArrayAccess_4= ruleArrayAccess | this_Negate_5= ruleNegate | this_Const_6= ruleConst )
            int alt21=6;
            switch ( input.LA(1) ) {
            case RULE_STRING:
                {
                switch ( input.LA(2) ) {
                case KEYWORD_1:
                    {
                    alt21=2;
                    }
                    break;
                case KEYWORD_7:
                    {
                    alt21=4;
                    }
                    break;
                case EOF:
                case KEYWORD_20:
                case KEYWORD_2:
                case KEYWORD_3:
                case KEYWORD_9:
                case RULE_TERM:
                    {
                    alt21=1;
                    }
                    break;
                case KEYWORD_10:
                    {
                    alt21=3;
                    }
                    break;
                default:
                    NoViableAltException nvae =
                        new NoViableAltException("1118:1: ( ( () ( (lv_name_1_0= ruleName ) ) ) | this_fnCall_2= rulefnCall | this_Message_3= ruleMessage | this_ArrayAccess_4= ruleArrayAccess | this_Negate_5= ruleNegate | this_Const_6= ruleConst )", 21, 1, input);

                    throw nvae;
                }

                }
                break;
            case RULE_SYMBOL_VAR:
                {
                switch ( input.LA(2) ) {
                case KEYWORD_1:
                    {
                    alt21=2;
                    }
                    break;
                case KEYWORD_7:
                    {
                    alt21=4;
                    }
                    break;
                case EOF:
                case KEYWORD_20:
                case KEYWORD_2:
                case KEYWORD_3:
                case KEYWORD_9:
                case RULE_TERM:
                    {
                    alt21=1;
                    }
                    break;
                case KEYWORD_10:
                    {
                    alt21=3;
                    }
                    break;
                default:
                    NoViableAltException nvae =
                        new NoViableAltException("1118:1: ( ( () ( (lv_name_1_0= ruleName ) ) ) | this_fnCall_2= rulefnCall | this_Message_3= ruleMessage | this_ArrayAccess_4= ruleArrayAccess | this_Negate_5= ruleNegate | this_Const_6= ruleConst )", 21, 2, input);

                    throw nvae;
                }

                }
                break;
            case KEYWORD_8:
                {
                alt21=5;
                }
                break;
            case RULE_STRING_HEX:
            case RULE_STRING_BOOL:
            case RULE_SYMBOL_NUM:
                {
                alt21=6;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("1118:1: ( ( () ( (lv_name_1_0= ruleName ) ) ) | this_fnCall_2= rulefnCall | this_Message_3= ruleMessage | this_ArrayAccess_4= ruleArrayAccess | this_Negate_5= ruleNegate | this_Const_6= ruleConst )", 21, 0, input);

                throw nvae;
            }

            switch (alt21) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1118:2: ( () ( (lv_name_1_0= ruleName ) ) )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1118:2: ( () ( (lv_name_1_0= ruleName ) ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1118:3: () ( (lv_name_1_0= ruleName ) )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1118:3: ()
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1119:5: 
                    {
                     
                            temp=factory.create(grammarAccess.getExpressionAccess().getExprVarAction_0_0().getType().getClassifier());
                            current = temp; 
                            temp = null;
                            CompositeNode newNode = createCompositeNode(grammarAccess.getExpressionAccess().getExprVarAction_0_0(), currentNode.getParent());
                        newNode.getChildren().add(currentNode);
                        moveLookaheadInfo(currentNode, newNode);
                        currentNode = newNode; 
                            associateNodeWithAstElement(currentNode, current); 
                        

                    }

                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1129:2: ( (lv_name_1_0= ruleName ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1130:1: (lv_name_1_0= ruleName )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1130:1: (lv_name_1_0= ruleName )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1131:3: lv_name_1_0= ruleName
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getExpressionAccess().getNameNameParserRuleCall_0_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleName_in_ruleExpression1921);
                    lv_name_1_0=ruleName();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getExpressionRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"name",
                    	        		lv_name_1_0, 
                    	        		"Name", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }


                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1155:5: this_fnCall_2= rulefnCall
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getExpressionAccess().getFnCallParserRuleCall_1(), currentNode); 
                        
                    pushFollow(FOLLOW_rulefnCall_in_ruleExpression1950);
                    this_fnCall_2=rulefnCall();
                    _fsp--;

                     
                            current = this_fnCall_2; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1165:5: this_Message_3= ruleMessage
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getExpressionAccess().getMessageParserRuleCall_2(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleMessage_in_ruleExpression1977);
                    this_Message_3=ruleMessage();
                    _fsp--;

                     
                            current = this_Message_3; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 4 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1175:5: this_ArrayAccess_4= ruleArrayAccess
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getExpressionAccess().getArrayAccessParserRuleCall_3(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleArrayAccess_in_ruleExpression2004);
                    this_ArrayAccess_4=ruleArrayAccess();
                    _fsp--;

                     
                            current = this_ArrayAccess_4; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 5 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1185:5: this_Negate_5= ruleNegate
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getExpressionAccess().getNegateParserRuleCall_4(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleNegate_in_ruleExpression2031);
                    this_Negate_5=ruleNegate();
                    _fsp--;

                     
                            current = this_Negate_5; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 6 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1195:5: this_Const_6= ruleConst
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getExpressionAccess().getConstParserRuleCall_5(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleConst_in_ruleExpression2058);
                    this_Const_6=ruleConst();
                    _fsp--;

                     
                            current = this_Const_6; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleExpression


    // $ANTLR start entryRuleArrayAccess
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1211:1: entryRuleArrayAccess returns [EObject current=null] : iv_ruleArrayAccess= ruleArrayAccess EOF ;
    public final EObject entryRuleArrayAccess() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleArrayAccess = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1212:2: (iv_ruleArrayAccess= ruleArrayAccess EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1213:2: iv_ruleArrayAccess= ruleArrayAccess EOF
            {
             currentNode = createCompositeNode(grammarAccess.getArrayAccessRule(), currentNode); 
            pushFollow(FOLLOW_ruleArrayAccess_in_entryRuleArrayAccess2092);
            iv_ruleArrayAccess=ruleArrayAccess();
            _fsp--;

             current =iv_ruleArrayAccess; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleArrayAccess2102); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleArrayAccess


    // $ANTLR start ruleArrayAccess
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1220:1: ruleArrayAccess returns [EObject current=null] : ( ( (lv_target_0_0= ruleName ) ) KEYWORD_7 ( (lv_index_2_0= ruleExpression ) ) KEYWORD_9 ) ;
    public final EObject ruleArrayAccess() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_target_0_0 = null;

        EObject lv_index_2_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1225:6: ( ( ( (lv_target_0_0= ruleName ) ) KEYWORD_7 ( (lv_index_2_0= ruleExpression ) ) KEYWORD_9 ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1226:1: ( ( (lv_target_0_0= ruleName ) ) KEYWORD_7 ( (lv_index_2_0= ruleExpression ) ) KEYWORD_9 )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1226:1: ( ( (lv_target_0_0= ruleName ) ) KEYWORD_7 ( (lv_index_2_0= ruleExpression ) ) KEYWORD_9 )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1226:2: ( (lv_target_0_0= ruleName ) ) KEYWORD_7 ( (lv_index_2_0= ruleExpression ) ) KEYWORD_9
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1226:2: ( (lv_target_0_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1227:1: (lv_target_0_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1227:1: (lv_target_0_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1228:3: lv_target_0_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getArrayAccessAccess().getTargetNameParserRuleCall_0_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleArrayAccess2148);
            lv_target_0_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getArrayAccessRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"target",
            	        		lv_target_0_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_7,FOLLOW_KEYWORD_7_in_ruleArrayAccess2159); 

                    createLeafNode(grammarAccess.getArrayAccessAccess().getLeftSquareBracketKeyword_1(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1255:1: ( (lv_index_2_0= ruleExpression ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1256:1: (lv_index_2_0= ruleExpression )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1256:1: (lv_index_2_0= ruleExpression )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1257:3: lv_index_2_0= ruleExpression
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getArrayAccessAccess().getIndexExpressionParserRuleCall_2_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleExpression_in_ruleArrayAccess2180);
            lv_index_2_0=ruleExpression();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getArrayAccessRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"index",
            	        		lv_index_2_0, 
            	        		"Expression", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_9,FOLLOW_KEYWORD_9_in_ruleArrayAccess2191); 

                    createLeafNode(grammarAccess.getArrayAccessAccess().getRightSquareBracketKeyword_3(), null); 
                

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleArrayAccess


    // $ANTLR start entryRulefnCall
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1292:1: entryRulefnCall returns [EObject current=null] : iv_rulefnCall= rulefnCall EOF ;
    public final EObject entryRulefnCall() throws RecognitionException {
        EObject current = null;

        EObject iv_rulefnCall = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1293:2: (iv_rulefnCall= rulefnCall EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1294:2: iv_rulefnCall= rulefnCall EOF
            {
             currentNode = createCompositeNode(grammarAccess.getFnCallRule(), currentNode); 
            pushFollow(FOLLOW_rulefnCall_in_entryRulefnCall2226);
            iv_rulefnCall=rulefnCall();
            _fsp--;

             current =iv_rulefnCall; 
            match(input,EOF,FOLLOW_EOF_in_entryRulefnCall2236); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRulefnCall


    // $ANTLR start rulefnCall
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1301:1: rulefnCall returns [EObject current=null] : ( ( (lv_target_0_0= ruleName ) ) KEYWORD_1 ( (lv_args_2_0= ruleArguments ) ) KEYWORD_2 ) ;
    public final EObject rulefnCall() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_target_0_0 = null;

        EObject lv_args_2_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1306:6: ( ( ( (lv_target_0_0= ruleName ) ) KEYWORD_1 ( (lv_args_2_0= ruleArguments ) ) KEYWORD_2 ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1307:1: ( ( (lv_target_0_0= ruleName ) ) KEYWORD_1 ( (lv_args_2_0= ruleArguments ) ) KEYWORD_2 )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1307:1: ( ( (lv_target_0_0= ruleName ) ) KEYWORD_1 ( (lv_args_2_0= ruleArguments ) ) KEYWORD_2 )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1307:2: ( (lv_target_0_0= ruleName ) ) KEYWORD_1 ( (lv_args_2_0= ruleArguments ) ) KEYWORD_2
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1307:2: ( (lv_target_0_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1308:1: (lv_target_0_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1308:1: (lv_target_0_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1309:3: lv_target_0_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getFnCallAccess().getTargetNameParserRuleCall_0_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_rulefnCall2282);
            lv_target_0_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getFnCallRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"target",
            	        		lv_target_0_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_1,FOLLOW_KEYWORD_1_in_rulefnCall2293); 

                    createLeafNode(grammarAccess.getFnCallAccess().getLeftParenthesisKeyword_1(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1336:1: ( (lv_args_2_0= ruleArguments ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1337:1: (lv_args_2_0= ruleArguments )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1337:1: (lv_args_2_0= ruleArguments )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1338:3: lv_args_2_0= ruleArguments
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getFnCallAccess().getArgsArgumentsParserRuleCall_2_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleArguments_in_rulefnCall2314);
            lv_args_2_0=ruleArguments();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getFnCallRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"args",
            	        		lv_args_2_0, 
            	        		"Arguments", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_2,FOLLOW_KEYWORD_2_in_rulefnCall2325); 

                    createLeafNode(grammarAccess.getFnCallAccess().getRightParenthesisKeyword_3(), null); 
                

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end rulefnCall


    // $ANTLR start entryRuleMessage
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1373:1: entryRuleMessage returns [EObject current=null] : iv_ruleMessage= ruleMessage EOF ;
    public final EObject entryRuleMessage() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleMessage = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1374:2: (iv_ruleMessage= ruleMessage EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1375:2: iv_ruleMessage= ruleMessage EOF
            {
             currentNode = createCompositeNode(grammarAccess.getMessageRule(), currentNode); 
            pushFollow(FOLLOW_ruleMessage_in_entryRuleMessage2360);
            iv_ruleMessage=ruleMessage();
            _fsp--;

             current =iv_ruleMessage; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleMessage2370); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleMessage


    // $ANTLR start ruleMessage
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1382:1: ruleMessage returns [EObject current=null] : ( ( (lv_target_0_0= ruleName ) ) KEYWORD_10 ( (lv_discard_2_0= KEYWORD_10 ) )? ( (lv_message_3_0= ruleName ) ) ( KEYWORD_5 ( (lv_scope_5_0= ruleName ) ) )? ( ( KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2 ) | ( KEYWORD_6 ( (lv_value_10_0= ruleExpression ) ) ) )? ) ;
    public final EObject ruleMessage() throws RecognitionException {
        EObject current = null;

        Token lv_discard_2_0=null;
        AntlrDatatypeRuleToken lv_target_0_0 = null;

        AntlrDatatypeRuleToken lv_message_3_0 = null;

        AntlrDatatypeRuleToken lv_scope_5_0 = null;

        EObject lv_args_7_0 = null;

        EObject lv_value_10_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1387:6: ( ( ( (lv_target_0_0= ruleName ) ) KEYWORD_10 ( (lv_discard_2_0= KEYWORD_10 ) )? ( (lv_message_3_0= ruleName ) ) ( KEYWORD_5 ( (lv_scope_5_0= ruleName ) ) )? ( ( KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2 ) | ( KEYWORD_6 ( (lv_value_10_0= ruleExpression ) ) ) )? ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1388:1: ( ( (lv_target_0_0= ruleName ) ) KEYWORD_10 ( (lv_discard_2_0= KEYWORD_10 ) )? ( (lv_message_3_0= ruleName ) ) ( KEYWORD_5 ( (lv_scope_5_0= ruleName ) ) )? ( ( KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2 ) | ( KEYWORD_6 ( (lv_value_10_0= ruleExpression ) ) ) )? )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1388:1: ( ( (lv_target_0_0= ruleName ) ) KEYWORD_10 ( (lv_discard_2_0= KEYWORD_10 ) )? ( (lv_message_3_0= ruleName ) ) ( KEYWORD_5 ( (lv_scope_5_0= ruleName ) ) )? ( ( KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2 ) | ( KEYWORD_6 ( (lv_value_10_0= ruleExpression ) ) ) )? )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1388:2: ( (lv_target_0_0= ruleName ) ) KEYWORD_10 ( (lv_discard_2_0= KEYWORD_10 ) )? ( (lv_message_3_0= ruleName ) ) ( KEYWORD_5 ( (lv_scope_5_0= ruleName ) ) )? ( ( KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2 ) | ( KEYWORD_6 ( (lv_value_10_0= ruleExpression ) ) ) )?
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1388:2: ( (lv_target_0_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1389:1: (lv_target_0_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1389:1: (lv_target_0_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1390:3: lv_target_0_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getMessageAccess().getTargetNameParserRuleCall_0_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleMessage2416);
            lv_target_0_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getMessageRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"target",
            	        		lv_target_0_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_10,FOLLOW_KEYWORD_10_in_ruleMessage2427); 

                    createLeafNode(grammarAccess.getMessageAccess().getTildeKeyword_1(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1417:1: ( (lv_discard_2_0= KEYWORD_10 ) )?
            int alt22=2;
            int LA22_0 = input.LA(1);

            if ( (LA22_0==KEYWORD_10) ) {
                alt22=1;
            }
            switch (alt22) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1418:1: (lv_discard_2_0= KEYWORD_10 )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1418:1: (lv_discard_2_0= KEYWORD_10 )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1419:3: lv_discard_2_0= KEYWORD_10
                    {
                    lv_discard_2_0=(Token)input.LT(1);
                    match(input,KEYWORD_10,FOLLOW_KEYWORD_10_in_ruleMessage2446); 

                            createLeafNode(grammarAccess.getMessageAccess().getDiscardTildeKeyword_2_0(), "discard"); 
                        

                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getMessageRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode, current);
                    	        }
                    	        
                    	        try {
                    	       		set(current, "discard", true, "~", lastConsumedNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	    

                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1439:3: ( (lv_message_3_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1440:1: (lv_message_3_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1440:1: (lv_message_3_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1441:3: lv_message_3_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getMessageAccess().getMessageNameParserRuleCall_3_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleMessage2481);
            lv_message_3_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getMessageRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"message",
            	        		lv_message_3_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1463:2: ( KEYWORD_5 ( (lv_scope_5_0= ruleName ) ) )?
            int alt23=2;
            int LA23_0 = input.LA(1);

            if ( (LA23_0==KEYWORD_5) ) {
                alt23=1;
            }
            switch (alt23) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1464:2: KEYWORD_5 ( (lv_scope_5_0= ruleName ) )
                    {
                    match(input,KEYWORD_5,FOLLOW_KEYWORD_5_in_ruleMessage2493); 

                            createLeafNode(grammarAccess.getMessageAccess().getColonKeyword_4_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1468:1: ( (lv_scope_5_0= ruleName ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1469:1: (lv_scope_5_0= ruleName )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1469:1: (lv_scope_5_0= ruleName )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1470:3: lv_scope_5_0= ruleName
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getMessageAccess().getScopeNameParserRuleCall_4_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleName_in_ruleMessage2514);
                    lv_scope_5_0=ruleName();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getMessageRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"scope",
                    	        		lv_scope_5_0, 
                    	        		"Name", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1492:4: ( ( KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2 ) | ( KEYWORD_6 ( (lv_value_10_0= ruleExpression ) ) ) )?
            int alt24=3;
            int LA24_0 = input.LA(1);

            if ( (LA24_0==KEYWORD_1) ) {
                alt24=1;
            }
            else if ( (LA24_0==KEYWORD_6) ) {
                alt24=2;
            }
            switch (alt24) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1492:5: ( KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2 )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1492:5: ( KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2 )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1493:2: KEYWORD_1 ( (lv_args_7_0= ruleArguments ) ) KEYWORD_2
                    {
                    match(input,KEYWORD_1,FOLLOW_KEYWORD_1_in_ruleMessage2529); 

                            createLeafNode(grammarAccess.getMessageAccess().getLeftParenthesisKeyword_5_0_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1497:1: ( (lv_args_7_0= ruleArguments ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1498:1: (lv_args_7_0= ruleArguments )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1498:1: (lv_args_7_0= ruleArguments )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1499:3: lv_args_7_0= ruleArguments
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getMessageAccess().getArgsArgumentsParserRuleCall_5_0_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleArguments_in_ruleMessage2550);
                    lv_args_7_0=ruleArguments();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getMessageRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"args",
                    	        		lv_args_7_0, 
                    	        		"Arguments", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }

                    match(input,KEYWORD_2,FOLLOW_KEYWORD_2_in_ruleMessage2561); 

                            createLeafNode(grammarAccess.getMessageAccess().getRightParenthesisKeyword_5_0_2(), null); 
                        

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1527:6: ( KEYWORD_6 ( (lv_value_10_0= ruleExpression ) ) )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1527:6: ( KEYWORD_6 ( (lv_value_10_0= ruleExpression ) ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1528:2: KEYWORD_6 ( (lv_value_10_0= ruleExpression ) )
                    {
                    match(input,KEYWORD_6,FOLLOW_KEYWORD_6_in_ruleMessage2580); 

                            createLeafNode(grammarAccess.getMessageAccess().getEqualsSignKeyword_5_1_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1532:1: ( (lv_value_10_0= ruleExpression ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1533:1: (lv_value_10_0= ruleExpression )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1533:1: (lv_value_10_0= ruleExpression )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1534:3: lv_value_10_0= ruleExpression
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getMessageAccess().getValueExpressionParserRuleCall_5_1_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleExpression_in_ruleMessage2601);
                    lv_value_10_0=ruleExpression();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getMessageRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"value",
                    	        		lv_value_10_0, 
                    	        		"Expression", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }


                    }


                    }
                    break;

            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleMessage


    // $ANTLR start entryRuleNegate
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1564:1: entryRuleNegate returns [EObject current=null] : iv_ruleNegate= ruleNegate EOF ;
    public final EObject entryRuleNegate() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleNegate = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1565:2: (iv_ruleNegate= ruleNegate EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1566:2: iv_ruleNegate= ruleNegate EOF
            {
             currentNode = createCompositeNode(grammarAccess.getNegateRule(), currentNode); 
            pushFollow(FOLLOW_ruleNegate_in_entryRuleNegate2639);
            iv_ruleNegate=ruleNegate();
            _fsp--;

             current =iv_ruleNegate; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleNegate2649); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleNegate


    // $ANTLR start ruleNegate
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1573:1: ruleNegate returns [EObject current=null] : ( KEYWORD_8 ( (lv_expression_1_0= ruleExpression ) ) ) ;
    public final EObject ruleNegate() throws RecognitionException {
        EObject current = null;

        EObject lv_expression_1_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1578:6: ( ( KEYWORD_8 ( (lv_expression_1_0= ruleExpression ) ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1579:1: ( KEYWORD_8 ( (lv_expression_1_0= ruleExpression ) ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1579:1: ( KEYWORD_8 ( (lv_expression_1_0= ruleExpression ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1580:2: KEYWORD_8 ( (lv_expression_1_0= ruleExpression ) )
            {
            match(input,KEYWORD_8,FOLLOW_KEYWORD_8_in_ruleNegate2685); 

                    createLeafNode(grammarAccess.getNegateAccess().getReverseSolidusKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1584:1: ( (lv_expression_1_0= ruleExpression ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1585:1: (lv_expression_1_0= ruleExpression )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1585:1: (lv_expression_1_0= ruleExpression )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1586:3: lv_expression_1_0= ruleExpression
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getNegateAccess().getExpressionExpressionParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleExpression_in_ruleNegate2706);
            lv_expression_1_0=ruleExpression();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getNegateRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"expression",
            	        		lv_expression_1_0, 
            	        		"Expression", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleNegate


    // $ANTLR start entryRuleConst
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1616:1: entryRuleConst returns [EObject current=null] : iv_ruleConst= ruleConst EOF ;
    public final EObject entryRuleConst() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleConst = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1617:2: (iv_ruleConst= ruleConst EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1618:2: iv_ruleConst= ruleConst EOF
            {
             currentNode = createCompositeNode(grammarAccess.getConstRule(), currentNode); 
            pushFollow(FOLLOW_ruleConst_in_entryRuleConst2741);
            iv_ruleConst=ruleConst();
            _fsp--;

             current =iv_ruleConst; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleConst2751); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleConst


    // $ANTLR start ruleConst
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1625:1: ruleConst returns [EObject current=null] : ( () ( ( (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX ) ) ) ) ;
    public final EObject ruleConst() throws RecognitionException {
        EObject current = null;

        Token lv_value_1_1=null;
        Token lv_value_1_2=null;
        Token lv_value_1_3=null;

         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1630:6: ( ( () ( ( (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX ) ) ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1631:1: ( () ( ( (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX ) ) ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1631:1: ( () ( ( (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX ) ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1631:2: () ( ( (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX ) ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1631:2: ()
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1632:5: 
            {
             
                    temp=factory.create(grammarAccess.getConstAccess().getConstAction_0().getType().getClassifier());
                    current = temp; 
                    temp = null;
                    CompositeNode newNode = createCompositeNode(grammarAccess.getConstAccess().getConstAction_0(), currentNode.getParent());
                newNode.getChildren().add(currentNode);
                moveLookaheadInfo(currentNode, newNode);
                currentNode = newNode; 
                    associateNodeWithAstElement(currentNode, current); 
                

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1642:2: ( ( (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1643:1: ( (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1643:1: ( (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1644:1: (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1644:1: (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX )
            int alt25=3;
            switch ( input.LA(1) ) {
            case RULE_SYMBOL_NUM:
                {
                alt25=1;
                }
                break;
            case RULE_STRING_BOOL:
                {
                alt25=2;
                }
                break;
            case RULE_STRING_HEX:
                {
                alt25=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("1644:1: (lv_value_1_1= RULE_SYMBOL_NUM | lv_value_1_2= RULE_STRING_BOOL | lv_value_1_3= RULE_STRING_HEX )", 25, 0, input);

                throw nvae;
            }

            switch (alt25) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1645:3: lv_value_1_1= RULE_SYMBOL_NUM
                    {
                    lv_value_1_1=(Token)input.LT(1);
                    match(input,RULE_SYMBOL_NUM,FOLLOW_RULE_SYMBOL_NUM_in_ruleConst2804); 

                    			createLeafNode(grammarAccess.getConstAccess().getValueSYMBOL_NUMTerminalRuleCall_1_0_0(), "value"); 
                    		

                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getConstRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode, current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"value",
                    	        		lv_value_1_1, 
                    	        		"SYMBOL_NUM", 
                    	        		lastConsumedNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	    

                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1666:8: lv_value_1_2= RULE_STRING_BOOL
                    {
                    lv_value_1_2=(Token)input.LT(1);
                    match(input,RULE_STRING_BOOL,FOLLOW_RULE_STRING_BOOL_in_ruleConst2824); 

                    			createLeafNode(grammarAccess.getConstAccess().getValueSTRING_BOOLTerminalRuleCall_1_0_1(), "value"); 
                    		

                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getConstRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode, current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"value",
                    	        		lv_value_1_2, 
                    	        		"STRING_BOOL", 
                    	        		lastConsumedNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	    

                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1687:8: lv_value_1_3= RULE_STRING_HEX
                    {
                    lv_value_1_3=(Token)input.LT(1);
                    match(input,RULE_STRING_HEX,FOLLOW_RULE_STRING_HEX_in_ruleConst2844); 

                    			createLeafNode(grammarAccess.getConstAccess().getValueSTRING_HEXTerminalRuleCall_1_0_2(), "value"); 
                    		

                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getConstRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode, current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"value",
                    	        		lv_value_1_3, 
                    	        		"STRING_HEX", 
                    	        		lastConsumedNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	    

                    }
                    break;

            }


            }


            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleConst


    // $ANTLR start entryRuleDirectives
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1719:1: entryRuleDirectives returns [EObject current=null] : iv_ruleDirectives= ruleDirectives EOF ;
    public final EObject entryRuleDirectives() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleDirectives = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1720:2: (iv_ruleDirectives= ruleDirectives EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1721:2: iv_ruleDirectives= ruleDirectives EOF
            {
             currentNode = createCompositeNode(grammarAccess.getDirectivesRule(), currentNode); 
            pushFollow(FOLLOW_ruleDirectives_in_entryRuleDirectives2887);
            iv_ruleDirectives=ruleDirectives();
            _fsp--;

             current =iv_ruleDirectives; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleDirectives2897); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleDirectives


    // $ANTLR start ruleDirectives
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1728:1: ruleDirectives returns [EObject current=null] : (this_Class_0= ruleClass | this_Options_1= ruleOptions | this_Routine_2= ruleRoutine ) ;
    public final EObject ruleDirectives() throws RecognitionException {
        EObject current = null;

        EObject this_Class_0 = null;

        EObject this_Options_1 = null;

        EObject this_Routine_2 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1733:6: ( (this_Class_0= ruleClass | this_Options_1= ruleOptions | this_Routine_2= ruleRoutine ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1734:1: (this_Class_0= ruleClass | this_Options_1= ruleOptions | this_Routine_2= ruleRoutine )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1734:1: (this_Class_0= ruleClass | this_Options_1= ruleOptions | this_Routine_2= ruleRoutine )
            int alt26=3;
            switch ( input.LA(1) ) {
            case KEYWORD_27:
                {
                alt26=1;
                }
                break;
            case KEYWORD_32:
                {
                alt26=2;
                }
                break;
            case KEYWORD_33:
                {
                alt26=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("1734:1: (this_Class_0= ruleClass | this_Options_1= ruleOptions | this_Routine_2= ruleRoutine )", 26, 0, input);

                throw nvae;
            }

            switch (alt26) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1735:5: this_Class_0= ruleClass
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getDirectivesAccess().getClassParserRuleCall_0(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleClass_in_ruleDirectives2944);
                    this_Class_0=ruleClass();
                    _fsp--;

                     
                            current = this_Class_0; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1745:5: this_Options_1= ruleOptions
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getDirectivesAccess().getOptionsParserRuleCall_1(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleOptions_in_ruleDirectives2971);
                    this_Options_1=ruleOptions();
                    _fsp--;

                     
                            current = this_Options_1; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1755:5: this_Routine_2= ruleRoutine
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getDirectivesAccess().getRoutineParserRuleCall_2(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleRoutine_in_ruleDirectives2998);
                    this_Routine_2=ruleRoutine();
                    _fsp--;

                     
                            current = this_Routine_2; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleDirectives


    // $ANTLR start entryRuleClass
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1771:1: entryRuleClass returns [EObject current=null] : iv_ruleClass= ruleClass EOF ;
    public final EObject entryRuleClass() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleClass = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1772:2: (iv_ruleClass= ruleClass EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1773:2: iv_ruleClass= ruleClass EOF
            {
             currentNode = createCompositeNode(grammarAccess.getClassRule(), currentNode); 
            pushFollow(FOLLOW_ruleClass_in_entryRuleClass3032);
            iv_ruleClass=ruleClass();
            _fsp--;

             current =iv_ruleClass; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleClass3042); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleClass


    // $ANTLR start ruleClass
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1780:1: ruleClass returns [EObject current=null] : ( KEYWORD_27 ( (lv_name_1_0= ruleName ) ) ( KEYWORD_34 ( (lv_meta_3_0= ruleName ) ) )? ( ( KEYWORD_31 ( (lv_subclass_5_0= ruleName ) ) ) | ( KEYWORD_37 ( (lv_mixin_7_0= ruleName ) ) ) )? ( (lv_visibility_8_0= ruleVISIBILITY ) )? ( KEYWORD_28 ( (lv_inherit_10_0= ruleName ) )+ )? ( RULE_TERM )+ ( (lv_content_12_0= ruleClassContent ) )* ) ;
    public final EObject ruleClass() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;

        AntlrDatatypeRuleToken lv_meta_3_0 = null;

        AntlrDatatypeRuleToken lv_subclass_5_0 = null;

        AntlrDatatypeRuleToken lv_mixin_7_0 = null;

        Enumerator lv_visibility_8_0 = null;

        AntlrDatatypeRuleToken lv_inherit_10_0 = null;

        EObject lv_content_12_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1785:6: ( ( KEYWORD_27 ( (lv_name_1_0= ruleName ) ) ( KEYWORD_34 ( (lv_meta_3_0= ruleName ) ) )? ( ( KEYWORD_31 ( (lv_subclass_5_0= ruleName ) ) ) | ( KEYWORD_37 ( (lv_mixin_7_0= ruleName ) ) ) )? ( (lv_visibility_8_0= ruleVISIBILITY ) )? ( KEYWORD_28 ( (lv_inherit_10_0= ruleName ) )+ )? ( RULE_TERM )+ ( (lv_content_12_0= ruleClassContent ) )* ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1786:1: ( KEYWORD_27 ( (lv_name_1_0= ruleName ) ) ( KEYWORD_34 ( (lv_meta_3_0= ruleName ) ) )? ( ( KEYWORD_31 ( (lv_subclass_5_0= ruleName ) ) ) | ( KEYWORD_37 ( (lv_mixin_7_0= ruleName ) ) ) )? ( (lv_visibility_8_0= ruleVISIBILITY ) )? ( KEYWORD_28 ( (lv_inherit_10_0= ruleName ) )+ )? ( RULE_TERM )+ ( (lv_content_12_0= ruleClassContent ) )* )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1786:1: ( KEYWORD_27 ( (lv_name_1_0= ruleName ) ) ( KEYWORD_34 ( (lv_meta_3_0= ruleName ) ) )? ( ( KEYWORD_31 ( (lv_subclass_5_0= ruleName ) ) ) | ( KEYWORD_37 ( (lv_mixin_7_0= ruleName ) ) ) )? ( (lv_visibility_8_0= ruleVISIBILITY ) )? ( KEYWORD_28 ( (lv_inherit_10_0= ruleName ) )+ )? ( RULE_TERM )+ ( (lv_content_12_0= ruleClassContent ) )* )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1787:2: KEYWORD_27 ( (lv_name_1_0= ruleName ) ) ( KEYWORD_34 ( (lv_meta_3_0= ruleName ) ) )? ( ( KEYWORD_31 ( (lv_subclass_5_0= ruleName ) ) ) | ( KEYWORD_37 ( (lv_mixin_7_0= ruleName ) ) ) )? ( (lv_visibility_8_0= ruleVISIBILITY ) )? ( KEYWORD_28 ( (lv_inherit_10_0= ruleName ) )+ )? ( RULE_TERM )+ ( (lv_content_12_0= ruleClassContent ) )*
            {
            match(input,KEYWORD_27,FOLLOW_KEYWORD_27_in_ruleClass3078); 

                    createLeafNode(grammarAccess.getClassAccess().getClassKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1791:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1792:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1792:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1793:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getClassAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleClass3099);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getClassRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1815:2: ( KEYWORD_34 ( (lv_meta_3_0= ruleName ) ) )?
            int alt27=2;
            int LA27_0 = input.LA(1);

            if ( (LA27_0==KEYWORD_34) ) {
                alt27=1;
            }
            switch (alt27) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1816:2: KEYWORD_34 ( (lv_meta_3_0= ruleName ) )
                    {
                    match(input,KEYWORD_34,FOLLOW_KEYWORD_34_in_ruleClass3111); 

                            createLeafNode(grammarAccess.getClassAccess().getMetaclassKeyword_2_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1820:1: ( (lv_meta_3_0= ruleName ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1821:1: (lv_meta_3_0= ruleName )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1821:1: (lv_meta_3_0= ruleName )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1822:3: lv_meta_3_0= ruleName
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getClassAccess().getMetaNameParserRuleCall_2_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleName_in_ruleClass3132);
                    lv_meta_3_0=ruleName();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getClassRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"meta",
                    	        		lv_meta_3_0, 
                    	        		"Name", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1844:4: ( ( KEYWORD_31 ( (lv_subclass_5_0= ruleName ) ) ) | ( KEYWORD_37 ( (lv_mixin_7_0= ruleName ) ) ) )?
            int alt28=3;
            int LA28_0 = input.LA(1);

            if ( (LA28_0==KEYWORD_31) ) {
                alt28=1;
            }
            else if ( (LA28_0==KEYWORD_37) ) {
                alt28=2;
            }
            switch (alt28) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1844:5: ( KEYWORD_31 ( (lv_subclass_5_0= ruleName ) ) )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1844:5: ( KEYWORD_31 ( (lv_subclass_5_0= ruleName ) ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1845:2: KEYWORD_31 ( (lv_subclass_5_0= ruleName ) )
                    {
                    match(input,KEYWORD_31,FOLLOW_KEYWORD_31_in_ruleClass3147); 

                            createLeafNode(grammarAccess.getClassAccess().getSubclassKeyword_3_0_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1849:1: ( (lv_subclass_5_0= ruleName ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1850:1: (lv_subclass_5_0= ruleName )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1850:1: (lv_subclass_5_0= ruleName )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1851:3: lv_subclass_5_0= ruleName
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getClassAccess().getSubclassNameParserRuleCall_3_0_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleName_in_ruleClass3168);
                    lv_subclass_5_0=ruleName();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getClassRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"subclass",
                    	        		lv_subclass_5_0, 
                    	        		"Name", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }


                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1874:6: ( KEYWORD_37 ( (lv_mixin_7_0= ruleName ) ) )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1874:6: ( KEYWORD_37 ( (lv_mixin_7_0= ruleName ) ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1875:2: KEYWORD_37 ( (lv_mixin_7_0= ruleName ) )
                    {
                    match(input,KEYWORD_37,FOLLOW_KEYWORD_37_in_ruleClass3187); 

                            createLeafNode(grammarAccess.getClassAccess().getMixinclassKeyword_3_1_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1879:1: ( (lv_mixin_7_0= ruleName ) )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1880:1: (lv_mixin_7_0= ruleName )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1880:1: (lv_mixin_7_0= ruleName )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1881:3: lv_mixin_7_0= ruleName
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getClassAccess().getMixinNameParserRuleCall_3_1_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleName_in_ruleClass3208);
                    lv_mixin_7_0=ruleName();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getClassRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"mixin",
                    	        		lv_mixin_7_0, 
                    	        		"Name", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }


                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1903:5: ( (lv_visibility_8_0= ruleVISIBILITY ) )?
            int alt29=2;
            int LA29_0 = input.LA(1);

            if ( (LA29_0==KEYWORD_29||LA29_0==KEYWORD_24) ) {
                alt29=1;
            }
            switch (alt29) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1904:1: (lv_visibility_8_0= ruleVISIBILITY )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1904:1: (lv_visibility_8_0= ruleVISIBILITY )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1905:3: lv_visibility_8_0= ruleVISIBILITY
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getClassAccess().getVisibilityVISIBILITYEnumRuleCall_4_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleVISIBILITY_in_ruleClass3232);
                    lv_visibility_8_0=ruleVISIBILITY();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getClassRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"visibility",
                    	        		lv_visibility_8_0, 
                    	        		"VISIBILITY", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1927:3: ( KEYWORD_28 ( (lv_inherit_10_0= ruleName ) )+ )?
            int alt31=2;
            int LA31_0 = input.LA(1);

            if ( (LA31_0==KEYWORD_28) ) {
                alt31=1;
            }
            switch (alt31) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1928:2: KEYWORD_28 ( (lv_inherit_10_0= ruleName ) )+
                    {
                    match(input,KEYWORD_28,FOLLOW_KEYWORD_28_in_ruleClass3245); 

                            createLeafNode(grammarAccess.getClassAccess().getInheritKeyword_5_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1932:1: ( (lv_inherit_10_0= ruleName ) )+
                    int cnt30=0;
                    loop30:
                    do {
                        int alt30=2;
                        int LA30_0 = input.LA(1);

                        if ( (LA30_0==RULE_STRING||LA30_0==RULE_SYMBOL_VAR) ) {
                            alt30=1;
                        }


                        switch (alt30) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1933:1: (lv_inherit_10_0= ruleName )
                    	    {
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1933:1: (lv_inherit_10_0= ruleName )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1934:3: lv_inherit_10_0= ruleName
                    	    {
                    	     
                    	    	        currentNode=createCompositeNode(grammarAccess.getClassAccess().getInheritNameParserRuleCall_5_1_0(), currentNode); 
                    	    	    
                    	    pushFollow(FOLLOW_ruleName_in_ruleClass3266);
                    	    lv_inherit_10_0=ruleName();
                    	    _fsp--;


                    	    	        if (current==null) {
                    	    	            current = factory.create(grammarAccess.getClassRule().getType().getClassifier());
                    	    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	    	        }
                    	    	        try {
                    	    	       		add(
                    	    	       			current, 
                    	    	       			"inherit",
                    	    	        		lv_inherit_10_0, 
                    	    	        		"Name", 
                    	    	        		currentNode);
                    	    	        } catch (ValueConverterException vce) {
                    	    				handleValueConverterException(vce);
                    	    	        }
                    	    	        currentNode = currentNode.getParent();
                    	    	    

                    	    }


                    	    }
                    	    break;

                    	default :
                    	    if ( cnt30 >= 1 ) break loop30;
                                EarlyExitException eee =
                                    new EarlyExitException(30, input);
                                throw eee;
                        }
                        cnt30++;
                    } while (true);


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1956:5: ( RULE_TERM )+
            int cnt32=0;
            loop32:
            do {
                int alt32=2;
                int LA32_0 = input.LA(1);

                if ( (LA32_0==RULE_TERM) ) {
                    alt32=1;
                }


                switch (alt32) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1956:6: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleClass3279); 
            	     
            	        createLeafNode(grammarAccess.getClassAccess().getTERMTerminalRuleCall_6(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt32 >= 1 ) break loop32;
                        EarlyExitException eee =
                            new EarlyExitException(32, input);
                        throw eee;
                }
                cnt32++;
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1960:3: ( (lv_content_12_0= ruleClassContent ) )*
            loop33:
            do {
                int alt33=2;
                int LA33_0 = input.LA(1);

                if ( ((LA33_0>=KEYWORD_38 && LA33_0<=KEYWORD_35)||LA33_0==KEYWORD_30) ) {
                    alt33=1;
                }


                switch (alt33) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1961:1: (lv_content_12_0= ruleClassContent )
            	    {
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1961:1: (lv_content_12_0= ruleClassContent )
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1962:3: lv_content_12_0= ruleClassContent
            	    {
            	     
            	    	        currentNode=createCompositeNode(grammarAccess.getClassAccess().getContentClassContentParserRuleCall_7_0(), currentNode); 
            	    	    
            	    pushFollow(FOLLOW_ruleClassContent_in_ruleClass3301);
            	    lv_content_12_0=ruleClassContent();
            	    _fsp--;


            	    	        if (current==null) {
            	    	            current = factory.create(grammarAccess.getClassRule().getType().getClassifier());
            	    	            associateNodeWithAstElement(currentNode.getParent(), current);
            	    	        }
            	    	        try {
            	    	       		add(
            	    	       			current, 
            	    	       			"content",
            	    	        		lv_content_12_0, 
            	    	        		"ClassContent", 
            	    	        		currentNode);
            	    	        } catch (ValueConverterException vce) {
            	    				handleValueConverterException(vce);
            	    	        }
            	    	        currentNode = currentNode.getParent();
            	    	    

            	    }


            	    }
            	    break;

            	default :
            	    break loop33;
                }
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleClass


    // $ANTLR start entryRuleClassContent
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1992:1: entryRuleClassContent returns [EObject current=null] : iv_ruleClassContent= ruleClassContent EOF ;
    public final EObject entryRuleClassContent() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleClassContent = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1993:2: (iv_ruleClassContent= ruleClassContent EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:1994:2: iv_ruleClassContent= ruleClassContent EOF
            {
             currentNode = createCompositeNode(grammarAccess.getClassContentRule(), currentNode); 
            pushFollow(FOLLOW_ruleClassContent_in_entryRuleClassContent3337);
            iv_ruleClassContent=ruleClassContent();
            _fsp--;

             current =iv_ruleClassContent; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleClassContent3347); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleClassContent


    // $ANTLR start ruleClassContent
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2001:1: ruleClassContent returns [EObject current=null] : (this_Method_0= ruleMethod | this_Attribute_1= ruleAttribute | this_Constant_2= ruleConstant ) ;
    public final EObject ruleClassContent() throws RecognitionException {
        EObject current = null;

        EObject this_Method_0 = null;

        EObject this_Attribute_1 = null;

        EObject this_Constant_2 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2006:6: ( (this_Method_0= ruleMethod | this_Attribute_1= ruleAttribute | this_Constant_2= ruleConstant ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2007:1: (this_Method_0= ruleMethod | this_Attribute_1= ruleAttribute | this_Constant_2= ruleConstant )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2007:1: (this_Method_0= ruleMethod | this_Attribute_1= ruleAttribute | this_Constant_2= ruleConstant )
            int alt34=3;
            switch ( input.LA(1) ) {
            case KEYWORD_30:
                {
                alt34=1;
                }
                break;
            case KEYWORD_38:
                {
                alt34=2;
                }
                break;
            case KEYWORD_35:
                {
                alt34=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("2007:1: (this_Method_0= ruleMethod | this_Attribute_1= ruleAttribute | this_Constant_2= ruleConstant )", 34, 0, input);

                throw nvae;
            }

            switch (alt34) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2008:5: this_Method_0= ruleMethod
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getClassContentAccess().getMethodParserRuleCall_0(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleMethod_in_ruleClassContent3394);
                    this_Method_0=ruleMethod();
                    _fsp--;

                     
                            current = this_Method_0; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2018:5: this_Attribute_1= ruleAttribute
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getClassContentAccess().getAttributeParserRuleCall_1(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleAttribute_in_ruleClassContent3421);
                    this_Attribute_1=ruleAttribute();
                    _fsp--;

                     
                            current = this_Attribute_1; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2028:5: this_Constant_2= ruleConstant
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getClassContentAccess().getConstantParserRuleCall_2(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleConstant_in_ruleClassContent3448);
                    this_Constant_2=ruleConstant();
                    _fsp--;

                     
                            current = this_Constant_2; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleClassContent


    // $ANTLR start entryRuleMethod
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2044:1: entryRuleMethod returns [EObject current=null] : iv_ruleMethod= ruleMethod EOF ;
    public final EObject entryRuleMethod() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleMethod = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2045:2: (iv_ruleMethod= ruleMethod EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2046:2: iv_ruleMethod= ruleMethod EOF
            {
             currentNode = createCompositeNode(grammarAccess.getMethodRule(), currentNode); 
            pushFollow(FOLLOW_ruleMethod_in_entryRuleMethod3482);
            iv_ruleMethod=ruleMethod();
            _fsp--;

             current =iv_ruleMethod; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleMethod3492); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleMethod


    // $ANTLR start ruleMethod
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2053:1: ruleMethod returns [EObject current=null] : ( KEYWORD_30 ( (lv_name_1_0= ruleName ) ) ( (lv_visibility_2_0= ruleVISIBILITY ) )? ( (lv_class_3_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_6_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_8_0= ruleInstruction ) )* ) ;
    public final EObject ruleMethod() throws RecognitionException {
        EObject current = null;

        Token lv_class_3_0=null;
        AntlrDatatypeRuleToken lv_name_1_0 = null;

        Enumerator lv_visibility_2_0 = null;

        AntlrDatatypeRuleToken lv_expose_6_0 = null;

        EObject lv_content_8_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2058:6: ( ( KEYWORD_30 ( (lv_name_1_0= ruleName ) ) ( (lv_visibility_2_0= ruleVISIBILITY ) )? ( (lv_class_3_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_6_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_8_0= ruleInstruction ) )* ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2059:1: ( KEYWORD_30 ( (lv_name_1_0= ruleName ) ) ( (lv_visibility_2_0= ruleVISIBILITY ) )? ( (lv_class_3_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_6_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_8_0= ruleInstruction ) )* )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2059:1: ( KEYWORD_30 ( (lv_name_1_0= ruleName ) ) ( (lv_visibility_2_0= ruleVISIBILITY ) )? ( (lv_class_3_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_6_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_8_0= ruleInstruction ) )* )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2060:2: KEYWORD_30 ( (lv_name_1_0= ruleName ) ) ( (lv_visibility_2_0= ruleVISIBILITY ) )? ( (lv_class_3_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_6_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_8_0= ruleInstruction ) )*
            {
            match(input,KEYWORD_30,FOLLOW_KEYWORD_30_in_ruleMethod3528); 

                    createLeafNode(grammarAccess.getMethodAccess().getMethodKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2064:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2065:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2065:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2066:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getMethodAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleMethod3549);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getMethodRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2088:2: ( (lv_visibility_2_0= ruleVISIBILITY ) )?
            int alt35=2;
            int LA35_0 = input.LA(1);

            if ( (LA35_0==KEYWORD_29||LA35_0==KEYWORD_24) ) {
                alt35=1;
            }
            switch (alt35) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2089:1: (lv_visibility_2_0= ruleVISIBILITY )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2089:1: (lv_visibility_2_0= ruleVISIBILITY )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2090:3: lv_visibility_2_0= ruleVISIBILITY
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getMethodAccess().getVisibilityVISIBILITYEnumRuleCall_2_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleVISIBILITY_in_ruleMethod3570);
                    lv_visibility_2_0=ruleVISIBILITY();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getMethodRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"visibility",
                    	        		lv_visibility_2_0, 
                    	        		"VISIBILITY", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2112:3: ( (lv_class_3_0= KEYWORD_21 ) )?
            int alt36=2;
            int LA36_0 = input.LA(1);

            if ( (LA36_0==KEYWORD_21) ) {
                alt36=1;
            }
            switch (alt36) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2113:1: (lv_class_3_0= KEYWORD_21 )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2113:1: (lv_class_3_0= KEYWORD_21 )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2114:3: lv_class_3_0= KEYWORD_21
                    {
                    lv_class_3_0=(Token)input.LT(1);
                    match(input,KEYWORD_21,FOLLOW_KEYWORD_21_in_ruleMethod3590); 

                            createLeafNode(grammarAccess.getMethodAccess().getClassClassKeyword_3_0(), "class"); 
                        

                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getMethodRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode, current);
                    	        }
                    	        
                    	        try {
                    	       		set(current, "class", true, "class", lastConsumedNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	    

                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2134:3: ( RULE_TERM )+
            int cnt37=0;
            loop37:
            do {
                int alt37=2;
                int LA37_0 = input.LA(1);

                if ( (LA37_0==RULE_TERM) ) {
                    alt37=1;
                }


                switch (alt37) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2134:4: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleMethod3614); 
            	     
            	        createLeafNode(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_4(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt37 >= 1 ) break loop37;
                        EarlyExitException eee =
                            new EarlyExitException(37, input);
                        throw eee;
                }
                cnt37++;
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2138:3: ( KEYWORD_23 ( (lv_expose_6_0= ruleName ) )+ ( RULE_TERM )+ )?
            int alt40=2;
            int LA40_0 = input.LA(1);

            if ( (LA40_0==KEYWORD_23) ) {
                alt40=1;
            }
            switch (alt40) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2139:2: KEYWORD_23 ( (lv_expose_6_0= ruleName ) )+ ( RULE_TERM )+
                    {
                    match(input,KEYWORD_23,FOLLOW_KEYWORD_23_in_ruleMethod3627); 

                            createLeafNode(grammarAccess.getMethodAccess().getExposeKeyword_5_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2143:1: ( (lv_expose_6_0= ruleName ) )+
                    int cnt38=0;
                    loop38:
                    do {
                        int alt38=2;
                        int LA38_0 = input.LA(1);

                        if ( (LA38_0==RULE_STRING||LA38_0==RULE_SYMBOL_VAR) ) {
                            alt38=1;
                        }


                        switch (alt38) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2144:1: (lv_expose_6_0= ruleName )
                    	    {
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2144:1: (lv_expose_6_0= ruleName )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2145:3: lv_expose_6_0= ruleName
                    	    {
                    	     
                    	    	        currentNode=createCompositeNode(grammarAccess.getMethodAccess().getExposeNameParserRuleCall_5_1_0(), currentNode); 
                    	    	    
                    	    pushFollow(FOLLOW_ruleName_in_ruleMethod3648);
                    	    lv_expose_6_0=ruleName();
                    	    _fsp--;


                    	    	        if (current==null) {
                    	    	            current = factory.create(grammarAccess.getMethodRule().getType().getClassifier());
                    	    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	    	        }
                    	    	        try {
                    	    	       		add(
                    	    	       			current, 
                    	    	       			"expose",
                    	    	        		lv_expose_6_0, 
                    	    	        		"Name", 
                    	    	        		currentNode);
                    	    	        } catch (ValueConverterException vce) {
                    	    				handleValueConverterException(vce);
                    	    	        }
                    	    	        currentNode = currentNode.getParent();
                    	    	    

                    	    }


                    	    }
                    	    break;

                    	default :
                    	    if ( cnt38 >= 1 ) break loop38;
                                EarlyExitException eee =
                                    new EarlyExitException(38, input);
                                throw eee;
                        }
                        cnt38++;
                    } while (true);

                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2167:3: ( RULE_TERM )+
                    int cnt39=0;
                    loop39:
                    do {
                        int alt39=2;
                        int LA39_0 = input.LA(1);

                        if ( (LA39_0==RULE_TERM) ) {
                            alt39=1;
                        }


                        switch (alt39) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2167:4: RULE_TERM
                    	    {
                    	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleMethod3659); 
                    	     
                    	        createLeafNode(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_5_2(), null); 
                    	        

                    	    }
                    	    break;

                    	default :
                    	    if ( cnt39 >= 1 ) break loop39;
                                EarlyExitException eee =
                                    new EarlyExitException(39, input);
                                throw eee;
                        }
                        cnt39++;
                    } while (true);


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2171:5: ( (lv_content_8_0= ruleInstruction ) )*
            loop41:
            do {
                int alt41=2;
                int LA41_0 = input.LA(1);

                if ( ((LA41_0>=KEYWORD_22 && LA41_0<=KEYWORD_18)||LA41_0==KEYWORD_13||(LA41_0>=KEYWORD_17 && LA41_0<=KEYWORD_12)||LA41_0==RULE_STRING||LA41_0==RULE_SYMBOL_VAR) ) {
                    alt41=1;
                }


                switch (alt41) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2172:1: (lv_content_8_0= ruleInstruction )
            	    {
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2172:1: (lv_content_8_0= ruleInstruction )
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2173:3: lv_content_8_0= ruleInstruction
            	    {
            	     
            	    	        currentNode=createCompositeNode(grammarAccess.getMethodAccess().getContentInstructionParserRuleCall_6_0(), currentNode); 
            	    	    
            	    pushFollow(FOLLOW_ruleInstruction_in_ruleMethod3683);
            	    lv_content_8_0=ruleInstruction();
            	    _fsp--;


            	    	        if (current==null) {
            	    	            current = factory.create(grammarAccess.getMethodRule().getType().getClassifier());
            	    	            associateNodeWithAstElement(currentNode.getParent(), current);
            	    	        }
            	    	        try {
            	    	       		add(
            	    	       			current, 
            	    	       			"content",
            	    	        		lv_content_8_0, 
            	    	        		"Instruction", 
            	    	        		currentNode);
            	    	        } catch (ValueConverterException vce) {
            	    				handleValueConverterException(vce);
            	    	        }
            	    	        currentNode = currentNode.getParent();
            	    	    

            	    }


            	    }
            	    break;

            	default :
            	    break loop41;
                }
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleMethod


    // $ANTLR start entryRuleAttribute
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2203:1: entryRuleAttribute returns [EObject current=null] : iv_ruleAttribute= ruleAttribute EOF ;
    public final EObject entryRuleAttribute() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleAttribute = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2204:2: (iv_ruleAttribute= ruleAttribute EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2205:2: iv_ruleAttribute= ruleAttribute EOF
            {
             currentNode = createCompositeNode(grammarAccess.getAttributeRule(), currentNode); 
            pushFollow(FOLLOW_ruleAttribute_in_entryRuleAttribute3719);
            iv_ruleAttribute=ruleAttribute();
            _fsp--;

             current =iv_ruleAttribute; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAttribute3729); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleAttribute


    // $ANTLR start ruleAttribute
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2212:1: ruleAttribute returns [EObject current=null] : ( (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal ) ( (lv_visibility_3_0= ruleVISIBILITY ) )? ( (lv_class_4_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_7_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_9_0= ruleInstruction ) )* ) ;
    public final EObject ruleAttribute() throws RecognitionException {
        EObject current = null;

        Token lv_class_4_0=null;
        EObject this_AttributeSet_0 = null;

        EObject this_AttributeGet_1 = null;

        EObject this_AttributeNormal_2 = null;

        Enumerator lv_visibility_3_0 = null;

        AntlrDatatypeRuleToken lv_expose_7_0 = null;

        EObject lv_content_9_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2217:6: ( ( (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal ) ( (lv_visibility_3_0= ruleVISIBILITY ) )? ( (lv_class_4_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_7_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_9_0= ruleInstruction ) )* ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2218:1: ( (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal ) ( (lv_visibility_3_0= ruleVISIBILITY ) )? ( (lv_class_4_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_7_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_9_0= ruleInstruction ) )* )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2218:1: ( (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal ) ( (lv_visibility_3_0= ruleVISIBILITY ) )? ( (lv_class_4_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_7_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_9_0= ruleInstruction ) )* )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2218:2: (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal ) ( (lv_visibility_3_0= ruleVISIBILITY ) )? ( (lv_class_4_0= KEYWORD_21 ) )? ( RULE_TERM )+ ( KEYWORD_23 ( (lv_expose_7_0= ruleName ) )+ ( RULE_TERM )+ )? ( (lv_content_9_0= ruleInstruction ) )*
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2218:2: (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal )
            int alt42=3;
            int LA42_0 = input.LA(1);

            if ( (LA42_0==KEYWORD_38) ) {
                int LA42_1 = input.LA(2);

                if ( (LA42_1==RULE_STRING) ) {
                    switch ( input.LA(3) ) {
                    case KEYWORD_29:
                    case KEYWORD_24:
                    case KEYWORD_21:
                    case RULE_TERM:
                        {
                        alt42=3;
                        }
                        break;
                    case KEYWORD_16:
                        {
                        alt42=1;
                        }
                        break;
                    case KEYWORD_15:
                        {
                        alt42=2;
                        }
                        break;
                    default:
                        NoViableAltException nvae =
                            new NoViableAltException("2218:2: (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal )", 42, 2, input);

                        throw nvae;
                    }

                }
                else if ( (LA42_1==RULE_SYMBOL_VAR) ) {
                    switch ( input.LA(3) ) {
                    case KEYWORD_15:
                        {
                        alt42=2;
                        }
                        break;
                    case KEYWORD_29:
                    case KEYWORD_24:
                    case KEYWORD_21:
                    case RULE_TERM:
                        {
                        alt42=3;
                        }
                        break;
                    case KEYWORD_16:
                        {
                        alt42=1;
                        }
                        break;
                    default:
                        NoViableAltException nvae =
                            new NoViableAltException("2218:2: (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal )", 42, 3, input);

                        throw nvae;
                    }

                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("2218:2: (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal )", 42, 1, input);

                    throw nvae;
                }
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("2218:2: (this_AttributeSet_0= ruleAttributeSet | this_AttributeGet_1= ruleAttributeGet | this_AttributeNormal_2= ruleAttributeNormal )", 42, 0, input);

                throw nvae;
            }
            switch (alt42) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2219:5: this_AttributeSet_0= ruleAttributeSet
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getAttributeAccess().getAttributeSetParserRuleCall_0_0(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleAttributeSet_in_ruleAttribute3777);
                    this_AttributeSet_0=ruleAttributeSet();
                    _fsp--;

                     
                            current = this_AttributeSet_0; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2229:5: this_AttributeGet_1= ruleAttributeGet
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getAttributeAccess().getAttributeGetParserRuleCall_0_1(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleAttributeGet_in_ruleAttribute3804);
                    this_AttributeGet_1=ruleAttributeGet();
                    _fsp--;

                     
                            current = this_AttributeGet_1; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2239:5: this_AttributeNormal_2= ruleAttributeNormal
                    {
                     
                            currentNode=createCompositeNode(grammarAccess.getAttributeAccess().getAttributeNormalParserRuleCall_0_2(), currentNode); 
                        
                    pushFollow(FOLLOW_ruleAttributeNormal_in_ruleAttribute3831);
                    this_AttributeNormal_2=ruleAttributeNormal();
                    _fsp--;

                     
                            current = this_AttributeNormal_2; 
                            currentNode = currentNode.getParent();
                        

                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2247:2: ( (lv_visibility_3_0= ruleVISIBILITY ) )?
            int alt43=2;
            int LA43_0 = input.LA(1);

            if ( (LA43_0==KEYWORD_29||LA43_0==KEYWORD_24) ) {
                alt43=1;
            }
            switch (alt43) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2248:1: (lv_visibility_3_0= ruleVISIBILITY )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2248:1: (lv_visibility_3_0= ruleVISIBILITY )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2249:3: lv_visibility_3_0= ruleVISIBILITY
                    {
                     
                    	        currentNode=createCompositeNode(grammarAccess.getAttributeAccess().getVisibilityVISIBILITYEnumRuleCall_1_0(), currentNode); 
                    	    
                    pushFollow(FOLLOW_ruleVISIBILITY_in_ruleAttribute3852);
                    lv_visibility_3_0=ruleVISIBILITY();
                    _fsp--;


                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getAttributeRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	        }
                    	        try {
                    	       		set(
                    	       			current, 
                    	       			"visibility",
                    	        		lv_visibility_3_0, 
                    	        		"VISIBILITY", 
                    	        		currentNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	        currentNode = currentNode.getParent();
                    	    

                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2271:3: ( (lv_class_4_0= KEYWORD_21 ) )?
            int alt44=2;
            int LA44_0 = input.LA(1);

            if ( (LA44_0==KEYWORD_21) ) {
                alt44=1;
            }
            switch (alt44) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2272:1: (lv_class_4_0= KEYWORD_21 )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2272:1: (lv_class_4_0= KEYWORD_21 )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2273:3: lv_class_4_0= KEYWORD_21
                    {
                    lv_class_4_0=(Token)input.LT(1);
                    match(input,KEYWORD_21,FOLLOW_KEYWORD_21_in_ruleAttribute3872); 

                            createLeafNode(grammarAccess.getAttributeAccess().getClassClassKeyword_2_0(), "class"); 
                        

                    	        if (current==null) {
                    	            current = factory.create(grammarAccess.getAttributeRule().getType().getClassifier());
                    	            associateNodeWithAstElement(currentNode, current);
                    	        }
                    	        
                    	        try {
                    	       		set(current, "class", true, "class", lastConsumedNode);
                    	        } catch (ValueConverterException vce) {
                    				handleValueConverterException(vce);
                    	        }
                    	    

                    }


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2293:3: ( RULE_TERM )+
            int cnt45=0;
            loop45:
            do {
                int alt45=2;
                int LA45_0 = input.LA(1);

                if ( (LA45_0==RULE_TERM) ) {
                    alt45=1;
                }


                switch (alt45) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2293:4: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleAttribute3896); 
            	     
            	        createLeafNode(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_3(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt45 >= 1 ) break loop45;
                        EarlyExitException eee =
                            new EarlyExitException(45, input);
                        throw eee;
                }
                cnt45++;
            } while (true);

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2297:3: ( KEYWORD_23 ( (lv_expose_7_0= ruleName ) )+ ( RULE_TERM )+ )?
            int alt48=2;
            int LA48_0 = input.LA(1);

            if ( (LA48_0==KEYWORD_23) ) {
                alt48=1;
            }
            switch (alt48) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2298:2: KEYWORD_23 ( (lv_expose_7_0= ruleName ) )+ ( RULE_TERM )+
                    {
                    match(input,KEYWORD_23,FOLLOW_KEYWORD_23_in_ruleAttribute3909); 

                            createLeafNode(grammarAccess.getAttributeAccess().getExposeKeyword_4_0(), null); 
                        
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2302:1: ( (lv_expose_7_0= ruleName ) )+
                    int cnt46=0;
                    loop46:
                    do {
                        int alt46=2;
                        int LA46_0 = input.LA(1);

                        if ( (LA46_0==RULE_STRING||LA46_0==RULE_SYMBOL_VAR) ) {
                            alt46=1;
                        }


                        switch (alt46) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2303:1: (lv_expose_7_0= ruleName )
                    	    {
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2303:1: (lv_expose_7_0= ruleName )
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2304:3: lv_expose_7_0= ruleName
                    	    {
                    	     
                    	    	        currentNode=createCompositeNode(grammarAccess.getAttributeAccess().getExposeNameParserRuleCall_4_1_0(), currentNode); 
                    	    	    
                    	    pushFollow(FOLLOW_ruleName_in_ruleAttribute3930);
                    	    lv_expose_7_0=ruleName();
                    	    _fsp--;


                    	    	        if (current==null) {
                    	    	            current = factory.create(grammarAccess.getAttributeRule().getType().getClassifier());
                    	    	            associateNodeWithAstElement(currentNode.getParent(), current);
                    	    	        }
                    	    	        try {
                    	    	       		add(
                    	    	       			current, 
                    	    	       			"expose",
                    	    	        		lv_expose_7_0, 
                    	    	        		"Name", 
                    	    	        		currentNode);
                    	    	        } catch (ValueConverterException vce) {
                    	    				handleValueConverterException(vce);
                    	    	        }
                    	    	        currentNode = currentNode.getParent();
                    	    	    

                    	    }


                    	    }
                    	    break;

                    	default :
                    	    if ( cnt46 >= 1 ) break loop46;
                                EarlyExitException eee =
                                    new EarlyExitException(46, input);
                                throw eee;
                        }
                        cnt46++;
                    } while (true);

                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2326:3: ( RULE_TERM )+
                    int cnt47=0;
                    loop47:
                    do {
                        int alt47=2;
                        int LA47_0 = input.LA(1);

                        if ( (LA47_0==RULE_TERM) ) {
                            alt47=1;
                        }


                        switch (alt47) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2326:4: RULE_TERM
                    	    {
                    	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleAttribute3941); 
                    	     
                    	        createLeafNode(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_4_2(), null); 
                    	        

                    	    }
                    	    break;

                    	default :
                    	    if ( cnt47 >= 1 ) break loop47;
                                EarlyExitException eee =
                                    new EarlyExitException(47, input);
                                throw eee;
                        }
                        cnt47++;
                    } while (true);


                    }
                    break;

            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2330:5: ( (lv_content_9_0= ruleInstruction ) )*
            loop49:
            do {
                int alt49=2;
                int LA49_0 = input.LA(1);

                if ( ((LA49_0>=KEYWORD_22 && LA49_0<=KEYWORD_18)||LA49_0==KEYWORD_13||(LA49_0>=KEYWORD_17 && LA49_0<=KEYWORD_12)||LA49_0==RULE_STRING||LA49_0==RULE_SYMBOL_VAR) ) {
                    alt49=1;
                }


                switch (alt49) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2331:1: (lv_content_9_0= ruleInstruction )
            	    {
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2331:1: (lv_content_9_0= ruleInstruction )
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2332:3: lv_content_9_0= ruleInstruction
            	    {
            	     
            	    	        currentNode=createCompositeNode(grammarAccess.getAttributeAccess().getContentInstructionParserRuleCall_5_0(), currentNode); 
            	    	    
            	    pushFollow(FOLLOW_ruleInstruction_in_ruleAttribute3965);
            	    lv_content_9_0=ruleInstruction();
            	    _fsp--;


            	    	        if (current==null) {
            	    	            current = factory.create(grammarAccess.getAttributeRule().getType().getClassifier());
            	    	            associateNodeWithAstElement(currentNode.getParent(), current);
            	    	        }
            	    	        try {
            	    	       		add(
            	    	       			current, 
            	    	       			"content",
            	    	        		lv_content_9_0, 
            	    	        		"Instruction", 
            	    	        		currentNode);
            	    	        } catch (ValueConverterException vce) {
            	    				handleValueConverterException(vce);
            	    	        }
            	    	        currentNode = currentNode.getParent();
            	    	    

            	    }


            	    }
            	    break;

            	default :
            	    break loop49;
                }
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleAttribute


    // $ANTLR start entryRuleAttributeSet
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2362:1: entryRuleAttributeSet returns [EObject current=null] : iv_ruleAttributeSet= ruleAttributeSet EOF ;
    public final EObject entryRuleAttributeSet() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleAttributeSet = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2363:2: (iv_ruleAttributeSet= ruleAttributeSet EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2364:2: iv_ruleAttributeSet= ruleAttributeSet EOF
            {
             currentNode = createCompositeNode(grammarAccess.getAttributeSetRule(), currentNode); 
            pushFollow(FOLLOW_ruleAttributeSet_in_entryRuleAttributeSet4001);
            iv_ruleAttributeSet=ruleAttributeSet();
            _fsp--;

             current =iv_ruleAttributeSet; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAttributeSet4011); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleAttributeSet


    // $ANTLR start ruleAttributeSet
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2371:1: ruleAttributeSet returns [EObject current=null] : ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_16 ) ;
    public final EObject ruleAttributeSet() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2376:6: ( ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_16 ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2377:1: ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_16 )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2377:1: ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_16 )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2378:2: KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_16
            {
            match(input,KEYWORD_38,FOLLOW_KEYWORD_38_in_ruleAttributeSet4047); 

                    createLeafNode(grammarAccess.getAttributeSetAccess().getAttributeKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2382:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2383:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2383:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2384:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getAttributeSetAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleAttributeSet4068);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getAttributeSetRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_16,FOLLOW_KEYWORD_16_in_ruleAttributeSet4079); 

                    createLeafNode(grammarAccess.getAttributeSetAccess().getSetKeyword_2(), null); 
                

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleAttributeSet


    // $ANTLR start entryRuleAttributeGet
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2419:1: entryRuleAttributeGet returns [EObject current=null] : iv_ruleAttributeGet= ruleAttributeGet EOF ;
    public final EObject entryRuleAttributeGet() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleAttributeGet = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2420:2: (iv_ruleAttributeGet= ruleAttributeGet EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2421:2: iv_ruleAttributeGet= ruleAttributeGet EOF
            {
             currentNode = createCompositeNode(grammarAccess.getAttributeGetRule(), currentNode); 
            pushFollow(FOLLOW_ruleAttributeGet_in_entryRuleAttributeGet4114);
            iv_ruleAttributeGet=ruleAttributeGet();
            _fsp--;

             current =iv_ruleAttributeGet; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAttributeGet4124); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleAttributeGet


    // $ANTLR start ruleAttributeGet
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2428:1: ruleAttributeGet returns [EObject current=null] : ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_15 ) ;
    public final EObject ruleAttributeGet() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2433:6: ( ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_15 ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2434:1: ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_15 )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2434:1: ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_15 )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2435:2: KEYWORD_38 ( (lv_name_1_0= ruleName ) ) KEYWORD_15
            {
            match(input,KEYWORD_38,FOLLOW_KEYWORD_38_in_ruleAttributeGet4160); 

                    createLeafNode(grammarAccess.getAttributeGetAccess().getAttributeKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2439:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2440:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2440:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2441:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getAttributeGetAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleAttributeGet4181);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getAttributeGetRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            match(input,KEYWORD_15,FOLLOW_KEYWORD_15_in_ruleAttributeGet4192); 

                    createLeafNode(grammarAccess.getAttributeGetAccess().getGetKeyword_2(), null); 
                

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleAttributeGet


    // $ANTLR start entryRuleAttributeNormal
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2476:1: entryRuleAttributeNormal returns [EObject current=null] : iv_ruleAttributeNormal= ruleAttributeNormal EOF ;
    public final EObject entryRuleAttributeNormal() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleAttributeNormal = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2477:2: (iv_ruleAttributeNormal= ruleAttributeNormal EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2478:2: iv_ruleAttributeNormal= ruleAttributeNormal EOF
            {
             currentNode = createCompositeNode(grammarAccess.getAttributeNormalRule(), currentNode); 
            pushFollow(FOLLOW_ruleAttributeNormal_in_entryRuleAttributeNormal4227);
            iv_ruleAttributeNormal=ruleAttributeNormal();
            _fsp--;

             current =iv_ruleAttributeNormal; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAttributeNormal4237); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleAttributeNormal


    // $ANTLR start ruleAttributeNormal
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2485:1: ruleAttributeNormal returns [EObject current=null] : ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) ) ;
    public final EObject ruleAttributeNormal() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2490:6: ( ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2491:1: ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2491:1: ( KEYWORD_38 ( (lv_name_1_0= ruleName ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2492:2: KEYWORD_38 ( (lv_name_1_0= ruleName ) )
            {
            match(input,KEYWORD_38,FOLLOW_KEYWORD_38_in_ruleAttributeNormal4273); 

                    createLeafNode(grammarAccess.getAttributeNormalAccess().getAttributeKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2496:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2497:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2497:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2498:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getAttributeNormalAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleAttributeNormal4294);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getAttributeNormalRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleAttributeNormal


    // $ANTLR start entryRuleConstant
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2528:1: entryRuleConstant returns [EObject current=null] : iv_ruleConstant= ruleConstant EOF ;
    public final EObject entryRuleConstant() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleConstant = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2529:2: (iv_ruleConstant= ruleConstant EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2530:2: iv_ruleConstant= ruleConstant EOF
            {
             currentNode = createCompositeNode(grammarAccess.getConstantRule(), currentNode); 
            pushFollow(FOLLOW_ruleConstant_in_entryRuleConstant4329);
            iv_ruleConstant=ruleConstant();
            _fsp--;

             current =iv_ruleConstant; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleConstant4339); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleConstant


    // $ANTLR start ruleConstant
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2537:1: ruleConstant returns [EObject current=null] : ( KEYWORD_35 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ ) ;
    public final EObject ruleConstant() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2542:6: ( ( KEYWORD_35 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2543:1: ( KEYWORD_35 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2543:1: ( KEYWORD_35 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2544:2: KEYWORD_35 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+
            {
            match(input,KEYWORD_35,FOLLOW_KEYWORD_35_in_ruleConstant4375); 

                    createLeafNode(grammarAccess.getConstantAccess().getConstantKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2548:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2549:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2549:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2550:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getConstantAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleConstant4396);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getConstantRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2572:2: ( RULE_TERM )+
            int cnt50=0;
            loop50:
            do {
                int alt50=2;
                int LA50_0 = input.LA(1);

                if ( (LA50_0==RULE_TERM) ) {
                    alt50=1;
                }


                switch (alt50) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2572:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleConstant4406); 
            	     
            	        createLeafNode(grammarAccess.getConstantAccess().getTERMTerminalRuleCall_2(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt50 >= 1 ) break loop50;
                        EarlyExitException eee =
                            new EarlyExitException(50, input);
                        throw eee;
                }
                cnt50++;
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleConstant


    // $ANTLR start entryRuleRoutine
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2584:1: entryRuleRoutine returns [EObject current=null] : iv_ruleRoutine= ruleRoutine EOF ;
    public final EObject entryRuleRoutine() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleRoutine = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2585:2: (iv_ruleRoutine= ruleRoutine EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2586:2: iv_ruleRoutine= ruleRoutine EOF
            {
             currentNode = createCompositeNode(grammarAccess.getRoutineRule(), currentNode); 
            pushFollow(FOLLOW_ruleRoutine_in_entryRuleRoutine4442);
            iv_ruleRoutine=ruleRoutine();
            _fsp--;

             current =iv_ruleRoutine; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleRoutine4452); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleRoutine


    // $ANTLR start ruleRoutine
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2593:1: ruleRoutine returns [EObject current=null] : ( KEYWORD_33 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ ) ;
    public final EObject ruleRoutine() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2598:6: ( ( KEYWORD_33 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2599:1: ( KEYWORD_33 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2599:1: ( KEYWORD_33 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2600:2: KEYWORD_33 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+
            {
            match(input,KEYWORD_33,FOLLOW_KEYWORD_33_in_ruleRoutine4488); 

                    createLeafNode(grammarAccess.getRoutineAccess().getRoutineKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2604:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2605:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2605:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2606:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getRoutineAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleRoutine4509);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getRoutineRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2628:2: ( RULE_TERM )+
            int cnt51=0;
            loop51:
            do {
                int alt51=2;
                int LA51_0 = input.LA(1);

                if ( (LA51_0==RULE_TERM) ) {
                    alt51=1;
                }


                switch (alt51) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2628:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleRoutine4519); 
            	     
            	        createLeafNode(grammarAccess.getRoutineAccess().getTERMTerminalRuleCall_2(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt51 >= 1 ) break loop51;
                        EarlyExitException eee =
                            new EarlyExitException(51, input);
                        throw eee;
                }
                cnt51++;
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleRoutine


    // $ANTLR start entryRuleRequires
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2640:1: entryRuleRequires returns [EObject current=null] : iv_ruleRequires= ruleRequires EOF ;
    public final EObject entryRuleRequires() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleRequires = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2641:2: (iv_ruleRequires= ruleRequires EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2642:2: iv_ruleRequires= ruleRequires EOF
            {
             currentNode = createCompositeNode(grammarAccess.getRequiresRule(), currentNode); 
            pushFollow(FOLLOW_ruleRequires_in_entryRuleRequires4555);
            iv_ruleRequires=ruleRequires();
            _fsp--;

             current =iv_ruleRequires; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleRequires4565); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleRequires


    // $ANTLR start ruleRequires
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2649:1: ruleRequires returns [EObject current=null] : ( KEYWORD_36 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ ) ;
    public final EObject ruleRequires() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2654:6: ( ( KEYWORD_36 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2655:1: ( KEYWORD_36 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2655:1: ( KEYWORD_36 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2656:2: KEYWORD_36 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+
            {
            match(input,KEYWORD_36,FOLLOW_KEYWORD_36_in_ruleRequires4601); 

                    createLeafNode(grammarAccess.getRequiresAccess().getRequiresKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2660:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2661:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2661:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2662:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getRequiresAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleRequires4622);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getRequiresRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2684:2: ( RULE_TERM )+
            int cnt52=0;
            loop52:
            do {
                int alt52=2;
                int LA52_0 = input.LA(1);

                if ( (LA52_0==RULE_TERM) ) {
                    alt52=1;
                }


                switch (alt52) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2684:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleRequires4632); 
            	     
            	        createLeafNode(grammarAccess.getRequiresAccess().getTERMTerminalRuleCall_2(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt52 >= 1 ) break loop52;
                        EarlyExitException eee =
                            new EarlyExitException(52, input);
                        throw eee;
                }
                cnt52++;
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleRequires


    // $ANTLR start entryRuleOptions
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2696:1: entryRuleOptions returns [EObject current=null] : iv_ruleOptions= ruleOptions EOF ;
    public final EObject entryRuleOptions() throws RecognitionException {
        EObject current = null;

        EObject iv_ruleOptions = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2697:2: (iv_ruleOptions= ruleOptions EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2698:2: iv_ruleOptions= ruleOptions EOF
            {
             currentNode = createCompositeNode(grammarAccess.getOptionsRule(), currentNode); 
            pushFollow(FOLLOW_ruleOptions_in_entryRuleOptions4668);
            iv_ruleOptions=ruleOptions();
            _fsp--;

             current =iv_ruleOptions; 
            match(input,EOF,FOLLOW_EOF_in_entryRuleOptions4678); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleOptions


    // $ANTLR start ruleOptions
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2705:1: ruleOptions returns [EObject current=null] : ( KEYWORD_32 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ ) ;
    public final EObject ruleOptions() throws RecognitionException {
        EObject current = null;

        AntlrDatatypeRuleToken lv_name_1_0 = null;


         EObject temp=null; setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2710:6: ( ( KEYWORD_32 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2711:1: ( KEYWORD_32 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2711:1: ( KEYWORD_32 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+ )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2712:2: KEYWORD_32 ( (lv_name_1_0= ruleName ) ) ( RULE_TERM )+
            {
            match(input,KEYWORD_32,FOLLOW_KEYWORD_32_in_ruleOptions4714); 

                    createLeafNode(grammarAccess.getOptionsAccess().getOptionsKeyword_0(), null); 
                
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2716:1: ( (lv_name_1_0= ruleName ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2717:1: (lv_name_1_0= ruleName )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2717:1: (lv_name_1_0= ruleName )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2718:3: lv_name_1_0= ruleName
            {
             
            	        currentNode=createCompositeNode(grammarAccess.getOptionsAccess().getNameNameParserRuleCall_1_0(), currentNode); 
            	    
            pushFollow(FOLLOW_ruleName_in_ruleOptions4735);
            lv_name_1_0=ruleName();
            _fsp--;


            	        if (current==null) {
            	            current = factory.create(grammarAccess.getOptionsRule().getType().getClassifier());
            	            associateNodeWithAstElement(currentNode.getParent(), current);
            	        }
            	        try {
            	       		set(
            	       			current, 
            	       			"name",
            	        		lv_name_1_0, 
            	        		"Name", 
            	        		currentNode);
            	        } catch (ValueConverterException vce) {
            				handleValueConverterException(vce);
            	        }
            	        currentNode = currentNode.getParent();
            	    

            }


            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2740:2: ( RULE_TERM )+
            int cnt53=0;
            loop53:
            do {
                int alt53=2;
                int LA53_0 = input.LA(1);

                if ( (LA53_0==RULE_TERM) ) {
                    alt53=1;
                }


                switch (alt53) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2740:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_ruleOptions4745); 
            	     
            	        createLeafNode(grammarAccess.getOptionsAccess().getTERMTerminalRuleCall_2(), null); 
            	        

            	    }
            	    break;

            	default :
            	    if ( cnt53 >= 1 ) break loop53;
                        EarlyExitException eee =
                            new EarlyExitException(53, input);
                        throw eee;
                }
                cnt53++;
            } while (true);


            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleOptions


    // $ANTLR start entryRuleName
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2752:1: entryRuleName returns [String current=null] : iv_ruleName= ruleName EOF ;
    public final String entryRuleName() throws RecognitionException {
        String current = null;

        AntlrDatatypeRuleToken iv_ruleName = null;


        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2753:1: (iv_ruleName= ruleName EOF )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2754:2: iv_ruleName= ruleName EOF
            {
             currentNode = createCompositeNode(grammarAccess.getNameRule(), currentNode); 
            pushFollow(FOLLOW_ruleName_in_entryRuleName4782);
            iv_ruleName=ruleName();
            _fsp--;

             current =iv_ruleName.getText(); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleName4793); 

            }

        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end entryRuleName


    // $ANTLR start ruleName
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2761:1: ruleName returns [AntlrDatatypeRuleToken current=new AntlrDatatypeRuleToken()] : (this_STRING_0= RULE_STRING | this_SYMBOL_VAR_1= RULE_SYMBOL_VAR ) ;
    public final AntlrDatatypeRuleToken ruleName() throws RecognitionException {
        AntlrDatatypeRuleToken current = new AntlrDatatypeRuleToken();

        Token this_STRING_0=null;
        Token this_SYMBOL_VAR_1=null;

         setCurrentLookahead(); resetLookahead(); 
            
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2766:6: ( (this_STRING_0= RULE_STRING | this_SYMBOL_VAR_1= RULE_SYMBOL_VAR ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2767:1: (this_STRING_0= RULE_STRING | this_SYMBOL_VAR_1= RULE_SYMBOL_VAR )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2767:1: (this_STRING_0= RULE_STRING | this_SYMBOL_VAR_1= RULE_SYMBOL_VAR )
            int alt54=2;
            int LA54_0 = input.LA(1);

            if ( (LA54_0==RULE_STRING) ) {
                alt54=1;
            }
            else if ( (LA54_0==RULE_SYMBOL_VAR) ) {
                alt54=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("2767:1: (this_STRING_0= RULE_STRING | this_SYMBOL_VAR_1= RULE_SYMBOL_VAR )", 54, 0, input);

                throw nvae;
            }
            switch (alt54) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2767:6: this_STRING_0= RULE_STRING
                    {
                    this_STRING_0=(Token)input.LT(1);
                    match(input,RULE_STRING,FOLLOW_RULE_STRING_in_ruleName4833); 

                    		current.merge(this_STRING_0);
                        
                     
                        createLeafNode(grammarAccess.getNameAccess().getSTRINGTerminalRuleCall_0(), null); 
                        

                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2775:10: this_SYMBOL_VAR_1= RULE_SYMBOL_VAR
                    {
                    this_SYMBOL_VAR_1=(Token)input.LT(1);
                    match(input,RULE_SYMBOL_VAR,FOLLOW_RULE_SYMBOL_VAR_in_ruleName4859); 

                    		current.merge(this_SYMBOL_VAR_1);
                        
                     
                        createLeafNode(grammarAccess.getNameAccess().getSYMBOL_VARTerminalRuleCall_1(), null); 
                        

                    }
                    break;

            }


            }

             resetLookahead(); 
            	    lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleName


    // $ANTLR start ruleVISIBILITY
    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2790:1: ruleVISIBILITY returns [Enumerator current=null] : ( ( KEYWORD_29 ) | ( KEYWORD_24 ) ) ;
    public final Enumerator ruleVISIBILITY() throws RecognitionException {
        Enumerator current = null;

         setCurrentLookahead(); resetLookahead(); 
        try {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2794:6: ( ( ( KEYWORD_29 ) | ( KEYWORD_24 ) ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2795:1: ( ( KEYWORD_29 ) | ( KEYWORD_24 ) )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2795:1: ( ( KEYWORD_29 ) | ( KEYWORD_24 ) )
            int alt55=2;
            int LA55_0 = input.LA(1);

            if ( (LA55_0==KEYWORD_29) ) {
                alt55=1;
            }
            else if ( (LA55_0==KEYWORD_24) ) {
                alt55=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("2795:1: ( ( KEYWORD_29 ) | ( KEYWORD_24 ) )", 55, 0, input);

                throw nvae;
            }
            switch (alt55) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2795:2: ( KEYWORD_29 )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2795:2: ( KEYWORD_29 )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2795:7: KEYWORD_29
                    {
                    match(input,KEYWORD_29,FOLLOW_KEYWORD_29_in_ruleVISIBILITY4919); 

                            current = grammarAccess.getVISIBILITYAccess().getPRIVATEEnumLiteralDeclaration_0().getEnumLiteral().getInstance();
                            createLeafNode(grammarAccess.getVISIBILITYAccess().getPRIVATEEnumLiteralDeclaration_0(), null); 
                        

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2801:6: ( KEYWORD_24 )
                    {
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2801:6: ( KEYWORD_24 )
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/internal/InternalRexx.g:2801:11: KEYWORD_24
                    {
                    match(input,KEYWORD_24,FOLLOW_KEYWORD_24_in_ruleVISIBILITY4936); 

                            current = grammarAccess.getVISIBILITYAccess().getPUBLICEnumLiteralDeclaration_1().getEnumLiteral().getInstance();
                            createLeafNode(grammarAccess.getVISIBILITYAccess().getPUBLICEnumLiteralDeclaration_1(), null); 
                        

                    }


                    }
                    break;

            }


            }

             resetLookahead(); 
                	lastConsumedNode = currentNode;
                
        }
         
            catch (RecognitionException re) { 
                recover(input,re); 
                appendSkippedTokens();
            } 
        finally {
        }
        return current;
    }
    // $ANTLR end ruleVISIBILITY


 

    public static final BitSet FOLLOW_ruleFile_in_entryRuleFile67 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleFile77 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleFile121 = new BitSet(new long[]{0x0004C000E2602B42L});
    public static final BitSet FOLLOW_ruleInstruction_in_ruleFile143 = new BitSet(new long[]{0x00048000E2602B42L});
    public static final BitSet FOLLOW_ruleRequires_in_ruleFile165 = new BitSet(new long[]{0x0000000000002B42L});
    public static final BitSet FOLLOW_ruleMethod_in_ruleFile187 = new BitSet(new long[]{0x0000000000002B02L});
    public static final BitSet FOLLOW_ruleDirectives_in_ruleFile209 = new BitSet(new long[]{0x0000000000002302L});
    public static final BitSet FOLLOW_ruleInstruction_in_entryRuleInstruction245 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleInstruction255 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAssignment_in_ruleInstruction303 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_ruleDo_in_ruleInstruction330 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_ruleIf_in_ruleInstruction357 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_ruleCall_in_ruleInstruction384 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_ruleArg_in_ruleInstruction411 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_ruleUseArg_in_ruleInstruction438 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_ruleRaise_in_ruleInstruction465 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_ruleMessage_in_ruleInstruction492 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleInstruction502 = new BitSet(new long[]{0x0000400000000002L});
    public static final BitSet FOLLOW_ruleAssignment_in_entryRuleAssignment538 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAssignment548 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_ruleAssignment594 = new BitSet(new long[]{0x0000002000000000L});
    public static final BitSet FOLLOW_KEYWORD_6_in_ruleAssignment605 = new BitSet(new long[]{0x0017808000000000L});
    public static final BitSet FOLLOW_ruleExpression_in_ruleAssignment626 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleIf_in_entryRuleIf661 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleIf671 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_12_in_ruleIf707 = new BitSet(new long[]{0x0017808000000000L});
    public static final BitSet FOLLOW_ruleExpression_in_ruleIf728 = new BitSet(new long[]{0x0000000001000000L});
    public static final BitSet FOLLOW_KEYWORD_20_in_ruleIf739 = new BitSet(new long[]{0x0004C000E2600000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleIf749 = new BitSet(new long[]{0x0004C000E2600000L});
    public static final BitSet FOLLOW_ruleInstruction_in_ruleIf771 = new BitSet(new long[]{0x0000000000800002L});
    public static final BitSet FOLLOW_KEYWORD_19_in_ruleIf783 = new BitSet(new long[]{0x0004C000E2600000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleIf793 = new BitSet(new long[]{0x0004C000E2600000L});
    public static final BitSet FOLLOW_ruleInstruction_in_ruleIf815 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleDo_in_entryRuleDo852 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleDo862 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_11_in_ruleDo898 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleDo908 = new BitSet(new long[]{0x0004C000E2600000L});
    public static final BitSet FOLLOW_ruleInstruction_in_ruleDo930 = new BitSet(new long[]{0x00048000E6600000L});
    public static final BitSet FOLLOW_KEYWORD_14_in_ruleDo942 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleCall_in_entryRuleCall977 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleCall987 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_18_in_ruleCall1023 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleCall1044 = new BitSet(new long[]{0x0017808000000002L});
    public static final BitSet FOLLOW_ruleArguments_in_ruleCall1065 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArg_in_entryRuleArg1100 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleArg1110 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_13_in_ruleArg1155 = new BitSet(new long[]{0x0004800000000002L});
    public static final BitSet FOLLOW_ruleName_in_ruleArg1177 = new BitSet(new long[]{0x0000000400000002L});
    public static final BitSet FOLLOW_KEYWORD_3_in_ruleArg1189 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleArg1210 = new BitSet(new long[]{0x0000000400000002L});
    public static final BitSet FOLLOW_ruleUseArg_in_entryRuleUseArg1249 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleUseArg1259 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_17_in_ruleUseArg1304 = new BitSet(new long[]{0x0000000002040000L});
    public static final BitSet FOLLOW_KEYWORD_25_in_ruleUseArg1323 = new BitSet(new long[]{0x0000000002000000L});
    public static final BitSet FOLLOW_KEYWORD_13_in_ruleUseArg1348 = new BitSet(new long[]{0x0004800000000002L});
    public static final BitSet FOLLOW_ruleUseArgs_in_ruleUseArg1370 = new BitSet(new long[]{0x0000000400000002L});
    public static final BitSet FOLLOW_KEYWORD_3_in_ruleUseArg1382 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleUseArgs_in_ruleUseArg1403 = new BitSet(new long[]{0x0000000400000002L});
    public static final BitSet FOLLOW_ruleUseArgs_in_entryRuleUseArgs1442 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleUseArgs1452 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_ruleUseArgs1498 = new BitSet(new long[]{0x0000002000000002L});
    public static final BitSet FOLLOW_KEYWORD_6_in_ruleUseArgs1510 = new BitSet(new long[]{0x0017808000000000L});
    public static final BitSet FOLLOW_ruleExpression_in_ruleUseArgs1531 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArguments_in_entryRuleArguments1568 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleArguments1578 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_ruleArguments1634 = new BitSet(new long[]{0x0000000400000002L});
    public static final BitSet FOLLOW_KEYWORD_3_in_ruleArguments1646 = new BitSet(new long[]{0x0017808000000000L});
    public static final BitSet FOLLOW_ruleExpression_in_ruleArguments1667 = new BitSet(new long[]{0x0000000400000002L});
    public static final BitSet FOLLOW_ruleRaise_in_entryRuleRaise1706 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleRaise1716 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_22_in_ruleRaise1752 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_KEYWORD_26_in_ruleRaise1763 = new BitSet(new long[]{0x0010000000000000L});
    public static final BitSet FOLLOW_RULE_SYMBOL_NUM_in_ruleRaise1780 = new BitSet(new long[]{0x0000000800000000L});
    public static final BitSet FOLLOW_KEYWORD_4_in_ruleRaise1797 = new BitSet(new long[]{0x0010000000000000L});
    public static final BitSet FOLLOW_RULE_SYMBOL_NUM_in_ruleRaise1814 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_entryRuleExpression1855 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleExpression1865 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_ruleExpression1921 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rulefnCall_in_ruleExpression1950 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMessage_in_ruleExpression1977 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArrayAccess_in_ruleExpression2004 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleNegate_in_ruleExpression2031 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleConst_in_ruleExpression2058 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArrayAccess_in_entryRuleArrayAccess2092 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleArrayAccess2102 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_ruleArrayAccess2148 = new BitSet(new long[]{0x0000004000000000L});
    public static final BitSet FOLLOW_KEYWORD_7_in_ruleArrayAccess2159 = new BitSet(new long[]{0x0017808000000000L});
    public static final BitSet FOLLOW_ruleExpression_in_ruleArrayAccess2180 = new BitSet(new long[]{0x0000010000000000L});
    public static final BitSet FOLLOW_KEYWORD_9_in_ruleArrayAccess2191 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rulefnCall_in_entryRulefnCall2226 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRulefnCall2236 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rulefnCall2282 = new BitSet(new long[]{0x0000000100000000L});
    public static final BitSet FOLLOW_KEYWORD_1_in_rulefnCall2293 = new BitSet(new long[]{0x0017808200000000L});
    public static final BitSet FOLLOW_ruleArguments_in_rulefnCall2314 = new BitSet(new long[]{0x0000000200000000L});
    public static final BitSet FOLLOW_KEYWORD_2_in_rulefnCall2325 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMessage_in_entryRuleMessage2360 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleMessage2370 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_ruleMessage2416 = new BitSet(new long[]{0x0000020000000000L});
    public static final BitSet FOLLOW_KEYWORD_10_in_ruleMessage2427 = new BitSet(new long[]{0x0004820000000000L});
    public static final BitSet FOLLOW_KEYWORD_10_in_ruleMessage2446 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleMessage2481 = new BitSet(new long[]{0x0000003100000002L});
    public static final BitSet FOLLOW_KEYWORD_5_in_ruleMessage2493 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleMessage2514 = new BitSet(new long[]{0x0000002100000002L});
    public static final BitSet FOLLOW_KEYWORD_1_in_ruleMessage2529 = new BitSet(new long[]{0x0017808200000000L});
    public static final BitSet FOLLOW_ruleArguments_in_ruleMessage2550 = new BitSet(new long[]{0x0000000200000000L});
    public static final BitSet FOLLOW_KEYWORD_2_in_ruleMessage2561 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_6_in_ruleMessage2580 = new BitSet(new long[]{0x0017808000000000L});
    public static final BitSet FOLLOW_ruleExpression_in_ruleMessage2601 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleNegate_in_entryRuleNegate2639 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleNegate2649 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_8_in_ruleNegate2685 = new BitSet(new long[]{0x0017808000000000L});
    public static final BitSet FOLLOW_ruleExpression_in_ruleNegate2706 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleConst_in_entryRuleConst2741 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleConst2751 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_SYMBOL_NUM_in_ruleConst2804 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_STRING_BOOL_in_ruleConst2824 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_STRING_HEX_in_ruleConst2844 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleDirectives_in_entryRuleDirectives2887 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleDirectives2897 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleClass_in_ruleDirectives2944 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleOptions_in_ruleDirectives2971 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleRoutine_in_ruleDirectives2998 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleClass_in_entryRuleClass3032 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleClass3042 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_27_in_ruleClass3078 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleClass3099 = new BitSet(new long[]{0x000040000002D480L});
    public static final BitSet FOLLOW_KEYWORD_34_in_ruleClass3111 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleClass3132 = new BitSet(new long[]{0x000040000002D080L});
    public static final BitSet FOLLOW_KEYWORD_31_in_ruleClass3147 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleClass3168 = new BitSet(new long[]{0x000040000002C000L});
    public static final BitSet FOLLOW_KEYWORD_37_in_ruleClass3187 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleClass3208 = new BitSet(new long[]{0x000040000002C000L});
    public static final BitSet FOLLOW_ruleVISIBILITY_in_ruleClass3232 = new BitSet(new long[]{0x0000400000004000L});
    public static final BitSet FOLLOW_KEYWORD_28_in_ruleClass3245 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleClass3266 = new BitSet(new long[]{0x0004C00000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleClass3279 = new BitSet(new long[]{0x0000400000000832L});
    public static final BitSet FOLLOW_ruleClassContent_in_ruleClass3301 = new BitSet(new long[]{0x0000000000000832L});
    public static final BitSet FOLLOW_ruleClassContent_in_entryRuleClassContent3337 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleClassContent3347 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMethod_in_ruleClassContent3394 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttribute_in_ruleClassContent3421 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleConstant_in_ruleClassContent3448 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMethod_in_entryRuleMethod3482 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleMethod3492 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_30_in_ruleMethod3528 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleMethod3549 = new BitSet(new long[]{0x0000400000128000L});
    public static final BitSet FOLLOW_ruleVISIBILITY_in_ruleMethod3570 = new BitSet(new long[]{0x0000400000100000L});
    public static final BitSet FOLLOW_KEYWORD_21_in_ruleMethod3590 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleMethod3614 = new BitSet(new long[]{0x0004C000E2610002L});
    public static final BitSet FOLLOW_KEYWORD_23_in_ruleMethod3627 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleMethod3648 = new BitSet(new long[]{0x0004C00000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleMethod3659 = new BitSet(new long[]{0x0004C000E2600002L});
    public static final BitSet FOLLOW_ruleInstruction_in_ruleMethod3683 = new BitSet(new long[]{0x00048000E2600002L});
    public static final BitSet FOLLOW_ruleAttribute_in_entryRuleAttribute3719 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAttribute3729 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeSet_in_ruleAttribute3777 = new BitSet(new long[]{0x0000400000128000L});
    public static final BitSet FOLLOW_ruleAttributeGet_in_ruleAttribute3804 = new BitSet(new long[]{0x0000400000128000L});
    public static final BitSet FOLLOW_ruleAttributeNormal_in_ruleAttribute3831 = new BitSet(new long[]{0x0000400000128000L});
    public static final BitSet FOLLOW_ruleVISIBILITY_in_ruleAttribute3852 = new BitSet(new long[]{0x0000400000100000L});
    public static final BitSet FOLLOW_KEYWORD_21_in_ruleAttribute3872 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleAttribute3896 = new BitSet(new long[]{0x0004C000E2610002L});
    public static final BitSet FOLLOW_KEYWORD_23_in_ruleAttribute3909 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleAttribute3930 = new BitSet(new long[]{0x0004C00000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleAttribute3941 = new BitSet(new long[]{0x0004C000E2600002L});
    public static final BitSet FOLLOW_ruleInstruction_in_ruleAttribute3965 = new BitSet(new long[]{0x00048000E2600002L});
    public static final BitSet FOLLOW_ruleAttributeSet_in_entryRuleAttributeSet4001 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAttributeSet4011 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_38_in_ruleAttributeSet4047 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleAttributeSet4068 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_KEYWORD_16_in_ruleAttributeSet4079 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeGet_in_entryRuleAttributeGet4114 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAttributeGet4124 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_38_in_ruleAttributeGet4160 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleAttributeGet4181 = new BitSet(new long[]{0x0000000008000000L});
    public static final BitSet FOLLOW_KEYWORD_15_in_ruleAttributeGet4192 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeNormal_in_entryRuleAttributeNormal4227 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAttributeNormal4237 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_38_in_ruleAttributeNormal4273 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleAttributeNormal4294 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleConstant_in_entryRuleConstant4329 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleConstant4339 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_35_in_ruleConstant4375 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleConstant4396 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleConstant4406 = new BitSet(new long[]{0x0000400000000002L});
    public static final BitSet FOLLOW_ruleRoutine_in_entryRuleRoutine4442 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleRoutine4452 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_33_in_ruleRoutine4488 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleRoutine4509 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleRoutine4519 = new BitSet(new long[]{0x0000400000000002L});
    public static final BitSet FOLLOW_ruleRequires_in_entryRuleRequires4555 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleRequires4565 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_36_in_ruleRequires4601 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleRequires4622 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleRequires4632 = new BitSet(new long[]{0x0000400000000002L});
    public static final BitSet FOLLOW_ruleOptions_in_entryRuleOptions4668 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleOptions4678 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_32_in_ruleOptions4714 = new BitSet(new long[]{0x0004800000000000L});
    public static final BitSet FOLLOW_ruleName_in_ruleOptions4735 = new BitSet(new long[]{0x0000400000000000L});
    public static final BitSet FOLLOW_RULE_TERM_in_ruleOptions4745 = new BitSet(new long[]{0x0000400000000002L});
    public static final BitSet FOLLOW_ruleName_in_entryRuleName4782 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleName4793 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_STRING_in_ruleName4833 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_SYMBOL_VAR_in_ruleName4859 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_29_in_ruleVISIBILITY4919 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_KEYWORD_24_in_ruleVISIBILITY4936 = new BitSet(new long[]{0x0000000000000002L});

}