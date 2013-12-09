#include "script.hpp"

#include <libtcc.h>

namespace library
{
	Script::Script()
	{
		this->state = tcc_new();
		
		if (this->state == nullptr)
		{
			throw std::string("Failed to initialize libtcc");
		}
	}
	
	Script::~Script()
	{
		if (this->state)
			tcc_delete(this->state);
			
		if (this->memory)
			free(this->memory);
	}
	
	void Script::setErrorFunc(error_func errfunc)
	{
		tcc_set_error_func(this->state, nullptr, errfunc);
	}
	
	void Script::compile(const std::string& program)
	{
		// memory output
		tcc_set_output_type(this->state, TCC_OUTPUT_MEMORY);
		tcc_compile_string(this->state, program.c_str());
		
		// resize/alloc memory to fit program
		if (this->memory == nullptr)
			this->memory = malloc(tcc_relocate(this->state, nullptr));
		else
			this->memory = realloc(this->memory, tcc_relocate(this->state, nullptr));
		
		// advertise location
		tcc_relocate(this->state, this->memory);
	}
	
	int Script::execute(const std::string& function)
	{
		typedef int (*intfunc) ();
		intfunc func = (intfunc) tcc_get_symbol(this->state, function.c_str());
		
		if (func)
			return func();
		else
			return -1;
	}
	
	int Script::execute(const std::string& function, void* data)
	{
		typedef int (*datafunc) (void* data);
		datafunc func = (datafunc) tcc_get_symbol(this->state, function.c_str());
		
		if (func)
			return func(data);
		else
			return -1;
	}
	
}

