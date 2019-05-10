#include "Professor.h"


Professor::Professor(int id, const string& name) : _id(id),
												   _name(name) { }


void Professor::AddCourseClass(CourseClass* courseClass)
{
	_courseClasses.push_back( courseClass );
}

