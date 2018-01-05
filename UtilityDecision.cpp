#include "UtilityDecision.h"
#include "Utility.h"
#include <time.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <set>

double RandomBetween(double smallNumber, double bigNumber)
{
    float diff = bigNumber - smallNumber;
    return (((double) rand() / RAND_MAX) * diff) + smallNumber;
}

/**
* Get Event Type from History data - Each region would have its biasness for event type
* Here, we calculate probability of each event type (out of 4) for each region
* And, then the chosen event type is the most likely event type from history
*
**/

int getEventTypeFromHistory(double eventXPos, double eventYPos, int eventRegionRadius, int noOfRegions, 
  int **eventHistoryMat, int **jamEventHistoryMat, int **accidentEventHistoryMat, 
  int **rcEventHistoryMat, int **whEventHistoryMat)
{
  int chosenEventType = 0;
  double randProb = (double) rand()/RAND_MAX;
  //std::cout<<"Here I am \n";
  
  int regionXCor = eventXPos/eventRegionRadius;
  int regionYCor = eventYPos/eventRegionRadius;

  double noOfEvents = eventHistoryMat[regionXCor][regionYCor];
  double noOfJams = jamEventHistoryMat[regionXCor][regionYCor];
  double noOfAccidents = accidentEventHistoryMat[regionXCor][regionYCor];
  double noOfRC = rcEventHistoryMat[regionXCor][regionYCor];
  double noOfWH = whEventHistoryMat[regionXCor][regionYCor];

  if(noOfEvents == 0)
  	return random(0,4);

  std::map<double, int> probEventTypeMap;
  probEventTypeMap[(double) floorf((noOfJams/noOfEvents) * 100) / 100] = 0;
  probEventTypeMap[(double) floorf((noOfAccidents/noOfEvents)*1000)/1000] = 1;
  probEventTypeMap[(double) floorf((noOfRC/noOfEvents)*10000)/10000] = 2;
  probEventTypeMap[(double) floorf((noOfWH/noOfEvents) * 10)/10] = 3;

  double totalProb = 0; 

  // if(isDebugMode == 7){
  		std::cout <<"\nEvent history as follows: \n";
  // }
  
  for (std::map<double, int>::reverse_iterator it=probEventTypeMap.rbegin(); it!=probEventTypeMap.rend(); ++it){	
 	if(isDebugMode == 7){
 		std::cout <<"Event Type: "<< it->second << " => " << it->first << " Rand prob. "<<randProb<< " Tot prob. " << totalProb <<'\n';
    }

    totalProb += it -> first;
    if(randProb <= totalProb){

        chosenEventType = it->second;
        break;
    }
  }

  // if(isDebugMode == 7){
  	std::cout<<"Events: " <<noOfEvents<<" "<<" Jams: "<<noOfJams<<" Accidents: "<< noOfAccidents<<" Road Closures: "<<noOfRC<<" Weather Hazard: "<<noOfWH<<"\n\n";
  	//std::cout<<" Prob: " <<probJam<<" "<<probAccident<<" "<<probRC<<" "<<probWH<<"\n";
  // }
  return chosenEventType;
}


