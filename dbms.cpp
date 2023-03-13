#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

class ClientModule {

public:
    struct Token {
        string name;
        string data;
    };

    // TODO: define specification
    struct Configuration {
        vector<Token> triggerTokens; // токены(константы) с которыми работаем
        vector<Token> inputTokens; // все токены которые пришли(запрос)
        vector<Token> outputTokens; // токены на выходе с зашифрованной константой
    };

    ClientModule() { };

    void setConfig(Configuration _config) {
        config = _config;
    }

    Configuration getConfig() {
        return config;
    }

    void addToken(ClientModule::Token _token, int tokenField) {
        switch (tokenField)
        {
        case 1:
            config.inputTokens.push_back(_token);
            break;
        case 2:
            config.triggerTokens.push_back(_token);
            break;
        case 3:
            config.outputTokens.push_back(_token);
            break;
        default:
            return;
        }
    }

private:
    Configuration config;

};

class KeyStore {

public:
    KeyStore() { };

    // Path: 
    //     "Database/Table/Column"
    //
    // Example:
    //     "mysql/orders/order_id"
    //

    struct Key {
        int metadata;
        char data;
        //string metadata; // algorithm, etc..
        //int length;
        //unsigned char* binaryData;
    };
    //void addKey(string moduleName, string path)
    //map<int, Key> getKeys(string moduleName, string path)
    void addKey(Key _key, int _metadata) {
        keys[_metadata] = _key;
    };
    void setKey(string moduleName, string path);
    
    map<int, Key> getKeys() {
        return keys;
    };

    void setKeys(map<int, Key> _keys) {
        keys = _keys;
    }

    /*
    ---------------
    Database Schema
    ---------------

    databases
     └─ tables
        └─ columns
            └─ keys (separate for RND, DET, OPE, ...)
    */

private:
    map<int, Key> keys;

};

class ServerModule {

public:
    ServerModule() { };

    struct Token {
        string name;
        string data;
    };

    // TODO: do we need this?
    //
    // return outputTokens
    vector<Token> processIncomingSql(vector<ClientModule::Token> inputTokens) {
        char key = 'x'; // how to get this one????
        vector<Token> output;
        for (ClientModule::Token token : inputTokens) {
            ServerModule::Token rtoken;
            rtoken.name = token.name;
            rtoken.data = token.data + key;
            output.push_back(rtoken);
        }
        return output;
    };
    /* REQUIRES:
     */
    struct ConfigurationS { // gRPC???
        vector<Token> triggerTokens;
        vector<ClientModule::Token> inputTokens; // допустим что вход только с клиента
        vector<Token> outputTokens;
    };
     


     // TODO: define what data in what format
    void processOutgoingData(/* data */);

    // TODO: define what RPC procedures this module provides
    // 
    // see Protobuf, gRPC

    ConfigurationS configS;

private:


};

int main()
{
    ClientModule client;
    ClientModule::Token token;
    ClientModule::Configuration config;
    token.name = "constant";
    token.data = "15000";
    client.setConfig(config);
    client.addToken(token, 2);

    KeyStore store;
    KeyStore::Key key;
    map<int, KeyStore::Key> keys;
    key.metadata = 0;
    key.data = 'x';
    store.addKey(key, key.metadata);

    token.data = token.data + store.getKeys()[0].data; // encrypt
    token.name = "decrypt";
    
    client.addToken(token, 3);

    ServerModule server;
    server.configS.inputTokens.push_back(token); // not ok
    server.configS.outputTokens = server.processIncomingSql(server.configS.inputTokens);

}


