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

/*
Generates random number between a certain range [min, max]
*/
int random(int min, int max) //range : [min, max)
{
    //std::cout << "Here, in random function,  the values are "<<min<<"   "<<max<<"\n\n";
   static bool first = true;
   if ( first )
   {
      //std::srand(time(NULL)); //seeding for the first time only!
     first = false;
   }
   return min + std::rand() % (max - min);
}



std::vector<Users> allMaliciousReportersWillReport(std::vector<Users> usersInRange, Event & event, bool isTrueEvent){
	std::vector<Users> malReportersInRange;
    for(int i = 0; i < usersInRange.size(); i++){
        //Users user = usersInRange[i];
       double randNum = (double) rand()/RAND_MAX;

       // std::cout << "User "<<user.getUserId()<< "  "<<randNum<<"\n\n";
        if((usersInRange[i].checkIfUserIsMalReporter()||
        		(randNum <= probOfSelfishReportersWhoReportFalseEvent && usersInRange[i].checkIfUserIsSelfishReporter())
            ||(randNum <= probOfHonestReportersWhoReportFalseEvent && usersInRange[i].checkIfUserIsReporter()))
        		&& !isTrueEvent){
            usersInRange[i].addReportedEvent(event);
            usersInRange[i].setMostRecentEvent(event);
          			std::vector<Event> falseEvents = usersInRange[i].getReportedEvents();

          			if(isDebugMode == 3){
          				std::cout <<"Mal Reporter           : "<<usersInRange[i].getUserId()<<"\n";
          				std::cout<<"# Reported False events : "<<falseEvents.size()<<"\n";
          				//Get all events
          				for(int j =0; j < falseEvents.size(); j++){
          					std::cout <<"False Event: "<<falseEvents[j].getEventId()<<" starttime: "<<falseEvents[j].getEventStartTime()<<" endtime: "<<falseEvents[j].getEventEndTime()<<"\n";
          				}
          				std::cout <<"\n";
          			}
          			malReportersInRange.push_back(usersInRange[i]);
        }
   }
    return malReportersInRange;
}

/**
* Only certain percentage of reporters in the range will
* report the true event
**/
std::vector<Users> certainPercReportersWillReport(std::vector<Users> usersInRange, Event event, bool isTrueEvent){
    //std::cout<<"Perc. Reporters who actually Report: "<< percOfReportersWhoReport<<"\n";
	if(isDebugMode == 2){
		std::cout <<"Randomly chosen reporters         : "<< "\n";
		std::cout <<"-------------------------START: REPORTERS-------------------------------\n";
	}

    std::vector<Users> reportersInRange;
    int countOfReportingUsers = 0;

   for(int i = 0; i < usersInRange.size(); i++){
        //Users user = usersInRange[i];
        double randNum = (double) rand()/RAND_MAX;

       // std::cout << "User "<<user.getUserId()<< "  "<<randNum<<"\n\n";
        if(((randNum <= probOfHonestReportersWhoReportTrueEvent && usersInRange[i].checkIfUserIsReporter()) ||
        		(randNum <= probOfMalReportersWhoReportTrueEvent && usersInRange[i].checkIfUserIsMalReporter()) ||
        		(randNum <= probOfSelfishReportersWhoReportTrueEvent && usersInRange[i].checkIfUserIsSelfishReporter()))
        		&& isTrueEvent){
            countOfReportingUsers++;
            //std::vector<Event> existingEvents = usersInRange[i].getReportedEvents();
            //if(existingEvents.size()>0)
            //   std::cout<<"Existing events: "<< existingEvents[0].getEventId()<<"\n";

            usersInRange[i].addReportedEvent(event);
            usersInRange[i].setMostRecentEvent(event);
            std::vector<Event> events = usersInRange[i].getReportedEvents();

            if(isDebugMode == 2){
            	std::cout <<"Reporter        : "<<usersInRange[i].getUserId()<<"\n";
            	std::cout<<"# Reported events: "<<events.size()<<"\n";
				//Get all events
				for(int i =0; i < events.size(); i++){
					std::cout <<"Event "<<events[i].getEventId()<<" starttime: "<<events[i].getEventStartTime()<<" endtime: "<<events[i].getEventEndTime()<<"\n";
				}
				std::cout <<"\n";
            }
            reportersInRange.push_back(usersInRange[i]);
        }
   }
   if(isDebugMode == 2){
		std::cout<<"\n\n# Reporters who actually reported: "<<countOfReportingUsers<<"\n";
		std::cout <<"-------------------------END: REPORTERS---------------------------------\n";
   }
    return reportersInRange;
}

/**
* All the raters in the range of the event will rate the event
*
**/
std::vector<Users> ratersWillRate(std::vector<Users> usersInRange, Event &event){
    //std::cout <<"All Raters rate : " <<"\n";
	if(isDebugMode == 2 || isDebugMode == 3){
		std::cout <<"--------------------------START: RATERS---------------------------------\n";
	}

    std::vector<Users> ratersInRange;
    int countOfRaters = 0;

    double positiveRatingsPerc = 0;
    double negativeRatingsPerc = 0;

    positiveRatingsPerc = event.checkIfEventFalse() == false ? positiveRatingsPercForTrueEvent:positiveRatingsPercForFalseEvent;
    negativeRatingsPerc = event.checkIfEventFalse() == false ? negativeRatingsPercForTrueEvent:negativeRatingsPercForFalseEvent;

    for(int i = 0; i < usersInRange.size(); i++){
           Users user = usersInRange[i];
           if(user.checkIfUserIsRater()){
               countOfRaters++;
           }
    }
    if(isDebugMode == 2 || isDebugMode == 3){
    	std::cout <<"Total number of Raters: "<< event.getEventId()<<" "<<countOfRaters<<"\n";
    }

    int noOfPositiveRaters = round(positiveRatingsPerc * countOfRaters);
    int noOfNegativeRaters = round(negativeRatingsPerc * countOfRaters);

  //   if(event.checkIfEventFalse() == false){ //Event is true
    	
  //   	if(countOfRaters - noOfPositiveRaters > 0)
  //   		noOfPositiveRaters = random(noOfPositiveRaters, countOfRaters);
    	
  //   	int potentialNumberOfNegativeRaters = 0;
    	
  //   	if(noOfNegativeRaters > 0)
  //  		 	potentialNumberOfNegativeRaters = random(0, noOfNegativeRaters);

		// if(potentialNumberOfNegativeRaters > (countOfRaters - noOfPositiveRaters)){
		// 	noOfNegativeRaters = countOfRaters - noOfPositiveRaters;
		// }
  //    	else
  //    		noOfNegativeRaters = potentialNumberOfNegativeRaters;
  //  	}
     	
  //   else{ //Event is false

  //   	if(noOfPositiveRaters > 0)
  //   		noOfPositiveRaters = random(0, noOfPositiveRaters);
    	
  //   	int potentialNumberOfNegativeRaters = 0;
    	
  //   	if(countOfRaters - noOfNegativeRaters > 0)
  //   		potentialNumberOfNegativeRaters = random(noOfNegativeRaters, countOfRaters);

  //   	if(potentialNumberOfNegativeRaters > (countOfRaters - noOfPositiveRaters)){
  //    		noOfNegativeRaters = countOfRaters - noOfPositiveRaters;
  //    	}
  //    	else{
  //    		noOfNegativeRaters = potentialNumberOfNegativeRaters;
  //    	}
  //   }

    //std::cout <<" Ratings Event: " <<event.getEventId()<<" "<<noOfPositiveRaters<<" "<<noOfNegativeRaters<<"\n";
    
  

    if(isDebugMode == 2 || isDebugMode == 3){
		std::cout<<"total no. of Positive Raters: " << positiveRatingsPerc<<"   "<<noOfPositiveRaters<<"\n";
		std::cout<<"total no. of Negative Raters: " << negativeRatingsPerc<<"    "<<noOfNegativeRaters<<"\n";
    }

    int countOfPositiveRaters = 0;
    int countOfNegativeRaters = 0;
    for(int i = 0; i < usersInRange.size(); i++){
        Users user = usersInRange[i];
        if(user.checkIfUserIsRater()){
        	if(countOfPositiveRaters < noOfPositiveRaters){
        		if(isDebugMode == 2 || isDebugMode == 3){
        			std::cout <<"Current Rater    : "<<user.getUserId()<<" =>Positive Rating   \n";
        		}
				event.setNoOfPositiveRatings(event.getNoOfPositiveRatings() + 1);
				countOfPositiveRaters++;
			}
        	else if(countOfNegativeRaters < noOfNegativeRaters){
        		if(isDebugMode == 2 || isDebugMode == 3){
        			std::cout <<"Current Rater    : "<<user.getUserId()<<" => Negative Rating   \n";
        		}
        		 event.setNoOfNegativeRatings(event.getNoOfNegativeRatings() + 1);
        		 countOfNegativeRaters++;
        	}
        	else{
        		if(isDebugMode == 2 || isDebugMode == 3){
        			std::cout <<"Current Rater    : "<<user.getUserId()<<" =>Uncertain Rating  \n";
        		}
        		 event.setNoOfUncertainRatings(event.getNoOfUncertainRatings() + 1);
        	}

        }
		user.addRatedEvent(event);
		user.setMostRecentEvent(event);


		std::vector<Event> events = user.getRatedEvents();
		//std::cout <<"\n";
		ratersInRange.push_back(user);
	}

    if(isDebugMode == 2 || isDebugMode == 3){
		std::cout<<"\n\n# Raters who rated: "<<countOfRaters<<"\n";
		std::cout <<"--------------------------END: RATERS-----------------------------------\n";
    }
    return ratersInRange;
}

