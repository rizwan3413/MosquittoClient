#ifndef DATAOBSERVER_H
#define DATAOBSERVER_H

#include <mosquittopp.h>
#include <string>
#include <vector>
#include <map>

class DataObserver : public mosqpp::mosquittopp
{
    public:
        ~DataObserver();
        static constexpr const char* TOPIC_DATA_BLE_SUB = "app/server/ble";
        static constexpr const char* TOPIC_DATA_GNSS_SUB = "app/server/gnss";
        static constexpr const char* TOPIC_DATA_BLE_PUB = "app/data/ble";
        static constexpr const char* TOPIC_DATA_GNSS_PUB = "app/data/gnss";
        void on_connect(int rc);
        void on_message(const struct mosquitto_message *message);
        void on_subscribe(int mid, int qos_count, const int *granted_qos);
        void on_disconnect(int rc);
        void on_publish(int mid);
        static void startDataObserver(const char *id, const char *host, int port,int keepalive);
        static DataObserver* getClientDataPointer();
        void send_message(std::string topic, std::map<std::string, std::string>&);
    private:
        DataObserver(const char *id, const char *host, int port, int keepalive);
        DataObserver(const DataObserver&) = delete;
        DataObserver& operator=(const DataObserver&) = delete;
        void getParsedStr (const struct mosquitto_message *message, std::vector <std::string>& dataVec);
        std::map<std::string,std::string> bleData;
        std::map<std::string,std::string> gnssData;
        int m_msgId;
        void storeBleData(std::vector<std::string>&);
        void storeGnssData(std::vector<std::string>&);
};

#endif
