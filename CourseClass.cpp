#include "CourseClass.h"


CourseClass::CourseClass(Professor* professor, Course* course, const list<StudentsGroup*>& groups,
						 bool requiresLab, int duration) :
						 _professor(professor),
						 _course(course),
						 _numberOfSeats(0),
						 _requiresLab(requiresLab),
						 _duration(duration)
{

	_professor->AddCourseClass( this );


	for( list<StudentsGroup*>::const_iterator it = groups.begin(); it != groups.end(); it++ )
	{
		( *it )->AddClass( this );
		_groups.push_back( *it );
		_numberOfSeats += ( *it )->GetNumberOfStudents();
	}
}


CourseClass::~CourseClass() { }


bool CourseClass::GroupsOverlap(const CourseClass& c ) const
{
	for( list<StudentsGroup*>::const_iterator it1 = _groups.begin(); it1 != _groups.end(); it1++ )
	{
		for( list<StudentsGroup*>::const_iterator it2 = c._groups.begin(); it2 != c._groups.end(); it2++ )
		{
			if( *it1 == *it2 )
				return true;
		}
	}

	return false;
}
