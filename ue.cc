/*
 * ue.cc
 *
 *  Created on: Jun 19, 2019
 *      Author: Bart
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

bool authAutor = false;
bool rbSetupResponse = false;
bool directTransfer = false;
bool sendUplinkData = false;

class UE : public cSimpleModule {
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(UE);

void UE::initialize() {
    cMessage *attachRequestMME = new cMessage("Attach request");
    EV << "UE salje \"Attach\" zathjeva prema baznoj stanici" << "\n";
    send(attachRequestMME, gate("out"));
}

void UE::handleMessage(cMessage *msg) {
    if(!authAutor){
        authAutor = true;

        cMessage *authAutorMsg = new cMessage("Authentication and Autorization");
        send(authAutorMsg, gate("out"));
    } else if(authAutor && !rbSetupResponse){
        rbSetupResponse = true;

        cMessage *rbSetupMsg = new cMessage("RB Setup");
        send(rbSetupMsg, gate("out"));
    } else if(rbSetupResponse && !directTransfer){
        directTransfer = true;

        cMessage *directTransferMsg = new cMessage("Direct Transfer");
        EV << "UE salje poruku eNB-u koja sadrzi \"Attach Complete\" signal" << "\n";
        send(directTransferMsg, gate("out"));
    } else if(directTransfer && !sendUplinkData){
        sendUplinkData = true;

        cMessage *uplinkData = new cMessage("Uplink Data");
        EV << "Uplink konekcija (tunel) je postavljena i UE krece sa slanjem uplink paketa prema PGW-u" << "\n";
        send(uplinkData, gate("out"));
    } else if(sendUplinkData){
        EV << "UE ima pristup internetu" << "\n";
    }
}

