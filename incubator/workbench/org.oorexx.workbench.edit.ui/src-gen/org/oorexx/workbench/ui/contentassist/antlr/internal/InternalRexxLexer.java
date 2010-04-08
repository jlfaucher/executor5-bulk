package org.oorexx.workbench.ui.contentassist.antlr.internal;

// Hack: Use our own Lexer superclass by means of import. 
// Currently there is no other way to specify the superclass for the lexer.
import org.eclipse.xtext.ui.editor.contentassist.antlr.internal.Lexer;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

public class InternalRexxLexer extends Lexer {
    public static final int RULE_OP_NGT=38;
    public static final int RULE_SYMBOL_VAR=8;
    public static final int RULE_SYMBOL_CLS=14;
    public static final int RULE_OP_GT=23;
    public static final int RULE_OP_PIPE=20;
    public static final int T60=60;
    public static final int T61=61;
    public static final int RULE_OP_SLASH2=34;
    public static final int T62=62;
    public static final int T63=63;
    public static final int T64=64;
    public static final int T65=65;
    public static final int T66=66;
    public static final int T67=67;
    public static final int RULE_OP_PERCENTEQ=54;
    public static final int T68=68;
    public static final int T69=69;
    public static final int RULE_OP_BSEQ=29;
    public static final int RULE_OP_STAREQ=52;
    public static final int RULE_OP_BSEQ2=33;
    public static final int RULE_OP_STAR2=37;
    public static final int RULE_OP_GT2=42;
    public static final int T70=70;
    public static final int T71=71;
    public static final int T72=72;
    public static final int T73=73;
    public static final int T74=74;
    public static final int RULE_OP_NGT2=48;
    public static final int T75=75;
    public static final int T76=76;
    public static final int T77=77;
    public static final int T78=78;
    public static final int T79=79;
    public static final int RULE_OP_LEQ=26;
    public static final int RULE_OP_SLAHS2EQ=55;
    public static final int RULE_OP_PIPE2=36;
    public static final int RULE_OP_EQ2=32;
    public static final int T80=80;
    public static final int T81=81;
    public static final int T82=82;
    public static final int T83=83;
    public static final int T84=84;
    public static final int T85=85;
    public static final int T86=86;
    public static final int RULE_OP_NEQ2=41;
    public static final int T87=87;
    public static final int T88=88;
    public static final int RULE_OP_MINUS=16;
    public static final int T89=89;
    public static final int RULE_OP_GT2EQ=44;
    public static final int RULE_OP_BSGT=27;
    public static final int RULE_OP_PLUSEQ=50;
    public static final int RULE_OP_SLASH=17;
    public static final int RULE_SYMBOL_NUM=4;
    public static final int T90=90;
    public static final int T91=91;
    public static final int T92=92;
    public static final int T93=93;
    public static final int T94=94;
    public static final int T95=95;
    public static final int T96=96;
    public static final int T97=97;
    public static final int RULE_OP_PIPEEQ=58;
    public static final int RULE_OP_GEQ=25;
    public static final int RULE_SL_COMMENT=12;
    public static final int RULE_OP_SLASHEQ=53;
    public static final int RULE_OP_GTLT=30;
    public static final int RULE_STRING_BOOL=5;
    public static final int RULE_OP_NLT=39;
    public static final int RULE_OP_LT=24;
    public static final int RULE_OP_ANDEQ=57;
    public static final int RULE_OP_STAR=19;
    public static final int RULE_STRING=7;
    public static final int RULE_OP_NEG=22;
    public static final int RULE_OP_PIPE2EQ=56;
    public static final int RULE_OP_AND2=35;
    public static final int Tokens=98;
    public static final int RULE_OP_BSLT2=45;
    public static final int RULE_OP_NEQ=40;
    public static final int RULE_OP_MINUSEQ=51;
    public static final int RULE_ML_COMMENT=11;
    public static final int RULE_OP_ET=21;
    public static final int RULE_TERM=9;
    public static final int RULE_OP_LTGT=31;
    public static final int RULE_OP_LT2EQ=49;
    public static final int EOF=-1;
    public static final int RULE_NL=13;
    public static final int RULE_OP_AND2EQ=59;
    public static final int RULE_OP_PERCENT=18;
    public static final int RULE_OP_LT2=43;
    public static final int RULE_STRING_HEX=6;
    public static final int RULE_OP_BSLT=28;
    public static final int RULE_OP_PLUS=15;
    public static final int RULE_OP_NLT2=46;
    public static final int RULE_OP_BSGT2=47;
    public static final int RULE_WS=10;
    public InternalRexxLexer() {;} 
    public InternalRexxLexer(CharStream input) {
        super(input);
    }
    public String getGrammarFileName() { return "../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g"; }

