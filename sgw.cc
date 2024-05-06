/*
 * sgw.cc
 *
 *  Created on: Jun 19, 2019
 *      Author: Bart
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

bool forwardSessionCreation = false;
bool returnSessionResponse = false;

bool sendUplinkData2 = false;
bool notifyMME1 = false;

bool modifyBearerResponse = false;
bool sendDownlinkData1 = false;

class SGW : public cSimpleModule {
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(SGW);

void SGW::initialize() {}

void SGW::handleMessage(cMessage *msg) {
    cModule *sender = msg->getSenderModule();
    std::string senderName = sender->getName();

    if(senderName == "mme" && !forwardSessionCreation){
        forwardSessionCreation = true;

        cMessage *forwardSessionCreationMsg = new cMessage("Create Session");
        send(forwardSessionCreationMsg, gate("out", 3));
    } else if(senderName == "pgw" && forwardSessionCreation && !returnSessionResponse){
        returnSessionResponse = true;

        cMessage *returnSessionResponseMsg = new cMessage("Create Session response");
        send(returnSessionResponseMsg, gate("out", 1));
    } else if(senderName == "enodeb" && returnSessionResponse && !sendUplinkData2){
        sendUplinkData2 = true;

        cMessage *uplinkData = new cMessage("Uplink Data");
        send(uplinkData, gate("out", 3));
    } else if(senderName == "pgw" && sendUplinkData2 && !notifyMME1){
        notifyMME1 = true;

        cMessage *notifyMMEMsg = new cMessage("");
        send(notifyMMEMsg, gate("out", 1));
    } else if(senderName == "mme" && notifyMME1 && !modifyBearerResponse){
        modifyBearerResponse = true;

        cMessage *bearerResponseMsg = new cMessage("Modify Bearer response");
        EV << "SGW odgovara MME-u na zahtjev modifikacije nosaca" << "\n";
        send(bearerResponseMsg, gate("out", 1));

        cMessage *notifyPGW = new cMessage("");
        EV << "Downlink konekcija (tunel) je postavljena" << "\n";
        send(notifyPGW, gate("out", 3));
    } else if(senderName == "pgw" && modifyBearerResponse && !sendDownlinkData1){
        sendDownlinkData1 = true;

        cMessage *downlinkDataMsg = new cMessage("Downlink Data");
        send(downlinkDataMsg, gate("out", 0));
    }
}