/**
* All the raters in the range of the event will rate the event
*
**/
std::vector<Users> certainPercOfRatersWillRate(std::vector<Users> usersInRange, Event &event, double certainPercRaters){
    //std::cout <<"All Raters rate : " <<"\n";
	if(isDebugMode == 2 || isDebugMode == 3){
		std::cout <<"--------------------------START: RATERS---------------------------------\n";
	}

    std::vector<Users> ratersInRange;
    int countOfRaters = 0;

    double positiveRatingsPerc = 0;
    double negativeRatingsPerc = 0;

    positiveRatingsPerc = event.checkIfEventFalse() == false ? positiveRatingsPercForTrueEvent:positiveRatingsPercForFalseEvent;
    negativeRatingsPerc = event.checkIfEventFalse() == false ? negativeRatingsPercForTrueEvent:negativeRatingsPercForFalseEvent;

    for(int i = 0; i < usersInRange.size(); i++){
    	double randNum = (double) rand()/RAND_MAX;
           Users user = usersInRange[i];
           if(randNum <= certainPercRaters && user.checkIfUserIsRater()){
               countOfRaters++;
           }
    }
    if(isDebugMode == 2 || isDebugMode == 3){
    	std::cout <<"Total number of Raters: "<< countOfRaters<<"\n";
    }

    int noOfPositiveRaters = round(positiveRatingsPerc * countOfRaters);
    int noOfNegativeRaters = round(negativeRatingsPerc * countOfRaters);

    //  if(event.checkIfEventFalse() == false){ //Event is true
        
    //     if(countOfRaters - noOfPositiveRaters > 0)
    //         noOfPositiveRaters = random(noOfPositiveRaters, countOfRaters);
        
    //     int potentialNumberOfNegativeRaters = 0;
        
    //     if(noOfNegativeRaters > 0)
    //         potentialNumberOfNegativeRaters = random(0, noOfNegativeRaters);

    //     if(potentialNumberOfNegativeRaters > (countOfRaters - noOfPositiveRaters)){
    //         noOfNegativeRaters = countOfRaters - noOfPositiveRaters;
    //     }
    //     else
    //         noOfNegativeRaters = potentialNumberOfNegativeRaters;
    // }
        
    // else{ //Event is false

    //     if(noOfPositiveRaters > 0)
    //         noOfPositiveRaters = random(0, noOfPositiveRaters);
        
    //     int potentialNumberOfNegativeRaters = 0;
        
    //     if(countOfRaters - noOfNegativeRaters > 0)
    //         potentialNumberOfNegativeRaters = random(noOfNegativeRaters, countOfRaters);

    //     if(potentialNumberOfNegativeRaters > (countOfRaters - noOfPositiveRaters)){
    //         noOfNegativeRaters = countOfRaters - noOfPositiveRaters;
    //     }
    //     else{
    //         noOfNegativeRaters = potentialNumberOfNegativeRaters;
    //     }
    // }


    if(isDebugMode == 2 || isDebugMode == 3){
		std::cout<<"total no. of Positive Raters: " << positiveRatingsPerc<<"   "<<noOfPositiveRaters<<"\n";
		std::cout<<"total no. of Negative Raters: " << negativeRatingsPerc<<"    "<<noOfNegativeRaters<<"\n";
    }

    int countOfPositiveRaters = 0;
    int countOfNegativeRaters = 0;
    for(int i = 0; i < usersInRange.size(); i++){
        Users user = usersInRange[i];
        if(user.checkIfUserIsRater()){
        	if(countOfPositiveRaters < noOfPositiveRaters){
        		if(isDebugMode == 2 || isDebugMode == 3){
        			std::cout <<"Current Rater    : "<<user.getUserId()<<" =>Positive Rating   \n";
        		}
				event.setNoOfPositiveRatings(event.getNoOfPositiveRatings() + 1);
				countOfPositiveRaters++;
			}
        	else if(countOfNegativeRaters < noOfNegativeRaters){
        		if(isDebugMode == 2 || isDebugMode == 3){
        			std::cout <<"Current Rater    : "<<user.getUserId()<<" => Negative Rating   \n";
        		}
        		 event.setNoOfNegativeRatings(event.getNoOfNegativeRatings() + 1);
        		 countOfNegativeRaters++;
        	}
        	else{
        		if(isDebugMode == 2 || isDebugMode == 3){
        			std::cout <<"Current Rater    : "<<user.getUserId()<<" =>Uncertain Rating  \n";
        		}
        		 event.setNoOfUncertainRatings(event.getNoOfUncertainRatings() + 1);
        	}

        }
		user.addRatedEvent(event);
		user.setMostRecentEvent(event);


		std::vector<Event> events = user.getRatedEvents();
		//std::cout <<"\n";
		ratersInRange.push_back(user);
	}

    if(isDebugMode == 2 || isDebugMode == 3){
		std::cout<<"\n\n# Raters who rated: "<<countOfRaters<<"\n";
		std::cout <<"--------------------------END: RATERS-----------------------------------\n";
    }
    return ratersInRange;
}
void updateUsersWithReportedEvents(Users users[], std::vector<Users> reportersInRange){
    for(int i = 0; i < reportersInRange.size(); i++){
        for(int j = 0; j < noOfUsers; j++){
            if(reportersInRange[i].getUserId() == users[j].getUserId()){
                //if(usersInRange[i].getReportedEvents().size)
                //std::cout <<
                //std::cout<<" User score: " <<users[j].getScore()<<" "<<reportersInRange[i].getScore()<<"\n";
                users[j].addReportedEvent(reportersInRange[i].getMostRecentEvent());
                users[j].setMostRecentEvent(reportersInRange[i].getMostRecentEvent());
                //users[j].addMultipleReportedEvents(usersInRange[i].getReportedEvents());
                //users[j].addMultipleRatedEvents(usersInRange[i].getRatedEvents());
            }
        }
    }
}
void updateUsersWithRatedEvents(Users users[], std::vector<Users> ratersInRange){
    for(int i = 0; i < ratersInRange.size(); i++){
        for(int j = 0; j < noOfUsers; j++){
            if(ratersInRange[i].getUserId() == users[j].getUserId()){
                //if(usersInRange[i].getReportedEvents().size)
                //std::cout <<
                users[j].addRatedEvent(ratersInRange[i].getMostRecentEvent());
                //users[j].addMultipleReportedEvents(usersInRange[i].getReportedEvents());
                //users[j].addMultipleRatedEvents(usersInRange[i].getRatedEvents());
            }

        }

    }

}

int getEventTypeIdFromHistory(int epoch, double xPos, double yPos){

double realisticProbOfEventType[10][4] = {
                      {0.48, 0, 0.52, 0},
                      {0.75, 0.01, 0.2, 0.03},
                      {0.56, 0.01, 0.2, 0.22},
                      {0.66, 0.01, 0.32, 0},
                      {0.46, 0, 0.53, 0},
                      {0.85, 0.02, 0.12, 0},
                      {0.79, 0.01, 0.19, 0},
                      {0.73, 0.01, 0.25, 0},
                      {0.45, 0.02, 0.52, 0},
                      {0.45, 0.02, 0.52, 0}
                    };

  double chosenEventTypeId = 3;
  double randProb = (double) rand()/RAND_MAX;
  int regionX = (xPos* 4)/(2 * scenarioWidth);
  int regionY = yPos/(2 * scenarioWidth);
  int regionId = regionX + 5 * regionY;

  if(isDebugMode == 6 || isDebugMode == 8)
    std::cout << xPos <<" " << yPos <<" "<<regionX <<" "<<regionY<<" "<<regionId<<"\n";
  
  double currentProb = 0;
  //For event types
  for(int i = 0; i < 4; i++){
    currentProb +=realisticProbOfEventType[regionId][i];
    if(currentProb >= randProb){
      chosenEventTypeId = i;
      break;
    }
  }
  if(isDebugMode == 6 || isDebugMode == 8)
    std::cout <<"Epoch: " << epoch<<" P-rand: "<<  randProb <<" P-curr: " << currentProb<<" chosenTypeId: "<<chosenEventTypeId<<"\n";
  
  return chosenEventTypeId;
}

/*
All the actions that need to be taken after a true occurs is handled here.
1. A certain percentage of users in range of the event report the event
2. All the raters would rate the event
3. T_1_2 represents True Event, Current Epoch Number and Event Type ID.
*/
void handleTrueEvent(double probOfTrueEventOccurence[], int currentEpoch, Square eventSquare, Users users[]){
        double randProb = (double) rand()/RAND_MAX;

        if(isDebugMode == 2){
        	std::cout <<"\n\nTry True event simulation " << randProb <<"\n";
        }
        //Event occured
        if(randProb <= probOfTrueEventOccurence[currentEpoch]){
        	if(isDebugMode == 2){
        		std::cout <<"----------------True event occurs:------------------------------------- \n";
        	}
            //Get the event location
            Point eventLocation;
            Event trueEvent;
           // int eventWidth = 20; //a feature of type of event
           // double percOfReportersInRangeWhoActuallyReport = 1.0;
           // double positiveRatingsPerc = 0.5;
          //  double negativeRatingsPerc = 0.3;


            double xPos = random((int)eventSquare.getLl().getX(),(int) eventSquare.getUr().getX()) ;
            double yPos =random((int)eventSquare.getLl().getY(), (int)eventSquare.getUr().getY()) ;
            eventLocation.setX(xPos);
            eventLocation.setY(yPos);

            trueEvent.setEventLocation(eventLocation);
            trueEvent.setEventWidth(eventWidth);

            //Randomly assign event type
            //int eventTypeId = random(0,4);

            int eventTypeId = getEventTypeIdFromHistory(currentEpoch, xPos, yPos);

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

            trueEvent.setEventId(eventId);
            trueEvent.setEventTypeId(eventTypeId);
            trueEvent.setEpochNum(currentEpoch);
            trueEvent.setFalseEvent(false);

            //update certain perc of Reporters in Range
           std::vector<Users> usersInRange = getAllUsersInRange( users, eventLocation);
           //Certain perc of honest reporters will report the event
           std::vector<Users> reportersInRange = certainPercReportersWillReport(usersInRange, trueEvent, true);
           updateUsersWithReportedEvents(users, reportersInRange);

           std::vector<Users> ratersInRange;
           if(simulationCase == 1 && eventLocation.getX() > areaSquare[0].getLl().getX() && eventLocation.getX() < areaSquare[0].getUr().getX()
        		   && eventLocation.getY() > areaSquare[0].getLl().getY() && eventLocation.getY() < areaSquare[0].getUr().getY()){
        	   //ask all raters in range to rate the event
        	   ratersInRange = certainPercOfRatersWillRate(usersInRange, trueEvent, percOfRaters);
           }
           else{
        	    //ask all raters in range to rate the event
        	   ratersInRange = ratersWillRate(usersInRange, trueEvent);
           }

           updateUsersWithRatedEvents(users, ratersInRange);

           //Push the updated trueEvent in the event list
           allEvents.push_back(trueEvent);

           if(isDebugMode == 2){
			   std::cout<<"Ratings for all Events: "<<"\n";
			   for(int i = 0; i < allEvents.size(); i++){
					Event tEvent = allEvents[i];
					std::cout <<"For Event: "<< tEvent.getEventId()<<"\n";
					std::cout <<"Num +ve Ratings         :" << tEvent.getNoOfPositiveRatings()<<std::endl;
					std::cout <<"Num of -ve Ratings      :"<<tEvent.getNoOfNegativeRatings()<< "\n";
					std::cout <<"Num of Uncertain Ratings:"<<tEvent.getNoOfUncertainRatings()<<"\n\n";
			   }
           }
        }
}


void handleFalseEvent(double probOfFalseEventOccurence[], int currentEpoch, Square eventSquare, Users users[]){
        double randProb = (double) rand()/RAND_MAX;
        if(isDebugMode == 3){
        	std::cout <<"\n\nTry false event simulation " << randProb <<"\n";
        }
        //Event occured
        if(randProb < probOfFalseEventOccurence[currentEpoch]){
        	if(isDebugMode == 3){
        		std::cout <<"False Event Occurs \n";
        	}
            //Get the event location
            Point eventLocation;
            Event falseEvent;
            //int eventWidth = 5; //a feature of type of event
            //double percOfReportersInRangeWhoActuallyReport = 1.0;
            //double positiveRatingsPerc = 0.5;
            //double negativeRatingsPerc = 0.3;

            double xPos = random((int)eventSquare.getLl().getX(),(int) eventSquare.getUr().getX()) ;
            double yPos =random((int)eventSquare.getLl().getY(), (int)eventSquare.getUr().getY()) ;
            eventLocation.setX(xPos);
            eventLocation.setY(yPos);

            falseEvent.setEventLocation(eventLocation);
            falseEvent.setEventWidth(eventWidth);

            //Randomly assign event type
            int eventTypeId = random(0,4);

            std::ostringstream oss1, oss2;
            oss1 << currentEpoch;
            oss2 << eventTypeId;
           std::string eventId = "F_"+ oss2.str()+"_"+oss1.str();
            //trueEvent.setEventId(eventId);
            //std::cout <<"The false eventID is "<< eventId <<" eventTypeID is: "<< eventTypeId<<"\n";

           falseEvent.setEventId(eventId);
           falseEvent.setEventTypeId(eventTypeId);
           falseEvent.setEpochNum(currentEpoch);
           falseEvent.setFalseEvent(true);

           if(isDebugMode == 3){
			   std::cout <<"False Event ID      : "<< eventId<<"\n";
			   std::cout <<"False Event Type ID : "<< eventTypeId<<"\n";
			   std::cout <<"False Event Location: "<< "("<<eventLocation.getX()<<", "<<eventLocation.getY()<<")" <<"\n";
			   std::cout <<"False Event Width   : "<< eventWidth<<"\n";
           }
            //update certain perc of Reporters in Range
           //std::vector<Users> usersInRange = getAllUsersInRange( users, eventLocation);
           //allMaliciousReportersWillReport(usersInRange, falseEvent, false);
           //ratersWillRate(usersInRange, falseEvent, positiveRatingsPerc, negativeRatingsPerc);

          std::vector<Users> usersInRange = getAllUsersInRange( users, eventLocation);
		  std::vector<Users> malReportersInRange = allMaliciousReportersWillReport(usersInRange, falseEvent, false);
		  updateUsersWithReportedEvents(users, malReportersInRange);

		  //ask all raters in range to rate the event
		  std::vector<Users> ratersInRange = ratersWillRate(usersInRange, falseEvent);
		  updateUsersWithRatedEvents(users, ratersInRange);

		  //Push the updated falseEvent in the event list
		  allEvents.push_back(falseEvent);

		  if(isDebugMode == 3){
			  std::cout<<"Ratings for all Events: "<<"\n";
			  for(int i = 0; i < allEvents.size(); i++){
				   Event tEvent = allEvents[i];
				   std::cout <<"For Event: "<< tEvent.getEventId()<<"\n";
				   std::cout <<"Num +ve Ratings         :" << tEvent.getNoOfPositiveRatings()<<std::endl;
				   std::cout <<"Num of -ve Ratings      :"<<tEvent.getNoOfNegativeRatings()<< "\n";
				   std::cout <<"Num of Uncertain Ratings:"<<tEvent.getNoOfUncertainRatings()<<"\n\n";
			  }
		  }
        }
}


