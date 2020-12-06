#include "IOSRateAppController.h"
#import <StoreKit/StoreKit.h>

void IOSRateAppController::RateAppRequest() {
    [SKStoreReviewController requestReview];
}

