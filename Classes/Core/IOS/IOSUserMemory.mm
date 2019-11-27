#include "IOSUserMemory.h"

IOSUserMemory::IOSUserMemory() {
    m_bindings = [PDKeychainBindings sharedKeychainBindings];
}

void IOSUserMemory::StoreUsername(const std::string& username) {
    NSString *nsUsername = [NSString stringWithCString:username.c_str() encoding:[NSString defaultCStringEncoding]];
    [m_bindings setObject:nsUsername forKey:@"username"];
}

void IOSUserMemory::StorePassword(const std::string& password) {
    NSString *nsPassword = [NSString stringWithCString:password.c_str() encoding:[NSString defaultCStringEncoding]];
    [m_bindings setObject:nsPassword forKey:@"password"];
}

std::string IOSUserMemory::RetrieveUsername() {
    NSString *username = [m_bindings objectForKey:@"username"];
    
    std::string stdUsername;
    if (username != nil) {
        stdUsername = [username UTF8String];
    }
    
    return stdUsername;
}

std::string IOSUserMemory::RetrievePassword() {
    NSString *password = [m_bindings objectForKey:@"password"];
    
    std::string stdPassword;
    if (password != nil) {
        stdPassword = [password UTF8String];
    }
    
    return stdPassword;
}
