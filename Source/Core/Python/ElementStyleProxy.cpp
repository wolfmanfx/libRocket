/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "precompiled.h"
#include "ElementStyleProxy.h"
#include <Rocket/Core/Property.h>
#include <Rocket/Core/Element.h>

namespace Rocket {
namespace Core {
namespace Python {

ElementStyleProxy::ElementStyleProxy(Element* _element)
{
	element = _element;
}

ElementStyleProxy::~ElementStyleProxy()
{
}

void ElementStyleProxy::InitialisePythonInterface()
{
	python::class_<ElementStyleProxy>("styleproxy", python::no_init)
		.def("__getattr__", &ElementStyleProxy::GetAttr, python::return_value_policy< python::return_by_value >())
		.def("__setattr__", &ElementStyleProxy::SetAttr)
		;
}

void ElementStyleProxy::SetAttr(const char* _key, const char* value)
{
	// Switch underscores to dashes, as the script interface can't use -'s
	EMP::Core::String key = EMP::Core::String(_key).Replace("_", "-");

	element->SetProperty(key.CString(), value);
}

EMP::Core::String ElementStyleProxy::GetAttr(const char* _key)
{
	// Switch underscores to dashes, as the script interface can't use -'s
	EMP::Core::String key = EMP::Core::String(_key).Replace("_", "-");

	const Property* property = element->GetProperty(key.CString());
	if ( !property )
	{
		EMP::Core::String message(128, "Invalid style property %s", _key);
		PyErr_SetString(PyExc_KeyError, message.CString());
		python::throw_error_already_set();
	}
	return property->ToString();
}

}
}
}
