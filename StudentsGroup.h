#include <list>
#include <string>

using namespace std;

class CourseClass;


class StudentsGroup
{

private:

	
	int _id;

	
	string _name;

	
	int _numberOfStudents;

	
	list<CourseClass*> _courseClasses;

public:

	
	StudentsGroup(int id, const string& name, int numberOfStudents);

	
	void AddClass(CourseClass* courseClass);

	
	inline int GetId() const { return _id; }

	
	inline const string& GetName() const { return _name; }

	
	inline int GetNumberOfStudents() const { return _numberOfStudents; }

	
	inline const list<CourseClass*>& GetCourseClasses() const { return _courseClasses; }

	
	inline bool operator ==(const StudentsGroup& rhs) const { return _id == rhs._id; }

};
