/*
    Copyright (C) 2018  Zaoqi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#define eoc_max 64

#define eq_p(x, y) ((x)==(y))
#define nat unsigned long long
#define string char*

/* 有 #define file "" */
#define echo(x) fputs(x, file)
#define echo_nat(x) fprintf(file, "%u", x)

#define IF(x) echo("#if ");{x}echo("\n");
#define ELSE echo("#else\n");
#define ELIF(x) echo("#elif ");{x}echo("\n");
#define ENDIF echo("#endif\n");
#define UNDEFINE(x) echo("#undef ");{x}echo("\n");
#define DEFINE(x, v) echo("#define ");{x}echo(" ");{v}echo("\n");
#define DEFINE_FUNCTION(name, args, v) echo("#define ");{name}echo("(");{args}echo(") ");{v}echo("\n");
#define ERROR(x) echo("#error ");{x}echo("\n");
#define LINE(x) {x}echo("\n");
#define INCLUDE(x) echo("#include ");{x}echo("\n");

#define String(x) echo("\"");echo(x);echo("\"");
#define X(x) echo(x);
#define Nat(x) echo_nat(x);
#define Defined(x) echo("defined(");{x}echo(")");
#define Not(x) echo("(!(");{x}echo("))");
#define Or(x, y) echo("((");{x}echo(")||(");{y}echo("))");
#define And(x, y) echo("((");{x}echo(")&&(");{y}echo("))");
#define Gt(x, y) echo("((");{x}echo(")>(");{y}echo("))");
#define GtEq(x, y) echo("((");{x}echo(")>=(");{y}echo("))");
#define Eq(x, y) echo("((");{x}echo(")==(");{y}echo("))");
#define Lt(x, y) echo("((");{x}echo(")<(");{y}echo("))");
#define LtEq(x, y) echo("((");{x}echo(")<=(");{y}echo("))");

#define CPlusPlus Defined(X("__cplusplus"))
#define StdC Defined(X("__STDC_VERSION__"))
#define StdC99 And(StdC,GtEq(X("__STDC_VERSION__"),X("199901L")))
#define StdC11 And(StdC,GtEq(X("__STDC_VERSION__"),X("201112L")))
#define CPlusPlus11 And(CPlusPlus,GtEq(X("__cplusplus"),X("201103L")))

#define WITH_MACRO_VA_ARGS(x) IF(Or(StdC99, CPlusPlus11)){x}ELSE ERROR(X("__VA_ARGS__ requires C99 or later or C++11 or later")) ENDIF

#define REDEFINE(x,v) UNDEFINE(x) DEFINE(x,v)
#define REDEFINE_FUNCTION(x,args,v) UNDEFINE(x) DEFINE_FUNCTION(x,args,v)

#define Call0(f) {f}X("()")
#define Call1(f, x) {f}X("("){x}X(")")
#define Call2(f, x, y) {f}X("("){x}X(","){y}X(")")
#define Call3(f, x, y, z) {f}X("("){x}X(","){y}X(","){z}X(")")

#define prefix_addbetween_from_to(prefix, addbetween, x, y) { \
	nat a=(x); \
	nat b=(y); \
	X(prefix); \
	Nat(a); \
	if(eq_p(a, b)){ \
	}else if(a<b){ \
		while(a!=b){ \
			a++;/*a<=b*/\
			X(addbetween prefix); \
			Nat(a);} \
	}else{/*a>b*/ \
		while(a!=b){ \
			a--;/*a>=b*/ \
			X(addbetween prefix); \
			Nat(a);}}}
#define var_from_to(x, y) prefix_addbetween_from_to("_", ",", x, y)
#define list_var_from_to(x, y) X("(")prefix_addbetween_from_to("_", ",", x, y)X(")")
#define from_to(x, y) prefix_addbetween_from_to("", ",", x, y)
#define for_in_from_to(v, x, y, body) { \
	nat for_in_from_toTEMPx=(x); \
	nat for_in_from_toTEMPy=(y); \
	{nat v=for_in_from_toTEMPx;{body}} \
	if(eq_p(for_in_from_toTEMPx, for_in_from_toTEMPy)){ \
	}else if(for_in_from_toTEMPx<for_in_from_toTEMPy){ \
		while(for_in_from_toTEMPx!=for_in_from_toTEMPy){ \
			for_in_from_toTEMPx++; \
			{nat v=for_in_from_toTEMPx;{body}}} \
	}else{ \
		while(for_in_from_toTEMPx!=for_in_from_toTEMPy){ \
			for_in_from_toTEMPx--; \
			{nat v=for_in_from_toTEMPx;{body}}}}}

#define HEADER(n, x) IF(Not(Defined(n))) DEFINE(n,) {x} ENDIF




/* 有 #define TOOLS_prefix "..." */
#define DEFINE_WITH_COUNT_2(ida,ider) \
	DEFINE_FUNCTION(ida,X("..."),Call1(X(TOOLS_prefix"expand"), \
		Call2(X(TOOLS_prefix"symbol_append_with_macro"), \
			ider, \
			Call1(X(TOOLS_prefix"count"),X("__VA_ARGS__"))) \
		X("(__VA_ARGS__)")))