void moveAllUsersRandomly(Users users[], int scenarioWidth){
	if(isDebugMode == 1){
		std::cout<<"\nMove Users Randomly: \n\n";
	}
    for(int i =0; i < noOfUsers; i++){
            int currentXPos = users[i].getUserLocation().getX();
            int currentYPos = users[i].getUserLocation().getY();

            
            int maxNextXPos = (currentXPos + userMovementRange) < 2*scenarioWidth ? (currentXPos + userMovementRange) : 200;
            int maxNextYPos = (currentYPos + userMovementRange) < 2*scenarioWidth ? (currentYPos + userMovementRange) : 200;
            //int minNextXPos = (currentXPos - userMovementRange) > 0 ? (currentXPos - userMovementRange) : currentXPos;
            //int minNextYPos = (currentYPos - userMovementRange) > 0 ? (currentYPos - userMovementRange) : currentYPos;

            double newXPos = random(currentXPos, maxNextXPos );
            double newYPos = random(currentYPos, maxNextYPos );

            Point location;
            location.setX(newXPos);
            location.setY(newYPos);

           // std::cout<<"\nOld Location for User: "<<users[i].getUserId()<<"  => ("<<users[i].getUserLocation().getX()<<" , "<<users[i].getUserLocation().getY()<<")\n";

            users[i].setUserLocation(location);

           // std::cout<<"New Location for User: "<<users[i].getUserId()<<"  => ("<<users[i].getUserLocation().getX()<<" , "<<users[i].getUserLocation().getY()<<")\n";
    }
}

int getMaxNoOfRatings(){
	int maxNoOfRatings = -1;
	for(int i = 0; i < allEvents.size(); i++){
		if(allEvents[i].getNoOfRatings() > maxNoOfRatings){
			maxNoOfRatings = allEvents[i].getNoOfRatings();
		}
	}
	//std::cout<<"Max no of ratings: " << maxNoOfRatings<<"\n";
	return maxNoOfRatings;
}
/**
 * For Each Event:
 *  	belief      = (noOfPositiveRatings + 1)/(noOfPositiveRatings + noOfNegativeRatings + noOfUncertainRatings+3);
 *  	disBelief   = (noOfNegativeRatings + 1)/(noOfPositiveRatings + noOfNegativeRatings + noOfUncertainRatings+3);
 *  	uncertainty = (noOfUncertainRatings + 1)/(noOfPositiveRatings + noOfNegativeRatings + noOfUncertainRatings+3);
 *  	qoi         = belief +  a * uncertainty;
 */
void calculateQOIForAllEvents(){

	std::cout <<"\nTotal Number of Events: "<<allEvents.size()<<"\n";
	std::cout<<"QOI for All Events: \n";
	std::ostringstream oss, usersSize, percMal;
	oss << uncertainB;
	usersSize << noOfUsers;
	percMal << percOfMalReporters;
	std::ofstream qoiFile ("/Users/vijay/Desktop/Trust_Journal/"+usersSize.str()+"_"+percMal.str()+"/Our_QOI"+oss.str()+".csv");

	std::ostringstream oss1, oss2;
	oss1 << positiveRatingsPercForTrueEvent;
	oss2 << negativeRatingsPercForTrueEvent;

	std::ofstream positiveQOIFile("/Users/vijay/Desktop/Trust_Journal/"+usersSize.str()+"_"+percMal.str()+"/QOI_"+oss1.str()+".csv");
	std::ofstream negativeQOIFile("/Users/vijay/Desktop/Trust_Journal/"+usersSize.str()+"_"+percMal.str()+"/QOI_"+oss2.str()+".csv");

    int countPositiveEvents = 0;
    int countNegativeEvents = 0;
    for(int i = 0; i < allEvents.size(); i++){

    	//std::cout <<" Required ratings: " << (percOfTotalRatings*getMaxNoOfRatings()) <<"\n";
    	//we do not consider the events in our calculation which received less no of ratings than required no of ratings
    	 if(allEvents[i].getNoOfRatings() <= percOfTotalRatings*getMaxNoOfRatings()){
    		//std::cout<<"Not a valid event: "<<allEvents[i].getNoOfRatings()<<" "<<getMaxNoOfRatings()<<"\n";
			continue;
    	 }
    	 else{
			//Event tEvent = allEvents[i];
			allEvents[i].calculateWeightForUncertainty(uncertainWeightMax, uncertainA, uncertainB, thresholdN, phi);
			allEvents[i].calculateWeightForBelief(beliefWeightMax, beliefA, beliefB);
			allEvents[i].calculateQOI(noOfUsers, alpha, beta, lambda, referencePoint);
			//allEvents[i].setQOIWeight();

			double posRatingsRatio = (double) allEvents[i].getNoOfPositiveRatings()/allEvents[i].getNoOfRatings();
			if(isDebugMode == 4)
				std::cout <<"QOI for Event: "<<allEvents[i].getEventId()<<"   "<<allEvents[i].getQOI()<< "  "<< allEvents[i].getQOIWeight()<<" W(b) = "<<allEvents[i].getWeightForBelief()<<" W(ra) = "<<allEvents[i].getWeightForUncertainty()<<" Total Ratings "<<allEvents[i].getNoOfRatings()<<"   +veRatings: "<<allEvents[i].getNoOfPositiveRatings()<<"   -veRatings: "<<allEvents[i].getNoOfNegativeRatings()<<"   UncertainRatings: "<<allEvents[i].getNoOfUncertainRatings()<<"\n";
			if(isDebugMode == 5)
				std::cout <<"QOI: "<< allEvents[i].getEventId()<<" "<< allEvents[i].getJosangQOI()<<" "<<allEvents[i].getQOI()<<" "<<allEvents[i].getQOIWeight()<<" "<<allEvents[i].getNoOfRatings()<<" "<<allEvents[i].getNoOfPositiveRatings()<<" "<<allEvents[i].getNoOfNegativeRatings()<<" "<<allEvents[i].getNoOfUncertainRatings()<<" "<<allEvents[i].getWeightForBelief()<<" "<<allEvents[i].getWeightForUncertainty()<<"\n";

			if(isDebugMode == 6){
				std::string eventId = allEvents[i].getEventId();
				//if(posRatingsRatio >= (positiveRatingsPercForTrueEvent - 0.3))
				if(eventId[0]=='T'){
                    countPositiveEvents++;
					positiveQOIFile << allEvents[i].getEventId()<<" , "<< allEvents[i].getJosangQOI()<<" , "<<allEvents[i].getQOI()<<" , "<<allEvents[i].getQOIWeight()<<" , "<<allEvents[i].getNoOfRatings()<<" , "<<allEvents[i].getNoOfPositiveRatings()<<" , "<<allEvents[i].getNoOfNegativeRatings()<<" , "<<allEvents[i].getNoOfUncertainRatings()<<" , "<<allEvents[i].getWeightForBelief()<<" , "<<allEvents[i].getWeightForUncertainty()<<", "<< countPositiveEvents<<"\n";
                }
				else{
                    countNegativeEvents++;
					negativeQOIFile << allEvents[i].getEventId()<<" , "<< allEvents[i].getJosangQOI()<<" , "<<allEvents[i].getQOI()<<" , "<<allEvents[i].getQOIWeight()<<" , "<<allEvents[i].getNoOfRatings()<<" , "<<allEvents[i].getNoOfPositiveRatings()<<" , "<<allEvents[i].getNoOfNegativeRatings()<<" , "<<allEvents[i].getNoOfUncertainRatings()<<" , "<<allEvents[i].getWeightForBelief()<<" , "<<allEvents[i].getWeightForUncertainty()<<" , " <<countNegativeEvents<<"\n";
                }

				qoiFile <<  allEvents[i].getEventId()<<" , "<< allEvents[i].getJosangQOI()<<" , "<<allEvents[i].getQOI()<<" , "<<allEvents[i].getQOIWeight()<<" , "<<allEvents[i].getNoOfRatings()<<" , "<<allEvents[i].getNoOfPositiveRatings()<<" , "<<allEvents[i].getNoOfNegativeRatings()<<" , "<<allEvents[i].getNoOfUncertainRatings()<<" , "<<allEvents[i].getWeightForBelief()<<" , "<<allEvents[i].getWeightForUncertainty()<<"\n";

			}
    	 }
    }
    std::cout <<"calculateQOIForAllEvents END \n";
    qoiFile.close();
    positiveQOIFile.close();
    negativeQOIFile.close();
}

/**
 * For Each Reporter:
 * 		score = (sum of QOI of reportedEvents)/total # events
 */
void calculateOldScoreForAllReporters(Users users[]){
    double score = 0;
    std::cout<<"\nScore for all Reporters: \n\n";
    for(int i = 0; i < noOfUsers; i++){
        score = 0;
        //Users tUser = users[i];
        if(users[i].checkIfUserIsReporter() || users[i].checkIfUserIsMalReporter()){
                std::vector<Event> reportedEvents = users[i].getReportedEvents();
                //for(int i =0; i < reportedEvents.size(); i++){
               // std::cout <<"Score for Event: "<<reportedEvents[i].getEventId()<<" starttime: "<<reportedEvents[i].getEventStartTime()<<" endtime: "<<reportedEvents[i].getEventEndTime()<<"\n";
           // }
            for(int j = 0; j < reportedEvents.size(); j++ ){
                //Event tEvent = reportedEvents[j];
                for(int k = 0 ; k < allEvents.size(); k++){
                    //Event aEvent = allEvents[k];
                    if(reportedEvents[j].getEventId() == allEvents[k].getEventId() ){
                        score += allEvents[k].getQOI();
                        //std::cout <<"Inside1 score: "<<score << "  "<< allEvents[k].getQOI()<<"\n";
                    }
                }
            }
            int countOfTrueEvents = 0, countOfFalseEvents = 0;
            //Get the count of true event
            for(int i = 0; i < allEvents.size(); i++){
            	Event event = allEvents[i];
            	if(event.checkIfEventFalse()== false){
            		countOfTrueEvents++;
            	}
            	else
            		countOfFalseEvents++;
            }
            if(users[i].checkIfUserIsMalReporter())
            	score = score/countOfFalseEvents;
            else
            	score = score/countOfTrueEvents;
            users[i].setScore(score);
            std::cout<<"Reporter: " << users[i].getUserId() << " Score: " << users[i].getScore() <<"\n";
        }

    }
}

double calculateScoreSD(Users users[], double meanValue, bool isPositive){
	double totalPositiveSum = 0, totalNegativeSum = 0, totalPositiveCount = 0, totalNegativeCount = 0, sdValue = 0;
	for(int i = 0; i < noOfUsers; i++){
			if(!users[i].checkIfUserIsRater()){
				if(isPositive ){
					if(users[i].getRawScore() > 0){
						totalPositiveSum += pow((users[i].getRawScore() - meanValue), 2);
						totalPositiveCount++;
					}
				}
				else{
					if(users[i].getRawScore() < 0){
						if(isDebugMode == 4)
							std::cout<< pow((users[i].getRawScore() - meanValue), 2) <<" ";
						totalNegativeSum += pow((users[i].getRawScore() - meanValue), 2);
						totalNegativeCount++;
					}
				}
			}
	}
	if(isDebugMode == 1)
		std::cout<<"\n";
	if(isPositive)
		sdValue = pow((totalPositiveSum/totalPositiveCount), 0.5);
	else
		sdValue = pow((totalNegativeSum/totalNegativeCount), 0.5);

	return sdValue;
}

double calculateScoreMean(Users users[], bool isPositive){
	double totalPositiveCount = 0, totalNegativeCount = 0, totalPositiveScore = 0, totalNegativeScore = 0, meanValue = 0;

	for(int i = 0; i < noOfUsers; i++){
		if(!users[i].checkIfUserIsRater()){
				if(isPositive && users[i].getRawScore() > 0){
					totalPositiveScore += users[i].getRawScore();
					totalPositiveCount++;
					if(isDebugMode == 4)
						std::cout << users[i].getRawScore()<<" ";
				}
				else if(!isPositive && users[i].getRawScore() < 0){
					totalNegativeScore += users[i].getRawScore();
					totalNegativeCount++;

					if(isDebugMode == 4)
						std::cout << users[i].getRawScore()<<" ";
				}
			}
		}
	if(isDebugMode == 4)
		std::cout<<"\n";

	if(isPositive){
		meanValue = totalPositiveScore/totalPositiveCount;
	}
	else{
		meanValue = totalNegativeScore/totalNegativeCount;
	}

	return meanValue;
}
/**
 * For Each Reporter:
 * 		score = (sum of QOI of reportedEvents)/total # events
 */
