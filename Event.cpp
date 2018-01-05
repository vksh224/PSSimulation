#include "Event.h"
#include <string>
#include <vector>
#include<iostream>
#include <cmath>
using namespace std;


Event:: Event()
{
    isEventFalse = false;
    noOfPositiveRatings = 0;
    noOfNegativeRatings = 0;
    noOfUncertainRatings = 0;
    aggregateReports = 0;
    aggregateReputation = 0;
    historicProb = -1;
    historicProspectProb = -1;
    eventConfidence = -1;
    eventUtilityConfidence = -1;

}

void Event :: setEventId(string _eventId){
    eventId = _eventId;
}

string Event:: getEventId(){
    return eventId;
}

void  Event::setFalseEvent(bool _isEventFalse){
    isEventFalse = _isEventFalse;
}

bool Event :: checkIfEventFalse(){
    return isEventFalse;
}

void Event::setEventLocation(Point _location){
    location = _location;
}
Point Event::getEventLocation(){
    return location;
}

int Event ::getEpochNum(){
    return epochNum;
}

void Event ::setEpochNum(int _epochNum){
    epochNum = _epochNum;
}

void Event:: setEventWidth(int _eventWidth){
    eventWidth = _eventWidth;
}

void Event:: setEventTypeId(int _eventTypeId){
    eventTypeId = _eventTypeId;
}

int Event :: getEventTypeId(){
    return eventTypeId;
}

int Event:: getEventStartTime(){
    return epochNum;
}

int Event::getEventEndTime(){
    //Road closure
    if (eventTypeId == 0){
        return epochNum ;
    }

    //Police
    else if(eventTypeId == 1){
        return epochNum+1;
    }

    //Jam
    else if(eventTypeId == 2){
         return epochNum + 2;
    }

    else if(eventTypeId == 3){
        return epochNum + 3;
    }
    else{
        return epochNum;
    }

}

int Event:: getNoOfPositiveRatings(){
    return noOfPositiveRatings;
}

void Event:: setNoOfPositiveRatings(int _noOfPositiveRatings){
    noOfPositiveRatings = _noOfPositiveRatings;
}

int Event :: getNoOfNegativeRatings(){
    return noOfNegativeRatings;
}

void Event :: setNoOfNegativeRatings(int _noOfNegativeRatings){
    noOfNegativeRatings = _noOfNegativeRatings;
}

int Event :: getNoOfUncertainRatings(){
    return noOfUncertainRatings;
}

void Event :: setNoOfUncertainRatings(int _noOfUncertainRatings){
    noOfUncertainRatings = _noOfUncertainRatings;
}

int Event:: getNoOfRatings(){
	return getNoOfPositiveRatings() + getNoOfNegativeRatings() + getNoOfUncertainRatings();
}
double Event :: getWeightForUncertainty(){
	return uncertainWeight;
}

double Event:: getWeightForBelief(){
	return beliefWeight;
}

double Event:: getPercOfPositiveRatings(){
	if(getNoOfRatings() == 0)
		return 0;
	else
		return (double)getNoOfPositiveRatings()/getNoOfRatings();
}

double Event :: getPercOfNegativeRatings(){
	//cout<<" Number of ratings: "<< getNoOfRatings()<<"\n";
	if(getNoOfRatings() == 0)
		return 0;
	else{
		return (double)getNoOfNegativeRatings()/getNoOfRatings();
	}

}

double Event :: getPercOfUncertainRatings(){
	if(getNoOfRatings() == 0)
		return 0;
	else
		return (double)getNoOfUncertainRatings()/getNoOfRatings();
}

double Event :: calculateBelief(){
        belief = (double)(noOfPositiveRatings + 1)/(noOfPositiveRatings + noOfNegativeRatings + noOfUncertainRatings+3);
        return belief;
}

double  Event :: calculateDisBelief(){
        disBelief = (double)(noOfNegativeRatings + 1)/(noOfPositiveRatings + noOfNegativeRatings + noOfUncertainRatings+3);
        return disBelief;
}

double Event :: calculateUncertainty(){
        uncertainty = (double)(noOfUncertainRatings + 1)/(noOfPositiveRatings + noOfNegativeRatings + noOfUncertainRatings+3);
        return uncertainty;
}

