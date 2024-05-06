/*
 * pgw.cc
 *
 *  Created on: Jun 19, 2019
 *      Author: Bart
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

bool createSessionResponse = false;
bool notifyMME = false;

bool sendDownlinkData = false;

class PGW : public cSimpleModule {
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(PGW);

void PGW::initialize() {}

void PGW::handleMessage(cMessage *msg) {
    cModule *sender = msg->getSenderModule();
    std::string senderName = sender->getName();

    if(senderName == "sgw" && !createSessionResponse){
        createSessionResponse = true;
        EV << "UE-u se dodijeli IPv4 adresa ili IPv6 prefiks" << "\n";

        cMessage *createSessionResponseMsg = new cMessage("Create Session repsonse");
        EV << "Slanje odgovora za zahtjev stvaranja sesije" << "\n";
        send(createSessionResponseMsg, gate("out", 0));
    } else if(senderName == "sgw" && createSessionResponse && !notifyMME){
        notifyMME = true;

        cMessage *notifyMMEMsg = new cMessage("");
        send(notifyMMEMsg, gate("out", 0));
    } else if(senderName == "sgw" && notifyMME && !sendDownlinkData){
        sendDownlinkData = true;

        cMessage *downlinkDataMsg = new cMessage("Downlink Data");
        EV << "PGW salje downlink podatke UE-u" << "\n";
        send(downlinkDataMsg, gate("out", 0));
    }
}


