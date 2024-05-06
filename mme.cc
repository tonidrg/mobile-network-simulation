/*
 * mme.cc
 *
 *  Created on: Jun 19, 2019
 *      Author: Bart
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

bool startAuthentication = false;
bool forwardAuth = false;
bool returnAuth = false;

bool createSession = false;
bool contextSetup = false;

bool notify_UE = false;
bool notifyUEUplink = false;

bool modifyBearer = false;

class MME : public cSimpleModule {
    protected:
        // The following redefined virtual function holds the algorithm.
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(MME);

void MME::initialize() {}

void MME::handleMessage(cMessage *msg) {
    cModule *sender = msg->getSenderModule();
    std::string senderName = sender->getName();

    if(senderName == "enodeb" && !startAuthentication){
        startAuthentication = true;

        cMessage *startAuthenticationMsg = new cMessage("");
        send(startAuthenticationMsg, gate("out", 2));
    } else if(senderName == "hss" && startAuthentication && !forwardAuth){
        forwardAuth = true;

        cMessage *forwardAuthMsg = new cMessage("Authentication and autorization");
        send(forwardAuthMsg, gate("out", 0));
    } else if(senderName == "enodeb" && forwardAuth && !returnAuth){
        returnAuth = true;

        cMessage *returnAuthMsg = new cMessage("Authentication and Autorization");
        send(returnAuthMsg, gate("out", 2));
    } else if(senderName == "hss" && returnAuth && !createSession){
        createSession = true;

        cMessage *createSessionMsg = new cMessage("Create Session");
        EV << "Slanje zathjeva za stvaranje sesije" << "\n";
        send(createSessionMsg, gate("out", 1));
    } else if(senderName == "sgw" && createSession && !contextSetup){
        contextSetup = true;

        cMessage *contextSetupMsg = new cMessage("Attach Accept/Initial Context Setup");
        EV << "MME salje zahtjev za postavljanje konteksta eNB-u" << "\n";
        send(contextSetupMsg, gate("out", 0));
    } else if(senderName == "enodeb" && contextSetup && !notify_UE){
        notify_UE = true;

        cMessage *notifyUEMsg = new cMessage("");
        send(notifyUEMsg, gate("out", 0));
    } else if(senderName == "enodeb" && notify_UE && !notifyUEUplink){
        notifyUEUplink = true;

        cMessage *uplinkMsg = new cMessage("");
        send(uplinkMsg, gate("out", 0));
    } else if(senderName == "sgw" && notifyUEUplink && !modifyBearer){
        modifyBearer = true;

        cMessage *modifyBearerMsg = new cMessage("Modify Bearer");
        EV << "MME salje zahtjev za modifikaciju nosaca" << "\n";
        send(modifyBearerMsg, gate("out", 1));
    }
}


