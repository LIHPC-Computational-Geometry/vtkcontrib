#include "VtkContrib/VtkContribInfos.h"

#include <assert.h>


const std::string VtkContribInfos::_version (VTK_CONTRIB_VERSION);


VtkContribInfos::VtkContribInfos ( )
{
	assert (0 && "VtkContribInfos::VtkContribInfos is not allowed.");
}


VtkContribInfos::VtkContribInfos (const VtkContribInfos&)
{
	assert (0 && "VtkContribInfos copy constructor is not allowed.");
}


VtkContribInfos& VtkContribInfos::operator = (const VtkContribInfos&)
{
	assert (0 && "VtkContribInfos operator = is not allowed.");
	return *this;
}


VtkContribInfos::~VtkContribInfos ( )
{
	assert (0 && "VtkContribInfos::~VtkContribInfos is not allowed.");
}


