#include "IOSDeviceUtil.h"
#import <StoreKit/StoreKit.h>

namespace {
    bool isChromeInstalled() {
        return [[UIApplication sharedApplication] canOpenURL: [NSURL URLWithString:@"googlechrome://"]];
    }

    void openChrome(NSURL *nsUrl) {
        NSString *scheme = nsUrl.scheme;

        // Replace the URL Scheme with the Chrome equivalent.
        NSString *chromeScheme = nil;
        if ([scheme isEqualToString:@"http"]) {
          chromeScheme = @"googlechrome";
        } else if ([scheme isEqualToString:@"https"]) {
          chromeScheme = @"googlechromes";
        }

        // Proceed only if a valid Google Chrome URI Scheme is available.
        if (chromeScheme) {
          NSString *absoluteString = [nsUrl absoluteString];
          NSRange rangeForScheme = [absoluteString rangeOfString:@":"];
          NSString *urlNoScheme =
              [absoluteString substringFromIndex:rangeForScheme.location];
          NSString *chromeURLString =
              [chromeScheme stringByAppendingString:urlNoScheme];
          NSURL *chromeURL = [NSURL URLWithString:chromeURLString];

          // Open the URL with Chrome.
          [[UIApplication sharedApplication] openURL:chromeURL];
        }
    }
}

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

void IOSDeviceUtil::OpenWebsite(const std::string& url) {
    NSString *nsUrlString =[NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
    NSURL *nsUrl = [NSURL URLWithString:nsUrlString];
    
    if (isChromeInstalled()) {
        openChrome(nsUrl);
    } else {
        [[UIApplication sharedApplication] openURL:nsUrl];
    }
}
