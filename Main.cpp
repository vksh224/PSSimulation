
#include "Main.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
//#include <unordered_map>
#include <string>
using namespace std;

vector<Event> allEvents;
//unordered_map<Event, string> toBePublishedTrueEvents2;

map<string, string> toBePublishedTrueEvents;

map<string, string> toBePublishedFalseEvents;

map<string, string> EUTPublishedTrueEvents;
map<string, string> EUTPublishedFalseEvents;


Square areaSquare[4];
int noOfUsers = 16;
//make sure that noOfReporters, noOfRaters are multiple of 4
int noOfReporters ;
int noOfRaters;
int noOfPassiveUsers;
int noOfEpochs ;
int eventWidth ;
int scenarioWidth ;
int scoreWindowSize ;
int userMovementRange;
int isDebugMode = -1; //No debug Messages
// isDebugMode = 1 // Cout messages for positions of reporters and raters
// isDebugMode = 2 // Cout messages for true Event
// isDebugMode = 3 // Cout messages for false Event
int thresholdN ;
int simulationCase ;

Event certainEventList[4]; //stores all other 3 fake event types along with that very true event
map<Event,int> aggregateReports;
map<Event,double> aggregateReputation;
int aggregatedReports[4];
double aggregatedReputation[4];
double eventConfidence[4];
double prospectProbEventType[4];
double utilityEventConfidence[4];
double valueFunctionForEachEventType[4];
double probEventType[4];
double EUTValueFunctionForEventType[4];

double percOfMalReporters ;
double percOfSelfishReporters ;


double probOfHonestReportersReportTrueEventTruly;
double probOfSelReportersReportTrueEventTruly ;
double probOfMalReportersReportTrueEventTruly = 0.7;

//It means, whatever false event type was generated, reporters reported that exactly
double probOfHonestReportersReportFalseEventTruly = 0.7;
double probOfSelReportersReportFalseEventTruly = 0.7;
double probOfMalReportersReportFalseEventTruly = 0.7;

double probOfHonestReportersWhoReportTrueEvent ;
double probOfMalReportersWhoReportTrueEvent ;
double probOfSelfishReportersWhoReportTrueEvent ;
double probOfSelfishReportersWhoReportFalseEvent ;

//Recently added (not in IQ2S paper)
double probOfHonestReportersWhoReportFalseEvent;
double probOfMalReportersWhoReportFalseEvent;

double positiveRatingsPercForTrueEvent ;
double negativeRatingsPercForTrueEvent ;
double positiveRatingsPercForFalseEvent;
double negativeRatingsPercForFalseEvent ;
double uncertainWeightMax ;
double uncertainA ;
double uncertainB ;
double beliefWeightMax ;
double beliefA ;
double beliefB ;
double percOfRaters ;
double percOfTotalRatings;
double phi;
double positiveRatingRatioThres;
double qoiThres;
double percOfEventsThres;
double alpha;
double beta;
double lambda;
double referencePoint;
double ds[1][3]; //will have combined belief at 0, combined disbelief at 1 and combined uncertainty at 2
double eventRegionRadius;

double reward;

//event confidence
double weightReport = 0.5;
double weightReputation = 0.5;

//report confidence
int noOfReportsThresLow = 5;
int noOfReportsThresHigh = 10;
double reportScaleParamC = 1;
double reportShapeParamB = 1;
double reportVarphiParam = 0.3;

//reputation confidence 
double reputationScaleParamC = 1;
double reputationShapeParamB = 1;

//Event Type History
double eventTheta = 2;
double eventPhi = 1;
double eventDelta = 0.8;
double eventLambda = 2;

double fracOfActualReportersThres;
double fracOfActualReportersReputationThres;

