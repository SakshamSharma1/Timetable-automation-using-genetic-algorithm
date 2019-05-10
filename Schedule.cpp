#include <iostream>
#include <vector>
#include "Schedule.h"
#include "Room.h"

Schedule::Schedule(int numberOfCrossoverPoints, int mutationSize,
				   int crossoverProbability, int mutationProbability) : _mutationSize(mutationSize),
				   _numberOfCrossoverPoints(numberOfCrossoverPoints),
				   _crossoverProbability(crossoverProbability),
				   _mutationProbability(mutationProbability),
				   _fitness(0)
{

	_slots.resize( DAYS_NUM * DAY_HOURS * Configuration::GetInstance().GetNumberOfRooms() );


	_criteria.resize( Configuration::GetInstance().GetNumberOfCourseClasses() * 5 );
}


Schedule::Schedule(const Schedule& c, bool setupOnly)
{
	if( !setupOnly )
	{

		_slots = c._slots;
		_classes = c._classes;


		_criteria = c._criteria;


		_fitness = c._fitness;
	}
	else
	{

		_slots.resize( DAYS_NUM * DAY_HOURS * Configuration::GetInstance().GetNumberOfRooms() );


		_criteria.resize( Configuration::GetInstance().GetNumberOfCourseClasses() * 5 );
	}


	_numberOfCrossoverPoints = c._numberOfCrossoverPoints;
	_mutationSize = c._mutationSize;
	_crossoverProbability = c._crossoverProbability;
	_mutationProbability = c._mutationProbability;
}


Schedule* Schedule::MakeCopy(bool setupOnly) const
{

	return new Schedule( *this, setupOnly );
}


Schedule* Schedule::MakeNewFromPrototype() const
{

	int size = (int)_slots.size();


	Schedule* newChromosome = new Schedule( *this, true );


	const list<CourseClass*>& c = Configuration::GetInstance().GetCourseClasses();
	for( list<CourseClass*>::const_iterator it = c.begin(); it != c.end(); it++ )
	{

		int nr = Configuration::GetInstance().GetNumberOfRooms();
		int dur = ( *it )->GetDuration();
		int day = rand() % DAYS_NUM;
		int room = rand() % nr;
		int time = rand() % ( DAY_HOURS + 1 - dur );
		int pos = day * nr * DAY_HOURS + room * DAY_HOURS + time;


		for( int i = dur - 1; i >= 0; i-- )
			newChromosome->_slots.at( pos + i ).push_back( *it );


		newChromosome->_classes.insert( pair<CourseClass*, int>( *it, pos ) );
	}

	newChromosome->CalculateFitness();


	return newChromosome;
}


Schedule* Schedule::Crossover(const Schedule& parent2) const
{

	if( rand() % 100 > _crossoverProbability )

		return new Schedule( *this, false );


	Schedule* n = new Schedule( *this, true );


	int size = (int)_classes.size();

	vector<bool> cp( size );


	for( int i = _numberOfCrossoverPoints; i > 0; i-- )
	{
		while( 1 )
		{
			int p = rand() % size;
			if( !cp[ p ] )
			{
				cp[ p ] = true;
				break;
			}
		}
	}

	hash_map<CourseClass*, int>::const_iterator it1 = _classes.begin();
	hash_map<CourseClass*, int>::const_iterator it2 = parent2._classes.begin();


	bool first = rand() % 2 == 0;
	for( int i = 0; i < size; i++ )
	{
		if( first )
		{

			n->_classes.insert( pair<CourseClass*, int>( ( *it1 ).first, ( *it1 ).second ) );

			for( int i = ( *it1 ).first->GetDuration() - 1; i >= 0; i-- )
				n->_slots[ ( *it1 ).second + i ].push_back( ( *it1 ).first );
		}
		else
		{

			n->_classes.insert( pair<CourseClass*, int>( ( *it2 ).first, ( *it2 ).second ) );

			for( int i = ( *it2 ).first->GetDuration() - 1; i >= 0; i-- )
				n->_slots[ ( *it2 ).second + i ].push_back( ( *it2 ).first );
		}


		if( cp[ i ] )

			first = !first;

		it1++;
		it2++;
	}

	n->CalculateFitness();


	return n;
}


