#include <iostream>
#include <cstring>
#include <cstdio>
#include <cassert>
#include "QAClient.h"
#include "QAServer.h"
using namespace std;

char* text, *question;
int main(){

    QAClient client;    
    text = client.GetText();
    int answer = 0;

    do{

        question = client.GetQuery();
        answer = GetAnswer(text,question);

    }while(client.SendAnswer(answer));

    return 0;
    
}