#include "IOSRestConnector.h"

std::string IOSRestConnector::SendRequest(const std::string& request, IRestReceiver* restReceiver) {
    
     //NSString* ipAddress = @"http://192.168.1.240:8081";
    //NSString* ipAddress = @"http://127.0.0.1:8081";
    NSString* ipAddress = @"https://knowyourcosmos.com:13213";
    NSString* formattedRequest = [NSString stringWithUTF8String:request.c_str()];
    NSString* fullRequestUrl = [NSString stringWithFormat:@"%@/%@", ipAddress, formattedRequest];
    NSURL *url = [NSURL URLWithString:fullRequestUrl];
    
    std::string requestId = std::to_string(std::intptr_t(restReceiver)) + request;
    AddRequest(requestId);
    
    NSURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:url];
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    [NSURLConnection sendAsynchronousRequest:urlRequest queue:queue completionHandler:^(NSURLResponse *response, NSData *data, NSError *error) {
        if (error) {
        } else {
            NSString* myString;
            myString = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
            std::string bar = std::string([myString UTF8String]);
            
            if (ContainsRequest(requestId)) {
                restReceiver->RestReceived(bar);
                CloseRequest(requestId);
            }
        }
    }];
    
    return requestId;
}
