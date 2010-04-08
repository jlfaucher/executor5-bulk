package org.oorexx.workbench.ui.contentassist.antlr.internal; 

import java.io.InputStream;
import org.eclipse.xtext.*;
import org.eclipse.xtext.parser.*;
import org.eclipse.xtext.parser.impl.*;
import org.eclipse.xtext.parsetree.*;
import org.eclipse.emf.ecore.util.EcoreUtil;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.xtext.parser.antlr.XtextTokenStream;
import org.eclipse.xtext.parser.antlr.XtextTokenStream.HiddenTokens;
import org.eclipse.xtext.ui.editor.contentassist.antlr.internal.AbstractInternalContentAssistParser;
import org.oorexx.workbench.services.RexxGrammarAccess;



import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

public class InternalRexxParser extends AbstractInternalContentAssistParser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "RULE_SYMBOL_NUM", "RULE_STRING_BOOL", "RULE_STRING_HEX", "RULE_STRING", "RULE_SYMBOL_VAR", "RULE_TERM", "RULE_WS", "RULE_ML_COMMENT", "RULE_SL_COMMENT", "RULE_NL", "RULE_SYMBOL_CLS", "RULE_OP_PLUS", "RULE_OP_MINUS", "RULE_OP_SLASH", "RULE_OP_PERCENT", "RULE_OP_STAR", "RULE_OP_PIPE", "RULE_OP_ET", "RULE_OP_NEG", "RULE_OP_GT", "RULE_OP_LT", "RULE_OP_GEQ", "RULE_OP_LEQ", "RULE_OP_BSGT", "RULE_OP_BSLT", "RULE_OP_BSEQ", "RULE_OP_GTLT", "RULE_OP_LTGT", "RULE_OP_EQ2", "RULE_OP_BSEQ2", "RULE_OP_SLASH2", "RULE_OP_AND2", "RULE_OP_PIPE2", "RULE_OP_STAR2", "RULE_OP_NGT", "RULE_OP_NLT", "RULE_OP_NEQ", "RULE_OP_NEQ2", "RULE_OP_GT2", "RULE_OP_LT2", "RULE_OP_GT2EQ", "RULE_OP_BSLT2", "RULE_OP_NLT2", "RULE_OP_BSGT2", "RULE_OP_NGT2", "RULE_OP_LT2EQ", "RULE_OP_PLUSEQ", "RULE_OP_MINUSEQ", "RULE_OP_STAREQ", "RULE_OP_SLASHEQ", "RULE_OP_PERCENTEQ", "RULE_OP_SLAHS2EQ", "RULE_OP_PIPE2EQ", "RULE_OP_ANDEQ", "RULE_OP_PIPEEQ", "RULE_OP_AND2EQ", "'private'", "'public'", "'='", "'if'", "'then'", "'else'", "'do'", "'end'", "'call'", "'arg'", "','", "'use'", "'raise'", "'syntax'", "'.'", "'['", "']'", "'('", "')'", "'~'", "':'", "'\\\\'", "'::class'", "'metaclass'", "'subclass'", "'mixinclass'", "'inherit'", "'::method'", "'expose'", "'::attribute'", "'set'", "'get'", "'::constant'", "'::routine'", "'::requires'", "'::options'", "'strict'", "'class'"
    };
    public static final int RULE_OP_NGT=38;
    public static final int RULE_SYMBOL_VAR=8;
    public static final int RULE_SYMBOL_CLS=14;
    public static final int RULE_OP_GT=23;
    public static final int RULE_OP_PIPE=20;
    public static final int RULE_OP_SLASHEQ=53;
    public static final int RULE_OP_GTLT=30;
    public static final int RULE_OP_SLASH2=34;
    public static final int RULE_STRING_BOOL=5;
    public static final int RULE_OP_PERCENTEQ=54;
    public static final int RULE_OP_BSEQ=29;
    public static final int RULE_OP_STAREQ=52;
    public static final int RULE_OP_BSEQ2=33;
    public static final int RULE_OP_NLT=39;
    public static final int RULE_OP_LT=24;
    public static final int RULE_OP_STAR2=37;
    public static final int RULE_OP_ANDEQ=57;
    public static final int RULE_OP_GT2=42;
    public static final int RULE_OP_STAR=19;
    public static final int RULE_OP_NGT2=48;
    public static final int RULE_STRING=7;
    public static final int RULE_OP_PIPE2EQ=56;
    public static final int RULE_OP_NEG=22;
    public static final int RULE_OP_AND2=35;
    public static final int RULE_OP_LEQ=26;
    public static final int RULE_OP_BSLT2=45;
    public static final int RULE_OP_NEQ=40;
    public static final int RULE_OP_MINUSEQ=51;
    public static final int RULE_ML_COMMENT=11;
    public static final int RULE_OP_SLAHS2EQ=55;
    public static final int RULE_OP_PIPE2=36;
    public static final int RULE_OP_ET=21;
    public static final int RULE_OP_EQ2=32;
    public static final int RULE_TERM=9;
    public static final int RULE_OP_LTGT=31;
    public static final int RULE_OP_NEQ2=41;
    public static final int RULE_OP_MINUS=16;
    public static final int RULE_OP_GT2EQ=44;
    public static final int RULE_OP_BSGT=27;
    public static final int RULE_OP_LT2EQ=49;
    public static final int EOF=-1;
    public static final int RULE_OP_PLUSEQ=50;
    public static final int RULE_NL=13;
    public static final int RULE_OP_AND2EQ=59;
    public static final int RULE_OP_SLASH=17;
    public static final int RULE_SYMBOL_NUM=4;
    public static final int RULE_OP_PERCENT=18;
    public static final int RULE_OP_LT2=43;
    public static final int RULE_STRING_HEX=6;
    public static final int RULE_OP_BSLT=28;
    public static final int RULE_OP_PIPEEQ=58;
    public static final int RULE_OP_PLUS=15;
    public static final int RULE_OP_NLT2=46;
    public static final int RULE_OP_GEQ=25;
    public static final int RULE_OP_BSGT2=47;
    public static final int RULE_SL_COMMENT=12;
    public static final int RULE_WS=10;

        public InternalRexxParser(TokenStream input) {
            super(input);
        }
        

    public String[] getTokenNames() { return tokenNames; }
    public String getGrammarFileName() { return "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g"; }


     
     	private RexxGrammarAccess grammarAccess;
     	
        public void setGrammarAccess(RexxGrammarAccess grammarAccess) {
        	this.grammarAccess = grammarAccess;
        }
        
        @Override
        protected Grammar getGrammar() {
        	return grammarAccess.getGrammar();
        }
        
        @Override
        protected String getValueForTokenName(String tokenName) {
        	return tokenName;
        }




    // $ANTLR start entryRuleFile
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:60:1: entryRuleFile : ruleFile EOF ;
    public final void entryRuleFile() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:61:1: ( ruleFile EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:62:1: ruleFile EOF
            {
             before(grammarAccess.getFileRule()); 
            pushFollow(FOLLOW_ruleFile_in_entryRuleFile61);
            ruleFile();
            _fsp--;

             after(grammarAccess.getFileRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleFile68); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleFile


    // $ANTLR start ruleFile
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:69:1: ruleFile : ( ( rule__File__Group__0 ) ) ;
    public final void ruleFile() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:73:2: ( ( ( rule__File__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:74:1: ( ( rule__File__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:74:1: ( ( rule__File__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:75:1: ( rule__File__Group__0 )
            {
             before(grammarAccess.getFileAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:76:1: ( rule__File__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:76:2: rule__File__Group__0
            {
            pushFollow(FOLLOW_rule__File__Group__0_in_ruleFile94);
            rule__File__Group__0();
            _fsp--;


            }

             after(grammarAccess.getFileAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleFile


    // $ANTLR start entryRuleInstruction
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:88:1: entryRuleInstruction : ruleInstruction EOF ;
    public final void entryRuleInstruction() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:89:1: ( ruleInstruction EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:90:1: ruleInstruction EOF
            {
             before(grammarAccess.getInstructionRule()); 
            pushFollow(FOLLOW_ruleInstruction_in_entryRuleInstruction121);
            ruleInstruction();
            _fsp--;

             after(grammarAccess.getInstructionRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleInstruction128); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleInstruction


    // $ANTLR start ruleInstruction
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:97:1: ruleInstruction : ( ( rule__Instruction__Group__0 ) ) ;
    public final void ruleInstruction() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:101:2: ( ( ( rule__Instruction__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:102:1: ( ( rule__Instruction__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:102:1: ( ( rule__Instruction__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:103:1: ( rule__Instruction__Group__0 )
            {
             before(grammarAccess.getInstructionAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:104:1: ( rule__Instruction__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:104:2: rule__Instruction__Group__0
            {
            pushFollow(FOLLOW_rule__Instruction__Group__0_in_ruleInstruction154);
            rule__Instruction__Group__0();
            _fsp--;


            }

             after(grammarAccess.getInstructionAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleInstruction


    // $ANTLR start entryRuleAssignment
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:116:1: entryRuleAssignment : ruleAssignment EOF ;
    public final void entryRuleAssignment() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:117:1: ( ruleAssignment EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:118:1: ruleAssignment EOF
            {
             before(grammarAccess.getAssignmentRule()); 
            pushFollow(FOLLOW_ruleAssignment_in_entryRuleAssignment181);
            ruleAssignment();
            _fsp--;

             after(grammarAccess.getAssignmentRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAssignment188); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleAssignment


    // $ANTLR start ruleAssignment
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:125:1: ruleAssignment : ( ( rule__Assignment__Group__0 ) ) ;
    public final void ruleAssignment() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:129:2: ( ( ( rule__Assignment__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:130:1: ( ( rule__Assignment__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:130:1: ( ( rule__Assignment__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:131:1: ( rule__Assignment__Group__0 )
            {
             before(grammarAccess.getAssignmentAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:132:1: ( rule__Assignment__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:132:2: rule__Assignment__Group__0
            {
            pushFollow(FOLLOW_rule__Assignment__Group__0_in_ruleAssignment214);
            rule__Assignment__Group__0();
            _fsp--;


            }

             after(grammarAccess.getAssignmentAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleAssignment


    // $ANTLR start entryRuleIf
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:144:1: entryRuleIf : ruleIf EOF ;
    public final void entryRuleIf() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:145:1: ( ruleIf EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:146:1: ruleIf EOF
            {
             before(grammarAccess.getIfRule()); 
            pushFollow(FOLLOW_ruleIf_in_entryRuleIf241);
            ruleIf();
            _fsp--;

             after(grammarAccess.getIfRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleIf248); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleIf


    // $ANTLR start ruleIf
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:153:1: ruleIf : ( ( rule__If__Group__0 ) ) ;
    public final void ruleIf() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:157:2: ( ( ( rule__If__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:158:1: ( ( rule__If__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:158:1: ( ( rule__If__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:159:1: ( rule__If__Group__0 )
            {
             before(grammarAccess.getIfAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:160:1: ( rule__If__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:160:2: rule__If__Group__0
            {
            pushFollow(FOLLOW_rule__If__Group__0_in_ruleIf274);
            rule__If__Group__0();
            _fsp--;


            }

             after(grammarAccess.getIfAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleIf


    // $ANTLR start entryRuleDo
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:172:1: entryRuleDo : ruleDo EOF ;
    public final void entryRuleDo() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:173:1: ( ruleDo EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:174:1: ruleDo EOF
            {
             before(grammarAccess.getDoRule()); 
            pushFollow(FOLLOW_ruleDo_in_entryRuleDo301);
            ruleDo();
            _fsp--;

             after(grammarAccess.getDoRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleDo308); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleDo


    // $ANTLR start ruleDo
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:181:1: ruleDo : ( ( rule__Do__Group__0 ) ) ;
    public final void ruleDo() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:185:2: ( ( ( rule__Do__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:186:1: ( ( rule__Do__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:186:1: ( ( rule__Do__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:187:1: ( rule__Do__Group__0 )
            {
             before(grammarAccess.getDoAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:188:1: ( rule__Do__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:188:2: rule__Do__Group__0
            {
            pushFollow(FOLLOW_rule__Do__Group__0_in_ruleDo334);
            rule__Do__Group__0();
            _fsp--;


            }

             after(grammarAccess.getDoAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleDo


    // $ANTLR start entryRuleCall
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:200:1: entryRuleCall : ruleCall EOF ;
    public final void entryRuleCall() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:201:1: ( ruleCall EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:202:1: ruleCall EOF
            {
             before(grammarAccess.getCallRule()); 
            pushFollow(FOLLOW_ruleCall_in_entryRuleCall361);
            ruleCall();
            _fsp--;

             after(grammarAccess.getCallRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleCall368); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleCall


    // $ANTLR start ruleCall
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:209:1: ruleCall : ( ( rule__Call__Group__0 ) ) ;
    public final void ruleCall() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:213:2: ( ( ( rule__Call__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:214:1: ( ( rule__Call__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:214:1: ( ( rule__Call__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:215:1: ( rule__Call__Group__0 )
            {
             before(grammarAccess.getCallAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:216:1: ( rule__Call__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:216:2: rule__Call__Group__0
            {
            pushFollow(FOLLOW_rule__Call__Group__0_in_ruleCall394);
            rule__Call__Group__0();
            _fsp--;


            }

             after(grammarAccess.getCallAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleCall


    // $ANTLR start entryRuleArg
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:228:1: entryRuleArg : ruleArg EOF ;
    public final void entryRuleArg() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:229:1: ( ruleArg EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:230:1: ruleArg EOF
            {
             before(grammarAccess.getArgRule()); 
            pushFollow(FOLLOW_ruleArg_in_entryRuleArg421);
            ruleArg();
            _fsp--;

             after(grammarAccess.getArgRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleArg428); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleArg


    // $ANTLR start ruleArg
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:237:1: ruleArg : ( ( rule__Arg__Group__0 ) ) ;
    public final void ruleArg() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:241:2: ( ( ( rule__Arg__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:242:1: ( ( rule__Arg__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:242:1: ( ( rule__Arg__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:243:1: ( rule__Arg__Group__0 )
            {
             before(grammarAccess.getArgAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:244:1: ( rule__Arg__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:244:2: rule__Arg__Group__0
            {
            pushFollow(FOLLOW_rule__Arg__Group__0_in_ruleArg454);
            rule__Arg__Group__0();
            _fsp--;


            }

             after(grammarAccess.getArgAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleArg


    // $ANTLR start entryRuleUseArg
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:256:1: entryRuleUseArg : ruleUseArg EOF ;
    public final void entryRuleUseArg() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:257:1: ( ruleUseArg EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:258:1: ruleUseArg EOF
            {
             before(grammarAccess.getUseArgRule()); 
            pushFollow(FOLLOW_ruleUseArg_in_entryRuleUseArg481);
            ruleUseArg();
            _fsp--;

             after(grammarAccess.getUseArgRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleUseArg488); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleUseArg


    // $ANTLR start ruleUseArg
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:265:1: ruleUseArg : ( ( rule__UseArg__Group__0 ) ) ;
    public final void ruleUseArg() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:269:2: ( ( ( rule__UseArg__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:270:1: ( ( rule__UseArg__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:270:1: ( ( rule__UseArg__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:271:1: ( rule__UseArg__Group__0 )
            {
             before(grammarAccess.getUseArgAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:272:1: ( rule__UseArg__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:272:2: rule__UseArg__Group__0
            {
            pushFollow(FOLLOW_rule__UseArg__Group__0_in_ruleUseArg514);
            rule__UseArg__Group__0();
            _fsp--;


            }

             after(grammarAccess.getUseArgAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleUseArg


    // $ANTLR start entryRuleUseArgs
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:284:1: entryRuleUseArgs : ruleUseArgs EOF ;
    public final void entryRuleUseArgs() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:285:1: ( ruleUseArgs EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:286:1: ruleUseArgs EOF
            {
             before(grammarAccess.getUseArgsRule()); 
            pushFollow(FOLLOW_ruleUseArgs_in_entryRuleUseArgs541);
            ruleUseArgs();
            _fsp--;

             after(grammarAccess.getUseArgsRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleUseArgs548); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleUseArgs


    // $ANTLR start ruleUseArgs
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:293:1: ruleUseArgs : ( ( rule__UseArgs__Group__0 ) ) ;
    public final void ruleUseArgs() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:297:2: ( ( ( rule__UseArgs__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:298:1: ( ( rule__UseArgs__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:298:1: ( ( rule__UseArgs__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:299:1: ( rule__UseArgs__Group__0 )
            {
             before(grammarAccess.getUseArgsAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:300:1: ( rule__UseArgs__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:300:2: rule__UseArgs__Group__0
            {
            pushFollow(FOLLOW_rule__UseArgs__Group__0_in_ruleUseArgs574);
            rule__UseArgs__Group__0();
            _fsp--;


            }

             after(grammarAccess.getUseArgsAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleUseArgs


    // $ANTLR start entryRuleArguments
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:312:1: entryRuleArguments : ruleArguments EOF ;
    public final void entryRuleArguments() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:313:1: ( ruleArguments EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:314:1: ruleArguments EOF
            {
             before(grammarAccess.getArgumentsRule()); 
            pushFollow(FOLLOW_ruleArguments_in_entryRuleArguments601);
            ruleArguments();
            _fsp--;

             after(grammarAccess.getArgumentsRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleArguments608); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleArguments


    // $ANTLR start ruleArguments
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:321:1: ruleArguments : ( ( rule__Arguments__Group__0 ) ) ;
    public final void ruleArguments() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:325:2: ( ( ( rule__Arguments__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:326:1: ( ( rule__Arguments__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:326:1: ( ( rule__Arguments__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:327:1: ( rule__Arguments__Group__0 )
            {
             before(grammarAccess.getArgumentsAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:328:1: ( rule__Arguments__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:328:2: rule__Arguments__Group__0
            {
            pushFollow(FOLLOW_rule__Arguments__Group__0_in_ruleArguments634);
            rule__Arguments__Group__0();
            _fsp--;


            }

             after(grammarAccess.getArgumentsAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleArguments


    // $ANTLR start entryRuleRaise
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:340:1: entryRuleRaise : ruleRaise EOF ;
    public final void entryRuleRaise() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:341:1: ( ruleRaise EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:342:1: ruleRaise EOF
            {
             before(grammarAccess.getRaiseRule()); 
            pushFollow(FOLLOW_ruleRaise_in_entryRuleRaise661);
            ruleRaise();
            _fsp--;

             after(grammarAccess.getRaiseRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleRaise668); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleRaise


    // $ANTLR start ruleRaise
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:349:1: ruleRaise : ( ( rule__Raise__Group__0 ) ) ;
    public final void ruleRaise() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:353:2: ( ( ( rule__Raise__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:354:1: ( ( rule__Raise__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:354:1: ( ( rule__Raise__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:355:1: ( rule__Raise__Group__0 )
            {
             before(grammarAccess.getRaiseAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:356:1: ( rule__Raise__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:356:2: rule__Raise__Group__0
            {
            pushFollow(FOLLOW_rule__Raise__Group__0_in_ruleRaise694);
            rule__Raise__Group__0();
            _fsp--;


            }

             after(grammarAccess.getRaiseAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleRaise


    // $ANTLR start entryRuleExpression
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:368:1: entryRuleExpression : ruleExpression EOF ;
    public final void entryRuleExpression() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:369:1: ( ruleExpression EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:370:1: ruleExpression EOF
            {
             before(grammarAccess.getExpressionRule()); 
            pushFollow(FOLLOW_ruleExpression_in_entryRuleExpression721);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getExpressionRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleExpression728); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleExpression


    // $ANTLR start ruleExpression
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:377:1: ruleExpression : ( ( rule__Expression__Alternatives ) ) ;
    public final void ruleExpression() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:381:2: ( ( ( rule__Expression__Alternatives ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:382:1: ( ( rule__Expression__Alternatives ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:382:1: ( ( rule__Expression__Alternatives ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:383:1: ( rule__Expression__Alternatives )
            {
             before(grammarAccess.getExpressionAccess().getAlternatives()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:384:1: ( rule__Expression__Alternatives )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:384:2: rule__Expression__Alternatives
            {
            pushFollow(FOLLOW_rule__Expression__Alternatives_in_ruleExpression754);
            rule__Expression__Alternatives();
            _fsp--;


            }

             after(grammarAccess.getExpressionAccess().getAlternatives()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleExpression


    // $ANTLR start entryRuleArrayAccess
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:396:1: entryRuleArrayAccess : ruleArrayAccess EOF ;
    public final void entryRuleArrayAccess() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:397:1: ( ruleArrayAccess EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:398:1: ruleArrayAccess EOF
            {
             before(grammarAccess.getArrayAccessRule()); 
            pushFollow(FOLLOW_ruleArrayAccess_in_entryRuleArrayAccess781);
            ruleArrayAccess();
            _fsp--;

             after(grammarAccess.getArrayAccessRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleArrayAccess788); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleArrayAccess


    // $ANTLR start ruleArrayAccess
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:405:1: ruleArrayAccess : ( ( rule__ArrayAccess__Group__0 ) ) ;
    public final void ruleArrayAccess() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:409:2: ( ( ( rule__ArrayAccess__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:410:1: ( ( rule__ArrayAccess__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:410:1: ( ( rule__ArrayAccess__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:411:1: ( rule__ArrayAccess__Group__0 )
            {
             before(grammarAccess.getArrayAccessAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:412:1: ( rule__ArrayAccess__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:412:2: rule__ArrayAccess__Group__0
            {
            pushFollow(FOLLOW_rule__ArrayAccess__Group__0_in_ruleArrayAccess814);
            rule__ArrayAccess__Group__0();
            _fsp--;


            }

             after(grammarAccess.getArrayAccessAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleArrayAccess


    // $ANTLR start entryRulefnCall
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:424:1: entryRulefnCall : rulefnCall EOF ;
    public final void entryRulefnCall() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:425:1: ( rulefnCall EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:426:1: rulefnCall EOF
            {
             before(grammarAccess.getFnCallRule()); 
            pushFollow(FOLLOW_rulefnCall_in_entryRulefnCall841);
            rulefnCall();
            _fsp--;

             after(grammarAccess.getFnCallRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRulefnCall848); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRulefnCall


    // $ANTLR start rulefnCall
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:433:1: rulefnCall : ( ( rule__FnCall__Group__0 ) ) ;
    public final void rulefnCall() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:437:2: ( ( ( rule__FnCall__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:438:1: ( ( rule__FnCall__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:438:1: ( ( rule__FnCall__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:439:1: ( rule__FnCall__Group__0 )
            {
             before(grammarAccess.getFnCallAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:440:1: ( rule__FnCall__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:440:2: rule__FnCall__Group__0
            {
            pushFollow(FOLLOW_rule__FnCall__Group__0_in_rulefnCall874);
            rule__FnCall__Group__0();
            _fsp--;


            }

             after(grammarAccess.getFnCallAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rulefnCall


    // $ANTLR start entryRuleMessage
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:452:1: entryRuleMessage : ruleMessage EOF ;
    public final void entryRuleMessage() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:453:1: ( ruleMessage EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:454:1: ruleMessage EOF
            {
             before(grammarAccess.getMessageRule()); 
            pushFollow(FOLLOW_ruleMessage_in_entryRuleMessage901);
            ruleMessage();
            _fsp--;

             after(grammarAccess.getMessageRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleMessage908); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleMessage


    // $ANTLR start ruleMessage
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:461:1: ruleMessage : ( ( rule__Message__Group__0 ) ) ;
    public final void ruleMessage() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:465:2: ( ( ( rule__Message__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:466:1: ( ( rule__Message__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:466:1: ( ( rule__Message__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:467:1: ( rule__Message__Group__0 )
            {
             before(grammarAccess.getMessageAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:468:1: ( rule__Message__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:468:2: rule__Message__Group__0
            {
            pushFollow(FOLLOW_rule__Message__Group__0_in_ruleMessage934);
            rule__Message__Group__0();
            _fsp--;


            }

             after(grammarAccess.getMessageAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleMessage


    // $ANTLR start entryRuleNegate
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:480:1: entryRuleNegate : ruleNegate EOF ;
    public final void entryRuleNegate() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:481:1: ( ruleNegate EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:482:1: ruleNegate EOF
            {
             before(grammarAccess.getNegateRule()); 
            pushFollow(FOLLOW_ruleNegate_in_entryRuleNegate961);
            ruleNegate();
            _fsp--;

             after(grammarAccess.getNegateRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleNegate968); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleNegate


    // $ANTLR start ruleNegate
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:489:1: ruleNegate : ( ( rule__Negate__Group__0 ) ) ;
    public final void ruleNegate() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:493:2: ( ( ( rule__Negate__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:494:1: ( ( rule__Negate__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:494:1: ( ( rule__Negate__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:495:1: ( rule__Negate__Group__0 )
            {
             before(grammarAccess.getNegateAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:496:1: ( rule__Negate__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:496:2: rule__Negate__Group__0
            {
            pushFollow(FOLLOW_rule__Negate__Group__0_in_ruleNegate994);
            rule__Negate__Group__0();
            _fsp--;


            }

             after(grammarAccess.getNegateAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleNegate


    // $ANTLR start entryRuleConst
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:508:1: entryRuleConst : ruleConst EOF ;
    public final void entryRuleConst() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:509:1: ( ruleConst EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:510:1: ruleConst EOF
            {
             before(grammarAccess.getConstRule()); 
            pushFollow(FOLLOW_ruleConst_in_entryRuleConst1021);
            ruleConst();
            _fsp--;

             after(grammarAccess.getConstRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleConst1028); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleConst


    // $ANTLR start ruleConst
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:517:1: ruleConst : ( ( rule__Const__Group__0 ) ) ;
    public final void ruleConst() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:521:2: ( ( ( rule__Const__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:522:1: ( ( rule__Const__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:522:1: ( ( rule__Const__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:523:1: ( rule__Const__Group__0 )
            {
             before(grammarAccess.getConstAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:524:1: ( rule__Const__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:524:2: rule__Const__Group__0
            {
            pushFollow(FOLLOW_rule__Const__Group__0_in_ruleConst1054);
            rule__Const__Group__0();
            _fsp--;


            }

             after(grammarAccess.getConstAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleConst


    // $ANTLR start entryRuleDirectives
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:536:1: entryRuleDirectives : ruleDirectives EOF ;
    public final void entryRuleDirectives() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:537:1: ( ruleDirectives EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:538:1: ruleDirectives EOF
            {
             before(grammarAccess.getDirectivesRule()); 
            pushFollow(FOLLOW_ruleDirectives_in_entryRuleDirectives1081);
            ruleDirectives();
            _fsp--;

             after(grammarAccess.getDirectivesRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleDirectives1088); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleDirectives


    // $ANTLR start ruleDirectives
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:545:1: ruleDirectives : ( ( rule__Directives__Alternatives ) ) ;
    public final void ruleDirectives() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:549:2: ( ( ( rule__Directives__Alternatives ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:550:1: ( ( rule__Directives__Alternatives ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:550:1: ( ( rule__Directives__Alternatives ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:551:1: ( rule__Directives__Alternatives )
            {
             before(grammarAccess.getDirectivesAccess().getAlternatives()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:552:1: ( rule__Directives__Alternatives )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:552:2: rule__Directives__Alternatives
            {
            pushFollow(FOLLOW_rule__Directives__Alternatives_in_ruleDirectives1114);
            rule__Directives__Alternatives();
            _fsp--;


            }

             after(grammarAccess.getDirectivesAccess().getAlternatives()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleDirectives


    // $ANTLR start entryRuleClass
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:564:1: entryRuleClass : ruleClass EOF ;
    public final void entryRuleClass() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:565:1: ( ruleClass EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:566:1: ruleClass EOF
            {
             before(grammarAccess.getClassRule()); 
            pushFollow(FOLLOW_ruleClass_in_entryRuleClass1141);
            ruleClass();
            _fsp--;

             after(grammarAccess.getClassRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleClass1148); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleClass


    // $ANTLR start ruleClass
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:573:1: ruleClass : ( ( rule__Class__Group__0 ) ) ;
    public final void ruleClass() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:577:2: ( ( ( rule__Class__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:578:1: ( ( rule__Class__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:578:1: ( ( rule__Class__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:579:1: ( rule__Class__Group__0 )
            {
             before(grammarAccess.getClassAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:580:1: ( rule__Class__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:580:2: rule__Class__Group__0
            {
            pushFollow(FOLLOW_rule__Class__Group__0_in_ruleClass1174);
            rule__Class__Group__0();
            _fsp--;


            }

             after(grammarAccess.getClassAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleClass


    // $ANTLR start entryRuleClassContent
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:592:1: entryRuleClassContent : ruleClassContent EOF ;
    public final void entryRuleClassContent() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:593:1: ( ruleClassContent EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:594:1: ruleClassContent EOF
            {
             before(grammarAccess.getClassContentRule()); 
            pushFollow(FOLLOW_ruleClassContent_in_entryRuleClassContent1201);
            ruleClassContent();
            _fsp--;

             after(grammarAccess.getClassContentRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleClassContent1208); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleClassContent


    // $ANTLR start ruleClassContent
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:601:1: ruleClassContent : ( ( rule__ClassContent__Alternatives ) ) ;
    public final void ruleClassContent() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:605:2: ( ( ( rule__ClassContent__Alternatives ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:606:1: ( ( rule__ClassContent__Alternatives ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:606:1: ( ( rule__ClassContent__Alternatives ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:607:1: ( rule__ClassContent__Alternatives )
            {
             before(grammarAccess.getClassContentAccess().getAlternatives()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:608:1: ( rule__ClassContent__Alternatives )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:608:2: rule__ClassContent__Alternatives
            {
            pushFollow(FOLLOW_rule__ClassContent__Alternatives_in_ruleClassContent1234);
            rule__ClassContent__Alternatives();
            _fsp--;


            }

             after(grammarAccess.getClassContentAccess().getAlternatives()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleClassContent


    // $ANTLR start entryRuleMethod
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:620:1: entryRuleMethod : ruleMethod EOF ;
    public final void entryRuleMethod() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:621:1: ( ruleMethod EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:622:1: ruleMethod EOF
            {
             before(grammarAccess.getMethodRule()); 
            pushFollow(FOLLOW_ruleMethod_in_entryRuleMethod1261);
            ruleMethod();
            _fsp--;

             after(grammarAccess.getMethodRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleMethod1268); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleMethod


    // $ANTLR start ruleMethod
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:629:1: ruleMethod : ( ( rule__Method__Group__0 ) ) ;
    public final void ruleMethod() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:633:2: ( ( ( rule__Method__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:634:1: ( ( rule__Method__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:634:1: ( ( rule__Method__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:635:1: ( rule__Method__Group__0 )
            {
             before(grammarAccess.getMethodAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:636:1: ( rule__Method__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:636:2: rule__Method__Group__0
            {
            pushFollow(FOLLOW_rule__Method__Group__0_in_ruleMethod1294);
            rule__Method__Group__0();
            _fsp--;


            }

             after(grammarAccess.getMethodAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleMethod


    // $ANTLR start entryRuleAttribute
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:648:1: entryRuleAttribute : ruleAttribute EOF ;
    public final void entryRuleAttribute() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:649:1: ( ruleAttribute EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:650:1: ruleAttribute EOF
            {
             before(grammarAccess.getAttributeRule()); 
            pushFollow(FOLLOW_ruleAttribute_in_entryRuleAttribute1321);
            ruleAttribute();
            _fsp--;

             after(grammarAccess.getAttributeRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAttribute1328); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleAttribute


    // $ANTLR start ruleAttribute
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:657:1: ruleAttribute : ( ( rule__Attribute__Group__0 ) ) ;
    public final void ruleAttribute() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:661:2: ( ( ( rule__Attribute__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:662:1: ( ( rule__Attribute__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:662:1: ( ( rule__Attribute__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:663:1: ( rule__Attribute__Group__0 )
            {
             before(grammarAccess.getAttributeAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:664:1: ( rule__Attribute__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:664:2: rule__Attribute__Group__0
            {
            pushFollow(FOLLOW_rule__Attribute__Group__0_in_ruleAttribute1354);
            rule__Attribute__Group__0();
            _fsp--;


            }

             after(grammarAccess.getAttributeAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleAttribute


    // $ANTLR start entryRuleAttributeSet
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:676:1: entryRuleAttributeSet : ruleAttributeSet EOF ;
    public final void entryRuleAttributeSet() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:677:1: ( ruleAttributeSet EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:678:1: ruleAttributeSet EOF
            {
             before(grammarAccess.getAttributeSetRule()); 
            pushFollow(FOLLOW_ruleAttributeSet_in_entryRuleAttributeSet1381);
            ruleAttributeSet();
            _fsp--;

             after(grammarAccess.getAttributeSetRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAttributeSet1388); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleAttributeSet


    // $ANTLR start ruleAttributeSet
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:685:1: ruleAttributeSet : ( ( rule__AttributeSet__Group__0 ) ) ;
    public final void ruleAttributeSet() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:689:2: ( ( ( rule__AttributeSet__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:690:1: ( ( rule__AttributeSet__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:690:1: ( ( rule__AttributeSet__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:691:1: ( rule__AttributeSet__Group__0 )
            {
             before(grammarAccess.getAttributeSetAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:692:1: ( rule__AttributeSet__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:692:2: rule__AttributeSet__Group__0
            {
            pushFollow(FOLLOW_rule__AttributeSet__Group__0_in_ruleAttributeSet1414);
            rule__AttributeSet__Group__0();
            _fsp--;


            }

             after(grammarAccess.getAttributeSetAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleAttributeSet


    // $ANTLR start entryRuleAttributeGet
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:704:1: entryRuleAttributeGet : ruleAttributeGet EOF ;
    public final void entryRuleAttributeGet() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:705:1: ( ruleAttributeGet EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:706:1: ruleAttributeGet EOF
            {
             before(grammarAccess.getAttributeGetRule()); 
            pushFollow(FOLLOW_ruleAttributeGet_in_entryRuleAttributeGet1441);
            ruleAttributeGet();
            _fsp--;

             after(grammarAccess.getAttributeGetRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAttributeGet1448); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleAttributeGet


    // $ANTLR start ruleAttributeGet
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:713:1: ruleAttributeGet : ( ( rule__AttributeGet__Group__0 ) ) ;
    public final void ruleAttributeGet() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:717:2: ( ( ( rule__AttributeGet__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:718:1: ( ( rule__AttributeGet__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:718:1: ( ( rule__AttributeGet__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:719:1: ( rule__AttributeGet__Group__0 )
            {
             before(grammarAccess.getAttributeGetAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:720:1: ( rule__AttributeGet__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:720:2: rule__AttributeGet__Group__0
            {
            pushFollow(FOLLOW_rule__AttributeGet__Group__0_in_ruleAttributeGet1474);
            rule__AttributeGet__Group__0();
            _fsp--;


            }

             after(grammarAccess.getAttributeGetAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleAttributeGet


    // $ANTLR start entryRuleAttributeNormal
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:732:1: entryRuleAttributeNormal : ruleAttributeNormal EOF ;
    public final void entryRuleAttributeNormal() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:733:1: ( ruleAttributeNormal EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:734:1: ruleAttributeNormal EOF
            {
             before(grammarAccess.getAttributeNormalRule()); 
            pushFollow(FOLLOW_ruleAttributeNormal_in_entryRuleAttributeNormal1501);
            ruleAttributeNormal();
            _fsp--;

             after(grammarAccess.getAttributeNormalRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleAttributeNormal1508); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleAttributeNormal


    // $ANTLR start ruleAttributeNormal
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:741:1: ruleAttributeNormal : ( ( rule__AttributeNormal__Group__0 ) ) ;
    public final void ruleAttributeNormal() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:745:2: ( ( ( rule__AttributeNormal__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:746:1: ( ( rule__AttributeNormal__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:746:1: ( ( rule__AttributeNormal__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:747:1: ( rule__AttributeNormal__Group__0 )
            {
             before(grammarAccess.getAttributeNormalAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:748:1: ( rule__AttributeNormal__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:748:2: rule__AttributeNormal__Group__0
            {
            pushFollow(FOLLOW_rule__AttributeNormal__Group__0_in_ruleAttributeNormal1534);
            rule__AttributeNormal__Group__0();
            _fsp--;


            }

             after(grammarAccess.getAttributeNormalAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleAttributeNormal


    // $ANTLR start entryRuleConstant
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:760:1: entryRuleConstant : ruleConstant EOF ;
    public final void entryRuleConstant() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:761:1: ( ruleConstant EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:762:1: ruleConstant EOF
            {
             before(grammarAccess.getConstantRule()); 
            pushFollow(FOLLOW_ruleConstant_in_entryRuleConstant1561);
            ruleConstant();
            _fsp--;

             after(grammarAccess.getConstantRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleConstant1568); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleConstant


    // $ANTLR start ruleConstant
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:769:1: ruleConstant : ( ( rule__Constant__Group__0 ) ) ;
    public final void ruleConstant() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:773:2: ( ( ( rule__Constant__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:774:1: ( ( rule__Constant__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:774:1: ( ( rule__Constant__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:775:1: ( rule__Constant__Group__0 )
            {
             before(grammarAccess.getConstantAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:776:1: ( rule__Constant__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:776:2: rule__Constant__Group__0
            {
            pushFollow(FOLLOW_rule__Constant__Group__0_in_ruleConstant1594);
            rule__Constant__Group__0();
            _fsp--;


            }

             after(grammarAccess.getConstantAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleConstant


    // $ANTLR start entryRuleRoutine
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:788:1: entryRuleRoutine : ruleRoutine EOF ;
    public final void entryRuleRoutine() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:789:1: ( ruleRoutine EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:790:1: ruleRoutine EOF
            {
             before(grammarAccess.getRoutineRule()); 
            pushFollow(FOLLOW_ruleRoutine_in_entryRuleRoutine1621);
            ruleRoutine();
            _fsp--;

             after(grammarAccess.getRoutineRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleRoutine1628); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleRoutine


    // $ANTLR start ruleRoutine
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:797:1: ruleRoutine : ( ( rule__Routine__Group__0 ) ) ;
    public final void ruleRoutine() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:801:2: ( ( ( rule__Routine__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:802:1: ( ( rule__Routine__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:802:1: ( ( rule__Routine__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:803:1: ( rule__Routine__Group__0 )
            {
             before(grammarAccess.getRoutineAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:804:1: ( rule__Routine__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:804:2: rule__Routine__Group__0
            {
            pushFollow(FOLLOW_rule__Routine__Group__0_in_ruleRoutine1654);
            rule__Routine__Group__0();
            _fsp--;


            }

             after(grammarAccess.getRoutineAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleRoutine


    // $ANTLR start entryRuleRequires
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:816:1: entryRuleRequires : ruleRequires EOF ;
    public final void entryRuleRequires() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:817:1: ( ruleRequires EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:818:1: ruleRequires EOF
            {
             before(grammarAccess.getRequiresRule()); 
            pushFollow(FOLLOW_ruleRequires_in_entryRuleRequires1681);
            ruleRequires();
            _fsp--;

             after(grammarAccess.getRequiresRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleRequires1688); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleRequires


    // $ANTLR start ruleRequires
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:825:1: ruleRequires : ( ( rule__Requires__Group__0 ) ) ;
    public final void ruleRequires() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:829:2: ( ( ( rule__Requires__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:830:1: ( ( rule__Requires__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:830:1: ( ( rule__Requires__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:831:1: ( rule__Requires__Group__0 )
            {
             before(grammarAccess.getRequiresAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:832:1: ( rule__Requires__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:832:2: rule__Requires__Group__0
            {
            pushFollow(FOLLOW_rule__Requires__Group__0_in_ruleRequires1714);
            rule__Requires__Group__0();
            _fsp--;


            }

             after(grammarAccess.getRequiresAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleRequires


    // $ANTLR start entryRuleOptions
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:844:1: entryRuleOptions : ruleOptions EOF ;
    public final void entryRuleOptions() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:845:1: ( ruleOptions EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:846:1: ruleOptions EOF
            {
             before(grammarAccess.getOptionsRule()); 
            pushFollow(FOLLOW_ruleOptions_in_entryRuleOptions1741);
            ruleOptions();
            _fsp--;

             after(grammarAccess.getOptionsRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleOptions1748); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleOptions


    // $ANTLR start ruleOptions
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:853:1: ruleOptions : ( ( rule__Options__Group__0 ) ) ;
    public final void ruleOptions() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:857:2: ( ( ( rule__Options__Group__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:858:1: ( ( rule__Options__Group__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:858:1: ( ( rule__Options__Group__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:859:1: ( rule__Options__Group__0 )
            {
             before(grammarAccess.getOptionsAccess().getGroup()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:860:1: ( rule__Options__Group__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:860:2: rule__Options__Group__0
            {
            pushFollow(FOLLOW_rule__Options__Group__0_in_ruleOptions1774);
            rule__Options__Group__0();
            _fsp--;


            }

             after(grammarAccess.getOptionsAccess().getGroup()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleOptions


    // $ANTLR start entryRuleName
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:872:1: entryRuleName : ruleName EOF ;
    public final void entryRuleName() throws RecognitionException {
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:873:1: ( ruleName EOF )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:874:1: ruleName EOF
            {
             before(grammarAccess.getNameRule()); 
            pushFollow(FOLLOW_ruleName_in_entryRuleName1801);
            ruleName();
            _fsp--;

             after(grammarAccess.getNameRule()); 
            match(input,EOF,FOLLOW_EOF_in_entryRuleName1808); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end entryRuleName


    // $ANTLR start ruleName
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:881:1: ruleName : ( ( rule__Name__Alternatives ) ) ;
    public final void ruleName() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:885:2: ( ( ( rule__Name__Alternatives ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:886:1: ( ( rule__Name__Alternatives ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:886:1: ( ( rule__Name__Alternatives ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:887:1: ( rule__Name__Alternatives )
            {
             before(grammarAccess.getNameAccess().getAlternatives()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:888:1: ( rule__Name__Alternatives )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:888:2: rule__Name__Alternatives
            {
            pushFollow(FOLLOW_rule__Name__Alternatives_in_ruleName1834);
            rule__Name__Alternatives();
            _fsp--;


            }

             after(grammarAccess.getNameAccess().getAlternatives()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleName


    // $ANTLR start ruleVISIBILITY
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:901:1: ruleVISIBILITY : ( ( rule__VISIBILITY__Alternatives ) ) ;
    public final void ruleVISIBILITY() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:905:1: ( ( ( rule__VISIBILITY__Alternatives ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:906:1: ( ( rule__VISIBILITY__Alternatives ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:906:1: ( ( rule__VISIBILITY__Alternatives ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:907:1: ( rule__VISIBILITY__Alternatives )
            {
             before(grammarAccess.getVISIBILITYAccess().getAlternatives()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:908:1: ( rule__VISIBILITY__Alternatives )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:908:2: rule__VISIBILITY__Alternatives
            {
            pushFollow(FOLLOW_rule__VISIBILITY__Alternatives_in_ruleVISIBILITY1871);
            rule__VISIBILITY__Alternatives();
            _fsp--;


            }

             after(grammarAccess.getVISIBILITYAccess().getAlternatives()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end ruleVISIBILITY


    // $ANTLR start rule__Instruction__Alternatives_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:919:1: rule__Instruction__Alternatives_0 : ( ( ruleAssignment ) | ( ruleDo ) | ( ruleIf ) | ( ruleCall ) | ( ruleArg ) | ( ruleUseArg ) | ( ruleRaise ) | ( ruleMessage ) );
    public final void rule__Instruction__Alternatives_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:923:1: ( ( ruleAssignment ) | ( ruleDo ) | ( ruleIf ) | ( ruleCall ) | ( ruleArg ) | ( ruleUseArg ) | ( ruleRaise ) | ( ruleMessage ) )
            int alt1=8;
            switch ( input.LA(1) ) {
            case RULE_STRING:
                {
                int LA1_1 = input.LA(2);

                if ( (LA1_1==79) ) {
                    alt1=8;
                }
                else if ( (LA1_1==62) ) {
                    alt1=1;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("919:1: rule__Instruction__Alternatives_0 : ( ( ruleAssignment ) | ( ruleDo ) | ( ruleIf ) | ( ruleCall ) | ( ruleArg ) | ( ruleUseArg ) | ( ruleRaise ) | ( ruleMessage ) );", 1, 1, input);

                    throw nvae;
                }
                }
                break;
            case RULE_SYMBOL_VAR:
                {
                int LA1_2 = input.LA(2);

                if ( (LA1_2==62) ) {
                    alt1=1;
                }
                else if ( (LA1_2==79) ) {
                    alt1=8;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("919:1: rule__Instruction__Alternatives_0 : ( ( ruleAssignment ) | ( ruleDo ) | ( ruleIf ) | ( ruleCall ) | ( ruleArg ) | ( ruleUseArg ) | ( ruleRaise ) | ( ruleMessage ) );", 1, 2, input);

                    throw nvae;
                }
                }
                break;
            case 66:
                {
                alt1=2;
                }
                break;
            case 63:
                {
                alt1=3;
                }
                break;
            case 68:
                {
                alt1=4;
                }
                break;
            case 69:
                {
                alt1=5;
                }
                break;
            case 71:
                {
                alt1=6;
                }
                break;
            case 72:
                {
                alt1=7;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("919:1: rule__Instruction__Alternatives_0 : ( ( ruleAssignment ) | ( ruleDo ) | ( ruleIf ) | ( ruleCall ) | ( ruleArg ) | ( ruleUseArg ) | ( ruleRaise ) | ( ruleMessage ) );", 1, 0, input);

                throw nvae;
            }

            switch (alt1) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:924:1: ( ruleAssignment )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:924:1: ( ruleAssignment )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:925:1: ruleAssignment
                    {
                     before(grammarAccess.getInstructionAccess().getAssignmentParserRuleCall_0_0()); 
                    pushFollow(FOLLOW_ruleAssignment_in_rule__Instruction__Alternatives_01906);
                    ruleAssignment();
                    _fsp--;

                     after(grammarAccess.getInstructionAccess().getAssignmentParserRuleCall_0_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:930:6: ( ruleDo )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:930:6: ( ruleDo )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:931:1: ruleDo
                    {
                     before(grammarAccess.getInstructionAccess().getDoParserRuleCall_0_1()); 
                    pushFollow(FOLLOW_ruleDo_in_rule__Instruction__Alternatives_01923);
                    ruleDo();
                    _fsp--;

                     after(grammarAccess.getInstructionAccess().getDoParserRuleCall_0_1()); 

                    }


                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:936:6: ( ruleIf )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:936:6: ( ruleIf )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:937:1: ruleIf
                    {
                     before(grammarAccess.getInstructionAccess().getIfParserRuleCall_0_2()); 
                    pushFollow(FOLLOW_ruleIf_in_rule__Instruction__Alternatives_01940);
                    ruleIf();
                    _fsp--;

                     after(grammarAccess.getInstructionAccess().getIfParserRuleCall_0_2()); 

                    }


                    }
                    break;
                case 4 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:942:6: ( ruleCall )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:942:6: ( ruleCall )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:943:1: ruleCall
                    {
                     before(grammarAccess.getInstructionAccess().getCallParserRuleCall_0_3()); 
                    pushFollow(FOLLOW_ruleCall_in_rule__Instruction__Alternatives_01957);
                    ruleCall();
                    _fsp--;

                     after(grammarAccess.getInstructionAccess().getCallParserRuleCall_0_3()); 

                    }


                    }
                    break;
                case 5 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:948:6: ( ruleArg )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:948:6: ( ruleArg )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:949:1: ruleArg
                    {
                     before(grammarAccess.getInstructionAccess().getArgParserRuleCall_0_4()); 
                    pushFollow(FOLLOW_ruleArg_in_rule__Instruction__Alternatives_01974);
                    ruleArg();
                    _fsp--;

                     after(grammarAccess.getInstructionAccess().getArgParserRuleCall_0_4()); 

                    }


                    }
                    break;
                case 6 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:954:6: ( ruleUseArg )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:954:6: ( ruleUseArg )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:955:1: ruleUseArg
                    {
                     before(grammarAccess.getInstructionAccess().getUseArgParserRuleCall_0_5()); 
                    pushFollow(FOLLOW_ruleUseArg_in_rule__Instruction__Alternatives_01991);
                    ruleUseArg();
                    _fsp--;

                     after(grammarAccess.getInstructionAccess().getUseArgParserRuleCall_0_5()); 

                    }


                    }
                    break;
                case 7 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:960:6: ( ruleRaise )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:960:6: ( ruleRaise )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:961:1: ruleRaise
                    {
                     before(grammarAccess.getInstructionAccess().getRaiseParserRuleCall_0_6()); 
                    pushFollow(FOLLOW_ruleRaise_in_rule__Instruction__Alternatives_02008);
                    ruleRaise();
                    _fsp--;

                     after(grammarAccess.getInstructionAccess().getRaiseParserRuleCall_0_6()); 

                    }


                    }
                    break;
                case 8 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:966:6: ( ruleMessage )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:966:6: ( ruleMessage )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:967:1: ruleMessage
                    {
                     before(grammarAccess.getInstructionAccess().getMessageParserRuleCall_0_7()); 
                    pushFollow(FOLLOW_ruleMessage_in_rule__Instruction__Alternatives_02025);
                    ruleMessage();
                    _fsp--;

                     after(grammarAccess.getInstructionAccess().getMessageParserRuleCall_0_7()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Instruction__Alternatives_0


    // $ANTLR start rule__Expression__Alternatives
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:977:1: rule__Expression__Alternatives : ( ( ( rule__Expression__Group_0__0 ) ) | ( rulefnCall ) | ( ruleMessage ) | ( ruleArrayAccess ) | ( ruleNegate ) | ( ruleConst ) );
    public final void rule__Expression__Alternatives() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:981:1: ( ( ( rule__Expression__Group_0__0 ) ) | ( rulefnCall ) | ( ruleMessage ) | ( ruleArrayAccess ) | ( ruleNegate ) | ( ruleConst ) )
            int alt2=6;
            switch ( input.LA(1) ) {
            case RULE_STRING:
                {
                switch ( input.LA(2) ) {
                case EOF:
                case RULE_TERM:
                case 64:
                case 70:
                case 76:
                case 78:
                    {
                    alt2=1;
                    }
                    break;
                case 75:
                    {
                    alt2=4;
                    }
                    break;
                case 77:
                    {
                    alt2=2;
                    }
                    break;
                case 79:
                    {
                    alt2=3;
                    }
                    break;
                default:
                    NoViableAltException nvae =
                        new NoViableAltException("977:1: rule__Expression__Alternatives : ( ( ( rule__Expression__Group_0__0 ) ) | ( rulefnCall ) | ( ruleMessage ) | ( ruleArrayAccess ) | ( ruleNegate ) | ( ruleConst ) );", 2, 1, input);

                    throw nvae;
                }

                }
                break;
            case RULE_SYMBOL_VAR:
                {
                switch ( input.LA(2) ) {
                case EOF:
                case RULE_TERM:
                case 64:
                case 70:
                case 76:
                case 78:
                    {
                    alt2=1;
                    }
                    break;
                case 75:
                    {
                    alt2=4;
                    }
                    break;
                case 77:
                    {
                    alt2=2;
                    }
                    break;
                case 79:
                    {
                    alt2=3;
                    }
                    break;
                default:
                    NoViableAltException nvae =
                        new NoViableAltException("977:1: rule__Expression__Alternatives : ( ( ( rule__Expression__Group_0__0 ) ) | ( rulefnCall ) | ( ruleMessage ) | ( ruleArrayAccess ) | ( ruleNegate ) | ( ruleConst ) );", 2, 2, input);

                    throw nvae;
                }

                }
                break;
            case 81:
                {
                alt2=5;
                }
                break;
            case RULE_SYMBOL_NUM:
            case RULE_STRING_BOOL:
            case RULE_STRING_HEX:
                {
                alt2=6;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("977:1: rule__Expression__Alternatives : ( ( ( rule__Expression__Group_0__0 ) ) | ( rulefnCall ) | ( ruleMessage ) | ( ruleArrayAccess ) | ( ruleNegate ) | ( ruleConst ) );", 2, 0, input);

                throw nvae;
            }

            switch (alt2) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:982:1: ( ( rule__Expression__Group_0__0 ) )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:982:1: ( ( rule__Expression__Group_0__0 ) )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:983:1: ( rule__Expression__Group_0__0 )
                    {
                     before(grammarAccess.getExpressionAccess().getGroup_0()); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:984:1: ( rule__Expression__Group_0__0 )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:984:2: rule__Expression__Group_0__0
                    {
                    pushFollow(FOLLOW_rule__Expression__Group_0__0_in_rule__Expression__Alternatives2057);
                    rule__Expression__Group_0__0();
                    _fsp--;


                    }

                     after(grammarAccess.getExpressionAccess().getGroup_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:988:6: ( rulefnCall )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:988:6: ( rulefnCall )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:989:1: rulefnCall
                    {
                     before(grammarAccess.getExpressionAccess().getFnCallParserRuleCall_1()); 
                    pushFollow(FOLLOW_rulefnCall_in_rule__Expression__Alternatives2075);
                    rulefnCall();
                    _fsp--;

                     after(grammarAccess.getExpressionAccess().getFnCallParserRuleCall_1()); 

                    }


                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:994:6: ( ruleMessage )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:994:6: ( ruleMessage )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:995:1: ruleMessage
                    {
                     before(grammarAccess.getExpressionAccess().getMessageParserRuleCall_2()); 
                    pushFollow(FOLLOW_ruleMessage_in_rule__Expression__Alternatives2092);
                    ruleMessage();
                    _fsp--;

                     after(grammarAccess.getExpressionAccess().getMessageParserRuleCall_2()); 

                    }


                    }
                    break;
                case 4 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1000:6: ( ruleArrayAccess )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1000:6: ( ruleArrayAccess )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1001:1: ruleArrayAccess
                    {
                     before(grammarAccess.getExpressionAccess().getArrayAccessParserRuleCall_3()); 
                    pushFollow(FOLLOW_ruleArrayAccess_in_rule__Expression__Alternatives2109);
                    ruleArrayAccess();
                    _fsp--;

                     after(grammarAccess.getExpressionAccess().getArrayAccessParserRuleCall_3()); 

                    }


                    }
                    break;
                case 5 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1006:6: ( ruleNegate )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1006:6: ( ruleNegate )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1007:1: ruleNegate
                    {
                     before(grammarAccess.getExpressionAccess().getNegateParserRuleCall_4()); 
                    pushFollow(FOLLOW_ruleNegate_in_rule__Expression__Alternatives2126);
                    ruleNegate();
                    _fsp--;

                     after(grammarAccess.getExpressionAccess().getNegateParserRuleCall_4()); 

                    }


                    }
                    break;
                case 6 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1012:6: ( ruleConst )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1012:6: ( ruleConst )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1013:1: ruleConst
                    {
                     before(grammarAccess.getExpressionAccess().getConstParserRuleCall_5()); 
                    pushFollow(FOLLOW_ruleConst_in_rule__Expression__Alternatives2143);
                    ruleConst();
                    _fsp--;

                     after(grammarAccess.getExpressionAccess().getConstParserRuleCall_5()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Expression__Alternatives


    // $ANTLR start rule__Message__Alternatives_5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1023:1: rule__Message__Alternatives_5 : ( ( ( rule__Message__Group_5_0__0 ) ) | ( ( rule__Message__Group_5_1__0 ) ) );
    public final void rule__Message__Alternatives_5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1027:1: ( ( ( rule__Message__Group_5_0__0 ) ) | ( ( rule__Message__Group_5_1__0 ) ) )
            int alt3=2;
            int LA3_0 = input.LA(1);

            if ( (LA3_0==77) ) {
                alt3=1;
            }
            else if ( (LA3_0==62) ) {
                alt3=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("1023:1: rule__Message__Alternatives_5 : ( ( ( rule__Message__Group_5_0__0 ) ) | ( ( rule__Message__Group_5_1__0 ) ) );", 3, 0, input);

                throw nvae;
            }
            switch (alt3) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1028:1: ( ( rule__Message__Group_5_0__0 ) )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1028:1: ( ( rule__Message__Group_5_0__0 ) )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1029:1: ( rule__Message__Group_5_0__0 )
                    {
                     before(grammarAccess.getMessageAccess().getGroup_5_0()); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1030:1: ( rule__Message__Group_5_0__0 )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1030:2: rule__Message__Group_5_0__0
                    {
                    pushFollow(FOLLOW_rule__Message__Group_5_0__0_in_rule__Message__Alternatives_52175);
                    rule__Message__Group_5_0__0();
                    _fsp--;


                    }

                     after(grammarAccess.getMessageAccess().getGroup_5_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1034:6: ( ( rule__Message__Group_5_1__0 ) )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1034:6: ( ( rule__Message__Group_5_1__0 ) )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1035:1: ( rule__Message__Group_5_1__0 )
                    {
                     before(grammarAccess.getMessageAccess().getGroup_5_1()); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1036:1: ( rule__Message__Group_5_1__0 )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1036:2: rule__Message__Group_5_1__0
                    {
                    pushFollow(FOLLOW_rule__Message__Group_5_1__0_in_rule__Message__Alternatives_52193);
                    rule__Message__Group_5_1__0();
                    _fsp--;


                    }

                     after(grammarAccess.getMessageAccess().getGroup_5_1()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Alternatives_5


    // $ANTLR start rule__Const__ValueAlternatives_1_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1045:1: rule__Const__ValueAlternatives_1_0 : ( ( RULE_SYMBOL_NUM ) | ( RULE_STRING_BOOL ) | ( RULE_STRING_HEX ) );
    public final void rule__Const__ValueAlternatives_1_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1049:1: ( ( RULE_SYMBOL_NUM ) | ( RULE_STRING_BOOL ) | ( RULE_STRING_HEX ) )
            int alt4=3;
            switch ( input.LA(1) ) {
            case RULE_SYMBOL_NUM:
                {
                alt4=1;
                }
                break;
            case RULE_STRING_BOOL:
                {
                alt4=2;
                }
                break;
            case RULE_STRING_HEX:
                {
                alt4=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("1045:1: rule__Const__ValueAlternatives_1_0 : ( ( RULE_SYMBOL_NUM ) | ( RULE_STRING_BOOL ) | ( RULE_STRING_HEX ) );", 4, 0, input);

                throw nvae;
            }

            switch (alt4) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1050:1: ( RULE_SYMBOL_NUM )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1050:1: ( RULE_SYMBOL_NUM )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1051:1: RULE_SYMBOL_NUM
                    {
                     before(grammarAccess.getConstAccess().getValueSYMBOL_NUMTerminalRuleCall_1_0_0()); 
                    match(input,RULE_SYMBOL_NUM,FOLLOW_RULE_SYMBOL_NUM_in_rule__Const__ValueAlternatives_1_02226); 
                     after(grammarAccess.getConstAccess().getValueSYMBOL_NUMTerminalRuleCall_1_0_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1056:6: ( RULE_STRING_BOOL )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1056:6: ( RULE_STRING_BOOL )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1057:1: RULE_STRING_BOOL
                    {
                     before(grammarAccess.getConstAccess().getValueSTRING_BOOLTerminalRuleCall_1_0_1()); 
                    match(input,RULE_STRING_BOOL,FOLLOW_RULE_STRING_BOOL_in_rule__Const__ValueAlternatives_1_02243); 
                     after(grammarAccess.getConstAccess().getValueSTRING_BOOLTerminalRuleCall_1_0_1()); 

                    }


                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1062:6: ( RULE_STRING_HEX )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1062:6: ( RULE_STRING_HEX )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1063:1: RULE_STRING_HEX
                    {
                     before(grammarAccess.getConstAccess().getValueSTRING_HEXTerminalRuleCall_1_0_2()); 
                    match(input,RULE_STRING_HEX,FOLLOW_RULE_STRING_HEX_in_rule__Const__ValueAlternatives_1_02260); 
                     after(grammarAccess.getConstAccess().getValueSTRING_HEXTerminalRuleCall_1_0_2()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Const__ValueAlternatives_1_0


    // $ANTLR start rule__Directives__Alternatives
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1073:1: rule__Directives__Alternatives : ( ( ruleClass ) | ( ruleOptions ) | ( ruleRoutine ) );
    public final void rule__Directives__Alternatives() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1077:1: ( ( ruleClass ) | ( ruleOptions ) | ( ruleRoutine ) )
            int alt5=3;
            switch ( input.LA(1) ) {
            case 82:
                {
                alt5=1;
                }
                break;
            case 95:
                {
                alt5=2;
                }
                break;
            case 93:
                {
                alt5=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("1073:1: rule__Directives__Alternatives : ( ( ruleClass ) | ( ruleOptions ) | ( ruleRoutine ) );", 5, 0, input);

                throw nvae;
            }

            switch (alt5) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1078:1: ( ruleClass )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1078:1: ( ruleClass )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1079:1: ruleClass
                    {
                     before(grammarAccess.getDirectivesAccess().getClassParserRuleCall_0()); 
                    pushFollow(FOLLOW_ruleClass_in_rule__Directives__Alternatives2292);
                    ruleClass();
                    _fsp--;

                     after(grammarAccess.getDirectivesAccess().getClassParserRuleCall_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1084:6: ( ruleOptions )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1084:6: ( ruleOptions )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1085:1: ruleOptions
                    {
                     before(grammarAccess.getDirectivesAccess().getOptionsParserRuleCall_1()); 
                    pushFollow(FOLLOW_ruleOptions_in_rule__Directives__Alternatives2309);
                    ruleOptions();
                    _fsp--;

                     after(grammarAccess.getDirectivesAccess().getOptionsParserRuleCall_1()); 

                    }


                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1090:6: ( ruleRoutine )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1090:6: ( ruleRoutine )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1091:1: ruleRoutine
                    {
                     before(grammarAccess.getDirectivesAccess().getRoutineParserRuleCall_2()); 
                    pushFollow(FOLLOW_ruleRoutine_in_rule__Directives__Alternatives2326);
                    ruleRoutine();
                    _fsp--;

                     after(grammarAccess.getDirectivesAccess().getRoutineParserRuleCall_2()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Directives__Alternatives


    // $ANTLR start rule__Class__Alternatives_3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1101:1: rule__Class__Alternatives_3 : ( ( ( rule__Class__Group_3_0__0 ) ) | ( ( rule__Class__Group_3_1__0 ) ) );
    public final void rule__Class__Alternatives_3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1105:1: ( ( ( rule__Class__Group_3_0__0 ) ) | ( ( rule__Class__Group_3_1__0 ) ) )
            int alt6=2;
            int LA6_0 = input.LA(1);

            if ( (LA6_0==84) ) {
                alt6=1;
            }
            else if ( (LA6_0==85) ) {
                alt6=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("1101:1: rule__Class__Alternatives_3 : ( ( ( rule__Class__Group_3_0__0 ) ) | ( ( rule__Class__Group_3_1__0 ) ) );", 6, 0, input);

                throw nvae;
            }
            switch (alt6) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1106:1: ( ( rule__Class__Group_3_0__0 ) )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1106:1: ( ( rule__Class__Group_3_0__0 ) )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1107:1: ( rule__Class__Group_3_0__0 )
                    {
                     before(grammarAccess.getClassAccess().getGroup_3_0()); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1108:1: ( rule__Class__Group_3_0__0 )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1108:2: rule__Class__Group_3_0__0
                    {
                    pushFollow(FOLLOW_rule__Class__Group_3_0__0_in_rule__Class__Alternatives_32358);
                    rule__Class__Group_3_0__0();
                    _fsp--;


                    }

                     after(grammarAccess.getClassAccess().getGroup_3_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1112:6: ( ( rule__Class__Group_3_1__0 ) )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1112:6: ( ( rule__Class__Group_3_1__0 ) )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1113:1: ( rule__Class__Group_3_1__0 )
                    {
                     before(grammarAccess.getClassAccess().getGroup_3_1()); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1114:1: ( rule__Class__Group_3_1__0 )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1114:2: rule__Class__Group_3_1__0
                    {
                    pushFollow(FOLLOW_rule__Class__Group_3_1__0_in_rule__Class__Alternatives_32376);
                    rule__Class__Group_3_1__0();
                    _fsp--;


                    }

                     after(grammarAccess.getClassAccess().getGroup_3_1()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Alternatives_3


    // $ANTLR start rule__ClassContent__Alternatives
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1123:1: rule__ClassContent__Alternatives : ( ( ruleMethod ) | ( ruleAttribute ) | ( ruleConstant ) );
    public final void rule__ClassContent__Alternatives() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1127:1: ( ( ruleMethod ) | ( ruleAttribute ) | ( ruleConstant ) )
            int alt7=3;
            switch ( input.LA(1) ) {
            case 87:
                {
                alt7=1;
                }
                break;
            case 89:
                {
                alt7=2;
                }
                break;
            case 92:
                {
                alt7=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("1123:1: rule__ClassContent__Alternatives : ( ( ruleMethod ) | ( ruleAttribute ) | ( ruleConstant ) );", 7, 0, input);

                throw nvae;
            }

            switch (alt7) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1128:1: ( ruleMethod )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1128:1: ( ruleMethod )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1129:1: ruleMethod
                    {
                     before(grammarAccess.getClassContentAccess().getMethodParserRuleCall_0()); 
                    pushFollow(FOLLOW_ruleMethod_in_rule__ClassContent__Alternatives2409);
                    ruleMethod();
                    _fsp--;

                     after(grammarAccess.getClassContentAccess().getMethodParserRuleCall_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1134:6: ( ruleAttribute )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1134:6: ( ruleAttribute )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1135:1: ruleAttribute
                    {
                     before(grammarAccess.getClassContentAccess().getAttributeParserRuleCall_1()); 
                    pushFollow(FOLLOW_ruleAttribute_in_rule__ClassContent__Alternatives2426);
                    ruleAttribute();
                    _fsp--;

                     after(grammarAccess.getClassContentAccess().getAttributeParserRuleCall_1()); 

                    }


                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1140:6: ( ruleConstant )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1140:6: ( ruleConstant )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1141:1: ruleConstant
                    {
                     before(grammarAccess.getClassContentAccess().getConstantParserRuleCall_2()); 
                    pushFollow(FOLLOW_ruleConstant_in_rule__ClassContent__Alternatives2443);
                    ruleConstant();
                    _fsp--;

                     after(grammarAccess.getClassContentAccess().getConstantParserRuleCall_2()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__ClassContent__Alternatives


    // $ANTLR start rule__Attribute__Alternatives_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1151:1: rule__Attribute__Alternatives_0 : ( ( ruleAttributeSet ) | ( ruleAttributeGet ) | ( ruleAttributeNormal ) );
    public final void rule__Attribute__Alternatives_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1155:1: ( ( ruleAttributeSet ) | ( ruleAttributeGet ) | ( ruleAttributeNormal ) )
            int alt8=3;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==89) ) {
                int LA8_1 = input.LA(2);

                if ( (LA8_1==RULE_STRING) ) {
                    switch ( input.LA(3) ) {
                    case 91:
                        {
                        alt8=2;
                        }
                        break;
                    case RULE_TERM:
                    case 60:
                    case 61:
                    case 97:
                        {
                        alt8=3;
                        }
                        break;
                    case 90:
                        {
                        alt8=1;
                        }
                        break;
                    default:
                        NoViableAltException nvae =
                            new NoViableAltException("1151:1: rule__Attribute__Alternatives_0 : ( ( ruleAttributeSet ) | ( ruleAttributeGet ) | ( ruleAttributeNormal ) );", 8, 2, input);

                        throw nvae;
                    }

                }
                else if ( (LA8_1==RULE_SYMBOL_VAR) ) {
                    switch ( input.LA(3) ) {
                    case 91:
                        {
                        alt8=2;
                        }
                        break;
                    case RULE_TERM:
                    case 60:
                    case 61:
                    case 97:
                        {
                        alt8=3;
                        }
                        break;
                    case 90:
                        {
                        alt8=1;
                        }
                        break;
                    default:
                        NoViableAltException nvae =
                            new NoViableAltException("1151:1: rule__Attribute__Alternatives_0 : ( ( ruleAttributeSet ) | ( ruleAttributeGet ) | ( ruleAttributeNormal ) );", 8, 3, input);

                        throw nvae;
                    }

                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("1151:1: rule__Attribute__Alternatives_0 : ( ( ruleAttributeSet ) | ( ruleAttributeGet ) | ( ruleAttributeNormal ) );", 8, 1, input);

                    throw nvae;
                }
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("1151:1: rule__Attribute__Alternatives_0 : ( ( ruleAttributeSet ) | ( ruleAttributeGet ) | ( ruleAttributeNormal ) );", 8, 0, input);

                throw nvae;
            }
            switch (alt8) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1156:1: ( ruleAttributeSet )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1156:1: ( ruleAttributeSet )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1157:1: ruleAttributeSet
                    {
                     before(grammarAccess.getAttributeAccess().getAttributeSetParserRuleCall_0_0()); 
                    pushFollow(FOLLOW_ruleAttributeSet_in_rule__Attribute__Alternatives_02475);
                    ruleAttributeSet();
                    _fsp--;

                     after(grammarAccess.getAttributeAccess().getAttributeSetParserRuleCall_0_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1162:6: ( ruleAttributeGet )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1162:6: ( ruleAttributeGet )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1163:1: ruleAttributeGet
                    {
                     before(grammarAccess.getAttributeAccess().getAttributeGetParserRuleCall_0_1()); 
                    pushFollow(FOLLOW_ruleAttributeGet_in_rule__Attribute__Alternatives_02492);
                    ruleAttributeGet();
                    _fsp--;

                     after(grammarAccess.getAttributeAccess().getAttributeGetParserRuleCall_0_1()); 

                    }


                    }
                    break;
                case 3 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1168:6: ( ruleAttributeNormal )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1168:6: ( ruleAttributeNormal )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1169:1: ruleAttributeNormal
                    {
                     before(grammarAccess.getAttributeAccess().getAttributeNormalParserRuleCall_0_2()); 
                    pushFollow(FOLLOW_ruleAttributeNormal_in_rule__Attribute__Alternatives_02509);
                    ruleAttributeNormal();
                    _fsp--;

                     after(grammarAccess.getAttributeAccess().getAttributeNormalParserRuleCall_0_2()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Alternatives_0


    // $ANTLR start rule__Name__Alternatives
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1179:1: rule__Name__Alternatives : ( ( RULE_STRING ) | ( RULE_SYMBOL_VAR ) );
    public final void rule__Name__Alternatives() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1183:1: ( ( RULE_STRING ) | ( RULE_SYMBOL_VAR ) )
            int alt9=2;
            int LA9_0 = input.LA(1);

            if ( (LA9_0==RULE_STRING) ) {
                alt9=1;
            }
            else if ( (LA9_0==RULE_SYMBOL_VAR) ) {
                alt9=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("1179:1: rule__Name__Alternatives : ( ( RULE_STRING ) | ( RULE_SYMBOL_VAR ) );", 9, 0, input);

                throw nvae;
            }
            switch (alt9) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1184:1: ( RULE_STRING )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1184:1: ( RULE_STRING )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1185:1: RULE_STRING
                    {
                     before(grammarAccess.getNameAccess().getSTRINGTerminalRuleCall_0()); 
                    match(input,RULE_STRING,FOLLOW_RULE_STRING_in_rule__Name__Alternatives2541); 
                     after(grammarAccess.getNameAccess().getSTRINGTerminalRuleCall_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1190:6: ( RULE_SYMBOL_VAR )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1190:6: ( RULE_SYMBOL_VAR )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1191:1: RULE_SYMBOL_VAR
                    {
                     before(grammarAccess.getNameAccess().getSYMBOL_VARTerminalRuleCall_1()); 
                    match(input,RULE_SYMBOL_VAR,FOLLOW_RULE_SYMBOL_VAR_in_rule__Name__Alternatives2558); 
                     after(grammarAccess.getNameAccess().getSYMBOL_VARTerminalRuleCall_1()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Name__Alternatives


    // $ANTLR start rule__VISIBILITY__Alternatives
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1201:1: rule__VISIBILITY__Alternatives : ( ( ( 'private' ) ) | ( ( 'public' ) ) );
    public final void rule__VISIBILITY__Alternatives() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1205:1: ( ( ( 'private' ) ) | ( ( 'public' ) ) )
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==60) ) {
                alt10=1;
            }
            else if ( (LA10_0==61) ) {
                alt10=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("1201:1: rule__VISIBILITY__Alternatives : ( ( ( 'private' ) ) | ( ( 'public' ) ) );", 10, 0, input);

                throw nvae;
            }
            switch (alt10) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1206:1: ( ( 'private' ) )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1206:1: ( ( 'private' ) )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1207:1: ( 'private' )
                    {
                     before(grammarAccess.getVISIBILITYAccess().getPRIVATEEnumLiteralDeclaration_0()); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1208:1: ( 'private' )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1208:3: 'private'
                    {
                    match(input,60,FOLLOW_60_in_rule__VISIBILITY__Alternatives2591); 

                    }

                     after(grammarAccess.getVISIBILITYAccess().getPRIVATEEnumLiteralDeclaration_0()); 

                    }


                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1213:6: ( ( 'public' ) )
                    {
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1213:6: ( ( 'public' ) )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1214:1: ( 'public' )
                    {
                     before(grammarAccess.getVISIBILITYAccess().getPUBLICEnumLiteralDeclaration_1()); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1215:1: ( 'public' )
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1215:3: 'public'
                    {
                    match(input,61,FOLLOW_61_in_rule__VISIBILITY__Alternatives2612); 

                    }

                     after(grammarAccess.getVISIBILITYAccess().getPUBLICEnumLiteralDeclaration_1()); 

                    }


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__VISIBILITY__Alternatives


    // $ANTLR start rule__File__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1227:1: rule__File__Group__0 : ( () ) rule__File__Group__1 ;
    public final void rule__File__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1231:1: ( ( () ) rule__File__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1232:1: ( () ) rule__File__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1232:1: ( () )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1233:1: ()
            {
             before(grammarAccess.getFileAccess().getFileAction_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1234:1: ()
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1236:1: 
            {
            }

             after(grammarAccess.getFileAccess().getFileAction_0()); 

            }

            pushFollow(FOLLOW_rule__File__Group__1_in_rule__File__Group__02659);
            rule__File__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__Group__0


    // $ANTLR start rule__File__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1247:1: rule__File__Group__1 : ( ( RULE_TERM )* ) rule__File__Group__2 ;
    public final void rule__File__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1251:1: ( ( ( RULE_TERM )* ) rule__File__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1252:1: ( ( RULE_TERM )* ) rule__File__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1252:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1253:1: ( RULE_TERM )*
            {
             before(grammarAccess.getFileAccess().getTERMTerminalRuleCall_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1254:1: ( RULE_TERM )*
            loop11:
            do {
                int alt11=2;
                int LA11_0 = input.LA(1);

                if ( (LA11_0==RULE_TERM) ) {
                    alt11=1;
                }


                switch (alt11) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1254:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__File__Group__12688); 

            	    }
            	    break;

            	default :
            	    break loop11;
                }
            } while (true);

             after(grammarAccess.getFileAccess().getTERMTerminalRuleCall_1()); 

            }

            pushFollow(FOLLOW_rule__File__Group__2_in_rule__File__Group__12698);
            rule__File__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__Group__1


    // $ANTLR start rule__File__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1265:1: rule__File__Group__2 : ( ( rule__File__InstructionsAssignment_2 )* ) rule__File__Group__3 ;
    public final void rule__File__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1269:1: ( ( ( rule__File__InstructionsAssignment_2 )* ) rule__File__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1270:1: ( ( rule__File__InstructionsAssignment_2 )* ) rule__File__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1270:1: ( ( rule__File__InstructionsAssignment_2 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1271:1: ( rule__File__InstructionsAssignment_2 )*
            {
             before(grammarAccess.getFileAccess().getInstructionsAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1272:1: ( rule__File__InstructionsAssignment_2 )*
            loop12:
            do {
                int alt12=2;
                int LA12_0 = input.LA(1);

                if ( ((LA12_0>=RULE_STRING && LA12_0<=RULE_SYMBOL_VAR)||LA12_0==63||LA12_0==66||(LA12_0>=68 && LA12_0<=69)||(LA12_0>=71 && LA12_0<=72)) ) {
                    alt12=1;
                }


                switch (alt12) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1272:2: rule__File__InstructionsAssignment_2
            	    {
            	    pushFollow(FOLLOW_rule__File__InstructionsAssignment_2_in_rule__File__Group__22726);
            	    rule__File__InstructionsAssignment_2();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop12;
                }
            } while (true);

             after(grammarAccess.getFileAccess().getInstructionsAssignment_2()); 

            }

            pushFollow(FOLLOW_rule__File__Group__3_in_rule__File__Group__22736);
            rule__File__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__Group__2


    // $ANTLR start rule__File__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1283:1: rule__File__Group__3 : ( ( rule__File__RequiresAssignment_3 )* ) rule__File__Group__4 ;
    public final void rule__File__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1287:1: ( ( ( rule__File__RequiresAssignment_3 )* ) rule__File__Group__4 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1288:1: ( ( rule__File__RequiresAssignment_3 )* ) rule__File__Group__4
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1288:1: ( ( rule__File__RequiresAssignment_3 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1289:1: ( rule__File__RequiresAssignment_3 )*
            {
             before(grammarAccess.getFileAccess().getRequiresAssignment_3()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1290:1: ( rule__File__RequiresAssignment_3 )*
            loop13:
            do {
                int alt13=2;
                int LA13_0 = input.LA(1);

                if ( (LA13_0==94) ) {
                    alt13=1;
                }


                switch (alt13) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1290:2: rule__File__RequiresAssignment_3
            	    {
            	    pushFollow(FOLLOW_rule__File__RequiresAssignment_3_in_rule__File__Group__32764);
            	    rule__File__RequiresAssignment_3();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop13;
                }
            } while (true);

             after(grammarAccess.getFileAccess().getRequiresAssignment_3()); 

            }

            pushFollow(FOLLOW_rule__File__Group__4_in_rule__File__Group__32774);
            rule__File__Group__4();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__Group__3


    // $ANTLR start rule__File__Group__4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1301:1: rule__File__Group__4 : ( ( rule__File__MethodsAssignment_4 )* ) rule__File__Group__5 ;
    public final void rule__File__Group__4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1305:1: ( ( ( rule__File__MethodsAssignment_4 )* ) rule__File__Group__5 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1306:1: ( ( rule__File__MethodsAssignment_4 )* ) rule__File__Group__5
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1306:1: ( ( rule__File__MethodsAssignment_4 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1307:1: ( rule__File__MethodsAssignment_4 )*
            {
             before(grammarAccess.getFileAccess().getMethodsAssignment_4()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1308:1: ( rule__File__MethodsAssignment_4 )*
            loop14:
            do {
                int alt14=2;
                int LA14_0 = input.LA(1);

                if ( (LA14_0==87) ) {
                    alt14=1;
                }


                switch (alt14) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1308:2: rule__File__MethodsAssignment_4
            	    {
            	    pushFollow(FOLLOW_rule__File__MethodsAssignment_4_in_rule__File__Group__42802);
            	    rule__File__MethodsAssignment_4();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop14;
                }
            } while (true);

             after(grammarAccess.getFileAccess().getMethodsAssignment_4()); 

            }

            pushFollow(FOLLOW_rule__File__Group__5_in_rule__File__Group__42812);
            rule__File__Group__5();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__Group__4


    // $ANTLR start rule__File__Group__5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1319:1: rule__File__Group__5 : ( ( rule__File__DirectivesAssignment_5 )* ) ;
    public final void rule__File__Group__5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1323:1: ( ( ( rule__File__DirectivesAssignment_5 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1324:1: ( ( rule__File__DirectivesAssignment_5 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1324:1: ( ( rule__File__DirectivesAssignment_5 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1325:1: ( rule__File__DirectivesAssignment_5 )*
            {
             before(grammarAccess.getFileAccess().getDirectivesAssignment_5()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1326:1: ( rule__File__DirectivesAssignment_5 )*
            loop15:
            do {
                int alt15=2;
                int LA15_0 = input.LA(1);

                if ( (LA15_0==82||LA15_0==93||LA15_0==95) ) {
                    alt15=1;
                }


                switch (alt15) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1326:2: rule__File__DirectivesAssignment_5
            	    {
            	    pushFollow(FOLLOW_rule__File__DirectivesAssignment_5_in_rule__File__Group__52840);
            	    rule__File__DirectivesAssignment_5();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop15;
                }
            } while (true);

             after(grammarAccess.getFileAccess().getDirectivesAssignment_5()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__Group__5


    // $ANTLR start rule__Instruction__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1348:1: rule__Instruction__Group__0 : ( ( rule__Instruction__Alternatives_0 ) ) rule__Instruction__Group__1 ;
    public final void rule__Instruction__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1352:1: ( ( ( rule__Instruction__Alternatives_0 ) ) rule__Instruction__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1353:1: ( ( rule__Instruction__Alternatives_0 ) ) rule__Instruction__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1353:1: ( ( rule__Instruction__Alternatives_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1354:1: ( rule__Instruction__Alternatives_0 )
            {
             before(grammarAccess.getInstructionAccess().getAlternatives_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1355:1: ( rule__Instruction__Alternatives_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1355:2: rule__Instruction__Alternatives_0
            {
            pushFollow(FOLLOW_rule__Instruction__Alternatives_0_in_rule__Instruction__Group__02887);
            rule__Instruction__Alternatives_0();
            _fsp--;


            }

             after(grammarAccess.getInstructionAccess().getAlternatives_0()); 

            }

            pushFollow(FOLLOW_rule__Instruction__Group__1_in_rule__Instruction__Group__02896);
            rule__Instruction__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Instruction__Group__0


    // $ANTLR start rule__Instruction__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1366:1: rule__Instruction__Group__1 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) ;
    public final void rule__Instruction__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1370:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1371:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1371:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1372:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1372:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1373:1: ( RULE_TERM )
            {
             before(grammarAccess.getInstructionAccess().getTERMTerminalRuleCall_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1374:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1374:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Instruction__Group__12927); 

            }

             after(grammarAccess.getInstructionAccess().getTERMTerminalRuleCall_1()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1377:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1378:1: ( RULE_TERM )*
            {
             before(grammarAccess.getInstructionAccess().getTERMTerminalRuleCall_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1379:1: ( RULE_TERM )*
            loop16:
            do {
                int alt16=2;
                int LA16_0 = input.LA(1);

                if ( (LA16_0==RULE_TERM) ) {
                    alt16=1;
                }


                switch (alt16) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1379:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Instruction__Group__12940); 

            	    }
            	    break;

            	default :
            	    break loop16;
                }
            } while (true);

             after(grammarAccess.getInstructionAccess().getTERMTerminalRuleCall_1()); 

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Instruction__Group__1


    // $ANTLR start rule__Assignment__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1394:1: rule__Assignment__Group__0 : ( ( rule__Assignment__TargetAssignment_0 ) ) rule__Assignment__Group__1 ;
    public final void rule__Assignment__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1398:1: ( ( ( rule__Assignment__TargetAssignment_0 ) ) rule__Assignment__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1399:1: ( ( rule__Assignment__TargetAssignment_0 ) ) rule__Assignment__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1399:1: ( ( rule__Assignment__TargetAssignment_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1400:1: ( rule__Assignment__TargetAssignment_0 )
            {
             before(grammarAccess.getAssignmentAccess().getTargetAssignment_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1401:1: ( rule__Assignment__TargetAssignment_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1401:2: rule__Assignment__TargetAssignment_0
            {
            pushFollow(FOLLOW_rule__Assignment__TargetAssignment_0_in_rule__Assignment__Group__02981);
            rule__Assignment__TargetAssignment_0();
            _fsp--;


            }

             after(grammarAccess.getAssignmentAccess().getTargetAssignment_0()); 

            }

            pushFollow(FOLLOW_rule__Assignment__Group__1_in_rule__Assignment__Group__02990);
            rule__Assignment__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Assignment__Group__0


    // $ANTLR start rule__Assignment__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1412:1: rule__Assignment__Group__1 : ( '=' ) rule__Assignment__Group__2 ;
    public final void rule__Assignment__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1416:1: ( ( '=' ) rule__Assignment__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1417:1: ( '=' ) rule__Assignment__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1417:1: ( '=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1418:1: '='
            {
             before(grammarAccess.getAssignmentAccess().getEqualsSignKeyword_1()); 
            match(input,62,FOLLOW_62_in_rule__Assignment__Group__13019); 
             after(grammarAccess.getAssignmentAccess().getEqualsSignKeyword_1()); 

            }

            pushFollow(FOLLOW_rule__Assignment__Group__2_in_rule__Assignment__Group__13029);
            rule__Assignment__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Assignment__Group__1


    // $ANTLR start rule__Assignment__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1432:1: rule__Assignment__Group__2 : ( ( rule__Assignment__ExpressionAssignment_2 ) ) ;
    public final void rule__Assignment__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1436:1: ( ( ( rule__Assignment__ExpressionAssignment_2 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1437:1: ( ( rule__Assignment__ExpressionAssignment_2 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1437:1: ( ( rule__Assignment__ExpressionAssignment_2 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1438:1: ( rule__Assignment__ExpressionAssignment_2 )
            {
             before(grammarAccess.getAssignmentAccess().getExpressionAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1439:1: ( rule__Assignment__ExpressionAssignment_2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1439:2: rule__Assignment__ExpressionAssignment_2
            {
            pushFollow(FOLLOW_rule__Assignment__ExpressionAssignment_2_in_rule__Assignment__Group__23057);
            rule__Assignment__ExpressionAssignment_2();
            _fsp--;


            }

             after(grammarAccess.getAssignmentAccess().getExpressionAssignment_2()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Assignment__Group__2


    // $ANTLR start rule__If__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1455:1: rule__If__Group__0 : ( 'if' ) rule__If__Group__1 ;
    public final void rule__If__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1459:1: ( ( 'if' ) rule__If__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1460:1: ( 'if' ) rule__If__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1460:1: ( 'if' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1461:1: 'if'
            {
             before(grammarAccess.getIfAccess().getIfKeyword_0()); 
            match(input,63,FOLLOW_63_in_rule__If__Group__03098); 
             after(grammarAccess.getIfAccess().getIfKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__If__Group__1_in_rule__If__Group__03108);
            rule__If__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group__0


    // $ANTLR start rule__If__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1475:1: rule__If__Group__1 : ( ( rule__If__ExpressionAssignment_1 ) ) rule__If__Group__2 ;
    public final void rule__If__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1479:1: ( ( ( rule__If__ExpressionAssignment_1 ) ) rule__If__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1480:1: ( ( rule__If__ExpressionAssignment_1 ) ) rule__If__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1480:1: ( ( rule__If__ExpressionAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1481:1: ( rule__If__ExpressionAssignment_1 )
            {
             before(grammarAccess.getIfAccess().getExpressionAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1482:1: ( rule__If__ExpressionAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1482:2: rule__If__ExpressionAssignment_1
            {
            pushFollow(FOLLOW_rule__If__ExpressionAssignment_1_in_rule__If__Group__13136);
            rule__If__ExpressionAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getIfAccess().getExpressionAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__If__Group__2_in_rule__If__Group__13145);
            rule__If__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group__1


    // $ANTLR start rule__If__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1493:1: rule__If__Group__2 : ( 'then' ) rule__If__Group__3 ;
    public final void rule__If__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1497:1: ( ( 'then' ) rule__If__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1498:1: ( 'then' ) rule__If__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1498:1: ( 'then' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1499:1: 'then'
            {
             before(grammarAccess.getIfAccess().getThenKeyword_2()); 
            match(input,64,FOLLOW_64_in_rule__If__Group__23174); 
             after(grammarAccess.getIfAccess().getThenKeyword_2()); 

            }

            pushFollow(FOLLOW_rule__If__Group__3_in_rule__If__Group__23184);
            rule__If__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group__2


    // $ANTLR start rule__If__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1513:1: rule__If__Group__3 : ( ( RULE_TERM )* ) rule__If__Group__4 ;
    public final void rule__If__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1517:1: ( ( ( RULE_TERM )* ) rule__If__Group__4 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1518:1: ( ( RULE_TERM )* ) rule__If__Group__4
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1518:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1519:1: ( RULE_TERM )*
            {
             before(grammarAccess.getIfAccess().getTERMTerminalRuleCall_3()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1520:1: ( RULE_TERM )*
            loop17:
            do {
                int alt17=2;
                int LA17_0 = input.LA(1);

                if ( (LA17_0==RULE_TERM) ) {
                    alt17=1;
                }


                switch (alt17) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1520:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__If__Group__33213); 

            	    }
            	    break;

            	default :
            	    break loop17;
                }
            } while (true);

             after(grammarAccess.getIfAccess().getTERMTerminalRuleCall_3()); 

            }

            pushFollow(FOLLOW_rule__If__Group__4_in_rule__If__Group__33223);
            rule__If__Group__4();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group__3


    // $ANTLR start rule__If__Group__4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1531:1: rule__If__Group__4 : ( ( rule__If__TopInstrAssignment_4 ) ) rule__If__Group__5 ;
    public final void rule__If__Group__4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1535:1: ( ( ( rule__If__TopInstrAssignment_4 ) ) rule__If__Group__5 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1536:1: ( ( rule__If__TopInstrAssignment_4 ) ) rule__If__Group__5
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1536:1: ( ( rule__If__TopInstrAssignment_4 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1537:1: ( rule__If__TopInstrAssignment_4 )
            {
             before(grammarAccess.getIfAccess().getTopInstrAssignment_4()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1538:1: ( rule__If__TopInstrAssignment_4 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1538:2: rule__If__TopInstrAssignment_4
            {
            pushFollow(FOLLOW_rule__If__TopInstrAssignment_4_in_rule__If__Group__43251);
            rule__If__TopInstrAssignment_4();
            _fsp--;


            }

             after(grammarAccess.getIfAccess().getTopInstrAssignment_4()); 

            }

            pushFollow(FOLLOW_rule__If__Group__5_in_rule__If__Group__43260);
            rule__If__Group__5();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group__4


    // $ANTLR start rule__If__Group__5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1549:1: rule__If__Group__5 : ( ( rule__If__Group_5__0 )? ) ;
    public final void rule__If__Group__5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1553:1: ( ( ( rule__If__Group_5__0 )? ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1554:1: ( ( rule__If__Group_5__0 )? )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1554:1: ( ( rule__If__Group_5__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1555:1: ( rule__If__Group_5__0 )?
            {
             before(grammarAccess.getIfAccess().getGroup_5()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1556:1: ( rule__If__Group_5__0 )?
            int alt18=2;
            int LA18_0 = input.LA(1);

            if ( (LA18_0==65) ) {
                alt18=1;
            }
            switch (alt18) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1556:2: rule__If__Group_5__0
                    {
                    pushFollow(FOLLOW_rule__If__Group_5__0_in_rule__If__Group__53288);
                    rule__If__Group_5__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getIfAccess().getGroup_5()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group__5


    // $ANTLR start rule__If__Group_5__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1578:1: rule__If__Group_5__0 : ( 'else' ) rule__If__Group_5__1 ;
    public final void rule__If__Group_5__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1582:1: ( ( 'else' ) rule__If__Group_5__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1583:1: ( 'else' ) rule__If__Group_5__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1583:1: ( 'else' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1584:1: 'else'
            {
             before(grammarAccess.getIfAccess().getElseKeyword_5_0()); 
            match(input,65,FOLLOW_65_in_rule__If__Group_5__03336); 
             after(grammarAccess.getIfAccess().getElseKeyword_5_0()); 

            }

            pushFollow(FOLLOW_rule__If__Group_5__1_in_rule__If__Group_5__03346);
            rule__If__Group_5__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group_5__0


    // $ANTLR start rule__If__Group_5__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1598:1: rule__If__Group_5__1 : ( ( RULE_TERM )* ) rule__If__Group_5__2 ;
    public final void rule__If__Group_5__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1602:1: ( ( ( RULE_TERM )* ) rule__If__Group_5__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1603:1: ( ( RULE_TERM )* ) rule__If__Group_5__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1603:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1604:1: ( RULE_TERM )*
            {
             before(grammarAccess.getIfAccess().getTERMTerminalRuleCall_5_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1605:1: ( RULE_TERM )*
            loop19:
            do {
                int alt19=2;
                int LA19_0 = input.LA(1);

                if ( (LA19_0==RULE_TERM) ) {
                    alt19=1;
                }


                switch (alt19) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1605:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__If__Group_5__13375); 

            	    }
            	    break;

            	default :
            	    break loop19;
                }
            } while (true);

             after(grammarAccess.getIfAccess().getTERMTerminalRuleCall_5_1()); 

            }

            pushFollow(FOLLOW_rule__If__Group_5__2_in_rule__If__Group_5__13385);
            rule__If__Group_5__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group_5__1


    // $ANTLR start rule__If__Group_5__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1616:1: rule__If__Group_5__2 : ( ( rule__If__BottomInstrAssignment_5_2 ) ) ;
    public final void rule__If__Group_5__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1620:1: ( ( ( rule__If__BottomInstrAssignment_5_2 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1621:1: ( ( rule__If__BottomInstrAssignment_5_2 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1621:1: ( ( rule__If__BottomInstrAssignment_5_2 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1622:1: ( rule__If__BottomInstrAssignment_5_2 )
            {
             before(grammarAccess.getIfAccess().getBottomInstrAssignment_5_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1623:1: ( rule__If__BottomInstrAssignment_5_2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1623:2: rule__If__BottomInstrAssignment_5_2
            {
            pushFollow(FOLLOW_rule__If__BottomInstrAssignment_5_2_in_rule__If__Group_5__23413);
            rule__If__BottomInstrAssignment_5_2();
            _fsp--;


            }

             after(grammarAccess.getIfAccess().getBottomInstrAssignment_5_2()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__Group_5__2


    // $ANTLR start rule__Do__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1639:1: rule__Do__Group__0 : ( 'do' ) rule__Do__Group__1 ;
    public final void rule__Do__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1643:1: ( ( 'do' ) rule__Do__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1644:1: ( 'do' ) rule__Do__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1644:1: ( 'do' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1645:1: 'do'
            {
             before(grammarAccess.getDoAccess().getDoKeyword_0()); 
            match(input,66,FOLLOW_66_in_rule__Do__Group__03454); 
             after(grammarAccess.getDoAccess().getDoKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Do__Group__1_in_rule__Do__Group__03464);
            rule__Do__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Do__Group__0


    // $ANTLR start rule__Do__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1659:1: rule__Do__Group__1 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Do__Group__2 ;
    public final void rule__Do__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1663:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Do__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1664:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Do__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1664:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1665:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1665:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1666:1: ( RULE_TERM )
            {
             before(grammarAccess.getDoAccess().getTERMTerminalRuleCall_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1667:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1667:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Do__Group__13495); 

            }

             after(grammarAccess.getDoAccess().getTERMTerminalRuleCall_1()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1670:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1671:1: ( RULE_TERM )*
            {
             before(grammarAccess.getDoAccess().getTERMTerminalRuleCall_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1672:1: ( RULE_TERM )*
            loop20:
            do {
                int alt20=2;
                int LA20_0 = input.LA(1);

                if ( (LA20_0==RULE_TERM) ) {
                    alt20=1;
                }


                switch (alt20) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1672:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Do__Group__13508); 

            	    }
            	    break;

            	default :
            	    break loop20;
                }
            } while (true);

             after(grammarAccess.getDoAccess().getTERMTerminalRuleCall_1()); 

            }


            }

            pushFollow(FOLLOW_rule__Do__Group__2_in_rule__Do__Group__13520);
            rule__Do__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Do__Group__1


    // $ANTLR start rule__Do__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1684:1: rule__Do__Group__2 : ( ( ( rule__Do__InstrsAssignment_2 ) ) ( ( rule__Do__InstrsAssignment_2 )* ) ) rule__Do__Group__3 ;
    public final void rule__Do__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1688:1: ( ( ( ( rule__Do__InstrsAssignment_2 ) ) ( ( rule__Do__InstrsAssignment_2 )* ) ) rule__Do__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1689:1: ( ( ( rule__Do__InstrsAssignment_2 ) ) ( ( rule__Do__InstrsAssignment_2 )* ) ) rule__Do__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1689:1: ( ( ( rule__Do__InstrsAssignment_2 ) ) ( ( rule__Do__InstrsAssignment_2 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1690:1: ( ( rule__Do__InstrsAssignment_2 ) ) ( ( rule__Do__InstrsAssignment_2 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1690:1: ( ( rule__Do__InstrsAssignment_2 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1691:1: ( rule__Do__InstrsAssignment_2 )
            {
             before(grammarAccess.getDoAccess().getInstrsAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1692:1: ( rule__Do__InstrsAssignment_2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1692:2: rule__Do__InstrsAssignment_2
            {
            pushFollow(FOLLOW_rule__Do__InstrsAssignment_2_in_rule__Do__Group__23550);
            rule__Do__InstrsAssignment_2();
            _fsp--;


            }

             after(grammarAccess.getDoAccess().getInstrsAssignment_2()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1695:1: ( ( rule__Do__InstrsAssignment_2 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1696:1: ( rule__Do__InstrsAssignment_2 )*
            {
             before(grammarAccess.getDoAccess().getInstrsAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1697:1: ( rule__Do__InstrsAssignment_2 )*
            loop21:
            do {
                int alt21=2;
                int LA21_0 = input.LA(1);

                if ( ((LA21_0>=RULE_STRING && LA21_0<=RULE_SYMBOL_VAR)||LA21_0==63||LA21_0==66||(LA21_0>=68 && LA21_0<=69)||(LA21_0>=71 && LA21_0<=72)) ) {
                    alt21=1;
                }


                switch (alt21) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1697:2: rule__Do__InstrsAssignment_2
            	    {
            	    pushFollow(FOLLOW_rule__Do__InstrsAssignment_2_in_rule__Do__Group__23562);
            	    rule__Do__InstrsAssignment_2();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop21;
                }
            } while (true);

             after(grammarAccess.getDoAccess().getInstrsAssignment_2()); 

            }


            }

            pushFollow(FOLLOW_rule__Do__Group__3_in_rule__Do__Group__23574);
            rule__Do__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Do__Group__2


    // $ANTLR start rule__Do__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1709:1: rule__Do__Group__3 : ( 'end' ) ;
    public final void rule__Do__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1713:1: ( ( 'end' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1714:1: ( 'end' )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1714:1: ( 'end' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1715:1: 'end'
            {
             before(grammarAccess.getDoAccess().getEndKeyword_3()); 
            match(input,67,FOLLOW_67_in_rule__Do__Group__33603); 
             after(grammarAccess.getDoAccess().getEndKeyword_3()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Do__Group__3


    // $ANTLR start rule__Call__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1736:1: rule__Call__Group__0 : ( 'call' ) rule__Call__Group__1 ;
    public final void rule__Call__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1740:1: ( ( 'call' ) rule__Call__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1741:1: ( 'call' ) rule__Call__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1741:1: ( 'call' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1742:1: 'call'
            {
             before(grammarAccess.getCallAccess().getCallKeyword_0()); 
            match(input,68,FOLLOW_68_in_rule__Call__Group__03647); 
             after(grammarAccess.getCallAccess().getCallKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Call__Group__1_in_rule__Call__Group__03657);
            rule__Call__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Call__Group__0


    // $ANTLR start rule__Call__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1756:1: rule__Call__Group__1 : ( ( rule__Call__NameAssignment_1 ) ) rule__Call__Group__2 ;
    public final void rule__Call__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1760:1: ( ( ( rule__Call__NameAssignment_1 ) ) rule__Call__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1761:1: ( ( rule__Call__NameAssignment_1 ) ) rule__Call__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1761:1: ( ( rule__Call__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1762:1: ( rule__Call__NameAssignment_1 )
            {
             before(grammarAccess.getCallAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1763:1: ( rule__Call__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1763:2: rule__Call__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__Call__NameAssignment_1_in_rule__Call__Group__13685);
            rule__Call__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getCallAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__Call__Group__2_in_rule__Call__Group__13694);
            rule__Call__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Call__Group__1


    // $ANTLR start rule__Call__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1774:1: rule__Call__Group__2 : ( ( rule__Call__ArgsAssignment_2 ) ) ;
    public final void rule__Call__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1778:1: ( ( ( rule__Call__ArgsAssignment_2 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1779:1: ( ( rule__Call__ArgsAssignment_2 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1779:1: ( ( rule__Call__ArgsAssignment_2 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1780:1: ( rule__Call__ArgsAssignment_2 )
            {
             before(grammarAccess.getCallAccess().getArgsAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1781:1: ( rule__Call__ArgsAssignment_2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1781:2: rule__Call__ArgsAssignment_2
            {
            pushFollow(FOLLOW_rule__Call__ArgsAssignment_2_in_rule__Call__Group__23722);
            rule__Call__ArgsAssignment_2();
            _fsp--;


            }

             after(grammarAccess.getCallAccess().getArgsAssignment_2()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Call__Group__2


    // $ANTLR start rule__Arg__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1797:1: rule__Arg__Group__0 : ( () ) rule__Arg__Group__1 ;
    public final void rule__Arg__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1801:1: ( ( () ) rule__Arg__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1802:1: ( () ) rule__Arg__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1802:1: ( () )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1803:1: ()
            {
             before(grammarAccess.getArgAccess().getArgAction_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1804:1: ()
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1806:1: 
            {
            }

             after(grammarAccess.getArgAccess().getArgAction_0()); 

            }

            pushFollow(FOLLOW_rule__Arg__Group__1_in_rule__Arg__Group__03772);
            rule__Arg__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__Group__0


    // $ANTLR start rule__Arg__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1817:1: rule__Arg__Group__1 : ( 'arg' ) rule__Arg__Group__2 ;
    public final void rule__Arg__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1821:1: ( ( 'arg' ) rule__Arg__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1822:1: ( 'arg' ) rule__Arg__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1822:1: ( 'arg' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1823:1: 'arg'
            {
             before(grammarAccess.getArgAccess().getArgKeyword_1()); 
            match(input,69,FOLLOW_69_in_rule__Arg__Group__13801); 
             after(grammarAccess.getArgAccess().getArgKeyword_1()); 

            }

            pushFollow(FOLLOW_rule__Arg__Group__2_in_rule__Arg__Group__13811);
            rule__Arg__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__Group__1


    // $ANTLR start rule__Arg__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1837:1: rule__Arg__Group__2 : ( ( rule__Arg__Group_2__0 )? ) ;
    public final void rule__Arg__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1841:1: ( ( ( rule__Arg__Group_2__0 )? ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1842:1: ( ( rule__Arg__Group_2__0 )? )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1842:1: ( ( rule__Arg__Group_2__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1843:1: ( rule__Arg__Group_2__0 )?
            {
             before(grammarAccess.getArgAccess().getGroup_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1844:1: ( rule__Arg__Group_2__0 )?
            int alt22=2;
            int LA22_0 = input.LA(1);

            if ( ((LA22_0>=RULE_STRING && LA22_0<=RULE_SYMBOL_VAR)) ) {
                alt22=1;
            }
            switch (alt22) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1844:2: rule__Arg__Group_2__0
                    {
                    pushFollow(FOLLOW_rule__Arg__Group_2__0_in_rule__Arg__Group__23839);
                    rule__Arg__Group_2__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getArgAccess().getGroup_2()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__Group__2


    // $ANTLR start rule__Arg__Group_2__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1860:1: rule__Arg__Group_2__0 : ( ( rule__Arg__ArgsAssignment_2_0 ) ) rule__Arg__Group_2__1 ;
    public final void rule__Arg__Group_2__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1864:1: ( ( ( rule__Arg__ArgsAssignment_2_0 ) ) rule__Arg__Group_2__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1865:1: ( ( rule__Arg__ArgsAssignment_2_0 ) ) rule__Arg__Group_2__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1865:1: ( ( rule__Arg__ArgsAssignment_2_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1866:1: ( rule__Arg__ArgsAssignment_2_0 )
            {
             before(grammarAccess.getArgAccess().getArgsAssignment_2_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1867:1: ( rule__Arg__ArgsAssignment_2_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1867:2: rule__Arg__ArgsAssignment_2_0
            {
            pushFollow(FOLLOW_rule__Arg__ArgsAssignment_2_0_in_rule__Arg__Group_2__03880);
            rule__Arg__ArgsAssignment_2_0();
            _fsp--;


            }

             after(grammarAccess.getArgAccess().getArgsAssignment_2_0()); 

            }

            pushFollow(FOLLOW_rule__Arg__Group_2__1_in_rule__Arg__Group_2__03889);
            rule__Arg__Group_2__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__Group_2__0


    // $ANTLR start rule__Arg__Group_2__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1878:1: rule__Arg__Group_2__1 : ( ( rule__Arg__Group_2_1__0 )* ) ;
    public final void rule__Arg__Group_2__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1882:1: ( ( ( rule__Arg__Group_2_1__0 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1883:1: ( ( rule__Arg__Group_2_1__0 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1883:1: ( ( rule__Arg__Group_2_1__0 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1884:1: ( rule__Arg__Group_2_1__0 )*
            {
             before(grammarAccess.getArgAccess().getGroup_2_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1885:1: ( rule__Arg__Group_2_1__0 )*
            loop23:
            do {
                int alt23=2;
                int LA23_0 = input.LA(1);

                if ( (LA23_0==70) ) {
                    alt23=1;
                }


                switch (alt23) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1885:2: rule__Arg__Group_2_1__0
            	    {
            	    pushFollow(FOLLOW_rule__Arg__Group_2_1__0_in_rule__Arg__Group_2__13917);
            	    rule__Arg__Group_2_1__0();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop23;
                }
            } while (true);

             after(grammarAccess.getArgAccess().getGroup_2_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__Group_2__1


    // $ANTLR start rule__Arg__Group_2_1__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1899:1: rule__Arg__Group_2_1__0 : ( ',' ) rule__Arg__Group_2_1__1 ;
    public final void rule__Arg__Group_2_1__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1903:1: ( ( ',' ) rule__Arg__Group_2_1__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1904:1: ( ',' ) rule__Arg__Group_2_1__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1904:1: ( ',' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1905:1: ','
            {
             before(grammarAccess.getArgAccess().getCommaKeyword_2_1_0()); 
            match(input,70,FOLLOW_70_in_rule__Arg__Group_2_1__03957); 
             after(grammarAccess.getArgAccess().getCommaKeyword_2_1_0()); 

            }

            pushFollow(FOLLOW_rule__Arg__Group_2_1__1_in_rule__Arg__Group_2_1__03967);
            rule__Arg__Group_2_1__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__Group_2_1__0


    // $ANTLR start rule__Arg__Group_2_1__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1919:1: rule__Arg__Group_2_1__1 : ( ( rule__Arg__ArgsAssignment_2_1_1 ) ) ;
    public final void rule__Arg__Group_2_1__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1923:1: ( ( ( rule__Arg__ArgsAssignment_2_1_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1924:1: ( ( rule__Arg__ArgsAssignment_2_1_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1924:1: ( ( rule__Arg__ArgsAssignment_2_1_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1925:1: ( rule__Arg__ArgsAssignment_2_1_1 )
            {
             before(grammarAccess.getArgAccess().getArgsAssignment_2_1_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1926:1: ( rule__Arg__ArgsAssignment_2_1_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1926:2: rule__Arg__ArgsAssignment_2_1_1
            {
            pushFollow(FOLLOW_rule__Arg__ArgsAssignment_2_1_1_in_rule__Arg__Group_2_1__13995);
            rule__Arg__ArgsAssignment_2_1_1();
            _fsp--;


            }

             after(grammarAccess.getArgAccess().getArgsAssignment_2_1_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__Group_2_1__1


    // $ANTLR start rule__UseArg__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1940:1: rule__UseArg__Group__0 : ( () ) rule__UseArg__Group__1 ;
    public final void rule__UseArg__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1944:1: ( ( () ) rule__UseArg__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1945:1: ( () ) rule__UseArg__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1945:1: ( () )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1946:1: ()
            {
             before(grammarAccess.getUseArgAccess().getUseArgAction_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1947:1: ()
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1949:1: 
            {
            }

             after(grammarAccess.getUseArgAccess().getUseArgAction_0()); 

            }

            pushFollow(FOLLOW_rule__UseArg__Group__1_in_rule__UseArg__Group__04043);
            rule__UseArg__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group__0


    // $ANTLR start rule__UseArg__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1960:1: rule__UseArg__Group__1 : ( 'use' ) rule__UseArg__Group__2 ;
    public final void rule__UseArg__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1964:1: ( ( 'use' ) rule__UseArg__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1965:1: ( 'use' ) rule__UseArg__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1965:1: ( 'use' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1966:1: 'use'
            {
             before(grammarAccess.getUseArgAccess().getUseKeyword_1()); 
            match(input,71,FOLLOW_71_in_rule__UseArg__Group__14072); 
             after(grammarAccess.getUseArgAccess().getUseKeyword_1()); 

            }

            pushFollow(FOLLOW_rule__UseArg__Group__2_in_rule__UseArg__Group__14082);
            rule__UseArg__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group__1


    // $ANTLR start rule__UseArg__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1980:1: rule__UseArg__Group__2 : ( ( rule__UseArg__StrictAssignment_2 )? ) rule__UseArg__Group__3 ;
    public final void rule__UseArg__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1984:1: ( ( ( rule__UseArg__StrictAssignment_2 )? ) rule__UseArg__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1985:1: ( ( rule__UseArg__StrictAssignment_2 )? ) rule__UseArg__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1985:1: ( ( rule__UseArg__StrictAssignment_2 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1986:1: ( rule__UseArg__StrictAssignment_2 )?
            {
             before(grammarAccess.getUseArgAccess().getStrictAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1987:1: ( rule__UseArg__StrictAssignment_2 )?
            int alt24=2;
            int LA24_0 = input.LA(1);

            if ( (LA24_0==96) ) {
                alt24=1;
            }
            switch (alt24) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1987:2: rule__UseArg__StrictAssignment_2
                    {
                    pushFollow(FOLLOW_rule__UseArg__StrictAssignment_2_in_rule__UseArg__Group__24110);
                    rule__UseArg__StrictAssignment_2();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getUseArgAccess().getStrictAssignment_2()); 

            }

            pushFollow(FOLLOW_rule__UseArg__Group__3_in_rule__UseArg__Group__24120);
            rule__UseArg__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group__2


    // $ANTLR start rule__UseArg__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1998:1: rule__UseArg__Group__3 : ( 'arg' ) rule__UseArg__Group__4 ;
    public final void rule__UseArg__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2002:1: ( ( 'arg' ) rule__UseArg__Group__4 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2003:1: ( 'arg' ) rule__UseArg__Group__4
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2003:1: ( 'arg' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2004:1: 'arg'
            {
             before(grammarAccess.getUseArgAccess().getArgKeyword_3()); 
            match(input,69,FOLLOW_69_in_rule__UseArg__Group__34149); 
             after(grammarAccess.getUseArgAccess().getArgKeyword_3()); 

            }

            pushFollow(FOLLOW_rule__UseArg__Group__4_in_rule__UseArg__Group__34159);
            rule__UseArg__Group__4();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group__3


    // $ANTLR start rule__UseArg__Group__4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2018:1: rule__UseArg__Group__4 : ( ( rule__UseArg__Group_4__0 )? ) ;
    public final void rule__UseArg__Group__4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2022:1: ( ( ( rule__UseArg__Group_4__0 )? ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2023:1: ( ( rule__UseArg__Group_4__0 )? )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2023:1: ( ( rule__UseArg__Group_4__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2024:1: ( rule__UseArg__Group_4__0 )?
            {
             before(grammarAccess.getUseArgAccess().getGroup_4()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2025:1: ( rule__UseArg__Group_4__0 )?
            int alt25=2;
            int LA25_0 = input.LA(1);

            if ( ((LA25_0>=RULE_STRING && LA25_0<=RULE_SYMBOL_VAR)) ) {
                alt25=1;
            }
            switch (alt25) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2025:2: rule__UseArg__Group_4__0
                    {
                    pushFollow(FOLLOW_rule__UseArg__Group_4__0_in_rule__UseArg__Group__44187);
                    rule__UseArg__Group_4__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getUseArgAccess().getGroup_4()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group__4


    // $ANTLR start rule__UseArg__Group_4__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2045:1: rule__UseArg__Group_4__0 : ( ( rule__UseArg__ArgsAssignment_4_0 ) ) rule__UseArg__Group_4__1 ;
    public final void rule__UseArg__Group_4__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2049:1: ( ( ( rule__UseArg__ArgsAssignment_4_0 ) ) rule__UseArg__Group_4__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2050:1: ( ( rule__UseArg__ArgsAssignment_4_0 ) ) rule__UseArg__Group_4__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2050:1: ( ( rule__UseArg__ArgsAssignment_4_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2051:1: ( rule__UseArg__ArgsAssignment_4_0 )
            {
             before(grammarAccess.getUseArgAccess().getArgsAssignment_4_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2052:1: ( rule__UseArg__ArgsAssignment_4_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2052:2: rule__UseArg__ArgsAssignment_4_0
            {
            pushFollow(FOLLOW_rule__UseArg__ArgsAssignment_4_0_in_rule__UseArg__Group_4__04232);
            rule__UseArg__ArgsAssignment_4_0();
            _fsp--;


            }

             after(grammarAccess.getUseArgAccess().getArgsAssignment_4_0()); 

            }

            pushFollow(FOLLOW_rule__UseArg__Group_4__1_in_rule__UseArg__Group_4__04241);
            rule__UseArg__Group_4__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group_4__0


    // $ANTLR start rule__UseArg__Group_4__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2063:1: rule__UseArg__Group_4__1 : ( ( rule__UseArg__Group_4_1__0 )* ) ;
    public final void rule__UseArg__Group_4__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2067:1: ( ( ( rule__UseArg__Group_4_1__0 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2068:1: ( ( rule__UseArg__Group_4_1__0 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2068:1: ( ( rule__UseArg__Group_4_1__0 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2069:1: ( rule__UseArg__Group_4_1__0 )*
            {
             before(grammarAccess.getUseArgAccess().getGroup_4_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2070:1: ( rule__UseArg__Group_4_1__0 )*
            loop26:
            do {
                int alt26=2;
                int LA26_0 = input.LA(1);

                if ( (LA26_0==70) ) {
                    alt26=1;
                }


                switch (alt26) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2070:2: rule__UseArg__Group_4_1__0
            	    {
            	    pushFollow(FOLLOW_rule__UseArg__Group_4_1__0_in_rule__UseArg__Group_4__14269);
            	    rule__UseArg__Group_4_1__0();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop26;
                }
            } while (true);

             after(grammarAccess.getUseArgAccess().getGroup_4_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group_4__1


    // $ANTLR start rule__UseArg__Group_4_1__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2084:1: rule__UseArg__Group_4_1__0 : ( ',' ) rule__UseArg__Group_4_1__1 ;
    public final void rule__UseArg__Group_4_1__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2088:1: ( ( ',' ) rule__UseArg__Group_4_1__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2089:1: ( ',' ) rule__UseArg__Group_4_1__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2089:1: ( ',' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2090:1: ','
            {
             before(grammarAccess.getUseArgAccess().getCommaKeyword_4_1_0()); 
            match(input,70,FOLLOW_70_in_rule__UseArg__Group_4_1__04309); 
             after(grammarAccess.getUseArgAccess().getCommaKeyword_4_1_0()); 

            }

            pushFollow(FOLLOW_rule__UseArg__Group_4_1__1_in_rule__UseArg__Group_4_1__04319);
            rule__UseArg__Group_4_1__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group_4_1__0


    // $ANTLR start rule__UseArg__Group_4_1__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2104:1: rule__UseArg__Group_4_1__1 : ( ( rule__UseArg__ArgsAssignment_4_1_1 ) ) ;
    public final void rule__UseArg__Group_4_1__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2108:1: ( ( ( rule__UseArg__ArgsAssignment_4_1_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2109:1: ( ( rule__UseArg__ArgsAssignment_4_1_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2109:1: ( ( rule__UseArg__ArgsAssignment_4_1_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2110:1: ( rule__UseArg__ArgsAssignment_4_1_1 )
            {
             before(grammarAccess.getUseArgAccess().getArgsAssignment_4_1_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2111:1: ( rule__UseArg__ArgsAssignment_4_1_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2111:2: rule__UseArg__ArgsAssignment_4_1_1
            {
            pushFollow(FOLLOW_rule__UseArg__ArgsAssignment_4_1_1_in_rule__UseArg__Group_4_1__14347);
            rule__UseArg__ArgsAssignment_4_1_1();
            _fsp--;


            }

             after(grammarAccess.getUseArgAccess().getArgsAssignment_4_1_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__Group_4_1__1


    // $ANTLR start rule__UseArgs__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2125:1: rule__UseArgs__Group__0 : ( ( rule__UseArgs__NameAssignment_0 ) ) rule__UseArgs__Group__1 ;
    public final void rule__UseArgs__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2129:1: ( ( ( rule__UseArgs__NameAssignment_0 ) ) rule__UseArgs__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2130:1: ( ( rule__UseArgs__NameAssignment_0 ) ) rule__UseArgs__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2130:1: ( ( rule__UseArgs__NameAssignment_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2131:1: ( rule__UseArgs__NameAssignment_0 )
            {
             before(grammarAccess.getUseArgsAccess().getNameAssignment_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2132:1: ( rule__UseArgs__NameAssignment_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2132:2: rule__UseArgs__NameAssignment_0
            {
            pushFollow(FOLLOW_rule__UseArgs__NameAssignment_0_in_rule__UseArgs__Group__04385);
            rule__UseArgs__NameAssignment_0();
            _fsp--;


            }

             after(grammarAccess.getUseArgsAccess().getNameAssignment_0()); 

            }

            pushFollow(FOLLOW_rule__UseArgs__Group__1_in_rule__UseArgs__Group__04394);
            rule__UseArgs__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArgs__Group__0


    // $ANTLR start rule__UseArgs__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2143:1: rule__UseArgs__Group__1 : ( ( rule__UseArgs__Group_1__0 )? ) ;
    public final void rule__UseArgs__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2147:1: ( ( ( rule__UseArgs__Group_1__0 )? ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2148:1: ( ( rule__UseArgs__Group_1__0 )? )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2148:1: ( ( rule__UseArgs__Group_1__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2149:1: ( rule__UseArgs__Group_1__0 )?
            {
             before(grammarAccess.getUseArgsAccess().getGroup_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2150:1: ( rule__UseArgs__Group_1__0 )?
            int alt27=2;
            int LA27_0 = input.LA(1);

            if ( (LA27_0==62) ) {
                alt27=1;
            }
            switch (alt27) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2150:2: rule__UseArgs__Group_1__0
                    {
                    pushFollow(FOLLOW_rule__UseArgs__Group_1__0_in_rule__UseArgs__Group__14422);
                    rule__UseArgs__Group_1__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getUseArgsAccess().getGroup_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArgs__Group__1


    // $ANTLR start rule__UseArgs__Group_1__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2164:1: rule__UseArgs__Group_1__0 : ( '=' ) rule__UseArgs__Group_1__1 ;
    public final void rule__UseArgs__Group_1__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2168:1: ( ( '=' ) rule__UseArgs__Group_1__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2169:1: ( '=' ) rule__UseArgs__Group_1__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2169:1: ( '=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2170:1: '='
            {
             before(grammarAccess.getUseArgsAccess().getEqualsSignKeyword_1_0()); 
            match(input,62,FOLLOW_62_in_rule__UseArgs__Group_1__04462); 
             after(grammarAccess.getUseArgsAccess().getEqualsSignKeyword_1_0()); 

            }

            pushFollow(FOLLOW_rule__UseArgs__Group_1__1_in_rule__UseArgs__Group_1__04472);
            rule__UseArgs__Group_1__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArgs__Group_1__0


    // $ANTLR start rule__UseArgs__Group_1__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2184:1: rule__UseArgs__Group_1__1 : ( ( rule__UseArgs__DefaultAssignment_1_1 ) ) ;
    public final void rule__UseArgs__Group_1__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2188:1: ( ( ( rule__UseArgs__DefaultAssignment_1_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2189:1: ( ( rule__UseArgs__DefaultAssignment_1_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2189:1: ( ( rule__UseArgs__DefaultAssignment_1_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2190:1: ( rule__UseArgs__DefaultAssignment_1_1 )
            {
             before(grammarAccess.getUseArgsAccess().getDefaultAssignment_1_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2191:1: ( rule__UseArgs__DefaultAssignment_1_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2191:2: rule__UseArgs__DefaultAssignment_1_1
            {
            pushFollow(FOLLOW_rule__UseArgs__DefaultAssignment_1_1_in_rule__UseArgs__Group_1__14500);
            rule__UseArgs__DefaultAssignment_1_1();
            _fsp--;


            }

             after(grammarAccess.getUseArgsAccess().getDefaultAssignment_1_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArgs__Group_1__1


    // $ANTLR start rule__Arguments__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2205:1: rule__Arguments__Group__0 : ( () ) rule__Arguments__Group__1 ;
    public final void rule__Arguments__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2209:1: ( ( () ) rule__Arguments__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2210:1: ( () ) rule__Arguments__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2210:1: ( () )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2211:1: ()
            {
             before(grammarAccess.getArgumentsAccess().getArgumentsAction_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2212:1: ()
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2214:1: 
            {
            }

             after(grammarAccess.getArgumentsAccess().getArgumentsAction_0()); 

            }

            pushFollow(FOLLOW_rule__Arguments__Group__1_in_rule__Arguments__Group__04548);
            rule__Arguments__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arguments__Group__0


    // $ANTLR start rule__Arguments__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2225:1: rule__Arguments__Group__1 : ( ( rule__Arguments__Group_1__0 )? ) ;
    public final void rule__Arguments__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2229:1: ( ( ( rule__Arguments__Group_1__0 )? ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2230:1: ( ( rule__Arguments__Group_1__0 )? )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2230:1: ( ( rule__Arguments__Group_1__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2231:1: ( rule__Arguments__Group_1__0 )?
            {
             before(grammarAccess.getArgumentsAccess().getGroup_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2232:1: ( rule__Arguments__Group_1__0 )?
            int alt28=2;
            int LA28_0 = input.LA(1);

            if ( ((LA28_0>=RULE_SYMBOL_NUM && LA28_0<=RULE_SYMBOL_VAR)||LA28_0==81) ) {
                alt28=1;
            }
            switch (alt28) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2232:2: rule__Arguments__Group_1__0
                    {
                    pushFollow(FOLLOW_rule__Arguments__Group_1__0_in_rule__Arguments__Group__14576);
                    rule__Arguments__Group_1__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getArgumentsAccess().getGroup_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arguments__Group__1


    // $ANTLR start rule__Arguments__Group_1__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2246:1: rule__Arguments__Group_1__0 : ( ( rule__Arguments__ArgumentsAssignment_1_0 ) ) rule__Arguments__Group_1__1 ;
    public final void rule__Arguments__Group_1__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2250:1: ( ( ( rule__Arguments__ArgumentsAssignment_1_0 ) ) rule__Arguments__Group_1__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2251:1: ( ( rule__Arguments__ArgumentsAssignment_1_0 ) ) rule__Arguments__Group_1__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2251:1: ( ( rule__Arguments__ArgumentsAssignment_1_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2252:1: ( rule__Arguments__ArgumentsAssignment_1_0 )
            {
             before(grammarAccess.getArgumentsAccess().getArgumentsAssignment_1_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2253:1: ( rule__Arguments__ArgumentsAssignment_1_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2253:2: rule__Arguments__ArgumentsAssignment_1_0
            {
            pushFollow(FOLLOW_rule__Arguments__ArgumentsAssignment_1_0_in_rule__Arguments__Group_1__04615);
            rule__Arguments__ArgumentsAssignment_1_0();
            _fsp--;


            }

             after(grammarAccess.getArgumentsAccess().getArgumentsAssignment_1_0()); 

            }

            pushFollow(FOLLOW_rule__Arguments__Group_1__1_in_rule__Arguments__Group_1__04624);
            rule__Arguments__Group_1__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arguments__Group_1__0


    // $ANTLR start rule__Arguments__Group_1__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2264:1: rule__Arguments__Group_1__1 : ( ( rule__Arguments__Group_1_1__0 )* ) ;
    public final void rule__Arguments__Group_1__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2268:1: ( ( ( rule__Arguments__Group_1_1__0 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2269:1: ( ( rule__Arguments__Group_1_1__0 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2269:1: ( ( rule__Arguments__Group_1_1__0 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2270:1: ( rule__Arguments__Group_1_1__0 )*
            {
             before(grammarAccess.getArgumentsAccess().getGroup_1_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2271:1: ( rule__Arguments__Group_1_1__0 )*
            loop29:
            do {
                int alt29=2;
                int LA29_0 = input.LA(1);

                if ( (LA29_0==70) ) {
                    alt29=1;
                }


                switch (alt29) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2271:2: rule__Arguments__Group_1_1__0
            	    {
            	    pushFollow(FOLLOW_rule__Arguments__Group_1_1__0_in_rule__Arguments__Group_1__14652);
            	    rule__Arguments__Group_1_1__0();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop29;
                }
            } while (true);

             after(grammarAccess.getArgumentsAccess().getGroup_1_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arguments__Group_1__1


    // $ANTLR start rule__Arguments__Group_1_1__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2285:1: rule__Arguments__Group_1_1__0 : ( ',' ) rule__Arguments__Group_1_1__1 ;
    public final void rule__Arguments__Group_1_1__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2289:1: ( ( ',' ) rule__Arguments__Group_1_1__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2290:1: ( ',' ) rule__Arguments__Group_1_1__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2290:1: ( ',' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2291:1: ','
            {
             before(grammarAccess.getArgumentsAccess().getCommaKeyword_1_1_0()); 
            match(input,70,FOLLOW_70_in_rule__Arguments__Group_1_1__04692); 
             after(grammarAccess.getArgumentsAccess().getCommaKeyword_1_1_0()); 

            }

            pushFollow(FOLLOW_rule__Arguments__Group_1_1__1_in_rule__Arguments__Group_1_1__04702);
            rule__Arguments__Group_1_1__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arguments__Group_1_1__0


    // $ANTLR start rule__Arguments__Group_1_1__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2305:1: rule__Arguments__Group_1_1__1 : ( ( rule__Arguments__ArgumentsAssignment_1_1_1 ) ) ;
    public final void rule__Arguments__Group_1_1__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2309:1: ( ( ( rule__Arguments__ArgumentsAssignment_1_1_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2310:1: ( ( rule__Arguments__ArgumentsAssignment_1_1_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2310:1: ( ( rule__Arguments__ArgumentsAssignment_1_1_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2311:1: ( rule__Arguments__ArgumentsAssignment_1_1_1 )
            {
             before(grammarAccess.getArgumentsAccess().getArgumentsAssignment_1_1_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2312:1: ( rule__Arguments__ArgumentsAssignment_1_1_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2312:2: rule__Arguments__ArgumentsAssignment_1_1_1
            {
            pushFollow(FOLLOW_rule__Arguments__ArgumentsAssignment_1_1_1_in_rule__Arguments__Group_1_1__14730);
            rule__Arguments__ArgumentsAssignment_1_1_1();
            _fsp--;


            }

             after(grammarAccess.getArgumentsAccess().getArgumentsAssignment_1_1_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arguments__Group_1_1__1


    // $ANTLR start rule__Raise__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2326:1: rule__Raise__Group__0 : ( 'raise' ) rule__Raise__Group__1 ;
    public final void rule__Raise__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2330:1: ( ( 'raise' ) rule__Raise__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2331:1: ( 'raise' ) rule__Raise__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2331:1: ( 'raise' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2332:1: 'raise'
            {
             before(grammarAccess.getRaiseAccess().getRaiseKeyword_0()); 
            match(input,72,FOLLOW_72_in_rule__Raise__Group__04769); 
             after(grammarAccess.getRaiseAccess().getRaiseKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Raise__Group__1_in_rule__Raise__Group__04779);
            rule__Raise__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Raise__Group__0


    // $ANTLR start rule__Raise__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2346:1: rule__Raise__Group__1 : ( 'syntax' ) rule__Raise__Group__2 ;
    public final void rule__Raise__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2350:1: ( ( 'syntax' ) rule__Raise__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2351:1: ( 'syntax' ) rule__Raise__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2351:1: ( 'syntax' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2352:1: 'syntax'
            {
             before(grammarAccess.getRaiseAccess().getSyntaxKeyword_1()); 
            match(input,73,FOLLOW_73_in_rule__Raise__Group__14808); 
             after(grammarAccess.getRaiseAccess().getSyntaxKeyword_1()); 

            }

            pushFollow(FOLLOW_rule__Raise__Group__2_in_rule__Raise__Group__14818);
            rule__Raise__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Raise__Group__1


    // $ANTLR start rule__Raise__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2366:1: rule__Raise__Group__2 : ( ( rule__Raise__MayorAssignment_2 ) ) rule__Raise__Group__3 ;
    public final void rule__Raise__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2370:1: ( ( ( rule__Raise__MayorAssignment_2 ) ) rule__Raise__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2371:1: ( ( rule__Raise__MayorAssignment_2 ) ) rule__Raise__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2371:1: ( ( rule__Raise__MayorAssignment_2 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2372:1: ( rule__Raise__MayorAssignment_2 )
            {
             before(grammarAccess.getRaiseAccess().getMayorAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2373:1: ( rule__Raise__MayorAssignment_2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2373:2: rule__Raise__MayorAssignment_2
            {
            pushFollow(FOLLOW_rule__Raise__MayorAssignment_2_in_rule__Raise__Group__24846);
            rule__Raise__MayorAssignment_2();
            _fsp--;


            }

             after(grammarAccess.getRaiseAccess().getMayorAssignment_2()); 

            }

            pushFollow(FOLLOW_rule__Raise__Group__3_in_rule__Raise__Group__24855);
            rule__Raise__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Raise__Group__2


    // $ANTLR start rule__Raise__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2384:1: rule__Raise__Group__3 : ( ( rule__Raise__Group_3__0 ) ) ;
    public final void rule__Raise__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2388:1: ( ( ( rule__Raise__Group_3__0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2389:1: ( ( rule__Raise__Group_3__0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2389:1: ( ( rule__Raise__Group_3__0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2390:1: ( rule__Raise__Group_3__0 )
            {
             before(grammarAccess.getRaiseAccess().getGroup_3()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2391:1: ( rule__Raise__Group_3__0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2391:2: rule__Raise__Group_3__0
            {
            pushFollow(FOLLOW_rule__Raise__Group_3__0_in_rule__Raise__Group__34883);
            rule__Raise__Group_3__0();
            _fsp--;


            }

             after(grammarAccess.getRaiseAccess().getGroup_3()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Raise__Group__3


    // $ANTLR start rule__Raise__Group_3__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2409:1: rule__Raise__Group_3__0 : ( '.' ) rule__Raise__Group_3__1 ;
    public final void rule__Raise__Group_3__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2413:1: ( ( '.' ) rule__Raise__Group_3__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2414:1: ( '.' ) rule__Raise__Group_3__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2414:1: ( '.' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2415:1: '.'
            {
             before(grammarAccess.getRaiseAccess().getFullStopKeyword_3_0()); 
            match(input,74,FOLLOW_74_in_rule__Raise__Group_3__04926); 
             after(grammarAccess.getRaiseAccess().getFullStopKeyword_3_0()); 

            }

            pushFollow(FOLLOW_rule__Raise__Group_3__1_in_rule__Raise__Group_3__04936);
            rule__Raise__Group_3__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Raise__Group_3__0


    // $ANTLR start rule__Raise__Group_3__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2429:1: rule__Raise__Group_3__1 : ( ( rule__Raise__MinorAssignment_3_1 ) ) ;
    public final void rule__Raise__Group_3__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2433:1: ( ( ( rule__Raise__MinorAssignment_3_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2434:1: ( ( rule__Raise__MinorAssignment_3_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2434:1: ( ( rule__Raise__MinorAssignment_3_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2435:1: ( rule__Raise__MinorAssignment_3_1 )
            {
             before(grammarAccess.getRaiseAccess().getMinorAssignment_3_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2436:1: ( rule__Raise__MinorAssignment_3_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2436:2: rule__Raise__MinorAssignment_3_1
            {
            pushFollow(FOLLOW_rule__Raise__MinorAssignment_3_1_in_rule__Raise__Group_3__14964);
            rule__Raise__MinorAssignment_3_1();
            _fsp--;


            }

             after(grammarAccess.getRaiseAccess().getMinorAssignment_3_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Raise__Group_3__1


    // $ANTLR start rule__Expression__Group_0__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2450:1: rule__Expression__Group_0__0 : ( () ) rule__Expression__Group_0__1 ;
    public final void rule__Expression__Group_0__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2454:1: ( ( () ) rule__Expression__Group_0__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2455:1: ( () ) rule__Expression__Group_0__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2455:1: ( () )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2456:1: ()
            {
             before(grammarAccess.getExpressionAccess().getExprVarAction_0_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2457:1: ()
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2459:1: 
            {
            }

             after(grammarAccess.getExpressionAccess().getExprVarAction_0_0()); 

            }

            pushFollow(FOLLOW_rule__Expression__Group_0__1_in_rule__Expression__Group_0__05012);
            rule__Expression__Group_0__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Expression__Group_0__0


    // $ANTLR start rule__Expression__Group_0__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2470:1: rule__Expression__Group_0__1 : ( ( rule__Expression__NameAssignment_0_1 ) ) ;
    public final void rule__Expression__Group_0__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2474:1: ( ( ( rule__Expression__NameAssignment_0_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2475:1: ( ( rule__Expression__NameAssignment_0_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2475:1: ( ( rule__Expression__NameAssignment_0_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2476:1: ( rule__Expression__NameAssignment_0_1 )
            {
             before(grammarAccess.getExpressionAccess().getNameAssignment_0_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2477:1: ( rule__Expression__NameAssignment_0_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2477:2: rule__Expression__NameAssignment_0_1
            {
            pushFollow(FOLLOW_rule__Expression__NameAssignment_0_1_in_rule__Expression__Group_0__15040);
            rule__Expression__NameAssignment_0_1();
            _fsp--;


            }

             after(grammarAccess.getExpressionAccess().getNameAssignment_0_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Expression__Group_0__1


    // $ANTLR start rule__ArrayAccess__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2491:1: rule__ArrayAccess__Group__0 : ( ( rule__ArrayAccess__TargetAssignment_0 ) ) rule__ArrayAccess__Group__1 ;
    public final void rule__ArrayAccess__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2495:1: ( ( ( rule__ArrayAccess__TargetAssignment_0 ) ) rule__ArrayAccess__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2496:1: ( ( rule__ArrayAccess__TargetAssignment_0 ) ) rule__ArrayAccess__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2496:1: ( ( rule__ArrayAccess__TargetAssignment_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2497:1: ( rule__ArrayAccess__TargetAssignment_0 )
            {
             before(grammarAccess.getArrayAccessAccess().getTargetAssignment_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2498:1: ( rule__ArrayAccess__TargetAssignment_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2498:2: rule__ArrayAccess__TargetAssignment_0
            {
            pushFollow(FOLLOW_rule__ArrayAccess__TargetAssignment_0_in_rule__ArrayAccess__Group__05078);
            rule__ArrayAccess__TargetAssignment_0();
            _fsp--;


            }

             after(grammarAccess.getArrayAccessAccess().getTargetAssignment_0()); 

            }

            pushFollow(FOLLOW_rule__ArrayAccess__Group__1_in_rule__ArrayAccess__Group__05087);
            rule__ArrayAccess__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__ArrayAccess__Group__0


    // $ANTLR start rule__ArrayAccess__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2509:1: rule__ArrayAccess__Group__1 : ( '[' ) rule__ArrayAccess__Group__2 ;
    public final void rule__ArrayAccess__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2513:1: ( ( '[' ) rule__ArrayAccess__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2514:1: ( '[' ) rule__ArrayAccess__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2514:1: ( '[' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2515:1: '['
            {
             before(grammarAccess.getArrayAccessAccess().getLeftSquareBracketKeyword_1()); 
            match(input,75,FOLLOW_75_in_rule__ArrayAccess__Group__15116); 
             after(grammarAccess.getArrayAccessAccess().getLeftSquareBracketKeyword_1()); 

            }

            pushFollow(FOLLOW_rule__ArrayAccess__Group__2_in_rule__ArrayAccess__Group__15126);
            rule__ArrayAccess__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__ArrayAccess__Group__1


    // $ANTLR start rule__ArrayAccess__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2529:1: rule__ArrayAccess__Group__2 : ( ( rule__ArrayAccess__IndexAssignment_2 ) ) rule__ArrayAccess__Group__3 ;
    public final void rule__ArrayAccess__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2533:1: ( ( ( rule__ArrayAccess__IndexAssignment_2 ) ) rule__ArrayAccess__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2534:1: ( ( rule__ArrayAccess__IndexAssignment_2 ) ) rule__ArrayAccess__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2534:1: ( ( rule__ArrayAccess__IndexAssignment_2 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2535:1: ( rule__ArrayAccess__IndexAssignment_2 )
            {
             before(grammarAccess.getArrayAccessAccess().getIndexAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2536:1: ( rule__ArrayAccess__IndexAssignment_2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2536:2: rule__ArrayAccess__IndexAssignment_2
            {
            pushFollow(FOLLOW_rule__ArrayAccess__IndexAssignment_2_in_rule__ArrayAccess__Group__25154);
            rule__ArrayAccess__IndexAssignment_2();
            _fsp--;


            }

             after(grammarAccess.getArrayAccessAccess().getIndexAssignment_2()); 

            }

            pushFollow(FOLLOW_rule__ArrayAccess__Group__3_in_rule__ArrayAccess__Group__25163);
            rule__ArrayAccess__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__ArrayAccess__Group__2


    // $ANTLR start rule__ArrayAccess__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2547:1: rule__ArrayAccess__Group__3 : ( ']' ) ;
    public final void rule__ArrayAccess__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2551:1: ( ( ']' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2552:1: ( ']' )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2552:1: ( ']' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2553:1: ']'
            {
             before(grammarAccess.getArrayAccessAccess().getRightSquareBracketKeyword_3()); 
            match(input,76,FOLLOW_76_in_rule__ArrayAccess__Group__35192); 
             after(grammarAccess.getArrayAccessAccess().getRightSquareBracketKeyword_3()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__ArrayAccess__Group__3


    // $ANTLR start rule__FnCall__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2574:1: rule__FnCall__Group__0 : ( ( rule__FnCall__TargetAssignment_0 ) ) rule__FnCall__Group__1 ;
    public final void rule__FnCall__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2578:1: ( ( ( rule__FnCall__TargetAssignment_0 ) ) rule__FnCall__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2579:1: ( ( rule__FnCall__TargetAssignment_0 ) ) rule__FnCall__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2579:1: ( ( rule__FnCall__TargetAssignment_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2580:1: ( rule__FnCall__TargetAssignment_0 )
            {
             before(grammarAccess.getFnCallAccess().getTargetAssignment_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2581:1: ( rule__FnCall__TargetAssignment_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2581:2: rule__FnCall__TargetAssignment_0
            {
            pushFollow(FOLLOW_rule__FnCall__TargetAssignment_0_in_rule__FnCall__Group__05235);
            rule__FnCall__TargetAssignment_0();
            _fsp--;


            }

             after(grammarAccess.getFnCallAccess().getTargetAssignment_0()); 

            }

            pushFollow(FOLLOW_rule__FnCall__Group__1_in_rule__FnCall__Group__05244);
            rule__FnCall__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__FnCall__Group__0


    // $ANTLR start rule__FnCall__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2592:1: rule__FnCall__Group__1 : ( '(' ) rule__FnCall__Group__2 ;
    public final void rule__FnCall__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2596:1: ( ( '(' ) rule__FnCall__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2597:1: ( '(' ) rule__FnCall__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2597:1: ( '(' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2598:1: '('
            {
             before(grammarAccess.getFnCallAccess().getLeftParenthesisKeyword_1()); 
            match(input,77,FOLLOW_77_in_rule__FnCall__Group__15273); 
             after(grammarAccess.getFnCallAccess().getLeftParenthesisKeyword_1()); 

            }

            pushFollow(FOLLOW_rule__FnCall__Group__2_in_rule__FnCall__Group__15283);
            rule__FnCall__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__FnCall__Group__1


    // $ANTLR start rule__FnCall__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2612:1: rule__FnCall__Group__2 : ( ( rule__FnCall__ArgsAssignment_2 ) ) rule__FnCall__Group__3 ;
    public final void rule__FnCall__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2616:1: ( ( ( rule__FnCall__ArgsAssignment_2 ) ) rule__FnCall__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2617:1: ( ( rule__FnCall__ArgsAssignment_2 ) ) rule__FnCall__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2617:1: ( ( rule__FnCall__ArgsAssignment_2 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2618:1: ( rule__FnCall__ArgsAssignment_2 )
            {
             before(grammarAccess.getFnCallAccess().getArgsAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2619:1: ( rule__FnCall__ArgsAssignment_2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2619:2: rule__FnCall__ArgsAssignment_2
            {
            pushFollow(FOLLOW_rule__FnCall__ArgsAssignment_2_in_rule__FnCall__Group__25311);
            rule__FnCall__ArgsAssignment_2();
            _fsp--;


            }

             after(grammarAccess.getFnCallAccess().getArgsAssignment_2()); 

            }

            pushFollow(FOLLOW_rule__FnCall__Group__3_in_rule__FnCall__Group__25320);
            rule__FnCall__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__FnCall__Group__2


    // $ANTLR start rule__FnCall__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2630:1: rule__FnCall__Group__3 : ( ')' ) ;
    public final void rule__FnCall__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2634:1: ( ( ')' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2635:1: ( ')' )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2635:1: ( ')' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2636:1: ')'
            {
             before(grammarAccess.getFnCallAccess().getRightParenthesisKeyword_3()); 
            match(input,78,FOLLOW_78_in_rule__FnCall__Group__35349); 
             after(grammarAccess.getFnCallAccess().getRightParenthesisKeyword_3()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__FnCall__Group__3


    // $ANTLR start rule__Message__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2657:1: rule__Message__Group__0 : ( ( rule__Message__TargetAssignment_0 ) ) rule__Message__Group__1 ;
    public final void rule__Message__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2661:1: ( ( ( rule__Message__TargetAssignment_0 ) ) rule__Message__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2662:1: ( ( rule__Message__TargetAssignment_0 ) ) rule__Message__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2662:1: ( ( rule__Message__TargetAssignment_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2663:1: ( rule__Message__TargetAssignment_0 )
            {
             before(grammarAccess.getMessageAccess().getTargetAssignment_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2664:1: ( rule__Message__TargetAssignment_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2664:2: rule__Message__TargetAssignment_0
            {
            pushFollow(FOLLOW_rule__Message__TargetAssignment_0_in_rule__Message__Group__05392);
            rule__Message__TargetAssignment_0();
            _fsp--;


            }

             after(grammarAccess.getMessageAccess().getTargetAssignment_0()); 

            }

            pushFollow(FOLLOW_rule__Message__Group__1_in_rule__Message__Group__05401);
            rule__Message__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group__0


    // $ANTLR start rule__Message__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2675:1: rule__Message__Group__1 : ( '~' ) rule__Message__Group__2 ;
    public final void rule__Message__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2679:1: ( ( '~' ) rule__Message__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2680:1: ( '~' ) rule__Message__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2680:1: ( '~' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2681:1: '~'
            {
             before(grammarAccess.getMessageAccess().getTildeKeyword_1()); 
            match(input,79,FOLLOW_79_in_rule__Message__Group__15430); 
             after(grammarAccess.getMessageAccess().getTildeKeyword_1()); 

            }

            pushFollow(FOLLOW_rule__Message__Group__2_in_rule__Message__Group__15440);
            rule__Message__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group__1


    // $ANTLR start rule__Message__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2695:1: rule__Message__Group__2 : ( ( rule__Message__DiscardAssignment_2 )? ) rule__Message__Group__3 ;
    public final void rule__Message__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2699:1: ( ( ( rule__Message__DiscardAssignment_2 )? ) rule__Message__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2700:1: ( ( rule__Message__DiscardAssignment_2 )? ) rule__Message__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2700:1: ( ( rule__Message__DiscardAssignment_2 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2701:1: ( rule__Message__DiscardAssignment_2 )?
            {
             before(grammarAccess.getMessageAccess().getDiscardAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2702:1: ( rule__Message__DiscardAssignment_2 )?
            int alt30=2;
            int LA30_0 = input.LA(1);

            if ( (LA30_0==79) ) {
                alt30=1;
            }
            switch (alt30) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2702:2: rule__Message__DiscardAssignment_2
                    {
                    pushFollow(FOLLOW_rule__Message__DiscardAssignment_2_in_rule__Message__Group__25468);
                    rule__Message__DiscardAssignment_2();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getMessageAccess().getDiscardAssignment_2()); 

            }

            pushFollow(FOLLOW_rule__Message__Group__3_in_rule__Message__Group__25478);
            rule__Message__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group__2


    // $ANTLR start rule__Message__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2713:1: rule__Message__Group__3 : ( ( rule__Message__MessageAssignment_3 ) ) rule__Message__Group__4 ;
    public final void rule__Message__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2717:1: ( ( ( rule__Message__MessageAssignment_3 ) ) rule__Message__Group__4 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2718:1: ( ( rule__Message__MessageAssignment_3 ) ) rule__Message__Group__4
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2718:1: ( ( rule__Message__MessageAssignment_3 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2719:1: ( rule__Message__MessageAssignment_3 )
            {
             before(grammarAccess.getMessageAccess().getMessageAssignment_3()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2720:1: ( rule__Message__MessageAssignment_3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2720:2: rule__Message__MessageAssignment_3
            {
            pushFollow(FOLLOW_rule__Message__MessageAssignment_3_in_rule__Message__Group__35506);
            rule__Message__MessageAssignment_3();
            _fsp--;


            }

             after(grammarAccess.getMessageAccess().getMessageAssignment_3()); 

            }

            pushFollow(FOLLOW_rule__Message__Group__4_in_rule__Message__Group__35515);
            rule__Message__Group__4();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group__3


    // $ANTLR start rule__Message__Group__4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2731:1: rule__Message__Group__4 : ( ( rule__Message__Group_4__0 )? ) rule__Message__Group__5 ;
    public final void rule__Message__Group__4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2735:1: ( ( ( rule__Message__Group_4__0 )? ) rule__Message__Group__5 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2736:1: ( ( rule__Message__Group_4__0 )? ) rule__Message__Group__5
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2736:1: ( ( rule__Message__Group_4__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2737:1: ( rule__Message__Group_4__0 )?
            {
             before(grammarAccess.getMessageAccess().getGroup_4()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2738:1: ( rule__Message__Group_4__0 )?
            int alt31=2;
            int LA31_0 = input.LA(1);

            if ( (LA31_0==80) ) {
                alt31=1;
            }
            switch (alt31) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2738:2: rule__Message__Group_4__0
                    {
                    pushFollow(FOLLOW_rule__Message__Group_4__0_in_rule__Message__Group__45543);
                    rule__Message__Group_4__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getMessageAccess().getGroup_4()); 

            }

            pushFollow(FOLLOW_rule__Message__Group__5_in_rule__Message__Group__45553);
            rule__Message__Group__5();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group__4


    // $ANTLR start rule__Message__Group__5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2749:1: rule__Message__Group__5 : ( ( rule__Message__Alternatives_5 )? ) ;
    public final void rule__Message__Group__5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2753:1: ( ( ( rule__Message__Alternatives_5 )? ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2754:1: ( ( rule__Message__Alternatives_5 )? )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2754:1: ( ( rule__Message__Alternatives_5 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2755:1: ( rule__Message__Alternatives_5 )?
            {
             before(grammarAccess.getMessageAccess().getAlternatives_5()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2756:1: ( rule__Message__Alternatives_5 )?
            int alt32=2;
            int LA32_0 = input.LA(1);

            if ( (LA32_0==62||LA32_0==77) ) {
                alt32=1;
            }
            switch (alt32) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2756:2: rule__Message__Alternatives_5
                    {
                    pushFollow(FOLLOW_rule__Message__Alternatives_5_in_rule__Message__Group__55581);
                    rule__Message__Alternatives_5();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getMessageAccess().getAlternatives_5()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group__5


    // $ANTLR start rule__Message__Group_4__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2778:1: rule__Message__Group_4__0 : ( ':' ) rule__Message__Group_4__1 ;
    public final void rule__Message__Group_4__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2782:1: ( ( ':' ) rule__Message__Group_4__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2783:1: ( ':' ) rule__Message__Group_4__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2783:1: ( ':' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2784:1: ':'
            {
             before(grammarAccess.getMessageAccess().getColonKeyword_4_0()); 
            match(input,80,FOLLOW_80_in_rule__Message__Group_4__05629); 
             after(grammarAccess.getMessageAccess().getColonKeyword_4_0()); 

            }

            pushFollow(FOLLOW_rule__Message__Group_4__1_in_rule__Message__Group_4__05639);
            rule__Message__Group_4__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group_4__0


    // $ANTLR start rule__Message__Group_4__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2798:1: rule__Message__Group_4__1 : ( ( rule__Message__ScopeAssignment_4_1 ) ) ;
    public final void rule__Message__Group_4__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2802:1: ( ( ( rule__Message__ScopeAssignment_4_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2803:1: ( ( rule__Message__ScopeAssignment_4_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2803:1: ( ( rule__Message__ScopeAssignment_4_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2804:1: ( rule__Message__ScopeAssignment_4_1 )
            {
             before(grammarAccess.getMessageAccess().getScopeAssignment_4_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2805:1: ( rule__Message__ScopeAssignment_4_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2805:2: rule__Message__ScopeAssignment_4_1
            {
            pushFollow(FOLLOW_rule__Message__ScopeAssignment_4_1_in_rule__Message__Group_4__15667);
            rule__Message__ScopeAssignment_4_1();
            _fsp--;


            }

             after(grammarAccess.getMessageAccess().getScopeAssignment_4_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group_4__1


    // $ANTLR start rule__Message__Group_5_0__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2819:1: rule__Message__Group_5_0__0 : ( '(' ) rule__Message__Group_5_0__1 ;
    public final void rule__Message__Group_5_0__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2823:1: ( ( '(' ) rule__Message__Group_5_0__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2824:1: ( '(' ) rule__Message__Group_5_0__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2824:1: ( '(' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2825:1: '('
            {
             before(grammarAccess.getMessageAccess().getLeftParenthesisKeyword_5_0_0()); 
            match(input,77,FOLLOW_77_in_rule__Message__Group_5_0__05706); 
             after(grammarAccess.getMessageAccess().getLeftParenthesisKeyword_5_0_0()); 

            }

            pushFollow(FOLLOW_rule__Message__Group_5_0__1_in_rule__Message__Group_5_0__05716);
            rule__Message__Group_5_0__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group_5_0__0


    // $ANTLR start rule__Message__Group_5_0__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2839:1: rule__Message__Group_5_0__1 : ( ( rule__Message__ArgsAssignment_5_0_1 ) ) rule__Message__Group_5_0__2 ;
    public final void rule__Message__Group_5_0__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2843:1: ( ( ( rule__Message__ArgsAssignment_5_0_1 ) ) rule__Message__Group_5_0__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2844:1: ( ( rule__Message__ArgsAssignment_5_0_1 ) ) rule__Message__Group_5_0__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2844:1: ( ( rule__Message__ArgsAssignment_5_0_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2845:1: ( rule__Message__ArgsAssignment_5_0_1 )
            {
             before(grammarAccess.getMessageAccess().getArgsAssignment_5_0_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2846:1: ( rule__Message__ArgsAssignment_5_0_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2846:2: rule__Message__ArgsAssignment_5_0_1
            {
            pushFollow(FOLLOW_rule__Message__ArgsAssignment_5_0_1_in_rule__Message__Group_5_0__15744);
            rule__Message__ArgsAssignment_5_0_1();
            _fsp--;


            }

             after(grammarAccess.getMessageAccess().getArgsAssignment_5_0_1()); 

            }

            pushFollow(FOLLOW_rule__Message__Group_5_0__2_in_rule__Message__Group_5_0__15753);
            rule__Message__Group_5_0__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group_5_0__1


    // $ANTLR start rule__Message__Group_5_0__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2857:1: rule__Message__Group_5_0__2 : ( ')' ) ;
    public final void rule__Message__Group_5_0__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2861:1: ( ( ')' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2862:1: ( ')' )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2862:1: ( ')' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2863:1: ')'
            {
             before(grammarAccess.getMessageAccess().getRightParenthesisKeyword_5_0_2()); 
            match(input,78,FOLLOW_78_in_rule__Message__Group_5_0__25782); 
             after(grammarAccess.getMessageAccess().getRightParenthesisKeyword_5_0_2()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group_5_0__2


    // $ANTLR start rule__Message__Group_5_1__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2882:1: rule__Message__Group_5_1__0 : ( '=' ) rule__Message__Group_5_1__1 ;
    public final void rule__Message__Group_5_1__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2886:1: ( ( '=' ) rule__Message__Group_5_1__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2887:1: ( '=' ) rule__Message__Group_5_1__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2887:1: ( '=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2888:1: '='
            {
             before(grammarAccess.getMessageAccess().getEqualsSignKeyword_5_1_0()); 
            match(input,62,FOLLOW_62_in_rule__Message__Group_5_1__05824); 
             after(grammarAccess.getMessageAccess().getEqualsSignKeyword_5_1_0()); 

            }

            pushFollow(FOLLOW_rule__Message__Group_5_1__1_in_rule__Message__Group_5_1__05834);
            rule__Message__Group_5_1__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group_5_1__0


    // $ANTLR start rule__Message__Group_5_1__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2902:1: rule__Message__Group_5_1__1 : ( ( rule__Message__ValueAssignment_5_1_1 ) ) ;
    public final void rule__Message__Group_5_1__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2906:1: ( ( ( rule__Message__ValueAssignment_5_1_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2907:1: ( ( rule__Message__ValueAssignment_5_1_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2907:1: ( ( rule__Message__ValueAssignment_5_1_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2908:1: ( rule__Message__ValueAssignment_5_1_1 )
            {
             before(grammarAccess.getMessageAccess().getValueAssignment_5_1_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2909:1: ( rule__Message__ValueAssignment_5_1_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2909:2: rule__Message__ValueAssignment_5_1_1
            {
            pushFollow(FOLLOW_rule__Message__ValueAssignment_5_1_1_in_rule__Message__Group_5_1__15862);
            rule__Message__ValueAssignment_5_1_1();
            _fsp--;


            }

             after(grammarAccess.getMessageAccess().getValueAssignment_5_1_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__Group_5_1__1


    // $ANTLR start rule__Negate__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2923:1: rule__Negate__Group__0 : ( '\\\\' ) rule__Negate__Group__1 ;
    public final void rule__Negate__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2927:1: ( ( '\\\\' ) rule__Negate__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2928:1: ( '\\\\' ) rule__Negate__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2928:1: ( '\\\\' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2929:1: '\\\\'
            {
             before(grammarAccess.getNegateAccess().getReverseSolidusKeyword_0()); 
            match(input,81,FOLLOW_81_in_rule__Negate__Group__05901); 
             after(grammarAccess.getNegateAccess().getReverseSolidusKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Negate__Group__1_in_rule__Negate__Group__05911);
            rule__Negate__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Negate__Group__0


    // $ANTLR start rule__Negate__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2943:1: rule__Negate__Group__1 : ( ( rule__Negate__ExpressionAssignment_1 ) ) ;
    public final void rule__Negate__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2947:1: ( ( ( rule__Negate__ExpressionAssignment_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2948:1: ( ( rule__Negate__ExpressionAssignment_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2948:1: ( ( rule__Negate__ExpressionAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2949:1: ( rule__Negate__ExpressionAssignment_1 )
            {
             before(grammarAccess.getNegateAccess().getExpressionAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2950:1: ( rule__Negate__ExpressionAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2950:2: rule__Negate__ExpressionAssignment_1
            {
            pushFollow(FOLLOW_rule__Negate__ExpressionAssignment_1_in_rule__Negate__Group__15939);
            rule__Negate__ExpressionAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getNegateAccess().getExpressionAssignment_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Negate__Group__1


    // $ANTLR start rule__Const__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2964:1: rule__Const__Group__0 : ( () ) rule__Const__Group__1 ;
    public final void rule__Const__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2968:1: ( ( () ) rule__Const__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2969:1: ( () ) rule__Const__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2969:1: ( () )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2970:1: ()
            {
             before(grammarAccess.getConstAccess().getConstAction_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2971:1: ()
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2973:1: 
            {
            }

             after(grammarAccess.getConstAccess().getConstAction_0()); 

            }

            pushFollow(FOLLOW_rule__Const__Group__1_in_rule__Const__Group__05987);
            rule__Const__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Const__Group__0


    // $ANTLR start rule__Const__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2984:1: rule__Const__Group__1 : ( ( rule__Const__ValueAssignment_1 ) ) ;
    public final void rule__Const__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2988:1: ( ( ( rule__Const__ValueAssignment_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2989:1: ( ( rule__Const__ValueAssignment_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2989:1: ( ( rule__Const__ValueAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2990:1: ( rule__Const__ValueAssignment_1 )
            {
             before(grammarAccess.getConstAccess().getValueAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2991:1: ( rule__Const__ValueAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:2991:2: rule__Const__ValueAssignment_1
            {
            pushFollow(FOLLOW_rule__Const__ValueAssignment_1_in_rule__Const__Group__16015);
            rule__Const__ValueAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getConstAccess().getValueAssignment_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Const__Group__1


    // $ANTLR start rule__Class__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3005:1: rule__Class__Group__0 : ( '::class' ) rule__Class__Group__1 ;
    public final void rule__Class__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3009:1: ( ( '::class' ) rule__Class__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3010:1: ( '::class' ) rule__Class__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3010:1: ( '::class' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3011:1: '::class'
            {
             before(grammarAccess.getClassAccess().getClassKeyword_0()); 
            match(input,82,FOLLOW_82_in_rule__Class__Group__06054); 
             after(grammarAccess.getClassAccess().getClassKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Class__Group__1_in_rule__Class__Group__06064);
            rule__Class__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group__0


    // $ANTLR start rule__Class__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3025:1: rule__Class__Group__1 : ( ( rule__Class__NameAssignment_1 ) ) rule__Class__Group__2 ;
    public final void rule__Class__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3029:1: ( ( ( rule__Class__NameAssignment_1 ) ) rule__Class__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3030:1: ( ( rule__Class__NameAssignment_1 ) ) rule__Class__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3030:1: ( ( rule__Class__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3031:1: ( rule__Class__NameAssignment_1 )
            {
             before(grammarAccess.getClassAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3032:1: ( rule__Class__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3032:2: rule__Class__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__Class__NameAssignment_1_in_rule__Class__Group__16092);
            rule__Class__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getClassAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__Class__Group__2_in_rule__Class__Group__16101);
            rule__Class__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group__1


    // $ANTLR start rule__Class__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3043:1: rule__Class__Group__2 : ( ( rule__Class__Group_2__0 )? ) rule__Class__Group__3 ;
    public final void rule__Class__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3047:1: ( ( ( rule__Class__Group_2__0 )? ) rule__Class__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3048:1: ( ( rule__Class__Group_2__0 )? ) rule__Class__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3048:1: ( ( rule__Class__Group_2__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3049:1: ( rule__Class__Group_2__0 )?
            {
             before(grammarAccess.getClassAccess().getGroup_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3050:1: ( rule__Class__Group_2__0 )?
            int alt33=2;
            int LA33_0 = input.LA(1);

            if ( (LA33_0==83) ) {
                alt33=1;
            }
            switch (alt33) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3050:2: rule__Class__Group_2__0
                    {
                    pushFollow(FOLLOW_rule__Class__Group_2__0_in_rule__Class__Group__26129);
                    rule__Class__Group_2__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getClassAccess().getGroup_2()); 

            }

            pushFollow(FOLLOW_rule__Class__Group__3_in_rule__Class__Group__26139);
            rule__Class__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group__2


    // $ANTLR start rule__Class__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3061:1: rule__Class__Group__3 : ( ( rule__Class__Alternatives_3 )? ) rule__Class__Group__4 ;
    public final void rule__Class__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3065:1: ( ( ( rule__Class__Alternatives_3 )? ) rule__Class__Group__4 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3066:1: ( ( rule__Class__Alternatives_3 )? ) rule__Class__Group__4
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3066:1: ( ( rule__Class__Alternatives_3 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3067:1: ( rule__Class__Alternatives_3 )?
            {
             before(grammarAccess.getClassAccess().getAlternatives_3()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3068:1: ( rule__Class__Alternatives_3 )?
            int alt34=2;
            int LA34_0 = input.LA(1);

            if ( ((LA34_0>=84 && LA34_0<=85)) ) {
                alt34=1;
            }
            switch (alt34) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3068:2: rule__Class__Alternatives_3
                    {
                    pushFollow(FOLLOW_rule__Class__Alternatives_3_in_rule__Class__Group__36167);
                    rule__Class__Alternatives_3();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getClassAccess().getAlternatives_3()); 

            }

            pushFollow(FOLLOW_rule__Class__Group__4_in_rule__Class__Group__36177);
            rule__Class__Group__4();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group__3


    // $ANTLR start rule__Class__Group__4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3079:1: rule__Class__Group__4 : ( ( rule__Class__VisibilityAssignment_4 )? ) rule__Class__Group__5 ;
    public final void rule__Class__Group__4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3083:1: ( ( ( rule__Class__VisibilityAssignment_4 )? ) rule__Class__Group__5 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3084:1: ( ( rule__Class__VisibilityAssignment_4 )? ) rule__Class__Group__5
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3084:1: ( ( rule__Class__VisibilityAssignment_4 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3085:1: ( rule__Class__VisibilityAssignment_4 )?
            {
             before(grammarAccess.getClassAccess().getVisibilityAssignment_4()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3086:1: ( rule__Class__VisibilityAssignment_4 )?
            int alt35=2;
            int LA35_0 = input.LA(1);

            if ( ((LA35_0>=60 && LA35_0<=61)) ) {
                alt35=1;
            }
            switch (alt35) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3086:2: rule__Class__VisibilityAssignment_4
                    {
                    pushFollow(FOLLOW_rule__Class__VisibilityAssignment_4_in_rule__Class__Group__46205);
                    rule__Class__VisibilityAssignment_4();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getClassAccess().getVisibilityAssignment_4()); 

            }

            pushFollow(FOLLOW_rule__Class__Group__5_in_rule__Class__Group__46215);
            rule__Class__Group__5();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group__4


    // $ANTLR start rule__Class__Group__5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3097:1: rule__Class__Group__5 : ( ( rule__Class__Group_5__0 )? ) rule__Class__Group__6 ;
    public final void rule__Class__Group__5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3101:1: ( ( ( rule__Class__Group_5__0 )? ) rule__Class__Group__6 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3102:1: ( ( rule__Class__Group_5__0 )? ) rule__Class__Group__6
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3102:1: ( ( rule__Class__Group_5__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3103:1: ( rule__Class__Group_5__0 )?
            {
             before(grammarAccess.getClassAccess().getGroup_5()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3104:1: ( rule__Class__Group_5__0 )?
            int alt36=2;
            int LA36_0 = input.LA(1);

            if ( (LA36_0==86) ) {
                alt36=1;
            }
            switch (alt36) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3104:2: rule__Class__Group_5__0
                    {
                    pushFollow(FOLLOW_rule__Class__Group_5__0_in_rule__Class__Group__56243);
                    rule__Class__Group_5__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getClassAccess().getGroup_5()); 

            }

            pushFollow(FOLLOW_rule__Class__Group__6_in_rule__Class__Group__56253);
            rule__Class__Group__6();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group__5


    // $ANTLR start rule__Class__Group__6
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3115:1: rule__Class__Group__6 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Class__Group__7 ;
    public final void rule__Class__Group__6() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3119:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Class__Group__7 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3120:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Class__Group__7
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3120:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3121:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3121:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3122:1: ( RULE_TERM )
            {
             before(grammarAccess.getClassAccess().getTERMTerminalRuleCall_6()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3123:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3123:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Class__Group__66284); 

            }

             after(grammarAccess.getClassAccess().getTERMTerminalRuleCall_6()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3126:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3127:1: ( RULE_TERM )*
            {
             before(grammarAccess.getClassAccess().getTERMTerminalRuleCall_6()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3128:1: ( RULE_TERM )*
            loop37:
            do {
                int alt37=2;
                int LA37_0 = input.LA(1);

                if ( (LA37_0==RULE_TERM) ) {
                    alt37=1;
                }


                switch (alt37) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3128:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Class__Group__66297); 

            	    }
            	    break;

            	default :
            	    break loop37;
                }
            } while (true);

             after(grammarAccess.getClassAccess().getTERMTerminalRuleCall_6()); 

            }


            }

            pushFollow(FOLLOW_rule__Class__Group__7_in_rule__Class__Group__66309);
            rule__Class__Group__7();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group__6


    // $ANTLR start rule__Class__Group__7
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3140:1: rule__Class__Group__7 : ( ( rule__Class__ContentAssignment_7 )* ) ;
    public final void rule__Class__Group__7() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3144:1: ( ( ( rule__Class__ContentAssignment_7 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3145:1: ( ( rule__Class__ContentAssignment_7 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3145:1: ( ( rule__Class__ContentAssignment_7 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3146:1: ( rule__Class__ContentAssignment_7 )*
            {
             before(grammarAccess.getClassAccess().getContentAssignment_7()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3147:1: ( rule__Class__ContentAssignment_7 )*
            loop38:
            do {
                int alt38=2;
                int LA38_0 = input.LA(1);

                if ( (LA38_0==87||LA38_0==89||LA38_0==92) ) {
                    alt38=1;
                }


                switch (alt38) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3147:2: rule__Class__ContentAssignment_7
            	    {
            	    pushFollow(FOLLOW_rule__Class__ContentAssignment_7_in_rule__Class__Group__76337);
            	    rule__Class__ContentAssignment_7();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop38;
                }
            } while (true);

             after(grammarAccess.getClassAccess().getContentAssignment_7()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group__7


    // $ANTLR start rule__Class__Group_2__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3173:1: rule__Class__Group_2__0 : ( 'metaclass' ) rule__Class__Group_2__1 ;
    public final void rule__Class__Group_2__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3177:1: ( ( 'metaclass' ) rule__Class__Group_2__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3178:1: ( 'metaclass' ) rule__Class__Group_2__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3178:1: ( 'metaclass' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3179:1: 'metaclass'
            {
             before(grammarAccess.getClassAccess().getMetaclassKeyword_2_0()); 
            match(input,83,FOLLOW_83_in_rule__Class__Group_2__06389); 
             after(grammarAccess.getClassAccess().getMetaclassKeyword_2_0()); 

            }

            pushFollow(FOLLOW_rule__Class__Group_2__1_in_rule__Class__Group_2__06399);
            rule__Class__Group_2__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group_2__0


    // $ANTLR start rule__Class__Group_2__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3193:1: rule__Class__Group_2__1 : ( ( rule__Class__MetaAssignment_2_1 ) ) ;
    public final void rule__Class__Group_2__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3197:1: ( ( ( rule__Class__MetaAssignment_2_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3198:1: ( ( rule__Class__MetaAssignment_2_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3198:1: ( ( rule__Class__MetaAssignment_2_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3199:1: ( rule__Class__MetaAssignment_2_1 )
            {
             before(grammarAccess.getClassAccess().getMetaAssignment_2_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3200:1: ( rule__Class__MetaAssignment_2_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3200:2: rule__Class__MetaAssignment_2_1
            {
            pushFollow(FOLLOW_rule__Class__MetaAssignment_2_1_in_rule__Class__Group_2__16427);
            rule__Class__MetaAssignment_2_1();
            _fsp--;


            }

             after(grammarAccess.getClassAccess().getMetaAssignment_2_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group_2__1


    // $ANTLR start rule__Class__Group_3_0__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3214:1: rule__Class__Group_3_0__0 : ( 'subclass' ) rule__Class__Group_3_0__1 ;
    public final void rule__Class__Group_3_0__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3218:1: ( ( 'subclass' ) rule__Class__Group_3_0__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3219:1: ( 'subclass' ) rule__Class__Group_3_0__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3219:1: ( 'subclass' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3220:1: 'subclass'
            {
             before(grammarAccess.getClassAccess().getSubclassKeyword_3_0_0()); 
            match(input,84,FOLLOW_84_in_rule__Class__Group_3_0__06466); 
             after(grammarAccess.getClassAccess().getSubclassKeyword_3_0_0()); 

            }

            pushFollow(FOLLOW_rule__Class__Group_3_0__1_in_rule__Class__Group_3_0__06476);
            rule__Class__Group_3_0__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group_3_0__0


    // $ANTLR start rule__Class__Group_3_0__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3234:1: rule__Class__Group_3_0__1 : ( ( rule__Class__SubclassAssignment_3_0_1 ) ) ;
    public final void rule__Class__Group_3_0__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3238:1: ( ( ( rule__Class__SubclassAssignment_3_0_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3239:1: ( ( rule__Class__SubclassAssignment_3_0_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3239:1: ( ( rule__Class__SubclassAssignment_3_0_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3240:1: ( rule__Class__SubclassAssignment_3_0_1 )
            {
             before(grammarAccess.getClassAccess().getSubclassAssignment_3_0_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3241:1: ( rule__Class__SubclassAssignment_3_0_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3241:2: rule__Class__SubclassAssignment_3_0_1
            {
            pushFollow(FOLLOW_rule__Class__SubclassAssignment_3_0_1_in_rule__Class__Group_3_0__16504);
            rule__Class__SubclassAssignment_3_0_1();
            _fsp--;


            }

             after(grammarAccess.getClassAccess().getSubclassAssignment_3_0_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group_3_0__1


    // $ANTLR start rule__Class__Group_3_1__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3255:1: rule__Class__Group_3_1__0 : ( 'mixinclass' ) rule__Class__Group_3_1__1 ;
    public final void rule__Class__Group_3_1__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3259:1: ( ( 'mixinclass' ) rule__Class__Group_3_1__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3260:1: ( 'mixinclass' ) rule__Class__Group_3_1__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3260:1: ( 'mixinclass' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3261:1: 'mixinclass'
            {
             before(grammarAccess.getClassAccess().getMixinclassKeyword_3_1_0()); 
            match(input,85,FOLLOW_85_in_rule__Class__Group_3_1__06543); 
             after(grammarAccess.getClassAccess().getMixinclassKeyword_3_1_0()); 

            }

            pushFollow(FOLLOW_rule__Class__Group_3_1__1_in_rule__Class__Group_3_1__06553);
            rule__Class__Group_3_1__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group_3_1__0


    // $ANTLR start rule__Class__Group_3_1__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3275:1: rule__Class__Group_3_1__1 : ( ( rule__Class__MixinAssignment_3_1_1 ) ) ;
    public final void rule__Class__Group_3_1__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3279:1: ( ( ( rule__Class__MixinAssignment_3_1_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3280:1: ( ( rule__Class__MixinAssignment_3_1_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3280:1: ( ( rule__Class__MixinAssignment_3_1_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3281:1: ( rule__Class__MixinAssignment_3_1_1 )
            {
             before(grammarAccess.getClassAccess().getMixinAssignment_3_1_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3282:1: ( rule__Class__MixinAssignment_3_1_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3282:2: rule__Class__MixinAssignment_3_1_1
            {
            pushFollow(FOLLOW_rule__Class__MixinAssignment_3_1_1_in_rule__Class__Group_3_1__16581);
            rule__Class__MixinAssignment_3_1_1();
            _fsp--;


            }

             after(grammarAccess.getClassAccess().getMixinAssignment_3_1_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group_3_1__1


    // $ANTLR start rule__Class__Group_5__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3296:1: rule__Class__Group_5__0 : ( 'inherit' ) rule__Class__Group_5__1 ;
    public final void rule__Class__Group_5__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3300:1: ( ( 'inherit' ) rule__Class__Group_5__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3301:1: ( 'inherit' ) rule__Class__Group_5__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3301:1: ( 'inherit' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3302:1: 'inherit'
            {
             before(grammarAccess.getClassAccess().getInheritKeyword_5_0()); 
            match(input,86,FOLLOW_86_in_rule__Class__Group_5__06620); 
             after(grammarAccess.getClassAccess().getInheritKeyword_5_0()); 

            }

            pushFollow(FOLLOW_rule__Class__Group_5__1_in_rule__Class__Group_5__06630);
            rule__Class__Group_5__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group_5__0


    // $ANTLR start rule__Class__Group_5__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3316:1: rule__Class__Group_5__1 : ( ( ( rule__Class__InheritAssignment_5_1 ) ) ( ( rule__Class__InheritAssignment_5_1 )* ) ) ;
    public final void rule__Class__Group_5__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3320:1: ( ( ( ( rule__Class__InheritAssignment_5_1 ) ) ( ( rule__Class__InheritAssignment_5_1 )* ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3321:1: ( ( ( rule__Class__InheritAssignment_5_1 ) ) ( ( rule__Class__InheritAssignment_5_1 )* ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3321:1: ( ( ( rule__Class__InheritAssignment_5_1 ) ) ( ( rule__Class__InheritAssignment_5_1 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3322:1: ( ( rule__Class__InheritAssignment_5_1 ) ) ( ( rule__Class__InheritAssignment_5_1 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3322:1: ( ( rule__Class__InheritAssignment_5_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3323:1: ( rule__Class__InheritAssignment_5_1 )
            {
             before(grammarAccess.getClassAccess().getInheritAssignment_5_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3324:1: ( rule__Class__InheritAssignment_5_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3324:2: rule__Class__InheritAssignment_5_1
            {
            pushFollow(FOLLOW_rule__Class__InheritAssignment_5_1_in_rule__Class__Group_5__16660);
            rule__Class__InheritAssignment_5_1();
            _fsp--;


            }

             after(grammarAccess.getClassAccess().getInheritAssignment_5_1()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3327:1: ( ( rule__Class__InheritAssignment_5_1 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3328:1: ( rule__Class__InheritAssignment_5_1 )*
            {
             before(grammarAccess.getClassAccess().getInheritAssignment_5_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3329:1: ( rule__Class__InheritAssignment_5_1 )*
            loop39:
            do {
                int alt39=2;
                int LA39_0 = input.LA(1);

                if ( ((LA39_0>=RULE_STRING && LA39_0<=RULE_SYMBOL_VAR)) ) {
                    alt39=1;
                }


                switch (alt39) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3329:2: rule__Class__InheritAssignment_5_1
            	    {
            	    pushFollow(FOLLOW_rule__Class__InheritAssignment_5_1_in_rule__Class__Group_5__16672);
            	    rule__Class__InheritAssignment_5_1();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop39;
                }
            } while (true);

             after(grammarAccess.getClassAccess().getInheritAssignment_5_1()); 

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__Group_5__1


    // $ANTLR start rule__Method__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3344:1: rule__Method__Group__0 : ( '::method' ) rule__Method__Group__1 ;
    public final void rule__Method__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3348:1: ( ( '::method' ) rule__Method__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3349:1: ( '::method' ) rule__Method__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3349:1: ( '::method' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3350:1: '::method'
            {
             before(grammarAccess.getMethodAccess().getMethodKeyword_0()); 
            match(input,87,FOLLOW_87_in_rule__Method__Group__06714); 
             after(grammarAccess.getMethodAccess().getMethodKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Method__Group__1_in_rule__Method__Group__06724);
            rule__Method__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group__0


    // $ANTLR start rule__Method__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3364:1: rule__Method__Group__1 : ( ( rule__Method__NameAssignment_1 ) ) rule__Method__Group__2 ;
    public final void rule__Method__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3368:1: ( ( ( rule__Method__NameAssignment_1 ) ) rule__Method__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3369:1: ( ( rule__Method__NameAssignment_1 ) ) rule__Method__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3369:1: ( ( rule__Method__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3370:1: ( rule__Method__NameAssignment_1 )
            {
             before(grammarAccess.getMethodAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3371:1: ( rule__Method__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3371:2: rule__Method__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__Method__NameAssignment_1_in_rule__Method__Group__16752);
            rule__Method__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getMethodAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__Method__Group__2_in_rule__Method__Group__16761);
            rule__Method__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group__1


    // $ANTLR start rule__Method__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3382:1: rule__Method__Group__2 : ( ( rule__Method__VisibilityAssignment_2 )? ) rule__Method__Group__3 ;
    public final void rule__Method__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3386:1: ( ( ( rule__Method__VisibilityAssignment_2 )? ) rule__Method__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3387:1: ( ( rule__Method__VisibilityAssignment_2 )? ) rule__Method__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3387:1: ( ( rule__Method__VisibilityAssignment_2 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3388:1: ( rule__Method__VisibilityAssignment_2 )?
            {
             before(grammarAccess.getMethodAccess().getVisibilityAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3389:1: ( rule__Method__VisibilityAssignment_2 )?
            int alt40=2;
            int LA40_0 = input.LA(1);

            if ( ((LA40_0>=60 && LA40_0<=61)) ) {
                alt40=1;
            }
            switch (alt40) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3389:2: rule__Method__VisibilityAssignment_2
                    {
                    pushFollow(FOLLOW_rule__Method__VisibilityAssignment_2_in_rule__Method__Group__26789);
                    rule__Method__VisibilityAssignment_2();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getMethodAccess().getVisibilityAssignment_2()); 

            }

            pushFollow(FOLLOW_rule__Method__Group__3_in_rule__Method__Group__26799);
            rule__Method__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group__2


    // $ANTLR start rule__Method__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3400:1: rule__Method__Group__3 : ( ( rule__Method__ClassAssignment_3 )? ) rule__Method__Group__4 ;
    public final void rule__Method__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3404:1: ( ( ( rule__Method__ClassAssignment_3 )? ) rule__Method__Group__4 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3405:1: ( ( rule__Method__ClassAssignment_3 )? ) rule__Method__Group__4
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3405:1: ( ( rule__Method__ClassAssignment_3 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3406:1: ( rule__Method__ClassAssignment_3 )?
            {
             before(grammarAccess.getMethodAccess().getClassAssignment_3()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3407:1: ( rule__Method__ClassAssignment_3 )?
            int alt41=2;
            int LA41_0 = input.LA(1);

            if ( (LA41_0==97) ) {
                alt41=1;
            }
            switch (alt41) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3407:2: rule__Method__ClassAssignment_3
                    {
                    pushFollow(FOLLOW_rule__Method__ClassAssignment_3_in_rule__Method__Group__36827);
                    rule__Method__ClassAssignment_3();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getMethodAccess().getClassAssignment_3()); 

            }

            pushFollow(FOLLOW_rule__Method__Group__4_in_rule__Method__Group__36837);
            rule__Method__Group__4();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group__3


    // $ANTLR start rule__Method__Group__4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3418:1: rule__Method__Group__4 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Method__Group__5 ;
    public final void rule__Method__Group__4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3422:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Method__Group__5 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3423:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Method__Group__5
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3423:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3424:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3424:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3425:1: ( RULE_TERM )
            {
             before(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_4()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3426:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3426:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Method__Group__46868); 

            }

             after(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_4()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3429:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3430:1: ( RULE_TERM )*
            {
             before(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_4()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3431:1: ( RULE_TERM )*
            loop42:
            do {
                int alt42=2;
                int LA42_0 = input.LA(1);

                if ( (LA42_0==RULE_TERM) ) {
                    alt42=1;
                }


                switch (alt42) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3431:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Method__Group__46881); 

            	    }
            	    break;

            	default :
            	    break loop42;
                }
            } while (true);

             after(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_4()); 

            }


            }

            pushFollow(FOLLOW_rule__Method__Group__5_in_rule__Method__Group__46893);
            rule__Method__Group__5();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group__4


    // $ANTLR start rule__Method__Group__5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3443:1: rule__Method__Group__5 : ( ( rule__Method__Group_5__0 )? ) rule__Method__Group__6 ;
    public final void rule__Method__Group__5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3447:1: ( ( ( rule__Method__Group_5__0 )? ) rule__Method__Group__6 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3448:1: ( ( rule__Method__Group_5__0 )? ) rule__Method__Group__6
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3448:1: ( ( rule__Method__Group_5__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3449:1: ( rule__Method__Group_5__0 )?
            {
             before(grammarAccess.getMethodAccess().getGroup_5()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3450:1: ( rule__Method__Group_5__0 )?
            int alt43=2;
            int LA43_0 = input.LA(1);

            if ( (LA43_0==88) ) {
                alt43=1;
            }
            switch (alt43) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3450:2: rule__Method__Group_5__0
                    {
                    pushFollow(FOLLOW_rule__Method__Group_5__0_in_rule__Method__Group__56921);
                    rule__Method__Group_5__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getMethodAccess().getGroup_5()); 

            }

            pushFollow(FOLLOW_rule__Method__Group__6_in_rule__Method__Group__56931);
            rule__Method__Group__6();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group__5


    // $ANTLR start rule__Method__Group__6
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3461:1: rule__Method__Group__6 : ( ( rule__Method__ContentAssignment_6 )* ) ;
    public final void rule__Method__Group__6() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3465:1: ( ( ( rule__Method__ContentAssignment_6 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3466:1: ( ( rule__Method__ContentAssignment_6 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3466:1: ( ( rule__Method__ContentAssignment_6 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3467:1: ( rule__Method__ContentAssignment_6 )*
            {
             before(grammarAccess.getMethodAccess().getContentAssignment_6()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3468:1: ( rule__Method__ContentAssignment_6 )*
            loop44:
            do {
                int alt44=2;
                int LA44_0 = input.LA(1);

                if ( ((LA44_0>=RULE_STRING && LA44_0<=RULE_SYMBOL_VAR)||LA44_0==63||LA44_0==66||(LA44_0>=68 && LA44_0<=69)||(LA44_0>=71 && LA44_0<=72)) ) {
                    alt44=1;
                }


                switch (alt44) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3468:2: rule__Method__ContentAssignment_6
            	    {
            	    pushFollow(FOLLOW_rule__Method__ContentAssignment_6_in_rule__Method__Group__66959);
            	    rule__Method__ContentAssignment_6();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop44;
                }
            } while (true);

             after(grammarAccess.getMethodAccess().getContentAssignment_6()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group__6


    // $ANTLR start rule__Method__Group_5__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3492:1: rule__Method__Group_5__0 : ( 'expose' ) rule__Method__Group_5__1 ;
    public final void rule__Method__Group_5__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3496:1: ( ( 'expose' ) rule__Method__Group_5__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3497:1: ( 'expose' ) rule__Method__Group_5__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3497:1: ( 'expose' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3498:1: 'expose'
            {
             before(grammarAccess.getMethodAccess().getExposeKeyword_5_0()); 
            match(input,88,FOLLOW_88_in_rule__Method__Group_5__07009); 
             after(grammarAccess.getMethodAccess().getExposeKeyword_5_0()); 

            }

            pushFollow(FOLLOW_rule__Method__Group_5__1_in_rule__Method__Group_5__07019);
            rule__Method__Group_5__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group_5__0


    // $ANTLR start rule__Method__Group_5__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3512:1: rule__Method__Group_5__1 : ( ( ( rule__Method__ExposeAssignment_5_1 ) ) ( ( rule__Method__ExposeAssignment_5_1 )* ) ) rule__Method__Group_5__2 ;
    public final void rule__Method__Group_5__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3516:1: ( ( ( ( rule__Method__ExposeAssignment_5_1 ) ) ( ( rule__Method__ExposeAssignment_5_1 )* ) ) rule__Method__Group_5__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3517:1: ( ( ( rule__Method__ExposeAssignment_5_1 ) ) ( ( rule__Method__ExposeAssignment_5_1 )* ) ) rule__Method__Group_5__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3517:1: ( ( ( rule__Method__ExposeAssignment_5_1 ) ) ( ( rule__Method__ExposeAssignment_5_1 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3518:1: ( ( rule__Method__ExposeAssignment_5_1 ) ) ( ( rule__Method__ExposeAssignment_5_1 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3518:1: ( ( rule__Method__ExposeAssignment_5_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3519:1: ( rule__Method__ExposeAssignment_5_1 )
            {
             before(grammarAccess.getMethodAccess().getExposeAssignment_5_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3520:1: ( rule__Method__ExposeAssignment_5_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3520:2: rule__Method__ExposeAssignment_5_1
            {
            pushFollow(FOLLOW_rule__Method__ExposeAssignment_5_1_in_rule__Method__Group_5__17049);
            rule__Method__ExposeAssignment_5_1();
            _fsp--;


            }

             after(grammarAccess.getMethodAccess().getExposeAssignment_5_1()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3523:1: ( ( rule__Method__ExposeAssignment_5_1 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3524:1: ( rule__Method__ExposeAssignment_5_1 )*
            {
             before(grammarAccess.getMethodAccess().getExposeAssignment_5_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3525:1: ( rule__Method__ExposeAssignment_5_1 )*
            loop45:
            do {
                int alt45=2;
                int LA45_0 = input.LA(1);

                if ( ((LA45_0>=RULE_STRING && LA45_0<=RULE_SYMBOL_VAR)) ) {
                    alt45=1;
                }


                switch (alt45) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3525:2: rule__Method__ExposeAssignment_5_1
            	    {
            	    pushFollow(FOLLOW_rule__Method__ExposeAssignment_5_1_in_rule__Method__Group_5__17061);
            	    rule__Method__ExposeAssignment_5_1();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop45;
                }
            } while (true);

             after(grammarAccess.getMethodAccess().getExposeAssignment_5_1()); 

            }


            }

            pushFollow(FOLLOW_rule__Method__Group_5__2_in_rule__Method__Group_5__17073);
            rule__Method__Group_5__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group_5__1


    // $ANTLR start rule__Method__Group_5__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3537:1: rule__Method__Group_5__2 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) ;
    public final void rule__Method__Group_5__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3541:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3542:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3542:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3543:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3543:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3544:1: ( RULE_TERM )
            {
             before(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_5_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3545:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3545:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Method__Group_5__27104); 

            }

             after(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_5_2()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3548:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3549:1: ( RULE_TERM )*
            {
             before(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_5_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3550:1: ( RULE_TERM )*
            loop46:
            do {
                int alt46=2;
                int LA46_0 = input.LA(1);

                if ( (LA46_0==RULE_TERM) ) {
                    alt46=1;
                }


                switch (alt46) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3550:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Method__Group_5__27117); 

            	    }
            	    break;

            	default :
            	    break loop46;
                }
            } while (true);

             after(grammarAccess.getMethodAccess().getTERMTerminalRuleCall_5_2()); 

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__Group_5__2


    // $ANTLR start rule__Attribute__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3567:1: rule__Attribute__Group__0 : ( ( rule__Attribute__Alternatives_0 ) ) rule__Attribute__Group__1 ;
    public final void rule__Attribute__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3571:1: ( ( ( rule__Attribute__Alternatives_0 ) ) rule__Attribute__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3572:1: ( ( rule__Attribute__Alternatives_0 ) ) rule__Attribute__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3572:1: ( ( rule__Attribute__Alternatives_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3573:1: ( rule__Attribute__Alternatives_0 )
            {
             before(grammarAccess.getAttributeAccess().getAlternatives_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3574:1: ( rule__Attribute__Alternatives_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3574:2: rule__Attribute__Alternatives_0
            {
            pushFollow(FOLLOW_rule__Attribute__Alternatives_0_in_rule__Attribute__Group__07160);
            rule__Attribute__Alternatives_0();
            _fsp--;


            }

             after(grammarAccess.getAttributeAccess().getAlternatives_0()); 

            }

            pushFollow(FOLLOW_rule__Attribute__Group__1_in_rule__Attribute__Group__07169);
            rule__Attribute__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group__0


    // $ANTLR start rule__Attribute__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3585:1: rule__Attribute__Group__1 : ( ( rule__Attribute__VisibilityAssignment_1 )? ) rule__Attribute__Group__2 ;
    public final void rule__Attribute__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3589:1: ( ( ( rule__Attribute__VisibilityAssignment_1 )? ) rule__Attribute__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3590:1: ( ( rule__Attribute__VisibilityAssignment_1 )? ) rule__Attribute__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3590:1: ( ( rule__Attribute__VisibilityAssignment_1 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3591:1: ( rule__Attribute__VisibilityAssignment_1 )?
            {
             before(grammarAccess.getAttributeAccess().getVisibilityAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3592:1: ( rule__Attribute__VisibilityAssignment_1 )?
            int alt47=2;
            int LA47_0 = input.LA(1);

            if ( ((LA47_0>=60 && LA47_0<=61)) ) {
                alt47=1;
            }
            switch (alt47) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3592:2: rule__Attribute__VisibilityAssignment_1
                    {
                    pushFollow(FOLLOW_rule__Attribute__VisibilityAssignment_1_in_rule__Attribute__Group__17197);
                    rule__Attribute__VisibilityAssignment_1();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getAttributeAccess().getVisibilityAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__Attribute__Group__2_in_rule__Attribute__Group__17207);
            rule__Attribute__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group__1


    // $ANTLR start rule__Attribute__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3603:1: rule__Attribute__Group__2 : ( ( rule__Attribute__ClassAssignment_2 )? ) rule__Attribute__Group__3 ;
    public final void rule__Attribute__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3607:1: ( ( ( rule__Attribute__ClassAssignment_2 )? ) rule__Attribute__Group__3 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3608:1: ( ( rule__Attribute__ClassAssignment_2 )? ) rule__Attribute__Group__3
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3608:1: ( ( rule__Attribute__ClassAssignment_2 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3609:1: ( rule__Attribute__ClassAssignment_2 )?
            {
             before(grammarAccess.getAttributeAccess().getClassAssignment_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3610:1: ( rule__Attribute__ClassAssignment_2 )?
            int alt48=2;
            int LA48_0 = input.LA(1);

            if ( (LA48_0==97) ) {
                alt48=1;
            }
            switch (alt48) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3610:2: rule__Attribute__ClassAssignment_2
                    {
                    pushFollow(FOLLOW_rule__Attribute__ClassAssignment_2_in_rule__Attribute__Group__27235);
                    rule__Attribute__ClassAssignment_2();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getAttributeAccess().getClassAssignment_2()); 

            }

            pushFollow(FOLLOW_rule__Attribute__Group__3_in_rule__Attribute__Group__27245);
            rule__Attribute__Group__3();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group__2


    // $ANTLR start rule__Attribute__Group__3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3621:1: rule__Attribute__Group__3 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Attribute__Group__4 ;
    public final void rule__Attribute__Group__3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3625:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Attribute__Group__4 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3626:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) rule__Attribute__Group__4
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3626:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3627:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3627:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3628:1: ( RULE_TERM )
            {
             before(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_3()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3629:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3629:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Attribute__Group__37276); 

            }

             after(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_3()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3632:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3633:1: ( RULE_TERM )*
            {
             before(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_3()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3634:1: ( RULE_TERM )*
            loop49:
            do {
                int alt49=2;
                int LA49_0 = input.LA(1);

                if ( (LA49_0==RULE_TERM) ) {
                    alt49=1;
                }


                switch (alt49) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3634:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Attribute__Group__37289); 

            	    }
            	    break;

            	default :
            	    break loop49;
                }
            } while (true);

             after(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_3()); 

            }


            }

            pushFollow(FOLLOW_rule__Attribute__Group__4_in_rule__Attribute__Group__37301);
            rule__Attribute__Group__4();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group__3


    // $ANTLR start rule__Attribute__Group__4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3646:1: rule__Attribute__Group__4 : ( ( rule__Attribute__Group_4__0 )? ) rule__Attribute__Group__5 ;
    public final void rule__Attribute__Group__4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3650:1: ( ( ( rule__Attribute__Group_4__0 )? ) rule__Attribute__Group__5 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3651:1: ( ( rule__Attribute__Group_4__0 )? ) rule__Attribute__Group__5
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3651:1: ( ( rule__Attribute__Group_4__0 )? )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3652:1: ( rule__Attribute__Group_4__0 )?
            {
             before(grammarAccess.getAttributeAccess().getGroup_4()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3653:1: ( rule__Attribute__Group_4__0 )?
            int alt50=2;
            int LA50_0 = input.LA(1);

            if ( (LA50_0==88) ) {
                alt50=1;
            }
            switch (alt50) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3653:2: rule__Attribute__Group_4__0
                    {
                    pushFollow(FOLLOW_rule__Attribute__Group_4__0_in_rule__Attribute__Group__47329);
                    rule__Attribute__Group_4__0();
                    _fsp--;


                    }
                    break;

            }

             after(grammarAccess.getAttributeAccess().getGroup_4()); 

            }

            pushFollow(FOLLOW_rule__Attribute__Group__5_in_rule__Attribute__Group__47339);
            rule__Attribute__Group__5();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group__4


    // $ANTLR start rule__Attribute__Group__5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3664:1: rule__Attribute__Group__5 : ( ( rule__Attribute__ContentAssignment_5 )* ) ;
    public final void rule__Attribute__Group__5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3668:1: ( ( ( rule__Attribute__ContentAssignment_5 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3669:1: ( ( rule__Attribute__ContentAssignment_5 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3669:1: ( ( rule__Attribute__ContentAssignment_5 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3670:1: ( rule__Attribute__ContentAssignment_5 )*
            {
             before(grammarAccess.getAttributeAccess().getContentAssignment_5()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3671:1: ( rule__Attribute__ContentAssignment_5 )*
            loop51:
            do {
                int alt51=2;
                int LA51_0 = input.LA(1);

                if ( ((LA51_0>=RULE_STRING && LA51_0<=RULE_SYMBOL_VAR)||LA51_0==63||LA51_0==66||(LA51_0>=68 && LA51_0<=69)||(LA51_0>=71 && LA51_0<=72)) ) {
                    alt51=1;
                }


                switch (alt51) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3671:2: rule__Attribute__ContentAssignment_5
            	    {
            	    pushFollow(FOLLOW_rule__Attribute__ContentAssignment_5_in_rule__Attribute__Group__57367);
            	    rule__Attribute__ContentAssignment_5();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop51;
                }
            } while (true);

             after(grammarAccess.getAttributeAccess().getContentAssignment_5()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group__5


    // $ANTLR start rule__Attribute__Group_4__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3693:1: rule__Attribute__Group_4__0 : ( 'expose' ) rule__Attribute__Group_4__1 ;
    public final void rule__Attribute__Group_4__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3697:1: ( ( 'expose' ) rule__Attribute__Group_4__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3698:1: ( 'expose' ) rule__Attribute__Group_4__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3698:1: ( 'expose' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3699:1: 'expose'
            {
             before(grammarAccess.getAttributeAccess().getExposeKeyword_4_0()); 
            match(input,88,FOLLOW_88_in_rule__Attribute__Group_4__07415); 
             after(grammarAccess.getAttributeAccess().getExposeKeyword_4_0()); 

            }

            pushFollow(FOLLOW_rule__Attribute__Group_4__1_in_rule__Attribute__Group_4__07425);
            rule__Attribute__Group_4__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group_4__0


    // $ANTLR start rule__Attribute__Group_4__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3713:1: rule__Attribute__Group_4__1 : ( ( ( rule__Attribute__ExposeAssignment_4_1 ) ) ( ( rule__Attribute__ExposeAssignment_4_1 )* ) ) rule__Attribute__Group_4__2 ;
    public final void rule__Attribute__Group_4__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3717:1: ( ( ( ( rule__Attribute__ExposeAssignment_4_1 ) ) ( ( rule__Attribute__ExposeAssignment_4_1 )* ) ) rule__Attribute__Group_4__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3718:1: ( ( ( rule__Attribute__ExposeAssignment_4_1 ) ) ( ( rule__Attribute__ExposeAssignment_4_1 )* ) ) rule__Attribute__Group_4__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3718:1: ( ( ( rule__Attribute__ExposeAssignment_4_1 ) ) ( ( rule__Attribute__ExposeAssignment_4_1 )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3719:1: ( ( rule__Attribute__ExposeAssignment_4_1 ) ) ( ( rule__Attribute__ExposeAssignment_4_1 )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3719:1: ( ( rule__Attribute__ExposeAssignment_4_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3720:1: ( rule__Attribute__ExposeAssignment_4_1 )
            {
             before(grammarAccess.getAttributeAccess().getExposeAssignment_4_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3721:1: ( rule__Attribute__ExposeAssignment_4_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3721:2: rule__Attribute__ExposeAssignment_4_1
            {
            pushFollow(FOLLOW_rule__Attribute__ExposeAssignment_4_1_in_rule__Attribute__Group_4__17455);
            rule__Attribute__ExposeAssignment_4_1();
            _fsp--;


            }

             after(grammarAccess.getAttributeAccess().getExposeAssignment_4_1()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3724:1: ( ( rule__Attribute__ExposeAssignment_4_1 )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3725:1: ( rule__Attribute__ExposeAssignment_4_1 )*
            {
             before(grammarAccess.getAttributeAccess().getExposeAssignment_4_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3726:1: ( rule__Attribute__ExposeAssignment_4_1 )*
            loop52:
            do {
                int alt52=2;
                int LA52_0 = input.LA(1);

                if ( ((LA52_0>=RULE_STRING && LA52_0<=RULE_SYMBOL_VAR)) ) {
                    alt52=1;
                }


                switch (alt52) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3726:2: rule__Attribute__ExposeAssignment_4_1
            	    {
            	    pushFollow(FOLLOW_rule__Attribute__ExposeAssignment_4_1_in_rule__Attribute__Group_4__17467);
            	    rule__Attribute__ExposeAssignment_4_1();
            	    _fsp--;


            	    }
            	    break;

            	default :
            	    break loop52;
                }
            } while (true);

             after(grammarAccess.getAttributeAccess().getExposeAssignment_4_1()); 

            }


            }

            pushFollow(FOLLOW_rule__Attribute__Group_4__2_in_rule__Attribute__Group_4__17479);
            rule__Attribute__Group_4__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group_4__1


    // $ANTLR start rule__Attribute__Group_4__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3738:1: rule__Attribute__Group_4__2 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) ;
    public final void rule__Attribute__Group_4__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3742:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3743:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3743:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3744:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3744:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3745:1: ( RULE_TERM )
            {
             before(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_4_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3746:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3746:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Attribute__Group_4__27510); 

            }

             after(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_4_2()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3749:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3750:1: ( RULE_TERM )*
            {
             before(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_4_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3751:1: ( RULE_TERM )*
            loop53:
            do {
                int alt53=2;
                int LA53_0 = input.LA(1);

                if ( (LA53_0==RULE_TERM) ) {
                    alt53=1;
                }


                switch (alt53) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3751:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Attribute__Group_4__27523); 

            	    }
            	    break;

            	default :
            	    break loop53;
                }
            } while (true);

             after(grammarAccess.getAttributeAccess().getTERMTerminalRuleCall_4_2()); 

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__Group_4__2


    // $ANTLR start rule__AttributeSet__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3768:1: rule__AttributeSet__Group__0 : ( '::attribute' ) rule__AttributeSet__Group__1 ;
    public final void rule__AttributeSet__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3772:1: ( ( '::attribute' ) rule__AttributeSet__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3773:1: ( '::attribute' ) rule__AttributeSet__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3773:1: ( '::attribute' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3774:1: '::attribute'
            {
             before(grammarAccess.getAttributeSetAccess().getAttributeKeyword_0()); 
            match(input,89,FOLLOW_89_in_rule__AttributeSet__Group__07567); 
             after(grammarAccess.getAttributeSetAccess().getAttributeKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__AttributeSet__Group__1_in_rule__AttributeSet__Group__07577);
            rule__AttributeSet__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeSet__Group__0


    // $ANTLR start rule__AttributeSet__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3788:1: rule__AttributeSet__Group__1 : ( ( rule__AttributeSet__NameAssignment_1 ) ) rule__AttributeSet__Group__2 ;
    public final void rule__AttributeSet__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3792:1: ( ( ( rule__AttributeSet__NameAssignment_1 ) ) rule__AttributeSet__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3793:1: ( ( rule__AttributeSet__NameAssignment_1 ) ) rule__AttributeSet__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3793:1: ( ( rule__AttributeSet__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3794:1: ( rule__AttributeSet__NameAssignment_1 )
            {
             before(grammarAccess.getAttributeSetAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3795:1: ( rule__AttributeSet__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3795:2: rule__AttributeSet__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__AttributeSet__NameAssignment_1_in_rule__AttributeSet__Group__17605);
            rule__AttributeSet__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getAttributeSetAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__AttributeSet__Group__2_in_rule__AttributeSet__Group__17614);
            rule__AttributeSet__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeSet__Group__1


    // $ANTLR start rule__AttributeSet__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3806:1: rule__AttributeSet__Group__2 : ( 'set' ) ;
    public final void rule__AttributeSet__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3810:1: ( ( 'set' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3811:1: ( 'set' )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3811:1: ( 'set' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3812:1: 'set'
            {
             before(grammarAccess.getAttributeSetAccess().getSetKeyword_2()); 
            match(input,90,FOLLOW_90_in_rule__AttributeSet__Group__27643); 
             after(grammarAccess.getAttributeSetAccess().getSetKeyword_2()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeSet__Group__2


    // $ANTLR start rule__AttributeGet__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3831:1: rule__AttributeGet__Group__0 : ( '::attribute' ) rule__AttributeGet__Group__1 ;
    public final void rule__AttributeGet__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3835:1: ( ( '::attribute' ) rule__AttributeGet__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3836:1: ( '::attribute' ) rule__AttributeGet__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3836:1: ( '::attribute' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3837:1: '::attribute'
            {
             before(grammarAccess.getAttributeGetAccess().getAttributeKeyword_0()); 
            match(input,89,FOLLOW_89_in_rule__AttributeGet__Group__07685); 
             after(grammarAccess.getAttributeGetAccess().getAttributeKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__AttributeGet__Group__1_in_rule__AttributeGet__Group__07695);
            rule__AttributeGet__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeGet__Group__0


    // $ANTLR start rule__AttributeGet__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3851:1: rule__AttributeGet__Group__1 : ( ( rule__AttributeGet__NameAssignment_1 ) ) rule__AttributeGet__Group__2 ;
    public final void rule__AttributeGet__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3855:1: ( ( ( rule__AttributeGet__NameAssignment_1 ) ) rule__AttributeGet__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3856:1: ( ( rule__AttributeGet__NameAssignment_1 ) ) rule__AttributeGet__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3856:1: ( ( rule__AttributeGet__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3857:1: ( rule__AttributeGet__NameAssignment_1 )
            {
             before(grammarAccess.getAttributeGetAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3858:1: ( rule__AttributeGet__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3858:2: rule__AttributeGet__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__AttributeGet__NameAssignment_1_in_rule__AttributeGet__Group__17723);
            rule__AttributeGet__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getAttributeGetAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__AttributeGet__Group__2_in_rule__AttributeGet__Group__17732);
            rule__AttributeGet__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeGet__Group__1


    // $ANTLR start rule__AttributeGet__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3869:1: rule__AttributeGet__Group__2 : ( 'get' ) ;
    public final void rule__AttributeGet__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3873:1: ( ( 'get' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3874:1: ( 'get' )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3874:1: ( 'get' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3875:1: 'get'
            {
             before(grammarAccess.getAttributeGetAccess().getGetKeyword_2()); 
            match(input,91,FOLLOW_91_in_rule__AttributeGet__Group__27761); 
             after(grammarAccess.getAttributeGetAccess().getGetKeyword_2()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeGet__Group__2


    // $ANTLR start rule__AttributeNormal__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3894:1: rule__AttributeNormal__Group__0 : ( '::attribute' ) rule__AttributeNormal__Group__1 ;
    public final void rule__AttributeNormal__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3898:1: ( ( '::attribute' ) rule__AttributeNormal__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3899:1: ( '::attribute' ) rule__AttributeNormal__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3899:1: ( '::attribute' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3900:1: '::attribute'
            {
             before(grammarAccess.getAttributeNormalAccess().getAttributeKeyword_0()); 
            match(input,89,FOLLOW_89_in_rule__AttributeNormal__Group__07803); 
             after(grammarAccess.getAttributeNormalAccess().getAttributeKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__AttributeNormal__Group__1_in_rule__AttributeNormal__Group__07813);
            rule__AttributeNormal__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeNormal__Group__0


    // $ANTLR start rule__AttributeNormal__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3914:1: rule__AttributeNormal__Group__1 : ( ( rule__AttributeNormal__NameAssignment_1 ) ) ;
    public final void rule__AttributeNormal__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3918:1: ( ( ( rule__AttributeNormal__NameAssignment_1 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3919:1: ( ( rule__AttributeNormal__NameAssignment_1 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3919:1: ( ( rule__AttributeNormal__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3920:1: ( rule__AttributeNormal__NameAssignment_1 )
            {
             before(grammarAccess.getAttributeNormalAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3921:1: ( rule__AttributeNormal__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3921:2: rule__AttributeNormal__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__AttributeNormal__NameAssignment_1_in_rule__AttributeNormal__Group__17841);
            rule__AttributeNormal__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getAttributeNormalAccess().getNameAssignment_1()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeNormal__Group__1


    // $ANTLR start rule__Constant__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3935:1: rule__Constant__Group__0 : ( '::constant' ) rule__Constant__Group__1 ;
    public final void rule__Constant__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3939:1: ( ( '::constant' ) rule__Constant__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3940:1: ( '::constant' ) rule__Constant__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3940:1: ( '::constant' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3941:1: '::constant'
            {
             before(grammarAccess.getConstantAccess().getConstantKeyword_0()); 
            match(input,92,FOLLOW_92_in_rule__Constant__Group__07880); 
             after(grammarAccess.getConstantAccess().getConstantKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Constant__Group__1_in_rule__Constant__Group__07890);
            rule__Constant__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Constant__Group__0


    // $ANTLR start rule__Constant__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3955:1: rule__Constant__Group__1 : ( ( rule__Constant__NameAssignment_1 ) ) rule__Constant__Group__2 ;
    public final void rule__Constant__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3959:1: ( ( ( rule__Constant__NameAssignment_1 ) ) rule__Constant__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3960:1: ( ( rule__Constant__NameAssignment_1 ) ) rule__Constant__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3960:1: ( ( rule__Constant__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3961:1: ( rule__Constant__NameAssignment_1 )
            {
             before(grammarAccess.getConstantAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3962:1: ( rule__Constant__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3962:2: rule__Constant__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__Constant__NameAssignment_1_in_rule__Constant__Group__17918);
            rule__Constant__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getConstantAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__Constant__Group__2_in_rule__Constant__Group__17927);
            rule__Constant__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Constant__Group__1


    // $ANTLR start rule__Constant__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3973:1: rule__Constant__Group__2 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) ;
    public final void rule__Constant__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3977:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3978:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3978:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3979:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3979:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3980:1: ( RULE_TERM )
            {
             before(grammarAccess.getConstantAccess().getTERMTerminalRuleCall_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3981:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3981:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Constant__Group__27958); 

            }

             after(grammarAccess.getConstantAccess().getTERMTerminalRuleCall_2()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3984:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3985:1: ( RULE_TERM )*
            {
             before(grammarAccess.getConstantAccess().getTERMTerminalRuleCall_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3986:1: ( RULE_TERM )*
            loop54:
            do {
                int alt54=2;
                int LA54_0 = input.LA(1);

                if ( (LA54_0==RULE_TERM) ) {
                    alt54=1;
                }


                switch (alt54) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:3986:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Constant__Group__27971); 

            	    }
            	    break;

            	default :
            	    break loop54;
                }
            } while (true);

             after(grammarAccess.getConstantAccess().getTERMTerminalRuleCall_2()); 

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Constant__Group__2


    // $ANTLR start rule__Routine__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4003:1: rule__Routine__Group__0 : ( '::routine' ) rule__Routine__Group__1 ;
    public final void rule__Routine__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4007:1: ( ( '::routine' ) rule__Routine__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4008:1: ( '::routine' ) rule__Routine__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4008:1: ( '::routine' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4009:1: '::routine'
            {
             before(grammarAccess.getRoutineAccess().getRoutineKeyword_0()); 
            match(input,93,FOLLOW_93_in_rule__Routine__Group__08015); 
             after(grammarAccess.getRoutineAccess().getRoutineKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Routine__Group__1_in_rule__Routine__Group__08025);
            rule__Routine__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Routine__Group__0


    // $ANTLR start rule__Routine__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4023:1: rule__Routine__Group__1 : ( ( rule__Routine__NameAssignment_1 ) ) rule__Routine__Group__2 ;
    public final void rule__Routine__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4027:1: ( ( ( rule__Routine__NameAssignment_1 ) ) rule__Routine__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4028:1: ( ( rule__Routine__NameAssignment_1 ) ) rule__Routine__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4028:1: ( ( rule__Routine__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4029:1: ( rule__Routine__NameAssignment_1 )
            {
             before(grammarAccess.getRoutineAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4030:1: ( rule__Routine__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4030:2: rule__Routine__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__Routine__NameAssignment_1_in_rule__Routine__Group__18053);
            rule__Routine__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getRoutineAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__Routine__Group__2_in_rule__Routine__Group__18062);
            rule__Routine__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Routine__Group__1


    // $ANTLR start rule__Routine__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4041:1: rule__Routine__Group__2 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) ;
    public final void rule__Routine__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4045:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4046:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4046:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4047:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4047:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4048:1: ( RULE_TERM )
            {
             before(grammarAccess.getRoutineAccess().getTERMTerminalRuleCall_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4049:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4049:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Routine__Group__28093); 

            }

             after(grammarAccess.getRoutineAccess().getTERMTerminalRuleCall_2()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4052:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4053:1: ( RULE_TERM )*
            {
             before(grammarAccess.getRoutineAccess().getTERMTerminalRuleCall_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4054:1: ( RULE_TERM )*
            loop55:
            do {
                int alt55=2;
                int LA55_0 = input.LA(1);

                if ( (LA55_0==RULE_TERM) ) {
                    alt55=1;
                }


                switch (alt55) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4054:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Routine__Group__28106); 

            	    }
            	    break;

            	default :
            	    break loop55;
                }
            } while (true);

             after(grammarAccess.getRoutineAccess().getTERMTerminalRuleCall_2()); 

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Routine__Group__2


    // $ANTLR start rule__Requires__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4071:1: rule__Requires__Group__0 : ( '::requires' ) rule__Requires__Group__1 ;
    public final void rule__Requires__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4075:1: ( ( '::requires' ) rule__Requires__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4076:1: ( '::requires' ) rule__Requires__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4076:1: ( '::requires' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4077:1: '::requires'
            {
             before(grammarAccess.getRequiresAccess().getRequiresKeyword_0()); 
            match(input,94,FOLLOW_94_in_rule__Requires__Group__08150); 
             after(grammarAccess.getRequiresAccess().getRequiresKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Requires__Group__1_in_rule__Requires__Group__08160);
            rule__Requires__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Requires__Group__0


    // $ANTLR start rule__Requires__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4091:1: rule__Requires__Group__1 : ( ( rule__Requires__NameAssignment_1 ) ) rule__Requires__Group__2 ;
    public final void rule__Requires__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4095:1: ( ( ( rule__Requires__NameAssignment_1 ) ) rule__Requires__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4096:1: ( ( rule__Requires__NameAssignment_1 ) ) rule__Requires__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4096:1: ( ( rule__Requires__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4097:1: ( rule__Requires__NameAssignment_1 )
            {
             before(grammarAccess.getRequiresAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4098:1: ( rule__Requires__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4098:2: rule__Requires__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__Requires__NameAssignment_1_in_rule__Requires__Group__18188);
            rule__Requires__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getRequiresAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__Requires__Group__2_in_rule__Requires__Group__18197);
            rule__Requires__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Requires__Group__1


    // $ANTLR start rule__Requires__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4109:1: rule__Requires__Group__2 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) ;
    public final void rule__Requires__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4113:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4114:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4114:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4115:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4115:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4116:1: ( RULE_TERM )
            {
             before(grammarAccess.getRequiresAccess().getTERMTerminalRuleCall_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4117:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4117:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Requires__Group__28228); 

            }

             after(grammarAccess.getRequiresAccess().getTERMTerminalRuleCall_2()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4120:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4121:1: ( RULE_TERM )*
            {
             before(grammarAccess.getRequiresAccess().getTERMTerminalRuleCall_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4122:1: ( RULE_TERM )*
            loop56:
            do {
                int alt56=2;
                int LA56_0 = input.LA(1);

                if ( (LA56_0==RULE_TERM) ) {
                    alt56=1;
                }


                switch (alt56) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4122:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Requires__Group__28241); 

            	    }
            	    break;

            	default :
            	    break loop56;
                }
            } while (true);

             after(grammarAccess.getRequiresAccess().getTERMTerminalRuleCall_2()); 

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Requires__Group__2


    // $ANTLR start rule__Options__Group__0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4139:1: rule__Options__Group__0 : ( '::options' ) rule__Options__Group__1 ;
    public final void rule__Options__Group__0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4143:1: ( ( '::options' ) rule__Options__Group__1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4144:1: ( '::options' ) rule__Options__Group__1
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4144:1: ( '::options' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4145:1: '::options'
            {
             before(grammarAccess.getOptionsAccess().getOptionsKeyword_0()); 
            match(input,95,FOLLOW_95_in_rule__Options__Group__08285); 
             after(grammarAccess.getOptionsAccess().getOptionsKeyword_0()); 

            }

            pushFollow(FOLLOW_rule__Options__Group__1_in_rule__Options__Group__08295);
            rule__Options__Group__1();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Options__Group__0


    // $ANTLR start rule__Options__Group__1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4159:1: rule__Options__Group__1 : ( ( rule__Options__NameAssignment_1 ) ) rule__Options__Group__2 ;
    public final void rule__Options__Group__1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4163:1: ( ( ( rule__Options__NameAssignment_1 ) ) rule__Options__Group__2 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4164:1: ( ( rule__Options__NameAssignment_1 ) ) rule__Options__Group__2
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4164:1: ( ( rule__Options__NameAssignment_1 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4165:1: ( rule__Options__NameAssignment_1 )
            {
             before(grammarAccess.getOptionsAccess().getNameAssignment_1()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4166:1: ( rule__Options__NameAssignment_1 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4166:2: rule__Options__NameAssignment_1
            {
            pushFollow(FOLLOW_rule__Options__NameAssignment_1_in_rule__Options__Group__18323);
            rule__Options__NameAssignment_1();
            _fsp--;


            }

             after(grammarAccess.getOptionsAccess().getNameAssignment_1()); 

            }

            pushFollow(FOLLOW_rule__Options__Group__2_in_rule__Options__Group__18332);
            rule__Options__Group__2();
            _fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Options__Group__1


    // $ANTLR start rule__Options__Group__2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4177:1: rule__Options__Group__2 : ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) ;
    public final void rule__Options__Group__2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4181:1: ( ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4182:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4182:1: ( ( ( RULE_TERM ) ) ( ( RULE_TERM )* ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4183:1: ( ( RULE_TERM ) ) ( ( RULE_TERM )* )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4183:1: ( ( RULE_TERM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4184:1: ( RULE_TERM )
            {
             before(grammarAccess.getOptionsAccess().getTERMTerminalRuleCall_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4185:1: ( RULE_TERM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4185:3: RULE_TERM
            {
            match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Options__Group__28363); 

            }

             after(grammarAccess.getOptionsAccess().getTERMTerminalRuleCall_2()); 

            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4188:1: ( ( RULE_TERM )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4189:1: ( RULE_TERM )*
            {
             before(grammarAccess.getOptionsAccess().getTERMTerminalRuleCall_2()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4190:1: ( RULE_TERM )*
            loop57:
            do {
                int alt57=2;
                int LA57_0 = input.LA(1);

                if ( (LA57_0==RULE_TERM) ) {
                    alt57=1;
                }


                switch (alt57) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4190:3: RULE_TERM
            	    {
            	    match(input,RULE_TERM,FOLLOW_RULE_TERM_in_rule__Options__Group__28376); 

            	    }
            	    break;

            	default :
            	    break loop57;
                }
            } while (true);

             after(grammarAccess.getOptionsAccess().getTERMTerminalRuleCall_2()); 

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Options__Group__2


    // $ANTLR start rule__File__InstructionsAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4208:1: rule__File__InstructionsAssignment_2 : ( ruleInstruction ) ;
    public final void rule__File__InstructionsAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4212:1: ( ( ruleInstruction ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4213:1: ( ruleInstruction )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4213:1: ( ruleInstruction )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4214:1: ruleInstruction
            {
             before(grammarAccess.getFileAccess().getInstructionsInstructionParserRuleCall_2_0()); 
            pushFollow(FOLLOW_ruleInstruction_in_rule__File__InstructionsAssignment_28420);
            ruleInstruction();
            _fsp--;

             after(grammarAccess.getFileAccess().getInstructionsInstructionParserRuleCall_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__InstructionsAssignment_2


    // $ANTLR start rule__File__RequiresAssignment_3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4223:1: rule__File__RequiresAssignment_3 : ( ruleRequires ) ;
    public final void rule__File__RequiresAssignment_3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4227:1: ( ( ruleRequires ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4228:1: ( ruleRequires )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4228:1: ( ruleRequires )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4229:1: ruleRequires
            {
             before(grammarAccess.getFileAccess().getRequiresRequiresParserRuleCall_3_0()); 
            pushFollow(FOLLOW_ruleRequires_in_rule__File__RequiresAssignment_38451);
            ruleRequires();
            _fsp--;

             after(grammarAccess.getFileAccess().getRequiresRequiresParserRuleCall_3_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__RequiresAssignment_3


    // $ANTLR start rule__File__MethodsAssignment_4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4238:1: rule__File__MethodsAssignment_4 : ( ruleMethod ) ;
    public final void rule__File__MethodsAssignment_4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4242:1: ( ( ruleMethod ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4243:1: ( ruleMethod )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4243:1: ( ruleMethod )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4244:1: ruleMethod
            {
             before(grammarAccess.getFileAccess().getMethodsMethodParserRuleCall_4_0()); 
            pushFollow(FOLLOW_ruleMethod_in_rule__File__MethodsAssignment_48482);
            ruleMethod();
            _fsp--;

             after(grammarAccess.getFileAccess().getMethodsMethodParserRuleCall_4_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__MethodsAssignment_4


    // $ANTLR start rule__File__DirectivesAssignment_5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4253:1: rule__File__DirectivesAssignment_5 : ( ruleDirectives ) ;
    public final void rule__File__DirectivesAssignment_5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4257:1: ( ( ruleDirectives ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4258:1: ( ruleDirectives )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4258:1: ( ruleDirectives )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4259:1: ruleDirectives
            {
             before(grammarAccess.getFileAccess().getDirectivesDirectivesParserRuleCall_5_0()); 
            pushFollow(FOLLOW_ruleDirectives_in_rule__File__DirectivesAssignment_58513);
            ruleDirectives();
            _fsp--;

             after(grammarAccess.getFileAccess().getDirectivesDirectivesParserRuleCall_5_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__File__DirectivesAssignment_5


    // $ANTLR start rule__Assignment__TargetAssignment_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4268:1: rule__Assignment__TargetAssignment_0 : ( ruleName ) ;
    public final void rule__Assignment__TargetAssignment_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4272:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4273:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4273:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4274:1: ruleName
            {
             before(grammarAccess.getAssignmentAccess().getTargetNameParserRuleCall_0_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Assignment__TargetAssignment_08544);
            ruleName();
            _fsp--;

             after(grammarAccess.getAssignmentAccess().getTargetNameParserRuleCall_0_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Assignment__TargetAssignment_0


    // $ANTLR start rule__Assignment__ExpressionAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4283:1: rule__Assignment__ExpressionAssignment_2 : ( ruleExpression ) ;
    public final void rule__Assignment__ExpressionAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4287:1: ( ( ruleExpression ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4288:1: ( ruleExpression )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4288:1: ( ruleExpression )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4289:1: ruleExpression
            {
             before(grammarAccess.getAssignmentAccess().getExpressionExpressionParserRuleCall_2_0()); 
            pushFollow(FOLLOW_ruleExpression_in_rule__Assignment__ExpressionAssignment_28575);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getAssignmentAccess().getExpressionExpressionParserRuleCall_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Assignment__ExpressionAssignment_2


    // $ANTLR start rule__If__ExpressionAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4298:1: rule__If__ExpressionAssignment_1 : ( ruleExpression ) ;
    public final void rule__If__ExpressionAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4302:1: ( ( ruleExpression ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4303:1: ( ruleExpression )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4303:1: ( ruleExpression )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4304:1: ruleExpression
            {
             before(grammarAccess.getIfAccess().getExpressionExpressionParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleExpression_in_rule__If__ExpressionAssignment_18606);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getIfAccess().getExpressionExpressionParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__ExpressionAssignment_1


    // $ANTLR start rule__If__TopInstrAssignment_4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4313:1: rule__If__TopInstrAssignment_4 : ( ruleInstruction ) ;
    public final void rule__If__TopInstrAssignment_4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4317:1: ( ( ruleInstruction ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4318:1: ( ruleInstruction )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4318:1: ( ruleInstruction )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4319:1: ruleInstruction
            {
             before(grammarAccess.getIfAccess().getTopInstrInstructionParserRuleCall_4_0()); 
            pushFollow(FOLLOW_ruleInstruction_in_rule__If__TopInstrAssignment_48637);
            ruleInstruction();
            _fsp--;

             after(grammarAccess.getIfAccess().getTopInstrInstructionParserRuleCall_4_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__TopInstrAssignment_4


    // $ANTLR start rule__If__BottomInstrAssignment_5_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4328:1: rule__If__BottomInstrAssignment_5_2 : ( ruleInstruction ) ;
    public final void rule__If__BottomInstrAssignment_5_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4332:1: ( ( ruleInstruction ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4333:1: ( ruleInstruction )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4333:1: ( ruleInstruction )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4334:1: ruleInstruction
            {
             before(grammarAccess.getIfAccess().getBottomInstrInstructionParserRuleCall_5_2_0()); 
            pushFollow(FOLLOW_ruleInstruction_in_rule__If__BottomInstrAssignment_5_28668);
            ruleInstruction();
            _fsp--;

             after(grammarAccess.getIfAccess().getBottomInstrInstructionParserRuleCall_5_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__If__BottomInstrAssignment_5_2


    // $ANTLR start rule__Do__InstrsAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4343:1: rule__Do__InstrsAssignment_2 : ( ruleInstruction ) ;
    public final void rule__Do__InstrsAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4347:1: ( ( ruleInstruction ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4348:1: ( ruleInstruction )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4348:1: ( ruleInstruction )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4349:1: ruleInstruction
            {
             before(grammarAccess.getDoAccess().getInstrsInstructionParserRuleCall_2_0()); 
            pushFollow(FOLLOW_ruleInstruction_in_rule__Do__InstrsAssignment_28699);
            ruleInstruction();
            _fsp--;

             after(grammarAccess.getDoAccess().getInstrsInstructionParserRuleCall_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Do__InstrsAssignment_2


    // $ANTLR start rule__Call__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4358:1: rule__Call__NameAssignment_1 : ( ruleName ) ;
    public final void rule__Call__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4362:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4363:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4363:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4364:1: ruleName
            {
             before(grammarAccess.getCallAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Call__NameAssignment_18730);
            ruleName();
            _fsp--;

             after(grammarAccess.getCallAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Call__NameAssignment_1


    // $ANTLR start rule__Call__ArgsAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4373:1: rule__Call__ArgsAssignment_2 : ( ruleArguments ) ;
    public final void rule__Call__ArgsAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4377:1: ( ( ruleArguments ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4378:1: ( ruleArguments )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4378:1: ( ruleArguments )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4379:1: ruleArguments
            {
             before(grammarAccess.getCallAccess().getArgsArgumentsParserRuleCall_2_0()); 
            pushFollow(FOLLOW_ruleArguments_in_rule__Call__ArgsAssignment_28761);
            ruleArguments();
            _fsp--;

             after(grammarAccess.getCallAccess().getArgsArgumentsParserRuleCall_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Call__ArgsAssignment_2


    // $ANTLR start rule__Arg__ArgsAssignment_2_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4388:1: rule__Arg__ArgsAssignment_2_0 : ( ruleName ) ;
    public final void rule__Arg__ArgsAssignment_2_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4392:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4393:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4393:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4394:1: ruleName
            {
             before(grammarAccess.getArgAccess().getArgsNameParserRuleCall_2_0_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Arg__ArgsAssignment_2_08792);
            ruleName();
            _fsp--;

             after(grammarAccess.getArgAccess().getArgsNameParserRuleCall_2_0_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__ArgsAssignment_2_0


    // $ANTLR start rule__Arg__ArgsAssignment_2_1_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4403:1: rule__Arg__ArgsAssignment_2_1_1 : ( ruleName ) ;
    public final void rule__Arg__ArgsAssignment_2_1_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4407:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4408:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4408:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4409:1: ruleName
            {
             before(grammarAccess.getArgAccess().getArgsNameParserRuleCall_2_1_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Arg__ArgsAssignment_2_1_18823);
            ruleName();
            _fsp--;

             after(grammarAccess.getArgAccess().getArgsNameParserRuleCall_2_1_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arg__ArgsAssignment_2_1_1


    // $ANTLR start rule__UseArg__StrictAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4418:1: rule__UseArg__StrictAssignment_2 : ( ( 'strict' ) ) ;
    public final void rule__UseArg__StrictAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4422:1: ( ( ( 'strict' ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4423:1: ( ( 'strict' ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4423:1: ( ( 'strict' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4424:1: ( 'strict' )
            {
             before(grammarAccess.getUseArgAccess().getStrictStrictKeyword_2_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4425:1: ( 'strict' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4426:1: 'strict'
            {
             before(grammarAccess.getUseArgAccess().getStrictStrictKeyword_2_0()); 
            match(input,96,FOLLOW_96_in_rule__UseArg__StrictAssignment_28859); 
             after(grammarAccess.getUseArgAccess().getStrictStrictKeyword_2_0()); 

            }

             after(grammarAccess.getUseArgAccess().getStrictStrictKeyword_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__StrictAssignment_2


    // $ANTLR start rule__UseArg__ArgsAssignment_4_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4441:1: rule__UseArg__ArgsAssignment_4_0 : ( ruleUseArgs ) ;
    public final void rule__UseArg__ArgsAssignment_4_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4445:1: ( ( ruleUseArgs ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4446:1: ( ruleUseArgs )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4446:1: ( ruleUseArgs )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4447:1: ruleUseArgs
            {
             before(grammarAccess.getUseArgAccess().getArgsUseArgsParserRuleCall_4_0_0()); 
            pushFollow(FOLLOW_ruleUseArgs_in_rule__UseArg__ArgsAssignment_4_08898);
            ruleUseArgs();
            _fsp--;

             after(grammarAccess.getUseArgAccess().getArgsUseArgsParserRuleCall_4_0_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__ArgsAssignment_4_0


    // $ANTLR start rule__UseArg__ArgsAssignment_4_1_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4456:1: rule__UseArg__ArgsAssignment_4_1_1 : ( ruleUseArgs ) ;
    public final void rule__UseArg__ArgsAssignment_4_1_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4460:1: ( ( ruleUseArgs ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4461:1: ( ruleUseArgs )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4461:1: ( ruleUseArgs )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4462:1: ruleUseArgs
            {
             before(grammarAccess.getUseArgAccess().getArgsUseArgsParserRuleCall_4_1_1_0()); 
            pushFollow(FOLLOW_ruleUseArgs_in_rule__UseArg__ArgsAssignment_4_1_18929);
            ruleUseArgs();
            _fsp--;

             after(grammarAccess.getUseArgAccess().getArgsUseArgsParserRuleCall_4_1_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArg__ArgsAssignment_4_1_1


    // $ANTLR start rule__UseArgs__NameAssignment_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4471:1: rule__UseArgs__NameAssignment_0 : ( ruleName ) ;
    public final void rule__UseArgs__NameAssignment_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4475:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4476:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4476:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4477:1: ruleName
            {
             before(grammarAccess.getUseArgsAccess().getNameNameParserRuleCall_0_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__UseArgs__NameAssignment_08960);
            ruleName();
            _fsp--;

             after(grammarAccess.getUseArgsAccess().getNameNameParserRuleCall_0_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArgs__NameAssignment_0


    // $ANTLR start rule__UseArgs__DefaultAssignment_1_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4486:1: rule__UseArgs__DefaultAssignment_1_1 : ( ruleExpression ) ;
    public final void rule__UseArgs__DefaultAssignment_1_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4490:1: ( ( ruleExpression ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4491:1: ( ruleExpression )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4491:1: ( ruleExpression )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4492:1: ruleExpression
            {
             before(grammarAccess.getUseArgsAccess().getDefaultExpressionParserRuleCall_1_1_0()); 
            pushFollow(FOLLOW_ruleExpression_in_rule__UseArgs__DefaultAssignment_1_18991);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getUseArgsAccess().getDefaultExpressionParserRuleCall_1_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__UseArgs__DefaultAssignment_1_1


    // $ANTLR start rule__Arguments__ArgumentsAssignment_1_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4501:1: rule__Arguments__ArgumentsAssignment_1_0 : ( ruleExpression ) ;
    public final void rule__Arguments__ArgumentsAssignment_1_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4505:1: ( ( ruleExpression ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4506:1: ( ruleExpression )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4506:1: ( ruleExpression )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4507:1: ruleExpression
            {
             before(grammarAccess.getArgumentsAccess().getArgumentsExpressionParserRuleCall_1_0_0()); 
            pushFollow(FOLLOW_ruleExpression_in_rule__Arguments__ArgumentsAssignment_1_09022);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getArgumentsAccess().getArgumentsExpressionParserRuleCall_1_0_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arguments__ArgumentsAssignment_1_0


    // $ANTLR start rule__Arguments__ArgumentsAssignment_1_1_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4516:1: rule__Arguments__ArgumentsAssignment_1_1_1 : ( ruleExpression ) ;
    public final void rule__Arguments__ArgumentsAssignment_1_1_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4520:1: ( ( ruleExpression ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4521:1: ( ruleExpression )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4521:1: ( ruleExpression )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4522:1: ruleExpression
            {
             before(grammarAccess.getArgumentsAccess().getArgumentsExpressionParserRuleCall_1_1_1_0()); 
            pushFollow(FOLLOW_ruleExpression_in_rule__Arguments__ArgumentsAssignment_1_1_19053);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getArgumentsAccess().getArgumentsExpressionParserRuleCall_1_1_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Arguments__ArgumentsAssignment_1_1_1


    // $ANTLR start rule__Raise__MayorAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4531:1: rule__Raise__MayorAssignment_2 : ( RULE_SYMBOL_NUM ) ;
    public final void rule__Raise__MayorAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4535:1: ( ( RULE_SYMBOL_NUM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4536:1: ( RULE_SYMBOL_NUM )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4536:1: ( RULE_SYMBOL_NUM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4537:1: RULE_SYMBOL_NUM
            {
             before(grammarAccess.getRaiseAccess().getMayorSYMBOL_NUMTerminalRuleCall_2_0()); 
            match(input,RULE_SYMBOL_NUM,FOLLOW_RULE_SYMBOL_NUM_in_rule__Raise__MayorAssignment_29084); 
             after(grammarAccess.getRaiseAccess().getMayorSYMBOL_NUMTerminalRuleCall_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Raise__MayorAssignment_2


    // $ANTLR start rule__Raise__MinorAssignment_3_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4546:1: rule__Raise__MinorAssignment_3_1 : ( RULE_SYMBOL_NUM ) ;
    public final void rule__Raise__MinorAssignment_3_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4550:1: ( ( RULE_SYMBOL_NUM ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4551:1: ( RULE_SYMBOL_NUM )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4551:1: ( RULE_SYMBOL_NUM )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4552:1: RULE_SYMBOL_NUM
            {
             before(grammarAccess.getRaiseAccess().getMinorSYMBOL_NUMTerminalRuleCall_3_1_0()); 
            match(input,RULE_SYMBOL_NUM,FOLLOW_RULE_SYMBOL_NUM_in_rule__Raise__MinorAssignment_3_19115); 
             after(grammarAccess.getRaiseAccess().getMinorSYMBOL_NUMTerminalRuleCall_3_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Raise__MinorAssignment_3_1


    // $ANTLR start rule__Expression__NameAssignment_0_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4561:1: rule__Expression__NameAssignment_0_1 : ( ruleName ) ;
    public final void rule__Expression__NameAssignment_0_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4565:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4566:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4566:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4567:1: ruleName
            {
             before(grammarAccess.getExpressionAccess().getNameNameParserRuleCall_0_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Expression__NameAssignment_0_19146);
            ruleName();
            _fsp--;

             after(grammarAccess.getExpressionAccess().getNameNameParserRuleCall_0_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Expression__NameAssignment_0_1


    // $ANTLR start rule__ArrayAccess__TargetAssignment_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4576:1: rule__ArrayAccess__TargetAssignment_0 : ( ruleName ) ;
    public final void rule__ArrayAccess__TargetAssignment_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4580:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4581:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4581:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4582:1: ruleName
            {
             before(grammarAccess.getArrayAccessAccess().getTargetNameParserRuleCall_0_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__ArrayAccess__TargetAssignment_09177);
            ruleName();
            _fsp--;

             after(grammarAccess.getArrayAccessAccess().getTargetNameParserRuleCall_0_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__ArrayAccess__TargetAssignment_0


    // $ANTLR start rule__ArrayAccess__IndexAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4591:1: rule__ArrayAccess__IndexAssignment_2 : ( ruleExpression ) ;
    public final void rule__ArrayAccess__IndexAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4595:1: ( ( ruleExpression ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4596:1: ( ruleExpression )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4596:1: ( ruleExpression )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4597:1: ruleExpression
            {
             before(grammarAccess.getArrayAccessAccess().getIndexExpressionParserRuleCall_2_0()); 
            pushFollow(FOLLOW_ruleExpression_in_rule__ArrayAccess__IndexAssignment_29208);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getArrayAccessAccess().getIndexExpressionParserRuleCall_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__ArrayAccess__IndexAssignment_2


    // $ANTLR start rule__FnCall__TargetAssignment_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4606:1: rule__FnCall__TargetAssignment_0 : ( ruleName ) ;
    public final void rule__FnCall__TargetAssignment_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4610:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4611:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4611:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4612:1: ruleName
            {
             before(grammarAccess.getFnCallAccess().getTargetNameParserRuleCall_0_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__FnCall__TargetAssignment_09239);
            ruleName();
            _fsp--;

             after(grammarAccess.getFnCallAccess().getTargetNameParserRuleCall_0_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__FnCall__TargetAssignment_0


    // $ANTLR start rule__FnCall__ArgsAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4621:1: rule__FnCall__ArgsAssignment_2 : ( ruleArguments ) ;
    public final void rule__FnCall__ArgsAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4625:1: ( ( ruleArguments ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4626:1: ( ruleArguments )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4626:1: ( ruleArguments )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4627:1: ruleArguments
            {
             before(grammarAccess.getFnCallAccess().getArgsArgumentsParserRuleCall_2_0()); 
            pushFollow(FOLLOW_ruleArguments_in_rule__FnCall__ArgsAssignment_29270);
            ruleArguments();
            _fsp--;

             after(grammarAccess.getFnCallAccess().getArgsArgumentsParserRuleCall_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__FnCall__ArgsAssignment_2


    // $ANTLR start rule__Message__TargetAssignment_0
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4636:1: rule__Message__TargetAssignment_0 : ( ruleName ) ;
    public final void rule__Message__TargetAssignment_0() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4640:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4641:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4641:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4642:1: ruleName
            {
             before(grammarAccess.getMessageAccess().getTargetNameParserRuleCall_0_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Message__TargetAssignment_09301);
            ruleName();
            _fsp--;

             after(grammarAccess.getMessageAccess().getTargetNameParserRuleCall_0_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__TargetAssignment_0


    // $ANTLR start rule__Message__DiscardAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4651:1: rule__Message__DiscardAssignment_2 : ( ( '~' ) ) ;
    public final void rule__Message__DiscardAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4655:1: ( ( ( '~' ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4656:1: ( ( '~' ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4656:1: ( ( '~' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4657:1: ( '~' )
            {
             before(grammarAccess.getMessageAccess().getDiscardTildeKeyword_2_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4658:1: ( '~' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4659:1: '~'
            {
             before(grammarAccess.getMessageAccess().getDiscardTildeKeyword_2_0()); 
            match(input,79,FOLLOW_79_in_rule__Message__DiscardAssignment_29337); 
             after(grammarAccess.getMessageAccess().getDiscardTildeKeyword_2_0()); 

            }

             after(grammarAccess.getMessageAccess().getDiscardTildeKeyword_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__DiscardAssignment_2


    // $ANTLR start rule__Message__MessageAssignment_3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4674:1: rule__Message__MessageAssignment_3 : ( ruleName ) ;
    public final void rule__Message__MessageAssignment_3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4678:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4679:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4679:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4680:1: ruleName
            {
             before(grammarAccess.getMessageAccess().getMessageNameParserRuleCall_3_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Message__MessageAssignment_39376);
            ruleName();
            _fsp--;

             after(grammarAccess.getMessageAccess().getMessageNameParserRuleCall_3_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__MessageAssignment_3


    // $ANTLR start rule__Message__ScopeAssignment_4_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4689:1: rule__Message__ScopeAssignment_4_1 : ( ruleName ) ;
    public final void rule__Message__ScopeAssignment_4_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4693:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4694:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4694:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4695:1: ruleName
            {
             before(grammarAccess.getMessageAccess().getScopeNameParserRuleCall_4_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Message__ScopeAssignment_4_19407);
            ruleName();
            _fsp--;

             after(grammarAccess.getMessageAccess().getScopeNameParserRuleCall_4_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__ScopeAssignment_4_1


    // $ANTLR start rule__Message__ArgsAssignment_5_0_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4704:1: rule__Message__ArgsAssignment_5_0_1 : ( ruleArguments ) ;
    public final void rule__Message__ArgsAssignment_5_0_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4708:1: ( ( ruleArguments ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4709:1: ( ruleArguments )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4709:1: ( ruleArguments )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4710:1: ruleArguments
            {
             before(grammarAccess.getMessageAccess().getArgsArgumentsParserRuleCall_5_0_1_0()); 
            pushFollow(FOLLOW_ruleArguments_in_rule__Message__ArgsAssignment_5_0_19438);
            ruleArguments();
            _fsp--;

             after(grammarAccess.getMessageAccess().getArgsArgumentsParserRuleCall_5_0_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__ArgsAssignment_5_0_1


    // $ANTLR start rule__Message__ValueAssignment_5_1_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4719:1: rule__Message__ValueAssignment_5_1_1 : ( ruleExpression ) ;
    public final void rule__Message__ValueAssignment_5_1_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4723:1: ( ( ruleExpression ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4724:1: ( ruleExpression )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4724:1: ( ruleExpression )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4725:1: ruleExpression
            {
             before(grammarAccess.getMessageAccess().getValueExpressionParserRuleCall_5_1_1_0()); 
            pushFollow(FOLLOW_ruleExpression_in_rule__Message__ValueAssignment_5_1_19469);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getMessageAccess().getValueExpressionParserRuleCall_5_1_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Message__ValueAssignment_5_1_1


    // $ANTLR start rule__Negate__ExpressionAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4734:1: rule__Negate__ExpressionAssignment_1 : ( ruleExpression ) ;
    public final void rule__Negate__ExpressionAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4738:1: ( ( ruleExpression ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4739:1: ( ruleExpression )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4739:1: ( ruleExpression )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4740:1: ruleExpression
            {
             before(grammarAccess.getNegateAccess().getExpressionExpressionParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleExpression_in_rule__Negate__ExpressionAssignment_19500);
            ruleExpression();
            _fsp--;

             after(grammarAccess.getNegateAccess().getExpressionExpressionParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Negate__ExpressionAssignment_1


    // $ANTLR start rule__Const__ValueAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4749:1: rule__Const__ValueAssignment_1 : ( ( rule__Const__ValueAlternatives_1_0 ) ) ;
    public final void rule__Const__ValueAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4753:1: ( ( ( rule__Const__ValueAlternatives_1_0 ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4754:1: ( ( rule__Const__ValueAlternatives_1_0 ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4754:1: ( ( rule__Const__ValueAlternatives_1_0 ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4755:1: ( rule__Const__ValueAlternatives_1_0 )
            {
             before(grammarAccess.getConstAccess().getValueAlternatives_1_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4756:1: ( rule__Const__ValueAlternatives_1_0 )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4756:2: rule__Const__ValueAlternatives_1_0
            {
            pushFollow(FOLLOW_rule__Const__ValueAlternatives_1_0_in_rule__Const__ValueAssignment_19531);
            rule__Const__ValueAlternatives_1_0();
            _fsp--;


            }

             after(grammarAccess.getConstAccess().getValueAlternatives_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Const__ValueAssignment_1


    // $ANTLR start rule__Class__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4765:1: rule__Class__NameAssignment_1 : ( ruleName ) ;
    public final void rule__Class__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4769:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4770:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4770:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4771:1: ruleName
            {
             before(grammarAccess.getClassAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Class__NameAssignment_19564);
            ruleName();
            _fsp--;

             after(grammarAccess.getClassAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__NameAssignment_1


    // $ANTLR start rule__Class__MetaAssignment_2_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4780:1: rule__Class__MetaAssignment_2_1 : ( ruleName ) ;
    public final void rule__Class__MetaAssignment_2_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4784:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4785:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4785:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4786:1: ruleName
            {
             before(grammarAccess.getClassAccess().getMetaNameParserRuleCall_2_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Class__MetaAssignment_2_19595);
            ruleName();
            _fsp--;

             after(grammarAccess.getClassAccess().getMetaNameParserRuleCall_2_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__MetaAssignment_2_1


    // $ANTLR start rule__Class__SubclassAssignment_3_0_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4795:1: rule__Class__SubclassAssignment_3_0_1 : ( ruleName ) ;
    public final void rule__Class__SubclassAssignment_3_0_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4799:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4800:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4800:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4801:1: ruleName
            {
             before(grammarAccess.getClassAccess().getSubclassNameParserRuleCall_3_0_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Class__SubclassAssignment_3_0_19626);
            ruleName();
            _fsp--;

             after(grammarAccess.getClassAccess().getSubclassNameParserRuleCall_3_0_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__SubclassAssignment_3_0_1


    // $ANTLR start rule__Class__MixinAssignment_3_1_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4810:1: rule__Class__MixinAssignment_3_1_1 : ( ruleName ) ;
    public final void rule__Class__MixinAssignment_3_1_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4814:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4815:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4815:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4816:1: ruleName
            {
             before(grammarAccess.getClassAccess().getMixinNameParserRuleCall_3_1_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Class__MixinAssignment_3_1_19657);
            ruleName();
            _fsp--;

             after(grammarAccess.getClassAccess().getMixinNameParserRuleCall_3_1_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__MixinAssignment_3_1_1


    // $ANTLR start rule__Class__VisibilityAssignment_4
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4825:1: rule__Class__VisibilityAssignment_4 : ( ruleVISIBILITY ) ;
    public final void rule__Class__VisibilityAssignment_4() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4829:1: ( ( ruleVISIBILITY ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4830:1: ( ruleVISIBILITY )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4830:1: ( ruleVISIBILITY )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4831:1: ruleVISIBILITY
            {
             before(grammarAccess.getClassAccess().getVisibilityVISIBILITYEnumRuleCall_4_0()); 
            pushFollow(FOLLOW_ruleVISIBILITY_in_rule__Class__VisibilityAssignment_49688);
            ruleVISIBILITY();
            _fsp--;

             after(grammarAccess.getClassAccess().getVisibilityVISIBILITYEnumRuleCall_4_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__VisibilityAssignment_4


    // $ANTLR start rule__Class__InheritAssignment_5_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4840:1: rule__Class__InheritAssignment_5_1 : ( ruleName ) ;
    public final void rule__Class__InheritAssignment_5_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4844:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4845:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4845:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4846:1: ruleName
            {
             before(grammarAccess.getClassAccess().getInheritNameParserRuleCall_5_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Class__InheritAssignment_5_19719);
            ruleName();
            _fsp--;

             after(grammarAccess.getClassAccess().getInheritNameParserRuleCall_5_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__InheritAssignment_5_1


    // $ANTLR start rule__Class__ContentAssignment_7
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4855:1: rule__Class__ContentAssignment_7 : ( ruleClassContent ) ;
    public final void rule__Class__ContentAssignment_7() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4859:1: ( ( ruleClassContent ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4860:1: ( ruleClassContent )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4860:1: ( ruleClassContent )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4861:1: ruleClassContent
            {
             before(grammarAccess.getClassAccess().getContentClassContentParserRuleCall_7_0()); 
            pushFollow(FOLLOW_ruleClassContent_in_rule__Class__ContentAssignment_79750);
            ruleClassContent();
            _fsp--;

             after(grammarAccess.getClassAccess().getContentClassContentParserRuleCall_7_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Class__ContentAssignment_7


    // $ANTLR start rule__Method__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4870:1: rule__Method__NameAssignment_1 : ( ruleName ) ;
    public final void rule__Method__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4874:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4875:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4875:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4876:1: ruleName
            {
             before(grammarAccess.getMethodAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Method__NameAssignment_19781);
            ruleName();
            _fsp--;

             after(grammarAccess.getMethodAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__NameAssignment_1


    // $ANTLR start rule__Method__VisibilityAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4885:1: rule__Method__VisibilityAssignment_2 : ( ruleVISIBILITY ) ;
    public final void rule__Method__VisibilityAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4889:1: ( ( ruleVISIBILITY ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4890:1: ( ruleVISIBILITY )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4890:1: ( ruleVISIBILITY )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4891:1: ruleVISIBILITY
            {
             before(grammarAccess.getMethodAccess().getVisibilityVISIBILITYEnumRuleCall_2_0()); 
            pushFollow(FOLLOW_ruleVISIBILITY_in_rule__Method__VisibilityAssignment_29812);
            ruleVISIBILITY();
            _fsp--;

             after(grammarAccess.getMethodAccess().getVisibilityVISIBILITYEnumRuleCall_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__VisibilityAssignment_2


    // $ANTLR start rule__Method__ClassAssignment_3
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4900:1: rule__Method__ClassAssignment_3 : ( ( 'class' ) ) ;
    public final void rule__Method__ClassAssignment_3() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4904:1: ( ( ( 'class' ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4905:1: ( ( 'class' ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4905:1: ( ( 'class' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4906:1: ( 'class' )
            {
             before(grammarAccess.getMethodAccess().getClassClassKeyword_3_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4907:1: ( 'class' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4908:1: 'class'
            {
             before(grammarAccess.getMethodAccess().getClassClassKeyword_3_0()); 
            match(input,97,FOLLOW_97_in_rule__Method__ClassAssignment_39848); 
             after(grammarAccess.getMethodAccess().getClassClassKeyword_3_0()); 

            }

             after(grammarAccess.getMethodAccess().getClassClassKeyword_3_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__ClassAssignment_3


    // $ANTLR start rule__Method__ExposeAssignment_5_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4923:1: rule__Method__ExposeAssignment_5_1 : ( ruleName ) ;
    public final void rule__Method__ExposeAssignment_5_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4927:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4928:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4928:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4929:1: ruleName
            {
             before(grammarAccess.getMethodAccess().getExposeNameParserRuleCall_5_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Method__ExposeAssignment_5_19887);
            ruleName();
            _fsp--;

             after(grammarAccess.getMethodAccess().getExposeNameParserRuleCall_5_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__ExposeAssignment_5_1


    // $ANTLR start rule__Method__ContentAssignment_6
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4938:1: rule__Method__ContentAssignment_6 : ( ruleInstruction ) ;
    public final void rule__Method__ContentAssignment_6() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4942:1: ( ( ruleInstruction ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4943:1: ( ruleInstruction )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4943:1: ( ruleInstruction )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4944:1: ruleInstruction
            {
             before(grammarAccess.getMethodAccess().getContentInstructionParserRuleCall_6_0()); 
            pushFollow(FOLLOW_ruleInstruction_in_rule__Method__ContentAssignment_69918);
            ruleInstruction();
            _fsp--;

             after(grammarAccess.getMethodAccess().getContentInstructionParserRuleCall_6_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Method__ContentAssignment_6


    // $ANTLR start rule__Attribute__VisibilityAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4953:1: rule__Attribute__VisibilityAssignment_1 : ( ruleVISIBILITY ) ;
    public final void rule__Attribute__VisibilityAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4957:1: ( ( ruleVISIBILITY ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4958:1: ( ruleVISIBILITY )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4958:1: ( ruleVISIBILITY )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4959:1: ruleVISIBILITY
            {
             before(grammarAccess.getAttributeAccess().getVisibilityVISIBILITYEnumRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleVISIBILITY_in_rule__Attribute__VisibilityAssignment_19949);
            ruleVISIBILITY();
            _fsp--;

             after(grammarAccess.getAttributeAccess().getVisibilityVISIBILITYEnumRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__VisibilityAssignment_1


    // $ANTLR start rule__Attribute__ClassAssignment_2
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4968:1: rule__Attribute__ClassAssignment_2 : ( ( 'class' ) ) ;
    public final void rule__Attribute__ClassAssignment_2() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4972:1: ( ( ( 'class' ) ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4973:1: ( ( 'class' ) )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4973:1: ( ( 'class' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4974:1: ( 'class' )
            {
             before(grammarAccess.getAttributeAccess().getClassClassKeyword_2_0()); 
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4975:1: ( 'class' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4976:1: 'class'
            {
             before(grammarAccess.getAttributeAccess().getClassClassKeyword_2_0()); 
            match(input,97,FOLLOW_97_in_rule__Attribute__ClassAssignment_29985); 
             after(grammarAccess.getAttributeAccess().getClassClassKeyword_2_0()); 

            }

             after(grammarAccess.getAttributeAccess().getClassClassKeyword_2_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__ClassAssignment_2


    // $ANTLR start rule__Attribute__ExposeAssignment_4_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4991:1: rule__Attribute__ExposeAssignment_4_1 : ( ruleName ) ;
    public final void rule__Attribute__ExposeAssignment_4_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4995:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4996:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4996:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:4997:1: ruleName
            {
             before(grammarAccess.getAttributeAccess().getExposeNameParserRuleCall_4_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Attribute__ExposeAssignment_4_110024);
            ruleName();
            _fsp--;

             after(grammarAccess.getAttributeAccess().getExposeNameParserRuleCall_4_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__ExposeAssignment_4_1


    // $ANTLR start rule__Attribute__ContentAssignment_5
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5006:1: rule__Attribute__ContentAssignment_5 : ( ruleInstruction ) ;
    public final void rule__Attribute__ContentAssignment_5() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5010:1: ( ( ruleInstruction ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5011:1: ( ruleInstruction )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5011:1: ( ruleInstruction )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5012:1: ruleInstruction
            {
             before(grammarAccess.getAttributeAccess().getContentInstructionParserRuleCall_5_0()); 
            pushFollow(FOLLOW_ruleInstruction_in_rule__Attribute__ContentAssignment_510055);
            ruleInstruction();
            _fsp--;

             after(grammarAccess.getAttributeAccess().getContentInstructionParserRuleCall_5_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Attribute__ContentAssignment_5


    // $ANTLR start rule__AttributeSet__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5021:1: rule__AttributeSet__NameAssignment_1 : ( ruleName ) ;
    public final void rule__AttributeSet__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5025:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5026:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5026:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5027:1: ruleName
            {
             before(grammarAccess.getAttributeSetAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__AttributeSet__NameAssignment_110086);
            ruleName();
            _fsp--;

             after(grammarAccess.getAttributeSetAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeSet__NameAssignment_1


    // $ANTLR start rule__AttributeGet__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5036:1: rule__AttributeGet__NameAssignment_1 : ( ruleName ) ;
    public final void rule__AttributeGet__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5040:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5041:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5041:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5042:1: ruleName
            {
             before(grammarAccess.getAttributeGetAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__AttributeGet__NameAssignment_110117);
            ruleName();
            _fsp--;

             after(grammarAccess.getAttributeGetAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeGet__NameAssignment_1


    // $ANTLR start rule__AttributeNormal__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5051:1: rule__AttributeNormal__NameAssignment_1 : ( ruleName ) ;
    public final void rule__AttributeNormal__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5055:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5056:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5056:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5057:1: ruleName
            {
             before(grammarAccess.getAttributeNormalAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__AttributeNormal__NameAssignment_110148);
            ruleName();
            _fsp--;

             after(grammarAccess.getAttributeNormalAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__AttributeNormal__NameAssignment_1


    // $ANTLR start rule__Constant__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5066:1: rule__Constant__NameAssignment_1 : ( ruleName ) ;
    public final void rule__Constant__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5070:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5071:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5071:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5072:1: ruleName
            {
             before(grammarAccess.getConstantAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Constant__NameAssignment_110179);
            ruleName();
            _fsp--;

             after(grammarAccess.getConstantAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Constant__NameAssignment_1


    // $ANTLR start rule__Routine__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5081:1: rule__Routine__NameAssignment_1 : ( ruleName ) ;
    public final void rule__Routine__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5085:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5086:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5086:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5087:1: ruleName
            {
             before(grammarAccess.getRoutineAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Routine__NameAssignment_110210);
            ruleName();
            _fsp--;

             after(grammarAccess.getRoutineAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Routine__NameAssignment_1


    // $ANTLR start rule__Requires__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5096:1: rule__Requires__NameAssignment_1 : ( ruleName ) ;
    public final void rule__Requires__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5100:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5101:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5101:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5102:1: ruleName
            {
             before(grammarAccess.getRequiresAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Requires__NameAssignment_110241);
            ruleName();
            _fsp--;

             after(grammarAccess.getRequiresAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Requires__NameAssignment_1


    // $ANTLR start rule__Options__NameAssignment_1
    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5111:1: rule__Options__NameAssignment_1 : ( ruleName ) ;
    public final void rule__Options__NameAssignment_1() throws RecognitionException {

        		int stackSize = keepStackSize();
            
        try {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5115:1: ( ( ruleName ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5116:1: ( ruleName )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5116:1: ( ruleName )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5117:1: ruleName
            {
             before(grammarAccess.getOptionsAccess().getNameNameParserRuleCall_1_0()); 
            pushFollow(FOLLOW_ruleName_in_rule__Options__NameAssignment_110272);
            ruleName();
            _fsp--;

             after(grammarAccess.getOptionsAccess().getNameNameParserRuleCall_1_0()); 

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {

            	restoreStackSize(stackSize);

        }
        return ;
    }
    // $ANTLR end rule__Options__NameAssignment_1


 

    public static final BitSet FOLLOW_ruleFile_in_entryRuleFile61 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleFile68 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__File__Group__0_in_ruleFile94 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleInstruction_in_entryRuleInstruction121 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleInstruction128 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Instruction__Group__0_in_ruleInstruction154 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAssignment_in_entryRuleAssignment181 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAssignment188 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Assignment__Group__0_in_ruleAssignment214 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleIf_in_entryRuleIf241 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleIf248 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__If__Group__0_in_ruleIf274 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleDo_in_entryRuleDo301 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleDo308 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Do__Group__0_in_ruleDo334 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleCall_in_entryRuleCall361 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleCall368 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Call__Group__0_in_ruleCall394 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArg_in_entryRuleArg421 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleArg428 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arg__Group__0_in_ruleArg454 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleUseArg_in_entryRuleUseArg481 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleUseArg488 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArg__Group__0_in_ruleUseArg514 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleUseArgs_in_entryRuleUseArgs541 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleUseArgs548 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArgs__Group__0_in_ruleUseArgs574 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArguments_in_entryRuleArguments601 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleArguments608 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arguments__Group__0_in_ruleArguments634 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleRaise_in_entryRuleRaise661 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleRaise668 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Raise__Group__0_in_ruleRaise694 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_entryRuleExpression721 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleExpression728 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Expression__Alternatives_in_ruleExpression754 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArrayAccess_in_entryRuleArrayAccess781 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleArrayAccess788 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__ArrayAccess__Group__0_in_ruleArrayAccess814 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rulefnCall_in_entryRulefnCall841 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRulefnCall848 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__FnCall__Group__0_in_rulefnCall874 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMessage_in_entryRuleMessage901 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleMessage908 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__Group__0_in_ruleMessage934 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleNegate_in_entryRuleNegate961 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleNegate968 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Negate__Group__0_in_ruleNegate994 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleConst_in_entryRuleConst1021 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleConst1028 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Const__Group__0_in_ruleConst1054 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleDirectives_in_entryRuleDirectives1081 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleDirectives1088 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Directives__Alternatives_in_ruleDirectives1114 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleClass_in_entryRuleClass1141 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleClass1148 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__Group__0_in_ruleClass1174 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleClassContent_in_entryRuleClassContent1201 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleClassContent1208 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__ClassContent__Alternatives_in_ruleClassContent1234 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMethod_in_entryRuleMethod1261 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleMethod1268 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Method__Group__0_in_ruleMethod1294 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttribute_in_entryRuleAttribute1321 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAttribute1328 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Attribute__Group__0_in_ruleAttribute1354 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeSet_in_entryRuleAttributeSet1381 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAttributeSet1388 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__AttributeSet__Group__0_in_ruleAttributeSet1414 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeGet_in_entryRuleAttributeGet1441 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAttributeGet1448 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__AttributeGet__Group__0_in_ruleAttributeGet1474 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeNormal_in_entryRuleAttributeNormal1501 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleAttributeNormal1508 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__AttributeNormal__Group__0_in_ruleAttributeNormal1534 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleConstant_in_entryRuleConstant1561 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleConstant1568 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Constant__Group__0_in_ruleConstant1594 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleRoutine_in_entryRuleRoutine1621 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleRoutine1628 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Routine__Group__0_in_ruleRoutine1654 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleRequires_in_entryRuleRequires1681 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleRequires1688 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Requires__Group__0_in_ruleRequires1714 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleOptions_in_entryRuleOptions1741 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleOptions1748 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Options__Group__0_in_ruleOptions1774 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_entryRuleName1801 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_entryRuleName1808 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Name__Alternatives_in_ruleName1834 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__VISIBILITY__Alternatives_in_ruleVISIBILITY1871 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAssignment_in_rule__Instruction__Alternatives_01906 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleDo_in_rule__Instruction__Alternatives_01923 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleIf_in_rule__Instruction__Alternatives_01940 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleCall_in_rule__Instruction__Alternatives_01957 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArg_in_rule__Instruction__Alternatives_01974 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleUseArg_in_rule__Instruction__Alternatives_01991 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleRaise_in_rule__Instruction__Alternatives_02008 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMessage_in_rule__Instruction__Alternatives_02025 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Expression__Group_0__0_in_rule__Expression__Alternatives2057 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rulefnCall_in_rule__Expression__Alternatives2075 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMessage_in_rule__Expression__Alternatives2092 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArrayAccess_in_rule__Expression__Alternatives2109 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleNegate_in_rule__Expression__Alternatives2126 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleConst_in_rule__Expression__Alternatives2143 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__Group_5_0__0_in_rule__Message__Alternatives_52175 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__Group_5_1__0_in_rule__Message__Alternatives_52193 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_SYMBOL_NUM_in_rule__Const__ValueAlternatives_1_02226 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_STRING_BOOL_in_rule__Const__ValueAlternatives_1_02243 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_STRING_HEX_in_rule__Const__ValueAlternatives_1_02260 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleClass_in_rule__Directives__Alternatives2292 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleOptions_in_rule__Directives__Alternatives2309 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleRoutine_in_rule__Directives__Alternatives2326 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__Group_3_0__0_in_rule__Class__Alternatives_32358 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__Group_3_1__0_in_rule__Class__Alternatives_32376 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMethod_in_rule__ClassContent__Alternatives2409 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttribute_in_rule__ClassContent__Alternatives2426 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleConstant_in_rule__ClassContent__Alternatives2443 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeSet_in_rule__Attribute__Alternatives_02475 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeGet_in_rule__Attribute__Alternatives_02492 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleAttributeNormal_in_rule__Attribute__Alternatives_02509 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_STRING_in_rule__Name__Alternatives2541 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_SYMBOL_VAR_in_rule__Name__Alternatives2558 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_60_in_rule__VISIBILITY__Alternatives2591 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_61_in_rule__VISIBILITY__Alternatives2612 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__File__Group__1_in_rule__File__Group__02659 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__File__Group__12688 = new BitSet(new long[]{0x8000000000000382L,0x00000000E08401B4L});
    public static final BitSet FOLLOW_rule__File__Group__2_in_rule__File__Group__12698 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__File__InstructionsAssignment_2_in_rule__File__Group__22726 = new BitSet(new long[]{0x8000000000000182L,0x00000000E08401B4L});
    public static final BitSet FOLLOW_rule__File__Group__3_in_rule__File__Group__22736 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__File__RequiresAssignment_3_in_rule__File__Group__32764 = new BitSet(new long[]{0x0000000000000002L,0x00000000E0840000L});
    public static final BitSet FOLLOW_rule__File__Group__4_in_rule__File__Group__32774 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__File__MethodsAssignment_4_in_rule__File__Group__42802 = new BitSet(new long[]{0x0000000000000002L,0x00000000A0840000L});
    public static final BitSet FOLLOW_rule__File__Group__5_in_rule__File__Group__42812 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__File__DirectivesAssignment_5_in_rule__File__Group__52840 = new BitSet(new long[]{0x0000000000000002L,0x00000000A0040000L});
    public static final BitSet FOLLOW_rule__Instruction__Alternatives_0_in_rule__Instruction__Group__02887 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Instruction__Group__1_in_rule__Instruction__Group__02896 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Instruction__Group__12927 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Instruction__Group__12940 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_rule__Assignment__TargetAssignment_0_in_rule__Assignment__Group__02981 = new BitSet(new long[]{0x4000000000000000L});
    public static final BitSet FOLLOW_rule__Assignment__Group__1_in_rule__Assignment__Group__02990 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_62_in_rule__Assignment__Group__13019 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000020000L});
    public static final BitSet FOLLOW_rule__Assignment__Group__2_in_rule__Assignment__Group__13029 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Assignment__ExpressionAssignment_2_in_rule__Assignment__Group__23057 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_63_in_rule__If__Group__03098 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000020000L});
    public static final BitSet FOLLOW_rule__If__Group__1_in_rule__If__Group__03108 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__If__ExpressionAssignment_1_in_rule__If__Group__13136 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000001L});
    public static final BitSet FOLLOW_rule__If__Group__2_in_rule__If__Group__13145 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_64_in_rule__If__Group__23174 = new BitSet(new long[]{0x8000000000000380L,0x00000000000001B4L});
    public static final BitSet FOLLOW_rule__If__Group__3_in_rule__If__Group__23184 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__If__Group__33213 = new BitSet(new long[]{0x8000000000000380L,0x00000000000001B4L});
    public static final BitSet FOLLOW_rule__If__Group__4_in_rule__If__Group__33223 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__If__TopInstrAssignment_4_in_rule__If__Group__43251 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000002L});
    public static final BitSet FOLLOW_rule__If__Group__5_in_rule__If__Group__43260 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__If__Group_5__0_in_rule__If__Group__53288 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_65_in_rule__If__Group_5__03336 = new BitSet(new long[]{0x8000000000000380L,0x00000000000001B4L});
    public static final BitSet FOLLOW_rule__If__Group_5__1_in_rule__If__Group_5__03346 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__If__Group_5__13375 = new BitSet(new long[]{0x8000000000000380L,0x00000000000001B4L});
    public static final BitSet FOLLOW_rule__If__Group_5__2_in_rule__If__Group_5__13385 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__If__BottomInstrAssignment_5_2_in_rule__If__Group_5__23413 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_66_in_rule__Do__Group__03454 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Do__Group__1_in_rule__Do__Group__03464 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Do__Group__13495 = new BitSet(new long[]{0x8000000000000380L,0x00000000000001B4L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Do__Group__13508 = new BitSet(new long[]{0x8000000000000380L,0x00000000000001B4L});
    public static final BitSet FOLLOW_rule__Do__Group__2_in_rule__Do__Group__13520 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Do__InstrsAssignment_2_in_rule__Do__Group__23550 = new BitSet(new long[]{0x8000000000000180L,0x00000000000001BCL});
    public static final BitSet FOLLOW_rule__Do__InstrsAssignment_2_in_rule__Do__Group__23562 = new BitSet(new long[]{0x8000000000000180L,0x00000000000001BCL});
    public static final BitSet FOLLOW_rule__Do__Group__3_in_rule__Do__Group__23574 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_67_in_rule__Do__Group__33603 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_68_in_rule__Call__Group__03647 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Call__Group__1_in_rule__Call__Group__03657 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Call__NameAssignment_1_in_rule__Call__Group__13685 = new BitSet(new long[]{0x00000000000001F2L,0x0000000000020000L});
    public static final BitSet FOLLOW_rule__Call__Group__2_in_rule__Call__Group__13694 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Call__ArgsAssignment_2_in_rule__Call__Group__23722 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arg__Group__1_in_rule__Arg__Group__03772 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_69_in_rule__Arg__Group__13801 = new BitSet(new long[]{0x0000000000000182L});
    public static final BitSet FOLLOW_rule__Arg__Group__2_in_rule__Arg__Group__13811 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arg__Group_2__0_in_rule__Arg__Group__23839 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arg__ArgsAssignment_2_0_in_rule__Arg__Group_2__03880 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000040L});
    public static final BitSet FOLLOW_rule__Arg__Group_2__1_in_rule__Arg__Group_2__03889 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arg__Group_2_1__0_in_rule__Arg__Group_2__13917 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000040L});
    public static final BitSet FOLLOW_70_in_rule__Arg__Group_2_1__03957 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Arg__Group_2_1__1_in_rule__Arg__Group_2_1__03967 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arg__ArgsAssignment_2_1_1_in_rule__Arg__Group_2_1__13995 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArg__Group__1_in_rule__UseArg__Group__04043 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_71_in_rule__UseArg__Group__14072 = new BitSet(new long[]{0x0000000000000000L,0x0000000100000020L});
    public static final BitSet FOLLOW_rule__UseArg__Group__2_in_rule__UseArg__Group__14082 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArg__StrictAssignment_2_in_rule__UseArg__Group__24110 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000020L});
    public static final BitSet FOLLOW_rule__UseArg__Group__3_in_rule__UseArg__Group__24120 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_69_in_rule__UseArg__Group__34149 = new BitSet(new long[]{0x0000000000000182L});
    public static final BitSet FOLLOW_rule__UseArg__Group__4_in_rule__UseArg__Group__34159 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArg__Group_4__0_in_rule__UseArg__Group__44187 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArg__ArgsAssignment_4_0_in_rule__UseArg__Group_4__04232 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000040L});
    public static final BitSet FOLLOW_rule__UseArg__Group_4__1_in_rule__UseArg__Group_4__04241 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArg__Group_4_1__0_in_rule__UseArg__Group_4__14269 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000040L});
    public static final BitSet FOLLOW_70_in_rule__UseArg__Group_4_1__04309 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__UseArg__Group_4_1__1_in_rule__UseArg__Group_4_1__04319 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArg__ArgsAssignment_4_1_1_in_rule__UseArg__Group_4_1__14347 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArgs__NameAssignment_0_in_rule__UseArgs__Group__04385 = new BitSet(new long[]{0x4000000000000002L});
    public static final BitSet FOLLOW_rule__UseArgs__Group__1_in_rule__UseArgs__Group__04394 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArgs__Group_1__0_in_rule__UseArgs__Group__14422 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_62_in_rule__UseArgs__Group_1__04462 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000020000L});
    public static final BitSet FOLLOW_rule__UseArgs__Group_1__1_in_rule__UseArgs__Group_1__04472 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__UseArgs__DefaultAssignment_1_1_in_rule__UseArgs__Group_1__14500 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arguments__Group__1_in_rule__Arguments__Group__04548 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arguments__Group_1__0_in_rule__Arguments__Group__14576 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arguments__ArgumentsAssignment_1_0_in_rule__Arguments__Group_1__04615 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000040L});
    public static final BitSet FOLLOW_rule__Arguments__Group_1__1_in_rule__Arguments__Group_1__04624 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arguments__Group_1_1__0_in_rule__Arguments__Group_1__14652 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000040L});
    public static final BitSet FOLLOW_70_in_rule__Arguments__Group_1_1__04692 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000020000L});
    public static final BitSet FOLLOW_rule__Arguments__Group_1_1__1_in_rule__Arguments__Group_1_1__04702 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Arguments__ArgumentsAssignment_1_1_1_in_rule__Arguments__Group_1_1__14730 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_72_in_rule__Raise__Group__04769 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Raise__Group__1_in_rule__Raise__Group__04779 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_73_in_rule__Raise__Group__14808 = new BitSet(new long[]{0x0000000000000010L});
    public static final BitSet FOLLOW_rule__Raise__Group__2_in_rule__Raise__Group__14818 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Raise__MayorAssignment_2_in_rule__Raise__Group__24846 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_rule__Raise__Group__3_in_rule__Raise__Group__24855 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Raise__Group_3__0_in_rule__Raise__Group__34883 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_74_in_rule__Raise__Group_3__04926 = new BitSet(new long[]{0x0000000000000010L});
    public static final BitSet FOLLOW_rule__Raise__Group_3__1_in_rule__Raise__Group_3__04936 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Raise__MinorAssignment_3_1_in_rule__Raise__Group_3__14964 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Expression__Group_0__1_in_rule__Expression__Group_0__05012 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Expression__NameAssignment_0_1_in_rule__Expression__Group_0__15040 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__ArrayAccess__TargetAssignment_0_in_rule__ArrayAccess__Group__05078 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000800L});
    public static final BitSet FOLLOW_rule__ArrayAccess__Group__1_in_rule__ArrayAccess__Group__05087 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_75_in_rule__ArrayAccess__Group__15116 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000020000L});
    public static final BitSet FOLLOW_rule__ArrayAccess__Group__2_in_rule__ArrayAccess__Group__15126 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__ArrayAccess__IndexAssignment_2_in_rule__ArrayAccess__Group__25154 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_rule__ArrayAccess__Group__3_in_rule__ArrayAccess__Group__25163 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_76_in_rule__ArrayAccess__Group__35192 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__FnCall__TargetAssignment_0_in_rule__FnCall__Group__05235 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_rule__FnCall__Group__1_in_rule__FnCall__Group__05244 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_77_in_rule__FnCall__Group__15273 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000024000L});
    public static final BitSet FOLLOW_rule__FnCall__Group__2_in_rule__FnCall__Group__15283 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__FnCall__ArgsAssignment_2_in_rule__FnCall__Group__25311 = new BitSet(new long[]{0x0000000000000000L,0x0000000000004000L});
    public static final BitSet FOLLOW_rule__FnCall__Group__3_in_rule__FnCall__Group__25320 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_78_in_rule__FnCall__Group__35349 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__TargetAssignment_0_in_rule__Message__Group__05392 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_rule__Message__Group__1_in_rule__Message__Group__05401 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_79_in_rule__Message__Group__15430 = new BitSet(new long[]{0x0000000000000180L,0x0000000000008000L});
    public static final BitSet FOLLOW_rule__Message__Group__2_in_rule__Message__Group__15440 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__DiscardAssignment_2_in_rule__Message__Group__25468 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Message__Group__3_in_rule__Message__Group__25478 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__MessageAssignment_3_in_rule__Message__Group__35506 = new BitSet(new long[]{0x4000000000000002L,0x0000000000012000L});
    public static final BitSet FOLLOW_rule__Message__Group__4_in_rule__Message__Group__35515 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__Group_4__0_in_rule__Message__Group__45543 = new BitSet(new long[]{0x4000000000000002L,0x0000000000002000L});
    public static final BitSet FOLLOW_rule__Message__Group__5_in_rule__Message__Group__45553 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__Alternatives_5_in_rule__Message__Group__55581 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_80_in_rule__Message__Group_4__05629 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Message__Group_4__1_in_rule__Message__Group_4__05639 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__ScopeAssignment_4_1_in_rule__Message__Group_4__15667 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_77_in_rule__Message__Group_5_0__05706 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000024000L});
    public static final BitSet FOLLOW_rule__Message__Group_5_0__1_in_rule__Message__Group_5_0__05716 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__ArgsAssignment_5_0_1_in_rule__Message__Group_5_0__15744 = new BitSet(new long[]{0x0000000000000000L,0x0000000000004000L});
    public static final BitSet FOLLOW_rule__Message__Group_5_0__2_in_rule__Message__Group_5_0__15753 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_78_in_rule__Message__Group_5_0__25782 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_62_in_rule__Message__Group_5_1__05824 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000020000L});
    public static final BitSet FOLLOW_rule__Message__Group_5_1__1_in_rule__Message__Group_5_1__05834 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Message__ValueAssignment_5_1_1_in_rule__Message__Group_5_1__15862 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_81_in_rule__Negate__Group__05901 = new BitSet(new long[]{0x00000000000001F0L,0x0000000000020000L});
    public static final BitSet FOLLOW_rule__Negate__Group__1_in_rule__Negate__Group__05911 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Negate__ExpressionAssignment_1_in_rule__Negate__Group__15939 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Const__Group__1_in_rule__Const__Group__05987 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Const__ValueAssignment_1_in_rule__Const__Group__16015 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_82_in_rule__Class__Group__06054 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Class__Group__1_in_rule__Class__Group__06064 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__NameAssignment_1_in_rule__Class__Group__16092 = new BitSet(new long[]{0x3000000000000200L,0x0000000000780000L});
    public static final BitSet FOLLOW_rule__Class__Group__2_in_rule__Class__Group__16101 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__Group_2__0_in_rule__Class__Group__26129 = new BitSet(new long[]{0x3000000000000200L,0x0000000000700000L});
    public static final BitSet FOLLOW_rule__Class__Group__3_in_rule__Class__Group__26139 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__Alternatives_3_in_rule__Class__Group__36167 = new BitSet(new long[]{0x3000000000000200L,0x0000000000400000L});
    public static final BitSet FOLLOW_rule__Class__Group__4_in_rule__Class__Group__36177 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__VisibilityAssignment_4_in_rule__Class__Group__46205 = new BitSet(new long[]{0x0000000000000200L,0x0000000000400000L});
    public static final BitSet FOLLOW_rule__Class__Group__5_in_rule__Class__Group__46215 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__Group_5__0_in_rule__Class__Group__56243 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Class__Group__6_in_rule__Class__Group__56253 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Class__Group__66284 = new BitSet(new long[]{0x0000000000000202L,0x0000000012800000L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Class__Group__66297 = new BitSet(new long[]{0x0000000000000202L,0x0000000012800000L});
    public static final BitSet FOLLOW_rule__Class__Group__7_in_rule__Class__Group__66309 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__ContentAssignment_7_in_rule__Class__Group__76337 = new BitSet(new long[]{0x0000000000000002L,0x0000000012800000L});
    public static final BitSet FOLLOW_83_in_rule__Class__Group_2__06389 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Class__Group_2__1_in_rule__Class__Group_2__06399 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__MetaAssignment_2_1_in_rule__Class__Group_2__16427 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_84_in_rule__Class__Group_3_0__06466 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Class__Group_3_0__1_in_rule__Class__Group_3_0__06476 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__SubclassAssignment_3_0_1_in_rule__Class__Group_3_0__16504 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_85_in_rule__Class__Group_3_1__06543 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Class__Group_3_1__1_in_rule__Class__Group_3_1__06553 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__MixinAssignment_3_1_1_in_rule__Class__Group_3_1__16581 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_86_in_rule__Class__Group_5__06620 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Class__Group_5__1_in_rule__Class__Group_5__06630 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Class__InheritAssignment_5_1_in_rule__Class__Group_5__16660 = new BitSet(new long[]{0x0000000000000182L});
    public static final BitSet FOLLOW_rule__Class__InheritAssignment_5_1_in_rule__Class__Group_5__16672 = new BitSet(new long[]{0x0000000000000182L});
    public static final BitSet FOLLOW_87_in_rule__Method__Group__06714 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Method__Group__1_in_rule__Method__Group__06724 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Method__NameAssignment_1_in_rule__Method__Group__16752 = new BitSet(new long[]{0x3000000000000200L,0x0000000200000000L});
    public static final BitSet FOLLOW_rule__Method__Group__2_in_rule__Method__Group__16761 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Method__VisibilityAssignment_2_in_rule__Method__Group__26789 = new BitSet(new long[]{0x0000000000000200L,0x0000000200000000L});
    public static final BitSet FOLLOW_rule__Method__Group__3_in_rule__Method__Group__26799 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Method__ClassAssignment_3_in_rule__Method__Group__36827 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Method__Group__4_in_rule__Method__Group__36837 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Method__Group__46868 = new BitSet(new long[]{0x8000000000000382L,0x00000000010001B4L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Method__Group__46881 = new BitSet(new long[]{0x8000000000000382L,0x00000000010001B4L});
    public static final BitSet FOLLOW_rule__Method__Group__5_in_rule__Method__Group__46893 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Method__Group_5__0_in_rule__Method__Group__56921 = new BitSet(new long[]{0x8000000000000182L,0x00000000000001B4L});
    public static final BitSet FOLLOW_rule__Method__Group__6_in_rule__Method__Group__56931 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Method__ContentAssignment_6_in_rule__Method__Group__66959 = new BitSet(new long[]{0x8000000000000182L,0x00000000000001B4L});
    public static final BitSet FOLLOW_88_in_rule__Method__Group_5__07009 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Method__Group_5__1_in_rule__Method__Group_5__07019 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Method__ExposeAssignment_5_1_in_rule__Method__Group_5__17049 = new BitSet(new long[]{0x0000000000000380L});
    public static final BitSet FOLLOW_rule__Method__ExposeAssignment_5_1_in_rule__Method__Group_5__17061 = new BitSet(new long[]{0x0000000000000380L});
    public static final BitSet FOLLOW_rule__Method__Group_5__2_in_rule__Method__Group_5__17073 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Method__Group_5__27104 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Method__Group_5__27117 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_rule__Attribute__Alternatives_0_in_rule__Attribute__Group__07160 = new BitSet(new long[]{0x3000000000000200L,0x0000000200000000L});
    public static final BitSet FOLLOW_rule__Attribute__Group__1_in_rule__Attribute__Group__07169 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Attribute__VisibilityAssignment_1_in_rule__Attribute__Group__17197 = new BitSet(new long[]{0x0000000000000200L,0x0000000200000000L});
    public static final BitSet FOLLOW_rule__Attribute__Group__2_in_rule__Attribute__Group__17207 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Attribute__ClassAssignment_2_in_rule__Attribute__Group__27235 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Attribute__Group__3_in_rule__Attribute__Group__27245 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Attribute__Group__37276 = new BitSet(new long[]{0x8000000000000382L,0x00000000010001B4L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Attribute__Group__37289 = new BitSet(new long[]{0x8000000000000382L,0x00000000010001B4L});
    public static final BitSet FOLLOW_rule__Attribute__Group__4_in_rule__Attribute__Group__37301 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Attribute__Group_4__0_in_rule__Attribute__Group__47329 = new BitSet(new long[]{0x8000000000000182L,0x00000000000001B4L});
    public static final BitSet FOLLOW_rule__Attribute__Group__5_in_rule__Attribute__Group__47339 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Attribute__ContentAssignment_5_in_rule__Attribute__Group__57367 = new BitSet(new long[]{0x8000000000000182L,0x00000000000001B4L});
    public static final BitSet FOLLOW_88_in_rule__Attribute__Group_4__07415 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Attribute__Group_4__1_in_rule__Attribute__Group_4__07425 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Attribute__ExposeAssignment_4_1_in_rule__Attribute__Group_4__17455 = new BitSet(new long[]{0x0000000000000380L});
    public static final BitSet FOLLOW_rule__Attribute__ExposeAssignment_4_1_in_rule__Attribute__Group_4__17467 = new BitSet(new long[]{0x0000000000000380L});
    public static final BitSet FOLLOW_rule__Attribute__Group_4__2_in_rule__Attribute__Group_4__17479 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Attribute__Group_4__27510 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Attribute__Group_4__27523 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_89_in_rule__AttributeSet__Group__07567 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__AttributeSet__Group__1_in_rule__AttributeSet__Group__07577 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__AttributeSet__NameAssignment_1_in_rule__AttributeSet__Group__17605 = new BitSet(new long[]{0x0000000000000000L,0x0000000004000000L});
    public static final BitSet FOLLOW_rule__AttributeSet__Group__2_in_rule__AttributeSet__Group__17614 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_90_in_rule__AttributeSet__Group__27643 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_89_in_rule__AttributeGet__Group__07685 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__AttributeGet__Group__1_in_rule__AttributeGet__Group__07695 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__AttributeGet__NameAssignment_1_in_rule__AttributeGet__Group__17723 = new BitSet(new long[]{0x0000000000000000L,0x0000000008000000L});
    public static final BitSet FOLLOW_rule__AttributeGet__Group__2_in_rule__AttributeGet__Group__17732 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_91_in_rule__AttributeGet__Group__27761 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_89_in_rule__AttributeNormal__Group__07803 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__AttributeNormal__Group__1_in_rule__AttributeNormal__Group__07813 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__AttributeNormal__NameAssignment_1_in_rule__AttributeNormal__Group__17841 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_92_in_rule__Constant__Group__07880 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Constant__Group__1_in_rule__Constant__Group__07890 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Constant__NameAssignment_1_in_rule__Constant__Group__17918 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Constant__Group__2_in_rule__Constant__Group__17927 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Constant__Group__27958 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Constant__Group__27971 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_93_in_rule__Routine__Group__08015 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Routine__Group__1_in_rule__Routine__Group__08025 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Routine__NameAssignment_1_in_rule__Routine__Group__18053 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Routine__Group__2_in_rule__Routine__Group__18062 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Routine__Group__28093 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Routine__Group__28106 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_94_in_rule__Requires__Group__08150 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Requires__Group__1_in_rule__Requires__Group__08160 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Requires__NameAssignment_1_in_rule__Requires__Group__18188 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Requires__Group__2_in_rule__Requires__Group__18197 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Requires__Group__28228 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Requires__Group__28241 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_95_in_rule__Options__Group__08285 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_rule__Options__Group__1_in_rule__Options__Group__08295 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Options__NameAssignment_1_in_rule__Options__Group__18323 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_rule__Options__Group__2_in_rule__Options__Group__18332 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Options__Group__28363 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_RULE_TERM_in_rule__Options__Group__28376 = new BitSet(new long[]{0x0000000000000202L});
    public static final BitSet FOLLOW_ruleInstruction_in_rule__File__InstructionsAssignment_28420 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleRequires_in_rule__File__RequiresAssignment_38451 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleMethod_in_rule__File__MethodsAssignment_48482 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleDirectives_in_rule__File__DirectivesAssignment_58513 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Assignment__TargetAssignment_08544 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_rule__Assignment__ExpressionAssignment_28575 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_rule__If__ExpressionAssignment_18606 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleInstruction_in_rule__If__TopInstrAssignment_48637 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleInstruction_in_rule__If__BottomInstrAssignment_5_28668 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleInstruction_in_rule__Do__InstrsAssignment_28699 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Call__NameAssignment_18730 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArguments_in_rule__Call__ArgsAssignment_28761 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Arg__ArgsAssignment_2_08792 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Arg__ArgsAssignment_2_1_18823 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_96_in_rule__UseArg__StrictAssignment_28859 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleUseArgs_in_rule__UseArg__ArgsAssignment_4_08898 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleUseArgs_in_rule__UseArg__ArgsAssignment_4_1_18929 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__UseArgs__NameAssignment_08960 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_rule__UseArgs__DefaultAssignment_1_18991 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_rule__Arguments__ArgumentsAssignment_1_09022 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_rule__Arguments__ArgumentsAssignment_1_1_19053 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_SYMBOL_NUM_in_rule__Raise__MayorAssignment_29084 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RULE_SYMBOL_NUM_in_rule__Raise__MinorAssignment_3_19115 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Expression__NameAssignment_0_19146 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__ArrayAccess__TargetAssignment_09177 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_rule__ArrayAccess__IndexAssignment_29208 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__FnCall__TargetAssignment_09239 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArguments_in_rule__FnCall__ArgsAssignment_29270 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Message__TargetAssignment_09301 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_79_in_rule__Message__DiscardAssignment_29337 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Message__MessageAssignment_39376 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Message__ScopeAssignment_4_19407 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleArguments_in_rule__Message__ArgsAssignment_5_0_19438 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_rule__Message__ValueAssignment_5_1_19469 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleExpression_in_rule__Negate__ExpressionAssignment_19500 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule__Const__ValueAlternatives_1_0_in_rule__Const__ValueAssignment_19531 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Class__NameAssignment_19564 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Class__MetaAssignment_2_19595 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Class__SubclassAssignment_3_0_19626 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Class__MixinAssignment_3_1_19657 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleVISIBILITY_in_rule__Class__VisibilityAssignment_49688 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Class__InheritAssignment_5_19719 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleClassContent_in_rule__Class__ContentAssignment_79750 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Method__NameAssignment_19781 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleVISIBILITY_in_rule__Method__VisibilityAssignment_29812 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_97_in_rule__Method__ClassAssignment_39848 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Method__ExposeAssignment_5_19887 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleInstruction_in_rule__Method__ContentAssignment_69918 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleVISIBILITY_in_rule__Attribute__VisibilityAssignment_19949 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_97_in_rule__Attribute__ClassAssignment_29985 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Attribute__ExposeAssignment_4_110024 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleInstruction_in_rule__Attribute__ContentAssignment_510055 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__AttributeSet__NameAssignment_110086 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__AttributeGet__NameAssignment_110117 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__AttributeNormal__NameAssignment_110148 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Constant__NameAssignment_110179 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Routine__NameAssignment_110210 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Requires__NameAssignment_110241 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ruleName_in_rule__Options__NameAssignment_110272 = new BitSet(new long[]{0x0000000000000002L});

}