#include "Users.h"
#include <vector>
#include <string>
using namespace std;

Users::Users()
{
    isReporter = false;
    isRater = false;
    isMalReporter = false;
    isSelfishReporter = false;
    rawScore = 0;
    score = 0;
}

void Users::setUserId(string _userId){
    userId = _userId;
}

string Users::getUserId(){
    return userId;
}

void Users:: setUserAsReporter(bool _isReporter){
    isReporter = _isReporter;
 }

void Users::setUserAsRater(bool _isRater){
    isRater = _isRater;
}

void Users:: setUserAsMalReporter(bool _isMalReporter){
    isMalReporter = _isMalReporter;
 }

void Users:: setUserAsSelfishReporter(bool _isSelfishReporter){
    isSelfishReporter = _isSelfishReporter;
 }

bool Users::checkIfUserIsReporter(){
    return isReporter;
}

bool Users::checkIfUserIsRater(){
    return isRater;
}

bool Users::checkIfUserIsMalReporter(){
    return isMalReporter;
}

bool Users::checkIfUserIsSelfishReporter(){
    return isSelfishReporter;
}

void Users::setUserLocation(Point _location){
    location = _location;
}

Point Users::getUserLocation(){
    return location;
}

vector<Event> Users::getReportedEvents(){
    return reportedEvents;
}

void Users:: addReportedEvent(Event event){
    bool isFound = false;
    for(int i = 0; i < reportedEvents.size(); i++){
        if(reportedEvents[i].getEventId() == event.getEventId()){
            isFound = true;
            break;
        }
    }
    if (isFound == false)
        reportedEvents.push_back(event);
}

vector<Event> Users:: getRatedEvents(){
    return ratedEvents;
}

void Users:: addRatedEvent(Event event){
    ratedEvents.push_back(event);
}

void Users:: addMultipleReportedEvents(vector<Event> _reportedEvents){
    for(int i = 0; i < _reportedEvents.size(); i++){
        addReportedEvent(_reportedEvents[i]);
    }
}

void Users:: addMultipleRatedEvents(vector<Event> _ratedEvents){
    for(int i = 0; i < _ratedEvents.size(); i++){
        addRatedEvent(_ratedEvents[i]);
    }
}

void Users:: setMostRecentEvent(Event event){
    recentEvent = event;
}

Event Users :: getMostRecentEvent(){
    return recentEvent;
}

double Users :: getScore(){
    return score;
}

void Users :: setScore(double _score){
    score = _score;
}

double Users :: getRawScore(){
    return rawScore;
}

void Users :: setRawScore(double _rawScore){
    rawScore = _rawScore;
}


void Users :: setNoOfTrueEventsReported(int _noOfTrueEventsReported){
	noOfTrueEventsReported = _noOfTrueEventsReported;
}

void Users :: setNoOfFalseEventsReported(int _noOfFalseEventsReported){
	noOfFalseEventsReported = _noOfFalseEventsReported;
}

int Users :: getNoOfTrueEventsReported (){
	return noOfTrueEventsReported;
}

int Users :: getNoOfFalseEventsReported (){
	return noOfFalseEventsReported;
}

int Users:: getNoOfEventsReported(){
	return getNoOfTrueEventsReported() + getNoOfFalseEventsReported();
}

void Users:: updateReportedEvents(std::vector <Event> _reportedEvents){

	reportedEvents = _reportedEvents;
}

void Users:: setDSScore(double _dsScore){
	dsScore = _dsScore;
}

double Users :: getDSScore(){
	return dsScore;
}

double Users :: getIncentives(){
    return incentives;
}

void Users :: setIncentives(double _incentives){
    incentives = _incentives;
}
