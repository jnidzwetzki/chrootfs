%{
#include "parser.h"
#include "parser_y.h" 
%}

%%

\/\/[^\n]*\n
\n
\t
[ ]
hide { return T_HIDE; }
show_only_user { return T_SHOW_ONLY_USER; }
\{ { return T_OB; }
\} { return T_CB; }

\/[a-zA-Z0-9/-_\.]+ { 
   char *res = malloc(strlen(yytext) + 1 * sizeof(char));
   strcpy(res, yytext);
   yylval.sval = res;
   return T_PATH; 
}

%% 

