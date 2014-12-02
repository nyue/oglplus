/**
 *  @file oglplus/ext/ARB_debug_output.ipp
 *  @brief Implementation of the wrapper for ARB_debug_output
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/lib/incl_begin.ipp>
#include <oglplus/utils/xml.hpp>
#include <oglplus/lib/incl_end.ipp>

namespace oglplus {

#if GL_ARB_debug_output

OGLPLUS_LIB_FUNC
void ARB_debug_output_UniqueEssence::
Call(const ARB_debug_output::CallbackData& data)
{
	if(GLsizei(buffer.capacity()) < data.length)
	{
		buffer.resize(data.length);
	}
	buffer.assign(data.message, data.length);
	if(already_done.find(buffer) == already_done.end())
	{
		already_done.insert(buffer);
		_callback(data);
	}
}

OGLPLUS_LIB_FUNC
ARB_debug_output_TreeEssence::
ARB_debug_output_TreeEssence(std::ostream& out)
 : dbgout(out)
{
	dbgout << "-+-[Begin]" << std::endl;
}

OGLPLUS_LIB_FUNC
ARB_debug_output_TreeEssence::
~ARB_debug_output_TreeEssence(void)
{
	dbgout << " `-[Done]" << std::endl;
}


OGLPLUS_LIB_FUNC
void ARB_debug_output_TreeEssence::
Call(const ARB_debug_output::CallbackData& data)
{
	dbgout << " |" << std::endl;
	dbgout << " +-+-[" << data.id << "] '" <<
		data.message << "'" << std::endl;
	dbgout << " | +---[source]   '" <<
		EnumValueName(data.source)  << "'" << std::endl;
	dbgout << " | +---[type]     '" <<
		EnumValueName(data.type)  << "'" << std::endl;
	dbgout << " | `---[severity] '" <<
		EnumValueName(data.severity)  << "'" << std::endl;
}

OGLPLUS_LIB_FUNC
ARB_debug_output_ToXMLEssence::
ARB_debug_output_ToXMLEssence(std::ostream& out)
 : dbgout(out)
{
	dbgout << "<?xml version='1.0' encoding='UTF-8'?>" << std::endl;
	dbgout << "<ARB_debug_output>" << std::endl;
}

OGLPLUS_LIB_FUNC
ARB_debug_output_ToXMLEssence::
~ARB_debug_output_ToXMLEssence(void)
{
	dbgout << "</ARB_debug_output>" << std::endl;
}


OGLPLUS_LIB_FUNC
void ARB_debug_output_ToXMLEssence::
Call(const ARB_debug_output::CallbackData& data)
{
	dbgout << "<entry>" << std::endl;
	dbgout << "<id>" << data.id << "</id>" << std::endl;
	dbgout
		<< "<message>";
	aux::xml_text_to_stream(data.message, data.length, dbgout)
		<< "</message>"
		<< std::endl;
	dbgout
		<< "<source>"
		<< EnumValueName(data.source)
		<< "</source>"
		<< std::endl;
	dbgout
		<< "<type>"
		<< EnumValueName(data.type)
		<< "</type>"
		<< std::endl;
	dbgout
		<< "<severity>"
		<< EnumValueName(data.severity)
		<< "</severity>"
		<< std::endl;

	dbgout << "</entry>" << std::endl;
}

#endif // ARB_debug_output

} // namespace oglplus