/*if(weight < 0){
finalWeight = -(1 - exp(-1 * fabs(weight)));
}
else if ( weight > 0){
finalWeight = (1 - exp( -1 * fabs(weight)));
}
else{
finalWeight = 0;
}*/
//score += allEvents[k].getQOI() * weight;
// if(score < 0){
//				finalScore = -(1 - 2 * exp(-1 * pow(fabs(score), 0.2)));
//
//			}
//			else if ( score > 0){
//				finalScore = (1 - 2 * exp( -1 * pow(fabs(score),0.2)));
//			}
//			else{
//				finalScore = 0;
//			}
            //if(allEvents.size() > 0 )
            	//score = score/ allEvents.size();

void  dampsterProduct(std::vector<Event> allReportedEvents , int i){
   // int noOfRatingsA = allReportedEvents[0][0] + allReportedEvents[0][1] + allReportedEvents[0][2];
    double beliefA =   ds[0][0];
    double disBeliefA = ds[0][1];
    double uncertainA = ds[0][2];
    //beliefA = 1 - disBeliefA - uncertainA;

   // int noOfRatingsB = allReportedEvents[i][0] + allReportedEvents[i][1] + allReportedEvents[i][2];
    double beliefB = allReportedEvents[i].getPercOfPositiveRatings();
    double disBeliefB = allReportedEvents[i].getPercOfNegativeRatings();
    double uncertainB = allReportedEvents[i].getPercOfUncertainRatings();
    //beliefB = 1- uncertainB - disBeliefB;


    ds[0][0] = ((beliefA * beliefB) + (beliefA * uncertainB) + (uncertainA * beliefB))/(1 - ((beliefA * disBeliefB) + (disBeliefA * beliefB)));
    ds[0][1] = ((disBeliefA * disBeliefB) + (disBeliefA * uncertainB) + (uncertainA * disBeliefB))/(1 - ((disBeliefA * beliefB) + (beliefA * disBeliefB)));
    ds[0][2] = 1- ds[0][0] - ds[0][1];
}

Users* updateReportedEventsWithUpdatedInformation(Users users[]){
	for(int i = 0; i < noOfUsers; i++){
		int countTrueEvent = 0;
		int countFalseEvent = 0;
		std::vector<Event> reportedEvents = users[i].getReportedEvents();

		for(int j = 0; j < reportedEvents.size(); j++){
			for(int k = 0; k < allEvents.size(); k++){
				if(allEvents[k].getEventId() == reportedEvents[j].getEventId()){
					 if(allEvents[k].getNoOfRatings() <= percOfTotalRatings* getMaxNoOfRatings()){
						continue;
					}
					//reportedEvents[j].setNoOfRatings( allEvents[k].getNoOfRatings());
					reportedEvents[j].setNoOfPositiveRatings(allEvents[k].getNoOfPositiveRatings());
					reportedEvents[j].setNoOfNegativeRatings(allEvents[k].getNoOfNegativeRatings());
					reportedEvents[j].setNoOfUncertainRatings(allEvents[k].getNoOfUncertainRatings());
					//std::cout << "After udpation: "<<reportedEvents[j].getNoOfPositiveRatings()<<"\n";

//					if(!allEvents[k].checkIfEventFalse() && allEvents[k].getQOI() >= qoiThres){
					if(!allEvents[k].checkIfEventFalse()){
						countTrueEvent++;
					}
					else{
						countFalseEvent++;
					}

				}
			}
		}
		users[i].setNoOfTrueEventsReported(countTrueEvent);
		users[i].setNoOfFalseEventsReported(countFalseEvent);
		users[i].updateReportedEvents(reportedEvents);

		reportedEvents = users[i].getReportedEvents();
		//std::cout << "CHECK " <<reportedEvents[0].getNoOfPositiveRatings()<<"\n";

	}
	return users;
}
void calculateDampsterScoreForAllReporters(Users users[]){
	users = updateReportedEventsWithUpdatedInformation(users);

	for(int i =0 ; i < noOfUsers; i++){

		if(!users[i].checkIfUserIsRater()){
			std::vector<Event> reportedEvents = users[i].getReportedEvents();

			//if(users[i].getUserId() == "H_LL_89")
//			if(users[i].getUserId() == "S_LL_44")
//
//			{
//				std::cout <<" For user: " <<users[i].getUserId() <<" "<< reportedEvents.size()<<"\n";
//				for(int j = 0;  j < reportedEvents.size(); j++){
//					std::cout <<" ("<<reportedEvents[j].getNoOfRatings()<<" , "<<reportedEvents[j].getNoOfPositiveRatings() <<" , " << reportedEvents[j].getPercOfPositiveRatings()<<" ) \n";
//
//				}
//			}
			if(reportedEvents.size() == 1){
				//ds[0][0] = reportedEvents[0].getPercOfPositiveRatings();
				//ds[0][1] = reportedEvents[0].getPercOfPositiveRatings();
				users[i].setDSScore(reportedEvents[0].getPercOfPositiveRatings() - reportedEvents[0].getPercOfNegativeRatings());
				//std::cout <<" Reporter"<<"Score: " <<users[i].getUserId()<<"  "<< users[i].getDSScore()<<" "<<reportedEvents.size()<<"  "<<reportedEvents[0].getNoOfPositiveRatings() <<" "<< reportedEvents[0].getNoOfNegativeRatings()<<"\n";
			}
			//std::cout <<"CHECK: " << reportedEvents[0].getNoOfPositiveRatings()<<"\n";
			if(reportedEvents.size()>1){
				ds[0][0] = reportedEvents[0].getPercOfPositiveRatings();
				ds[0][1] = reportedEvents[0].getPercOfNegativeRatings();
				ds[0][2] = reportedEvents[0].getPercOfUncertainRatings();

				//ds[0][2] = 1 - ds[0][0] - ds[0][1];

				//if(users[i].getUserId() == "H_LL_89")
//				if(users[i].getUserId() == "S_LL_44")
//					std::cout <<"Event 1:       "<<ds[0][0]<<" "<<ds[0][1]<<" "<<ds[0][2]<<" "<< (ds[0][0] + ds[0][1] + ds[0][2])<<"\n";
				for(int j =1 ; j < reportedEvents.size(); j++){
					if(reportedEvents[j].getNoOfRatings() <= percOfTotalRatings * getMaxNoOfRatings()){
						continue;
					}
					else{
						//std::cout << ds[0][0]<<" "<<ds[0][1]<<" "<<ds[0][2]<<"\n";
						//std::cout<< reportedEvents[j].getPercOfPositiveRatings()<<" "<<reportedEvents[j].getPercOfNegativeRatings()<<" "<<reportedEvents[j].getPercOfUncertainRatings()<<"\n";
						//if(users[i].getUserId() == "H_LL_89"){
//						if(users[i].getUserId() == "S_LL_44"){
//							std::cout <<"Until Now: "<<ds[0][0]<<" "<<ds[0][1]<<" "<<ds[0][2]<<" "<< (ds[0][0] + ds[0][1] + ds[0][2])<<"\n";
//							std::cout<<"Event "<<j+1<<"  : "<< reportedEvents[j].getPercOfPositiveRatings()<<" "<<reportedEvents[j].getPercOfNegativeRatings()<<" "<<reportedEvents[j].getPercOfUncertainRatings()<<" "<<(reportedEvents[j].getPercOfPositiveRatings()+reportedEvents[j].getPercOfNegativeRatings()+reportedEvents[j].getPercOfUncertainRatings())<<"\n";
//						}
						dampsterProduct(reportedEvents, j);
						//if(users[i].getUserId() == "H_LL_89"){
//						if(users[i].getUserId() == "S_LL_44"){
//							//std::cout<<"Event "<<j+1<<" "<< reportedEvents[j].getPercOfPositiveRatings()<<" "<<reportedEvents[j].getPercOfNegativeRatings()<<" "<<reportedEvents[j].getPercOfUncertainRatings()<<"\n";
//							std::cout <<"Combined : "<< ds[0][0]<<" "<<ds[0][1]<<" "<<ds[0][2]<<" "<<(ds[0][0] + ds[0][1] + ds[0][2])<<"\n\n";
//
//						}

					}
				}
				users[i].setDSScore(ds[0][0] - ds[0][1]);
				//std::cout <<" Reporter"<<"Score: " <<users[i].getUserId()<<"  "<< users[i].getDSScore()<<" "<<reportedEvents.size()<<"  "<<reportedEvents[0].getNoOfPositiveRatings() <<" "<< reportedEvents[0].getNoOfNegativeRatings()<<"\n";

			}
		}
	}
	sortDSScoreAsReportersBasedOnParticipationAndTruthFulness(users);
	showDSScoreBasedOnPositiveEvents(users);
}