void readFromFile(){
   std::string line;
   std::string deliminater = "=";
   std::ifstream myfile ("input.txt");
    //string::size_type sz;
   std::string noOfUsersVal="  ";
   string tempNum ;
   string tempNumFloat ;

    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            //std::cout <<" Line is: " << line <<std::endl;
           
        	 if(line.find("isDebugMode") !=std::string::npos){
				 tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 String2Int(tempNum, isDebugMode);
				 if(isDebugMode == 0)
					 std::cout << "Is Debug Mode :   "<<isDebugMode << "\n";
			 }
        	 else if(line.find("noOfUsers") !=std::string::npos){
                tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
                //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
                String2Int(tempNum, noOfUsers);
                if(isDebugMode == 0)
                	std::cout << "Total Number of Users:   "<<noOfUsers << "\n";
            }
        	 else if(line.find("noOfReporters") !=std::string::npos){
                tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
                //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
                String2Int(tempNum, noOfReporters);
                if(isDebugMode == 0)
                	std::cout << "Total Number of Reporters:  "<<noOfReporters << "\n";
            }
        	 else if(line.find("noOfRaters") !=std::string::npos){
                tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
                //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
                String2Int(tempNum, noOfRaters);
                if(isDebugMode == 0)
                	std::cout << "Total Number of Raters:   "<<noOfRaters << "\n";
            }
        	 else if(line.find("noOfEpochs") !=std::string::npos){
                tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
                //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
                String2Int(tempNum, noOfEpochs);
                if(isDebugMode == 0)
                	std::cout << "Total Number of Epochs:   "<<noOfEpochs << "\n";
            }
        	 else if(line.find("noOfPassiveUsers") !=std::string::npos){
                tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
                //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
                String2Int(tempNum, noOfPassiveUsers);
                if(isDebugMode == 0)
                	std::cout << "Total Number of Passive Users:   "<<noOfPassiveUsers << "\n";
            }
        	 else if(line.find("eventWidth") !=std::string::npos){
				tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				//usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				String2Int(tempNum, eventWidth);
				if(isDebugMode == 0)
					std::cout << "Event Width:   "<<eventWidth << "\n";
			}
        	 else if(line.find("scenarioWidth") !=std::string::npos){
				tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				//usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				String2Int(tempNum, scenarioWidth);
				if(isDebugMode == 0)
					std::cout << "Scenario Width:   "<<scenarioWidth << "\n";
			}
        	 else if(line.find("scoreWindowSize") !=std::string::npos){
				tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				//usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				String2Int(tempNum, scoreWindowSize);
				if(isDebugMode == 0)
					std::cout << "Score Window Size:   "<<scoreWindowSize << "\n";
			}
        	 else if(line.find("userMovementRange") !=std::string::npos){
				tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				//usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				String2Int(tempNum, userMovementRange);
				if(isDebugMode == 0)
					std::cout << "User Movement Range:   "<<userMovementRange << "\n";
			}
        	 else if(line.find("percOfMalReporters") !=std::string::npos){
                tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
                //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
                //String2Int(tempNumFloat, perOfMalReporters);
                percOfMalReporters = strtod(tempNumFloat.c_str(), NULL);
                if(isDebugMode == 0)
                	std::cout << "Perc. of Malicious Reporters:   "<<percOfMalReporters << "\n";
            }
        	 else if(line.find("percOfSelfishReporters") !=std::string::npos){
				tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				//usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				//String2Int(tempNumFloat, perOfMalReporters);
				percOfSelfishReporters = strtod(tempNumFloat.c_str(), NULL);
				if(isDebugMode == 0)
					std::cout << "Perc. of Selfish Reporters:   "<<percOfSelfishReporters << "\n";
			}
        	 else if(line.find("probOfHonestReportersWhoReportTrueEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfHonestReportersWhoReportTrueEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Honest Reporters who report a true event:   "<<probOfHonestReportersWhoReportTrueEvent << "\n";
			 }
        	 else if(line.find("probOfMalReportersWhoReportTrueEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfMalReportersWhoReportTrueEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Mal Reporters who report a true event:   "<<probOfMalReportersWhoReportTrueEvent << "\n";
			 }
        	 else if(line.find("probOfSelfishReportersWhoReportTrueEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfSelfishReportersWhoReportTrueEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Selfish Reporters who report a true event:   "<<probOfSelfishReportersWhoReportTrueEvent << "\n";
			 }
        	 else if(line.find("probOfSelfishReportersWhoReportFalseEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfSelfishReportersWhoReportFalseEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Selfish Reporters who report a false event:   "<<probOfSelfishReportersWhoReportFalseEvent << "\n";
			 }
        	 else if(line.find("positiveRatingsPercForTrueEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 positiveRatingsPercForTrueEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Perc. of Positive Ratings For True Event:   "<<positiveRatingsPercForTrueEvent << "\n";
			 }
        	 else if(line.find("negativeRatingsPercForTrueEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 negativeRatingsPercForTrueEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Perc. of Negative Ratings For True Event:   "<<negativeRatingsPercForTrueEvent << "\n";
			 }
        	 else if(line.find("positiveRatingsPercForFalseEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 positiveRatingsPercForFalseEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Perc. of Positive Ratings For False Event:   "<<positiveRatingsPercForFalseEvent << "\n";
			 }
        	 else if(line.find("negativeRatingsPercForFalseEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 negativeRatingsPercForFalseEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Perc. of Negative Ratings For True Event:   "<<negativeRatingsPercForFalseEvent << "\n";
			 }
        	 else if(line.find("uncertainWeightMax") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 uncertainWeightMax = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Max value possible for uncertain ratings:   "<<uncertainWeightMax << "\n";
			 }
        	 else if(line.find("uncertainA") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 uncertainA = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "The value of A for uncertain ratings :   "<<uncertainA << "\n";
			 }
        	 else if(line.find("uncertainB") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 uncertainB = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "The value of B for uncertain ratings:   "<<uncertainB << "\n";
			 }
        	 else if(line.find("beliefWeightMax") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 beliefWeightMax = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Max value possible for belief:   "<<beliefWeightMax << "\n";
			 }
        	 else if(line.find("beliefA") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 beliefA = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "The value of A for belief :   "<<beliefA << "\n";

			 }
        	 else if(line.find("beliefB") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 beliefB = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "The value of B for uncertain ratings:   "<<beliefB << "\n";
			 }
        	 else if(line.find("simulationCase") !=std::string::npos){
				 tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 String2Int(tempNum, simulationCase);
				 if(isDebugMode == 0)
					 std::cout << "simulation Case :   "<<simulationCase << "\n";
			 }
        	 else if(line.find("percOfRaters") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 percOfRaters = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Percentage of raters:   "<<percOfRaters << "\n";
			 }
        	 else if(line.find("thresholdN") !=std::string::npos){
				 tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 String2Int(tempNum, thresholdN);
				 if(isDebugMode == 0)
					 std::cout << "Threshold N :   "<<thresholdN << "\n";
			 }
        	 else if(line.find("percOfTotalRatings") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 percOfTotalRatings = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Percentage of total ratings required:   "<<percOfTotalRatings << "\n";
			 }
        	 else if(line.find("phi") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 phi = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Value of phi:   "<<phi << "\n";
			 }
        	 else if(line.find("positiveRatingRatioThres") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 positiveRatingRatioThres = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Value of positive rating ratio:   "<< positiveRatingRatioThres << "\n";
			 }
        	 else if(line.find("qoiThreshold") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 qoiThres = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Value of QOI threshold:   "<< qoiThres << "\n";
			 }
        	 else if(line.find("percOfEventsThreshold") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 percOfEventsThres = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Perc of Event threshold:   "<< percOfEventsThres << "\n";
			 }
			 else if(line.find("alpha") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 alpha = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Alpa is:   "<< alpha << "\n";
			 }
			 else if(line.find("beta") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 beta = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Beta is:   "<< beta << "\n";
			 }
			 else if(line.find("lambda") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 lambda = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Lambda is:   "<< lambda << "\n";
			 }
			 else if(line.find("referencePoint") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 referencePoint = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "ReferencePoint is:   "<< referencePoint << "\n";
			 }
			  else if(line.find("reward") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 reward = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Reward is:   "<< reward << "\n";
			 }
			 else if(line.find("eventRegionRadius") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 eventRegionRadius = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Event Region Radius is:   "<< eventRegionRadius << "\n";
			 }
			 else if(line.find("weightReport") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 weightReport = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Weightage of Report is:   "<< weightReport << "\n";
			 }
			  else if(line.find("weightReputation") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 weightReputation = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Weightage of Reputation is:   "<< weightReputation << "\n";
			 }
			  else if(line.find("reportScaleParamC") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 reportScaleParamC = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Report Scale Parameter C is:   "<< reportScaleParamC << "\n";
			 }
			 else if(line.find("reportShapeParamB") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 reportShapeParamB = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Report Shape Parameter B is:   "<< reportShapeParamB << "\n";
			 }
			 else if(line.find("reportVarphiParam") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 reportVarphiParam = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Report Varphi Param is:   "<< reportVarphiParam << "\n";
			 }
			  else if(line.find("reputationsScaleParamC") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 reputationScaleParamC = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Reputation Scale Parameter C is:   "<< reputationScaleParamC << "\n";
			 }
			 else if(line.find("reputationShapeParamB") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 reputationShapeParamB = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Reputation Shape Parameter B is:   "<< reputationShapeParamB << "\n";
			 }
			  else if(line.find("eventTheta") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 eventTheta = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "History Event Theta is:   "<< eventTheta << "\n";
			 }
			 else if(line.find("eventPhi") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 eventPhi = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "History Event Phi is:   "<< eventPhi << "\n";
			 }
			 else if(line.find("eventDelta") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 eventDelta = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "History Event Delta is:   "<< eventDelta << "\n";
			 }
			 else if(line.find("eventLambda") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
		
				 eventLambda = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "History Event Lambda is:   "<< eventLambda << "\n";
			 }
			 else if(line.find("noOfReportsThresLow") !=std::string::npos){
				 tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 String2Int(tempNum, noOfReportsThresLow);
				 if(isDebugMode == 0)
					 std::cout << "Reports Number of Threshold Low :   "<<noOfReportsThresLow << "\n";
			 }
			 else if(line.find("noOfReportsThresHigh") !=std::string::npos){
				 tempNum = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 String2Int(tempNum, noOfReportsThresHigh);
				 if(isDebugMode == 0)
					 std::cout << "Reports Number of Threshold High :   "<<noOfReportsThresHigh << "\n";
			 }

			  else if(line.find("probOfHonestReportersWhoReportFalseEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfHonestReportersWhoReportFalseEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Honest Reporters who report a false event:   "<<probOfHonestReportersWhoReportFalseEvent << "\n";
			 }
			 else if(line.find("probOfMalReportersWhoReportFalseEvent") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfMalReportersWhoReportFalseEvent = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Malicous Reporters who report a false event:   "<<probOfMalReportersWhoReportFalseEvent << "\n";
			 }
			 else if(line.find("probOfHonestReportersReportTrueEventTruly") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfHonestReportersReportTrueEventTruly = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Honest Reporters who report a true event truly:   "<<probOfHonestReportersReportTrueEventTruly << "\n";
			 }
			  else if(line.find("probOfSelReportersReportTrueEventTruly") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfSelReportersReportTrueEventTruly = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Selfish Reporters who report a true event truly:   "<<probOfSelReportersReportTrueEventTruly << "\n";
			 }
			  else if(line.find("probOfMalReportersReportTrueEventTruly") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfMalReportersReportTrueEventTruly = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					std::cout << "Prob. of Malicous Reporters who report a true event truly:   "<<probOfMalReportersReportTrueEventTruly << "\n";
			 }
			  else if(line.find("probOfHonestReportersReportFalseEventTruly") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfHonestReportersReportFalseEventTruly = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Honest Reporters who report a false event truly:   "<<probOfHonestReportersReportFalseEventTruly << "\n";
			 }
			  else if(line.find("probOfSelReportersReportFalseEventTruly") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfSelReportersReportFalseEventTruly = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Selfish Reporters who report a false event truly:   "<<probOfSelReportersReportFalseEventTruly << "\n";
			 }
			 else if(line.find("probOfMalReportersReportFalseEventTruly") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 probOfMalReportersReportFalseEventTruly = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "Prob. of Malicous Reporters who report a false event truly:   "<<probOfMalReportersReportFalseEventTruly << "\n";
			 }

			  else if(line.find("fracOfActualReportersThres") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 fracOfActualReportersThres = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "On average, fraction of total reporters who report   "<<fracOfActualReportersThres << "\n";
			 }

			  else if(line.find("fracOfActualReportersReputationThres") !=std::string::npos){
				 tempNumFloat = line.substr(line.find(deliminater)+1, line.size()-1);
				 //usersNo = strtol(noOfUsersVal.c_str(), &endPtr, 10);
				 //String2Int(tempNumFloat, perOfMalReporters);
				 fracOfActualReportersReputationThres = strtod(tempNumFloat.c_str(), NULL);
				 if(isDebugMode == 0)
					 std::cout << "On average, fraction of total reporters' reputation "<<fracOfActualReportersReputationThres << "\n";
			 }


        }
    }
    else{
        std::cout <<" Could not open the file \n";
    }
    myfile.close();
}
int main(){
    //read input parameters from the file
    readFromFile();
    Point a(0,0);

    //double width = 10;

    //Input 4 squares
    /*-----------------
    |    UL   |  UR     |
    |         |         |
    -----------------
    | LL      |  LR     |
    |         |         |
    -----------------*/

    //Lower Left Square
    Point temp = a;
    areaSquare[0].setSquare(temp, scenarioWidth);

    if(isDebugMode == 1){
    	  cout <<"First Square Quadrant(Lowest Left) : \n";
    	  printSquare(areaSquare, 0);
    }


    temp = areaSquare[0].getLr();
    areaSquare[1].setSquare(temp, scenarioWidth);
    if(isDebugMode == 1){
    	cout <<"Second Square Quadrant(Lowest Right) : \n";
    	printSquare(areaSquare, 1);
    }

    // Upper Left Square
    temp = areaSquare[0].getUl();
    areaSquare[2].setSquare(temp, scenarioWidth);
    if(isDebugMode == 1){
    	cout <<"Third Square Quadrant(Upper Left) : \n";
    	printSquare(areaSquare, 2);
    }

    // Upper Right Square
    temp = areaSquare[0].getUr();
    areaSquare[3].setSquare(temp, scenarioWidth);
    if(isDebugMode == 1){
    	cout <<"Fourth Square Quadrant(Upper Right) : \n";
    	printSquare(areaSquare, 3);
    }
    // Event square (3/4rth of the considered square)

    Square eventSquare;
    Point eventInitPos;
    eventInitPos.setX(a.getX()+ scenarioWidth/2); // Total size of square is twice the width
    eventInitPos.setY(a.getY() + scenarioWidth/2);
    eventSquare.setSquare(eventInitPos, scenarioWidth);

    if(isDebugMode == 1){
    	cout <<"Event Square Quadrant : \n";
    	cout << "LL "<<eventSquare.getLl().getX()<<"," <<eventSquare.getLl().getY()<<"\n";
    	cout << "UL "<<eventSquare.getUl().getX()<<"," <<eventSquare.getUl().getY()<<"\n";
    	cout << "LR "<<eventSquare.getLr().getX()<<"," <<eventSquare.getLr().getY()<<"\n";
    	cout << "UR "<<eventSquare.getUr().getX()<<"," <<eventSquare.getUr().getY()<<"\n\n";
    }

    //Place Reporters, Raters and Passive users in each areaSquare
    // int noOfEpochs = 48;
    // int noOfReporters = 8;
    // int noOfRaters = 8;
    // int noOfPassiveUsers = 0;
    // double perOfMalReporters = 0.5;
    int noOfSelfishReporters = percOfSelfishReporters * noOfReporters;
    int noOfMalReporters = percOfMalReporters * noOfReporters;

    if(isDebugMode == 4){
		std::cout <<"No. of Reporters: "<<noOfReporters<<"\n";
		std::cout <<"No of Mal Reporters: "<<noOfMalReporters<<" \n";
		std::cout <<"No of Selfish Reporters: "<<noOfSelfishReporters<<"\n";
    }

 	//For event publishing
    int noOfRegions = ( scenarioWidth)/(eventRegionRadius);
	int **eventHistoryMat = new int*[noOfRegions+1];
	int **jamEventHistoryMat = new int*[noOfRegions+1];
	int **accidentEventHistoryMat = new int*[noOfRegions+1];
 	int **rcEventHistoryMat = new int *[noOfRegions+1];
  	int **whEventHistoryMat = new int*[noOfRegions+1];

	
	for(int i = 0 ; i < noOfRegions + 1; i++){
		eventHistoryMat[i] = new int [noOfRegions+1];
		jamEventHistoryMat[i] = new int [noOfRegions+1];
		accidentEventHistoryMat[i] = new int [noOfRegions+1];
    	rcEventHistoryMat[i] = new int [noOfRegions + 1];
    	whEventHistoryMat[i] = new int [noOfRegions +1];
	}

	//Initialize matrix
	for(int i = 0; i < noOfRegions + 1; i++){
		for(int j = 0; j < noOfRegions + 1; j++){
			eventHistoryMat[i][j] = 0;
			jamEventHistoryMat[i][j] = 0;
			accidentEventHistoryMat[i][j] =0;
      		rcEventHistoryMat[i][j] = 0;
      		whEventHistoryMat[i][j] = 0;
		}
	}

    //From Waze data, 2/3 of total epochs has low probability of occurrence of event
    //Rest has high probability of occurrence of events
	double probOfTrueEventOccurence[noOfEpochs];
    double probOfFalseEventOccurence[noOfEpochs];

    for(int i = 0 ; i <= noOfEpochs; i++){

    	probOfTrueEventOccurence[i] = 1;
    	probOfFalseEventOccurence[i] = 0.9;
//    	if(i%3 == 0){
//    		probOfTrueEventOccurence[i] = 0.25;
//    		probOfFalseEventOccurence[i] = 0.25;
//    	}
//    	else{
//    		probOfTrueEventOccurence[i] = 0.75;
//    		probOfFalseEventOccurence[i] = 0.75;
//    	}
    }

   // int noOfUsers = noOfReporters + noOfRaters + noOfPassiveUsers;

    Users *users = new Users[noOfUsers];

    std::cout <<" Total Number of Epochs: " << noOfEpochs <<"\n";
    int currentWindowEpoch = 0, currentWindowEpochAfterLearning = scoreWindowSize + 25;

    double sumPT_P = 0, sumPT_R = 0, sumPT_F = 0, sumEUT_P = 0, sumEUT_R = 0, sumEUT_F = 0;
    double sumPT_P_R = 0, sumPT_R_R = 0, sumPT_F_R = 0, sumEUT_P_R = 0, sumEUT_R_R = 0, sumEUT_F_R = 0;

    int noOfInterations = 1;
    
    for(int i = 0; i < noOfInterations; i++){

    //1 will be replaced by noOfEpochs
    for(int epochNum = 0; epochNum < noOfEpochs; epochNum++)
    {

        //initially place the users in all the square quadrants uniformly
        if(epochNum == 0){
             placeAllReportersInAllSquares(users, areaSquare, noOfReporters, noOfMalReporters, noOfSelfishReporters);
             placeAllRatersInAllSquares(users, noOfRaters, noOfReporters, areaSquare);
             if(isDebugMode == 1){
            	   showAllUsers(users);
            	   //showEventSquare(eventSquare);
             }
            getUserEventLocation(areaSquare, users, 0);
        }
        else if (epochNum <= scoreWindowSize){

        	if(isDebugMode == 1 || isDebugMode == 2 || isDebugMode == 3){
        		 cout <<"Current Epoch: "<< epochNum<<"\n";
        	}

        	//cout <<"Try handling True Event \n";
            handleTrueEvent(probOfTrueEventOccurence, epochNum, eventSquare, users);

            //cout<<"Try handling False Event \n";
            handleFalseEvent(probOfFalseEventOccurence, epochNum, eventSquare, users);

            if(epochNum == currentWindowEpoch + scoreWindowSize){
            	calculateDampsterScoreForAllReporters(users);
				calculateQOIForAllEvents();
				calculateScoreForAllReporters(users);
				//incentivizeUsersBasedOnReputation(users, reward);
				getUserEventLocation(areaSquare, users, 1);
				
				//getUserHistoryInEachRegion(eventRegionRadius, noOfRegions, users);
				currentWindowEpoch += scoreWindowSize;
			}
            //Move the users randomly in all square quadrants
            if(epochNum%3 == 0)
            	moveAllUsersRandomly(users, scenarioWidth);
        }
        else{
        	// std::cout <<" ========== Epoch " << epochNum <<" ==================== \n";
        	Event event;
        	getEventHistoryInEachRegion(epochNum, eventRegionRadius, noOfRegions, eventHistoryMat, jamEventHistoryMat, accidentEventHistoryMat, rcEventHistoryMat, whEventHistoryMat );
        	
        	//================handle true event based on learned history================
        	event = handleTrueEventAfterLearning(probOfTrueEventOccurence, epochNum, eventSquare, users,eventRegionRadius, noOfRegions, eventHistoryMat, jamEventHistoryMat, accidentEventHistoryMat, rcEventHistoryMat, whEventHistoryMat );
        	
        	if(event.getEventId() != "INVALID"){

	        	cout <<"Current True Event is " <<event.getEventId()<<"\n";
	        	
	        	//compute prospect probability of each type
	        	computeProspectProbabilityOfEventK(eventRegionRadius, noOfRegions, event, eventHistoryMat, jamEventHistoryMat, accidentEventHistoryMat, rcEventHistoryMat, whEventHistoryMat );
	        	
	        	//compute confidence for current event
	        	computeConfidenceForEventK(users, event);
	        	computeUtilityEventConfidenceForEventK(users, event);

	        	event.setHistoricProb(probEventType[event.getEventTypeId()]);
	        	event.setHistoricProspectProb(prospectProbEventType[event.getEventTypeId()]);
	        	event.setEventConfidence(eventConfidence[event.getEventTypeId()]);
	        	event.setEventUtilityConfidence(utilityEventConfidence[event.getEventTypeId()]);
	        	updateEventInAllEvents(event);

	        	//Arguments: publishThresholdPerc, event
	        	shouldPublishEventK_NEW2(0.5, event, true);

	        	cout <<"Prob: " << event.getHistoricProb() <<" Prospect Prob: " << event.getHistoricProspectProb()<<" Confidence: " << event.getEventConfidence() <<" Utility: " << event.getEventUtilityConfidence() << "\n";
	        }

        	//==============handle False event based on learned history=================
        	event = handleFalseEventAfterLearning(probOfTrueEventOccurence, epochNum, eventSquare, users,eventRegionRadius, noOfRegions, eventHistoryMat, jamEventHistoryMat, accidentEventHistoryMat, rcEventHistoryMat, whEventHistoryMat );
        	
        	if(event.getEventId() != "INVALID"){

	        	cout <<"Current False Event is " <<event.getEventId()<<"\n";
	        	
	        	//compute prospect probability of each type
	        	computeProspectProbabilityOfEventK(eventRegionRadius, noOfRegions, event, eventHistoryMat, jamEventHistoryMat, accidentEventHistoryMat, rcEventHistoryMat, whEventHistoryMat );
	        	
	        	//compute confidence for current event
	        	computeConfidenceForEventK(users, event);
	        	computeUtilityEventConfidenceForEventK(users, event);

	        	event.setHistoricProb(probEventType[event.getEventTypeId()]);
	        	event.setHistoricProspectProb(probEventType[event.getEventTypeId()]);
	        	event.setEventConfidence(eventConfidence[event.getEventTypeId()]);
	        	event.setEventUtilityConfidence(eventConfidence[event.getEventTypeId()]);

	        	updateEventInAllEvents(event);
	        	
	        	//publishThreshold
	        	shouldPublishEventK_NEW2(0.5, event, false);

	        	//calculate QOI and Reputation again
	        	calculateDampsterScoreForAllReporters(users);
				calculateQOIForAllEvents();
				calculateScoreForAllReporters(users);
				
				incentivizeUsersBasedOnReputation(users, reward);
				getUserEventLocation(areaSquare, users, 1);
			}
			//if(epochNum == currentWindowEpochAfterLearning){
			if(epochNum == noOfEpochs - 1){

				double cntEUTPublishedTrueEvents = 0, cntPTPublishedTrueEvents = 0, cntEUTPublishedEvents = 0, cntPTPublishedEvents = 0;
				double cntPTTrueEvents = 0, cntEUTTrueEvents = 0;
    			double EUTPrecision = 0, PTPrecision = 0, EUTRecall = 0, PTRecall = 0, EUTFScore =0, PTFScore = 0;

    			if(isDebugMode == 6 || isDebugMode == 8){
					std::cout <<"\nOLD: PT Published True Events : " <<"\n";

					for (std::map<string,string>::iterator it=toBePublishedTrueEvents.begin(); it!=toBePublishedTrueEvents.end(); ++it){
						if(isDebugMode == 6)
							std::cout << it->first << " => " << it->second << '\n';

						if(it->second == "PUB"){
							cntPTPublishedEvents++;

							if((it->first).find("T_") != std::string::npos && (it->first).find("F_") == std::string::npos){
								cntPTPublishedTrueEvents++;
							}
						}

						if((it->first).find("T_") != std::string::npos && (it->first).find("F_") == std::string::npos){
							cntPTTrueEvents++;
						}

					}
					
					//if(isDebugMode == 6 )	
	    			std::cout <<"\nOLD: PT Published False Events : " <<"\n";

					for (std::map<string,string>::iterator it=toBePublishedFalseEvents.begin(); it!=toBePublishedFalseEvents.end(); ++it){
						//if(isDebugMode == 6)	
						std::cout << it->first << " => " << it->second << '\n';
						
						if(it->second == "PUB"){
							cntPTPublishedEvents++;
						}
					}
	    			
	    			//if(isDebugMode == 6)
	    			std::cout <<"\nOLD: EUT Published True Events : " <<"\n";
					
					for (std::map<string,string>::iterator it=EUTPublishedTrueEvents.begin(); it!=EUTPublishedTrueEvents.end(); ++it){
						//if(isDebugMode == 6)	
						std::cout << it->first << " => " << it->second << '\n';

						if(it->second == "PUB"){
							cntEUTPublishedEvents++;

							if((it->first).find("T_") != std::string::npos && (it->first).find("F_") == std::string::npos){
								cntEUTPublishedTrueEvents++;
							}
						}

						if((it->first).find("T_") != std::string::npos && (it->first).find("F_") == std::string::npos){
							cntEUTTrueEvents++;
						}
					}
    					
					//if(isDebugMode == 6)
	    			std::cout <<"\nOLD: EUT Published False Events : " <<"\n";
					
					for (std::map<string,string>::iterator it=EUTPublishedFalseEvents.begin(); it!=EUTPublishedFalseEvents.end(); ++it){
						//if(isDebugMode == 6)	
						std::cout << it->first << " => " << it->second << '\n';

						if(it->second == "PUB"){
							cntEUTPublishedEvents++;
						}
					}
					
	    			PTPrecision =  cntPTPublishedTrueEvents/cntPTPublishedEvents;
	    			PTRecall = cntPTPublishedTrueEvents/cntPTTrueEvents;
	    			PTFScore = (2 * PTPrecision * PTRecall)/(PTPrecision + PTRecall);

	    			
	    			EUTPrecision = cntEUTPublishedTrueEvents/cntEUTPublishedEvents;
	    			EUTRecall = cntEUTPublishedTrueEvents/cntEUTTrueEvents;
	    			EUTFScore = (2 * EUTPrecision * EUTRecall)/(EUTPrecision + EUTRecall);
	    			
	    			std::cout<<"\nOLD: PT Count: #published TE " << cntPTPublishedTrueEvents<<" #published E "<<cntPTPublishedEvents <<" #TE: "<< cntPTTrueEvents <<"\n";
	    			std::cout<<"OLD: EUT Count: #published TE " << cntEUTPublishedTrueEvents<<" #published E "<<cntEUTPublishedEvents <<" #TE: "<< cntEUTTrueEvents <<"\n";
	    			
	    			std::cout <<"\nOLD: PT: Precision " << PTPrecision <<" Recall " << PTRecall <<" FScore " << PTFScore <<"\n";
	    			std::cout <<"OLD: EUT: Precision " << EUTPrecision << " Recall " << EUTRecall <<" FScore " << EUTFScore <<"\n";
	    		}

				//New Code

				cntEUTPublishedTrueEvents = 0;
				cntPTPublishedTrueEvents = 0; 
				cntEUTPublishedEvents = 0; 
				cntPTPublishedEvents = 0;
				cntPTTrueEvents = 0;
				cntEUTTrueEvents = 0;
    			
    			int numberOfEvents = 0;

    			double cntPTPublishedRareTrueEvents = 0, cntPTPublishedRareEvents = 0, cntPTRareTrueEvents = 0;
    			double cntEUTPublishedRareTrueEvents = 0, cntEUTPublishedRareEvents = 0, cntEUTRareTrueEvents = 0;

    			double PT_P_R = 0, PT_R_R = 0, PT_F_R = 0, EUT_P_R = 0, EUT_R_R = 0, EUT_F_R = 0;

    			double rareThreshold = 0.3, frequentThreshold = 0;

    			// if (isDebugMode == 8 ||  isDebugMode == 6)
    				cout <<"\nNEW PT True Events: " <<"\n";
				
				for(int i = 0; i < allEvents.size(); i++){
					if(!allEvents[i].checkIfEventFalse() && (allEvents[i].getEventCPTPublish() == "PUB" || allEvents[i].getEventCPTPublish() == "NOT_PUB")){
						
						//TODO
						cout << allEvents[i].getEventId() << " " << allEvents[i].getEventCPTPublish() <<" Pi: " << allEvents[i].getHistoricProspectProb() <<" Utility: "<< allEvents[i].getEventUtilityConfidence() <<" Conf: " << allEvents[i].getEventConfidence() << "\n";	
						
						if(allEvents[i].getEventCPTPublish() == "PUB"){
							cntPTPublishedTrueEvents++;
							cntPTPublishedEvents++;
						}

						if(allEvents[i].getHistoricProb() <= rareThreshold && allEvents[i].getHistoricProb() >= frequentThreshold)

						{
							if(allEvents[i].getEventCPTPublish() == "PUB"){
								cntPTPublishedRareTrueEvents++;
								cntPTPublishedRareEvents++;
							}
							cntPTRareTrueEvents++;

							//double historicProbWeight = computeWeightOfProb(allEvents[i].getHistoricProb(),1);
							
							//TODO
							// cout << allEvents[i].getEventId() << " " << allEvents[i].getEventCPTPublish() <<" " << allEvents[i].getHistoricProspectProb() <<" "<< allEvents[i].getEventUtilityConfidence() <<"\n";	
						
						}

						cntPTTrueEvents++;
						
					}
				}


				// if (isDebugMode == 8 ||  isDebugMode == 6)
					cout <<"\nNEW PT False Events: " <<"\n";
				
				for(int i = 0; i < allEvents.size(); i++){
					if(allEvents[i].checkIfEventFalse() && (allEvents[i].getEventCPTPublish() == "PUB" || allEvents[i].getEventCPTPublish() == "NOT_PUB")){
						//TODO
						cout << allEvents[i].getEventId() << " " << allEvents[i].getEventCPTPublish() <<" Pi: " << allEvents[i].getHistoricProspectProb() <<" Utility: "<< allEvents[i].getEventUtilityConfidence() <<" Conf: " << allEvents[i].getEventConfidence() << "\n";	
						
						if(allEvents[i].getEventCPTPublish() == "PUB"){
							cntPTPublishedEvents++;

							if(allEvents[i].getHistoricProb() <= rareThreshold && allEvents[i].getHistoricProb() >= frequentThreshold)
							{	
								cntPTPublishedRareEvents++;

							}
							//cout <<"CPT: PUBLISHED FALSE EVENT: " << allEvents[i].getEventId() <<" " << allEvents[i].getHistoricProb() <<" " <<allEvents[i].getEventConfidence() <<"\n";
						}
						//TODO
						// if(allEvents[i].getHistoricProb() < rareThreshold){
						// 	cout << allEvents[i].getEventId() << " " << allEvents[i].getEventCPTPublish() <<" " << allEvents[i].getHistoricProspectProb() <<" "<< allEvents[i].getEventUtilityConfidence() <<"\n";	
						// }
					}
				}
				// if (isDebugMode == 8 ||  isDebugMode == 6)
				cout <<"\nNEW EUT: True Events: " <<"\n";
				
				for(int i = 0; i < allEvents.size(); i++){
					if(!allEvents[i].checkIfEventFalse() && (allEvents[i].getEventEUTPublish() == "PUB" || allEvents[i].getEventEUTPublish() == "NOT_PUB")){
						//TODO
						cout << allEvents[i].getEventId() << " " << allEvents[i].getEventEUTPublish() <<" P: " << allEvents[i].getHistoricProb() <<" Conf: "<< allEvents[i].getEventConfidence() <<"\n";	
						
						if(allEvents[i].getEventEUTPublish() == "PUB"){
							cntEUTPublishedTrueEvents++;
							cntEUTPublishedEvents++;
						}

						if(allEvents[i].getHistoricProb() <= rareThreshold && allEvents[i].getHistoricProb() >= frequentThreshold)
						{
							if(allEvents[i].getEventEUTPublish() == "PUB"){
								cntEUTPublishedRareTrueEvents++;
								cntEUTPublishedRareEvents++;
							}
							cntEUTRareTrueEvents++;
							//TODO
							// cout << allEvents[i].getEventId() << " " << allEvents[i].getEventEUTPublish() <<" " << allEvents[i].getHistoricProb() <<" "<< allEvents[i].getEventConfidence() <<"\n";	
						}

						cntEUTTrueEvents++;
						
						// if(allEvents[i].checkIfEventFalse() && allEvents[i].getEventCPTPublish() == "PUB"){
						// 	cout <<"\nCPT: PUBLISHED FALSE EVENT: " << allEvents[i].getEventId() <<" " << allEvents[i].getHistoricProb() <<" " <<allEvents[i].getEventConfidence() <<"\n";
						// }
					}
				}

				cout <<"\nNEW EUT: False Events: " <<"\n";
				for(int i = 0; i < allEvents.size(); i++){
					if(allEvents[i].checkIfEventFalse() && (allEvents[i].getEventEUTPublish() == "PUB" || allEvents[i].getEventEUTPublish() == "NOT_PUB")){
						//TODO
						cout << allEvents[i].getEventId() << " " << allEvents[i].getEventEUTPublish() <<" P: " << allEvents[i].getHistoricProb() <<" Conf: "<< allEvents[i].getEventConfidence() <<"\n";	
						
						if(allEvents[i].getEventEUTPublish() == "PUB"){
							cntEUTPublishedEvents++;
							if(allEvents[i].getHistoricProb() <= rareThreshold && allEvents[i].getHistoricProb() >= frequentThreshold){	
								cntEUTPublishedRareEvents++;
							}
							//cout <<"CPT: PUBLISHED FALSE EVENT: " << allEvents[i].getEventId() <<" " << allEvents[i].getHistoricProb() <<" " <<allEvents[i].getEventConfidence() <<"\n";
						}	

						//TODO
						// if(allEvents[i].getHistoricProb() < rareThreshold){
						// 	cout << allEvents[i].getEventId() << " " << allEvents[i].getEventEUTPublish() <<" " << allEvents[i].getHistoricProb()<<" "<< allEvents[i].getEventConfidence() <<"\n";
						// }
					}
				}
				
				std::ostringstream oss;
				oss << noOfEpochs;
			 	// oss << likelihood;
			 	// oss << probOfHonestReportersReportTrueEventTruly;

				// std::ofstream outputFile ("/Users/vijay/Desktop/Trust_Journal/desc_common.txt", std::ofstream::out | std::ofstream::app);
				
				std::ofstream outputFile ("/Users/vijay/Desktop/Trust_Journal/desc_epoch_"+ oss.str() +".txt", std::ofstream::out | std::ofstream::app);
				//std::ofstream outputFile ("/Users/vijay/Desktop/Trust_Journal/desc_likelihood_"+ oss.str() +".txt", std::ofstream::out | std::ofstream::app);
				//std::ofstream outputFile ("/Users/vijay/Desktop/Trust_Journal/desc_userFraction_"+ oss.str() +".txt", std::ofstream::out | std::ofstream::app);
				
				// std::ofstream finalFile ("/Users/vijay/Desktop/Trust_Journal/decision_common.txt", std::ofstream::out | std::ofstream::app);
				
				 std::ofstream finalFile ("/Users/vijay/Desktop/Trust_Journal/decision_epochs_"+ oss.str() +".txt", std::ofstream::out | std::ofstream::app);
				// std::ofstream finalFile ("/Users/vijay/Desktop/Trust_Journal/decision_likelihood_"+ oss.str() +".txt", std::ofstream::out | std::ofstream::app);
				// std::ofstream finalFile ("/Users/vijay/Desktop/Trust_Journal/decision_userFraction_"+ oss.str() +".txt", std::ofstream::out | std::ofstream::app);
	

    			outputFile << "\nEpoch: " <<currentWindowEpochAfterLearning <<"\n";
    			finalFile << currentWindowEpochAfterLearning <<" ";

				if(cntPTPublishedEvents == 0)
					PTPrecision = 0;
				else
					PTPrecision =  cntPTPublishedTrueEvents/cntPTPublishedEvents;

				if(cntPTTrueEvents == 0)
					PTRecall = 0;
				else
    				PTRecall = cntPTPublishedTrueEvents/cntPTTrueEvents;
    			
    			if(PTPrecision == 0 && PTRecall == 0)
    				PTFScore = 0;
    			else
    				PTFScore = (2 * PTPrecision * PTRecall)/(PTPrecision + PTRecall);

    			sumPT_P += PTPrecision;
    			sumPT_R += PTRecall;
    			sumPT_F += PTFScore;
    			
    			finalFile << PTPrecision << " " << PTRecall << " " << PTFScore <<" ";

    			if(cntEUTPublishedEvents == 0)
    				EUTPrecision = 0;
    			else
    				EUTPrecision = cntEUTPublishedTrueEvents/cntEUTPublishedEvents;

    			if(cntEUTTrueEvents == 0)
    				EUTRecall = 0;
    			else
    				EUTRecall = cntEUTPublishedTrueEvents/cntEUTTrueEvents;

    			if(EUTPrecision == 0 && EUTRecall == 0)
    				EUTFScore = 0;
    			else
    				EUTFScore = (2 * EUTPrecision * EUTRecall)/(EUTPrecision + EUTRecall);


    			sumEUT_P += EUTPrecision;
    			sumEUT_R += EUTRecall;
    			sumEUT_F += EUTFScore;

    			std::cout<<"\nNEW: PT Count: #published TE " << cntPTPublishedTrueEvents<<" #published E "<<cntPTPublishedEvents <<" #TE: "<< cntPTTrueEvents <<"\n";
	    		std::cout<<"NEW: EUT Count: #published TE " << cntEUTPublishedTrueEvents<<" #published E "<<cntEUTPublishedEvents <<" #TE: "<< cntEUTTrueEvents <<"\n";

    			std::cout <<"\nNEW PT: Precision " << PTPrecision <<" Recall " << PTRecall <<" FScore " << PTFScore <<"\n";
    			outputFile <<"\nNEW PT: Precision " << PTPrecision <<" Recall " << PTRecall <<" FScore " << PTFScore <<"\n";
    			
    			std::cout <<"NEW EUT: Precision " << EUTPrecision << " Recall " << EUTRecall <<" FScore " << EUTFScore <<"\n";
    			outputFile <<"NEW EUT: Precision " << EUTPrecision << " Recall " << EUTRecall <<" FScore " << EUTFScore <<"\n";
    			finalFile << EUTPrecision <<" " << EUTRecall << " " << EUTFScore << " ";

    			//Rare Events

    			if(cntPTPublishedRareEvents == 0)
    				PT_P_R = 0;
    			else
    				PT_P_R =  cntPTPublishedRareTrueEvents/cntPTPublishedRareEvents;
    			
    			if(cntPTRareTrueEvents == 0)
    				PT_R_R = 0;
    			else
    				PT_R_R = cntPTPublishedRareTrueEvents/cntPTRareTrueEvents;

    			if(PT_P_R == 0 && PT_R_R == 0)
    				PT_F_R = 0;
    			else
    				PT_F_R = (2 * PT_P_R * PT_R_R)/(PT_P_R + PT_R_R);


    			sumPT_P_R += PT_P_R;
    			sumPT_R_R += PT_R_R;
    			sumPT_F_R += PT_F_R;


    			std::cout<<"\nNEW: Rare PT Count: #published TE " << cntPTPublishedRareTrueEvents<<" #published E "<<cntPTPublishedRareEvents <<" #TE: "<< cntPTRareTrueEvents <<"\n";
	    		outputFile<<"\nNEW: Rare PT Count: #published TE " << cntPTPublishedRareTrueEvents<<" #published E "<<cntPTPublishedRareEvents <<" #TE: "<< cntPTRareTrueEvents <<"\n";
    			
    			std::cout <<"NEW Rare PT: Precision " << PT_P_R <<" Recall " << PT_R_R <<" FScore " << PT_F_R <<"\n";
    			outputFile <<"NEW Rare PT: Precision " << PT_P_R <<" Recall " << PT_R_R <<" FScore " << PT_F_R <<"\n";
    			finalFile << PT_P_R << " " << PT_R_R << " " << PT_F_R << " ";

    			if(cntEUTPublishedRareEvents == 0)
    				EUT_P_R = 0;
    			else
    				EUT_P_R = cntEUTPublishedRareTrueEvents/cntEUTPublishedRareEvents;

    			if(cntEUTRareTrueEvents == 0)
    				EUT_R_R = 0;
    			else
    				EUT_R_R = cntEUTPublishedRareTrueEvents/cntEUTRareTrueEvents;

    			if(EUT_P_R == 0 && EUT_R_R == 0)
    				EUT_F_R = 0;
    			else
    				EUT_F_R = (2 * EUT_P_R * EUT_R_R)/(EUT_P_R + EUT_R_R);


    			sumEUT_P_R += EUT_P_R;
    			sumEUT_R_R += EUT_R_R;
    			sumEUT_F_R += EUT_F_R;

    			std::cout<<"\nNEW: Rare EUT Count: #published TE " << cntEUTPublishedRareTrueEvents<<" #published E "<<cntEUTPublishedRareEvents <<" #TE: "<< cntEUTRareTrueEvents <<"\n";
	    		outputFile<<"\nNEW: Rare EUT Count: #published TE " << cntEUTPublishedRareTrueEvents<<" #published E "<<cntEUTPublishedRareEvents <<" #TE: "<< cntEUTRareTrueEvents <<"\n";
    			
    			std::cout <<"NEW Rare EUT: Precision " << EUT_P_R << " Recall " << EUT_R_R <<" FScore " << EUT_F_R <<"\n\n";
    			outputFile <<"NEW Rare EUT: Precision " << EUT_P_R << " Recall " << EUT_R_R <<" FScore " << EUT_F_R <<"\n\n";
    			
    			finalFile << EUT_P_R << " " << EUT_R_R << " " << EUT_F_R << "\n";

    			std::cout << PTPrecision <<" " << PTRecall <<" " << PTFScore <<" " << EUTPrecision <<" " << EUTRecall << " " << EUTFScore << " " <<  PT_P_R <<" " << PT_R_R <<" " << PT_F_R <<" " << EUT_P_R <<" " << EUT_R_R << " " << EUT_F_R << " " << noOfEpochs <<" " << probOfHonestReportersReportTrueEventTruly <<" " << rareThreshold <<  "\n";
   				// std::cout << PTPrecision <<" " << PTRecall <<" " << PTFScore <<" " << EUTPrecision <<" " << EUTRecall << " " << EUTFScore << " " <<  PT_P_R <<" " << PT_R_R <<" " << PT_F_R <<" " << EUT_P_R <<" " << EUT_R_R << " " << EUT_F_R << " " << noOfEpochs <<" " << probOfHonestReportersReportTrueEventTruly <<" " << frequentThreshold <<  "\n";
   

    			// std::cout <<"Old: Total Number of Events: " << toBePublishedTrueEvents.size() + toBePublishedFalseEvents.size() <<"\n";
    			// std::cout <<" NEW: Total Number of Events: " << numberOfEvents <<"\n";
	
    			outputFile.close();
    			finalFile.close();
    			currentWindowEpochAfterLearning += 25;
			}
        }
    }
    sumPT_P /= noOfInterations;
    sumPT_R /= noOfInterations;
    sumPT_F /= noOfInterations;
    sumEUT_P /= noOfInterations; 
    sumEUT_R /= noOfInterations;
    sumEUT_F /= noOfInterations;
    sumPT_P_R /= noOfInterations;
    sumPT_R_R /= noOfInterations;
    sumPT_F_R /= noOfInterations;
    sumEUT_P_R /= noOfInterations;
    sumEUT_R_R /= noOfInterations;
    sumEUT_F_R /= noOfInterations;

    // std::cout << sumPT_P <<" " << sumPT_R <<" " << sumPT_F <<" " <<sumEUT_P <<" " <<sumEUT_R << " " << sumEUT_F << "\n"; 
    // std::cout << sumPT_P_R <<" " << sumPT_R_R <<" " << sumPT_F_R <<" " <<sumEUT_P_R <<" " <<sumEUT_R_R << " " << sumEUT_F_R << "\n";
    // finalFile <<"Average: " << " " << sumPT_P <<" " << sumPT_R <<" " << sumPT_F <<" " <<sumEUT_P <<" " <<sumEUT_R << " " << sumEUT_F << " " << sumPT_P_R <<" " << sumPT_R_R <<" " << sumPT_F_R <<" " <<sumEUT_P_R <<" " <<sumEUT_R_R << " " << sumEUT_F_R << "\n";
    
}
    return 0;

}
