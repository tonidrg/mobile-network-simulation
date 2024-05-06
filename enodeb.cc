/*
 * enodeb.cc
 *
 *  Created on: Jun 19, 2019
 *      Author: Bart
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

bool forwardAttachRequestMME = false;
bool forwardAuthentication = false;
bool returnAuthentication = false;

bool rbSetup = false;
bool initialContext = false;

bool notifyUE = false;
bool attachComplete = false;
bool notifyUEUplinkData = false;

bool sendUplinkData1 = false;
bool sendDownlinkData2 = false;

class ENodeB : public cSimpleModule {
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(ENodeB);

void ENodeB::initialize() {}

void ENodeB::handleMessage(cMessage *msg) {
    cModule *sender = msg->getSenderModule();
    std::string senderName = sender->getName();

    if(senderName == "ue" && !forwardAttachRequestMME){
        forwardAttachRequestMME = true;

        cMessage *forwardARMME = new cMessage("Attach request");
        EV << "eNB prosljeduje \"Attach\" zahtjeva od UE prema MME" << "\n";
        send(forwardARMME, gate("out", 1));
    } else if(senderName == "mme" && forwardAttachRequestMME && !forwardAuthentication){
        forwardAuthentication = true;

        cMessage *forwardAuthenticationMsg = new cMessage("Authentication and autorization");
        send(forwardAuthenticationMsg, gate("out", 0));
    } else if(senderName == "ue" && forwardAuthentication && !returnAuthentication){
        returnAuthentication = true;

        cMessage *returnAuthMsg = new cMessage("Authentication and Autorization");
        send(returnAuthMsg, gate("out", 1));
    } else if(senderName == "mme" && returnAuthentication && !rbSetup){
        rbSetup = true;

        cMessage *rbSetupMsg = new cMessage("RB Setup");
        EV << "Rekonfiguracija radio nosaca, prema parametrima primljenim od strane MME, izmedu UE i eNB" << "\n";
        send(rbSetupMsg, gate("out", 0));
    } else if(senderName == "ue" && rbSetup && !initialContext){
        initialContext = true;

        cMessage *initialContextMsg = new cMessage("Initial Context");
        EV << "eNB salje odgovor za postavljanje konteksta MME-u" << "\n";
        send(initialContextMsg, gate("out", 1));
    } else if(senderName == "mme" && initialContext && !notifyUE){
        notifyUE = true;

        cMessage *notifyUEMsg = new cMessage("");
        send(notifyUEMsg, gate("out", 0));
    } else if(senderName == "ue" && notifyUE && !attachComplete){
        attachComplete = true;

        cMessage *attachCompleteMsg = new cMessage("Attach Complete");
        EV << "eNB prosljeduje \"Attach Complete\" poruku MME-u" << "\n";
        send(attachCompleteMsg, gate("out", 1));
    } else if(senderName == "mme" && attachComplete && !notifyUEUplinkData){
        notifyUEUplinkData = true;

        cMessage *uplinkMsg = new cMessage("");
        send(uplinkMsg, gate("out", 0));
    } else if(senderName == "ue" && notifyUEUplinkData && !sendUplinkData1){
        sendUplinkData1 = true;

        cMessage *uplinkData = new cMessage("Uplink Data");
        send(uplinkData, gate("out", 2));
    } else if(senderName == "sgw" && sendUplinkData1 && !sendDownlinkData2){
        sendDownlinkData2 = true;

        cMessage *downlinkData = new cMessage("Downlink Data");
        send(downlinkData, gate("out", 0));
    }
}


