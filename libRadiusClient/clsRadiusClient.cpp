#include "clsRadiusClient.h"
#include "RadiusClientStack.h"
#include "config.h"
#include "log.h"

string g_RadiusServerAddress;
string g_RadiusSecret;
string g_NasIP;
int g_RadiusAuthenticationPort;
int g_RadiusAccountingPort;
int g_RadiusAccountingTimeOut;
int g_RadiusAccountingRetryCount;
int g_RadiusAuthenticationTimeOut;
int g_RadiusAuthenticationRetryCount;


clsRadiusClient::clsRadiusClient(void)
{

}

clsRadiusClient::~clsRadiusClient(void)
{
}


void clsRadiusClient::LoadSocket()
{
#ifdef _WIN32
    //load socket
    int answer;
    WSADATA wsaData;
    WORD wVersionRequested = 0; //WORD
    answer = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(answer < 0)
    {
        printf("::Error On Load Socket::");
        return;
    }
#endif
}

void clsRadiusClient::UnLoadSocket()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

void clsRadiusClient::SetServerAddress(const char* RadiusHOST)
{
    g_RadiusServerAddress = RadiusHOST;
}

void clsRadiusClient::SetCurrentServerAddress(const char *IPAddress)
{
    g_NasIP = IPAddress;
}
void clsRadiusClient::SetServerAuthPort(int port)
{
    g_RadiusAuthenticationPort = port;
}
void clsRadiusClient::SetServerAccountPort(int port)
{
    g_RadiusAccountingPort = port;
}
void clsRadiusClient::SetSecret(const char* secret)
{
    g_RadiusSecret = secret;
}

void clsRadiusClient::SetAccountingTimeOut(int TimeOut, int RetryCount)
{
    g_RadiusAccountingTimeOut = TimeOut;
    g_RadiusAccountingRetryCount = RetryCount;
}

void clsRadiusClient::SetAuthenticationTimeOut(int TimeOut, int RetryCount)
{
    g_RadiusAuthenticationTimeOut = TimeOut;
    g_RadiusAuthenticationRetryCount = RetryCount;
}

AUTHENTICATION clsRadiusClient::Authentication(const char* User, const char* Pass)
{
    //load socket
    LoadSocket();

    //stack
    RadiusClientStack Stack;

    if (!Stack.isValid() || g_RadiusSecret == "" || g_RadiusAuthenticationPort <= 0)
    {
        //printf("Can not create RADIUS client stack\n");
        UnLoadSocket();
        return AUTHENTICATION_FAILED;
    }

    // add connection with some RADIUS server
    Stack.addServer(g_RadiusServerAddress.data(), g_RadiusAuthenticationPort, RadiusSecret(g_RadiusSecret.data()), g_RadiusAuthenticationTimeOut , g_RadiusAuthenticationRetryCount); //D_RADIUS_AUTHENTICATION_PORT

    // create a new packet
    RadiusPacket Pck(D_PACKET_ACCESS_REQUEST, Stack);

    //for attributes
    RadiusAttribute MyAttr;

    //add user
    Pck.addAttribute(D_ATTR_USER_NAME, MyAttr);
    MyAttr.setString(User);

    //add pass
    Pck.addAttribute(D_ATTR_USER_PASSWORD, MyAttr);
    MyAttr.setUserPassword(Pass);

    //Set NasPort - for ibs
    //Pck.addAttribute(D_ATTR_NAS_PORT, MyAttr);
    //MyAttr.setNumber(99999);


    // add Service-Type attribute
    //Pck.addAttribute(D_ATTR_NAS_PORT_TYPE, MyAttr);
    //Pck.addAttribute(D_ATTR_FRAMED_PROTOCOL, MyAttr);
    //MyAttr.setNumber(2);
    //MyAttr.setType(2); //2 = PPP


    // send the packet
    //Pck.dump();
    RadiusPacket *Response = Stack.sendPacket(Pck, true);

    if(!Response)
    {
        UnLoadSocket();
        return AUTHENTICATION_FAILED;
    }

    //Response->dump();
    //parse response
    if(Response->getCode() == AUTHENTICATION_ACCEPT ) //2 = accept
    {
        //printf("Authentication-Accept\n");
        UnLoadSocket();
        return AUTHENTICATION_ACCEPT;
    }
    //Response->getID
    //printf("Authentication Faild, Reason %s(%d)\n", Response->getCodeDescription(),  Response->getCode());
    UnLoadSocket();
    return (AUTHENTICATION)Response->getCode();
}

