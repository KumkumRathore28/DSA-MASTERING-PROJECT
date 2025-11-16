#include "User.h"

User::User() : id(-1), name(""), email(""), profile("") {}

User::User(int id, const std::string& name, const std::string& email, const std::string& profile)
    : id(id), name(name), email(email), profile(profile) {}

int User::getId() const { 
    return id; 
}

std::string User::getName() const { 
    return name; 
}

std::string User::getEmail() const { 
    return email; 
}

std::string User::getProfile() const { 
    return profile; 
}

void User::setName(const std::string& name) { 
    this->name = name; 
}

void User::setEmail(const std::string& email) { 
    this->email = email; 
}

void User::setProfile(const std::string& profile) { 
    this->profile = profile; 
}