void showDSScoreBasedOnPositiveEvents(Users users[]){
		int countOfUsers = 0;
		std::ostringstream oss1, oss2, usersSize, percMal;
		double positiveRatingRatio = 0.0;
		 usersSize << noOfUsers;
		 percMal << percOfMalReporters;
		 std::ofstream dsScore0_20("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str() +"_"+percMal.str()+"/DS_Score_0_0.2.csv");
		 std::ofstream dsScore20_40 ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/DS_Score_0.2_0.4.csv");
		 std::ofstream dsScore40_60 ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/DS_Score_0.4_0.6.csv");
		 std::ofstream dsScore60_80 ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/DS_Score_0.6_0.8.csv");
		 std::ofstream dsScore80_100 ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/DS_Score_0.8_1.csv");

			for(int i = 0; i < noOfUsers; i++){
					if(users[i].getNoOfEventsReported() == 0 )
						continue;
					else{

						positiveRatingRatio = (double) users[i].getNoOfTrueEventsReported()/users[i].getNoOfEventsReported();
						//std::cout <<"Positive Rating Ratio: " << positiveRatingRatio<<" \n";
						if(positiveRatingRatio <= 0.25){
							countOfUsers++;
							//std::cout<<"Reporter_0_0.2: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";

							if(isDebugMode == 6){
								if(dsScore0_20.is_open()){
									dsScore0_20 << "Reporter_0_0.25: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
								}
							}
						}
						else if(0.25 < positiveRatingRatio && positiveRatingRatio <= 0.5){
								countOfUsers++;

								if(isDebugMode == 6){
									if(dsScore20_40.is_open()){
										dsScore20_40 << "Reporter_0.25_0.5: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
									}
								}
							}
						else if(0.5 < positiveRatingRatio  && positiveRatingRatio <= 0.75){
								countOfUsers++;

								if(isDebugMode == 6){
									if(dsScore40_60.is_open()){
										dsScore40_60 << "Reporter_0.5_0.75: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
									}
								}
							}
						else if(0.75 < positiveRatingRatio && positiveRatingRatio <= 1){
								countOfUsers++;

								if(isDebugMode == 6){
									if(dsScore60_80.is_open()){
										dsScore60_80 << "Reporter_0.6_0.8: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
									}
								}
							}
//						else if(positiveRatingRatio > 0.8){
//								countOfUsers++;
//
//								if(isDebugMode == 6){
//									if(dsScore80_100.is_open()){
//										dsScore80_100 << "Reporter_0.8_1: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
//									}
//								}
//							}
					}

			}
			dsScore0_20.close();
			dsScore20_40.close();
			dsScore40_60.close();
			dsScore60_80.close();
			dsScore80_100.close();
}
void sortDSScoreAsReportersBasedOnParticipationAndTruthFulness(Users users[]){
	double positiveRatingRatio = 0;
	int countOfUsers = 0;
	std::ostringstream oss1, oss2, usersSize , percMal;
	usersSize << noOfUsers;
	percMal << percOfMalReporters;
	 std::ofstream dsScore("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/DS_Score.csv");
	 std::ofstream dsMal ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/DS_Score_malicious.csv");
	 std::ofstream dsSel ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/DS_Score_selfish.csv");
	 std::ofstream dsHonest ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/DS_Score_honest.csv");


	//Malicious Guy
		for(int i = 0; i < noOfUsers; i++){


				if(users[i].getNoOfEventsReported() <= percOfEventsThres * allEvents.size())
					continue;
				else{

					//positiveRatingRatio = (double) users[i].getNoOfTrueEventsReported()/users[i].getNoOfEventsReported();

					//std::cout<<" Malicious: " << positiveRatingRatio<<" "<< ( 1 - positiveRatingRatioThres)<<"\n";
					//if(positiveRatingRatio <= (1 - positiveRatingRatioThres)){
					std::string userId = users[i].getUserId();
					if(userId[0] =='M'){
						countOfUsers++;
						if(isDebugMode == 4)
							std::cout <<"Malicious Reporter: "+ users[i].getUserId()<<" Score: "<<users[i].getDSScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";
						else if(isDebugMode == 5)
							std::cout <<"MaliciousReporter: " <<countOfUsers << " " << users[i].getUserId()<<" "<<users[i].getDSScore()<<"  "<<users[i].getNoOfEventsReported()<< "  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

						else if(isDebugMode == 6){
							if(dsMal.is_open()){
								dsMal << "MaliciousReporter: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
														}
							if(dsScore.is_open()){
								dsScore << "MaliciousReporter: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
							}
						}
					}
				}

			}
			dsMal.close();

			//Not participated guys
//			for(int i = 0; i < noOfUsers; i++){
//				if(users[i].getNoOfEventsReported() == 0 && !users[i].checkIfUserIsReporter()){
//					countOfUsers++;
//					if(isDebugMode == 4)
//						std::cout <<"NotParticipatedReporter: "+ users[i].getUserId()<<" Score: "<<users[i].getDSScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";
//					else if(isDebugMode == 5)
//						std::cout <<"NotParticipatedReporter: " <<countOfUsers << " " << users[i].getUserId()<<" "<<users[i].getDSScore()<<"  "<<users[i].getNoOfEventsReported()<< "  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";
//
//					else if(isDebugMode == 6){
//						if(dsMal.is_open()){
//							dsMal << "NotParticipatedReporter: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
//						}
//						if(dsScore.is_open()){
//							dsScore << "NotParticipatedReporter: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
//						}
//					}
//				}
//			}

	//Selfish Guy
	for(int i = 0; i < noOfUsers; i++){

		if(users[i].getNoOfEventsReported() <= percOfEventsThres * allEvents.size())
			continue;

			else{

//				positiveRatingRatio = (double) users[i].getNoOfTrueEventsReported()/users[i].getNoOfEventsReported();
//				if((positiveRatingRatio < positiveRatingRatioThres && positiveRatingRatio > (1 - positiveRatingRatioThres))){

				std::string userId = users[i].getUserId();
				if(userId[0] =='S'){
					countOfUsers++;
					if(isDebugMode == 4)
						std::cout <<"Selfish Reporter: "+ users[i].getUserId()<<" Score: "<<users[i].getDSScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";
					else if(isDebugMode == 5)
						std::cout <<"SelfishReporter: "<< countOfUsers <<" "<< users[i].getUserId()<<" "<<users[i].getDSScore()<<"  "<<users[i].getNoOfEventsReported()<< "  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

					else if(isDebugMode == 6){
						if(dsSel.is_open()){
							dsSel <<"SelfishReporter: "<<" , "<< users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" , "<<users[i].getNoOfEventsReported()<< " ,  "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";

						}
						if(dsScore.is_open()){
							dsScore <<"SelfishReporter: "<<" , "<< users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" , "<<users[i].getNoOfEventsReported()<< " ,  "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
						}
					}
				}
			}

		}
		dsSel.close();

	//Honest Guy
		for(int i = 0; i < noOfUsers; i++){


			if(users[i].getNoOfEventsReported() <= percOfEventsThres * allEvents.size())
				continue;
			else{

//				positiveRatingRatio = (double) users[i].getNoOfTrueEventsReported()/users[i].getNoOfEventsReported();
//
//				if(positiveRatingRatio >= positiveRatingRatioThres){
				std::string userId = users[i].getUserId();
				if(userId[0] =='H'){
					countOfUsers++;
					if(isDebugMode == 4)
						std::cout <<"Honest Reporter: "+ users[i].getUserId()<<" Score: "<<users[i].getDSScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";
					else if(isDebugMode == 5)
						std::cout <<"HonestReporter: "<< countOfUsers<<" "<< users[i].getUserId()<<" "<<users[i].getDSScore()<<"  "<<users[i].getNoOfEventsReported()<< "  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

					else if(isDebugMode == 6){
						if(dsHonest.is_open()){
							dsHonest <<"HonestReporter: "<< " , " << users[i].getUserId()<<" , "<< countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
						}
						if(dsScore.is_open()){
							dsScore <<"HonestReporter: "<< " , " << users[i].getUserId()<<" , "<< countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
						}
					}
				}
			}
		}
		dsHonest.close();
		dsScore.close();
}

void calculateScoreForAllReporters(Users users[]){

    double score = 0, weight = 0, countOfTrueEvents = 0, countOfFalseEvents = 0;
    if(isDebugMode == 4)
    	std::cout<<"\nScore for all Reporters: \n\n";

    for(int i = 0; i < noOfUsers; i++){
        score = 0;
        countOfTrueEvents = 0;
        countOfFalseEvents = 0;
        //Users tUser = users[i];
        if(!users[i].checkIfUserIsRater()){
            std::vector<Event> reportedEvents = users[i].getReportedEvents();
                
            //Checking

            // for(int j = 0; j < reportedEvents.size(); j++ ){
            //   if(users[i].getUserId() == "H_LL_194" && isDebugMode == 6){
            //     std::cout << " " << reportedEvents[j].getEventId() << " , " ;
            //   }
            // }
            
            // for(int k = 0 ; k < allEvents.size(); k++){
            //  if(users[i].getUserId() == "H_LL_194" && isDebugMode == 6){
            //     std::cout << " " << allEvents[k].getEventId() << " , " ;
            //   }
            // }

            for(int j = 0; j < reportedEvents.size(); j++ ){
                
                //Event tEvent = reportedEvents[j];

                for(int k = 0 ; k < allEvents.size(); k++){
                   
                    //Event aEvent = allEvents[k];
                    //
                    if(reportedEvents[j].getEventId() == allEvents[k].getEventId() ){
                        
                    	//we do not consider the events which has less no of ratings than required no of ratings
                        if(allEvents[k].getNoOfRatings() <= percOfTotalRatings* getMaxNoOfRatings()){
                        	continue;
                        }
                        else{
          								//weight = log(allEvents[k].getQOI()/ (1 - allEvents[k].getQOI()));
          								score += allEvents[k].getQOIWeight();
          								
          								// allEvents[k].getQOI() > qoiThres
          								if(!reportedEvents[j].checkIfEventFalse()){
          									countOfTrueEvents++;
          								}
          								else
          									countOfFalseEvents++;
          								//std::cout <<"Inside1 score: "<<score << "  "<< allEvents[k].getQOI()<<"\n";

          								if(users[i].getUserId() == "H_LL_135" && isDebugMode == 6){
          									std::cout <<" User  H_LL_135: ("<< reportedEvents[j].getEventId() << " , " << allEvents[k].getEventId()<<" , "<<allEvents[k].getNoOfRatings()<<" , "<<allEvents[k].getNoOfPositiveRatings()<<" , "<<allEvents[k].getQOI()<<" , "<<allEvents[k].getQOIWeight()<< " ) \n";
          								}
                        }
                    }
                }
            }
            users[i].setRawScore(score);
            users[i].setNoOfTrueEventsReported(countOfTrueEvents);
            users[i].setNoOfFalseEventsReported(countOfFalseEvents);

        }

    }
    if(isDebugMode == 5)
    	std::cout <<" Raw Scores: \n\n";
    sortRawScoreAsTypeOfReporter(users);
    if(isDebugMode == 4)
    	std::cout<<"All positive scores: \n";

   double positiveScoreMean = calculateScoreMean(users, true);

   if(isDebugMode == 4)
	   std::cout<<"All negative scores: \n";

   double negativeScoreMean = calculateScoreMean(users, false);
   double positiveScoreSD = calculateScoreSD(users, positiveScoreMean, true);

   if(isDebugMode == 1)
	   std::cout <<"Negative SD values : \n";

   double negativeScoreSD = calculateScoreSD(users,negativeScoreMean, false);

   if(isDebugMode == 4){
	   std::cout <<" The means are : " << positiveScoreMean<<"  " << negativeScoreMean<<"\n";
	   std::cout <<" The SDs are   : " << positiveScoreSD <<"  " << negativeScoreSD <<"\n";
   }
   calculateFinalScoreForAllReporters(users, positiveScoreMean, negativeScoreMean, positiveScoreSD, negativeScoreSD);

}


void calculateFinalScoreForAllReporters(Users users[], double pScoreMean, double nScoreMean, double pScoreSD, double nScoreSD){
	double finalScore = 0;
	for(int i = 0; i < noOfUsers; i++){
	   if(!users[i].checkIfUserIsRater()){
		   	   if(users[i].getRawScore() > 0){
		   		 finalScore = 1/ (1 + exp(-1 * ((users[i].getRawScore() - pScoreMean)/pScoreSD)));
		   		//finalScore = (1 - exp(-1 * fabs((users[i].getRawScore() - pScoreMean)/pScoreSD)));
		   	   }
		   	   else if(users[i].getRawScore() < 0){
		   		finalScore = -1 * (1/ (1 + exp(-1 * (fabs(users[i].getRawScore()) - fabs(nScoreMean))/nScoreSD)));
		   		//finalScore = -(1 - exp(-1 * fabs((fabs(users[i].getRawScore()) - fabs(nScoreMean))/nScoreSD)));

		   	   }
		   	   else{
		   		   finalScore = users[i].getRawScore();
		   		  // std::cout <<" Here I should be: "<<users[i].getUserId()<<" "<<finalScore<<"\n";
		   	   }
		   	   users[i].setScore(finalScore);
	   }
}
//	std::cout <<"Final Scores:\n\n";
	//sortScoreAsTypeOfReporter(users);
	sortScoreAsReportersBasedOnParticipationAndTruthFulness(users);
	//sortScoreForHonestReporters(users);
	// showOurScoreBasedOnPositiveEvents(users);
}

void incentivizeUsersBasedOnReputation(Users users[], double reward){
	double incentiveOur = 0, incentiveDS = 0, totalIncentiveOur = 0, totalIncentiveDS = 0, incentiveRatio = 0;
	double sumOfFinalPositiveScore = 0, noOfReportersWithPositiveReputation = 0, noOfReporters = 0;


    std::ostringstream usersSize, percMal;
    usersSize << noOfUsers;
    percMal << percOfMalReporters;
    std::ofstream ourIncentive("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Incentive.csv");
    std::ofstream ourMalIncentive("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Incentive_malicious.csv");
    std::ofstream ourSelIncentive("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Incentive_selfish.csv");
    std::ofstream ourHonestIncentive("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Incentive_honest.csv");
    std::ofstream incentiveDistribution("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/incentiveDistribution.csv");

	//calculate sum of reputation scores of all reporters with positive reputation score
	for(int i = 0 ; i < noOfUsers; i++){
		if(!users[i].checkIfUserIsRater()){
			if(users[i].getScore() > 0){
				sumOfFinalPositiveScore += users[i].getScore();
				noOfReportersWithPositiveReputation++;
			}
			noOfReporters++;
		}
	}
	std::cout <<" No of Reporters " << noOfReporters<<"\n";
	std::cout <<" No of Positive Reporters "<<noOfReportersWithPositiveReputation <<"\n";
	std::cout <<" Sum of Final Positive Score " << sumOfFinalPositiveScore <<"\n";

    int countHonestReporters = 1;
    double minIncentiveOur = 999, maxIncentiveOur = -1;
    double minIncentiveDS = 999, maxIncentiveDS = -1;

	for(int i = 0 ; i < noOfUsers; i++){
		if(!users[i].checkIfUserIsRater()){

            if(users[i].getNoOfEventsReported() <= percOfEventsThres * allEvents.size()){
                //std::cout << "here I am "<<users[i].getUserId()<<" "<<users[i].getNoOfEventsReported()<<"\n";
                continue;
            }

			std::string userId = users[i].getUserId();

			if(users[i].getScore() > 0 && userId[0] == 'H')
            {
				//(users[i].getScore()/sumOfFinalPositiveScore) *
				incentiveOur = (users[i].getScore()/sumOfFinalPositiveScore) * ((reward *  noOfReportersWithPositiveReputation)/noOfReporters);
				incentiveDS = (users[i].getDSScore()/sumOfFinalPositiveScore) * ((reward *  noOfReportersWithPositiveReputation)/noOfReporters);
				//std::cout <<users[i].getUserId()<<" "<<users[i].getScore()<<" "<<incentiveOur<<" "<< users[i].getDSScore()<<" "<<incentiveDS<<"\n";
				
				totalIncentiveDS += incentiveDS;
				totalIncentiveOur += incentiveOur;

                if(minIncentiveOur > incentiveOur){
                    minIncentiveOur = incentiveOur;
                }

                if(minIncentiveDS > incentiveDS){
                    minIncentiveDS = incentiveDS;
                }

                if(maxIncentiveOur < incentiveOur){
                    maxIncentiveOur = incentiveOur;
                }

                if(maxIncentiveDS < incentiveDS){
                    maxIncentiveDS = incentiveDS;
                }

				//users[i].setOurIncentives(incentiveOur);
                //users[i].setDSIncentives(incentiveDS);
                
                if(isDebugMode == 4)
                    std::cout <<"Honest Reporter: "<< users[i].getUserId()<<" Our Incentives: "<<incentiveOur<<" DS Incentives: "<<incentiveDS<<"\n";
                else if(isDebugMode == 5)
                    std::cout <<"HonestReporter: "<< users[i].getUserId()<<" Our Incentives: "<<incentiveOur<<" DS Incentives: "<<incentiveDS<<"\n";
                else if(isDebugMode == 6)
                {
                    if(ourHonestIncentive.is_open()){
                        ourHonestIncentive <<"HonestReporter: , "<<countHonestReporters<<" , "<< users[i].getUserId()<<" , "<<users[i].getScore()<<", "<<users[i].getDSScore()<<" , "<<incentiveOur<<" , " <<incentiveDS<<"\n";
                    }
                    if(ourIncentive.is_open()){
                        ourIncentive <<"HonestReporter: , " << users[i].getUserId()<<" , "<<users[i].getScore()<<", "<<users[i].getDSScore()<<" , "<<incentiveOur<<" , " <<incentiveDS<<"\n";
                    }
                }
                countHonestReporters++ ;
			}
		}
	}
    std::cout << "Honest: Our Incentives: " << " Min: " << minIncentiveOur<<" Mean: "<<(totalIncentiveOur)/countHonestReporters <<" Max: "<< maxIncentiveOur<<"\n";
    std::cout << "Honest: DS Incentives: " << " Min: "<< minIncentiveDS<<" Mean: "<<(totalIncentiveDS)/countHonestReporters<< " Max: "<<maxIncentiveDS <<"\n";

    incentiveDistribution <<"Honest: Our Incentives: " << " , " << minIncentiveOur<<" , "<<(totalIncentiveOur)/countHonestReporters <<" , "<< maxIncentiveOur<<"\n";
    incentiveDistribution << "Honest: DS Incentives: " << " , "<< minIncentiveDS<<" , "<<(totalIncentiveDS)/countHonestReporters<< " , "<<maxIncentiveDS <<"\n";

    ourHonestIncentive.close();


    int countSelReporters = 1;
    totalIncentiveDS = 0;
    totalIncentiveOur = 0;
    minIncentiveOur = 999;
    maxIncentiveOur = -1;
    minIncentiveDS = 999;
    maxIncentiveDS = -1;
    for(int i = 0 ; i < noOfUsers; i++){
        if(!users[i].checkIfUserIsRater()){

            if(users[i].getNoOfEventsReported() <= percOfEventsThres * allEvents.size()){
                //std::cout << "here I am "<<users[i].getUserId()<<" "<<users[i].getNoOfEventsReported()<<"\n";
                continue;
            }

            std::string userId = users[i].getUserId();

            if(users[i].getScore() > 0 && userId[0] == 'S')
            {
                //(users[i].getScore()/sumOfFinalPositiveScore) *
                incentiveOur = (users[i].getScore()/sumOfFinalPositiveScore) * ((reward *  noOfReportersWithPositiveReputation)/noOfReporters);
                incentiveDS = (users[i].getDSScore()/sumOfFinalPositiveScore) * ((reward *  noOfReportersWithPositiveReputation)/noOfReporters);
                //std::cout <<users[i].getUserId()<<" "<<users[i].getScore()<<" "<<incentiveOur<<" "<< users[i].getDSScore()<<" "<<incentiveDS<<"\n";
                
                totalIncentiveDS += incentiveDS;
                totalIncentiveOur += incentiveOur;

                if(minIncentiveOur > incentiveOur){
                    minIncentiveOur = incentiveOur;
                }

                if(minIncentiveDS > incentiveDS){
                    minIncentiveDS = incentiveDS;
                }

                if(maxIncentiveOur < incentiveOur){
                    maxIncentiveOur = incentiveOur;
                }

                if(maxIncentiveDS < incentiveDS){
                    maxIncentiveDS = incentiveDS;
                }

               if(isDebugMode == 4)
                    std::cout <<"Selfish Reporter: "<< users[i].getUserId()<<" Our Incentives: "<<incentiveOur<<" DS Incentives: "<<incentiveDS<<"\n";
                else if(isDebugMode == 5)
                    std::cout <<"SelfishReporter: "<< users[i].getUserId()<<" Our Incentives: "<<incentiveOur<<" DS Incentives: "<<incentiveDS<<"\n";
                else if(isDebugMode == 6)
                {
                    if(ourSelIncentive.is_open()){
                        ourSelIncentive <<"SelfishReporter: , "<< countSelReporters<<" , "<<users[i].getUserId()<<" , "<<users[i].getScore()<<", "<<users[i].getDSScore()<<" , "<<incentiveOur<<" , " <<incentiveDS<<"\n";
                    }
                    if(ourIncentive.is_open()){
                        ourIncentive <<"SelfishReporter: , " << users[i].getUserId()<<" , "<<users[i].getScore()<<", "<<users[i].getDSScore()<<" , "<<incentiveOur<<" , " <<incentiveDS<<"\n";
                    }
                }
                countSelReporters++;
            }
        }
    }
    std::cout << "Selfish: Our Incentives: " << " Min: " << minIncentiveOur<<" Mean: "<<(totalIncentiveOur)/countHonestReporters <<" Max: "<< maxIncentiveOur<<"\n";
    std::cout << "Selfish: DS Incentives: " << " Min: "<< minIncentiveDS<<" Mean: "<<(totalIncentiveDS)/countHonestReporters<< " Max: "<<maxIncentiveDS <<"\n";

    incentiveDistribution <<"Selfish: Our Incentives: " << " , " << minIncentiveOur<<" , "<<(totalIncentiveOur)/countHonestReporters <<" , "<< maxIncentiveOur<<"\n";
    incentiveDistribution <<"Selfish: DS Incentives: " << " , "<< minIncentiveDS<<" , "<<(totalIncentiveDS)/countHonestReporters<< " , "<<maxIncentiveDS <<"\n";

    ourSelIncentive.close();
    ourIncentive.close();
    incentiveDistribution.close();
	//std::cout <<"Ratio of our to DS: " << (totalIncentiveOur/totalIncentiveDS) <<" \n";

}



void showOurScoreBasedOnPositiveEvents(Users users[]){
		int countOfUsers = 0;
		std::ostringstream oss1, oss2, usersSize , percMal;
		double positiveRatingRatio = 0.0;
		usersSize << noOfUsers;
		percMal << percOfMalReporters;
		 std::ofstream ourScore0_20("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score_0_0.2.csv");
		 std::ofstream ourScore20_40 ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score_0.2_0.4.csv");
		 std::ofstream ourScore40_60 ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score_0.4_0.6.csv");
		 std::ofstream ourScore60_80 ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score_0.6_0.8.csv");
		 std::ofstream ourScore80_100 ("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score_0.8_1.csv");

			for(int i = 0; i < noOfUsers; i++){
				if(users[i].getNoOfEventsReported() < percOfEventsThres * allEvents.size())
						continue;
					else{

						positiveRatingRatio = (double) users[i].getNoOfTrueEventsReported()/users[i].getNoOfEventsReported();
						//std::cout <<"Positive Rating Ratio: " << positiveRatingRatio<<" \n";
						if(positiveRatingRatio <= 0.25){
							countOfUsers++;
							//std::cout<<"Reporter_0_0.2: "<< " , " << users[i].getUserId()<<" , "<<countOfUsers <<" , "<<users[i].getDSScore()<<" ,  "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";

						if(isDebugMode == 6){
							if(ourScore0_20.is_open()){
									ourScore0_20 <<"Reporter0_0.2: , "<<users[i].getUserId()<<" , "<<countOfUsers<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
								}
								}
							}
							else if(0.25 < positiveRatingRatio && positiveRatingRatio <= 0.5){
									countOfUsers++;

									if(isDebugMode == 6){
										if(ourScore20_40.is_open()){
											ourScore20_40 <<"Reporter0.2_0.4: , "<<users[i].getUserId()<<" , "<<countOfUsers<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
										}
									}
								}
							else if(0.5 < positiveRatingRatio && positiveRatingRatio <= 0.75){
									countOfUsers++;

									if(isDebugMode == 6){
										if(ourScore40_60.is_open()){
											ourScore40_60 <<"Reporter0.4_0.6: , "<<users[i].getUserId()<<" , "<<countOfUsers<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
										}
									}
								}
							else if(0.75 < positiveRatingRatio && positiveRatingRatio <= 1){
									countOfUsers++;

									if(isDebugMode == 6){
										if(ourScore60_80.is_open()){
											ourScore60_80 <<"Reporter0.6_0.8: , "<<users[i].getUserId()<<" , "<<countOfUsers<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
										}
									}
								}
//							else if(positiveRatingRatio > 0.8){
//									countOfUsers++;
//
//									if(isDebugMode == 6){
//										if(ourScore80_100.is_open()){
//											ourScore80_100 <<"Reporter0.8_1: , "<<users[i].getUserId()<<" , "<<countOfUsers<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
//									}
//								}
//							}

					}
			}
			ourScore0_20.close();
			ourScore20_40.close();
			ourScore40_60.close();
			ourScore60_80.close();
			ourScore80_100.close();
}

void sortScoreForHonestReporters(Users users[]){
	Users *honestUsers = new Users[noOfUsers];
	int noOfHonestUsers =0;
	for(int i = 0; i < noOfUsers; i++){
		std::string userId = users[i].getUserId();
				if( userId[0] == 'H'){
					honestUsers[noOfHonestUsers] = users[i];
					noOfHonestUsers++;
				}
	}
	std::cout <<" Total number of honest users: "<<noOfHonestUsers<<"\n";

	for(int i = 0; i < noOfHonestUsers; i++){
			for(int j = 0; j < i; j++){
				if(honestUsers[i].getNoOfEventsReported() < honestUsers[j].getNoOfEventsReported()){
						Users temp = honestUsers[i];
						honestUsers[i] = honestUsers[j];
						honestUsers[j] = honestUsers[i];
				}
			}
		}

	std::cout <<"Based on Participation \n";
	for(int i =0; i < noOfHonestUsers ; i++){
		std::cout <<i <<" Honest Reporter: "<<honestUsers[i].getUserId()<<" Score: "<< honestUsers[i].getScore()<<" "<<honestUsers[i].getNoOfEventsReported()<<"\n";
//		 std::vector<Event> reportedEvents = honestUsers[i].getReportedEvents();
//		 std::cout <<"Reported Events: \n";
//		for(int j = 0 ; j < reportedEvents.size(); j++){
//			 for(int k = 0 ; k < allEvents.size(); k++){
//
//			    if(reportedEvents[j].getEventId() == allEvents[k].getEventId() ){
//			    	std::cout <<"("<< allEvents[k].getEventId()<<" - "<<allEvents[k].getQOIWeight()<<"), ";
//			    }
//			    else
//			    	continue;
//		}
//		}
//		std::cout<<"\n";
	}

}

void sortScoreAsReportersBasedOnParticipationAndTruthFulness(Users users[]){
	double positiveRatingRatio = 0;

	std::ostringstream usersSize, percMal;
	usersSize << noOfUsers;
	percMal << percOfMalReporters;
	std::ofstream ourScore("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score.csv");
	std::ofstream ourMal("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score_malicious.csv");
	std::ofstream ourSel("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score_selfish.csv");
	std::ofstream ourHonest("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/Our_Score_honest.csv");

	int countReporters = 0;

	//Malicious Guy
		for(int i = 0; i < noOfUsers; i++){

			if(users[i].getNoOfEventsReported() <= percOfEventsThres * allEvents.size()){
				//std::cout << "here I am "<<users[i].getUserId()<<" "<<users[i].getNoOfEventsReported()<<"\n";
				continue;
			}

			else{

//				positiveRatingRatio = (double) users[i].getNoOfTrueEventsReported()/users[i].getNoOfEventsReported();
//				if(positiveRatingRatio <= (1 - positiveRatingRatioThres)){

				std::string userId = users[i].getUserId();
				if(userId[0] =='M'){
					countReporters++;

					if(isDebugMode == 4)
						std::cout <<"Malicious Reporter: "<< users[i].getUserId()<<" Score: "<<users[i].getScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";
					else if(isDebugMode == 5)
						std::cout <<"MaliciousReporter: "<< countReporters<<" "<<users[i].getUserId()<<" "<<users[i].getScore()<<"  "<<users[i].getNoOfEventsReported()<< "  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

					else if(isDebugMode == 6){
						if(ourMal.is_open()){
							ourMal <<"MaliciousReporter: , "<<users[i].getUserId()<<" , "<<countReporters<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";

						}
						if(ourScore.is_open()){
							ourScore <<"MaliciousReporter: , "<<users[i].getUserId()<<" , "<<countReporters<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";

						}
					}
				}
			}

		}
		ourMal.close();

		//Not participated guys
//		for(int i = 0; i < noOfUsers; i++){
//				if(users[i].getNoOfEventsReported() == 0 && !users[i].checkIfUserIsReporter()){
//					countReporters++;
//					if(isDebugMode == 4)
//						std::cout <<"NotParticipated Reporter: "<< users[i].getUserId()<<" Score: "<<users[i].getScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";
//					else if(isDebugMode == 5)
//						std::cout <<"NotParticipatedReporter: "<< countReporters<<" "<<users[i].getUserId()<<" "<<users[i].getScore()<<"  "<<users[i].getNoOfEventsReported()<< "  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";
//
//					else if(isDebugMode == 6){
//						if(ourMal.is_open()){
//							ourMal <<"NotParticipatedReporter: , "<<users[i].getUserId()<<" , "<<countReporters<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
//
//						}
//						if(ourScore.is_open()){
//							ourScore <<"NotParticipatedReporter: , "<<users[i].getUserId()<<" , "<<countReporters<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
//
//						}
//					}
//				}
//			}
	//Selfish Guy
	for(int i = 0; i < noOfUsers; i++){

			if(users[i].getNoOfEventsReported() <= percOfEventsThres * allEvents.size())
				continue;

			else{

//				positiveRatingRatio = (double) users[i].getNoOfTrueEventsReported()/users[i].getNoOfEventsReported();
//				if((positiveRatingRatio < positiveRatingRatioThres && positiveRatingRatio > ( 1- positiveRatingRatioThres))){

				std::string userId = users[i].getUserId();
				if(userId[0] =='S'){
					countReporters++;
					if(isDebugMode == 4)
						std::cout <<"Selfish Reporter: "<< users[i].getUserId()<<" Score: "<<users[i].getScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";
					else if(isDebugMode == 5)
						std::cout <<"SelfishReporter: "<< users[i].getUserId()<<" "<<users[i].getScore()<<"  "<<users[i].getNoOfEventsReported()<< "  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

					else if(isDebugMode == 6){
						if(ourSel.is_open()){
							ourSel << "SelfishReporter: , "<<users[i].getUserId()<<" , "<<countReporters<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " ,  "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
						}
						if(ourScore.is_open()){
							ourScore <<"SelfishReporter: , "<<users[i].getUserId()<<" , "<<countReporters<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";

						}
					}
				}
			}

		}
	ourSel.close();

	//Honest Guy
	for(int i = 0; i < noOfUsers; i++){


		if(users[i].getNoOfEventsReported() <= percOfEventsThres * allEvents.size())
			continue;
		else{

//			positiveRatingRatio = (double) users[i].getNoOfTrueEventsReported()/users[i].getNoOfEventsReported();
//			if(positiveRatingRatio >= positiveRatingRatioThres){

			std::string userId = users[i].getUserId();
			if(userId[0] =='H'){
				countReporters++;
				if(isDebugMode == 4)
					std::cout <<"Honest Reporter: "+ users[i].getUserId()<<" Score: "<<users[i].getScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";
				else if(isDebugMode == 5)
					std::cout <<"HonestReporter: "+ users[i].getUserId()<<" "<<users[i].getScore()<<"  "<<users[i].getNoOfEventsReported()<< "  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

				else if(isDebugMode == 6){
					if(ourHonest.is_open()){
						ourHonest <<"HonestReporter: , "<<users[i].getUserId()<<" , "<<countReporters<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";

					}
					if(ourScore.is_open()){
						ourScore <<"HonestReporter: , "<<users[i].getUserId()<<" , "<<countReporters<<" , "<<users[i].getScore()<<" , "<<users[i].getNoOfEventsReported()<< " , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";

					}
				}
			}
		}

	}
	ourHonest.close();
	ourScore.close();
}

void sortRawScoreAsTypeOfReporter(Users users[]){
	std::ostringstream usersSize, percMal ;
	usersSize << noOfUsers;
	percMal << percOfMalReporters;
	 std::ofstream rawScore("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/RawScore.csv");
	 std::ofstream rawScoreMal("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/RawScore_Malicious.csv");
	 std::ofstream rawScoreSelfish("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/RawScore_Selfish.csv");
	 std::ofstream rawScoreHonest("/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/RawScore_Honest.csv");

	 std::string name = "/Users/vijay/Desktop/Trust_Journal/"+ usersSize.str()+"_"+percMal.str()+"/RawScore.csv";
	 std::cout << name<<"\n";

	 int countReporters = 0;
	//std::sort(users, users+noOfUsers);
	//All Malicious Reporters
	for(int i = 0; i < noOfUsers; i++){
		std::string userId = users[i].getUserId();
		if( userId[0] == 'M'){
			countReporters++;
			if(isDebugMode == 4)
				std::cout <<"ScoreForMaliciousReporter: " << users[i].getUserId()<<" Score: "<<users[i].getRawScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else if(isDebugMode == 5)
				 std::cout <<"ScoreForMaliciousReporter:  "<< users[i].getUserId()<<" "<< users[i].getRawScore()<<" " << users[i].getNoOfEventsReported()<<"  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else if(isDebugMode == 6){
				 rawScoreMal <<"ScoreForMaliciousReporter: , "<< countReporters<<" , "<<users[i].getUserId()<<" , "<< users[i].getRawScore()<<" , " << users[i].getNoOfEventsReported()<<" , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
				 rawScore <<"ScoreForMaliciousReporter: , "<< countReporters<<", " <<users[i].getUserId()<<" , "<< users[i].getRawScore()<<" , " << users[i].getNoOfEventsReported()<<" , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
			}
		}
	}
	rawScoreMal.close();
	std::cout<<"\n\n";

	for(int i = 0; i < noOfUsers; i++){
		std::string userId = users[i].getUserId();
		if(userId[0] == 'S'){
			countReporters++;
			if(isDebugMode == 4)
				std::cout <<"ScoreForSelfishReporter: " << users[i].getUserId()<<" Score: "<<users[i].getRawScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else if(isDebugMode == 5)
				 std::cout <<"ScoreForSelfishReporter: "<<  users[i].getUserId()<<" "<< users[i].getRawScore()<<" " << users[i].getNoOfEventsReported()<<"  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else if(isDebugMode == 6){
				rawScoreSelfish <<"ScoreForSelfishReporter: , "<< countReporters<<" , "<< users[i].getUserId()<<" , "<< users[i].getRawScore()<<" , " << users[i].getNoOfEventsReported()<<" , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
				rawScore <<"ScoreForSelfishReporter: , "<< countReporters<<" , "<<users[i].getUserId()<<" , "<< users[i].getRawScore()<<" , " << users[i].getNoOfEventsReported()<<" , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
			}
		}
	}
	rawScoreSelfish.close();
	std::cout<<"\n\n";

	for(int i = 0; i < noOfUsers; i++){
		std::string userId = users[i].getUserId();
		if(userId[0] == 'H'){
			countReporters++;
			if(isDebugMode == 4)
				std::cout <<"ScoreForHonestReporter: "+ users[i].getUserId()<<" Score: "<<users[i].getRawScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else if(isDebugMode == 5)
				 std::cout <<"ScoreForHonestReporter: "<< users[i].getUserId()<<" "<< users[i].getRawScore()<<" " << users[i].getNoOfEventsReported()<<"  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else if(isDebugMode == 6){
				rawScoreHonest <<"ScoreForHonestReporter: , "<< countReporters<<" , "<< users[i].getUserId()<<" , "<< users[i].getRawScore()<<" , " << users[i].getNoOfEventsReported()<<" , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
				rawScore <<"ScoreForHonestReporter: , "<< countReporters<<" , "<< users[i].getUserId()<<" , "<< users[i].getRawScore()<<" , " << users[i].getNoOfEventsReported()<<" , "<<users[i].getNoOfTrueEventsReported()<<" , "<<users[i].getNoOfFalseEventsReported()<<"\n";
			}
		}
	}
	rawScoreHonest.close();
	rawScore.close();
	std::cout<<"\n\n";
}

