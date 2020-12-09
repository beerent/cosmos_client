#include "IOSDeviceUtil.h"
#import <StoreKit/StoreKit.h>

IOSDeviceUtil::IOSDeviceUtil() {
    m_bindings = [PDKeychainBindings sharedKeychainBindings];
}

void IOSDeviceUtil::RateAppRequest() {
    [SKStoreReviewController requestReview];
}

std::string IOSDeviceUtil::GetBuildVersion() {
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    return std::string([version UTF8String]);
}

void IOSDeviceUtil::WriteToDeviceStorage(const std::string& key, const std::string& value) {
    NSString *nsKey =[NSString stringWithCString:key.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *nsValue = [NSString stringWithCString:value.c_str() encoding:[NSString defaultCStringEncoding]];
    
    [m_bindings setObject:nsValue forKey:nsKey];
}

std::string IOSDeviceUtil::ReadFromDeviceStorage(const std::string& key) {
    NSString *nsKey =[NSString stringWithCString:key.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *nsValue = [m_bindings objectForKey:nsKey];
    
    std::string stdValue;
    if (nsValue != nil) {
        stdValue = [nsValue UTF8String];
    }
    
    return stdValue;
}