bool clsRadiusClient::Accounting(ACCOUNTING AccountingType, const char *UserName, const char *ClientIP, int TimeOut, int RetryCount, size_t nas_port, bool needResponse, size_t Input, size_t Output)
{
    if(!g_NasIP.data()){
        DebugPrint("Radius NasIP is not set.");
        return false;
    }
    //load socket
    LoadSocket();

    //stack
    RadiusClientStack Stack;

    if ( !Stack.isValid() )
    {
        //printf("Can not create RADIUS client stack\n");
        UnLoadSocket();
        return false;
    }

    // add connection with some RADIUS server
    Stack.addServer(g_RadiusServerAddress.data(), g_RadiusAccountingPort, RadiusSecret(g_RadiusSecret.data()), TimeOut, RetryCount);

    // create a new accountng packet
    RadiusPacket Pck(D_PACKET_ACCOUNTING_REQUEST, Stack);

    //for attributes
    RadiusAttribute MyAttr;


    //set status type (Start , Stop ,Update)
    Pck.addAttribute(D_ATTR_ACCT_STATUS_TYPE, MyAttr);
    MyAttr.setNumber(AccountingType);

    //Set UserName
    if(UserName != NULL){
        Pck.addAttribute(D_ATTR_USER_NAME, MyAttr);
        MyAttr.setString(UserName);
    }


    //Set NasType
    if(nas_port > 0){
        Pck.addAttribute(D_ATTR_NAS_PORT_TYPE, MyAttr);
        MyAttr.setNumber(1); //Sync

        //Set NasPort
        Pck.addAttribute(D_ATTR_NAS_PORT, MyAttr);
        MyAttr.setNumber(nas_port);
    }

    //set Nas IP-Server
    struct in_addr IPaddr;
    IPaddr.s_addr = inet_addr(g_NasIP.data());
    Pck.addAttribute(D_ATTR_NAS_IP_ADDRESS, MyAttr);
    MyAttr.setIPAddress(IPaddr);


    //Nas idntfier
    Pck.addAttribute(D_ATTR_NAS_IDENTIFIER, MyAttr);
    MyAttr.setString(ACCOUNTING_NAME);

    // Set Seasion-id
    //Pck.addAttribute(D_ATTR_ACCT_SESSION_ID, MyAttr);
    //MyAttr.setString("Mysession");

    //caller id
    if(ClientIP != NULL){
        Pck.addAttribute(D_ATTR_CALLED_STATION_ID, MyAttr);
        struct in_addr CallerIP;
        CallerIP.s_addr = inet_addr(ClientIP);
        MyAttr.setIPAddress(CallerIP);
    }

    // add Service-Type attribute
    Pck.addAttribute(D_ATTR_SERVICE_TYPE, MyAttr);
    MyAttr.setNumber(2);	//Framed bayad bashe

    // add Framed-Protocole
    Pck.addAttribute(D_ATTR_FRAMED_PROTOCOL, MyAttr);
    MyAttr.setNumber(1);	//PPP

    //set client ip
    if(ClientIP != NULL){
        Pck.addAttribute(D_ATTR_CALLED_STATION_ID, MyAttr);
        struct in_addr ClientCalledIP;
        ClientCalledIP.s_addr = inet_addr(ClientIP);
        MyAttr.setIPAddress(ClientCalledIP);
    }


    //add upload-size
    if(Output > 0)
    {
        // add Framed-Protocole
        Pck.addAttribute(D_ATTR_ACCT_OUTPUT_OCTETS, MyAttr);
        MyAttr.setNumber(Output);
    }

    //add download-size
    if(Input > 0)
    {
        // add Framed-Protocole
        Pck.addAttribute(D_ATTR_ACCT_INPUT_OCTETS, MyAttr);
        MyAttr.setNumber(Input);
    }


    // send the packet
    //Pck.dump();
    RadiusPacket *Response = Stack.sendPacket(Pck, needResponse);

    //
    //parse response
    if(Response)
    {
        //Response->accountingRequestAuthenticator(&RadiusSecret("asan"));
        //printf("== accounting OK == \n");
        //Response->dump();
        UnLoadSocket();
        return true;
    }

    UnLoadSocket();
    return false;
}

bool clsRadiusClient::Accounting_ON()
{
    return Accounting(ACCOUNTING_ON, NULL, NULL, g_RadiusAccountingTimeOut, g_RadiusAccountingRetryCount, 0, false);
}

bool clsRadiusClient::Accounting_START(const char *UserName, const char *ClientIP, uint32_t SessionID)
{
    return Accounting(ACCOUNTING_START, UserName, ClientIP, g_RadiusAccountingTimeOut,g_RadiusAccountingRetryCount, SessionID);
}

bool clsRadiusClient::Accounting_STOP(const char *UserName, const char *ClientIP, uint32_t SessionID)
{
    return Accounting(ACCOUNTING_STOP, UserName, ClientIP, g_RadiusAccountingTimeOut, g_RadiusAccountingRetryCount, SessionID);
}