void sortScoreAsTypeOfReporter(Users users[]){
	//std::sort(users, users+noOfUsers);
	//All Malicious Reporters
	for(int i = 0; i < noOfUsers; i++){
		std::string userId = users[i].getUserId();
		if( userId[0] == 'M'){
			if(isDebugMode == 4)
				std::cout <<"ScoreForMaliciousReporter: "+ users[i].getUserId()<<" Score: "<<users[i].getScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else
				 std::cout <<"ScoreForMaliciousReporter:  "<< users[i].getUserId()<<" "<< users[i].getScore()<<" " << users[i].getNoOfEventsReported()<<"  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";

		}
	}
	std::cout<<"\n\n";

	for(int i = 0; i < noOfUsers; i++){
		std::string userId = users[i].getUserId();
		if(userId[0] == 'S'){
			if(isDebugMode == 4)
				std::cout <<"ScoreForSelfishReporter: "+ users[i].getUserId()<<" Score: "<<users[i].getScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else
				 std::cout <<"ScoreForSelfishReporter: "<< users[i].getUserId()<<" "<< users[i].getScore()<<" " << users[i].getNoOfEventsReported()<<"  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";
		}
	}
	std::cout<<"\n\n";

	for(int i = 0; i < noOfUsers; i++){
		std::string userId = users[i].getUserId();
		if(userId[0] == 'H'){
			if(isDebugMode == 4)
				std::cout <<"ScoreForHonestReporter: "+ users[i].getUserId()<<" Score: "<<users[i].getScore()<<" Total Events: "<<users[i].getNoOfEventsReported()<< "  trueEvents: "<<users[i].getNoOfTrueEventsReported()<<"   falseEvents: "<<users[i].getNoOfFalseEventsReported()<<"\n";

			else
				 std::cout <<"ScoreForHonestReporter: "<< users[i].getUserId()<<" "<< users[i].getScore()<<" " << users[i].getNoOfEventsReported()<<"  "<<users[i].getNoOfTrueEventsReported()<<" "<<users[i].getNoOfFalseEventsReported()<<"\n";
		}
	}
	std::cout<<"\n\n";
}