std::vector<Users> reportersWillReportsTheEvent(std::vector<Users> reportersInRange, int eventTypeId, int currentEpoch, 
	bool isTrueEvent, double probOfReportersReportingAnEvent, double probReportersWhoReportAnEventTypeTruly ){
    //std::cout<<"Perc. Reporters who actually Report: "<< percOfReportersWhoReport<<"\n";
	if(isDebugMode == 2){
    	std::cout <<"Randomly chosen reporters after learning        : "<< "\n";
    	std::cout <<"-------------------------START: REPORTERS after learning-------------------------------\n";
  	}

    int reportersCount = reportersInRange.size();
   	
	double otherEventType1_ReportPerc = RandomBetween(0, 1 - probReportersWhoReportAnEventTypeTruly);
    double otherEventType2_ReportPerc = RandomBetween(0, 1 - probReportersWhoReportAnEventTypeTruly-otherEventType1_ReportPerc);
    double otherEventType3_ReportPerc = 1-probReportersWhoReportAnEventTypeTruly-otherEventType1_ReportPerc - otherEventType2_ReportPerc;				
    
    // int countReportersWhoReportTrueEventType = reportersCount * percReportersWhoReportAnEventTypeTruly;
    // int countReportersWhoReportOtherEventType1 = reportersCount * otherEventType1_ReportPerc;
    // int countReportersWhoReportOtherEventType2 = reportersCount * otherEventType2_ReportPerc;
    // int countReportersWhoReportOtherEventType3 = reportersCount * otherEventType3_ReportPerc;
    // int noOfReportersReported = countReportersWhoReportTrueEventType + countReportersWhoReportOtherEventType1 + countReportersWhoReportOtherEventType2 + countReportersWhoReportOtherEventType3;

    //if(isDebugMode == 7){
    //	std::cout<<reportersCount<<" #Reporters: "<<noOfReportersReported<<" P-reportE: "<<probOfReportersReportingAnEvent<<" P-reportETruly "<<probReportersWhoReportAnEventTypeTruly<<" "<<otherEventType1_ReportPerc<<" "<<otherEventType2_ReportPerc<<" "<<otherEventType3_ReportPerc<<"\n";
   // }

    int cntEligibleReporters = 0, cntNonEligibleReports = 0;
   	for(int i = 0; i < reportersInRange.size(); i++){

  //  		if(reportersInRange[i].getUserId() == "H_LL_128" ){
		// 	std::cout <<"User H_LL_128 - Score: " << reportersInRange[i].getScore() << "\n";		
		// }

   		if(reportersInRange[i].getScore() < 0)
   		{
   			cntNonEligibleReports++;	
   			continue;
   		}

   		//The reports of reporters whose score is greater than 0 are only considered
		Event anEvent;
		//std::cout <<reportersCount<<" "<<countReportersWhoReportTrueEventType<<" "<<countReportersWhoReportOtherEventType1<<" "<<countReportersWhoReportOtherEventType2<<" "<<countReportersWhoReportOtherEventType3<<"\n";

		double randNum = (double) rand()/RAND_MAX;

       // std::cout << "User "<<user.getUserId()<< "  "<<randNum<<"\n\n";
        if(randNum <= probOfReportersReportingAnEvent){

        	double randEventTypeProb = (double) rand()/RAND_MAX;
			if(randEventTypeProb <= probReportersWhoReportAnEventTypeTruly){
			
				std::ostringstream oss1, oss2;
				oss1 << currentEpoch;
				oss2 << eventTypeId;
				std::string eventId = "";
				if(isTrueEvent == true)
					eventId = "T_"+ oss2.str()+"_"+oss1.str();
				else
					eventId = "F_"+ oss2.str()+"_"+oss1.str();
				
				anEvent.setEventId(eventId);
				anEvent.setEventTypeId(eventTypeId);
				anEvent.setEpochNum(currentEpoch);

				//For true event, we save false and vice versa
				anEvent.setFalseEvent(!isTrueEvent);
				
				//countReportersWhoReportTrueEventType--;
				//std::cout <<eventTypeId<<" "<<currentEpoch<<"\n";
				
			}
			else if(randEventTypeProb <= probReportersWhoReportAnEventTypeTruly + otherEventType1_ReportPerc ){
				
				std::ostringstream oss1, oss2;
				oss1 << currentEpoch;
			
				int fakeEventId1 = -1;
				if(eventTypeId == 0)
					fakeEventId1 = 1;
				else if(eventTypeId > 0)
					fakeEventId1 = 0;

				oss2 << fakeEventId1;

				std::string eventId ="";

				if(isTrueEvent)
					eventId =  "F_T_"+ oss2.str()+"_"+oss1.str();
				else
					eventId = "F_F_"+ oss2.str()+"_"+oss1.str();

				anEvent.setEventId(eventId);
				anEvent.setEventTypeId(fakeEventId1);
				anEvent.setEpochNum(currentEpoch);
				anEvent.setFalseEvent(true);
				//countReportersWhoReportOtherEventType1--;

			}
			else if(randEventTypeProb <= probReportersWhoReportAnEventTypeTruly + otherEventType1_ReportPerc + otherEventType2_ReportPerc ){
			
				std::ostringstream oss1, oss2;
				oss1 << currentEpoch;
			
				int fakeEventId2 = -1;
				if(eventTypeId == 2)
					fakeEventId2 = 1;
				else 
					fakeEventId2 = 2;
			

				oss2 << fakeEventId2;
				std::string eventId ="";
				
				if(isTrueEvent)
					eventId =  "F_T_"+ oss2.str()+"_"+oss1.str();
				else
					eventId = "F_F_"+ oss2.str()+"_"+oss1.str();

				anEvent.setEventId(eventId);
				anEvent.setEventTypeId(fakeEventId2);
				anEvent.setEpochNum(currentEpoch);
				anEvent.setFalseEvent(true);
				
				//countReportersWhoReportOtherEventType2--;

			}
			else{
			
				std::ostringstream oss1, oss2;
				oss1 << currentEpoch;
				int fakeEventId3 = -1;
				if(eventTypeId == 3)
					fakeEventId3 = 1;
				else if(eventTypeId != 3)
					fakeEventId3 = 3;
			
				oss2 << fakeEventId3;
				std::string eventId ="";
				
				if(isTrueEvent)
					eventId =  "F_T_"+ oss2.str()+"_"+oss1.str();
				else
					eventId = "F_F_"+ oss2.str()+"_"+oss1.str();

				anEvent.setEventId(eventId);
				anEvent.setEventTypeId(fakeEventId3);
				anEvent.setEpochNum(currentEpoch);
				anEvent.setFalseEvent(true);
				
				//countReportersWhoReportOtherEventType3--;

			}
			cntEligibleReporters++;

			reportersInRange[i].addReportedEvent(anEvent);
			reportersInRange[i].setMostRecentEvent(anEvent);

			if(reportersInRange[i].getUserId() == "H_LL_128" ){
				std::cout <<"Recent event Id: " << anEvent.getEventId() <<" " << reportersInRange[i].getMostRecentEvent().getEventId() <<" \n";
			}

			//current event and its other event types
			certainEventList[anEvent.getEventTypeId()] = anEvent;
			
			// std::cout<<"Certain Event List : " << anEvent.getEventTypeId()<<" "<< certainEventList[anEvent.getEventTypeId()].getEventId()<<"\n";
				
			//std::cout<<"Certain Event List : " << anEvent.getEventTypeId()<<" "<< certainEventList[anEvent.getEventTypeId()].getEventId()<<"\n";
				
		}	
   	}
   	//noOfReportersReported -=(countReportersWhoReportTrueEventType + countReportersWhoReportOtherEventType1 + countReportersWhoReportOtherEventType2 + countReportersWhoReportOtherEventType3);
   	
   	//if(isDebugMode == 7){
   		std::cout <<"Total reporters who reported : "<<cntEligibleReporters<<" Non Eligible: " << cntNonEligibleReports <<" Expected: " << reportersCount <<"\n";
   	//}

   	return reportersInRange;
}

std::vector<Users> getAllReportersInRange(std::vector<Users> allUsers){
	std::vector<Users> reporters;
	for(int i = 0; i < allUsers.size(); i++){
		if(allUsers[i].checkIfUserIsReporter())
			reporters.push_back(allUsers[i]);
	}
	return reporters;
}

std::vector<Users> getAllMalReportersInRange(std::vector<Users> allUsers){
	std::vector<Users> malReporters;
	for(int i = 0; i < allUsers.size(); i++){
		if(allUsers[i].checkIfUserIsMalReporter())
			malReporters.push_back(allUsers[i]);
	}
	return malReporters;
}

std::vector<Users> getAllSelfishReportersInRange(std::vector<Users> allUsers){
	std::vector<Users> selReporters;
	for(int i = 0; i < allUsers.size(); i++){
		if(allUsers[i].checkIfUserIsSelfishReporter())
			selReporters.push_back(allUsers[i]);
	}
	return selReporters;
}

