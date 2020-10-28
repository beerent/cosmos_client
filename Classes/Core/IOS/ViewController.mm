#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    
    [super viewDidLoad];
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    //have to reverse the bounds - probably similar reasoning to why we're reversing things in
    //renderer.cpp but I am not looking into that today.
    screenBounds = CGRectMake(0, 0, screenBounds.size.height, screenBounds.size.width);
    
    m_view = [[GLView alloc] initWithFrame: screenBounds];
    m_view.contentMode = UIViewContentModeScaleToFill;
    self.view = m_view;

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
