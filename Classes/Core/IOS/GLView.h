#import "IOjerAppEngine2d.hpp"
#import <QuartzCore/QuartzCore.h>
#include <IOSRestConnector.h>
#include <IOSKeyboardActivator.h>
#include <IOSUserMemory.h>

@interface kbv : UIView <UIKeyInput>

- (void)insertText:(NSString *)text;
- (void)deleteBackward;
- (BOOL)hasText;
- (BOOL)canBecomeFirstResponder;

- (void) OnCharacterPressed: (char) c;
- (void) OnDeletePressed;

@end

@interface GLView : kbv {
@private
    EAGLContext* m_context;
    IOjerAppEngine2d* m_ojerEngine2d;
    IOSRestConnector m_restConnector;
    IOSKeyboardActivator m_keyboardActivator;
    IOSUserMemory m_userMemory;
    
    float m_timestamp;
}

- (void) drawView: (CADisplayLink*) displayLink;
- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event;
- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event;
- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event;

- (void) OnCharacterPressed: (char) c;
- (void) OnEnterPressed;
- (void) OnDeletePressed;

@end