/**
*	Assume that a true event occurs in the given time epoch, then
*	
*
**/
Event handleTrueEventAfterLearning(double probOfTrueEventOccurence[], int currentEpoch, Square eventSquare, Users users[],
int eventRegionRadius, int noOfRegions, int **eventHistoryMat, int **jamEventHistoryMat, int **accidentEventHistoryMat, 
int **rcEventHistoryMat, int **whEventHistoryMat)
{
    double randProb = (double) rand()/RAND_MAX;
    Event trueEvent;
   
    if(isDebugMode == 7){
      std::cout <<"\n\nTry True event After Learning simulation " << randProb <<"\n";
     }
    //Event occured
    if(randProb <= probOfTrueEventOccurence[currentEpoch])
    {
    	if(isDebugMode == 2){
        	std::cout <<"----------------True event after learning occurs:------------------------------------- \n";
      	}

	    //Get the event location
	    Point eventLocation;

	   // std::cout <<" Event Square: " << eventSquare.getLl().getX() <<" "<<eventSquare.getUr().getX() <<"\n";

	    double xPos = random((int)eventSquare.getLl().getX(),(int) eventSquare.getUr().getX()) ;
	    double yPos =random((int)eventSquare.getLl().getY(), (int)eventSquare.getUr().getY()) ;
	    eventLocation.setX(xPos);
	    eventLocation.setY(yPos);

	    trueEvent.setEventLocation(eventLocation);
	    trueEvent.setEventWidth(eventWidth);

	    //Randomly assign event type
	    int eventTypeId = getEventTypeFromHistory(xPos, yPos, eventRegionRadius, noOfRegions, eventHistoryMat, 
	      jamEventHistoryMat, accidentEventHistoryMat, rcEventHistoryMat, whEventHistoryMat );

	   // std::cout<<" Event type : " <<eventTypeId<<" \n";
	   
	   	std::ostringstream oss1, oss2;
	   	oss1 << currentEpoch;
	   	oss2 << eventTypeId;
	   	std::string eventId = "T_"+ oss2.str()+"_"+oss1.str();
	    //trueEvent.setEventId(eventId);
	   	if(isDebugMode == 2){
	      std::cout <<"True Event ID : "<< eventId<<"\n";
	      std::cout <<"Event Type ID : "<< eventTypeId<<"\n";
	      std::cout <<"Event Location: "<< "("<<eventLocation.getX()<<", "<<eventLocation.getY()<<")" <<"\n";
	      std::cout <<"Event Width   : "<< eventWidth<<"\n";
	   	}

	   	// std::cout<<"\n=============================\n";
	   	// std::cout <<"True Event ID : "<< eventId<<"\n";
	    trueEvent.setEventId(eventId);
	    trueEvent.setEventTypeId(eventTypeId);
	    trueEvent.setEpochNum(currentEpoch);
	    trueEvent.setFalseEvent(false);

	    //update certain perc of Reporters in Range
	   std::vector<Users> usersInRange = getAllUsersInRange( users, eventLocation);
	   //Honest Users
	   std::vector<Users> reportersInRange = getAllReportersInRange(usersInRange);
	   std::vector<Users> malReportersInRange = getAllMalReportersInRange(usersInRange);
	   std::vector<Users> selfishReportersInRange = getAllSelfishReportersInRange(usersInRange);

	   // for(int i = 0; i  < reportersInRange.size(); i++){
	   // 		std::cout << reportersInRange[i].getUserId() <<" ";
	   // }

	   if(isDebugMode == 7){
	   		std::cout<<"Reporters count: Honest "<< reportersInRange.size()<<" Selfish "<<selfishReportersInRange.size()<<" Malicious "<< malReportersInRange.size()<<"\n";

	   		std::cout<<"Honest\n";
	   	}

	   reportersInRange = reportersWillReportsTheEvent(reportersInRange, eventTypeId, currentEpoch, true, probOfHonestReportersWhoReportTrueEvent, probOfHonestReportersReportTrueEventTruly);
	   
	   if(isDebugMode == 7){
	   		std::cout<<"Selfish\n";
	   	}

	   selfishReportersInRange = reportersWillReportsTheEvent(selfishReportersInRange, eventTypeId, currentEpoch, true, probOfSelfishReportersWhoReportTrueEvent, probOfSelReportersReportTrueEventTruly);
	  
	   if(isDebugMode == 7){
	   		std::cout<<"Malicious\n";
	   	}
	   malReportersInRange = reportersWillReportsTheEvent(malReportersInRange, eventTypeId, currentEpoch, true, probOfMalReportersWhoReportTrueEvent, probOfMalReportersReportTrueEventTruly);

	   //If no reporter reported, this event has not occured
	   if(reportersInRange.size() + selfishReportersInRange.size() + malReportersInRange.size() == 0){
	   		Event dummyEvent;
	   		dummyEvent.setEventId("INVALID");
	   		return dummyEvent;
	   }

	   updateUsersWithReportedEvents(users, reportersInRange);
	   updateUsersWithReportedEvents(users, selfishReportersInRange);
	   updateUsersWithReportedEvents(users, malReportersInRange);


	   calculateAggregateReportsAndReputation(users, currentEpoch, trueEvent, true);
	   //calculateAggregateReputationForEachEventType(users, currentEpoch);


	    // for(int i =0; i < noOfUsers; i++){
	   // 		std::cout<<" User: " << users[i].getScore()<<"\n";
	   // }

	   int k  = 0;
	   Event wronglyPerceivedEvents[4];

	    for(int i =0; i < noOfUsers; i++){
   			Event recentEvent = users[i].getMostRecentEvent();
			
			bool found = false;
			//check if array has the element
   			for (int j =0; j < 4; j++){
   				if(wronglyPerceivedEvents[j].getEventId() == recentEvent.getEventId()){
   					found = true;
   					break;
   				}
   			}

   			if(recentEvent.getEpochNum() == trueEvent.getEpochNum() && found == false && recentEvent.getEventId().find("T_")!= std::string::npos){
   				wronglyPerceivedEvents[k++] = recentEvent;

   				// std::cout <<" Event: " << recentEvent.getEventId()<<"\n";
   			}

   		}
   		
	   //Rate the event
	   std::vector<Users> ratersInRange;

       // if(simulationCase == 1 && eventLocation.getX() > areaSquare[0].getLl().getX() && eventLocation.getX() < areaSquare[0].getUr().getX()
    		 //   && eventLocation.getY() > areaSquare[0].getLl().getY() && eventLocation.getY() < areaSquare[0].getUr().getY()){
    	  //  //ask all raters in range to rate the event
    	  //  ratersInRange = certainPercOfRatersWillRate(usersInRange, trueEvent, percOfRaters);
       // }
       // else{
    	    //ask all raters in range to rate the event
    	   
       //}

       if(isDebugMode == 7){
	  		std::cout <<" Total number of perceived true events: " << k <<"\n";
	  	}

       //Push the updated trueEvent in the event list
       for(int i = 0; i < k; i++){
       		if(isDebugMode == 7){
       			std::cout <<" Event " << wronglyPerceivedEvents[i].getEventId()<<"\n";
       		}

       		ratersInRange = ratersWillRate(usersInRange, wronglyPerceivedEvents[i]);
       		updateUsersWithRatedEvents(users, ratersInRange);

       		//missed before
       		wronglyPerceivedEvents[i].setEventLocation(trueEvent.getEventLocation());
       		allEvents.push_back(wronglyPerceivedEvents[i]);
       }
     
       //allEvents.push_back()

       if(isDebugMode == 7){
		  //  std::cout<<"Ratings for all Events: "<<"\n";
		   for(int i = 0; i < allEvents.size(); i++){
				Event tEvent = allEvents[i];

				// if(tEvent.getEpochNum() != trueEvent.getEpochNum()){
				// 	continue;
				// }
				// std::cout <<"For True Event: "<< tEvent.getEventId()<<"\n";
				// std::cout <<"Num +ve Ratings         :" << tEvent.getNoOfPositiveRatings()<<std::endl;
				// std::cout <<"Num of -ve Ratings      :"<<tEvent.getNoOfNegativeRatings()<< "\n";
				// std::cout <<"Num of Uncertain Ratings:"<<tEvent.getNoOfUncertainRatings()<<"\n\n";
		   }

		}
    }
    return trueEvent;
}


