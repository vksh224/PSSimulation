#ifndef UTILITY_H
#define UTILITY_H

#include "Square.h"
#include "Users.h"
#include <vector>

extern Square areaSquare[4];
extern int noOfUsers;
extern int userMovementRange;

extern std::vector<Event> allEvents;
extern int eventWidth;
extern int thresholdN;
extern int isDebugMode;
extern int simulationCase;

extern double percOfMalReporters;
extern double percOfSelfishReporters;
extern double probOfHonestReportersWhoReportTrueEvent;
extern double probOfHonestReportersWhoReportFalseEvent;
extern double probOfMalReportersWhoReportTrueEvent;
extern double probOfSelfishReportersWhoReportTrueEvent;
extern double probOfSelfishReportersWhoReportFalseEvent;
extern double positiveRatingsPercForTrueEvent;
extern double negativeRatingsPercForTrueEvent;
extern double positiveRatingsPercForFalseEvent;
extern double negativeRatingsPercForFalseEvent;
extern double uncertainWeightMax;
extern double uncertainA;
extern double uncertainB;
extern double beliefWeightMax;
extern double beliefA;
extern double beliefB;
extern double percOfRaters;
extern double percOfTotalRatings;
extern double phi;
extern double positiveRatingRatioThres;
extern double qoiThres;
extern double percOfEventsThres;
extern double alpha; //alpha, beta and lambda are against prospect theory
extern double beta;
extern double lambda;
extern double referencePoint;
extern double ds[1][3];
extern int scenarioWidth;



//Users dsUsers[noOfUsers];

bool String2Int(const std::string & str, int& result);

int random(int min, int max);

void placeAllReportersInAllSquares(Users user[], Square areaSquare[4], int noOfReporters, int noOfMalReporters, int noOfSelfishReporters);
void placeAllRatersInAllSquares(Users user[], int noOfRaters, int noOfReporters, Square areaSquare[4]);
std::vector<Users> getAllUsersInRange(Users users[], Point eventLocation);
std::vector<Users> allMaliciousReportersWillReport(std::vector<Users> usersInRange, Event & event, bool isTrueEvent);
std::vector<Users> certainPercReportersWillReport(std::vector<Users> usersInRange, Event event, bool isTrueEvent);

void updateUsersWithReportedEvents(Users users[], std::vector<Users> reportersInRange);
void updateUsersWithRatedEvents(Users users[], std::vector<Users> ratersInRange);

void showAllUsers(Users users[]);
void showEventSquare(Square eventSquare);
std::vector<Users> ratersWillRate(std::vector<Users> userInRange, Event & event);
std::vector<Users> ratersWillRate(std::vector<Users> userInRange, Event & event, double certainPercRaters);

void handleTrueEvent(double probOfTrueEventOccurence[], int currentEpoch, Square eventSquare, Users users[]);
void handleFalseEvent(double probOfFalseEventOccurence[], int currentEpoch, Square eventSquare, Users users[]);

void moveAllUsersRandomly(Users users[], int scenarioWidth);
void printSquare(Square areaSquare[], int index);

double calculateScoreMean(Users users[],  bool isPositive);
double calculateScoreSD(Users users[], double meanValue, bool isPositive);

int getMaxNoOfRatings();
void sortRawScoreAsTypeOfReporter(Users users[]);
void sortScoreAsTypeOfReporter(Users users[]);
void sortScoreAsReportersBasedOnParticipationAndTruthFulness(Users users[]);
void sortScoreForHonestReporters(Users users[]);
//void sortScoreForMaliciousReporters(Users[] users);
//void sortScoreForSelfishReporters(Users[] users);
void sortDSScoreAsReportersBasedOnParticipationAndTruthFulness(Users users[]);
void showDSScoreBasedOnPositiveEvents(Users users[]);
void showOurScoreBasedOnPositiveEvents(Users users[]);

Users* updateReportedEventsWithUpdatedInformation(Users users[]);
void calculateDampsterScoreForAllReporters(Users users[]);
void  dampsterProduct(std::vector<Event> reportedEvents , int i);

void calculateQOIForAllEvents();
void calculateScoreForAllReporters(Users users[]);
void calculateFinalScoreForAllReporters(Users users[], double pScoreMean, double nScoreMean, double pScoreSD, double nScoreSD);
void incentivizeUsersBasedOnReputation(Users users[], double reward);

void getUserEventLocation(Square areaSquare[], Users users[], int initialEndFlag);
int getEventTypeIdFromHistory(int epoch, double xPos, double yPos);

#endif // UTILITY_H
