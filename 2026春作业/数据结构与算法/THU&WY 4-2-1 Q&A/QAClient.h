#ifndef QACLIENT
#define QACLIENT
#include <cstdio>
#include <cassert>
#include <iostream>
using namespace std;

class QAClient
{
private:
    bool get_text;
    bool answer_recived;
    int num_answer_received;
    int num_query_sent;
    char* text;
    char* query;
    int query_num;
public:
    QAClient();
    char* GetText();
    char* GetQuery();
    bool SendAnswer(int position);
    ~QAClient();
};

#endif