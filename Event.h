#ifndef EVENT_H
#define EVENT_H

#include <string>
#include "Point.h"
#include <vector>
#include <math.h>

class Event
{
    public:
        Event();
        void setEventId(std::string _eventId);
        std::string getEventId();
        void setFalseEvent(bool _isEventFalse);
        bool checkIfEventFalse();
        void setEventLocation(Point _location);
        Point getEventLocation();
        void setEventWidth(int _eventWidth);

        int getNoOfPositiveRatings();
        void setNoOfPositiveRatings(int _noOfPositiveRatings);
        int getNoOfNegativeRatings();
        void setNoOfNegativeRatings(int _noOfNegativeRatings);
        int getNoOfUncertainRatings();
        void setNoOfUncertainRatings(int _noOfUncertainRatings);
        int getNoOfRatings();

        double getPercOfPositiveRatings();
        double getPercOfNegativeRatings();
        double getPercOfUncertainRatings();

        double getWeightForUncertainty();
        double getWeightForBelief();

        void setEventTypeId(int _eventTypeId);
        int getEventTypeId();
        void setEpochNum(int _epochNum);
        int getEpochNum();

        int getEventStartTime();
        int getEventEndTime();

        double calculateBelief();
        double calculateDisBelief();
        double calculateUncertainty();
        void calculateOldWeightForUncertainty(double uncertainWeightMax, double uncertainA, double uncertainB);
        void calculateWeightForUncertainty(double uncertainWeightMax, double uncertainA, double uncertainB, int thresholdN, double phi);
        void calculateWeightForBelief(double beliefWeightMax, double beliefA, double beliefB);
        void setQOIWeight(double qoi, double alpha, double beta, double lambda, double referencePoint);
        void  calculateQOI(int noOfUsers, double alpha, double beta, double lambda, double referencePoint);
        double getQOI();
        double getJosangQOI();
        double getQOIWeight();

        void setAggregateReports(int aggReports);
        int getAggregateReports();

        void setAggregateReputation(double aggReputation);
        double getAggregateReputation();

        void setHistoricProb(double historicProb);
        void setHistoricProspectProb(double historicProspectProb);
        void setEventConfidence(double eventConfidence);
        void setEventUtilityConfidence(double eventUtilityConfidence);

        double getHistoricProb();
        double getHistoricProspectProb();
        double getEventConfidence();
        double getEventUtilityConfidence();

        void setEventCPTPublish(std::string eventPublish);
        std::string getEventCPTPublish();

        void setEventEUTPublish(std::string eventPublish);
        std::string getEventEUTPublish();
        
    protected:
    private:
        std::string eventId;
        bool isEventFalse;
        Point location;
        int eventWidth;
        int eventTypeId;
        int epochNum;
        int noOfPositiveRatings;
        int noOfNegativeRatings;
        int noOfUncertainRatings;
        double belief;
        double disBelief;
        double uncertainty;
        double qoi;
        double josangQOI;
        double uncertainWeight;
        double beliefWeight;
        double qoiWeight;
        int aggregateReports;
        double aggregateReputation;

        double historicProb;
        double historicProspectProb;
        double eventConfidence;
        double eventUtilityConfidence;

        std::string eventCPTPublished;
        std::string eventEUTPublished;

};

#endif // EVENT_H
