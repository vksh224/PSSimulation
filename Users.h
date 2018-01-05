#ifndef USERS_H
#define USERS_H

#include "Point.h"
#include "Event.h"
#include <string>
#include <vector>

class Users
{
    public:
        Users();

        void setUserId(std::string _userId);
        std::string getUserId();
        void setUserAsReporter(bool _isReporter);
        void setUserAsRater(bool _isRater);
        void setUserAsMalReporter(bool _isMalReporter);
        void setUserAsSelfishReporter(bool _isSelfishReporter);

        bool checkIfUserIsReporter();
        bool checkIfUserIsRater();
        bool checkIfUserIsMalReporter();
        bool checkIfUserIsSelfishReporter();

        void setUserLocation(Point _location);
        Point getUserLocation();
        std::vector<Event> getReportedEvents();
        void addReportedEvent(Event event);
        void updateReportedEvents(std::vector<Event>_reportedEvents);

        std::vector<Event> getRatedEvents();
        void addRatedEvent(Event event);

        void addMultipleReportedEvents(std::vector<Event> _reportedEvents);
        void addMultipleRatedEvents(std::vector<Event> _ratedEvents);

        void setMostRecentEvent(Event event);
        Event getMostRecentEvent();

        double getScore();
        double getRawScore();
        void setScore(double score);
        void setRawScore(double score);

        double getDSScore();
        void setDSScore(double score);

        double getIncentives();
        void setIncentives( double incentives);

        void setNoOfTrueEventsReported(int noOfTrueEventsReported);
        void setNoOfFalseEventsReported(int noOfFalseEventsReported);
        int getNoOfTrueEventsReported ();
        int getNoOfFalseEventsReported ();
        int getNoOfEventsReported();

    protected:
    private:
        std::string userId;
        bool isReporter;
        bool isRater;
        bool isMalReporter;
        bool isSelfishReporter;
        Point location;
        std::vector<Event> reportedEvents;
        std::vector<Event> ratedEvents;
        Event recentEvent;
        double score;
        double dsScore;
        double rawScore;
        int noOfTrueEventsReported;
        int noOfFalseEventsReported;
        double incentives;
};

#endif // USERS_H
