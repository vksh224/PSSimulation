#include "Square.h"
#include "Users.h"
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

extern std::vector<Event> allEvents;
//extern std::unordered_map<Event, std::string> toBePublishedTrueEvents2;

extern std::map<std::string, std::string> toBePublishedTrueEvents;
extern std::map<std::string, std::string> toBePublishedFalseEvents;

extern std::map<std::string, std::string> EUTPublishedTrueEvents;
extern std::map<std::string, std::string> EUTPublishedFalseEvents;



extern int eventWidth;
extern int thresholdN;
extern int isDebugMode;
extern int simulationCase;



extern double probOfHonestReportersReportTrueEventTruly;
extern double probOfSelReportersReportTrueEventTruly;
extern double probOfMalReportersReportTrueEventTruly;

//It means, whatever false event type was generated, reporters reported that exactly
extern double probOfHonestReportersReportFalseEventTruly;
extern double probOfSelReportersReportFalseEventTruly;
extern double probOfMalReportersReportFalseEventTruly;

extern double probOfHonestReportersWhoReportTrueEvent;
extern double probOfMalReportersWhoReportTrueEvent;
extern double probOfSelfishReportersWhoReportTrueEvent;
extern double probOfSelfishReportersWhoReportFalseEvent;

//Recently added (not in IQ2S paper)
extern double probOfHonestReportersWhoReportFalseEvent;
extern double probOfMalReportersWhoReportFalseEvent;

//regions
//extern int noOfRegions;
extern int **eventHistoryMat;
extern int **jamEventHistoryMat;
extern int **accidentEventHistoryMat;
extern int **rcEventHistoryMat;
extern int **whEventHistoryMat;
extern Event certainEventList[4];
extern std::map<Event,int> aggregateReports;
extern std::map<Event,double> aggregateReputation;
extern int aggregatedReports[4];
extern double aggregatedReputation[4];
extern double eventConfidence[4];
extern double utilityEventConfidence[4];
extern double prospectProbEventType[4];
extern double valueFunctionForEachEventType[4];
extern double probEventType[4];
extern double EUTValueFunctionForEventType[4];

//event confidence
extern double weightReport;
extern double weightReputation;

//report confidence
extern int noOfReportsThresLow;
extern int noOfReportsThresHigh;
extern double reportScaleParamC;
extern double reportShapeParamB;
extern double reportVarphiParam;

//reputation confidence 
extern double reputationScaleParamC;
extern double reputationShapeParamB;

//Event Type History
extern double eventTheta;
extern double eventPhi;
extern double eventDelta;
extern double eventLambda;

extern double fracOfActualReportersThres;
extern double fracOfActualReportersReputationThres;

void initialize();
void getEventHistoryInEachRegion(int epochNum, int eventRegionRadius, int noOfRegions, int **eventHistoryMat, int **jamEventHistoryMat, 
	int **accidentEventHistoryMat, int **rcEventHistoryMat, int **whEventHistoryMat);
void getUserHistoryInEachRegion(int eventRegionRadius, int noOfRegions, Users users[]);
int getEventTypeFromHistory(double xPos, double yPos, int eventRegionRadius, int noOfRegions, int **eventHistoryMat, 
	int **jamEventHistoryMat, int **accidentEventHistoryMat, int **rcEventHistoryMat, int **whEventHistoryMat);

Event handleTrueEventAfterLearning(double probOfTrueEventOccurence[], int currentEpoch, Square eventSquare, Users users[],
	int eventRegionRadius, int noOfRegions, int **eventHistoryMat, int **jamEventHistoryMat, int **accidentEventHistoryMat, 
	int **rcEventHistoryMat, int **whEventHistoryMat);

Event handleFalseEventAfterLearning(double probOfFalseEventOccurence[], int currentEpoch, Square eventSquare, Users users[],
	int eventRegionRadius, int noOfRegions, int **eventHistoryMat, int **jamEventHistoryMat, int **accidentEventHistoryMat, 
	int **rcEventHistoryMat, int **whEventHistoryMat);


void computeConfidenceForEventK(Users users[], Event event);
double getReputationConfidence(Event event, int eventType);
double getReportsConfidence(Event event, int eventType);
void calculateNumberOfReportsForEachEventType(Users users[] , int currentEpoch);
void calculateAggregateReportsAndReputation(Users users[], int currentEpoch, Event event, bool isTrueEvent);
void computeProspectProbabilityOfEventK(int eventRegionRadius, int noOfRegions, Event event, int **eventHistoryMat, int **jamEventHistoryMat, 
	int **accidentEventHistoryMat, int **rcEventHistoryMat, int **whEventHistoryMat);
void computeUtilityEventConfidenceForEventK(Users users[], Event event);
void shouldPublishEventK(double publishThresPerc, Event event);
void shouldPublishEventK_NEW(double publishThresPerc, Event event);
void shouldPublishEventK_NEW2(double publishThresPerc, Event event, bool isTrue);
void updateEventInAllEvents(Event event);
void updateFakeEventInAllEvents(std::string eventId, std::string str, bool isCPT);
double computeWeightOfProb(double p, int errorType);
double computeUtilityOfConfidence(double c);

