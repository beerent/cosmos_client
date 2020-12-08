#import "GLView.h"
#import <OpenGLES/ES2/gl.h> // <-- for GL_RENDERBUFFER only

@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame]) {
               float ver = [[[UIDevice currentDevice] systemVersion] floatValue];
        float scale = 1.0f;
        
        if(ver >= 3.2f)
        {
            UIScreen* mainscr = [UIScreen mainScreen];
            scale = mainscr.scale;
        }
        
        
        self.contentScaleFactor = scale;
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.contentsScale= scale;
        self.multipleTouchEnabled = YES;
        
        eaglLayer.opaque = YES;

        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        m_context = [[EAGLContext alloc] initWithAPI:api];

        if (!m_context || ![EAGLContext setCurrentContext:m_context]) {
            [self release];
            return nil;
        }

        
        NSLog(@"Using OpenGL ES 2.0");
        m_ojerEngine2d = CreateRenderer();
        m_ojerEngine2d->SetRestConnector(&m_restConnector);
        m_keyboardActivator.SetView(self);
        m_ojerEngine2d->GetKeyboardManager()->RegisterKeyboardActivator(&m_keyboardActivator);
        m_ojerEngine2d->SetUserMemory(&m_userMemory);
        m_ojerEngine2d->SetDeviceUtil(&m_deviceUtil);
        
        [m_context
            renderbufferStorage:GL_RENDERBUFFER
            fromDrawable: eaglLayer];
        
        m_ojerEngine2d->Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame), CreateResourceLoader(), scale);
        
        [self drawView: nil];
        m_timestamp = CACurrentMediaTime();

        CADisplayLink* displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self
                                     selector:@selector(drawView:)];
        
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop]
                     forMode:NSDefaultRunLoopMode];
        
        m_ojerEngine2d->OnRotate((DeviceOrientation) UIDeviceOrientationPortrait);
        [self drawView: nil];
    }
    return self;
}

- (void) drawView: (CADisplayLink*) displayLink
{
    if (displayLink != nil) {
        float elapsedSeconds = displayLink.timestamp - m_timestamp;
        m_timestamp = displayLink.timestamp;
        m_ojerEngine2d->OnDeltaTime(elapsedSeconds);
    }
    
    //this is a hack to delay rendering until the UI had time to initialize.
    if ([self ShouldRender]) {
        m_ojerEngine2d->Render();
        [m_context presentRenderbuffer:GL_RENDERBUFFER];
    }
}

//this is a hack to delay rendering until the UI had time to initialize.
- (bool) ShouldRender {
    if (renderDelayCount <= 5) {
        renderDelayCount++;
    }
    
    return renderDelayCount > 5;
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self];
    m_ojerEngine2d->OnFingerDown(ivec2(location.x, location.y));
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self];
    CGPoint previous  = [touch previousLocationInView: self];

    m_ojerEngine2d->OnFingerUp(ivec2(previous.x, previous.y),
                               ivec2(location.x, location.y));
}

- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint previous  = [touch previousLocationInView: self];
    CGPoint current = [touch locationInView: self];
    m_ojerEngine2d->OnFingerMove(ivec2(previous.x, previous.y),
                                      ivec2(current.x, current.y));
}

- (void) OnCharacterPressed: (char) c {
    m_ojerEngine2d->GetKeyboardManager()->OnCharacterPressed(c);
}

- (void) OnEnterPressed {
    m_ojerEngine2d->GetKeyboardManager()->OnEnterPressed();
}

- (void) OnDeletePressed {
    m_ojerEngine2d->GetKeyboardManager()->OnDeletePressed();
}

@end

@implementation kbv

- (void)insertText:(NSString *)text {
    char value = std::string([text UTF8String])[0];
    
    if (value == '\n') {
        [self OnEnterPressed];
    } else {
        [self OnCharacterPressed: value];
    }
}

- (void)deleteBackward {
    [self OnDeletePressed];
}

- (BOOL)hasText {
    return YES;
}

- (BOOL)canBecomeFirstResponder {
    return YES;
}

- (void) OnCharacterPressed: (char) c {
    //do nothing! override me!
}

- (void) OnEnterPressed {
    //do nothing! override me!
}

- (void) OnDeletePressed {
    //do nothing! override me!
}

@end