Event handleFalseEventAfterLearning(double probOfFalseEventOccurence[], int currentEpoch, Square eventSquare, Users users[],
int eventRegionRadius, int noOfRegions, int **eventHistoryMat, int **jamEventHistoryMat, int **accidentEventHistoryMat, 
int **rcEventHistoryMat, int **whEventHistoryMat)
{
    double randProb = (double) rand()/RAND_MAX;
    Event falseEvent;

    if(isDebugMode == 7){
      std::cout <<"\n\nTry False event After Learning simulation " << randProb <<"\n";
     }

    //Event occured
    if(randProb <= probOfFalseEventOccurence[currentEpoch])
    {
    	if(isDebugMode == 2){
        	std::cout <<"----------------False event after learning occurs:------------------------------------- \n";
      	}

	    //Get the event location
	    Point eventLocation;
	    double xPos = random((int)eventSquare.getLl().getX(),(int) eventSquare.getUr().getX()) ;
	    double yPos =random((int)eventSquare.getLl().getY(), (int)eventSquare.getUr().getY()) ;
	    eventLocation.setX(xPos);
	    eventLocation.setY(yPos);

	   	falseEvent.setEventLocation(eventLocation);
	    falseEvent.setEventWidth(eventWidth);

	    //Randomly assign event type
	   // int eventTypeId = getEventTypeFromHistory(xPos, yPos, eventRegionRadius, noOfRegions, eventHistoryMat, 
	   //   jamEventHistoryMat, accidentEventHistoryMat, rcEventHistoryMat, whEventHistoryMat );

	    //Randomly assign event type
        int eventTypeId = random(0,4);

        if(isDebugMode == 7){
        	std::cout<<"Generated False Event type : " <<eventTypeId<<" \n";
        }
	    
	   	std::ostringstream oss1, oss2;
	   	oss1 << currentEpoch;
	   	oss2 << eventTypeId;
	   	std::string eventId = "F_"+ oss2.str()+"_"+oss1.str();
	    //trueEvent.setEventId(eventId);
	   	if(isDebugMode == 2){
	      std::cout <<"False Event ID : "<< eventId<<"\n";
	      std::cout <<"Event Type ID : "<< eventTypeId<<"\n";
	      std::cout <<"Event Location: "<< "("<<eventLocation.getX()<<", "<<eventLocation.getY()<<")" <<"\n";
	      std::cout <<"Event Width   : "<< eventWidth<<"\n";
	   	}

	   		std::cout<<"\n=============================\n";
	    	std::cout <<"False Event ID : "<< eventId<<"\n";

	    falseEvent.setEventId(eventId);
	    falseEvent.setEventTypeId(eventTypeId);
	    falseEvent.setEpochNum(currentEpoch);
	    falseEvent.setFalseEvent(true);


	    //update certain perc of Reporters in Range
	   std::vector<Users> usersInRange = getAllUsersInRange( users, eventLocation);
	   std::vector<Users> reportersInRange = getAllReportersInRange(usersInRange);
	   std::vector<Users> malReportersInRange = getAllMalReportersInRange(usersInRange);
	   std::vector<Users> selfishReportersInRange = getAllSelfishReportersInRange(usersInRange);

	   if(isDebugMode == 7){
	   		std::cout<<"Reporters count: Honest => "<< reportersInRange.size()<<" Sel => "<<selfishReportersInRange.size()<<" Mal => "<< malReportersInRange.size()<<"\n";
	   	}
	   
	   if(isDebugMode == 7){
	   		//std::cout<<"Honest: probReportersReportingFalseEvent percEventTypeTruly  otherEventType1  otherEventType2  otherEventType3\n";
   			std::cout<<"Honest\n";
   		}

   		//honest reporters will never report a false event
	   reportersInRange = reportersWillReportsTheEvent(reportersInRange, eventTypeId, currentEpoch, false, probOfHonestReportersWhoReportFalseEvent, probOfHonestReportersReportFalseEventTruly);
	
		if(isDebugMode == 7){
	   		//std::cout<<"Selfish: probReportersReportingFalseEvent percEventTypeTruly  otherEventType1  otherEventType2  otherEventType3\n";
   			std::cout<<"Selfish\n";
   		}

	   selfishReportersInRange = reportersWillReportsTheEvent(selfishReportersInRange, eventTypeId, currentEpoch, false, probOfSelfishReportersWhoReportFalseEvent, probOfSelReportersReportFalseEventTruly);
	  
	  	if(isDebugMode == 7){
	  		//std::cout<<"Malicous: probReportersReportingFalseEvent percEventTypeTruly  otherEventType1  otherEventType2  otherEventType3\n";
   			std::cout<<"Malicious\n";
   		}

	   malReportersInRange = reportersWillReportsTheEvent(malReportersInRange, eventTypeId, currentEpoch, false, probOfMalReportersWhoReportFalseEvent, probOfMalReportersReportFalseEventTruly);


	   if((reportersInRange.size() + selfishReportersInRange.size() + malReportersInRange.size()) == 0){
	   		Event dummyEvent;
	   		std::cout <<"None of the reporters reported: " << falseEvent.getEventId() <<"\n";
	   		dummyEvent.setEventId("INVALID");
	   		return dummyEvent;
	   }

	   updateUsersWithReportedEvents(users, reportersInRange);
	   updateUsersWithReportedEvents(users, selfishReportersInRange);
	   updateUsersWithReportedEvents(users, malReportersInRange);

	   std::cout <<"Reporters size: " << reportersInRange.size() <<" " << selfishReportersInRange.size() <<" "<<malReportersInRange.size()<< " \n ";
	   //allEvents.push_back(falseEvent);

	   //calcuate reports and reputation for each event type based on honest users in the range
	   calculateAggregateReportsAndReputation(users, currentEpoch, falseEvent, false);

	   //Now add all the wrongly perceived event types to the event list as well
	   int k  = 0;
	   Event wronglyPerceivedEvents[4];

	    for(int i =0; i < noOfUsers; i++){
   			Event recentEvent = users[i].getMostRecentEvent();
			
			bool found = false;
			//check if array has the element
   			for (int j =0; j < 4; j++){
   				if(wronglyPerceivedEvents[j].getEventId() == recentEvent.getEventId()){
   					found = true;
   					break;
   				}
   			}

   			if(recentEvent.getEpochNum() == falseEvent.getEpochNum() && found == false &&
   			 recentEvent.getEventId().find("T_")  == std::string::npos &&
   			 recentEvent.getEventId().find("F_") != std::string::npos){
   				wronglyPerceivedEvents[k++] = recentEvent;
   				// std::cout <<" ==========  FOund Event: " << recentEvent.getEventId()<<"\n";
   			}
   		}
 
   		// std::cout <<"Size: " << (sizeof(wronglyPerceivedEvents)/sizeof(wronglyPerceivedEvents[0])) <<" K " << k << "\n";
   		//Since, it is the testing phase, it may so happen that no reporter (with score >= 0) reported the event.
   		if(k == 0){
   			k++;
   			wronglyPerceivedEvents[0] = falseEvent;
   		}
   		
	   //Rate the event
	   std::vector<Users> ratersInRange;

	   if(isDebugMode == 7){
 	   		std::cout <<" Total number of perceived false events: " << k <<"\n";
 	   	}

       //Push the updated False Event in the event list
       for(int i = 0; i < k; i++){
       		if(isDebugMode == 7){
       			std::cout <<" False Event " << wronglyPerceivedEvents[i].getEventId()<<"\n";
       		}

       		ratersInRange = ratersWillRate(usersInRange, wronglyPerceivedEvents[i]);
       		updateUsersWithRatedEvents(users, ratersInRange);


       		//missed before
       		wronglyPerceivedEvents[i].setEventLocation(falseEvent.getEventLocation());
       		allEvents.push_back(wronglyPerceivedEvents[i]);

       		//falseEvent = wronglyPerceivedEvents[i];
       }
     
       //allEvents.push_back()

       //if(isDebugMode == 7){
		  //  std::cout<<"Ratings for all Events: "<<"\n";
		  //  for(int i = 0; i < allEvents.size(); i++){
				// Event tEvent = allEvents[i];
				    
				// if(tEvent.getEpochNum() == falseEvent.getEpochNum() &&
				// 	tEvent.getEventId().find("T_")  == std::string::npos &&
   	// 		 		tEvent.getEventId().find("F_") != std::string::npos){
				
				// 	std::cout <<"For False Event: "<< tEvent.getEventId()<<"\n";
				// 	std::cout <<"Num +ve Ratings         :" << tEvent.getNoOfPositiveRatings()<<std::endl;
				// 	std::cout <<"Num of -ve Ratings      :"<<tEvent.getNoOfNegativeRatings()<< "\n";
				// 	std::cout <<"Num of Uncertain Ratings:"<<tEvent.getNoOfUncertainRatings()<<"\n\n";
				// }
		  //  }
		//}
    }

    return falseEvent;
}

