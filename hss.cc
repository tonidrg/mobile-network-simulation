/*
 * hss.cc
 *
 *  Created on: Jun 19, 2019
 *      Author: Bart
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

bool authenticationAutorization = false;
bool confirmedUE = false;

class HSS : public cSimpleModule {
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(HSS);

void HSS::initialize() {}

void HSS::handleMessage(cMessage *msg) {
    cModule *sender = msg->getSenderModule();
    std::string senderName = sender->getName();

    if(senderName == "mme" && !authenticationAutorization){
        authenticationAutorization = true;

        cMessage *authAutoriz = new cMessage("Authentication and Autorization");
        EV << "HHS vrsi autentifikaciju i autorizaciju UE-a" << "\n";
        send(authAutoriz, gate("out"));
    } else if(senderName == "mme" && authenticationAutorization && !confirmedUE){
        confirmedUE = true;
        EV << "SGW saznaje adresu od PGW" << "\n";

        cMessage *confirmedUEMsg = new cMessage("");
        send(confirmedUEMsg, gate("out"));
    }
}