void Schedule::Mutation()
{

	if( rand() % 100 > _mutationProbability )
		return;


	int numberOfClasses = (int)_classes.size();

	int size = (int)_slots.size();


	for( int i = _mutationSize; i > 0; i-- )
	{

		int mpos = rand() % numberOfClasses;
		int pos1 = 0;
		hash_map<CourseClass*, int>::iterator it = _classes.begin();
		for( ; mpos > 0; it++, mpos-- )
			;


		pos1 = ( *it ).second;

		CourseClass* cc1 = ( *it ).first;


		int nr = Configuration::GetInstance().GetNumberOfRooms();
		int dur = cc1->GetDuration();
		int day = rand() % DAYS_NUM;
		int room = rand() % nr;
		int time = rand() % ( DAY_HOURS + 1 - dur );
		int pos2 = day * nr * DAY_HOURS + room * DAY_HOURS + time;


		for( int i = dur - 1; i >= 0; i-- )
		{

			list<CourseClass*>& cl = _slots[ pos1 + i ];
			for( list<CourseClass*>::iterator it = cl.begin(); it != cl.end(); it++ )
			{
				if( *it == cc1 )
				{
					cl.erase( it );
					break;
				}
			}


			_slots.at( pos2 + i ).push_back( cc1 );
		}


		_classes[ cc1 ] = pos2;
	}

	CalculateFitness();
}


void Schedule::CalculateFitness()
{

	int score = 0;

	int numberOfRooms = Configuration::GetInstance().GetNumberOfRooms();
	int daySize = DAY_HOURS * numberOfRooms;

	int ci = 0;


	for( hash_map<CourseClass*, int>::const_iterator it = _classes.begin(); it != _classes.end(); ++it, ci += 5 )
	{

		int p = ( *it ).second;
		int day = p / daySize;
		int time = p % daySize;
		int room = time / DAY_HOURS;
		time = time % DAY_HOURS;

		int dur = ( *it ).first->GetDuration();


		bool ro = false;
		for( int i = dur - 1; i >= 0; i-- )
		{
			if( _slots[ p + i ].size() > 1 )
			{
				ro = true;
				break;
			}
		}


		if( !ro )
			score++;

		_criteria[ ci + 0 ] = !ro;

		CourseClass* cc = ( *it ).first;
		Room* r = Configuration::GetInstance().GetRoomById( room );

		_criteria[ ci + 1 ] = r->GetNumberOfSeats() >= cc->GetNumberOfSeats();
		if( _criteria[ ci + 1 ] )
			score++;


		_criteria[ ci + 2 ] = !cc->IsLabRequired() || ( cc->IsLabRequired() && r->IsLab() );
		if( _criteria[ ci + 2 ] )
			score++;

		bool po = false, go = false;

		for( int i = numberOfRooms, t = day * daySize + time; i > 0; i--, t += DAY_HOURS )
		{

			for( int i = dur - 1; i >= 0; i-- )
			{

				const list<CourseClass*>& cl = _slots[ t + i ];
				for( list<CourseClass*>::const_iterator it = cl.begin(); it != cl.end(); it++ )
				{
					if( cc != *it )
					{

						if( !po && cc->ProfessorOverlaps( **it ) )
							po = true;


						if( !go && cc->GroupsOverlap( **it ) )
							go = true;


						if( po && go )
							goto total_overlap;
					}
				}
			}
		}

total_overlap:


		if( !po )
			score++;
		_criteria[ ci + 3 ] = !po;


		if( !go )
			score++;
		_criteria[ ci + 4 ] = !go;
	}


	_fitness = (float)score / ( Configuration::GetInstance().GetNumberOfCourseClasses() * DAYS_NUM );
}


Algorithm* Algorithm::_instance = NULL;


CCriticalSection Algorithm::_instanceSect;


Algorithm& Algorithm::GetInstance()
{
	CSingleLock lock( &_instanceSect, TRUE );


	if( _instance == NULL )
	{

		srand( GetTickCount() );


		Schedule* prototype = new Schedule( 2, 2, 80, 3 );


		_instance = new Algorithm( 100, 8, 5, prototype, new ScheduleObserver() );
	}

	return *_instance;
}


void Algorithm::FreeInstance()
{
	CSingleLock lock( &_instanceSect, TRUE );


	if( _instance != NULL )
	{
		delete _instance->_prototype;
		delete _instance->_observer;
		delete _instance;

		_instance = NULL;
	}
}


