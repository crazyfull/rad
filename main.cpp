#include <iostream>
#include "libRadiusClient/clsRadiusClient.h"
using namespace std;

int main(int argc, char *argv[])
{

    bool isSend;
    //set config
    clsRadiusClient::SetServerAddress("127.0.0.1");
    clsRadiusClient::SetCurrentServerAddress("192.168.1.250");
    clsRadiusClient::SetServerAuthPort(1812);
    clsRadiusClient::SetServerAccountPort(1812);
    clsRadiusClient::SetSecret("moji");
    clsRadiusClient::SetAccountingTimeOut(4, 1);
    clsRadiusClient::SetAuthenticationTimeOut(3, 1);

    //start radius
    clsRadiusClient::Accounting_ON();


    //AUTHENTICATION
    /**/
    AUTHENTICATION ret = clsRadiusClient::Authentication("nemo", "gooz");
    if(ret == AUTHENTICATION_FAILED){
        printf("AUTHENTICATION FAILED\n");
        return 1;
    }


    if(ret == AUTHENTICATION_PENDING){
        printf("AUTHENTICATION PENDING\n");
        return 1;
    }

    if(ret == AUTHENTICATION_ASK_FROM_RADIUS){
        printf("AUTHENTICATION ASK FROM RADIUS\n");
        return 1;
    }

    if(ret == AUTHENTICATION_ACCEPT){
        printf("AUTHENTICATION ACCEPT\n");
    }

    if(ret == AUTHENTICATION_REJECT){
        printf("AUTHENTICATION REJECT\n");
        return 1;
    }

    //start accounting
    isSend = clsRadiusClient::Accounting_START("test31", "180.24.54.12", 600);
    if(isSend){
        printf("Successfully Accounting_START\n");
    }

    //stop accounting
    /*
    isSend = clsRadiusClient::Accounting_STOP("test31", "180.24.54.12", 600);
    if(isSend){
        printf("Successfully Accounting_STOP\n");
    }
*/
    getchar();
    return 0;
}