#define DEFINE_WITH_COUNT(ida) DEFINE_WITH_COUNT_2(ida, ida)
#define TOOLS \
	HEADER(X(TOOLS_prefix"dEFINEd"),WITH_MACRO_VA_ARGS( \
		IF(StdC11) \
			DEFINE_FUNCTION(X(TOOLS_prefix"error"),X("x"),X("_Static_assert(0,x)")) \
		ELIF(CPlusPlus11) \
			DEFINE_FUNCTION(X(TOOLS_prefix"error"),X("x"),X("static_assert(0,x)")) \
		ELSE \
			DEFINE_FUNCTION(X(TOOLS_prefix"error"),X("x"),X("{{{!!!ERROR x ERROR!!!}}}")) \
		ENDIF \
		DEFINE_FUNCTION(X(TOOLS_prefix"expand"),X("x"),X("x")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"countHELPER0"),var_from_to(0,eoc_max)X(",x,..."),X("x")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"countHELPER1"),X("..."), \
			Call1(X(TOOLS_prefix"expand"),Call2(X(TOOLS_prefix"countHELPER0"),X("__VA_ARGS__"),from_to(eoc_max, 0)))) \
		DEFINE_FUNCTION(X(TOOLS_prefix"count"),X("..."), \
			Call2(X(TOOLS_prefix"countHELPER1"),X("_Nothing"),X("##__VA_ARGS__"))) \
		DEFINE_FUNCTION(X(TOOLS_prefix"symbol_append"),X("x,y"),X("x##y")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"symbol_append_with_macro"),X("x,y"),Call2(X(TOOLS_prefix"symbol_append"),X("x"),X("y"))) \
		DEFINE_FUNCTION(X(TOOLS_prefix"with_count"),X("ider,..."), \
			Call1(X(TOOLS_prefix"expand"), \
				Call2(X(TOOLS_prefix"symbol_append_with_macro"), \
					X("ider"), \
					Call1(X(TOOLS_prefix"count"),X("__VA_ARGS__"))) \
				X("(__VA_ARGS__)"))) \
		\
		DEFINE(X(TOOLS_prefix"count_assert0"),) \
		DEFINE(X(TOOLS_prefix"count_assert1"),Call1(X(TOOLS_prefix"error"), \
			String("this compiler does not offers an extension that allows ## to appear after a comma and before __VA_ARGS__ , in which case the ## does nothing when __VA_ARGS__ is non-empty, but removes the comma when __VA_ARGS__ is empty"))) \
		LINE(Call1(X(TOOLS_prefix"expand"), \
			Call2(X(TOOLS_prefix"symbol_append_with_macro"), \
				X(TOOLS_prefix"count_assert"), \
				Call0(X(TOOLS_prefix"count"))))) \
		\
		DEFINE_WITH_COUNT(X(TOOLS_prefix"init")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"init1"),X("x"),) \
		for_in_from_to(i, 2, eoc_max, { \
			nat isub=i-1; \
			DEFINE_FUNCTION(X(TOOLS_prefix"init")Nat(i),var_from_to(1,i),var_from_to(1,isub)) }) \
		\
		DEFINE_WITH_COUNT(X(TOOLS_prefix"last")) \
		for_in_from_to(i, 1, eoc_max, { \
			DEFINE_FUNCTION(X(TOOLS_prefix"last")Nat(i),var_from_to(1,i),X("_")Nat(i)) }) \
		\
		DEFINE_FUNCTION(X(TOOLS_prefix"unbracket"),X("..."),X("__VA_ARGS__")) \
		\
		DEFINE_FUNCTION(X(TOOLS_prefix"reduce"),X("f,xs"), Call1(X(TOOLS_prefix"expand"), \
			Call2(X(TOOLS_prefix"reduce_s"), X("f"), X(TOOLS_prefix"unbracket xs")))) \
		DEFINE_WITH_COUNT(X(TOOLS_prefix"reduce_s")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"reduce_s")Nat(2),X("f,x"),X("x")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"reduce_s")Nat(3),X("f,x,y"),X("f(x,y)")) \
		for_in_from_to(i, 4, eoc_max, { \
			nat length=i-1; \
			nat p1=length/2; \
			nat p2=length-p1; \
			nat p2base=i-p2+1; \
			DEFINE_FUNCTION(X(TOOLS_prefix"reduce_s")Nat(i),var_from_to(1,i),/*_1=f*/ \
				Call2(X("_1"), \
					Call1(X(TOOLS_prefix"reduce_s")Nat(p1+1),var_from_to(1,p1+1)), \
					Call2(X(TOOLS_prefix"reduce_s")Nat(p2+1),X("_1"),var_from_to(p2base,i)))) }) \
		\
		DEFINE_FUNCTION(X(TOOLS_prefix"map"),X("f,xs"), \
			Call3(X(TOOLS_prefix"if"),Call1(X(TOOLS_prefix"null_p"), X("xs")), \
				X("()"), \
				Call2(X(TOOLS_prefix"map_s"), X("f"), X(TOOLS_prefix"unbracket xs")))) \
		DEFINE_WITH_COUNT(X(TOOLS_prefix"map_s")) \
		for_in_from_to(i, 2, eoc_max, { \
			DEFINE_FUNCTION(X(TOOLS_prefix"map_s")Nat(i),var_from_to(1,i),/*_1=f*/ X("(_1(_2)") \
				for(nat k=3;k<=i;k++){/*不是for_in_from_to*/ \
					X(",_1(_")Nat(k)X(")")} \
				X(")"))}) \
		\
		DEFINE_FUNCTION(X(TOOLS_prefix"if"),X("b,x,y"), \
			Call1(X(TOOLS_prefix"expand"), \
				Call2(X(TOOLS_prefix"symbol_append_with_macro"), \
					X(TOOLS_prefix"if"), \
					X("b")) \
				X("(x,y)"))) \
		DEFINE_FUNCTION(X(TOOLS_prefix"if1"),X("x,y"), X("x")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"iftrue"),X("x,y"), X("x")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"if0"),X("x,y"), X("y")) \
		DEFINE_FUNCTION(X(TOOLS_prefix"iffalse"),X("x,y"), X("y")) \
		\
		DEFINE_FUNCTION(X(TOOLS_prefix"zero_p"),X("x"), \
			Call2(X(TOOLS_prefix"symbol_append_with_macro"), \
				X(TOOLS_prefix"zero_p"), \
				X("x"))) \
		DEFINE(X(TOOLS_prefix"zero_p")Nat(0), X("true")) \
		for_in_from_to(i, 1, eoc_max, { \
			DEFINE(X(TOOLS_prefix"zero_p")Nat(i), X("false"))}) \
		DEFINE_FUNCTION(X(TOOLS_prefix"null_p"), X("xs"), Call1(X(TOOLS_prefix"expand"), Call1(X(TOOLS_prefix"zero_p"), \
			X(TOOLS_prefix"count xs")))) \
	))

