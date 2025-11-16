#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    int id;
    std::string name;
    std::string email;
    std::string profile;

public:
    User();
    User(int id, const std::string& name, const std::string& email, const std::string& profile = "");
    
    int getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getProfile() const;
    
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setProfile(const std::string& profile);
};

#endif
