#!/bin/bash
#    Copyright (C) 2018  Zaoqi

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Affero General Public License as published
#    by the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Affero General Public License for more details.

#    You should have received a copy of the GNU Affero General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

# 参考https://blog.csdn.net/u011787119/article/details/53815950
if [ -z "$prefix" ]
then
	prefix=EOC_HELPER_
fi

max=64

from_to(){
	eval echo {$1..$2}
}
prefix_from_to(){
	eval echo $1{$2..$3}
}
prefix_from_to_suffix(){
	eval echo $1{$2..$3}$4
}

define(){
	echo "#define $prefix$*"
}
defn(){
	echo -n "#define $prefix$1("
	shift
	local temp="$1"
	shift
	local first="true"
	while true
	do
		if [ -z "$1" ]
		then
			echo ") $temp"
			return
		else
			if [ -n "$first" ]
			then
				first=""
			else
				echo -n ","
			fi
			echo -n "$temp"
			temp="$1"
			shift
		fi
	done
}
add_between(){
	local x="$1"
	shift
	echo -n "$1"
	shift
	while [ -n "$1" ]
	do
		echo -n "$x"
		echo -n "$1"
		shift
	done
	echo
}
call(){
	echo -n "$prefix$1("
	shift
	local first="true"
	while [ -n "$1" ]
	do
		if [ -n "$first" ]
		then
			first=""
		else
			echo -n ","
		fi
		echo -n "$1"
		shift
	done
	echo -n ")"
}
S(){
	echo "$prefix$*"
}
symbol_append(){
	if [ -z "$2" ]
	then
		echo "$1"
	else
		local x="$1"
		shift
		echo $(call symbol_append_with_macro "$x" $(symbol_append "$@"))
	fi
}
expand(){
	call expand "$*"
}
ifdef(){
	echo "#ifdef $*"
}
el(){
	echo else
}
endif(){
	echo endif
}
defn error x "{{{!!!ERROR x ERROR!!!}}}"
defn expand x x
defn count_helper0 $(prefix_from_to _ 0 $max) x ... x
defn count_helper1 ... $(expand $(call count_helper0 __VA_ARGS__ $(from_to $max 0)))
defn count ... $(call count_helper1 _Nothing '##__VA_ARGS__')
echo '#define EOC_HELPER_count_assert0()
#define EOC_HELPER_count_assert1() EOC_HELPER_error("this compiler does not offers an extension that allows ## to appear after a comma and before __VA_ARGS__ , in which case the ## does nothing when __VA_ARGS__ is non-empty, but removes the comma when __VA_ARGS__ is empty")
EOC_HELPER_expand(EOC_HELPER_symbol_append_with_macro(EOC_HELPER_count_assert, EOC_HELPER_count()) ())'
defn symbol_append x y "x##y"
defn symbol_append_with_macro x y $(call symbol_append x y)
defn with_count ider ... $(expand $(symbol_append ider $(call count __VA_ARGS__))'(__VA_ARGS__)')

defn if b x y $(expand $(symbol_append $(S if) b)'(x,y)')
defn if1 x y x
defn iftrue x y x
defn if0 x y y
defn iffalse x y y

defn tail ... $(call with_count $(S tail) __VA_ARGS__)
for i in $(from_to 1 $max)
do
	defn tail$i $(prefix_from_to _ 1 $i) _$i
done

defn init ... $(call with_count $(S init) __VA_ARGS__)
defn init1 x ' '
for i in $(from_to 2 $max)
do
	let isub=i-1
	defn init$i $(prefix_from_to _ 1 $i) $(add_between , $(prefix_from_to _ 1 $isub))
done

defn zero_p x $(symbol_append $(S zero_p) x)
define zero_p0 true
for i in $(from_to 1 $max)
do
	define zero_p$i false
done