/* 有 #define REQUIRE_prefix "..." */
#define REQUIRE_BEGIN \
	for_in_from_to(i, 1, eoc_max-1, { \
		IF(Defined(X(REQUIRE_prefix)Nat(i))) }) \
	IF(Defined(X(REQUIRE_prefix)Nat(eoc_max))) \
		ERROR(X("Too many levels of require<")) \
	for_in_from_to(i, eoc_max, 1, { \
		ELSE \
		DEFINE(X(REQUIRE_prefix)Nat(i),) \
		ENDIF })
#define REQUIRE_END \
	for_in_from_to(i, 1, eoc_max, { \
		IF(Defined(X(REQUIRE_prefix)Nat(i))) }) \
	IF(Defined(X(REQUIRE_prefix)Nat(eoc_max+1))) \
		ERROR(X("Too many levels of require")) \
	for_in_from_to(i, eoc_max, 1, { \
		ELSE \
		UNDEFINE(X(REQUIRE_prefix)Nat(i)) \
		ENDIF }) \
	ELSE \
		ERROR(X("Too many levels of >require")) \
	ENDIF

/* 有
#define LANG_prefix "..."
#define LANG_EXPORT("...") ...

LANG_define => 全局定義
#define LANG_DEFINE_prefix "..."
#define LANG_DEFINE_EXPORT("...") ...
#
*/
#define LANG \
	WITH_MACRO_VA_ARGS( \
		HEADER(X(LANG_prefix"_static_dEFINEd"), \
			IF(CPlusPlus) \
				DEFINE(X(LANG_prefix"HELPERstaticDefine_inlineDefine"),X("static inline")) \
				DEFINE(X(LANG_prefix"HELPERexternDefine_inlineDefine"),X("inline")) \
				DEFINE(X(LANG_prefix"HELPERexternDeclare_inlineDefine"),X("inline")) \
			ELIF(StdC99) \
				DEFINE(X(LANG_prefix"HELPERstaticDefine_inlineDefine"),X("static inline")) \
				DEFINE(X(LANG_prefix"HELPERexternDefine_inlineDefine"),X("extern inline")) \
				DEFINE(X(LANG_prefix"HELPERexternDeclare_inlineDefine"),X("inline")) \
			ELSE \
				ERROR(X("inline requires C99 or later or C++")) \
			ENDIF \
			LANG_EXPORT("make_void") LINE(X(LANG_prefix"HELPERstaticDefine_inlineDefine void "LANG_prefix"make_void(void){}")) \
			\
			/* 例 define_type(var(a, int)) */\
			LANG_EXPORT("define_type") \
			DEFINE(X(LANG_prefix"define_type(x)"),X("typedef x;")) \
			\
			LANG_EXPORT("int8") LANG_EXPORT("int16") LANG_EXPORT("int32") LANG_EXPORT("int64") LANG_EXPORT("int_pointer") \
			LANG_EXPORT("nat8") LANG_EXPORT("nat16") LANG_EXPORT("nat32") LANG_EXPORT("nat64") LANG_EXPORT("nat_pointer") \
			LANG_EXPORT("byte") LANG_EXPORT("signed_byte") \
			IF(CPlusPlus11) \
				LANG_EXPORT("restrict") \
				DEFINE(X(LANG_prefix"restrict"),) \
				INCLUDE(X("<cstddef>")) \
				LINE(X("using std::size_t;")) \
				INCLUDE(X("<cstdint>")) \
				LINE(X("typedef std::int_least8_t "LANG_prefix"int8;")) \
				LINE(X("typedef std::int_least8_t "LANG_prefix"signed_byte;")) \
				LINE(X("typedef std::int_least16_t "LANG_prefix"int16;")) \
				LINE(X("typedef std::int_least32_t "LANG_prefix"int32;")) \
				LINE(X("typedef std::int_least64_t "LANG_prefix"int64;")) \
				LINE(X("typedef std::uint_least8_t "LANG_prefix"nat8;")) \
				LINE(X("typedef std::uint_least8_t "LANG_prefix"byte;")) \
				LINE(X("typedef std::uint_least16_t "LANG_prefix"nat16;")) \
				LINE(X("typedef std::uint_least32_t "LANG_prefix"nat32;")) \
				LINE(X("typedef std::uint_least64_t "LANG_prefix"nat64;")) \
				LINE(X("typedef std::uintptr_t "LANG_prefix"nat_pointer;")) \
				LINE(X("typedef std::intptr_t "LANG_prefix"int_pointer;")) \
			ELIF(StdC99) \
				INCLUDE(X("<stddef.h>")) \
				INCLUDE(X("<stdint.h>")) \
				LINE(X("typedef int_least8_t "LANG_prefix"int8;")) \
				LINE(X("typedef int_least8_t "LANG_prefix"signed_byte;")) \
				LINE(X("typedef int_least16_t "LANG_prefix"int16;")) \
				LINE(X("typedef int_least32_t "LANG_prefix"int32;")) \
				LINE(X("typedef int_least64_t "LANG_prefix"int64;")) \
				LINE(X("typedef uint_least8_t "LANG_prefix"nat8;")) \
				LINE(X("typedef uint_least8_t "LANG_prefix"byte;")) \
				LINE(X("typedef uint_least16_t "LANG_prefix"nat16;")) \
				LINE(X("typedef uint_least32_t "LANG_prefix"nat32;")) \
				LINE(X("typedef uint_least64_t "LANG_prefix"nat64;")) \
				LINE(X("typedef uintptr_t "LANG_prefix"nat_pointer;")) \
				LINE(X("typedef intptr_t "LANG_prefix"int_pointer;")) \
			ELSE \
				ERROR(X("<stdint.h> or <cstdint> requires C99 or later or C++11 or later")) \
			ENDIF \
			\
			LANG_EXPORT("cast") DEFINE_FUNCTION(X(LANG_prefix"cast"),X("x,t"),X("((t)x)")) \
			LANG_EXPORT("ann") DEFINE_FUNCTION(X(LANG_prefix"ann"), \
				X("x,t"),X("({t "LANG_prefix"tEMp_ann=x;"LANG_prefix"tEMp_annotate;})")) \
			LANG_EXPORT("bool_not") DEFINE_FUNCTION(X(LANG_prefix"bool_not"),X("x"),X("(!(x))")) \
			LANG_EXPORT("eq_p") DEFINE_FUNCTION(X(LANG_prefix"eq_p"),X("..."), \
				Call2(X(TOOLS_prefix"reduce_s"),X(LANG_prefix"eq_p2"),X("__VA_ARGS__"))) \
			DEFINE_FUNCTION(X(LANG_prefix"eq_p2"),X("x,y"),X("((x)==(y))")) \
			LANG_EXPORT("not_eq_p") DEFINE_FUNCTION(X(LANG_prefix"not_eq_p"),X("x,y"),X("((x)!=(y))")) \
			LANG_EXPORT("bool_and") DEFINE_FUNCTION(X(LANG_prefix"bool_and"),X("..."), \
				Call2(X(TOOLS_prefix"reduce_s"),X(LANG_prefix"bool_and2"),X("__VA_ARGS__"))) \
			DEFINE_FUNCTION(X(LANG_prefix"bool_and2"),X("x,y"),X("((x)&&(y))")) \
			LANG_EXPORT("bool_or") DEFINE_FUNCTION(X(LANG_prefix"bool_or"),X("..."), \
				Call2(X(TOOLS_prefix"reduce_s"),X(LANG_prefix"bool_or2"),X("__VA_ARGS__"))) \
			DEFINE_FUNCTION(X(LANG_prefix"bool_or2"),X("x,y"),X("((x)||(y))")) \
			\
			LANG_EXPORT("anonymous_struct") LANG_EXPORT("declare_struct") LANG_EXPORT("define_struct") \
			LANG_EXPORT("anonymous_union") LANG_EXPORT("declare_union") LANG_EXPORT("define_union") \
			LANG_EXPORT("anonymous_enumeration") LANG_EXPORT("declare_enumeration") LANG_EXPORT("define_enumeration") \
			DEFINE_FUNCTION(X(LANG_prefix"anonymous_struct"),X("x"),X("struct{x}")) \
			DEFINE_FUNCTION(X(LANG_prefix"anonymous_union"),X("x"),X("union{x}")) \
			DEFINE_FUNCTION(X(LANG_prefix"anonymous_enumeration"),X("x"),X("enum{x}")) \
			DEFINE_FUNCTION(X(LANG_prefix"define_strunienu_hELPEr"),X("..."),X("{__VA_ARGS__};")) \
			IF(CPlusPlus) \
				DEFINE_FUNCTION(X(LANG_prefix"declare_struct"),X("x"),X("struct x;")) \
				DEFINE_FUNCTION(X(LANG_prefix"define_struct"),X("x"),X("struct x "LANG_prefix"define_strunienu_hELPEr")) \
				DEFINE_FUNCTION(X(LANG_prefix"declare_union"),X("x"),X("union x;")) \
				DEFINE_FUNCTION(X(LANG_prefix"define_union"),X("x"),X("union x "LANG_prefix"define_strunienu_hELPEr")) \
				DEFINE_FUNCTION(X(LANG_prefix"declare_enumeration"),X("x"),X("enum x;")) \
				DEFINE_FUNCTION(X(LANG_prefix"define_enumeration"),X("x"),X("enum x "LANG_prefix"define_strunienu_hELPEr")) \
			ELSE \
				DEFINE_FUNCTION(X(LANG_prefix"declare_struct"),X("x"),X("struct x;typedef struct x x;")) \
				DEFINE_FUNCTION(X(LANG_prefix"define_struct"),X("x"), \
					Call1(X(LANG_prefix"declare_struct"),X("x"))X("struct x "LANG_prefix"define_strunienu_hELPEr")) \
				DEFINE_FUNCTION(X(LANG_prefix"declare_union"),X("x"),X("union x;typedef union x x;")) \
				DEFINE_FUNCTION(X(LANG_prefix"define_union"),X("x"), \
					Call1(X(LANG_prefix"declare_union"),X("x"))X("union x "LANG_prefix"define_strunienu_hELPEr")) \
				DEFINE_FUNCTION(X(LANG_prefix"declare_enumeration"),X("x"),X("enum x;typedef enum x x;")) \
				DEFINE_FUNCTION(X(LANG_prefix"define_enumeration"),X("x"), \
					Call1(X(LANG_prefix"declare_enumeration"),X("x"))X("enum x "LANG_prefix"define_strunienu_hELPEr")) \
			ENDIF \
			\
			IF(And(Defined(X("__bool_true_false_are_defined")),X("__bool_true_false_are_defined"))) \
			ELIF(CPlusPlus) \
			ELIF(StdC99) \
				INCLUDE(X("<stdbool.h>")) \
			ELSE \
				DEFINE(X(LANG_prefix"__bool_true_false_are_defined"),Nat(1)) \
				DEFINE(X(LANG_prefix"bool"),X("unsigned char")) \
				DEFINE(X(LANG_prefix"true"),Nat(1)) \
				DEFINE(X(LANG_prefix"false"),Nat(0)) \
			ENDIF \
			LANG_EXPORT_IFDEF("__bool_true_false_are_defined",LANG_prefix"__bool_true_false_are_defined") \
			LANG_EXPORT_IFDEF("bool",LANG_prefix"__bool_true_false_are_defined") \
			LANG_EXPORT_IFDEF("true",LANG_prefix"__bool_true_false_are_defined") \
			LANG_EXPORT_IFDEF("false",LANG_prefix"__bool_true_false_are_defined") \
			\
			LANG_EXPORT("let_current_continuation_notFirstClass") /*let_current_continuation_notFirstClass(name,type)( ... )*/\
			DEFINE_FUNCTION(X(LANG_prefix"let_current_continuation_notFirstClass"),X("ider,t"), X("({") \
				X("t "LANG_prefix"tEMp_letcc_notVal_val;") \
				X("t "LANG_prefix"tEMp_letcc_notVal_val_##ider;") \
				X("goto "LANG_prefix"tEMp_letcc_do_##ider;") \
				X(LANG_prefix"tEMp_letcc_throw_##ider:") \
				X(LANG_prefix"tEMp_letcc_notVal_val="LANG_prefix"tEMp_letcc_notVal_val_##ider;") \
				X("goto "LANG_prefix"tEMp_letcc_return;") \
				X("goto "LANG_prefix"tEMp_letcc_do_##ider:") \
				X(LANG_prefix"let_current_continuation_notFirstClass_hELPEr")) \
			DEFINE_FUNCTION(X(LANG_prefix"let_current_continuation_notFirstClass_hELPEr"),X("body"), \
				X(LANG_prefix"tEMp_letcc_notVal_val=({body});") \
				X(LANG_prefix"tEMp_letcc_return:") \
				X(LANG_prefix"tEMp_letcc_notVal_val;})")) \
			LANG_EXPORT("continuation_notFirstClass_throw") \
			DEFINE_FUNCTION(X(LANG_prefix"continuation_notFirstClass_throw"),X("ider,x,type"),X("({") \
				X(LANG_prefix"tEMp_letcc_notVal_val_##ider=x;") \
				X("goto "LANG_prefix"tEMp_letcc_throw_##ider;") \
				X("type tEMp;") \
				X("tEMp;})")) \
			\
			/* 例 lambda_s(int, int x)(x+y;) */\
			/* 例 lambda_s_withTypeOfBody(int, int x __ int)(x+y;) */\
			/* 例 lambda_withTypeOfBody((int, int x) __ int)(x+y;) */\
			/* 例 withTypeOfBody_lambda(int __(int, int x))(x+y;) */\
			DEFINE(X(LANG_prefix"lambda_error"),Call1(X(TOOLS_prefix"error"),String( \
				"lambda_s requires C++11 or later," \
				"lambda_s_withTypeOfBody, withTypeOfBody_lambda and lambda_withTypeOfBody requires GCC or C++11 or later" \
				))) \
			LANG_EXPORT("__") DEFINE(X(LANG_prefix"__"),X(",")) \
			LANG_EXPORT("lambda_s") \
			LANG_EXPORT("lambda_withTypeOfBody") \
			IF(CPlusPlus11) \
				DEFINE_FUNCTION(X(LANG_prefix"lambda_s"),X("..."),X("[&](__VA_ARGS__) "LANG_prefix"lambda_hELPEr")) \
				DEFINE_FUNCTION(X(LANG_prefix"lambda_s_withTypeOfBody"),X("..."), \
					X("[&](")Call1(X(TOOLS_prefix"init"),X("__VA_ARGS__"))X(")->") \
					Call1(X(TOOLS_prefix"last"),X("__VA_ARGS__"))X(" "LANG_prefix"lambda_hELPEr")) \
				DEFINE_FUNCTION(X(LANG_prefix"lambda_withTypeOfBody"),X("args,ret"),X("[&](args)->ret "LANG_prefix"lambda_hELPEr")) \
				DEFINE_FUNCTION(X(LANG_prefix"withTypeOfBody_lambda"),X("ret,args"),X("[&](args)->ret "LANG_prefix"lambda_hELPEr")) \
				DEFINE_FUNCTION(X(LANG_prefix"lambda_hELPEr"),X("x"),X("{return ({x});}")) \
			ELSE \
				DEFINE_FUNCTION(X(LANG_prefix"lambda_s"),X("..."),X(LANG_prefix"lambda_error")) \
				IF(Defined(X("__GNUC__"))) \
					DEFINE_FUNCTION(X(LANG_prefix"lambda_s_withTypeOfBody"),X("..."), \
						X("({")Call1(X(TOOLS_prefix"last"),X("__VA_ARGS__"))X(" ")X(LANG_prefix"lambda_withTypeOfBody_tEMp(") \
							Call1(X(TOOLS_prefix"init"),X("__VA_ARGS__"))X(")"LANG_prefix"lambda_withTypeOfBody_hELPEr")) \
					DEFINE_FUNCTION(X(LANG_prefix"lambda_withTypeOfBody"),X("args,ret"), \
						X("({ret "LANG_prefix"lambda_withTypeOfBody_tEMp(args)"LANG_prefix"lambda_withTypeOfBody_hELPEr")) \
					DEFINE_FUNCTION(X(LANG_prefix"withTypeOfBody_lambda"),X("ret,args"), \
						X("({ret "LANG_prefix"lambda_withTypeOfBody_tEMp(args)"LANG_prefix"lambda_withTypeOfBody_hELPEr")) \
					DEFINE_FUNCTION(X(LANG_prefix"lambda_withTypeOfBody_hELPEr"),X("x"), \
						X("{return ({x});}"LANG_prefix"lambda_withTypeOfBody_tEMp;})")) \
				ELSE \
					DEFINE_FUNCTION(X(LANG_prefix"lambda_s_withTypeOfBody"),X("..."),X(LANG_prefix"lambda_error")) \
					DEFINE_FUNCTION(X(LANG_prefix"lambda_withTypeOfBody"),X("..."),X(LANG_prefix"lambda_error")) \
					DEFINE_FUNCTION(X(LANG_prefix"withTypeOfBody_lambda"),X("..."),X(LANG_prefix"lambda_error")) \
				ENDIF \
			ENDIF \
			\
			LANG_EXPORT("var") LANG_EXPORT("var_lambda_s_withTypeOfBody") \
			DEFINE_WITH_COUNT(X(LANG_prefix"var")) \
			DEFINE_FUNCTION(X(LANG_prefix"var2"),X("ider,type"),X("type ider")) \
			DEFINE_FUNCTION(X(LANG_prefix"var_lambda_s_withTypeOfBody"),X("ider,..."), \
				Call1(X(TOOLS_prefix"last"),X("__VA_ARGS__")) \
				X(" (*ider)(")Call1(X(TOOLS_prefix"init"),X("__VA_ARGS__"))X(")")) \
			IF(CPlusPlus11) \
				DEFINE_FUNCTION(X(LANG_prefix"var1"),X("ider"),X("auto ider")) \
			ELSE \
				DEFINE_FUNCTION(X(LANG_prefix"var1"),X("ider"),Call1(X(TOOLS_prefix"error"), \
						String("auto requires C++11 or later"))) \
			ENDIF \
			\
			/*if_then_else(b)(...;)(...;)*/\
			LANG_EXPORT("if_then_else") \
			DEFINE_FUNCTION(X(LANG_prefix"if_then_else"),X("b"),X("((b)?({"LANG_prefix"if_then_else_hELPEr_A")) \
			DEFINE_FUNCTION(X(LANG_prefix"if_then_else_hELPEr_A"),X("x"),X("x}):({"LANG_prefix"if_then_else_hELPEr_B")) \
			DEFINE_FUNCTION(X(LANG_prefix"if_then_else_hELPEr_B"),X("x"),X("x}))")) \
			\
			LANG_EXPORT("_") \
			DEFINE(X(LANG_prefix"_"),X("()")) \
			/*case_const_number(x, type)(((a ...), ...;),((b ...), ...;)...) default为((), ...;)*/ \
			LANG_EXPORT("case_const_number") \
			DEFINE_FUNCTION(X(LANG_prefix"case_const_number"),X("x,type"),X("({")/*WIP*/ \
				X("type "LANG_prefix"case_const_number_tEMp;") \
				X("switch(x){") \
				X(LANG_prefix"case_const_number_hELPEr") \
				) \
			DEFINE_WITH_COUNT(X(LANG_prefix"case_const_number_hELPEr")) \
			for_in_from_to(i, 1, eoc_max, { \
				DEFINE_FUNCTION(X(LANG_prefix"case_const_number_hELPEr")Nat(i),var_from_to(1,i), \
					for_in_from_to(j, 1, i, { \
						X(LANG_prefix"case_const_number_hELPEr_each")X(" _")Nat(j)X(" ") \
					}) \
					/*分号为了多出来的逗号*/X(";}"LANG_prefix"case_const_number_tEMp;})"))}) \
			DEFINE_FUNCTION(X(LANG_prefix"case_const_number_hELPEr_each0"),X("x"), \
				Call1(X(TOOLS_prefix"expand"), X(LANG_prefix"case_const_number_hELPEr_each")X(" x"))) \
			DEFINE_FUNCTION(X(LANG_prefix"case_const_number_hELPEr_append"),X("x,y"),X("x y")) \
			LINE(X("static const int "LANG_prefix"case_const_number_hELPEr_each;"))/*多出来的逗号*/ \
			DEFINE_FUNCTION(X(LANG_prefix"case_const_number_hELPEr_each"),X("cond,..."), \
				Call3(X(TOOLS_prefix"if"),Call1(X(TOOLS_prefix"null_p"), X("cond")),/*匹配任何*/ \
					X("default:") \
					X(LANG_prefix"case_const_number_tEMp=")Call1(X(LANG_prefix"begin"),X("__VA_ARGS__"))X(";"), \
					\
					Call1(X(TOOLS_prefix"expand"),Call2(X(TOOLS_prefix"reduce"), X(LANG_prefix"case_const_number_hELPEr_append"),\
						Call1(X(TOOLS_prefix"expand"),Call2(X(TOOLS_prefix"map"),X(LANG_prefix"case_const_number_hELPEr_cond"),X("cond"))))) \
					X(LANG_prefix"case_const_number_tEMp=")Call1(X(LANG_prefix"begin"),X("__VA_ARGS__"))X(";break;") \
					)) \
			DEFINE_FUNCTION(X(LANG_prefix"case_const_number_hELPEr_cond"),X("x"), X("case x:")) \
			\
			/*begin(...;)*/\
			LANG_EXPORT("begin") \
			DEFINE_FUNCTION(X(LANG_prefix"begin"),X("x"),X("({x})")) \
			\
			DEFINE_FUNCTION(X(LANG_prefix"hELPEr_declare_lambda_s_withTypeOfBody"),X("prefix,ider,..."), \
				X("prefix ")Call1(X(TOOLS_prefix"last"),X("__VA_ARGS__")) \
				X(" ider(")Call1(X(TOOLS_prefix"init"),X("__VA_ARGS__"))X(");")) \
			DEFINE_FUNCTION(X(LANG_prefix"hELPEr_define_lambda_s_withTypeOfBody"),X("prefix,ider,..."), \
				X("prefix ")Call1(X(TOOLS_prefix"last"),X("__VA_ARGS__")) \
				X(" ider(")Call1(X(TOOLS_prefix"init"),X("__VA_ARGS__"))X(")") \
				X(LANG_prefix"hELPEr_define_lambda_hELPEr")) \
			DEFINE_FUNCTION(X(LANG_prefix"hELPEr_declare_lambda_withTypeOfBody"),X("prefix,iderargs,ret"), \
				X("prefix ret iderargs;")) \
			DEFINE_FUNCTION(X(LANG_prefix"hELPEr_define_lambda_withTypeOfBody"),X("prefix,iderargs,ret"), \
				X("prefix ret iderargs ") \
				X(LANG_prefix"hELPEr_define_lambda_hELPEr")) \
			DEFINE_FUNCTION(X(LANG_prefix"hELPEr_declare_withTypeOfBody_lambda"),X("prefix,retiderargs"), \
				X("prefix retiderargs;")) \
			DEFINE_FUNCTION(X(LANG_prefix"hELPEr_define_withTypeOfBody_lambda"),X("prefix,retiderargs"), \
				X("prefix retiderargs ") \
				X(LANG_prefix"hELPEr_define_lambda_hELPEr")) \
			DEFINE_FUNCTION(X(LANG_prefix"hELPEr_define_lambda_hELPEr"),X("x"),X("{return ({x});}")) \
			\
			LANG_EXPORT("declare_public") DEFINE_FUNCTION(X(LANG_prefix"declare_public"),X("typename"),X("extern typename;")) \
			LANG_EXPORT("define_private") DEFINE_WITH_COUNT(X(LANG_prefix"define_private")) \
			LANG_EXPORT("define_public") DEFINE_WITH_COUNT(X(LANG_prefix"define_public")) \
			LANG_EXPORT("declare_private") \
			\
			DEFINE_FUNCTION(X(LANG_prefix"ignore"),X("..."),) \
			LANG_NOT_STATIC_EXPORT \
		)/*HEADER(X(LANG_prefix"_static_dEFINEd"),*/ \
		LANG_NOT_STATIC \
	)