Algorithm::Algorithm(int numberOfChromosomes, int replaceByGeneration, int trackBest,
					 Schedule* prototype, ScheduleObserver* observer) : _replaceByGeneration(replaceByGeneration),
					 _currentBestSize(0),
					 _prototype(prototype),
					 _observer(observer),
					 _currentGeneration(0),
					 _state(AS_USER_STOPED)
{

	if( numberOfChromosomes < 2 )
		numberOfChromosomes = 2;


	if( trackBest < 1 )
		trackBest = 1;

	if( _replaceByGeneration < 1 )
		_replaceByGeneration = 1;
	else if( _replaceByGeneration > numberOfChromosomes - trackBest )
		_replaceByGeneration = numberOfChromosomes - trackBest;


	_chromosomes.resize( numberOfChromosomes );
	_bestFlags.resize( numberOfChromosomes );


	_bestChromosomes.resize( trackBest );


	for( int i = (int)_chromosomes.size() - 1; i >= 0; --i )
	{
		_chromosomes[ i ] = NULL;
		_bestFlags[ i ] = false;
	}
}


Algorithm::~Algorithm()
{

	for( vector<Schedule*>::iterator it = _chromosomes.begin(); it != _chromosomes.end(); ++it )
	{
		if( *it )
			delete *it;
	}
}

void Algorithm::Start()
{
	if( !_prototype )
		return;

	CSingleLock lock( &_stateSect, TRUE );


	if( _state == AS_RUNNING )
		return;

	_state = AS_RUNNING;

	lock.Unlock();

	if( _observer )

		_observer->EvolutionStateChanged( _state );


	ClearBest();


	int i = 0;
	for( vector<Schedule*>::iterator it = _chromosomes.begin(); it != _chromosomes.end(); ++it, ++i )
	{

		if( *it )
			delete *it;


		*it = _prototype->MakeNewFromPrototype();
		AddToBest( i );
	}

	_currentGeneration = 0;

	while( 1 )
	{
		lock.Lock();


		if( _state != AS_RUNNING )
		{
			lock.Unlock();
			break;
		}

		Schedule* best = GetBestChromosome();


		if( best->GetFitness() >= 1 )
		{
			_state = AS_CRITERIA_STOPPED;
			lock.Unlock();
			break;
		}

		lock.Unlock();


		vector<Schedule*> offspring;
		offspring.resize( _replaceByGeneration );
		for( int j = 0; j < _replaceByGeneration; j++ )
		{

			Schedule* p1 = _chromosomes[ rand() % _chromosomes.size() ];
			Schedule* p2 = _chromosomes[ rand() % _chromosomes.size() ];

			offspring[ j ] = p1->Crossover( *p2 );
			offspring[ j ]->Mutation();
		}


		for( int j = 0; j < _replaceByGeneration; j++ )
		{
			int ci;
			do
			{

				ci = rand() % (int)_chromosomes.size();


			} while( IsInBest( ci ) );


			delete _chromosomes[ ci ];
			_chromosomes[ ci ] = offspring[ j ];


			AddToBest( ci );
		}


		if( best != GetBestChromosome() && _observer )

			_observer->NewBestChromosome( *GetBestChromosome() );

		_currentGeneration++;
	}

	if( _observer )

		_observer->EvolutionStateChanged( _state );
}


void Algorithm::Stop()
{
	CSingleLock lock( &_stateSect, TRUE );

	if( _state == AS_RUNNING )
		_state = AS_USER_STOPED;

	lock.Unlock();
}


Schedule* Algorithm::GetBestChromosome() const
{
	return _chromosomes[ _bestChromosomes[ 0 ] ];
}


void Algorithm::AddToBest(int chromosomeIndex)
{


	if( ( _currentBestSize == (int)_bestChromosomes.size() &&
		_chromosomes[ _bestChromosomes[ _currentBestSize - 1 ] ]->GetFitness() >=
		_chromosomes[ chromosomeIndex ]->GetFitness() ) || _bestFlags[ chromosomeIndex ] )
		return;


	int i = _currentBestSize;
	for( ; i > 0; i-- )
	{

		if( i < (int)_bestChromosomes.size() )
		{

			if( _chromosomes[ _bestChromosomes[ i - 1 ] ]->GetFitness() >
				_chromosomes[ chromosomeIndex ]->GetFitness() )
				break;


			_bestChromosomes[ i ] = _bestChromosomes[ i - 1 ];
		}
		else

			_bestFlags[ _bestChromosomes[ i - 1 ] ] = false;
	}


	_bestChromosomes[ i ] = chromosomeIndex;
	_bestFlags[ chromosomeIndex ] = true;


	if( _currentBestSize < (int)_bestChromosomes.size() )
		_currentBestSize++;
}


bool Algorithm::IsInBest(int chromosomeIndex)
{
	return _bestFlags[ chromosomeIndex ];
}


void Algorithm::ClearBest()
{
	for( int i = (int)_bestFlags.size() - 1; i >= 0; --i )
		_bestFlags[ i ] = false;

	_currentBestSize = 0;
}
