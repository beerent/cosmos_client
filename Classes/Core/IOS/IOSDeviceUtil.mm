#include "IOSDeviceUtil.h"
#import <StoreKit/StoreKit.h>

void IOSDeviceUtil::RateAppRequest() {
    [SKStoreReviewController requestReview];
}
