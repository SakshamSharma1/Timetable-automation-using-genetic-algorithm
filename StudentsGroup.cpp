#include "StudentsGroup.h"


StudentsGroup::StudentsGroup(int id, const string& name, int numberOfStudents) : _id(id),
																				 _name(name),
																				 _numberOfStudents(numberOfStudents)
{
}


void StudentsGroup::AddClass(CourseClass* courseClass)
{
	_courseClasses.push_back( courseClass );
}
