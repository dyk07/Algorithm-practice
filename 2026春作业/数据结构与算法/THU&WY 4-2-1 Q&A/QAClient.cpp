#include "QAClient.h"

QAClient::QAClient(){
        get_text = false;
        answer_recived = true;
        num_answer_received = 0;
        num_query_sent = 0;
        text = new char[10000000];
        query = new char[2000];
}

QAClient::~QAClient(){
        delete []text;
        delete []query;
}

char* QAClient::GetText(){
        if (get_text)
            return text;
        scanf("%s", text);
        scanf("%d", &query_num);
        get_text = true;
        return text;
}

char* QAClient::GetQuery(){
        if (!get_text){
            printf("You haven't get the text yet");
            assert(false);
        }
        if (!answer_recived){
            printf("You haven't give me the answer to the last query");
            assert(false);
        }
        if (num_answer_received > query_num){
            printf("You've given too many answers");
            assert(false);
        }
        answer_recived = true;
        scanf("%s", query);
        num_query_sent ++;
        return query;
}

bool QAClient::SendAnswer(int position){
        printf("%d\n", position);
        answer_recived = true;
        num_answer_received ++;
        assert(num_answer_received <= num_query_sent);
        assert(num_answer_received <= query_num);
        return (num_answer_received != query_num);
}