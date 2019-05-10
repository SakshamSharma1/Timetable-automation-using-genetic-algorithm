#include <list>
#include <string>

using namespace std;

class CourseClass;


class Professor
{

private:

	
	int _id;

	
	string _name;

	
	list<CourseClass*> _courseClasses;

public:

	
	Professor(int id, const string& name);

	
	void AddCourseClass(CourseClass* courseClass);

	
	inline int GetId() const { return _id; }

	
	inline const string& GetName() const { return _name; }

	
	inline const list<CourseClass*>& GetCourseClasses() const { return _courseClasses; }

	
	inline bool operator ==(const Professor& rhs) const { return _id == rhs._id; }

};