    // $ANTLR start T60
    public final void mT60() throws RecognitionException {
        try {
            int _type = T60;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:10:5: ( 'private' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:10:7: 'private'
            {
            match("private"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T60

    // $ANTLR start T61
    public final void mT61() throws RecognitionException {
        try {
            int _type = T61;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:11:5: ( 'public' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:11:7: 'public'
            {
            match("public"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T61

    // $ANTLR start T62
    public final void mT62() throws RecognitionException {
        try {
            int _type = T62;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:12:5: ( '=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:12:7: '='
            {
            match('='); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T62

    // $ANTLR start T63
    public final void mT63() throws RecognitionException {
        try {
            int _type = T63;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:13:5: ( 'if' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:13:7: 'if'
            {
            match("if"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T63

    // $ANTLR start T64
    public final void mT64() throws RecognitionException {
        try {
            int _type = T64;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:14:5: ( 'then' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:14:7: 'then'
            {
            match("then"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T64

    // $ANTLR start T65
    public final void mT65() throws RecognitionException {
        try {
            int _type = T65;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:15:5: ( 'else' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:15:7: 'else'
            {
            match("else"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T65

    // $ANTLR start T66
    public final void mT66() throws RecognitionException {
        try {
            int _type = T66;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:16:5: ( 'do' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:16:7: 'do'
            {
            match("do"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T66

    // $ANTLR start T67
    public final void mT67() throws RecognitionException {
        try {
            int _type = T67;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:17:5: ( 'end' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:17:7: 'end'
            {
            match("end"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T67

    // $ANTLR start T68
    public final void mT68() throws RecognitionException {
        try {
            int _type = T68;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:18:5: ( 'call' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:18:7: 'call'
            {
            match("call"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T68

    // $ANTLR start T69
    public final void mT69() throws RecognitionException {
        try {
            int _type = T69;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:19:5: ( 'arg' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:19:7: 'arg'
            {
            match("arg"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T69

    // $ANTLR start T70
    public final void mT70() throws RecognitionException {
        try {
            int _type = T70;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:20:5: ( ',' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:20:7: ','
            {
            match(','); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T70

    // $ANTLR start T71
    public final void mT71() throws RecognitionException {
        try {
            int _type = T71;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:21:5: ( 'use' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:21:7: 'use'
            {
            match("use"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T71

    // $ANTLR start T72
    public final void mT72() throws RecognitionException {
        try {
            int _type = T72;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:22:5: ( 'raise' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:22:7: 'raise'
            {
            match("raise"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T72

    // $ANTLR start T73
    public final void mT73() throws RecognitionException {
        try {
            int _type = T73;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:23:5: ( 'syntax' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:23:7: 'syntax'
            {
            match("syntax"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T73

    // $ANTLR start T74
    public final void mT74() throws RecognitionException {
        try {
            int _type = T74;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:24:5: ( '.' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:24:7: '.'
            {
            match('.'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T74

    // $ANTLR start T75
    public final void mT75() throws RecognitionException {
        try {
            int _type = T75;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:25:5: ( '[' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:25:7: '['
            {
            match('['); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T75

    // $ANTLR start T76
    public final void mT76() throws RecognitionException {
        try {
            int _type = T76;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:26:5: ( ']' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:26:7: ']'
            {
            match(']'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T76

    // $ANTLR start T77
    public final void mT77() throws RecognitionException {
        try {
            int _type = T77;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:27:5: ( '(' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:27:7: '('
            {
            match('('); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T77

    // $ANTLR start T78
    public final void mT78() throws RecognitionException {
        try {
            int _type = T78;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:28:5: ( ')' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:28:7: ')'
            {
            match(')'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T78

    // $ANTLR start T79
    public final void mT79() throws RecognitionException {
        try {
            int _type = T79;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:29:5: ( '~' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:29:7: '~'
            {
            match('~'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T79

    // $ANTLR start T80
    public final void mT80() throws RecognitionException {
        try {
            int _type = T80;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:30:5: ( ':' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:30:7: ':'
            {
            match(':'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T80

    // $ANTLR start T81
    public final void mT81() throws RecognitionException {
        try {
            int _type = T81;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:31:5: ( '\\\\' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:31:7: '\\\\'
            {
            match('\\'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T81

    // $ANTLR start T82
    public final void mT82() throws RecognitionException {
        try {
            int _type = T82;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:32:5: ( '::class' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:32:7: '::class'
            {
            match("::class"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T82

    // $ANTLR start T83
    public final void mT83() throws RecognitionException {
        try {
            int _type = T83;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:33:5: ( 'metaclass' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:33:7: 'metaclass'
            {
            match("metaclass"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T83

    // $ANTLR start T84
    public final void mT84() throws RecognitionException {
        try {
            int _type = T84;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:34:5: ( 'subclass' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:34:7: 'subclass'
            {
            match("subclass"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T84

    // $ANTLR start T85
    public final void mT85() throws RecognitionException {
        try {
            int _type = T85;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:35:5: ( 'mixinclass' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:35:7: 'mixinclass'
            {
            match("mixinclass"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T85

    // $ANTLR start T86
    public final void mT86() throws RecognitionException {
        try {
            int _type = T86;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:36:5: ( 'inherit' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:36:7: 'inherit'
            {
            match("inherit"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T86

    // $ANTLR start T87
    public final void mT87() throws RecognitionException {
        try {
            int _type = T87;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:37:5: ( '::method' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:37:7: '::method'
            {
            match("::method"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T87

    // $ANTLR start T88
    public final void mT88() throws RecognitionException {
        try {
            int _type = T88;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:38:5: ( 'expose' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:38:7: 'expose'
            {
            match("expose"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T88

    // $ANTLR start T89
    public final void mT89() throws RecognitionException {
        try {
            int _type = T89;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:39:5: ( '::attribute' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:39:7: '::attribute'
            {
            match("::attribute"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T89

    // $ANTLR start T90
    public final void mT90() throws RecognitionException {
        try {
            int _type = T90;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:40:5: ( 'set' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:40:7: 'set'
            {
            match("set"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T90

    // $ANTLR start T91
    public final void mT91() throws RecognitionException {
        try {
            int _type = T91;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:41:5: ( 'get' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:41:7: 'get'
            {
            match("get"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T91

    // $ANTLR start T92
    public final void mT92() throws RecognitionException {
        try {
            int _type = T92;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:42:5: ( '::constant' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:42:7: '::constant'
            {
            match("::constant"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T92

    // $ANTLR start T93
    public final void mT93() throws RecognitionException {
        try {
            int _type = T93;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:43:5: ( '::routine' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:43:7: '::routine'
            {
            match("::routine"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T93

    // $ANTLR start T94
    public final void mT94() throws RecognitionException {
        try {
            int _type = T94;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:44:5: ( '::requires' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:44:7: '::requires'
            {
            match("::requires"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T94

    // $ANTLR start T95
    public final void mT95() throws RecognitionException {
        try {
            int _type = T95;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:45:5: ( '::options' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:45:7: '::options'
            {
            match("::options"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T95

    // $ANTLR start T96
    public final void mT96() throws RecognitionException {
        try {
            int _type = T96;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:46:5: ( 'strict' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:46:7: 'strict'
            {
            match("strict"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T96

    // $ANTLR start T97
    public final void mT97() throws RecognitionException {
        try {
            int _type = T97;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:47:5: ( 'class' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:47:7: 'class'
            {
            match("class"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end T97

    // $ANTLR start RULE_WS
    public final void mRULE_WS() throws RecognitionException {
        try {
            int _type = RULE_WS;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5127:9: ( ( ' ' | '\\t' )+ )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5127:11: ( ' ' | '\\t' )+
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5127:11: ( ' ' | '\\t' )+
            int cnt1=0;
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0=='\t'||LA1_0==' ') ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:
            	    {
            	    if ( input.LA(1)=='\t'||input.LA(1)==' ' ) {
            	        input.consume();

            	    }
            	    else {
            	        MismatchedSetException mse =
            	            new MismatchedSetException(null,input);
            	        recover(mse);    throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    if ( cnt1 >= 1 ) break loop1;
                        EarlyExitException eee =
                            new EarlyExitException(1, input);
                        throw eee;
                }
                cnt1++;
            } while (true);


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_WS

    // $ANTLR start RULE_ML_COMMENT
    public final void mRULE_ML_COMMENT() throws RecognitionException {
        try {
            int _type = RULE_ML_COMMENT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5129:17: ( '/*' ( options {greedy=false; } : . )* '*/' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5129:19: '/*' ( options {greedy=false; } : . )* '*/'
            {
            match("/*"); 

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5129:24: ( options {greedy=false; } : . )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0=='*') ) {
                    int LA2_1 = input.LA(2);

                    if ( (LA2_1=='/') ) {
                        alt2=2;
                    }
                    else if ( ((LA2_1>='\u0000' && LA2_1<='.')||(LA2_1>='0' && LA2_1<='\uFFFE')) ) {
                        alt2=1;
                    }


                }
                else if ( ((LA2_0>='\u0000' && LA2_0<=')')||(LA2_0>='+' && LA2_0<='\uFFFE')) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5129:52: .
            	    {
            	    matchAny(); 

            	    }
            	    break;

            	default :
            	    break loop2;
                }
            } while (true);

            match("*/"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_ML_COMMENT

    // $ANTLR start RULE_SL_COMMENT
    public final void mRULE_SL_COMMENT() throws RecognitionException {
        try {
            int _type = RULE_SL_COMMENT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5131:17: ( '--' (~ ( ( '\\r' | '\\n' ) ) )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5131:19: '--' (~ ( ( '\\r' | '\\n' ) ) )*
            {
            match("--"); 

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5131:24: (~ ( ( '\\r' | '\\n' ) ) )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( ((LA3_0>='\u0000' && LA3_0<='\t')||(LA3_0>='\u000B' && LA3_0<='\f')||(LA3_0>='\u000E' && LA3_0<='\uFFFE')) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5131:24: ~ ( ( '\\r' | '\\n' ) )
            	    {
            	    if ( (input.LA(1)>='\u0000' && input.LA(1)<='\t')||(input.LA(1)>='\u000B' && input.LA(1)<='\f')||(input.LA(1)>='\u000E' && input.LA(1)<='\uFFFE') ) {
            	        input.consume();

            	    }
            	    else {
            	        MismatchedSetException mse =
            	            new MismatchedSetException(null,input);
            	        recover(mse);    throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    break loop3;
                }
            } while (true);


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_SL_COMMENT

    // $ANTLR start RULE_TERM
    public final void mRULE_TERM() throws RecognitionException {
        try {
            int _type = RULE_TERM;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5133:11: ( ( RULE_NL | ';' )+ )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5133:13: ( RULE_NL | ';' )+
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5133:13: ( RULE_NL | ';' )+
            int cnt4=0;
            loop4:
            do {
                int alt4=3;
                int LA4_0 = input.LA(1);

                if ( (LA4_0=='\n'||LA4_0=='\r') ) {
                    alt4=1;
                }
                else if ( (LA4_0==';') ) {
                    alt4=2;
                }


                switch (alt4) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5133:14: RULE_NL
            	    {
            	    mRULE_NL(); 

            	    }
            	    break;
            	case 2 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5133:22: ';'
            	    {
            	    match(';'); 

            	    }
            	    break;

            	default :
            	    if ( cnt4 >= 1 ) break loop4;
                        EarlyExitException eee =
                            new EarlyExitException(4, input);
                        throw eee;
                }
                cnt4++;
            } while (true);


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_TERM

    // $ANTLR start RULE_NL
    public final void mRULE_NL() throws RecognitionException {
        try {
            int _type = RULE_NL;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5135:9: ( ( '\\r' )? '\\n' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5135:11: ( '\\r' )? '\\n'
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5135:11: ( '\\r' )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0=='\r') ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5135:11: '\\r'
                    {
                    match('\r'); 

                    }
                    break;

            }

            match('\n'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_NL

    // $ANTLR start RULE_STRING
    public final void mRULE_STRING() throws RecognitionException {
        try {
            int _type = RULE_STRING;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:13: ( ( '\"' (~ ( '\"' ) )* '\"' | '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\'' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:15: ( '\"' (~ ( '\"' ) )* '\"' | '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\'' )
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:15: ( '\"' (~ ( '\"' ) )* '\"' | '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\'' )
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0=='\"') ) {
                alt8=1;
            }
            else if ( (LA8_0=='\'') ) {
                alt8=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("5137:15: ( '\"' (~ ( '\"' ) )* '\"' | '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\'' )", 8, 0, input);

                throw nvae;
            }
            switch (alt8) {
                case 1 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:16: '\"' (~ ( '\"' ) )* '\"'
                    {
                    match('\"'); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:20: (~ ( '\"' ) )*
                    loop6:
                    do {
                        int alt6=2;
                        int LA6_0 = input.LA(1);

                        if ( ((LA6_0>='\u0000' && LA6_0<='!')||(LA6_0>='#' && LA6_0<='\uFFFE')) ) {
                            alt6=1;
                        }


                        switch (alt6) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:20: ~ ( '\"' )
                    	    {
                    	    if ( (input.LA(1)>='\u0000' && input.LA(1)<='!')||(input.LA(1)>='#' && input.LA(1)<='\uFFFE') ) {
                    	        input.consume();

                    	    }
                    	    else {
                    	        MismatchedSetException mse =
                    	            new MismatchedSetException(null,input);
                    	        recover(mse);    throw mse;
                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop6;
                        }
                    } while (true);

                    match('\"'); 

                    }
                    break;
                case 2 :
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:32: '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\''
                    {
                    match('\''); 
                    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:37: (~ ( '\\'' ) | '\\'\\'' )*
                    loop7:
                    do {
                        int alt7=3;
                        int LA7_0 = input.LA(1);

                        if ( (LA7_0=='\'') ) {
                            int LA7_1 = input.LA(2);

                            if ( (LA7_1=='\'') ) {
                                alt7=2;
                            }


                        }
                        else if ( ((LA7_0>='\u0000' && LA7_0<='&')||(LA7_0>='(' && LA7_0<='\uFFFE')) ) {
                            alt7=1;
                        }


                        switch (alt7) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:38: ~ ( '\\'' )
                    	    {
                    	    if ( (input.LA(1)>='\u0000' && input.LA(1)<='&')||(input.LA(1)>='(' && input.LA(1)<='\uFFFE') ) {
                    	        input.consume();

                    	    }
                    	    else {
                    	        MismatchedSetException mse =
                    	            new MismatchedSetException(null,input);
                    	        recover(mse);    throw mse;
                    	    }


                    	    }
                    	    break;
                    	case 2 :
                    	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5137:46: '\\'\\''
                    	    {
                    	    match("\'\'"); 


                    	    }
                    	    break;

                    	default :
                    	    break loop7;
                        }
                    } while (true);

                    match('\''); 

                    }
                    break;

            }


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_STRING

    // $ANTLR start RULE_STRING_HEX
    public final void mRULE_STRING_HEX() throws RecognitionException {
        try {
            int _type = RULE_STRING_HEX;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5139:17: ( RULE_STRING ( 'X' | 'x' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5139:19: RULE_STRING ( 'X' | 'x' )
            {
            mRULE_STRING(); 
            if ( input.LA(1)=='X'||input.LA(1)=='x' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_STRING_HEX

    // $ANTLR start RULE_STRING_BOOL
    public final void mRULE_STRING_BOOL() throws RecognitionException {
        try {
            int _type = RULE_STRING_BOOL;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5141:18: ( RULE_STRING ( 'B' | 'b' ) )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5141:20: RULE_STRING ( 'B' | 'b' )
            {
            mRULE_STRING(); 
            if ( input.LA(1)=='B'||input.LA(1)=='b' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_STRING_BOOL

    // $ANTLR start RULE_SYMBOL_VAR
    public final void mRULE_SYMBOL_VAR() throws RecognitionException {
        try {
            int _type = RULE_SYMBOL_VAR;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5143:17: ( ( 'A' .. 'Z' | 'a' .. 'z' | '_' | '?' | '!' ) ( 'A' .. 'Z' | 'a' .. 'z' | '0' .. '9' | '_' | '.' | '!' | '?' )* )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5143:19: ( 'A' .. 'Z' | 'a' .. 'z' | '_' | '?' | '!' ) ( 'A' .. 'Z' | 'a' .. 'z' | '0' .. '9' | '_' | '.' | '!' | '?' )*
            {
            if ( input.LA(1)=='!'||input.LA(1)=='?'||(input.LA(1)>='A' && input.LA(1)<='Z')||input.LA(1)=='_'||(input.LA(1)>='a' && input.LA(1)<='z') ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5143:51: ( 'A' .. 'Z' | 'a' .. 'z' | '0' .. '9' | '_' | '.' | '!' | '?' )*
            loop9:
            do {
                int alt9=2;
                int LA9_0 = input.LA(1);

                if ( (LA9_0=='!'||LA9_0=='.'||(LA9_0>='0' && LA9_0<='9')||LA9_0=='?'||(LA9_0>='A' && LA9_0<='Z')||LA9_0=='_'||(LA9_0>='a' && LA9_0<='z')) ) {
                    alt9=1;
                }


                switch (alt9) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:
            	    {
            	    if ( input.LA(1)=='!'||input.LA(1)=='.'||(input.LA(1)>='0' && input.LA(1)<='9')||input.LA(1)=='?'||(input.LA(1)>='A' && input.LA(1)<='Z')||input.LA(1)=='_'||(input.LA(1)>='a' && input.LA(1)<='z') ) {
            	        input.consume();

            	    }
            	    else {
            	        MismatchedSetException mse =
            	            new MismatchedSetException(null,input);
            	        recover(mse);    throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    break loop9;
                }
            } while (true);


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_SYMBOL_VAR

    // $ANTLR start RULE_SYMBOL_CLS
    public final void mRULE_SYMBOL_CLS() throws RecognitionException {
        try {
            int _type = RULE_SYMBOL_CLS;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5145:17: ( '.' RULE_SYMBOL_VAR )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5145:19: '.' RULE_SYMBOL_VAR
            {
            match('.'); 
            mRULE_SYMBOL_VAR(); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_SYMBOL_CLS

    // $ANTLR start RULE_SYMBOL_NUM
    public final void mRULE_SYMBOL_NUM() throws RecognitionException {
        try {
            int _type = RULE_SYMBOL_NUM;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5147:17: ( ( '0' .. '9' )+ )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5147:19: ( '0' .. '9' )+
            {
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5147:19: ( '0' .. '9' )+
            int cnt10=0;
            loop10:
            do {
                int alt10=2;
                int LA10_0 = input.LA(1);

                if ( ((LA10_0>='0' && LA10_0<='9')) ) {
                    alt10=1;
                }


                switch (alt10) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5147:20: '0' .. '9'
            	    {
            	    matchRange('0','9'); 

            	    }
            	    break;

            	default :
            	    if ( cnt10 >= 1 ) break loop10;
                        EarlyExitException eee =
                            new EarlyExitException(10, input);
                        throw eee;
                }
                cnt10++;
            } while (true);


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_SYMBOL_NUM

    // $ANTLR start RULE_OP_PLUS
    public final void mRULE_OP_PLUS() throws RecognitionException {
        try {
            int _type = RULE_OP_PLUS;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5149:14: ( '+' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5149:16: '+'
            {
            match('+'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_PLUS

    // $ANTLR start RULE_OP_MINUS
    public final void mRULE_OP_MINUS() throws RecognitionException {
        try {
            int _type = RULE_OP_MINUS;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5151:15: ( '-' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5151:17: '-'
            {
            match('-'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_MINUS

    // $ANTLR start RULE_OP_SLASH
    public final void mRULE_OP_SLASH() throws RecognitionException {
        try {
            int _type = RULE_OP_SLASH;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5153:15: ( '/' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5153:17: '/'
            {
            match('/'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_SLASH

    // $ANTLR start RULE_OP_PERCENT
    public final void mRULE_OP_PERCENT() throws RecognitionException {
        try {
            int _type = RULE_OP_PERCENT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5155:17: ( '%' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5155:19: '%'
            {
            match('%'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_PERCENT

    // $ANTLR start RULE_OP_STAR
    public final void mRULE_OP_STAR() throws RecognitionException {
        try {
            int _type = RULE_OP_STAR;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5157:14: ( '*' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5157:16: '*'
            {
            match('*'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_STAR

    // $ANTLR start RULE_OP_PIPE
    public final void mRULE_OP_PIPE() throws RecognitionException {
        try {
            int _type = RULE_OP_PIPE;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5159:14: ( '|' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5159:16: '|'
            {
            match('|'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_PIPE

    // $ANTLR start RULE_OP_ET
    public final void mRULE_OP_ET() throws RecognitionException {
        try {
            int _type = RULE_OP_ET;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5161:12: ( '&' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5161:14: '&'
            {
            match('&'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_ET

    // $ANTLR start RULE_OP_NEG
    public final void mRULE_OP_NEG() throws RecognitionException {
        try {
            int _type = RULE_OP_NEG;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5163:13: ( '\\u00AC' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5163:15: '\\u00AC'
            {
            match('\u00AC'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_NEG

    // $ANTLR start RULE_OP_GT
    public final void mRULE_OP_GT() throws RecognitionException {
        try {
            int _type = RULE_OP_GT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5165:12: ( '>' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5165:14: '>'
            {
            match('>'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_GT

    // $ANTLR start RULE_OP_LT
    public final void mRULE_OP_LT() throws RecognitionException {
        try {
            int _type = RULE_OP_LT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5167:12: ( '<' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5167:14: '<'
            {
            match('<'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_LT

    // $ANTLR start RULE_OP_GEQ
    public final void mRULE_OP_GEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_GEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5169:13: ( '>=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5169:15: '>='
            {
            match(">="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_GEQ

    // $ANTLR start RULE_OP_LEQ
    public final void mRULE_OP_LEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_LEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5171:13: ( '<=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5171:15: '<='
            {
            match("<="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_LEQ

    // $ANTLR start RULE_OP_BSGT
    public final void mRULE_OP_BSGT() throws RecognitionException {
        try {
            int _type = RULE_OP_BSGT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5173:14: ( '\\\\>' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5173:16: '\\\\>'
            {
            match("\\>"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_BSGT

    // $ANTLR start RULE_OP_BSLT
    public final void mRULE_OP_BSLT() throws RecognitionException {
        try {
            int _type = RULE_OP_BSLT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5175:14: ( '\\\\<' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5175:16: '\\\\<'
            {
            match("\\<"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_BSLT

    // $ANTLR start RULE_OP_BSEQ
    public final void mRULE_OP_BSEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_BSEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5177:14: ( '\\\\=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5177:16: '\\\\='
            {
            match("\\="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_BSEQ

    // $ANTLR start RULE_OP_GTLT
    public final void mRULE_OP_GTLT() throws RecognitionException {
        try {
            int _type = RULE_OP_GTLT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5179:14: ( '><' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5179:16: '><'
            {
            match("><"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_GTLT

    // $ANTLR start RULE_OP_LTGT
    public final void mRULE_OP_LTGT() throws RecognitionException {
        try {
            int _type = RULE_OP_LTGT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5181:14: ( '<>' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5181:16: '<>'
            {
            match("<>"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_LTGT

    // $ANTLR start RULE_OP_EQ2
    public final void mRULE_OP_EQ2() throws RecognitionException {
        try {
            int _type = RULE_OP_EQ2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5183:13: ( '==' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5183:15: '=='
            {
            match("=="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_EQ2

    // $ANTLR start RULE_OP_BSEQ2
    public final void mRULE_OP_BSEQ2() throws RecognitionException {
        try {
            int _type = RULE_OP_BSEQ2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5185:15: ( '\\\\==' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5185:17: '\\\\=='
            {
            match("\\=="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_BSEQ2

    // $ANTLR start RULE_OP_SLASH2
    public final void mRULE_OP_SLASH2() throws RecognitionException {
        try {
            int _type = RULE_OP_SLASH2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5187:16: ( '//' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5187:18: '//'
            {
            match("//"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_SLASH2

    // $ANTLR start RULE_OP_AND2
    public final void mRULE_OP_AND2() throws RecognitionException {
        try {
            int _type = RULE_OP_AND2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5189:14: ( '&&' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5189:16: '&&'
            {
            match("&&"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_AND2

    // $ANTLR start RULE_OP_PIPE2
    public final void mRULE_OP_PIPE2() throws RecognitionException {
        try {
            int _type = RULE_OP_PIPE2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5191:15: ( '||' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5191:17: '||'
            {
            match("||"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_PIPE2

    // $ANTLR start RULE_OP_STAR2
    public final void mRULE_OP_STAR2() throws RecognitionException {
        try {
            int _type = RULE_OP_STAR2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5193:15: ( '**' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5193:17: '**'
            {
            match("**"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_STAR2

    // $ANTLR start RULE_OP_NGT
    public final void mRULE_OP_NGT() throws RecognitionException {
        try {
            int _type = RULE_OP_NGT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5195:13: ( '\\u00AC>' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5195:15: '\\u00AC>'
            {
            match("\u00AC>"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_NGT

    // $ANTLR start RULE_OP_NLT
    public final void mRULE_OP_NLT() throws RecognitionException {
        try {
            int _type = RULE_OP_NLT;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5197:13: ( '\\u00AC<' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5197:15: '\\u00AC<'
            {
            match("\u00AC<"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_NLT

    // $ANTLR start RULE_OP_NEQ
    public final void mRULE_OP_NEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_NEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5199:13: ( '\\u00AC=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5199:15: '\\u00AC='
            {
            match("\u00AC="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_NEQ

    // $ANTLR start RULE_OP_NEQ2
    public final void mRULE_OP_NEQ2() throws RecognitionException {
        try {
            int _type = RULE_OP_NEQ2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5201:14: ( '\\u00AC==' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5201:16: '\\u00AC=='
            {
            match("\u00AC=="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_NEQ2

    // $ANTLR start RULE_OP_GT2
    public final void mRULE_OP_GT2() throws RecognitionException {
        try {
            int _type = RULE_OP_GT2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5203:13: ( '>>' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5203:15: '>>'
            {
            match(">>"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_GT2

    // $ANTLR start RULE_OP_LT2
    public final void mRULE_OP_LT2() throws RecognitionException {
        try {
            int _type = RULE_OP_LT2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5205:13: ( '<<' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5205:15: '<<'
            {
            match("<<"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_LT2

    // $ANTLR start RULE_OP_GT2EQ
    public final void mRULE_OP_GT2EQ() throws RecognitionException {
        try {
            int _type = RULE_OP_GT2EQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5207:15: ( '>>=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5207:17: '>>='
            {
            match(">>="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_GT2EQ

    // $ANTLR start RULE_OP_BSLT2
    public final void mRULE_OP_BSLT2() throws RecognitionException {
        try {
            int _type = RULE_OP_BSLT2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5209:15: ( '\\\\<<' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5209:17: '\\\\<<'
            {
            match("\\<<"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_BSLT2

    // $ANTLR start RULE_OP_NLT2
    public final void mRULE_OP_NLT2() throws RecognitionException {
        try {
            int _type = RULE_OP_NLT2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5211:14: ( '\\u00AC<<' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5211:16: '\\u00AC<<'
            {
            match("\u00AC<<"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_NLT2

    // $ANTLR start RULE_OP_BSGT2
    public final void mRULE_OP_BSGT2() throws RecognitionException {
        try {
            int _type = RULE_OP_BSGT2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5213:15: ( '\\\\>>' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5213:17: '\\\\>>'
            {
            match("\\>>"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_BSGT2

    // $ANTLR start RULE_OP_NGT2
    public final void mRULE_OP_NGT2() throws RecognitionException {
        try {
            int _type = RULE_OP_NGT2;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5215:14: ( '\\u00AC>>' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5215:16: '\\u00AC>>'
            {
            match("\u00AC>>"); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_NGT2

    // $ANTLR start RULE_OP_LT2EQ
    public final void mRULE_OP_LT2EQ() throws RecognitionException {
        try {
            int _type = RULE_OP_LT2EQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5217:15: ( '<<=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5217:17: '<<='
            {
            match("<<="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_LT2EQ

    // $ANTLR start RULE_OP_PLUSEQ
    public final void mRULE_OP_PLUSEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_PLUSEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5219:16: ( '+=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5219:18: '+='
            {
            match("+="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_PLUSEQ

    // $ANTLR start RULE_OP_MINUSEQ
    public final void mRULE_OP_MINUSEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_MINUSEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5221:17: ( '-=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5221:19: '-='
            {
            match("-="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_MINUSEQ

    // $ANTLR start RULE_OP_STAREQ
    public final void mRULE_OP_STAREQ() throws RecognitionException {
        try {
            int _type = RULE_OP_STAREQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5223:16: ( '*=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5223:18: '*='
            {
            match("*="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_STAREQ

    // $ANTLR start RULE_OP_SLASHEQ
    public final void mRULE_OP_SLASHEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_SLASHEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5225:17: ( '/=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5225:19: '/='
            {
            match("/="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_SLASHEQ

    // $ANTLR start RULE_OP_PERCENTEQ
    public final void mRULE_OP_PERCENTEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_PERCENTEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5227:19: ( '%=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5227:21: '%='
            {
            match("%="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_PERCENTEQ

    // $ANTLR start RULE_OP_SLAHS2EQ
    public final void mRULE_OP_SLAHS2EQ() throws RecognitionException {
        try {
            int _type = RULE_OP_SLAHS2EQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5229:18: ( '//=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5229:20: '//='
            {
            match("//="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_SLAHS2EQ

    // $ANTLR start RULE_OP_PIPE2EQ
    public final void mRULE_OP_PIPE2EQ() throws RecognitionException {
        try {
            int _type = RULE_OP_PIPE2EQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5231:17: ( '||=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5231:19: '||='
            {
            match("||="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_PIPE2EQ

    // $ANTLR start RULE_OP_ANDEQ
    public final void mRULE_OP_ANDEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_ANDEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5233:15: ( '&=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5233:17: '&='
            {
            match("&="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_ANDEQ

    // $ANTLR start RULE_OP_PIPEEQ
    public final void mRULE_OP_PIPEEQ() throws RecognitionException {
        try {
            int _type = RULE_OP_PIPEEQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5235:16: ( '|=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5235:18: '|='
            {
            match("|="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_PIPEEQ

    // $ANTLR start RULE_OP_AND2EQ
    public final void mRULE_OP_AND2EQ() throws RecognitionException {
        try {
            int _type = RULE_OP_AND2EQ;
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5237:16: ( '&&=' )
            // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:5237:18: '&&='
            {
            match("&&="); 


            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end RULE_OP_AND2EQ

    public void mTokens() throws RecognitionException {
        // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:8: ( T60 | T61 | T62 | T63 | T64 | T65 | T66 | T67 | T68 | T69 | T70 | T71 | T72 | T73 | T74 | T75 | T76 | T77 | T78 | T79 | T80 | T81 | T82 | T83 | T84 | T85 | T86 | T87 | T88 | T89 | T90 | T91 | T92 | T93 | T94 | T95 | T96 | T97 | RULE_WS | RULE_ML_COMMENT | RULE_SL_COMMENT | RULE_TERM | RULE_NL | RULE_STRING | RULE_STRING_HEX | RULE_STRING_BOOL | RULE_SYMBOL_VAR | RULE_SYMBOL_CLS | RULE_SYMBOL_NUM | RULE_OP_PLUS | RULE_OP_MINUS | RULE_OP_SLASH | RULE_OP_PERCENT | RULE_OP_STAR | RULE_OP_PIPE | RULE_OP_ET | RULE_OP_NEG | RULE_OP_GT | RULE_OP_LT | RULE_OP_GEQ | RULE_OP_LEQ | RULE_OP_BSGT | RULE_OP_BSLT | RULE_OP_BSEQ | RULE_OP_GTLT | RULE_OP_LTGT | RULE_OP_EQ2 | RULE_OP_BSEQ2 | RULE_OP_SLASH2 | RULE_OP_AND2 | RULE_OP_PIPE2 | RULE_OP_STAR2 | RULE_OP_NGT | RULE_OP_NLT | RULE_OP_NEQ | RULE_OP_NEQ2 | RULE_OP_GT2 | RULE_OP_LT2 | RULE_OP_GT2EQ | RULE_OP_BSLT2 | RULE_OP_NLT2 | RULE_OP_BSGT2 | RULE_OP_NGT2 | RULE_OP_LT2EQ | RULE_OP_PLUSEQ | RULE_OP_MINUSEQ | RULE_OP_STAREQ | RULE_OP_SLASHEQ | RULE_OP_PERCENTEQ | RULE_OP_SLAHS2EQ | RULE_OP_PIPE2EQ | RULE_OP_ANDEQ | RULE_OP_PIPEEQ | RULE_OP_AND2EQ )
        int alt11=94;
        alt11 = dfa11.predict(input);
        switch (alt11) {
            case 1 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:10: T60
                {
                mT60(); 

                }
                break;
            case 2 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:14: T61
                {
                mT61(); 

                }
                break;
            case 3 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:18: T62
                {
                mT62(); 

                }
                break;
            case 4 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:22: T63
                {
                mT63(); 

                }
                break;
            case 5 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:26: T64
                {
                mT64(); 

                }
                break;
            case 6 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:30: T65
                {
                mT65(); 

                }
                break;
            case 7 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:34: T66
                {
                mT66(); 

                }
                break;
            case 8 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:38: T67
                {
                mT67(); 

                }
                break;
            case 9 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:42: T68
                {
                mT68(); 

                }
                break;
            case 10 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:46: T69
                {
                mT69(); 

                }
                break;
            case 11 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:50: T70
                {
                mT70(); 

                }
                break;
            case 12 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:54: T71
                {
                mT71(); 

                }
                break;
            case 13 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:58: T72
                {
                mT72(); 

                }
                break;
            case 14 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:62: T73
                {
                mT73(); 

                }
                break;
            case 15 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:66: T74
                {
                mT74(); 

                }
                break;
            case 16 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:70: T75
                {
                mT75(); 

                }
                break;
            case 17 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:74: T76
                {
                mT76(); 

                }
                break;
            case 18 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:78: T77
                {
                mT77(); 

                }
                break;
            case 19 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:82: T78
                {
                mT78(); 

                }
                break;
            case 20 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:86: T79
                {
                mT79(); 

                }
                break;
            case 21 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:90: T80
                {
                mT80(); 

                }
                break;
            case 22 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:94: T81
                {
                mT81(); 

                }
                break;
            case 23 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:98: T82
                {
                mT82(); 

                }
                break;
            case 24 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:102: T83
                {
                mT83(); 

                }
                break;
            case 25 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:106: T84
                {
                mT84(); 

                }
                break;
            case 26 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:110: T85
                {
                mT85(); 

                }
                break;
            case 27 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:114: T86
                {
                mT86(); 

                }
                break;
            case 28 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:118: T87
                {
                mT87(); 

                }
                break;
            case 29 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:122: T88
                {
                mT88(); 

                }
                break;
            case 30 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:126: T89
                {
                mT89(); 

                }
                break;
            case 31 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:130: T90
                {
                mT90(); 

                }
                break;
            case 32 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:134: T91
                {
                mT91(); 

                }
                break;
            case 33 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:138: T92
                {
                mT92(); 

                }
                break;
            case 34 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:142: T93
                {
                mT93(); 

                }
                break;
            case 35 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:146: T94
                {
                mT94(); 

                }
                break;
            case 36 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:150: T95
                {
                mT95(); 

                }
                break;
            case 37 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:154: T96
                {
                mT96(); 

                }
                break;
            case 38 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:158: T97
                {
                mT97(); 

                }
                break;
            case 39 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:162: RULE_WS
                {
                mRULE_WS(); 

                }
                break;
            case 40 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:170: RULE_ML_COMMENT
                {
                mRULE_ML_COMMENT(); 

                }
                break;
            case 41 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:186: RULE_SL_COMMENT
                {
                mRULE_SL_COMMENT(); 

                }
                break;
            case 42 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:202: RULE_TERM
                {
                mRULE_TERM(); 

                }
                break;
            case 43 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:212: RULE_NL
                {
                mRULE_NL(); 

                }
                break;
            case 44 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:220: RULE_STRING
                {
                mRULE_STRING(); 

                }
                break;
            case 45 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:232: RULE_STRING_HEX
                {
                mRULE_STRING_HEX(); 

                }
                break;
            case 46 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:248: RULE_STRING_BOOL
                {
                mRULE_STRING_BOOL(); 

                }
                break;
            case 47 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:265: RULE_SYMBOL_VAR
                {
                mRULE_SYMBOL_VAR(); 

                }
                break;
            case 48 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:281: RULE_SYMBOL_CLS
                {
                mRULE_SYMBOL_CLS(); 

                }
                break;
            case 49 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:297: RULE_SYMBOL_NUM
                {
                mRULE_SYMBOL_NUM(); 

                }
                break;
            case 50 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:313: RULE_OP_PLUS
                {
                mRULE_OP_PLUS(); 

                }
                break;
            case 51 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:326: RULE_OP_MINUS
                {
                mRULE_OP_MINUS(); 

                }
                break;
            case 52 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:340: RULE_OP_SLASH
                {
                mRULE_OP_SLASH(); 

                }
                break;
            case 53 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:354: RULE_OP_PERCENT
                {
                mRULE_OP_PERCENT(); 

                }
                break;
            case 54 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:370: RULE_OP_STAR
                {
                mRULE_OP_STAR(); 

                }
                break;
            case 55 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:383: RULE_OP_PIPE
                {
                mRULE_OP_PIPE(); 

                }
                break;
            case 56 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:396: RULE_OP_ET
                {
                mRULE_OP_ET(); 

                }
                break;
            case 57 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:407: RULE_OP_NEG
                {
                mRULE_OP_NEG(); 

                }
                break;
            case 58 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:419: RULE_OP_GT
                {
                mRULE_OP_GT(); 

                }
                break;
            case 59 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:430: RULE_OP_LT
                {
                mRULE_OP_LT(); 

                }
                break;
            case 60 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:441: RULE_OP_GEQ
                {
                mRULE_OP_GEQ(); 

                }
                break;
            case 61 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:453: RULE_OP_LEQ
                {
                mRULE_OP_LEQ(); 

                }
                break;
            case 62 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:465: RULE_OP_BSGT
                {
                mRULE_OP_BSGT(); 

                }
                break;
            case 63 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:478: RULE_OP_BSLT
                {
                mRULE_OP_BSLT(); 

                }
                break;
            case 64 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:491: RULE_OP_BSEQ
                {
                mRULE_OP_BSEQ(); 

                }
                break;
            case 65 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:504: RULE_OP_GTLT
                {
                mRULE_OP_GTLT(); 

                }
                break;
            case 66 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:517: RULE_OP_LTGT
                {
                mRULE_OP_LTGT(); 

                }
                break;
            case 67 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:530: RULE_OP_EQ2
                {
                mRULE_OP_EQ2(); 

                }
                break;
            case 68 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:542: RULE_OP_BSEQ2
                {
                mRULE_OP_BSEQ2(); 

                }
                break;
            case 69 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:556: RULE_OP_SLASH2
                {
                mRULE_OP_SLASH2(); 

                }
                break;
            case 70 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:571: RULE_OP_AND2
                {
                mRULE_OP_AND2(); 

                }
                break;
            case 71 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:584: RULE_OP_PIPE2
                {
                mRULE_OP_PIPE2(); 

                }
                break;
            case 72 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:598: RULE_OP_STAR2
                {
                mRULE_OP_STAR2(); 

                }
                break;
            case 73 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:612: RULE_OP_NGT
                {
                mRULE_OP_NGT(); 

                }
                break;
            case 74 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:624: RULE_OP_NLT
                {
                mRULE_OP_NLT(); 

                }
                break;
            case 75 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:636: RULE_OP_NEQ
                {
                mRULE_OP_NEQ(); 

                }
                break;
            case 76 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:648: RULE_OP_NEQ2
                {
                mRULE_OP_NEQ2(); 

                }
                break;
            case 77 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:661: RULE_OP_GT2
                {
                mRULE_OP_GT2(); 

                }
                break;
            case 78 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:673: RULE_OP_LT2
                {
                mRULE_OP_LT2(); 

                }
                break;
            case 79 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:685: RULE_OP_GT2EQ
                {
                mRULE_OP_GT2EQ(); 

                }
                break;
            case 80 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:699: RULE_OP_BSLT2
                {
                mRULE_OP_BSLT2(); 

                }
                break;
            case 81 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:713: RULE_OP_NLT2
                {
                mRULE_OP_NLT2(); 

                }
                break;
            case 82 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:726: RULE_OP_BSGT2
                {
                mRULE_OP_BSGT2(); 

                }
                break;
            case 83 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:740: RULE_OP_NGT2
                {
                mRULE_OP_NGT2(); 

                }
                break;
            case 84 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:753: RULE_OP_LT2EQ
                {
                mRULE_OP_LT2EQ(); 

                }
                break;
            case 85 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:767: RULE_OP_PLUSEQ
                {
                mRULE_OP_PLUSEQ(); 

                }
                break;
            case 86 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:782: RULE_OP_MINUSEQ
                {
                mRULE_OP_MINUSEQ(); 

                }
                break;
            case 87 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:798: RULE_OP_STAREQ
                {
                mRULE_OP_STAREQ(); 

                }
                break;
            case 88 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:813: RULE_OP_SLASHEQ
                {
                mRULE_OP_SLASHEQ(); 

                }
                break;
            case 89 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:829: RULE_OP_PERCENTEQ
                {
                mRULE_OP_PERCENTEQ(); 

                }
                break;
            case 90 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:847: RULE_OP_SLAHS2EQ
                {
                mRULE_OP_SLAHS2EQ(); 

                }
                break;
            case 91 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:864: RULE_OP_PIPE2EQ
                {
                mRULE_OP_PIPE2EQ(); 

                }
                break;
            case 92 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:880: RULE_OP_ANDEQ
                {
                mRULE_OP_ANDEQ(); 

                }
                break;
            case 93 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:894: RULE_OP_PIPEEQ
                {
                mRULE_OP_PIPEEQ(); 

                }
                break;
            case 94 :
                // ../org.oorexx.workbench.edit.ui/src-gen/org/oorexx/workbench/ui/contentassist/antlr/internal/InternalRexx.g:1:909: RULE_OP_AND2EQ
                {
                mRULE_OP_AND2EQ(); 

                }
                break;

        }

    }


    protected DFA11 dfa11 = new DFA11(this);
    static final String DFA11_eotS =
        "\1\uffff\1\37\1\54\6\37\1\uffff\3\37\1\76\5\uffff\1\100\1\104\2"+
        "\37\1\uffff\1\113\1\116\7\uffff\1\124\1\126\1\131\1\134\1\137\1"+
        "\143\1\147\1\153\2\37\2\uffff\1\37\1\157\4\37\1\164\11\37\4\uffff"+
        "\1\u0084\1\u0086\1\u0088\1\uffff\3\37\1\uffff\1\u008d\6\uffff\1"+
        "\u008e\1\uffff\1\u008e\7\uffff\1\u0093\2\uffff\1\u0095\2\uffff\1"+
        "\u0097\1\u0099\1\u009b\3\uffff\1\u009d\3\uffff\1\u009f\1\uffff\3"+
        "\37\1\uffff\3\37\1\u00a6\1\uffff\2\37\1\u00a9\1\u00aa\3\37\1\u00ae"+
        "\1\37\13\uffff\2\37\1\u00b6\24\uffff\3\37\1\u00ba\1\37\1\u00bc\1"+
        "\uffff\1\37\1\u00be\2\uffff\3\37\1\uffff\1\37\4\uffff\2\37\1\uffff"+
        "\3\37\1\uffff\1\37\1\uffff\1\u00c9\1\uffff\1\u00ca\6\37\1\u00d1"+
        "\1\37\1\u00d3\2\uffff\1\u00d4\1\37\1\u00d6\2\37\1\u00d9\1\uffff"+
        "\1\u00da\2\uffff\1\37\1\uffff\2\37\2\uffff\1\u00de\2\37\1\uffff"+
        "\1\u00e1\1\37\1\uffff\1\u00e3\1\uffff";
    static final String DFA11_eofS =
        "\u00e4\uffff";
    static final String DFA11_minS =
        "\1\11\1\162\1\75\1\146\1\150\1\154\1\157\1\141\1\162\1\uffff\1\163"+
        "\1\141\1\145\1\41\5\uffff\1\72\1\74\2\145\1\uffff\1\52\1\55\1\12"+
        "\2\uffff\2\0\2\uffff\2\75\1\52\1\75\1\46\3\74\1\151\1\142\2\uffff"+
        "\1\150\1\41\1\145\1\160\1\163\1\144\1\41\1\141\1\154\1\147\1\145"+
        "\1\151\1\156\1\142\1\164\1\162\2\uffff\1\141\1\uffff\1\76\1\74\1"+
        "\75\1\uffff\1\164\1\170\1\164\1\uffff\1\75\5\uffff\1\0\1\102\1\0"+
        "\1\47\7\uffff\1\75\2\uffff\1\75\2\uffff\1\76\1\74\1\75\3\uffff\1"+
        "\75\3\uffff\1\75\1\uffff\1\166\1\154\1\145\1\uffff\1\156\1\157\1"+
        "\145\1\41\1\uffff\1\163\1\154\2\41\1\163\1\164\1\143\1\41\1\151"+
        "\2\uffff\1\145\1\154\7\uffff\1\141\1\151\1\41\5\uffff\1\0\16\uffff"+
        "\1\141\1\151\1\162\1\41\1\163\1\41\1\uffff\1\163\1\41\2\uffff\1"+
        "\145\1\141\1\154\1\uffff\1\143\4\uffff\1\143\1\156\1\uffff\1\164"+
        "\1\143\1\151\1\uffff\1\145\1\uffff\1\41\1\uffff\1\41\1\170\1\141"+
        "\1\164\1\154\1\143\1\145\1\41\1\164\1\41\2\uffff\1\41\1\163\1\41"+
        "\1\141\1\154\1\41\1\uffff\1\41\2\uffff\1\163\1\uffff\1\163\1\141"+
        "\2\uffff\1\41\2\163\1\uffff\1\41\1\163\1\uffff\1\41\1\uffff";
    static final String DFA11_maxS =
        "\1\u00ac\1\165\1\75\1\156\1\150\1\170\1\157\1\154\1\162\1\uffff"+
        "\1\163\1\141\1\171\1\172\5\uffff\1\72\1\76\1\151\1\145\1\uffff\2"+
        "\75\1\12\2\uffff\2\ufffe\2\uffff\3\75\1\174\1\75\3\76\1\151\1\142"+
        "\2\uffff\1\150\1\172\1\145\1\160\1\163\1\144\1\172\1\141\1\154\1"+
        "\147\1\145\1\151\1\156\1\142\1\164\1\162\2\uffff\1\162\1\uffff\1"+
        "\76\1\74\1\75\1\uffff\1\164\1\170\1\164\1\uffff\1\75\5\uffff\1\ufffe"+
        "\1\170\1\ufffe\1\170\7\uffff\1\75\2\uffff\1\75\2\uffff\1\76\1\74"+
        "\1\75\3\uffff\1\75\3\uffff\1\75\1\uffff\1\166\1\154\1\145\1\uffff"+
        "\1\156\1\157\1\145\1\172\1\uffff\1\163\1\154\2\172\1\163\1\164\1"+
        "\143\1\172\1\151\2\uffff\2\157\7\uffff\1\141\1\151\1\172\5\uffff"+
        "\1\ufffe\16\uffff\1\141\1\151\1\162\1\172\1\163\1\172\1\uffff\1"+
        "\163\1\172\2\uffff\1\145\1\141\1\154\1\uffff\1\143\4\uffff\1\143"+
        "\1\156\1\uffff\1\164\1\143\1\151\1\uffff\1\145\1\uffff\1\172\1\uffff"+
        "\1\172\1\170\1\141\1\164\1\154\1\143\1\145\1\172\1\164\1\172\2\uffff"+
        "\1\172\1\163\1\172\1\141\1\154\1\172\1\uffff\1\172\2\uffff\1\163"+
        "\1\uffff\1\163\1\141\2\uffff\1\172\2\163\1\uffff\1\172\1\163\1\uffff"+
        "\1\172\1\uffff";
    static final String DFA11_acceptS =
        "\11\uffff\1\13\4\uffff\1\20\1\21\1\22\1\23\1\24\4\uffff\1\47\3\uffff"+
        "\2\52\2\uffff\1\57\1\61\12\uffff\1\103\1\3\20\uffff\1\60\1\17\1"+
        "\uffff\1\25\3\uffff\1\26\3\uffff\1\50\1\uffff\1\130\1\64\1\51\1"+
        "\126\1\63\4\uffff\1\125\1\62\1\131\1\65\1\110\1\127\1\66\1\uffff"+
        "\1\135\1\67\1\uffff\1\134\1\70\3\uffff\1\71\1\74\1\101\1\uffff\1"+
        "\72\1\75\1\102\1\uffff\1\73\3\uffff\1\4\4\uffff\1\7\11\uffff\1\34"+
        "\1\44\2\uffff\1\36\1\122\1\76\1\120\1\77\1\104\1\100\3\uffff\1\132"+
        "\1\105\1\54\1\56\1\55\1\uffff\1\133\1\107\1\136\1\106\1\123\1\111"+
        "\1\121\1\112\1\114\1\113\1\117\1\115\1\124\1\116\6\uffff\1\10\2"+
        "\uffff\1\12\1\14\3\uffff\1\37\1\uffff\1\43\1\42\1\41\1\27\2\uffff"+
        "\1\40\3\uffff\1\5\1\uffff\1\6\1\uffff\1\11\12\uffff\1\46\1\15\6"+
        "\uffff\1\2\1\uffff\1\35\1\16\1\uffff\1\45\2\uffff\1\1\1\33\3\uffff"+
        "\1\31\2\uffff\1\30\1\uffff\1\32";
    static final String DFA11_specialS =
        "\u00e4\uffff}>";
    static final String[] DFA11_transitionS = {
            "\1\27\1\33\2\uffff\1\32\22\uffff\1\27\1\37\1\35\2\uffff\1\42"+
            "\1\45\1\36\1\20\1\21\1\43\1\41\1\11\1\31\1\15\1\30\12\40\1\23"+
            "\1\34\1\50\1\2\1\47\1\37\1\uffff\32\37\1\16\1\24\1\17\1\uffff"+
            "\1\37\1\uffff\1\10\1\37\1\7\1\6\1\5\1\37\1\26\1\37\1\3\3\37"+
            "\1\25\2\37\1\1\1\37\1\13\1\14\1\4\1\12\5\37\1\uffff\1\44\1\uffff"+
            "\1\22\55\uffff\1\46",
            "\1\51\2\uffff\1\52",
            "\1\53",
            "\1\56\7\uffff\1\55",
            "\1\57",
            "\1\61\1\uffff\1\62\11\uffff\1\60",
            "\1\63",
            "\1\65\12\uffff\1\64",
            "\1\66",
            "",
            "\1\67",
            "\1\70",
            "\1\73\16\uffff\1\74\1\72\3\uffff\1\71",
            "\1\75\35\uffff\1\75\1\uffff\32\75\4\uffff\1\75\1\uffff\32\75",
            "",
            "",
            "",
            "",
            "",
            "\1\77",
            "\1\102\1\103\1\101",
            "\1\105\3\uffff\1\106",
            "\1\107",
            "",
            "\1\110\4\uffff\1\111\15\uffff\1\112",
            "\1\114\17\uffff\1\115",
            "\1\33",
            "",
            "",
            "\42\117\1\120\uffdc\117",
            "\47\121\1\122\uffd7\121",
            "",
            "",
            "\1\123",
            "\1\125",
            "\1\127\22\uffff\1\130",
            "\1\133\76\uffff\1\132",
            "\1\135\26\uffff\1\136",
            "\1\141\1\142\1\140",
            "\1\145\1\144\1\146",
            "\1\152\1\150\1\151",
            "\1\154",
            "\1\155",
            "",
            "",
            "\1\156",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\160",
            "\1\161",
            "\1\162",
            "\1\163",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\165",
            "\1\166",
            "\1\167",
            "\1\170",
            "\1\171",
            "\1\172",
            "\1\173",
            "\1\174",
            "\1\175",
            "",
            "",
            "\1\u0082\1\uffff\1\u0081\11\uffff\1\176\1\uffff\1\177\2\uffff"+
            "\1\u0080",
            "",
            "\1\u0083",
            "\1\u0085",
            "\1\u0087",
            "",
            "\1\u0089",
            "\1\u008a",
            "\1\u008b",
            "",
            "\1\u008c",
            "",
            "",
            "",
            "",
            "",
            "\42\117\1\120\uffdc\117",
            "\1\u008f\25\uffff\1\u0090\11\uffff\1\u008f\25\uffff\1\u0090",
            "\47\121\1\122\uffd7\121",
            "\1\u0091\32\uffff\1\u008f\25\uffff\1\u0090\11\uffff\1\u008f"+
            "\25\uffff\1\u0090",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u0092",
            "",
            "",
            "\1\u0094",
            "",
            "",
            "\1\u0096",
            "\1\u0098",
            "\1\u009a",
            "",
            "",
            "",
            "\1\u009c",
            "",
            "",
            "",
            "\1\u009e",
            "",
            "\1\u00a0",
            "\1\u00a1",
            "\1\u00a2",
            "",
            "\1\u00a3",
            "\1\u00a4",
            "\1\u00a5",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "\1\u00a7",
            "\1\u00a8",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00ab",
            "\1\u00ac",
            "\1\u00ad",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00af",
            "",
            "",
            "\1\u00b0\11\uffff\1\u00b1",
            "\1\u00b3\2\uffff\1\u00b2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u00b4",
            "\1\u00b5",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "",
            "",
            "",
            "\47\121\1\122\uffd7\121",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u00b7",
            "\1\u00b8",
            "\1\u00b9",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00bb",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "\1\u00bd",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "\1\u00bf",
            "\1\u00c0",
            "\1\u00c1",
            "",
            "\1\u00c2",
            "",
            "",
            "",
            "",
            "\1\u00c3",
            "\1\u00c4",
            "",
            "\1\u00c5",
            "\1\u00c6",
            "\1\u00c7",
            "",
            "\1\u00c8",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00cb",
            "\1\u00cc",
            "\1\u00cd",
            "\1\u00ce",
            "\1\u00cf",
            "\1\u00d0",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00d2",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00d5",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00d7",
            "\1\u00d8",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "\1\u00db",
            "",
            "\1\u00dc",
            "\1\u00dd",
            "",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00df",
            "\1\u00e0",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00e2",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            ""
    };

    static final short[] DFA11_eot = DFA.unpackEncodedString(DFA11_eotS);
    static final short[] DFA11_eof = DFA.unpackEncodedString(DFA11_eofS);
    static final char[] DFA11_min = DFA.unpackEncodedStringToUnsignedChars(DFA11_minS);
    static final char[] DFA11_max = DFA.unpackEncodedStringToUnsignedChars(DFA11_maxS);
    static final short[] DFA11_accept = DFA.unpackEncodedString(DFA11_acceptS);
    static final short[] DFA11_special = DFA.unpackEncodedString(DFA11_specialS);
    static final short[][] DFA11_transition;

    static {
        int numStates = DFA11_transitionS.length;
        DFA11_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA11_transition[i] = DFA.unpackEncodedString(DFA11_transitionS[i]);
        }
    }

    class DFA11 extends DFA {

        public DFA11(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 11;
            this.eot = DFA11_eot;
            this.eof = DFA11_eof;
            this.min = DFA11_min;
            this.max = DFA11_max;
            this.accept = DFA11_accept;
            this.special = DFA11_special;
            this.transition = DFA11_transition;
        }
        public String getDescription() {
            return "1:1: Tokens : ( T60 | T61 | T62 | T63 | T64 | T65 | T66 | T67 | T68 | T69 | T70 | T71 | T72 | T73 | T74 | T75 | T76 | T77 | T78 | T79 | T80 | T81 | T82 | T83 | T84 | T85 | T86 | T87 | T88 | T89 | T90 | T91 | T92 | T93 | T94 | T95 | T96 | T97 | RULE_WS | RULE_ML_COMMENT | RULE_SL_COMMENT | RULE_TERM | RULE_NL | RULE_STRING | RULE_STRING_HEX | RULE_STRING_BOOL | RULE_SYMBOL_VAR | RULE_SYMBOL_CLS | RULE_SYMBOL_NUM | RULE_OP_PLUS | RULE_OP_MINUS | RULE_OP_SLASH | RULE_OP_PERCENT | RULE_OP_STAR | RULE_OP_PIPE | RULE_OP_ET | RULE_OP_NEG | RULE_OP_GT | RULE_OP_LT | RULE_OP_GEQ | RULE_OP_LEQ | RULE_OP_BSGT | RULE_OP_BSLT | RULE_OP_BSEQ | RULE_OP_GTLT | RULE_OP_LTGT | RULE_OP_EQ2 | RULE_OP_BSEQ2 | RULE_OP_SLASH2 | RULE_OP_AND2 | RULE_OP_PIPE2 | RULE_OP_STAR2 | RULE_OP_NGT | RULE_OP_NLT | RULE_OP_NEQ | RULE_OP_NEQ2 | RULE_OP_GT2 | RULE_OP_LT2 | RULE_OP_GT2EQ | RULE_OP_BSLT2 | RULE_OP_NLT2 | RULE_OP_BSGT2 | RULE_OP_NGT2 | RULE_OP_LT2EQ | RULE_OP_PLUSEQ | RULE_OP_MINUSEQ | RULE_OP_STAREQ | RULE_OP_SLASHEQ | RULE_OP_PERCENTEQ | RULE_OP_SLAHS2EQ | RULE_OP_PIPE2EQ | RULE_OP_ANDEQ | RULE_OP_PIPEEQ | RULE_OP_AND2EQ );";
        }
    }
 

}