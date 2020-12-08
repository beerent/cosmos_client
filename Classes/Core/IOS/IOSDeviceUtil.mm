#include "IOSDeviceUtil.h"
#import <StoreKit/StoreKit.h>

void IOSDeviceUtil::RateAppRequest() {
    [SKStoreReviewController requestReview];
}

std::string IOSDeviceUtil::GetBuildVersion() {
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    return std::string([version UTF8String]);

}