void Event:: calculateOldWeightForUncertainty(double uncertainWeightMax, double uncertainA, double uncertainB){
	//by default
	uncertainWeight = 0.5;

	int totalNoOfRatings = getNoOfPositiveRatings() + getNoOfNegativeRatings() + getNoOfUncertainRatings();
	double denominator = 1 + ( uncertainA * exp(- uncertainB * totalNoOfRatings));
	uncertainWeight = uncertainWeightMax/denominator;
	//std::cout <<"The value is: "<<uncertainA << "  "<<uncertainB<<"  "<<totalNoOfRatings<< "   "<<uncertainWeight<<"\n";
	//return uncertainWeight;
}


void Event:: calculateWeightForUncertainty(double uncertainWeightMax, double uncertainA, double uncertainB, int thresholdN, double phi){
	//by default
	uncertainWeight = 0.5;

	int totalNoOfRatings = getNoOfPositiveRatings() + getNoOfNegativeRatings() + getNoOfUncertainRatings();
	//uncertainWeight = exp(-pow(totalNoOfRatings, 0.5));
	if(totalNoOfRatings > thresholdN)
		uncertainWeight = exp(-1 * pow((totalNoOfRatings - thresholdN), phi));
		//uncertainWeight = (uncertainWeightMax * uncertainA* exp(- uncertainB * totalNoOfRatings)) / (1 + uncertainA * exp(- uncertainB * totalNoOfRatings));

	else
       // uncertainWeight = 0.5;
		uncertainWeight = (uncertainWeightMax) / (1 + uncertainA * exp(- uncertainB * totalNoOfRatings));

	//std::cout <<"The value is: "<<uncertainA << "  "<<uncertainB<<"  "<<totalNoOfRatings<< "   "<<uncertainWeight<<"\n";
	//return uncertainWeight;
}


void Event :: calculateWeightForBelief(double beliefWeightMax, double beliefA, double beliefB){
	beliefWeight = 0.5;

	int totalNoOfRatings = getNoOfPositiveRatings() + getNoOfNegativeRatings() + getNoOfUncertainRatings();
	double denominator = 1 + ( beliefA * exp(- beliefB * totalNoOfRatings));
	beliefWeight = beliefWeightMax/denominator;
}

void Event :: setQOIWeight(double qoi, double alpha, double beta, double lambda, double referencePoint){
	//qoiWeight = log(qoi/ (1 - qoi));
    if(qoi >= referencePoint){
        qoiWeight = pow((qoi), alpha);
    }
    else{
        qoiWeight = (- lambda) * pow((referencePoint - qoi), beta);
    }
}

void Event :: calculateQOI(int noOfUsers, double alpha, double beta, double lambda, double referencePoint){
        qoi = beliefWeight * calculateBelief() +  uncertainWeight * calculateUncertainty();
        setQOIWeight(qoi, alpha, beta, lambda, referencePoint);
}

double Event :: getQOI(){
    return qoi;
}

double Event :: getJosangQOI(){
	josangQOI = calculateBelief() + 0.5 * calculateUncertainty();
	return josangQOI;
}

double Event :: getQOIWeight(){
	return qoiWeight;
}

void Event :: setAggregateReports(int aggReports){
    aggregateReports = aggReports;
}

int Event :: getAggregateReports(){
    return aggregateReports;
}

void Event :: setAggregateReputation(double aggReputation){
    aggregateReputation = aggReputation;
}
        
double Event :: getAggregateReputation(){
    return aggregateReputation;
}

void Event :: setHistoricProb(double _historicProb){
    historicProb = _historicProb;
}

void Event :: setHistoricProspectProb(double _historicProspectProb){
    historicProspectProb = _historicProspectProb;
}

void Event :: setEventConfidence(double _eventConfidence){
    eventConfidence = _eventConfidence;
}

void Event :: setEventUtilityConfidence(double _eventUtilityConfidence){
    eventUtilityConfidence = _eventUtilityConfidence;
}

double Event :: getHistoricProb(){
    return historicProb;
}

double Event :: getHistoricProspectProb(){
    return historicProspectProb;
}

double Event :: getEventConfidence(){
    return eventConfidence;
}

double Event :: getEventUtilityConfidence(){
    return eventUtilityConfidence;
}

void Event :: setEventCPTPublish(string _eventPublished){
    eventCPTPublished = _eventPublished;
}

string Event :: getEventCPTPublish(){
    return eventCPTPublished;
}

void Event :: setEventEUTPublish(string _eventPublished){
    eventEUTPublished = _eventPublished;
}

string Event :: getEventEUTPublish(){
    return eventEUTPublished;
}



