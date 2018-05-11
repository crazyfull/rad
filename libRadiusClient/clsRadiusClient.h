#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
using namespace std;


extern string g_RadiusServerAddress;
extern string g_RadiusSecret;
extern string g_NasIP;
extern int g_RadiusAuthenticationPort;
extern int g_RadiusAccountingPort;

extern int g_RadiusAccountingTimeOut;
extern int g_RadiusAccountingRetryCount;

extern int g_RadiusAuthenticationTimeOut;
extern int g_RadiusAuthenticationRetryCount;

typedef enum AUTHENTICATION
{
    AUTHENTICATION_FAILED = 999,
    AUTHENTICATION_PENDING = 998,
    AUTHENTICATION_ASK_FROM_RADIUS = 997,
    AUTHENTICATION_ACCEPT = 2,
    AUTHENTICATION_REJECT = 3
};

typedef enum ACCOUNTING
{
    ACCOUNTING_START = 1,
    ACCOUNTING_STOP = 2,
    ACCOUNTING_INTERIM_UPDATE = 3,
    ACCOUNTING_ON=7,
    ACCOUNTING_OFF=8
};

class clsRadiusClient
{
private:
    //varibale

    //functions
    static void LoadSocket();
    static void UnLoadSocket();

public:
    clsRadiusClient(void);
    ~clsRadiusClient(void);

    static void SetServerAddress(const char *RadiusHOST);
    static void SetCurrentServerAddress(const char *IPAddress);
    static void SetServerAuthPort(int port);
    static void SetServerAccountPort(int port);
    static void SetSecret(const char *secret);
    static void SetAccountingTimeOut(int TimeOut, int RetryCount);
    static void SetAuthenticationTimeOut(int TimeOut, int RetryCount);


    static AUTHENTICATION Authentication(const char* User, const char* Pass);

    static bool Accounting(ACCOUNTING AccountingType, const char *UserName, const char *ClientIP, int TimeOut, int RetryCount, size_t nas_port, bool needResponse = true, size_t Input = 0, size_t Output = 0);
    static bool Accounting_START(const char *UserName, const char *ClientIP, uint32_t SessionID);
    static bool Accounting_STOP(const char *UserName, const char *ClientIP, uint32_t SessionID);
    static bool Accounting_ON();
};
