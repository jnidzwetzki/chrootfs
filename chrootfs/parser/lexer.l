%{
#include "tree.h"
#include "parser.h"
#include "parser_y.h" 

int gLineNumber = 1;

%}

%option noyywrap

%%

\/\/[^\n]*\n            { gLineNumber++; }
\n                      { gLineNumber++; }
\t
[ ]
hide                    { return T_HIDE; }
show_only_user          { return T_SHOW_ONLY_USER; }
\{                      { return T_OB; }
\}                      { return T_CB; }
.                       { return yytext[0]; }

\/[a-zA-Z0-9/-_\.]+ { 
   char *res = malloc(strlen(yytext) + 1 * sizeof(char));
   strcpy(res, yytext);
   yylval.sval = res;
   return T_PATH; 
}


%% 
void yyerror( const char* str )
{
        fprintf( stderr, "ERROR: %s (line %d)\n", str, gLineNumber );
}
