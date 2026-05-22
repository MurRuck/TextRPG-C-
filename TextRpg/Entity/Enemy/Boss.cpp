// Boss.cpp

#include "Boss.h"
#include <cstdlib>
using namespace std;

Boss::Boss(string name, int level)
	: EnemyBase(name, level, 1.5f)
{
}