/**
* Calculate aggregate reports and reputation of all users that reported the most recent event
*
**/
void calculateAggregateReportsAndReputation(Users users[], int currentEpoch, Event event, bool isTrueEvent){
   	int countReportsJam = 0;
   	int countReportsAccident = 0;
   	int countReportsRC = 0;
   	int countReportsWH = 0;

   	double countUserReputationJam =0;
	double countUserReputationAccident = 0;
	double countUserReputationRC = 0;
	double countUserReputationWH = 0;

	int count =1;
  
   for(int i =0; i < noOfUsers; i++){
   		// This is not from history. Its the most recent one.

   		Event recentEvent = users[i].getMostRecentEvent();
   	
   		//current epoch makes sure that its always the current input event since we only consider one event per epoch

   		if(!users[i].checkIfUserIsRater() && recentEvent.getEpochNum() == currentEpoch &&
   			((isTrueEvent == true && recentEvent.getEventId().find("T_")!= std::string::npos) || 
   			(isTrueEvent == false && recentEvent.getEventId().find("T_")== std::string::npos &&
   			recentEvent.getEventId().find("F_")!= std::string::npos))){

   			//These conditions are important to make sure that for a false event, only reports for that false event are considered and so on.
   			
   			//std::cout<<(count)<<" Recent event at user: "<<users[i].getUserId()<<" => "<<recentEvent.getEventId()<<" "<<recentEvent.getEventTypeId()<<"\n";
   			count+= 1;

   			if(recentEvent.getEventTypeId() == 0 && users[i].getScore() >= 0){
   				countReportsJam++;
   				countUserReputationJam += users[i].getScore();
   			}
   			else if(recentEvent.getEventTypeId() == 1 && users[i].getScore() >= 0 ){
   				countReportsAccident++;
   				countUserReputationAccident += users[i].getScore();
   			}
   			else if(recentEvent.getEventTypeId() == 2 && users[i].getScore() >= 0){
   				countReportsRC++;
   				countUserReputationRC += users[i].getScore();
   			}
   			else if(recentEvent.getEventTypeId() == 3 && users[i].getScore() >= 0){
   				countReportsWH++;
   				countUserReputationWH +=users[i].getScore();
   			}
   			// std::cout<< "Recent Event " <<users[i].getUserId()<<" "<<recentEvent.getEventId()<<" "<<recentEvent.getEventTypeId()<<" "<<recentEvent.getEpochNum()<<"\n";
   		}
   }
  
   aggregatedReports[0] = countReportsJam;
   aggregatedReports[1] = countReportsAccident;
   aggregatedReports[2] = countReportsRC;
   aggregatedReports[3] = countReportsWH;

   aggregatedReputation[0] = countUserReputationJam;
   aggregatedReputation[1] = countUserReputationAccident;
   aggregatedReputation[2] = countUserReputationRC;
   aggregatedReputation[3] = countUserReputationWH;

   if(isDebugMode == 7){
   		// std::cout<<"After "<<aggregatedReports[0]<<" "<<countReportsJam<<"\n";
   		std::cout<<"Agg. Reports - Jam: " <<countReportsJam<<" Accident: "<<countReportsAccident<<" RC: "<<countReportsRC<<" WH: "<<countReportsWH<<"\n";
   		std::cout<<"Agg. Reputation - Jam: " <<countUserReputationJam<<" Accident: "<< countUserReputationAccident<<" RC: "<< countUserReputationRC<<" WH: "<<countUserReputationWH<<"\n";
  	}
}

