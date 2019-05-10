
#include <bits/stdc++.h>
#include "CourseClass.h"

using namespace std;
using namespace stdext;

class CChildView;
class Schedule;
class Algorithm;


#define DAY_HOURS	12

#define DAYS_NUM	5

enum AlgorithmState
{
	AS_USER_STOPED,
	AS_CRITERIA_STOPPED,
	AS_RUNNING
};

class Schedule
{

	friend class ScheduleObserver;

private:


	int _numberOfCrossoverPoints;


	int _mutationSize;


	int _crossoverProbability;


	int _mutationProbability;


	float _fitness;


	vector<bool> _criteria;


	vector<list<CourseClass*>> _slots;



	hash_map<CourseClass*, int> _classes;

public:


	Schedule(int numberOfCrossoverPoints, int mutationSize,
		int crossoverProbability, int mutationProbability);


	Schedule(const Schedule& c, bool setupOnly);


	Schedule* MakeCopy(bool setupOnly) const;


	Schedule* MakeNewFromPrototype() const;


	Schedule* Crossover(const Schedule& parent2) const;


	void Mutation();


	void CalculateFitness();


	float GetFitness() const { return _fitness; }


	inline const hash_map<CourseClass*, int>& GetClasses() const { return _classes; }


	inline const vector<bool>& GetCriteria() const { return _criteria; }


	inline const vector<list<CourseClass*>>& GetSlots() const { return _slots; }

};


class Algorithm
{

private:


	vector<Schedule*> _chromosomes;


	vector<bool> _bestFlags;


	vector<int> _bestChromosomes;


	int _currentBestSize;


	int _replaceByGeneration;


	ScheduleObserver* _observer;


	Schedule* _prototype;


	int _currentGeneration;


	AlgorithmState _state;


	CCriticalSection _stateSect;


	static Algorithm* _instance;


	static CCriticalSection _instanceSect;

public:


	static Algorithm& GetInstance();


	static void FreeInstance();


	Algorithm(int numberOfChromosomes, int replaceByGeneration, int trackBest,
		Schedule* prototype, ScheduleObserver* observer);


	~Algorithm();


	void Start();


	void Stop();


	Schedule* GetBestChromosome() const;


	inline int GetCurrentGeneration() const { return _currentGeneration; }


	inline ScheduleObserver* GetObserver() const { return _observer; }

private:


	void AddToBest(int chromosomeIndex);


	bool IsInBest(int chromosomeIndex);


	void ClearBest();

};

