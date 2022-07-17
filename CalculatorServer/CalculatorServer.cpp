// CalculatorServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

void ProcessQueryString(char* req,SOCKET c) {
	char* p1 = strstr(req," ");
	char* p2 = strstr(p1 + 1, " ");
	int querystringLength = p2 - (p1+1);
	printf("%s", p1);
	char* querystrings = (char*)malloc(querystringLength + 1);
	memcpy(querystrings, p1+1, querystringLength);
	querystrings[querystringLength] = 0;

	printf("QueryString:\n%s\n", querystrings);

    char op[64], param1[512], param2[512];

    char* query = strtok(querystrings+2, "&");
    printf("%s", query);

    while (query != NULL)
    {
        char* p = strstr(query, "=");

        // Tach phan name
        int nameLength = p - query;
        char* name = (char*)malloc(nameLength + 1);
        memcpy(name, query, nameLength);
        name[nameLength] = 0;

        // Tach phan value
        int valueLength = strlen(query) - nameLength - 1;
        char* value = (char*)malloc(valueLength + 1);
        memcpy(value, query + nameLength + 1, valueLength);
        value[valueLength] = 0;

        if (strcmp(name, "op") == 0)
        {
            sprintf(op, "%s", value);
        }
        else if (strcmp(name, "param1") == 0)
        {
            sprintf(param1, "%s", value);
        }
        else if (strcmp(name, "param2") == 0)
        {
            sprintf(param2, "%s", value);
        }
        else {
            const char* response = "HTTP/1.1 404 Error\r\nContent-Type: text/html\r\n\r\n<html><body><p>Tham số không hợp lệ</p></body></html>";
            send(c, response, strlen(response), 0);
            return;
        }

        query = strtok(NULL, "&");
    }

    if (op == NULL || param1 == NULL || param2 == NULL)
    {
        const char* response = "HTTP/1.1 404 Error\r\nContent-Type: text/html\r\n\r\n<html><body><p>Thiếu tham số truyền vào</p></body></html>";
        send(c, response, strlen(response), 0);
    }
    else {
        char response[1024];
        if (strcmp(op, "add") == 0)
        {
            int num1 = atoi(param1);
            int num2 = atoi(param2);
            int result = num1 + num2;
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>%d + %d = %d</h1></body></html>", num1, num2, result);
            send(c, response, strlen(response), 0);
        }
        else if (strcmp(op, "sub") == 0)
        {
            int num1 = atoi(param1);
            int num2 = atoi(param2);
            int result = num1 - num2;
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>%d - %d = %d</h1></body></html>", num1, num2, result);
            send(c, response, strlen(response), 0);
        }
        else if (strcmp(op, "mul") == 0)
        {
            int num1 = atoi(param1);
            int num2 = atoi(param2);
            int result = num1 * num2;
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>%d x %d = %d</h1></body></html>", num1, num2, result);
            send(c, response, strlen(response), 0);
        }
        else if (strcmp(op, "div") == 0)
        {
            int num1 = atoi(param1);
            int num2 = atoi(param2);
            int result = num1 / num2;
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>%d / %d = %d</h1></body></html>", num1, num2, result);
            send(c, response, strlen(response), 0);
        }
        else {
            const char* response = "HTTP/1.1 404 Error\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Phep tinh khong hop le</h1></body></html>";
            send(c, response, strlen(response), 0);
        }
    }
}
int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);
	char buf[2048];
	int ret;
    while (1)
    {
        // Chap nhan ket noi
        SOCKET client = accept(listener, NULL, NULL);
        // Nhan yeu cau
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            closesocket(client);
            continue;
        }
        // Xu ly yeu cau
        if (ret < sizeof(buf))
            buf[ret] = 0;
        printf("%s\n", buf);
        ProcessQueryString(buf, client);
        // Tra lai ket qua
        // Dong ket noi
        closesocket(client);
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