/*
* Here, we place equal number of malicious reporters randomly in all four squares.
* It means, 1/4 of total malicious reporters exist within the boundary of each square.

*/
void placeAllReportersInAllSquares(Users users[], Square areaSquare[], int noOfReporters, int noOfMalReporters, int noOfSelfishReporters){
	if(isDebugMode == 4)
		std::cout <<"Total Reporters: " <<noOfReporters<< "   Mal Reporters: "<<noOfMalReporters<<" Selfish Reporters: " << noOfSelfishReporters<<"\n";
	int noOfReportersInAQuadrant = round(noOfReporters/4);
	if(isDebugMode == 4)
		std::cout << "No of Reporters In A Quadrant: " << noOfReportersInAQuadrant <<"\n";

	int countOfReporters = 0, countOfMalReporters = 0, currIndex = 0, countOfSelfishReporters = 0;
    //For each square quadrant
    for(int index = 0; index <4; index++){
        countOfMalReporters = 0;
        countOfSelfishReporters = 0;

        //Also, make sure than summation of noOfReporters/4 should be equal to noOfReporters
        if(index == 3){
        	noOfReportersInAQuadrant = noOfReporters - 3 * noOfReportersInAQuadrant;
        	if(isDebugMode == 4)
        		std::cout << "Quadrant "<< index<< " No of Reporters In Last Quadrant: " << noOfReportersInAQuadrant <<"\n";
        }

         for(int i = 0; i < noOfReportersInAQuadrant ; i++){
            currIndex = countOfReporters;
            std::ostringstream oss;
           // std::cout <<"No of Mal reporters "<<noOfMalReporters<<"\n";
            int noOfMalReportersInAQuadrant = round(noOfMalReporters/4);
            int noOfSelfishReportersInAQuadrant = round(noOfSelfishReporters/4);

            if(isDebugMode == 4){
            	 std::cout << "Quadrant "<< index<< " No of Mal Reporters In A Quadrant: " << noOfMalReportersInAQuadrant <<"\n";
            	 std::cout << "Quadrant "<< index<< " No of Selfish Reporters In A Quadrant: " << noOfSelfishReportersInAQuadrant <<"\n";
            }

           //Place 1/4 of total malicious reporters to each square quadrant
            if(countOfMalReporters < noOfMalReportersInAQuadrant ){
                oss << currIndex;
               std::string u_id;
               if (isDebugMode == 5){
            	   u_id = oss.str();
               }
               else{
					if(index == 0){
						u_id = "M_LL_"+ oss.str();
					}
					else if(index == 1){
						u_id = "M_LR_"+ oss.str();
					}
					else if(index == 2){
						u_id = "M_UL_"+ oss.str();
					}
					else{
						u_id = "M_UR_"+ oss.str();
					}
               }


                users[currIndex].setUserId(u_id);
                users[currIndex].setUserAsMalReporter(true);
                users[currIndex].setUserAsSelfishReporter(false);
                users[currIndex].setUserAsReporter(false);
                countOfMalReporters++;

            }
            //Place 1/4 of total malicious reporters to each square quadrant
            else if(countOfSelfishReporters < noOfSelfishReportersInAQuadrant ){
                    oss << currIndex;
                   std::string u_id;
                   if (isDebugMode == 5){
                	   u_id = oss.str();
                   }
                   else{
    					if(index == 0){
    						u_id = "S_LL_"+ oss.str();
    					}
    					else if(index == 1){
    						u_id = "S_LR_"+ oss.str();
    					}
    					else if(index == 2){
    						u_id = "S_UL_"+ oss.str();
    					}
    					else{
    						u_id = "S_UR_"+ oss.str();
    					}
                   }


                    users[currIndex].setUserId(u_id);
                    users[currIndex].setUserAsSelfishReporter(true);
                    users[currIndex].setUserAsMalReporter(false);
                    users[currIndex].setUserAsReporter(false);
                    countOfSelfishReporters++;

                }
            //Honest reporters
            else{
               // int idNumber = n;
                oss << currIndex;
               std::string u_id;

               if(isDebugMode == 5){
            	   u_id = oss.str();

               }
               else{
            	   if(index == 0){
					   u_id = "H_LL_"+ oss.str();
				   }
				   else if(index == 1){
					   u_id = "H_LR_"+oss.str();
				   }
				   else if(index == 2){
					   u_id = "H_UL_"+oss.str();
				   }
				   else{
					   u_id = "H_UR_"+oss.str();
				   }
               }

                //string u_id = "H_LL_"+ oss.str();
                users[currIndex].setUserId(u_id);
                users[currIndex].setUserAsMalReporter(false);
                users[currIndex].setUserAsSelfishReporter(false);
                users[currIndex].setUserAsReporter(true);
            }

            Point location;


            double xPos = random((int)areaSquare[index].getLl().getX(),(int) areaSquare[index].getUr().getX()) ;
            double yPos =random((int)areaSquare[index].getLl().getY(), (int)areaSquare[index].getUr().getY()) ;
            location.setX(xPos);
            location.setY(yPos);
            //std::cout<<" The random numbers: "<<xPos<<" "<<yPos<<std::std::endl;
            users[currIndex].setUserLocation(location);
            //std::cout <<"User "<< currIndex << users[currIndex].getUserId()<<"\n";
            countOfReporters++;
        }
    }
}

