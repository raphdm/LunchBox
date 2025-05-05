#ifndef AUTH_H
#define AUTH_H
#include <string>

using namespace std;

class User
{
private:
    string username;
    string password;
    bool isLoggedIn;

public:
    User();
    string getUsername() const;
    bool getLoginStatus() const;

    bool login(const string& username, const string& password);
    void logout();
    bool registerUser(const string& username, const string& password);
    bool changePassword(const string& oldPassword, const string& newPassword);

    bool saveUserCredentials() const;
    bool loadUserCredentials(const string& username);
};

bool displayLoginMenu(User& user);

#endif