#define LANG_NOT_STATIC_EXPORT_do(x) \
	LANG_EXPORT("declare_"#x"_withTypeOfBody_lambda") \
	LANG_EXPORT("define_"#x"_withTypeOfBody_lambda") \
	LANG_EXPORT("declare_"#x"_lambda_withTypeOfBody") \
	LANG_EXPORT("define_"#x"_lambda_withTypeOfBody") \
	LANG_EXPORT("declare_"#x"_lambda_s_withTypeOfBody") \
	LANG_EXPORT("define_"#x"_lambda_s_withTypeOfBody")
#define LANG_NOT_STATIC_EXPORT \
	LANG_NOT_STATIC_EXPORT_do(public) \
	LANG_NOT_STATIC_EXPORT_do(private) \
	LANG_NOT_STATIC_EXPORT_do(public_inline) \
	LANG_NOT_STATIC_EXPORT_do(private_inline)
#define HELPER_declare_define_lambda_MACRO(x, prefix) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_lambda_withTypeOfBody"),X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_declare_lambda_withTypeOfBody"),X(prefix),X("__VA_ARGS__")))) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_lambda_withTypeOfBody"), X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_define_lambda_withTypeOfBody"),X(prefix),X("__VA_ARGS__")))) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_withTypeOfBody_lambda"),X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_declare_withTypeOfBody_lambda"),X(prefix),X("__VA_ARGS__")))) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_withTypeOfBody_lambda"), X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_define_withTypeOfBody_lambda"),X(prefix),X("__VA_ARGS__")))) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_lambda_s_withTypeOfBody"),X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_declare_lambda_s_withTypeOfBody"),X(prefix),X("__VA_ARGS__")))) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_lambda_s_withTypeOfBody"), X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_define_lambda_s_withTypeOfBody"),X(prefix),X("__VA_ARGS__"))))
#define HELPER_declare_defineIsDeclare_lambda_MACRO(x, prefix) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_lambda_withTypeOfBody"),X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_declare_lambda_withTypeOfBody"),X(prefix),X("__VA_ARGS__")))) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_lambda_withTypeOfBody"), X("..."), \
		Call1(X(LANG_prefix"declare_"#x"_lambda_withTypeOfBody"),X("__VA_ARGS__"))X(LANG_prefix"ignore")) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_withTypeOfBody_lambda"),X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_declare_withTypeOfBody_lambda"),X(prefix),X("__VA_ARGS__")))) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_withTypeOfBody_lambda"), X("..."), \
		Call1(X(LANG_prefix"declare_"#x"_withTypeOfBody_lambda"),X("__VA_ARGS__"))X(LANG_prefix"ignore")) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_lambda_s_withTypeOfBody"),X("..."), \
		Call1(X(TOOLS_prefix"expand"),Call2(X(LANG_prefix"hELPEr_declare_lambda_s_withTypeOfBody"),X(prefix),X("__VA_ARGS__")))) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_lambda_s_withTypeOfBody"), X("..."), \
		Call1(X(LANG_prefix"declare_"#x"_lambda_s_withTypeOfBody"),X("__VA_ARGS__"))X(LANG_prefix"ignore"))
#define HELPER_declareDefineAreNothing_lambda_MACRO(x) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_lambda_withTypeOfBody"),X("..."),) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_lambda_withTypeOfBody"),X("..."),X(LANG_prefix"ignore")) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_withTypeOfBody_lambda"),X("..."),) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_withTypeOfBody_lambda"),X("..."),X(LANG_prefix"ignore")) \
	REDEFINE_FUNCTION(X(LANG_prefix"declare_"#x"_lambda_s_withTypeOfBody"),X("..."),) \
	REDEFINE_FUNCTION(X(LANG_prefix"define_"#x"_lambda_s_withTypeOfBody"),X("..."),X(LANG_prefix"ignore"))
#define LANG_NOT_STATIC \
	IF(Not(And(Defined(X(LANG_prefix"is_require")), \
		Or( \
			And(Defined(X(REQUIRE_prefix)Nat(1)),X(LANG_prefix"is_require")), \
			And(Not(Defined(X(REQUIRE_prefix)Nat(1))),Not(X(LANG_prefix"is_require"))))))) \
		IF(Defined(X(REQUIRE_prefix)Nat(1))) \
			REDEFINE(X(LANG_prefix"is_require"),Nat(1)) \
			REDEFINE_FUNCTION(X(LANG_prefix"declare_private"),X("typename"),) \
			REDEFINE_FUNCTION(X(LANG_prefix"define_private1"),X("typename"),) \
			REDEFINE_FUNCTION(X(LANG_prefix"define_private2"),X("typename,val"),) \
			REDEFINE_FUNCTION(X(LANG_prefix"define_public1"),X("typename"),Call1(X(LANG_prefix"declare_public"),X("typename"))) \
			REDEFINE_FUNCTION(X(LANG_prefix"define_public2"),X("typename,val"),Call1(X(LANG_prefix"declare_public"),X("typename"))) \
			HELPER_declare_defineIsDeclare_lambda_MACRO(public, "extern") \
			HELPER_declareDefineAreNothing_lambda_MACRO(private) \
			HELPER_declare_define_lambda_MACRO(public_inline, LANG_prefix"HELPERexternDeclare_inlineDefine") \
			HELPER_declareDefineAreNothing_lambda_MACRO(private_inline) \
		ELSE \
			REDEFINE(X(LANG_prefix"is_require"),Nat(0)) \
			REDEFINE_FUNCTION(X(LANG_prefix"declare_private"),X("typename"),X("static typename;")) \
			REDEFINE_FUNCTION(X(LANG_prefix"define_private1"),X("typename"),X("static typename;")) \
			REDEFINE_FUNCTION(X(LANG_prefix"define_private2"),X("typename,val"),X("static typename=val;")) \
			REDEFINE_FUNCTION(X(LANG_prefix"define_public1"),X("typename"), \
				Call1(X(LANG_prefix"declare_public"),X("typename")) \
				X("typename;")) \
			REDEFINE_FUNCTION(X(LANG_prefix"define_public2"),X("typename,val"), \
				Call1(X(LANG_prefix"declare_public"),X("typename")) \
				X("typename=val;")) \
			HELPER_declare_define_lambda_MACRO(public, "extern") \
			HELPER_declare_define_lambda_MACRO(private, "static") \
			HELPER_declare_define_lambda_MACRO(public_inline, LANG_prefix"HELPERexternDefine_inlineDefine") \
			HELPER_declare_define_lambda_MACRO(private_inline, LANG_prefix"HELPERstaticDefine_inlineDefine") \
		ENDIF \
	ENDIF

void file_append(FILE* f,char* path){
	int c;
	FILE* tail=fopen(path, "rb");
	while ((c=fgetc(tail)) != EOF) {
		fputc(c,f);}
	fclose(tail);}

int main(){
	char* head=
"/* Generated by https://github.com/zaoqi/Expression-Oriented-C/blob/master/make.c */\n"
"/*\n"
"    Copyright (C) 2018  Zaoqi\n"
"\n"
"    This program is free software: you can redistribute it and/or modify\n"
"    it under the terms of the GNU Affero General Public License as published\n"
"    by the Free Software Foundation, either version 3 of the License, or\n"
"    (at your option) any later version.\n"
"\n"
"    This program is distributed in the hope that it will be useful,\n"
"    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"    GNU Affero General Public License for more details.\n"
"\n"
"    You should have received a copy of the GNU Affero General Public License\n"
"    along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
"*/\n";
	#define prefix "exprOrientedC_"
	#define TOOLS_prefix prefix"TOOLS_"
	#define LANG_prefix prefix"LANG_"
	#define REQUIRE_prefix prefix"REQUIRE_"
	#define EXTERNC_END prefix"EXTERNC" /* 表示定义未导出 */
	{FILE* f=fopen("module<", "w");fputs(head, f);
		{FILE* x=fopen("externC<", "w");fputs(head, x);fputs("#include \"require<\"\n", x);fclose(x);}
		#define doundef_path "undef.h"
		FILE* doundef=fopen(doundef_path, "w");
		#define doredef_path "redef.h"
		FILE* doredef=fopen(doredef_path, "w");
		#define file f
		TOOLS
		#define LANG_EXPORT(x) { \
			fputs("#undef "x"\n",doundef); \
			fputs("#define "x" "LANG_prefix x"\n",doredef); \
			DEFINE(X(x),X(LANG_prefix x))}
		#define LANG_EXPORT_IFDEF(x, m) { \
			fputs("#ifdef "m"\n#undef "x"\n#endif\n",doundef); \
			fputs("#ifdef "m"\n#define "x" "LANG_prefix x"\n#endif\n",doredef); \
			IF(Defined(X(m))) DEFINE(X(x),X(LANG_prefix x)) ENDIF}
		LANG
		fclose(doredef);
		fclose(doundef);
		{IF(Defined(X(EXTERNC_END)))
			UNDEFINE(X(EXTERNC_END))
			file_append(f, doredef_path);
		ENDIF}
		fclose(f);
		{
			FILE* externC_end=fopen(">externC", "w");
			fputs(head, externC_end);
			fputs("#include \">require\"\n"
				"#undef "EXTERNC_END"\n"
				"#define "EXTERNC_END"\n", externC_end);
			file_append(externC_end, doundef_path);
			fclose(externC_end);}}
	{FILE* x=fopen(">module", "w");fputs(head, x);fclose(x);}
	{
		{FILE* f=fopen("require<", "w");fputs(head, f);
			#undef file
			#define file f
			REQUIRE_BEGIN
			INCLUDE(String("module<"))
			fclose(f);}
		{FILE* f=fopen(">require", "w");fputs(head, f);
			#undef file
			#define file f
			REQUIRE_END
			INCLUDE(String("module<"))
			fclose(f);}}
}