/*
* Place 1/4 of total raters in each square quadrant
* User Id for reporters starts from "R"
*/
void placeAllRatersInAllSquares(Users user[], int noOfRaters, int noOfReporters, Square areaSquare[]){

	int noOfRatersInAQuadrant = round(noOfRaters/4);

	int countOfRaters = 0;
	int currIndex = 0;
    //For each square quadrant
    for(int index = 0; index < 4; index++){
        // int countOfRaters = 0;
    	if(index == 3){
    		noOfRatersInAQuadrant = noOfRaters - 3 * noOfRatersInAQuadrant;
    	}

        //NOTE: Instead of 1, it should be noOfRaters/4
         for(int i = 0; i < noOfRatersInAQuadrant ; i++){
            currIndex = noOfReporters + countOfRaters;
            std::ostringstream oss;
            //int idNumber = noOfReporters + i + 1;
            oss << currIndex;
           std::string u_id;

                if(index == 0){
                    u_id = "R_LL_"+ oss.str();
                }
                else if(index == 1){
                    u_id = "R_LR_"+oss.str();
                }
                else if(index == 2){
                    u_id = "R_UL_"+oss.str();
                }
                else{
                    u_id = "R_UR_"+oss.str();
                }
            user[currIndex].setUserId(u_id);
            user[currIndex].setUserAsRater(true);

            Point location;
            double xPos = random((int)areaSquare[index].getLl().getX(),(int) areaSquare[index].getUr().getX()) ;
            double yPos = random((int)areaSquare[index].getLl().getY(), (int)areaSquare[index].getUr().getY()) ;
            location.setX(xPos);
            location.setY(yPos);
           // std::cout<<" The random numbers: "<<xPos<<" "<<yPos<<std::endl;
            user[currIndex].setUserLocation(location);
            countOfRaters++;
        }
    }
}

/*
Return all the users i.e. Malicious Reporters, Honest Reporters and Raters in the range of
given event location
*/
std::vector<Users> getAllUsersInRange(Users users[], Point eventLocation){
    std::vector<Users> allUsers;
    //std::cout <<"Current event location is " << eventLocation.getX()<<"   "<<eventLocation.getY()<<"\n";
    if(isDebugMode == 2 || isDebugMode == 3 ){
    		std::cout <<"User ID: "<< " isMalReporter "<< " isReporter " << " isRater "<<"\n";
    }
    for(int i =0; i < noOfUsers; i++){
            double startX = eventLocation.getX() - eventWidth;
            double endX = eventLocation.getX() + eventWidth;
            double startY = eventLocation.getY() - eventWidth;
            double endY = eventLocation.getY() + eventWidth;
            double userX = users[i].getUserLocation().getX();
            double userY = users[i].getUserLocation().getY();

        //std::cout << startX<<"  "<< userX<<"  "<<endX <<"\n";
        //std::cout << startY<<"  "<< userY<<"  "<<endY<<"\n";
        //Check if the user is located in the square boundary of the event location of certain boudary width
        if(( startX< userX && userX < endX && startY < userY && userY < endY)){

                allUsers.push_back(users[i]);
                if(isDebugMode == 2 || isDebugMode == 3 ){
                	std::cout <<users[i].getUserId()<<"         "<<users[i].checkIfUserIsMalReporter() << "              "<<users[i].checkIfUserIsReporter() <<"          "<< users[i].checkIfUserIsRater()<<"\n";
                }
        }
    }
    if(isDebugMode == 2 || isDebugMode == 3 ){
    	std::cout <<"In Range Users:  "<< allUsers.size()<<"\n";
    }
return allUsers;

}


//For debugging
void showAllUsers(Users users[]){
	std::cout<<"\nAll Reporters \n";
    for(int i =0 ; i < noOfUsers; i++ ){
        std::cout <<"User " << i+1 << "    "<<users[i].getUserId()<<"\n";
    }
}

//For debugging
void showEventSquare(Square eventSquare){
    std::cout << eventSquare.getLl().getX()<<"  "<<eventSquare.getLr().getX()<<"\n";
}

//get coordinates of all users and events 
void getUserEventLocation(Square areaSquare[], Users allUsers[], int initialEndFlag){
  std::ofstream userLocationH("/Users/vijay/Desktop/Trust_Journal/userLocation_honest_"+std::to_string(initialEndFlag)+".csv");
  std::ofstream userLocationM("/Users/vijay/Desktop/Trust_Journal/userLocation_malicious_"+std::to_string(initialEndFlag)+".csv");
  std::ofstream userLocationS("/Users/vijay/Desktop/Trust_Journal/userLocation_selfish_"+std::to_string(initialEndFlag)+".csv");
  std::ofstream eventLocation("/Users/vijay/Desktop/Trust_Journal/eventLocation_"+std::to_string(initialEndFlag)+".csv");
   
  //get the boundary
  //for(int i = 0; i < 4; i++){
    // userLocation << "square_"<<i<<" ,"<< areaSquare[i].getLl().getX()<<" , "<<areaSquare[i].getLl().getY()<<"\n";
    // userLocation << "square_"<<i<<" ,"<< areaSquare[i].getUl().getX()<<" , "<<areaSquare[i].getUl().getY()<<"\n";
    // userLocation << "square_"<<i<<" ,"<< areaSquare[i].getLr().getX()<<" , "<<areaSquare[i].getLr().getY()<<"\n";
    // userLocation << "square_"<<i<<" ,"<< areaSquare[i].getUr().getX()<<" , "<<areaSquare[i].getUr().getY()<<"\n";

 
 	  // get location of all Events
  for(int i = 0; i < allEvents.size(); i++){
    eventLocation << allEvents[i].getEventId()<<" ," << allEvents[i].getEventLocation().getX()<<" , "<< allEvents[i].getEventLocation().getY()<<"\n";
  }

  eventLocation.close();
  // get location of all Users
  for(int i = 0 ; i < noOfUsers; i++){

  	if(allUsers[i].getUserId().find("S_")!=std::string::npos) 
      userLocationS << allUsers[i].getUserId()<<" , " << allUsers[i].getUserLocation().getX()<< " ,  " << allUsers[i].getUserLocation().getY()<<"\n"; 
 
    else if(allUsers[i].getUserId().find("H_")!=std::string::npos)
      userLocationH << allUsers[i].getUserId()<<" , " << allUsers[i].getUserLocation().getX()<< " ,  " << allUsers[i].getUserLocation().getY()<<"\n"; 
    
    else if(allUsers[i].getUserId().find("M_")!=std::string::npos)
      userLocationM << allUsers[i].getUserId()<<" , " << allUsers[i].getUserLocation().getX()<< " ,  " << allUsers[i].getUserLocation().getY()<<"\n"; 
    
    
  }

  userLocationH.close();
  userLocationM.close();
  userLocationS.close();

}

// Does this handle all the edge cases? Who knows...
// Better make sure you test it thoroughly if you write it yourself!
bool String2Int(const std::string& str, int& result)
{
   std::string::const_iterator i = str.begin();
 
    if (i == str.end())
        return false;
 
    bool negative = false;
 
    if (*i == '-')
    {
        negative = true;
        ++i;
 
        if (i == str.end())
            return false;
    }
 
    result = 0;
 
    for (; i != str.end(); ++i)
    {
        //std::cout <<"Value : "<<*i <<"\n";
        if(*i == ' ')
            continue;
        if (*i < '0' || *i > '9')
            return false;
 
        result *= 10;
        result += *i - '0';
    }
 
    if (negative)
    {
        result = -result;
    }
 
    return true;
}

void printSquare(Square areaSquare[], int index){
    std::cout << "LL "<<areaSquare[index].getLl().getX()<<"," <<areaSquare[index].getLl().getY()<<"\n";
    std::cout << "UL "<< areaSquare[index].getUl().getX()<<"," <<areaSquare[index].getUl().getY()<<"\n";
    std::cout << "LR "<<areaSquare[index].getLr().getX()<<"," <<areaSquare[index].getLr().getY()<<"\n";
    std::cout << "UR "<<areaSquare[index].getUr().getX()<<"," <<areaSquare[index].getUr().getY()<<"\n\n";

}


