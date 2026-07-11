#include "customer.h"

const int DOLLARS_PER_POINT = 1;

void Customer :: UpdateLevel()
{
    delete CurrentLevel;
    CurrentLevel = LevelFactory :: getLevel(Points);
}

void Customer :: AddPoints(double TotalCost)
{
    double NewPoints = TotalCost / DOLLARS_PER_POINT;
    double MultiPoints = CurrentLevel -> getPoints();

    Points += NewPoints * MultiPoints;
    UpdateLevel();
}