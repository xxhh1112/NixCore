// Copyright (c) 2018-2019 The NIX Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NIX_GOV_H
#define NIX_GOV_H

#include <serialize.h>
#include <uint256.h>
#include <amount.h>
#include <univalue.h>

#include <stdexcept>
#include <vector>


#include <boost/asio.hpp>
#include <boost/bind.hpp>

class CGovernance;

//static const std::string GOVERNANCE_URL = "www.governance.nixplatform.io";
static const std::string GOVERNANCE_URL = "134.209.47.211";

// amount of time in second for how often to refresh proposals
static const uint64_t REFRESH_TIME = 60;

extern CGovernance g_governance;
extern uint64_t last_refresh_time;

enum RequestTypes
{
    GET_PROPOSALS = 1,
    CAST_VOTE = 2,
};

struct Proposals{
    std::string vote_id;
    std::string name;
    std::string details;
    std::string address;
    std::string amount;
    std::string txid;
    std::string start_time;
    std::string end_time;
    std::string votes_affirm;
    std::string votes_oppose;

    std::string toString()
    {
       return "Vote ID: "          + vote_id + " \n\n" +
               "Name: "            + name + " \n\n" +
               "Details: "         + details + " \n\n" +
               "Address: "         + address + " \n\n" +
               "Amount: "          + amount + " \n\n" +
               "TxID: "            + txid + " \n\n" +
               "Start Time: "      + start_time + " \n\n" +
               "End Time: "        + end_time + " \n\n" +
               "Votes Affirm: "    + votes_affirm + " \n\n" +
               "Votes Oppose: "    + votes_oppose;
    }

    UniValue toJSONString()
    {
        UniValue result(UniValue::VOBJ);
        result.pushKV("vote_id", vote_id);
        result.pushKV("name", name);
        result.pushKV("details", details);
        result.pushKV("address", address);
        result.pushKV("amount", amount);
        result.pushKV("txid", txid);
        result.pushKV("start_time",start_time);
        result.pushKV("end_time", end_time);
        result.pushKV("votes_affirm", votes_affirm);
        result.pushKV("votes_oppose", votes_oppose);

        return result;
    }


};

class CGovernance
{
private:
    bool ready;

public:
    CGovernance();
    ~CGovernance();

    void GetRequests(RequestTypes rType);
    void PostRequest(RequestTypes rType, std::string json);
    //Get data
    std::string g_data;
    //Post data
    std::string p_data;

    std::vector<Proposals> proposals;
    bool isReady(){return ready;}
    void setReady(){ready = true;}

};

using boost::asio::ip::tcp;

typedef void(*HTTPRequestDataReceived)(char*, size_t);
typedef void(*HTTPRequestComplete)();

class HTTPGetRequest
{
public:
 HTTPGetRequest(
  boost::asio::io_service& io_service,
  std::string host,
  std::string relativeURL,
  HTTPRequestDataReceived receivedCB,
  HTTPRequestComplete completeCB,
  std::string jsonPost = "");

 ~HTTPGetRequest();

public:
 void sendRequest();
 void postRequest();

private:
 HTTPRequestDataReceived m_receivedCB;
 HTTPRequestComplete m_completeCB;

 std::string m_host;
 std::string m_relativeURL;
 std::string m_postURL;

 tcp::socket m_socket;
 boost::asio::io_service &m_io_service;
 tcp::resolver m_resolver;

 boost::asio::streambuf m_request;
 boost::asio::streambuf m_response;

 void ReadData();
};

class CGovernanceEntry
{
public:
    CAmount voteWeight;
    std::string voteID;

    CGovernanceEntry()
    {
        SetNull();
    }

    void SetNull()
    {
        voteWeight = 0;
        voteID.clear();
    }
    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(voteWeight);
        READWRITE(voteID);
    }
};

#endif // NIX_GOV_H