/**
* get the history of occurence of each true event (out of 4 possible events) in each region 
*
**/
void getEventHistoryInEachRegion(int currentEpoch, int eventRegionRadius, int noOfRegions, int **eventHistoryMat, int **jamEventHistoryMat, int **accidentEventHistoryMat, int **rcEventHistoryMat, int **whEventHistoryMat){
	
	//Clean the matrices
	for(int i = 0; i < noOfRegions + 1; i++){
		for(int j = 0; j < noOfRegions + 1; j++){
			eventHistoryMat[i][j] = 0;
			jamEventHistoryMat[i][j] = 0;
			accidentEventHistoryMat[i][j] =0;
      		rcEventHistoryMat[i][j] = 0;
      		whEventHistoryMat[i][j] = 0;
		}
	}

	std::cout <<"Event Size: " << allEvents.size()<<"\n";

	for(int i = 0; i < allEvents.size(); i++)
	{
		//std::cout <<"Event: " <<allEvents[i].getEventId() <<" ";
		int curX = (allEvents[i].getEventLocation().getX()/eventRegionRadius);
		int curY = (allEvents[i].getEventLocation().getY()/eventRegionRadius);
			
        if(allEvents[i].getQOI() >= 0.5)
          eventHistoryMat[curX][curY]++;

		if(allEvents[i].getEventId().find("_0_") != std::string::npos){
          if(allEvents[i].getQOI() >= 0.5)
			jamEventHistoryMat[curX][curY]++;
		}

        if(allEvents[i].getEventId().find("_1_") != std::string::npos){
          if(allEvents[i].getQOI() >= 0.5)
            accidentEventHistoryMat[curX][curY]++;
        }
        if(allEvents[i].getEventId().find("_2_") != std::string::npos){
          if(allEvents[i].getQOI() >= 0.5)
            rcEventHistoryMat[curX][curY]++;
        }
        if(allEvents[i].getEventId().find("_3_") != std::string::npos){
          if(allEvents[i].getQOI() >= 0.5)
            whEventHistoryMat[curX][curY]++;
        }
	if(isDebugMode == 7){
		if(allEvents[i].getEpochNum() >= currentEpoch - 1)
		std::cout<<" History Events: " << curX <<" "<< curY <<" "<<allEvents[i].getEventLocation().getX() <<" "<<allEvents[i].getEventLocation().getY()<<" " <<" "<<allEvents[i].getEventId()<<" "<<" "<<allEvents[i].getQOI()<<" "<<eventHistoryMat[curX][curY]<<" "<<jamEventHistoryMat[curX][curY]<<" "<<accidentEventHistoryMat[curX][curY]<<" "<<rcEventHistoryMat[curX][curY]<<" "<<whEventHistoryMat[curX][curY]<<" \n";
	
	}
	}
}

// double getReportsConfidence(Event event, int eventTypeId){
	
// 	double reportConfidence = 0;
// 	int N = aggregatedReports[eventTypeId];


// 	if(aggregatedReports[eventTypeId] <= noOfReportsThresLow){
// 		reportConfidence = exp(-reportShapeParamB * exp(- reportScaleParamC * N));
// 	}
// 	else if(aggregatedReports[eventTypeId] >= noOfReportsThresHigh){
// 		reportConfidence = exp(- pow((N - noOfReportsThresHigh), reportVarphiParam));
// 	}
// 	else{
// 		reportConfidence = 1;
// 	}
// 	//std::cout<<"The aggregate Report confidence for event type: " << eventTypeId<<" "<<reportConfidence<<" N: "<< N<<"\n";
// 	return aggregatedReports[eventTypeId];
// }


// double getReputationConfidence(Event event, int eventTypeId){
// 	double reputationConfidence = 0;
// 	double R = aggregatedReputation[eventTypeId];
// 	//std::cout<<"The aggregate reputation for event type: " << eventTypeId<<" "<<R<<"\n";
// 	reputationConfidence = exp(- reputationShapeParamB * exp(- reputationScaleParamC * R ));
// 	return reputationConfidence;
// }

void computeConfidenceForEventK(Users users[], Event event){
	
	double totalReportsForEventK = 0, expectedReportsForEventK = 0;
	double totalReputationForEventK = 0, expectedReputationForEventK = 0;


	//update certain perc of Reporters in Range
	std::vector<Users> usersInRange = getAllUsersInRange( users, event.getEventLocation());

	for(int i = 0; i < usersInRange.size(); i++){
		if(!usersInRange[i].checkIfUserIsRater() && usersInRange[i].getScore() > 0){
			expectedReportsForEventK++;
			expectedReputationForEventK += usersInRange[i].getScore();
		}
	}
	std::cout <<"Expected Reports: " << expectedReportsForEventK  << "\n";
	
	for(int i = 0; i < 4; i++){
		totalReportsForEventK += aggregatedReports[i];
		totalReputationForEventK += aggregatedReputation[i];
	}

	for(int i =0; i <4; i++){

		//std::cout <<"COndition:  " << (aggregatedReports[i]/expectedReportsForEventK) <<"\n";
		// && (aggregatedReputation[i]/totalReputationForEventK) < fracOfActualReportersReputationThres 
		if((aggregatedReports[i]/expectedReportsForEventK) < fracOfActualReportersThres)
			eventConfidence[i] = 0;
		else
			eventConfidence[i] = weightReport * (aggregatedReports[i]/expectedReportsForEventK) +  (1 - weightReport) * (aggregatedReputation[i]/expectedReputationForEventK);
		
		if(isDebugMode == 6 || isDebugMode == 8){
			std::cout <<"Reports: " << aggregatedReports[i] <<" Expected: " <<expectedReportsForEventK <<" Frac: " << (aggregatedReports[i]/totalReportsForEventK) << " Rep: "<< aggregatedReputation[i] <<" Tot: "<< totalReputationForEventK<<" Frac: "<<(aggregatedReputation[i]/totalReputationForEventK) << "\n";
		}
		
	}

	if(isDebugMode == 7){
		std::cout<<"\n=============================\n";
	}
}

void computeUtilityEventConfidenceForEventK(Users users[], Event event){
	for(int i = 0; i < 4; i++){
		if(eventConfidence[i] >= 0.5)
			utilityEventConfidence[i] = pow((eventConfidence[i] - 0.5), eventTheta);
		else
			//utilityEventConfidence[i] = -eventLambda * (-1 * pow(eventConfidence[i], eventPhi));
			utilityEventConfidence[i] = -1 * eventLambda * pow((0.5 - eventConfidence[i]), eventPhi);
			//utilityEventConfidence[i] =  pow((eventConfidence[i]), eventPhi);
	}
}


void computeProspectProbabilityOfEventK(int eventRegionRadius, int noOfRegions, Event event, int **eventHistoryMat, 
	int **jamEventHistoryMat, int **accidentEventHistoryMat, int **rcEventHistoryMat, int **whEventHistoryMat)
{
  
  double eventXPos = event.getEventLocation().getX();
  double eventYPos = event.getEventLocation().getY();

  int regionXCor = eventXPos/eventRegionRadius;
  int regionYCor = eventYPos/eventRegionRadius;

  double noOfEvents = eventHistoryMat[regionXCor][regionYCor];
  double noOfJams = jamEventHistoryMat[regionXCor][regionYCor];
  double noOfAccidents = accidentEventHistoryMat[regionXCor][regionYCor];
  double noOfRC = rcEventHistoryMat[regionXCor][regionYCor];
  double noOfWH = whEventHistoryMat[regionXCor][regionYCor];

  if(noOfEvents == 0)
  {
  	for(int i = 0; i < 4; i++){
  		 probEventType[i] = 0.25;
  	}
  }
  else{
  	  //double probEventType[4];
	  probEventType[0] = noOfJams/noOfEvents;
	  probEventType[1] = noOfAccidents/noOfEvents;
	  probEventType[2] = noOfRC/noOfEvents;
	  probEventType[3] = noOfWH/noOfEvents;
  }


	for(int i = 0; i < 4; i++){	
	  prospectProbEventType[i] = pow(probEventType[i], eventDelta)/pow(((pow(probEventType[i], eventDelta)) + pow((1 - probEventType[i]), eventDelta)), (1/eventDelta));	
	}

  //std::cout<<"Events: " <<noOfEvents<<" "<<" Jams: "<<noOfJams<<" Accidents: "<< noOfAccident<<" Road Closures: "<<noOfRC<<" Weather Hazard: "<<noOfWH<<"\n";
  //std::cout<<" Prob: " <<probJam<<" "<<probAccident<<" "<<probRC<<" "<<probWH<<"\n";
}


