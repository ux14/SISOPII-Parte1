#include <iostream>
#include <arpa/inet.h>

using namespace std;

bool isValidUsername(string username)
{
    if (username.length() < 4 || username.length() > 20)
        return false;

    return true;
}

bool isValidServerIp(string server_ip)
{
    char buffer[sizeof(struct in6_addr)];

    if (inet_pton(AF_INET, server_ip.c_str(), buffer) != 1)
        return false;

    return true;
}

bool isValidPort(string port_str)
{
    int port;

    try
    {
        port = stoi(port_str);
    }
    catch (...)
    {
        return false;
    }

    if (port < 0 || port > 65535)
    {
        return false;
    }

    return true;
}

void validateInputs(string username, string server_ip, string port)
{
    if (!isValidUsername(username))
    {
        cout << "ERROR: invalid username" << endl;
        exit(-1);
    }

    if (!isValidServerIp(server_ip))
    {
        cout << "ERROR: invalid server IP" << endl;
        exit(-1);
    }

    if (!isValidPort(port))
    {
        cout << "ERROR: invalid port" << endl;
        exit(-1);
    }
}