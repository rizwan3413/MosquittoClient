/*******************************************************************************
* DataObserver.cpp                                                              *
* FUNCTIONAL DESCRIPTION:                                                       *
* Works as MQTT client                                                          *
********************************************************************************/

#include "DataObserver.h"
#include <iostream>
#include <sstream>

/*****************************************************************************************
 * Global Pointer to DataObserver Client
*****************************************************************************************/
DataObserver* dataClient = nullptr;

/*****************************************************************************************
 * Constructor
*****************************************************************************************/
DataObserver::DataObserver(const char *id, const char *host, int port,int keepalive) : mosquittopp(id)
{
    m_msgId = 0;
    mosqpp::lib_init(); 
    connect_async(host, port, keepalive);
    loop_start();
};

/*****************************************************************************************
 * Destructor
*****************************************************************************************/
DataObserver::~DataObserver()
{
    disconnect();
    loop_stop();
    mosqpp::lib_cleanup();
}

/*****************************************************************************************
 * Connect Callback. Called onnce connection is established
*****************************************************************************************/
void DataObserver::on_connect(int rc)
{
	if(rc == 0){
        std::cout << "Connected with the Broker with return code: " << rc  << "\n";
        /* Subscribe for CAN and GNSS Data */
	    subscribe(NULL, TOPIC_DATA_BLE_SUB);
	    subscribe(NULL, TOPIC_DATA_GNSS_SUB); 
    }
    else {
        std::cout << "MQTT connection disconnected, return code:" <<  rc << "\n";
    }
}

/*****************************************************************************************
 * On Message Callback. Called on recieving message on app/server/ble and app/server/gnss topic
*****************************************************************************************/
void DataObserver::on_message(const struct mosquitto_message *message)
{
    std::vector <std::string> dataStr;
    std::string topic_name = message->topic;
    getParsedStr (message, dataStr);

    if(topic_name.compare(TOPIC_DATA_BLE_SUB) == 0 ) {
        storeBleData(dataStr);
        send_message(TOPIC_DATA_BLE_PUB, bleData);
    }

    else if (topic_name.compare(TOPIC_DATA_GNSS_SUB) == 0) {
        storeGnssData(dataStr);
        send_message(TOPIC_DATA_GNSS_PUB, gnssData);
    }
   
    else {
        std::cout << "Recieved data on unknown topic: " << topic_name << "\n";
    }
}

/*****************************************************************************************
 * Function to store BLE Data
*****************************************************************************************/
void DataObserver::storeBleData(std::vector<std::string> &dataStr) {
    if (dataStr.at(0).compare("data") == 0) {
        for (unsigned int idx = 1; idx < dataStr.size()-1; idx+=2){
            std::string characteristic = dataStr.at(idx);
            std::string characteristic_value = dataStr.at(idx+1);
            std::cout << "Key:"  << characteristic <<  " Value: " << characteristic_value << "\n";
            if(bleData.find(characteristic) == bleData.end()) {
                bleData.insert(std::pair<std::string, std::string>(characteristic, characteristic_value));
            }
            else {
                bleData[characteristic] = characteristic_value;
            }
        }
    }
    else {
        std::cout << "Data format is not correct\n";
    }
}

/*****************************************************************************************
 * Function to store GNSS Data
*****************************************************************************************/
void DataObserver::storeGnssData(std::vector<std::string> &dataStr) {
    if (dataStr.at(0).compare("data") == 0) {
        for (unsigned int idx = 1; idx < dataStr.size()-1; idx+=2) {
            std::string characteristic = dataStr.at(idx);
            std::string characteristic_value = dataStr.at(idx+1);
            std::cout << "Key:"  << characteristic <<  " Value: " << characteristic_value << "\n";
            if(gnssData.find(characteristic) == gnssData.end()) {
                gnssData.insert(std::pair<std::string, std::string>(characteristic, characteristic_value));
            }
            else {
                gnssData[characteristic] = characteristic_value;
            }
       }
    }
    else {
        std::cout << "Data format is not correct\n";
    }
}

/*****************************************************************************************
 * On subscribe callback. Called on successfull sub/scription on topic
*****************************************************************************************/
void DataObserver::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    std::cout << "Successfully subscribed to the topic" << "\n";
}

/*****************************************************************************************
 * On disconnect callback. Called once client is disconnected from MQTT Broker
*****************************************************************************************/
void DataObserver::on_disconnect(int rc) {
    std::cout << "Disconnected with return code: " << rc << "\n";
}

/*****************************************************************************************
 * On publish callback. Called each time a message succeed to be sent to broker
*****************************************************************************************/
void DataObserver::on_publish(int mid)
{
    std::cout << "Message with mid->  " << mid << " succeed to be published\n";
}


/*****************************************************************************************
 * Called each time a message needs to be sent to broker
*****************************************************************************************/
void DataObserver::send_message(std::string topic, std::map<std::string, std::string>& data)
{
    std::string buf = "data";
    for(auto member: data) {
        buf += ", " + member.first + "=" + member.second;
    }
    std::cout << "Trying to publish on topic: app/ble/hr, msg: " << m_msgId << ": " << buf << "\n";
    if (publish(&m_msgId, topic.c_str(), buf.length(), (void*)buf.c_str(), 2, false) == 0) { 
        std::cout << "Message (" << m_msgId << ") published successfully on " << topic << "\n";
    }
    else {
        std::cout << "Failed to send message(" << m_msgId << ") on " << topic << "\n";
    }
}


/*****************************************************************************************
 * Function to parsed received message by , delimeter and fill the data in dataVect
*****************************************************************************************/
void DataObserver::getParsedStr (const struct mosquitto_message *message, std::vector<std::string>& dataVec)
{
    std::string msg ((char*)message->payload) ;
    size_t found = msg.find(',');
    dataVec.push_back(std::string(msg,0,found));
    msg = msg.substr(found+1);
    std::stringstream ss(msg);
    while (ss.good())
    {
        std::string substr;
        getline( ss, substr, ',' );
        size_t found = substr.find('='); 
        if (found != std::string::npos) {
            dataVec.push_back(substr.substr(0,found));
            dataVec.push_back(substr.substr(found+1));
        }
    }
    std::cout << "Size of parsed vector " << dataVec.size() << "\n";
}

/*****************************************************************************************
 * Create the object of DataObserver client
*****************************************************************************************/
void DataObserver::startDataObserver(const char *id, const char *host, int port,int keepalive)
{
    dataClient = new DataObserver(id, host, port, keepalive);  
}

/*****************************************************************************************
 * Return the pointer to Global DataObserver client pointer
*****************************************************************************************/
DataObserver* DataObserver::getClientDataPointer()
{
    return dataClient;
}

