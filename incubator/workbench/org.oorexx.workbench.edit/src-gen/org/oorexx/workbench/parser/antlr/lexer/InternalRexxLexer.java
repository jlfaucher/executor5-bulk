package org.oorexx.workbench.parser.antlr.lexer;

// Hack: Use our own Lexer superclass by means of import. 
// Currently there is no other way to specify the superclass for the lexer.
import org.eclipse.xtext.parser.antlr.Lexer;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

public class InternalRexxLexer extends Lexer {
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
    public static final int KEYWORD_3=34;
    public static final int RULE_OP_LEQ=64;
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
    public static final int KEYWORD_10=41;
    public static final int RULE_OP_MINUS=54;
    public static final int KEYWORD_11=30;
    public static final int RULE_OP_GT2EQ=82;
    public static final int KEYWORD_12=31;
    public static final int KEYWORD_13=25;
    public static final int KEYWORD_14=26;
    public static final int RULE_OP_BSGT=65;
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
    public InternalRexxLexer() {;} 
    public InternalRexxLexer(CharStream input) {
        super(input);
    }
    public String getGrammarFileName() { return "../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g"; }

    // $ANTLR start KEYWORD_38
    public final void mKEYWORD_38() throws RecognitionException {
        try {
            int _type = KEYWORD_38;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:19:12: ( ':' ':' ( 'A' | 'a' ) ( 'T' | 't' ) ( 'T' | 't' ) ( 'R' | 'r' ) ( 'I' | 'i' ) ( 'B' | 'b' ) ( 'U' | 'u' ) ( 'T' | 't' ) ( 'E' | 'e' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:19:14: ':' ':' ( 'A' | 'a' ) ( 'T' | 't' ) ( 'T' | 't' ) ( 'R' | 'r' ) ( 'I' | 'i' ) ( 'B' | 'b' ) ( 'U' | 'u' ) ( 'T' | 't' ) ( 'E' | 'e' )
            {
            match(':'); 
            match(':'); 
            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='B'||input.LA(1)=='b' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
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
    // $ANTLR end KEYWORD_38

    // $ANTLR start KEYWORD_35
    public final void mKEYWORD_35() throws RecognitionException {
        try {
            int _type = KEYWORD_35;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:21:12: ( ':' ':' ( 'C' | 'c' ) ( 'O' | 'o' ) ( 'N' | 'n' ) ( 'S' | 's' ) ( 'T' | 't' ) ( 'A' | 'a' ) ( 'N' | 'n' ) ( 'T' | 't' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:21:14: ':' ':' ( 'C' | 'c' ) ( 'O' | 'o' ) ( 'N' | 'n' ) ( 'S' | 's' ) ( 'T' | 't' ) ( 'A' | 'a' ) ( 'N' | 'n' ) ( 'T' | 't' )
            {
            match(':'); 
            match(':'); 
            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
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
    // $ANTLR end KEYWORD_35

    // $ANTLR start KEYWORD_36
    public final void mKEYWORD_36() throws RecognitionException {
        try {
            int _type = KEYWORD_36;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:23:12: ( ':' ':' ( 'R' | 'r' ) ( 'E' | 'e' ) ( 'Q' | 'q' ) ( 'U' | 'u' ) ( 'I' | 'i' ) ( 'R' | 'r' ) ( 'E' | 'e' ) ( 'S' | 's' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:23:14: ':' ':' ( 'R' | 'r' ) ( 'E' | 'e' ) ( 'Q' | 'q' ) ( 'U' | 'u' ) ( 'I' | 'i' ) ( 'R' | 'r' ) ( 'E' | 'e' ) ( 'S' | 's' )
            {
            match(':'); 
            match(':'); 
            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='Q'||input.LA(1)=='q' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
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
    // $ANTLR end KEYWORD_36

    // $ANTLR start KEYWORD_37
    public final void mKEYWORD_37() throws RecognitionException {
        try {
            int _type = KEYWORD_37;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:25:12: ( ( 'M' | 'm' ) ( 'I' | 'i' ) ( 'X' | 'x' ) ( 'I' | 'i' ) ( 'N' | 'n' ) ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:25:14: ( 'M' | 'm' ) ( 'I' | 'i' ) ( 'X' | 'x' ) ( 'I' | 'i' ) ( 'N' | 'n' ) ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' )
            {
            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='X'||input.LA(1)=='x' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
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
    // $ANTLR end KEYWORD_37

    // $ANTLR start KEYWORD_32
    public final void mKEYWORD_32() throws RecognitionException {
        try {
            int _type = KEYWORD_32;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:27:12: ( ':' ':' ( 'O' | 'o' ) ( 'P' | 'p' ) ( 'T' | 't' ) ( 'I' | 'i' ) ( 'O' | 'o' ) ( 'N' | 'n' ) ( 'S' | 's' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:27:14: ':' ':' ( 'O' | 'o' ) ( 'P' | 'p' ) ( 'T' | 't' ) ( 'I' | 'i' ) ( 'O' | 'o' ) ( 'N' | 'n' ) ( 'S' | 's' )
            {
            match(':'); 
            match(':'); 
            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
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
    // $ANTLR end KEYWORD_32

    // $ANTLR start KEYWORD_33
    public final void mKEYWORD_33() throws RecognitionException {
        try {
            int _type = KEYWORD_33;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:29:12: ( ':' ':' ( 'R' | 'r' ) ( 'O' | 'o' ) ( 'U' | 'u' ) ( 'T' | 't' ) ( 'I' | 'i' ) ( 'N' | 'n' ) ( 'E' | 'e' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:29:14: ':' ':' ( 'R' | 'r' ) ( 'O' | 'o' ) ( 'U' | 'u' ) ( 'T' | 't' ) ( 'I' | 'i' ) ( 'N' | 'n' ) ( 'E' | 'e' )
            {
            match(':'); 
            match(':'); 
            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
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
    // $ANTLR end KEYWORD_33

    // $ANTLR start KEYWORD_34
    public final void mKEYWORD_34() throws RecognitionException {
        try {
            int _type = KEYWORD_34;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:31:12: ( ( 'M' | 'm' ) ( 'E' | 'e' ) ( 'T' | 't' ) ( 'A' | 'a' ) ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:31:14: ( 'M' | 'm' ) ( 'E' | 'e' ) ( 'T' | 't' ) ( 'A' | 'a' ) ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' )
            {
            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
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
    // $ANTLR end KEYWORD_34

    // $ANTLR start KEYWORD_30
    public final void mKEYWORD_30() throws RecognitionException {
        try {
            int _type = KEYWORD_30;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:33:12: ( ':' ':' ( 'M' | 'm' ) ( 'E' | 'e' ) ( 'T' | 't' ) ( 'H' | 'h' ) ( 'O' | 'o' ) ( 'D' | 'd' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:33:14: ':' ':' ( 'M' | 'm' ) ( 'E' | 'e' ) ( 'T' | 't' ) ( 'H' | 'h' ) ( 'O' | 'o' ) ( 'D' | 'd' )
            {
            match(':'); 
            match(':'); 
            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='H'||input.LA(1)=='h' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='D'||input.LA(1)=='d' ) {
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
    // $ANTLR end KEYWORD_30

    // $ANTLR start KEYWORD_31
    public final void mKEYWORD_31() throws RecognitionException {
        try {
            int _type = KEYWORD_31;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:35:12: ( ( 'S' | 's' ) ( 'U' | 'u' ) ( 'B' | 'b' ) ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:35:14: ( 'S' | 's' ) ( 'U' | 'u' ) ( 'B' | 'b' ) ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' )
            {
            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='B'||input.LA(1)=='b' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
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
    // $ANTLR end KEYWORD_31

    // $ANTLR start KEYWORD_27
    public final void mKEYWORD_27() throws RecognitionException {
        try {
            int _type = KEYWORD_27;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:37:12: ( ':' ':' ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:37:14: ':' ':' ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' )
            {
            match(':'); 
            match(':'); 
            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
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
    // $ANTLR end KEYWORD_27

    // $ANTLR start KEYWORD_28
    public final void mKEYWORD_28() throws RecognitionException {
        try {
            int _type = KEYWORD_28;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:39:12: ( ( 'I' | 'i' ) ( 'N' | 'n' ) ( 'H' | 'h' ) ( 'E' | 'e' ) ( 'R' | 'r' ) ( 'I' | 'i' ) ( 'T' | 't' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:39:14: ( 'I' | 'i' ) ( 'N' | 'n' ) ( 'H' | 'h' ) ( 'E' | 'e' ) ( 'R' | 'r' ) ( 'I' | 'i' ) ( 'T' | 't' )
            {
            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='H'||input.LA(1)=='h' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
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
    // $ANTLR end KEYWORD_28

    // $ANTLR start KEYWORD_29
    public final void mKEYWORD_29() throws RecognitionException {
        try {
            int _type = KEYWORD_29;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:41:12: ( ( 'P' | 'p' ) ( 'R' | 'r' ) ( 'I' | 'i' ) ( 'V' | 'v' ) ( 'A' | 'a' ) ( 'T' | 't' ) ( 'E' | 'e' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:41:14: ( 'P' | 'p' ) ( 'R' | 'r' ) ( 'I' | 'i' ) ( 'V' | 'v' ) ( 'A' | 'a' ) ( 'T' | 't' ) ( 'E' | 'e' )
            {
            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='V'||input.LA(1)=='v' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
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
    // $ANTLR end KEYWORD_29

    // $ANTLR start KEYWORD_23
    public final void mKEYWORD_23() throws RecognitionException {
        try {
            int _type = KEYWORD_23;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:43:12: ( ( 'E' | 'e' ) ( 'X' | 'x' ) ( 'P' | 'p' ) ( 'O' | 'o' ) ( 'S' | 's' ) ( 'E' | 'e' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:43:14: ( 'E' | 'e' ) ( 'X' | 'x' ) ( 'P' | 'p' ) ( 'O' | 'o' ) ( 'S' | 's' ) ( 'E' | 'e' )
            {
            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='X'||input.LA(1)=='x' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
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
    // $ANTLR end KEYWORD_23

    // $ANTLR start KEYWORD_24
    public final void mKEYWORD_24() throws RecognitionException {
        try {
            int _type = KEYWORD_24;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:45:12: ( ( 'P' | 'p' ) ( 'U' | 'u' ) ( 'B' | 'b' ) ( 'L' | 'l' ) ( 'I' | 'i' ) ( 'C' | 'c' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:45:14: ( 'P' | 'p' ) ( 'U' | 'u' ) ( 'B' | 'b' ) ( 'L' | 'l' ) ( 'I' | 'i' ) ( 'C' | 'c' )
            {
            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='B'||input.LA(1)=='b' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
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
    // $ANTLR end KEYWORD_24

    // $ANTLR start KEYWORD_25
    public final void mKEYWORD_25() throws RecognitionException {
        try {
            int _type = KEYWORD_25;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:47:12: ( ( 'S' | 's' ) ( 'T' | 't' ) ( 'R' | 'r' ) ( 'I' | 'i' ) ( 'C' | 'c' ) ( 'T' | 't' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:47:14: ( 'S' | 's' ) ( 'T' | 't' ) ( 'R' | 'r' ) ( 'I' | 'i' ) ( 'C' | 'c' ) ( 'T' | 't' )
            {
            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
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
    // $ANTLR end KEYWORD_25

    // $ANTLR start KEYWORD_26
    public final void mKEYWORD_26() throws RecognitionException {
        try {
            int _type = KEYWORD_26;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:49:12: ( ( 'S' | 's' ) ( 'Y' | 'y' ) ( 'N' | 'n' ) ( 'T' | 't' ) ( 'A' | 'a' ) ( 'X' | 'x' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:49:14: ( 'S' | 's' ) ( 'Y' | 'y' ) ( 'N' | 'n' ) ( 'T' | 't' ) ( 'A' | 'a' ) ( 'X' | 'x' )
            {
            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='Y'||input.LA(1)=='y' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

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
    // $ANTLR end KEYWORD_26

    // $ANTLR start KEYWORD_21
    public final void mKEYWORD_21() throws RecognitionException {
        try {
            int _type = KEYWORD_21;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:51:12: ( ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:51:14: ( 'C' | 'c' ) ( 'L' | 'l' ) ( 'A' | 'a' ) ( 'S' | 's' ) ( 'S' | 's' )
            {
            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
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
    // $ANTLR end KEYWORD_21

    // $ANTLR start KEYWORD_22
    public final void mKEYWORD_22() throws RecognitionException {
        try {
            int _type = KEYWORD_22;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:53:12: ( ( 'R' | 'r' ) ( 'A' | 'a' ) ( 'I' | 'i' ) ( 'S' | 's' ) ( 'E' | 'e' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:53:14: ( 'R' | 'r' ) ( 'A' | 'a' ) ( 'I' | 'i' ) ( 'S' | 's' ) ( 'E' | 'e' )
            {
            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
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
    // $ANTLR end KEYWORD_22

    // $ANTLR start KEYWORD_18
    public final void mKEYWORD_18() throws RecognitionException {
        try {
            int _type = KEYWORD_18;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:55:12: ( ( 'C' | 'c' ) ( 'A' | 'a' ) ( 'L' | 'l' ) ( 'L' | 'l' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:55:14: ( 'C' | 'c' ) ( 'A' | 'a' ) ( 'L' | 'l' ) ( 'L' | 'l' )
            {
            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
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
    // $ANTLR end KEYWORD_18

    // $ANTLR start KEYWORD_19
    public final void mKEYWORD_19() throws RecognitionException {
        try {
            int _type = KEYWORD_19;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:57:12: ( ( 'E' | 'e' ) ( 'L' | 'l' ) ( 'S' | 's' ) ( 'E' | 'e' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:57:14: ( 'E' | 'e' ) ( 'L' | 'l' ) ( 'S' | 's' ) ( 'E' | 'e' )
            {
            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
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
    // $ANTLR end KEYWORD_19

    // $ANTLR start KEYWORD_20
    public final void mKEYWORD_20() throws RecognitionException {
        try {
            int _type = KEYWORD_20;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:59:12: ( ( 'T' | 't' ) ( 'H' | 'h' ) ( 'E' | 'e' ) ( 'N' | 'n' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:59:14: ( 'T' | 't' ) ( 'H' | 'h' ) ( 'E' | 'e' ) ( 'N' | 'n' )
            {
            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='H'||input.LA(1)=='h' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
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
    // $ANTLR end KEYWORD_20

    // $ANTLR start KEYWORD_13
    public final void mKEYWORD_13() throws RecognitionException {
        try {
            int _type = KEYWORD_13;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:61:12: ( ( 'A' | 'a' ) ( 'R' | 'r' ) ( 'G' | 'g' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:61:14: ( 'A' | 'a' ) ( 'R' | 'r' ) ( 'G' | 'g' )
            {
            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='G'||input.LA(1)=='g' ) {
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
    // $ANTLR end KEYWORD_13

    // $ANTLR start KEYWORD_14
    public final void mKEYWORD_14() throws RecognitionException {
        try {
            int _type = KEYWORD_14;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:63:12: ( ( 'E' | 'e' ) ( 'N' | 'n' ) ( 'D' | 'd' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:63:14: ( 'E' | 'e' ) ( 'N' | 'n' ) ( 'D' | 'd' )
            {
            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='D'||input.LA(1)=='d' ) {
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
    // $ANTLR end KEYWORD_14

    // $ANTLR start KEYWORD_15
    public final void mKEYWORD_15() throws RecognitionException {
        try {
            int _type = KEYWORD_15;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:65:12: ( ( 'G' | 'g' ) ( 'E' | 'e' ) ( 'T' | 't' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:65:14: ( 'G' | 'g' ) ( 'E' | 'e' ) ( 'T' | 't' )
            {
            if ( input.LA(1)=='G'||input.LA(1)=='g' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
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
    // $ANTLR end KEYWORD_15

    // $ANTLR start KEYWORD_16
    public final void mKEYWORD_16() throws RecognitionException {
        try {
            int _type = KEYWORD_16;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:67:12: ( ( 'S' | 's' ) ( 'E' | 'e' ) ( 'T' | 't' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:67:14: ( 'S' | 's' ) ( 'E' | 'e' ) ( 'T' | 't' )
            {
            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
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
    // $ANTLR end KEYWORD_16

    // $ANTLR start KEYWORD_17
    public final void mKEYWORD_17() throws RecognitionException {
        try {
            int _type = KEYWORD_17;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:69:12: ( ( 'U' | 'u' ) ( 'S' | 's' ) ( 'E' | 'e' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:69:14: ( 'U' | 'u' ) ( 'S' | 's' ) ( 'E' | 'e' )
            {
            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
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
    // $ANTLR end KEYWORD_17

    // $ANTLR start KEYWORD_11
    public final void mKEYWORD_11() throws RecognitionException {
        try {
            int _type = KEYWORD_11;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:71:12: ( ( 'D' | 'd' ) ( 'O' | 'o' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:71:14: ( 'D' | 'd' ) ( 'O' | 'o' )
            {
            if ( input.LA(1)=='D'||input.LA(1)=='d' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
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
    // $ANTLR end KEYWORD_11

    // $ANTLR start KEYWORD_12
    public final void mKEYWORD_12() throws RecognitionException {
        try {
            int _type = KEYWORD_12;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:73:12: ( ( 'I' | 'i' ) ( 'F' | 'f' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:73:14: ( 'I' | 'i' ) ( 'F' | 'f' )
            {
            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            if ( input.LA(1)=='F'||input.LA(1)=='f' ) {
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
    // $ANTLR end KEYWORD_12

    // $ANTLR start KEYWORD_1
    public final void mKEYWORD_1() throws RecognitionException {
        try {
            int _type = KEYWORD_1;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:75:11: ( '(' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:75:13: '('
            {
            match('('); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_1

    // $ANTLR start KEYWORD_2
    public final void mKEYWORD_2() throws RecognitionException {
        try {
            int _type = KEYWORD_2;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:77:11: ( ')' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:77:13: ')'
            {
            match(')'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_2

    // $ANTLR start KEYWORD_3
    public final void mKEYWORD_3() throws RecognitionException {
        try {
            int _type = KEYWORD_3;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:79:11: ( ',' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:79:13: ','
            {
            match(','); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_3

    // $ANTLR start KEYWORD_4
    public final void mKEYWORD_4() throws RecognitionException {
        try {
            int _type = KEYWORD_4;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:81:11: ( '.' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:81:13: '.'
            {
            match('.'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_4

    // $ANTLR start KEYWORD_5
    public final void mKEYWORD_5() throws RecognitionException {
        try {
            int _type = KEYWORD_5;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:83:11: ( ':' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:83:13: ':'
            {
            match(':'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_5

    // $ANTLR start KEYWORD_6
    public final void mKEYWORD_6() throws RecognitionException {
        try {
            int _type = KEYWORD_6;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:85:11: ( '=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:85:13: '='
            {
            match('='); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_6

    // $ANTLR start KEYWORD_7
    public final void mKEYWORD_7() throws RecognitionException {
        try {
            int _type = KEYWORD_7;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:87:11: ( '[' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:87:13: '['
            {
            match('['); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_7

    // $ANTLR start KEYWORD_8
    public final void mKEYWORD_8() throws RecognitionException {
        try {
            int _type = KEYWORD_8;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:89:11: ( '\\\\' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:89:13: '\\\\'
            {
            match('\\'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_8

    // $ANTLR start KEYWORD_9
    public final void mKEYWORD_9() throws RecognitionException {
        try {
            int _type = KEYWORD_9;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:91:11: ( ']' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:91:13: ']'
            {
            match(']'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_9

    // $ANTLR start KEYWORD_10
    public final void mKEYWORD_10() throws RecognitionException {
        try {
            int _type = KEYWORD_10;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:93:12: ( '~' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:93:14: '~'
            {
            match('~'); 

            }

            this.type = _type;
        }
        finally {
        }
    }
    // $ANTLR end KEYWORD_10

    // $ANTLR start RULE_WS
    public final void mRULE_WS() throws RecognitionException {
        try {
            int _type = RULE_WS;
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:97:9: ( ( ' ' | '\\t' )+ )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:97:11: ( ' ' | '\\t' )+
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:97:11: ( ' ' | '\\t' )+
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
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:99:17: ( '/*' ( options {greedy=false; } : . )* '*/' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:99:19: '/*' ( options {greedy=false; } : . )* '*/'
            {
            match("/*"); 

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:99:24: ( options {greedy=false; } : . )*
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
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:99:52: .
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:101:17: ( '--' (~ ( ( '\\r' | '\\n' ) ) )* )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:101:19: '--' (~ ( ( '\\r' | '\\n' ) ) )*
            {
            match("--"); 

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:101:24: (~ ( ( '\\r' | '\\n' ) ) )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( ((LA3_0>='\u0000' && LA3_0<='\t')||(LA3_0>='\u000B' && LA3_0<='\f')||(LA3_0>='\u000E' && LA3_0<='\uFFFE')) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:101:24: ~ ( ( '\\r' | '\\n' ) )
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:103:11: ( ( RULE_NL | ';' )+ )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:103:13: ( RULE_NL | ';' )+
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:103:13: ( RULE_NL | ';' )+
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
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:103:14: RULE_NL
            	    {
            	    mRULE_NL(); 

            	    }
            	    break;
            	case 2 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:103:22: ';'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:105:9: ( ( '\\r' )? '\\n' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:105:11: ( '\\r' )? '\\n'
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:105:11: ( '\\r' )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0=='\r') ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:105:11: '\\r'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:13: ( ( '\"' (~ ( '\"' ) )* '\"' | '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\'' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:15: ( '\"' (~ ( '\"' ) )* '\"' | '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\'' )
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:15: ( '\"' (~ ( '\"' ) )* '\"' | '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\'' )
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
                    new NoViableAltException("107:15: ( '\"' (~ ( '\"' ) )* '\"' | '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\'' )", 8, 0, input);

                throw nvae;
            }
            switch (alt8) {
                case 1 :
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:16: '\"' (~ ( '\"' ) )* '\"'
                    {
                    match('\"'); 
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:20: (~ ( '\"' ) )*
                    loop6:
                    do {
                        int alt6=2;
                        int LA6_0 = input.LA(1);

                        if ( ((LA6_0>='\u0000' && LA6_0<='!')||(LA6_0>='#' && LA6_0<='\uFFFE')) ) {
                            alt6=1;
                        }


                        switch (alt6) {
                    	case 1 :
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:20: ~ ( '\"' )
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
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:32: '\\'' (~ ( '\\'' ) | '\\'\\'' )* '\\''
                    {
                    match('\''); 
                    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:37: (~ ( '\\'' ) | '\\'\\'' )*
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
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:38: ~ ( '\\'' )
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
                    	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:107:46: '\\'\\''
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:109:17: ( RULE_STRING ( 'X' | 'x' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:109:19: RULE_STRING ( 'X' | 'x' )
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:111:18: ( RULE_STRING ( 'B' | 'b' ) )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:111:20: RULE_STRING ( 'B' | 'b' )
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:113:17: ( ( 'A' .. 'Z' | 'a' .. 'z' | '_' | '?' | '!' ) ( 'A' .. 'Z' | 'a' .. 'z' | '0' .. '9' | '_' | '.' | '!' | '?' )* )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:113:19: ( 'A' .. 'Z' | 'a' .. 'z' | '_' | '?' | '!' ) ( 'A' .. 'Z' | 'a' .. 'z' | '0' .. '9' | '_' | '.' | '!' | '?' )*
            {
            if ( input.LA(1)=='!'||input.LA(1)=='?'||(input.LA(1)>='A' && input.LA(1)<='Z')||input.LA(1)=='_'||(input.LA(1)>='a' && input.LA(1)<='z') ) {
                input.consume();

            }
            else {
                MismatchedSetException mse =
                    new MismatchedSetException(null,input);
                recover(mse);    throw mse;
            }

            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:113:51: ( 'A' .. 'Z' | 'a' .. 'z' | '0' .. '9' | '_' | '.' | '!' | '?' )*
            loop9:
            do {
                int alt9=2;
                int LA9_0 = input.LA(1);

                if ( (LA9_0=='!'||LA9_0=='.'||(LA9_0>='0' && LA9_0<='9')||LA9_0=='?'||(LA9_0>='A' && LA9_0<='Z')||LA9_0=='_'||(LA9_0>='a' && LA9_0<='z')) ) {
                    alt9=1;
                }


                switch (alt9) {
            	case 1 :
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:115:17: ( '.' RULE_SYMBOL_VAR )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:115:19: '.' RULE_SYMBOL_VAR
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:117:17: ( ( '0' .. '9' )+ )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:117:19: ( '0' .. '9' )+
            {
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:117:19: ( '0' .. '9' )+
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
            	    // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:117:20: '0' .. '9'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:119:14: ( '+' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:119:16: '+'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:121:15: ( '-' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:121:17: '-'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:123:15: ( '/' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:123:17: '/'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:125:17: ( '%' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:125:19: '%'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:127:14: ( '*' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:127:16: '*'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:129:14: ( '|' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:129:16: '|'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:131:12: ( '&' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:131:14: '&'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:133:13: ( '\\u00AC' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:133:15: '\\u00AC'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:135:12: ( '>' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:135:14: '>'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:137:12: ( '<' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:137:14: '<'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:139:13: ( '>=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:139:15: '>='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:141:13: ( '<=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:141:15: '<='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:143:14: ( '\\\\>' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:143:16: '\\\\>'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:145:14: ( '\\\\<' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:145:16: '\\\\<'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:147:14: ( '\\\\=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:147:16: '\\\\='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:149:14: ( '><' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:149:16: '><'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:151:14: ( '<>' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:151:16: '<>'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:153:13: ( '==' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:153:15: '=='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:155:15: ( '\\\\==' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:155:17: '\\\\=='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:157:16: ( '//' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:157:18: '//'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:159:14: ( '&&' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:159:16: '&&'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:161:15: ( '||' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:161:17: '||'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:163:15: ( '**' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:163:17: '**'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:165:13: ( '\\u00AC>' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:165:15: '\\u00AC>'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:167:13: ( '\\u00AC<' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:167:15: '\\u00AC<'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:169:13: ( '\\u00AC=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:169:15: '\\u00AC='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:171:14: ( '\\u00AC==' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:171:16: '\\u00AC=='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:173:13: ( '>>' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:173:15: '>>'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:175:13: ( '<<' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:175:15: '<<'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:177:15: ( '>>=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:177:17: '>>='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:179:15: ( '\\\\<<' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:179:17: '\\\\<<'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:181:14: ( '\\u00AC<<' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:181:16: '\\u00AC<<'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:183:15: ( '\\\\>>' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:183:17: '\\\\>>'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:185:14: ( '\\u00AC>>' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:185:16: '\\u00AC>>'
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:187:15: ( '<<=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:187:17: '<<='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:189:16: ( '+=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:189:18: '+='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:191:17: ( '-=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:191:19: '-='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:193:16: ( '*=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:193:18: '*='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:195:17: ( '/=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:195:19: '/='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:197:19: ( '%=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:197:21: '%='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:199:18: ( '//=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:199:20: '//='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:201:17: ( '||=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:201:19: '||='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:203:15: ( '&=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:203:17: '&='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:205:16: ( '|=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:205:18: '|='
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
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:207:16: ( '&&=' )
            // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:207:18: '&&='
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
        // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:8: ( KEYWORD_38 | KEYWORD_35 | KEYWORD_36 | KEYWORD_37 | KEYWORD_32 | KEYWORD_33 | KEYWORD_34 | KEYWORD_30 | KEYWORD_31 | KEYWORD_27 | KEYWORD_28 | KEYWORD_29 | KEYWORD_23 | KEYWORD_24 | KEYWORD_25 | KEYWORD_26 | KEYWORD_21 | KEYWORD_22 | KEYWORD_18 | KEYWORD_19 | KEYWORD_20 | KEYWORD_13 | KEYWORD_14 | KEYWORD_15 | KEYWORD_16 | KEYWORD_17 | KEYWORD_11 | KEYWORD_12 | KEYWORD_1 | KEYWORD_2 | KEYWORD_3 | KEYWORD_4 | KEYWORD_5 | KEYWORD_6 | KEYWORD_7 | KEYWORD_8 | KEYWORD_9 | KEYWORD_10 | RULE_WS | RULE_ML_COMMENT | RULE_SL_COMMENT | RULE_TERM | RULE_NL | RULE_STRING | RULE_STRING_HEX | RULE_STRING_BOOL | RULE_SYMBOL_VAR | RULE_SYMBOL_CLS | RULE_SYMBOL_NUM | RULE_OP_PLUS | RULE_OP_MINUS | RULE_OP_SLASH | RULE_OP_PERCENT | RULE_OP_STAR | RULE_OP_PIPE | RULE_OP_ET | RULE_OP_NEG | RULE_OP_GT | RULE_OP_LT | RULE_OP_GEQ | RULE_OP_LEQ | RULE_OP_BSGT | RULE_OP_BSLT | RULE_OP_BSEQ | RULE_OP_GTLT | RULE_OP_LTGT | RULE_OP_EQ2 | RULE_OP_BSEQ2 | RULE_OP_SLASH2 | RULE_OP_AND2 | RULE_OP_PIPE2 | RULE_OP_STAR2 | RULE_OP_NGT | RULE_OP_NLT | RULE_OP_NEQ | RULE_OP_NEQ2 | RULE_OP_GT2 | RULE_OP_LT2 | RULE_OP_GT2EQ | RULE_OP_BSLT2 | RULE_OP_NLT2 | RULE_OP_BSGT2 | RULE_OP_NGT2 | RULE_OP_LT2EQ | RULE_OP_PLUSEQ | RULE_OP_MINUSEQ | RULE_OP_STAREQ | RULE_OP_SLASHEQ | RULE_OP_PERCENTEQ | RULE_OP_SLAHS2EQ | RULE_OP_PIPE2EQ | RULE_OP_ANDEQ | RULE_OP_PIPEEQ | RULE_OP_AND2EQ )
        int alt11=94;
        alt11 = dfa11.predict(input);
        switch (alt11) {
            case 1 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:10: KEYWORD_38
                {
                mKEYWORD_38(); 

                }
                break;
            case 2 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:21: KEYWORD_35
                {
                mKEYWORD_35(); 

                }
                break;
            case 3 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:32: KEYWORD_36
                {
                mKEYWORD_36(); 

                }
                break;
            case 4 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:43: KEYWORD_37
                {
                mKEYWORD_37(); 

                }
                break;
            case 5 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:54: KEYWORD_32
                {
                mKEYWORD_32(); 

                }
                break;
            case 6 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:65: KEYWORD_33
                {
                mKEYWORD_33(); 

                }
                break;
            case 7 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:76: KEYWORD_34
                {
                mKEYWORD_34(); 

                }
                break;
            case 8 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:87: KEYWORD_30
                {
                mKEYWORD_30(); 

                }
                break;
            case 9 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:98: KEYWORD_31
                {
                mKEYWORD_31(); 

                }
                break;
            case 10 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:109: KEYWORD_27
                {
                mKEYWORD_27(); 

                }
                break;
            case 11 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:120: KEYWORD_28
                {
                mKEYWORD_28(); 

                }
                break;
            case 12 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:131: KEYWORD_29
                {
                mKEYWORD_29(); 

                }
                break;
            case 13 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:142: KEYWORD_23
                {
                mKEYWORD_23(); 

                }
                break;
            case 14 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:153: KEYWORD_24
                {
                mKEYWORD_24(); 

                }
                break;
            case 15 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:164: KEYWORD_25
                {
                mKEYWORD_25(); 

                }
                break;
            case 16 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:175: KEYWORD_26
                {
                mKEYWORD_26(); 

                }
                break;
            case 17 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:186: KEYWORD_21
                {
                mKEYWORD_21(); 

                }
                break;
            case 18 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:197: KEYWORD_22
                {
                mKEYWORD_22(); 

                }
                break;
            case 19 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:208: KEYWORD_18
                {
                mKEYWORD_18(); 

                }
                break;
            case 20 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:219: KEYWORD_19
                {
                mKEYWORD_19(); 

                }
                break;
            case 21 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:230: KEYWORD_20
                {
                mKEYWORD_20(); 

                }
                break;
            case 22 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:241: KEYWORD_13
                {
                mKEYWORD_13(); 

                }
                break;
            case 23 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:252: KEYWORD_14
                {
                mKEYWORD_14(); 

                }
                break;
            case 24 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:263: KEYWORD_15
                {
                mKEYWORD_15(); 

                }
                break;
            case 25 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:274: KEYWORD_16
                {
                mKEYWORD_16(); 

                }
                break;
            case 26 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:285: KEYWORD_17
                {
                mKEYWORD_17(); 

                }
                break;
            case 27 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:296: KEYWORD_11
                {
                mKEYWORD_11(); 

                }
                break;
            case 28 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:307: KEYWORD_12
                {
                mKEYWORD_12(); 

                }
                break;
            case 29 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:318: KEYWORD_1
                {
                mKEYWORD_1(); 

                }
                break;
            case 30 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:328: KEYWORD_2
                {
                mKEYWORD_2(); 

                }
                break;
            case 31 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:338: KEYWORD_3
                {
                mKEYWORD_3(); 

                }
                break;
            case 32 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:348: KEYWORD_4
                {
                mKEYWORD_4(); 

                }
                break;
            case 33 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:358: KEYWORD_5
                {
                mKEYWORD_5(); 

                }
                break;
            case 34 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:368: KEYWORD_6
                {
                mKEYWORD_6(); 

                }
                break;
            case 35 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:378: KEYWORD_7
                {
                mKEYWORD_7(); 

                }
                break;
            case 36 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:388: KEYWORD_8
                {
                mKEYWORD_8(); 

                }
                break;
            case 37 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:398: KEYWORD_9
                {
                mKEYWORD_9(); 

                }
                break;
            case 38 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:408: KEYWORD_10
                {
                mKEYWORD_10(); 

                }
                break;
            case 39 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:419: RULE_WS
                {
                mRULE_WS(); 

                }
                break;
            case 40 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:427: RULE_ML_COMMENT
                {
                mRULE_ML_COMMENT(); 

                }
                break;
            case 41 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:443: RULE_SL_COMMENT
                {
                mRULE_SL_COMMENT(); 

                }
                break;
            case 42 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:459: RULE_TERM
                {
                mRULE_TERM(); 

                }
                break;
            case 43 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:469: RULE_NL
                {
                mRULE_NL(); 

                }
                break;
            case 44 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:477: RULE_STRING
                {
                mRULE_STRING(); 

                }
                break;
            case 45 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:489: RULE_STRING_HEX
                {
                mRULE_STRING_HEX(); 

                }
                break;
            case 46 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:505: RULE_STRING_BOOL
                {
                mRULE_STRING_BOOL(); 

                }
                break;
            case 47 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:522: RULE_SYMBOL_VAR
                {
                mRULE_SYMBOL_VAR(); 

                }
                break;
            case 48 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:538: RULE_SYMBOL_CLS
                {
                mRULE_SYMBOL_CLS(); 

                }
                break;
            case 49 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:554: RULE_SYMBOL_NUM
                {
                mRULE_SYMBOL_NUM(); 

                }
                break;
            case 50 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:570: RULE_OP_PLUS
                {
                mRULE_OP_PLUS(); 

                }
                break;
            case 51 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:583: RULE_OP_MINUS
                {
                mRULE_OP_MINUS(); 

                }
                break;
            case 52 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:597: RULE_OP_SLASH
                {
                mRULE_OP_SLASH(); 

                }
                break;
            case 53 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:611: RULE_OP_PERCENT
                {
                mRULE_OP_PERCENT(); 

                }
                break;
            case 54 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:627: RULE_OP_STAR
                {
                mRULE_OP_STAR(); 

                }
                break;
            case 55 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:640: RULE_OP_PIPE
                {
                mRULE_OP_PIPE(); 

                }
                break;
            case 56 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:653: RULE_OP_ET
                {
                mRULE_OP_ET(); 

                }
                break;
            case 57 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:664: RULE_OP_NEG
                {
                mRULE_OP_NEG(); 

                }
                break;
            case 58 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:676: RULE_OP_GT
                {
                mRULE_OP_GT(); 

                }
                break;
            case 59 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:687: RULE_OP_LT
                {
                mRULE_OP_LT(); 

                }
                break;
            case 60 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:698: RULE_OP_GEQ
                {
                mRULE_OP_GEQ(); 

                }
                break;
            case 61 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:710: RULE_OP_LEQ
                {
                mRULE_OP_LEQ(); 

                }
                break;
            case 62 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:722: RULE_OP_BSGT
                {
                mRULE_OP_BSGT(); 

                }
                break;
            case 63 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:735: RULE_OP_BSLT
                {
                mRULE_OP_BSLT(); 

                }
                break;
            case 64 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:748: RULE_OP_BSEQ
                {
                mRULE_OP_BSEQ(); 

                }
                break;
            case 65 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:761: RULE_OP_GTLT
                {
                mRULE_OP_GTLT(); 

                }
                break;
            case 66 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:774: RULE_OP_LTGT
                {
                mRULE_OP_LTGT(); 

                }
                break;
            case 67 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:787: RULE_OP_EQ2
                {
                mRULE_OP_EQ2(); 

                }
                break;
            case 68 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:799: RULE_OP_BSEQ2
                {
                mRULE_OP_BSEQ2(); 

                }
                break;
            case 69 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:813: RULE_OP_SLASH2
                {
                mRULE_OP_SLASH2(); 

                }
                break;
            case 70 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:828: RULE_OP_AND2
                {
                mRULE_OP_AND2(); 

                }
                break;
            case 71 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:841: RULE_OP_PIPE2
                {
                mRULE_OP_PIPE2(); 

                }
                break;
            case 72 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:855: RULE_OP_STAR2
                {
                mRULE_OP_STAR2(); 

                }
                break;
            case 73 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:869: RULE_OP_NGT
                {
                mRULE_OP_NGT(); 

                }
                break;
            case 74 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:881: RULE_OP_NLT
                {
                mRULE_OP_NLT(); 

                }
                break;
            case 75 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:893: RULE_OP_NEQ
                {
                mRULE_OP_NEQ(); 

                }
                break;
            case 76 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:905: RULE_OP_NEQ2
                {
                mRULE_OP_NEQ2(); 

                }
                break;
            case 77 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:918: RULE_OP_GT2
                {
                mRULE_OP_GT2(); 

                }
                break;
            case 78 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:930: RULE_OP_LT2
                {
                mRULE_OP_LT2(); 

                }
                break;
            case 79 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:942: RULE_OP_GT2EQ
                {
                mRULE_OP_GT2EQ(); 

                }
                break;
            case 80 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:956: RULE_OP_BSLT2
                {
                mRULE_OP_BSLT2(); 

                }
                break;
            case 81 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:970: RULE_OP_NLT2
                {
                mRULE_OP_NLT2(); 

                }
                break;
            case 82 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:983: RULE_OP_BSGT2
                {
                mRULE_OP_BSGT2(); 

                }
                break;
            case 83 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:997: RULE_OP_NGT2
                {
                mRULE_OP_NGT2(); 

                }
                break;
            case 84 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1010: RULE_OP_LT2EQ
                {
                mRULE_OP_LT2EQ(); 

                }
                break;
            case 85 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1024: RULE_OP_PLUSEQ
                {
                mRULE_OP_PLUSEQ(); 

                }
                break;
            case 86 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1039: RULE_OP_MINUSEQ
                {
                mRULE_OP_MINUSEQ(); 

                }
                break;
            case 87 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1055: RULE_OP_STAREQ
                {
                mRULE_OP_STAREQ(); 

                }
                break;
            case 88 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1070: RULE_OP_SLASHEQ
                {
                mRULE_OP_SLASHEQ(); 

                }
                break;
            case 89 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1086: RULE_OP_PERCENTEQ
                {
                mRULE_OP_PERCENTEQ(); 

                }
                break;
            case 90 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1104: RULE_OP_SLAHS2EQ
                {
                mRULE_OP_SLAHS2EQ(); 

                }
                break;
            case 91 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1121: RULE_OP_PIPE2EQ
                {
                mRULE_OP_PIPE2EQ(); 

                }
                break;
            case 92 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1137: RULE_OP_ANDEQ
                {
                mRULE_OP_ANDEQ(); 

                }
                break;
            case 93 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1151: RULE_OP_PIPEEQ
                {
                mRULE_OP_PIPEEQ(); 

                }
                break;
            case 94 :
                // ../org.oorexx.workbench.edit/src-gen/org/oorexx/workbench/parser/antlr/lexer/InternalRexx.g:1:1166: RULE_OP_AND2EQ
                {
                mRULE_OP_AND2EQ(); 

                }
                break;

        }

    }


    protected DFA11 dfa11 = new DFA11(this);
    static final String DFA11_eotS =
        "\1\uffff\1\52\14\37\3\uffff\1\101\1\103\1\uffff\1\107\3\uffff\1"+
        "\113\1\116\7\uffff\1\124\1\126\1\131\1\134\1\137\1\143\1\147\1\153"+
        "\2\uffff\7\37\1\170\14\37\1\u0085\4\uffff\1\u0087\1\u0089\1\u008b"+
        "\2\uffff\1\u008d\6\uffff\1\u008e\1\uffff\1\u008e\7\uffff\1\u0093"+
        "\2\uffff\1\u0095\2\uffff\1\u0097\1\u0099\1\u009b\3\uffff\1\u009d"+
        "\3\uffff\1\u009f\6\uffff\5\37\1\u00a9\1\37\1\uffff\4\37\1\u00af"+
        "\4\37\1\u00b4\1\u00b5\1\u00b6\37\uffff\5\37\1\uffff\4\37\1\u00c0"+
        "\1\uffff\1\37\1\u00c2\1\37\1\u00c4\3\uffff\11\37\1\uffff\1\u00ce"+
        "\1\uffff\1\u00cf\1\uffff\3\37\1\u00d3\1\u00d4\2\37\1\u00d7\1\u00d8"+
        "\2\uffff\3\37\2\uffff\1\u00dc\1\u00dd\2\uffff\2\37\1\u00e0\2\uffff"+
        "\1\37\1\u00e2\1\uffff\1\u00e3\2\uffff";
    static final String DFA11_eofS =
        "\u00e4\uffff";
    static final String DFA11_minS =
        "\1\11\1\72\2\105\1\106\1\122\1\114\2\101\1\110\1\122\1\105\1\123"+
        "\1\117\3\uffff\1\41\1\75\1\uffff\1\74\3\uffff\1\52\1\55\1\12\2\uffff"+
        "\2\0\2\uffff\2\75\1\52\1\75\1\46\3\74\1\101\1\uffff\1\130\1\124"+
        "\1\102\1\122\1\116\1\124\1\110\1\41\1\111\1\102\1\120\1\123\1\104"+
        "\1\101\1\114\1\111\1\105\1\107\1\124\1\105\1\41\4\uffff\1\76\1\74"+
        "\1\75\2\uffff\1\75\5\uffff\1\0\1\102\1\0\1\47\7\uffff\1\75\2\uffff"+
        "\1\75\2\uffff\1\76\1\74\1\75\3\uffff\1\75\3\uffff\1\75\3\uffff\1"+
        "\105\1\uffff\1\114\1\111\1\101\1\103\1\111\1\124\1\41\1\105\1\uffff"+
        "\1\126\1\114\1\117\1\105\1\41\1\123\1\114\1\123\1\116\3\41\14\uffff"+
        "\1\0\22\uffff\1\116\1\103\1\114\1\103\1\101\1\uffff\1\122\1\101"+
        "\1\111\1\123\1\41\1\uffff\1\123\1\41\1\105\1\41\3\uffff\1\103\1"+
        "\114\1\101\1\124\1\130\1\111\1\124\1\103\1\105\1\uffff\1\41\1\uffff"+
        "\1\41\1\uffff\1\114\1\101\1\123\2\41\1\124\1\105\2\41\2\uffff\1"+
        "\101\2\123\2\uffff\2\41\2\uffff\2\123\1\41\2\uffff\1\123\1\41\1"+
        "\uffff\1\41\2\uffff";
    static final String DFA11_maxS =
        "\1\u00ac\1\72\1\151\1\171\1\156\1\165\1\170\1\154\1\141\1\150\1"+
        "\162\1\145\1\163\1\157\3\uffff\1\172\1\75\1\uffff\1\76\3\uffff\2"+
        "\75\1\12\2\uffff\2\ufffe\2\uffff\3\75\1\174\1\75\3\76\1\162\1\uffff"+
        "\1\170\1\164\1\142\1\162\1\156\1\164\1\150\1\172\1\151\1\142\1\160"+
        "\1\163\1\144\1\141\1\154\1\151\1\145\1\147\1\164\1\145\1\172\4\uffff"+
        "\1\76\1\74\1\75\2\uffff\1\75\5\uffff\1\ufffe\1\170\1\ufffe\1\170"+
        "\7\uffff\1\75\2\uffff\1\75\2\uffff\1\76\1\74\1\75\3\uffff\1\75\3"+
        "\uffff\1\75\3\uffff\1\157\1\uffff\1\157\1\151\1\141\1\143\1\151"+
        "\1\164\1\172\1\145\1\uffff\1\166\1\154\1\157\1\145\1\172\1\163\1"+
        "\154\1\163\1\156\3\172\14\uffff\1\ufffe\22\uffff\1\156\1\143\1\154"+
        "\1\143\1\141\1\uffff\1\162\1\141\1\151\1\163\1\172\1\uffff\1\163"+
        "\1\172\1\145\1\172\3\uffff\1\143\1\154\1\141\1\164\1\170\1\151\1"+
        "\164\1\143\1\145\1\uffff\1\172\1\uffff\1\172\1\uffff\1\154\1\141"+
        "\1\163\2\172\1\164\1\145\2\172\2\uffff\1\141\2\163\2\uffff\2\172"+
        "\2\uffff\2\163\1\172\2\uffff\1\163\1\172\1\uffff\1\172\2\uffff";
    static final String DFA11_acceptS =
        "\16\uffff\1\35\1\36\1\37\2\uffff\1\43\1\uffff\1\45\1\46\1\47\3\uffff"+
        "\2\52\2\uffff\1\57\1\61\11\uffff\1\41\25\uffff\1\60\1\40\1\103\1"+
        "\42\3\uffff\1\44\1\50\1\uffff\1\130\1\64\1\51\1\126\1\63\4\uffff"+
        "\1\125\1\62\1\131\1\65\1\110\1\127\1\66\1\uffff\1\135\1\67\1\uffff"+
        "\1\134\1\70\3\uffff\1\71\1\74\1\101\1\uffff\1\72\1\75\1\102\1\uffff"+
        "\1\73\1\1\1\5\1\uffff\1\10\10\uffff\1\34\14\uffff\1\33\1\122\1\76"+
        "\1\120\1\77\1\104\1\100\1\132\1\105\1\54\1\55\1\56\1\uffff\1\133"+
        "\1\107\1\136\1\106\1\123\1\111\1\121\1\112\1\114\1\113\1\117\1\115"+
        "\1\124\1\116\1\3\1\6\1\12\1\2\5\uffff\1\31\5\uffff\1\27\4\uffff"+
        "\1\26\1\30\1\32\11\uffff\1\24\1\uffff\1\23\1\uffff\1\25\11\uffff"+
        "\1\21\1\22\3\uffff\1\17\1\20\2\uffff\1\16\1\15\3\uffff\1\13\1\14"+
        "\2\uffff\1\11\1\uffff\1\7\1\4";
    static final String DFA11_specialS =
        "\u00e4\uffff}>";
    static final String[] DFA11_transitionS = {
            "\1\27\1\33\2\uffff\1\32\22\uffff\1\27\1\37\1\35\2\uffff\1\42"+
            "\1\45\1\36\1\16\1\17\1\43\1\41\1\20\1\31\1\21\1\30\12\40\1\1"+
            "\1\34\1\50\1\22\1\47\1\37\1\uffff\1\12\1\37\1\7\1\15\1\6\1\37"+
            "\1\13\1\37\1\4\3\37\1\2\2\37\1\5\1\37\1\10\1\3\1\11\1\14\5\37"+
            "\1\23\1\24\1\25\1\uffff\1\37\1\uffff\1\12\1\37\1\7\1\15\1\6"+
            "\1\37\1\13\1\37\1\4\3\37\1\2\2\37\1\5\1\37\1\10\1\3\1\11\1\14"+
            "\5\37\1\uffff\1\44\1\uffff\1\26\55\uffff\1\46",
            "\1\51",
            "\1\54\3\uffff\1\53\33\uffff\1\54\3\uffff\1\53",
            "\1\60\16\uffff\1\56\1\55\3\uffff\1\57\13\uffff\1\60\16\uffff"+
            "\1\56\1\55\3\uffff\1\57",
            "\1\62\7\uffff\1\61\27\uffff\1\62\7\uffff\1\61",
            "\1\63\2\uffff\1\64\34\uffff\1\63\2\uffff\1\64",
            "\1\66\1\uffff\1\67\11\uffff\1\65\23\uffff\1\66\1\uffff\1\67"+
            "\11\uffff\1\65",
            "\1\71\12\uffff\1\70\24\uffff\1\71\12\uffff\1\70",
            "\1\72\37\uffff\1\72",
            "\1\73\37\uffff\1\73",
            "\1\74\37\uffff\1\74",
            "\1\75\37\uffff\1\75",
            "\1\76\37\uffff\1\76",
            "\1\77\37\uffff\1\77",
            "",
            "",
            "",
            "\1\100\35\uffff\1\100\1\uffff\32\100\4\uffff\1\100\1\uffff\32"+
            "\100",
            "\1\102",
            "",
            "\1\105\1\106\1\104",
            "",
            "",
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
            "\1\154\1\uffff\1\160\11\uffff\1\157\1\uffff\1\155\2\uffff\1"+
            "\156\16\uffff\1\154\1\uffff\1\160\11\uffff\1\157\1\uffff\1\155"+
            "\2\uffff\1\156",
            "",
            "\1\161\37\uffff\1\161",
            "\1\162\37\uffff\1\162",
            "\1\163\37\uffff\1\163",
            "\1\164\37\uffff\1\164",
            "\1\165\37\uffff\1\165",
            "\1\166\37\uffff\1\166",
            "\1\167\37\uffff\1\167",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\171\37\uffff\1\171",
            "\1\172\37\uffff\1\172",
            "\1\173\37\uffff\1\173",
            "\1\174\37\uffff\1\174",
            "\1\175\37\uffff\1\175",
            "\1\176\37\uffff\1\176",
            "\1\177\37\uffff\1\177",
            "\1\u0080\37\uffff\1\u0080",
            "\1\u0081\37\uffff\1\u0081",
            "\1\u0082\37\uffff\1\u0082",
            "\1\u0083\37\uffff\1\u0083",
            "\1\u0084\37\uffff\1\u0084",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "",
            "",
            "\1\u0086",
            "\1\u0088",
            "\1\u008a",
            "",
            "",
            "\1\u008c",
            "",
            "",
            "",
            "",
            "",
            "\42\117\1\120\uffdc\117",
            "\1\u0090\25\uffff\1\u008f\11\uffff\1\u0090\25\uffff\1\u008f",
            "\47\121\1\122\uffd7\121",
            "\1\u0091\32\uffff\1\u0090\25\uffff\1\u008f\11\uffff\1\u0090"+
            "\25\uffff\1\u008f",
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
            "",
            "",
            "\1\u00a0\11\uffff\1\u00a1\25\uffff\1\u00a0\11\uffff\1\u00a1",
            "",
            "\1\u00a2\2\uffff\1\u00a3\34\uffff\1\u00a2\2\uffff\1\u00a3",
            "\1\u00a4\37\uffff\1\u00a4",
            "\1\u00a5\37\uffff\1\u00a5",
            "\1\u00a6\37\uffff\1\u00a6",
            "\1\u00a7\37\uffff\1\u00a7",
            "\1\u00a8\37\uffff\1\u00a8",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00aa\37\uffff\1\u00aa",
            "",
            "\1\u00ab\37\uffff\1\u00ab",
            "\1\u00ac\37\uffff\1\u00ac",
            "\1\u00ad\37\uffff\1\u00ad",
            "\1\u00ae\37\uffff\1\u00ae",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00b0\37\uffff\1\u00b0",
            "\1\u00b1\37\uffff\1\u00b1",
            "\1\u00b2\37\uffff\1\u00b2",
            "\1\u00b3\37\uffff\1\u00b3",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
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
            "",
            "",
            "",
            "",
            "\1\u00b7\37\uffff\1\u00b7",
            "\1\u00b8\37\uffff\1\u00b8",
            "\1\u00b9\37\uffff\1\u00b9",
            "\1\u00ba\37\uffff\1\u00ba",
            "\1\u00bb\37\uffff\1\u00bb",
            "",
            "\1\u00bc\37\uffff\1\u00bc",
            "\1\u00bd\37\uffff\1\u00bd",
            "\1\u00be\37\uffff\1\u00be",
            "\1\u00bf\37\uffff\1\u00bf",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "\1\u00c1\37\uffff\1\u00c1",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00c3\37\uffff\1\u00c3",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "",
            "\1\u00c5\37\uffff\1\u00c5",
            "\1\u00c6\37\uffff\1\u00c6",
            "\1\u00c7\37\uffff\1\u00c7",
            "\1\u00c8\37\uffff\1\u00c8",
            "\1\u00c9\37\uffff\1\u00c9",
            "\1\u00ca\37\uffff\1\u00ca",
            "\1\u00cb\37\uffff\1\u00cb",
            "\1\u00cc\37\uffff\1\u00cc",
            "\1\u00cd\37\uffff\1\u00cd",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "\1\u00d0\37\uffff\1\u00d0",
            "\1\u00d1\37\uffff\1\u00d1",
            "\1\u00d2\37\uffff\1\u00d2",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\u00d5\37\uffff\1\u00d5",
            "\1\u00d6\37\uffff\1\u00d6",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "\1\u00d9\37\uffff\1\u00d9",
            "\1\u00da\37\uffff\1\u00da",
            "\1\u00db\37\uffff\1\u00db",
            "",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "\1\u00de\37\uffff\1\u00de",
            "\1\u00df\37\uffff\1\u00df",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "",
            "\1\u00e1\37\uffff\1\u00e1",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
            "\1\37\14\uffff\1\37\1\uffff\12\37\5\uffff\1\37\1\uffff\32\37"+
            "\4\uffff\1\37\1\uffff\32\37",
            "",
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
            return "1:1: Tokens : ( KEYWORD_38 | KEYWORD_35 | KEYWORD_36 | KEYWORD_37 | KEYWORD_32 | KEYWORD_33 | KEYWORD_34 | KEYWORD_30 | KEYWORD_31 | KEYWORD_27 | KEYWORD_28 | KEYWORD_29 | KEYWORD_23 | KEYWORD_24 | KEYWORD_25 | KEYWORD_26 | KEYWORD_21 | KEYWORD_22 | KEYWORD_18 | KEYWORD_19 | KEYWORD_20 | KEYWORD_13 | KEYWORD_14 | KEYWORD_15 | KEYWORD_16 | KEYWORD_17 | KEYWORD_11 | KEYWORD_12 | KEYWORD_1 | KEYWORD_2 | KEYWORD_3 | KEYWORD_4 | KEYWORD_5 | KEYWORD_6 | KEYWORD_7 | KEYWORD_8 | KEYWORD_9 | KEYWORD_10 | RULE_WS | RULE_ML_COMMENT | RULE_SL_COMMENT | RULE_TERM | RULE_NL | RULE_STRING | RULE_STRING_HEX | RULE_STRING_BOOL | RULE_SYMBOL_VAR | RULE_SYMBOL_CLS | RULE_SYMBOL_NUM | RULE_OP_PLUS | RULE_OP_MINUS | RULE_OP_SLASH | RULE_OP_PERCENT | RULE_OP_STAR | RULE_OP_PIPE | RULE_OP_ET | RULE_OP_NEG | RULE_OP_GT | RULE_OP_LT | RULE_OP_GEQ | RULE_OP_LEQ | RULE_OP_BSGT | RULE_OP_BSLT | RULE_OP_BSEQ | RULE_OP_GTLT | RULE_OP_LTGT | RULE_OP_EQ2 | RULE_OP_BSEQ2 | RULE_OP_SLASH2 | RULE_OP_AND2 | RULE_OP_PIPE2 | RULE_OP_STAR2 | RULE_OP_NGT | RULE_OP_NLT | RULE_OP_NEQ | RULE_OP_NEQ2 | RULE_OP_GT2 | RULE_OP_LT2 | RULE_OP_GT2EQ | RULE_OP_BSLT2 | RULE_OP_NLT2 | RULE_OP_BSGT2 | RULE_OP_NGT2 | RULE_OP_LT2EQ | RULE_OP_PLUSEQ | RULE_OP_MINUSEQ | RULE_OP_STAREQ | RULE_OP_SLASHEQ | RULE_OP_PERCENTEQ | RULE_OP_SLAHS2EQ | RULE_OP_PIPE2EQ | RULE_OP_ANDEQ | RULE_OP_PIPEEQ | RULE_OP_AND2EQ );";
        }
    }
 

}