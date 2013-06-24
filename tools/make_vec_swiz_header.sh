#!/bin/bash
# Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
RootDir=${1:-${PWD}}


function PrintFileHeader()
{
	echo "/*"
	echo " *  .file ${2##${RootDir}/}"
	echo " *"
	echo " *  Automatically generated header file. DO NOT modify manually,"
	echo " *  edit '${1##${RootDir}/}' instead."
	echo " *"
	echo " *  Copyright 2010-$(date +%Y) Matus Chochlik. Distributed under the Boost"
	echo " *  Software License, Version 1.0. (See accompanying file"
	echo " *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
	echo " */"
	echo
}

function Combinations()
{
	[[ ${#1} -lt 2 ]] && return
	local Coords=${1}
	local SepCoords=$(echo ${Coords} | sed 's/./,&/g')
	local CombExpr=$(
		for I in $(seq 1 ${2})
		do echo -n "{${SepCoords:1}}"
		done
	)
	eval "echo ${CombExpr}"
}

function MakeSwizzle()
{
	local Coords=${1}
	local MaxN=${#Coords}

	echo "template <typename T, std::size_t N>"
	echo "class Swizzled_${Coords};"
	for N in $(seq 1 ${MaxN})
	do
		echo
		echo "template <typename T>"
		echo "class Swizzled_${Coords}<T, ${N}>"
		echo " : public Vector<T, ${N}>"
		echo "{"
		echo "public:"
		echo "	Swizzled_${Coords}(Vector<T, ${N}> v)"
		echo "	 : Vector<T, ${N}>(v)"
		echo "	{ }"
		echo
		for I in $(seq 1 ${N})
		do
			echo "	T ${Coords:$[I-1]:1}(void) const " \
				"{ return this->template At($[I-1]); }"
		done

		for I in $(seq 2 ${N})
		do
			echo
			for Comb in $(Combinations ${Coords:0:${N}} ${I})
			do
				echo -n "	Vector<T, ${#Comb}> ${Comb}(void) const" \
					"{ return Vector<T, ${#Comb}>("
				echo ${Comb} | sed 's/./&(),/g' | sed 's/,$/); }/'
			done
		done
		echo "};"
	done
	echo
	echo "template <typename T, std::size_t N>"
	echo "inline Swizzled_${Coords}<T, N> Swizzle_${Coords}(Vector<T, N> v)"
	echo "{"
	echo "	return Swizzled_${Coords}<T, N>(v);"
	echo "}"
}

function MakeSwizzlePythonBindings()
{
	local Coords=${1}
	local MaxN=${#Coords}

	for N in $(seq 1 ${MaxN})
	do
		echo
		echo "template <typename T>"
		echo "void oglplus_py_export_Vector_Swizzle_xyzw("
		echo "	oglplus::Vector<T, ${N}>*,"
		echo "	const char* name"
		echo ")"
		echo "{"
		echo "	namespace bpy = ::boost::python;"
		echo "	using namespace oglplus;"
		echo
		echo "	typedef oglplus::Swizzled_${Coords}<T, ${N}> Swizzled;"
		echo "	typedef oglplus::Vector<T, ${N}> Vector;"
		echo "	bpy::class_<Swizzled, bpy::bases<Vector> >(name, bpy::init<Vector>())"
		for I in $(seq 1 ${N})
		do echo "		.def(\"${Coords:$[I-1]:1}\", &Swizzled::${Coords:$[I-1]:1})"
		done
		for I in $(seq 2 ${N})
		do
			for Comb in $(Combinations ${Coords:0:${N}} ${I})
			do echo "		.def(\"${Comb}\", &Swizzled::${Comb})"
			done
		done
		echo "	;"
		echo "	bpy::def(\"Swizzle_${Coords}\", &Swizzle_${Coords}<T, ${N}>);"
		echo "}"
	done
}

# C++
OutputDir="${RootDir}/include/oglplus/auxiliary"
mkdir -p ${OutputDir}
Output="${OutputDir}/vector_swizzle.ipp"

(
	exec > ${Output}
	PrintFileHeader tools/$(basename $0) ${Output}
	MakeSwizzle xyzw
	echo
	echo "template <typename T, std::size_t N>"
	echo "inline Swizzled_xyzw<T, N> Swizzle(Vector<T, N> v)"
	echo "{"
	echo "	return Swizzled_xyzw<T, N>(v);"
	echo "}"
) &&
git add ${Output}

# Python bindings
OutputDir="${RootDir}/source/bindings/python/oglplus"
mkdir -p ${OutputDir}
Output="${OutputDir}/vector_swizzle.ipp"

(
	exec > ${Output}
	PrintFileHeader tools/$(basename $0) ${Output}
	MakeSwizzlePythonBindings xyzw
) &&
git add ${Output}