double computeWeightOfProb(double p, int errorType){
	double eventDeltaNeg = 0.69;
	double pi = 0;
	if(errorType == 1 || errorType == 4)
	  pi = pow(p, eventDelta)/pow(((pow(p, eventDelta)) + pow((1 - p), eventDelta)), (1/eventDelta));
	else
		pi = pow(p, eventDeltaNeg)/pow(((pow(p, eventDeltaNeg)) + pow((1 - p), eventDeltaNeg)), (1/eventDeltaNeg));
	  return pi;
	
}

double computeUtilityOfConfidence(double c){
	double u = 0;
	if(c >= 0.5)
			u = pow(c, eventTheta);
		else
			//utilityEventConfidence[i] = -eventLambda * (-1 * pow(eventConfidence[i], eventPhi));
			u = -1 * eventLambda * pow((0.5 - c), eventPhi);
			//utilityEventConfidence[i] =  pow((eventConfidence[i]), eventPhi);

	return u;
}


void shouldPublishEventK_NEW2(double publishThresPerc, Event event, bool isTrueEvent){

	double sumPublishProspect = 0, sumPublishEUT = 0;
	 double cost[4] = {2, -1, -1, 1};
	// double cost[4] = {1, -2, -1, 1};
	// double cost[4] = {1, -1, -2, 1};
	// double cost[4] = {1, -1, -1, 1};
	double max = -99999, EUT_max = -99999;
	int k = -1, EUT_k = -1; //chosenEventType
	

	//Get chosen Event Type - What event to publish
	for(int i = 0; i < 4; i++){
		if(max < utilityEventConfidence[i]){
			max = utilityEventConfidence[i];
			k = i;
		}
	}
	//CPT
	if(utilityEventConfidence[k] <= 0 || computeUtilityOfConfidence(eventConfidence[k]) <= 0){
		toBePublishedTrueEvents[event.getEventId()] = "NOT_PUB";
		//toBePublishedTrueEvents2[event] = "NOT_PUB";
		event.setEventCPTPublish("NOT_PUB");
	}
	else{
		std::cout <<"Chosen Event Type: " << k <<" Prob " << computeWeightOfProb(probEventType[k], 1) << " Utility: " << computeUtilityOfConfidence(eventConfidence[k]) <<" Confidence: " << eventConfidence[k]<<"\n";
		// Whether to publish

		// sumPublishProspect = (cost[0] * computeWeightOfProb(probEventType[k], 1) * utilityEventConfidence[k]) + 
		// 					(cost[1] * computeWeightOfProb(1 - probEventType[k], 2) * (utilityEventConfidence[k])) +
		// 					(cost[2] * computeWeightOfProb(probEventType[k], 3) * (1 - utilityEventConfidence[k])) +
		// 					(cost[3] * computeWeightOfProb(1 - probEventType[k], 4) * (1 - utilityEventConfidence[k]));


		sumPublishProspect = 
						(
						(cost[0] * computeWeightOfProb(probEventType[k], 1) * computeUtilityOfConfidence(eventConfidence[k])) + 
						(cost[1] * computeWeightOfProb(1 - probEventType[k], 2) * computeUtilityOfConfidence(eventConfidence[k]))
						) - 
						(
						(cost[2] * computeWeightOfProb(probEventType[k], 3) * computeUtilityOfConfidence(1 - eventConfidence[k])) +
						(cost[3] * computeWeightOfProb(1 - probEventType[k], 4) * computeUtilityOfConfidence(1 - eventConfidence[k]))
						);
	
		// std::cout<<"Case 1: " <<computeWeightOfProb(probEventType[k], 1) <<" "<<computeUtilityOfConfidence(eventConfidence[k])<<"\n";
		// std::cout<<"Case 2: " <<computeWeightOfProb(1 - probEventType[k], 2) <<" "<<computeUtilityOfConfidence(eventConfidence[k])<<"\n";
		// std::cout<<"Case 3: " <<computeWeightOfProb(probEventType[k], 3) <<" "<<computeUtilityOfConfidence(1 - eventConfidence[k])<<"\n";
		// std::cout<<"Case 4: " <<computeWeightOfProb(1 - probEventType[k], 4) <<" "<<computeUtilityOfConfidence(1 - eventConfidence[k])<<"\n";

		std::cout <<"Sum Publish Prospect: " << sumPublishProspect <<"\n";
		//std::cout <<"Chosen event type: " << k<<" " <<"\n";
		if(event.getEventId().find("T_")!= std::string::npos 
			&& event.getEventId().find("F_T") == std::string::npos
		 	&& k == event.getEventTypeId() 
		 	&& utilityEventConfidence[k] >= 0 
		 	&& sumPublishProspect > 0
		 	){
		 	
		 	toBePublishedTrueEvents[event.getEventId()] = "PUB";
		 	//toBePublishedTrueEvents2[event] = "PUB";
		 	event.setEventCPTPublish("PUB");
		 	std::cout <<"PT PUBLISH TRUE EVENT: " << event.getEventId()<<" " << event.getEventTypeId()<<" " << event.getEventCPTPublish() <<"\n\n";
		
		 }
		 else if(event.getEventId().find("F_")!= std::string::npos 
			/* && event.getEventId().find("F_T") == std::string::npos */
		 	&& k == event.getEventTypeId() 
		 	&& utilityEventConfidence[k] >= 0 
		 	&& sumPublishProspect > 0
		 	){
		 	
		 	toBePublishedFalseEvents[event.getEventId()] = "PUB";
		 	//toBePublishedTrueEvents2[event] = "PUB";
		 	event.setEventCPTPublish("PUB");
		 	std::cout <<"PT PUBLISH FALSE EVENT: " << event.getEventId()<<" " << event.getEventTypeId()<<" " << event.getEventCPTPublish() <<"\n\n";
		
		 }
		 else{

		 	if(k >= 0 && utilityEventConfidence[k] >= 0 && sumPublishProspect > 0){
		 		std::ostringstream oss1, oss2;
				oss1 << k;
		        oss2 << event.getEpochNum();
				std::string fakeEventId;

				if(isTrueEvent == true){
					fakeEventId= "F_T_"+ oss1.str()+"_"+oss2.str();
				}
				else if (isTrueEvent == false){
					fakeEventId= "F_F_"+ oss1.str()+"_"+oss2.str();
				}
				toBePublishedFalseEvents[fakeEventId] = "PUB";
				//fakeEvent.setEventCPTPublish("PUB");

				//is CPT
				updateFakeEventInAllEvents(fakeEventId, "PUB", true);
		 	}
			
			toBePublishedTrueEvents[event.getEventId()] = "NOT_PUB";
			//toBePublishedTrueEvents2[event] = "NOT_PUB";
			event.setEventCPTPublish("NOT_PUB");
	 	}
	}
	//EUT
	// if(eventConfidence[EUT_k] < 0){
	// 	EUTPublishedTrueEvents[event.getEventId()] = "NOT_PUB";
	// }
	// else{

	for(int i = 0; i < 4; i++){
		if(EUT_max < eventConfidence[i]){
			EUT_max = eventConfidence[i];
			EUT_k = i;
		}
	}

	if(eventConfidence[EUT_k] <= 0){
		EUTPublishedTrueEvents[event.getEventId()] = "NOT_PUB";
		event.setEventEUTPublish("NOT_PUB");
	}

	else{
		std::cout <<"EUT Chosen Event Type: " << EUT_k <<" Confidence: "<< eventConfidence[EUT_k] <<"\n";
		// Whether to publish

		double R = 0.25;

		sumPublishEUT = (
						(cost[0] * probEventType[EUT_k] * eventConfidence[EUT_k]) + 
						(cost[1] * (1 - probEventType[EUT_k]) * eventConfidence[EUT_k])
						)  -	
						(
						(cost[2] * (probEventType[EUT_k]) * (1 - eventConfidence[EUT_k])) +
						(cost[3] * (1 - probEventType[EUT_k]) * (1 - eventConfidence[EUT_k]))
						);
		

		// sumPublishEUT = (cost[0] * probEventType[EUT_k] * (1 - exp(-1 * eventConfidence[EUT_k]/R))) + 
		// 					(cost[1] * (1 - probEventType[EUT_k]) * (1 - exp( -1 * eventConfidence[EUT_k]/R)))  -
		// 					((cost[2] * (probEventType[EUT_k]) * (1 - exp(-1 * (1 - eventConfidence[EUT_k])/R))) +
		// 					(cost[3] * (1 - probEventType[EUT_k]) * (1 - exp(-1 * (1 - eventConfidence[EUT_k])/R))));
		
		// std::cout<<"Case 1: " <<(probEventType[EUT_k]) <<" "<<eventConfidence[EUT_k]<<"\n";
		// std::cout<<"Case 2: " <<(1 - probEventType[EUT_k]) <<" "<<eventConfidence[EUT_k]<<"\n";
		// std::cout<<"Case 3: " <<(probEventType[EUT_k]) <<" "<<(1 - eventConfidence[EUT_k])<<"\n";
		// std::cout<<"Case 4: " <<(1 - probEventType[EUT_k]) <<" "<<(1 - eventConfidence[EUT_k])<<"\n";

		std::cout <<"Sum Publish EUT: " << sumPublishEUT <<"\n";
		//std::cout <<"EUT Chosen event type: " << EUT_k<<" " <<"\n";
		if(event.getEventId().find("T_")!= std::string::npos 
			&& event.getEventId().find("F_T") == std::string::npos
		 	&& EUT_k == event.getEventTypeId() 
		 	&& eventConfidence[EUT_k] >= 0 
		 	&& sumPublishEUT > 0){
		 	
		 	EUTPublishedTrueEvents[event.getEventId()] = "PUB";
		 	event.setEventEUTPublish("PUB");

		 	std::cout <<"EUT PUBLISH EVENT: " << event.getEventId()<<" " << event.getEventTypeId()<<"\n\n";
		
		 }
		 else if(event.getEventId().find("F_")!= std::string::npos 
			/* && event.getEventId().find("F_T") == std::string::npos */
		 	&& EUT_k == event.getEventTypeId() 
		 	&& eventConfidence[EUT_k] >= 0 
		 	&& sumPublishEUT > 0
		 	){
		 	
		 	EUTPublishedFalseEvents[event.getEventId()] = "PUB";
		 	//toBePublishedTrueEvents2[event] = "PUB";
		 	event.setEventEUTPublish("PUB");
		 	std::cout <<"EUT PUBLISH FALSE EVENT: " << event.getEventId()<<" " << event.getEventTypeId()<<" " << event.getEventEUTPublish() <<"\n\n";
		
		 }
		 else{

		 	if(EUT_k >= 0 && eventConfidence[EUT_k] >= 0 && sumPublishEUT > 0){
		 		std::ostringstream oss1, oss2;
				oss1 << EUT_k;
		        oss2 << event.getEpochNum();

				std::string fakeEventId;
				if(isTrueEvent == true){
					fakeEventId= "F_T_"+ oss1.str()+"_"+oss2.str();
				}
				else if (isTrueEvent == false){
					fakeEventId= "F_F_"+ oss1.str()+"_"+oss2.str();
				}

				EUTPublishedFalseEvents[fakeEventId] = "PUB";
				updateFakeEventInAllEvents(fakeEventId, "PUB", false);
				std::cout <<"EUT FALSE PUBLISH EVENT: " << fakeEventId<<"\n\n";
		 	}
			
			EUTPublishedTrueEvents[event.getEventId()] = "NOT_PUB";
			event.setEventEUTPublish("NOT_PUB");
	 	}
 	}
 	updateEventInAllEvents(event);
}

void updateEventInAllEvents(Event event){
	std::string eventId = event.getEventId();

	for(int i = 0; i < allEvents.size(); i++){
		if(allEvents[i].getEventId() == eventId){
			allEvents[i].setEventCPTPublish(event.getEventCPTPublish());
			allEvents[i].setEventEUTPublish(event.getEventEUTPublish());
			allEvents[i].setHistoricProb(event.getHistoricProb());
			allEvents[i].setHistoricProspectProb(event.getHistoricProspectProb());
			allEvents[i].setEventConfidence(event.getEventConfidence());
			allEvents[i].setEventUtilityConfidence(event.getEventUtilityConfidence());
			break;
		}
	}
}

void updateFakeEventInAllEvents(std::string eventId, std::string isPublished, bool isCPT){
	for(int i = 0; i < allEvents.size(); i++){
		if(allEvents[i].getEventId() == eventId){
			if(isCPT){

				allEvents[i].setEventCPTPublish(isPublished);
			}
			else{

				allEvents[i].setEventEUTPublish(isPublished);
			}
		}
	}
}




